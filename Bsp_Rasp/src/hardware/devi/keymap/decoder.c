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
 *  decoder.c
 *
 *  This is an implementation of conversion of keyboard input into UNICOD     
 */


#include <sys/devi.h>

#include <photon/PkKeyDef.h>
#include <sys/usbcodes.h>
#include "storage.h"
#include <sys/keymap.h>

#define _OR_          (0)
#define _AND_         (1) 

#define MAX_UNICODE_LEN (20)
#if 0
static void printLL(char * text, _uint64 val);
#endif

static void test_actions(pst_header_t pHeader,int nModNum, int nPressRelease);
static void check_comp_state(pst_header_t pHeader, const wchar_t * pCodes, int * pnLen, wchar_t * pResCodes, 
		    int * pnResLen, int nMaxLen);
static void print_mod_state(pst_header_t pHeader, _uint64 modState);
static void init_comp_state(pst_header_t pHeader, int nSection);


int nState = 0;
static _uint8 anKeys[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static USBKCode make_brake_exceptions[] = { 0x48 }; // Keys that don't have break

/* Description: function processes keyboard activity                                */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/*              USBKCode code - USB usage code                                      */
/*              int nMakeBreak - 0 - release, 1 - press                             */
/* Output     : wchar_t pBuf - buffer for result string                             */
/* In/Out     : int * pnBufLen - max buffer length; on return - length of the result*/
/*              string                                                              */
/* Return     : None                                                                */
void kmap_process_key_event(pst_header_t pHeader, USBKCode code, 
	int nMakeBreak, wchar_t * pBuf, int * pnBufLen,int * pFlags)
{
   int nModNum;
   wchar_t aUNICodes[MAX_UNICODE_LEN];
   int nUNICODELen;
   int nMaxLen = *pnBufLen;
   int isDeadKey = 0;
   int i,j;
   
   *pnBufLen = 0;

	if(NULL != pFlags)
		*pFlags = 0;   
   /* Check if it is a compose sequence ignition                                    */
   /* we can be in one of three states:                                             */
   /*    - regular state (0==header.pComp->nModState)&&(header.pComp -> nCurSect<0) */
   /*    - expectation state (one and only one bit of header.pComp->nModState is on)*/
   /*    - composition state (header.pComp -> nCurSect >= 0)                        */
   
   if(!(nState & IGNORE_COMP) && (code >= KS_Control_L) && (code <= KS_RightGUI))
     {
     /* Note: exclude Shift from combination starter. Otherwise there is no way to      */
     /*       input capital letteres or any shifted keys                                */
     if((code == KS_Shift_L) || (code == KS_Shift_R))
          ; /* Just do nothing - shift keys don't impact composition's processing       */
     else
     if(pHeader -> pComp -> nCurSect >= 0) /* Already in composition state              */
	  {
         if(nMakeBreak == KEY_PRESS) 
	   	     pHeader -> pComp -> nCurSect = -1; /* Kill it                              */
	  }
     else
	  {
	  BYTE mask = 1 << (code - KS_Control_L);
	  int nSection = (code - KS_Control_L) % 4 + 1;
	  if((pHeader -> pComp -> nModState) & mask)
	       {
	       if(nMakeBreak == KEY_RELEASE) 
		  /* set it on                                                       */
		  init_comp_state(pHeader, nSection);
		  if(NULL != pFlags)
		  	*pFlags |= PFLAG_O_DEAD_KEY;
	       /* Otherwise this is repeat press - just ignore it                       */
	       }
	  else
	  if(0 == pHeader -> pComp -> nModState) /* Set expectation state               */
	       {
	       if((nMakeBreak == KEY_PRESS) &&
		  (pHeader -> pComp -> sect[nSection].nComps > 0))
		    {
		    int i, j = sizeof(anKeys);
		    for(i = 0; i < j; ++i)
			 {
			 if(anKeys[i])
			   break;
			 }
		    if(i == j) /* There is no pressed keys now                          */
	                pHeader -> pComp -> nModState |= mask; /* set expectation state */
		    }
	       }
	  else
	       pHeader -> pComp -> nModState = 0;        /* Kill it                     */
	  }
     }
   else /* Any key kill expectation state                                               */
     pHeader -> pComp -> nModState = 0;        /* Kill it                               */

   /* In order to check possibility to switch to expectation state keep array of        */
   /* pressed/released keys: corresponding bit set to 1 (pressed) or to 0 (released)    */
   for(i = 0,j = sizeof(make_brake_exceptions)/sizeof(USBKCode); i < j; ++i)
   	{
   	if(make_brake_exceptions[i] == code)
   		break;
   	}
   if(i == j) //This is not an exception key that never produce Break
    {
	if(nMakeBreak)
     anKeys[code / 8] |= (1 << (code % 8));
    else
     anKeys[code / 8] &= ~(1 << (code % 8));
    }
   
   /* Check actions                                                                 */
   if(!(nState & IGNORE_ACTIONS))
      for(nModNum = 0; nModNum < pHeader -> pMod -> nOfModifiers; ++nModNum)
        {
        if(pHeader -> pMod -> aModifiers[nModNum].modKey == code) 
	     { // This is one of modifiers
	       // and it's state  was changed ( if nMakeBreak == 1 and modifier already in 
	       // down state, no any action should be executed - this is key repeating
	     if((0 == nMakeBreak) || !(pHeader -> pMod -> modState & (1LL << nModNum)))
  	         test_actions(pHeader, nModNum, nMakeBreak);
	     break;
	     }
        }
   
   /* Check if UNICODE sequence is generated                                        */
   kmap_get_unicode(pHeader, code, 0, aUNICodes, &nUNICODELen, &isDeadKey);

  if(KEY_PRESS == nMakeBreak) {

   if(isDeadKey != 0)
     {
     init_comp_state(pHeader, (KS_Alt_L - KS_Control_L) % 4 + 1);
	 if(NULL != pFlags)
	  	*pFlags |= PFLAG_O_DEAD_KEY;
     }

   if(0 == nUNICODELen) /* nothing                                                  */
     return;


   /* Process comosed sequences (if it is not a shift key)                          */
   /* When we are in composite state, Shift keys only change modifiers              */
   if((code != KS_Shift_L) && (code != KS_Shift_R))
   while(nUNICODELen > 0)
     {
         /* if we are in composition state, test new character                      */
      if(pHeader -> pComp -> nCurSect < 0)
	  break;
      check_comp_state(pHeader, aUNICodes, &nUNICODELen, pBuf, pnBufLen, nMaxLen);
      }
  }
  
   if(nUNICODELen > 0) 
     {
     int nCpyLen = nUNICODELen; 
     if(nMaxLen - *pnBufLen < nUNICODELen)
	  {
	  nCpyLen = nMaxLen - *pnBufLen;
	  if(verbosity > 0)
	       fprintf(stderr, "Error (putKeybEvent): too short buffer\n");
	  }
     if(0 < nCpyLen)
          memcpy((void *)(pBuf + *pnBufLen), aUNICodes, nCpyLen * sizeof(wchar_t));
     *pnBufLen += nCpyLen;
     }
          
} 


/* Description: using USB code and current modifiers state, function resolves       */
/*              UNICODE string                                                      */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/*              USBKCode code - usage code of current key                           */
/*              int bNeedCAP - if 1, CAP code is requested                          */
/* Output     : wchar_t ** pCode - pointer to return UNICODE string                 */
/*              int * pnLen - pointer to return found string length                 */
/* Return     : None                                                                */
void kmap_get_unicode(pst_header_t pHeader, USBKCode code, int bNeedCAP,wchar_t * pCodes, int * pnLen, int * pIsDeadKey)
{
   int nKeyMapSect;
   int nBlk;
   pkey_map_item_t pItem;
   _uint64 modState;
   int nLen;
   
   *pCodes = NULL;
   *pnLen = 0;

   /* Each storage section (sections differ in state of manipulators) consists of a*/
   /* chain of mapping tables. First we search for corresponding section, then go  */
   /* through mapping tables in this section in order to find UNICODE (usually one */
   /* byte) string which corresponds to USB parameter code                         */
   for(nKeyMapSect = 0; nKeyMapSect < pHeader -> pKMapDir -> nOfMapTables; ++nKeyMapSect)
     {
     modState = pHeader -> pKMapDir -> dirTable[nKeyMapSect].modState; /* Valid modifiers */
     if((bNeedCAP && (0 == modState)) ||
	(!bNeedCAP && (modState == (pHeader -> pMod -> modState &  pHeader -> pKMapDir -> dirTable[nKeyMapSect].modToTest))))
	  {
	  pItem = (pkey_map_item_t)((BYTE *)&(pHeader -> pKMapDir -> dirTable[pHeader -> pKMapDir -> nOfMapTables]) + 
		   pHeader -> pKMapDir -> dirTable[nKeyMapSect].offset);
	  for(nBlk = 0; nBlk <  pHeader -> pKMapDir -> dirTable[nKeyMapSect].nBlocks; ++nBlk)
	       {
	       /* Type "1" - single                                        */
	       if(1 == pItem -> descriptor.strSingle.type)
		    { 
		    nLen = pItem -> descriptor.strSingle.length;
	            if(pItem -> nCode == code)
			 {
			 if(pItem -> nUNICode[0] != Pk_InvalidSymbol)
			      {
			      if(bNeedCAP) // CAP is a last (may be a single) code in UNICOD string
				   {
				   pCodes[0] = pItem -> nUNICode[pItem -> descriptor.strSingle.length - 1];
				   *pnLen = 1;   
				   }
			      else
				   {
			           *pnLen = pItem -> descriptor.strSingle.length;
			           memcpy(pCodes, pItem -> nUNICode, *pnLen * sizeof(wchar_t));
				   if(pItem -> descriptor.strSingle.bHasCAP) // Ignore last CAP code
					--*pnLen;
				   }
			      }
			 if(NULL != pIsDeadKey)
				 *pIsDeadKey = ((pItem -> descriptor.strSingle.deadKey == 0) ? 0 : 1);
			 return;
			 }
		    }
	       /* type "0" - table which covers a range of USB codes (always maps  */
	       /* to single UNICODE code)                                          */ 
	       else
		    {
		    nLen = pItem -> descriptor.strArray.length;
		    if((pItem -> nCode <= code) && ((pItem -> nCode + pItem -> descriptor.strArray.length) > code))
			 {
			 pCodes[0] = pItem -> nUNICode[code - pItem -> nCode];
			 *pnLen = 1;
			 return;
			 }
		    }
	       pItem = (pkey_map_item_t)&(pItem -> nUNICode[nLen]);
	       }
	  }
     }
}


/* Description: function checks if this is a deadkey code                           */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/*              wchar_t cDeadKeyCode - dead key code                                */
/* Out        : None                                                                */
/* Return     : 1 if cDeadKeyCode is a code of Dead Key, 0 otherwise                */                                                         
int  kmap_is_dead_key(pst_header_t pHeader, wchar_t cDeadKeyCode)
{
   int i;
   for(i = 0; i < MAX_DEAD_KEYS; ++i)
     {
     if(0 == pHeader -> pComp -> aDeadKeys[i])
	  break;
     else
     if(cDeadKeyCode == pHeader -> pComp -> aDeadKeys[i])
	  return 1;
     }
   return 0;
}


/* Description: function returns bit array of standard modifiers                    */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/* Output     : None                                                                */
/* Return     : bit array of standard modifiers                                     */                                                         
_uint32 kmap_get_std_modifiers(pst_header_t pHeader)
{
   _uint32 nStdModifiers = 0;
   _uint32 nModifiers = (_uint32)(pHeader -> pMod -> modState); // First 32 bits of our modifiers' bit array
   int i, j;
   _uint32 mask = 1;
   
   for(i = 0; i < ARRAY_SIZE(pHeader -> stdModArray); ++i)
     {
     j = pHeader -> stdModArray[i];
     if(((uint8_t)-1 != j) &&   // Do we have this standard modifier amoung our modifiers array?
	((1 << j) & nModifiers)) // And is it set ON ?
	  nStdModifiers |= mask; // Yes, set standard modifier ON
     mask <<= 1; // Shift mask to analize next standard modifier
     }

   return nStdModifiers;
}



/* Description: function returns bit array of LEDs                                  */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/* Output     : None                                                                */
/* Return     : bit array of standard modifiers                                     */                                                         
_uint8 kmap_get_leds(pst_header_t pHeader)
{
   _uint8  nLeds = 0;
   _uint32 nModifiers;
   int i, j;
   _uint8 mask = 1;

   
   nModifiers = (_uint32)(pHeader -> pMod -> modState); // First 32 bits of our modifiers' bit array
   for(i = 0; i < ARRAY_SIZE(pHeader -> leds); ++i)
     {
     j = pHeader -> leds[i];
     if((-1 != j) &&   // Do we have this standard modifier amoung our modifiers array?
	((1 << j) & nModifiers)) // And is it set ON ?
	  nLeds |= mask; // Yes, set standard led ON
     mask <<= 1; // Shift mask to analize next standard LED
     }
   return nLeds;
}

/* Description: function sets modifiers' state using new LED state                  */
/* Input      : pst_header_t pHeader - key map descriptor                              */
/*              _uint8 leds - new LED state                                         */
/* Output     : None                                                                */
/* Return     : bit array of standard modifiers                                     */                                                         
void kmap_set_leds(pst_header_t pHeader, _uint8 leds)
{
   int i, j;
   _uint8 mask = 1;
   
   
   for(i = 0; i < ARRAY_SIZE(pHeader -> leds); ++i)
     {
     j = pHeader -> leds[i];
     if(-1 != j)    // Do we have this standard modifier amoung our modifiers array?
	{
	if(leds & mask)
	     pHeader -> pMod -> modState |= (1LL << j);
	else
	     pHeader -> pMod -> modState &= ~(1LL << j);
	}
     mask <<= 1;
     }
}



/* Description: function copies keyboard state                                      */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/* Output     : None                                                                */
/* Return     : bit array of standard modifiers                                     */                                                         
void kmap_copy_keyboard_state(pst_header_t pFrom, pst_header_t pTo)
{
   int i, j;
   if(pFrom == pTo)
   	  return;   
   kmap_reset_keyboard_state(pTo);
   
   for(i = 0; i < pFrom -> pMod -> nOfModifiers; ++i)
     {
     for(j = 0; j < pTo -> pMod -> nOfModifiers; ++j)
	{
	if(0 == strcmp(pFrom -> pMod -> aModifiers[i].pModName, 
		       pFrom -> pMod -> aModifiers[j].pModName))
	     {
	     if((pFrom -> pMod -> modState) & (1LL << i))
  	         pTo -> pMod -> modState |= (1LL << j);
	     else
		 pTo -> pMod -> modState &= ~(1LL << j);
	     break;
	     }
	}
     }
   
}

/* Description: function reset keyboard state                                       */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/* Output     : None                                                                */
/* Return     : bit array of standard modifiers                                     */                                                         
void kmap_reset_keyboard_state(pst_header_t  pHeader)
{
   if(pHeader)  {
   	   if(pHeader -> pMod)
	       	pHeader -> pMod -> modState = 0;
	   if(pHeader -> pComp)
	   		pHeader -> pComp -> nCurSect = -1; /* Kill combination                              */       
   }
}


/* Description: function passes through modifiers' array and does actions if need   */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/*            : int nModNum - modifier number                                       */
/*              int nPressRelease - 0 - release, 1 - press                          */
/* Output     : None                                                                */
/* Return     : None                                                                */
void test_actions(pst_header_t pHeader, int nModNum, int nPressRelease)
{
   //int i;
   //int nOfModifiers = pHeader -> pMod -> nOfModifiers;
   int nActNum;
   paction_t pAct;
   int nBefore = 0; /* is before state satisfied ?                                  */
   _uint64 nPressReleaseMask  = 1LL << nModNum;
   _uint64 nPressReleaseState;
   _uint64 toDoLongMask;
   _uint64 nNewModState = pHeader -> pMod -> modState;

   if(1 == nPressRelease) /* Pressed */
     {
      nNewModState |= 1LL << nModNum; /* Set on this modifier */
      nPressReleaseState =  nPressReleaseMask;
     }
   else
     {
      nNewModState &= ~(1LL << nModNum);  /* Set off this modifier */
      nPressReleaseState =  0;
     }
   
   /* And then change virtual modifiers                                             */ 
   for(nActNum = 0, pAct = pHeader -> pAct -> aActions; nActNum <  pHeader -> pAct -> nOfActions;
       ++nActNum, ++pAct)
      {
      /* test state before                                                          */
      _uint64 modBefore = pAct -> modBefore & pAct -> modStateBefore,
	      modCurr   = pAct -> modBefore  & nNewModState;
	 

      if(!(pAct -> modBefore)) /* Just ignore before state - we dont care about it  */
	 nBefore = 1;
      else
      if(_AND_ == pAct -> flags.bOperation)
	 nBefore = (modBefore == modCurr);
      else /* _OR_                                                                  */
	 nBefore = ~((modBefore ^ modCurr) | ~(pAct -> modBefore));

      /* before is OK; then check action                                            */	 
      if(nBefore)
	 {
	 
	 if(pAct -> modAction) /* If this acton descriptor demands any modifier to be involved */
	     {
	     _uint64 mask = (pAct -> modAction & nPressReleaseMask);

	     if(0 == (mask & ~(pAct -> modStateAction ^ nPressReleaseState)))
	        continue; 
	     } 
	 /* all conditions are satisfied - do the action                            */

	 /* testAction(pAct -> modToDo, pAct -> newState);                          */
	 /* doesn't do it now; it's too sophisticated                               */
	 
   
	    
	 toDoLongMask = 1LL << pAct -> modToDo;
	 switch(pAct -> flags.newState)
	      {
	       case _ACT_UP_:
		 nPressReleaseMask &= ~toDoLongMask;
		 nNewModState &= ~toDoLongMask;
		 break;
	       case _ACT_DOWN_:
		 nPressReleaseMask &= ~toDoLongMask;
		 nNewModState |= toDoLongMask;
		 break;
	       case _ACT_PRESS_:
		 nPressReleaseMask  |= toDoLongMask;
		 nPressReleaseState |= toDoLongMask;
		 break;
	       case _ACT_RELEASE_:	 
		 nPressReleaseMask |= toDoLongMask;
		 nPressReleaseState &= ~toDoLongMask;
		 break;
	      }
	 }
      }

   
   nNewModState |= nPressReleaseState; /* Add pressed */
   nNewModState &= ~nPressReleaseMask | nPressReleaseState; /* Remove released */
   
   if(verbosity >= 5)
     {
      printf("New state of modifiers:\n");
      print_mod_state(pHeader, nNewModState);
     }
   
   pHeader -> pMod -> modState = nNewModState;
   
}


/* Description: check compose state and probably replace compose sequence           */
/* Input      : pst_header_t pHeader - key map descriptor                           */
/* In/Out     : const wchar_t * pCodes - input UNICODE string to check              */
/*              int * pnLen - pointer length of input string                        */
/*              wchar_t * pResCodes - buffer to get the result replacement          */
/*              int * pnResLen - pointer to length of the result string             */
/* In         : int nMaxLen - size of the result buffer                             */
/* Output     : None                                                                */
/* Return     : None                                                                */
void check_comp_state(pst_header_t pHeader, const wchar_t * pCodes, int * pnLen, wchar_t * pResCodes, 
		    int * pnResLen, int nMaxLen)
{
   int nChar = 0, nCurChar = 0;
   int nItem, nItems;
   int nOffset = pHeader -> pComp -> nCurOffset;
   int nCpyLen;
   
   
   pcompose_item_t pItem = ( pcompose_item_t)((BYTE *)(pHeader -> pComp -> pComps) + /* start of area +     */
          pHeader -> pComp -> sect[pHeader -> pComp -> nCurSect].nOffset +        /* offset of section + */
	  pHeader -> pComp -> nCurOffset);                              /* current offset in section */
   pcompose_item_t pNewItem = NULL, 
                pFirstItem = pItem,
                pFoundItem = NULL;
   // Something has been already found. But we try to continue: 
   // maybe there are longer sequences with the same prefix
   if((pItem -> lengthIn > 0) &&
      (pItem -> lengthIn == pHeader -> pComp -> nCurLen))
      pFoundItem = pItem;
   
   for(nItem = pHeader -> pComp -> nCurItemNum,
       nItems =  pHeader -> pComp -> sect[pHeader -> pComp -> nCurSect].nComps;
       nItem < nItems;
       ++nItem)
      {
      /* every time except the first one                                           */
      if(nItem > pHeader -> pComp -> nCurItemNum) 
	   {
	   /* Move to the next composition descriptor                              */
           pNewItem = ( pcompose_item_t)&(pItem -> data[pItem -> lengthOut +
		      pItem -> lengthIn]);
	   nOffset += (BYTE *)pNewItem - (BYTE *)pItem;
	   pItem = pNewItem;
	   if(pHeader -> pComp -> nCurLen > 0) /* should we check it ?             */
	       {
	       if(pItem -> lengthIn <  pHeader -> pComp -> nCurLen)
		    continue;   /* too short                                        */
	       /* we quit after the first uncompare because strings are ordered!    */	  
	       if(memcmp(pFirstItem -> data, pItem -> data, 
		   sizeof(wchar_t) * pHeader -> pComp -> nCurLen))
		    break; /* no more string to compare                             */
		  
	       }
	   }
      /* If there are no chance to find something else, leave now                   */
      /* We have already found something  and                                       */
      /* (Length of the next item input sequence less or equal to length of the     */
      /* found item or next item input sequence doesn't start from the found item   */
      /* input sequence                                                             */
      if((NULL!= pFoundItem) && 
	 ((pItem -> lengthIn <= pFoundItem -> lengthIn) || 
	  memcmp(pItem -> data, pFoundItem -> data, (pFoundItem -> lengthIn) * sizeof(wchar_t))))
	  break;
	 
      for(nChar = pHeader -> pComp -> nCurLen, nCurChar = 0;
	   ;
           ++nChar, ++nCurChar)
           {
           if(nChar == pItem -> lengthIn) /* end of the stored compose sequence ?   */
	      {
	      pFoundItem = pItem;
	      break;
	      }

	   if(nCurChar >= *pnLen) /* No more characters in input pCodes string      */
              {
	      pHeader -> pComp -> nCurItemNum = nItem;
	      pHeader -> pComp -> nCurOffset  = nOffset;
              pHeader -> pComp -> nCurLen += *pnLen;
              *pnResLen = 0;
	      *pnLen = 0; /* all characters in the input string has been processed  */
              return;
              }

           if(pItem -> data[nChar] != pCodes[nCurChar]) /* has still been compared? */
	      { 
	      --nChar;
	      --nCurChar;
	      break;   /* No                                                        */
	      }
           }
   
      /* not compared; try next composed sequence                                   */	 
      }

   /* We come here in case of non-compare                                           */
   
   pHeader -> pComp -> nCurSect = -1; /* Return to the regular state                */
   if(NULL != pFoundItem) /* Not compared but something has been already found      */
     {
      /* result string is a replace string + the rest of the input string           */
      nCpyLen = min(pFoundItem -> lengthOut, nMaxLen);
      *pnResLen += nCpyLen;
      nMaxLen -= nCpyLen;
      memcpy(pResCodes, (void *)&(pFoundItem -> data[pFoundItem -> lengthIn]), 
	     nCpyLen * sizeof(wchar_t));
      if(nCurChar < *pnLen)   /* there is still something in input string           */
          {                   /* then modify input string                           */
	  nCpyLen = *pnLen - nCurChar;
          memmove((void *)pCodes, (void *)(pCodes + nCurChar), 
	         sizeof(wchar_t) * nCpyLen);
	  *pnLen = nCpyLen;
          }
      else
	  *pnLen = 0; 
      return; 
     }
   else
     {
      /* First copy to output buffer pResCodes "frozen" characters                   */
     if(pHeader -> pComp -> nCurLen > 0)
        {
        *pnResLen = min(pHeader -> pComp -> nCurLen, nMaxLen);
        memcpy(pResCodes, pFirstItem -> data, *pnResLen * sizeof(wchar_t));
        pResCodes += *pnResLen;
        nMaxLen -= *pnResLen;
        }
      /* then append the used part of input  string pCodes                            */
      if(nChar > 0)
	{
        nCpyLen = min(nChar, nMaxLen);
        *pnResLen += nCpyLen;
        *pnLen -= nCpyLen;
        memcpy(pResCodes, pCodes, nCpyLen * sizeof(wchar_t));
        /* And shift input buffer if there is still something left in it              */
	if(*pnLen > 0)
	   {
           memmove((void *)pCodes, (void *)(pCodes + nCpyLen), 
		   sizeof(wchar_t) * *pnLen);
	   }
	}
     }
}



void print_mod_state(pst_header_t pHeader, _uint64 modState)
{
   int i;
   for(i = 0; i < 64; ++i)
	{
	if(modState & 1)
	     {
	     printf("   - %s (%i)\n", pHeader -> pMod -> aModifiers[i].pModName, i);
	     }
	modState >>= 1;
	}
}

void init_comp_state(pst_header_t pHeader, int nSection) 
{
     pHeader -> pComp -> nModState = 0;
     pHeader -> pComp -> nCurSect = nSection;  /* We support now only Alt composed keys */  
     pHeader -> pComp -> nCurItemNum = 0;
     pHeader -> pComp -> nCurOffset = 0;
     pHeader -> pComp -> nCurLen = 0;
}

#if 0
void printLL(char * text, _uint64 val)
{
   printf("%s 0x%08x%08x\n", text, (int)(val >> 32),
        (int)(val & 0x00000000ffffffff));
}
#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/keymap/decoder.c $ $Rev: 657836 $" );
