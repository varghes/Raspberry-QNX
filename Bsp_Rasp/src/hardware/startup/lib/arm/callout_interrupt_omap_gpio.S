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
 * TI OMAP 1510 / 5910, and 1610 / 5912 specific GPIO interrupt callouts.
 *
 * The OMAP 1510 / 5910 had a single GPIO interrupt controller. The
 * OMAP 1610 / 5912 has four sections, each with different base addresses.
 * However, the register offsets and functionality is different between
 * the 1510 and the 1610, so different callouts are needed. The original
 * callouts are named interrupt_*_omap_gpio, whereas the new ones are named
 * interrupt_*_omap_gpioN, where N is the section, 1, 2, 3, or 4.
 *
 * There are also callouts for the MPUIO interrupts. However, since 
 * reading of the MPUIO_INT register causes all currently active
 * interrupts to be cleared, only one MPUIO interrupt at a time is
 * supported.
 *
 * interrupt_id_* and interrupt_eoi_* are copied and intermixed with other
 * kernel code during initialisation.
 *
 * They do not follow normal calling conventions, and must fall through
 * to the end, rather than attempting to perform a return instruction.
 *
 * The OMAP_INTR_GENFLAG_* bits in the intrinfo_entry defines which of the
 * following values can be loaded on entry to these code fragments:
 *
 *	r5 - holds the syspageptr				(OMAP_INTR_GENFLAG_SYSPAGE  set)
 *	r6 - holds the intrinfo_entry pointer	(OMAP_INTR_GENFLAG_INTRINFO set)
 *	r7 - holds the interrupt mask count		(OMAP_INTR_GENFLAG_INTRMASK set)
 *
 * The interrupt_id_* routine returns the (controller-relative) level in r4
 */

#include "callout.ah"
#include <arm/omap.h>

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

/*
 * For original OMAP 1510 / 5910 GPIO interrupt controller 
 */

interrupt_patch: 
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x1c	// size of GPIO registers
	ldr		r1, Lintr_base
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

/*
 * For MPUIO interrupt controller on all OMAP versions
 */

interrupt_patch_mpuio: 
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x38	// size of MPUIO registers
	ldr		r1, Lmpuio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

/*
 * The interrupt_id_mpuio callout needs access to the CPU timer
 * registers, to work around a chip errata...
 */


interrupt_patch_mpuio_timer: 
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0x38	// size of MPUIO registers
	ldr		r1, Lmpuio_base
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Map 32kHz OS timer registers
	 */

	mov		r0, #OMAP_32KHZ_TIMER_SIZE	
	ldr		r1, Lostimer_base
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}


/*
 * For OMAP 1610 / 5912 GPIO interrupt controllers
 */
interrupt_patch_gpio1:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #0xf4	// size of GPIO registers
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/*
	 * Patch the callout routine
	 */

	ldmia	sp!,{r4,pc}

interrupt_patch_gpio1_pri_sec:
/* 
 * We need to map registers on the primary and secondary OMAP interrupt
 * controllers here, since the NEW_IRQ_AGR bit of each controller needs to
 * be hit after the IRQ from the cascading controller has cleared / masked 
 * the interrupting level, otherwise excessive/unnecessary interrupt ID 
 * callouts are run. See OMAP main interrupt callouts for full description
 * of issue.
 */
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map GPIO interrupt controller registers
	 */
	mov		r0, #0xf4	// size of GPIO registers
	ldr		r1, [sp, #8]
	ldr		r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	/* map the primary interrupt controller registers */
	mov     r0, #OMAP_INTR_SIZE // size of interrupt registers
	ldr     r1, Lintr1_base
	bl      callout_io_map
	CALLOUT_PATCH   r4, r0, r1, r2, ip

	/* map the secondary interrupt controller registers */
	mov     r0, #OMAP_INTR_SIZE // size of interrupt registers
	ldr     r1, Lintr2_base
	bl      callout_io_map
	CALLOUT_PATCH   r4, r0, r1, r2, ip

	/*
	 * Patch the callout routine
	 */

	ldmia	sp!,{r4,pc}

Lintr1_base: 	.word	OMAP_INTR1_BASE
Lintr2_base: 	.word	OMAP_INTR2_BASE
Lintr_base:		.word	OMAP_GPIO_BASE 
Lmpuio_base:	.word	OMAP_MPUIO_BASE
Lostimer_base:	.word	OMAP_32KHZ_TIMER_BASE

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source. Bit 15 == IRQ 15 == highest priority
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap_gpio, 0, interrupt_patch)
	/*
	 * Get the GPIO interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/*
	 * read the GPIO interrupt status and mask registers,
	 * and determine the highest priority unmasked interrupt
	 */

	ldrh	r4, [ip, #GPIO_INTERRUPT_STATUS_REG]
	ldrh	r5, [ip, #GPIO_INTERRUPT_MASK_REG]
	bic		r1, r4, r5

	/*
	 * check for glitch
	 */

	cmp		r1, #0	
	bne		1f
	mov		r4, #-1
	b		3f

1:
	mov		r4, #16
    mov     r2, #1

0:  sub     r4, r4, #1
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */

    mov     r2, r2, lsl r4

	orr		r5, r2, r5
    strh    r5, [ip, #GPIO_INTERRUPT_MASK_REG]

	/*
	 * clear the interrupt
	 */

	strh	r2, [ip, #GPIO_INTERRUPT_STATUS_REG]
3:
CALLOUT_END(interrupt_id_omap_gpio)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap_gpio, 0, interrupt_patch)
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

    teq     r7, #0
    bne     0f

	mov		r2, #1
	mov		r2, r2, lsl r4	

	/*
	 * unmask interrupt
	 */

    ldrh    r1, [ip, #GPIO_INTERRUPT_MASK_REG]
    bic     r1, r1, r2
    strh    r1, [ip, #GPIO_INTERRUPT_MASK_REG]

0:
CALLOUT_END(interrupt_eoi_omap_gpio)


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
CALLOUT_START(interrupt_mask_omap_gpio, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1
    /*
     * Mask the interrupt
     */
    ldrh    r0, [ip, #GPIO_INTERRUPT_MASK_REG]
    orr     r0, r0, r2, lsl r1
    strh    r0, [ip, #GPIO_INTERRUPT_MASK_REG]

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap_gpio)


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
CALLOUT_START(interrupt_unmask_omap_gpio, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1

    /*
     * Unmask the interrupt
     */
    ldrh    r0, [ip, #GPIO_INTERRUPT_MASK_REG]
    mov     r2, r2, lsl r1
    bic     r0, r0, r2
    strh    r0, [ip, #GPIO_INTERRUPT_MASK_REG]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap_gpio)


/*
 * -----------------------------------------------------------------------
 * Identify interrupt source. Bit 15 == IRQ 15 == highest priority
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap_gpio1, 0, interrupt_patch_gpio1_pri_sec)
	/*
	 * Get the GPIO interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* get primary OMAP interrupt controller base */
	mov		r0,     #0x000000ff
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/* get secondary OMAP interrupt controller base */
	mov		r3,     #0x000000ff
	orr		r3, r3, #0x0000ff00
	orr		r3, r3, #0x00ff0000
	orr		r3, r3, #0xff000000

	/*
	 * read the GPIO interrupt status and mask registers,
	 * and determine the highest priority unmasked interrupt
	 */

	ldrh	r4, [ip, #OMAP_GPIO_IRQSTATUS1]
	ldrh	r5, [ip, #OMAP_GPIO_IRQENABLE1]
	and		r1, r4, r5

	/*
	 * check for glitch
	 */

	cmp		r1, #0	
	bne		1f
	mov		r4, #-1
	b		3f

1:
	mov		r4, #16
    mov     r2, #1

0:  sub     r4, r4, #1
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */

    mov     r2, r2, lsl r4
    strh    r2, [ip, #OMAP_GPIO_CLEAR_IRQENABLE1]

	/*
	 * clear the interrupt
	 */
	strh	r2, [ip, #OMAP_GPIO_IRQSTATUS1]

	/* hit NEW_IRQ_AGREEMENT bit on secondary */
    ldr     r1,[r3, #OMAP_INTR_CONTROL_REG]
    orr     r1, r1, #0x01
    str     r1,[r3, #OMAP_INTR_CONTROL_REG]

	/* hit NEW_IRQ_AGREEMENT bit on primary */
    ldr     r1,[r0, #OMAP_INTR_CONTROL_REG]
    orr     r1, r1, #0x01
    str     r1,[r0, #OMAP_INTR_CONTROL_REG]
3:
CALLOUT_END(interrupt_id_omap_gpio1)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap_gpio1, 0, interrupt_patch_gpio1)
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

    teq     r7, #0
    bne     0f

	mov		r2, #1
	mov		r2, r2, lsl r4	

	/*
	 * unmask interrupt
	 */
    strh    r2, [ip, #OMAP_GPIO_SET_IRQENABLE1]

0:
CALLOUT_END(interrupt_eoi_omap_gpio1)


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
CALLOUT_START(interrupt_mask_omap_gpio1, 0, interrupt_patch_gpio1)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1
    /*
     * Mask the interrupt
     */
    mov     r2, r2, lsl r1
    strh    r2, [ip, #OMAP_GPIO_CLEAR_IRQENABLE1]

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap_gpio1)


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
CALLOUT_START(interrupt_unmask_omap_gpio1, 0, interrupt_patch_gpio1)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1

    /*
     * Unmask the interrupt
     */
    mov     r2, r2, lsl r1
    strh    r2, [ip, #OMAP_GPIO_SET_IRQENABLE1]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap_gpio1)


/*
 * -----------------------------------------------------------------------
 * Identify interrupt source. Bit 15 == IRQ 15 == highest priority
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_omap_mpuio, 0, interrupt_patch_mpuio_timer)
	/*
	 * Get the MPUIO interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0,     #0x000000ff
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * read the MPUIO interrupt status and mask registers,
	 * and determine the highest priority unmasked interrupt
	 */
	
	/*
	 * ERRATA - reading the MPUIO_INT register when the 32 kHz clock
	 * is low or in transition can cause a lock-up of the state machine,
	 * such that further interrupts are ignored. Work-around is to poll
	 * a timer that's running off the 32kHz clock, and only read MPUIO_INT
	 * after the timer value has changed, indicating an increment.
	 * The OS timer registers have been mapped in for this purpose.
	 */


	ldr		r4, [r0, #OMAP_32KHZ_TIMER_TCR]
1:
	ldr		r5, [r0, #OMAP_32KHZ_TIMER_TCR]
	cmp		r4, r5
	beq		1b

	ldrh	r4, [ip, #OMAP_MPUIO_INT]	/* reading this register clears all ints */
	ldrh	r5, [ip, #OMAP_MPUIO_MASKIT]
	bic		r1, r4, r5

	/*
	 * check for glitch
	 */

	cmp		r1, #0	
	bne		1f
	mov		r4, #-1
	b		3f

1:
	mov		r4, #16
    mov     r2, #1

0:  sub     r4, r4, #1
    tst     r1, r2, lsl r4
    beq     0b

    /*
     * Mask the interrupt
     */

	mov     r2, r2, lsl r4
	orr		r5, r2, r5
	strh    r5, [ip, #OMAP_MPUIO_MASKIT]


3:
CALLOUT_END(interrupt_id_omap_mpuio)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_omap_mpuio, 0, interrupt_patch_mpuio)
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

    teq     r7, #0
    bne     0f

	mov		r2, #1
	mov		r2, r2, lsl r4	

	/*
	 * unmask interrupt
	 */

    ldrh    r1, [ip, #OMAP_MPUIO_MASKIT]
    bic     r1, r1, r2
    strh    r1, [ip, #OMAP_MPUIO_MASKIT]

0:
CALLOUT_END(interrupt_eoi_omap_mpuio)


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
CALLOUT_START(interrupt_mask_omap_mpuio, 0, interrupt_patch_mpuio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1
    /*
     * Mask the interrupt
     */
    ldrh    r0, [ip, #OMAP_MPUIO_MASKIT]
    orr     r0, r0, r2, lsl r1
    strh    r0, [ip, #OMAP_MPUIO_MASKIT]

    mov     r0, #0
    mov     pc, lr
CALLOUT_END(interrupt_mask_omap_mpuio)


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
CALLOUT_START(interrupt_unmask_omap_mpuio, 0, interrupt_patch_mpuio)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    mov     r2, #1

    /*
     * Unmask the interrupt
     */
    ldrh    r0, [ip, #OMAP_MPUIO_MASKIT]
    mov     r2, r2, lsl r1
    bic     r0, r0, r2
    strh    r0, [ip, #OMAP_MPUIO_MASKIT]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_omap_mpuio)



