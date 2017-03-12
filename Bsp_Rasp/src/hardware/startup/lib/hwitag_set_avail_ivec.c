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
#include <drvr/hwinfo.h>


/*******************************************************************************
* hwitag_set_avail_ivec
* 
* if the vector tag <ivec_idx> is free for the device corresponding to <hwi_off>,
* then set that tag to <vector>. If <ivec_idx> < 0, then the next available
* HWI_TAG_NAME_irq tag that is free will be used. This function will only add
* the vector to a free tag (ie. current value == HWI_ILLEGAL_VECTOR). If no free
* tags are found, the function will fail. You can use hwi_set_ivec() to force a
* known value for a specific tag
*  
* Returns: -1 on error or the vector tag index that was set (base 0)
* 
*/ 
int hwitag_set_avail_ivec(unsigned hwi_off, int ivec_idx, uint32_t vector)
{
	unsigned tag_idx = (ivec_idx < 0) ? 0 : ivec_idx;
	hwi_tag *tag;

	while ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, &tag_idx)) != NULL)
	{
		/* if we find a free tag, use it */
		if (tag->irq.vector == HWI_ILLEGAL_VECTOR) {
			tag->irq.vector = vector;
			if (debug_flag >= 2) {
				hwi_tag *device_tag = hwi_off2tag(hwi_off);
				kprintf("%s%d, ivec idx %d == %d\n", __hwi_find_string(device_tag->item.itemname),
							hwi_find_unit(hwi_off), tag_idx - 1, tag->irq.vector);
			}
			return (tag_idx - 1);
		}
		/* if the caller wanted to add at a specific index, we have failed
		 * otherwise keep searching for a free tag */
		if (ivec_idx >= 0) break;
	}
	return -1;
}
	
__SRCVERSION("$URL$ $Rev$");
