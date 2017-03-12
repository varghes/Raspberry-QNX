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
 * Initialize cpuinfo structures in the system page.
 * This code is hardware independant and should not have to be changed
 * changed by end users.
 */

unsigned	trap_vectors;
unsigned	mmu_cr_set;
unsigned	mmu_cr_clr;

/*
 * -----------------------------------------------------------------------
 * Page flush callouts that go into the syspage.arm.mmu section
 * -----------------------------------------------------------------------
 */

static output_callout_t			*page_prev_output_rtn;

static output_callout_t *
callout_output_page(int sizing)
{
	struct arm_cpu_entry		*cpu = lsp.cpu.arm_cpu.p;

	callout_output_one(sizing, &cpu->page_flush);
	callout_output_one(sizing, &cpu->page_flush_deferred);

	return page_prev_output_rtn;
}

static void
add_page_callouts(const struct armv_chip *chip)
{
	struct arm_cpu_entry		*cpu = lsp.cpu.arm_cpu.p;

	/*
	 * Hook into the callout sizing/output list
	 */
	if (page_prev_output_rtn == 0) {
		page_prev_output_rtn = callout_output_rtn;
		callout_output_rtn = (output_callout_t *)callout_output_page;
	}
	cpu->page_flush = (void *)chip->flush;
	cpu->page_flush_deferred = (void *)chip->deferred;
}


void
init_one_cpuinfo(unsigned cpunum)
{
	struct cpuinfo_entry	*cpu;
	unsigned				cpuid;
	const struct armv_chip	*chip;

	cpu = &lsp.cpuinfo.p[cpunum];

	cpu->cpu = cpuid = arm_mmu_cpuid();

	/*
	 * Work out what core we are running on
	 */
	chip = armv_chip_detect();
	if (chip == 0) {
		crash("Unsupported CPUID");
	}

	cpu->flags = 0;
	if (shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL) {
		cpu->flags |= CPU_FLAG_MMU;
	}
	if (lsp.syspage.p->num_cpu > 1) {
		cpu->flags |= ARM_CPU_FLAG_SMP;
	}

	if (cpunum == 0) {
		/*
		 * WARNING: we assume the following are the same for ALL cpus:
		 *          - mmu_cr_set/mmu_cr_clr
		 *          - page callouts
		 *          - power callout
		 */

		/*
		 * Set default mmu_cr_set values and trap_vectors
		 */
		mmu_cr_set = ARM_MMU_CR_V | ARM_MMU_CR_L | ARM_MMU_CR_D | ARM_MMU_CR_P
				   | ARM_MMU_CR_A | ARM_MMU_CR_W | ARM_MMU_CR_C | ARM_MMU_CR_M;
		trap_vectors = 0xffff0000;

		mmu_cr_clr = chip->mmu_cr_clr;
		mmu_cr_set |= chip->mmu_cr_set;
		mmu_cr_set &= ~mmu_cr_clr;

		add_page_callouts(chip);

		if (chip->power) {
			add_callout(offsetof(struct callout_entry, power), chip->power);
		}

		if (shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL) {
			int	i;

			/*
			 * Allocate and map the trap vector table
			 */
			paddr32_t	pa = calloc_ram(__PAGESIZE, __PAGESIZE);

			for (i = 0; i < lsp.syspage.p->num_cpu; i++) {
				arm_map_cpu(i, trap_vectors, pa, ARM_PTE_RW | ARM_PTE_CB);
			}

			/*
			 * Set up the trap entry point to be "ldr pc, [pc, #0x18]"
			 * These jump slot addresses are all zero, so until these have
			 * been properly set up, any exception will result in a loop.
			 */
			for (i = 0; i < 8; i++) {
				*((unsigned *)pa + i) = 0xe59ff018;
			}
		}
	}

	cpu->name = add_string(chip->name);
	cycles_per_loop = chip->cycles;

	/*
	 * Add cache callouts
	 */
	arm_add_cache(cpu, chip->cache);

	/*
	 * Do any extra CPU specific initialisation
	 */
	if (chip->setup) {
		chip->setup(cpu, cpuid);
	}

	/*
	 * Set the CPU speed
	 */
	if (cpu_freq != 0) {
		cpu->speed = cpu_freq / 1000000;
	}
	else {
		cpu->speed = (cycles_per_loop != 0) ? arm_cpuspeed() : 0;
	}
	if (debug_flag) {
		kprintf("CPU%d: %x: %s rev %d %dMHz\n", cpunum, cpuid, chip->name, cpuid & 15, cpu->speed);
	}
}

void
init_cpuinfo()
{
	struct cpuinfo_entry	*cpu;
	unsigned				num;
	unsigned				i;

	num = lsp.syspage.p->num_cpu;
	if (num > 1) {
		extern unsigned	__cpu_flags;

		/*
		 * __cpu_flags is never explictly initialised in libstartup so set it
		 * to ensure mem_barrier/__cpu_membarrier executes the DMB operation
		 */
		__cpu_flags |= ARM_CPU_FLAG_SMP;
	}

	cpu = set_syspage_section(&lsp.cpuinfo, sizeof(*lsp.cpuinfo.p) * num);
	for (i = 0; i < num; i++) {
		cpu[i].ins_cache  = system_icache_idx;
		cpu[i].data_cache = system_dcache_idx;
	}
	init_one_cpuinfo(0);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_cpuinfo.c $ $Rev: 657836 $" );
