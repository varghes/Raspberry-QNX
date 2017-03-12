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
//#include <hw/m48t5x.h>

//
//  RTC code to drive the SGS-Thomson M48T59 rtc/nvram chip
//
// Normally, the registers are mapped as:
// Low addr: 	offset 0
// High addr:	offset 1
// Data:		offset 3
//

#define M48T5X_TIME_REGS 0x1ff0

static int
rdcmos(unsigned i) {
	chip_write8(0, i & 0xff);			// Low 8 bits of addr
	chip_write8(1, (i >> 8) & 0xff);	// High 8 bits
 	return(chip_read8(3));				// Read data
}

unsigned long
rtc_time_m48t5x(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;

	//Tell Neutrino what kind of chip for 'rtc' utility
	hwi_add_rtc("m48t5x", base, reg_shift, 4, mmap, cent_reg);

	chip_access(base, reg_shift, mmap, 4);

	do {
		tm.tm_sec  = rdcmos(M48T5X_TIME_REGS + 0x9);
		tm.tm_min  = rdcmos(M48T5X_TIME_REGS + 0xa);
		tm.tm_hour = rdcmos(M48T5X_TIME_REGS + 0xb);
		tm.tm_mday = rdcmos(M48T5X_TIME_REGS + 0xd);
		tm.tm_mon  = rdcmos(M48T5X_TIME_REGS + 0xe);
		tm.tm_year = rdcmos(M48T5X_TIME_REGS + 0xf);

		//Loop while time inconsistent
	} while(tm.tm_sec != rdcmos(M48T5X_TIME_REGS + 0x9));

	chip_done();

	tm.tm_sec &= ~0x80;

	tm.tm_sec  = bcd2bin(tm.tm_sec);
	tm.tm_min  = bcd2bin(tm.tm_min);
	tm.tm_hour = bcd2bin(tm.tm_hour);
	tm.tm_mday = bcd2bin(tm.tm_mday);
	tm.tm_mon  = bcd2bin(tm.tm_mon);
	tm.tm_year = bcd2bin(tm.tm_year);

	tm.tm_mon -= 1;

	if(tm.tm_year < 70) {
		tm.tm_year += 100; //21st century.
	}

	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_m48t5x.c $Rev: 657836 $");
