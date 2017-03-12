/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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




#include "proto.h"

spi_ocb_t *
_spi_ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr)
{
	spi_ocb_t	*ocb;
//	SPIDEV		*drvhdl = (SPIDEV *)attr;
//	spi_dev_t	*dev = (spi_dev_t *)drvhdl->hdl;

	if (NULL == (ocb = calloc(1, sizeof(*ocb))))
		return NULL;

	ocb->chip = SPI_DEV_ID_NONE;

	return ocb;
}

void
_spi_ocb_free(IOFUNC_OCB_T *ocb)
{
	free(ocb);
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_ocb.c $ $Rev: 657836 $" );
