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
 * init_raminfo.c
 *	Tell syspage about our RAM configuration
 */
#include "startup.h"
#include <arm/omap.h>

#define MEMBASE				0x10000000
#define RESERVED_MEM_SIZE	0x00200000

void omap5912_init_raminfo() {

	unsigned long ramsize = 0;
	uintptr_t   emiff_config;
	unsigned long tmp, tmp2;

	emiff_config = startup_io_map(4,OMAP_TC_BASE + OMAP_EMIFF_SDRAM_CONFIG);

	tmp = in32(emiff_config);

	startup_io_unmap(emiff_config);

	/* bits 29:28, and 7:4 determine size. Refer to table 32 of latest OMAP 3.2 
	 * subsystem manual (SPRU749)
	 */

	tmp2 = (tmp >> 4) & 0x0f;
	tmp2 |= (tmp >> 24) & 0x30;

	switch (tmp2) {
		case 2:
		ramsize = MEG(2);
		break;

		case 0:
		ramsize = MEG(4);
		break;

		case 6:
		case 7:
		ramsize = MEG(8);
		break;

		case 4:
		case 5:
		case 11:
		ramsize = MEG(16);
		break;

		case 9:
		case 15:
		ramsize = MEG(32);
		break;

		case 13:
		ramsize = MEG(64);
		break;

		case 16:
		case 32:
		ramsize = MEG(128);
		break;

		default:
		ramsize = MEG(16);  // if all else fails, there should be at least 16M present
		break;
	}

	add_ram(MEMBASE, ramsize);

	// reserve an area of memory for DSP/link
	alloc_ram(MEMBASE + ramsize - RESERVED_MEM_SIZE, RESERVED_MEM_SIZE, 1);

    hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_MISC, "misc", 0);
    hwi_add_location( (MEMBASE + ramsize - RESERVED_MEM_SIZE), RESERVED_MEM_SIZE, 0, hwi_find_as((MEMBASE + ramsize - RESERVED_MEM_SIZE), 0));

}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/omap5912_init_raminfo.c $ $Rev: 657836 $" );
