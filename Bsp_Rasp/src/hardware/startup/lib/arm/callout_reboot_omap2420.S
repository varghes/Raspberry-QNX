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
 * TI OMAP2420 system reboot
 */

#include "callout.ah"
#include <arm/omap2420.h>

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * On entry:
 *	r0 - physical address of syspage
 *	r1 - virtual  address of syspage
 *	r2 - offset from start of syspage to start of the callout routine
 *	r3 - offset from start of syspage to read/write data used by callout
 * -----------------------------------------------------------------------
 */

patch_reboot:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2					/* address of callout routine */

	/*
	 * Map reset control registers
	 */
	mov		r0, #OMAP2420_PRCM_SIZE		/* size of reset control registers */
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	OMAP2420_PRCM_BASE

#define ARM_CPSR_T 		(1 << 5)
#define ARM_CPSR_F 		(1 << 6)
#define ARM_CPSR_I 		(1 << 7)

CALLOUT_START(reboot_omap2420, 0, patch_reboot)
    /*
     * Get the base address of Clock, Reset, and Power Mode Control registers (patched)
     */
    mov     ip,     #0x000000FF
    orr     ip, ip, #0x0000FF00
    orr     ip, ip, #0x00FF0000
    orr     ip, ip, #0xFF000000

	/*
	 * Disable ARM interrupt
	 */
	mrs		r4, cpsr
	orr		r4, r4, #ARM_CPSR_I | ARM_CPSR_F 
	msr		cpsr, r4

	/*
	 * Set RST_GS bit
	 */
	mov		r1, #2
	str		r1, [ip, #OMAP2420_RM_RSTCTRL_WKUP]

0:
	b		0b

CALLOUT_END(reboot_omap2420)
