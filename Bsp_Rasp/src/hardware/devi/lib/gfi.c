
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
 * We extend the OCB and attr structures - set things up for the function
 * prototypes in iofunc.h and resmgr.h
 */
struct gfi_ocb;
struct gfi_ig;

#define IOFUNC_OCB_T		struct gfi_ocb
#define RESMGR_OCB_T		struct gfi_ocb
#define IOFUNC_ATTR_T		struct gfi_ig
#define RESMGR_HANDLE_T		struct gfi_ig

#include <pthread.h>
#include <sys/resmgr.h>
#include <sys/gfi_iomsg.h>
#include <gf/gfi.h>

#include <sys/devi.h>
#include "photon.h"

#define MAX_EVENT_QUEUE_SIZE	64

typedef struct gfi_ig
{
	iofunc_attr_t attr;						/* Must be first */
	int ocb_size;									/* Must be second */
	int resmgr_id;
}
gfi_ig_t;

typedef struct gfi_ocb
{
	iofunc_ocb_t ocb;							/* Must be first */
	gfi_ig_t *gfig;
	int unblock;
}
gfi_ocb_t;

typedef struct layer_info
{
	struct layer_info *next;
	gfi_ocb_t *owner;
	int z_pos;
	int x1, y1, x2, y2;
	unsigned flags;
	gfi_event_t *events;
	int ev_head;
	int ev_tail;
	int max_events;
	int n_events;
}
layer_info_t;

static resmgr_connect_funcs_t connect_func;
static resmgr_io_funcs_t io_func;

pthread_mutex_t gfi_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gfi_cv = PTHREAD_COND_INITIALIZER;

/* Pointer to global list of all input layers, sorted by z_pos, front to back */
layer_info_t *linfo_list;
int events_enqueued;

layer_info_t *kbd_focus_layer;

static gfi_event_t *enqueue_ev (layer_info_t * linfo);

int bbox_x1, bbox_y1, bbox_x2, bbox_y2;

static layer_info_t *
find_linfo (int z_pos)
{
	layer_info_t *linfo;

	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if (linfo->z_pos == z_pos) {
			return linfo;
		}
	}

	return NULL;
}

static void
calc_bbox ()
{
	layer_info_t *linfo;

	bbox_x1 = bbox_y1 = INT_MAX;
	bbox_x2 = bbox_y2 = 0;

	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if (bbox_x1 > linfo->x1) {
			bbox_x1 = linfo->x1;
		}
		if (bbox_y1 > linfo->y1) {
			bbox_y1 = linfo->y1;
		}
		if (bbox_x2 < linfo->x2) {
			bbox_x2 = linfo->x2;
		}
		if (bbox_y2 < linfo->y2) {
			bbox_y2 = linfo->y2;
		}
	}
}

static int
register_layer (gfi_ocb_t * ocb, gfi_iomsg_t * cmd)
{
	layer_info_t *linfo, *newlinfo, *prev = NULL;
	int z_pos = cmd->u.reg.z_pos;

	/*
	 * First of all, make sure there isn't already an input layer at this
	 * Z position.
	 */
	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if (linfo->z_pos == z_pos) {
			return EBUSY;
		}
	}

	if ((newlinfo = calloc (sizeof *newlinfo, 1)) == NULL) {
		return errno;
	}

	newlinfo->z_pos = z_pos;
	newlinfo->owner = ocb;

	if (linfo_list == NULL) {
		linfo_list = newlinfo;
	} else {
		/* Insert it on the global list, keeping it sorted */
		for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
			if (z_pos > linfo->z_pos) {
				if (linfo == linfo_list) {
					newlinfo->next = linfo_list;
					linfo_list = newlinfo;
				} else {
					newlinfo->next = linfo;
					prev->next = newlinfo;
				}
				break;
			}
			prev = linfo;
		}
		if (linfo == NULL && prev != NULL) {
			/* Wasn't inserted yet, put it at the end of the list */
			prev->next = newlinfo;
		}
	}

	return EOK;
}

static int
unregister_layer (gfi_ocb_t * ocb, gfi_iomsg_t * cmd)
{
	layer_info_t *linfo, *prev = NULL;
	int z_pos = cmd->u.unreg.z_pos;

	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if (linfo->z_pos == z_pos) {
			if (linfo == linfo_list) {
				linfo_list = linfo->next;
			} else {
				prev->next = linfo->next;
			}
			if (linfo->events != NULL) {
				free (linfo->events);
			}
			if (kbd_focus_layer == linfo) {
				kbd_focus_layer = NULL;
			}
			free (linfo);
			calc_bbox ();
			return EOK;
		}
		prev = linfo;
	}

	return ENOENT;
}

static int
get_event (gfi_ocb_t * ocb, gfi_iomsg_t * cmd, gfi_iomsg_reply_t * rep)
{
	layer_info_t *linfo;
	int block = cmd->u.get_event.block;
	gfi_event_t *gfev;
	int i;

	rep->u.get_event.got_event = 0;

	if (block) {
		while (events_enqueued == 0 && !ocb->unblock) {
			pthread_cond_wait (&gfi_cv, &gfi_mutex);

			if (ocb->unblock) {
				return EINTR;
			}
		}
	} else {
		if (events_enqueued == 0) {
			return EOK;
		}
	}

	for (;;) {
		int found = 0;

		for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
			for (i = 0; i < cmd->u.get_event.nlayers; i++) {
				if (cmd->u.get_event.layer_z_pos[i] ==
						linfo->z_pos && linfo->n_events > 0) {
					found = 1;
					break;
				}
			}
			if (found) {
				break;
			}
		}

		/*
		 * User not interested in the layer(s) for which event(s) are
		 * currently enqueued.
		 */
		if (linfo == NULL) {
			if (block) {
				pthread_cond_wait (&gfi_cv, &gfi_mutex);
				if (ocb->unblock) {
					return EINTR;
				}
			} else {
				return EOK;
			}
		} else {
			break;
		}
	}

	rep->u.get_event.got_event = 1;

	gfev = &linfo->events[linfo->ev_head];

	rep->u.get_event.ev = *gfev;
	rep->u.get_event.got_event = 1;

	if (++linfo->ev_head == linfo->max_events) {
		linfo->ev_head = 0;
	}

	linfo->n_events--;
	events_enqueued--;

	return EOK;
}

static int
set_attrs (gfi_iomsg_t * cmd)
{
	layer_info_t *linfo;
	unsigned flags = cmd->u.set_attrs.flags;

	if ((linfo = find_linfo (cmd->u.set_attrs.z_pos)) != NULL) {
		linfo->x1 = cmd->u.set_attrs.x1;
		linfo->y1 = cmd->u.set_attrs.y1;
		linfo->x2 = cmd->u.set_attrs.x2;
		linfo->y2 = cmd->u.set_attrs.y2;

		if ((flags & GFI_ATTR_PTR_SENSITIVE) !=
				(linfo->flags & GFI_ATTR_PTR_SENSITIVE)) {
			linfo->flags = flags;
		} else {
			linfo->flags = flags;
		}

		calc_bbox ();

		return EOK;
	}

	return ENOENT;
}

static int
set_focus (gfi_iomsg_t * cmd)
{
	layer_info_t *linfo;
	int z_pos = cmd->u.set_focus.z_pos;

	if (z_pos == -1) {
		kbd_focus_layer = NULL;
	}

	if ((linfo = find_linfo (z_pos)) != NULL) {
		kbd_focus_layer = linfo;
		return EOK;
	}

	return ENOENT;
}

static int
emit_event (gfi_iomsg_t * cmd)
{
	layer_info_t *linfo;
	int z_pos = cmd->u.emit_event.z_pos;
	gfi_event_t *gfev;

	if ((linfo = find_linfo (z_pos)) != NULL) {
		if ((gfev = enqueue_ev (linfo)) == NULL) {
			return ENOMEM;
		}
		*gfev = cmd->u.emit_event.ev;

		return EOK;
	}

	return ENOENT;
}

int
gfi_open (resmgr_context_t * ctp,
					io_open_t * msg, RESMGR_HANDLE_T * gfig, void *extra)
{
	if (gfig == NULL) {
		return ENOSYS;
	}

	return iofunc_open_default (ctp, msg, (iofunc_attr_t *) gfig, extra);
}


int gfi_devctl (resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
	int status = 0, nbytes = 0;
	input_module_t  *module;

	status = iofunc_devctl_default (ctp, msg, (iofunc_ocb_t *)ocb);

	if (status != _RESMGR_DEFAULT) {
		return (status);
	}

	status = EOK;

	switch (msg->i.dcmd) {
	case _SETDEVICESTATE: {
		struct _device_state *device_state;

		device_state = (struct _device_state *)(_DEVCTL_DATA (*msg));

		switch (device_state->type.type) {
		case DEVI_CLASS_ABS:
			// look for an module who matches device
			if (bus_interrogate (DEVI_CLASS_ABS, &module) == 0) {
			    return ENODEV;
			}
				
			(module->devctrl) (module, DEVCTL_SET_ABS_STATE, (int *) device_state->state); 
			break;

		case DEVI_CLASS_KBD:
			// look for an module who matches device
			if( bus_interrogate (DEVI_CLASS_KBD, &module) == 0 ) {
			    return ENODEV;
			}
			
			(module->devctrl) (module, DEVCTL_SET_KBD_STATE, (int *) device_state->state); 
			break;

		case DEVI_CLASS_REL:
			// look for an module who matches device
			if( bus_interrogate (DEVI_CLASS_REL, &module) == 0 ) {
			    return ENODEV;
			}

			(module->devctrl) (module, DEVCTL_SET_REL_STATE, (int *) device_state->state); 
			break;
		}
		break;
	}
	default:
		fprintf (stderr, "DEVCTL command %i is not supported\n",  (int)(msg->i.dcmd));
		status = ENOSYS;
	}

	memset (&msg->o, 0, sizeof (msg->o));
	msg->o.nbytes = nbytes;
	msg->o.ret_val = status;

	return ((status == EOK) ? _RESMGR_PTR (ctp, &msg->o, sizeof (msg->o) + nbytes) : _RESMGR_ERRNO (status));
}

int
gfi_close (resmgr_context_t * ctp, void *reserved, RESMGR_OCB_T * ocb)
{
	layer_info_t *linfo, *prev = NULL;

	pthread_mutex_lock (&gfi_mutex);

	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if (linfo->owner == ocb) {
			if (linfo == linfo_list) {
				linfo_list = linfo->next;
			} else {
				prev->next = linfo->next;
			}
			if (linfo->events != NULL) {
				free (linfo->events);
			}
			if (kbd_focus_layer == linfo) {
				kbd_focus_layer = NULL;
			}
			free (linfo);
		}
		prev = linfo;
	}

	pthread_mutex_unlock (&gfi_mutex);

	return 0;
}

int
gfi_unblock (resmgr_context_t * ctp, io_pulse_t * msg, RESMGR_OCB_T * ocb)
{
	int rc;

	if ((rc = iofunc_unblock_default (ctp,
																		msg,
																		(iofunc_ocb_t *) ocb)) !=
			_RESMGR_DEFAULT) {
		return rc;
	}

	pthread_mutex_lock (&gfi_mutex);

	ocb->unblock = 1;
	pthread_cond_broadcast (&gfi_cv);

	pthread_mutex_unlock (&gfi_mutex);

	return _RESMGR_DEFAULT;
}

static int
set_ptr_ctrl( gfi_iomsg_t *msg ) 
{
	input_module_t 			*module;
	struct devctl_abs_mode 	abs_mode;
	
	// look for an absolute device
	if( bus_interrogate (DEVI_CLASS_ABS, &module) == 0 ) {
		return ENODEV;
	}
	
	if( verbosity ) {
		printf( "set_ptr_ctrl(%s), module=%p\n", 
					msg->u.set_ptr_ctrl.flags & 0x01 ? "cooked" : "raw",
					module );
	}

	abs_mode.mode = msg->u.set_ptr_ctrl.flags & 0x01 ? ABS_XLATE_MODE : ABS_RAW_MODE;
	
	if( module && module->devctrl )
			module->devctrl( module, DEVCTL_ABS_MODE, &abs_mode );

	return EOK;
}

static int
set_ptr_matrix( int mode, gfi_iomsg_t *msg )
{
	input_module_t 			*module;
	
	// look for an absolute device
	if( bus_interrogate (DEVI_CLASS_ABS, &module) == 0 ) {
		return ENODEV;
	}
	
	if( verbosity ) {
		printf( "set_ptr_matrix(%d), module=%p\n", mode, module );
	}
	
	if( !module || !module->devctrl ) {
		return ENOENT;
	}
	
	if( mode == 3 ) {
		struct _three_point_calib_matrix matrix;
		matrix.An = msg->u.set_ptr_matrix3.An;
		matrix.Bn = msg->u.set_ptr_matrix3.Bn;
		matrix.Cn = msg->u.set_ptr_matrix3.Cn;
		matrix.Dn = msg->u.set_ptr_matrix3.Dn;
		matrix.En = msg->u.set_ptr_matrix3.En;
		matrix.Fn = msg->u.set_ptr_matrix3.Fn;
		matrix.Divider = msg->u.set_ptr_matrix3.Divider;
	
		module->devctrl( module, DEVCTL_SET_CALIB_MATRIX, &matrix);
		
	} else if( mode == 4 ) {
		struct devctl_disparea_res 	new_res;
		struct devctl_devcoord 		new_coords;
		
		new_res.xl = msg->u.set_ptr_matrix4.scr_xoffset;
		new_res.yl = msg->u.set_ptr_matrix4.scr_yoffset;
		new_res.xh = msg->u.set_ptr_matrix4.scr_xscale;
		new_res.yh = msg->u.set_ptr_matrix4.scr_yscale;

		module->devctrl( module, DEVCTL_RESCHG, &new_res );
		
		new_coords.swap = msg->u.set_ptr_matrix4.swap;
		new_coords.xl = msg->u.set_ptr_matrix4.coord_xoffset;
		new_coords.yl = msg->u.set_ptr_matrix4.coord_yoffset;
		new_coords.xh = msg->u.set_ptr_matrix4.coord_xscale;
		new_coords.yh = msg->u.set_ptr_matrix4.coord_yscale;

		module->devctrl( module, DEVCTL_COORDCHG, &new_coords );
		
	} else {
		return EINVAL;
	}
	return EOK;
}

int
gfi_msg (resmgr_context_t * ctp, io_msg_t * msg, RESMGR_OCB_T * ocb)
{
	gfi_iomsg_reply_t rep;
	int rc = ENOSYS;
	int replysize = 0;
	gfi_iomsg_t *cmd = (gfi_iomsg_t *) msg;

	iofunc_attr_unlock ((void *) ((iofunc_ocb_t *) ocb)->attr);
	pthread_mutex_lock (&gfi_mutex);

	switch (msg->i.subtype) {
	case GFI_REGISTER_LAYER:
		rc = register_layer (ocb, cmd);
		break;
	case GFI_UNREGISTER_LAYER:
		rc = unregister_layer (ocb, cmd);
		break;
	case GFI_SET_ATTRS:
		rc = set_attrs (cmd);
		break;
	case GFI_GET_EVENT:
		rc = get_event (ocb, cmd, &rep);
		replysize = sizeof (rep.u.get_event);
		break;
	case GFI_SET_FOCUS:
		rc = set_focus (cmd);
		break;
	case GFI_EMIT_EVENT:
		rc = emit_event (cmd);
		break;
	case GFI_SET_PTR_CTRL:
		rc = set_ptr_ctrl(cmd);
		break;
	case GFI_SET_PTR_MATRIX3:
		rc = set_ptr_matrix( 3, cmd );
		break;
	case GFI_SET_PTR_MATRIX4:
		rc = set_ptr_matrix( 4, cmd );
		break;
	default:
		break;
	}
	ocb->unblock = 0;
	pthread_mutex_unlock (&gfi_mutex);
	iofunc_attr_lock ((void *) ((iofunc_ocb_t *) ocb)->attr);

	if (rc == EOK) {
		if (replysize >= 0) {
			MsgReply (ctp->rcvid, EOK, &rep, replysize);
		}

		return _RESMGR_NOREPLY;
	}

	return _RESMGR_STATUS (ctp, rc);
}

int
gfi_close_dup (resmgr_context_t * ctp, io_close_t * msg, RESMGR_OCB_T * ocb)
{
	pthread_mutex_lock (&gfi_mutex);
	ocb->unblock = 1;
	pthread_cond_broadcast (&gfi_cv);
	pthread_mutex_unlock (&gfi_mutex);

	return iofunc_close_dup_default (ctp, msg, &ocb->ocb);
}

int
gfi_register (int input_group)
{
	gfi_ig_t *gfig;
	thread_pool_attr_t pool_attr;
	thread_pool_t *tpp;
	dispatch_t *dpp;
	char devname[32];

	if ((gfig = calloc (sizeof (*gfig), 1)) == NULL) {
		return -1;
	}

	gfig->ocb_size = sizeof (gfi_ocb_t);

	iofunc_attr_init (&gfig->attr, S_IFCHR | 0660, 0, NULL);
/* XXX TODO: group ownership as per io-display */

	if (connect_func.open == NULL) {
		iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_func,
											_RESMGR_IO_NFUNCS, &io_func);

		connect_func.open = gfi_open;

		io_func.close_ocb = gfi_close;
		io_func.msg = gfi_msg;
		io_func.unblock = gfi_unblock;
		io_func.close_dup = gfi_close_dup;
		io_func.devctl = gfi_devctl;
	}

	sprintf (devname, "/dev/devi/ig%d", input_group);

	if ((dpp = dispatch_create ()) == NULL) {
		fprintf (stderr, "Unable to allocate dispatch context\n");
		return -1;
	}

	memset (&pool_attr, 0, sizeof pool_attr);
	pool_attr.handle = dpp;

	/* We are only doing resmgr-type attach */
	pool_attr.context_alloc = resmgr_context_alloc;
	pool_attr.block_func = resmgr_block;
	pool_attr.unblock_func = resmgr_unblock;
	pool_attr.handler_func = resmgr_handler;
	pool_attr.context_free = resmgr_context_free;
	pool_attr.lo_water = 2;
	pool_attr.hi_water = 4;
	pool_attr.increment = 1;
	pool_attr.maximum = 50;

	if ((tpp = thread_pool_create (&pool_attr, POOL_FLAG_EXIT_SELF)) == NULL) {
		fprintf (stderr, "Unable to initialize thread pool\n");
		return EXIT_FAILURE;
	}

	if (pthread_create (NULL, pool_attr.attr, (void *) thread_pool_start, tpp)) {
		perror ("thread_create");
		exit (EXIT_FAILURE);
	}

	/* Register the pathname */
	if ((gfig->resmgr_id = resmgr_attach (dpp, NULL, devname,
																				_FTYPE_ANY, 0, &connect_func,
																				&io_func, gfig)) == -1) {
		fprintf (stderr, "%s: unable to attach name", devname);
		return -1;
	}

	return 0;
}

static gfi_event_t *
enqueue_ev (layer_info_t * linfo)
{
	void *freeptr;
	gfi_event_t *gfev;

	/* Is there room on the queue? */
	if (linfo->n_events >= linfo->max_events) {
		if (linfo->max_events >= MAX_EVENT_QUEUE_SIZE) {
			/*
			 * Don't want to grow event memory indefinitely.
			 * Chances are the app has lost it's marbles, and isn't
			 * reading the events anymore.
			 */
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_WARNING,
						 "Dropped pointer event for layer %d: queue full", linfo->z_pos);
			return NULL;
		}

		/* Grow the queue memory */
		if (linfo->max_events == 0) {
			linfo->max_events = 2;
		} else {
			linfo->max_events *= 2;
		}

		freeptr = linfo->events;

		linfo->events = realloc (linfo->events,
														 linfo->max_events * sizeof (gfi_event_t));

		if (linfo->events == NULL) {
			linfo->max_events = linfo->n_events = 0;
			linfo->ev_head = linfo->ev_tail = 0;
			free (freeptr);
			return NULL;
		}

		/* copy to the end, relies on buffer doubling size */
		memcpy (&linfo->events[linfo->n_events], &linfo->events[0],
						linfo->n_events * sizeof (gfi_event_t));
		/* shift left */
		memmove (&linfo->events[0], &linfo->events[linfo->ev_head],
						 linfo->n_events * sizeof (gfi_event_t));
		/* update */
		linfo->ev_head = 0;
		linfo->ev_tail = linfo->n_events;
	}

	gfev = &linfo->events[linfo->ev_tail];

	events_enqueued++;
	linfo->n_events++;

	if (++linfo->ev_tail == linfo->max_events) {
		linfo->ev_tail = 0;
	}

	pthread_cond_broadcast (&gfi_cv);

	return gfev;
}

static void
enqueue_ptr (layer_info_t * linfo,
						 PhRawPtrEvent_t const *ptr_ev, int xpos, int ypos, unsigned msec)
{
	gfi_event_t *gfev;

	if ((gfev = enqueue_ev (linfo)) == NULL) {
		return;
	}

	gfev->type = GFI_EVENT_TYPE_PTR;
	gfev->u.ptr.x = xpos - linfo->x1;
	gfev->u.ptr.y = ypos - linfo->y1;
	gfev->u.ptr.buttons = ptr_ev->button_state;
}

int gfi_missed_delta_x;
int gfi_missed_delta_y;

int
gfi_emit_ptr (PhRawPtrEvent_t const *ptr_ev)
{
	PhRawPtrCoord_t const *coord;
	layer_info_t *linfo;
	static int xpos = INT_MAX, ypos;
	int i;
	unsigned msec;
	int rc = 0;
	int last_xpos = xpos, last_ypos = ypos;
	int raw_mode = 0;

	input_module_t                  *module;

	// look for an absolute device
	if( bus_interrogate (DEVI_CLASS_ABS, &module) != 0 ) {
		extern int abs_get_mode( input_module_t *);
		raw_mode = abs_get_mode( module ) == 0;
	}

	if (xpos == INT_MAX) {
		if (OptFlags & OPT_NO_PHOTON) {
			xpos = 0;
		} else {
			/* Establish the initial Photon cursor position */
			PhCursorInfo_t cinfo;

			PhQueryCursor (IpGroup, &cinfo);
			xpos = cinfo.pos.x;
			ypos = cinfo.pos.y;

			if (bbox_x1 > xpos) {
				bbox_x1 = xpos;
			}
			if (bbox_x2 < xpos) {
				bbox_x2 = xpos;
			}
			if (bbox_y1 > ypos) {
				bbox_y1 = ypos;
			}
			if (bbox_y2 < ypos) {
				bbox_y2 = ypos;
			}
		}
	}

	msec = ptr_ev->msec;

	pthread_mutex_lock (&gfi_mutex);

	for (i = ptr_ev->num_coord, coord = ptr_ev->coord; --i >= 0; coord++) {
		if (ptr_ev->raw_flags & Ph_PTR_RAW_FLAG_XY_REL) {
			xpos += coord->x;
			ypos -= coord->y;
		} else {
			xpos = coord->x;
			ypos = coord->y;
		}
		msec += coord->dmsec;
	}

	if (!raw_mode) {
		/*
	 	* Clamp current pointer position to be inside the bounding box
	 	* of all sensitive layers for this display/input group.
		*/

		if (xpos > bbox_x2) {
			xpos = bbox_x2;
		}
		if (ypos > bbox_y2) {
			ypos = bbox_y2;
		}
		if (xpos < bbox_x1) {
			xpos = bbox_x1;
		}
		if (ypos < bbox_y1) {
			ypos = bbox_y1;
		}
	}
	
	/*
	 * Walk through the input layers, seeing if the event hits any
	 * senstive or opaque regions
	 */
	for (linfo = linfo_list; linfo != NULL; linfo = linfo->next) {
		if ( raw_mode || (xpos >= linfo->x1 && xpos <= linfo->x2 &&
				ypos >= linfo->y1 && ypos <= linfo->y2)) {
			if ((linfo->flags & GFI_ATTR_PTR_SENSITIVE) ) {
				enqueue_ptr (linfo, ptr_ev, xpos, ypos, msec);
			}
			if (!(linfo->flags & GFI_ATTR_PTR_TRANSPARENT)) {
				/* Swallow up the event */
				rc = 1;
				break;
			}
		}
	}

	if (rc == 1 && (ptr_ev->raw_flags & Ph_PTR_RAW_FLAG_XY_REL)) {
		gfi_missed_delta_x += xpos - last_xpos;
		gfi_missed_delta_y += ypos - last_ypos;
	}

	pthread_mutex_unlock (&gfi_mutex);

	return rc;
}

int
gfi_emit_kbd (PhRawKeyEvent_t const *kbd_ev)
{
	int rc = 0;
	gfi_event_t *gfev;

	pthread_mutex_lock (&gfi_mutex);

	if (kbd_focus_layer != NULL) {
		if (kbd_focus_layer->flags & GFI_ATTR_KEY_SENSITIVE) {
			gfev = &kbd_focus_layer->events[kbd_focus_layer->ev_tail];
			if ((gfev = enqueue_ev (kbd_focus_layer)) != NULL) {
				gfev->type = GFI_EVENT_TYPE_KEY;
				gfev->u.key.modifiers = kbd_ev->key_mods;
				gfev->u.key.flags = kbd_ev->key_flags;
				gfev->u.key.cap = kbd_ev->key_cap;
				gfev->u.key.sym = kbd_ev->key_sym;
				gfev->u.key.scan = kbd_ev->key_scan;
			}
		}

		if (!(kbd_focus_layer->flags & GFI_ATTR_KEY_TRANSPARENT)) {
			/* Prevent the key press from getting to Photon */
			rc = 1;
		}
	}

	pthread_mutex_unlock (&gfi_mutex);

	return rc;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/gfi.c $ $Rev: 657836 $" );
