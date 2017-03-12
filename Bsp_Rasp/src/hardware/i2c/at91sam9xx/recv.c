/*
 * $QNXLicenseC: 
 * Copyright 2008, 2009, QNX Software Systems.  
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

i2c_status_t
at91sam9xx_recv(void *hdl, void *buf, unsigned int len, unsigned int stop)
{
	at91sam9xx_dev_t  *dev = hdl;
	uint32_t          cr;

	if (len <= 0) 
		return I2C_STATUS_DONE;

	if (dev->slave_addr & INVALID_ADDR)
		return I2C_STATUS_ERROR;

	out32(dev->regbase + AT91SAM9XX_I2C_MMREG_OFF, 
		in32(dev->regbase + AT91SAM9XX_I2C_MMREG_OFF) | MMREG_MREAD);

	if(!dev->own_addr)
	{
		/* send slave address */
		if (dev->slave_addr_fmt == I2C_ADDRFMT_7BIT) 
		{
			out32(dev->regbase + AT91SAM9XX_I2C_MMREG_OFF, 
					(MMREG_MREAD | ((dev->slave_addr) << 16)));
		}
		else 
		{
			slogf(_SLOG_SETCODE(_SLOGC_TEST, 0) , 
					_SLOG_ERROR, "I2C 10 Bit addressing   \n" );	
		}

	}

	/* For interrupt service routine */
	dev->tot_len = len;
	dev->cur_len = 0;
	dev->buf     = buf;
	dev->txrx    = AT91SAM_I2C_RX;
	dev->stop    = stop;
	dev->status  = 0;

	/* Start transfer */
	cr = CTRREG_START | ( ((len == 1)&&(dev->stop))? CTRREG_STOP:0 );
	out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, cr);

	/* Enable interrupt */	
	out32(dev->regbase + AT91SAM9XX_I2C_IEREG_OFF, 
			STSREG_NACK | STSREG_RXRDY | STSREG_TXCOMP);
	
	return (at91sam9xx_wait_complete ((at91sam9xx_dev_t *) dev));
}
