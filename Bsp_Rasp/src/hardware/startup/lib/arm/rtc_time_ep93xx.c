/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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

/* read the realtime clock in the Cirrus Logic Ep93xx family of SOC devices */

#include "startup.h"
#include <hw/hwinfo_ep93xx.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <sys/mman.h>
#include <errno.h>

unsigned long rtc_time_ep93xx(void)
{
	unsigned hwi_off = hwi_find_device(EP93xx_HWI_RTC, 0);
	unsigned long t = 0;

	if (hwi_off != HWI_NULL_OFF)
	{
		hwiattr_rtc_t attr;

		if ((hwiattr_get_rtc(hwi_off, &attr) == EOK) &&
			(attr.common.location.base != 0) && (attr.common.location.len > 0))
		{
			/* read the device registers and extract the time */
			volatile uint32_t *reg = (uint32_t *)startup_io_map(attr.common.location.len, attr.common.location.base);
			if (reg != MAP_FAILED)
			{
				t = (unsigned long)reg[0];	// data register is at offset 0
				startup_io_unmap((uintptr_t)reg);
			}
		}
	}
	if ((debug_flag) && (t > 0)) kprintf("RTC val: %d\n", t);
	return t;
}
