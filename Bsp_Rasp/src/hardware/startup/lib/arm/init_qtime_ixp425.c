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
 * IXP425 specific timer support.
 *
 * This should be usable by any board that uses an IXP425 (IXC1100).
 */

#include "startup.h"
#include "arm/ixc1100.h"

uintptr_t	ixp425_timer_base;


/*
 * The IXP425 timers are driven from a 66.66MHz clock
 *
 *	RATE = (1/10^SCALE)/FREQ
 */

#define IXP425_CLOCK_FREQ	66660000UL
#define IXP425_CLOCK_RATE	15001500UL
#define	IXP425_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _ixp425) },
	{ CALLOUT_SLOT(timer_value, _ixp425) },
	{ CALLOUT_SLOT(timer_reload, _ixp425) },
};

static unsigned
timer_start_ixp425()
{
	return in32(ixp425_timer_base + IXP425_TIMER0_VAL);
}

static unsigned
timer_diff_ixp425(unsigned start)
{
	unsigned now = in32(ixp425_timer_base + IXP425_TIMER0_VAL) & ~0x3;

	return (unsigned)((int)start - (int)now);
}

void
init_qtime_ixp425()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	ixp425_timer_base = startup_io_map(IXP425_TIMER_SIZE, IXP425_TIMER_BASE);


	/*
	 * Set the timer free running with maximum countdown
	 */

	out32(ixp425_timer_base + IXP425_TIMER0_RELOAD, 0xfffffffc);
	out32(ixp425_timer_base + IXP425_TIMER_STATUS, 0x5);


	timer_start = timer_start_ixp425;
	timer_diff = timer_diff_ixp425;

	qtime->intr = 5; //timer0 interrupt

	qtime->timer_rate  = IXP425_CLOCK_RATE;
	qtime->timer_scale = IXP425_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)IXP425_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_ixp425.c $ $Rev: 657836 $" );
