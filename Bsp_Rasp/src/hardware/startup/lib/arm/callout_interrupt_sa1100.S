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
 * SA-1100 specific interrupt callouts.
 *
 * This should be usable by any board that uses an SA-1100.
 * The board specific startup must set the following global variables:
 *	sa1100_grer:	mask of valid rising-edge triggered GPIO interrupts
 *	sa1100_gfer:	mask of valid falling-edge triggered GPIO interrupts
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
 * FIXME: WARNING - SA1100 GPIO_12-GPIO27 are NEVER DISABLED by the
 *		  id and mask callouts. They can only be disabled by clearing
 *		  the edge detects, which means we can lose interrupts if the
 *		  device interrupts before the eoi/unmask callout is called,
 *		  since the edge transition occurs before the edge detect is
 *		  reenabled. This means that interrupt handlers for devices
 *		  using these interrupts must cope with potential re-entrancy
 *			1) pulse/event handlers should mutex the event handler to
 *			   serialise the interrupts (the pulses will be queued).
 *			2) handler functions need to be made re-entrant.
 *
 *		  Note that at the moment, the only drivers using the GPIO
 *		  interrupts are the SA1110 CompactFlash and ucb1300. These
 *		  both use pulses and have a mutex protecting the handler.
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

Lintr_base:		.word	SA1100_INTR_BASE
Lgpio_base:		.word	SA1100_GPIO_BASE
Lsa1100_grer:	.word	sa1100_grer
Lsa1100_gfer:	.word	sa1100_gfer

/*
 * -----------------------------------------------------------------------
 * SA1100 interrupt controller callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_sa1100, 0, patch_intr_gpio)
	mov		r0,     #0x000000ff		// interrupt controller base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	mov		r3,     #0x000000ff		// GPIO controller base (patched)
	orr		r3, r3, #0x0000ff00
	orr		r3, r3, #0x00ff0000
	orr		r3, r3, #0xff000000

	/*
	 * Read pending IRQ interrupts and scan for first set bit
	 */
	ldr		r1, [r0, #SA1100_ICIP]
	mov		r4, #32
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 * Clear edge detect status for GPIO_0 to GPIO_10 if necessary
	 */
	ldr		r1, [r0, #SA1100_ICMR]
	mov		r2, r2, lsl r4
	cmp		r4, #10
	strle	r2, [r3, #SA1100_GEDR]
	bic		r1, r1, r2
	str		r1, [r0, #SA1100_ICMR]
1:
CALLOUT_END(interrupt_id_sa1100)


CALLOUT_START(interrupt_eoi_sa1100, 0, patch_intr)
	mov		r0,     #0x000000ff		// interrupt controller base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	/*
	 * Enable interrupt source.
	 */
	ldr		r1, [r0, #SA1100_ICMR]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #SA1100_ICMR]
0:
CALLOUT_END(interrupt_eoi_sa1100)

/*
 * error = interrupt_mask_sa1100(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_sa1100, 0, patch_intr)
	mov		r0,     #0x000000ff		// interrupt controller base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Clear ICMR bit (1 << interrupt_number)
	 */
	ldr		r2, [r0, #SA1100_ICMR]
	mov		r3, #1
	bic		r2, r2, r3, lsl r1
	str		r2, [r0, #SA1100_ICMR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_sa1100)


/*
 * error = interrupt_unmask_sa1100(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_sa1100, 0, patch_intr_gpio_gxer)
	mov		r0,     #0x000000ff		// interrupt controller base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	mov		r2,     #0x000000ff		// GPIO controller base (patched)
	orr		r2, r2, #0x0000ff00
	orr		r2, r2, #0x00ff0000
	orr		r2, r2, #0xff000000

	mov		r3,     #0x000000ff		// sa1100_grer value (patched)
	orr		r3, r3, #0x0000ff00
	orr		r3, r3, #0x00ff0000
	orr		r3, r3, #0xff000000

	mov		ip,     #0x000000ff		// sa1100_gfer value (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	stmdb	sp!, {lr}

	/*
	 * Convert vector to bit mask
	 */
	mov		lr, #1
	mov		lr, lr, lsl r1

	/*
	 * Enable interrupt source
	 * Set condition codes to determine if we need to enable edge detects
	 */
	cmp		r1, #10
	ldr		r1, [r0, #SA1100_ICMR]
	orr		r1, r1, lr
	str		r1, [r0, #SA1100_ICMR]

	/*
	 * Enable edge detect if necessary
	 */
	bgt		0f
	ands	r3, r3, lr
	ldrne	r1, [r2, #SA1100_GRER]
	orrne	r1, r1, r3
	strne	r1, [r2, #SA1100_GRER]

	ands	ip, ip, lr
	ldrne	r1, [r2, #SA1100_GFER]
	orrne	r1, r1, ip
	strne	r1, [r2, #SA1100_GFER]

0:	mov		r0, #0
	ldmia	sp!, {pc}
CALLOUT_END(interrupt_unmask_sa1100)

/*
 * -----------------------------------------------------------------------
 * SA1100 GPIO_11 - GPIO_27 interrupts.
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_sa1100_gpio, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Read edge detect register and scan for first set bit
	 */
	ldr		r1, [r0, #SA1100_GEDR]
	mov		r4, #28
	mov		r2, #1
0:	sub		r4, r4, #1
	cmp		r4, #10
	beq		1f
	tst		r1, r2, lsl r4
	beq		0b
	mov		r2, r2, lsl r4

	/*
	 * Clear edge detect status
	 */
	str		r2, [r0, #SA1100_GEDR]

#ifdef	FIXME	// don't disable edge detect. See note at top of file.
	/*
	 * Disable interrupt by clearing edge detect
	 */
	ldr		r1, [r0, #SA1100_GRER]
	ldr		r3, [r0, #SA1100_GFER]
	bic		r1, r1, r2
	str		r1, [r0, #SA1100_GRER]
	bic		r3, r3, r2
	str		r3, [r0, #SA1100_GFER]
#endif

	/*
	 * Adjust r4 to indicate vector offset from vector_base
	 */
1:	sub		r4, r4, #11
CALLOUT_END(interrupt_id_sa1100_gpio)


#ifdef	FIXME	// edge detect is enabled. See note at top of file.
CALLOUT_START(interrupt_eoi_sa1100_gpio, 0, patch_gpio_gxer)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	mov		r2,     #0x00000000		// GRER mask
	orr		r2, r2, #0x0000ff00
	orr		r2, r2, #0x00ff0000
	orr		r2, r2, #0xff000000

	mov		r3,     #0x00000000		// GFER mask
	orr		r3, r3, #0x0000ff00
	orr		r3, r3, #0x00ff0000
	orr		r3, r3, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	/*
	 * Turn vector into GPIO bitmask
	 */
	mov		ip, #1
	add		r1, r4, #11
	mov		ip,	ip, lsl r1

	/*
	 * Enable appropriate edge detect register
	 */
	ands	r2, r2, ip
	ldrne	r1, [r0, #SA1100_GRER]
	orrne	r1, r1, r2
	strne	r1, [r0, #SA1100_GRER]

	ands	r3, r3, ip
	ldrne	r1, [r0, #SA1100_GFER]
	orrne	r1, r1, r3
	strne	r1, [r0, #SA1100_GFER]
0:
#else
CALLOUT_START(interrupt_eoi_sa1100_gpio, 0, 0)
#endif
CALLOUT_END(interrupt_eoi_sa1100_gpio)


/*
 * error = interrupt_mask_sa1100_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_sa1100_gpio, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Turn vector into GPIO bitmask
	 */
	add		r1, r1, #11
	mov		r2, #1
	mov		r1, r2, lsl r1

#ifdef	FIXME	// don't disable edge detect. See note at top of file.
	/*
	 * Disable edge detect
	 */
	ldr		r2, [r0, #SA1100_GRER]
	ldr		r3, [r0, #SA1100_GFER]
	bic		r2, r2, r1
	str		r2, [r0, #SA1100_GRER]
	bic		r3, r3, r1
	str		r3, [r0, #SA1100_GFER]
#endif
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_sa1100_gpio)


/*
 * error = interrupt_unmask_sa1100_gpio(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_sa1100_gpio, 0, patch_gpio_gxer)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	mov		r2,     #0x00000000		// GRER mask
	orr		r2, r2, #0x0000ff00
	orr		r2, r2, #0x00ff0000
	orr		r2, r2, #0xff000000

	mov		r3,     #0x00000000		// GFER mask
	orr		r3, r3, #0x0000ff00
	orr		r3, r3, #0x00ff0000
	orr		r3, r3, #0xff000000

	/*
	 * Turn vector into GPIO bitmask
	 */
	add		r1, r1, #11
	mov		ip, #1
	mov		ip, ip, lsl r1

	/*
	 * Enable edge detect
	 */
	ands	r2, r2, ip
	ldrne	r1, [r0, #SA1100_GRER]
	orrne	r1, r1, r2
	strne	r1, [r0, #SA1100_GRER]

	ands	r3, r3, ip
	ldrne	r1, [r0, #SA1100_GFER]
	orrne	r1, r1, r3
	strne	r1, [r0, #SA1100_GFER]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_sa1100_gpio)

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
 * Patch callouts with SA1100_INTR_BASE
 */
patch_intr:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	mov		r0, #SA1100_INTR_SIZE
	ldr		r1, Lintr_base
	bl		callout_io_map

	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * Patch callouts with SA1100_GPIO_BASE
 */
patch_gpio:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	mov		r0, #SA1100_GPIO_SIZE
	ldr		r1, Lgpio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * Patch callouts with SA1100_INTR_BASE and SA1100_GPIO_BASE
 */
patch_intr_gpio:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	mov		r0, #SA1100_INTR_SIZE
	ldr		r1, Lintr_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	mov		r0, #SA1100_GPIO_SIZE
	ldr		r1, Lgpio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * Patch callouts with SA1100_GPIO_BASE and GRER/GFER values
 */
patch_gpio_gxer:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	mov		r0, #SA1100_GPIO_SIZE
	ldr		r1, Lgpio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldr		r0, Lsa1100_grer
	ldr		r0, [r0]
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldr		r0, Lsa1100_gfer
	ldr		r0, [r0]
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * Patch callouts with SA1100_INTR_BASE, SA1100_GPIO_BASE and GRER/GFER values
 */
patch_intr_gpio_gxer:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	mov		r0, #SA1100_INTR_SIZE
	ldr		r1, Lintr_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	mov		r0, #SA1100_GPIO_SIZE
	ldr		r1, Lgpio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldr		r0, Lsa1100_grer
	ldr		r0, [r0]
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldr		r0, Lsa1100_gfer
	ldr		r0, [r0]
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}
