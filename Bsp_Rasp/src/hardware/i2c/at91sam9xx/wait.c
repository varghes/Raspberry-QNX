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

static i2c_status_t
at91sam9xx_check_status(at91sam9xx_dev_t *dev)
{
	if (dev->status & STSREG_NACK) {
		slogf (0, _SLOG_ERROR, "%s: NACK\n", __func__);
		return I2C_STATUS_NACK;
	}

	if (dev->status & STSREG_TXCOMP) {
		return I2C_STATUS_DONE;
	}

	return I2C_STATUS_ERROR;
}

i2c_status_t
at91sam9xx_wait_complete(at91sam9xx_dev_t *dev)
{
	uint64_t	ntime;
	int		interr = EOK;

	ntime = (uint64_t) ( (uint64_t)(10000000) 
			* (uint64_t)(10000000)/(uint64_t)dev->speed );

	TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_INTR, NULL, &ntime, NULL);
	interr = InterruptWait(0, NULL);
	if (interr == ETIMEDOUT) {
		slogf (0, _SLOG_ERROR, "%s: Timeout, total data %d sent data %d\n", 
				__func__, dev->tot_len, dev->cur_len);
		return I2C_STATUS_BUSY;
	}

	return at91sam9xx_check_status(dev);
}

