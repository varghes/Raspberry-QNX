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
 * SA-1111 specific interrupt callouts.
 *
 * This should be usable by any board that uses an SA-1111.
 * The board specific startup must set the following global variables:
 *	sa11x1_base:	base physical address of the SA-1111 chip registers.
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
 *
 * NOTE: there are problems with the masking of various SA1111 interrupt
 *		 sources via INTEN0/INTEN1:
 *
 *		 - some sources will lose interrupts when they are masked.
 *		   The current workaround is to NOT mask them via INTEN0/1.
 *		   This means that the driver interrupt handlers must be prepared
 *		   to deal with the fact that the interrupt is NOT masked during
 *		   the execution of the handler.
 *
 *		 - some sources will constantly re-assert the interrupt unless it
 *		   it is masked via INTEN0/1. Experimentation using the INTTEST
 *		   registers shows the following interrupt sources behave this way:
 *			. 18, 19 (PS/2 mouse TX and RX)
 *			. 21, 22 (PS/2 trackpad TX and RX)
 *			. 24, 25 (SSP TX and TX)
 *			. 36, 37 (Audio TFSR abd RFSR)
 *
 *		   The current workaround is to specifically check for these sources
 *		   and mask/unmask them via INTEN0/1.
 */

#include "callout.ah"

Lsa11x1_base:	.word	sa11x1_base

CALLOUT_START(interrupt_id_sa1111, 0, patch_intr)
	mov		r0,     #0x000000ff		// INTR base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Read INTSTAT1 and scan for first set bit
	 */
	ldr		r1, [r0, #SA1111_INTSTATCLR1]
	mov		r4, #23
	teq		r1, #0
	beq		1f
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Build bitmask and adjust vector number and enable register address
	 */
	mov		r2, r2, lsl r4
	add		r4, r4, #32
	add		r0, r0, #4
	b		2f

1:	/*
	 * Read INTSTAT0 and scan for first set bit
	 */
	ldr		r1, [r0, #SA1111_INTSTATCLR0]
	mov		r4, #32
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		3f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Build bitmask
	 */
	mov		r2, r2, lsl r4

2:	/*
	 * Check if we must disable interrupt source.
	 * See comment at top of this file.
	 *
	 * FIXME: we currently only check the PS/2 RX interrupts.
	 */
	teq		r4, #19						// PS/2 mouse RX
	teqne	r4, #22						// PS/2 trackpad RX
	bne		2f
	ldr		r1, [r0, #SA1111_INTEN0]
	bic		r1, r1, r2
	str		r1, [r0, #SA1111_INTEN0]
2:	/*
	 * Clear interrupt status
	 */
	str		r2, [r0, #SA1111_INTSTATCLR0]
3:
CALLOUT_END(interrupt_id_sa1111)


CALLOUT_START(interrupt_eoi_sa1111, 0, patch_intr)
	mov		r0,     #0x000000ff		// INTR base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	/*
	 * Check if we must unmask the interrupt
	 * See comment at top of this file.
	 *
	 * FIXME: we currently only check the PS/2 RX interrupts
	 */
	teq		r4, #19						// PS/2 mouse RX
	teqne	r4, #22						// PS/2 trackpad RX
	bne		0f

	/*
	 * Adjust vector and enable register address if necessary and build mask
	 */
	mov		ip, #1
	cmp		r4, #32
	subge	r2, r4, #32
	addge	r0, r0, #4
	movge	ip, ip, lsl r2
	movlt	ip, ip, lsl r4

	/*
	 * Enable interrupt source
	 */
	ldr		r2, [r0, #SA1111_INTEN0]
	orr		r2, r2, ip
	str		r2, [r0, #SA1111_INTEN0]
0:
CALLOUT_END(interrupt_eoi_sa1111)


/*
 * error = interrupt_mask_sa1111(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_sa1111, 0, patch_intr)

	mov		r0,     #0x000000ff		// INTR base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Check if we must mask the interrupt
	 * See comment at top of this file.
	 *
	 * FIXME: we currently only check the PS/2 RX interrupts
	 */
	teq		r1, #19						// PS/2 mouse RX
	teqne	r1, #22						// PS/2 trackpad RX
	bne		0f

	/*
	 * Adjust vector and enable register address if necessary and build mask
	 */
	mov		ip, #1
	cmp		r1, #32
	subge	r1, r1, #32
	addge	r0, r0, #4
	mov		ip, ip, lsl r1

	ldr		r2, [r0, #SA1111_INTEN0]
	bic		r2, r2, ip
	str		r2, [r0, #SA1111_INTEN0]

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_sa1111)


/*
 * error = interrupt_unmask_sa1111(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_sa1111, 0, patch_intr)
	mov		r0,     #0x000000ff		// INTR base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Adjust vector and enable register address if necessary and build mask
	 */
	mov		ip, #1
	cmp		r1, #32
	subge	r1, r1, #32
	addge	r0, r0, #4
	mov		ip, ip, lsl r1

	/*
	 * Clear interrupt status and enable interrupt source
	 */
	ldr		r2, [r0, #SA1111_INTEN0]
	orr		r2, r2, ip
	str		r2, [r0, #SA1111_INTEN0]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_sa1111)

/*
 * Patch callouts that require only SA1100_INTR
 *
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 */
patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	ldr		r1, Lsa11x1_base
	ldr		r1, [r1]
	mov		r0, #SA1111_INTR_SIZE		// size of interrupt registers
	add		r1, r1, #SA1111_INTR
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}
