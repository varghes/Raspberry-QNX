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
// bcm2835 datasheet - no proper interrupt information
// refer alternate  source codes
//IRQC_IRQPEND1_OFF in this register USB is the usefull interrrupt ,
//I have no plans to add that ..
 

#include "callout.ah"
#include <arm/bcm2835.h>


//
/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code for IRC
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */
interrupt_patch_aic:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					// address of callout routine

	/*
	 * Map interrupt controller registers
	 */
	mov		r0, #ARM_AIC_SIZE	// size of AIC registers
	ldr		r1, Lintr_base
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}
  Lintr_base:	.word	ARM_IRQCTLR_BASEOFF

/*
 * -----------------------------------------------------------------------
 * Identify interrupt source.
 *
 * Returns interrupt number in r4
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_id_bcm2835_aic, 0, interrupt_patch_aic)
    /*
     * Get the IRC interrupt controller base address (patched)
     */
    mov		ip,     #0x000000ff
    orr		ip, ip, #0x0000ff00
    orr		ip, ip, #0x00ff0000
    orr		ip, ip, #0xff000000

/*
	 * Get interrupt status and scan for first set bit
	 */
	ldr		r1, [ip, #IRQC_IRQPEND2_OFF]
	mov		r4, #32							// highest interrupt bit
	mov		r2, #1
0:	subs	r4, r4, #1      // r4=r4-1
	blt		1f              //jump to label1
	tst		r1, r2, lsl r4  // r1 ,r2<<r4
	beq		0b              // go back
	/*
	 * Mask the interrupt source  -> disable interrupt
	 */

	ldr		r1, [ip, #IRQC_DISABIRQ2_OFF]
	orr		r1, r1, r2, lsl r4
	str		r1, [ip, #IRQC_DISABIRQ2_OFF]
	add 	r4, r4, #32   //actual source
    b 2f

1:

	ldr		r1, [ip, #IRQC_IRQPEND1_OFF]
	mov		r4, #32							// highest interrupt bit
	mov		r2, #1
3:	subs	r4, r4, #1      // r4=r4-1
	blt		2f              //jump to label1
	tst		r1, r2, lsl r4  // r1 ,r2<<r4
	beq		3b              // go back
	/*
	 * Mask the interrupt source  -> disable interrupt
	 */

	ldr		r1, [ip, #IRQC_DISABIRQ1_OFF]
	orr		r1, r1, r2, lsl r4
	str		r1, [ip, #IRQC_DISABIRQ1_OFF]

2:

/*	mov		r2, #BCM2835_IRQR2_UART0  testing
	str		r2, [ip, #IRQC_DISABIRQ2_OFF]
	mov		r4, #25

	mov		r2, #BCM2835_IRQR1_TIMER3
	str		r2, [ip, #IRQC_DISABIRQ1_OFF]
	mov		r4, #3 */

    CALLOUT_END(interrupt_id_bcm2835_aic)


/*
 * -----------------------------------------------------------------------
 * Acknowledge specified interrupt
 *
 * On entry:
 *	r4 contains the interrupt number
 *	r7 contains the interrupt mask count
 * -----------------------------------------------------------------------
 */
CALLOUT_START(interrupt_eoi_bcm2835_aic, 0, interrupt_patch_aic)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    	/*
	 * skip if mask count is zer0
	 */
	teq		r7, #0
	bne		0f

    CMP   r4, #31
    bgt  1f

	ldr		r1, [ip, #IRQC_ENABIRQ1_OFF]  // unmask or enable
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [ip, #IRQC_ENABIRQ1_OFF]
	b 0f

1:
    sub 	r4, r4, #32
	ldr		r1, [ip, #IRQC_ENABIRQ2_OFF]  // unmask or enable
	mov		r2, #1
	orr		r1, r1, r2, lsl r4
	str		r1, [ip, #IRQC_ENABIRQ2_OFF]
0:

/*mov		r2, #BCM2835_IRQR1_TIMER3  //testing
  str		r2, [ip, #IRQC_ENABIRQ1_OFF]
  mov		r2, #BCM2835_IRQR2_UART0
  str		r2, [ip, #IRQC_ENABIRQ2_OFF]*/

CALLOUT_END(interrupt_eoi_bcm2835_aic)


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
CALLOUT_START(interrupt_mask_bcm2835_aic, 0, interrupt_patch_aic)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    /*
     * Mask the interrupt -disable
     */
    CMP   r1, #31
    bgt  1f

	ldr		r3, [ip, #IRQC_DISABIRQ1_OFF]  // unmask or enable
	mov		r2, #1
	orr		r3, r3, r2, lsl r1
	str		r3, [ip, #IRQC_DISABIRQ1_OFF]
	b 0f

1:
    sub 	r1, r1, #32
	ldr		r3, [ip, #IRQC_DISABIRQ2_OFF]  // unmask or enable
	mov		r2, #1
	orr		r3, r3, r2, lsl r1
	str		r3, [ip, #IRQC_DISABIRQ2_OFF]
0:

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_mask_bcm2835_aic)


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
CALLOUT_START(interrupt_unmask_bcm2835_aic, 0, interrupt_patch_aic)
	/*
	 * Get the interrupt controller base address (patched)
	 */
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

    CMP   r1, #31
    bgt  1f

	ldr		r3, [ip, #IRQC_ENABIRQ1_OFF]  // unmask or enable
	mov		r2, #1
	orr		r3, r3, r2, lsl r1
	str		r3, [ip, #IRQC_ENABIRQ1_OFF]
	b 0f

1:
    sub 	r1, r1, #32
	ldr		r3, [ip, #IRQC_ENABIRQ2_OFF]  // unmask or enable
	mov		r2, #1
	orr		r3, r3, r2, lsl r1
	str		r3, [ip, #IRQC_ENABIRQ2_OFF]
0:

	mov		r0, #0
	mov		pc, lr
CALLOUT_END(interrupt_unmask_bcm2835_aic)

