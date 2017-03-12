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








#ifndef __HID_H_INCLUDED__
#define __HID_H_INCLUDED__

#include <sys/hidut.h>
#include <sys/hiddi.h>

#define MAX_SLIDER                16
#define MAX_HAT                   16

#define HID_MOUSE_HAS_WHEEL       (0x01)
#define HID_MOUSE_WHEEL_ON        (0x02)

/*******************************************************************************
*
* Keyboard Data Structures
*
*******************************************************************************/

typedef struct _hid_keyboard_data {
	_uint16 nLeds;                  /* Number of leds                       */
	_uint16 nKeys;                  /* Size of keyboard array               */
	_uint16 nRate;                  /* Time interval to repeat (in msecs)   */
	_uint16 nDelay;                 /* Delay time interval (in msecs)       */
} hid_keyboard_data_t, *pHid_keyboard_data_t;



/*******************************************************************************
*
* Mouse Data Structures
*
*******************************************************************************/

typedef struct _mouse_data {
	_uint16 nButtons;               /* Number of buttons                    */
	_uint8  flags;                  /* Flags                                */
} mouse_data_t, *pMouse_data_t;

typedef struct _mouse_raw_data {
	_uint8  btnStates;              /* Buttons states (each bit == 1 corresponds to pressed button */

	_int16 x;                       /* pointer x-movement                   */
	_int16 y;                       /* pointer y-movement                   */
	_int16 z;                       /* wheel movement (if wheel exists)     */
} mouse_raw_data_t, *pMouse_raw_data_t;


/*******************************************************************************
*
* Joystick Data Structures
*
*******************************************************************************/

typedef struct _joystick_attrib {
	_uint16 nButtons;               /* Number of buttons                    */
	_uint16 nButtons2;               /* Number of buttons                    */
	_uint8  flags;                  /* Flags                                */
} joystick_attrib_t, *pJoystick_attrib_t;

typedef struct _joystick_data {
//	_uint16 nButtons_1;             /* Number of buttons                                           */
	_uint8  btnStates_1;            /* Buttons states (each bit == 1 corresponds to pressed button */
	_uint64 button_state;
//	_uint16 nButtons_2;             /* Number of buttons                                           */
	_uint8  btnStates_2;            /* Buttons states (each bit == 1 corresponds to pressed button */
	/* Pointer    */
	_uint32 x;
	_uint32 y;
	_uint32 z;
	/* Hat switch */
	_uint32 hat_switch;
	/* Slider   */
	_uint32 slider;
	/* Rotational */
	_uint32 Rx;
	_uint32 Ry;
	_uint32 Rz;
} joystick_raw_data_t, * pJoystick_raw_data_t;


/*******************************************************************************
*
* Touchscreen Data Structures
*
*******************************************************************************/

typedef struct _touch_attrib {
		_uint16 nButtons;               /* Number of buttons                    */
		_uint8  flags;                  /* Flags                                */
} touch_attrib_t, *pTouch_attrib_t;

typedef struct _touch_raw_data {
	_uint8 touched;                 /* 1 if screen yes, otherwise 0         */

	_int16 x;                       /* Absolute x-position                  */
	_int16 y;                       /* Absolute y-position                  */
	_int16 z;                       /* Absolute y-position                  */
} touch_raw_data_t, *pTouch_raw_data_t;


/*******************************************************************************
*
* Consumer Control Data Structures
*
*******************************************************************************/

typedef struct _control_attrib {
		_uint16 nButtons;               /* Number of buttons                    */
		_uint8  flags;                  /* Flags                                */
} control_attrib_t, *pControl_attrib_t;

typedef struct _control_raw_data {
	_uint32 button_state;

	_int16 Rx;
	_int16 Ry;
	_int16 Rz;
} control_raw_data_t, *pControl_raw_data_t;



#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/hid.h $ $Rev: 657836 $" )
