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

static int
match_name(struct asinfo_entry *base, struct asinfo_entry *as, const char *name) {
	for( ;; ) {
		if(strcmp(__hwi_find_string(as->name), name) == 0) return(1);
		if(as->owner == AS_NULL_OFF) return(0);
		as = ((struct asinfo_entry *)((uint8_t *)base + as->owner));
	}
}

unsigned
as_find_containing(unsigned off, paddr_t start, paddr_t end, const char *container) {
	struct asinfo_entry	*as;
	struct asinfo_entry	*base;
	struct asinfo_entry	*stop;

	as = base = lsp.asinfo.p;
	if(off != AS_NULL_OFF) {
		as = ((struct asinfo_entry *)((uint8_t *)as + off)) + 1;
	}
	stop = (struct asinfo_entry *)((uint8_t *)base + lsp.asinfo.size);
	for( ;; ) {
		if(as >= stop) return(AS_NULL_OFF);
		if(match_name(base, as, container)) {
			if((start >= as->start) && (start <= as->end)) break;
			if((end >= as->start) && (end <= as->end)) break;
			if((start <= as->start) && (end >= as->end)) break;
		}
		++as;
	}
	return((uintptr_t)as - (uintptr_t)base);
}

__SRCVERSION("as_find_containing.c $Rev: 657836 $");
