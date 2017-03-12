/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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








#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

/* photon options */
#define INP_REG_DISABLE_CASB	0x00000001
#define INP_REG_NO_GRAFX        0x00000002

/* general options */
#define OPT_NO_PHOTON           0x0001
#define OPT_RESMGR              0x0002
#define OPT_DISPLAY_RES         0x0004

/* flags for devi lib calls */
#define DEVI_SHARE_RSRC		(1 << 0)
#define DEVI_PULSE_ALLOC	-1

#define MSEC(_t) ((unsigned long)(((_t).tv_sec * 1000) + ((_t).tv_nsec / 1000000)))

/* Mouse types              */
#define NO_WHEEL_MOUSE           0x00
#define WHEEL_3B_MOUSE           0x03
#define WHEEL_5B_MOUSE           0x04

/* serial port config flags */

#define DEVCTL_CFLAG_MASK	(CSIZE | CSTOPB | PARENB | PARODD)
#define PODD			(PARENB | PARODD)
#define PNONE			0
#define PEVEN			PARENB
#define STOP1			0
#define STOP2			CSTOPB

/* global flags mask */
#define FLAGS_GLOBAL		0x00FF

/* absolute (touchscreen) flags */

#define ABS_MAX			16383	/* Calibrated max */
#define ABSOLUTE	   	0x0008	/* Direct coordinates */
#define ABS_SCREEN		0x0009	/* Touch screen */
#define ABS_WORLD		0x000A	/* Digitizing tablet */
#define ABS_TYPE_MASK		0x000F	/* Type mask */
#define ABS_UNCALIBRATED	0x0010	/* Raw coordinates */
#define ABS_PRESS_DATA		0x0020	/* touch screen pressure */
#define ABS_RAW_MODE            0x0040  /* operate in raw mode */
#define ABS_XLATE_MODE          0x0080  /* operate in translation mode */

#define PTR_BUT_SWAP		0x0001  /* swap status of buttons */
#define PTR_X_SWAP		0x0002
#define PTR_Y_SWAP		0x0004
#define PTR_Z_SWAP		0x0008

/* devctls -- */

#define DEVCTL_SETBAUD		0x0010	/* change the baud rate */
#define DEVCTL_RESET		0x0020	/* reset */
#define DEVCTL_LED		0x0030	/* set keyboard leds */
#define DEVCTL_GETKBD		0x0040	/* get keyboard rate and delay */
#define DEVCTL_SETKBD		0x0050	/* set keyboard rate and delay */
#define DEVCTL_RESET_KBD	0x0060	/* reset keyboard device */
#define DEVCTL_RESET_PS2MOUSE	0x0070	/* reset ps2 controller */
#define DEVCTL_ISKBD		0x0080	/* device emitting real scancodes? */
#define DEVCTL_RESCHG		0x0090	/* establish new display resolution */
#define DEVCTL_GETDEVFLAGS	0x00a0	/* get the device flags */
#define DEVCTL_GETKEYF	 	0x00b0	/* get keymap file name */
#define DEVCTL_SETKEYF 		0x00c0	/* set keymap file name */
#define DEVCTL_GETPTRBTNS 	0x00d0	/* get available buttons mask */
#define	DEVCTL_GETPTRCOORD	0x00e0	/* get number of device coordinates */
#define	DEVCTL_GETPTRPRESS	0x00f0	/* get number of device pressures */
#define DEVCTL_GETCOORDRNG	0x0100	/* get the range of a coordinate */
#define DEVCTL_GETPRESSRNG	0x0110	/* get the range of a pressure */
#define DEVCTL_GETDEVTYPE	0x0120	/* get the range of a pressure */
#define DEVCTL_COORDCHG		0x0130	/* change the device coordinates */
#define DEVCTL_BELL		0x0140	/* sound the bell */
#define DEVCTL_GETBELLPARM	0x0150	/* get bell parameters */
#define DEVCTL_SETBELLPARM	0x0160	/* set bell parameters */
#define DEVCTL_GETLED		0x0170	/* get LED status */
#define DEVCTL_SETLED		0x0180	/* set LED status */
#define DEVCTL_GETLEDMASK	0x0190	/* get the LED's present on device */
#define DEVCTL_SETTRIG		0x01c0  /* set trigger level */
#define DEVCTL_GETLOADKBDS  0x01d0  /* get list of loaded keyboards    */
#define DEVCTL_SEL_KBD      0x01d1  /* select keyboard                 */
#define DEVCTL_GET_SEL_KBD  0x01d2  /* get selected keyboard           */
#define DEVCTL_ADD_KBD      0x01d3  /* add new keyboard                */
#define DEVCTL_REMOVE_KBD   0x01d4  /* remove kbd                      */
#define DEVCTL_RELOAD_KBDS  0x01d5  /* reload keyboards from scratch   */
#define DEVCTL_SET_CALIB_MATRIX 0x01d6 /* Set the 3 Point Calibration Matrix */
#define DEVCTL_SET_STATE        0x01d7 /* Set the Device State */
#define DEVCTL_SET_KBD_STATE    0x01d8 /* Set the KBD device State */
#define DEVCTL_SET_REL_STATE    0x01d9 /* Set the REL device State */
#define DEVCTL_SET_ABS_STATE    0x01da /* Set the ABS device State */



/* signalled on lower layers once reset complete */
#define DEVCTL_CHECKPOINT	0x01d0  

#define DEVCTL_SETACCEL		0x01f00	/* change relative filter parms */
#define DEVCTL_GETACCEL		0x02000	

#define DEVCTL_GETSWAP		0x02100
#define DEVCTL_SETSWAP		0x02200

#define DEVCTL_ABS_MODE         0x02400

#define DEVCTL_GETWHEEL		0x02600
#define DEVCTL_SETWHEEL		0x02700

#define DEVCTL_GETWHEEL_ACCEL   0x02800 
#define DEVCTL_SETWHEEL_ACCEL   0x02900 

#define DEVCTL_GETZMODE         0x02a00  /* I mouse has a wheel, how to deal with it ? */
#define DEVCTL_SETZMODE         0x02b00  /* See constants below                        */

#define DEVCTL_GETSAMPLE_RATE   0x02c00  /* Number of samples per second               */ 
#define DEVCTL_SETSAMPLE_RATE   0x02d00

#define DEVCTL_GET_RESOLUTION   0x02e00
#define DEVCTL_SET_RESOLUTION   0x02f00  /* Value sets power of two counts per mm      */


#define DEVCTL_GETMOUSETYPE     0x04000  /* get mouse type: returns in buffer struct devctl_mouse_types; types - see before  */
#define DEVCTL_SETMOUSETYPE     0x04100  /* send in buffer int value (new mouse type); types - see before                    */


#define DEVCTL_RESET_COMPOSESTATE	0x01e0


/* Z-modes (see DEVCTL_SET(GET)ZMODE */
#define ZMODE_KEY_UPDOWN        1   /* Transform z-move to key UP and DOWN; this is a default value  */
#define ZMODE_PAGE_UP_DOWN      2   /* Transform z-move to key PgUP and PgDOWN */
#define ZMODE_SEND_CLIENT_ONLY  3   /* Send z-move information to client only  */



#define scalloc(a)			calloc((a),1)
#define smalloc(a)			malloc(a)
#define srealloc(a,b,c)			realloc(a,c)

#endif









__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/const.h $ $Rev: 657836 $" )
