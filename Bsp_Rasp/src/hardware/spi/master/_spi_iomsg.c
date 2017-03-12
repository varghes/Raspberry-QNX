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
_spi_iomsg(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb)
{
	spi_msg_t	*spimsg = (spi_msg_t *)msg;

	if (((spimsg->msg_hdr.i.combine_len < sizeof(spi_msg_t))) || ((spimsg->msg_hdr.i.mgrid != _IOMGR_SPI)))
        return ENOSYS;
	if ((spimsg->msg_hdr.i.subtype >= _SPI_IOMSG_READ) && (spimsg->msg_hdr.i.subtype <= _SPI_IOMSG_DMAXCHANGE)) {
		int err;

		if ((err =_spi_lock_check(ctp, spimsg->device, ocb)) != EOK)
			return err;

		switch (spimsg->msg_hdr.i.subtype) {
			case _SPI_IOMSG_READ:
				return _spi_iomsg_read(ctp, msg, ocb);
				break;
			case _SPI_IOMSG_WRITE:
				return _spi_iomsg_write(ctp, msg, ocb);
				break;
			case _SPI_IOMSG_EXCHANGE:
				return _spi_iomsg_xchange(ctp, msg, ocb);
				break;
			case _SPI_IOMSG_CMDREAD:
				return _spi_iomsg_cmdread(ctp, msg, ocb);
				break;
			case _SPI_IOMSG_DMAXCHANGE:
				return _spi_iomsg_dmaxchange(ctp, msg, ocb);
				break;
		}
	}

	return EINVAL;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_iomsg.c $ $Rev: 657836 $" );
