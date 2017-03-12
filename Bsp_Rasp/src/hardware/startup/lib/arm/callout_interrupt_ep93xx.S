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
 * EP93xx specific interrupt callouts
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

.extern	non_vectored_int_mask

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
/*
* patcher1 patches the base address of the VIC's into the 'ip' register and also
* loads VIC1 and VIC2 default vect addr registers with the offset of their
* corresponding non-vectored interrupt masks (at the r/w offset). It will also
* initialize the non-vectored interrupt masks from the patch data (which is
* passed as 'uint32_t patch_data[2]')
*/
patcher1:
	stmdb	sp!,{r4,lr}

	ldr		r1, Lpaddr

	/* set the offset of the non-vectored int masks into the default vec addr registers */
	str		r3, [r1, #EP93xx_VIC_VEC_ADDR_DFLT]
	add		r1, #0x10000
	add		r3, #4
	str		r3, [r1, #EP93xx_VIC_VEC_ADDR_DFLT]

	/* initialize the VIC1/2 non vectored interrupt masks */
	ldr		r4, [sp, #8]	// r4 = patch_data
	add		r3, r0			// need r/w address (not offset)
	/* VIC1 non-vectored interrupt mask */
	ldr		r1, [r4]
	str		r1, [r3, #-4]
	/* VIC2 non-vectored interrupt mask */
	ldr		r1, [r4, #4]
	str		r1, [r3]

	add		r4, r0, r2					// address of callout routine
	/* Map interrupt controller registers */
	mov		r0, #EP93xx_INTR_SIZE
	ldr		r1, Lpaddr
	mov		r2, #0xB00				// PROT_READ|PROT_WRITE|PROT_NOCACHE
	bl		callout_memory_map

	/* Patch the callout routines with the mapped address (in r0) */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	EP93xx_INTR_BASE

/*
* patcher2 patches the base address of the VIC's into the 'ip' register
*/
patcher2:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/* Map interrupt controller registers */
	mov		r0, #EP93xx_INTR_SIZE
	ldr		r1, Lpaddr
	mov		r2, #0xB00				// PROT_READ|PROT_WRITE|PROT_NOCACHE
	bl		callout_memory_map

	/* Patch the callout routines with the mapped address (in r0) */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 *
 * The interrupts are handled as follows
 * read vector from VIC1
 * if (vector != default vector)
 *     return vector
 * read vector from VIC2
 * if (vector != default vector)
 *     return vector
 * read VIC1 status register and mask out vectored interrupts
 * if (status != 0)
 *     return first bit set starting at bit 31
 * read VIC2 status register and mask out vectored interrupts
 * if (status != 0)
 *     return first bit set starting at bit 31
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_ep93xx, non_vectored_int_mask, patcher1)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	mov		r3, #0
	mov		r1, #1

	/* Read VIC1 vector address register */
	ldr		r4, [ip, #EP93xx_VIC_VEC_ADDR_CURR]
	cmp		r4, #31
	bgt		5f		// not a vectored VIC1 interrupt, check VIC2
	/*
	 * CHIP BUG ?? - additional check required for valid VIC1 interrupt
	 * I have noticed that VIC1 can contain the VIC2 vector in its VectAddr
	 * register. This was confirmed by the fact that the Vic1VectAddr value is the
	 * same as an expected Vic2VectAddr and the vector id corresponds to a VIC2
	 * interrupt and corresponding bit that is not even set in the VIC 1 status
	 * or raw status registers.
	 * I call this a bug because the documentation does not describe this behaviour
	 * and it seems illogical for the Vic1VectAddr register to contain a vector value
	 * for an interrupt not even set in the VIC 1 status or raw status registers.
	 * The workaround is to check the VIC1 status for the bit corresponding to
	 * the Vic1VectAddr and ignore the VicVectAddr value if it is not set
	 * Note that not doing this extra check will cause the kernel to crash on
	 * any VIC2 vectored interrupt with what appears to be too high a nesting
	 * depth (ie. We return thinking this is a VIC1 interrupt but the handler
	 * determines there is no interrupt for the VIC1 device because there really
	 * isn't and because we don't disable the interrupt - don't need to because
	 * the hardware prioritization logic is supposed to prevent same ot lower
	 * interrupts - we nest on the same VIC1 - but really VIC2 - interrupt)
	*/
	ldr		r2, [ip, #EP93xx_VIC_IRQ_STATUS]
	tst		r2, r1, lsl r4
	bne		1f	// a legitimate VIC1 vectored interrupt. Don't need to mask so jump to 1
5:
	/*
	 * Not a VIC 1 vectored interrupt so have to write the vector addr register now
	 * because we won't do it at eoi time. If we don't write this register we
	 * don't see any non vectored interrupts. I believe this is because regardless
	 * of whether a vectored interrupt is the source or not, the fact that the
	 * EP93xx_VIC_VEC_ADDR_CURR is read, requires it to be written in order for
	 * the prioritization hardware to "re-evaluate" the next highest priority
	 * interrupt
	*/
	str		r4, [ip, #EP93xx_VIC_VEC_ADDR_CURR]

	/* Read VIC2 vector address register */
	add		ip, #0x10000
	mov		r3, #32
	ldr		r4, [ip, #EP93xx_VIC_VEC_ADDR_CURR]
	cmp		r4, #31
	ble		1f		// don't need to mask a vectored interrupt, so jump to 1
	/*
	 * Not a VIC 2 vectored interrupt so have to write the vector addr register now
	 * because we won't do it at eoi time. If we don't write this register we
	 * don't see any non vectored interrupts. I believe this is because regardless
	 * of whether a vectored interrupt is the source or not, the fact that the
	 * EP93xx_VIC_VEC_ADDR_CURR is read, requires it to be written in order for
	 * the prioritization hardware to "re-evaluate" the next highest priority
	 * interrupt
	*/
	str		r4, [ip, #EP93xx_VIC_VEC_ADDR_CURR]

	/* Read pending IRQ interrupts from VIC1 (patched with EP93xx_INTR_BASE) */
	sub		ip, #0x10000
	mov		r3, #0
	ldr		r2, [ip, #EP93xx_VIC_IRQ_STATUS]
	ldr		r4, [ip, #EP93xx_VIC_VEC_ADDR_DFLT]
	ldr		r4, [r4, r5]	// get the mask of non-vectored interrupts
	and		r2, r4
	cmp		r2, #0
	bne		id_scan

	/* Read pending IRQ interrupts from VIC2 (patched with EP93xx_INTR_BASE) */
	add		ip, #0x10000
	mov		r3, #32
	ldr		r2, [ip, #EP93xx_VIC_IRQ_STATUS]
	ldr		r4, [ip, #EP93xx_VIC_VEC_ADDR_DFLT]
	ldr		r4, [r4, r5]	// get the mask of non-vectored interrupts
	and		r2, r4
	mov		r4, #-1		/* spurious if VIC2 status is also 0 */
	cmp		r2, #0
	beq		0f

id_scan:
	/* Scan for first set bit (starting with MSb) */
	mov		r4, #32
	mov		r1, #1
3:
	subs	r4, r4, #1
	blt		0f			/* r4 has rolled to -1 */
	tst		r2, r1, lsl r4
	beq		3b

2:
	/* clear (and disable/mask) the interrupt */
	mov		r1, r1, lsl r4
	str		r1, [ip, #EP93xx_VIC_INT_CLEAR]
1:
	add		r4, r4, r3
0:

CALLOUT_END(interrupt_id_ep93xx)

/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_ep93xx, 0, patcher2)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* normalize the vector to the appropriate controller */
	mov		r1, r4		// don't modify r4
	cmp		r1, #32
	blt		2f
	sub		r1, #32
	add		ip, #0x10000
2:
	/* if this is a vectored interrupt, write to the vector address register */
	ldr		r2, [ip, #EP93xx_VIC_VEC_ADDR_DFLT]
	ldr		r2, [r2, r5]	// get the mask of non-vectored interrupts
	mov		r3, #1
	tst		r2, r3, lsl r1
	bne		1f
	str		r1, [ip, #EP93xx_VIC_VEC_ADDR_CURR]
	/* fall through */

1:
	/* Only unmask interrupt if mask count is zero */
	teq		r7, #0
	bne		0f

	/* re-enable (unmask) the interrupt */
	ldr		r2, [ip, #EP93xx_VIC_INT_ENABLE]
	orr		r2, r3, lsl r1
	str		r2, [ip, #EP93xx_VIC_INT_ENABLE]
0:
CALLOUT_END(interrupt_eoi_ep93xx)


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
CALLOUT_START(interrupt_mask_ep93xx, 0, patcher2)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* normalize the vector to the appropriate controller */
	cmp		r1, #32
	blt		1f
	sub		r1, #32
	add		ip, #0x10000

1:
	/* Clear enable for bit (1 << interrupt_number) */
	mov		r2, #1
	mov		r2, r2, lsl r1
	str		r2, [ip, #EP93xx_VIC_INT_CLEAR]
	mov		r0, #0
	mov		pc, lr

CALLOUT_END(interrupt_mask_ep93xx)


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
CALLOUT_START(interrupt_unmask_ep93xx, 0, patcher2)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* normalize the vector to the appropriate controller */
	cmp		r1, #32
	blt		1f
	sub		r1, #32
	add		ip, #0x10000

1:
	/* Set enable for bit (1 << interrupt_number) */
	mov		r2, #1
	mov		r2, r2, lsl r1
	ldr		r1, [ip, #EP93xx_VIC_INT_ENABLE]
	orr		r1, r1, r2
	str		r1, [ip, #EP93xx_VIC_INT_ENABLE]
	mov		r0, #0
	mov		pc, lr

CALLOUT_END(interrupt_unmask_ep93xx)

/*
 * -----------------------------------------------------------------------
 * interrupt config
 *
 * We don't want anyone to be able to attach to a cascade vector. Normally
 * the config table would give us this table but hack it in for now since we
 * aren't using the vector/config tables for this device yet.
 * Vectors not attachable are for the GPIO's 59, 20, 21, 22

 * On entry:
 *	r0 - syspage_ptr
 *	r1 - interrupt number
 *
 * Returns:
 *	r0 - config flag value
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_config_ep93xx, 0, 0)

	mov		r0, #INTR_CONFIG_FLAG_DISALLOWED
	orr		r0, #INTR_CONFIG_FLAG_PREATTACH

	cmp		r1, #59
	beq		0f
	cmp		r1, #20
	beq		0f
	cmp		r1, #21
	beq		0f
	cmp		r1, #22
	beq		0f

	/* no flags to return */
	mov		r0, #0
0:
	mov		pc, lr

CALLOUT_END(interrupt_config_ep93xx)

