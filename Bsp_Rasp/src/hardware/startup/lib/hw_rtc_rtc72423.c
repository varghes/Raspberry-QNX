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





/*
 * This is the version which deals with the FOX RTC-72423 part. 
 */
#include "startup.h"

/* register D */
#define SECADJ			0x80	/* 30 sec adj clock */
#define IRQFLAG			0x40	/* IRQ reset/flag */
#define BUSY			0x20	/* clock in transferring data */
#define HOLD			0x10	/* stop clock transfer */

/* register E */
#define T0				0x80	/* interrupt source */
#define T1				0x40	/* interrupt source */
#define ITRPT			0x20	/* interrupt mode */
#define MASK			0x10	/* interrupt mask */

/* register F */
#define TEST			0x80	/* 30 sec adj clock */
#define AMPM			0x40	/* clock in 12 hour AM/PM format */
#define STOP			0x20	/* stop the clock divider */
#define RESET			0x10	/* reset the clock chip register */

#define REG_D			0xd
#define REG_E			0xe
#define REG_F			0xf

/*
 * Clock setup for FOX RTC-72423 part. This is an embedded part so we can use
 * what ever conventions we want for the year registers. Note there is no
 * Century byte or other ram on this so we use the year between 0-70 to
 * indicate if it is the next century/
 *
 * addresses that we use (all fields are BCD)

 0 seconds 1
 1 seconds 10
 2 minutes 1
 3 minutes 10
 4 hours 1 
 5 hours 10 & hour format
 6 day 1
 7 day 10
 8 month 1
 9 month 10
 A year 1
 B year 10
 C day of week (we set this up but do not use it)
 D command register 
 E interrup control
 F command register 
 */

unsigned long
rtc_time_rtc72423(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;
	unsigned	bottom;

	//Tell Neutrino what kind of chip for 'rtc' utility
	hwi_add_rtc("rtc72423", base, reg_shift, 16, mmap, cent_reg);

	chip_access(base, reg_shift, mmap, 16);

   	do {
		// get the data
		bottom = chip_read8(0);
		tm.tm_sec	= bcd2bin((chip_read8(1)  <<4) | bottom);
		tm.tm_min	= bcd2bin((chip_read8(3)  <<4) | chip_read8(2));
		tm.tm_hour	= bcd2bin(((chip_read8(5) & 3)  <<4) | chip_read8(4));
		tm.tm_mday	= bcd2bin((chip_read8(7)  <<4) | chip_read8(6));
		tm.tm_mon	= bcd2bin((chip_read8(9)  <<4) | chip_read8(8));
		tm.tm_year	= bcd2bin((chip_read8(11) <<4) | chip_read8(10));

		//Loop while time inconsistent
	} while(bottom != chip_read8(0));

	chip_done();

	if(tm.tm_year < 70) tm.tm_year += 100;
	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_rtc72423.c $Rev: 657836 $");
