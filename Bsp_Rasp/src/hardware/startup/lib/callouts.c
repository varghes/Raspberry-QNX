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

//
// This code is hardware independent and should not have to be
// changed by end users.
//

struct co_info {
	struct co_info	*next;
	union {
		void			*owner;
		unsigned		rw_off;
	}				u;
	void			*data;
};


static struct co_info			*co_free_list;
static struct co_info			*rw_addrs;
static struct co_info			*patch_data;
static unsigned					rw_size;
static unsigned					rtn_size;

static struct co_info *
get_co_info(void) {
	struct co_info	*next;
	unsigned		i;

	#define	INFO_BLOCK	20

	if(co_free_list == NULL) {
		next = ws_alloc(sizeof(*next)*INFO_BLOCK);
		if(next == NULL) {
			crash("No memory for callout information structure.\n");
		}
		for(i = 0; i < INFO_BLOCK; ++i) {
			next->next = co_free_list;
			co_free_list = next;
			++next;
		}
	}
	next = co_free_list;
	co_free_list = next->next;
	return(next);
}

unsigned
output_callouts(int sizing) {
	output_callout_t		*one_callout;
	unsigned	size;

	one_callout = callout_output_rtn;
	do {
		one_callout = (output_callout_t *)one_callout(sizing);
	} while(one_callout != NULL);

	rw_size = ROUND(rw_size, sizeof(uint64_t));
	size = ROUND(rtn_size, sizeof(uint64_t)) + rw_size;
	if(sizing) rtn_size = 0; // Init trigger in callout_write_one
	return(size);
}

void
callout_output_one(int sizing, void *rp) {
	const struct callout_rtn	*rtn;
	callout_fp_t				*fp;
	struct co_info				*rw;
	unsigned					rw_off;
	uintptr_t					vaddr;

	fp = rp;
	rtn = (struct callout_rtn *)*fp;
	if(rtn == NULL) {
		return;
	}
	if(sizing) {
		rtn_size += rtn->rtn_size;
		if(rtn->rw_size != NULL) {
			rw = rw_addrs;
			for( ;; ) {
				if(rw == NULL) break;
				if(rw->data == rtn->rw_size) return;
				rw = rw->next;
			}
			rw = get_co_info();
			rw->data = rtn->rw_size;
			rw->u.rw_off = rw_size;
			rw->next = rw_addrs;
			rw_addrs = rw;
			rw_size += *rtn->rw_size;
		}
		return;
	}

	rw_off = lsp.syspage.p->total_size;
	if(rtn_size == 0) {
		memset((uint8_t *)_syspage_ptr + rw_off, 0, rw_size);
		rtn_size = rw_off + rw_size;
	}
	memmove((uint8_t *)_syspage_ptr + rtn_size, rtn->rtn_code, rtn->rtn_size);
	if(rtn->rw_size != NULL) {
		for(rw = rw_addrs; rw->data != rtn->rw_size; rw = rw->next) {
			// nothing to do.
		}
		rw_off += rw->u.rw_off;
	}
	vaddr = (uintptr_t)lsp.system_private.p->kern_syspageptr;
	if(rtn->patcher != NULL) {
		void			*data;
		struct co_info	*pd;
		
	   	data = NULL;
		pd = patch_data;
		for( ;; ) {
			if(pd == NULL) break;
			if(pd->u.owner == rp) {
				data = pd->data;
				break;
			}
			pd = pd->next;
		}
		rtn->patcher(syspage_paddr, vaddr, rtn_size, rw_off, data, rtn);
	}
	vaddr += rtn_size;
	rtn_size += rtn->rtn_size;
	*fp = (callout_fp_t)vaddr;
}

void
callout_register_data(void *rp, void *data) {
	struct co_info				*pd;
	const struct callout_rtn	*rtn;
	callout_fp_t				*fp;
	
	fp = rp;
	rtn = (struct callout_rtn *)*fp;
	if(data != NULL && (rtn != NULL) && (rtn->patcher != NULL)) {
		// Make sure that we're not overriding something already given.
		for(pd = patch_data; pd != NULL; pd = pd->next) {
			if(pd->u.owner == rp) return;
		}
		pd = get_co_info();
		pd->next = patch_data;
		patch_data = pd;
		pd->u.owner = rp;
		pd->data = data;
	}
}

void
callout_reloc_data(void *base, unsigned len, int diff) {
	struct co_info	*pd;
	
	// Temp syspage storage has moved around. Adjust registered
	// addresses.
	for(pd = patch_data; pd != NULL; pd = pd->next) {
		if(((uintptr_t)base <= (uintptr_t)pd->u.owner)
		   && (((uintptr_t)base + len) > (uintptr_t)pd->u.owner)) {
			pd->u.owner = (void *)((uintptr_t)pd->u.owner + diff);
		}
	}
}


void *
add_callout(unsigned offset, const struct callout_rtn *callout) {
	void			(**call)(void) = (void (**)(void))lsp.callout.p;

	call += offset / sizeof(*call);
	*call = (void (*)(void))callout;

	if( offset >= offsetof(struct callout_entry, debug[0])
	 && offset <  offsetof(struct callout_entry, debug[1])) {
		callout_register_data(call, &dbg_device[0]);
	 } else if( offset >= offsetof(struct callout_entry, debug[1]) 
	         && offset <  offsetof(struct callout_entry, debug[2])) {
		callout_register_data(call, &dbg_device[1]);
	 }

	return (void *)call;	
}

void
add_callout_array(const struct callout_slot *slots, unsigned size) {
	unsigned	i;
	unsigned	idx;

	idx = size / sizeof(*slots);
	for(i = 0; i < idx; ++i, ++slots) {
		add_callout(slots->offset, slots->callout);
	}
}

static output_callout_t *
callout_output_section(int sizing) {
	void			(**call)(void) = (void (**)(void))lsp.callout.p;
	unsigned 		i;
	unsigned		nelts;

	nelts = sizeof(*lsp.callout.p) / sizeof(*call);
	for(i = 0; i < nelts; ++i, ++call) {
		if(!sizing) {
			//
			// No longer required because of callout_register_data().
			// Can be removed once we get all the callouts converted.
			//
			if(i >= (offsetof(struct callout_entry, debug[1]) / sizeof(callout_fp_t))) {
				// Kludge: the patcher routines have to know which debug
				// channel they're working on.
				patch_channel = 1;
			}
		}
		callout_output_one(sizing, call);
	}
	return(NULL);
}
	
output_callout_t *callout_output_rtn = (output_callout_t *)callout_output_section;

__SRCVERSION("callouts.c $Rev: 657836 $");
