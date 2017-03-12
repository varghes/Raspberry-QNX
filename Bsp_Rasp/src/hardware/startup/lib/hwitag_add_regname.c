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
* hwitag_add_regname
* 
* add the regname tag to device <hwi_off> and and set the tag fields as
* specified in the remaining parameters.
* 
* Note that the <hwi_off> parameter is ignored because there is currently no
* way to add tags to an existing item. The API provides for the removal of this
* restriction in the future
*  
* Returns: n/a
* 
*/ 
void hwitag_add_regname(unsigned hwi_off, const char *name, unsigned regoffset)
{
	hwi_tag		*tag;
	unsigned	string_idx = add_string(name);

	tag = hwi_alloc_tag(HWI_TAG_INFO(regname));
	tag->regname.regname = string_idx;
	tag->regname.offset = regoffset;
}

__SRCVERSION("hwitag_add_regname.c $Rev: 169805 $");
