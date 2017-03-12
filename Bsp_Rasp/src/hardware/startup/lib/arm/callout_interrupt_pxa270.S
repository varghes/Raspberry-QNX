/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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
 * PXA270-specific interrupt callouts.
 *
 * PXA270 is an enhancement of PXA25x.
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
 * FIXME: WARNING - GPIO_2-GPIO_120 are NEVER DISABLED by the id and mask
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
#include "arm/pxa270.h"

/*
 * Data in RW storage
 */
rw_intr:	.word	40
#define	OFF_INTR		0		// interrupt controller base
#define	OFF_GPIO		4		// GPIO register base
#define	OFF_GRER0		8		// pxa270_grer0
#define	OFF_GRER1		12		// pxa270_grer1
#define	OFF_GRER2		16		// pxa270_grer2
#define	OFF_GRER3		20		// pxa270_grer3
#define	OFF_GFER0		24		// pxa270_gfer0
#define	OFF_GFER1		28		// pxa270_gfer1
#define	OFF_GFER2		32		// pxa270_gfer2
#define	OFF_GFER3		36		// pxa270_gfer3

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
	mov		r0, #0x200
	ldr		r1, Lgpio_base
	bl		callout_io_map
	str		r0, [r4, #OFF_GPIO]

	/*
	 * Store the grer/gfer values
	 */
	ldr		r0, =pxa270_grer0
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER0]

	ldr		r0, =pxa270_grer1
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER1]

	ldr		r0, =pxa270_grer2
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER2]

	ldr		r0, =pxa270_grer3
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GRER3]

	ldr		r0, =pxa270_gfer0
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER0]

	ldr		r0, =pxa270_gfer1
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER1]

	ldr		r0, =pxa270_gfer2
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER2]

	ldr		r0, =pxa270_gfer3
	ldr		r0, [r0]
	str		r0, [r4, #OFF_GFER3]

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

CALLOUT_START(interrupt_id_pxa270, rw_intr, patch_id)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_INTR]

	/*
	 * Read highest priority pending interrupt
	 * Check for VAL_IRQ
	 * NOTE According to errata, IRQ and FIQ values are swapped
	 */
	ldr     r1, [r0, #PXA270_ICHP]
	mov     r2, #1
	tst     r1, #0x80000000
	moveq	r4, #-1					// no interrupt asserted
	beq     1f

	/*
	 * Extract interrupt id
	 */
	mov     r4, r1, lsr #16
	and     r4, r4, #0x3f

	/*
	 * Mask the interrupt source
	 */
	cmp     r4, #32
	blt     2f

	ldr     r1, [r0, #PXA270_ICMR2]
	sub     r3, r4, #32
	mov     r2, r2, lsl r3
	bic		r1, r1, r2
	str     r1, [r0, #PXA270_ICMR2]
	ldr     r1, [r0, #PXA270_ICMR2]	// read back to ensure write has completed
	b       1f

2:
	ldr     r1, [r0, #PXA250_ICMR]
	mov     r2, r2, lsl r4
	bic		r1, r1, r2
	str     r1, [r0, #PXA250_ICMR]
	ldr     r1, [r0, #PXA250_ICMR]	// read back to ensure write has completed

	/*
	 * Clear GEDR0 for GPIO_0/GPIO_1 interrupts
	 */
	ldr		r0, [ip, #OFF_GPIO]
	sub		r1, r4, #8
	teq		r1, #0
	teqne	r1, #1

	moveq	r2, #1
	moveq	r2, r2, lsl r1

	streq	r2, [r0, #PXA250_GEDR0]
1:
CALLOUT_END(interrupt_id_pxa270)


CALLOUT_START(interrupt_eoi_pxa270, rw_intr, patch_other)
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
	cmp		r4, #32
	blt		2f

	sub		r4, r4, #32
	ldr		r1, [r0, #PXA270_ICMR2]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #PXA270_ICMR2]
	ldr		r1, [r0, #PXA270_ICMR2]	// read back to ensure write has completed
	b       0f

2:
	ldr     r1, [r0, #PXA250_ICMR]
	mov		r2, #1
	orr     r1, r1, r2, lsl r4
	str     r1, [r0, #PXA250_ICMR]
	ldr		r1, [r0, #PXA250_ICMR]	// read back to ensure write has completed

0:
CALLOUT_END(interrupt_eoi_pxa270)

/*
 * error = interrupt_mask_pxa270(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_pxa270, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_INTR]

	cmp		r1, #32
	blt		2f

	/*
	 * Clear ICMR bit (1 << interrupt_number)
	 */

	sub		r1, r1, #32
	ldr		r2, [r0, #PXA270_ICMR2]
	mov		r3, #1
	bic		r2, r2, r3, lsl r1
	str		r2, [r0, #PXA270_ICMR2]
	ldr		r2, [r0, #PXA270_ICMR2]	// read back to ensure write has completed
	b		0f

2:
	ldr		r2, [r0, #PXA250_ICMR]
	mov		r3, #1
	bic		r2, r2, r3, lsl r1
	str		r2, [r0, #PXA250_ICMR]
	ldr		r2, [r0, #PXA250_ICMR]	// read back to ensure write has completed

0:
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_pxa270)


/*
 * error = interrupt_unmask_pxa270(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_pxa270, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_INTR]

	cmp		r1, #32
	blt		2f

	/*
	 * Convert vector to bit mask
	 */
	sub		r1, r1, #32
	mov		r2, #1
	mov		r2, r2, lsl r1

	/*
	 * Enable interrupt source
	 */
	ldr		r3, [r0, #PXA270_ICMR2]
	orr		r3, r3, r2
	str		r3, [r0, #PXA270_ICMR2]
	ldr		r3, [r0, #PXA270_ICMR2]	// read back to ensure write has completed
	b		0f

2:
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
	ldr		r3, [r0, #PXA250_ICMR]	// read back to ensure write has completed

	/*
	 * Enable edge detect for GPIO_0/GPIO_1
	 */
	sub		r1, r1, #8
	teq		r1, #0
	teqne	r1, #1
	bne		0f

	ldr		r3, [ip, #OFF_GFER0]
	ldr		r0, [ip, #OFF_GPIO]

	mov		r2, #1
	mov		r2, r2, lsl r1
    
	ands	r3, r3, r2
	ldrne	r3, [r0, #PXA250_GFER0]
	orrne	r3, r3, r2
	strne	r3, [r0, #PXA250_GFER0]

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_pxa270)

/*
 * -----------------------------------------------------------------------
 * PXA250 GPIO interrupt callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_pxa270_gpio, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_GPIO]

	mov		r4, #-1
	mov		r2, #1

	/*
	 * Read GEDR3
	 */
	ldr		r1, [r0, #PXA270_GEDR3]
	ands	r1, r1, #0x01FFFFFF
	beq		1f

	mov		r4, #25
0:	subs	r4, r4, #1
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA270_GEDR3]		// clear edge detect
	add		r4, r4, #96 - 2 			// Adjust r4, since vector base starts at GPIO_2
	b		2f

1:
	/*
	 * Read GEDR2
	 */
	ldr		r1, [r0, #PXA250_GEDR2]
	ands	r1, r1, #0xFFFFFFFF
	beq		1f

	mov		r4, #32
0:	subs	r4, r4, #1
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR2]		// clear edge detect
	add		r4, r4, #64 - 2 			// Adjust r4, since vector base starts at GPIO_2
	b		2f

1:	/*
	 * Read GEDR1
	 */
	ldr		r1, [r0, #PXA250_GEDR1]
	ands	r1, r1, #0xFFFFFFFF
	beq		1f

	mov		r4, #32
0:	subs	r4, r4, #1
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR1]		// clear edge detect
	add		r4, r4, #32 - 2 			// Adjust r4, since vector base starts at GPIO_2
	b		2f

1:	/*
	 * Read GEDR0
	 */
	ldr		r1, [r0, #PXA250_GEDR0]
	ands	r1, r1, #0xFFFFFF18
	beq		2f

	mov		r4, #32
0:	sub		r4, r4, #1
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #PXA250_GEDR0]		// clear edge detect
	add		r4, r4, #-2					// Adjust r4, since vector base starts at GPIO_2

	/*
	 *
	 * FIXME: we don't disable the interrupt here, since we can only do that
	 *		  by disabling the edge detects. This would mean we can lose an
	 *		  interrupt if it occurs whilst the edge detects are disabled.
	 */
2:
CALLOUT_END(interrupt_id_pxa270_gpio)


CALLOUT_START(interrupt_eoi_pxa270_gpio, 0, 0)
	/*
	 * FIXME: since we don't mask the interrupt in interrupt_id_pxa270_gpio
	 *		  there is nothing to do in this callout.
	 */
CALLOUT_END(interrupt_eoi_pxa270_gpio)

/*
 * error = interrupt_mask_pxa270_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_pxa270_gpio, 0, 0)
	/*
	 * FIXME: since we can't reliably mask the interrupt we do nothing
	 */
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_pxa270_gpio)


/*
 * error = interrupt_unmask_pxa270_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_pxa270_gpio, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_GPIO]

	add		r1, r1, #2

	/*
	 * Calculate which register to hit and the bitmask
	 */
	bic		r2, r1, #0x1F		// r2 >> 3 is the register offset
	and		r1, r1, #0x1F		// r1, shift count
	add		ip, ip, r2, lsr #3

	cmp		r2, #0x60
	addeq	r0, r0, #0x100		// GR/FER3 = GR/FER0 + 0x100
	addne	r0, r0, r2, lsr #3	// GR/FERx = GR/FER0 + offset

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
CALLOUT_END(interrupt_unmask_pxa270_gpio)



