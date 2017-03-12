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
 * Generic ARMv7 CPU initialisation
 *
 * FIXME: this may still need additional work...
 */

void
armv_setup_v7(struct cpuinfo_entry *cpu, unsigned cpuid, unsigned cpunum)
{
	unsigned	tmp;

	/*
	 * Check for presence of VFP (cp10/11) and enable access if present
	 */
	tmp = arm_v6_cp15_cpacr_get();
	tmp |= ARM_V6_CPACR_VFP;
	arm_v6_cp15_cpacr_set(tmp);	

	/*
	 * Access bits will not be written if coprocessor is not present
	 */
	tmp = arm_v6_cp15_cpacr_get();
	if ((tmp & ARM_V6_CPACR_VFP) == ARM_V6_CPACR_VFP) {
		unsigned	mvfr0;
		unsigned	mvfr1;

		/*
		 * Indicate we have an FPU unit
		 */
		cpu->flags |= CPU_FLAG_FPU;

		/*
		 * Check if we have Neon or VFP only
		 */
		__asm__ __volatile__("mrc	p10, 7, %0, c7, c0, 0" : "=r"(mvfr0));
		__asm__ __volatile__("mrc	p10, 7, %0, c6, c0, 0" : "=r"(mvfr1));
		if ((mvfr1 & 0x00ffff00) != 0) {
			cpu->flags |= ARM_CPU_FLAG_NEON;
		}

		if (debug_flag) {
			__asm__ __volatile__("mrc	p10, 7, %0, c0, c0, 0" : "=r" (tmp));
			kprintf("CPU%d: VFP-d%d FPSID=%x\n", cpunum, (mvfr0 & 0xf) * 16, tmp);
			if (cpu->flags & ARM_CPU_FLAG_NEON) {
				kprintf("CPU%d: NEON MVFR0=%x MVFR1=%x\n", cpunum, mvfr0, mvfr1);
			}
		}
	}

	/*
	 * Indicate we are ARMv7 architecture using ARMv6 extended page tables
	 * Use vstart_v7 to enable MMU to start kernel
	 */
	cpu->flags |= ARM_CPU_FLAG_V7 | ARM_CPU_FLAG_V6 | ARM_CPU_FLAG_V6_ASID;
	vstart = vstart_v7;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_setup_v7.c $ $Rev: 657836 $" );
