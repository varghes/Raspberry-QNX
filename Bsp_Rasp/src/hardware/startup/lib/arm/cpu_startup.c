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





#include "startup.h"

/*
 * Perform CPU specific startup initialization.
 * This code is hardware independant and should not have to be changed
 * changed by end users.
 */
void
cpu_startup()
{ 
	const struct armv_chip	*chip;

	/*
	 * Enable icache and branch prediction if appropriate
	 */
	chip = armv_chip_detect();
	if (chip) {
		unsigned	bits = chip->mmu_cr_set & (ARM_MMU_CR_I | ARM_MMU_CR_Z);
		unsigned	mmucr;

		if (bits) {
			__asm__ __volatile__("mrc	p15, 0, %0, c1, c0, 0" : "=r" (mmucr));
			__asm__ __volatile__("mcr	p15, 0, %0, c1, c0, 0" : : "r" (mmucr | bits));
		}
	}
	board_cpu_startup();
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/cpu_startup.c $ $Rev: 657836 $" );
