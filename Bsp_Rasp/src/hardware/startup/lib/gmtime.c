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





#include <time.h>

struct tm *
_gmtime(const time_t *timer, struct tm *tmbuf) {
	unsigned long	t, days, secs, year, mon;
	static unsigned char	mdays [2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };
	
	days = *timer / (60L*60L*24L);
	secs = *timer % (60L*60L*24L);
	tmbuf->tm_wday = (days + 4) % 7;
	for (year = 0; ; year++) {
		t = 365 + (((year + 2) % 4) == 0);
		if (t > days) break;
		days -= t;
	}
	tmbuf->tm_yday = days;
	for (mon = 0; mon < 12; mon++) {
		t = mdays [((year + 2) % 4) == 0][mon];
		if (t > days) break;
		days -= t;
	}
	tmbuf->tm_sec  = secs % 60;
	tmbuf->tm_min  = (secs / 60) % 60;
	tmbuf->tm_hour = secs / 60 / 60;
	tmbuf->tm_mday = days + 1;
	tmbuf->tm_mon  = mon;
	tmbuf->tm_year = year + 70;
	return tmbuf;
}

__SRCVERSION("gmtime.c $Rev: 657836 $");
