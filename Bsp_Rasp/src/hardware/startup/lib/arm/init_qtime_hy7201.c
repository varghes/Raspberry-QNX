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
 * HY7201 specific timer support.
 *
 * This should be usable by any board that uses an HY7201.
 */

#include "startup.h"
#include "arm/hy7201.h"

/*
 * The HY7201 timers are driven from the 3.6864MHz clock
 */
#define	HY7201_CLOCK_FREQ	3686400UL
#define	HY7201_CLOCK_RATE	271267361UL
#define	HY7201_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _hy7201) },
	{ CALLOUT_SLOT(timer_value, _hy7201) },
	{ CALLOUT_SLOT(timer_reload, _hy7201) },
};

static unsigned
timer_start_hy7201()
{
	out32(HY7201_TIMER_BASE + HY7201_TIMER_BASE0, 0xffffffff);
	out32(HY7201_TIMER_BASE + HY7201_TIMER_CTL0, HY7201_TIMER_CTL_RESET);
	out32(HY7201_TIMER_BASE + HY7201_TIMER_CTL0, HY7201_TIMER_CTL_START);
	return in32(HY7201_TIMER_BASE + HY7201_TIMER_CNT0);
}

static unsigned
timer_diff_hy7201(unsigned start)
{
	return in32(HY7201_TIMER_BASE + HY7201_TIMER_CNT0) - start;
}

void
init_qtime_hy7201()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Enable the timers
	 */
	out32(HY7201_TIMER_BASE + HY7201_TIMER_TOPCTL, HY7201_TIMER_TOPCTL_ENABLE);
	timer_start = timer_start_hy7201;
	timer_diff = timer_diff_hy7201;

	qtime->intr = 9;

	qtime->timer_rate  = HY7201_CLOCK_RATE;
	qtime->timer_scale = HY7201_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)HY7201_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_hy7201.c $ $Rev: 657836 $" );
