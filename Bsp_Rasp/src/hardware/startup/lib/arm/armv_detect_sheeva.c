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
 * Detect various configurations for Marvell Sheeva processors
 */

const struct armv_chip *
armv_detect_sheeva(void)
{
	unsigned	cpuid_ext;

	__asm__ __volatile__("mrc	p15, 1, %0, c15, c12, 0" : "=r"(cpuid_ext));
	switch (cpuid_ext) {
	case 0x00020001:
		return &armv_chip_sheeva_v6;

	case 0x00030001:
		return &armv_chip_sheeva_v7;

	default:
		return 0;
	}
}

/*
 * Detect if Sheeva core implements iWMMX2
 */
void
armv_sheeva_wmmx_detect(struct cpuinfo_entry *cpu, unsigned cpunum)
{
	unsigned	tmp;

	/*
	 * Attempt to enable CP0/CP1
	 */
	tmp = arm_v6_cp15_cpacr_get();
	tmp |= ARM_V6_CPACR_WMMX2;
	arm_v6_cp15_cpacr_set(tmp);

	/*
	 * Bits will only be set if CP0/CP1 are present
	 */
	tmp = arm_v6_cp15_cpacr_get();
	if ((tmp & ARM_V6_CPACR_WMMX2) == ARM_V6_CPACR_WMMX2) {
		/*
		 * Indicate iWMMX2 unit is present
		 */
		cpu->flags |= ARM_CPU_FLAG_WMMX2;

		if (debug_flag) {
			unsigned	wcid;

			__asm__ __volatile__("mrc	p1, 0, %0, c0, c0, 0" :"=r"(wcid));
			kprintf("CPU0: iWMMX wCID=%x\n", wcid);
		}

		/*
		 * Access is controlled by kernel
		 */
		arm_v6_cp15_cpacr_set(tmp & ~ARM_V6_CPACR_WMMX2);
	}
}

/*
 * Handle various Errata
 */
void
armv_sheeva_errata_check(unsigned cpuid)
{
	unsigned	tmp;

	/*
	 * FIXME: Apply the following fixes for Z0, Z1 and Y0 revisions
	 *        Errata documentation says they are relevant for Y0 so the
	 *        assumption is they are also present on earlier revisions.
	 */
	if ((cpuid & 0xf) <= 2) {
		/*
		 * Errata 3383 - disable branch return stack prediction
		 */
		tmp = arm_v6_cp15_auxcr_get();
		tmp &= ~1;
		arm_v6_cp15_auxcr_set(tmp);

		__asm__ __volatile__("mrc	p15, 1, %0, c15, c1, 0" : "=r"(tmp));
		tmp |= 0x01000000;	// Errata 3097 - disable L2 prefetch
		tmp &= ~0x10000000;	// Errata 3681 - disable L2 write-allocate
		__asm__ __volatile__("mcr	p15, 1, %0, c15, c1, 0" : : "r"(tmp));
	}
}

__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/trunk/hardware/startup/lib/arm/armv_detect_sheeva.c $ $Rev: 217585 $" );
