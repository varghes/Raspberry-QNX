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

#include "startup.h"
#include "hwinfo_private.h"
#include <drvr/hwinfo.h>


/*******************************************************************************
* hwibus_add_usb
* 
* See sys/hwinfo.h for interface description
* 
* Returns: the offset in the hwinfo section of the added bus or HWI_NULL_OFF
* 			on error
*/ 
unsigned hwibus_add_usb(unsigned parent_hwi_off, hwiattr_usb_t *attr)
{
	unsigned hwi_off = hwibus_add(HWI_ITEM_BUS_USB, parent_hwi_off);

	if ((hwi_off != HWI_NULL_OFF) && (attr != NULL))
	{
		hwitag_add_common(hwi_off, &attr->common);
	}
	return hwi_off;
}

__SRCVERSION("hwibus_add_usb.c $Rev: 169805 $");
