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
 * SA-1100 specific timer support.
 *
 * This should be usable by any board that uses an SA-1100.
 */

#include "startup.h"
#include "arm/sa1100.h"

uintptr_t	sa1100_timer_base;

/*
 * The SA-1100 timers are driven from the 3.6864MHz clock
 */
#define	SA1100_CLOCK_FREQ	3686400UL
#define	SA1100_CLOCK_RATE	271267361UL
#define	SA1100_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _sa1100) },
	{ CALLOUT_SLOT(timer_value, _sa1100) },
	{ CALLOUT_SLOT(timer_reload, _sa1100) },
};

static unsigned
timer_start_sa1100()
{
	return in32(sa1100_timer_base + SA1100_OSCR);
}

static unsigned
timer_diff_sa1100(unsigned start)
{
	unsigned now = in32(sa1100_timer_base + SA1100_OSCR);

	return (unsigned)((int)now - (int)start);
}

void
init_qtime_sa1100()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	sa1100_timer_base = startup_io_map(0x20, SA1100_TIMER_BASE);

	/*
	 * Clear match status and interrupt enable, and disable watchdog reset
	 */
	out32(sa1100_timer_base + SA1100_OSSR, 0);
	out32(sa1100_timer_base + SA1100_OIER, 0);
	out32(sa1100_timer_base + SA1100_OWER, 0);

	timer_start = timer_start_sa1100;
	timer_diff = timer_diff_sa1100;

	qtime->intr = SA1100_INTR_TIMER_0;

	qtime->timer_rate  = SA1100_CLOCK_RATE;
	qtime->timer_scale = SA1100_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)SA1100_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_sa1100.c $ $Rev: 657836 $" );
