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
#include <hw/mc146818.h>

//
// Read a Motorola 146818 compatable RTC (IBM AT standard).
//

static int
rdcmos(unsigned i) {
	chip_write8(0, i);			//CMOS addr reg
	return(chip_read8(1));		//CMOS data reg
}

static int inline check_uip_clr (void)
{
	return (rdcmos(MC146818_SRA) & MC146818_SRA_UIP) == 0;
}

unsigned long
rtc_time_mc146818(paddr_t base, unsigned reg_shift, int mmap, int cent_reg) {
	struct tm	tm;
	unsigned	save_hour;
	unsigned	reg_b;
	unsigned	cent;
	unsigned char	sra;

	//Tell Neutrino what kind of chip for 'rtc' utility
	hwi_add_rtc("mc146818", base, reg_shift, 2, mmap, cent_reg);

	chip_access(base, reg_shift, mmap, 2);

	// bail if the clock is not running.
	sra = rdcmos(MC146818_SRA);
	if ((sra & 0x60) != 0x20) {
		chip_write8 (1, (sra | 0x60));	//Check for ATI IXP200 RTC - these bits are reserved
		if ((rdcmos(MC146818_SRA) & 0x60) != 0) {
			chip_write8 (1, sra);	//restore old value
			return(0L);
			}
		}
	reg_b = rdcmos(MC146818_SRB);

	while (1) {
		if (check_uip_clr ()) {
			tm.tm_sec  = rdcmos(0);
			tm.tm_min  = rdcmos(2);
			tm.tm_hour = rdcmos(4);
			tm.tm_mday = rdcmos(7);
			tm.tm_mon  = rdcmos(8);
			tm.tm_year = rdcmos(9);
			if (check_uip_clr ())
				break;
		}

		//Loop while time inconsistent
	}

	chip_done();

	save_hour = tm.tm_hour;
	tm.tm_hour &= ~0x80;

	if(!(reg_b & MC146818_SRB_DM)) {
		tm.tm_sec  = bcd2bin(tm.tm_sec);
		tm.tm_min  = bcd2bin(tm.tm_min);
		tm.tm_hour = bcd2bin(tm.tm_hour);
		tm.tm_mday = bcd2bin(tm.tm_mday);
		tm.tm_mon  = bcd2bin(tm.tm_mon);
		tm.tm_year = bcd2bin(tm.tm_year);
	}
	if(!(reg_b & MC146818_SRB_24_12) && (save_hour & 0x80)) {
		//12 hour format & past 12pm
		tm.tm_hour += 12;
	}
	tm.tm_mon -= 1;

	if(cent_reg >= 0) {
		cent = rdcmos(cent_reg);			//century
		if(!(reg_b & MC146818_SRB_DM)) {
			cent = bcd2bin(cent_reg);
		}
		if(cent == 20) tm.tm_year += 100;
	} else if(tm.tm_year < 70) {
		tm.tm_year += 100; //21st century.
	}

	return(calc_time_t(&tm));
}

__SRCVERSION("hw_rtc_mc146818.c $Rev: 657836 $");
