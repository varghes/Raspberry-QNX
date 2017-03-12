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
 * IXP1200 specific timer support.
 *
 * This should be usable by any board that uses an IXP1200.
 */

#include "startup.h"
#include "arm/ixp1200.h"

uintptr_t	ixp1200_timer_base;

/*
 * The IXP1200 timers are driven from the 3.6864MHz clock
 */
#define	IXP1200_CLOCK_FREQ	3686400UL
#define	IXP1200_CLOCK_RATE	271267361UL
#define	IXP1200_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _ixp1200) },
	{ CALLOUT_SLOT(timer_value, _ixp1200) },
	{ CALLOUT_SLOT(timer_reload, _ixp1200) },
};

static unsigned
timer_start_ixp1200()
{
	return in32(ixp1200_timer_base + IXP1200_TIMER_1_VALUE);
}

static unsigned
timer_diff_ixp1200(unsigned start)
{
	unsigned now = in32(ixp1200_timer_base + IXP1200_TIMER_1_VALUE);

	return (unsigned)((int)start - (int)now);
}

void
init_qtime_ixp1200()
{
	struct qtime_entry	*qtime = alloc_qtime();
	unsigned			pll;
	static const char	pll_mult[] = {
		/*
		 * Multiplier for IXP1200_CLOCK_FREQ for PLL_CFG values
		 * FIXME: these are taken from the Programmer's Reference Manual,
		 *        but I'm not sure they are actually 100% correct...
		 */
		8,
		10,
		12,
		14,
		16,
		18,
		20,
		22,
		24,
		26,
		28,
		30,
		36,
		40,
		42,
		44,
		45,
		48,
		52,
		54,
		56,
		60,
		63
	};

	/*
	 * Map the timer registers
	 */
	ixp1200_timer_base = startup_io_map(IXP1200_TIMER_SIZE, IXP1200_TIMER_BASE);

	/*
	 * Calulate CPU core PLL frequency
	 */
	pll = in32(IXP1200_PLL_CFG_BASE) & IXP1200_PLL_CFG_CCF;
	if (pll > 22) {
		crash("PLL_CFG is %d. Max supported value is 22\n", pll);
	}
	pll = IXP1200_CLOCK_FREQ * pll_mult[pll];

	/*
	 * Set the timer free running, divide by 16, with maximum countdown
	 */
	pll = pll / 16;	
	out32(ixp1200_timer_base + IXP1200_TIMER_1_LOAD, 0x00ffffff);
	out32(ixp1200_timer_base + IXP1200_TIMER_1_CONTROL, IXP1200_TIMER_CONTROL_EN | IXP1200_TIMER_CONTROL_STP_16 | IXP1200_TIMER_CONTROL_FREE);

	timer_start = timer_start_ixp1200;
	timer_diff = timer_diff_ixp1200;

	qtime->intr = 36;

	qtime->timer_rate  = IXP1200_CLOCK_RATE;
	qtime->timer_scale = IXP1200_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)pll;
	invert_timer_freq(qtime, pll);

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_ixp1200.c $ $Rev: 657836 $" );
