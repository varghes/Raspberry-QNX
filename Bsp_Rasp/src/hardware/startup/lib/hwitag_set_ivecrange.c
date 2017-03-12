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
 * hwi_set_ivecrange
 * 
 * Unconditionally set the interrupt vector range attribute for the device
 * corresponding to <hwi_off>.
 * The API allows for the setting of a specific range block in the case where
 * there could be more than block of interrupt vectors per device. Typically
 * this would not be the case and <ivecrange_idx> will be 0 indicating the first
 * instance
 * 
 * Returns: true if the specified tag was successfully set, otherwise false 
 * 
*/
int hwitag_set_ivecrange(unsigned hwi_off, unsigned ivecrange_idx, unsigned start_vector, unsigned num)
{
	hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irqrange, &ivecrange_idx);
	if (tag != NULL) {
		tag->irqrange.irq = start_vector;
		tag->irqrange.num = num;
	}
	return (tag == NULL) ? 0 : 1;
}

__SRCVERSION("$URL$ $Rev$");
