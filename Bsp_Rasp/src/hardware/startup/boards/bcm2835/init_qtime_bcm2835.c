/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems.
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
 * bcm2835 specific timer support.
 */

#include "startup.h"
#include <arm/bcm2835.h>

extern struct callout_rtn	timer_load_bcm2835;
extern struct callout_rtn	timer_value_bcm2835;
extern struct callout_rtn	timer_reload_bcm2835;

static uintptr_t	bcm2835_timer_base;
static uintptr_t bcm2835_timer_intr_base;

unsigned long BCM2835_clock(int OP);

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _bcm2835) },
	{ CALLOUT_SLOT(timer_value, _bcm2835) },
	{ CALLOUT_SLOT(timer_reload, _bcm2835) },
};

static unsigned
timer_start_bcm2835() {
	return in32(bcm2835_timer_base + BCM2835_TIMER_VALUE_LOWER) ;
}

static unsigned
timer_diff_bcm2835(unsigned start)
{
	unsigned now = in32(bcm2835_timer_base + BCM2835_TIMER_VALUE_LOWER) ;
	return (unsigned)((int)now - (int)start);
}

void
init_qtime_bcm2835()
{
	struct qtime_entry *qtime = alloc_qtime();
	/*
	 * Map the timer registers
	 */
	bcm2835_timer_base = startup_io_map(BCM2835_TIMER_SIZE, BCM2835_TIMER_BASE);
	bcm2835_timer_intr_base = startup_io_map(ARM_AIC_SIZE,ARM_IRQCTLR_BASEOFF);

	timer_start = timer_start_bcm2835;
	timer_diff = timer_diff_bcm2835;

    #define	BCM2835_CLOCK_FREQ         1000000UL
    #define	BCM2835_CLOCK_RATE         1000000000UL
    #define	BCM2835_CLOCK_SCALE        -15

	qtime->intr = BCM2835_PHYSIRQ_TIMER3;
	qtime->timer_rate  = BCM2835_CLOCK_RATE;
	qtime->timer_scale=BCM2835_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)BCM2835_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}
