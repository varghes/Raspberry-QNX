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

/*
 * Clock setup for DS1501/1511 part.
 * addresses that we use (all fields are BCD)
 *
 * 0 seconds
 * 1 minutes
 * 2 hours
 * 3 day of week (we set this up but do not use it)
 * 4 day
 * 5 month
 * 6 year (0-99)
 * 7 century
 * f control b register
 */

#define	TRANSFER_ENABLE	(1 << 7)

unsigned long
rtc_time_ds15x1(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;
	int			centry;

	/*
	 * Tell Neutrino what kind of chip for 'rtc' utility
	 */
	hwi_add_rtc("ds15x1", base, reg_shift, 16, mmap, 0x07);

	chip_access(base, reg_shift, mmap, 16);

	/*
	 * Set TE bit
	 */
	chip_write8(0xf, chip_read8(0xf) | TRANSFER_ENABLE);

	do {
		tm.tm_sec  = chip_read8(0);
		tm.tm_min  = chip_read8(1);
		tm.tm_hour = chip_read8(2);
		tm.tm_mday = chip_read8(4);
		tm.tm_mon  = chip_read8(5);
		tm.tm_year = chip_read8(6);
		centry     = chip_read8(7);

		/*
		 * Loop while time inconsistent
		 */
	} while(tm.tm_sec != chip_read8(0));

	chip_done();

	tm.tm_mon &= 0x1F;

	tm.tm_sec  = bcd2bin(tm.tm_sec);
	tm.tm_min  = bcd2bin(tm.tm_min);
	tm.tm_hour = bcd2bin(tm.tm_hour);
	tm.tm_mday = bcd2bin(tm.tm_mday);
	tm.tm_mon  = bcd2bin(tm.tm_mon);
	tm.tm_year = bcd2bin(tm.tm_year);

	if (centry == 0x20)
		tm.tm_year += 100;

	tm.tm_mon -= 1;

	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_ds15x1.c $Rev: 657836 $");
