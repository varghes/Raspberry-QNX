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
#include <arm/s3c2410.h>

unsigned long
rtc_time_s3c2410(unsigned base)
{
	struct tm	tm;

	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_RTC, "s3c2410", 0);
	hwi_add_location(base, S3C2410_RTC_SIZE, 0, hwi_find_as(base, 1));

	// get the current time from the RTC, and convert it to seconds since epoch

    chip_access(base, 0, 0, S3C2410_RTC_SIZE);

	// enable RTC 
	chip_write8(S3C2410_RTCCON, chip_read8(S3C2410_RTCCON) | 1);

    // convert BCD to binary
    tm.tm_sec  = bcd2bin(chip_read8(S3C2410_BCDSEC) & 0xff);    // seconds
    tm.tm_min  = bcd2bin(chip_read8(S3C2410_BCDMIN) & 0xff);    // minutes
    tm.tm_hour = bcd2bin(chip_read8(S3C2410_BCDHOUR) & 0xff);   // hours
    tm.tm_mday = bcd2bin(chip_read8(S3C2410_BCDDAY) & 0xff);    // day
    tm.tm_mon  = bcd2bin(chip_read8(S3C2410_BCDMON) & 0xff) -1;    // month
    tm.tm_year = (bcd2bin(chip_read8(S3C2410_BCDYEAR) & 0xff))+100;   // year

    chip_done();

    return(calc_time_t(&tm));

}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/hw_rtc_s3c2410.c $ $Rev: 657836 $" );
