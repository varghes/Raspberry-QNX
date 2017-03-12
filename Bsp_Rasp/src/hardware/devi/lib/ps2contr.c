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




// PS2 controller service functons implementation


#include "externs.h"
#include "ps2api.h"
#include "ps2contr.h"

static int	variable_delay = TRY_LIMIT;


/* Description: function enables/disables devices connected to 8042 contrller       */
/* Input      : pData pd  - pointer to common driver data block                     */
/*              int bEnable - TRUE/FALSE - enable/disable interrupts                */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
toggle_devices (pData pd, int bEnable)
{
	send_device (pd, (bEnable ? KBD_ENABLE : KBD_DISABLE), SEND_CONTR,
							 DO_DELAY);
	send_device (pd, (bEnable ? AUX_ENABLE : AUX_DISABLE), SEND_CONTR,
							 DO_DELAY);
}

/* Description: function toggles on/off interrupt signals from devices connected to */
/* 8042 contrller                                                                   */
/* Input      : pData pd - pointer to common driver data block                      */
/*              int bEnable - TRUE/FALSE - enable/disable interrupts                */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
toggle_interrupt (pData pd, int bEnable)
{
	unsigned char cmd_byte;

	get_command_byte (pd, &cmd_byte);

	if (bEnable)									// Enable interrupts
		cmd_byte |= KBD_INTR_ENABLED | AUX_INTR_ENABLED;
	else
		cmd_byte &= ~(KBD_INTR_ENABLED | AUX_INTR_ENABLED);

	set_command_byte (pd, cmd_byte);
	if (verbosity >= 5)
		printf ("toggle_interrupt %s \n", bEnable ? "On" : "Off");
}

/* Description: function reads controller command byte                              */
/* Input      : pData pd - pointer to common driver data block                      */
/* Output       unsigned char * cmd_byte - where to read data                       */
/* Return     : 0 if OK, otherwise -1                                               */
int
get_command_byte (pData pd, unsigned char *pCmd_byte)
{
	int rc = EOK;

	if (pd->flags & HAVE_CMD_BYTE) {
		*pCmd_byte = pd->cmd_byte;
		return 0;
	}

	toggle_devices (pd, 0);				// Disable
	// This code executed only once. I disable interrupt here
	// because some BIOSes generate interrupt when controller
	// replies to request. In this case keyboard interrupt 
	// handler reading data register (0x60 for standard 8042)
	// reset OUT_BUFF_FULL flag in controller status register 
	// and polling doesn't work properly

	InterruptDisable ();

	if (send_device (pd, GET_CMD_BYTE, SEND_CONTR, NO_DELAY)) {
		if (verbosity)
			fprintf (stderr, "error sending to device\n");
	}

	if (get_device_poll (pd, -1, pCmd_byte))
		rc = -1;
	else
		// Reset disable bits - we never use them
		*pCmd_byte &= ~(KBD_DISABLED | AUX_DISABLED);

	InterruptEnable ();

	toggle_devices (pd, 1);				// Enable 
	if (verbosity >= 5)
		printf ("get_command_byte rc = %i, byte = %#x\n", rc, (int) *pCmd_byte);
	if (0 == rc) {
		pd->cmd_byte = *pCmd_byte;
		pd->flags |= HAVE_CMD_BYTE;
	}
	return (rc);

}

/* Description: function writes new command byte to 8042 status register            */
/* Input      : struct data *dp - pointer to common driver data block               */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
set_command_byte (pData pd, unsigned char c)
{
	/* send out new command byte */
	send_device (pd, SET_CMD_BYTE, SEND_CONTR, DO_DELAY);
	send_device (pd, c, SEND_DEVICE, DO_DELAY);
	if (pd->flags & HAVE_CMD_BYTE)
		pd->cmd_byte = c;
}


/* Description: function reads byte from controller data register                   */
/* Input      : pData pd - pointer to common driver data block                      */
/* Output       unsigned char * cmd_byte - where to read data                       */
/* Return     : 0 if OK, otherwise -1                                               */
int
get_device_poll (pData pd, int nPortNum, unsigned char *c)
{
	int nTries;
	unsigned char status = 0;


//	for (nTries = TRY_LIMIT;
	for (nTries = variable_delay;
		 (nTries > 0)
		 && (pd->f_chdat (nPortNum, status = in8 (pd->nStatPort_id)));
		 nTries--) {
		if (status & OUT_BUFF_FULL) 
			// There is something in register, but not from the desired device - eat unsolicited data
			in8 (pd->nDataPort_id);
		nanospin_ns(pd -> time_delay); 
		if ((status = in8 (pd->nStatPort_id)) & 0x40) {
			slogf (12, 2, "ps2 - Device Timeout (0x%x)", status);
			return (-1);
		}
	}

	if (nTries == 0) {
		if (verbosity >= 5)
			fprintf (stderr,
							 "Polling: no data received. Status byte = %#x, nTries = %i\n",
							 (int) status, (int) TRY_LIMIT);
		return -1;
	}

	*c = in8 (pd->nDataPort_id);

	if (verbosity >= 5)
		printf ("Polling: get %#x\n", (int) *c);
	return 0;

}

/* Description: function gets data package from the specific device                 */
/* Input      : pData *dp - pointer to common driver data block                     */
/*            : int nPortNum - target                                               */
/*              unsigned char * cmds - commands to send                             */
/*              int nLen - buffer length                                            */
/* Return     : 0 if OK, otherwise -1                                               */
/* Comment    : specially for command that expect ACK                               */
int
read_data_sync (pData pd, int nPortNum, unsigned char *buf, int nLen)
{
	int nBytes;

	for (nBytes = 0; nBytes < nLen; ++nBytes) {
		if (get_device_poll (pd, nPortNum, buf)) {
			return -1;
			}
		++buf;
	}
	return 0;
}



/* Description: function sends data to device                                       */
/* Input      : pData *dp - pointer to common driver data block                     */
/*              unsigned char cmd - command to send                                 */
/*              int dest - where to send (see comment below)                        */
/*              int bDelay - if TRUE, insert one i/o cycle delay at the end         */
/* Output     : None                                                                */
/* Return     : None                                                                */
int
send_device (pData pd, unsigned char cmd, int dest, int bDelay)
{
	/*
	 * From doc: "I/O address 0x60 and 0x64 should be written only when the
	 *            Status regiser Input Buffer Full bit *and* the Output 
	 *            Buffer Full bit is 0"
	 */

	if (drain_input (pd))
		return -1;

	if (drain_output (pd))
		return -1;

	if (verbosity >= 5)
		printf ("Sending %#x to %s\n", cmd,
						(SEND_DEVICE == dest) ? "device" : " controller");


	if (dest == SEND_DEVICE)			// Send to device
		out8 (pd->nDataPort_id, cmd);
	else													// Send to controller
		out8 (pd->nStatPort_id, cmd);

	// Just run one i/o cycle to immitate delay for slow controllers
	if (bDelay)
		in8 (pd->nStatPort_id);
	return 0;

}

/* Description: function switch controller to transfer next byte to specified target*/
/* Input      : pData *dp - pointer to common driver data block                     */
/*              int nPortNum - target device                                        */
/* Output     : None                                                                */
/* Return     : 0 if OK, !0 if error                                                */
void
select_target (pData pd, int nPortNum)
{
	if (0 >= nPortNum) {						// Keyboard
		variable_delay = TRY_LIMIT;
		return;
		}

	if (pd->flags & MULTIPLEX_MODE) {
	} else {
		variable_delay = MOUSE_TRY_LIMIT;		
		if (verbosity >= 5)
			printf ("Switch to AUX %i\n", nPortNum);
		send_device (pd, pd->f_getsw (nPortNum), SEND_CONTR, DO_DELAY);
	}

}

/* Description: function drains input status                                        */
/* Input      : pData *dp - pointer to common driver data block                     */
/* Output     : None                                                                */
/* Return     : 0 if OK, !0 if error                                                */
int
drain_input (pData pd)
{
	int nTries;

//	for (nTries = TRY_LIMIT;
	for (nTries = variable_delay;
			 nTries && (in8 (pd->nStatPort_id) & IN_BUFF_FULL); nTries--) {
		nanospin_ns (pd->time_delay);
	}

	if ((0 == nTries) && (verbosity >= 3))
		printf ("drain_input failed\n");
	return (nTries == 0);
}


/* Description: function drains data port                                           */
/* Input      : pData *dp - pointer to common driver data block                     */
/* Output     : None                                                                */
/* Return     : 0 if OK, !0 if error                                                */
int
drain_output (pData pd)
{
	int nTries;

//	for (nTries = TRY_LIMIT;
	for (nTries = variable_delay;
		nTries && (in8 (pd->nStatPort_id) & OUT_BUFF_FULL); nTries--) {
		nanospin_ns (pd->time_delay);
		in8 (pd->nDataPort_id);
	}

	if ((0 == nTries) && (verbosity >= 3))
		printf ("drain_output failed\n");
	return (nTries == 0);

}

/* Description: function initiates keyboard self test                               */
/* Input      : struct data *dp - pointer to common driver data block               */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int
self_test (pData pd)
{
	unsigned char c;

	send_device (pd, SELF_TEST, SEND_CONTR, NO_DELAY);
	if (get_device_poll (pd, -1, &c)) {
		return -1;
		}

	if (c != SELF_TEST_OK)
		return -2;

	return (0);
}




/* Description: function disconnect interrupt handler and do some clean-up          */
/* Input      : struct data *dp - pointer to common driver data block               */
/*              int nIrq_id - irq ID (that was returend by InterruptAttach)         */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
clean_irq (pData pd, int nIrq_id)
{
	int nIrq;

	if (pd->irq_aux_id == nIrq_id) {
		if (pd->irq_base_id == nIrq_id)	// This is a shared IRQ
			nIrq_id = 0;
		pd->irq_aux_id = 0;
		nIrq = pd->irq_aux;
	} else if (pd->irq_base_id == nIrq_id) {
		if (pd->irq_aux_id == nIrq_id)	// This is a shared IRQ
			nIrq_id = 0;
		pd->irq_base_id = 0;
		nIrq = pd->irq_base;
	} else
		return;

	// Disable device if interrupt handler isn't valid any more
	// I keep this code to prevent system locking because now the 
	// default interrupt handler for IRQ12 doesn't work properly
	if (0 == pd->irq_aux_id)
		send_device (pd, AUX_DISABLE, SEND_CONTR, DO_DELAY);

	if (0 < nIrq_id) {
		if (verbosity)
			printf ("Detach interrupt %i(%i)\n", nIrq, nIrq_id);
		if (-1 == InterruptDetach (nIrq_id)) {
			char *pMsgTxt = "Error: cannot detach interrupt handler %i (%s)\n";

			fprintf (stderr, pMsgTxt, nIrq_id, strerror (errno));
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, nIrq_id,
						 strerror (errno));
		}
	}

}



/* Description: this is a 8042 controller interrut handler                          */
/* Input      : void *area - the pointer to the global data area                    */
/*              int id - interrut ID (returned by InterruptAttach)                  */
/* Output     : None                                                                */
/* Return     : const struct sigevent * - pointer to sigevent structure for the     */
/*              corresponding port processing rutine                                */
const struct sigevent *
contr_intr (void *area, int id)
{
	struct sigevent *pEv = NULL;
	pData pd = (pData) (area);
	unsigned char status;
	int nIrqMask = 0;
	int nIrqId;										/* type of IRQ - keyboard or AUX                                   */
	int nPortNum;
	int nBlkNum;


	if (pd->irq_base_id == pd->irq_aux_id)
		nIrqId = NOT_DEF_IRQ_TYPE;
	else if (id == pd->irq_base_id) {
		nIrqMask = pd->irq_aux_id;
		nIrqId = KBD_IRQ_TYPE;
	} else {
		nIrqMask = pd->irq_base_id;
		nIrqId = AUX_IRQ_TYPE;
	}

	if (nIrqMask > 0)
		InterruptMask (nIrqMask, -1);

	status = in8 (pd->nStatPort_id);


	if (!(status & OUT_BUFF_FULL)) {
		if (nIrqMask > 0)
			InterruptUnmask (nIrqMask, -1);
		return NULL;
	}

	switch (nPortNum = pd->f_valid (status, nIrqId)) {
	case RET_DATA_ERROR:
	case RET_NO_DATA:
		in8 (pd->nDataPort_id);			/* Clear interrupt    */
		if (nIrqMask > 0)
			InterruptUnmask (nIrqMask, -1);
		return NULL;
	default:
		break;
	}

	for (nBlkNum = 0; nBlkNum < MAX_EVENTS; ++nBlkNum) {
		if (NO_PULSE_ASSIGNED == pd->ev[nBlkNum].sigev_code)	/* Free block */
			break;

	}

	if (nBlkNum == MAX_EVENTS) {	/* Buffer overflow                        */
		in8 (pd->nDataPort_id);			/* Just reset input port to make it available for receiving of new data */
	} else {
		pEv = pd->ev + nBlkNum;
		pEv->sigev_coid = pd->ncoids[nPortNum];
		pEv->sigev_code = pd->nPulseID[nPortNum];
		pEv->sigev_priority = pd->nPriority[nPortNum];
		pEv->sigev_value.sival_int = nBlkNum;
		pd->inData[nBlkNum] = in8 (pd->nDataPort_id);
	}

	if (nIrqMask > 0)
		InterruptUnmask (nIrqMask, -1);

	return pEv;
}



// Default callback servises for regular 8042 controller

/* Description: functions checks status value to determine if data came             */
/* Input      : int nPortNum - port number                                          */
/*              unsigned char nStatus - value that was read from status port        */
/* Output     : None                                                                */
/* Return     : 0 if there is no data for nPortNum, otherwise !0                    */
int
f_check_data (int nPortNum, unsigned char nStatus)
{
	unsigned char mask, value;

	mask = OUT_BUFF_FULL | AUX_DATA_COME;
	value = OUT_BUFF_FULL;
	if (0 < nPortNum)							// Not keyboard
		value |= AUX_DATA_COME;

	return (value != (nStatus & mask));

}

/* Description: functions returns controller command that can be used to switch     */
/*              output to target port                                               */
/* Input      : int nPortNum - port number                                          */
/* Output     : None                                                                */
/* Return     : controller command                                                  */
int
f_get_switch_cmd (int nPortNum)
{
	return AUX_MAGIC;
}

/* Description: functions checks status byte and if there is no error returns port  */
/*              number from which data came                                         */
/*              output to target port                                               */
/* Input      : int nPortNum - port number                                          */
/* Output     : None                                                                */
/* Return     : port number or negative value if error occured                      */
int
f_validate_data (int nStatus, int nIrqId)
{
	if (NOT_DEF_IRQ_TYPE == nIrqId)	// In case when keyboard and mouse share one and the same interrupt
		nIrqId = ((nStatus & AUX_DATA_COME) ? AUX_IRQ_TYPE : KBD_IRQ_TYPE);

	if (nStatus & PARITY_ERROR)
		return RET_DATA_ERROR;
	else if ((nIrqId == AUX_IRQ_TYPE) && !(nStatus & AUX_DATA_COME))
		return RET_NO_DATA;
	else
		return ((nIrqId == AUX_IRQ_TYPE) ? DEF_AUX_PORT : DEF_KBD_PORT);
}


/* Description: functions modifies controller command byte to enable port           */
/* Input      : int nPortNum - port number                                          */
/*              unsigned char cmd - current command byte value                      */
/* Output     : None                                                                */
/* Return     : new command byte value                                              */
int
f_enable_port_cmd (int nPortNum, unsigned char cmd)
{
	if (0 != nPortNum)						// AUX device
	{
		cmd &= ~AUX_DISABLED;
		cmd |= AUX_INTR_ENABLED;
	} else												// Keyboard
	{
		cmd &= ~KBD_DISABLED;
		cmd |= KBD_INTR_ENABLED;
		cmd |= KBD_TRANS_MODE;
	}

	return cmd;
}

/* Description: functions switchs controller from legasy to multiplex mode          */
/* Input      : int nNewMode                                                        */
/*              unsigned char cmd - current command byte value                      */
/* Output     : None                                                                */
/* Return     : 0 if switch OK, otherwise -1                                        */
int
f_switch_mode (int nNewMode)
{
	return ((nNewMode == SET_LEGACY_MODE) ? 0 : -1);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/ps2contr.c $ $Rev: 657836 $" );
