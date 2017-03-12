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
#include <arm/sa1100.h>

unsigned long
rtc_time_sa1100(unsigned rttr)
{
	unsigned	tmp;

	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_RTC, "sa1100", 0);

	/*
	 * FIXME: disable the RTC and 1HZ interrupts?
	 *        According to the manual, these are undefined on reset
	 */
	out32(SA1100_RTC_BASE + SA1100_RTSR, 0);

	/*
	 * Check if the IPL has set the RTTR.
	 */
	tmp = in32(SA1100_RTC_BASE + SA1100_RTTR);
	if (tmp == 0) {
		/*
		 * RTTR is not set - the RTC has been clocked at 32.768KHz since
		 * the reset, so the time read below is going to very wrong...
		 */
		if (rttr == 0) {
			/*
			 * Set default value for perfect 32.768KHz crystal
			 */
			rttr = 0x7fff;
		}
		out32(SA1100_RTC_BASE + SA1100_RTTR, rttr);
	}

	/*
	 * Read counter register
	 */
	return in32(SA1100_RTC_BASE + SA1100_RCNR);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_rtc_sa1100.c $ $Rev: 657836 $" );
