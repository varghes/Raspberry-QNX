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
 * SA-1100 specific reboot
 *
 * This should be usable by any board that uses an SA-1100.
 *
 * NOTE: this code waits for UART1 and UART3 transmitters to drain.
 *		 This ensures that all shutdown messages or debug stub messages
 *		 are output before the processor is reset.
 *
 *		 If UART1/3 are not used for debug serial I/O, the board specific
 *		 startup program should provide it's own modified copy of this code.
 */

#include "callout.ah"

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
patch_reboot:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #SA1100_RESET_SIZE		// size of interrupt registers
	ldr		r1, Lreset
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map UART1 registers
	 */
	mov		r0, #SA1100_UART_SIZE
	ldr		r1, Luart1
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map UART3 registers
	 */
	mov		r0, #SA1100_UART_SIZE
	ldr		r1, Luart3
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

Lreset:	.word	SA1100_RESET_BASE
Luart1:	.word	SA1100_UART1_BASE
Luart3:	.word	SA1100_UART3_BASE

CALLOUT_START(reboot_sa1100, 0, patch_reboot)
	mov		ip,     #0x000000ff		// reset controller base (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0,     #0x000000ff		// UART1 base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	mov		r1,     #0x000000ff		// UART3 base (patched)
	orr		r1, r1, #0x0000ff00
	orr		r1, r1, #0x00ff0000
	orr		r1, r1, #0xff000000

	/*
	 * Wait for UARTs to drain so we don't lose final debug messages
	 * or debugger stub ack to the host debugger.
	 */
0:	ldr		r2, [r0, #SA1100_UTSR1]
	tst		r2, #SA1100_UTSR1_TBY
	bne		0b

1:	ldr		r2, [r1, #SA1100_UTSR1]
	tst		r2, #SA1100_UTSR1_TBY
	bne		1b

	mov		r1, #SA1100_RSRR_SWR
	str		r1, [ip, #SA1100_RSRR]
0:	b		0b
CALLOUT_END(reboot_sa1100)
