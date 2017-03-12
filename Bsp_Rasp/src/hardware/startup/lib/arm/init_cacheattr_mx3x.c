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

#define iMX3x_L2CC_CTRL_REG				(*(volatile uint32_t*)0x30000100)
#define iMX3x_L2CC_ACR_REG				(*(volatile uint32_t*)0x30000104)
#define iMX3x_L2CC_INVAL_BY_WAY_REG		(*(volatile uint32_t*)0x3000077c)
#define iMX3x_L2CC_DEBUG_CTRL_REG		(*(volatile uint32_t*)0x30000f40)

//
// Initialize system_icache_idx & system_dcache_idx variables with
// information on board level system caches. Caches implemented
// on the CPU itself will dealt with in init_cpuinfo().
//
void
init_cacheattr_mx3x(int enable)
{
	/* Disable L2-Cache */
	iMX3x_L2CC_CTRL_REG = 0;

	/* Configure Auxiliary Control Register (ACR) */
	iMX3x_L2CC_ACR_REG = 0x3001b;

	/* Invalidate Cache */
	iMX3x_L2CC_INVAL_BY_WAY_REG = 0xff;
	while (iMX3x_L2CC_INVAL_BY_WAY_REG)
		;

	if (enable) {
		/*
		 * FIXME: until we can ensure coherency with page table walks we
		 *        must set the L2 to be write-through.
		 *
		 *        callout_cache_l2_mx3x.S assumes a write-through cache, and
		 *        will need to be updated if this code configures the cache
		 *        for write-back.
		 */
		iMX3x_L2CC_DEBUG_CTRL_REG |= 0x2;

		/* Enable Cache */
		iMX3x_L2CC_CTRL_REG = 1;

		/*
		 * Add the L2 cache.
		 * FIXME: should have CACHE_FLAG_CTRL_PHYS but we always flush the whole
		 *        cache so it doesn't matter.
		 */
		system_icache_idx = system_dcache_idx = add_cache(CACHE_LIST_END,
														  CACHE_FLAG_UNIFIED,
														  32,
														  (128*1024)/32,
														  &cache_imx3x_l2);
	}
	if (debug_flag) {
		kprintf("L2 cache %s\n", enable ? "enabled" : "disabled");
	}
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/init_cacheattr_mx3x.c $ $Rev: 657836 $" );
