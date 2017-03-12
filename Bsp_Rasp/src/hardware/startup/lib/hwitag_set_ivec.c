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
 * hwi_set_ivec
 * 
 * Unconditionally set the interrupt vector attribute for the device
 * corresponding to <hwi_off>.
 * The API allows for the setting of a specific vector in the case where there
 * are more than one interrupt vector per device. 
 *  
 * Returns: true if the specified tag was successfully set, otherwise false 
 * 
*/
int hwitag_set_ivec(unsigned hwi_off, unsigned ivec_idx, unsigned vector)
{
	hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, &ivec_idx);
	if (tag != NULL)
	{
		tag->irq.vector = vector;
		if (debug_flag >= 2) {
			hwi_tag *device_tag = hwi_off2tag(hwi_off);
			kprintf("%s%d, ivec idx %d == %d\n", __hwi_find_string(device_tag->item.itemname),
						hwi_find_unit(hwi_off), ivec_idx - 1, tag->irq.vector);
		}
	}
	return (tag == NULL) ? 0 : 1;
}

__SRCVERSION("$URL$ $Rev$");
