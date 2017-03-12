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
 * lib.c
 *
 * Useful library routines used by module code.
 *
 */
#include <sys/devi.h>


static dispatch_t *dpp = NULL;	// Dispatch handler
static int IpCoid = -1;					// connection id to our receive channel 


/* devi_register_pulse
 *
 * Associate a pulse code with a function.  When the input runtime
 * system receives a pulse with the specified code, it will call
 * the associated function.  If code is DEVI_PULSE_ALLOC, a pulse
 * code will be allocated for the caller.  func is the function to
 * associate, if it is NULL, then the modules pulse() callback is used.
 *
 * Returns: valid pulse code used to associate the function
 *          -1 on error
 */

int
devi_register_pulse (input_module_t * module, int code,
										 int (*func) (message_context_t *, int, unsigned, void *))
{
	int rval;
	int val;

	if (NULL == dpp)
		return EAGAIN;

	if (!module)
		return EINVAL;

	if (!func) {
		if (!module->pulse)
			return EINVAL;
		else
			func = module->pulse;
	}

	if (code == DEVI_PULSE_ALLOC)
		val = MSG_FLAG_ALLOC_PULSE;
	else
		val = code;

	rval = pulse_attach (dpp, val, code, func, (void *) module);

	if (rval < 0)
		errno_print ("pulse_attach");

	if (verbosity >= 5)
		printf ("devi_register_pulse: assigning code:%d\n", rval);


	return (rval);
}


/* devi_register_interrupt
 *
 * Attach to an interrupt.  Default method of attachment is to
 * use InterruptAttachEvent and send back a pulse when the irq
 * triggers.  This behaviour can be overriden by passing in your
 * own event structure in the 5th parameter, evp.  For example if
 * you wanted to spawn a separate interrupt handling thread and
 * process interrupts within it you could setup the event structure
 * to send back SIGEV_INTR.  If evp is NULL, a pulse will be allocated
 * and the modules pulse() callback will be associated with it.  If the
 * pc argument is not NULL and not equal to DEVI_PULSE_ALLOC, it will
 * be used as the pulse code to associate the pulse() callback.  
 * The flags argument can be set to DEVI_SHARE_RSRC, this will tell
 * Proc's resource database manager to allow this interrupt to be
 * shared. 
 *
 * Returns: a valid interrupt id
 *          -1 on error
 * Note that if the pc parameter was non-NULL, then the allocated pulse
 * code will be returned in it.
 */

int
devi_register_interrupt (int intr, int prio, int *pc, input_module_t * mp,
												 struct sigevent *evp, unsigned flags)
{
	struct sigevent event;
	int rval, code;
	rsrc_request_t res;
	int coid;

	if (!mp)
		return (-1);

	if (pc)
		code = *pc;
	else
		code = DEVI_PULSE_ALLOC;

	if (prio <= 0)
		prio = 15;

	memset (&res, 0, sizeof (res));

	/* request irq via the proc resource manager database */

#ifdef _RSRC_DB_READY_PROC

	res.length = 1;
	res.start = res.end = intr;
	res.flags |= RSRCDBMGR_IRQ | RSRCDBMGR_FLAG_RANGE;
	if (flags & DEVI_SHARE_RSRC)
		res.flags |= RSRCDBMGR_FLAG_SHARE;

	if (rsrcdbmgr_attach (&res, 1) < 0) {
		char *pMsgTxt = "Error: failure in requesting irq %d from system\n";

		if (verbosity)
			fprintf (stderr, pMsgTxt, intr);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, intr);
		return (-1);
	}
#endif

	/* if a sigevent struct was not passed in create one for it */

	if (!evp) {
		memset (&event, 0, sizeof (event));
		if (code == DEVI_PULSE_ALLOC) {
			code = devi_register_pulse (mp, DEVI_PULSE_ALLOC, mp->pulse);
			if (code < 0) {
				char *pMsgTxt = "Error: unable to attach pulse callback handler\n";

				if (verbosity)
					fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
				return (-1);
			}
		}
		coid = devi_get_coid ();
		if (coid < 0)
			return -1;
		SIGEV_PULSE_INIT (&event, coid, prio, code, 0);
		evp = &event;
		if (pc)
			*pc = code;
	}

	rval = InterruptAttachEvent (intr, evp,
															 _NTO_INTR_FLAGS_TRK_MSK |
															 _NTO_INTR_FLAGS_PROCESS);

	if (rval < 0)
		errno_print ("InterruptAttachEvent");

	return (rval);
}

/*
 * devi_register_timer()
 *
 * Create a timer. The caller must call timer_settime() to arm it.
 * The default notification is a pulse when the timer * expires. 
 * This can be overriden by passing a pre-built event in evp.
 *
 * Returns: a valid interrupt id
 *          -1 on error
 * Note that if the pc parameter was non-NULL, then the allocated pulse
 * code will be returned in it.
 */
timer_t
devi_register_timer (input_module_t * mp, int prio, int *pc,
										 struct sigevent * evp)
{
	struct sigevent ev;
	int code;
	int id;
	int coid;

	if (!mp) {
		return -1;
	}

	if (pc) {
		code = *pc;
	} else {
		code = DEVI_PULSE_ALLOC;
	}

	if (prio <= 0) {
		prio = 15;
	}

	if (!evp) {
		memset (&ev, 0, sizeof (ev));
		if (code == DEVI_PULSE_ALLOC) {
			code = devi_register_pulse (mp, code, mp->pulse);
			if (code == -1) {
				char *pMsgTxt = "Error: unable to attach pulse callback handler\n";

				if (verbosity)
					fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
				return (-1);
			}
		}

		coid = devi_get_coid ();
		if (coid < 0)
			return -1;
		SIGEV_PULSE_INIT (&ev, coid, prio, code, 0);

		evp = &ev;
		if (pc) {
			*pc = code;
		}
	}

	if (timer_create (CLOCK_REALTIME, &ev, &id) == -1) {
		errno_print ("timer_create");
		return -1;
	}
	return id;
}




/* self explanatory */

void
clk_get (struct timespec *ts)
{
	clock_gettime (CLOCK_REALTIME, ts);
}




/* devi_request_iorange
 *
 * Used to map in device registers into memory.  flags can
 * be DEVI_SHARE_RSRC to indicate that the io range can be
 * shared.
 *
 * Returns: a pointer to the first location in the mapped in
 *          range
 *          MAP_FAILED on error
 *
 */

uintptr_t
devi_request_iorange (unsigned start, unsigned len, unsigned flags)
{
	rsrc_request_t res;
	uintptr_t ptr;

	memset (&res, 0, sizeof (res));
	res.length = len;
	res.start = start;
	res.end = start + len;
	res.flags |= RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE;
	if (flags & DEVI_SHARE_RSRC)
		res.flags |= RSRCDBMGR_FLAG_SHARE;

#ifdef _RSRC_DB_READY_PROC

	if (rsrcdbmgr_attach (&res, 1) < 0) {
		char *pMsgTxt =
			"Error: failure in requesting iorange (%x, len: %x) from the system\n";
		if (verbosity)
			fprintf (stderr, pMsgTxt, start, len);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, start, len);
		return ((uintptr_t) MAP_FAILED);
	}
#endif

	ptr = mmap_device_io (len, start);
	if (ptr == (uintptr_t) MAP_FAILED) {
		errno_print ("mmap_device_io");
		return ((uintptr_t) MAP_FAILED);
	}

	return (ptr);
}

/* local service function devi_get_coid
 *
 * Supplies connection ID
 * 
 * Returns  connection ID on success
 *         -1 on error
 */

int
devi_get_coid ()
{

	/* create a connection to ourselves if needed */

	if (IpCoid < 0)
		IpCoid = message_connect (dpp, MSG_FLAG_SIDE_CHANNEL);

	return IpCoid;
}


/* Description: this function returns dispatch handle                               */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : current local dispatch handle                                       */
dispatch_t *
devi_get_dispatch_handle ()
{
	return dpp;
}

/* Description: this function initializes input system local dispatch handle        */
/* Input      : dispatch_t * pdp - new dispatch handle                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : Should be called as soon as possible                                */
void
devi_set_dispatch_handle (dispatch_t * pdp)
{
	dpp = pdp;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/lib.c $ $Rev: 657836 $" );
