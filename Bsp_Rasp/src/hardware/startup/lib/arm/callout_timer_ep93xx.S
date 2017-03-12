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
 * Cirrus Logic EP93xx timer support
 *
 * This should be usable by any board that uses a Cirrus Logic EP93xx processor
 * which contains an ARM 920T core 
*/

#include "callout.ah"


/*
 * --------------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * --------------------------------------------------------------------------
 */
patch_timer:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map registers
	 */
	mov		r0, #EP93xx_QTIMER_SIZE
	ldr		r1, Lpaddr
	mov		r2, #0xB00				// PROT_READ|PROT_WRITE|PROT_NOCACHE
	bl		callout_memory_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	EP93xx_QTIMER_BASE

/*
 * --------------------------------------------------------------------------
 * Timer Load
 *
 * We expect that the timer mode and clksel have been already configured properly.
 * The interrupt remains masked until it is enabled via intr_unmask_ep93xx.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_load_ep93xx, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* disable the timer for the load operation and clear any pending interrupt */
	ldr		r0, [ip, #EP93xx_QTIMER_CTRL]
	bic		r0, r0, #EP93xx_TIMER_CTRL_ENABLE
	str		r0, [ip, #EP93xx_QTIMER_CTRL]
	str		r0, [ip, #EP93xx_QTIMER_CLR]
	
	/* write the load register */
	ldr		r0, [r1, #QT_TIMER_LOAD]
	str		r0, [ip, #EP93xx_QTIMER_LOAD]

	/* re-enable the timer */
	ldr		r0, [ip, #EP93xx_QTIMER_CTRL]
	orr		r0, r0, #EP93xx_TIMER_CTRL_ENABLE
	str		r0, [ip, #EP93xx_QTIMER_CTRL]

	mov		pc, lr
CALLOUT_END(timer_load_ep93xx)


/*
 * --------------------------------------------------------------------------
 * Read the current timer value, relative to the last clock tick
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 *
 * On exit:
 *	r0 - the difference between the qtimer->timer_load and teh timer VALUE
 *		 register (same as timer_value_ep93xx)
 *
 * FIXME: this doesn't deal with the counter wrapping, eg. ClockCycles just
 *		  at the point where the clock interrupt is triggerred.
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_value_ep93xx, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* Latch the current timer value and return qtp->timer_load - value */
	ldr		r0, [ip, #EP93xx_QTIMER_VALUE]
	ldr		r1, [r1, #QT_TIMER_LOAD]
	sub		r0, r1, r0

	mov		pc, lr
CALLOUT_END(timer_value_ep93xx)


/*
 * --------------------------------------------------------------------------
 * Reload the timer
 *
 * This routine acts as the ISR for the system clock. It is an optional routine
 * but since this timer hardware needs to have the interrupt cleared, we need it
 * even though the timer will reload itself from the LOAD register.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 *
 * On exit:
 *	r0 - 0 if not reloaded, non-zero otherwise
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_reload_ep93xx, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* Clear the timer interrupt */
	mov		r0, #1		/* a non-zero return value indicates we have serviced the interrupt */
	str		r0, [ip, #EP93xx_QTIMER_CLR]
	mov		pc, lr
CALLOUT_END(timer_reload_ep93xx)
