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
 * hwi_add_synonym
 * 
 * Add synonym (or alias) <synonym_name> for the device item at <hwi_off>.
 * 
 * Returns: the hwinfo section offset of the synonym item (not the real item)
 * 			of HWI_NULL_OFF on error
*/
unsigned hwi_add_synonym(unsigned hwi_off, const char *synonym_name)
{
	hwi_tag *tag;
	unsigned owner = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_ROOT_HW, NULL);
	
	if ((hwi_off == HWI_NULL_OFF) || (owner == HWI_NULL_OFF)) { 
		return HWI_NULL_OFF;
	}

	tag = hwi_alloc_item(HWI_TAG_INFO(synonym), synonym_name, owner);
	tag->synonym.real = hwi_off;

	return hwi_tag2off(tag);
}

__SRCVERSION("$URL$ $Rev$");
