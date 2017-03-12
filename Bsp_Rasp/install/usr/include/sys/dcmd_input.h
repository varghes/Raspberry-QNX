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







/*
 *  dcmd_input.h
 *
 */

#ifndef __DCMD_INPUT_H_INCLUDED
#define __DCMD_INPUT_H_INCLUDED

#ifndef _DEVCTL_H_INCLUDED
 #include <devctl.h>
#endif

#ifndef __DCMD_MISC_H_INCLUDED
 #include <sys/dcmd_misc.h>
#endif

#ifndef _TIME_H_INCLUDED
 #include <time.h>
#endif

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

__BEGIN_DECLS

#include <_pack64.h>

/*--------------------------------------------------------------------------------*/

#define	KB_OFFSET	0x10	/* offset of kb devctls in the input space */

struct _keyboard_data {
    unsigned long           modifiers;
    unsigned long           flags;
    unsigned long           key_cap;
    unsigned long           key_sym;
    unsigned long           key_scan;
};

struct _keyboard_packet {                   /* start of struct returned from read() */
    struct timespec         time;
    struct _keyboard_data   data;
};

#define _KEYBOARD_MODE_SCAN      0x0000     /* single byte scancodes are returned */
#define _KEYBOARD_MODE_PACKET    0x0001     /* _keyboard_packets are returned */
#define _KEYBOARD_MODE_UNICODE	 0x0002		/* unicode (16 bit quantities) are returned */
#define _KEYBOARD_MODE_MASK      0x0003

struct _keyboard_ctrl {
    struct _interact_device type;           /* Device type */
    unsigned long           flags;          /* Device type flags */
    unsigned long           mode;           /* Mode of currently read packets */
    unsigned short          rate;
    unsigned short          delay;
    char                    mapfile[_POSIX_PATH_MAX];
    unsigned long           zero[9];
};

#define _KEYBOARDGETCTRL        __DIOF(_DCMD_INPUT, KB_OFFSET+0, struct _keyboard_ctrl)
#define _KEYBOARDSETCTRL        __DIOT(_DCMD_INPUT, KB_OFFSET+1, struct _keyboard_ctrl)
#define _KEYBOARDGETKEYMAPPATH  __DIOF(_DCMD_INPUT, KB_OFFSET+2, char[_POSIX_PATH_MAX])
#define _KEYBOARDSETKEYMAPPATH  __DIOT(_DCMD_INPUT, KB_OFFSET+3, char[_POSIX_PATH_MAX])
#define _KEYBOARDRELOADCONFIG   __DIOT(_DCMD_INPUT, KB_OFFSET+4, struct _interact_device)

/*--------------------------------------------------------------------------------*/


#define PTR_OFFSET	0x20

#define _POINTER_FLAG_PROPERTIES    0x000088ff      /* Properties mask */
#define _POINTER_FLAG_ABSOLUTE      0x00000001      /* Absolute co-ordinates (proximity) */
#define _POINTER_FLAG_BUTTON        0x00000002      /* buttons is non-zero */
#define _POINTER_FLAG_COORD         0x00000800      /* num_coord is non-zero */
#define _POINTER_FLAG_PRESSURE      0x00008000      /* num_pressure is non-zero */
#define _POINTER_FLAG_VALUE         (_POINTER_FLAG_COORD|_POINTER_FLAG_PRESSURE)

#define _POINTER_FLAG_CSCALE        0x00000700      /* Unit scale mask (for coords) */
#define _POINTER_FLAG_CUNITY        0x00000000      /* Units are not world related */
#define _POINTER_FLAG_CMETRIC       0x00000100      /* each unit is a number of micrometers (meter / 1000000) */
#define _POINTER_FLAG_CIMPERIAL     0x00000200      /* each unit is a number of microns (inch / 1000000) */
#define _POINTER_FLAG_CSCREEN       0x00000300      /* each unit is relitave to screen */

#define _POINTER_FLAG_PSCALE        0x00007000      /* Unit scale mask (for pressure) */
#define _POINTER_FLAG_PUNITY        0x00000000      /* Units are not world related */

#define _POINTER_FLAG_TOUCH         (_POINTER_FLAG_ABSOLUTE|_POINTER_FLAG_COORD|_POINTER_FLAG_CSCREEN)
#define _POINTER_FLAG_MOUSE         (_POINTER_FLAG_COORD)

#define _POINTER_FLAG_BSWAP			0x00010000
#define _POINTER_FLAG_XSWAP			0x00020000
#define _POINTER_FLAG_YSWAP			0x00040000
#define _POINTER_FLAG_ZSWAP			0x00080000
#define _POINTER_FLAG_SWAP_MASK		0x000f0000
#define _POINTER_FLAG_SWAP_POS		16
 
#define _POINTER_MODE_COMPRESSED    0x0001          /* Data is compressed */

#define _POINTER_BUTTON_1           0x00000001
#define _POINTER_BUTTON_2           0x00000002
#define _POINTER_BUTTON_3           0x00000004
#define _POINTER_BUTTON_4           0x00000008
#define _POINTER_BUTTON_5           0x00000010
#define _POINTER_BUTTON_6           0x00000020
#define _POINTER_BUTTON_7           0x00000040
#define _POINTER_BUTTON_8           0x00000080
#define _POINTER_BUTTON_RIGHT       _POINTER_BUTTON_1
#define _POINTER_BUTTON_MIDDLE      _POINTER_BUTTON_2
#define _POINTER_BUTTON_LEFT        _POINTER_BUTTON_3

#define _POINTER_IOCTL_MAX_POINTS   6       /* Maximum number of data points available with ioctl(), otherwise use qnx_ioctl() */

struct _pointer_packet_hdr {                /* start of struct returned from read() */
    struct timespec         time;
    unsigned long           buttons;        /* Currently depressed buttons */
};

struct _pointer_packet {                    /* returned from read() (variable number of data items) */
    struct _pointer_packet_hdr hdr;
    long                    data[1];        /* variable amount of data (first coord, then pressure) */
};

struct _mouse_packet {                      /* common form for "mouse" pointing devices */
    struct _pointer_packet_hdr hdr;
    long                    dx;             /* num_coord == 2, num_pressure = 0 */
    long                    dy;
    long                    dz;
};

struct _touch1_packet {                     /* common form for "touch" pointing devices (no pressure) */
    struct _pointer_packet_hdr hdr;
    long                    x;              /* num_coord = 2, num_pressure = 0 */
    long                    y;
    long                    z;
};

struct _touch2_packet {                     /* common form for "touch" pointing devices (with pressure) */
    struct _pointer_packet_hdr hdr;
    long                    x;              /* num_coord = 2, num_pressure = 1 */
    long                    y;
    long                    pressure;
};

struct _joystick_packet_hdr {                /* start of struct returned from read() */
	struct timespec         time;
	unsigned long long      button_state;    /* Currently depressed buttons */
};

struct _joystick_packet {
	struct _joystick_packet_hdr hdr;
	long          x;
	long          y;
	long          z;

	short         Rx;
	short         Ry;
	short         Rz;

	unsigned short function_count;             /* Stores the number of sliders in the low bits, and Hats in the high bits */
//unsigned      *slider[MAX_SLIDER];
	unsigned      slider;
//unsigned      *hat[MAX_HAT];
	unsigned      hat_switch;

	struct timespec   timestamp;
};

struct _control_packet_hdr {                /* start of struct returned from read() */
	struct timespec         time;
	unsigned long long      button_state;   /* Currently depressed buttons */
};

struct _control_packet {
	struct _control_packet_hdr hdr;

	short Rx;
	short Ry;
	short Rz;

	struct timespec   timestamp;
};

struct _pointer_info {
    struct _interact_device type;           /* Device type */
    unsigned long           flags;          /* Device type flags (read only) */
    unsigned char           num_coord;      /* Number of coordinats (first range values) */
    unsigned char           num_pressure;   /* Number of pressures (after coord values) */
    unsigned short          mode;           /* Mode of currently read packets */
    unsigned long           buttons;        /* Buttons available bitmap */
    struct _pointer_range {
        long                    min;
        long                    max;
        unsigned long           units;      /* Microns or micrometer per unit (if used) */
    }                       range[_POINTER_IOCTL_MAX_POINTS];
};

struct _pointer_ctrl {
    struct _interact_device type;           /* Device type */
	unsigned				flags;
	short					gain;
	short					threshold;
	int           state;
};

typedef union {
    long                    number;         /* number to perform operations on */
    struct {
        short                   integer;    /* integer component of number */
        unsigned short          frac;       /* fractional component of number */
    }                       component;
} hex_frac_t;

struct _pointer_matrix {
    hex_frac_t              xscale,  xy;
    hex_frac_t              yx,      yscale;
    hex_frac_t              xoffset, yoffset;
};

struct _three_point_calib_matrix {
  long long An, Bn, Cn, Dn, En, Fn, Divider;
};

struct _pointer_transform {
    struct _interact_device type;
    struct _pointer_matrix  xy_coord;       /* Only the first two coordinats are transformed */
    struct _pointer_scale {
        long                    min;
        long                    max;
    }                       pressure[_POINTER_IOCTL_MAX_POINTS];
};

#define _POINTERGETINFO         __DIOF(_DCMD_INPUT, PTR_OFFSET+0, struct _pointer_info)
#define _POINTERSETCTRL         __DIOT(_DCMD_INPUT, PTR_OFFSET+1, struct _pointer_ctrl)
#define _POINTERGETMATRIX       __DIOF(_DCMD_INPUT, PTR_OFFSET+2, struct _pointer_matrix)
#define _POINTERSETMATRIX       __DIOT(_DCMD_INPUT, PTR_OFFSET+3, struct _pointer_matrix)
#define _POINTERGETCTRL         __DIOF(_DCMD_INPUT, PTR_OFFSET+4, struct _pointer_ctrl)

/*--------------------------------------------------------------------------------*/

#define FB_OFFSET	0x30

#define _FEEDBACK_LED_1             0x00000001
#define _FEEDBACK_LED_2             0x00000002
#define _FEEDBACK_LED_3             0x00000004
#define _FEEDBACK_LED_4             0x00000008
#define _FEEDBACK_LED_SCROLL        _FEEDBACK_LED_1
#define _FEEDBACK_LED_NUM           _FEEDBACK_LED_2
#define _FEEDBACK_LED_CAPS          _FEEDBACK_LED_3

struct _feedback_led {
    struct _interact_device type;
	int	value;
	int	mask;
};

struct _feedback_bell {
    struct _interact_device type;
    unsigned short          flags;              /* Flags */
    unsigned short          frequency;          /* In Hz, zero is rest */
    unsigned short          duration;           /* duration in ms */
    unsigned short          volume;             /* USHRT_MAX is full volume */
};

#define	_FEEDBACK_BELL_FLAG_FREQ		0x0004
#define	_FEEDBACK_BELL_FLAG_DURATION	0x0002
#define	_FEEDBACK_BELL_FLAG_VOLUME		0x0001

#define _FEEDBACK_BELL_FLAG_FLUSH  		0x0001      /* Flush pending bells */
#define _FEEDBACK_BELL_VOLUME_DFLT		(USHRT_MAX/2)

#define _FEEDBACK_STRING_MAX        255

#define _FEEDBACKSETLED         __DIOT(_DCMD_INPUT, FB_OFFSET+1, struct _feedback_led)
#define _FEEDBACKGETLED         __DIOF(_DCMD_INPUT, FB_OFFSET+2, struct _feedback_led)
#define _FEEDBACKGETLEDMASK		__DIOF(_DCMD_INPUT, FB_OFFSET+3, struct _feedback_led)
#define _FEEDBACKBELL           __DIOT(_DCMD_INPUT, FB_OFFSET+5, struct _feedback_bell)
#define _FEEDBACKINTEGER        __DIOF(_DCMD_INPUT, FB_OFFSET+0xA, long)
#define _FEEDBACKSTRING         __DIOT(_DCMD_INPUT, FB_OFFSET+0xF, char[_FEEDBACK_STRING_MAX])

/*--------------------------------------------------------------------------------*/

#define DEVICE_OFFSET 0x40  /* offset of device devctls in the input space */

struct _device_state {
	struct _interact_device type;
	int state;
};

#define _SETDEVICESTATE       __DIOT(_DCMD_INPUT, DEVICE_OFFSET+0, struct _device_state)

#include <_packpop.h>

__END_DECLS

#endif

#ifdef __QNXNTO__
#include <sys/srcversion.h>
__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/public/sys/dcmd_input.h $ $Rev: 657836 $" )
#endif
