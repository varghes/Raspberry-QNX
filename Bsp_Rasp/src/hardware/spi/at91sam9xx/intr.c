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

// I am not interested in adding SPI Interrupt support , but I certainly willing to configure some
// GPIO pin for this purpose , this is helpfull for MCP2515 ,enc28j60


#include "at91sam9xx_spi.h"



/*
 * We use the same buffer for transmit and receive
 * For exchange, that's exactly what we wanted
 * For Read, it doesn't matter what we write to SPI, so we are OK.
 * For transmit, the receive data is put at the buffer we just transmitted, we are still OK.
 */

static const struct sigevent *spi_intr(void *area, int id)
{
	at91sam9xx_spi_t	*dev = area;
	uintptr_t	base = dev->vbase;

	uint32_t	data;

	// clear source
	in32(  base + AT91_SPI_STATREG);
    
	//empty RX buffer and fill tx buffer(if required)
	data = in32(base + AT91_SPI_RXDATA) & 0xFFFF;

	if (dev->rlen < dev->xlen) {
		switch (dev->dlen) {
			case 1:
				dev->pbuf[dev->rlen++] = data;

				/*
				 * More to transmit?
				 */
				if (dev->tlen < dev->xlen)
				out32(base + AT91_SPI_TXDATA, dev->pbuf[dev->tlen++] | SPI_TXDATA_PCS(dev->curr_cs));
				break;
			case 2:
				*(uint16_t *)(&dev->pbuf[dev->rlen]) = data;
				dev->rlen += 2;

				/*
				 * More to transmit?
				 */
				if (dev->tlen < dev->xlen) {
				out32(base + AT91_SPI_TXDATA, (*(uint16_t *)(&dev->pbuf[dev->tlen])) | SPI_TXDATA_PCS(dev->curr_cs));
				dev->tlen += 2;
				}
				break;
		}
	}

	if (dev->rlen >= dev->xlen) {
		out32(base + AT91_SPI_INTDISREG, SPI_INTREG_RDRF);	/* Disable interrupt */
		return (&dev->spievent);
	}

	return 0;
}

int at91sam9xx_attach_intr(at91sam9xx_spi_t *dev)
{
	if ((dev->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
		return -1;

	if ((dev->coid = ConnectAttach(0, 0, dev->chid, _NTO_SIDE_CHANNEL, 0)) == -1) 
		goto fail0;

	dev->spievent.sigev_notify   = SIGEV_PULSE;
	dev->spievent.sigev_coid     = dev->coid;
	dev->spievent.sigev_code     = AT91_SPI_EVENT;
	dev->spievent.sigev_priority = AT91_SPI_PRIORITY;

	/*
	 * Attach SPI interrupt
	 */
	dev->iid = InterruptAttach(dev->irq, spi_intr, dev, 0, _NTO_INTR_FLAGS_TRK_MSK);

	if (dev->iid != -1)
		return 0;

	ConnectDetach(dev->coid);
fail0:
	ChannelDestroy(dev->chid);

	return -1;
}
