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
// This code is hardware independant and should not have to be
// changed by end users.
//

const static uint8_t offsets[] = {
	offsetof(struct intrinfo_entry, id.rtn),
	offsetof(struct intrinfo_entry, eoi.rtn),
	offsetof(struct intrinfo_entry, mask),
	offsetof(struct intrinfo_entry, unmask),
	offsetof(struct intrinfo_entry, config),
};

static output_callout_t	*intr_prev_output_rtn;

static output_callout_t *
callout_output_intr(int sizing) {
	unsigned	i;
	unsigned	nelts;

	nelts = lsp.intrinfo.size / sizeof(*lsp.intrinfo.p);
	for(i = 0; i < nelts; ++i) {
		uint8_t		*base = (uint8_t *)&lsp.intrinfo.p[i];
		unsigned	j;
		
		for(j = 0; j < NUM_ELTS(offsets); ++j) {
			void	(**rtn)(void) = (void (**)(void))(base + offsets[j]);
			
			callout_output_one(sizing, rtn);
		}
	}
	return(intr_prev_output_rtn);
}


struct intrinfo_entry *
add_interrupt(const struct startup_intrinfo *startup_intr) {
	struct intrinfo_entry	*intr;
	unsigned				i;

	if(intr_prev_output_rtn == NULL) {
		//Hook into the sizing/writing list
		intr_prev_output_rtn = callout_output_rtn;
		callout_output_rtn = (output_callout_t *)callout_output_intr;
	}

	intr = grow_syspage_section(&lsp.intrinfo, sizeof(*intr));
	//Point at newly allocated entry
	intr = (void *)((uint8_t *)intr + lsp.intrinfo.size - sizeof(*intr));
	*intr = *(struct intrinfo_entry *)startup_intr;
	if(startup_intr->id.rtn != NULL) {
		intr->id.size = startup_intr->id.rtn->rtn_size;
	}
	if(startup_intr->eoi.rtn != NULL) {
		intr->eoi.size = startup_intr->eoi.rtn->rtn_size;
	}
	
	for(i = 0; i < NUM_ELTS(offsets); ++i) {
		void	(**rtn)(void) = (void (**)(void))((uintptr_t)intr + offsets[i]);
		
		callout_register_data(rtn, startup_intr->patch_data);
	}
	return(intr);
}

void
add_interrupt_array(const struct startup_intrinfo *intrs, unsigned size) {
	unsigned	i;
	unsigned	incr;

	incr = sizeof(*intrs);
	//
	//Backwards compatability stuff - can be removed once
	//all the startups have been recompiled with the new startup_intrinfo
	//structure
	//
	if((size % sizeof(*intrs)) != 0) {
		incr -= sizeof(void *);
	}
	for(i = size / incr; i > 0; --i) {
		add_interrupt(intrs);
		intrs = (void *)((uintptr_t)intrs + incr);
	}
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/add_interrupt.c $ $Rev: 657836 $" );
