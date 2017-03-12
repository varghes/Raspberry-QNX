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
 * AT91SAM9XX specific timer support.
 *
 * This should be usable by any board that uses an AT91SAM9XX
 */

#include "startup.h"
#include <arm/at91sam9xx.h>

extern struct callout_rtn	timer_load_at91sam9xx;
extern struct callout_rtn	timer_value_at91sam9xx;
extern struct callout_rtn	timer_reload_at91sam9xx;

static uintptr_t	at91sam9xx_timer_base;

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _at91sam9xx) },
	{ CALLOUT_SLOT(timer_value, _at91sam9xx) },
	{ CALLOUT_SLOT(timer_reload, _at91sam9xx) },
};

static unsigned
timer_start_at91sam9xx() {
	return in32(at91sam9xx_timer_base + AT91_PIT_PIIR) & 0x000FFFFF;
}

static unsigned
timer_diff_at91sam9xx(unsigned start)
{
	unsigned now = in32(at91sam9xx_timer_base + AT91_PIT_PIIR) & 0x000FFFFF;

	return (unsigned)((int)now - (int)start);
}

void
init_qtime_at91sam9xx()
{
	struct qtime_entry	*qtime = alloc_qtime();

	/*
	 * Map the timer registers
	 */
	at91sam9xx_timer_base = startup_io_map(AT91_PIT_SIZE, AT91_PIT_BASE);

	/*
	 * Disable clock
	 */
	out32(at91sam9xx_timer_base + AT91_PIT_MR, 0); 
	

	timer_start = timer_start_at91sam9xx;
	timer_diff = timer_diff_at91sam9xx;

	/* 
	 * IRQ number for TC0 is 1
	 */
	qtime->intr = 1;

#define	AT91SAM_CLOCK_FREQ         3125000UL
#define	AT91SAM_CLOCK_RATE         320000000UL
#define	AT91SAM_CLOCK_SCALE        -15     

	qtime->timer_rate  = AT91SAM_CLOCK_RATE;
	qtime->timer_scale = AT91SAM_CLOCK_SCALE;
	qtime->cycles_per_sec = (uint64_t)AT91SAM_CLOCK_FREQ;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}
