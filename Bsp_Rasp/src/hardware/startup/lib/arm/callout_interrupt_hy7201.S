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
 * HY7201 specific interrupt callouts.
 *
 * This should be usable by any board that uses an HY7201.
 * The board specific startup must set the following global variables:
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
 */

#include "callout.ah"

/*
 * ------------------------------------------------------------------------
 * Primary interrupt controller
 * ------------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_hy7201, 0, patch_intc)
	mov		r0,     #0x000000ff		// INTC base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [r0, #HY7201_INTC_STATUS]
	mov		r4, #24							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #HY7201_INTC_ENABLE]
	bic		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_INTC_ENABLE]
1:
CALLOUT_END(interrupt_id_hy7201)


CALLOUT_START(interrupt_eoi_hy7201, 0, patch_intc)
	mov		r0,     #0x000000ff		// INTC base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r1, [r0, #HY7201_INTC_ENABLE]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_INTC_ENABLE]
0:
CALLOUT_END(interrupt_eoi_hy7201)


/*
 * error = interrupt_mask_hy7201(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_hy7201, 0, patch_intc)

	mov		r0,     #0x000000ff		// INTC base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_INTC_ENABLE]
	mov		r2, #1
	bic		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_INTC_ENABLE]		

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_hy7201)


/*
 * error = interrupt_unmask_hy7201(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_hy7201, 0, patch_intc)
	mov		r0,     #0x000000ff		// INTC base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_INTC_ENABLE]
	mov		r2, #1
	orr		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_INTC_ENABLE]		

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_hy7201)

/*
 * Patch callouts that require only HY7201_INTC_BASE
 *
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 */
patch_intc:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	ldr		r1, Lintc_base
	mov		r0, #0x14					// size of interrupt registers
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

Lintc_base:	.word	HY7201_INTC_BASE

/*
 * ------------------------------------------------------------------------
 * GPIO A interrupts
 * NOTE: we assume the board-specific startup program has set PAIE/PAIP.
 * ------------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_hy7201_gpioa, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [r0, #HY7201_GPIO_PAIS]
	mov		r4, #8							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #HY7201_GPIO_PAIM]
	mov		r2, r2, lsl r4
	bic		r1, r1, r2
	str		r1, [r0, #HY7201_GPIO_PAIM]

	/*
	 * Clear interrupt if edge mode
	 */
	ldr		r1, [r0, #HY7201_GPIO_PAIE]
	tst		r1, r2
	strne	r2, [r0, #HY7201_GPIO_PAIC]
1:
CALLOUT_END(interrupt_id_hy7201_gpioa)


CALLOUT_START(interrupt_eoi_hy7201_gpioa, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r1, [r0, #HY7201_GPIO_PAIM]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_GPIO_PAIM]
0:
CALLOUT_END(interrupt_eoi_hy7201_gpioa)


/*
 * error = interrupt_mask_hy7201_gpioa(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_hy7201_gpioa, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PAIM]
	mov		r2, #1
	bic		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PAIM]		

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_hy7201_gpioa)


/*
 * error = interrupt_unmask_hy7201_gpioa(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_hy7201_gpioa, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PAIM]
	mov		r2, #1
	orr		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PAIM]		

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_hy7201_gpioa)

/*
 * ------------------------------------------------------------------------
 * GPIO B interrupts
 * NOTE: we assume the board-specific startup program has set PBIE/PBIP.
 * ------------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_hy7201_gpiob, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [r0, #HY7201_GPIO_PBIS]
	mov		r4, #8							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #HY7201_GPIO_PBIM]
	mov		r2, r2, lsl r4
	bic		r1, r1, r2
	str		r1, [r0, #HY7201_GPIO_PBIM]

	/*
	 * Clear interrupt if edge mode
	 */
	ldr		r1, [r0, #HY7201_GPIO_PBIE]
	tst		r1, r2
	strne	r2, [r0, #HY7201_GPIO_PBIC]
1:
CALLOUT_END(interrupt_id_hy7201_gpiob)


CALLOUT_START(interrupt_eoi_hy7201_gpiob, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r1, [r0, #HY7201_GPIO_PBIM]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_GPIO_PBIM]
0:
CALLOUT_END(interrupt_eoi_hy7201_gpiob)


/*
 * error = interrupt_mask_hy7201_gpiob(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_hy7201_gpiob, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PBIM]
	mov		r2, #1
	bic		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PBIM]		

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_hy7201_gpiob)


/*
 * error = interrupt_unmask_hy7201_gpiob(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_hy7201_gpiob, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PBIM]
	mov		r2, #1
	orr		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PBIM]		

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_hy7201_gpiob)

/*
 * ------------------------------------------------------------------------
 * GPIO C interrupts
 * NOTE: we assume the board-specific startup program has set PCIE/PCIP.
 * ------------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_hy7201_gpioc, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [r0, #HY7201_GPIO_PCIS]
	mov		r4, #8							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #HY7201_GPIO_PCIM]
	mov		r2, r2, lsl r4
	bic		r1, r1, r2
	str		r1, [r0, #HY7201_GPIO_PCIM]

	/*
	 * Clear interrupt if edge mode
	 */
	ldr		r1, [r0, #HY7201_GPIO_PCIE]
	tst		r1, r2
	strne	r2, [r0, #HY7201_GPIO_PCIC]
1:
CALLOUT_END(interrupt_id_hy7201_gpioc)


CALLOUT_START(interrupt_eoi_hy7201_gpioc, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r1, [r0, #HY7201_GPIO_PCIM]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_GPIO_PCIM]
0:
CALLOUT_END(interrupt_eoi_hy7201_gpioc)


/*
 * error = interrupt_mask_hy7201_gpioc(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_hy7201_gpioc, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PCIM]
	mov		r2, #1
	bic		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PCIM]		

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_hy7201_gpioc)


/*
 * error = interrupt_unmask_hy7201_gpioc(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_hy7201_gpioc, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PCIM]
	mov		r2, #1
	orr		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PCIM]		

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_hy7201_gpioc)

/*
 * ------------------------------------------------------------------------
 * GPIO C interrupts
 * NOTE: we assume the board-specific startup program has set PDIE/PDIP.
 * ------------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_hy7201_gpiod, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [r0, #HY7201_GPIO_PDIS]
	mov		r4, #8							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1
	blt		1f
	tst		r1, r2, lsl r4
	beq		0b

	/*
	 * Mask the interrupt source
	 */
	ldr		r1, [r0, #HY7201_GPIO_PDIM]
	mov		r2, r2, lsl r4
	bic		r1, r1, r2
	str		r1, [r0, #HY7201_GPIO_PDIM]

	/*
	 * Clear interrupt if edge mode
	 */
	ldr		r1, [r0, #HY7201_GPIO_PDIE]
	tst		r1, r2
	strne	r2, [r0, #HY7201_GPIO_PDIC]
1:
CALLOUT_END(interrupt_id_hy7201_gpiod)


CALLOUT_START(interrupt_eoi_hy7201_gpiod, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	/*
	 * Only unmask interrupt if mask count is zero
	 */
	teq		r7, #0
	bne		0f

	ldr		r1, [r0, #HY7201_GPIO_PDIM]
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [r0, #HY7201_GPIO_PDIM]
0:
CALLOUT_END(interrupt_eoi_hy7201_gpiod)


/*
 * error = interrupt_mask_hy7201_gpiod(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_mask_hy7201_gpiod, 0, patch_gpio)

	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PDIM]
	mov		r2, #1
	bic		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PDIM]		

0:	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_hy7201_gpiod)


/*
 * error = interrupt_unmask_hy7201_gpiod(syspage_ptr, vector)
 */
CALLOUT_START(interrupt_unmask_hy7201_gpiod, 0, patch_gpio)
	mov		r0,     #0x000000ff		// GPIO base (patched)
	orr		r0, r0, #0x0000ff00
	orr		r0, r0, #0x00ff0000
	orr		r0, r0, #0xff000000

	ldr		ip, [r0, #HY7201_GPIO_PDIM]
	mov		r2, #1
	orr		ip, ip, r2, lsl r1
	str		ip, [r0, #HY7201_GPIO_PDIM]		

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_hy7201_gpiod)

/*
 * Patch callouts that require only GPIO registers
 *
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 */
patch_gpio:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	ldr		r1, Lgpio_base
	mov		r0, #0x80					// size of gpio registers
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

Lgpio_base:	.word	HY7201_GPIO_BASE
