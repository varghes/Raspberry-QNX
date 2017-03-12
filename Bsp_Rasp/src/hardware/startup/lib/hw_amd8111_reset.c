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
#include <hw/inout.h>

//
// Make sure the ethernet isn't generating any interrupts or receiving
// data.
//

#define AMD8111_CMD0			0x48

void
amd8111_reset(paddr_t base, int mem_mapped) {
	uintptr_t	io_base;

	io_base = startup_io_map(0x100, base);

	out32(io_base + AMD8111_CMD0, 0);

	startup_io_unmap(io_base);
}

__SRCVERSION("hw_amd8111_reset.c $Rev: 657836 $");
