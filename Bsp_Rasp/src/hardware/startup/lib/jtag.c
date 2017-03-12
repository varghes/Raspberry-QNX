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

static paddr_t	jtag_syspage_address = NULL_PADDR;

void
jtag_reserve_memory(paddr_t resmem_addr, size_t resmem_size, uint8_t resmem_flag) {
	void	*p;

    // Reserve user specified block of memory
	alloc_ram(resmem_addr, resmem_size, 1);
	p = startup_memory_map(resmem_size, resmem_addr, PROT_READ|PROT_WRITE);
	// Determine if reserved memory should be cleared
	if(!resmem_flag) {
		memset(p, 0, resmem_size);
	}
	startup_memory_unmap(p);
}

void
jtag_reserve_syspage_addr(paddr_t jtag_syspage_addr) {

	// Save the address to be used later
	jtag_syspage_address = jtag_syspage_addr;

  	// Reserve a block of memory to store the address of the system page
	alloc_ram(jtag_syspage_address, sizeof(syspage_paddr), 1);
}

void
jtag_store_syspage_addr(void) {
	// Save the address of the system page at a known location to be
	// read by a JTAG.
	// Only save the system page address if some memory has been reserved
	if(jtag_syspage_address != NULL_PADDR) {
		paddr32_t	*p;

		p = startup_memory_map(sizeof(syspage_paddr), jtag_syspage_address, PROT_READ|PROT_WRITE);
		*p = syspage_paddr;
		startup_memory_unmap(p);
	}
}

__SRCVERSION("jtag.c $Rev: 657836 $");
