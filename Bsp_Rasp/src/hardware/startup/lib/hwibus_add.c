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
 * hwibus_add
 * 
 * adds the bus <bus_name> to the device (or bus) <parent_hwi_off>. We ignore
 * the class specifier
 * 
 * Returns: the hwi_off for the added bus or HWI_NULL_OFF on error
 * 
 * Note that although we used the name 'bus' to be consistent with previous
 * nomenclature, from a topology perscpective, we actually mean any interconnect
 * to which a device is connected
*/
unsigned hwibus_add(const char *bus_name, unsigned parent_hwi_off)
{
	unsigned hwi_off = HWI_NULL_OFF;

	/* if no parent is provided for the bus, it will get added to HWI_ITEM_ROOT_HW */
	if ((parent_hwi_off != HWI_NULL_OFF) ||
		(parent_hwi_off = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_ROOT_HW, NULL)) != HWI_NULL_OFF)
	{
		hwi_tag *tag = hwi_alloc_item(HWI_TAG_INFO(bus), bus_name, parent_hwi_off);
		if (tag != NULL) hwi_off = hwi_tag2off(tag);
	}
	return hwi_off;
}

__SRCVERSION("hwibus_add.c $Rev: 169805 $");
