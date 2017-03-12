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




// PS2 controller API implementation

#include <pthread.h>
#include "assert.h"
#include "externs.h"
#include "ps2api.h"
#include "ps2contr.h"

// Global data   

static volatile data glbData = {
	0,
	KBD_DEF_IRQ,
	-1,
	AUX_DEF_IRQ,
	-1,
	TIME_DELAY,
	0,
	2,														// Number of ports
	DATA_DEF_PORT,
	STATUS_DEF_PORT,
	-1,
	-1,
	f_check_data,
	f_get_switch_cmd,
	f_validate_data,
	f_enable_port_cmd,
	f_switch_mode
		// ev is not initialized here
};

static pthread_mutex_t m_cmd;		/* Use it to queue device commands                   */

/* Description: function registers logical port                                     */
/* Input      : dispatch_t * dp -- dispatch handle (if caller supplies              */
/*              DEFAULT_DISPHANDLE, the  program takes default dispatch handle      */
/*              calling devi_get_dispatch_handle function                           */
/*              pulse - callback function that would process input pulse            */
/*              int coid - connection ID (if caller supplies DEFAULT_COID,          */
/*              the program takes default coid calling devi_get_coid function       */
/*              int nPortNum - number of port to activate; 0 is reserved for the    */
/*              keyboard                                                            */
/*              void * data  - data that will be transferred to pulse function      */
/* Output     : None                                                                */
/* Return     : 0 if OK, othervise negative error code                              */
int
register_device (dispatch_t * dp,
								 int (*pulse) (message_context_t *, int, unsigned, void *),
								 int coid, int nPortNum, void *data)
{
	int nCoid = 0;

	if (DEFAULT_DISPHANDLE == dp)
		dp = devi_get_dispatch_handle ();


	if (NULL == pulse && !coid)
		return -1;

	glbData.dpp = dp;

	if (nPortNum < 0 || nPortNum >= glbData.nPorts) {
		if (verbosity >= 3)
			printf ("Port %i cannot be registered: wrong port number\n", nPortNum);
		return -2;									/* wrong port number                                               */
	}

	if (NO_PULSE_ASSIGNED != glbData.nPulseID[nPortNum]) {
		if (verbosity >= 3)
			printf ("Port %i has been already registered\n", nPortNum);
		return -3;									/* this port has been already registered                           */
	}

	if (coid == DEFAULT_COID) {
		nCoid = pulse_attach (glbData.dpp, MSG_FLAG_ALLOC_PULSE, 0, pulse, data);
		if (nCoid < 0)
			return -4;								/* error pulse registration                                        */

	}
	// It is very important to call devi-get_coid after pulse_attach because
	// pulse_attach implicitly initialize input channel (the work that resmgr_attach
	// usually does, but it happens only when driver is launched without Photon
	// interface); otherwise devi_get_coid fails
	if (DEFAULT_COID == coid)
		glbData.ncoids[nPortNum] = devi_get_coid ();
	else
		glbData.ncoids[nPortNum] = coid;


	if (verbosity >= 3)
		printf ("Pulse assigned %i\n", nCoid);

	if (!pulse)
		glbData.nPulseID[nPortNum] = *((int *) data);	// hack
	else
		glbData.nPulseID[nPortNum] = nCoid;


	// Attach interrupt handler if it has not been attached yet
	// Note: Two IRQs (base & aux) share one and the same interrupt handler
	if ((0 == nPortNum) && (0 == glbData.irq_base_id)) {
		int irq_base_id;

		if ((glbData.irq_base == glbData.irq_aux) && (glbData.irq_aux_id > 0))
			irq_base_id = glbData.irq_aux_id;
		else {
			irq_base_id = InterruptAttach (glbData.irq_base,
																		 contr_intr,
																		 (void *) &glbData, sizeof (glbData),
//            _NTO_INTR_FLAGS_END     |
																		 _NTO_INTR_FLAGS_PROCESS |
																		 _NTO_INTR_FLAGS_TRK_MSK);
			if (verbosity >= 1)
				printf ("Attach kbd interrupt handler %i (%i)\n",
								glbData.irq_base, irq_base_id);
		}
		if (irq_base_id < 0) {
			char *pMsgTxt = "Error: could not attach KBD interrput\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			return -5;
		} else {
			if (verbosity >= 3)
				printf ("KBD interrput attached\n");
		}
		glbData.irq_base_id = irq_base_id;
	} else if ((0 < nPortNum) && (0 == glbData.irq_aux_id)) {
		int irq_aux_id;

		if ((glbData.irq_base == glbData.irq_aux) && (glbData.irq_base_id > 0))
			irq_aux_id = glbData.irq_base_id;
		else {
			irq_aux_id = InterruptAttach (glbData.irq_aux,
																		contr_intr,
																		(void *) &glbData, sizeof (glbData),
//            _NTO_INTR_FLAGS_END     |
																		_NTO_INTR_FLAGS_PROCESS |
																		_NTO_INTR_FLAGS_TRK_MSK);
			if (verbosity >= 1)
				printf ("Attach AUX interrupt handler %i (%i)\n",
								glbData.irq_aux, irq_aux_id);
		}

		if (irq_aux_id < 0) {
			char *pMsgTxt = "Error: could not attach AUX interrput\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			return -5;
		} else {
			if (verbosity >= 3)
				printf ("AUX interrput attached\n");
		}

		glbData.irq_aux_id = irq_aux_id;
	}

	return 0;

}


/* Description: function closes logical port                                        */
/* Input      : int nPortNum - number of logical port to be closed                  */
/*              keyboard                                                            */
/* Output     : None                                                                */
/* Return     : 0 if OK, othervise negative error code                              */
int
unregister_device (int nPortNum)
{
	int i;
	int nIrq_id = -1;

	if (nPortNum < 0 || nPortNum >= glbData.nPorts)
		return -2;									/* wrong port number                                               */

	if (NO_PULSE_ASSIGNED == glbData.nPulseID[nPortNum])
		return -3;									/* this port has not  been  registered                             */

	pulse_detach (glbData.dpp, glbData.nPulseID[nPortNum], 0);
	glbData.nPulseID[nPortNum] = NO_PULSE_ASSIGNED;

	// Detach interrupt if there are no more devices on it
	if (0 == nPortNum)						// Keyboard
		nIrq_id = glbData.irq_base_id;
	else {
		for (i = 1; i < glbData.nPorts; ++i) {
			if (NO_PULSE_ASSIGNED != glbData.nPulseID[nPortNum])
				break;
		}
		if (glbData.nPorts == i)		// There are no more AUX devices
			nIrq_id = glbData.irq_aux_id;
	}

	if (nIrq_id > 0)
		clean_irq ((pData) & glbData, nIrq_id);

	return 0;

}


/* Description: function resets logical port                                        */
/* Input      : int nPortNum - number of port to reset                              */
/* Output     : None                                                                */
/* Return     : 0 if OK, othervise negative error code                              */
int
reset_device (int nPortNum)
{
	unsigned char cmd_byte;

	// Cannot reset if controller in legacy mode
	if ((nPortNum > 1) && (0 == (glbData.flags & MULTIPLEX_MODE)))
		return -1;

	if (drain_input ((pData) & glbData))
		return -2;

	if (get_command_byte ((pData) & glbData, &cmd_byte)) {
		if (verbosity)
			fprintf (stderr, "Cannot get command byte\n");
		return -2;
	}
	cmd_byte = glbData.f_encmd (nPortNum, cmd_byte);

	set_command_byte ((pData) & glbData, cmd_byte);

	return 0;
}

/* Description: change scheduling priority for this port processing                 */
/* Input      : int nPortNum - target                                               */
/*              int nNewPrty - new priority (if  0 - set default )                  */
/* Output     : None                                                                */
/* Return     : Old priority for this port                                          */
int
set_priority (int nPortNum, int nNewPrty)
{
	int nOldPrty = glbData.nPriority[nPortNum];
	int nMaxPrty = sched_get_priority_max (sched_getscheduler (0));

	if (0 < nNewPrty)
		glbData.nPriority[nPortNum] = min (nNewPrty, nMaxPrty);
	else
		glbData.nPriority[nPortNum] = min ((getprio (0) + PROMOTE_PRTY),
																			 nMaxPrty);
	return nOldPrty;
}

/* Description: function synchronically sends command to target device but not      */
/*              expects ACK (useful for RESET and RESET_WRAP_MODE                   */
/* Input      : int nPortNum - target                                               */
/*              unsigned char cmd - command to send                                 */
/*              int nReplyLen - the length of expectable reply                      */
/* Output     : unsigned char * pReply - buffer to accept reply from device         */
/*              (can be NULL if nReplyLen == 0                                      */
/* Return     : 0 if OK, otherwise -1                                               */
int
send_device_command (int nPortNum, unsigned char cmd,
										 unsigned char *pReply, int nReplyLen)
{
	pData pd = (pData) & glbData;
	int bDoEnable = 0;
	unsigned char cmd_byte;
	int rc = EOK;

	pthread_mutex_lock (&m_cmd);

	if (get_command_byte (pd, &cmd_byte))
		rc = -1;
	else {
		if (cmd_byte & (KBD_INTR_ENABLED | AUX_INTR_ENABLED)) {
			bDoEnable = 1;
			toggle_interrupt (pd, INTR_DISABLE);
		}

		select_target (pd, nPortNum);
		send_device (pd, cmd, SEND_DEVICE, NO_DELAY);

		if ((nReplyLen > 0) && (NULL != pReply))
			rc = read_data_sync ((pData) & glbData, nPortNum, pReply, nReplyLen);
		if (bDoEnable)
			toggle_interrupt (pd, INTR_ENABLE);
	}

	pthread_mutex_unlock (&m_cmd);

	return rc;
}


/* Description: function synchronically sends command to target device              */
/* Input      : int nPortNum - target                                               */
/*              unsigned char cmd - command to send                                 */
/*              int nReplyLen - the length of expectable reply                      */
/* Output     : unsigned char * pReply - buffer to accept reply from device         */
/*              (can be NULL if nReplyLen == 0                                      */
/* Return     : 0 if OK, otherwise -1                                               */
int
send_command (int nPortNum, unsigned char cmd,
							unsigned char *pReply, int nReplyLen)
{
	return send_command_set (nPortNum, &cmd, 1, pReply, nReplyLen);
}

/* Description: function synchronically sends set of commands to target device      */
/* Input      : int nPortNum - target                                               */
/*              unsigned char * pCmds - commands to send                            */
/*              int nCmdsLen - command sequence length                              */
/*              int nReplyLen - the length of expectable reply                      */
/* Output     : unsigned char * pReply - buffer to accept reply from device         */
/*              (can be NULL if nReplyLen == 0                                      */
/* Return     : 0 if OK, otherwise -1                                               */
/* Comment    : specially for command that expect ACK                               */
int
send_command_set (int nPortNum, unsigned char *pCmds, int nCmdsLen,
									unsigned char *pReply, int nReplyLen)
{

	int rc = 0;
	int idCount;
	int nOfResends = 0;
	unsigned char cmd_byte;
	unsigned char data_byte;
	int bDoEnable = 0;
	pData pd = (pData) & glbData;

	pthread_mutex_lock (&m_cmd);

	if (get_command_byte (pd, &cmd_byte))
		rc = -1;
	else {
		if (cmd_byte & (KBD_INTR_ENABLED | AUX_INTR_ENABLED)) {
			bDoEnable = 1;
			toggle_interrupt (pd, INTR_DISABLE);
		}


		for (idCount = 0; idCount < nCmdsLen; ++idCount) {
			select_target (pd, nPortNum);
			send_device (pd, pCmds[idCount], SEND_DEVICE, NO_DELAY);
			/* and wait for ACK */
			if (get_device_poll (pd, nPortNum, &data_byte)) {
				if (verbosity >= 3)
					fprintf (stderr, "No ACK!\n");
				rc = -1;
				break;
			}
			if (data_byte == ACK)			/* ACK */
				nOfResends = 0;
			else if (data_byte == RESEND) {	/* RESEND request */
//  delay(RESEND_DELAY);
				if (++nOfResends > MAX_RESEND) {
					if (verbosity >= 3) {
						fprintf (stderr, "Too many resends (%#x); stop i/o operation\n",
										 (int) (pCmds[idCount]));
						fprintf (stderr, "The failed command sequence is: \n");
						for (idCount = 0; idCount < nCmdsLen; ++idCount)
							fprintf (stderr, "\t%#02x ", (int) pCmds[idCount]);
						fprintf (stderr, "\n");
					}
					rc = -3;
					break;
				}
				--idCount;							/* Repeat last command  */
			} else {
				if (verbosity >= 3)
					fprintf (stderr, "No ACK! Received %#x\n", (int) data_byte);
				rc = -2;
				break;
			}
		}


		if ((0 == rc) && (nReplyLen > 0) && (NULL != pReply))
			rc = read_data_sync ((pData) & glbData, nPortNum, pReply, nReplyLen);
		if (bDoEnable)
			toggle_interrupt (pd, INTR_ENABLE);
	}

	pthread_mutex_unlock (&m_cmd);

	return rc;
}

/* Description: function initializes PS2 API system                                 */
/* Input      : int flags - flags; now valid values - DO_SELF_TEST & MULTIPLEX_MODE */
/*              int nIrq_base - valid keyboard IRQ or SET_DEFAULT or SET_NONE       */
/*              int nIrq_aux - valid AUX IRQ or SET_DEFAULT or SET_NONE             */
/*              int nDataPort - basic PS2 controller data port or SET_DEFAULT       */
/*              int nStatPort - status PS2 controller port or SET_DEFAULT           */
/* Return     : 0 if OK, otherwise negative error code                              */
int
init_contr (int flags, int nIrq_base, int nIrq_aux, int nDataPort,
						int nStatusPort)
{


	if ((glbData.flags & INIT_DONE) == 0) {
		int i;
		int nThreadPrty;
		pthread_mutexattr_t mattr;

		pthread_mutexattr_init (&mattr);
		pthread_mutex_init (&m_cmd, &mattr);
		pthread_mutexattr_destroy (&mattr);

		memset ((void *) glbData.ev, 0, sizeof (glbData.ev));


		/* map in device memory */
		glbData.nDataPort =
			((SET_DEFAULT != nDataPort) ? nDataPort : DATA_DEF_PORT);
		glbData.nStatPort =
			((SET_DEFAULT != nIrq_aux) ? nStatusPort : STATUS_DEF_PORT);

		/* get i/o resources                                                */
		glbData.nDataPort_id =
			devi_request_iorange (glbData.nDataPort, 1, DEVI_SHARE_RSRC);
		if (verbosity >= 3)
			printf ("Data port registered (%#x)\n", glbData.nDataPort_id);
		if (glbData.nDataPort_id == (uintptr_t) MAP_FAILED)
			return (-1);
		glbData.nStatPort_id =
			devi_request_iorange (glbData.nStatPort, 1, DEVI_SHARE_RSRC);
		if (verbosity >= 3)
			printf ("Status port registered (%#x)\n", glbData.nStatPort_id);
		if (glbData.nStatPort_id == (uintptr_t) MAP_FAILED)
			return (-1);

		/*
		 * Some controllers need to be initialized before they can 
		 * start functioning.  Telling the controller to start a 
		 * self-test usually gets the ball rolling.  Usually this is 
		 * done by the BIOS but on non-bios machines we gotta do this 
		 * ourselves.  FLAG_RESET_KB is turned on via the -r option.
		 */

		if (flags & INIT_SELF_TEST) {
			if (self_test ((pData) & glbData) < 0) {
				char *pMsgTxt = "Failure to execute PS2 controller self test\n";

				fprintf (stderr, pMsgTxt);
				slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_WARNING, pMsgTxt);
				return (-2);
			}
			glbData.flags |= DO_SELF_TEST;
		}


		if (flags & INIT_MULTIPLEX_MODE) {
			// Try to switch controller into multiplex mode
			if (switch_controller_mode (SET_MULTIPLEX_MODE))
				return -3;							// Controller does't support multiplexing
		} else
			glbData.nPorts = 2;

		/* initialize device pulse IDs                                                */
		nThreadPrty = min ((getprio (0) + PROMOTE_PRTY),
											 sched_get_priority_max (sched_getscheduler (0)));
		for (i = 0; i < glbData.nPorts; ++i) {
			glbData.nPulseID[i] = NO_PULSE_ASSIGNED;
			glbData.nPriority[i] = nThreadPrty;
		}
		/* Initialize event blocks pool                                               */

		for (i = 0; i < MAX_EVENTS; ++i) {
			SIGEV_PULSE_INIT (&glbData.ev[i], 0, PROCES_PRTY, NO_PULSE_ASSIGNED, 0);
		}

		ThreadCtl (_NTO_TCTL_IO, 0);

		glbData.time_delay = TIME_DELAY;

		glbData.irq_base_id = glbData.irq_aux_id = 0;

		glbData.flags |= INIT_DONE;
	}

	if (nIrq_base == SET_DEFAULT)
		glbData.irq_base = KBD_DEF_IRQ;
	else if (nIrq_base != SET_NONE)
		glbData.irq_base = nIrq_base;

	if (nIrq_aux == SET_DEFAULT)
		glbData.irq_aux = KBD_DEF_IRQ;
	else if (nIrq_aux != SET_NONE)
		glbData.irq_aux = nIrq_aux;

	return (0);

}

/* Description: call this function if you want to replace some of default callbacks */
/* Input      : f_chdat, f_getsw, f_valid, f_encmd, f_swmode - callback functions   */
/*              (you can find their prototypes in  ps2api.h)                        */
/* Return     : None                                                                */
void
set_callbacks (check_data * f_chdat, get_switch_cmd * f_getsw,
							 validate_data * f_valid, enable_port_cmd * f_encmd,
							 switch_mode * f_swmode)
{

	if (NULL != f_chdat)
		glbData.f_chdat = f_chdat;
	if (NULL != f_getsw)
		glbData.f_getsw = f_getsw;
	if (NULL != f_valid)
		glbData.f_valid = f_valid;
	if (NULL != f_encmd)
		glbData.f_encmd = f_encmd;
	if (NULL != f_swmode)
		glbData.f_swmode = f_swmode;

}


/* Description: function switch controller to and from multiplexing mode            */
/*              (if controller supports it)                                         */
/* Input      : int nNewMode - SET_LEGACY_MODE or SET_MULTIPLEX_MODE                */
/* Return     : 0 if OK, otherwise -1                                               */
int
switch_controller_mode (int nNewMode)
{
	int rc = 0;

	if (SET_LEGACY_MODE && (glbData.flags & MULTIPLEX_MODE)) {
		if (0 == glbData.f_swmode (nNewMode))
			glbData.flags &= ~MULTIPLEX_MODE;
		else
			rc = -1;
	} else if (SET_MULTIPLEX_MODE && (0 == (glbData.flags & MULTIPLEX_MODE))) {
		if (0 == glbData.f_swmode (nNewMode))
			glbData.flags |= MULTIPLEX_MODE;
		else
			rc = -1;
	}

	return rc;
}

/* Description: function is called to clean-up controller state                     */
/*              (it is not nescessary if all devices has to be closed )             */
/* Input      : None                                                                */
/* Return     : None                                                                */
void
shutdown_contr ()
{
	if (glbData.flags & INIT_DONE) {
		clean_irq ((pData) & glbData, glbData.irq_base_id);
		clean_irq ((pData) & glbData, glbData.irq_aux_id);
		glbData.flags &= ~INIT_DONE;
	}
}

/* Description: function returns  data that comes from interrupt handler            */
/* Input      : message_context_t *ctp - pointer to module descriptor               */
/* Return     : None                                                                */
unsigned char
get_in_data (int blk)
{
	unsigned char c;
	int numBlk = blk;

	assert (numBlk >= 0 && numBlk < MAX_EVENTS);
	// Get data
	c = glbData.inData[numBlk];
	// And free event block
	glbData.ev[numBlk].sigev_code = NO_PULSE_ASSIGNED;
	return c;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/ps2api.c $ $Rev: 657836 $" );
