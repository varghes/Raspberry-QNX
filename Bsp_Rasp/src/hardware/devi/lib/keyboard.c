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
 * keyboard.c
 *
 * Keyboard filter.  Uses a keyboard definition file to interpret
 * keyboard  codes into symbols.  Also keeps track of modifier
 * stat information.
 *
 */

#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/devi.h>
#include "photon.h"
#include <sys/keymap.h>
#include <sys/keycodes.h>


#define KEYMODMASK              (Pk_KM_Alt | Pk_KM_Ctrl)

struct devctl_led ledctl;


/* Separate processing of switch state
 * Switch state is a state between  the moment switch 
 * combination pressed and the moment real imput stars again
 * There  is a problem during this period because Aly or Ctrl
 * being pressed, continue to emit keystrokes (make). Each extra make
 * that comes to program can switch it to combination expectation mode
 * and next character can be combined with it and produce wrong UNICODE 
 * symbol. For example, if I press LeftAlt-Shift being in russian
 * keyboard layout to switch to an english one, if I type then 's', it suspend it,
 * then after next keystroke sends to Photon special symbol instead of 's'. It
 * looks bizare
 */


typedef struct data
{
	p_kmap_data kmapData;
	int switcher;									// current switch state 
}
tData, *ptData;

static void send_char (input_module_t * module, unsigned flags, unsigned scan,
											 unsigned cap, unsigned sym);
static int reset (input_module_t * module);
static void set_switch_state_on (ptData p, int switcher);
static void set_switch_state_off (ptData p);
static int is_switch_state_on (ptData p);
static void inform_photon_about_switch (input_module_t * module);


/* Description: function sends keyboard data to upper tier                          */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              unsigned flags - data description flags (see sys/keycodes.h)        */
/*              unsigned scan - scan code                                           */
/*              unsigned cap - key cap code                                         */
/*              unsigned sym - UNICOD symbol                                        */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : scan, cap and sym has valid values only if correspondent flags bits */
/*              are set on                                                          */
void
send_char (input_module_t * module, unsigned flags, unsigned scan,
					 unsigned cap, unsigned sym)
{
	p_kmap_data dp = ((ptData) (module->data))->kmapData;
	struct packet_kbd kbuf;


	kbuf.key.modifiers = kmap_get_std_modifiers (&(dp->header));

	kbuf.key.flags = flags;
	kbuf.key.key_cap = (flags & (KEY_CAP_VALID | KEY_OEM_CAP)) ? cap : scan ^ 5;
	kbuf.key.key_sym = (flags & (KEY_SYM_VALID | KEY_SYM_VALID_EX |KEY_DEAD)) ? sym : scan ^ 10;

	if (is_switch_state_on ((ptData) (module->data))) {
		int switcher = ((ptData) (module->data))->switcher;

		if (((KBDMAP_SWITCH_LEFT_ALT_SHIFT == switcher) &&
				 ((kbuf.key.modifiers & KEYMODMASK) != Pk_KM_Alt)) ||
				((KBDMAP_SWITCH_ANY_CTRL_SHIFT == switcher) &&
				 ((kbuf.key.modifiers & KEYMODMASK) != Pk_KM_Ctrl)))
			set_switch_state_off ((ptData) (module->data));
		// Inform photon about keyboard mapping switch
		inform_photon_about_switch (module);
	}


	kbuf.key.key_scan = (flags & KEY_SCAN_VALID) ? scan : sym ^ 21;
	clk_get (&kbuf.timestamp);

	if (!(OptFlags & OPT_NO_PHOTON) && (flags & KEY_SYM_VALID)) {	// Keyboard switch action
		int bIsShift = (kbuf.key.key_sym == Pk_Shift_L)
			|| (kbuf.key.key_sym == Pk_Shift_R);
		int switcher = kmap_get_active_switcher ();

		if (((KBDMAP_SWITCH_LEFT_ALT_SHIFT == switcher) && bIsShift &&
				 ((kbuf.key.modifiers & KEYMODMASK) == Pk_KM_Alt)) ||
				((KBDMAP_SWITCH_ANY_CTRL_SHIFT == switcher) && bIsShift &&
				 ((kbuf.key.modifiers & KEYMODMASK) == Pk_KM_Ctrl))) {
			kmap_switch_to_next (&(((ptData) (module->data))->kmapData));
			set_switch_state_on ((ptData) (module->data), switcher);
			return;
		}
	}
	if (verbosity >= 5)
		printf ("Set keyboard message to queue\n");
	devi_enqueue_packet (module, (char *) &kbuf, sizeof (kbuf));
}



/* Description: Callback initialisation function; it is called when input module is */
/*              initializing the input system                                       */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Actualy this function does nothing                                  */
static int
init (input_module_t * module)
{
	ledctl.mask = (KEYIND_NUM_LOCK | KEYIND_CAPS_LOCK | KEYIND_SCROLL_LOCK);
	ledctl.value = 0;
	if (!(module->data = calloc (sizeof (tData), 1)))
		return -1;
	((ptData) (module->data))->switcher = 0;
	((ptData) (module->data))->kmapData = NULL;
	return (EOK);
}

/* Description: This is a callback function for DEVCTRL command processing          */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
static int
devctrl (input_module_t * module, int event, void *ptr)
{
	p_kmap_data dp = ((ptData) (module->data))->kmapData;
	struct devctl_led *ledctrl = ptr;
	input_module_t *down = module->down;

	switch (event) {
	case DEVCTL_RESET_COMPOSESTATE:{

			break;
		}

	case DEVCTL_GETKEYF:
		if (ptr != NULL) {
			if (dp->name)
				strcpy (ptr, dp->name);
			else
				*(char *) ptr = '\0';
		}
		break;
	case DEVCTL_SETKEYF:
		{
			int rc;

			if (EOK !=
					kmap_add_new_keyboard (ptr, &(((ptData) (module->data))->kmapData)))
				return -1;
			rc =
				kmap_switch_to_keyboard (ptr, &(((ptData) (module->data))->kmapData));
			inform_photon_about_switch (module);
			return rc;
		}
	case DEVCTL_GETLED:
		ledctrl->mask = KEYIND_CAPS_LOCK | KEYIND_NUM_LOCK | KEYIND_SCROLL_LOCK;
		ledctrl->value = kmap_get_leds (&(dp->header));
		break;
	case DEVCTL_SETLED:
		{
			short led = ledctrl->value;

			led &= (KEYIND_NUM_LOCK | KEYIND_CAPS_LOCK | KEYIND_SCROLL_LOCK);
			if ((down->devctrl) (down, DEVCTL_LED, &led) == EOK)
				kmap_set_leds (&(dp->header), led);
			kmap_set_leds (&(dp->header), (_uint8) led);
			break;
		}
	case DEVCTL_GETLEDMASK:
		ledctrl->mask = KEYIND_CAPS_LOCK | KEYIND_NUM_LOCK | KEYIND_SCROLL_LOCK;
		ledctrl->value = 0;
		break;
	case DEVCTL_GETLOADKBDS:			/* get list of loaded keyboards    */
		if (NULL != ptr) {
			int i;
			int nPos = 0;
			int nNameLen;
			char *pName;
			int nLoadedKbds = kmap_get_num_of_loaded_keyboards ();
			p_kmap_data pData;

			// The loaded keyboard names are separated with '\0'
			for (i = 0; i < nLoadedKbds; ++i) {
				kmap_get_kmap_data_by_number (i, &pData);
				if (NULL == pData)
					continue;
				pName = basename (pData->name);
				nNameLen = strlen (pName);
				if ((nPos + nNameLen + 1) < _POSIX_PATH_MAX) {
					strcpy ((char *) ptr + nPos, pName);
					nPos += nNameLen + 1;
				}
			}
			*((char *) ptr + nPos + 1) = '\0';	// End of list
			break;
		}
		return -1;
	case DEVCTL_SEL_KBD:					/* select keyboard                 */
		if (NULL != ptr) {
			int rc = kmap_switch_to_keyboard (ptr,
																				&(((ptData) (module->data))->
																					kmapData));
			inform_photon_about_switch (module);
			return rc;
		}
		return -1;
	case DEVCTL_GET_SEL_KBD:			/* get selected keyboard           */
		if ((NULL != ptr) && (NULL != dp)) {
			char *pName = basename (dp->name);

			if (_POSIX_PATH_MAX > strlen (pName)) {
				strcpy ((char *) ptr, pName);
				break;
			}
		}
		return -1;
	case DEVCTL_ADD_KBD:					/* add new keyboard                */
		if (NULL != ptr) {
			int rc = kmap_add_new_keyboard (ptr,
																			&(((ptData) (module->data))->kmapData));

			inform_photon_about_switch (module);
			return rc;
		}
		return -1;
	case DEVCTL_REMOVE_KBD:			/* remove kbd                      */
		if (NULL != ptr) {
			int rc = kmap_remove_keyboard (ptr);

			inform_photon_about_switch (module);
			return rc;
		}
		return -1;									// error! Wrong parameter
	case DEVCTL_RELOAD_KBDS:			/* reload keyboards */
		kmap_remove_all_keyboards ();
		reset (module);
		inform_photon_about_switch (module);
		break;
	case DEVCTL_SET_KBD_STATE:{
		(down->devctrl) (down, DEVCTL_SET_STATE, ptr);
		break;
	}
	default:
		return ((down->devctrl) (down, event, ptr));
	}

	return EOK;
}

/* Description: this callback funtion is called when the module is linked into the  */
/*              event bus; it is used to set initial module state on the protocol   */
/*              level                                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise -1                                               */
static int
reset (input_module_t * module)
{
	int rc = 0;

	// Load name of dflt keyboard mapping file
	if (0 == kmap_get_num_of_loaded_keyboards ()) {
		char proc_path[PATH_MAX + 1];
		char *p;

		strncpy (proc_path, pFull_process_name, sizeof (proc_path) - 1);
		proc_path[sizeof (proc_path) - 1] = '\0';
		p = strrchr (proc_path, '/');
		if (NULL == p)
			proc_path[0] = '\0';
		else
			*(p + 1) = '\0';
		rc = kmap_load_keyboards (&(((ptData) (module->data))->kmapData),
															proc_path);
	}
	if (0 == rc)
		devctrl (module, DEVCTL_SETLED, &ledctl);

	return (rc);
}

/* Description: main filter   processing function.  It will be called  by the       */
/*              protocol layer to pass its data to process.                         */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int num - number of packets to process                              */
/*              void * arg - data to process                                        */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
static int
input (input_module_t * module, int num, void *ptr)
{
	struct packet_kbd *kin = (struct packet_kbd *) ptr;
	p_kmap_data dp = ((ptData) (module->data))->kmapData;
	int i;
	unsigned flags;
	short nOldLed = 0, nNewLed;
	int nLen;


	if (NULL == ((ptData) (module->data))->kmapData)	/* No active keyboard mapping file                                       */
		return EOK;

	for (i = 0; i < num; ++i) {
		if (kin[i].bMakeBreak)			// Key pressed
			nOldLed = kmap_get_leds (&(dp->header));	// Keep LED state; if comming data change LED state
		else												// Released key - forget about repeat
			dp->lastKey = KS_None;
		// send command to keyboard to reflect this changes
		flags = KEY_SCAN_VALID;
		if (KS_None != kin[i].nUSBCode) {	// comming data include USB code
			flags |= KEY_CAP_VALID;
			if (!(kin[i].key.flags & KEY_CAP_VALID)) {	// If there is no cap code in comming data, get it
				wchar_t nCapCode;
				int isDeadKey = 0;

				kmap_get_unicode (&(dp->header), kin[i].nUSBCode, NEED_CAP, &nCapCode,
													&nLen, &isDeadKey);
				if (1 != nLen)
					flags &= ~KEY_CAP_VALID;
				else
					kin[i].key.key_cap = nCapCode;
			}

			if (!(kin[i].key.flags & KEY_SYM_VALID)) {	// If there is no UNICOD symbol in coming data, get it
				int kmap_flags = 0;
				wchar_t nUnicod;

				nLen = 1;
				kmap_process_key_event (&(dp->header), kin[i].nUSBCode,
																kin[i].bMakeBreak, &nUnicod, &nLen,
																&kmap_flags);
				if (1 == nLen) {
					kin[i].key.key_sym = nUnicod;
					if( kin[i].bMakeBreak == KEY_PRESS ) 
						flags |= KEY_SYM_VALID;
					else
						flags |= KEY_SYM_VALID_EX;
				}
				if (kmap_flags & PFLAG_O_DEAD_KEY) {
					flags |= KEY_DEAD;
				}
			}

		}
		if (kin[i].bMakeBreak)			// Key pressed
		{
			if (kin[i].nUSBCode) {
				flags |= KEY_DOWN;			// If this is multi-scancode key (0xE0 + something), only last sending
				// has KEY_DOWN flag
				if (dp->lastKey == kin[i].nUSBCode)
					flags |= KEY_REPEAT;
				dp->lastKey = kin[i].nUSBCode;
			}
			nNewLed = kmap_get_leds (&(dp->header));
			if (nNewLed != nOldLed) {	// send command to keyboard to change LED state
				ledctl.value = nNewLed;
				ledctl.mask = KEYIND_CAPS_LOCK | KEYIND_NUM_LOCK | KEYIND_SCROLL_LOCK;
				devctrl (module, DEVCTL_SETLED, &ledctl);
			}
		}
		// Send data to upper tier (Photon or resource consumer)
		send_char (module, flags, kin[i].key.key_scan, kin[i].key.key_cap,
							 kin[i].key.key_sym);
	}

	return (EOK);
}

/* Description: this is a callback function for command line parameter processing   */
/*              (all valid parameters for device module are listed in keyboard.args)*/
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int opt  - parameter code                                           */
/*              char * optarg - optional parameter value                            */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise -1                                             */
static int
parm (input_module_t * module, int opt, char *optarg)
{

	switch (opt) {
	case 'k':										// Set keyboard map file
		if (NULL != optarg)
			return kmap_add_new_keyboard (optarg,
																		&(((ptData) (module->data))->kmapData));
		break;
	case 'L':
		ledctl.mask = (KEYIND_NUM_LOCK | KEYIND_CAPS_LOCK | KEYIND_SCROLL_LOCK);
		if (strchr (optarg, 'N'))
			ledctl.value |= KEYIND_NUM_LOCK;
		if (strchr (optarg, 'C'))
			ledctl.value |= KEYIND_CAPS_LOCK;
		if (strchr (optarg, 'S'))
			ledctl.value |= KEYIND_SCROLL_LOCK;
		break;
	default:
		break;
	}

	return (-1);
}


/* Description: this is a callback function which is called when resourse manager   */
/*              is shutting down                                                    */
/* Input      : input_module_t * module - pointer to module descriptor              */
/*              int ms  - program doesn't use this parameter                        */
/* Output     : None                                                                */
/* Return     : EOK                                                                 */
/* Comment    : Does nothing for the keyboard filter level                          */
static int
keyboard_shutdown (input_module_t * module, int delay)
{
	return (0);
}

void
set_switch_state_on (ptData p, int switcher)
{
	p->switcher = switcher;
}

void
set_switch_state_off (ptData p)
{
	p->switcher = 0;
	kmap_reset_keyboard_state (&(p->kmapData->header));
}

int
is_switch_state_on (ptData p)
{
	return (0 != p->switcher);
}

static void
inform_photon_about_switch (input_module_t * module)
{
	struct packet_kbd kbuf;

	memset (&kbuf, 0, sizeof (kbuf));
	kbuf.key.flags = KEY_MAPPING_CHANGED;
	devi_enqueue_packet (module, (char *) &kbuf, sizeof (kbuf));
	if (verbosity > 0)
		printf ("Send message to Photon about keyboard switching\n");
}


input_module_t keyboard_filter = {
	NULL,
	NULL,
	NULL,
	0,
	DEVI_CLASS_KBD | DEVI_MODULE_TYPE_FILTER,
	"keyboard",
	__DATE__,
	"k:L:",
	NULL,													// initialized by us (in init)
	init,
	reset,
	input,
	NULL,													// output 
	NULL,													// pulse not used (this is a pure filter)
	parm,
	devctrl,
	keyboard_shutdown
};

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/keyboard.c $ $Rev: 657836 $" );
