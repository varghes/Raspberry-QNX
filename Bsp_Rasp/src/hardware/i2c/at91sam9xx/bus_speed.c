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


int
at91sam9xx_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed)
{
	at91sam9xx_dev_t	*dev = hdl;
	unsigned int		i2c_div, i2c_freq_val;
	unsigned int 		ckdiv = 0;
	unsigned char		settle = 0;

	if (speed > 400000) {
		errno = EINVAL;
		return -1;
	}

	dev->speed = speed;

	while(!settle){
		i2c_div = ((dev->clk / (2 * speed)) - 3) / (1 << ckdiv);
		if(i2c_div <= 255)
			settle = 1; 
		else
			ckdiv++;
	}
	if( ckdiv > 7 )
	{
		errno = EINVAL;
		return -1;
	}

	i2c_freq_val = (ckdiv << 16) | (i2c_div << 8) | i2c_div; 

	out32( dev->regbase + AT91SAM9XX_I2C_CLOCKGEN_OFF, i2c_freq_val );

	return 0;
}
