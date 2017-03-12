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
#
# $QNXLicenseA:  
# Copyright 2006, QNX Software Systems. All Rights Reserved.
#
# This source code may contain confidential information of QNX Software 
# Systems (QSS) and its licensors.  Any use, reproduction, modification, 
# disclosure, distribution or transfer of this software, or any software 
# that includes or is based upon any of this code, is prohibited unless 
# expressly authorized by QSS by written agreement.  For more information 
# (including whether this source code file has been published) please
# email licensing@qnx.com. $
#

/*
 * MPCore distributed interrupt controller callouts.
 *
 * NOTE: the board specific startup is responsible for defining the
 *       mpcore_scu_base variable and assigning it to the physical
 *       address that the SCU is implemented on that board.
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
#include "arm/mpcore.h"

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 *  [sp] - patch data specified by startup_intrinfo struct
 * -----------------------------------------------------------------------
 */

patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map SCU base
	 */
	ldr		r1, [sp, #8]				// &mpcore_scu_base
	mov		r0, #MPCORE_GIC_DIST_BASE + MPCORE_GIC_DIST_SIZE
	ldr		r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_mpcore, 0, patch_intr)
	mov		ip,     #0x000000ff		// SCU base (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	add		r0, ip, #MPCORE_GIC_CPU_BASE
	add		r1, ip, #MPCORE_GIC_DIST_BASE

	/*
	 * Get interrupt ID and check for special cases:
	 * ID0    - used for IPI: immediately send eoi
	 * ID1023 - spurious interrupt: set vector to -1
	 */
	ldr		r4, [r0, #MPCORE_GIC_CPU_IACK]
	bics	ip, r4, #MPCORE_GIC_CPU_IACK_SRCID	// mask off CPU source id
	streq	r4, [r0, #MPCORE_GIC_CPU_EOI]
	mov		r4, ip
	beq		0f
	mov		ip,     #0x0ff
	orr		ip, ip, #0x300
	teq		r4, ip
	mvneq	r4, #0
	beq		0f

	/*
	 * Mask the interrupt source
	 */
	and		r2, r4, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	mov		r2, r4, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #MPCORE_GIC_DIST_ENABLE_CLR		// ENABLE_CLR[id / 32]
	str		r3, [r1, r2]

	/*
	 * Send EOI to controller
	 */
	str		r4, [r0, #MPCORE_GIC_CPU_EOI]
0:
CALLOUT_END(interrupt_id_mpcore)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_mpcore, 0, patch_intr)
	mov		r0,     #0x000000ff		// SCU base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Special case: IPI interrupt (ID0) is never disabled
	 */
	teq		r4, #0
	beq		0f

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	add		r0, r0, #MPCORE_GIC_DIST_BASE

	and		r2, r4, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	mov		r2, r4, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #MPCORE_GIC_DIST_ENABLE_SET		// ENABLE_SET[id / 32]
	str		r3, [r0, r2]
0:
CALLOUT_END(interrupt_eoi_mpcore)


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
CALLOUT_START(interrupt_mask_mpcore, 0, patch_intr)
	mov		r0,     #0x000000ff		// SCU base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	add		r0, r0, #MPCORE_GIC_DIST_BASE

	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #MPCORE_GIC_DIST_ENABLE_CLR		// ENABLE_CLR[id / 32]
	str		r3, [r0, r2]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_mpcore)


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
CALLOUT_START(interrupt_unmask_mpcore, 0, patch_intr)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		r0,     #0x000000ff
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	add		r0, r0, #MPCORE_GIC_DIST_BASE

	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2							// 1 << (id % 32)
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #MPCORE_GIC_DIST_ENABLE_SET		// ENABLE_SET[id / 32]
	str		r3, [r0, r2]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_mpcore)

/*
 * -----------------------------------------------------------------------
 * Configure interrupt flags for a specified interrupt vector:
 *	r0 - syspage pointer
 *	r1 - intrinfo_entry pointer
 *	r2 - vector number on this controller
 *
 * Returns INTR_CONFIG_FLAG_IPI if this vector is an IPI vector
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_config_mpcore, 0, 0)
	/*
	 * Use ID0 for IPI
	 */
	teq		r2, #0
	moveq	r0, #INTR_CONFIG_FLAG_IPI
	movne	r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_config_mpcore)
