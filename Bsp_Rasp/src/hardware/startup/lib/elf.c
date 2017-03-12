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
#include <sys/elf_nto.h>

Elf32_Phdr *
is_elf(Elf32_Ehdr *hdr, paddr32_t addr) {
	Elf32_Ehdr	*p;

	p = startup_memory_map(sizeof(*hdr), addr, PROT_READ);
	*hdr = *p;
	startup_memory_unmap(p);

	if (memcmp(hdr->e_ident, ELFMAG, SELFMAG)
		|| hdr->e_ident [EI_DATA] != ELFDATANATIVE
		|| hdr->e_machine != EM_NATIVE
		|| hdr->e_phnum == 0
		|| hdr->e_phentsize != sizeof(Elf32_Phdr))
		return(NULL);

	return startup_memory_map(hdr->e_phnum * sizeof(Elf32_Phdr),
				addr + hdr->e_phoff, PROT_READ|PROT_WRITE);
}


#define PAGEBITS 	(__PAGESIZE-1)

static void
load_elf32mmu(paddr32_t addr, Elf32_Ehdr *hdr, Elf32_Phdr *phdr) {
	int			i;

	for(i = 0; i < hdr->e_phnum; ++i, ++phdr) {
		switch(phdr->p_type) {
		case PT_LOAD:
			{
				size_t memsz  = phdr->p_memsz + (phdr->p_vaddr & PAGEBITS);
				size_t filesz = phdr->p_filesz + (phdr->p_vaddr & PAGEBITS);
				uintptr_t vaddr  = phdr->p_vaddr & ~PAGEBITS;
				paddr32_t paddr  = phdr->p_paddr & ~PAGEBITS;
				paddr32_t daddr;

				if((boot_vaddr_base == 0u) || (boot_vaddr_base > vaddr)){
					boot_vaddr_base = vaddr;
				}
				if(boot_vaddr_end < (vaddr + memsz)){
					boot_vaddr_end = vaddr + memsz;
				}
								
				if((phdr->p_flags & PF_W) == 0 && memsz == filesz) {
					filesz = memsz = ROUNDPG(filesz);
				}
				if(phdr->p_paddr != 0) {
					if (phdr->p_memsz == phdr->p_filesz) {
						// mkifs has padded out the data/bss section
						filesz = memsz = ROUNDPG(filesz);
					}
					elf_map(vaddr, paddr, filesz & ~PAGEBITS, phdr->p_flags);
					memsz = ROUNDPG(memsz - (filesz & ~PAGEBITS));
					if(memsz) {
						daddr = calloc_ram(memsz, __PAGESIZE);
						copy_memory(daddr, (paddr + (filesz & ~PAGEBITS)), filesz & PAGEBITS);
						elf_map(vaddr + (filesz & ~PAGEBITS), daddr, memsz, phdr->p_flags);
					}
				} else {
#ifndef BOOTSTRAPS_RUN_ONE_TO_ONE
	/*
	 * We need to do different things depending on whether the bootstrap
	 * executables a run in a physical <-> virtual one to one mapping
	 * area (MIPS, SH, PPC) or if they use the normal virtual address
	 * mapping gear (X86, ARM). Basically, is startup or procnto
	 * enabling the MMU on the CPU.
	 */
	#error BOOTSTRAPS_RUN_ONE_TO_ONE must be defined
#endif
#if BOOTSTRAPS_RUN_ONE_TO_ONE
					daddr = alloc_ram(vaddr - shdr->paddr_bias, ROUNDPG(memsz), __PAGESIZE);
					if(daddr != vaddr - shdr->paddr_bias) {
						crash("Error: can not allocate RAM for proc in XIP.\n");
					}
					memmove((void *)phdr->p_vaddr,
							MAKE_1TO1_PTR(addr + phdr->p_offset),
							phdr->p_filesz);
					memset((void *)(phdr->p_vaddr + phdr->p_filesz), 0,
								(phdr->p_memsz - phdr->p_filesz));
#else
					daddr = calloc_ram(ROUNDPG(memsz), __PAGESIZE);
					copy_memory(daddr, (addr + phdr->p_offset) - (phdr->p_vaddr & PAGEBITS), filesz);
					elf_map(vaddr, daddr, memsz, phdr->p_flags);
#endif
				}
			}
			break;
		}
	}
}


uintptr_t
load_elf32(paddr32_t addr) {
	Elf32_Ehdr	hdr;
	Elf32_Phdr	*phdr;
	Elf32_Phdr	*phdr_start;
	int			i;
	
	if((phdr_start = is_elf(&hdr, addr)) == NULL) {
		return(~0L);
	}

	if(!(shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL)) {
		phdr = phdr_start;
		for(i = 0; i < hdr.e_phnum; ++i, ++phdr) {
			switch(phdr->p_type) {
			case PT_LOAD:
				if(MAKE_1TO1_PTR(phdr->p_paddr) != (void *)phdr->p_vaddr) {
					alloc_ram(phdr->p_vaddr - shdr->paddr_bias, phdr->p_memsz, 1);
					memmove((void *)phdr->p_vaddr,
						MAKE_1TO1_PTR(addr + phdr->p_offset),
						phdr->p_filesz);
					memset((void *)(phdr->p_vaddr+phdr->p_filesz), 0,
								phdr->p_memsz - phdr->p_filesz);
				}
				break;
			}
		}
#ifdef __X86__
	} else if(load_elf32mmu_4m(addr, &hdr, phdr_start)) {
		// Nothing to do
#endif
	} else {
		load_elf32mmu(addr, &hdr, phdr_start);
	}

	startup_memory_unmap(phdr_start);

	return hdr.e_entry;
}

__SRCVERSION("elf.c $Rev: 657836 $");
