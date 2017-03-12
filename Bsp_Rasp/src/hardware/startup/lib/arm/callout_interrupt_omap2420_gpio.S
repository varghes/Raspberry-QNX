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
 * TI OMAP2420 specific GPIO interrupt callouts.
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
 * Patch callout code (for GPIO)
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch_gpio:
	stmdb	sp!, {r4, lr}
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	add		r4, r0, r2			// address of callout routine

	ldr		r0, Lpaddr
	cmp		r0, r1				// already mapped in this address?
	ldreq	r0, Lvaddr
	beq		1f

	str		r1, Lpaddr

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x100			// size of GPIO registers
	bl		callout_io_map
	str		r0, Lvaddr

1:
	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!, {r4, pc}

Lpaddr:	.word	0xFFFFFFFF
Lvaddr:	.word	0xFFFFFFFF

/*
 * -----------------------------------------------------------------------
 * Identify GPIO interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap2420_gpio, 0, interrupt_patch_gpio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * Read Interrupt Mask and Status
	 */
	ldr		r3, [ip, #OMAP2420_GPIO_IRQSTATUS1]
	ldr		r2, [ip, #OMAP2420_GPIO_IRQENABLE1]
	and		r2, r3, r2

	/*
	 * Scan for first set bit
	 */
	mov		r4, #32
	mov		r1, #1

0:	
	subs	r4, r4, #1
	blt		1f
	tst		r2, r1, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	mov		r1, r1, lsl r4
	str		r1, [ip, #OMAP2420_GPIO_CLEARIRQENABLE1]
	
	/*
	 * Clear interrupt status
	 */
	str		r1, [ip, #OMAP2420_GPIO_IRQSTATUS1]

1:
CALLOUT_END(interrupt_id_omap2420_gpio)

/*
 * -----------------------------------------------------------------------
 * Acknowledge specified GPIO interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap2420_gpio, 0, interrupt_patch_gpio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Only unmask interrupt if mask count is zero
     */
	teq		r7, #0
	bne		0f

	mov		r2, #1
	mov		r2, r2, lsl r4
	str		r2, [ip, #OMAP2420_GPIO_SETIRQENABLE1]		/* Set IRQ enable 1 */

0:
CALLOUT_END(interrupt_eoi_omap2420_gpio)

/*
 * -----------------------------------------------------------------------
 * Mask specified GPIO interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_mask_omap2420_gpio, 0, interrupt_patch_gpio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Mask the interrupt
     */
	mov		r2, #1
	mov		r2, r2, lsl r1
    str     r2, [ip, #OMAP2420_GPIO_CLEARIRQENABLE1]		/* Clear IRQ enable 1 */

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap2420_gpio)

/*
 * -----------------------------------------------------------------------
 * Unmask specified GPIO interrupt
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_unmask_omap2420_gpio, 0, interrupt_patch_gpio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Unmask the interrupt
     */
	mov		r2, #1
	mov		r2, r2, lsl r1
	str		r2, [ip, #OMAP2420_GPIO_SETIRQENABLE1]		/* Set IRQ enable 1 */

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap2420_gpio)
