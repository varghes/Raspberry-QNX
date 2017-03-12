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
#include "storage.h"
#include <sys/keymap.h>


#if defined(__BIGENDIAN__)
#define ENDIAN_SWAP(pVal)  switch(sizeof(*pVal))        \
                              {                         \
  	                          case 2:                   \
                                 ENDIAN_SWAP16(pVal);   \
		                         break;                 \
	                          case 4:                   \
                                 ENDIAN_SWAP32(pVal);   \
		                         break;                 \
	                          case 8:                   \
                                 ENDIAN_SWAP64(pVal);   \
		                         break;                 \
	                      } 
#else
#define ENDIAN_SWAP(pVal)  ;
#endif
#if defined(__MIPS__)
#define COPY_DATA(to, from) memcpy((void *)&to, from, sizeof(to)); \
		                    from += sizeof(to);
#else
#define COPY_DATA(to, from) 
#endif

// Standard modifiers
static struct _stdMod {
   char * name;
   _uint32 nPos;
} stdMod[] =
{
     {"Shift", Pk_KM_Shift},
     {"Ctrl", Pk_KM_Ctrl},
     {"Alt", Pk_KM_Alt},
     {"AltGr", Pk_KM_AltGr},
     {"Shl3", Pk_KM_Shl3},
     {"Mod6", Pk_KM_Mod6},
     {"Mod7", Pk_KM_Mod7},
     {"Mod8", Pk_KM_Mod8},

     {"Shift_Lock", Pk_KM_Shift_Lock},
     {"Ctrl_Lock", Pk_KM_Ctrl_Lock},
     {"Alt_Lock", Pk_KM_Alt_Lock},	
     {"AltGr_Lock", Pk_KM_AltGr_Lock},
     {"Shl3_Lock", Pk_KM_Shl3_Lock},
     {"Mod6_Lock", Pk_KM_Mod6_Lock},
     {"Mod7_Lock", Pk_KM_Mod7_Lock},	
     {"Mod8_Lock", Pk_KM_Mod8_Lock},

     {"Caps_Lock", Pk_KM_Caps_Lock},
     {"Num_Lock", Pk_KM_Num_Lock},
     {"Scroll_Lock", Pk_KM_Scroll_Lock}
};

static struct _leds {
   char * ledName;
   _uint32 nStdLed;
} aLeds[] =

  {  
     {"Scroll_Lock", KEYIND_SCROLL_LOCK}, 
     {"Num_Lock",    KEYIND_NUM_LOCK},
     {"Caps_Lock",   KEYIND_CAPS_LOCK}
  };
     


static void * load_key_map(int nFileId);
static void * load_actions(int nFileId);
static void * load_modifiers(int nFileId, const pmod_array_t oldModifiers);
static void * load_compositions(int nFileId);


/* Description: free allocated memory we don't need more                            */
/* Input      : pst_header_t pHeader - pointer to header object                     */
/* Output     : None                                                                */
/* Return     : addres of key map area (starts from keyMapDir)                      */
int kmap_free_memory(pst_header_t pHeader)
{
   
   if(NULL == pHeader)
     return 0;
   
   if(NULL != pHeader -> pMod)
     {
     int i;
     for(i = 0; i < pHeader -> pMod -> nOfModifiers; ++i)
	  {
	  if(NULL != pHeader -> pMod -> aModifiers[i].pModName)
	       free(pHeader -> pMod -> aModifiers[i].pModName);
	  }
     free(pHeader -> pMod);
     }
   pHeader -> pMod = NULL;
   
   if(NULL != pHeader -> pAct)
     free(pHeader -> pAct);
   pHeader -> pAct = NULL;

   if(NULL != pHeader -> pComp)
     {
     if(NULL != pHeader -> pComp -> pComps)
	  free(pHeader -> pComp -> pComps);
     free(pHeader -> pComp);
     }
   pHeader -> pComp = NULL;

   if(NULL != pHeader -> pKMapDir)
     free(pHeader -> pKMapDir);
   pHeader -> pKMapDir = NULL;
   
   return 0;   
}

/* Description: function loads new keyboard definition file                         */
/* Input      : const char * fname - name of file to load                           */
/*              pst_header_t pHeader - pointer to header object                     */
/* Output     : None                                                                */
/* Return     : addres of key map area (starts from keyMapDir)                      */
int kmap_load_kbd_file(const char * fname, pst_header_t pHeader)
{
   int nFileId;
   char signature[20];
   st_header_t newHeader;
   BYTE nNameLen;
   int i, j, k;

   assert(NULL != fname);
   assert(NULL != pHeader);
   
   if(0 > (nFileId = open(fname, O_RDONLY))) 
     {
     if(verbosity)
         printf("Error: file %s - %s; cannot load keymap file\n", fname, strerror(errno));
     return errno;
     }
   if(0 > read(nFileId, signature, sizeof(KEYBIN_SIGNATURE) - 1))
     return errno;
   
   if(strncmp(KEYBIN_SIGNATURE, signature, sizeof((KEYBIN_SIGNATURE) - 1)))
     {
     if(verbosity)
         printf("Error: %s is a wrong file - exit\n", fname);
     return -2; /* this is not a keyboard definition file                           */
     }
   if(0 > read(nFileId, (void *)&nNameLen, sizeof(nNameLen)))
     return errno;
   if(0 > lseek(nFileId, nNameLen, SEEK_CUR))
     return errno;
   
   if(NULL == (newHeader.pMod = load_modifiers(nFileId, pHeader -> pMod)))
      return errno;
   
   memset(newHeader.stdModArray, -1, sizeof(newHeader.stdModArray));
   for(i = 0; i < ARRAY_SIZE(stdMod); ++i)
     {
     for(j = 0; j < newHeader.pMod -> nOfModifiers; ++j)
        {
	if(0 == strcmp(stdMod[i].name, newHeader.pMod -> aModifiers[j].pModName))
	     {
	     _uint32 mask = stdMod[i].nPos; 
	     for(k = 0; k < ARRAY_SIZE(newHeader.stdModArray); ++k)
		{
		mask >>= 1;
		if(0 == mask)
		   {
	           newHeader.stdModArray[k] = j; // i-th standard - our j-th
		   break;
		   }
		}
	     break;	
	     }
        }
     }

   memset(newHeader.leds, -1, sizeof(newHeader.leds));
   for(i = 0; i < ARRAY_SIZE(aLeds); ++i)
     {
     for(j = 0; j < newHeader.pMod -> nOfModifiers; ++j)
        {
	if(0 == strcmp(aLeds[i].ledName, newHeader.pMod -> aModifiers[j].pModName))
	     {
	     _uint32 mask = aLeds[i].nStdLed;
	     for(k = 0; k < ARRAY_SIZE(newHeader.leds); ++k)
		{
		mask >>= 1;
		if(0 == mask)
		   {
	           newHeader.leds[k] = j; // i-th standard led - our j-th
		   break;
		   }
		}
	     break;
	     }
	}
     }

   
   if(NULL == (newHeader.pComp = load_compositions(nFileId)))
      return -1;

   if(NULL == (newHeader.pAct = load_actions(nFileId)))
      return -1;
   
   
   if(NULL == (newHeader.pKMapDir = load_key_map(nFileId)))
      return -1;
  
   kmap_free_memory(pHeader);
   *pHeader = newHeader;
   
   return 0;
}



/* Description: function load key mapping tables                                    */
/* Input      : int nFileId - input file handler                                    */
/* Output     : None                                                                */
/* Return     : addres of key map area (starts from keyMapDir)                      */
void *  load_key_map(int nFileId)
{
   int i, j;
   pkey_map_item_t pItem;
   int nBlk;
   int nLen;
   int nSize;
   BYTE * pMap; 
   pkey_map_dir_t pDir;
#if defined(__MIPS__)
   BYTE * pWorkArea; // We allocate work area to use as a place for data alignment
   BYTE * pSrcPtr;   // Current pointer to the area we read data from file (before rearrangement)
   BYTE * pSrcSecPtr;// Current pointer to the section keymap in area we read data from file (before rearrangement)
   BYTE * pEndOfDir; // Pointer to the end of dir in source area
   int    nOffset;   // Current offset in work area
   int    nTotalSize;
   pdir_item_t p_dir;
#endif

   if(sizeof(nSize) !=  read(nFileId, (void *)&nSize, sizeof(nSize)))
     return NULL;

   ENDIAN_SWAP(&nSize);
   
   pMap = malloc(nSize);
#if defined(__MIPS__)
   pSrcPtr  = pMap;
   pWorkArea = malloc(nSize * 4);
#endif

   pDir = ( pkey_map_dir_t)pMap;
   if(NULL == pMap)
     return NULL;
   
   if(nSize != read(nFileId, pMap, nSize))
     {
     free(pMap);
     return NULL;
     }

   ENDIAN_SWAP(&(pDir -> nOfMapTables));
#if defined(__MIPS__)
   ((pkey_map_dir_t)pWorkArea) -> nOfMapTables = pDir -> nOfMapTables;
   pDir = (pkey_map_dir_t)pWorkArea;
   pSrcPtr += sizeof(pDir -> nOfMapTables);
   nOffset  = 0;
   pEndOfDir = pSrcPtr + pDir -> nOfMapTables * (sizeof(p_dir -> modState) + 
  		                              sizeof(p_dir -> modToTest) +
				              sizeof(p_dir -> nBlocks) +
					      sizeof(p_dir -> offset));
#endif
   for(i = 0; i < pDir -> nOfMapTables; ++i)
     {
     COPY_DATA((pDir -> dirTable[i].modState), pSrcPtr)
     COPY_DATA((pDir -> dirTable[i].modToTest), pSrcPtr)
     COPY_DATA((pDir -> dirTable[i].nBlocks), pSrcPtr)
     COPY_DATA((pDir -> dirTable[i].offset), pSrcPtr)
     ENDIAN_SWAP(&(pDir -> dirTable[i].modState));
     ENDIAN_SWAP(&(pDir -> dirTable[i].modToTest));
     ENDIAN_SWAP(&(pDir -> dirTable[i].nBlocks));
     ENDIAN_SWAP(&(pDir -> dirTable[i].offset));
#if defined(__MIPS__)
     pSrcSecPtr = pEndOfDir + pDir -> dirTable[i].offset;
     pDir -> dirTable[i].offset = nOffset;
#endif
     pItem = (pkey_map_item_t)((BYTE *)&(pDir -> dirTable[pDir -> nOfMapTables]) + 
		   pDir -> dirTable[i].offset);
     for(nBlk = 0; nBlk <  pDir -> dirTable[i].nBlocks; ++nBlk)
        {
#if defined(__MIPS__)
	COPY_DATA((pItem -> nCode), pSrcSecPtr)
	COPY_DATA((pItem -> descriptor), pSrcSecPtr)
	nOffset += sizeof(key_map_item_t);
#endif
    	ENDIAN_SWAP(&(pItem -> nCode));
        /* Type "1" - single                                        */
        if(1 == pItem -> descriptor.strSingle.type)
	       nLen = pItem -> descriptor.strSingle.length;
	else
	       nLen = pItem -> descriptor.strArray.length;
	for(j = 0; j < nLen; ++j)
	   {
#if defined(__MIPS__)
           if(j > 0)
	         nOffset += sizeof(pItem -> nUNICode[j]);
	   COPY_DATA(pItem -> nUNICode[j],  pSrcSecPtr);
#endif
	   ENDIAN_SWAP((pItem -> nUNICode + j));
	   }
        pItem = (pkey_map_item_t)&(pItem -> nUNICode[nLen]);
	}
     }
 
#if defined(__MIPS__)
   nTotalSize = (pEndOfDir - pMap) + nOffset;
   pWorkArea = realloc(pWorkArea, nTotalSize);
   free(pMap);
   pMap = pWorkArea;
#endif
   /* printf("NDirItems = %i\n", (int)(pDir -> nOfMapTables)); */
   return pMap;   
}

/* Description: function loads array of action descriptors                          */
/* Input      : int nFileId - input file handler                                    */
/* Output     : None                                                                */
/* Return     : addres of key map area (starts from keyMapDir)                      */
void * load_actions(int nFileId)
{
   int i;
   int nSize;
   int nLen;
   BYTE * pArea;
   pact_array_t pActs;
   paction_t pAct;
#if defined(__MIPS__)
   BYTE * pWorkArea; // We allocate work area to use as a place for data alignment
   BYTE * pSrcPtr;   // Current pointer to the area we read data from file (before rearrangement)
   paction_t pact;
   int    nActionSize = sizeof(pact -> modBefore) +
	                sizeof(pact -> modStateBefore) +
			sizeof(pact -> modAction) +
			sizeof(pact -> modStateAction) +
			sizeof(pact -> modToDo) +
			1;
#else
   int    nActionSize = sizeof(action_t);
#endif

   
   
   if(sizeof(nSize) !=  read(nFileId, (void *)&nSize, sizeof(nSize)))
     return NULL;

   ENDIAN_SWAP(&nSize);
   
   nLen = nSize * nActionSize;
   pArea = malloc(nLen + sizeof(pActs -> nOfActions));

   if(NULL == pArea)
     return NULL;

#if defined(__MIPS__)
   pSrcPtr  = pArea + sizeof(pActs -> nOfActions);
   pWorkArea = malloc(sizeof(act_array_t) + sizeof(action_t) * (nSize - 1));
   if(NULL == pWorkArea)
     {
     free(pArea);
     return NULL;
     }
   pActs = (pact_array_t)pWorkArea;
#else
   pActs = (pact_array_t)pArea;
#endif

   pActs -> nOfActions = nSize;
   
   if(nLen != read(nFileId, pArea + sizeof(pActs -> nOfActions), nLen))
     {
     free(pArea);
#if defined(__MIPS__)
     free(pWorkArea);
#endif
     return NULL;	
     }

   for(i = 0; i < nSize; ++i)
     {
     pAct = pActs -> aActions + i;
     COPY_DATA((pAct -> modBefore), pSrcPtr)
     ENDIAN_SWAP(&(pAct -> modBefore));
     COPY_DATA((pAct -> modStateBefore), pSrcPtr)
     ENDIAN_SWAP(&(pAct -> modStateBefore));
     COPY_DATA((pAct -> modAction), pSrcPtr)
     ENDIAN_SWAP(&(pAct -> modAction));
     COPY_DATA((pAct -> modStateAction), pSrcPtr)
     ENDIAN_SWAP(&(pAct -> modStateAction));
     COPY_DATA(pAct -> modToDo, pSrcPtr)
     COPY_DATA(pAct -> flags, pSrcPtr) 
     }
#if defined(__MIPS__)
   free(pArea);
   pArea = pWorkArea;
#endif   
   return pArea;
}

/* Description: function loads array of modifiers                                   */
/* Input      : int nFileId - input file handler                                    */
/*              const pModArray oldModifiers - previous modifiers                   */
/* Output     : None                                                                */
/* Return     : addres of modifiers area (see modArray)                             */
void * load_modifiers(int nFileId, const pmod_array_t pOldModifiers)
{
   int nSize;
   int nLen;
   BYTE nNameLen;
   BYTE * pArea;
   char * pName;
   pmod_array_t pMods;
   pmodifier pMod;
   int i, j;
   
   
   if(sizeof(nSize) !=  read(nFileId, (void *)&nSize, sizeof(nSize)))
     return NULL;

   ENDIAN_SWAP(&nSize);
      
   nLen =  sizeof(mod_array_t) + (nSize - 1) * sizeof(modifier);
   pArea = malloc(nLen);

   if(NULL == pArea)
     {
     char * pMsgTxt = "Error: cannot allocate memory (%i)\n";
     fprintf(stderr, pMsgTxt, nLen);
     slogf(_SLOG_SETCODE(_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, nLen);
     return NULL;
     }

   pMods = ( pmod_array_t)pArea;
   pMods -> nOfModifiers = nSize;
   pMods -> modState = 0;
   
   for(i = 0; i < nSize; ++i)
     {
     if(sizeof(nNameLen) != read(nFileId, (void *)&nNameLen, sizeof(nNameLen)))
	  break;
     if(NULL == (pName = (char *)malloc(nNameLen + 1)))
	  break;
     if(nNameLen != read(nFileId, pName, nNameLen))
	  break;
     pName[nNameLen] = '\0';	
     pMods -> aModifiers[i].pModName = pName;
     if(sizeof(pMod -> modKey) != read(nFileId, (void *)&(pMods -> aModifiers[i].modKey),
				       sizeof(pMod -> modKey)))
	  break;
#if defined(__BIGENDIAN__)
     ENDIAN_SWAP(&(pMods -> aModifiers[i].modKey));
#endif   
     /* try to keep modifiers' state in case of locality switch                     */
     if(NULL != pOldModifiers)
     for(j = 0; j < pOldModifiers -> nOfModifiers; ++j)
	  {
	  if(0 == strcmp(pOldModifiers -> aModifiers[j].pModName, pName))
	       {
	       if(pOldModifiers -> modState & (1LL << j))
	          pMods -> modState |= (1LL << i);
	       break;
	       }
	  }
     }
      
   if(i != nSize) /* Error */
     {
     for(j = 0; j <= i; ++j)
	  {
	  if(NULL != pMods -> aModifiers[j].pModName)
	       free(pMods -> aModifiers[j].pModName);
	  }
     free(pArea);
     return NULL;
     }
   
   return pArea;
}

/* Description: function loads compositions                                         */
/* Input      : int nFileId - input file handler                                    */
/* Output     : None                                                                */
/* Return     : addres of key map area (starts from keyMapDir)                      */
void * load_compositions(int nFileId)
{
   int i, j, k;
   pcompose_item_t pItem;
   int nSize;
   int nItems;
   pcomp_area_t pCArea;
#if defined(__MIPS__)
   BYTE * pOldCompArea;
   BYTE * pSrcArea;
   int nSectInd = 0;
#endif
   
   pCArea = (pcomp_area_t)malloc(sizeof(comp_area_t));
   
   if(NULL == pCArea)
     return NULL;
   
   memset(pCArea, 0, sizeof(comp_area_t));
   
   nSize = sizeof(pCArea -> aDeadKeys) + sizeof(pCArea -> sect);
   /* read Deadkeys array + index                                                   */
   if(nSize != read(nFileId, (void *)pCArea, nSize))
      return NULL;
   
   /* size of pComps section                                                        */
   if(sizeof(nSize) !=  read(nFileId, (void *)&nSize, sizeof(nSize)))
      return NULL;


#if defined(__BIGENDIAN__)
   ENDIAN_SWAP(&nSize);
   for(i = 0; i < ARRAY_SIZE(pCArea -> aDeadKeys); ++i)
     {
     ENDIAN_SWAP(pCArea -> aDeadKeys + i);
     }
   for(i = 0; i < ARRAY_SIZE(pCArea -> sect); ++i)
     {
     ENDIAN_SWAP(&(pCArea -> sect[i].nComps));
     ENDIAN_SWAP(&(pCArea -> sect[i].nOffset));
     }
#endif   
   
   nSize -= sizeof(nItems);
      
   if(0 >= nSize) /* Empty !                                                        */
     return pCArea;
   
   pCArea -> pComps  = malloc(nSize);

   if(NULL ==  pCArea -> pComps)
     return NULL;
   
      
   /* read number of items*/
   if(sizeof(nItems) != read(nFileId, (void *)&nItems, sizeof(nItems)))
      return NULL;

   /* read composition descriptors  */
   if(nSize != read(nFileId, pCArea -> pComps, nSize))
     {
     free(pCArea -> pComps);
     free(pCArea);
     return NULL;
     }
     
#if defined(__MIPS__)
	pOldCompArea = pCArea -> pComps;
	pCArea -> pComps = malloc(nSize * 2);
	if(NULL == pCArea -> pComps)
		{
		free(pOldCompArea);
		free(pCArea);
		return NULL;
		}
	pSrcArea = pOldCompArea;
#endif


   ENDIAN_SWAP(&nItems);

   pItem = (pcompose_item_t)(pCArea -> pComps);
   for(i = 0; i < nItems; ++i)
     {
#if defined(__MIPS__)     
     if((pSrcArea - pOldCompArea) == pCArea -> sect[nSectInd].nOffset)
       {
       pCArea -> sect[nSectInd].nOffset = (BYTE *)pItem - (BYTE *)(pCArea -> pComps);
       ++nSectInd;
       }
     COPY_DATA(*(BYTE *)pItem, pSrcArea);
#endif     
     k = pItem -> lengthIn + pItem -> lengthOut;
     for(j = 0; j < k; ++j)
       {
       COPY_DATA(pItem -> data[j], pSrcArea);
       ENDIAN_SWAP(pItem -> data + j);	  
       }
     pItem = (pcompose_item_t)(pItem -> data + j);
     }

#if defined(__MIPS__)
   if(0 < nItems)
     nSize =  (BYTE *)(pItem -> data + j)- (BYTE *)(pCArea -> pComps);
   else
     nSize = sizeof(comp_area_t);
   realloc(pCArea -> pComps, nSize);
   free(pOldCompArea);
#endif   

   pCArea -> nCurSect = -1; /* Set init mode - no composition ON                    */      
   
   return pCArea;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/keymap/loadkbd.c $ $Rev: 657836 $" );
