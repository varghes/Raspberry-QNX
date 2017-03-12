/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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
 * ARMv7 page table entries for MPcore:
 * - use TEX/C/B=001/1/1 write-back/write-allocate
 * - use shared access (set S bit)
 * - user mappings use ASID (set nG bit)
 */
const struct armv_pte	armv_pte_v7mp = {
	ARM_PTE_SP | ARM_PTE_CB | ARM_PTE_V6_SP_TEX(1) | ARM_PTE_V6_S | ARM_PTE_V6_AP_URO | ARM_PTE_V6_nG,	// upte_ro
	ARM_PTE_SP | ARM_PTE_CB | ARM_PTE_V6_SP_TEX(1) | ARM_PTE_V6_S | ARM_PTE_V6_AP_URW | ARM_PTE_V6_nG,	// upte_rw
	ARM_PTE_SP | ARM_PTE_CB | ARM_PTE_V6_SP_TEX(1) | ARM_PTE_V6_S | ARM_PTE_V6_AP_KRO,									// kpte_ro
	ARM_PTE_SP | ARM_PTE_CB | ARM_PTE_V6_SP_TEX(1) | ARM_PTE_V6_S | ARM_PTE_V6_AP_KRW,									// kpte_rw
	ARM_PTE_CB | ARM_PTE_V6_SP_TEX_MASK,					// mask_nc
	ARM_PTP_V6_L2,											// l1_pgtable
	ARM_PTP_V6_SC | ARM_PTP_V6_AP_KRO | ARM_PTP_V6_S,		// kscn_ro
	ARM_PTP_V6_SC | ARM_PTP_V6_AP_KRW | ARM_PTP_V6_S,		// kscn_rw
	ARM_PTP_WB | ARM_PTP_V6_TEX(1)							// kscn_cb
};


__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/trunk/hardware/startup/lib/arm/armv_pte_v7mp.c $ $Rev: 217585 $" );
