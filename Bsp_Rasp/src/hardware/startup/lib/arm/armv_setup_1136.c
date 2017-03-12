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
 * Additional ARM1136 specific CPU initialisation
 */

void
armv_setup_1136(struct cpuinfo_entry *cpu, unsigned cpuid)
{
	unsigned	tmp;

	/*
	 * Perform generic ARMv6 CPU initialisation
	 */
	armv_setup_v6(cpu, cpuid, 0);

	/*
	 * errata #364296 - possible data cache corruption with hit under miss
	 * errata #380532 - load multiple to pc can be corrupted or deadlock
	 *
	 * Workaround by using undocumented aux cr bit to disable hit under miss
	 */
	mmu_cr_set |= ARM_MMU_CR_FI;
	tmp = arm_v6_cp15_auxcr_get();
	tmp |= 0x80000000;
	arm_v6_cp15_auxcr_set(tmp);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_setup_1136.c $ $Rev: 657836 $" );
