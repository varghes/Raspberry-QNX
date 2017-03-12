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
 *  storage.h
 *
 *  This file includes structures that are used by keyboard mapping  programs
 */

#ifndef _STORAGE_H
#define _STORAGE_H

#ifndef __MIPS__
#include <_pack1.h>
#endif

#include <sys/usbcodes.h>

#define KEYBIN_SIGNATURE  "KDEF-RTP"

#define MAX_DEAD_KEYS     (20)   


/* structure defines virtual modifier declared in keyboard layout description file */
/* Note: first eight modifiers are used for standard modifiers (USB):              */
/*     - LEFT_CTRL                                                                 */
/*     - LEFT_SHIFT                                                                */
/*     - LEFT_ALT                                                                  */
/*     - LEFT_GUI                                                                  */
/*     - RIGHT_CTRL                                                                */
/*     - RIGHT_SHIFT                                                               */
/*     - RIGHT_ALT                                                                 */
/*     - RIGHT_GUI                                                                 */
/* All other modifiers are either a key that was  assigned to be a modifier        */
/* (NUM_LOCK for example) or completely virtual modifiers (SHIFT for example).     */
/* All non-default modifiers get initial values and can be changed by actions      */
/* As more as 56 virtual modifiers can be declared                                 */
typedef struct _modifier
{
   char *pModName;             /* global modifier identifier                       */
   USBKCode modKey;            /* 0 if no key associated with this modifier        */
                               /* otherwise key USB code (see usbcodes.h)          */
} modifier, *pmodifier;

typedef struct _modArray
{
   short nOfModifiers;
   _uint64   modState; /* up to 64 modifiers; each bit corresponds to one modifier */
   modifier aModifiers[1];
} mod_array_t, * pmod_array_t;

typedef struct _action
{
   /* state of modifiers before action                                             */
   _uint64      modBefore;                /*  vector of modifiers we test state    */
                                          /*  before; 1 - test, 0 - ignore         */
   _uint64      modStateBefore;           /*  vector of modifiers state before     */
   _uint64      modAction;                /*  vector of modifiers that involved in */
                                          /*  action; 1 - involved, 0 - ignore     */
   _uint64      modStateAction;           /*  how these modifies can be changed    */
   _uint8       modToDo;                  /*  modifier number we  should change    */
   struct {
#if defined(__BIGENDIAN__)
   BYTE reserve                 :  5;     /*  reserve                              */
   BYTE newState                :  2;     /*  0 - press  , 1 - release             */
                                          /*  2 - up,      3 - down                */ 
   /* which modifier and how should be changed                                     */
   /* Up & Down changes global state, Press & Release - just temp variabes that    */
   /* decoder uses as array of curent actions                                      */
   #define _ACT_UP_                (0)
   #define _ACT_DOWN_              (1)
   #define _ACT_PRESS_             (2)
   #define _ACT_RELEASE_           (3)
   unsigned int bOperation      :  1;     /*  0 - "OR", 1 - "AND"                  */
                                          /*  Logical operation for before state   */
#else
   BYTE bOperation              :  1;     /*  0 - "OR", 1 - "AND"                  */
                                          /*  Logical operation for before state   */
   /* which modifier and how should be changed                                     */
   /* Up & Down changes global state, Press & Release - just temp variabes that    */
   /* decoder uses as array of curent actions                                      */
   #define _ACT_UP_                (0)
   #define _ACT_DOWN_              (1)
   #define _ACT_PRESS_             (2)
   #define _ACT_RELEASE_           (3)
   BYTE newState                :  2;     /*  0 - press  , 1 - release             */
                                          /*  2 - up,      3 - down                */ 
   BYTE         reserve         :  5;     /*  reserve                              */
#endif   
   } flags;
} action_t, * paction_t;


typedef struct _actArray          /* array of actions                              */
{
   short  nOfActions;
   action_t aActions[1];
} act_array_t, * pact_array_t;


/* Composition sections                                                           */
#define COMP_START_DEADKEY (0)
#define COMP_START_CTRL    (1)
#define COMP_START_SHIFT   (2)
#define COMP_START_ALT     (3)
#define COMP_START_GUI     (4)

/* We search the table of compositions sequentialy; we selected this implementation*/
/* because of the composed chains are quite short and so the cost of support any   */
/* general structure could be much more in comparing with this simple approach     */
typedef struct _composeItem
{
#if defined(__BIGENDIAN__)
   BYTE lengthOut         : 4;   /* length of result UNICODE string                 */
   BYTE lengthIn          : 4;   /* length of composed string                       */
#else
   BYTE lengthIn          : 4;   /* length of composed string                       */
   BYTE lengthOut         : 4;   /* length of result UNICODE string                 */
#endif   
   wchar_t          data[1];     /* first UNICODE comp string, then out string      */
} compose_item_t, * pcompose_item_t;

typedef struct _compArea         /* area where composition descriptors are stored   */
{
   wchar_t aDeadKeys[MAX_DEAD_KEYS]; /* Dead keys that were specified for this      */
		                 /* keyboard mapping file                           */
   /* four different modifiers can be applied (see comment to composeItem structure */
   /* composition descriptors grouped in four different sections for each possible  */
   /* modifier to expedite search; each section can be empty (nComps == 0)          */
   /* the last fifth section is created for combinations that can be ignited by     */
   /* dead keys  (see sample.kdef file)                                             */
   struct {  /* there are five sections                                             */
      _uint16 nComps; /* number of compositions in section                          */
      _uint16 nOffset;/* offset of section beginning from the start of pComps       */
   } sect[5];
   BYTE nModState;  /* we use this field to monitor compose ignition(sequential     */
                    /* pressing and releasing of any default modifier (from         */
                    /* KS_Control_L to KS_Right_GUI)                                */
   /* If it is a composition state, following variables describe it                 */
   short nCurSect;    /* number of active section; -1 if none                       */ 
   short nCurItemNum; /* Number of compose string we stand on                       */
   short nCurOffset;  /* offset of current compose sequence from the start of sect  */
   short nCurLen;     /* current length of compose sequence                         */
   /* End of composition state descriptor                                           */
   void *  pComps;
} comp_area_t, * pcomp_area_t;

/* In general, map table provides 1-to-n mapping: 1 USB code can be mapped to  n    */
/* UNICODE characters (about combination see comment later)                         */
/* array of modification flags is a key to the map table                            */
/* mapping table is a set of items  two main types                                  */
/*   - continuous array of map items                                                */
/*   - single map item                                                              */
/* array is defined using the first item code and its length; each array item       */
/* then mapping items follow;                                                       */
/* single item is defined just with USB key code                                    */
/* both array item and single item have three main types of mapping                 */
/*   - start of combination of codes (see compose characters in sample map file     */
/*   - set of UNICODE symbols (if output consists of more than one cha              */
/*   - single UNICODE code (this modified key should be mapped to just single code  */


typedef struct _keyMapItem
{
   USBKCode nCode;            /* USB code to be remapped (if type == 1 - this       */
                              /* code just corresponds to the  first item in array  */
   union {
     struct {
#if defined(__BIGENDIAN__)
        BYTE      length: 7;  /* length of array                                    */
        BYTE      type  : 1;  /* 0 - array of codes                                 */
                              /* This is a mapping array (may be just from 1 item)  */
                              /* In this case nCode corresponds to the first        */
                              /* item in this table. CAP == code                    */
	                      /* (CAP makes sense only for non-modified USB codes)  */
     } strArray;
     struct {
        BYTE      length: 4;  /* Length of string                                   */
  	BYTE      reserve:1;
	BYTE      deadKey:1;  /* 1 if this is a dead key                            */
        BYTE      bHasCAP:1;  /* does it has CAP ?                                  */
                              /* If CAP code exists it is always the last in the    */
	                      /* string (if length == 1, CAP == code)               */
        BYTE      type  : 1;  /* 1 - single code or string of UNICODs               */
     } strSingle;
#else
        BYTE      type  : 1;  /* 0 - array of codes                                 */
                              /* This is a mapping array (may be just from 1 item)  */
                              /* In this case nCode corresponds to the first        */
                              /* item in this table. CAP == code                    */
	                      /* (CAP makes sense only for non-modified USB codes)  */
        BYTE      length: 7;  /* length of array                                    */
     } strArray;
     struct {
        BYTE      type  : 1;  /* 1 - single code or string of UNICODs               */
        BYTE      bHasCAP:1;  /* does it has CAP ?                                  */
                              /* If CAP code exists it is always the last in the    */
	                      /* string (if length == 1, CAP == code)               */
	BYTE      deadKey:1;  /* 1 if this is a dead key                            */
	BYTE      reserve:1;
        BYTE      length: 4;  /* Length of string                                   */
     } strSingle;
#endif      
   } descriptor;
   wchar_t nUNICode[1];       /* string of UNICODEs                                 */
} key_map_item_t, * pkey_map_item_t;

typedef struct _dirItem
{
  _uint64  modState;          /* the state of modifiers for this map table          */
  _uint64  modToTest;         /* Each '1' s a modifier we have to test              */
  _uint16  nBlocks;           /* number of descriptive blocks in this table         */             
  _uint32  offset;            /* offset of map table from the end of directory      */
} dir_item_t, * pdir_item_t;


typedef struct _keyMapDir
{
   short nOfMapTables;
   dir_item_t dirTable[1];
} key_map_dir_t, * pkey_map_dir_t;

/* structure defines keyboard layout description file header                        */
typedef struct _stHeader
{
   _uint8 leds[8];                /* table to get LEDs                              */
   _uint8 stdModArray[32];        /* table to re-map to standard modifiers          */
   pmod_array_t pMod;             /* virtual modifiers                              */
   pact_array_t pAct;             /* actions that change modifier state             */
   pkey_map_dir_t pKMapDir;       /* key map directory                              */
   pcomp_area_t  pComp;           /* area where combinations of codes are located   */
                                  /* this area is used for mapping of composed      */
                                  /* characters sequences                           */
} st_header_t, * pst_header_t;


#ifndef __MIPS__
#include <_packpop.h>
#endif 


#endif  /*_STORAGE_H*/  

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/storage.h $ $Rev: 657836 $" )
