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
 * SA-1100 specific polled serial I/O
 *
 * This should be usable by any board that uses an SA-1100.
 */

#include "callout.ah"

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0     - physical address of syspage
 *	r1     - virtual  address of syspage
 *	r2     - offset from start of syspage to start of the callout routine
 *	r3     - offset from start of syspage to read/write data used by callout
 *  [sp]   - data registered with callout via callout_register_data()
 *  [sp+4] - address of callout definition (CALLOUT_START)
 * -----------------------------------------------------------------------
 */
patch_debug:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map registers
	 */
	ldr		r1, [sp, #8]				// debug_devices[...]
	ldr		r1, [r1, #DDI_BASE]
	mov		r0, #SA1100_UART_SIZE
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * void	display_char_sa1100(struct sypage_entry *, char)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(display_char_sa1100, 0, patch_debug)
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000
0:	ldr		r3, [ip, #SA1100_UTSR1]
	tst		r3, #SA1100_UTSR1_TNF
	beq		0b
	str		r1, [ip, #SA1100_UTDR]
	mov		pc, lr
CALLOUT_END(display_char_sa1100)


/*
 * -----------------------------------------------------------------------
 * char	display_char_sa1100(struct sypage_entry *)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(poll_key_sa1100, 0, patch_debug)
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000
0:	ldr		r3, [ip, #SA1100_UTSR1]
	tst		r3, #SA1100_UTSR1_RNE
	ldrne	r0, [ip, #SA1100_UTDR]
	mvneq	r0, #1
	mov		pc, lr
CALLOUT_END(poll_key_sa1100)


/*
 * -----------------------------------------------------------------------
 * -----------------------------------------------------------------------
 */
CALLOUT_START(break_detect_sa1100, 0, patch_debug)
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Check if RBB is set to indicate start of break
	 */
	ldr		r0, [ip, #SA1100_UTSR0]
	ands	r0, r0, #SA1100_UTSR0_RBB
	moveq	pc, lr

	/*
	 * Clear RBB and eat the null character.
	 * Wait for REB to clear the h/w interlock.
	 */
	str		r0, [ip, #SA1100_UTSR0]
	ldr		r0, [ip, #SA1100_UTDR]
0:	ldr		r0, [ip, #SA1100_UTSR0]
	ands	r0, r0, #SA1100_UTSR0_REB
	beq		0b
	str		r0, [ip, #SA1100_UTSR0]
	mov		r0, #1
	mov		pc, lr
CALLOUT_END(break_detect_sa1100)
