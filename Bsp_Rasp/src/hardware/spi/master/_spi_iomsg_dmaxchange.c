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

int
_spi_iomsg_dmaxchange(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb)
{
	spi_msg_t		*spimsg = (spi_msg_t *)msg;
    spi_dma_paddr_t	*paddr;
    int				nbytes;
	SPIDEV			*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_dev_t		*dev = drvhdl->hdl;
	uint32_t		chip = spimsg->device & SPI_DEV_ID_MASK;

	if (spimsg->msg_hdr.i.combine_len < sizeof(spi_msg_t) + sizeof(spi_dma_paddr_t))
        return ENOSYS;

	if (chip == SPI_DEV_ID_NONE)
		chip = ocb->chip;

	/* check if message buffer is too short */
	nbytes = spimsg->xlen;

	if (dev->funcs->dma_xfer == NULL)
		return ENOTSUP;

	paddr = (spi_dma_paddr_t *)(spimsg + 1);

	if (nbytes <= 0) {
		if (paddr->wpaddr != 0) {
        	_IO_SET_WRITE_NBYTES(ctp, 0);
        	return _RESMGR_NPARTS(0);
		}
		else if (paddr->rpaddr != 0) {
    	    _IO_SET_READ_NBYTES(ctp, 0);
    	    return _RESMGR_NPARTS(0);
		}
	}

	nbytes = dev->funcs->dma_xfer(drvhdl, chip, paddr, nbytes);

	if (nbytes == 0)
		return EAGAIN;

	if (nbytes > 0) {
		_IO_SET_WRITE_NBYTES(ctp, nbytes);
		return _RESMGR_NPARTS(0);
	}

	return EIO;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_iomsg_dmaxchange.c $ $Rev: 657836 $" );
