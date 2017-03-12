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


int
interrupt_valid(int interrupt) {
	unsigned				i;
 	struct intrinfo_entry	*intr = lsp.intrinfo.p;
	int						num = lsp.intrinfo.size / sizeof(*intr);
	unsigned				vect;

	for(i = 0; i < num; ++i, ++intr) {
		vect = intr->vector_base;
		if((interrupt >= vect) && (interrupt < (vect+intr->num_vectors))) {
			return 1;
		}	
	}	
	return 0;
}

__SRCVERSION("interrupt_valid.c $Rev: 657836 $");
