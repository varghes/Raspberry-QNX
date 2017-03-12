/*
 * $QNXLicenseC: 
 * Copyright 2007,2008,  QNX Software Systems.  
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
 * Freescale i.MX31ADS system reboot
 */

#include "callout.ah"
#include <arm/mx31.h>

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
	add		r4, r0, r2

	/*
	 * Map reset control registers
	 */
	mov		r0, #0x04
	ldr		r1, Lpaddr
	bl		callout_io_map

	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	MX31_WDOG_BASE


CALLOUT_START(reboot_mx31, 0, patch_reboot)
    /*
     * Get the Watchdog Timer base address (patched)
     */
    mov     ip,   #0x000000ff
    orr     ip, ip, #0x0000ff00
    orr     ip, ip, #0x00ff0000
    orr     ip, ip, #0xff000000

	mov		r1, #0x0130
	strh	r1, [ip, #MX31_WDOG_CR]
	orr		r1, r1, #0x04
	strh	r1, [ip, #MX31_WDOG_CR]
0:
	b		0b
CALLOUT_END(reboot_mx31)
