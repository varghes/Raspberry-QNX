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
 * PXA250 specific interrupt callouts.
 *
 * This should be usable by any board that uses a PXA250.
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
 * FIXME: WARNING - GPIO_2-GPIO_80 are NEVER DISABLED by the id and mask
 *		  callouts. They can only be disabled by clearing the edge detects,
 *		  which means we can lose interrupts if the device interrupts before
 *		  the eoi/unmask callout is called, since the edge transition occurs
 *		  before the edge detect is reenabled. This means that interrupt
 *		  handlers for devices using these interrupts must cope with the
 *	      potential re-entrancy:
 *			1) pulse/event handlers should mutex the event handler to
 *			   serialise the interrupts (the pulses will be queued).
 *			2) handler functions need to be made re-entrant.
 *
 *		  The real fix would require us to keep a "soft" disable mask
 *		  which is used in the id callout. When a "disabled" interrupt
 *		  is detected, it is marked as "pending", and the edge detect
 *		  is disabled by the id callout. The eoi and unmask callouts
 *		  need to check the "pending" status and invoke interrupt
 *		  handling. This requires some way of invoking the interrupt
 *		  dispatch code in the kernel.
 */

#include "callout.ah"

/*
 * Data in RW storage
 */
rw_intr:	.word	32
#define	OFF_INTR		0		// interrupt controller base
#define	OFF_GPIO		4		// GPIO register base
#define	OFF_GRER0		8		// pxa250_grer0
#define	OFF_GRER1		12		// pxa250_grer1
#define	OFF_GRER2		16		// pxa250_grer2
#define	OFF_GFER0		20		// pxa250_gfer0
#define	OFF_GFER1		24		// pxa250_gfer1
#define	OFF_GFER2		28		// pxa250_gfer2

Lintr_base:	.word	PXA250_INTR_BASE
Lgpio_base:	.word	PXA250_GPIO_BASE

/*
 * -----------------------------------------------------------------------
 * Callout patch routines
 *
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */

/*
 * Patch the id callout, and set up the RW storage
 */
patch_id:
	stmdb	sp!,{r4,lr}
	add		r2, r0, r2			// address of callout routine
	add		r4, r0, r3			// address of rw data

	/*
	 * Patch the callout with the rw offset
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
	 * Map PXA250_INTR_BASE and store in RW data
	 */
	mov		r0, #PXA250_INTR_SIZE
	ldr		r1, Lintr_base
	bl		callout_io_map
	str		r0, [r4, #OFF_INTR]

	/*
	 * MAP PXA250_GPIO_BASE and store in RW data
	 */
	mov		r0, #PXA250_GPIO_SIZE
	ldr		r1, Lgpio_base
	bl		callout_io_map
	str		r0, [r4, #OFF_GPIO]

	/*
	 * Store the grer/gfer values
	 */
	ldr		r0, =pxa250_grer0
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER0]

	ldr		r0, =pxa250_grer1
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER1]

	ldr		r0, =pxa250_grer2
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER2]

	ldr		r0, =pxa250_gfer0
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER0]

	ldr		r0, =pxa250_gfer1
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER1]

	ldr		r0, =pxa250_gfer2
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER2]

	ldmia	sp!,{r4,pc}

/*
 * Patch other callouts
 */
patch_other:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2			// address of callout routine

	/*
	 * Patch callout with rw offset
	 */
	and		ip, r3, #0xff
	ldr		r1, [r4]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r4], #4
	mov		ip, r3, lsr #8
	ldr		r1, [r4]
	bic		r1, r1, #0xff
	orr		r1, r1, ip
	str		r1, [r4]

	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * PXA250 interrupt controller callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_pxa250, rw_intr, patch_id)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_INTR]

	/*
	 * Read pending IRQ interrupts and scan for first set bit
	 */
	ldr		r1, [r0, #PXA250_ICIP]
	mov		r4, #32
	mov		r2, #1
0:	sub		r4, r4, #1
	teq		r4, #7							// interrupts 0-7 are reserved
	subeq	r4, r4, #8
	beq		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #PXA250_ICMR]
	mov		r2, r2, lsl r4
	bic		r1, r1, r2
	str		r1, [r0, #PXA250_ICMR]

	/*
	 * Clear GEDR0 for GPIO_0/GPIO_1 interrupts
	 */
	ldr		r0, [ip, #OFF_GPIO]
	sub		r1, r4, #7
	teq		r1, #1
	teqne	r1, #2
	streq	r1, [r0, #PXA250_GEDR0]
1:
CALLOUT_END(interrupt_id_pxa250)


CALLOUT_START(interrupt_eoi_pxa250, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r0, [ip, #OFF_INTR]

	/*
	 * Enable interrupt source.
	 */
	mov		r2, #1
	ldr		r1, [r0, #PXA250_ICMR]
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #PXA250_ICMR]
0:
CALLOUT_END(interrupt_eoi_pxa250)

/*
 * error = interrupt_mask_pxa250(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_pxa250, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_INTR]

	/*
	 * Clear ICMR bit (1 << interrupt_number)
	 */
	ldr		r2, [r0, #PXA250_ICMR]
	mov		r3, #1
	bic		r2, r2, r3, lsl r1
	str		r2, [r0, #PXA250_ICMR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_pxa250)


/*
 * error = interrupt_unmask_pxa250(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_pxa250, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_INTR]

	/*
	 * Convert vector to bit mask
	 */
	mov		r2, #1
	mov		r2, r2, lsl r1

	/*
	 * Enable interrupt source
	 */
	ldr		r3, [r0, #PXA250_ICMR]
	orr		r3, r3, r2
	str		r3, [r0, #PXA250_ICMR]

	/*
	 * Enable edge detect for GPIO_0/GPIO_1
	 */
	sub		r1, r1, #7
	teq		r1, #1
	teqne	r1, #2
	bne		0f

	ldr		r2, [ip, #OFF_GRER0]
	ldr		r3, [ip, #OFF_GFER0]
	ldr		r0, [ip, #OFF_GPIO]

	ands	r2, r2, r1
	ldrne	r2, [r0, #PXA250_GRER0]
	orrne	r2, r2, r1
	strne	r2, [r0, #PXA250_GRER0]

	ands	r3, r3, r1
	ldrne	r2, [r0, #PXA250_GFER0]
	orrne	r2, r2, r1
	strne	r2, [r0, #PXA250_GFER0]

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_pxa250)

/*
 * -----------------------------------------------------------------------
 * PXA250 GPIO interrupt callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_pxa250_gpio, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_GPIO]

	/*
	 * Read GEDR2
	 */
	ldr		r1, [r0, #PXA250_GEDR2]
	mov		r4, #17
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR2]		// clear edge detect
	add		r4, r4, #64
	b		3f

1:	/*
	 * Read GEDR1
	 */
	ldr		r1, [r0, #PXA250_GEDR1]
	mov		r4, #32
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		2f
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR1]		// clear edge detect
	add		r4, r4, #32
	b		3f

2:	/*
	 * Read GEDR0
	 */
	ldr		r1, [r0, #PXA250_GEDR0]
	mov		r4, #32
	mov		r2, #1
0:	sub		r4, r4, #1
	cmp		r4, #1						// don't do GPIO_1/0
	beq		3f
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR0]		// clear edge detect

	/*
	 * Adjust r4, since vector base starts at GPIO_2
	 *
	 * FIXME: we don't disable the interrupt here, since we can only do that
	 *		  by disabling the edge detects. This would mean we can lose an
	 *		  interrupt if it occurs whilst the edge detects are disabled.
	 */
3:	sub		r4, r4, #2
CALLOUT_END(interrupt_id_pxa250_gpio)


CALLOUT_START(interrupt_eoi_pxa250_gpio, 0, 0)
	/*
	 * FIXME: since we don't mask the interrupt in interrupt_id_pxa250_gpio
	 *		  there is nothing to do in this callout.
	 */
CALLOUT_END(interrupt_eoi_pxa250_gpio)

/*
 * error = interrupt_mask_pxa250_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_pxa250_gpio, 0, 0)
	/*
	 * FIXME: since we can't reliably mask the interrupt we do nothing
	 */
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_pxa250_gpio)


/*
 * error = interrupt_unmask_pxa250_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_pxa250_gpio, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_GPIO]

	add		r1, r1, #2

	/*
	 * Calculate which register to hit and the bitmask
	 */
	cmp		r1, #64
	addge	r0, r0, #4
	addge	ip, ip, #4
	cmplt	r1, #32
	addge	r0, r0, #4
	addge	ip, ip, #4

	and		r1, r1, #31
	mov		r2, #1
	mov		r1, r2, lsl r1

	/*
	 * Enable edge detects
	 */
	ldr		r2, [ip, #OFF_GRER0]
	ldr		r3, [ip, #OFF_GFER0]

	ands	r2, r2, r1
	ldrne	r2, [r0, #PXA250_GRER0]
	orrne	r2, r2, r1
	strne	r2, [r0, #PXA250_GRER0]

	ands	r3, r3, r1
	ldrne	r2, [r0, #PXA250_GFER0]
	orrne	r2, r2, r1
	strne	r2, [r0, #PXA250_GFER0]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_pxa250_gpio)
