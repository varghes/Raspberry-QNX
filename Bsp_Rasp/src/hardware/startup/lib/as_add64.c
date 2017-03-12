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


unsigned
as_add64(paddr64_t start, paddr64_t end, unsigned attr, const char *name, unsigned owner) {
	uint8_t				*p;
	struct asinfo_entry	*as;
	unsigned			name_off;
	unsigned			off;

	name_off = add_string(name);

	p = grow_syspage_section(&lsp.asinfo, sizeof(*as));
	as = (struct asinfo_entry *)(p + lsp.asinfo.size - sizeof(*as));

	as->start = start;
	as->end   = end;
	as->owner = owner;
	as->name = name_off;
	as->attr = attr & 0xffff;
	as->priority = AS_PRIORITY_DEFAULT;
	as->alloc_checker = NULL;

	if(owner != AS_NULL_OFF) {
		struct asinfo_entry	*own_as;

		own_as = (struct asinfo_entry *)(p + owner);

		own_as->attr |= AS_ATTR_KIDS;
	}

	off = (uintptr_t)as - (uintptr_t)p;

	if(attr & AS_OVERLAY_IO) {
		as_add(start, end, attr & ~AS_OVERLAY_IO, "io", off);
	}

	return(off);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/as_add64.c $ $Rev: 657836 $" );
