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

const struct armv_chip armv_chip_ixp23xx = {
	.cpuid		= 0x6200,
	.name		= "ixp23xx",
	.mmu_cr_set	= ARM_MMU_CR_S|ARM_MMU_CR_I|ARM_MMU_CR_L2,
	.mmu_cr_clr	= ARM_MMU_CR_Z,			// cleared due to chip errata
	.cycles		= 2,
	.cache		= &armv_cache_xsc3,
	.power		= 0,
	.flush		= &page_flush_xsc3,
	.deferred	= &page_flush_deferred_xsc3,
	.pte		= &armv_pte_xsc3,
	.pte_wa		= 0,					// not supported see armv_pte_ixp2xxx.c
	.pte_wb		= &armv_pte_xsc3,
	.pte_wt		= 0,					// not supported see armv_pte_ixp2xxx.c
	.setup		= armv_setup_xscale,
	.ttb_attr	= 0,
	.pte_attr	= 0,
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_chip_23xx.c $ $Rev: 657836 $" );
