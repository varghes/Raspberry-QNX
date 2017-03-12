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
 * alloc_qtime()
 *	Allocate the qtime section and initialize the boot time from the
 *  IPL info or an RTC chip.
 */

struct qtime_entry *
alloc_qtime() {
	struct qtime_entry *qtime = set_syspage_section(&lsp.qtime, sizeof(*lsp.qtime.p));
	const struct startup_info_time	*time;

	qtime->epoch = 1970;
	if(!(misc_flags & MISC_FLAG_SUPPRESS_BOOTTIME)) {
		time = find_startup_info(NULL, STARTUP_INFO_TIME);
		qtime->boot_time = (time != NULL) ? time->time : rtc_time();
		qtime->nsec_tod_adjust = qtime->boot_time * (uint64_t)1000000000;
	}
	return(qtime);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/alloc_qtime.c $ $Rev: 657836 $" );
