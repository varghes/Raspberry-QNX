#
# Copyright 2011, QNX Software Systems. 
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
#
# $QNXLicenseA$
#

#include "callout.ah"
#include <arm/gic.h>

/*
 * Routine to patch callout code
 *
 * On entry:
 *	r0     - physical address of syspage
 *	r1     - virtual  address of syspage
 *	r2     - offset from start of syspage to start of the callout routine
 *	r3     - offset from start of syspage to read/write data used by callout
 */
patch_sendipi:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2				// address of callout routine
	
	ldr		r1, =gic_dist_base
	mov		r0, #ARM_GICD_SIZE
	ldr		r1, [r1]
	bl		callout_io_map
	CALLOUT_PATCH	r4, r0, r1, r2, ip

	ldmia	sp!,{r4,pc}

/*
 * On entry:
 * r0 - _syspage_ptr
 * r1 - cpu target
 * r2 - cmd
 * r3 - &ipicmds[cpu]
 */
CALLOUT_START(sendipi_gic, 0, patch_sendipi)
	mov		ip,     #0x000000ff		// SCU GIC_DIST_BASE (patched)
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	stmdb	sp!,{r4,r5,lr}

	mrs  	r5,cpsr
	cpsid	i

	/*
	 * Set ipicmd[cpu]
	 */
0:	ldrex	r0, [r3]
	orr		r4, r0, r2
	strex	lr, r4, [r3]
	teq		lr, #1
	beq		0b						// strex failed - retry
	mcr		p15, 0, lr, c7, c10, 4	// dsb FIXME: deprecated in ARMv7	

	/*
	 * Trigger ID0 to target CPU if no commands were already pending
	 */
	teq		r0, #0
	moveq	r0, #0x10000
	moveq	r0, r0, lsl r1			// cpu target = ((1 << cpu) << 16)
	streq	r0, [ip, #ARM_GICD_SGIR]

	msr  	cpsr,r5

	ldmia	sp!,{r4,r5,pc}	
CALLOUT_END(sendipi_gic)
