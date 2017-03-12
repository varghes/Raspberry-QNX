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




#include "startup.h"

/*
 * Additional Marvell Sheeva specific CPU initialisation
 */

void
armv_setup_sheeva_v7(struct cpuinfo_entry *cpu, unsigned cpuid)
{
	unsigned	aux;
	unsigned	cpunum = arm_v6_cpunum();

	/*
	 * Perform generic ARMv7 CPU initialisation
	 */
	armv_setup_v7(cpu, cpuid, cpunum);

	/*
	 * Set up Auxiliary Control register values
	 */
	aux = arm_v6_cp15_auxcr_get();	
	aux |= (1 << 1);		// enable L2 cache
	arm_v6_cp15_auxcr_set(aux);

	/*
	 * Check for iWMMX2 unit (VFP is detected in armv_setup_v7)
	 */
	armv_sheeva_wmmx_detect(cpu, cpunum);

	/*
	 * Implement any errata workarounds
	 */
	armv_sheeva_errata_check(cpuid);
}


__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/trunk/hardware/startup/lib/arm/armv_setup_sheeva_v7.c $ $Rev: 217585 $" );
