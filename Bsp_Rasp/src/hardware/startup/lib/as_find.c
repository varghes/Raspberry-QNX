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
#include <stdarg.h>
#include <alloca.h>

struct name_list {
	struct name_list	*prev;
	const char			*name;
};

static int
match_item(struct asinfo_entry *as, struct name_list *name) {
	for( ;; ) {
		if(strcmp(name->name, __hwi_find_string(as->name)) != 0) return(0);
		name = name->prev;
		if(name == NULL) return(1);
		if(as->owner == AS_NULL_OFF) return(0);
		as = (struct asinfo_entry *)((uint8_t *)lsp.asinfo.p + as->owner);
	}
}

unsigned
as_find(unsigned start, ...) {
	struct asinfo_entry	*as;
	struct asinfo_entry	*base;
	struct asinfo_entry	*end;
	va_list				args;
	struct name_list	*list;
	struct name_list	*curr;
	const char			*name;

	list = NULL;
	va_start(args, start);
	for(;;) {
		name = va_arg(args, char *);
		if(name == NULL) break;
		curr = alloca(sizeof(*curr));
		curr->name = name;
		curr->prev = list;
		list = curr;
	}
	if (!name && !list) {
		crash("%s: NULL name argument\n", __FUNCTION__);
	}
	va_end(args);
	as = base = lsp.asinfo.p;
	if(start != AS_NULL_OFF) {
		as = ((struct asinfo_entry *)((uint8_t *)as + start)) + 1;
	}
	end = (struct asinfo_entry *)((uint8_t *)base + lsp.asinfo.size);
	for( ;; ) {
		if(as >= end) return(AS_NULL_OFF);
		if(match_item(as, list)) return((uintptr_t)as - (uintptr_t)base);
		++as;
	}
}

__SRCVERSION("as_find.c $Rev: 657836 $");
