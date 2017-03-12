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
#include <alloca.h>

struct as_range {
	struct as_range	*next;
	paddr_t			start;
	paddr_t			end;
	unsigned		owner;
};

unsigned
as_add_containing(paddr_t start, paddr_t end, unsigned attr, const char *name, const char *container) {
	unsigned			owner;
	unsigned			off;
	unsigned			start_off;
	struct asinfo_entry	*as;
	paddr_t				piece_start;
	paddr_t				piece_end;
	unsigned			piece_attr;
	struct as_range		*list;
	struct as_range		**list_owner;

	list = NULL;
	off = 0;
	owner = AS_NULL_OFF;
	for( ;; ) {
		owner = as_find_containing(owner, start, end, container);
		if(owner == AS_NULL_OFF) break;
		as = as_off2info(owner);
		piece_start = start;
		if(as->start > piece_start) piece_start = as->start;
		piece_end = end;
		if(as->end < piece_end) piece_end = as->end;
		list_owner = &list;
		for( ;; ) {
			paddr_t			old_end;
			struct as_range	*curr;
			struct as_range	*new;

			curr = *list_owner;
			if(curr == NULL) {
				new = alloca(sizeof(*curr));
				new->start = piece_start;
				new->end   = piece_end;
				new->owner = owner;
				new->next  = NULL;
				*list_owner = new;
				break;
			}
			if(curr->start <= piece_start && curr->end >= piece_start) {
				if(curr->start == piece_start && curr->end == piece_end) {
					curr->owner = owner;
					break;
				}
				new = alloca(sizeof(*curr));
				new->start = piece_start;
				new->end   = piece_end;
				new->owner = owner;
				if(curr->start == piece_start) {
					curr->start = piece_end + 1;
					new->next = curr;
					*list_owner = new;
					break;
				}
				old_end = curr->end;
				curr->end = piece_start - 1;
				new->next = curr->next;
				curr->next = new;
				if(old_end != piece_end) {
					new = alloca(sizeof(*curr));
					new->start = piece_end + 1;
					new->end   = old_end;
					new->owner = curr->owner;
					new->next = curr->next->next;
					curr->next->next = new;
				}
				break;
			}
			list_owner = &curr->next;
		}
	}
	piece_attr = (attr & ~AS_OVERLAY_IO) | AS_ATTR_CONTINUED;
	start_off = AS_NULL_OFF;
	while(list != NULL) {
		if(list->start <= list->end) {
			off = as_add(list->start, list->end, piece_attr, name, list->owner);
			if(start_off == AS_NULL_OFF) start_off = off;
		}
		list = list->next;
	}
	if(start_off != AS_NULL_OFF) {
		as = as_off2info(off);
		as->attr &= ~AS_ATTR_CONTINUED;
		if(attr & AS_OVERLAY_IO) {
			off = start_off;
	
			for( ;; ) {
				as = as_off2info(off);
				as_add(as->start, as->end, attr & ~AS_OVERLAY_IO, name, off);
				if(!(as->attr & AS_ATTR_CONTINUED)) break;
				off += sizeof(*as);
			}
		}
	}
	return(start_off);
}

__SRCVERSION("as_add_containing.c $Rev: 657836 $");
