/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008 QNX Software Systems.  
 *  
 * Licensed under the Apache License, Version 2.0 (the "License"). You  
 * may not reproduce, modify or distribute this software except in  
 * compliance with the License. You may obtain a copy of the License  
 * at: http://www.apache.org/licenses/LICENSE-2.0  
 *  
 * Unless required by applicable law or agreed to in writing, software  
 * distributed under the License is distributed on an "AS IS" basis,  
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied. 
 * 
 * This file may contain contributions from others, either as  
 * contributors under the License or as licensors under other terms.   
 * Please review this entire file for other proprietary rights or license  
 * notices, as well as the QNX Development Suite License Guide at  
 * http://licensing.qnx.com/license-guide/ for other information. 
 * $ 
 */




/* 
 * absolute.c
 *
 * Filter module for absolute bus lines. 
 *
 */
#include <sys/devi.h>
#include <string.h>

#define FLAG_INIT	                0x0001
#define FLAG_FILTER               0x0002

#define BUTTON_FILTER             0x0010
#define NOISE_FILTER              0x0020

#define FLAG_XSWAP                0x0100
#define FLAG_YSWAP                0x0200
#define FLAG_BSWAP                0x0400
#define FLAG_NOXFORM              0x0800
#define FLAG_THREE_PT_CALIB       0x1000
#define FLAG_SOFT_RELEASE         0x4000

#define DEFAULT_MAX_DIFFERENCE    24

#define RELEASE_DELAY             100000000

/* Touch screen origin (0,0) */
#define UPPER_LEFT	              0
#define LOWER_LEFT	              1
#define LOWER_RIGHT	              2
#define UPPER_RIGHT	              3

#define CALIB_DIR                 "/etc/system/config/calib.%s"

static int abs_init (input_module_t * module);
static int abs_reset (input_module_t * module);
static int abs_devctrl (input_module_t * module, int event, void *ptr);
static int abs_input (input_module_t * module, int num, void *ptr);
static int abs_pulse (message_context_t *, int, unsigned, void *);
static int abs_parm (input_module_t * module, int opt, char *optarg);
static int abs_shutdown (input_module_t * module, int delay);

input_module_t absolute_filter = {
	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_ABS | DEVI_MODULE_TYPE_FILTER,
	"Abs",
	__DATE__,
	"a:bcxyS:f:o:s:X:Y:Fd:O:",
	NULL,
	abs_init,
	abs_reset,
	abs_input,
	NULL,
	abs_pulse,
	abs_parm,
	abs_devctrl,
	abs_shutdown,
};


typedef struct _point
{
	uint64_t x, y;
}
point_t;


struct data
{
	int flags;
	struct devctl_disparea_res res;
	int xyswap;
	long rxl, rxh, ryl, ryh, rzl, rzh;
	long sxl, sxh, syl, syh, szl, szh;
	long rdx, rdy;
	long sdx, sdy;
	int num_coords;
	int num_pressures;
	int gain;
	int calib;
	int count;
	char fn[256];
	int translate;
	struct _three_point_calib_matrix matrix;
	point_t display[5];						/* Array of the actual display points on the screen */
	int xoffset, yoffset;
	int max_delta;           /* Max difference in positional data, helps weed out noise */
	unsigned lastx, lasty, lastz;
	timer_t         timerid;
	struct itimerspec itime;

	int pulse_code; /* Soft Release pulse code */
	unsigned long release_delay;

	/* This is used for Calibration Free calculations */ 	
	int touch_origin;     /* UL = 0, LL = 1, LR = 2, UR = 3 */
	uint64_t pts_per_pixel_x;
	uint64_t pts_per_pixel_y;
	struct devctl_coord_range matrix_range;
};

static void set_trans_range (struct data *dp);
static int init_transform (struct data *dp);
static int transform (struct data *dp, long *x, long *y);
static int filter (struct data *dp, struct packet_abs filter_array[], struct packet_abs *filtered_point);
int calibration_free_transformation (struct data *dp, long *x, long *y);


static int abs_init (input_module_t * module)
{
	struct data *dp = module->data;
	char hostname[64];
	char *absf;

	if (!module->data) {
		if (!(dp = module->data = scalloc (sizeof *dp))) {
			return (-1);
		}
		dp->calib = 0;
		dp->flags = ABSOLUTE | FLAG_THREE_PT_CALIB;
		dp->sxl = 0;
		dp->sxh = 639;
		dp->syl = 0;
		dp->syh = 479;
		dp->gain = 1;
		dp->translate = 1;					/* always translate */
		dp->xoffset = 0;
		dp->yoffset = 0;
		dp->max_delta = DEFAULT_MAX_DIFFERENCE;
		dp->pulse_code = DEVI_PULSE_ALLOC;
		dp->release_delay = RELEASE_DELAY;
		dp->touch_origin = 0;
		dp->pts_per_pixel_x = 0;
		dp->pts_per_pixel_y = 0;

		/* get the calibration filename */

		/* env var ABSF overrides default */

		if ((absf = getenv ("ABSF")) != NULL)
			strcpy (dp->fn, absf);

		else {
			if (gethostname (hostname, sizeof (hostname)) < 0) {
				char *pMsgTxt = "Error: %s - unable to get hostname (%s)\n";

				fprintf (stderr, pMsgTxt, module->name, strerror (errno));
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 module->name, strerror (errno));
				return EXIT_FAILURE;
			}
			sprintf (dp->fn, CALIB_DIR, hostname);
		}

		if (verbosity >= 4) {
			if (!dp->fn)
				printf ("Unable to determine calibration filename\n");
			else
				printf ("Calibration filename: %s\n", dp->fn);
		}

	}

	return (0);
}

// return 1 -- translated, 0 - raw mode
int abs_get_mode( input_module_t *module ) 
{
	struct data *dp = (struct data*)module->data;
	return dp->translate ? 1 : 0;
}


static int abs_reset (input_module_t * module)
{
	struct data *dp = module->data;
	input_module_t *down = module->down;

	if ((dp->flags & FLAG_INIT) == 0) {

		struct devctl_coord_range range;
		unsigned short flags;

		dp->flags |= FLAG_INIT;

		if (dp->flags & FLAG_SOFT_RELEASE) {
			/* Create touch release timer */
			dp->timerid = devi_register_timer (module, 15, &dp->pulse_code, NULL);
		}

		/* 
		 * Since z transforms are not supported we simply retrieve the
		 * z range from the layer below.  One day absolutef might 
		 * support absolute 3D coordinate pointing devices; 
		 * hence the distinction
		 */

		(down->devctrl) (down, DEVCTL_GETDEVFLAGS, &flags);
		(down->devctrl) (down, DEVCTL_GETPTRPRESS, &dp->num_pressures);
		(down->devctrl) (down, DEVCTL_GETPTRCOORD, &dp->num_coords);

		/* Grab the touchscreen matrix and screen sizes */
		(down->devctrl) (down, DEVCTL_GETCOORDRNG, &dp->matrix_range);

		/* Check to see if we actually got something, otherwise default to 1024x768 */
		if (!dp->matrix_range.max) 
			dp->matrix_range.max = 1023;

		if (flags & ABS_PRESS_DATA) {
			if (dp->num_pressures > 1) {
				char *pMsgTxt =
					"Error: %s - unsupported input device - multiple pressures\n";
				fprintf (stderr, pMsgTxt, module->name);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 module->name);
				return (-1);
			}
			(down->devctrl) (down, DEVCTL_GETPRESSRNG, &range);
		}

		else if (flags & PTR_Z_DATA) {
			if (dp->num_coords > 2) {
				char *pMsgTxt = "Error: %s - unsupported input device\n";

				fprintf (stderr, pMsgTxt, module->name);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
							 module->name);
				return (-1);
			}
			/* future: */
			/*  (in->devctrl)(in, DEVCTL_GETCOORDRNG, &range); */
		}

		dp->szl = range.min;
		dp->szh = range.max;
	}

	dp->calib = init_transform (dp);

	/* signal our input layer so that it knows we are installed */
	(down->devctrl) (down, DEVCTL_CHECKPOINT, NULL);

	return (0);
}


static int abs_devctrl (input_module_t * module, int event, void *ptr)
{
	struct data *dp = module->data;
	input_module_t *down = module->down;

	switch (event) {

	case DEVCTL_RESET:{

			struct devctl_disparea_res *scrn = ptr;

			if (scrn) {
				dp->sxl = scrn->xl;
				dp->sxh = scrn->xh;
				dp->syl = scrn->yl;
				dp->syh = scrn->yh;
			}

			abs_reset (module);

			break;
		}

	case DEVCTL_RESCHG:{

			struct devctl_disparea_res *scrn = ptr;

			dp->sxl = scrn->xl;
			dp->sxh = scrn->xh;
			dp->syl = scrn->yl;
			dp->syh = scrn->yh;

			set_trans_range (dp);

			break;
		}

	case DEVCTL_ABS_MODE:{

			struct devctl_abs_mode *abs_mode = ptr;

			if (abs_mode->mode == ABS_XLATE_MODE) {
				dp->translate = 1;

				if (verbosity)
					printf ("DEVCTL_ABS_MODE: xlate\n");

			} else {
				dp->translate = 0;

				if (verbosity)
					printf ("DEVCTL_ABS_MODE: raw\n");
			}
			break;
		}

	case DEVCTL_COORDCHG:{

			struct devctl_devcoord *device = ptr;

			dp->xyswap = device->swap;
			dp->rxl = device->xl;
			dp->rxh = device->xh;
			dp->ryl = device->yl;
			dp->ryh = device->yh;
			
			dp->flags &= ~FLAG_THREE_PT_CALIB;

			dp->calib = 1;
			set_trans_range (dp);

			break;
		}

	case DEVCTL_GETDEVFLAGS:

		/* get the flags from the layers below */
		(down->devctrl) (down, DEVCTL_GETDEVFLAGS, ptr);
		/* ... and add our info to it */
		*(unsigned long *) ptr |= ABSOLUTE;
		break;

	case DEVCTL_GETPTRBTNS:

		(down->devctrl) (down, DEVCTL_GETPTRBTNS, ptr);
		break;

	case DEVCTL_GETPTRCOORD:

		/* we've already determined number of coordinates at init */
		memcpy (ptr, &dp->num_coords, sizeof (dp->num_coords));
		break;

	case DEVCTL_GETPTRPRESS:

		/* we've already determined number of pressures at init */
		memcpy (ptr, &dp->num_pressures, sizeof (dp->num_pressures));
		break;

	case DEVCTL_GETCOORDRNG:

		if (dp->calib) {
			struct devctl_coord_range *range = ptr;

			/* if we're scaling the coordinate then we 
			   determine range */
			switch (range->which) {

			case 0:									/* X range being queried */
				range->min = dp->sxl;
				range->max = dp->sxh;
				break;
			case 1:									/* Y range being queried */
				range->min = dp->syl;
				range->max = dp->syh;
				break;
			case 2:									/* Z range being queried */
				range->min = dp->szl;
				range->max = dp->szh;
				break;
			}
		} else {
			(down->devctrl) (down, DEVCTL_GETCOORDRNG, ptr);
		}
		break;

	case DEVCTL_GETPRESSRNG:

		/* we currently don't scale pressure */
		(down->devctrl) (down, DEVCTL_GETPRESSRNG, ptr);
		break;

	case DEVCTL_SET_CALIB_MATRIX:{
			struct _three_point_calib_matrix *matrix = ptr;

			if (verbosity > 3)
				fprintf (stderr, "Updated calibration points have been applied\n");

			dp->matrix.An = matrix->An;
			dp->matrix.Bn = matrix->Bn;
			dp->matrix.Cn = matrix->Cn;
			dp->matrix.Dn = matrix->Dn;
			dp->matrix.En = matrix->En;
			dp->matrix.Fn = matrix->Fn;
			dp->matrix.Divider = matrix->Divider;

			dp->flags |= FLAG_THREE_PT_CALIB;
			dp->calib = 1;
			break;
		}

	case DEVCTL_SET_ABS_STATE:{
			(down->devctrl) (down, DEVCTL_SET_STATE, ptr);
			break;
		}
	}

	return (0);
}


static int abs_input (input_module_t * module, int num, void *ptr)
{
	struct data *dp = module->data;
	struct packet_abs *abuf = (struct packet_abs *) ptr;
	int i;
	static int lx, ly, lz, lb;
	static int filter_count = 0;
	static struct packet_abs filter_array[3];

	if (dp->flags & FLAG_SOFT_RELEASE) {
		/* Stop timer */
		dp->itime.it_value.tv_sec = 0;
		dp->itime.it_value.tv_nsec = 0;
		dp->itime.it_interval.tv_sec = 0;
		dp->itime.it_interval.tv_nsec = 0;

		/* Set touch release timer */
		timer_settime(dp->timerid, 0, &dp->itime, NULL);
	}

	for (i = 0; i < num; i++, abuf++) {
		/* Populate the filter array, we are not doing anything with this yet */
		if ((dp->flags & FLAG_FILTER) && (abuf->buttons)) {
			/* Fill the array */
			filter_array[filter_count] = *abuf;

			if (filter_count == 3) { 
				if (filter (dp, filter_array, abuf) == -1) {
					filter_count = 0;
					return (0);
				}
			} else {
				filter_count++;
				return (0);
			}
		}

		filter_count = 0;

		if (dp->flags & FLAG_SOFT_RELEASE) {
			abuf->buttons = _POINTER_BUTTON_LEFT; 
		}

		if (dp->translate) {
			struct packet_abs tmp;

			tmp = *abuf;
			tmp.flags |= ABSOLUTE;
			tmp.flags &= ~ABS_UNCALIBRATED;

			/* If we have a calibration file lets use it, otherwise lets try to use the calculated mode */
			if (!(dp->flags & FLAG_NOXFORM)) {
				if (dp->calib) {
					transform (dp, (long *) &tmp.x, (long *) &tmp.y);
				} else {
					calibration_free_transformation (dp, (long *) &tmp.x, (long *) &tmp.y);
				}
			}

			if (tmp.x != lx || tmp.y != ly || tmp.z != lz || tmp.buttons != lb) {
				lx = tmp.x;
				ly = tmp.y;
				lz = tmp.z;
				lb = tmp.buttons;

				if (dp->flags & FLAG_BSWAP) {
					switch (tmp.buttons & 0x00000005) {

					case 1L:
						tmp.buttons &= ~1L;
						tmp.buttons |= 4L;
						break;

					case 4L:
						tmp.buttons &= ~4L;
						tmp.buttons |= 1L;
						break;
					}
				}

				if (dp->flags & FLAG_XSWAP) {
					tmp.x = (int) dp->sxh - tmp.x;
				}

				if (dp->flags & FLAG_YSWAP) {
					tmp.y = (int) dp->syh - tmp.y;
				}

				/* Add any additional offsets */
				tmp.x += dp->xoffset;
				tmp.y += dp->yoffset;

				dp->lastx = tmp.x;
				dp->lasty = tmp.y;
				dp->lastz = tmp.z;

				/* and send them on their way */
				devi_enqueue_packet (module, (char *) &tmp, sizeof (tmp));
			}
		} else {
			if (verbosity >= 3)
				printf ("Raw mode : ");

			abuf->flags |= ABSOLUTE;
			if (abuf->x != lx || abuf->y != ly || abuf->z != lz
					|| abuf->buttons != lb) {

				lx = abuf->x;
				ly = abuf->y;
				lz = abuf->z;
				lb = abuf->buttons;

				/* Add any additional offsets */
				abuf->x += dp->xoffset;
				abuf->y += dp->yoffset;

				if (verbosity >= 3)
					printf ("(%d, %d)\n", lx, ly);

				if (dp->flags & FLAG_SOFT_RELEASE) {
					dp->lastx = abuf->x;
					dp->lasty = abuf->y;
					dp->lastz = abuf->z;
				}

				/* and send them on their way */
				devi_enqueue_packet (module, (char *) abuf, sizeof (*abuf));
			}
		}
	}

	if (dp->flags & FLAG_SOFT_RELEASE) {
		/* (Re)Start timer */
		dp->itime.it_value.tv_sec = 0;
		dp->itime.it_value.tv_nsec = dp->release_delay;
		dp->itime.it_interval.tv_sec = 0;
		dp->itime.it_interval.tv_nsec = 0;

		/* Set touch release timer */
		timer_settime(dp->timerid, 0, &dp->itime, NULL);
	}

	return (0);
}


/*=============================================================================

  Name:         abs_pulse

  Description:  This is the timer pulse handler that gets called when the timer
                has expired.  This simply injects a release event into the abs
                module.

=============================================================================*/

static int abs_pulse (message_context_t *ctp, int code, unsigned flags, void *data)
{
	input_module_t    *module = (input_module_t *) data;
	struct data       *dp = module->data;
	struct packet_abs touch_data;

	touch_data.x = dp->lastx;
	touch_data.y = dp->lasty;
	touch_data.z = dp->lastz;
	touch_data.buttons = 0;

	if (verbosity >= 3)
		fprintf(stderr, "X:%d Y:%d Soft release injected.\n", touch_data.x, touch_data.y);

	/* and send them on their way */
	devi_enqueue_packet (module, (char *) &touch_data, sizeof (touch_data));

	return (0);
}


static int abs_parm (input_module_t * module, int opt, char *optarg)
{
	struct data *dp = module->data;
	char *p;
	char *pMsgTxt =
		"Error: %s - not a valid algorithm type for absolute module(%s).  Valid algorithms 3,4,5.  Defaulting to 4 point.\n";

	switch (opt) {
	case 'a':
		switch (strtol (optarg, 0, 0)) {
		case 3:
		case 4:
			{
				const char *msg = "-a3 and -a4 are deprecated. Auto detection is based on the contents of the calib file.";
				fprintf( stderr, "%s\n", msg );
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, "%s", msg );
			}
			break;
			
		case 5:
			break;
			
		default:

			fprintf (stderr, pMsgTxt, strerror (errno), dp->fn);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
						 strerror (errno), dp->fn);

			exit (0);
		}
		break;

	case 'b':
		dp->flags |= FLAG_BSWAP;
		break;

	case 'x':
		dp->flags ^= FLAG_XSWAP;
		break;

	case 'y':
		dp->flags ^= FLAG_YSWAP;
		break;

	case 'f':
		strcpy (dp->fn, optarg);
		break;

	case 'F':
		dp->flags |= FLAG_FILTER;
		break;
	case 'S':
		dp->release_delay = (atol (optarg)) * 1000000;   /* Convert to nsecs */
		dp->flags |= FLAG_SOFT_RELEASE;
		break;
	case 'o':
		p = optarg;

		while (p && (*p++ != ','));

		*(p - 1) = '\0';
		dp->sxl = atoi (optarg);
		dp->syl = atoi (p);
		dp->calib = init_transform (dp);
		break;

	case 's':
		p = optarg;

		while (p && (*p++ != ','));

		*(p - 1) = '\0';
		dp->sxh = atoi (optarg);
		dp->syh = atoi (p);
		dp->sxl = 0;
		dp->syl = 0;
		dp->calib = init_transform (dp);
		break;
	case 'O':
		/* Touch screen origin (UL, LL, LR, UR) */
		dp->touch_origin = atoi (optarg);
		break;
	case 'c':
		dp->flags ^= FLAG_NOXFORM;
		break;
	case 'X':
		dp->xoffset = atoi (optarg);
		break;
	case 'Y':
		dp->yoffset = atoi (optarg);
		break;
	case 'd':
		dp->max_delta = atoi (optarg);
		break;

	default:
		return (-1);
	}

	return (0);
}


static int abs_shutdown (input_module_t * module, int delay)
{
	return (0);
}

static int get_count( const char *s )
{
	int n = 0;
	char line[ strlen(s) + 1 ], *delims = ": \n", *p;
	strcpy( line, s );
	p = strtok( line, delims );
	while( p ) {
		n++;
		p = strtok( NULL, delims );
	}
	return n;
}


static int init_transform (struct data *dp)
{
	int found = 0;
	FILE *calibfile;
	static char buffer[1024];
	char *p, *delims = ":", *parse = " \n", resl[15], resh[15];

	if (verbosity >= 8)
		printf ("Attempting to open calibration file: %s\n", dp->fn);

	if ((calibfile = fopen (dp->fn, "r")) == NULL) {
		char *pMsgTxt =
			"Error: %s - cannot open calibration file for absolute filter module(%s)\n";

		fprintf (stderr, pMsgTxt, strerror (errno), dp->fn);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
					 strerror (errno), dp->fn);

	} else {
		int line_no = 0;
		sprintf (resl, "%dx%d", (int) dp->sxl, (int) dp->syl);
		sprintf (resh, "%dx%d", (int) dp->sxh, (int) dp->syh);

		while ((p = fgets (buffer, sizeof(buffer), calibfile)) != NULL) {
			int n = get_count( buffer );
			line_no ++;
			p = strtok (p, delims);
			if (p && !strncmp (p, resl, sizeof (resl))) {
				p = strtok (NULL, delims);
				if (p && !strncmp (p, resh, sizeof (resh))) {
					if( n == 7 ) {
						dp->flags &= ~FLAG_THREE_PT_CALIB;
					} else if( n == 9 ) {
						dp->flags |= FLAG_THREE_PT_CALIB;
					} else {
						fprintf( stderr, "invalid calib line #%d -- ignored\n", line_no );
						continue;
					}

					if ((p = strtok (NULL, delims)) == NULL)
						return (0);

					if ((p = strtok (p, parse)) == NULL)
						return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.An = atoll (p);
					else
						dp->rxl = atoi (p);

					if ((p = strtok (NULL, parse)) == NULL)
						return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.Bn = atoll (p);
					else
						dp->rxh = atoi (p);

					if ((p = strtok (NULL, parse)) == NULL)
						return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.Cn = atoll (p);
					else
						dp->ryl = atoi (p);

					if ((p = strtok (NULL, parse)) == NULL)
						return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.Dn = atoll (p);
					else
						dp->ryh = atoi (p);

					if ((p = strtok (NULL, parse)) == NULL)
						return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.En = atoll (p);
					else
						dp->xyswap = atoi (p);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						if ((p = strtok (NULL, parse)) == NULL)
							return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.Fn = atoll (p);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						if ((p = strtok (NULL, parse)) == NULL)
							return (0);

					if (dp->flags & FLAG_THREE_PT_CALIB)
						dp->matrix.Divider = atoll (p);

					found = 1;
				}
			}
		}
		fclose (calibfile);
	}

	if (found) {
		set_trans_range (dp);
	} else {

		uint32_t touch_matrix_size;
		uint32_t display_max_x, display_max_y;

    /* Calculate how many points per pixel there are */
    display_max_x = dp->sxh;
    display_max_y = dp->syh;
		touch_matrix_size = dp->matrix_range.max ? dp->matrix_range.max : 1023;

		/* Get the points per pixel and shift it by 16 so we can avoid floating point math */
    dp->pts_per_pixel_x = (display_max_x << 16) / touch_matrix_size;
    dp->pts_per_pixel_y = (display_max_y << 16) / touch_matrix_size;
	}

	if (verbosity >= 4) {
		printf ("%s calib info for [%dx%d]%s\n", found ? "Found" : "Did not find", (int) dp->sxh, (int) dp->syh, found ? "." : ".  Attempting to use calculated transformation.");
	}

	if (verbosity >= 4) {
		if (dp->flags & FLAG_THREE_PT_CALIB)
			fprintf (stderr, "Calibration Matrix An: %lld, Bn: %lld, Cn: %lld, Dn: %lld, En: %lld, Fn: %lld, Divider: %lld\n",
			  dp->matrix.An, dp->matrix.Bn, dp->matrix.Cn, dp->matrix.Dn, dp->matrix.En, dp->matrix.Fn, dp->matrix.Divider);

		else {
			printf ("Calibration parameters\n");
			printf ("sxl : %d sxh : %d\n", (int) dp->sxl, (int) dp->sxh);
			printf ("syl : %d syh : %d\n", (int) dp->syl, (int) dp->syh);
			printf ("rxl : %d rxh : %d\n", (int) dp->rxl, (int) dp->rxh);
			printf ("ryl : %d ryh : %d\n", (int) dp->ryl, (int) dp->ryh);
		}
	}

	return (found);
}


static void set_trans_range (struct data *dp)
{
	dp->sdx = dp->sxh - dp->sxl;
	dp->sdy = dp->syh - dp->syl;

	dp->rdx = dp->rxh - dp->rxl;
	dp->rdy = dp->ryh - dp->ryl;
}


/*=============================================================================

  Name:         calibration_free_transformation 

  Description:  This will use the known screen orientation, touch screen matrix
                size and screen size to calculate an accurate location on the
                screen to display the touch event.  This can be used for fixed
                touchscreens that are not suseptable to mis-alignment issues.
                It is still recommended that the screen is properly calibrated
                using one of the calibration utilities.

=============================================================================*/

int calibration_free_transformation (struct data *dp, long *x, long *y)
{
	/* Correct for screen orientation */
	switch (dp->touch_origin) {
		case UPPER_LEFT:
			break;
		case LOWER_LEFT:
			*y = dp->matrix_range.max - *y;
			break;
		case LOWER_RIGHT:
			*x = dp->matrix_range.max - *x;
			*y = dp->matrix_range.max - *y;
			break;
		case UPPER_RIGHT:
			*x = dp->matrix_range.max - *x;
			break;
		default:
			return (-1);
	}

	/* Shift the value back down by 16 so we can avoid using floating point math */
	*x = (*x * dp->pts_per_pixel_x) >> 16;
	*y = (*y * dp->pts_per_pixel_y) >> 16;

	return (0);
}


static int transform (struct data *dp, long *x, long *y)
{
	long tx, ty;
	
	/* 3 Point Transformation */
	if (dp->flags & FLAG_THREE_PT_CALIB) {
		tx = *x;
		ty = *y;
		
		if (dp->matrix.Divider) {
			*x =
				((dp->matrix.An * tx) + (dp->matrix.Bn * ty) +
				 dp->matrix.Cn) / dp->matrix.Divider;
			*y =
				((dp->matrix.Dn * tx) + (dp->matrix.En * ty) +
				 dp->matrix.Fn) / dp->matrix.Divider;

			return (0);
		} else
			return (-1);
	}
	
	if (dp->xyswap) {
		tx = *y;
		ty = *x;
	} else {
		tx = *x;
		ty = *y;
	}



	/* 4 Point transformation */
	/* transform X coordinate */
	if (dp->rdx == 0)
		*x = dp->sxl;
	else {
		if (dp->rdx > 0) {
			if (tx < dp->rxl)
				tx = dp->rxl;
			else if (tx > dp->rxh)
				tx = dp->rxh;
		} else {
			if (tx > dp->rxl)
				tx = dp->rxl;
			else if (tx < dp->rxh)
				tx = dp->rxh;
		}
		*x = (int) (((long) dp->sdx * (tx - dp->rxl)) / dp->rdx) + dp->sxl;
	}

	/* transform Y coordinate */
	if (dp->rdy == 0)
		*y = dp->syl;
	else {
		if (dp->rdy > 0) {
			if (ty < dp->ryl)
				ty = dp->ryl;
			else if (ty > dp->ryh)
				ty = dp->ryh;
		} else {
			if (ty > dp->ryl)
				ty = dp->ryl;
			else if (ty < dp->ryh)
				ty = dp->ryh;
		}
		*y = (int) (((long) dp->sdy * (ty - dp->ryl)) / dp->rdy) + dp->syl;
	}

	return (0);
}



unsigned noise_filter (struct data *dp, unsigned dirty_position[])
{
	unsigned filtered_position;
	int diff[3];

	diff[0] = abs (dirty_position[0] - dirty_position[1]);
	diff[1] = abs (dirty_position[1] - dirty_position[2]);
	diff[2] = abs (dirty_position[2] - dirty_position[0]);

	/* Reject any bad data points */
	if ((diff[0] > dp->max_delta) || (diff[1] > dp->max_delta) || (diff[2] > dp->max_delta)) {
		if (verbosity > 2)
			fprintf (stderr, "Absolute Filter Error: Data is invalid, max noise exceeded.\n");

		return (-1);
	}

	if (diff[0] < diff[1]) {
		if (diff[0] < diff[2]) {
			filtered_position = dirty_position[0] + dirty_position[1];
		} else {
			filtered_position = dirty_position[0] + dirty_position[2];
		}
	} else {
		if (diff[1] < diff[2]) {
			filtered_position = dirty_position[1] + dirty_position[2];
		} else {
			filtered_position = dirty_position[0] + dirty_position[2];
		}
	}

	return (filtered_position >>= 1);
}


/* Note filter requires three samples of data from the same location.  Driver
   needs to sample this three times and then emit those data points to the absolute module */
int filter (struct data *dp, struct packet_abs filter_array[], struct packet_abs *abuf)
{
	unsigned x[3];
	unsigned y[3];
	int i = 0;

	for (; i < 3; i++) {
		x[i] = filter_array[i].x;
		y[i] = filter_array[i].y;
	}

	abuf->x = noise_filter (dp, x);
	abuf->y = noise_filter (dp, y);

	if ((abuf->x == -1) || (abuf->y == -1))
		return (-1);

	return (0);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/absolute.c $ $Rev: 657836 $" );
