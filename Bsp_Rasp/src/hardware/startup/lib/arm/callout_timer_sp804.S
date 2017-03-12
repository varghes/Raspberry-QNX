# Copyright 2011, QNX Software Systems. 
# 
# Licensed under the Apache License, Version 2.0 (the "License"). You 
# may not reproduce, modify or distribute this software except in  
# compliance with the License. You may obtain a copy of the License 
# at: http://www.apache.org/licenses/LICENSE-2.0 
# 
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
#
# This file may contain contributions from others, either as  
# contributors under the License or as licensors under other terms.  
# Please review this entire file for other proprietary rights or license 
# notices, as well as the QNX Development Suite License Guide at  
# http://licensing.qnx.com/license-guide/ for other information.
#


#include "callout.ah"
#include <arm/sp804.h>

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
	mov		r0, #SP804_SIZE				// size of registers
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	0x1c110000				// base physical address

/*
 * --------------------------------------------------------------------------
 * Set clock resolution, and enable interrupt triggering as a side effect.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_load_sp804, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	ldr		r0, [r1, #QT_TIMER_LOAD]

	/*
	 * Clear timer interrupt and set counter
	 * NOTE that the Integrator CP needs bit 5 set to enable interrupts.
	 */
	str		r0, [ip, #SP804_INTCLR]
	mov		r1, #SP804_CTRL_EN|SP804_CTRL_INTEN|SP804_CTRL_PERIODIC|SP804_CTRL_32BIT
	str		r1, [ip, #SP804_CTRL]
	str		r0, [ip, #SP804_LOAD]

	mov		pc, lr
CALLOUT_END(timer_load_sp804)


/*
 * --------------------------------------------------------------------------
 * Read the current timer value, relative to the last clock tick
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 *
 * FIXME: this doesn't deal with the counter wrapping, eg. ClockCycles just
 *		  at the point where the clock interrupt is triggerred.
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_value_sp804, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Latch the current timer value and return qtp->timer_load - value
	 */
	ldr		r0, [ip, #SP804_VALUE]
	ldr		r1, [r1, #QT_TIMER_LOAD]
	sub		r0, r1, r0

	mov		pc, lr
CALLOUT_END(timer_value_sp804)


/*
 * --------------------------------------------------------------------------
 * Clear timer interrupt.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_reload_sp804, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Clear the timer interrupt
	 */
	str		r0, [ip, #SP804_INTCLR]

	mov		pc, lr
CALLOUT_END(timer_reload_sp804)
