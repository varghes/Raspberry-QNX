/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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

/*******************************************************************************
 * hwidev_add
 * 
 * adds the device <device_name> to the bus <bus_hwi_off>. We ignore the class
 * specifier
 * 
 * This function corrects the limitations of hwi_add_device() which does not
 * allow adding devices to specific bus instances
 * 
 * Returns: the hwi_off for the added device or HWI_NULL_OFF on error
*/
unsigned hwidev_add(const char *device_name, unsigned pnp, unsigned bus_hwi_off)
{
	unsigned hwi_off = HWI_NULL_OFF;

	/* if no bus is provided for the device or the bus does not exist, it will get added to HWI_ITEM_BUS_UNKNOWN */
	if ((bus_hwi_off != HWI_NULL_OFF) ||
		(bus_hwi_off = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_ROOT_HW, HWI_ITEM_BUS_UNKNOWN, NULL)) != HWI_NULL_OFF)
	{
		hwi_tag *tag = hwi_alloc_item(HWI_TAG_INFO(device), device_name, bus_hwi_off);
		if (tag != NULL)
		{
			tag->device.pnpid = pnp;
			hwi_off = hwi_tag2off(tag);
		}
	}
	return hwi_off;
}

__SRCVERSION("hwidev_add.c $Rev: 169805 $");
