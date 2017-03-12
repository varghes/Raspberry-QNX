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
#include <arm/pxa250.h>

unsigned long
rtc_time_pxa250(unsigned rttr)
{
	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_RTC, "pxa250", 0);

	/*
	 * Set the RTTR if necessary.
	 * The h/w reset default is for a perfect 32.768KHz crystal.
	 */
	if (rttr != 0) {
		out32(PXA250_RTC_BASE + PXA250_RTTR, rttr);
	}

	/*
	 * Read counter register.
	 * The manual says the RTSR interrupts bits are cleared on reset.
	 */
	return in32(PXA250_RTC_BASE + PXA250_RCNR);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_rtc_pxa250.c $ $Rev: 657836 $" );
