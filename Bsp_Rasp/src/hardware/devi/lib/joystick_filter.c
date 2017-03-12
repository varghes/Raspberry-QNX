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
 * joystick.c
 *
 * This is the filter module for all mice type devices.  It allows
 * data passed up from protocol modules to be further manipulated.
 * Data is received from the protocol module in struct packet_joystick's.
 *
 * Data manipulation includes swapping button functionality and setting
 * gain and threshold.
 *
 */
#include <sys/devi.h>
#include <assert.h>
#include <stdio.h>
#include <cfgopen.h>

#define THRESHOLD_POWER		2
#define FLAG_XSWAP		0x0100
#define FLAG_YSWAP		0x0200
#define FLAG_BSWAP		0x0400
#define FLAG_RWHEEL             0x0800


#define MAX_WHEEL_ACCELERATION      (5)	/* Default max_wheel_acceleration                       */
#define MAX_MAX_WHEEL_ACCELERATION (10)	/* Maximum max_wheel_acceleration                       */
#define WHEEL_GAIN                  (1)	/* Mouse wheel acceleration parameter                   */
#define MAX_WHEEL_GAIN              (6)	/* Maximum wheel acceleration parameter                 */
#define CURSOR_SCALE                (1)	/* default "cost" of one unit                           */
#define MAX_GAIN                    (5)	/* max "cost" of one unit                               */
#define CURSOR_THRESHOLD            (5)	/* default value of cursor_threshold                    */
#define MAX_CURSOR_THRESHOLD        (250)	/* Upper limit for cursor_threshold                     */
#define CURSOR_ACCEL_STEP           (5)
#define DOUBLE_CLICK_DELAY          (250)	/* Double click delay in msec                           */

#define TRAP_FILE                   "/etc/system/trap/devi-joystick-opts"

struct data
{
	unsigned flags;
};


static int joystick_init (input_module_t * module);
static int joystick_devctrl (input_module_t * module, int event, void *ptr);
static int joystick_reset (input_module_t * module);
static int joystick_input (input_module_t * module, int num, void *arg);
static int joystick_parm (input_module_t * module, int opt, char *optarg);
static int joystick_shutdown (input_module_t * module, int delay);
//static void calculate_cursor_move (struct data *dp, struct packet_joystick *p);

input_module_t joystick_filter = {

	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_JOYSTICK | DEVI_MODULE_TYPE_FILTER,
	"joy",
	__DATE__,
	"xy",
	NULL,													// initialized by us (in init)
	joystick_init,
	joystick_reset,
	joystick_input,
	NULL,													// output
	NULL,													// pulse not used (this is a pure filter)
	joystick_parm,
	joystick_devctrl,
	joystick_shutdown
};



/* Description: Callback initialisation function; it is called when input module is */
/*              initializing the input system                                       */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Actualy this function does nothing                                  */
int joystick_init (input_module_t * module)
{
	struct data *dp = module->data;

	if (!module->data) {
		if (!(dp = module->data = scalloc (sizeof *dp)))
			return (-1);
	}

	return EOK;
}

/* Description: This is a callback function for DEVCTRL command processing          */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int joystick_devctrl (input_module_t * module, int event, void *ptr)
{
	//struct data *dp = module->data;
	input_module_t *down = module->down;

	switch (event) {

	case DEVCTL_GETDEVFLAGS:
		/* get the flags from the layers below */
		return (down->devctrl) (down, DEVCTL_GETDEVFLAGS, ptr);

	case DEVCTL_GETPTRBTNS:
		/* we're really not concerned with buttons, let 'em through */
		return (down->devctrl) (down, DEVCTL_GETPTRBTNS, ptr);

	case DEVCTL_GETPTRCOORD:
		/* we've already determined number of coordinates at init */
//		memcpy (ptr, &dp->num_coords, sizeof (dp->num_coords));
		break;

	default:
		break;
	}

	return (0);
}


/* Description: this callback funtion is called when the module is linked into the  */
/*              event bus; it is used to set initial module state on the protocol   */
/*              level                                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
int joystick_reset (input_module_t * module)
{
/*
	struct data *dp = module->data;
	input_module_t *down = module->down;
*/
	//(down->devctrl) (down, DEVCTL_GETPTRCOORD, &dp->num_coords);

	return (0);
}


/* Description: main filter   processing function.  It will be called  by the       */
/*              protocol layer to pass its data to process.                         */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int num - number of packets to process                              */
/*              void * arg - data to process                                        */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
int joystick_input (input_module_t * module, int num, void *arg)
{
/*
	struct packet_joystick *p = (struct packet_joystick *) arg;
	struct data *dp = module->data;
	static struct packet_joystick mbuf;
	int valid = 0;
	input_module_t *pkbd;

	if (verbosity >= 8)
		printf ("%s:Received %d packets from %s\n", module->name, num, module->down->name);
*/

/*	for (; num; num--) {
		mbuf.x = p->x;
		mbuf.y = p->y;
		mbuf.z = p->z;
		
		mbuf.Rx = p->Rx;
		mbuf.Ry = p->Ry;
		mbuf.Rz = p->Rz;

		mbuf.hat_switch = p->hat_switch;
		mbuf.slider = p->slider;
		mbuf.button_state = p->button_state;

		if (verbosity > 2)
			fprintf ("Joystick filter data: x = %d, y = %d, z = %d, buttons = %x, Rotate X %d, Rotate Y %d, Rotate Z %d, Slider %d, Hat Switch %d\n", 
							mbuf.x, mbuf.y, mbuf.z, mbuf.button_state, mbuf.Rx, mbuf.Ry, mbuf.Rz, mbuf.slider, mbuf.hat_switch);
		
	}

	if (verbosity >= 8) {
		printf ("%s:Enqueuing packet\n", module->name);
	}
	devi_enqueue_packet (module, (char *) &mbuf, sizeof (mbuf));
*/
	return (0);
}


/* Description: this is a callback function for command line parameter processing   */
/*              (all valid parameters for device module are listed in               */
/*              joystick_filter.args)                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int opt  - parameter code                                           */
/*              char * optarg - optional parameter value                            */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
int joystick_parm (input_module_t * module, int opt, char *optarg)
{
	struct data *dp = module->data;
//	int nVal;

	switch (opt) {

	case 'x':
		dp->flags ^= FLAG_XSWAP;
		break;

	case 'y':
		dp->flags ^= FLAG_YSWAP;
		break;

	default:
		return (-1);
	}

	return (0);
}


/* Description: this is a callback function which is called when resourse manager   */
/*              is shutting down                                                    */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int ms  - program doesn't use this parameter                        */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Does nothing on the mouse filter tier                               */
int joystick_shutdown (input_module_t * module, int delay)
{
	return EOK;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/joystick_filter.c $ $Rev: 657836 $" );
