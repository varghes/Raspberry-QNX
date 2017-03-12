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

const struct arm_cache_config	sa1100_dcache_config = {
	CACHE_FLAG_DATA|CACHE_FLAG_VIRTUAL|CACHE_FLAG_WRITEBACK,
	32,
	256
};

const struct arm_cache_config	sa1100_icache_config = {
	CACHE_FLAG_INSTR|CACHE_FLAG_VIRTUAL,
	32,
	512
};

const struct armv_cache	armv_cache_sa1100 = {
	&sa1100_dcache_config,	// no cache type register - use static values
	&cache_sa_d,
	&sa1100_icache_config,	// no cache type register - use static values
	&cache_sa_i
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_cache_sa1100.c $ $Rev: 657836 $" );
