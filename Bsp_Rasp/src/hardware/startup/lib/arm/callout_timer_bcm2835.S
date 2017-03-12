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


#include "callout.ah"
#include <arm/bcm2835.h>


/*
 * --------------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *  r0 - physical address of syspage
 *  r1 - virtual  address of syspage
 *  r2 - offset from start of syspage to start of the callout routine
 *  r3 - offset from start of syspage to read/write data used by callout
 * --------------------------------------------------------------------------
 */
patch_timer:
    stmdb   sp!,{r4,lr}
    add     r4, r0, r2                  // address of callout routine

    /*
     * Enabling interrupts , this will be fixed
     */
    mov     r0, #BCM2835_TIMER_SIZE // size of registers
    ldr     r1, Lpaddr
    bl      callout_io_map

    /*
     * Patch the callout routine
     */
    CALLOUT_PATCH   r4, r0, r1, r2, ip

    ldmia   sp!,{r4,pc}

Lpaddr: .word   BCM2835_TIMER_BASE


/*
 * --------------------------------------------------------------------------
 * Set clock resolution, and enable interrupt triggering as a side effect.
 * The interrupt remains masked until it is enabled via intr_unmask_at91sam9xx
 *
 * On entry:
 *  r0 - pointer to syspage_entry
 *  r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_load_bcm2835, 0, patch_timer)
    /*
     * Get the address of the timer registers (patched)
     */
    mov     ip,     #0x000000ff
    orr     ip, ip, #0x0000ff00
    orr     ip, ip, #0x00ff0000
    orr     ip, ip, #0xff000000


/*
	 * Update match register 0
	 */
	ldr		r2, [ip, #BCM2835_TIMER_VALUE_LOWER]
	ldr		r3, [r1, #QT_TIMER_LOAD]
	add		r2, r2, r3
	str		r2, [ip, #BCM2835_TIMER3_CMP]

	/*
	 * Clear the match status register
	 */
	mov		r2, #TIMER_CS_MATCH_3
	str		r2, [ip, #BCM2835_TIMER_CS]

    mov     pc, lr
CALLOUT_END(timer_load_bcm2835)


/*
 * --------------------------------------------------------------------------
 * Read the current timer value, relative to the last clock tick
 *
 * On entry:
 *  r0 - pointer to syspage_entry
 *  r1 - pointer to qtime_entry
 *
 * NOTE: If the counter wraps, eg. ClockCycles just at the point where the
 *       clock interrupt is triggerred, we simply return the wrapped counter
 *       because the wrap is taken care of in the ClockCycles.S arm code. 
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_value_bcm2835, 0, patch_timer)
    /*
     * Get the address of the timer registers (patched)
     */
    mov     ip,     #0x000000ff
    orr     ip, ip, #0x0000ff00
    orr     ip, ip, #0x00ff0000
    orr     ip, ip, #0xff000000

    /*
     * Latch the current timer value and mask out the overflow counter
     */
  	ldr		r0, [ip, #BCM2835_TIMER_VALUE_LOWER]
	ldr		r1, [r1, #QT_TIMER_LOAD]
	sub		r0, r1, r0

    mov     pc, lr
CALLOUT_END(timer_value_bcm2835)


/*
 * --------------------------------------------------------------------------
 * Clear timer interrupt.
 *
 * On entry:
 *  r0 - pointer to syspage_entry
 *  r1 - pointer to qtime_entry
 * --------------------------------------------------------------------------
 */
CALLOUT_START(timer_reload_bcm2835, 0, patch_timer)
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

	/*
	 * Clear the match status register
	 */
	mov		r2, #TIMER_CS_MATCH_3
	str		r2, [ip, #BCM2835_TIMER_CS]

	/*
	 * Update match register 0
	 */
	ldr		r2, [ip, #BCM2835_TIMER3_CMP]
	ldr		r3, [r1, #QT_TIMER_LOAD]
	ldr		r1, [ip, #BCM2835_TIMER_VALUE_LOWER]
	add		r2, r2, r3
	str		r2, [ip, #BCM2835_TIMER3_CMP]

	/*
	 * If OCSR has gone past new match value, set match = OSCR + timer_load
	 */
	sub		r2, r2, r1
	cmp		r2, #0
	addle	r2, r1, r3
	strle	r2, [ip, #BCM2835_TIMER3_CMP]

    mov     pc, lr
CALLOUT_END(timer_reload_bcm2835)
