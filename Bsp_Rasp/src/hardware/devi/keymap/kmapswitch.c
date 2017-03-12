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
 *
 *  loadkbd.c
 *
 *  This is an implementation of functions for loading of keboard file
 */

#include <sys/devi.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <photon/PkKeyDef.h>
#include <libgen.h>
#include "storage.h"
#include <sys/keymap.h>


#define MAX_KBDS                (10)
#define MAX_USB_CODES           (10)

#define DEFAULT_KEYBOARD_PATH	"/usr/photon/keyboard/"
#define DEFAULT_KBD_NAME        "en_US_101.kbd"



static p_kmap_data aData[MAX_KBDS];
static int nCurKbd = -1;
static int nLoadedKbds = 0;
static int switcher = KBDMAP_SWITCH_DEFAULT;

static void free_kbd_data(p_kmap_data pKbdData);
static int load_kbd( p_kmap_data dp, const char *name );
static int dflt_kbd( p_kmap_data dp, const char * path);
static int switch_to_keyboard_by_number(int nNewKbd,  p_kmap_data * ppData);



/* Description: function load keyboard mapping files into memory                    */
/* Input      : char * ppath - path to load mapping files from (or NULL if default) */
/*              default path is either /usr/photon/keyboard or current working dir  */
/* Output     : struct data * pKbdData - new active keymap descriptor               */
/* Return     : EOK if OK, or error code                                            */
/* Comment    : If input driver'd command line specifies which keyboards to load,   */
/*              program uses this information. Otherwise function tries to get this */
/*              information from /etc/system/trap/.KEYBOARD.hostname file which     */
/*              stipulates which keyboars  are supposed to be loaded. In case of    */
/*              absence f this file, default keyboard en_US_101.kbd will be loaded  */
int kmap_load_keyboards(p_kmap_data * ppData, char * ppath)
{
	int i;
    FILE * fd;
    int    nPos;
    int iSwitcher;
    char   hostname[ PATH_MAX + 1 ], 
           keyboard_file[ PATH_MAX + 1 ],
           kmap_name[ PATH_MAX + 1 ],
           p_proc_path[PATH_MAX + 1];
   if(verbosity)
      printf("Process home directory is %s\n", ppath);
   if(NULL != ppath)
      {
      char * p;
      strncpy(p_proc_path, ppath, sizeof(p_proc_path) - 1);
      p = strrchr(p_proc_path, '/');
      if(NULL != p)
	   *(p + 1) = '\0';
      else 
	   ppath = NULL;
      }
   if(NULL == ppath)
      strcpy(p_proc_path, "./");
   /* Try to get keyboard mapping file name                                           */
   strcpy( keyboard_file, "/etc/system/trap/.KEYBOARD");
   for(i = 0; i < 2; ++i) {
	   /* Try to get keyboard data from .KEYBOARD.[hostname] file
	    * Each line of this file is the name of keyboard mapping file
	    * If line has just a number between 1 and 4 we use it as a 
	    * keyboard switcher ID                                                         */
	   if(verbosity)
	      printf("Try to open keyboard descriptor ( %s)\n", keyboard_file);
	   if ( NULL != (fd = fopen ( keyboard_file, "r" ))) {
         while ( fgets ( kmap_name, sizeof(kmap_name), fd )) {
			 nPos = strlen(kmap_name) - 1;
			 if('\n' == kmap_name[nPos])
			      kmap_name[nPos] = '\0';
			 /* Take keyboard switcher (see the top of this file)                      */
			 if(0 == ( iSwitcher =  strtol(kmap_name, NULL, 10)))
		  	      kmap_add_new_keyboard(kmap_name, NULL);
			 else {
			 	 switcher = iSwitcher;
				 if(verbosity)
				     printf("Keyboard switcher is %i\n", switcher);
			 }
		 }
	     fclose(fd);
	     break;
	  }
	  // If we didn't find KEYBOARD file, try append it with the hostname
	  if( 0 != gethostname( hostname, sizeof( hostname )) )
      	 hostname[0] = '\0';
	  sprintf( keyboard_file, "/etc/system/trap/.KEYBOARD.%s", hostname );
   }
   if(0 == nLoadedKbds)
     { // Then use the default keyboard
   if(verbosity)
      printf("Try to load default keyboard\n");
	
     aData[0]  = malloc(sizeof(kmap_data));
     memset(aData[0], 0, sizeof(kmap_data));
     if(EOK == dflt_kbd( aData[0], p_proc_path ))
        nLoadedKbds = 1;
     else
        {
        char * pMsgTxt = "Keyboard mapping file wasn't specified in the command line.\n"
			 "The default mapping file also couldn't be loaded\n"
			 "Keyboard driver starting failure\n";
	if(verbosity)
	     fprintf(stderr, pMsgTxt);
	slogf(_SLOG_SETCODE(_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
	return ENOENT;
	}
     }
		       
   if(((KBDMAP_SWITCH_ANY_CTRL_SHIFT < switcher) || (KBDMAP_SWITCH_NONE > switcher)))
     switcher = KBDMAP_SWITCH_DEFAULT; // This is a default switcher!
   nCurKbd = 0;
   
   if(verbosity)
      printf("Switch to keyboard %s\n", aData[0] -> name);
   switch_to_keyboard_by_number(0, ppData);

   return EOK;
}

/* Description: function loads keyboard mapping file into memory                    */
/* Input      : char * name - name of the file to load (for instance fr_CA_102.kbd) */
/* Output     : struct data * pKbdData - new active keymap descriptor               */
/* Return     : EOK if OK, or error code                                            */
int kmap_add_new_keyboard( char * name, p_kmap_data * ppData)
{
   if((nLoadedKbds >= MAX_KBDS) ||
      (NULL == name))
       return EINVAL;
   if(EOK == kmap_switch_to_keyboard(name, ppData))
      return EOK;
   if(NULL != (aData[nLoadedKbds] = malloc(sizeof(kmap_data))))
      {
      memset(aData[nLoadedKbds], 0, sizeof(kmap_data));
      if(EOK == load_kbd(aData[nLoadedKbds], name))
	 {
	 if(!nLoadedKbds)
	    {
 	    nCurKbd = 0;
	    if(NULL != ppData)
	       *ppData = aData[nLoadedKbds];
	    }
	 ++nLoadedKbds;
	 return EOK;
         }
      }
    /* Error                                                                       */
    if(aData[nLoadedKbds])
         free(aData[nLoadedKbds]);
    return -1;
}


/* Description: function removs keyboard mapping file from memory                   */
/* Input      : char * name - name of the file to load (for instance fr_CA_102.kbd) */
/* Output     : None                                                                */
/* Return     : EOK if OK, or error code                                            */
int kmap_remove_keyboard(char * name)
{
    int i;
    int rc = EOK;
    int bDeleted = 0;
    for(i = 0; i < nLoadedKbds; ++i)
       {
       if(bDeleted)
	  {
	  aData[i - 1] = aData[i];
	  }
       else
       if(0 == stricmp(basename(aData[i] -> name), name))
	  {
	  if(i == nCurKbd) /* Don't remove selected keyboard                       */
	      return -1;
	  free_kbd_data(aData[i]); /* Clean up                                */
	  bDeleted = 1;
	  if(nCurKbd > i)
	     --nCurKbd;  /* Correct selected index                                 */
	  }
       }
    if(!bDeleted)
       rc = -1;
    else
        --nLoadedKbds; // Decrement loaded keyboard's counter
   
    return rc;
}

/* Description: function removes all loaded keyboards                               */
/* Input      : None															    */
/* Output     : None                                                                */
/* Return     : EOK if OK, or error code                                            */
int kmap_remove_all_keyboards() {
	int i;
	
    for(i = 0; i < nLoadedKbds; ++i) {
    	free_kbd_data(aData[i]);
    }
    nLoadedKbds = 0;
    nCurKbd = -1;
    return EOK;
}


/* Description: function changes active keyboard layout                             */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : struct data * pKbdData - new active keymap descriptor               */
/* Return     : EOK if okay or -1                                                   */
int kmap_switch_to_keyboard(char * name, p_kmap_data * ppData)
{
    int i;
    for(i = 0; i < nLoadedKbds; ++i)
       {
       if(0 == stricmp(basename(aData[i] -> name), name))
	  {
	  switch_to_keyboard_by_number(i, ppData);
	  return EOK;
	  }
       }
    return -1;
}

/* Description: function switchs to next keyboard mapping; does nothing if only     */
/*              one keboard is loaded                                               */
/* Input      : input_module_t * module - pointer to module descriptor              */
/* Output     : struct data * pKbdData - new active keymap descriptor               */
/* Return     : None                                                                */
void kmap_switch_to_next(p_kmap_data * ppData)
{
   if(nLoadedKbds > 0)
      switch_to_keyboard_by_number(((nCurKbd + 1) % nLoadedKbds), ppData);
}

/* Description: function returns number of loaded keyboards                         */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : number of loaded keyboards                                          */
int kmap_get_num_of_loaded_keyboards()
{
   return nLoadedKbds;
}

/* Description: function finds keymap data descriptor using keyboard name           */
/* Input      : char * name - name of the file to load (for instance fr_CA_102.kbd) */
/* Output     : struct data * pKbdData -  keymap descriptor                         */
/* Return     : EOK if okay, or -1                                                  */
int kmap_get_kmap_data_by_name(char * name, p_kmap_data * ppData)
{
    int i;
    for(i = 0; i < nLoadedKbds; ++i)
       {
       if(0 == stricmp(basename(aData[i] -> name), name))
	  {
	  kmap_get_kmap_data_by_number(i, ppData);
	  return EOK;
	  }
       }
    return -1;
}

/* Description: function finds keymap data descriptor using relative number         */
/* Input      : int nKbd - relative number in array of loaded keyboards             */
/* Output     : struct data * pKbdData -  keymap descriptor                         */
/* Return     : EOK if okay, or -1                                                  */
/* Comment    : this is mostly a service function                                   */
int kmap_get_kmap_data_by_number(int nKbd, p_kmap_data * ppData)
{
    if((nKbd < 0) || (nKbd >= nLoadedKbds))
        return -1;
    *ppData = aData[nKbd];
    return EOK;
}


/* Description: function returns code of active switcher (see include/keymap.h)     */
/* Input      : char * name - name of the file to load (for instance fr_CA_102.kbd) */
/* Output     : struct data * pKbdData -  keymap descriptor                         */
/* Return     : EOK if okay, or -1                                                  */
int kmap_get_active_switcher()
{
   return switcher;
}
   
/* Static functions                                                                 */

/* Description: function makes active nNewKbd                                       */
/* Input      : int nNewKbd - relative number in array of loaded keyboards          */
/* Output     : struct data * pKbdData -  new active keymap descriptor              */
/* Return     : EOK if okay, or -1                                                  */
int switch_to_keyboard_by_number(int nNewKbd,  p_kmap_data * ppData)
{   
    if((nNewKbd < 0) || (nNewKbd >= nLoadedKbds))
        return -1;
    if(nCurKbd >= 0) {
	    aData[nCurKbd] -> header.pComp -> nCurSect = -1; /* Kill expectation state */
        kmap_copy_keyboard_state(&(aData[nCurKbd] -> header), &(aData[nNewKbd] -> header));
    }
    *ppData    = aData[nNewKbd];
    nCurKbd = nNewKbd;
    return EOK;
}


/* Description: function setups default kbd if no one was specified                 */
/* Input      : struct data *dp - module private data                               */
/* Output     : None                                                                */
/* Return     : EOK of ok, otherwise error code                                     */
int dflt_kbd( p_kmap_data dp, const char * ppath )
{   
    char default_file_name[PATH_MAX + 1];
    int  rc;
   
    if(NULL != dp->name)
        free(dp->name);
    dp -> name = NULL;
    // First, try to use process home directory
    strcpy(default_file_name, ppath);
    strcat(default_file_name, DEFAULT_KBD_NAME);
    if(verbosity)
        printf("Load default keymap file (%s)\n", default_file_name);
    if(EOK != (rc = load_kbd(dp, default_file_name)))
        // If not found, try to search in other locations
	// (trust this work to load_kbd)
        rc = load_kbd(dp, NULL);
    return rc;
   
}


/* Description: function loads keyboard mapping file                                */
/* Input      : struct data *dp - module private data                               */
/*              const char * name - file name                                       */
/* Output     : None                                                                */
/* Return     : EOK if of, otherwise error code                                     */
/* Comment    : If name is NULL, tries to take dp->name;if the last one is also NULL*/
/*              tries to take it from the environment (KBD environment variable); if*/
/*              the name has still been NULL, use DEFAULT_KBD_NAME                  */
int load_kbd( p_kmap_data dp, const char *name ) 
{
    int rc = EOK;
    char buff[PATH_MAX];
    char * pSlash, * pDot;
   
    if (name == NULL ) 
	{
	name = dp->name;
	if (!name && !(name = getenv("KBD")))
	     name = DEFAULT_KBD_NAME;
	}
		
     if (strchr(name, '/')) // This is exactly the name we want to use; don't modify it
     	strcpy(buff, name);
     else 
	{
	char *def_path;
	if ((def_path = getenv("PHOTON_PATH"))) 
	    {
	    strcpy(buff, def_path);
	    strcat(buff, "/keyboard/");
	    }
	else 				
	    strcpy(buff, DEFAULT_KEYBOARD_PATH);
	
	strcat(buff, name);
	}

     pDot = strrchr(buff, '.');
     pSlash = strrchr(buff, '/');
     if((NULL == pDot) || ((NULL != pSlash) && (pDot < pSlash)))
	strcat(buff, ".kbd");

     if (verbosity)
	printf("Opening keyboard file: %s\n", buff ); 
        
     if(EOK == (rc = kmap_load_kbd_file(buff, &(dp -> header))))
        {
	if(NULL != dp -> name)
	   free(dp -> name);
	dp -> name = strdup(buff);
	dp -> lastKey = KS_None;
	}

     return rc;
}

/* Description: function release keyboard layout data                               */
/* Input      : struct data * pKbdData - private data to free                       */
/* Output     : None                                                                */
/* Return     : None                                                                */
void free_kbd_data(p_kmap_data pKbdData)
{
   if(NULL == pKbdData)
     return;
   kmap_free_memory(&(pKbdData -> header));
   if(NULL != pKbdData -> name)
     free(pKbdData -> name);
   free(pKbdData);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/keymap/kmapswitch.c $ $Rev: 657836 $" );
