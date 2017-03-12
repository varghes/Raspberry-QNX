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
 * service.c
 *
 * This file contains code that allows dynamic configuration/manipulation
 * of input modules at runtime.  Currently it only supports configuration
 * by use of Photon System and Service messaging.  Later it will also
 * support devctrl via the resource manager interface.
 *
 * Examples of things that can be done dynamically are touchscreen 
 * calibration, setting mouse parameters like gain and threshold,
 * and setting the keyboard rate.
 */


#include <sys/devi.h>
#include "photon.h"

void system_event (PhEvent_t * msg, input_region_data_t * regdat);
void service_event (PhEvent_t * msg, input_region_data_t * regdat);
void service_message (struct _Ph_msg_forward *msg,
											input_region_data_t * regdat);
void filter_ctrl (short type, void *ctrl);


#define EVENT_SIZE    sizeof(PhEvent_t) + 2000

int
photon_input_pulse (input_region_data_t * regdat)
{

	static PhEvent_t *event;

	if (verbosity)
		printf ("Received a message from the Photon server.\n");

	if (event == NULL) {
		if ((event = malloc (EVENT_SIZE)) == NULL) {
			perror ("malloc");
			return -1;
		}
	}

	switch (PhEventPeek (event, EVENT_SIZE)) {

	case Ph_EVENT_MSG:

		if (event->type == Ph_EV_SYSTEM) {
			system_event (event, regdat);
		}

		else if (event->type == Ph_EV_SERVICE) {
			service_event (event, regdat);
		}

		break;

	case _Ph_SERV_INTERACT:
	case _Ph_SERV_KEYBOARD:
	case _Ph_SERV_POINTER:

		service_message ((struct _Ph_msg_forward *) event, regdat);
		break;

	case -1:
		perror ("PhEventPeek failed");
		break;
	}

	return (0);
}


void
system_event (PhEvent_t * msg, input_region_data_t * regdat)
{
	PhRect_t *rect;
	struct devctl_disparea_res new_res;

	switch (msg->subtype) {

	case Ph_SYSTEM_REGION_CHANGE:{
			PhSystemEvent_t *sys;

			sys = PhGetData (msg);
			if (sys->RegionChange.input_group == regdat->region.input_group) {

				if ((sys->RegionChange.flags & 0x00ff) == Ph_GRAFX_REGION) {

					if (sys->RegionChange.fields == -1) {
						input_module_t *module;

						if (bus_interrogate (DEVI_CLASS_ABS, &module)) {
							rect = &sys->RegionChange.rect;

							/* 
							 * This will insure we get the
							 * "real" graphics region 
							 */

							query_grafx_region (regdat->region.input_group, rect);

							new_res.xl = rect->ul.x;
							new_res.yl = rect->ul.y;
							new_res.xh = rect->lr.x;
							new_res.yh = rect->lr.y;

							do {
								(module->devctrl) (module, DEVCTL_RESCHG, &new_res);
							} while (bus_interrogate (0, &module));
						}
					}
				}
			}
			break;
		}
	default:
		break;
	}
}

void
service_event (PhEvent_t * msg, input_region_data_t * regdat)
{
	short *inputctrl_ev;

	if (verbosity >= 8)
		printf ("service_event\n");

	switch (msg->subtype) {

	case Ph_EV_POINTER_CTRL:

		if (verbosity)
			printf ("Ph_EV_POINTER_CTRL\n");

	case Ph_EV_KEYBOARD_CTRL:

		if (verbosity)
			printf ("Ph_EV_KEYBOARD_CTRL\n");

		inputctrl_ev = PhGetData (msg);

		switch (*inputctrl_ev) {

		case _CTRL_FILTER:
			filter_ctrl (msg->subtype, inputctrl_ev);
			break;

#if 0
		case _CTRL_DEVICE:
			device_ctrl (msg->subtype, inputctrl_ev);
			break;
#endif

		default:
			break;
		}
	}
}

void
service_message (struct _Ph_msg_forward *msg, input_region_data_t * regdat)
{
	input_module_t *module;
	iov_t smx[3], rmx[1];
	io_devctl_t devhdr;

	struct
	{
		struct _Ph_msg_forward fwd;
		int result;
	}
	reply;

	SETIOV (rmx, &reply, sizeof (reply));

	memset (&reply, '\0', sizeof (reply));

	devhdr.i.type = _IO_DEVCTL;
	devhdr.i.combine_len = 0;
	devhdr.i.dcmd = _Ph_REPLY;
	devhdr.i.zero = 0;
	SETIOV (&smx[0], &devhdr.i, sizeof (devhdr.i));

	reply.fwd.hdr.type = _Ph_REPLY;
	reply.fwd.pid = msg->pid;
	reply.fwd.len = sizeof (reply.result);
	reply.result = EOK;

	SETIOV (&smx[1], &reply, sizeof (reply));

	switch (msg->hdr.subtype) {

	case _INTERACTTYPE & 0xffff:{

			struct _interact_device *interact =
				(struct _interact_device *) ((char *) msg +
																		 sizeof (struct _Ph_msg_forward));

			module = interact->handle;
			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_REL:
			case DEVI_CLASS_ABS:
				interact->type = _INTERACT_TYPE_POINTER;
				break;

			case DEVI_CLASS_KBD:
				interact->type = _INTERACT_TYPE_KEY;
				break;

			default:
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*interact);
			SETIOV (&smx[2], interact, sizeof (*interact));
			break;

		}

	case _POINTERGETINFO & 0xffff:{

			unsigned long flags;

			struct _pointer_info *ctrl =
				(struct _pointer_info *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			struct devctl_coord_range range;

			module = ctrl->type.handle;
			if (!module) {
				break;
			}

			memset (ctrl, '\0', sizeof (*ctrl));
			ctrl->type.type = _INTERACT_TYPE_POINTER;

			if (module->devctrl) {
				(module->devctrl) (module, DEVCTL_GETPTRBTNS, &ctrl->buttons);
				(module->devctrl) (module, DEVCTL_GETDEVFLAGS, &flags);
			}

			ctrl->flags = 0U;

			if (flags & ABSOLUTE) {
				ctrl->flags |= _POINTER_FLAG_TOUCH;
				if (flags & ABS_PRESS_DATA) {
					ctrl->flags |= _POINTER_FLAG_PRESSURE;
				}
			} else {
				ctrl->flags |= _POINTER_FLAG_MOUSE;
			}

			if (ctrl->buttons) {
				ctrl->flags |= _POINTER_FLAG_BUTTON;
			}

			if (module->devctrl) {

				if ((module->devctrl) (module, DEVCTL_GETPTRCOORD,
															 &ctrl->num_coord) != -1) {

					if (flags & ABSOLUTE) {

						for (range.which = 0; range.which <
								 ctrl->num_coord; range.which++) {
							(module->devctrl) (module, DEVCTL_GETCOORDRNG, &range);
							ctrl->range[range.which].min = range.min;
							ctrl->range[range.which].max = range.max;
						}
					}
				} else {
					ctrl->num_coord = 0;
				}
			} else {
				ctrl->num_coord = 0;
			}

			if (module->devctrl) {
				if ((module->devctrl) (module, DEVCTL_GETPTRPRESS,
															 &ctrl->num_pressure) != -1) {

					for (range.which = 0; range.which <
							 ctrl->num_pressure; range.which++) {

						(module->devctrl) (module, DEVCTL_GETPRESSRNG, &range);
						ctrl->range[range.which + ctrl->num_coord].min = range.min;
						ctrl->range[range.which + ctrl->num_coord].max = range.max;
					}
				} else {
					ctrl->num_pressure = 0;
				}
			} else {
				ctrl->num_pressure = 0;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*ctrl);
			SETIOV (&smx[2], ctrl, sizeof (*ctrl));
			break;
		}

	case _POINTERGETCTRL & 0xffff:{

			struct _pointer_ctrl *ctrl =
				(struct _pointer_ctrl *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			if (verbosity)
				printf ("POINTERGETCTRL\n");

			module = ctrl->type.handle;
			if (!module) {
				reply.result = ESRCH;
				goto reply;
			}

			memset (ctrl, '\0', sizeof (*ctrl));
			if (module->devctrl) {
				struct devctl_rel_accel accel;
				struct devctl_wheel_accel wheel_accel;

				if (module) {
					unsigned swap = 0, wheel = 0;

					if ((module->devctrl) (module, DEVCTL_GETACCEL, &accel) == -1) {
						reply.result = ENOTSUP;
					}

					if ((module->devctrl) (module, DEVCTL_GETWHEEL_ACCEL,
																 &wheel_accel) == -1) {
						reply.result = ENOTSUP;
					}

					if ((module->devctrl) (module, DEVCTL_GETSWAP, &swap) == -1) {
						reply.result = ENOTSUP;
					}
					if ((module->devctrl) (module, DEVCTL_GETWHEEL, &wheel) == -1) {
						reply.result = ENOTSUP;
					}

					ctrl->flags = (swap << _POINTER_FLAG_SWAP_POS)
						| (wheel & 3) << 20;
					ctrl->gain = accel.gain;
					ctrl->threshold = accel.threshold;
				} else {
					reply.result = ESRCH;
				}
			}

		reply:
			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*ctrl);
			SETIOV (&smx[2], ctrl, sizeof (*ctrl));
			break;
		}

	case _POINTERSETCTRL & 0xffff:{

			struct _pointer_ctrl *ctrl =
				(struct _pointer_ctrl *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			module = ctrl->type.handle;
			if (!module) {
				break;
			}

			if (module->devctrl) {
				struct devctl_rel_accel accel;
				unsigned swap, wheel;

				accel.gain = ctrl->gain;
				accel.threshold = ctrl->threshold;
				swap = (ctrl->flags & _POINTER_FLAG_SWAP_MASK) >>
					_POINTER_FLAG_SWAP_POS;
				wheel = (ctrl->flags & 0x00300000) >> 20;

				if (module) {
					if ((module->devctrl) (module, DEVCTL_SETACCEL, &accel) == -1) {
						reply.result = ENOTSUP;
					}
					if ((module->devctrl) (module, DEVCTL_SETSWAP, &swap) == -1) {
						reply.result = ENOTSUP;
					}
					if ((module->devctrl) (module, DEVCTL_SETWHEEL, &wheel) == -1)
						reply.result = ENOTSUP;
				} else {
					reply.result = ESRCH;
				}
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*ctrl);
			SETIOV (&smx[2], ctrl, sizeof (*ctrl));
			break;
		}

	case _POINTERSETMATRIX & 0xffff:{

			struct _pointer_transform *transform =
				(struct _pointer_transform *) ((char *) msg +
																			 sizeof (struct _Ph_msg_forward));

			module = transform->type.handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_ABS:{

					struct devctl_disparea_res new_res;

					new_res.xl = transform->xy_coord.xoffset.number;
					new_res.yl = transform->xy_coord.yoffset.number;
					new_res.xh = transform->xy_coord.xscale.number;
					new_res.yh = transform->xy_coord.yscale.number;

					if (module && module->devctrl) {
						(module->devctrl) (module, DEVCTL_RESCHG, &new_res);
					}
				}

			default:
				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*transform);
			SETIOV (&smx[2], transform, sizeof (*transform));
			break;
		}

	case _KEYBOARDGETCTRL & 0xffff:{

			struct _keyboard_ctrl *kbctrl =
				(struct _keyboard_ctrl *) ((char *) msg +
																	 sizeof (struct _Ph_msg_forward));

			module = (kbctrl) ? kbctrl->type.handle : NULL;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					struct devctl_getkbd info;

					(module->devctrl) (module, DEVCTL_GETKBD, &info);
					kbctrl->rate = info.rate;
					kbctrl->delay = info.delay;
					break;
				}

			default:

				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*kbctrl);
			SETIOV (&smx[2], kbctrl, sizeof (*kbctrl));
			break;
		}

	case _KEYBOARDSETCTRL & 0xffff:{

			struct _keyboard_ctrl *kbctrl =
				(struct _keyboard_ctrl *) ((char *) msg +
																	 sizeof (struct _Ph_msg_forward));

			module = kbctrl->type.handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					if (module->devctrl) {
						struct devctl_setkbd devctl;

						if (verbosity) {
							printf ("%s: setting kb ctrl parms [rate %d, delay %d]\n",
											module->name, kbctrl->rate, kbctrl->delay);
						}
						devctl.delay = kbctrl->delay;
						devctl.rate = kbctrl->rate;
						if ((module->devctrl) (module, DEVCTL_SETKBD, &devctl) == -1) {
							reply.result = ENOTSUP;
						}
					}

					break;
				}

			default:

				reply.result = EINVAL;
				break;
			}
			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*kbctrl);
			SETIOV (&smx[2], kbctrl, sizeof (*kbctrl));
			break;
		}

	case _KEYBOARDGETKEYMAPPATH & 0xffff:{

			struct _keyboard_ctrl *keymap =
				(struct _keyboard_ctrl *) ((char *) msg +
																	 sizeof (struct _Ph_msg_forward));

			if (verbosity)
				printf ("devctl: KEYBOARDGETKEYMAPPATH\n");

			module = keymap->type.handle;
			if (!module || !keymap) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					char *kmf = keymap->mapfile;

					if (kmf) {
						if ((module->devctrl) (module, DEVCTL_GETKEYF, kmf) == -1) {
							reply.result = ENOTSUP;
						}
					}

					break;
				}

			default:

				reply.result = EINVAL;
				break;
			}
			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*keymap);
			SETIOV (&smx[2], keymap, sizeof (*keymap));
			break;
		}

	case _KEYBOARDSETKEYMAPPATH & 0xffff:{

			struct _keyboard_ctrl *keymap =
				(struct _keyboard_ctrl *) ((char *) msg +
																	 sizeof (struct _Ph_msg_forward));

			if (verbosity)
				printf ("devctl: KEYBOARDSETKEYMAPPATH\n");

			module = keymap->type.handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					char *kmf = keymap->mapfile;

					if ((module->devctrl) (module, DEVCTL_SETKEYF, kmf)
							== -1) {
						reply.result = ENOTSUP;
					}
					break;
				}

			default:

				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*keymap);
			SETIOV (&smx[2], keymap, sizeof (*keymap));
			break;
		}

	case _KEYBOARDRELOADCONFIG & 0xffff:
		{
			struct _interact_device *pDevice =
				(struct _interact_device *) ((char *) msg +
																		 sizeof (struct _Ph_msg_forward));
			if (verbosity)
				printf ("devctl: KEYBOARDRELOADCONFIG\n");

			module = pDevice->handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {
			case DEVI_CLASS_KBD:
				if ((module->devctrl) (module, DEVCTL_RELOAD_KBDS, NULL) == -1)
					reply.result = ENOTSUP;
				break;
			default:
				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*pDevice);
			SETIOV (&smx[2], pDevice, sizeof (*pDevice));

		}
		break;

	case _FEEDBACKGETLEDMASK & 0xffff:{

			struct _feedback_led *led =
				(struct _feedback_led *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			module = led->type.handle;
			if (!module)
				break;

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					if (led) {
						(module->devctrl) (module, DEVCTL_GETLEDMASK, led);
					}
					break;
				}
			default:
				reply.result = EINVAL;
				break;
			}
			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*led);
			SETIOV (&smx[2], led, sizeof (*led));
			break;
		}

	case _FEEDBACKGETLED & 0xffff:{
			struct _feedback_led *led =
				(struct _feedback_led *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			module = led->type.handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					if (led) {
						struct devctl_led led2;

						(module->devctrl) (module, DEVCTL_GETLED, &led2);
						led->value = led2.value;
						led->mask = led2.mask;
					}

					break;
				}

			default:

				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*led);
			SETIOV (&smx[2], led, sizeof (*led));
			break;
		}

	case _FEEDBACKSETLED & 0xffff:{

			struct _feedback_led *led =
				(struct _feedback_led *) ((char *) msg +
																	sizeof (struct _Ph_msg_forward));

			module = led->type.handle;
			if (!module) {
				break;
			}

			switch (module->type & DEVI_CLASS_MASK) {

			case DEVI_CLASS_KBD:{

					reply.result = (module->devctrl) (module, DEVCTL_SETLED, led);
					break;
				}

			default:
				reply.result = EINVAL;
				break;
			}

			reply.fwd.hdr.subtype = msg->hdr.subtype;
			reply.fwd.len += sizeof (*led);
			SETIOV (&smx[2], led, sizeof (*led));
			break;
		}

	case _SETDEVICESTATE & 0xffff: {

		struct _device_state *device_state =
		  (struct _device_state *) ((char *) msg +
		                           sizeof (struct _Ph_msg_forward));

		module = device_state->type.handle;

		if (!module) {
			break;
		}

		/* Reply now since we don't return anything, this covers the Stop
		   condition which simply exits the driver */
		reply.fwd.hdr.subtype = msg->hdr.subtype;
		reply.fwd.len += sizeof (*device_state);

		SETIOV (&smx[2], device_state, sizeof (*device_state));

		devhdr.i.nbytes = sizeof (reply) + reply.fwd.len;

		if (MsgSendv (IpCurrentFd, smx, 3, rmx, 1) < 0)
			perror ("MsgSendv");

		/* Find out what device the user wants to change the state for */
		switch (module->type & DEVI_CLASS_MASK) {

		case DEVI_CLASS_ABS:
			(module->devctrl) (module, DEVCTL_SET_ABS_STATE, (int *) device_state->state);
			break;

		case DEVI_CLASS_KBD:
			(module->devctrl) (module, DEVCTL_SET_KBD_STATE, (int *) device_state->state);
			break;

		case DEVI_CLASS_REL:
			(module->devctrl) (module, DEVCTL_SET_REL_STATE, (int *) device_state->state);
			break;
		}

		return;
	}

	default:
		reply.result = EINVAL;
		break;
	}

	devhdr.i.nbytes = sizeof (reply) + reply.fwd.len;

	if (MsgSendv (IpCurrentFd, smx, 3, rmx, 1) < 0)
		perror ("MsgSendv");

}

void
filter_ctrl (short type, void *ctrl)
{
	struct devctl_disparea_res new_res;
	struct devctl_devcoord new_coords;
	struct devctl_abs_mode abs_mode;
	input_module_t *module;
	unsigned flags, mask;
	PhKeyboardCtrlEvent_t *kbd = NULL;
	PhPointerCtrlEvent_t *ptr = NULL;

	module = NULL;

	if (verbosity) {
		printf ("Got a filter control event\n");
	}
	if (type == Ph_EV_KEYBOARD_CTRL) {
		if (bus_interrogate (DEVI_CLASS_KBD, &module) == 0) {
			return;
		}
		kbd = (PhKeyboardCtrlEvent_t *) ctrl;
		flags = kbd->ctrl.filter.flags;
		mask = KEYBOARD_CTRL_TYPE_MASK;

	} else {
		if (bus_interrogate (DEVI_CLASS_ABS, &module) == 0) {
			if (bus_interrogate (DEVI_CLASS_REL, &module) == 0) {
				return;
			}
		}

		ptr = (PhPointerCtrlEvent_t *) ctrl;
		flags = ptr->ctrl.filter.flags;
		mask = POINTER_CTRL_TYPE_MASK;
	}

	switch (flags & mask) {

	case POINTER_CTRL_ON:
		if (verbosity)
			printf ("POINTER_CTRL_ON\n");

		abs_mode.mode = ABS_XLATE_MODE;

		if (module && module->devctrl)
			(module->devctrl) (module, DEVCTL_ABS_MODE, &abs_mode);

		break;

	case POINTER_CTRL_OFF:
		if (verbosity)
			printf ("POINTER_CTRL_OFF\n");

		abs_mode.mode = ABS_RAW_MODE;

		if (module && module->devctrl)
			(module->devctrl) (module, DEVCTL_ABS_MODE, &abs_mode);

		break;

	case POINTER_CTRL_SETMATRIX:{

			/* For the older four point calib info */
			struct _pointer_matrix *scrn = &ptr->ctrl.filter.scrn;
			struct _pointer_matrix *coord = &ptr->ctrl.filter.coord;

			/* Newer 3 Point calibration */
			if (ptr->ctrl.filter.flags & POINTER_CTRL_THREEPTMATRIX) {
				if (module && module->devctrl) {
					(module->devctrl) (module, DEVCTL_SET_CALIB_MATRIX,
														 &ptr->three_pt_matrix);
				}
			}
			if (ptr->ctrl.filter.flags & POINTER_CTRL_SCRNMATRIX) {

				new_res.xl = scrn->xoffset.number;
				new_res.yl = scrn->yoffset.number;
				new_res.xh = scrn->xscale.number;
				new_res.yh = scrn->yscale.number;

				if (module && module->devctrl) {
					(module->devctrl) (module, DEVCTL_RESCHG, &new_res);
				}
			}

			if (ptr->ctrl.filter.flags & POINTER_CTRL_DEVMATRIX) {

				if (ptr->ctrl.filter.flags & POINTER_CTRL_SWAP)
					new_coords.swap = 1;
				else
					new_coords.swap = 0;

				new_coords.xl = coord->xoffset.number;
				new_coords.yl = coord->yoffset.number;
				new_coords.xh = coord->xscale.number;
				new_coords.yh = coord->yscale.number;

				if (module && module->devctrl) {
					(module->devctrl) (module, DEVCTL_COORDCHG, &new_coords);
				}
			}
			break;
		}

	case KEYBOARD_CTRL_FILENAME:

		if (verbosity)
			printf ("KEYBOARD_CTRL_FILENAME\n");

		if (module && module->devctrl) {
			(module->devctrl) (module, DEVCTL_SETKEYF, kbd->ctrl.filter.file);
		}
		break;

	case POINTER_CTRL_GAIN:
	case POINTER_CTRL_THRESHOLD:{
			struct devctl_rel_accel acceleration_parms;

			memset (&acceleration_parms, 0xff, sizeof (acceleration_parms));

			if (flags & POINTER_CTRL_GAIN) {
				acceleration_parms.gain = ptr->ctrl.filter.gain;
			}

			if (flags & POINTER_CTRL_THRESHOLD) {
				acceleration_parms.threshold = ptr->ctrl.filter.threshold;
			}

			(module->devctrl) (module, DEVCTL_SETACCEL, &acceleration_parms);
			break;
		}

	default:
		break;
	}
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/service.c $ $Rev: 657836 $" );
