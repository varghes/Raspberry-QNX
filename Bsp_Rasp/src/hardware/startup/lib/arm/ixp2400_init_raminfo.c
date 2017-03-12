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
 *  ixp2400_init_raminfo.c
 *	Tell syspage about our RAM configuration of IXP2400 based systems
 */
#include "startup.h"
#include "arm/ixp2400.h"


void
ixp2400_init_raminfo(void) {
	uint32_t		du_control;
	uint32_t		size;

	

	du_control = *(uint32_t *)(IXP2400_DDRSDRAM_BASE + IXP2400_DDRSDRAM_DU_CONTROL);

	switch((du_control >> IXP2400_DDRSDRAM_NUMROW_COL_SHIFT) & IXP2400_DDRSDRAM_NUMROW_COL_MASK){
			case 0: if ((du_control >> IXP2400_DDRSDRAM_NUM_SIDES_SHIFT) & 0x1)  				// size 0
						size = 0x8000000;
					else 
						size = 0x4000000;
					break;
			case 1: size = 0x4000000; break;		
			case 2: size = 0x8000000; break;
			case 3: if ((du_control >> IXP2400_DDRSDRAM_NUM_SIDES_SHIFT) & 0x1)  				// size 0
						size = 0x20000000;
					else 
						size = 0x10000000;
					break;
			case 4: size = 0x20000000;break;
			case 5: size = 0x40000000;break;
			case 6: size = 0x40000000;break;
			default: size = 0x2000000;break;
	}
	add_ram(0,size);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/ixp2400_init_raminfo.c $ $Rev: 657836 $" );
