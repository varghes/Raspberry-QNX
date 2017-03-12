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
 * IXP2400 timer support
 *
 * This should be usable by any board that uses an IXP2400.
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
	mov		r0, #IXP2400_TIMER_SIZE		// size of registers
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

Lpaddr:	.word	IXP2400_TIMER_BASE


/*
 * --------------------------------------------------------------------------
 * Set clock resolution, and enable interrupt triggering as a side effect.
 * The interrupt remains masked until it is enabled via intr_unmask_pxa250
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_load_ixp2400, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


	/*
	 * Clear any pending interrupt
	 */

	str		ip, [ip, #IXP2400_TIMER1_CLEAR]

	/*
	 * Set IXP2400_TIMER1_LOAD
	 */

	ldr		r3, [r1, #QT_TIMER_LOAD]
	str		r3, [ip, #IXP2400_TIMER1_LOAD]

	/*
	 * Make sure timer is periodic
	 */

	ldr		r2, [ip, #IXP2400_TIMER1_CONTROL]
	orr		r2, r2,  #IXP2400_TIMER_CONTROL_ACT
	str		r2, [ip, #IXP2400_TIMER1_CONTROL]
	mov		pc, lr
CALLOUT_END(timer_load_ixp2400)


/*
 * --------------------------------------------------------------------------
 * Read the current timer value, relative to the last clock tick
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_value_ixp2400, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * offset = TIMER1_LOAD - TIMER1_STATUS
	 */

	ldr		r2, [ip, #IXP2400_TIMER1_LOAD]
	ldr		r3, [ip, #IXP2400_TIMER1_STATUS]
	sub		r0, r2, r3

	mov		pc, lr
CALLOUT_END(timer_value_ixp2400)


/*
 * --------------------------------------------------------------------------
 * Reload the match register to trigger an interrupt at the next clock tick.
 *
 * On entry:
 *	r0 - pointer to syspage_entry
 *	r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_reload_ixp2400, 0, patch_timer)
	/*
	 * Get the address of the timer registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Clear 
	 */
    mov		r0, #1
	str		r0, [ip, #IXP2400_TIMER1_CLEAR]
	mov		pc, lr
CALLOUT_END(timer_reload_ixp2400)
