#
# Copyright 2007, 2008, QNX Software Systems. 
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
/*
 * SA-1100 timer support
 *
 * This should be usable by any board that uses an SA-1100.
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
	mov		r0, #0x20					// size of registers
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	SA1100_TIMER_BASE


/*
 * --------------------------------------------------------------------------
 * Set clock resolution, and enable interrupt triggering as a side effect.
 * The interrupt remains masked until it is enabled via intr_unmask_sa1100
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_load_sa1100, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Update match register 0
	 */
	ldr		r2, [ip, #SA1100_OSCR]
	ldr		r3, [r1, #QT_TIMER_LOAD]
	add		r2, r2, r3
	str		r2, [ip, #SA1100_OSMR0]

	/*
	 * Clear the match status register
	 */
	mov		r2, #SA1100_OSSR_M0
	str		r2, [ip, #SA1100_OSSR]

	/*
	 * Enable interrupt trigger from match register 0
	 */
	str		r2, [ip, #SA1100_OIER]
	orr		r2, r2, #SA1100_OIER_E0
	str		r2, [ip, #SA1100_OIER]

	mov		pc, lr
CALLOUT_END(timer_load_sa1100)


/*
 * --------------------------------------------------------------------------
 * Read the current timer value, relative to the last clock tick
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_value_sa1100, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * offset = qtimeptr->timer_load + counter - match
	 */
	ldr		r2, [ip, #SA1100_OSMR0]
	ldr		r3, [ip, #SA1100_OSCR]
	ldr		r0, [r1, #QT_TIMER_LOAD]
	sub		r2, r3, r2
	add		r0, r0, r2

	mov		pc, lr
CALLOUT_END(timer_value_sa1100)


/*
 * --------------------------------------------------------------------------
 * Reload the match register to trigger an interrupt at the next clock tick.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_reload_sa1100, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Mask interrupts around update of timer registers
	 */
	mrs		r0, cpsr
	orr		r2, r0, #0x80	// ARM_CPSR_I
	msr		cpsr, r2

	/*
	 * Clear the match status register
	 */
	mov		r2, #SA1100_OSSR_M0
	str		r2, [ip, #SA1100_OSSR]

	/*
	 * Update match register 0
	 */
	ldr		r2, [ip, #SA1100_OSMR0]
	ldr		r3, [r1, #QT_TIMER_LOAD]
	ldr		r1, [ip, #SA1100_OSCR]
	add		r2, r2, r3
	cmp		r2, r1
	addle	r2, r1, r3
	str		r2, [ip, #SA1100_OSMR0]

	/*
	 * Restore interrupts
	 */
	msr		cpsr, r0

	mov		pc, lr
CALLOUT_END(timer_reload_sa1100)
