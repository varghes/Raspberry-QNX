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

const struct armv_chip armv_chip_a8 = {
	.cpuid		= 0xc080,
	.name		= "Cortex A8",
	.mmu_cr_set	= ARM_MMU_CR_XP|ARM_MMU_CR_I|ARM_MMU_CR_Z,
	.mmu_cr_clr	= 0,
	.cycles		= 2,
	.cache		= &armv_cache_a8,
	.power		= &power_v7_wfi,
	.flush		= &page_flush_a8,
	.deferred	= &page_flush_deferred_a8,
	.pte		= &armv_pte_v7wa,
	.pte_wa		= &armv_pte_v7wa,
	.pte_wb		= &armv_pte_v7wb,
	.pte_wt		= &armv_pte_v7wt,
	.setup		= armv_setup_a8,
	.ttb_attr	= ARM_TTBR_RGN_WT,
	.pte_attr	= ARM_PTE_V6_SP_XN|ARM_PTE_WT,
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_chip_a8.c $ $Rev: 657836 $" );
