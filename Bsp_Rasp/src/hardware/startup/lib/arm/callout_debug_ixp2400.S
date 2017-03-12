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
 * ixp2400  polled serial I/O
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
	 * Map registers and patch callout routine
	 */
	ldr		r1, [sp, #8]				// debug_device[...]
	ldr		r2, [r1, #DDI_SHIFT]
	ldr		r1, [r1, #DDI_BASE]
	mov		r0, #REG_LS
	mov		r0, r0, lsl r2
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Patch callout routine with LSR register address
	 */
	ldr		r1, [sp, #8]				// debug_device[...]
	ldr		r2, [r1, #DDI_SHIFT]
	mov		r0, #REG_LS
	mov		r0, r0, lsl r2
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * void	display_char_ixp2400(struct sypage_entry *, char)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(display_char_ixp2400, 0, patch_debug)
	mov		ip,     #0x000000ff		// register base (= tx holding register)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0,     #0x000000ff		// LSR register offset
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Wait for TXRDY
	 */
	add		r0, r0, ip
0:	ldr		r3, [r0]
	tst		r3, #LSR_TXRDY
	beq		0b

	/*
	 * Output character
	 */
	str		r1, [ip]

	mov		pc, lr
CALLOUT_END(display_char_ixp2400)


/*
 * -----------------------------------------------------------------------
 * char	poll_key_ixp2400(struct sypage_entry *)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(poll_key_ixp2400, 0, patch_debug)
	mov		ip,     #0x000000ff		// register base (= tx holding register)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0,     #0x000000ff		// LSR register offset
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * If RXRDY, read character, otherwise return -1
	 */
	add		r0, r0, ip
0:	ldr	    r3, [r0]
	tst		r3, #LSR_RXRDY
	ldrne	r0, [ip]
	mvneq	r0, #1

	mov		pc, lr
CALLOUT_END(poll_key_ixp2400)


/*
 * -----------------------------------------------------------------------
 * int break_detect(struct syspage_entry *)
 *
 * Return 1 if break detected, 0 otherwise
 * -----------------------------------------------------------------------
 */
CALLOUT_START(break_detect_ixp2400, 0, patch_debug)
	mov		ip,     #0x000000ff		// register base (= tx holding register)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0,     #0x000000ff		// LSR register offset
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	add		r0, r0, ip

	/*
	 * Check for break indicator
	 */
	ldr		r0, [r0]
	ands	r0, r0, #LSR_BI
	moveq	pc, lr

	/*
	 * Eat the null character
	 */
	ldr		r0, [ip]
	mov		r0, #1
	mov		pc, lr
CALLOUT_END(break_detect_ixp2400)



