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








#ifndef __DEVI_H_INCLUDED__
#define __DEVI_H_INCLUDED__

#include "errdbg.h"

#include <arpa/inet.h>
#include <devctl.h>
#include <errno.h>
#include <fcntl.h>
#include <hw/inout.h>
#include <malloc.h>
#include <process.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dcmd_chr.h>
#include <sys/dcmd_input.h>
#include <sys/dcmd_misc.h>
#include <sys/mman.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/procfs.h>
#include <sys/procmgr.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/socket.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/keycodes.h>
#include <sys/keytable.h>

#include <sys/usbcodes.h>

#define MODULE_TABLE_SIZE 32
#define RESMGR_Q_SIZE 		100

#define MAX_SLIDER          16
#define MAX_HAT             16

typedef struct _input_module	input_module_t;
typedef struct _devi_attr 	devi_attr_t;
typedef struct _devi_ocb	devi_ocb_t;

typedef _uint8                  BYTE;

typedef struct Line {

	int				flags;
#define DEVI_LINE_HAVE_DEVICE	(1 << 0)
#define DEVI_LINE_HAVE_PROTO	(1 << 1)
#define DEVI_LINE_HAVE_FILTER	(1 << 2)

	int				type;
/* DEVI_CLASS_* */

	struct Line			*next;
	input_module_t			*bottom; /* device layer */
	input_module_t			*top; /* filter layer (typically) */
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	union {
		struct _keyboard_packet *kq;
		struct _mouse_packet 	*mq;
		struct _touch1_packet	*tq;
		struct _joystick_packet *jq;
		struct _control_packet *cq;
	} u;
	int				elem_size;

	int 				head;
	devi_attr_t			*attr;	

} event_bus_line_t;

#ifndef IOFUNC_ATTR_T
#define IOFUNC_ATTR_T           devi_attr_t
#endif
#ifndef IOFUNC_OCB_T
#define IOFUNC_OCB_T            devi_ocb_t
#endif
#ifndef THREAD_POOL_PARAM_T
#define THREAD_POOL_PARAM_T     dispatch_context_t
#endif

#include <sys/iofunc.h>

struct _devi_ocb {
	
	iofunc_ocb_t		ocb;
	uint32_t		flags;
#define OCB_UNBLOCK_WANTED	(1 << 0)
	int			read_ptr;
};


struct _ocb_list {

	devi_ocb_t		*ocb;
	struct _ocb_list	*prev;
	struct _ocb_list	*next;
};


struct _devi_attr {

  	iofunc_attr_t 		attr;
	int			ocb_size;
	uint32_t		flags;
#define DEVI_NOTIFICATION_ARMED	(1 << 0)
	event_bus_line_t	*line;
	struct _ocb_list	*ocb_list;
	struct _wait_q		*wait_queue;
	iofunc_notify_t		notify[3];
};

#define ARRAY_SIZE(a)    (sizeof(a) / sizeof(a[0]))


#include <sys/resmgr.h>
#include <sys/dispatch.h>


struct _input_module {

	input_module_t			*up;
  	input_module_t			*down;
	struct Line			*line;
	int				flags;
#define MODULE_FLAG_INUSE	(1 << 0)

	int				type;

#define DEVI_CLASS_MASK						0x000000ff
#define DEVI_CLASS_NONE						0x00000001
#define DEVI_CLASS_KBD						0x00000002
#define DEVI_CLASS_REL						0x00000004
#define DEVI_CLASS_ABS						0x00000008
#define DEVI_CLASS_JOYSTICK					0x00000010
#define DEVI_CLASS_GAMEPAD					0x00000020
#define DEVI_CLASS_CONTROL					0x00000040

#define DEVI_MODULE_TYPE_MASK				0x0000ff00
#define DEVI_MODULE_TYPE_FILTER				0x00000100
#define DEVI_MODULE_TYPE_PROTO				0x00000200
#define DEVI_MODULE_TYPE_DEVICE				0x00000400

	char     			name[12], date[12];
	const char	   		*args;
	void				*data;
	int				(*init)(input_module_t *);
	int				(*reset)(input_module_t *);
	int				(*input)(input_module_t *, int, void *);
	int				(*output)(input_module_t *, void *, int);
	int				(*pulse)(message_context_t *, int, unsigned, void *);
	int				(*parm)(input_module_t *, int, char *);
	int				(*devctrl)(input_module_t *, int, void *);
	int				(*shutdown)(input_module_t *, int);
};


/* Common callbacks  */
typedef struct _common_callbacks {
   int nCallbacks;
   int (*pre_init)();
   int (*post_init)();
   int (*pre_shutdown)();
   int (*post_shutdown)();
} common_callbacks_t, * pCommon_callbacks_t;

struct _wait_q {

	devi_ocb_t		*ocb;
	int			rcvid;
	int			nbytes;
	struct _wait_q		*prev;
	struct _wait_q		*next;
};


typedef struct _buffer {
	char				*head;
	char				*tail;
	char				*buff;
	int 				cnt;
	int				bufsize;
	int				recsize;
	pthread_mutex_t 		mutex;
} buffer_t;


struct packet_rel {
	unsigned			flags;
#define	PTR_TYPE_MASK		0x0f00	/* general flags (all pointing devs) */
#define PTR_Z_DATA		0x0100	/* Z coordinate data available */
	int				dx;
	int				dy;
	int				dz;
	unsigned 	        	buttons;
	/* typically _POINTER_BUTTON_RIGHT, _POINTER_BUTTON_MIDDLE or
	 * _POINTER_BUTTON_LEFT
	 */

	struct timespec			timestamp;
};


struct packet_abs {
	unsigned			flags;
/* see const.h, set it to ABSOLUTE|ABS_UNCALIBRATED in the proto module */
	unsigned			x;
	unsigned 			y;
	unsigned 			z;
	unsigned 			buttons; /*  _POINTER_BUTTON_LEFT */
	struct timespec			timestamp;
};

struct packet_kbd {
	struct timespec			timestamp;
        USBKCode                        nUSBCode;
        unsigned char                   bMakeBreak;
/* See /usr/include/sys/dcmd_input.h for the format of this field. */
	struct _keyboard_data	        key; 
};

struct packet_joystick {
	_uint32			x;
	_uint32			y;
	_uint32			z;

	int				Rx;
	int				Ry;
	int				Rz;
	
	uint16_t function_count;             /* Stores the number of sliders in the low bits, and Hats in the high bits */
	//unsigned			*slider[MAX_SLIDER];
	unsigned			slider;
	//unsigned			*hat[MAX_HAT];
	unsigned			hat_switch;
	_uint64			button_state;

	struct timespec		timestamp;
};

struct packet_control {
	_uint64 button_state;

	int16_t Rx;
	int16_t Ry;
	int16_t Rz;

	struct timespec		timestamp;
};

struct devctl_setbaud {
	unsigned			baud;
	tcflag_t			cflag;	/* from termios.h */
};

struct devctl_settrig {
	int		                trigger_level;
};

struct devctl_getkbd {
	short	  		rate;	/* 0 to 31 where 0 is 30Hz */
	short			delay;	/* 0 to 3 where 0 is 250ms, and 3 is 1s */
};

struct devctl_setkbd {
	short			rate;	/* 0 to 31 where 0 is 30Hz */
	short			delay;	/* 0 to 3 where 0 is 250ms, and 3 is 1s */
};

struct devctl_disparea_res {
	long	                xl,xh;
	long	                yl,yh;
};

struct devctl_devcoord {
	int     		swap;
	long	                xl,xh;
	long	                yl,yh;
};

struct devctl_coord_range {
	int	        	which;
	long	                min;
	long	                max;
};

struct devctl_led {
	int	               value;
	int	               mask;
};

struct devctl_bell {
	unsigned short          flags;              /* Flags */
	unsigned short          frequency;          /* In Hz, zero is rest */
	unsigned short          duration;           /* duration in ms */
	unsigned short          volume;             /* USHRT_MAX is full volume */
};

struct devctl_rel_accel {
	int 			gain;
	int 			threshold;
};

struct devctl_wheel_accel {
	int 			step;
	int 			accel;
};

struct devctl_abs_mode {
	int                 mode;
};

struct devctl_mouse_types
{
    unsigned char type ;    /* max level     */
    unsigned char curtype;  /* current level */
};

#define LIST_FIRST_ITEM(head) ((head) -> lh_first)
#define LIST_NEXT_ITEM(elm, field) ((elm == NULL) ? NULL : (elm) -> field.le_next)
#define LIST_PREV_ITEM(elm, field) ((elm == NULL) ? NULL : (elm) -> field.le_prev)

/* Device States */
#define STATE_START             1
#define STATE_STOP              2
#define STATE_RESTART           3
#define STATE_ENABLE            4
#define STATE_DISABLE           5
#define STATE_STANDBY           6

#include "const.h"
#include "externs.h"
#include "proto.h"

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/private/sys/devi.h $ $Rev: 657836 $" )
