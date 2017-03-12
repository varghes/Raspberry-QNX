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








#ifndef __USB_H_INCLUDED__
#define __USB_H_INCLUDED__


#include <sys/usbdi.h>

typedef struct _controlRequest 
{
   _uint8 dir ;    /* Direction (URB_DIR_IN or URB_DIR_OUT)                           */
   _uint8 type;    /* Type (USB_TYPE_STANDARD, USB_TYPE_CLASS or USB_TYPE_VENDOR)     */
   _uint8 rec ;    /* Recipient (USB_RECIPENT_DEVICE, _INTERFACE, _ENDPOINT, _OTHER)  */
   _uint8 req ;    /* Request - vendor specific or one of standard                    */
   _uint16 value;  /* Command parameter                                               */
   _uint16 index;  /* Command index( request specific                                 */
} controlRequest, * pControlRequest;

/* Device status flags                                                                */
#define USB_DEVICE_STATUS_FLAGS            (0xff)
#define USB_DEVICE_ON                      (0x01)   
#define USB_DEVICE_PRESENT                 (0x02)

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/usb.h $ $Rev: 657836 $" )
