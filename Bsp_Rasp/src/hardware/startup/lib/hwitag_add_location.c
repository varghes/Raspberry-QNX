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
* hwitag_add_location
* 
* add the location tag to device <hwi_off> and set the tag fields as specified
* in remaining arguments.
* 
* Note that the <hwi_off> parameter is ignored because there is currently no
* way to add tags to an existing item. The API provides for the removal of this
* restriction in the future
*  
* Returns: -1 on error or the vector tag index that was set (base 0)
* 
*/
void hwitag_add_location(unsigned hwi_off, paddr_t base, paddr_t len, unsigned reg_shift, unsigned addr_space)
{
	hwi_tag		*tag;

	tag = hwi_alloc_tag(HWI_TAG_INFO(location));
	tag->location.base = base;
	tag->location.len = len;
	tag->location.regshift = reg_shift;
	tag->location.addrspace = addr_space;
}

__SRCVERSION("hwitag_add_location.c $Rev: 657836 $");
