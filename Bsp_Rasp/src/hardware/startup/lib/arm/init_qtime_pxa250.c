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
 * PXA250 specific timer support.
 *
 * This should be usable by any board that uses a PXA250.
 */

#include "startup.h"
#include "arm/pxa250.h"

uintptr_t	pxa250_timer_base;

/*
 * The PXA250 timers are driven from the 3.6864MHz clock
 */
#define	PXA250_CLOCK_FREQ	3686400UL
#define	PXA250_CLOCK_RATE	271267361UL
#define	PXA250_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _pxa250) },
	{ CALLOUT_SLOT(timer_value, _pxa250) },
	{ CALLOUT_SLOT(timer_reload, _pxa250) },
};

static unsigned
timer_start_pxa250()
{
	return in32(pxa250_timer_base + PXA250_OSCR);
}

static unsigned
timer_diff_pxa250(unsigned start)
{
	unsigned now = in32(pxa250_timer_base + PXA250_OSCR);

	return (unsigned)((int)now - (int)start);
}

void
init_qtime_pxa250()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	pxa250_timer_base = startup_io_map(0x20, PXA250_TIMER_BASE);

	/*
	 * Clear match status and interrupt enable, and disable watchdog reset
	 */
	out32(pxa250_timer_base + PXA250_OIER, in32(pxa250_timer_base + PXA250_OIER) & ~0xf);;
	out32(pxa250_timer_base + PXA250_OSSR, in32(pxa250_timer_base + PXA250_OSSR) | 0x0f);
	out32(pxa250_timer_base + PXA250_OWER, 0);

	timer_start = timer_start_pxa250;
	timer_diff = timer_diff_pxa250;

	qtime->intr = 26;

	qtime->timer_rate  = PXA250_CLOCK_RATE;
	qtime->timer_scale = PXA250_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)PXA250_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_pxa250.c $ $Rev: 657836 $" );
