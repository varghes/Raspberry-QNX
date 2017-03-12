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
 * Clock setup for DS1743 part.

 * addresses that we use (all fields are BCD)

 0 - 0x1ff7		NVRAM
 0x1ff8			control & century bits (0-39)
 0x1ff9			OSC & seconds (0-59)
 0x1ffa			minute (0-59)
 0x1ffb			hour (0-23)
 0x1ffc			day of week (1-7)
 0x1ffd			date (1-31)
 0x1ffe			month (1-12)
 0x1fff			year (0-99)
*/

#define DS1743_CONTROL		0x1ff8
#define DS1743_SECONDS		0x1ff9
#define DS1743_MINUTES		0x1ffa
#define DS1743_HOUR			0x1ffb
#define DS1743_DAY			0x1ffc
#define DS1743_DATE			0x1ffd
#define DS1743_MONTH		0x1ffe
#define DS1743_YEAR			0x1fff

#define DS1743_CONTROL_R			0x40
#define DS1743_CONTROL_W			0x80
#define DS1743_CONTROL_CENT_MASK	0x3f

#define DS1743_SECONDS_OSC			0x80
#define DS1743_SECONDS_MASK			0x7f

#define DS1743_DAY_FT				0x40
#define DS1743_DAY_BF				0x40
#define DS1743_DAY_MASK				0x07

#define DS1743_MINUTES_MASK			0x7f
#define DS1743_HOUR_MASK			0x3f
#define DS1743_DATE_MASK			0x3f
#define DS1743_MONTH_MASK			0x1f


unsigned long
rtc_time_ds1743(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;
	unsigned	cent;
	unsigned	reg;

	//Tell Neutrino what kind of chip for 'rtc' utility
	hwi_add_rtc("ds1743", base, reg_shift, DS1743_YEAR+1, mmap, -1);

	chip_access(base, reg_shift, mmap, DS1743_YEAR+1);

	// Stop the chip from updating
	chip_write8(DS1743_CONTROL, chip_read8(DS1743_CONTROL) | DS1743_CONTROL_R);

	reg = chip_read8(DS1743_SECONDS);
	if(reg & DS1743_SECONDS_OSC) {
		// clock oscillator not running
		chip_write8(DS1743_SECONDS, reg & ~DS1743_SECONDS_OSC);
	}
	reg = chip_read8(DS1743_DAY);
	if(reg & DS1743_DAY_FT) {
		// need to turn off frequency test mode
		chip_write8(DS1743_DAY, reg & ~DS1743_DAY_FT);
	}

	// convert BCD to binary 
	tm.tm_sec 	= bcd2bin(chip_read8(DS1743_SECONDS) & DS1743_SECONDS_MASK);
	tm.tm_min 	= bcd2bin(chip_read8(DS1743_MINUTES) & DS1743_MINUTES_MASK);
	tm.tm_hour	= bcd2bin(chip_read8(DS1743_HOUR) & DS1743_HOUR_MASK);
	tm.tm_mday	= bcd2bin(chip_read8(DS1743_DATE) & DS1743_DATE_MASK);
	tm.tm_mon	= bcd2bin(chip_read8(DS1743_MONTH) & DS1743_MONTH_MASK) - 1;
	tm.tm_year	= bcd2bin(chip_read8(DS1743_YEAR));
	cent		= bcd2bin(chip_read8(DS1743_CONTROL) & DS1743_CONTROL_CENT_MASK);

	// Start the chip updating again
	chip_write8(DS1743_CONTROL, chip_read8(DS1743_CONTROL) & ~DS1743_CONTROL_R);

	tm.tm_year += (cent-19) * 100;

	chip_done();

	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_ds1743.c $Rev: 657836 $");
