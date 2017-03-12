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

#define AMPM			0x40	/* clock in 12 hour AM/PM format */
#define MILTIME			0x00	/* clock in 24 hour format */
#define TRANSFER_ENABLE	0x80	/* enable transfer counters -> registers */
#define CMD_REG			11

/*
 * Clock setup for DS1386 part. 

 * addresses that we use (all fields are BCD)

 0 hundreths of seconds
 1 seconds
 2 minutes
 4 hours & hour format
 6 day of week (we set this up but do not use it)
 8 day
 9 month
 A year (0-99)
 B command register 
 E user area 2 (OS reserved)
 10 - 7FFF (7FFF) user area 32K (8K) battery backed up SRAM
*/

unsigned long
rtc_time_ds1386(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;
	unsigned	cent;

	//Tell Neutrino what kind of chip for 'rtc' utility
	hwi_add_rtc("ds1386", base, reg_shift, 16, mmap, cent_reg);

	chip_access(base, reg_shift, mmap, 16);

   	do {
		// convert BCD to binary 
		tm.tm_sec 	= bcd2bin(chip_read8(1));		// seconds
		tm.tm_min 	= bcd2bin(chip_read8(2));		// minutes
		tm.tm_hour	= bcd2bin(chip_read8(4) & 0x3f);	// hours
		tm.tm_mday	= bcd2bin(chip_read8(8));		// day
		tm.tm_mon	= bcd2bin(chip_read8(9) & 0x3f) - 1;	// month
		tm.tm_year	= bcd2bin(chip_read8(10));		// year

		//Loop while time inconsistent
	} while(tm.tm_sec != bcd2bin(chip_read8(1)));

	if(cent_reg >= 0) {
		cent = bcd2bin(chip_read8(cent_reg));		// century
		if(cent == 20) tm.tm_year += 100;
	} else if(tm.tm_year < 70) {
		tm.tm_year += 100;
	}

	chip_done();


	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_ds1386.c $Rev: 657836 $");
