/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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
 * EP93xx specific interrupt callouts for cascaded GPIO
 *
 * There are 2 sets of callouts, 1 for the PORTA amd B GPIO's which are
 * identified by the general gpio interrupt and a second set for Port F (which
 * has a separate interrupt for each pin although this is not relevant to the
 * callout). Without getting much trickier with the patcher, the 2 different
 * sets of callouts are required.
 * 
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

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 *  r4 - patch data pointer
 * -----------------------------------------------------------------------
 */
interrupt_patch:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/* Map interrupt controller registers */
	mov		r0, #EP93xx_GPIO_CTRL_SIZE
	ldr		r1, Lpaddr
	mov		r2, #0xB00				// PROT_READ|PROT_WRITE|PROT_NOCACHE
	bl		callout_memory_map

	/* Patch the callout routines with the mapped address (in r0) */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	EP93xx_GPIO_CTRL_BASE

/*
================================================================================
                              PORT A/B callouts
================================================================================
*/
/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 *
 * GPIO prioritization will be PORTA then PORTB with the bit scan starting
 * with the MSb. Therefore the 16 GPIO's are prioritized as ...
 * interrupts will be prioritized as
 *		- PORT A bit 31 (highest)
 *		- PORT B bit 0 (lowest)
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_ep93xx_gpio_ab, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r4, #-1		/* spurious assumed */

	/* Read pending interrupts from GPIO Port A */
	mov		r3, #0
	mov		r0, #EP93xx_GPIO_PORTA_INT_CLEAR
	ldr		r2, [ip, #EP93xx_GPIO_PORTA_INT_STATUS]
	cmp		r2, #0
	bne		id_scan

	/* Read pending interrupts from GPIO Port B */
	mov		r3, #8
	mov		r0, #EP93xx_GPIO_PORTB_INT_CLEAR
	ldr		r2, [ip, #EP93xx_GPIO_PORTB_INT_STATUS]
	cmp		r2, #0
	bne		id_scan
	beq		0f	// spurious

id_scan:
	/* Scan for first set bit (starting with MSb) */
	mov		r4, #8
	mov		r1, #1
1:	
	subs	r4, r4, #1
	blt		0f			/* r4 has rolled to -1 */
	tst		r2, r1, lsl r4
	beq		1b

	/* clear and disable/mask the interrupt */
	mov		r1, r1, lsl r4
	str		r1, [ip, r0]	// clear the interrupt (in case its edge sensitive)
	add		r0, #4			// move to the enable register
	ldr		r2, [ip, r0]
	bic		r2, r2, r1		// disable the interrupt
	str		r2, [ip, r0]
	add		r4, r4, r3
0:

CALLOUT_END(interrupt_id_ep93xx_gpio_ab)

/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_ep93xx_gpio_ab, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* Only unmask interrupt if mask count is zero */
	teq		r7, #0
	bne		0f

	/* normalize the vector to the appropriate gpio port */
	mov		r7,	#EP93xx_GPIO_PORTA_INT_ENABLE
	cmp		r4, #8
	blt		1f
	mov		r7,	#EP93xx_GPIO_PORTB_INT_ENABLE
	sub		r4, #8
	cmp		r4, #8
	blt		1f
	b		0f		/* not the correct id */
1:
	mov		r2, #1
	mov		r2, r2, lsl r4
	
	/* re-enable (unmask) the interrupt */
	ldr		r1, [ip, r7]
	orr		r1, r1, r2
	str		r1, [ip, r7]
0:
CALLOUT_END(interrupt_eoi_ep93xx_gpio_ab)


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
CALLOUT_START(interrupt_mask_ep93xx_gpio_ab, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0, #0	// error

	/* normalize the vector to the appropriate gpio port */
	mov		r2,	#EP93xx_GPIO_PORTA_INT_ENABLE
	cmp		r1, #8
	blt		1f
	mov		r2,	#EP93xx_GPIO_PORTB_INT_ENABLE
	sub		r1, #8
	cmp		r1, #8
	blt		1f
	b		0f		/* not the correct id */

1:
	mov		r0, #1
	mov		r0, r0, lsl r1
	
	/* disable (mask) the interrupt */
	ldr		r1, [ip, r2]
	bic		r1, r1, r0
	str		r1, [ip, r2]
0:

CALLOUT_END(interrupt_mask_ep93xx_gpio_ab)


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
CALLOUT_START(interrupt_unmask_ep93xx_gpio_ab, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0, #0	// error

	/* normalize the vector to the appropriate gpio port */
	mov		r2,	#EP93xx_GPIO_PORTA_INT_ENABLE
	cmp		r1, #8
	blt		1f
	mov		r2,	#EP93xx_GPIO_PORTB_INT_ENABLE
	sub		r1, #8
	cmp		r1, #8
	blt		1f
	b		0f		/* not the correct id */

1:
	mov		r0, #1
	mov		r0, r0, lsl r1
	
	/* enable (unmask) the interrupt */
	ldr		r1, [ip, r2]
	orr		r1, r1, r0
	str		r1, [ip, r2]
0:

CALLOUT_END(interrupt_unmask_ep93xx_gpio_ab)

/*
 * -----------------------------------------------------------------------
 * interrupt config
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_config_ep93xx_gpio_ab, 0, 0)

	mov		r0, #0
	mov		pc, lr

CALLOUT_END(interrupt_config_ep93xx_gpio_ab)

/*
================================================================================
                              PORT F callouts
================================================================================
*/

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 *
 * GPIO prioritization will be with the bit scan starting with the MSb.
 * Some implementation have fewer than 8 bits (ex. 9301) but we don't assume
 * this in the callout
 *
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_ep93xx_gpio_f, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r4, #-1		/* spurious assumed */

	/* Read pending interrupts from GPIO Port F */
	ldr		r2, [ip, #EP93xx_GPIO_PORTF_INT_STATUS]
	cmp		r2, #0
	beq		0f

	/* Scan for first set bit (starting with MSb) */
	mov		r4, #8
	mov		r1, #1
1:	
	subs	r4, r4, #1
	blt		0f			/* r4 has rolled to -1 */
	tst		r2, r1, lsl r4
	beq		1b

	/* clear and disable/mask the interrupt */
	mov		r1, r1, lsl r4
	str		r1, [ip, #EP93xx_GPIO_PORTF_INT_CLEAR]	// clear the interrupt (in case its edge sensitive)
	ldr		r2, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
	bic		r2, r2, r1
	str		r2, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
0:

CALLOUT_END(interrupt_id_ep93xx_gpio_f)

/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_ep93xx_gpio_f, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* Only unmask interrupt if mask count is zero */
	teq		r7, #0
	bne		0f

	/* normalize the vector to the appropriate gpio port */
	cmp		r4, #8
	bge		0f		/* not the correct id */

	mov		r2, #1
	mov		r2, r2, lsl r4
	
	/* re-enable (unmask) the interrupt */
	ldr		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
	orr		r1, r1, r2
	str		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
0:
CALLOUT_END(interrupt_eoi_ep93xx_gpio_f)


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
CALLOUT_START(interrupt_mask_ep93xx_gpio_f, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0, #0	// error

	/* normalize the vector to the appropriate gpio port */
	cmp		r1, #8
	bge		0f		/* not the correct id */

	mov		r0, #1
	mov		r0, r0, lsl r1
	
	/* disable (mask) the interrupt */
	ldr		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
	bic		r1, r1, r0
	str		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
0:

CALLOUT_END(interrupt_mask_ep93xx_gpio_f)


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
CALLOUT_START(interrupt_unmask_ep93xx_gpio_f, 0, interrupt_patch)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r0, #0	// error

	/* normalize the vector to the appropriate gpio port */
	cmp		r1, #8
	bge		0f		/* not the correct id */

	mov		r0, #1
	mov		r0, r0, lsl r1
	
	/* enable (unmask) the interrupt */
	ldr		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
	orr		r1, r1, r0
	str		r1, [ip, #EP93xx_GPIO_PORTF_INT_ENABLE]
0:

CALLOUT_END(interrupt_unmask_ep93xx_gpio_f)

/*
 * -----------------------------------------------------------------------
 * interrupt config
 *
 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - error status
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_config_ep93xx_gpio_f, 0, 0)

	mov		r0, #0
	mov		pc, lr

CALLOUT_END(interrupt_config_ep93xx_gpio_f)
