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








#ifndef __KEYMAP_H_INCLUDED__
#define __KEYMAP_H_INCLUDED__

#ifndef	_STDDEF_H_INCLUDED
#include <stddef.h>
#endif

/* #include "storage.h" */

#include <sys/usbcodes.h>

#define KEY_PRESS     (1)
#define KEY_RELEASE   (0)

#define IGNORE_COMP     0x01
#define IGNORE_ACTIONS  0x02

#define DONT_NEED_CAP   0
#define NEED_CAP        1

/* Keyboard switchers */
#define KBDMAP_SWITCH_LEFT_ALT_SHIFT          (1)
#define KBDMAP_SWITCH_ANY_CTRL_SHIFT          (2)
#define KBDMAP_SWITCH_NONE			          (0)
#define KBDMAP_SWITCH_DEFAULT		          (KBDMAP_SWITCH_LEFT_ALT_SHIFT)

#ifndef _STORAGE_H

/* struct _modArray; */
typedef struct _modArray * pmod_array_t; 
/* struct _actArray; */
typedef struct _actArray *  pact_array_t; 
/* struct _compArea; */
typedef struct _compArea *  pcomp_area_t; 
/* struct _keyMapDir; */
typedef struct _keyMapDir *  pkey_map_dir_t; 

/* structure defines keyboard layout description file header                        */
typedef struct _stHeader
{
   _Uint8t leds[8];                /* table to get LEDs                              */
   _Uint8t stdModArray[32];        /* table to re-map to standard modifiers          */
   pmod_array_t pMod;             /* virtual modifiers                              */
   pact_array_t pAct;             /* actions that change modifier state             */
   pkey_map_dir_t pKMapDir;       /* key map directory                              */
   pcomp_area_t  pComp;           /* area where combinations of codes are located   */
                                  /* this area is used for mapping of composed      */
                                  /* characters sequences                           */
} st_header_t, * pst_header_t;
#endif

typedef struct _kmap_data {
        struct _stHeader  header;
        USBKCode          lastKey; 
        char              *name;
} kmap_data, * p_kmap_data;


/* Using USB code and current modifiers state, function resolves  UNICODE string    */
void kmap_get_unicode(pst_header_t pHeader,USBKCode code, int bNeedCAP, wchar_t * pCodes, int * pnLen, int * pIsDeadKey);

/* function processes keyboard activity                                             */
void kmap_process_key_event(pst_header_t pHeader, USBKCode code, int nMakeBreak, 
			    wchar_t * pBuf, int * pnBufLen, int * pFlags);
/* These are processing flags. pFlags is an input/output parameter
 * For now we have only one output flag 
 */			    
#define PFLAG_O_DEAD_KEY  0x01			    

/* function checks if this is a deadkey code                                        */
int  kmap_is_dead_key(pst_header_t pHeader, wchar_t cDeadKeyCode);

/* function returns bit array of standard modifiers                                 */
_Uint32t kmap_get_std_modifiers(pst_header_t pHeader);

/* function returns bit array of LEDs                                               */
_Uint8t kmap_get_leds(pst_header_t pHeader);

/* function sets modifiers' state using new LED state                               */
void kmap_set_leds(pst_header_t pHeader, _Uint8t leds);

/* function copies keyboard state                                                   */
void kmap_copy_keyboard_state(pst_header_t pFrom, pst_header_t pTo);

/* function reset keyboard state                                                    */
void kmap_reset_keyboard_state(pst_header_t  pHeader);

/* function loads new keyboard definition file                                      */
int kmap_load_kbd_file(const char * fname, pst_header_t pHeader);

/* free allocated memory we don't need more                                         */
int kmap_free_memory(pst_header_t pHeader);

/* load keyboard mapping files                                                      */
int kmap_load_keyboards(p_kmap_data * ppData, char * proc_path);

/* Load new mapping file                                                            */
int kmap_add_new_keyboard( char * name, p_kmap_data * ppData);

/* Remove keyboard mapping file from memory                                         */
int kmap_remove_keyboard(char * name);

/* Remove keyboard mapping file from memory                                         */
int kmap_remove_all_keyboards();

/* Make another mapping file active                                                 */
int kmap_switch_to_keyboard(char * name, p_kmap_data * ppData);

/* Switch to next keyboard                                                          */
void kmap_switch_to_next(p_kmap_data * ppData);

/* Number of loaded keyboards                                                       */
int kmap_get_num_of_loaded_keyboards();

/* Get keymap descriptor                                                            */
int kmap_get_kmap_data_by_name(char * name, p_kmap_data * ppData);

/* Get keymap descriptor                                                            */
int kmap_get_kmap_data_by_number(int nKbd, p_kmap_data * ppData);

/* Get current switcher (see constants in the beginning of this file                */
int kmap_get_active_switcher();

#endif /* __KEYMAP_H_INCLUDED                                                       */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/public/sys/keymap.h $ $Rev: 657836 $" )
