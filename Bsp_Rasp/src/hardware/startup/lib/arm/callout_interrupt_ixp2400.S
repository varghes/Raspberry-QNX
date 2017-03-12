/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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
 * Intel IXP2400 specific interrupt callouts
 *
 * interrupt_id_* and interrupt_eoi_* are copied and intermixed with other
 * kernel code during initialisation.
 *
 * They do not follow normal calling conventions, and must fall through
 * to the end, rather than attempting to perform a return instruction.
 *
 * The INTR_GENFLAG_* bits in the intrinfo_entry defines which of the
 * following values can be loaded on entry to these code fragments:
 *
 *	r5 - holds the syspageptr				(INTR_GENFLAG_SYSPAGE  set)
 *	r6 - holds the intrinfo_entry pointer	(INTR_GENFLAG_INTRINFO set)
 *	r7 - holds the interrupt mask count		(INTR_GENFLAG_INTRMASK set)
 *	r8 - holds INTRLEVEL pointer
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
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


/*
 * Patch callouts with IXP2400_IRQ_BASE
 */
patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map registers
	 */

	mov		r0, #IXP2400_IRQ_SIZE	// size of registers
	ldr		r1, Lintr_base
	bl		callout_io_map


	CALLOUT_PATCH	r4, r0, r1, r2, ip


	ldmia	sp!,{r4,pc}

Lintr_base:	.word	IXP2400_IRQ_BASE

CALLOUT_START(interrupt_id_ixp2400, 0, patch_intr)
	/*
	 *  Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 *  Scan for first set bit
	 */
	ldr		r1, [ip, #IXP2400_IRQ_STATUS]		// indicates any active interrupts
	mov		r4, #32								// number of sources

	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 *  mask the interrupt source
	 */
	mov		r2, r2, lsl r4
	str		r2, [ip, #IXP2400_IRQ_ENABLE_CLR]
1:

CALLOUT_END(interrupt_id_ixp2400)


CALLOUT_START(interrupt_eoi_ixp2400, 0, patch_intr)

	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Get current INT_MASK value
	 */
	ldr		r0, [ip, #IXP2400_IRQ_ENABLE]

	/*
	 * Only unmask if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	mov		r2, #1
	mov		r2, r2, lsl r4
	orr		r0, r0, r2

0:	/*
	 * set the enable 
	 */
	str		r0, [ip, #IXP2400_IRQ_ENABLE_SET]
	

CALLOUT_END(interrupt_eoi_ixp2400)


/*
 * error = interrupt_mask_becc(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_ixp2400, 0, patch_intr)
	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


    ldr     r0, [ip, #IXP2400_IRQ_ENABLE]
    mov     r2, #1
    mov     r2, r2, lsl r1
    str     r2, [ip, #IXP2400_IRQ_ENABLE_CLR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_ixp2400)


/*
 * error = interrupt_unmask_becc(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_ixp2400, 0, patch_intr)
	/*
	 * Get the address of the interrupt registers (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000


    ldr     r0, [ip, #IXP2400_IRQ_ENABLE]
    mov     r2, #1
    mov     r2, r2, lsl r1
    orr     r0, r0, r2


    str     r0, [ip, #IXP2400_IRQ_ENABLE_SET]
	
	mov		r0,#0
	mov		pc, lr

CALLOUT_END(interrupt_unmask_ixp2400)

