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
 * IXP1200 specific interrupt callouts.
 *
 * This should be usable by any board that uses a IXP1200.
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
 */

#include "callout.ah"

/*
 * Data in RW storage
 */
rw_intr:	.word	16
#define	OFF_IRQ			0	// IRQ register address
#define	OFF_PCI_CFG		4	// PCI_CFG base address
#define	OFF_UART		8	// UART base address
#define	OFF_FBI			12	// FBU base address

Lirq:	.word	IXP1200_IRQ_BASE
Lpci:	.word	IXP1200_PCI_CFG_BASE
Luart:	.word	IXP1200_UART_BASE
Lfbi:	.word	IXP1200_FBI_BASE

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
	 * Map IXP1200_IRQ_BASE and store in RW data
	 */
	mov		r0, #IXP1200_IRQ_SIZE
	ldr		r1, Lirq
	bl		callout_io_map
	str		r0, [r4, #OFF_IRQ]

	/*
	 * Map IXP1200_PCI_CFG_BASE and store in RW data
	 */
	mov		r0, #IXP1200_PCI_CFG_SIZE
	ldr		r1, Lpci
	bl		callout_io_map
	str		r0, [r4, #OFF_PCI_CFG]

	/*
	 * Map IXP1200_UART_BASE and store in RW data
	 */
	mov		r0, #IXP1200_UART_SIZE
	ldr		r1, Luart
	bl		callout_io_map
	str		r0, [r4, #OFF_UART]

	/*
	 * Map IXP1200_FBI_BASE and store in RW data
	 */
	mov		r0, #IXP1200_FBI_SIZE
	ldr		r1, Luart
	bl		callout_io_map
	str		r0, [r4, #OFF_FBI]

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
 * IXP1200 IRQ register callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_ixp1200, rw_intr, patch_id)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_IRQ]

	/*
	 * Read pending IRQ interrupts and scan for first set bit
	 */
	mov		r4, #9					// 9-31 are reserved
	ldr		r1, [r0]
	mov		r2, #1
0:	sub		r4, r4, #1
	teq		r4, #1					// interrupts 0-1 are reserved
	subeq	r4, r4, #2
	beq		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * PCI interrupt is cascaded - we can mask in the PCI IRQ_ENABLE register
	 */
	teq		r4, #2
	beq		1f

	/*
	 * CINT and Microengine interrupts must be masked in the FBI IREG register
	 * Note that bits 0-26 are write 1 to clear, so we mask these out.
	 */
	teq		r4, #3
	teqne	r4, #4
	bne		0f
	ldr		r0, [ip, #OFF_FBI]
	ldr		r1, [r0, #IXP1200_IREG]
	teq		r4, #3
	biceq	r1, r1, #IXP1200_IREG_CINTIRQ
	bicne	r1, r1, #IXP1200_IREG_UEIRQ
	bic		r1, r1, #0x07000000
	bic		r1, r1, #0x00ff0000
	bic		r1, r1, #0x0000ff00
	bic		r1, r1, #0x000000ff
	str		r1, [r0, #IXP1200_IREG]
	b		1f

#ifdef	FIXME
	/*
	 * Check and mask SRAM, RTC and SDRAM interrupts
	 */
#endif

	/*
	 * UART interrupt has to be masked in the UART_CR register
	 * - InterruptUnmask() called by the driver will re-enable RIE
	 * - driver must set XIE if appropriate
	 */
0:	teq		r4, #8
	bne		0f
	ldr		r0, [ip, #OFF_UART]
	ldr		r1, [r0, #IXP1200_UART_CR]
	bic		r1, r1, #IXP1200_UART_CR_RIE | IXP1200_UART_CR_XIE
	str		r1, [r0, #IXP1200_UART_CR]
	/*
	 * FALL_THRU
	 */

1:
CALLOUT_END(interrupt_id_ixp1200)


CALLOUT_START(interrupt_eoi_ixp1200, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		1f

	/*
	 * PCI interrupt is cascaded
	 */
	teq		r4, #2
	beq		1f

	/*
	 * CINT and Microengine interrupts must be enabled in the FBI IREG register
	 * Note that bits 0-26 are write 1 to clear, so we mask these out.
	 */
	teq		r4, #3
	teqne	r4, #4
	bne		0f
	ldr		r0, [ip, #OFF_FBI]
	ldr		r1, [r0, #IXP1200_IREG]
	teq		r4, #3
	orreq	r1, r1, #IXP1200_IREG_CINTIRQ
	orrne	r1, r1, #IXP1200_IREG_UEIRQ
	bic		r1, r1, #0x07000000
	bic		r1, r1, #0x00ff0000
	bic		r1, r1, #0x0000ff00
	bic		r1, r1, #0x000000ff
	str		r1, [r0, #IXP1200_IREG]
	b		1f

#ifdef	FIXME
	/*
	 * Check and mask SRAM, RTC and SDRAM interrupts
	 */
#endif

	/*
	 * UART interrupt must be enabled in the UART_CR
	 * - set RIE to unmask Rx interrupt
	 * - driver must set XIE if appropriate
	 */
0:	teq		r4, #8					// UART interrupt?
	bne		0f
	ldr		r0, [ip, #OFF_UART]
	ldr		r1, [r0, #IXP1200_UART_CR]
	orr		r1, r1, #IXP1200_UART_CR_RIE
	str		r1, [r0, #IXP1200_UART_CR]
	/*
	 * FALL_THRU
	 */
1:
CALLOUT_END(interrupt_eoi_ixp1200)

/*
 * error = interrupt_mask_ixp1200(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_ixp1200, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	/*
	 * CINT and Microengine interrupts must be masked in FBI IREG register
	 * Note that bits 0-26 are write 1 to clear, so we mask these out.
	 */
	teq		r1, #3
	teqne	r1, #4
	bne		0f
	ldr		r0, [ip, #OFF_FBI]
	ldr		r1, [r0, #IXP1200_IREG]
	teq		r4, #3
	biceq	r1, r1, #IXP1200_IREG_CINTIRQ
	bicne	r1, r1, #IXP1200_IREG_UEIRQ
	bic		r1, r1, #0x07000000
	bic		r1, r1, #0x00ff0000
	bic		r1, r1, #0x0000ff00
	bic		r1, r1, #0x000000ff
	str		r1, [r0, #IXP1200_IREG]
	b		1f

#ifdef	FIXME
	/*
	 * Check and mask SRAM, RTC and SDRAM interrupts
	 */
#endif

	/*
	 * UART interrupt must be masked in the UART_CR
	 * - InterruptUnmask() called by the driver will re-enable RIE
	 * - driver must set XIE if appropriate
	 */
0:	teq		r1, #8					// UART interrupt?
	bne		1f
	ldr		r0, [ip, #OFF_UART]
	ldr		r1, [r0, #IXP1200_UART_CR]
	bic		r1, r1, #IXP1200_UART_CR_RIE | IXP1200_UART_CR_XIE
	str		r1, [r0, #IXP1200_UART_CR]
	/*
	 * FALL_THRU
	 */

1:
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_ixp1200)


/*
 * error = interrupt_unmask_ixp1200(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_ixp1200, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	/*
	 * CINT and Microengine interrupts must be masked in FBI IREG register
	 * Note that bits 0-26 are write 1 to clear, so we mask these out.
	 */
	teq		r1, #3
	teqne	r1, #4
	bne		0f
	ldr		r0, [ip, #OFF_FBI]
	ldr		r1, [r0, #IXP1200_IREG]
	teq		r4, #3
	orreq	r1, r1, #IXP1200_IREG_CINTIRQ
	orrne	r1, r1, #IXP1200_IREG_UEIRQ
	bic		r1, r1, #0x07000000
	bic		r1, r1, #0x00ff0000
	bic		r1, r1, #0x0000ff00
	bic		r1, r1, #0x000000ff
	str		r1, [r0, #IXP1200_IREG]
	b		1f

#ifdef	FIXME
	/*
	 * Check and mask SRAM, RTC and SDRAM interrupts
	 */
#endif

	/*
	 * UART interrupt must be unmasked in the UART_CR
	 * - set RIE to unmask Rx interrupt
	 * - driver must set XIE if appropriate
	 */
0:	teq		r1, #8					// UART interrupt?
	bne		1f

	/*
	 * Set RIE to unmask the interrupt:
	 * - driver will set XIE if appropriate on first transmit
	 */
	ldr		r0, [ip, #OFF_UART]
	ldr		r1, [r0, #IXP1200_UART_CR]
	orr		r1, r1, #IXP1200_UART_CR_RIE
	str		r1, [r0, #IXP1200_UART_CR]
	/*
	 * FALL_THRU
	 */

1:
	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_ixp1200)

/*
 * -----------------------------------------------------------------------
 * IXP1200 PCI unit interrupt callouts
 * -----------------------------------------------------------------------
 */

CALLOUT_START(interrupt_id_ixp1200_pci, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	ldr		r0, [ip, #OFF_PCI_CFG]

	mov		r4, #32
	ldr		r1, [r0, #IXP1200_IRQ_STATUS]
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	mov		r2, r2, lsl r4
	str		r2, [r0, #IXP1200_IRQ_ENABLE_CLEAR]		// clear interrupt enable
1:	
CALLOUT_END(interrupt_id_ixp1200_pci)


CALLOUT_START(interrupt_eoi_ixp1200_pci, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r5, ip				// INTR_GENFLAG_LOAD_SYSPAGE specified

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r0, [ip, #OFF_PCI_CFG]

	/*
	 * Convert vector to bitmask and enable interrupt
	 */
	mov		r2, #1
	mov		r2, r2, lsl r4
	str		r2, [r0, #IXP1200_IRQ_ENABLE_SET]
0:
CALLOUT_END(interrupt_eoi_ixp1200_pci)

/*
 * error = interrupt_mask_ixp1200_pci(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_ixp1200_pci, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_PCI_CFG]

	/*
	 * Convert vector to bitmask and clear bit in IRQ_ENABLE
	 */
	mov		r2, #1
	mov		r2, r2, lsl r1
	str		r2, [r0, #IXP1200_IRQ_ENABLE_CLEAR]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_ixp1200_pci)


/*
 * error = interrupt_unmask_ixp1200_pci(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_ixp1200_pci, rw_intr, patch_other)
	mov		ip,     #0x000000ff		// RW data offset (patched)
	orr		ip, ip, #0x0000ff00
	add		ip, r0, ip

	ldr		r0, [ip, #OFF_PCI_CFG]

	/*
	 * Convert vector to bitmask and set bit in IRQ_ENABLE
	 */
	mov		r2, #1
	mov		r2, r2, lsl r1
	str		r2, [r0, #IXP1200_IRQ_ENABLE_SET]

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_ixp1200_pci)
