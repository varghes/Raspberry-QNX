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




// PS2 controller API specification

#ifndef _PS2CONTR_H
#define _PS2CONTR_H

#include <stdlib.h>
#include <stdio.h>

#define START_PULSE_CODE         (100)
#define PROCES_PRTY              (15)
#define PROMOTE_PRTY             (5)

//#define TRY_LIMIT                (1000000)
//#define TIME_DELAY               (10000)
//#define MAX_RESEND               (100)
#define TRY_LIMIT                (10000)
#define	MOUSE_TRY_LIMIT			 (100000)
#define TIME_DELAY               (10000)
#define MAX_RESEND               (10)

#define INTR_ENABLE              (1)
#define INTR_DISABLE             (0)

// Command targets
#define SEND_DEVICE              (1)
#define SEND_CONTR               (2)

// Delay after OUT ?
#define DO_DELAY                 (1)
#define NO_DELAY                 (0)

// Resend

// #define RESEND_DELAY             (5)   // Delay if device requests resend

// Typical PS2 device and controller replies
#define ACK                      (0xfa)	/* ack code                                */
#define RESEND                   (0xfe)	/* resend                                  */
#define SELF_TEST_OK             (0x55)	/* self test OK                            */


// Status register bits
#define OUT_BUFF_FULL            (0x01)	/* output register is full                 */
#define IN_BUFF_FULL             (0x02)	/* controller is ready to accept new byte  */
#define INOUT_BUFF_FULL          (0x03)	/* combination of two previous flags       */
#define AUX_DATA_COME            (0x20)	/* data comes from AUX device              */
#define PARITY_ERROR             (0x80)	/* parity error                            */

// Controller command byte bits
#define KBD_INTR_ENABLED         (0x01)	/* Enable interrupt fro the keyboard       */
#define AUX_INTR_ENABLED         (0x02)	/* Enable interrupt fro the mouse          */
#define KBD_DISABLED             (0x10)	/* Keybord interface disabled              */
#define AUX_DISABLED             (0x20)	/* AUX interface disabled                  */
#define KBD_TRANS_MODE           (0x40)	/* Keyboard is in translate mode           */


// Controller commands
#define AUX_MAGIC                (0xd4)	/* tells kbd ctrler next byte for aux      */
#define KBD_ENABLE               (0xae)	/* enable keyboard                         */
#define KBD_DISABLE              (0xad)	/* disable keyboard                        */
#define AUX_ENABLE               (0xa8)	/* enable the aux device(s)                */
#define AUX_DISABLE              (0xa7)	/* disable the aux device(s)               */
#define SELF_TEST                (0xaa)	/* controller self test                    */
#define GET_CMD_BYTE             (0x20)	/* get the controller command byte         */
#define SET_CMD_BYTE             (0x60)	/* write the controller command byte       */


#define DATA_DEF_PORT            (0x60)	/* controller default ioport               */
#define STATUS_DEF_PORT          (0x64)	/* contrller default status port           */


#define KBD_DEF_IRQ              (1)	/* default keyboard irq                    */
#define AUX_DEF_IRQ              (12)	/* default aux irq                         */

#define MAX_DEVICES              (10)
#define MAX_EVENTS               (100)

#define NO_PULSE_ASSIGNED        (-1)

// For 8042
#define DEF_KBD_PORT             (0)
#define DEF_AUX_PORT             (1)

typedef struct _data
{
#define INIT_DONE             (0x01)
#define MULTIPLEX_MODE        (0x02)
#define DO_SELF_TEST          (0x04)
#define HAVE_CMD_BYTE         (0x08)
	int flags;										/* interrupt handler local flags                     */
	int irq_base;									/* keyboard IRQ                                      */
	int irq_base_id;							/* keyboard IRQ id (returned by InterruptAttach)     */
	int irq_aux;									/* auxiliary IRQ                                     */
	int irq_aux_id;								/* auxiliary IRQ id (returned by InterruptAttach)    */
	int time_delay;								/* atomic timeout for replies                        */
	int cmd_byte;									/* command byte                                      */
	int nPorts;										/* number of available ports                         */
	int nDataPort;								/* data i/o port number                              */
	int nStatPort;								/* status i/o port number                            */
	int nDataPort_id;							/* mapped data i/o port number                       */
	int nStatPort_id;							/* mapped status i/o port number                     */
	check_data *f_chdat;					/* callback function to check if data comes          */
	get_switch_cmd *f_getsw;			/* callback function to get controller switch command */
	validate_data *f_valid;				/* callback function that validates data and         */
	/* determines logical port number                    */
	enable_port_cmd *f_encmd;			/* callback function that returns port enable command */
	/* byte                                              */
	switch_mode *f_swmode;				/* callback function that returns port switch command */
	/* byte                                              */
	dispatch_t *dpp;
	int nConnID;									/* connection ID                                     */
	int ncoids[MAX_DEVICES];			/* connection ID                                     */
	int nPulseID[MAX_DEVICES];		/* Pulse IDs (from pulse_attach)                     */
	int nPriority[MAX_DEVICES];		/* Pulse IDs (from pulse_attach)                     */

	struct sigevent ev[MAX_EVENTS];	/* Events pool                                   */
	unsigned char inData[MAX_EVENTS];	/* Data exchange buffer (between Interrupt handler */
	/* and pulse processing program                */
}
data, *pData;


// This code makes sence for Solution Engine only
#ifdef VARIANT_sengine

static inline uint8_t
sengine_in8 (uintptr_t addr)
{
	return in16 (addr) >> 8;
}

static inline void
sengine_out8 (uintptr_t addr, uint8_t val)
{
	return out16 (addr, val << 8);
}

#define in8 sengine_in8
#define out8 sengine_out8

#endif



// Local service functions   
int drain_input (pData pd);
int drain_output (pData pd);
void toggle_devices (pData pd, int bEnable);
void toggle_interrupt (pData pd, int bEnable);
int get_command_byte (pData pd, unsigned char *cmd_byte);
int get_device_poll (pData pd, int nPortNum, unsigned char *c);
int read_data_sync (pData pd, int nPortNum, unsigned char *buf, int nLen);
void set_command_byte (pData pd, unsigned char c);
int send_device (pData pd, unsigned char cmd, int dest, int bDelay);
void select_target (pData pd, int nPortNum);
int drain_input (pData pd);
int drain_output (pData pd);
int self_test (pData pd);
void clean_irq (pData pd, int nIrq_id);

// Default callbacks
int f_check_data (int nPortNum, unsigned char nStatus);
int f_get_switch_cmd (int nPortNum);

int f_validate_data (int nStatus, int nIrqId);

int f_enable_port_cmd (int nPortNum, unsigned char cmd);
int f_switch_mode (int nNewMode);


// Interrupt handler prototype   
const struct sigevent *contr_intr (void *area, int id);


#endif // _PS2CONTR_H

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/ps2contr.h $ $Rev: 657836 $" )
