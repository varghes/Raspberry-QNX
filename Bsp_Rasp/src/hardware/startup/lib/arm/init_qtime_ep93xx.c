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
 * Cirrus Logic EP93xx qtimer support
*/

#include "startup.h"
#include <arm/ep93xx.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <errno.h>

extern struct callout_rtn	timer_load_ep93xx;
extern struct callout_rtn	timer_value_ep93xx;
extern struct callout_rtn	timer_reload_ep93xx;

static uintptr_t	timer_base;

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT(timer_load, _ep93xx) },
	{ CALLOUT_SLOT(timer_value, _ep93xx) },
	{ CALLOUT_SLOT(timer_reload, _ep93xx) },
};

/*
 * timer_start_ep93xx
 *
 * Start the timer.
 * We don't assume that the timer is enabled and so will re-start it if it is.
 * This requires disabling, re-writing the LOAD register (we use its current
 * value or UINT32_MAX if its zero) as instructed in the doc for the Control
 * register EN bit and then re-enable.
 * 
 * Returns: the current value of the counter (ie. the VALUE register)
*/
static unsigned timer_start_ep93xx(void)
{
	volatile uint32_t ctrl_reg = in32(timer_base + EP93xx_TIMER_CTRL);
	volatile uint32_t load_reg = in32(timer_base + EP93xx_TIMER_LOAD);
	
	/* disable the timer, write the LOAD register (doc says this is required) then re-enable */
	out32(timer_base + EP93xx_TIMER_CTRL, ctrl_reg & ~EP93xx_TIMER_CTRL_ENABLE);
	out32(timer_base + EP93xx_TIMER_LOAD, load_reg);
	out32(timer_base + EP93xx_TIMER_CTRL, ctrl_reg | EP93xx_TIMER_CTRL_ENABLE);

	return in32(timer_base + EP93xx_TIMER_VALUE);
}

/*
 * timer_diff_ep93xx
 * 
 * Return the difference between the current counter value and <start>
*/
static unsigned timer_diff_ep93xx(unsigned start)
{
	unsigned now = in32(timer_base + EP93xx_TIMER_VALUE);
	return start - now;	// down counter
}


// this could move to generic location
static uint64_t qtime_scale[] =
{
	[0] = 1ULL, [1] = 10ULL, [2] = 100ULL, [3] = 1000ULL,
	[4] = 10000ULL, [5] = 100000, [6] = 1000000ULL, [7] = 10000000ULL,
	[8] = 100000000ULL, [9] = 1000000000ULL, [10] = 10000000000ULL, [11] = 100000000000ULL,
	[12] = 1000000000000ULL, [13] = 10000000000000ULL, [14] = 1000000000000000ULL, [15] = 1000000000000000ULL,
};
#define QTIME_CLK_RATE(__scale__, __freq__)		((unsigned long)(qtime_scale[-1*(__scale__)] / (uint64_t)__freq__))
#define QTIME_CLK_SCALE(__rate__, __freq__)		/* need implementation */	
#define QTIME_CLK_FREQ(__scale__, __rate__)		((unsigned long)(qtime_scale[-1*(__scale__)] / (uint64_t)__rate__))

/*
 * init_qtime
 * 
 * Initialize the qtime entry. The timer to use has been added to the HWINFO
 * section as "qtimer"
*/
void init_qtime()
{
	struct qtime_entry	*qtime = alloc_qtime();		// alloc_qtime() call rtc_time()
	unsigned hwi_off = hwi_find_device("qtimer", 0);
    hwiattr_timer_t qtimer_attr;
    int r = hwiattr_get_timer(hwi_off, &qtimer_attr);
    long best_freq;
    unsigned hires_timer;
    unsigned i;

    /* both asserts tells us which operation failed */
    ASSERT(hwi_off != HWI_NULL_OFF);
    ASSERT(r == EOK);
    ASSERT(qtimer_attr.common.location.len > 0);
    ASSERT(qtimer_attr.common.location.base != NULL);
    ASSERT(qtimer_attr.num_clks > 0);

	qtime->intr = hwitag_find_ivec(hwi_off, NULL);
	ASSERT(qtime->intr != HWI_ILLEGAL_VECTOR);
	
	/* if multiple clock sources have been provided, choose the highest resolution */
	best_freq = 0;
	hires_timer = 0;	// we already know there is at least 1 clock source (see ASSERT above)
	for (i=0; i<qtimer_attr.num_clks; i++)
	{
		long freq = hwitag_find_clkfreq(hwi_off, &i);
		if ((i > 0) && (freq > best_freq)) {
			hires_timer = i;
			best_freq = freq;
		}
	}
	ASSERT(hires_timer <= 1);	// I know CLKSEL is only 1 bit (ie. only 2 clock sources)

	qtime->timer_scale = -15;
	i = hires_timer;
	qtime->cycles_per_sec = (uint64_t)hwitag_find_clkfreq(hwi_off, &i);	// don't pass hires_timer as its changed by the call
	ASSERT(qtime->cycles_per_sec > 0);
	qtime->timer_rate = QTIME_CLK_RATE(qtime->timer_scale, qtime->cycles_per_sec);
// kprintf("cps = %x %x, rate = %d\n", ((uint32_t *)&qtime->cycles_per_sec)[0], ((uint32_t *)&qtime->cycles_per_sec)[1], qtime->timer_rate);
	/* Map the registers (timer_base) and select the hires_timer clock source and periodic mode */
	timer_base = startup_io_map(qtimer_attr.common.location.len, qtimer_attr.common.location.base);
	ASSERT(timer_base != NULL);
	out32(timer_base + EP93xx_TIMER_LOAD, UINT32_MAX);	// preset the LOAD register
// kprintf("clksel(%d) = 0x%x\n", hires_timer, EP93xx_TIMER_CTRL_CLKSEL(hires_timer));
	out32(timer_base + EP93xx_TIMER_CTRL,
			EP93xx_TIMER_CTRL_CLKSEL(hires_timer) |		// hires_timer holds which clock to select
			EP93xx_TIMER_CTRL_CLKMODE_PERIODIC);

	timer_start = timer_start_ep93xx;
	timer_diff  = timer_diff_ep93xx;

	add_callout_array(timer_callouts, sizeof(timer_callouts));
	
	/* start the timer */
	timer_start();
}
