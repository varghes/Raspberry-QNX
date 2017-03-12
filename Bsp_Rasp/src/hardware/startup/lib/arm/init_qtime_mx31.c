/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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
 * i.MX31 specific timer support.
 *
 * This should be usable by any board that uses an i.MX31.
 */

#include "startup.h"
#include <arm/mx31.h>

extern struct callout_rtn	timer_load_mx31;
extern struct callout_rtn	timer_value_mx31;
extern struct callout_rtn	timer_reload_mx31;

extern unsigned		mx31_per_clock;

static uintptr_t	mx31_epit_base;


static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _mx31) },
	{ CALLOUT_SLOT(timer_value, _mx31) },
	{ CALLOUT_SLOT(timer_reload, _mx31) },
};

static unsigned
timer_start_mx31()
{
	return in32(mx31_epit_base + MX31_EPIT_CNR);
}

static unsigned
timer_diff_mx31(unsigned start)
{
	unsigned now = in32(mx31_epit_base + MX31_EPIT_CNR);

	return (unsigned)((int)start - (int)now);
}

void
init_qtime_mx31()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	mx31_epit_base = startup_io_map(0x20, MX31_EPIT1_BASE);

	/*
	 * IPL_CLK, EPIT enabled
	 */
	out32(mx31_epit_base + MX31_EPIT_CR, (1 << 24) | (1 << 19) | (1 << 0));

	timer_start = timer_start_mx31;
	timer_diff = timer_diff_mx31;

	qtime->intr = 28;

#define MX31_CLOCK_FREQ		66500000UL
#define MX31_CLOCK_RATE		15037594UL	
#define MX31_CLOCK_SCALE	-15
	if (mx31_per_clock == 0) {
		qtime->timer_rate  = MX31_CLOCK_RATE;
		qtime->timer_scale = MX31_CLOCK_SCALE;
		qtime->cycles_per_sec = (uint64_t)MX31_CLOCK_FREQ;
	}
	else {
		qtime->timer_rate  = (unsigned long)((uint64_t)1000000000 * (uint64_t)1000000 / (uint64_t)mx31_per_clock);
		qtime->timer_scale = -15;
		qtime->cycles_per_sec = (uint64_t)mx31_per_clock;
	}

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_qtime_mx31.c $ $Rev: 657836 $" );
