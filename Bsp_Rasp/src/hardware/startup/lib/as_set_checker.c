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

static output_callout_t	*as_prev_output_rtn;

typedef int (as_rtn)(struct syspage_entry *, _uint64 *, _uint64 *, size_t, size_t);

static output_callout_t *
callout_output_as(int sizing) {
	unsigned	i;
	unsigned	num;

	num = lsp.asinfo.size / sizeof(*lsp.asinfo.p);
	for(i = 0; i < num; ++i) {
		struct asinfo_entry	*as = &lsp.asinfo.p[i];

		callout_output_one(sizing, &as->alloc_checker);
		if(!sizing) {
			while(as->attr & AS_ATTR_CONTINUED) {
				as[1].alloc_checker = as[0].alloc_checker;
				++as;
			}
		}
	}
	return(as_prev_output_rtn);
}

void
as_set_checker(unsigned off, const struct callout_rtn *rtn) {
	struct asinfo_entry	*as = as_off2info(off);

	if(as_prev_output_rtn == NULL) {
		//Hook into the sizing/writing list
		as_prev_output_rtn = callout_output_rtn;
		callout_output_rtn = (output_callout_t *)callout_output_as;
	}
	as->alloc_checker = (as_rtn *)rtn;
}

__SRCVERSION("as_set_checker.c $Rev: 657836 $");
