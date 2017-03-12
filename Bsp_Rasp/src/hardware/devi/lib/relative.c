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
 * relative.c
 *
 * This is the filter module for all mice type devices.  It allows
 * data passed up from protocol modules to be further manipulated.
 * Data is received from the protocol module in struct packet_rel's.
 *
 * Data manipulation includes swapping button functionality and setting
 * gain and threshold.
 *
 */
#include <sys/devi.h>
#include <assert.h>
#include <stdio.h>
#include <cfgopen.h>

#define THRESHOLD_POWER		2
#define FLAG_XSWAP		0x0100
#define FLAG_YSWAP		0x0200
#define FLAG_BSWAP		0x0400
#define FLAG_RWHEEL             0x0800


#define MAX_WHEEL_ACCELERATION      (5)	/* Default max_wheel_acceleration                       */
#define MAX_MAX_WHEEL_ACCELERATION (10)	/* Maximum max_wheel_acceleration                       */
#define WHEEL_GAIN                  (1)	/* Mouse wheel acceleration parameter                   */
#define MAX_WHEEL_GAIN              (6)	/* Maximum wheel acceleration parameter                 */
#define CURSOR_SCALE                (1)	/* default "cost" of one unit                           */
#define MAX_GAIN                    (5)	/* max "cost" of one unit                               */
#define CURSOR_THRESHOLD            (5)	/* default value of cursor_threshold                    */
#define MAX_CURSOR_THRESHOLD        (250)	/* Upper limit for cursor_threshold                     */
#define CURSOR_ACCEL_STEP           (5)
#define DOUBLE_CLICK_DELAY          (250)	/* Double click delay in msec                           */

#define TRAP_FILE                   "/etc/system/trap/devi-rel-opts"

extern input_module_t keyboard_filter;
USBKCode keyCode[] = { 1, 0 };	// 1 corresponds to make (0 - break, but we send make message  only)

struct data
{
	unsigned flags;

	/* cursor parameters                           */
	int cursor_x;
	int cursor_y;
	int threshold;
	int gain;
	/* wheel parameters                            */
	int wheel_last_move;
	int wheel_accelerator;
	int max_wheel_acceleration;
	int wheel_gain;
	int zmode;										/* see comment posible values in const.h */

	unsigned int msec;

	unsigned char num_coords;
};

typedef struct
{

	int mouse_gain,								/* 1 */
	 mouse_buttons_swaped,				/* 0 - no, 1 - yes */
	 mouse_double_click_delay,		/* 250 msec */
	 mouse_wheel_enabled,					/* 0 - no, 1 - yes */
	 mouse_x_reversed,						/* 0 - no, 1 - yes */
	 mouse_y_reversed,						/* 0 - no, 1 - yes */
	 mouse_threshold,							/* 100 mickeys */
	 mouse_wheel_acceleration,		/* Accelearation parameter for wheel operating; max value - 10, default - 3 */
	 mouse_wheel_lines;						/* number of key-us (-downs) per one wheel step; default value is 1         */

}
ITF_OPTIONS_t;



static int rel_init (input_module_t * module);
static int rel_devctrl (input_module_t * module, int event, void *ptr);
static int rel_reset (input_module_t * module);
static int rel_input (input_module_t * module, int num, void *arg);
static int rel_parm (input_module_t * module, int opt, char *optarg);
static int rel_shutdown (input_module_t * module, int delay);
static void calculate_cursor_move (struct data *dp, struct packet_rel *p);

input_module_t relative_filter = {

	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_REL | DEVI_MODULE_TYPE_FILTER,
	"Rel",
	__DATE__,
	"lxyaT:G:",
	NULL,													// initialized by us (in init)
	rel_init,
	rel_reset,
	rel_input,
	NULL,													// output
	NULL,													// pulse not used (this is a pure filter)
	rel_parm,
	rel_devctrl,
	rel_shutdown
};

/* Description: function loads mouse options from trap file                         */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
load_rel_opts (input_module_t * module)
{
	struct data *dp = module->data;
	FILE *f;
	char buf[128], *key, *opt, *arg;
	char *delims = "\t \n";



	/* Setup defaults */
	dp->gain = WHEEL_GAIN;
	dp->threshold = MAX_CURSOR_THRESHOLD;
	dp->flags &= ~FLAG_BSWAP;			// No
	dp->flags |= FLAG_RWHEEL;			// default wheel
	dp->wheel_gain = 1;
	dp->max_wheel_acceleration = MAX_WHEEL_ACCELERATION;
	/* Don't use it now  
	   = DOUBLE_CLICK_DELAY; // msec 

	   mouse_x_reversed    
	   mouse_y_reversed  
	 */
	if (NULL == (f = fcfgopen (TRAP_FILE, "r", CFGFILE_NODE, NULL, NULL, 0))) {
		if (verbosity)
			fprintf (stderr, "Cannot open trap file %s (%s)\n", TRAP_FILE,
							 strerror (errno));
		return;
	}

	dp->flags &= ~FLAG_RWHEEL;		// default wheel
	/* read and parse the data */
	while (fgets (buf, sizeof (buf), f)) {

		if ('#' == buf[0])					// This is a comment
			continue;

		/* If we can't recognize the syntax, we quietly skip the line */
		if (NULL == (key = strtok (buf, delims)))
			continue;

		/* Get the option name */
		if (NULL == (opt = strtok (NULL, delims)))
			continue;

		/* Arg is optional, we keep it empty if it is not defined */
		if (NULL == (arg = strtok (NULL, delims)))
			arg = "";

		/* Currenly we recognize the 'rel' keyword only */
		if (0 == strcmp ("rel", key)) {
			if (0 == strcmp ("-G", opt))	// Cursor gain
			{
				int gain = atoi (arg);

				if ((gain > 0) && (gain <= MAX_GAIN))
					dp->gain = gain;
			} else if (0 == strcmp ("-T", opt))	// Cursor threshold
			{
				int threshold = atoi (arg);

				if (threshold == 0)			// Compatibility with previous version
					threshold = MAX_CURSOR_THRESHOLD;

				if ((threshold > 0) && (threshold <= MAX_CURSOR_THRESHOLD))
					dp->threshold = threshold;
			} else if (0 == strcmp ("-l", opt))	// Swap buttons
			{
				dp->flags |= FLAG_BSWAP;
			} else
				// Doesn't use these parameters now
			if (0 == strcmp ("-x", opt))	// x-reverse
			{
			} else
				// Doesn't use these parameters now
			if (0 == strcmp ("-y", opt))	// y-reverse
			{
			} else if (0 == strcmp ("-W", opt))	// Wheel parameters
			{
				int wheel_gain, wheel_threshold;

				dp->flags |= FLAG_RWHEEL;	// Wheel enabled
				if (NULL != arg)
					sscanf (arg, "%i,%i", &wheel_gain, &wheel_threshold);
				if ((wheel_gain > 0) && (wheel_gain <= MAX_WHEEL_GAIN))
					dp->wheel_gain = wheel_gain;
				if ((wheel_threshold > 0)
						&& (wheel_threshold <= MAX_MAX_WHEEL_ACCELERATION))
					dp->max_wheel_acceleration = wheel_threshold;
			} else
				// Doesn't use these parameters now
			if (0 == strcmp ("-D", opt))	// Double click delay
			{
			} else if (verbosity) {
				fprintf (stderr, "Warning: mouse trap file - invalid option '%s'\n",
								 opt);
			}
		}														// End of keyword analizing
	}															// End of trap file reading

	fclose (f);

}

/* Description: function save mouse options to the trap file                        */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
save_rel_opts (input_module_t * module)
{
	struct data *dp = module->data;
	FILE *f;


	if (NULL == (f = fcfgopen (TRAP_FILE, "w+", CFGFILE_NODE, NULL, NULL, 0))) {
		if (verbosity)
			fprintf (stderr, "Cannot open trap file %s (%s)\n", TRAP_FILE,
							 strerror (errno));
		return;
	}
	// Cursor parameters
	fprintf (f, "rel -G %d\n", dp->gain);
	fprintf (f, "rel -T %d\n", dp->threshold);

	/* Don't support these parameter now
	   fprintf( f, "rel -D %d\n",  DOUBLE_CLICK_DELAY ); 
	 */

	// Wheel parameters  
	if (dp->flags & FLAG_RWHEEL)
		fprintf (f, "rel -W %i,%i\n", dp->wheel_gain, dp->max_wheel_acceleration);

	// Buttons swapping  
	if (dp->flags & FLAG_BSWAP)
		fprintf (f, "rel -l\n");

	/* Don't support these parameters now
	   if( data->mouse_x_reversed )
	   fprintf( f, "rel -x\n" );

	   if( data->mouse_y_reversed )
	   fprintf( f, "rel -y\n" );
	 */

	fclose (f);


}


/* Description: Callback initialisation function; it is called when input module is */
/*              initializing the input system                                       */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Actualy this function does nothing                                  */
int
rel_init (input_module_t * module)
{
	struct data *dp = module->data;

	if (!module->data) {
		if (!(dp = module->data = scalloc (sizeof *dp)))
			return (-1);
	}
	dp->wheel_gain = WHEEL_GAIN;
	dp->max_wheel_acceleration = MAX_WHEEL_ACCELERATION;
	dp->gain = MAX_GAIN;
	dp->threshold = MAX_CURSOR_THRESHOLD;
	dp->zmode = ZMODE_KEY_UPDOWN;

	load_rel_opts (module);
	return EOK;
}

/* Description: This is a callback function for DEVCTRL command processing          */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int
rel_devctrl (input_module_t * module, int event, void *ptr)
{
	struct data *dp = module->data;
	input_module_t *down = module->down;

	switch (event) {

	case DEVCTL_GETDEVFLAGS:
		/* get the flags from the layers below */
		return (down->devctrl) (down, DEVCTL_GETDEVFLAGS, ptr);

	case DEVCTL_GETPTRBTNS:
		/* we're really not concerned with buttons, let 'em through */
		return (down->devctrl) (down, DEVCTL_GETPTRBTNS, ptr);

	case DEVCTL_GETWHEEL:
		return (down->devctrl) (down, DEVCTL_GETWHEEL, ptr);

	case DEVCTL_SETWHEEL:
		(down->devctrl) (down, DEVCTL_SETWHEEL, ptr);
		if (*(unsigned *) ptr & 1) {
			dp->flags |= FLAG_RWHEEL;
		} else {
			dp->flags &= ~FLAG_RWHEEL;
		}
		save_rel_opts (module);
		break;

	case DEVCTL_GETWHEEL_ACCEL:
		if (NULL != ptr) {
			struct devctl_wheel_accel *parms = (struct devctl_wheel_accel *) ptr;

			parms->step = dp->wheel_gain;
			parms->accel = dp->max_wheel_acceleration;
		}
		break;
	case DEVCTL_SETWHEEL_ACCEL:
		if (NULL != ptr) {
			struct devctl_wheel_accel *parms = (struct devctl_wheel_accel *) ptr;

			if (parms->step > 0)
				dp->wheel_gain = parms->step;
			if (parms->accel > 0 && parms->accel < MAX_WHEEL_ACCELERATION)
				dp->max_wheel_acceleration = parms->accel;
			save_rel_opts (module);
		}
		break;

	case DEVCTL_GETPTRCOORD:
		/* we've already determined number of coordinates at init */
		memcpy (ptr, &dp->num_coords, sizeof (dp->num_coords));
		break;

	case DEVCTL_GETACCEL:
		if (NULL != ptr) {
			struct devctl_rel_accel *parms = ptr;

			parms->gain = dp->gain;
			parms->threshold = dp->threshold;
		} else
			return -1;
		break;
	case DEVCTL_SETACCEL:
		if (NULL != ptr) {
			struct devctl_rel_accel *parms = ptr;

			if ((parms->gain > 0) && (parms->gain <= MAX_GAIN))
				dp->gain = parms->gain;
			if ((parms->threshold > 0)
					&& (parms->threshold <= MAX_CURSOR_THRESHOLD))
				dp->threshold = parms->threshold;
			save_rel_opts (module);
		} else
			return -1;
		break;

	case DEVCTL_GETSWAP:
		if (NULL != ptr) {
			unsigned *swap = ptr;

			if (dp->flags & FLAG_BSWAP)
				*swap |= PTR_BUT_SWAP;
			if (dp->flags & FLAG_XSWAP)
				*swap |= PTR_X_SWAP;

			if (dp->flags & FLAG_YSWAP)
				*swap |= PTR_Y_SWAP;
		} else
			return -1;
		break;

	case DEVCTL_SETSWAP:
		if (NULL != ptr) {
			unsigned *swap = ptr;

			if (*swap & PTR_BUT_SWAP)
				dp->flags |= FLAG_BSWAP;
			else
				dp->flags &= ~FLAG_BSWAP;

			if (*swap & PTR_X_SWAP)
				dp->flags |= FLAG_XSWAP;
			else
				dp->flags &= ~FLAG_XSWAP;

			if (*swap & PTR_Y_SWAP)
				dp->flags |= FLAG_YSWAP;
			else
				dp->flags &= ~FLAG_YSWAP;

			save_rel_opts (module);
		} else
			return -1;
		break;
	case DEVCTL_GETZMODE:
		if (NULL != ptr)
			*(int *) ptr = dp->zmode;
		else
			return -1;
		break;
	case DEVCTL_SETZMODE:
		if (NULL != ptr) {
			int nZmode = *(int *) ptr;

			if (nZmode != 0) {
				if (nZmode < ZMODE_KEY_UPDOWN || nZmode > ZMODE_SEND_CLIENT_ONLY)
					dp->zmode = nZmode;
				else
					return -1;
			}
		} else
			return -1;
		break;
	case DEVCTL_SET_REL_STATE:{
		(down->devctrl) (down, DEVCTL_SET_STATE, ptr);
		break;
	}
	default:
		break;
	}

	return (0);
}


/* Description: this callback funtion is called when the module is linked into the  */
/*              event bus; it is used to set initial module state on the protocol   */
/*              level                                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int
rel_reset (input_module_t * module)
{
	struct data *dp = module->data;
	input_module_t *down = module->down;
	unsigned wheel;

	dp->wheel_last_move = 0;
	dp->wheel_accelerator = 0;
	dp->cursor_x = 0;
	dp->cursor_y = 0;

	(down->devctrl) (down, DEVCTL_GETPTRCOORD, &dp->num_coords);

	if (dp->flags & FLAG_RWHEEL) {
		wheel = 1;
		(down->devctrl) (down, DEVCTL_SETWHEEL, &wheel);
	}

	return (0);
}


/* Description: main filter   processing function.  It will be called  by the       */
/*              protocol layer to pass its data to process.                         */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int num - number of packets to process                              */
/*              void * arg - data to process                                        */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
int
rel_input (input_module_t * module, int num, void *arg)
{
	struct packet_rel *p = (struct packet_rel *) arg;
	struct data *dp = module->data;
	static struct packet_rel mbuf;
	int valid = 0;
	unsigned msec;
	input_module_t *pkbd;
	int zmove = 0;

	if (verbosity >= 8)
		printf ("%s:Received %d packets from %s\n", module->name, num,
						module->down->name);




	for (; num; num--) {

		if (verbosity >= 3)
			printf ("Raw Mouse Data X: %d Y: %d Buttons: %d\n", p->dx, p->dy, p->buttons);

		msec = MSEC (p->timestamp);
		/* if time interval between two sequential samples more */
		/* than 200 msec, just reset wheel acceleration         */
		if ((msec - dp->msec) > 200) {
			dp->wheel_last_move = 0;
			dp->wheel_accelerator = 0;
			dp->cursor_x = 0;
			dp->cursor_y = 0;
		}
		dp->msec = msec;
		if (dp->flags & FLAG_RWHEEL) {
			/* Acceleration of wheel spinning                  */
			/* We do it if mouse sends quickly similar signals */
			/* with amplitude == 1                             */
			if ((dp->wheel_last_move == p->dz) && (1 == abs (p->dz))) {
				if (dp->wheel_accelerator < dp->max_wheel_acceleration)
					++(dp->wheel_accelerator);
			} else
				dp->wheel_accelerator = abs (p->dz);

			dp->wheel_last_move = p->dz;
			/* Process wheel spinning via keyboard protocol module */
			pkbd = keyboard_filter.down;
			zmove = dp->wheel_accelerator *
				((dp->wheel_accelerator > 1) ? dp->wheel_gain : 1);
			if ((dp->zmode != ZMODE_SEND_CLIENT_ONLY) &&
					(NULL != pkbd) && (NULL != pkbd->data)) {
				int zm = zmove;
				struct packet_kbd kbuf;

				kbuf.key.flags = 0;
				if (p->dz > 0) {
					if (ZMODE_KEY_UPDOWN == dp->zmode)
						kbuf.nUSBCode = KS_Down;
					else
						kbuf.nUSBCode = KS_PgDn;
				} else {
					if (ZMODE_KEY_UPDOWN == dp->zmode)
						kbuf.nUSBCode = KS_Up;
					else
						kbuf.nUSBCode = KS_PgUp;
				}
				while (zm--) {
					if (keyboard_filter.down) {
						kbuf.bMakeBreak = 1;
						keyboard_filter.input (&keyboard_filter, 1, &kbuf);
						kbuf.bMakeBreak = 0;
						keyboard_filter.input (&keyboard_filter, 1, &kbuf);
					}

				}
			}

		}
		/* Check to see if we need to apply any mouse acceleration or not */
		if ((dp->gain > 0) && (dp->threshold > 0))
			calculate_cursor_move (dp, p);

		if (dp->flags & FLAG_XSWAP)
			p->dx = -p->dx;
		if (dp->flags & FLAG_YSWAP)
			p->dy = -p->dy;
		if (p->dz < 0)
			p->dz = -zmove;
		else
			p->dz = zmove;

		if (dp->flags & FLAG_BSWAP) {
			p->buttons =
				(p->buttons & ~(_POINTER_BUTTON_LEFT | _POINTER_BUTTON_RIGHT)) |
				((p->buttons & _POINTER_BUTTON_LEFT) ? _POINTER_BUTTON_RIGHT : 0) |
				((p->buttons & _POINTER_BUTTON_RIGHT) ? _POINTER_BUTTON_LEFT : 0);
		}
		if (valid) {
			if (p->buttons == mbuf.buttons) {
				mbuf.dx += p->dx;
				mbuf.dy += p->dy;
				mbuf.dz += p->dz;
				mbuf.timestamp = p->timestamp;
				continue;
			}
			if (verbosity >= 8) {
				printf ("%s:Enqueuing packet\n", module->name);
			}
			devi_enqueue_packet (module, (char *) &mbuf, sizeof (mbuf));
		}
		valid = 1;
		mbuf = *p++;
	}

	if (valid) {
		if (verbosity >= 8) {
			printf ("%s:Enqueuing packet\n", module->name);
		}
		devi_enqueue_packet (module, (char *) &mbuf, sizeof (mbuf));
	}

	return (0);
}


/* Description: this is a callback function for command line parameter processing   */
/*              (all valid parameters for device module are listed in               */
/*              relative_filter.args)                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int opt  - parameter code                                           */
/*              char * optarg - optional parameter value                            */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
int
rel_parm (input_module_t * module, int opt, char *optarg)
{
	struct data *dp = module->data;
	int nVal;


	switch (opt) {

	case 'a':
		nVal = strtol (optarg, 0, 0);
		if (nVal <= MAX_WHEEL_ACCELERATION)
			dp->max_wheel_acceleration = nVal;
		break;
	case 'l':
		dp->flags ^= FLAG_BSWAP;
		break;

	case 'x':
		dp->flags ^= FLAG_XSWAP;
		break;

	case 'y':
		dp->flags ^= FLAG_YSWAP;
		break;

	case 'T':
		dp->threshold = atoi (optarg);
		if (dp->threshold <= 0 || dp->threshold > MAX_CURSOR_THRESHOLD) {
			dp->threshold = MAX_CURSOR_THRESHOLD;
		} else {
			dp->threshold <<= THRESHOLD_POWER;
		}
		break;

	case 'G':
		dp->gain = atoi (optarg);
		break;

	default:
		return (-1);
	}

	return (0);
}


/* Description: this is a callback function which is called when resourse manager   */
/*              is shutting down                                                    */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int ms  - program doesn't use this parameter                        */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Does nothing on the mouse filter tier                               */
int
rel_shutdown (input_module_t * module, int delay)
{
	return EOK;
}


/* Description: this is an internal service function that evaluates mouse movement  */
/*              using acceleration algorithm                                        */
/* Input      : struct data * dp - pointer to internal module data structure        */
/*              struct packet_rel *p - pointer to raw mouse data package            */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
calculate_cursor_move (struct data *dp, struct packet_rel *p)
{
	int nCoeff;
	int x = p->dx, y = p->dy;
	int x0 = x, y0 = y;

	if ((dp->cursor_x >= 0 && x >= 0) || (dp->cursor_x <= 0 && x <= 0))
		dp->cursor_x += x;
	else
		dp->cursor_x = 0;

	if ((dp->cursor_y >= 0 && y >= 0) || (dp->cursor_y <= 0 && y <= 0))
		dp->cursor_y += y;
	else
		dp->cursor_y = 0;

	if (MAX_CURSOR_THRESHOLD == dp->threshold)
		nCoeff = 0;
	else
		/* This coefficient CURSOR_ACCEL_STEP is used in order to get more smooth 
		   changing of mouse behaviour while modifying threshold parameter */
		nCoeff = min (CURSOR_ACCEL_STEP,
									(abs (dp->cursor_x) + abs (dp->cursor_y)) / dp->threshold);

	if (dp->gain > 0) {
		x = x * dp->gain;
		y = y * dp->gain;
	}

	if (nCoeff > 0) {
		y *= nCoeff;
		x *= nCoeff;
	}

	p->dy = y;
	p->dx = x;

	if (verbosity >= 4)
		fprintf (stderr,
						 "x0 = %i, y0 = %i, x(before) = %i, y(before) = %i, thresh = %i, gain = %i, x(new) = %i, y(new) = %i, coeff = %i\n",
						 x0, y0, (int) (dp->cursor_x), (int) (dp->cursor_y),
						 (int) (dp->threshold), (int) (dp->gain), (int) (p->dx),
						 (int) (p->dy), nCoeff);

}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/relative.c $ $Rev: 657836 $" );
