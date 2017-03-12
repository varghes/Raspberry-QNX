/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, 2009 QNX Software Systems.  
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




#include "at91sam9xx_spi.h"


int at91sam9xx_cfg(void *hdl, spi_cfg_t *cfg)
{
	at91sam9xx_spi_t	*dev = hdl;
	uint32_t	ctrl, i, drate;

	if (cfg == NULL)
		return 0;

	ctrl = cfg->mode & SPI_MODE_CHAR_LEN_MASK;
	if (ctrl > 16 || ctrl < 8)
		return 0;

    ctrl = (ctrl-8) << SPI_CHIPSEL_BITS_POS;

    for(i=1; i < 256; i++) {
        drate = dev->clock / i;
        
        // Assign the datarate if calculated <= desired...
        // OR assign lowest possible rate last time through the loop
		if (drate <= cfg->clock_rate || i==255) {
			cfg->clock_rate = drate;
			ctrl |= i << SPI_CHIPSEL_SCBR_POS;
			break;
		}
    }
	if (cfg->mode & SPI_MODE_CKPOL_HIGH)
		ctrl |= SPI_CHIPSEL_CPOL;	/* Active high polarity */

	if (cfg->mode & SPI_MODE_CKPHASE_HALF)
		ctrl |= SPI_CHIPSEL_NCPHA;	/* Phase 1 */

	ctrl |= (SPI_CHIPSEL_CSAAT | 
	         DLYBCT(dev->clock, AT45_DLYBCT) | 
	         DLYBS(dev->clock, AT45_DLYBS));

	return ctrl;
}

