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
 *  scanusb.c
 *
 *  This is an implementation conversion problems from scan to usb and vice versa     
 */

#include <sys/devi.h>
#include "sys/usbcodes.h"

static USBKCode aScanToUSB[] = {
	KS_None, KS_Escape, KS_1, KS_2, KS_3, KS_4, KS_5, KS_6,	/* 0 */
	KS_7, KS_8, KS_9, KS_0, KS_minus, KS_equal, KS_BackSpace, KS_Tab,
	KS_q, KS_w, KS_e, KS_r, KS_t, KS_y, KS_u, KS_i,	/* 1 */
	KS_o, KS_p, KS_bracketleft, KS_bracketright, KS_Enter, KS_Control_L, KS_a,
		KS_s,
	KS_d, KS_f, KS_g, KS_h, KS_j, KS_k, KS_l, KS_semicolon,	/* 2 */
	KS_apostrophe, KS_grave, KS_Shift_L, KS_backslash, KS_z, KS_x, KS_c, KS_v,
	KS_b, KS_n, KS_m, KS_comma, KS_period, KS_slash, KS_Shift_R, KS_KP_Multiply,	/* 3 */
	KS_Alt_L, KS_space, KS_Caps_Lock, KS_f1, KS_f2, KS_f3, KS_f4, KS_f5,
	KS_f6, KS_f7, KS_f8, KS_f9, KS_f10, KS_Num_Lock, KS_Hold_Screen, KS_KP_Home,	/* 4 */
	KS_KP_Up, KS_KP_Prior, KS_KP_Subtract, KS_KP_Left, KS_KP_Begin, KS_KP_Right,
		KS_KP_Add, KS_KP_End,
	KS_KP_Down, KS_KP_Next, KS_KP_Insert, KS_KP_Delete, KS_Print_Screen, KS_None, KS_NonUSSlash, KS_f11,	/* 5 */
	KS_f12, KS_f13, KS_None, KS_None, KS_Internat6, KS_f14, KS_f15, KS_f16,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* 6 */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_Internat2, KS_None, KS_None, KS_Internat1, KS_None, KS_None, KS_Lang5, KS_Lang4,	/* 7 */
	KS_Lang3, KS_Internat4, KS_None, KS_Internat5, KS_None, KS_Internat3,
		KS_KP_Comma, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* 8 */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* 9 */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* a */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* b */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* c */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* d */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,	/* e */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None,
	KS_None, KS_Lang2, KS_Lang1, KS_None, KS_None, KS_None, KS_None, KS_None,	/* f */
	KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None, KS_None
};

#define MAX_USB_CODE (KS_RightGUI)

_uint8 *aUsbToScan = NULL;

typedef struct _scanToUSB_Exception
{
	BYTE *pScanCodes;
	USBKCode USBCode;
}
scanToUSB_Exception, *pScanToUSB_Exception;

static scanToUSB_Exception excTable[] = {
	{(BYTE *)"\x2b", (USBKCode)KS_NonUSNumsign},		/* This is an Euro keyboard exception: the same scan code as KS_backslash      */
	/* was not included to aScanToUSB array; encoding from scan to USB is ambigous */
	/* That is why we always return KS_backslash for 0x2b scan code. On the other  */
	/* hand, while encoding from USB to scan we have correct scan code for both USB */
	/* codes                                                                       */
	{(BYTE *)"\xe0\x1c", (USBKCode)KS_KP_Enter},
	{(BYTE *)"\xe0\x1d", KS_Control_R},
	{(BYTE *)"\xe0\x2a", KS_None},				/* Dead scan code  */
	{(BYTE *)"\xe0\x35", KS_KP_Divide},
	{(BYTE *)"\xe0\x37", KS_Print_Screen},	/* Actualy it is 0xe02ae037 (make) and 0xe0b7e0aa (breake);    */
	/* we drop 0xe0aa and 0xe02a because these are dead scan codes */
	{(BYTE *)"\xe0\x38", KS_Alt_R},
	{(BYTE *)"\xe0\x46\xe0\xc6", KS_Pause},
	{(BYTE *)"\xe0\x47", KS_Home},
	{(BYTE *)"\xe0\x48", KS_Up},
	{(BYTE *)"\xe0\x49", KS_PgUp},
	{(BYTE *)"\xe0\x4b", KS_Left},
	{(BYTE *)"\xe0\x4d", KS_Right},
	{(BYTE *)"\xe0\x4f", KS_End},
	{(BYTE *)"\xe0\x50", KS_Down},
	{(BYTE *)"\xe0\x51", KS_PgDn},
	{(BYTE *)"\xe0\x52", KS_Insert},
	{(BYTE *)"\xe0\x53", KS_Delete},
	{(BYTE *)"\xe0\x5b", KS_LeftGUI},
	{(BYTE *)"\xe0\x5c", KS_RightGUI},
	{(BYTE *)"\xe0\x5d", KS_Application},
	{(BYTE *)"\xe1\x1d\x45\xe1\x9d\xc5", KS_Pause},
/* Dead scan codes                                                                  */
	{(BYTE *)"\xe0\xaa", KS_None},
	{(BYTE *)"\xe0\xb6", KS_None},
	{(BYTE *)"\xe0\x36", KS_None}

};

static USBKCode getUSBCodeFromScanCodes (BYTE * nScanCodes,
																				 int nNumScanCodes);
static USBKCode searchExceptionTable (BYTE * nScanCodes, int nNumScanCodes,
																			int *pnCompareLength);



/* Description: decodes scan codes sequences to single USB code                     */
/* Input      : BYTE * nScanCodes - scan codes sequence                             */
/*              int nNumScanCodes - its length                                      */
/* Output     :                                                                     */
/* Return     : USB code or KS_None if illegal                                      */
USBKCode
getUSBCodeFromScanCodes (BYTE * nScanCodes, int nNumScanCodes)
{
	USBKCode code = KS_None;
	int nCompareLength;

	/* one scan code - resolve it using table                                        */
	if (1 == nNumScanCodes)
		code = aScanToUSB[*nScanCodes];
	if (KS_None == code) {
		/* maybe this is an exception                                                  */
		code = searchExceptionTable (nScanCodes, nNumScanCodes, &nCompareLength);
		if (nNumScanCodes != nCompareLength)
			code = KS_None;
	}

	return code;
}


/* Description: look up to exception table for scan sequese translation             */
/* Input      : BYTE * nScanCodes - scan codes sequence                             */
/*              int nNumScanCodes - its length                                      */
/* Output     : int * pnCompareLength - number of used scan codes if result is      */
/*              positive                                                            */
/* Return     : USB code or KS_None if not found                                    */
USBKCode
searchExceptionTable (BYTE * nScanCodes, int nNumScanCodes,
											int *pnCompareLength)
{
	int i, j, k;


	*pnCompareLength = 0;
	for (i = 0, j = ARRAY_SIZE (excTable); i < j; ++i) {
		for (k = 0; k < nNumScanCodes; ++k) {
			if (excTable[i].pScanCodes[k] != *(nScanCodes + k))
				break;
			if (0 == excTable[i].pScanCodes[k])
				break;
		}
		if (0 == excTable[i].pScanCodes[k]) {	/* Compared ?                              */
			*pnCompareLength = k;
			return excTable[i].USBCode;	/* Yes                                     */
		} else if (k == nNumScanCodes)
			break;
	}

	return KS_None;
}


/* Description: function translates scan codes sequence to USB usage                */
/* In/Out     : unsigned char * buf - scan codes; before returning all processed    */
/*              scan codes have to be removed from buf                              */
/*              int * pnBytes - buf length (before and after)                       */
/*              int *pnLen - max result length (in) / result length (out)           */
/* Output     : USBKCode * usbCodes - translation results                           */
/*              unsigned char * makeBreakes array of make/break codes               */
/* Return     : EOK if ok, error code if fails                                      */
int
devi_scan_to_usb (unsigned char *buf, int *pnBytes, USBKCode * usbCodes,
									unsigned char *makeBreakes, int *pnLen)
{
	int nPos = 0;
	int nUsbPos = 0;
	USBKCode code;
	int nCompareLength;



	while (nPos < *pnBytes) {
		if ((0xe0 == buf[nPos]) ||	// This is positively a scan codes combintion
				(0xe1 == buf[nPos])) {
			makeBreakes[nUsbPos] = ((0x80 & *(buf + nPos + 1)) ? 0 : 1);	// Is it make or break?
			*(buf + nPos + 1) &= 0x7f;	// We don't need any more make/break flag
			code =
				searchExceptionTable (buf + nPos, *pnBytes - nPos, &nCompareLength);
			if (0 == nCompareLength)	// The sequence of scan codes was not resolved yet; 
			{													// probably it has still not completed
				if (1 == makeBreakes[nUsbPos])	// Restore make/break flag for future analysis
					*(buf + nPos + 1) |= 0x80;
				memmove (buf, buf + nPos, *pnBytes - nPos);	// Remove already precessed scan codes from buf
				*pnBytes -= nPos;
				*pnLen = nUsbPos;
				return EOK;
			}
			// Yes, it's resolved 
			usbCodes[nUsbPos] = code;	// Take USB code
			nPos += nCompareLength;		// And move forward in buf
		} else											// Regular scan code
		{
			makeBreakes[nUsbPos] = ((0x80 & *(buf + nPos)) ? 0 : 1);	// Analize make/break
			*(buf + nPos) &= 0x7f;		// And remove make/break flag
			usbCodes[nUsbPos] = getUSBCodeFromScanCodes (buf + nPos, 1);	// Decode to USB
			++nPos;
		}
		if (KS_None != usbCodes[nUsbPos])
			++nUsbPos;
		if (nUsbPos == *pnLen)
			break;

		//  printf("nBytes = %i, buf = 0x%08x, usb = 0x%04hx\n", nBytes,  *(int *)(tmpBuf + nPos), usbCodes[nUsbPos]);
	}

	*pnBytes = 0;									// "Empty" input buffer

	*pnLen = nUsbPos;

	return EOK;
}


/* Description: function translates scan codes sequence to USB usage                */
/* Input      : USBKcode usbCode - code to translate                                */
/*              int *pnLen - max result length (in) / result length (out)           */
/* Output     : _uint8 * buf - translation results                                  */
/* Return     : EOK if OK, error code if fails                                      */
int
devi_usb_to_scan (USBKCode usbCode, int nMakeBreak, _uint8 * buf, int *pnLen)
{
	int i;

	if (NULL == aUsbToScan)				// build translation array - do it one time 
	{
		aUsbToScan = malloc (MAX_USB_CODE + 1);
		if (NULL == aUsbToScan)
			return ENOMEM;
		memset (aUsbToScan, 0, MAX_USB_CODE + 1);
		for (i = 0; i < ARRAY_SIZE (aScanToUSB); ++i) {
			if (aScanToUSB[i] != KS_None)
				aUsbToScan[aScanToUSB[i]] = i;
		}
	}
	if (usbCode >= MAX_USB_CODE)
		return EINVAL;
	if (0 != aUsbToScan[usbCode])	// direct translation
	{
		*buf = aUsbToScan[usbCode];
		if (0 == nMakeBreak)				// Released
			*buf |= 0x80;
		*pnLen = 1;
		return EOK;
	}
	// look up to the exception table
	for (i = 0; i < ARRAY_SIZE (excTable); ++i) {
		if (excTable[i].USBCode == usbCode) {
			int nLen = strlen ((char *)(excTable[i].pScanCodes));

			if (nLen > *pnLen)
				return ERANGE;
			strcpy ((char *)buf, (char *)(excTable[i].pScanCodes));
			if (0 == nMakeBreak)			// Key released
			{
				if (nLen > 2)						// This is pause - doesn't have break code
					return EINVAL;
				*(buf + 1) |= 0x80;
			}
			*pnLen = nLen;
			return EOK;
		}
	}
	return EINVAL;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/scanusb.c $ $Rev: 657836 $" );
