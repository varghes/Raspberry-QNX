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

paddr_t		L1_paddr;
paddr_t		L1_vaddr;
paddr_t		L2_paddr;
paddr_t		startup_base;
unsigned	startup_size;

/*
 * Create section mappings in the 1-1 mapping area for the largest RAM bank
 */
static void
map_1to1_ram()
{
	unsigned	as_off = AS_NULL_OFF;
	unsigned	base = 0;
	unsigned	size = 0;
	unsigned	vaddr;

	while ((as_off = as_find(as_off, "ram", 0)) != AS_NULL_OFF) {
		struct asinfo_entry *as = (struct asinfo_entry *)((uintptr_t)lsp.asinfo.p + as_off);
		unsigned			sz  = as->end - as->start + 1;

		if (sz > size) {
			base = (unsigned)as->start;
			size = sz;
		}
	}
	if (size == 0) {
		crash("no ram entry in asinfo?");
	}
	if (base & ARM_SCMASK) {
		if (debug_flag) {
			kprintf("WARNING: RAM base at %x - not using 1to1 area\n", base);
		}
		return;
	}

	/*
	 * Limit mapping to 256MB
	 */
	if (size > ARM_1TO1_SIZE) {
		size = ARM_1TO1_SIZE;
	}
	size = (size + ARM_SCMASK) & ~ARM_SCMASK;
	as_add(base, base + size - 1, AS_ATTR_RAM, "1to1", as_default());

	for (vaddr = ARM_1TO1_BASE; size; vaddr += ARM_SCSIZE, base += ARM_SCSIZE, size -= ARM_SCSIZE) {
		arm_scmap(vaddr, base, ARM_PTE_RW | ARM_PTE_CB);
	}
}

/*
 * Initialize page tables. This prepares the system to leap into virtual mode.
 * This code is hardware independant and should not have to be
 * changed by end users.
 */
void
init_mmu(void)
{
	unsigned	base;
	unsigned	ncpu = lsp.syspage.p->num_cpu;
	unsigned	L1size = ncpu * ARM_L1_SIZE;
	unsigned	L2size = ncpu * __PAGESIZE;

	/*
	 * Get the CPU-specific PTE descriptors
	 */
	arm_pte_setup();

	/*
	 * Allocate the L1 table and the "page directory" used to map L2 tables
	 */
	L1_paddr = calloc_ram(L1size, ARM_L1_SIZE);
	L2_paddr = calloc_ram(L2size, __PAGESIZE);

	/*
	 * Map the "page directory" within itself
	 */
	arm_pdmap(ARM_PTP_BASE);

	/*
	 * Map the real L1 table
	 */
	L1_vaddr = arm_map(~0L, L1_paddr, L1size, ARM_MAP_NOEXEC | ARM_PTE_RW | armv_chip->pte_attr);
	L1_paddr |= armv_chip->ttb_attr;

	/*
	 * Section map startup code to allow transition to virtual addresses.
	 * This 1-1 mapping is also used by kdebug to access the imagefs.
	 * procnto uses syspage->un.arm.startup_base/startup_size to unmap it.
	 */
	startup_base = shdr->ram_paddr & ~ARM_SCMASK;
	startup_size = shdr->ram_size;
	for (base = startup_base; base < startup_base + startup_size; base += ARM_SCSIZE) {
		arm_scmap(base, base, ARM_PTE_RO);
	}

	/*
	 * Map RAM into the 1-1 mapping area
	 */
	map_1to1_ram();
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_mmu.c $ $Rev: 657836 $" );
