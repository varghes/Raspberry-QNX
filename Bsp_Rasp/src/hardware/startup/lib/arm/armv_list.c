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

/*
 * List of core configurations that init_cpuinfo() will attempt to detect.
 */

const struct armv_chip *armv_list[] = {
	&armv_chip_720,

	&armv_chip_920,
	&armv_chip_922,
	&armv_chip_925,
	&armv_chip_926,

	&armv_chip_1020,
	&armv_chip_1022,

	&armv_chip_sa1100,
	&armv_chip_sa1110,

	&armv_chip_ixp1200,

	&armv_chip_80200,

	&armv_chip_pxa250a,	// stepping A0/A1
	&armv_chip_pxa250b,	// stepping B0/B1/B2
	&armv_chip_pxa255,	// pxa250 stepping C0

	&armv_chip_pxa210b,	// stepping B0/B1/B2
	&armv_chip_pxa210c,	// stepping C0

	&armv_chip_pxa270,

	&armv_chip_ixp2400,
	&armv_chip_ixp2800,

	&armv_chip_ixp23xx,

	&armv_chip_ixp425,
	&armv_chip_ixp465,

	&armv_chip_1136,
	&armv_chip_1176,
	&armv_chip_mp11,

	&armv_chip_a8,

	&armv_chip_sheeva,

	&armv_chip_a9,

	&armv_chip_a15,

	0
};


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/armv_list.c $ $Rev: 657836 $" );
