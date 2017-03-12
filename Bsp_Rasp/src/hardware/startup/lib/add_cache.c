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

static output_callout_t	*cache_prev_output_rtn;

static output_callout_t *
callout_output_cache(int sizing) {
	unsigned	i;
	unsigned	num_cache;

	num_cache = lsp.cacheattr.size / sizeof(*lsp.cacheattr.p);
	for(i = 0; i < num_cache; ++i) {
		struct cacheattr_entry	*cache = &lsp.cacheattr.p[i];

		callout_output_one(sizing, &cache->control);
	}
	return(cache_prev_output_rtn);
}

int
add_cache_ways(int next, unsigned flags, unsigned line_size, unsigned num_lines,
			unsigned ways, const struct callout_rtn *rtn) {
	struct cacheattr_entry *cache  = lsp.cacheattr.p;
	int						num = lsp.cacheattr.size / sizeof(*cache);
	int						i;

	if(cache_prev_output_rtn == NULL) {
		//Hook into the sizing/writing list
		cache_prev_output_rtn = callout_output_rtn;
		callout_output_rtn = (output_callout_t *)callout_output_cache;
	}
	//Look for an already existing entry that matches
	for(i = 0; i < num; ++i, ++cache) {
		if( cache->next == next
		 && cache->flags == flags
		 && cache->line_size == line_size
		 && cache->num_lines == num_lines
		 && cache->control == (cache_rtn *)rtn) return(i);
	}
	//Have to add a new entry
	cache = grow_syspage_section(&lsp.cacheattr, sizeof(*cache));
	cache += i;
	cache->next = next;
	cache->flags = flags;
	cache->line_size = line_size;
	cache->num_lines = num_lines;
	cache->ways = ways;
	cache->control = (cache_rtn *)rtn;
	return(i);
}

int
add_cache(int next, unsigned flags, unsigned line_size, unsigned num_lines,
			const struct callout_rtn *rtn) {
	return add_cache_ways(next, flags, line_size, num_lines, 0, rtn);
}



__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/add_cache.c $ $Rev: 657836 $" );
