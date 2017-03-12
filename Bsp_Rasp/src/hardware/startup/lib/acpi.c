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
#include <hw/acpi.h>

paddr_t
acpi_find_table(unsigned table_name, unsigned *lenp) {
	static unsigned		num_tbls = -1;
	static paddr_t		*tbl_array;
	paddr_t				tbl_paddr;
	acpi_rsdp			*root;
	acpi_generic		*tbl;
	unsigned			i;
	unsigned			sig;

	if(num_tbls == 0) return NULL_PADDR;
	if(tbl_array == NULL) {
		root = board_find_acpi_rsdp();
		if(root == NULL) {
			num_tbls = 0;
			return NULL_PADDR;
		}
		if(root->v1.Revision == ACPI_RSDP_REVISION_1_0) {
			tbl_paddr = root->v1.RsdtAddress;
			startup_memory_unmap(root);
			tbl = startup_memory_map(0x4096, tbl_paddr, PROT_READ);
			num_tbls = (tbl->hdr.Length - offsetof(acpi_rsdt, Entry)) / sizeof(tbl->rsdt.Entry[0]);
			tbl_array = ws_alloc(num_tbls * sizeof(*tbl_array));
			for(i = 0; i < num_tbls; ++i) {
				tbl_array[i] = tbl->rsdt.Entry[i];
			}
		} else {
			tbl_paddr = root->XsdtAddress;
			startup_memory_unmap(root);
			tbl = startup_memory_map(0x4096, tbl_paddr, PROT_READ);
			num_tbls = (tbl->hdr.Length - offsetof(acpi_xsdt, Entry)) / sizeof(tbl->xsdt.Entry[0]);
			tbl_array = ws_alloc(num_tbls * sizeof(*tbl_array));
			for(i = 0; i < num_tbls; ++i) {
				tbl_array[i] = tbl->xsdt.Entry[i];
			}
		}
		startup_memory_unmap(tbl);
	}
	for(i = 0; i < num_tbls; ++i) {
		tbl_paddr = tbl_array[i];
		tbl = startup_memory_map(sizeof(*tbl), tbl_paddr, PROT_READ);
		sig = tbl->hdr.Signature;
		if(lenp != NULL) *lenp = tbl->hdr.Length;
		startup_memory_unmap(tbl);
		if(sig == table_name) return tbl_paddr;
	}
	return NULL_PADDR;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/acpi.c $ $Rev: 657836 $" );
