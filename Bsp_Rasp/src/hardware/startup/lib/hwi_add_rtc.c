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
hwi_add_rtc(const char *name, paddr_t base, unsigned reg_shift, unsigned len,
				int mmap, int cent_reg) {

	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_RTC, name, 0);
	hwi_add_location(base, len << reg_shift, reg_shift, hwi_find_as(base, mmap));
	if(cent_reg != -1) {
		hwi_tag	*cent = hwi_alloc_tag(HWI_TAG_INFO(regname));

		cent->regname.regname = 0;
		cent->regname.offset = cent_reg;
	}
}

__SRCVERSION("hwi_add_rtc.c $Rev: 657836 $");
