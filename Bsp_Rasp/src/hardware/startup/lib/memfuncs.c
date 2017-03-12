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





/*
 * memfuncs.c
 *	Convert between virtual and physical addressing
 */
#include <string.h>
#include "startup.h"


void
copy_memory(paddr32_t dst, paddr32_t src, size_t len) {
	uint8_t		*d;
	uint8_t		*s;
	unsigned	max;
	unsigned	amount;

	max = (lsp.mdriver.size > 0) ? mdriver_max : len;
	for( ;; ) {
		mdriver_check();
		amount = (len > max) ? max : len;

		// We make the assumption that the destination is going to
		// be in the one-to-one mapping area.
		d = MAKE_1TO1_PTR(dst);
		s = startup_memory_map(amount, src, PROT_READ);
		memmove(d, s, amount);
		startup_memory_unmap(s);
		len -= amount;
		if(len == 0) break;
		src += amount;
		dst += amount;
	}	
}

__SRCVERSION("memfuncs.c $Rev: 657836 $");
