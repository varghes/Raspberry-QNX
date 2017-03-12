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
#include <time.h>

#define SEC_PER_MIN		60UL
#define SEC_PER_HOUR	(60UL * SEC_PER_MIN)
#define SEC_PER_DAY		(24UL * SEC_PER_HOUR)

unsigned long
calc_time_t(const struct tm *tm) {
	unsigned				year;
	unsigned long			sec;
	const static unsigned long	month_sec[] = {
		0L,         2678400L,  5097600L,  7776000L, 10368000L, 13046400L,
		15638400L, 18316800L, 20995200L, 23587200L, 26265600L, 28857600L 
	} ;

	year = tm->tm_year - 70;
	sec = ((year * 365UL) + ((year + 1) >> 2)) * SEC_PER_DAY;
	sec += month_sec[tm->tm_mon];
	year = tm->tm_year + 1900;
	if(tm->tm_mon >= 2
	   && (year % 4) == 0
	   && (((year % 100) != 0) || ((year % 400) == 0))) {
		sec += SEC_PER_DAY;
	}
	sec += (tm->tm_mday-1) * SEC_PER_DAY;
	sec += tm->tm_hour * SEC_PER_HOUR;
	sec += tm->tm_min * SEC_PER_MIN;

	return(sec + tm->tm_sec);
}

__SRCVERSION("calc_time_t.c $Rev: 657836 $");
