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
 * IXP2400 specific timer support.
 *
 * This should be usable by any board that uses an IXP2400.
 */

#include "startup.h"
#include "arm/ixp2400.h"

uintptr_t	ixp2400_timer_base;

/*
 * The IXP2400 timers are driven from a 50MHz clock
 */
#define	IXP2400_CLOCK_FREQ	50000000UL
#define	IXP2400_CLOCK_RATE	20000000UL
#define	IXP2400_CLOCK_SCALE	-15

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _ixp2400) },
	{ CALLOUT_SLOT(timer_value, _ixp2400) },
	{ CALLOUT_SLOT(timer_reload, _ixp2400) },
};

static unsigned
timer_start_ixp2400()
{
	return in32(ixp2400_timer_base + IXP2400_TIMER1_STATUS);
}

static unsigned
timer_diff_ixp2400(unsigned start)
{
	unsigned now = in32(ixp2400_timer_base + IXP2400_TIMER1_STATUS);

	return (unsigned)((int)start - (int)now);
}

void
init_qtime_ixp2400()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	ixp2400_timer_base = startup_io_map(IXP2400_TIMER_SIZE, IXP2400_TIMER_BASE);


	/*
	 * Set the timer free running with maximum countdown
	 */

	out32(ixp2400_timer_base + IXP2400_TIMER1_LOAD, 0xffffffff);
	out32(ixp2400_timer_base + IXP2400_TIMER1_CONTROL,0x80);

	timer_start = timer_start_ixp2400;
	timer_diff = timer_diff_ixp2400;

	qtime->intr = 4;

	qtime->timer_rate  = IXP2400_CLOCK_RATE;
	qtime->timer_scale = IXP2400_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)IXP2400_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_ixp2400.c $ $Rev: 657836 $" );
