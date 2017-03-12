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
 * Cirrus Logic EP93xx reboot support
 *
 * This should be usable by any board that uses a Cirrus Logic EP93xx processor
 * which contains an ARM 920T core
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
 * -----------------------------------------------------------------------
 */
patch_reboot:
	stmdb	sp!,{r4,lr}
	add		r4, r0, r2

	/*
	 * Map reset control registers
	 */
	mov		r0, #EP93xx_SYSCTRL_SIZE
	ldr		r1, Lpaddr
	mov		r2, #0xB00				// PROT_READ|PROT_WRITE|PROT_NOCACHE
	bl		callout_memory_map
	/*
	 * Patch the callout routine
	 */
	CALLOUT_PATCH	r4, r0, r1, r2, ip
	ldmia	sp!,{r4,pc}

Lpaddr:	.word	EP93xx_SYSCTRL_BASE

/*
 * -----------------------------------------------------------------------------
 * reboot_ep93xx
 *
 * r0 - syspage ptr
 * r1 - 'abnormal' flag (if non zero we simply hang)
 *
 * Software reset is effected by a 1 to 0 transition of bit 31 in the DeviceCfg
 * register. We must unlock the register in order to write to it each time
 * -----------------------------------------------------------------------------
*/
CALLOUT_START(reboot_ep93xx, 0, patch_reboot)
	mov		ip,     #0x000000ff
	orr		ip, ip, #0x0000ff00
	orr		ip, ip, #0x00ff0000
	orr		ip, ip, #0xff000000

	/* disable interrupts */
	mrs		r0, cpsr
	orr		r0, r0, #0xc0
	msr		cpsr, r0

	/* test the 'abnormal' flag and if non-zero, just hang */
	cmp		r1, #0
	bne		0f

	/* perform a processor reset */
	mov		r1, #0xAA
	ldr		r0, [ip, #EP93xx_SYSCTRL_DEVICECFG]
	/* make sure bit 31 is set */
	orr		r0, r0, #0x80000000
	str		r1, [ip, #EP93xx_SYSCTRL_SYSSWLOCK]
	str		r0, [ip, #EP93xx_SYSCTRL_DEVICECFG]

	/* now clear bit 31 to cause the reset */
	bic		r0, r0, #0x80000000
	str		r1, [ip, #EP93xx_SYSCTRL_SYSSWLOCK]
	str		r0, [ip, #EP93xx_SYSCTRL_DEVICECFG]

0:	b		0b

CALLOUT_END(reboot_ep93xx)
