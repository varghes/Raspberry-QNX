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
#include <arm/mpcore.h>

/*
 * The board specific startup MUST set this to the SCU address
 */
unsigned mpcore_scu_base = ~0;

/*
 * Additional ARM11 MPcore specific CPU initialisation
 */

void
armv_setup_mp11(struct cpuinfo_entry *cpu, unsigned cpuid)
{
	unsigned	aux;
	unsigned	cpunum;

	cpunum = arm_v6_cpunum();
	if (cpunum == 0) {
		if (mpcore_scu_base == ~0) {
			crash("mpcore_scu_base is not set");
		}

		/*
		 * SCU invalidate all and enable SCU
		 */
		out32(mpcore_scu_base + MPCORE_SCU_INVALIDATE, -1);
		out32(mpcore_scu_base + MPCORE_SCU_CTRL, MPCORE_SCU_CTRL_EN);
	}
	else {
		unsigned	base = mpcore_scu_base + MPCORE_GIC_CPU_BASE;

		/*
		 * Initialise the SCU interrupt distributor interface for this cpu.
		 * For CPU0, this should be initialised in init_intrinfo()
		 */
		out32(base + MPCORE_GIC_CPU_PRIO_MASK, 0xf0);
		out32(base + MPCORE_GIC_CPU_CTRL, MPCORE_GIC_CPU_CTRL_EN);
	}

	/*
	 * Configure CP15 auxiliary control register:
	 * - inclusive L1/L2 caches
	 * - SMP mode
	 * - enable all branch prediction
	 */
	aux = arm_v6_cp15_auxcr_get();
	aux &= ~MPCORE_CP15_AUX_EXCL;
	aux |= MPCORE_CP15_AUX_SMP | MPCORE_CP15_AUX_F | MPCORE_CP15_AUX_SB | MPCORE_CP15_AUX_DB | MPCORE_CP15_AUX_RS;
	arm_v6_cp15_auxcr_set(aux);	


	/*
	 * Set S-bit in TTBR0 to indicate page table walks use shared memory
	 * vstart sets TTBR0 using L1_paddr + (cpu * 16K) so this will pick
	 * up the S-bit in that address calculation.
	 */
	L1_paddr |= ARM_MMU_TTBR_S;

	/*
	 * Perform generic ARMv6 CPU initialisation
	 */
	armv_setup_v6(cpu, cpuid, cpunum);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_setup_mp11.c $ $Rev: 657836 $" );
