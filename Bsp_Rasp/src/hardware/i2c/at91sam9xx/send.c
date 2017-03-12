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
at91sam9xx_send(void *hdl, void *buf, unsigned int len, unsigned int stop){
	at91sam9xx_dev_t   *dev = hdl;

	if (len <= 0)
		return I2C_STATUS_DONE;

	if (dev->slave_addr & INVALID_ADDR){
		slogf(_SLOG_SETCODE(_SLOGC_TEST, 0) , _SLOG_ERROR, "Invalid address  \n");
		return I2C_STATUS_ERROR;
	}
	if (dev->slave_addr_fmt == I2C_ADDRFMT_7BIT)
	{
		out32(dev->regbase + AT91SAM9XX_I2C_MMREG_OFF,(dev->slave_addr << 16));
	}
	else
	{
		slogf(_SLOG_SETCODE(_SLOGC_TEST, 0) , _SLOG_ERROR, 
				"I2C 10 Bit addressing not supported  \n");
		return I2C_STATUS_ERROR;
	}

	/* For interrupt service routine */
	dev->tot_len = len;
	dev->cur_len = 0;
	dev->buf     = buf;
	dev->txrx    = AT91SAM_I2C_TX;
	dev->stop    = stop;
	dev->status  = 0;

	/* Enable interrupts */
	out32(dev->regbase + AT91SAM9XX_I2C_IEREG_OFF, 
			STSREG_NACK | STSREG_TXRDY | STSREG_TXCOMP);

	/* Start transfer */
	out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, CTRREG_START);

	return at91sam9xx_wait_complete (dev);	
}
