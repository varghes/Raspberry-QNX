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
_spi_read(resmgr_context_t *ctp, io_read_t *msg, spi_ocb_t *ocb)
{
	uint8_t		*buf;
	int			nbytes;
	int			nonblock = 0;
	int			status;
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_dev_t	*dev = drvhdl->hdl;

    if ((status = iofunc_read_verify(ctp, msg, &ocb->hdr, &nonblock)) != EOK)
        return status;

    if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE)
        return ENOSYS;

	/*
	 * Check to see if the device is locked
	 */
	if ((status =_spi_lock_check(ctp, ocb->chip, ocb)) != EOK)
		return status;

    if (msg->i.nbytes <= 0) {
        _IO_SET_READ_NBYTES(ctp, 0);
        return _RESMGR_NPARTS(0);
    }

    /* check if message buffer is too short */
    nbytes = msg->i.nbytes;
    if (nbytes > ctp->msg_max_size) {
        if (dev->buflen < nbytes) {
            dev->buflen = nbytes;
			if (dev->buf)
            	free(dev->buf);
            if (NULL == (dev->buf = malloc(dev->buflen))) {
                dev->buflen = 0;
                return ENOMEM;
            }
        }
        buf = dev->buf;
    }
	else
        buf = (uint8_t *)msg;

	buf = dev->funcs->xfer(drvhdl, _SPI_DEV_READ(ocb->chip), buf, &nbytes);

	if (nbytes > 0) {
		_IO_SET_READ_NBYTES(ctp, nbytes);
		return _RESMGR_PTR(ctp, buf, nbytes);
	}

	return EIO;
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_read.c $ $Rev: 657836 $" );
