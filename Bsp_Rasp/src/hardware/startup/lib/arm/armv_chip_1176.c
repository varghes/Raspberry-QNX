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

const struct armv_chip armv_chip_1176 = {
	.cpuid		= 0xb760,
	.name		= "arm1176",
	.mmu_cr_set	= ARM_MMU_CR_XP|ARM_MMU_CR_I|ARM_MMU_CR_Z,
	.mmu_cr_clr	= ARM_MMU_CR_R|ARM_MMU_CR_S,
	.cycles		= 2,
	.cache		= &armv_cache_1136,
	.power		= 0,
	.flush		= &page_flush_1136,
	.deferred	= &page_flush_deferred_1136,
	.pte		= &armv_pte_v6wb,
	.pte_wa		= 0,							// not supported by core
	.pte_wb		= &armv_pte_v6wb,
	.pte_wt		= &armv_pte_v6wt,
	.setup		= armv_setup_1136,
	.ttb_attr	= ARM_TTBR_RGN_WT,
	.pte_attr	= ARM_PTE_V6_SP_XN|ARM_PTE_WT,
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_chip_1176.c $ $Rev: 657836 $" );
