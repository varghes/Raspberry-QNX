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
 * IXP23xx specific timer support.
 *
 * This should be usable by any board that uses an IXP23xx.
 */

#include "startup.h"
#include <arm/ixp23xx.h>

uintptr_t	ixp23xx_timer_base;

/*
 * The IXP23XX timers are driven from a 75MHz clock
 */
#define	IXP23XX_CLOCK_FREQ	75000000UL
#define	IXP23XX_CLOCK_RATE	13333333UL
#define	IXP23XX_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _ixp23xx) },
	{ CALLOUT_SLOT(timer_value, _ixp23xx) },
	{ CALLOUT_SLOT(timer_reload, _ixp23xx) },
};

static unsigned
timer_start_ixp23xx()
{
	return in32(ixp23xx_timer_base + IXP23XX_OST_TIM0);
}

static unsigned
timer_diff_ixp23xx(unsigned start)
{
	unsigned now = in32(ixp23xx_timer_base + IXP23XX_OST_TIM0);

	return (unsigned)((int)start - (int)now);
}

void
init_qtime_ixp23xx()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	ixp23xx_timer_base = startup_io_map(IXP23XX_OST_SIZE, IXP23XX_OST_BASE);

	/*
	 * Set the timer with maximum countdown, auto reload. The two least significant
	 * bits of the reload register are actually control bits, and are written as zero
	 * when loading the count value. Bit one selects one-shot (1) or Auto-reload (0), 
	 * and bit 0 starts the timer running.
	 */

	out32(ixp23xx_timer_base + IXP23XX_OST_TIM0_RL, 0xfffffffc);
	
	/* clear any pending interrupt on the timer */
	out32(ixp23xx_timer_base + IXP23XX_OST_STS, 0x01);

	timer_start = timer_start_ixp23xx;
	timer_diff = timer_diff_ixp23xx;

	qtime->intr = 3;

	qtime->timer_rate  = IXP23XX_CLOCK_RATE;
	qtime->timer_scale = IXP23XX_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)IXP23XX_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_ixp23xx.c $ $Rev: 657836 $" );
