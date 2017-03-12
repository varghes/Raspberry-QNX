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
 * resmgr.c
 *
 * This file contains the code used to implement the resource manager
 * interface for the devi drivers.  
 *
 * Status right now is basic functionality working.  Device files
 * can be read in blocking, non-blocking modes.  I/O notification has
 * also been implemented.  devctl() has not been implemented yet.
 * Other changes are *very* likely to follow ...
 *
 * Basic idea is that a device file will be established for each
 * event bus line running.  Keyboard devices will be called 
 * /dev/devi/keyboardN, with N starting at 0.  Relative devices will be
 * called /dev/devi/mouseN.  Absolute devices /dev/devi/touchN.  
 *
 * Once opened the devices can be read.  Data returned will be
 * binary data and will depend on the type of devices opened.
 * Keyboard devices will return struct _keyboard_packet, relative
 * devices struct _mouse_packet, absolute devices struct _touch1_packet.
 *
 */


#include <sys/devi.h>

#define DEVICE_NAME 	"/dev/devi/%s%d"
#define INC_PTR(p)	(p + 1) % RESMGR_Q_SIZE;

static int add_ocb (devi_attr_t *, RESMGR_OCB_T *);
static int remove_ocb (devi_attr_t *, RESMGR_OCB_T *);
static int devi_open (resmgr_context_t * ctp, io_open_t * msg,
											RESMGR_HANDLE_T * handle, void *extra);
static int devi_read (resmgr_context_t * ctp, io_read_t * msg,
											RESMGR_OCB_T * ocb);
static int devi_close (resmgr_context_t * ctp, void *reserved,
											 RESMGR_OCB_T * ocb);
static int devi_notify (resmgr_context_t * ctp, io_notify_t * msg,
												RESMGR_OCB_T * ocb);
static int devi_devctl (resmgr_context_t * ctp, io_devctl_t * msg,
												RESMGR_OCB_T * ocb);
static int devi_unblock (resmgr_context_t * ctp, io_pulse_t * msg,
												 RESMGR_OCB_T * ocb);

static int add_wait_queue (devi_attr_t *, devi_ocb_t *, resmgr_context_t *,
													 int);
static int remove_wait_queue (devi_attr_t *, struct _wait_q *);
static int process_wait_queue (devi_attr_t * attr);
static int process_read_data (int, devi_attr_t * attr, devi_ocb_t * ocb, int, int nonblock);


int
resmgr_create_device (event_bus_line_t * line)
{
	devi_attr_t *attr;
	resmgr_attr_t res_attr;
	static resmgr_connect_funcs_t connect_funcs;
	static resmgr_io_funcs_t io_funcs;
	static int once = 0;
	int i;
	char name[48];
	char path[_POSIX_PATH_MAX];
	int id;
	dev_t d;

	if ((attr = malloc (sizeof (devi_attr_t))) == NULL) {
		errno_print ("malloc");
		return (-1);
	}

	if (!once) {
		iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &connect_funcs,
											_RESMGR_IO_NFUNCS, &io_funcs);
		connect_funcs.open = devi_open;
		io_funcs.devctl = devi_devctl;
		io_funcs.read = devi_read;
		io_funcs.close_ocb = devi_close;
		io_funcs.unblock = devi_unblock;
		io_funcs.notify = devi_notify;
		once = 1;
	}

	iofunc_attr_init (&attr->attr, 0666 | S_IFCHR, NULL, NULL);
	attr->flags = line->type;
	attr->ocb_list = NULL;
	line->attr = attr;


	switch (attr->flags) {

	case DEVI_CLASS_KBD:
		strcpy (name, "keyboard");
		break;
	case DEVI_CLASS_REL:
		strcpy (name, "mouse");
		break;
	case DEVI_CLASS_ABS:
		strcpy (name, "touch");
		break;
	case DEVI_CLASS_JOYSTICK:
		strcpy (name, "joystick");
		break;
	case DEVI_CLASS_GAMEPAD:
		strcpy (name, "gamepad");
		break;
	case DEVI_CLASS_CONTROL:
		strcpy (name, "control");
		break;
	}

	attr->line = line;

	memset (&res_attr, 0, sizeof (res_attr));
	res_attr.msg_max_size = 2048;
	res_attr.nparts_max = 3;

	for (i = 0; i < 10; i++) {

		sprintf (path, DEVICE_NAME, name, i);
		if (access (path, F_OK) != 0)
			break;
	}
	if (i == 10) {

		char *pMsgTxt = "Error: unable to create device %s\n";

		fprintf (stderr, pMsgTxt, path);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, path);
		free (attr);
		return (-1);
	}

	id = resmgr_attach (devi_get_dispatch_handle (), &res_attr, path,
											_FTYPE_ANY, 0, &connect_funcs,
											&io_funcs, (void *) &attr->attr);

	if (id < 0) {
		char *pMsgTxt = "Error: could not attach resource manager\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		free (attr);
		return (-1);
	}

	attr->attr.rdev = rsrcdbmgr_devno_attach (name, -1, 0);
	resmgr_devino (id, &d, &attr->attr.inode);

	return (0);
}


void
resmgr_enqueue_packet (event_bus_line_t * line, char *data, int size)
{
	int nidx;
	struct _ocb_list *op;
	devi_attr_t *attr = line->attr;

	pthread_mutex_lock (&line->mutex);

	if (NULL == line->attr)
		return;

	for (op = line->attr->ocb_list; op; op = op->next) {
		nidx = INC_PTR (line->head);
		if (nidx == op->ocb->read_ptr) {
			op->ocb->read_ptr = INC_PTR (op->ocb->read_ptr);
		}

	}

	switch (line->type) {

	case DEVI_CLASS_KBD:{

			struct packet_kbd *kbd_data = (struct packet_kbd *) data;
			struct _keyboard_packet *kbd = &line->u.kq[line->head];

			memcpy (&kbd->time, &kbd_data->timestamp, sizeof(struct timespec));
			kbd->data.modifiers = kbd_data->key.modifiers;
			kbd->data.flags = kbd_data->key.flags;	
			kbd->data.key_cap = kbd_data->key.key_cap;
			kbd->data.key_sym = kbd_data->key.key_sym;
			kbd->data.key_scan = kbd_data->key.key_scan;
			break;
	}

	case DEVI_CLASS_REL:{

			struct packet_rel *rel = (struct packet_rel *) data;
			struct _mouse_packet *mp = &line->u.mq[line->head];

			memcpy (&mp->hdr.time, &rel->timestamp, sizeof (struct timespec));
			mp->hdr.buttons = rel->buttons;
			mp->dx = rel->dx;
			mp->dy = rel->dy;
			mp->dz = rel->dz;
			if (verbosity)
				printf ("EN: x: %d, y: %d, buttons: %x\n",
								rel->dx, rel->dy, rel->buttons);

			break;
		}

	case DEVI_CLASS_ABS:{

			struct packet_abs *abs = (struct packet_abs *) data;
			struct _touch1_packet *tp = &line->u.tq[line->head];

			memcpy (&tp->hdr.time, &abs->timestamp, sizeof (struct timespec));
			tp->hdr.buttons = abs->buttons;
			tp->x = abs->x;
			tp->y = abs->y;
			tp->z = abs->z;
			break;
		}
	case DEVI_CLASS_JOYSTICK:{
			struct packet_joystick *joystick_data = (struct packet_joystick *) data;
			struct _joystick_packet *jp = &line->u.jq[line->head];

			memcpy (&jp->hdr.time, &joystick_data->timestamp, sizeof (struct timespec));
			jp->x = joystick_data->x;
			jp->y = joystick_data->y;
			jp->z = joystick_data->z;

			jp->Rx = joystick_data->Rx;
			jp->Ry = joystick_data->Ry;
			jp->Rz = joystick_data->Rz;

			jp->slider = joystick_data->slider;

			jp->hat_switch = joystick_data->hat_switch;

			jp->hdr.button_state = joystick_data->button_state;
			break;
		}
	case DEVI_CLASS_GAMEPAD:{
		break;
	}
	case DEVI_CLASS_CONTROL:{
			struct packet_control *control = (struct packet_control *) data;
			struct _control_packet *cp = &line->u.cq[line->head];

			memcpy (&cp->hdr.time, &control->timestamp, sizeof (struct timespec));

			cp->hdr.button_state = control->button_state;

			cp->Rx = control->Rx;
			cp->Ry = control->Ry;
			cp->Rz = control->Rz;

			break;
		}
	default:
		break;
	}

	line->head = INC_PTR (line->head);

	if (attr->flags & DEVI_NOTIFICATION_ARMED) {
		iofunc_notify_trigger (attr->notify, 1, IOFUNC_NOTIFY_INPUT);
		attr->flags &= ~DEVI_NOTIFICATION_ARMED;
	}

	process_wait_queue (line->attr);
	pthread_mutex_unlock (&line->mutex);
}


static int
devi_open (resmgr_context_t * ctp, io_open_t * msg, RESMGR_HANDLE_T * handle,
					 void *extra)
{
	return (iofunc_open_default (ctp, msg, &handle->attr, extra));
}


static int
devi_read (resmgr_context_t * ctp, io_read_t * msg, RESMGR_OCB_T * ocb)
{
	int nonblock;
	devi_attr_t *attr;
	event_bus_line_t *line;

	attr = ocb->ocb.attr;
	line = attr->line;

	if (iofunc_read_verify (ctp, msg, &ocb->ocb, &nonblock) != EOK)
		return (errno);

	pthread_mutex_lock (&line->mutex);

	/* queue is empty */
	if (ocb->read_ptr == line->head) {

		if (nonblock) {
			pthread_mutex_unlock (&line->mutex);
			return (EAGAIN);
		}

		else {
			add_wait_queue (attr, ocb, ctp, msg->i.nbytes);
			pthread_mutex_unlock (&line->mutex);
			return (_RESMGR_NOREPLY);
		}
	}

	if (process_read_data (ctp->rcvid, attr, ocb, msg->i.nbytes, nonblock) < 0) {
		if (nonblock) {
			pthread_mutex_unlock (&line->mutex);
			return (EAGAIN);
		} else {
			add_wait_queue (attr, ocb, ctp, msg->i.nbytes);
			pthread_mutex_unlock (&line->mutex);
			return (_RESMGR_NOREPLY);
		}
	}

	pthread_mutex_unlock (&line->mutex);

	return (_RESMGR_NOREPLY);
}


static int
devi_close (resmgr_context_t * ctp, void *reserved, RESMGR_OCB_T * ocb)
{
	devi_attr_t *attr = ocb->ocb.attr;

	iofunc_notify_remove (ctp, attr->notify);
	remove_ocb (attr, ocb);
	return (iofunc_close_ocb_default (ctp, reserved, &ocb->ocb));
}


static int
devi_notify (resmgr_context_t * ctp, io_notify_t * msg, RESMGR_OCB_T * ocb)
{
	devi_attr_t *attr = ocb->ocb.attr;
	event_bus_line_t *line = attr->line;
	int trig = 0, n;
	int sts;

	/* we don't support output or out of band notification */

	if (msg->i.flags & (_NOTIFY_COND_OBAND | _NOTIFY_COND_OUTPUT))
		return (ENOSYS);

	pthread_mutex_lock (&line->mutex);

	/* if queue non-empty, trigger */
	if (ocb->read_ptr != line->head)
		trig = _NOTIFY_COND_INPUT;

	pthread_mutex_unlock (&line->mutex);

	sts = iofunc_notify (ctp, msg, attr->notify, trig, NULL, &n);
	if (sts == EBUSY)
		return (EBUSY);

	if (n)
		attr->flags |= DEVI_NOTIFICATION_ARMED;

	return (_RESMGR_NPARTS (1));
}


static int
devi_devctl (resmgr_context_t * ctp, io_devctl_t * msg, RESMGR_OCB_T * ocb)
{
	devi_attr_t *attr;
	iov_t iov;
	char *buffer, *allocd;
	long dcmd;
	int error, nbytes;

	attr = ocb->ocb.attr;
	error = EOK;

	if ((error =
			 iofunc_devctl (ctp, msg, &ocb->ocb, &attr->attr)) != _RESMGR_DEFAULT)
		return (error);

	/* ripped off from deva....:-) */
	if (!(get_device_direction (dcmd = msg->i.dcmd) & DEVDIR_TOFROM))
		nbytes = 0, buffer = allocd = NULL;
	else if ((nbytes = msg->i.nbytes) <= ctp->msg_max_size - sizeof (*msg))
		buffer = _DEVCTL_DATA (*msg), allocd = NULL;
	else if ((buffer = allocd = malloc ((nbytes + 3) & ~3)) == NULL)
		return (_RESMGR_ERRNO (ENOMEM));
	else if ((get_device_direction (dcmd) & DEVDIR_TO)) {
		SETIOV (&iov, buffer, nbytes);
		if (resmgr_msgreadv (ctp, &iov, 1, sizeof (msg->i)) == -1)
			goto error_out;
	}

	/* TODO: now call a helper function to actually do the devctl
	   returns < 0 on error, else nbytes to xfer back on EOK */

	msg->o.ret_val = error;
	if (error == EOK && get_device_direction (dcmd) & DEVDIR_FROM) {
		SETIOV (&iov, buffer, nbytes);
		if (resmgr_msgwritev (ctp, &iov, 1, sizeof (msg->o)) == -1)
			goto error_out;
	}

	if (allocd != NULL)
		free (allocd);

	return ((error == EOK) ? _RESMGR_PTR (ctp, &msg->o, sizeof (msg->o)) :
					_RESMGR_ERRNO (error));

error_out:
	error = errno;
	if (allocd != NULL)
		free (allocd);
	return (_RESMGR_ERRNO (error));
}


static int
devi_unblock (resmgr_context_t * ctp, io_pulse_t * msg, RESMGR_OCB_T * ocb)
{
	devi_attr_t *attr = ocb->ocb.attr;
	struct _wait_q *wp;

	for (wp = attr->wait_queue; wp; wp = wp->next) {
		if (wp->ocb == ocb) {
			MsgReply (wp->rcvid, EINTR, NULL, 0);
			remove_wait_queue (attr, wp);
			break;
		}
	}
	return (iofunc_unblock_default (ctp, msg, &ocb->ocb));
}


static int
add_ocb (devi_attr_t * attr, RESMGR_OCB_T * ocb)
{
	struct _ocb_list *op;

	if (attr->ocb_list == NULL) {

		attr->ocb_list = malloc (sizeof (struct _ocb_list));
		if (attr->ocb_list == NULL) {
			errno_print ("malloc");
			return (-1);
		}
		attr->ocb_list->ocb = ocb;
		attr->ocb_list->next = NULL;
		attr->ocb_list->prev = NULL;
	}

	else {

		for (op = attr->ocb_list; op->next != NULL; op = op->next);

		op->next = malloc (sizeof (struct _ocb_list));
		if (op->next == NULL) {
			errno_print ("malloc");
			return (-1);
		}
		op->next->ocb = ocb;
		op->next->prev = op;
		op->next->next = NULL;
	}

	return (0);
}


static int
remove_ocb (devi_attr_t * attr, RESMGR_OCB_T * ocb)
{
	struct _ocb_list *op;

	for (op = attr->ocb_list; op; op = op->next) {
		if (op->ocb == ocb)
			break;
	}

	if (op == NULL) {
		char *pMsgTxt = "Error: nable to remove ocb from list, not found\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (-1);
	}

	/* If removing first one */

	if (op->prev == NULL) {

		if (op->next) {
			attr->ocb_list = op->next;
			op->next->prev = NULL;
		} else {
			attr->ocb_list = NULL;
		}
	} else {
		if (op->next) {
			op->prev->next = op->next;
			op->next->prev = op->prev;
		} else
			op->prev->next = NULL;
	}
	return (0);
}


/*
 * There is some trickery needed here due to the fact that two resource
 * manager interfaces are living in the same process.  We have two different
 * types of OCB that we need to allocate.  In our extended iofuncs_attr_t
 * structures, we place an "ocb_size" field after the "attr" field.  If
 * the size field is set, then we're allocating an OCB for the GFI interface,
 * and "ocb_size" specifies the size of the OCB structure to allocated.
 * If the size field is 0, then it's an OCB for the legacy resmgr interface.
 */
IOFUNC_OCB_T *
iofunc_ocb_calloc (resmgr_context_t * ctp, IOFUNC_ATTR_T * attr)
{
	devi_ocb_t *ocb;
	int size = *(int *) ((iofunc_attr_t *) attr + 1);

	if (size == 0) {
		if (!(ocb = calloc (sizeof *ocb, 1))) {
			errno = ENFILE;
		}

		if (add_ocb (attr, ocb) < 0) {
			free (ocb);
			return (NULL);
		}

		pthread_mutex_lock (&attr->line->mutex);
		ocb->read_ptr = attr->line->head;
		pthread_mutex_unlock (&attr->line->mutex);
	} else {
		if (!(ocb = calloc (size, 1))) {
			errno = ENFILE;
		}
	}

	return (ocb);
}

void
iofunc_ocb_free (IOFUNC_OCB_T * ocb)
{
	free (ocb);
}

static int
add_wait_queue (devi_attr_t * attr, devi_ocb_t * ocb, resmgr_context_t * ctp,
								int nbytes)
{
	struct _wait_q *wp, *p;

	wp = malloc (sizeof (struct _wait_q));
	if (wp == NULL) {
		errno_print ("malloc");
		return (-1);
	}

	wp->ocb = ocb;
	wp->rcvid = ctp->rcvid;
	wp->nbytes = nbytes;
	wp->next = wp->prev = NULL;

	if (attr->wait_queue == NULL)
		attr->wait_queue = wp;
	else {
		for (p = attr->wait_queue; p->next != NULL; p = p->next);
		p->next = wp;
		wp->prev = p;
	}

	return (0);
}

static int
process_wait_queue (devi_attr_t * attr)
{
	struct _wait_q *wp;

	if (!attr->wait_queue)
		return (0);

	while ((wp = attr->wait_queue)) {
		if (process_read_data (wp->rcvid, attr, wp->ocb, wp->nbytes, 0) == EOK)
			remove_wait_queue (attr, wp);
	}

	return (0);
}


static int
process_read_data (int rcvid, devi_attr_t * attr, devi_ocb_t * ocb,
									 int nbytes, int nonblock)
{
	int q_size;
	int num_req_packets;
	int num_pkts_xfer;
	int num_pkts_end;
	iov_t iov[3];
	int parts = 1;
	char *ptr = NULL;
	event_bus_line_t *line = attr->line;

	if (line->head > ocb->read_ptr)
		q_size = line->head - ocb->read_ptr;
	else
		q_size = RESMGR_Q_SIZE - ocb->read_ptr + line->head;

	num_req_packets = nbytes / line->elem_size;

	if (num_req_packets == 0)
		MsgReply (rcvid, EOK, NULL, 0);

	if (q_size < num_req_packets && !nonblock) {
		return (-1);
	}

	num_pkts_xfer = min (num_req_packets, q_size);
	num_pkts_end = RESMGR_Q_SIZE - ocb->read_ptr;

	switch (line->type) {

	case DEVI_CLASS_KBD:
		ptr = (char *) line->u.kq;
		break;
	case DEVI_CLASS_REL:
		ptr = (char *) line->u.mq;
		break;
	case DEVI_CLASS_ABS:
		ptr = (char *) line->u.tq;
		break;
	case DEVI_CLASS_JOYSTICK:
		ptr = (char *) line->u.jq;
		break;
	case DEVI_CLASS_GAMEPAD:
		break;
	case DEVI_CLASS_CONTROL:
		ptr = (char *) line->u.cq;
		break;
	}

	if (num_pkts_end >= num_pkts_xfer)
		SETIOV (&iov[0], ptr + line->elem_size * ocb->read_ptr,
						num_pkts_xfer * line->elem_size);
	else {
		SETIOV (&iov[0], ptr + line->elem_size * ocb->read_ptr,
						num_pkts_end * line->elem_size);
		num_pkts_xfer -= num_pkts_end;
		SETIOV (&iov[1], ptr, num_pkts_xfer * line->elem_size);
		parts = 2;
	}
	MsgReplyv (rcvid, num_pkts_xfer * line->elem_size, iov, parts);
	ocb->read_ptr = (ocb->read_ptr + num_pkts_xfer) % RESMGR_Q_SIZE;

	return (EOK);
}


static int
remove_wait_queue (devi_attr_t * attr, struct _wait_q *remove)
{
	struct _wait_q *wp;

	for (wp = attr->wait_queue; wp; wp = wp->next) {
		if (wp == remove) {

			if (wp->prev == NULL) {
				attr->wait_queue = wp->next;
				if (attr->wait_queue)
					attr->wait_queue->prev = NULL;
			}

			else {
				wp->prev->next = wp->next;
				if (wp->next)
					wp->next->prev = wp->prev;
			}
			free (wp);
			break;
		}
	}

	return (0);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/resmgr.c $ $Rev: 657836 $" );
