/*
 * $QNXLicenseC:
 * Copyright 2011, QNX Software Systems. 
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

/*
 * Configuration for Cortex-A15 MPCore:
 * - cache/page callouts use inner-cacheable/broadcast operations
 * - pte encodings use Shared/Write-back/Write-allocate encodings
 *
 * NOTE: we need to set bit 10 (ARM_MMU_CR_F) to enable swp instructions.
 *       These were deprecated in ARMv6 and Cortex-A15 disables them by
 *       default causing them to generate illegal instruction exceptions.
 *
 *       We also need to explicitly specify ARM_MMU_CR_XP even though that
 *       bit is always set to 1 by hardware so that arm_pte.c can detect
 *       that we are using the ARMv6 VMSA page table format.
 */
const struct armv_chip armv_chip_a15 = {
	.cpuid		= 0xc0f0,
	.name		= "Cortex A15",
	.mmu_cr_set	= ARM_MMU_CR_XP|ARM_MMU_CR_I|ARM_MMU_CR_Z|ARM_MMU_CR_F,
	.mmu_cr_clr	= 0,
	.cycles		= 2,
	.cache		= &armv_cache_a15,
	.power		= &power_v7_wfi,
	.flush		= &page_flush_a15,
	.deferred	= &page_flush_deferred_a15,
	.pte		= &armv_pte_v7mp,
	.pte_wa		= &armv_pte_v7mp,
	.pte_wb		= 0,				// not supported
	.pte_wt		= 0,				// not supported
	.setup		= armv_setup_a15,
	.ttb_attr	= ARM_TTBR_IRGN_WA|ARM_TTBR_RGN_WA|ARM_TTBR_S,
	.pte_attr	= ARM_PTE_V6_SP_XN|ARM_PTE_V6_WA|ARM_PTE_V6_S,
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_chip_a15.c $ $Rev: 657836 $" );
