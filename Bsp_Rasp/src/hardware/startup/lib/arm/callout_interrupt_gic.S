#
# Copyright 2011, QNX Software Systems. 
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
# expressly authorized by QSS by written agreement.  For more information 
# (including whether this source code file has been published) please
# email licensing@qnx.com. $
#

/*
 * ARM Generic Interrupt Controller callouts.
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
#include "arm/gic.h"

/*
 * Data in RW storage
 */
rw_intr:	.word	8
#define OFF_GIC_DIST	0		// GIC distributor registers
#define OFF_GIC_CPU		4		// GIC cpu interface registers

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

patch_id:
	stmdb	sp!,{r4,lr}
	add		r2, r0, r2					// address of callout routine
	add		r4, r0, r3					// address of RW data

	/*
	 * Patch callout with the rw offset
	 */
	and		ip, r3, #0xff
	ldr		r1, [r2]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2], #4
	mov		ip, r3, lsr #8
	and		ip, ip, #0xff
	ldr		r1, [r2]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2]

	/*
	 * Map gic_dist_base and store in RW data
	 */
	ldr		r1, =gic_dist_base
	mov		r0, #ARM_GICD_SIZE
	ldr		r1, [r1]
	bl		callout_io_map
	str		r0, [r4, #OFF_GIC_DIST]	

	/*
	 * Map gic_cpu_base and store in RW data
	 */
	ldr		r1, =gic_cpu_base
	mov		r0, #ARM_GICC_SIZE
	ldr		r1, [r1]
	bl		callout_io_map
	str		r0, [r4, #OFF_GIC_CPU]	

	ldmia	sp!,{r4,pc}

/*
 * Patch RW storage only
 */
patch_rw:
	stmdb	sp!,{r4,lr}
	add		r2, r0, r2					// address of callout routine

	/*
	 * Patch callout with the rw offset
	 */
	and		ip, r3, #0xff
	ldr		r1, [r2]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2], #4
	mov		ip, r3, lsr #8
	and		ip, ip, #0xff
	ldr		r1, [r2]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r2]

	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_gic, rw_intr, patch_id)
	mov		ip,     #0x000000ff		// RW data offset (patched)	
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_GIC_CPU]	// cpu interface registers
	ldr		r1, [ip, #OFF_GIC_DIST]	// distributor registers

	/*
	 * Get interrupt ID and check for special cases:
	 * ID0    - used for IPI: immediately send eoi as we need src cpu to eoi
	 * ID1023 - spurious interrupt: set vector to -1
	 */
	ldr		r4, [r0, #ARM_GICC_IAR]
	bics	ip, r4, #ARM_GICC_IAR_SRCID
	streq	r4, [r0, #ARM_GICC_EOIR]
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
	mov		r3, r3, lsl r2						// 1 << (id % 32)
	mov		r2, r4, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #ARM_GICD_ICENABLERn		// ENABLE_CLR[id / 32]
	str		r3, [r1, r2]

	/*
	 * Send EOI to controller
	 */
	str		r4, [r0, #ARM_GICC_EOIR]
0:
CALLOUT_END(interrupt_id_gic)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_gic, rw_intr, patch_rw)
	mov		ip,     #0x000000ff		// RW data offset (patched)	
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

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

	ldr		r0, [ip, #OFF_GIC_DIST]				// distributor registers
	and		r2, r4, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2						// 1 << (id % 32)
	mov		r2, r4, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #ARM_GICD_ISENABLERn		// ENABLE_SET[id / 32]
	str		r3, [r0, r2]
0:
CALLOUT_END(interrupt_eoi_gic)


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
CALLOUT_START(interrupt_mask_gic, rw_intr, patch_rw)
	mov		ip,     #0x000000ff		// RW data offset (patched)	
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_GIC_DIST]				// distributor registers

	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2						// 1 << (id % 32)
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #ARM_GICD_ICENABLERn		// ENABLE_CLR[id / 32]
	str		r3, [r0, r2]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_gic)


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
CALLOUT_START(interrupt_unmask_gic, rw_intr, patch_rw)
	mov		ip,     #0x000000ff		// RW data offset (patched)	
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_GIC_DIST]				// distributor registers

	and		r2, r1, #0x1f
	mov		r3, #1
	mov		r3, r3, lsl r2						// 1 << (id % 32)
	mov		r2, r1, lsr #5
	mov		r2, r2, lsl #2
	add		r2, r2, #ARM_GICD_ISENABLERn		// ENABLE_SET[id / 32]
	str		r3, [r0, r2]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_gic)

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
CALLOUT_START(interrupt_config_gic, 0, 0)
	/*
	 * Use ID0 for IPI
	 */
	teq		r2, #0
	moveq	r0, #INTR_CONFIG_FLAG_IPI
	movne	r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_config_gic)
