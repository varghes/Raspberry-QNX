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

void *
at91sam9xx_init(int argc, char *argv[])
{
	at91sam9xx_dev_t      *dev;

	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) 
	{
		perror("ThreadCtl");
		return NULL;
	}

	dev = malloc(sizeof(at91sam9xx_dev_t));
	if (!dev)
	{
		return NULL;		
	}

	if (-1 == at91sam9xx_options(dev, argc, argv))
	{
		goto fail;		
	}

	dev->regbase = mmap_device_io(dev->reglen, dev->physbase);
	if (dev->regbase == (uintptr_t)MAP_FAILED) 
	{
		perror("mmap_device_io");
		goto fail;
	}

	/* Reset I2C module */
	out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, CTRREG_SWRST);

	delay (10);

	/*Reading RHR to reset RXRDY */
	in32(dev->regbase + AT91SAM9XX_I2C_RHREG_OFF); 

	if (at91sam9xx_attach_intr(dev))
		goto fail;

	/* Set clock prescaler using default baud*/
	at91sam9xx_set_bus_speed(dev, 100000, NULL);  
	dev->own_addr=0;	

	/* Disable interrupts */
	out32(dev->regbase + AT91SAM9XX_I2C_IDREG_OFF, 0xFFFFFFFF);

	/* Disable slave mode */
	out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, CTRREG_SVDIS);

	/* Enable master mode*/
	out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, CTRREG_MSEN);
	
	return dev;

fail:
	free(dev);
	return NULL;
}

static const struct sigevent *at91sam9xx_intr(void *hdl, int id)
{
	uint32_t 	sts;
	at91sam9xx_dev_t *dev = (at91sam9xx_dev_t *)hdl;

	/*
	 * Get status
	 */
	dev->status = in32(dev->regbase + AT91SAM9XX_I2C_STSREG_OFF);
	sts = dev->status & in32(dev->regbase + AT91SAM9XX_I2C_IMREG_OFF);

	if (sts & STSREG_NACK)
	{
		/* Disable interrupts */
		out32(dev->regbase + AT91SAM9XX_I2C_IDREG_OFF, 0xFFFFFFFF);

		dev->txrx = AT91SAM_I2C_IDLE;
		return  (&dev->intrevent);
	}
	else if ((dev->txrx == AT91SAM_I2C_RX) && (sts & STSREG_RXRDY))
	{
		if (dev->tot_len==1)
		{
			dev->buf[dev->cur_len] = (int8_t)in32(dev->regbase 
					+ AT91SAM9XX_I2C_RHREG_OFF);
			return (NULL);
		}

		if ((dev->cur_len < dev->tot_len))
		{
			dev->buf[dev->cur_len] = (int8_t)in32(dev->regbase 
					+ AT91SAM9XX_I2C_RHREG_OFF);
		}
		dev->cur_len++;
		if (dev->cur_len >= dev->tot_len - 1) 
		{
			/* Issue stop condition one byte before the last one. */
			out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, 
					dev->stop ? CTRREG_STOP:0 );
		}
	} 
	else if ((dev->txrx == AT91SAM_I2C_TX) && (sts & STSREG_TXRDY)) 
	{
		if (dev->cur_len < dev->tot_len) 
		{
			out32(dev->regbase + AT91SAM9XX_I2C_THREG_OFF, 
					dev->buf[dev->cur_len++]);
		}
		else 
		{
			/* Send stop condition */
			out32(dev->regbase + AT91SAM9XX_I2C_CTRREG_OFF, CTRREG_STOP);
			
			/* Disable TXRDY interrupt */
			out32(dev->regbase + AT91SAM9XX_I2C_IDREG_OFF, STSREG_TXRDY);
		}
	}
	else {
		/* Disable interrupts for either TXCOMP or error interrupt */
		out32(dev->regbase + AT91SAM9XX_I2C_IDREG_OFF, 0xFFFFFFFF);

		dev->txrx = AT91SAM_I2C_IDLE;
		return  (&dev->intrevent);
	}

	return (NULL);
}

int at91sam9xx_attach_intr(at91sam9xx_dev_t *dev)
{
	/* Disable interrupts */
	out32(dev->regbase + AT91SAM9XX_I2C_IDREG_OFF, 0xFFFFFFFF);

	/* Initialize interrupt handler */
	SIGEV_INTR_INIT(&dev->intrevent);

	dev->iid = InterruptAttach(dev->intr, at91sam9xx_intr, 
			dev, 0, _NTO_INTR_FLAGS_TRK_MSK);

	if (dev->iid == -1) 
	{
		perror("InterruptAttach");
		return -1;
	}

	return 0;
}

