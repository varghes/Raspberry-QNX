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
#include <arm/mpcore.h>

/*
 * Additional Cortex A9 MPCore specific CPU initialisation
 */

#define	CPUID_REVISION(x)	((x) & 0x00f0000f)
#define	CPUID_R2P0			0x00200000
#define CPUID_R2P2			0x00200002

void
armv_setup_a9mp(struct cpuinfo_entry *cpu, unsigned cpuid)
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
		/*
		 * Initialise the GIC cpu interface for this cpu.
		 * For CPU0, this should be initialised in init_intrinfo()
		 */
		arm_gic_cpu_init();	
	}

	/*
	 * Configure CP15 auxiliary control register:
	 * - clear EXCL (bit 7) to set inclusive caches
	 * - set SMP (bit 6)
	 * - set FW  (bit 0) enable cache/TLB maintainence broadcast
	 *
	 * FIXME: should use symbolic constants
	 *        what about bit 8 (alloc in one way)
	 *        what about bit 3 (full of zero mode)
	 */
	aux = arm_v6_cp15_auxcr_get();	
	aux &= ~(1 << 7);
	aux |= (1 << 6) | (1 << 0);
	if (CPUID_REVISION(cpuid) < CPUID_R2P0) {
		aux &= ~((1 << 2) | (1 << 1));	// Errata #719332 - disable prefetch
	}
	arm_v6_cp15_auxcr_set(aux);	

	/*
	 * Processor implements ARMv7 Multiprocessor Extensions
	 */
	cpu->flags |= ARM_CPU_FLAG_V7_MP;

	if (CPUID_REVISION(cpuid) < CPUID_R2P0) {
		/*
		 * Inner-shareable TLB ops don't work correctly so make sure
		 * procnto-smp won't use them for handling TLB coherency.
		 * Without this flag it will use IPIs when necessary.
		 */
		cpu->flags &= ~ARM_CPU_FLAG_V7_MP;	// Errata #720789

		mmu_cr_set |= ARM_MMU_CR_RR;		// Errata #716044
	}

	if (CPUID_REVISION(cpuid) <= CPUID_R2P2) {
		__asm__ __volatile__("mrc	p15, 0, %0, c15, c0, 1" : "=r"(aux));
		aux |= (1 << 4);	// Errata #742230 - disable DMB_lite
		__asm__ __volatile__("mcr	p15, 0, %0, c15, c0, 1" : : "r"(aux));
	}

	/*
	 * Set S-bit in TTBR0 to indicate page table walks use shared memory
	 * vstart sets TTBR0 using L1_paddr + (cpu * 16K) so this will pick
	 * up the S-bit in that address calculation.
	 */
	L1_paddr |= ARM_MMU_TTBR_S;

	/*
	 * Perform generic ARMv7 CPU initialisation
	 */
	armv_setup_v7(cpu, cpuid, cpunum);
}


__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/trunk/hardware/startup/lib/arm/armv_setup_a9mp.c $ $Rev: 217585 $" );
