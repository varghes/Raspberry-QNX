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

void 
hwi_add_device(const char *bname, const char *cname, const char *dname, unsigned pnp) {
	hwi_tag		*tag;
	unsigned	bus;
	unsigned	class;

	bus = hwi_find_item(HWI_NULL_OFF, bname, NULL);
	if(bus == HWI_NULL_OFF) {
		unsigned off = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_ROOT_HW, NULL);

		tag = hwi_alloc_item(HWI_TAG_INFO(bus), bname, off);
		bus = hwi_tag2off(tag);
	}
	class = hwi_find_item(HWI_NULL_OFF, bname, cname, NULL);
	if(class == HWI_NULL_OFF) {
		tag = hwi_alloc_item(HWI_TAG_INFO(group), cname, bus);
		class = hwi_tag2off(tag);
	}
	tag = hwi_alloc_item(HWI_TAG_INFO(device), dname, class);
	tag->device.pnpid = pnp;
}

__SRCVERSION("hwi_add_device.c $Rev: 657836 $");
