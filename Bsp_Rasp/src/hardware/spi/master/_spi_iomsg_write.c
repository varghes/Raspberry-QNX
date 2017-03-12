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
_spi_iomsg_write(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb)
{
	spi_msg_t	*spimsg = (spi_msg_t *)msg;
    uint8_t		*buf;
    int			nbytes, msglen, status;
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_dev_t	*dev = drvhdl->hdl;
	uint16_t	chip = spimsg->device & SPI_DEV_ID_MASK;

	/* check if message buffer is too short */
	nbytes = spimsg->xlen;

    if (nbytes <= 0) {
        _IO_SET_WRITE_NBYTES(ctp, 0);
        return _RESMGR_NPARTS(0);
    }

	msglen = nbytes + sizeof(spi_msg_t);

	if (msglen > ctp->msg_max_size) {
		if (dev->buflen < msglen) {
			dev->buflen = msglen;
			if (dev->buf)
				free(dev->buf);
			if ((dev->buf = malloc(dev->buflen)) == NULL) {
				dev->buflen = 0;
				return ENOMEM;
			}
		}

		status = resmgr_msgread(ctp, dev->buf, msglen, 0);
		if (status < 0)
			return errno;
		if (status < msglen)
			return EFAULT;

		buf = dev->buf;
	}
	else
		buf = (uint8_t *)msg;

	if (chip == SPI_DEV_ID_NONE)
		chip = ocb->chip;

	buf = dev->funcs->xfer(drvhdl, _SPI_DEV_WRITE(chip), buf + sizeof(spi_msg_t), &nbytes);

	if (nbytes == 0)
		return EAGAIN;

	if (nbytes > 0) {
		_IO_SET_WRITE_NBYTES(ctp, nbytes);
		return _RESMGR_NPARTS(0);
	}

	return EIO;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_iomsg_write.c $ $Rev: 657836 $" );
