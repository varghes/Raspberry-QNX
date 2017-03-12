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
 * TI OMAP2420 specific interrupt callouts.
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
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/omap2420.h>


/*
 * -----------------------------------------------------------------------
 * Routines to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */

interrupt_patch_omap2420:
	stmdb	sp!, {r4, lr}
	add		r4, r0, r2					// address of callout routine

	ldr		r0, Lvaddr
	cmp		r0, #-1
	bne		1f

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #OMAP2420_INTC_SIZE		// size to map
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map
	str		r0, Lvaddr

1:
	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!, {r4, pc}

Lvaddr:	.word	-1

/* -----------------------------------------------------------------------
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap2420, 0, interrupt_patch_omap2420)
	/*
	 * Get the AINTC interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	ldr		r4, [ip, #OMAP2420_INTC_SIR_IRQ]
	cmp		r4, #95
	movgt	r4, #-1
	bgt		1f

	/*
	 * Mask the interrupt
	 */
	and		r1, r4, #31
	sub		r2, r4, r1
	add		r2, r2, ip
	mov		r3, #1
	mov		r1, r3, lsl r1
	str		r1, [r2, #OMAP2420_INTC_MIR_SET0]

	/*
	 * Acknowledge IRQ
	 */
	str		r3, [ip, #OMAP2420_INTC_CONTROL]
1:
CALLOUT_END(interrupt_id_omap2420)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap2420, 0, interrupt_patch_omap2420)
	/*
	 * Get the AINTC interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		1f

	/*
	 * unmask interrupt
	 */
	and		r1, r4, #31
	sub		r2, r4, r1
	add		r2, r2, ip
	mov		r3, #1
	mov		r1, r3, lsl r1
	str		r1, [r2, #OMAP2420_INTC_MIR_CLEAR0]
1:
CALLOUT_END(interrupt_eoi_omap2420)


/*
 * -----------------------------------------------------------------------
 * Mask specified interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_mask_omap2420, 0, interrupt_patch_omap2420)
	/*
	 * Get the AINTC interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Mask the interrupt
	 */
	and		r2, r1, #31
	sub		r1, r1, r2
	add		r1, r1, ip
	mov		r3, #1
	mov		r3, r3, lsl r2
	str		r3, [r1, #OMAP2420_INTC_MIR_SET0]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_omap2420)


/*
 * -----------------------------------------------------------------------
 * Unmask specified interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_unmask_omap2420, 0, interrupt_patch_omap2420)
	/*
	 * Get the AINTC interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Unmask the interrupt
	 */
	and		r2, r1, #31
	rsb		r1, r2, r1
	add		r1, r1, ip
	mov		r3, #1
	mov		r3, r3, lsl r2
	str		r3, [r1, #OMAP2420_INTC_MIR_CLEAR0]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap2420)

