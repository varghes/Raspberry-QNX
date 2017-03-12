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
 * photon.c
 *
 * This file contains the code for the photon interface to the devi
 * drivers.  The photon interface consists of its own thread, which
 * we'll call the photon thread.  When the thread starts up it registers
 * with photon, sets up input device regions and goes to sleep waiting
 * for event bus lines to queue it data.  Data coming up from filter modules 
 * is put on a queue called the PhEventBuffer and the photon thread is
 * woken up.  Once it wakes up it processes the data and injects raw events
 * into photon event space.  Keyboard packets are injected into photon
 * event space as RawKeyboardEvents and absolute and relative packets are 
 * injected into photon event space as RawPointerEvents.
 *
 */

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/devi.h>
#include <gf/gf.h>
#include "photon.h"

static input_region_data_t Regdat;
static pthread_key_t InjectorSpecificKey;
static pthread_t IpInjector;
static pid_t IpPid;
static unsigned IpThrottle;
static buffer_t *PhEventBuffer;

static int signal_table[] = {

	SIGHUP,
	SIGQUIT,
	SIGILL,
	SIGABRT,
	SIGTERM,
	SIG_INPUT_EVENT,
	SIG_PH_EVENT,
	0
};

static struct _Ph_ctrl *ph;


/* start_photon_interface
 * 
 * Create the photon thread and event buffer.
 * 
 * Returns  0 on success
 *         -1 on error
 */

int
start_photon_interface (uint32_t reg_flags)
{
	pthread_attr_t attr;

	Regdat.flags = reg_flags;

	IpCurrentFd = -1;

	if (verbosity) {
		char *pMsgTxt = "Input: starting Photon interface\n";
		char *pAltMsgTxt = "Input: starting non Photon interface\n";

		fprintf (stderr, (OptFlags & OPT_NO_PHOTON) ? pAltMsgTxt : pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_NOTICE, (OptFlags & OPT_NO_PHOTON) ? pAltMsgTxt : pMsgTxt);
	}

	if ((PhEventBuffer = buff_create (0, sizeof (input_data_t))) == NULL) {

		char *pMsgTxt = "Error: Photon buffer creation failed\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (-1);
	}

	IpPid = getpid ();

	pthread_key_create (&InjectorSpecificKey, injector_specific_destroy);
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

	pthread_create (&IpInjector, &attr, ph_thread, &Regdat);

	return (0);
}


/* ph_thread
 *
 * This is the photon thread.  It waits for a signal telling it to either
 * process a system/service event from Photon or to process its event
 * queue 
 *
 * TODO: There is some timer stuff in here, legacy from qnx4.  It either has
 *       to be fixed or removed...
 */

void *
ph_thread (void *arg)
{
	input_region_data_t *Regdat = (input_region_data_t *) arg;
	struct sigevent timer_spec;
	timer_t tmrid;
	buffer_t *bptr = PhEventBuffer;
	input_data_t interface_pkt;
	inject_event_t *event;
	sigset_t valid_signals;
	int *signalp = signal_table;
	siginfo_t info;
	struct sched_param param;
	input_module_t *module;	

	sched_getparam (0, &param);
	param.sched_priority += 2;
	sched_setparam (0, &param);

	Regdat->throttle = IpThrottle;

	sigemptyset (&valid_signals);

	while (*signalp)
		sigaddset (&valid_signals, *signalp++);

	sigprocmask (SIG_BLOCK, &valid_signals, 0);
	memset (&timer_spec, '\0', sizeof (timer_spec));
	timer_spec.sigev_notify = SIGEV_SIGNAL;
	timer_spec.sigev_signo = SIG_INPUT_EVENT;

	if (timer_create (0, &timer_spec, &tmrid) == -1) {
		perror ("create timer:");
		return (NULL);
	}

	if ((event = scalloc (sizeof (*event))) == 0) {
		char *pMsgTxt = "Error: not enough memory to create event struct\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (NULL);
	}

	pthread_setspecific (InjectorSpecificKey, (void *) event);

	if (photon_connect (IpTargetDev, Regdat) < 0) {
		int display_xRes, display_yRes;

		if (!(OptFlags & OPT_NO_PHOTON)) {
			char *pMsgTxt = "Error: cannot connect to Photon device\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			input_shutdown ();
			exit (EXIT_FAILURE);
		} else if (bus_interrogate (DEVI_CLASS_ABS, &module)) {
			struct devctl_disparea_res new_res;

			if (OptFlags & OPT_DISPLAY_RES) {
				display_xRes = xRes;
				display_yRes = yRes;
			} else {
				/* Attempt to get the graphics info from io-display */
				gf_dev_t           gdev;
				gf_display_info_t  display_info;
				gf_dev_info_t      gdev_info;
				gf_display_t       display;
				int                display_index = 0, err = 0;
				const char         *device_name = GF_DEVICE_INDEX(0);
 
				if( (err=gf_dev_attach( &gdev, device_name, &gdev_info )) != GF_ERR_OK ) {
					fprintf( stderr, "gf_dev_attach(): error = %d\n", err );
//					return EXIT_FAILURE;
				}

				if( (err=gf_display_attach( &display, gdev, display_index, &display_info )) != GF_ERR_OK ) {
					fprintf( stderr, "gf_display_attach(): error = %d\n", err );
//					return EXIT_FAILURE;
				}

				/* Grab resolution */
				if (verbosity) 
					fprintf (stderr, "GF Display Resolution X:%d, Y%d\n", (display_info.xres - 1), (display_info.yres - 1));

				display_xRes = display_info.xres;
				display_yRes = display_info.yres;

				gf_display_detach (display);
			}

			new_res.xl = 0;
			new_res.yl = 0;
			new_res.xh = (display_xRes - 1);
			new_res.yh = (display_yRes - 1);

			do {
				if (verbosity >= 4) {
					fprintf (stderr, "Issuing devctrl to %s\n", module->name);
				}
				if (module->devctrl) {
					(module->devctrl) (module, DEVCTL_RESET, &new_res);
				}
			} while (bus_interrogate (0, &module));
		}
	}

	for (;;) {
		switch (SignalWaitinfo (&valid_signals, &info)) {

		case SIG_PH_EVENT:
			photon_input_pulse (Regdat);
			break;

		case SIG_INPUT_EVENT:
			if (info.si_code == SI_TIMER) {
				if (!buff_waiting (bptr)) {

					struct itimerspec setting;

					/* dis-arm timer */
					setting.it_value.tv_sec = 0;
					setting.it_value.tv_nsec = 0;
					timer_settime (tmrid, 0, &setting, 0);
				}

				else {
					inject (bptr, &interface_pkt, Regdat);
				}

			} else {
				sigset_t pending;
				struct itimerspec cur_time;

				if (buff_waiting (bptr)) {
					if (Regdat->throttle && (timer_gettime (tmrid, &cur_time) == -1)) {

						perror ("Get time:");
						return (NULL);
					}


					/* get pending signals */
					if (Regdat->throttle)
						sigpending (&pending);

					/* if the timer is disabled *and* there are no 
					 * signals pending 
					 * on us; then do the injection thing. 
					 */
					if (Regdat->throttle == 0 ||
							(cur_time.it_value.tv_nsec == 0
							 && !sigismember (&pending, SIG_INPUT_EVENT))) {

						inject (bptr, &interface_pkt, Regdat);

						/* if there is stuff in the buff, set the timer */
						if (Regdat->throttle) {
							struct itimerspec setting;

							/* arm the timer  */
							setting.it_value.tv_sec = 0;
							setting.it_value.tv_nsec = (Regdat->throttle * 1000000);
							timer_settime (tmrid, 0, &setting, 0);
						}
					}
				}
			}
			break;
		case SIGHUP:
		case SIGQUIT:
		case SIGTERM:
		case SIGILL:
		case SIGABRT:
			input_shutdown ();
			break;
		default:
			break;
		}

	}
}


/* photon_connect
 *
 * Attach to the Photon server (or something acting like a photon server), 
 * set up input device regions and register for service events.
 * 
 * Returns  0 on success
 *         -1 on error
 */

int
photon_connect (char *device, input_region_data_t * Regdat)
{
	int i, num_devs = 0;
	PhChannelParms_t parms;
	unsigned region_flags;
	input_module_t *module;
	struct sigevent event;
	char *photon_dev;

	struct
	{
		PhRegionDataHdr_t hdr;
		struct _interact_mgr info;
	}
	region_data;

	memset (&parms, 0x00, sizeof parms);
	parms.max_q_entries = 1;

	parms.flags = Ph_SYSTEM_PROCESS;
	if (!Regdat->throttle) {
		parms.flags |= Ph_NO_HOLD;
	}

	/* open photon */
	if (!(ph = PhOpen (device, &parms))) {
		if (!(OptFlags & OPT_NO_PHOTON)) {
			errno_print ("PhOpen");
		}
		return (-1);
	}

	IpCurrentFd = ph->fd;
	memset (&event, 0x00, sizeof (event));
	_Ph_ = ph;

	event.sigev_notify = SIGEV_SIGNAL_THREAD;
	event.sigev_signo = SIG_PH_EVENT;
	PhNotify (&event);

	_PhRegisterService (_Ph_SERV_INTERACT);

	region_flags = Ph_FORCE_FRONT;

	if (bus_interrogate (DEVI_CLASS_KBD, &module)) {

		region_flags |= Ph_KBD_REGION;
		_PhRegisterService (_Ph_SERV_KEYBOARD);
		do {
			region_data.info.devices[num_devs].handle = module;
			region_data.info.devices[num_devs].type = _INTERACT_TYPE_KEY;
			region_data.info.devices[num_devs].feedback = 0;
			num_devs++;
		} while (bus_interrogate (0, &module));
	}

	if (bus_interrogate (DEVI_CLASS_REL, &module)) {

		region_flags |= Ph_PTR_REGION;
		_PhRegisterService (_Ph_SERV_POINTER);
		do {
			region_data.info.devices[num_devs].handle = module;
			region_data.info.devices[num_devs].type = _INTERACT_TYPE_POINTER;
			region_data.info.devices[num_devs].feedback = 0;
			num_devs++;
		} while (bus_interrogate (0, &module));
	}

	memset (&Regdat->region, 0x00, sizeof Regdat->region);

	if (bus_interrogate (DEVI_CLASS_ABS, &module)) {

		Regdat->region.events_sense = Ph_EV_SYSTEM;
		if (!(region_flags & Ph_PTR_REGION)) {
			region_flags |= Ph_PTR_REGION;
			_PhRegisterService (_Ph_SERV_POINTER);
		}
		do {
			region_data.info.devices[num_devs].handle = module;
			region_data.info.devices[num_devs].type = _INTERACT_TYPE_POINTER;
			region_data.info.devices[num_devs].feedback = 0;
			num_devs++;
		} while (bus_interrogate (0, &module));
	}

	Regdat->region.events_sense |= (Ph_EV_SYSTEM | Ph_EV_SERVICE);
	Regdat->region.flags = region_flags;
	i = Ph_REGION_EV_SENSE | Ph_REGION_FLAGS | Ph_REGION_RECT;

	if (device) {
		photon_dev = device;
	} else {
		if ((photon_dev = getenv ("PHOTON")) == NULL) {
			photon_dev = "/dev/photon";
		}
	}

	Regdat->region.input_group = IpGroup;

	i |= Ph_REGION_INPUT_GROUP;

	memset (&Regdat->rect, 0x0, sizeof (Regdat->rect));

	region_data.info.id =
		Regdat->rid = PhRegionOpen (i, &Regdat->region, &Regdat->rect, 0);

	if (bus_interrogate (DEVI_CLASS_ABS, &module)) {
		struct devctl_disparea_res new_res;

		if (!(Regdat->flags & INP_REG_NO_GRAFX)) {

			if (verbosity >= 4)
				printf ("Attempting to determine graphics region dimensions\n");

			if (query_grafx_region (Regdat->region.input_group, &Regdat->rect) < 0)
				return (-1);

			if (verbosity >= 4)
				printf ("Graphics region dimensions are %d:%d %d:%d\n",
								Regdat->rect.ul.x,
								Regdat->rect.ul.y, Regdat->rect.lr.x, Regdat->rect.lr.y);

			PhRegionClose (Regdat->rid);
			region_data.info.id =
				Regdat->rid = PhRegionOpen (i, &Regdat->region, &Regdat->rect, 0);

			new_res.xl = Regdat->rect.ul.x;
			new_res.yl = Regdat->rect.ul.y;
			new_res.xh = Regdat->rect.lr.x;
			new_res.yh = Regdat->rect.lr.y;


			do {
				if (verbosity >= 4) {
					printf ("Issuing devctrl to %s\n", module->name);
				}
				if (module->devctrl) {
					(module->devctrl) (module, DEVCTL_RESET, &new_res);
				}
			} while (bus_interrogate (0, &module));
		}
	}

	if (Regdat->rid == 0) {
		char *pMsgTxt = "Error: cannot open device region\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		kill (IpPid, SIGTERM);			/* kill our pid */
		return (-1);								/* no device region |-] */
	}

	Regdat->region.rid = Regdat->rid;
	Regdat->region.data_len = sizeof (region_data);
	region_data.hdr.len = sizeof (region_data.info);
	region_data.hdr.type = Ph_RDATA_INPMGRINFO;
	region_data.info.devices[num_devs].handle = NULL;
	PhRegionChange (Ph_REGION_DATA, 0, &Regdat->region, NULL, &region_data);
	return (0);
}


void
injector_specific_destroy (void *arg)
{
	inject_event_t *event = (inject_event_t *) arg;

	free (event);
}


/* raw_enqueue
 * 
 * This function is used to enqueue data to be sent out into photon
 * event space, and/or to the GFI interface.  Type refers to either
 * DEVI_CLASS_ABS, DEVI_CLASS_KBD, or DEVI_CLASS_REL.  dptr points to a
 * struct packet_abs, packet_kbd, or packet_rel.  size is the size of
 *what dptr points to.
 *
 * Returns  0 on success
 *         -1 on error
 */

int
raw_enqueue (unsigned type, char *dptr, int size)
{
	buffer_t *bptr = PhEventBuffer;
	input_data_t input_data;

	if (!dptr)
		return (-1);

	if (!PhEventBuffer)
		return (-1);

	input_data.hdr.type = type;
	input_data.hdr.flags = 0;
	input_data.hdr.reply_pid = 0;

	pthread_mutex_lock (&bptr->mutex);

	if (size > (bptr->bufsize - bptr->cnt)) {

		/* we need more space than we currently have in the ring buffer.
		 * resizing the buffer is time-consuming so double it's size
		 */
		bptr->bufsize *= 2;

		if (bptr->cnt) {

			if (bptr->tail == bptr->head) {
				bptr->tail = bptr->buff = realloc (bptr->buff, bptr->bufsize);

				bptr->head = &bptr->buff[bptr->bufsize / 2];
			} else {
				int head_offset = bptr->head - bptr->buff;
				int tail_offset = bptr->tail - bptr->buff;
				char *tmp, *newtail;

				tmp = malloc (bptr->bufsize);

				newtail = tmp + tail_offset;

				if (bptr->tail > bptr->head) {
					int bytes = &bptr->buff[bptr->bufsize / 2] - bptr->tail;

					memcpy (newtail, bptr->tail, bytes);
					memcpy (newtail + bytes, bptr->buff, head_offset);

					bptr->head = newtail + bytes + head_offset;
				} else {
					memcpy (newtail, bptr->tail, (bptr->head - bptr->tail));
					bptr->head = tmp + head_offset;
				}

				bptr->tail = newtail;
				free (bptr->buff);

				bptr->buff = tmp;
			}

		} else {

			/* this means that the first element that was ever 
			 * put on the buffer, exceeded the buffers size
			 */

			bptr->buff = realloc (bptr->buff, bptr->bufsize);
			bptr->head = bptr->tail = bptr->buff;
		}
	}

	if (size) {

		buff_append (bptr, (char *) &input_data, sizeof (input_data.hdr));
		buff_append (bptr, dptr, size);
		if (size < sizeof (input_data.u)) {
			buff_append (bptr, (char *) &input_data.u,
									 sizeof (input_data.u) - size);
		}
	} else {
		buff_append (bptr, (char *) &input_data, sizeof (input_data));
	}

	pthread_mutex_unlock (&bptr->mutex);

	/* Signal the Injector */
	for (;;) {
		if (SignalKill (ND_LOCAL_NODE, IpPid,
										IpInjector, SIG_INPUT_EVENT, 0, 0) == -1) {

			if (errno == EAGAIN) {
				continue;
			} else {
				exit (1);
			}
		}
		break;
	}

	return (0);
}


/* inject_input
 *
 * Passing NULL for idp forces a pointer event injection 
 * (as long as there is data to inject)
 *
 * returns: INJECTED if event was actually injected, NOT_INJECTED if event was
 *          added to packet
 */

static int
inject_input (input_data_t * idp, buffer_t * bptr,
							input_region_data_t * Regdat)
{
	int rc = NOT_INJECTED;
	PhRawPtrCoord_t *c;
	PhRect_t *region_area = &Regdat->rect;
	PhRegion_t *region = &Regdat->region;

	inject_event_t *event = pthread_getspecific (InjectorSpecificKey);


	/* null idp means "close off" transaction */

	if (idp == NULL) {

		/* if there are any new coords since the last injection, then inject */
		if ((NULL != event) && event->u.ptr.num_coord) {
			trig_ptr (&event->u.ptr, region->input_group, Regdat->rid);
			rc = INJECTED;
		}
		return (rc);
	}

	switch (idp->hdr.type) {

	case DEVI_CLASS_REL:{

			struct packet_rel *rbuf = &idp->u.rel;
			unsigned flags;
			unsigned msec = MSEC (rbuf->timestamp);

			flags = (rbuf->flags & PTR_Z_DATA) ?
				Ph_PTR_FLAG_Z_REL | Ph_PTR_FLAG_Z_NONE : 0;

			if (event->u.ptr.num_coord) {

				if ((rbuf->buttons == event->u.ptr.button_state) &&
						(event->u.ptr.flags == flags) &&
						(event->u.ptr.raw_flags == Ph_PTR_RAW_FLAG_XY_REL) &&
						(event->u.ptr.num_coord < MAX_COORD - 1)) {

					c = &event->u.ptr.coord[event->u.ptr.num_coord++];
					c->dmsec = msec - event->millisec;
					event->millisec = msec;
					c->x = rbuf->dx;
					c->y = rbuf->dy;
					c->z = rbuf->dz;
					break;

				} else {
					trig_ptr (&event->u.ptr, region->input_group, Regdat->rid);
					rc = INJECTED;
				}
			}

			event->millisec = event->u.ptr.msec = msec;
			event->u.ptr.button_state = rbuf->buttons;
			event->u.ptr.flags = flags;
			event->u.ptr.raw_flags = Ph_PTR_RAW_FLAG_XY_REL;
			event->u.ptr.num_coord = 1;
			event->u.ptr.zero = 0;
			c = event->u.ptr.coord;
			c->dmsec = 0;
			c->x = rbuf->dx;
			c->y = rbuf->dy;
			c->z = rbuf->dz;
			if (verbosity)
				printf ("EN: x: %d, y: %d, buttons: %x\n",
								rbuf->dx, rbuf->dy, rbuf->buttons);

			break;
		}

	case DEVI_CLASS_ABS:{

			struct packet_abs *abuf = &idp->u.abs;
			unsigned msec = MSEC (abuf->timestamp);
			unsigned flags;

			if (abuf->flags & ABS_UNCALIBRATED) {
				if (event->abs_max == 0) {
					event->abs_max = min (max (region_area->lr.x, region_area->lr.y),
																ABS_MAX);
				}
				event->abs_max = max (event->abs_max, max (abuf->x, abuf->y));
			} else {
				event->abs_max = ABS_MAX;
			}

			switch (abuf->flags & PTR_TYPE_MASK) {

			case ABS_SCREEN:
			case ABS_WORLD:
				abuf->x = (abuf->x * region_area->lr.x) / event->abs_max;
				abuf->y = (abuf->y * region_area->lr.y) / event->abs_max;
			case ABSOLUTE:
				break;
			default:
				break;
			}

			flags = (abuf->flags & ABS_PRESS_DATA) ?
				Ph_PTR_FLAG_Z_PRESSURE | Ph_PTR_FLAG_Z_ABS : 0;

			if (event->u.ptr.num_coord) {

				if ((abuf->buttons == event->u.ptr.button_state) &&
						(event->u.ptr.flags == flags) &&
						(event->u.ptr.raw_flags == 0) &&
						(event->u.ptr.num_coord < MAX_COORD - 1)) {

					c = &event->u.ptr.coord[event->u.ptr.num_coord++];
					c->dmsec = msec - event->millisec;
					event->millisec = msec;
					c->x = abuf->x;
					c->y = abuf->y;
					c->z = flags ? abuf->z : 0L;
					break;
				} else {
					trig_ptr (&event->u.ptr, region->input_group, Regdat->rid);
					rc = INJECTED;
				}
			}

			event->u.ptr.button_state = abuf->buttons;
			event->millisec = event->u.ptr.msec = msec;
			event->u.ptr.flags = flags;
			event->u.ptr.raw_flags = 0;
			event->u.ptr.num_coord = 1;
			event->u.ptr.zero = 0;
			c = event->u.ptr.coord;
			c->dmsec = 0;
			c->x = abuf->x;
			c->y = abuf->y;
			c->z = flags ? abuf->z : 0L;

			break;
		}

	case DEVI_CLASS_KBD:{				/*  Keyboard packets */

			PhRawKeyEvent_t kev;
			struct packet_kbd *kbuf = &idp->u.kbd;

			memset (&kev, 0, sizeof (kev));
			kev.key_mods = kbuf->key.modifiers;
			kev.key_flags = kbuf->key.flags;	// & ~KEY_SCAN_VALID;
			kev.key_cap = kbuf->key.key_cap;
			kev.key_sym = kbuf->key.key_sym;
			kev.key_scan = kbuf->key.key_scan;

			/* catch CASB */
			if (kev.key_flags) {

				if ((kev.key_flags & Pk_KF_Cap_Valid) && kev.key_cap == Pk_BackSpace
						&& (kev.key_mods & (Pk_KM_Shift | Pk_KM_Ctrl | Pk_KM_Alt)) ==
						(Pk_KM_Shift | Pk_KM_Ctrl | Pk_KM_Alt)) {

					slay_photon (Regdat->flags);
				}
				trig_kbd (&kev, region->input_group, Regdat->rid);
				rc = INJECTED;
			}
			break;
		}
	default:
		break;
	}

	return (rc);
}


/* trig_ptr
 * 
 * Emit a raw pointer event into Photon event space.
 *
 */

extern int gfi_missed_delta_x;
extern int gfi_missed_delta_y;

void
trig_ptr (PhRawPtrEvent_t * ptr_ev, unsigned group, PhRid_t rid)
{
	PhEvent_t ev;
	PhRect_t rect;

	memset (&ev, 0x00, sizeof ev);
	memset (&rect, 0x00, sizeof rect);

	ev.type = Ph_EV_RAW;
	ev.subtype = Ph_EV_RAW_PTR;

	ev.data_len = offsetof (PhRawPtrEvent_t, coord) +
		ptr_ev->num_coord * sizeof (PhRawPtrCoord_t);

	ev.num_rects = 1;
	ev.input_group = group;
	ev.emitter.rid = rid;

	rect.ul.x = rect.ul.y = 0;
	rect.lr.x = rect.lr.y = 0;

	if (verbosity >= 4) {

		printf ("X=%d Y=%d Z=%d\n",
						((PhRawPtrCoord_t *) ((char *) ptr_ev +
																	offsetof (PhRawPtrEvent_t, coord)))->x,
						((PhRawPtrCoord_t *) ((char *) ptr_ev +
																	offsetof (PhRawPtrEvent_t, coord)))->y,
						((PhRawPtrCoord_t *) ((char *) ptr_ev +
																	offsetof (PhRawPtrEvent_t, coord)))->z);

		printf ("Emitting buttons=%08X\n", ptr_ev->button_state);
	}

	if (gfi_emit_ptr (ptr_ev) != 1) {
		/* GFI interface didn't block the event */
		if (ptr_ev->raw_flags & Ph_PTR_RAW_FLAG_XY_REL) {
			PhRawPtrCoord_t *c = ptr_ev->coord;

			c->x += gfi_missed_delta_x;
			c->y -= gfi_missed_delta_y;

			gfi_missed_delta_x = gfi_missed_delta_y = 0;
		}

		if (rid != 0 && IpCurrentFd >= 0) {
			/* We are connected to Photon */
			PhEventEmit (&ev, &rect, ptr_ev);
		}
	}

	ptr_ev->num_coord = 0;
}


/* trig_kbd
 * 
 * Emit raw keyboard events into Photon event space.
 *
 */

void
trig_kbd (PhRawKeyEvent_t * k, unsigned group, PhRid_t rid)
{
	PhEvent_t ev;
	PhRect_t rect;

	memset (&ev, 0x00, sizeof ev);
	memset (&rect, 0x00, sizeof rect);

	ev.type = Ph_EV_RAW;
	ev.subtype = Ph_EV_RAW_KEY;
	ev.data_len = sizeof *k;
	ev.input_group = group;
	ev.num_rects = 1;
	ev.emitter.rid = rid;

	rect.ul.x = rect.ul.y = 0;
	rect.lr.x = rect.lr.y = 0;

	if (verbosity >= 4) {
		printf ("Mods:  %0x\n", (unsigned int) k->key_mods);
		printf ("Flags: %0x\n", (unsigned int) k->key_flags);
		printf ("Cap:   %0x\n", (unsigned int) k->key_cap);
		printf ("Sym:   %0x\n", (unsigned int) k->key_sym);
		printf ("Scan:  %0x\n", (unsigned int) k->key_scan);
	}

	if (gfi_emit_kbd (k) != 1) {
		/* GFI interface didn't block the event */
		if (rid != 0 && IpCurrentFd >= 0) {
			/* We are connected to Photon */
			PhEventEmit (&ev, &rect, k);
			return;
		}
	}
}


void
inject (buffer_t * bptr, input_data_t * idp, input_region_data_t * Regdat)
{
	for (;;) {

		if (buff_delete (bptr, (char *) idp, sizeof (*idp)) == 0) {

			/* force injection */
			inject_input (NULL, NULL, Regdat);
			break;
		} else {
			inject_input (idp, bptr, Regdat);
		}
	}
}


/* query_grafx_region
 *
 * Used by touchscreen drivers to get the graphics region dimensions so that
 * calibration can proceed.
 *
 * Returns  0 on success
 *         -1 on error
 */

int
query_grafx_region (int group, PhRect_t * rect)
{
	int i, found = 0, itr = 0;
	struct Ph_region gregion;
	PhRid_t *rid;
	PhRegionDataHdr_t *rdata;
	PhGrafxRegionData_t *gdata;

	/* allocate memory for graphics region rid array */
	if ((rid = calloc (sizeof (*rid), MAX_QUERY_RIDS)) == NULL) {
		return (ENOMEM);
	}

	/*
	 * Wait for graphics region to appear so that we can 
	 * get its dimensions 
	 */

	while ((found = PhQueryRids (Ph_RIDQUERY_TOWARD, 0, group,
															 Ph_GRAFX_REGION,
															 0, 0, 0, rid, MAX_QUERY_RIDS)) < 1) {

		if (verbosity >= 4) {
			printf ("Waiting for a graphics region\n");
		}
		delay (500);
		if (itr++ > 15) {
			char *pMsgTxt = "Error: unable to find a graphics region\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			free (rid);
			return (-1);
		}
	}

	/* locate a real graphics region amongst the choices */
	for (i = 0; i < found; i++) {

		/* grab the region data and check to make sure its ok */
		PhRegionQuery (rid[i], &gregion, rect, NULL, 0);

		/* allocate memory for graphics region data array */
		if ((rdata = calloc (gregion.data_len, 1)) == NULL) {
			free (rid);
			return (ENOMEM);
		}

		PhRegionQuery (rid[i], &gregion, rect, rdata, gregion.data_len);

		gdata =
			(PhGrafxRegionData_t *) PhRegionDataFindType (&gregion, rdata,
																										Ph_RDATA_GFXINFO);

		if (gdata && (gdata->info.valid_fields & Ph_GFX_CAPABILITIES)) {
			if (gdata->info.capabilities & Ph_GCAP_LOCALHW) {
				if (verbosity >= 4) {
					printf ("Found graphics region owned by local hardware\n");
				}
				free (rdata);
				break;
			} else {
				char *pMsgTxt =
					"Error: found graphics region which is not local hardware\n";
				fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			}
		} else {
			char *pMsgTxt =
				"Error: found graphics region with no capabilities data\n";
			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		}
		free (rdata);
	}
	free (rid);
	return (0);
}


pid_t
photon_pid (void)
{
	int photon_fd;
	struct _server_info srvrinfo;

	PhInfo (ph, Ph_INFO_CTRL_FD, &photon_fd, sizeof (photon_fd));
	if (ConnectServerInfo (IpPid, photon_fd, &srvrinfo) != -1) {
		return (srvrinfo.pid);
	}
	return (-1);
}


void
slay_photon (unsigned flags)
{
	pid_t ph_pid;

	if (!(flags & INP_REG_DISABLE_CASB)) {

		if ((ph_pid = photon_pid ()) != -1) {

			if (kill (ph_pid, SIGTERM) == -1) {
				if (verbosity) {
					char *pMsgTxt = "Error: could not set signal on %d: %s\n";

					fprintf (stderr, pMsgTxt, ph_pid, strerror (errno));
					slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
								 ph_pid, strerror (errno));
				}
			}
		} else {
			char *pMsgTxt = "Error: unable to slay off photon\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		}
	}
}

/* devi_enqueue_packet
 *
 * Used by filter layer modules to dispatch a completed packet to
 * the proper interface, photon or resource manager.
 *
 * Returns  0 on success
 *         -1 on error
 */

int
devi_enqueue_packet (input_module_t * module, char *dptr, unsigned size)
{
	if (playFile) {
		// we are in the playback mode -- ignore all real events...
		return 0;
	}

	if (!module)
		return (-1);

	if (OptFlags & OPT_RESMGR)
		resmgr_enqueue_packet (module->line, dptr, size);

	if (logFile) {
		struct timespec t;

		clock_gettime (CLOCK_REALTIME, &t);
		// save the time stamp
		fwrite (&t, sizeof (t), 1, logFile);
		// save the data
		fwrite (&module->type, sizeof (module->type), 1, logFile);
		fwrite (&size, sizeof (size), 1, logFile);
		fwrite (dptr, size, 1, logFile);
		fflush (logFile);
	}

	raw_enqueue (module->type & DEVI_CLASS_MASK, dptr, size);

	if (logFile && (module->type & DEVI_CLASS_MASK) == DEVI_CLASS_KBD) {
		struct packet_kbd *kbuf = (struct packet_kbd *) dptr;

		if (kbuf->key.key_cap == KEYCODE_MENU) {
			extern void set_abs_pos (int, int);

			set_abs_pos (0, 0);
		}
	}
	return (0);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/photon.c $ $Rev: 657836 $" );
