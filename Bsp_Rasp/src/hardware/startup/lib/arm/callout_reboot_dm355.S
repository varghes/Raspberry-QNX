#  
# Copyright 2008, QNX Software Systems. 
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
 * TI DM355 system reboot
 * Enable Timer2/Watchdog timer and Simply spin here to wait the time out
 * 
 */

#include "callout.ah"
#include <arm/dm355.h>


patch_reboot:
	stmdb			sp!, {r4,lr}
	add				r4, r0, r2
	
	/*
	 * Map WDT registers
	 */
	mov				r0, #0x48				// size of WDT registers */
	ldr				r1, Lpaddr
	bl				callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia			sp!, {r4,pc}

Lpaddr: .word	DM355_TMR_WDT_REGS

CALLOUT_START(reboot_dm355, 0, patch_reboot)
    /*
     * Get the base address of WDT registers (patched)
     */

	mov     ip,     #0x000000ff
	orr     ip, ip, #0x0000ff00
	orr     ip, ip, #0x00ff0000
	orr     ip, ip, #0xff000000

	mov     r1, #0x0b				// Initial State: TIMMODE=2h, TIM12RS=1, TIM34RS=1
	str     r1, [ip, #DM355_TMR_TGCR]
	
	ldr     r1, Wdkey1				// Pre-active state: WDKEY=A5C6, WDEN=1;
	str     r1, [ip, #DM355_TMR_WDTCR]

	ldr     r1, Wdkey2				// Active state: WDKEY=DAE7, WDEN=1;
	str     r1, [ip, #DM355_TMR_WDTCR]

0:	b	0b							// spin here to wait WDT time out

Wdkey1: .word	0xa5c64000
Wdkey2: .word	0xda7e4000
CALLOUT_END(reboot_dm355)

