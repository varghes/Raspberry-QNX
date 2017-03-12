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

/*
 * AT91SAM9xx Advanced Interrupt Controller (AIC) support.
 */

#include "startup.h"
#include <arm/bcm2835.h>

extern struct callout_rtn	interrupt_id_bcm2835_aic;
extern struct callout_rtn	interrupt_eoi_bcm2835_aic;
extern struct callout_rtn	interrupt_mask_bcm2835_aic;
extern struct callout_rtn	interrupt_unmask_bcm2835_aic;


static paddr_t aic_base = ARM_IRQCTLR_BASEOFF;
const static struct startup_intrinfo	intrs[] = {
	{	_NTO_INTR_CLASS_EXTERNAL, 	// vector base
		64,							// number of vectors
		_NTO_INTR_SPARE,			// cascade vector
		0,							// CPU vector base
		0,							// CPU vector stride
		0,							// flags

		{ INTR_GENFLAG_LOAD_SYSPAGE,	0, &interrupt_id_bcm2835_aic },
		{ INTR_GENFLAG_LOAD_SYSPAGE | INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_bcm2835_aic },
		&interrupt_mask_bcm2835_aic,	// mask   callout
		&interrupt_unmask_bcm2835_aic,	// unmask callout
		0,							// config callout
		&aic_base,
	},
};



void init_intrinfo()
{

	out32(aic_base + IRQC_DISABIRQ1_OFF,0xffffffff );
	out32(aic_base + IRQC_DISABIRQ2_OFF,0xffffffff );
    out32(aic_base + IRQC_DISABBASIC_OFF,0xffff );

	add_interrupt_array(intrs, sizeof(intrs));
}
