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
// This function exists to make it easier for callout patchers
// to deal with the differences between 32 and 64 bit paddr_t's.
// By calling here with a pointer to the paddr, we maintain the same
// calling conventions in both cases for the assembly code and let the
// compiler sort it all out.
//

void *
callout_memory_map_indirect(unsigned size, paddr_t *phys, unsigned prot_flags) {
	return callout_memory_map(size, *phys, prot_flags);
}

__SRCVERSION("callout_memory_map_indirect.c $Rev: 657836 $");
