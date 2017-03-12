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
do_adjust(struct bootargs_entry *bap, const char *old, const char *new, 
			unsigned start, char *countp) {
	unsigned 		total_len;
	unsigned 		add_len;
	unsigned 		sub_len;
	int				diff;
	char			*src;
	char			*p;
	unsigned		num = *countp;

	p = bap->args;
	while(start != 0) {
		p += strlen(p) + 1;
		--start;
	};
	while(num != 0) {
		sub_len = strlen(p) + 1;
		if((old != NULL) && (strcmp(old, p) == 0)) goto do_operation;
		p += sub_len;
		--num;
	}
	if(old == NULL) {
		// adding new one
		sub_len = 0;
		goto do_operation;
	}
	return 0;

do_operation:		
	if(new == NULL) {
		// deleting entry
		*countp -= 1;
		add_len = 0;
	} else { 
		// adding/replacing entry
		add_len = strlen(new) + 1;
		if(old == NULL) {
			// adding entry
			*countp += 1;
		}
	}
	total_len = bap->size_lo + (bap->size_hi << 8);
	diff = add_len - sub_len;
	src = p - diff;
	memmove(p, src, &bap->args[total_len - offsetof(struct bootargs_entry, args)] - src);
	memcpy(p, new, add_len);
	total_len += diff;
	bap->size_lo = total_len & 0xff;
	bap->size_hi = total_len >> 8;
	return 1;
}


int
bootstrap_arg_adjust(struct bootargs_entry *bap, const char *old, const char *new) {
	return do_adjust(bap, old, new, 0, &bap->argc);
}


int
bootstrap_env_adjust(struct bootargs_entry *bap, const char *old, const char *new) {
	return do_adjust(bap, old, new, bap->argc, &bap->envc);
}
__SRCVERSION("bootstrap_cmdline.c $Rev: 657836 $");
