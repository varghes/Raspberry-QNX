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

/*
 * Need mutex protect if use multithread
 */

spi_lock_t *
_spi_islock(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb)
{
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_lock_t	*lock = drvhdl->lock;

	if (lock == NULL)
		return NULL;

	device &= SPI_DEV_ID_MASK;

	while (lock) {
		if (lock->device == device)
			return lock;
		lock = lock->next;
	}

	return NULL;
}


int
_spi_lock_dev(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb)
{
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_lock_t	*lock = drvhdl->lock;

	if (NULL == (lock = calloc(1, sizeof(spi_lock_t))))
		return (ENOMEM);

	/*
	 * Simply add to the head of the lock list
	 */
	lock->owner  = ocb;
	lock->device = device &= SPI_DEV_ID_MASK;
	lock->next   = drvhdl->lock;
	drvhdl->lock = lock;

	return (EOK);
}

int
_spi_unlock_dev(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb)
{
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_lock_t	*prev, *curr;

	prev = curr = drvhdl->lock;

	if (curr == NULL)
		return EINVAL;

	device &= SPI_DEV_ID_MASK;

	while (curr) {
		/*
		 * Only the owner can unlock the device
		 */
		if (((curr->device == device) || (device == SPI_DEV_ID_NONE)) && (curr->owner == ocb)) {
			if (curr == prev)	/* on the top the list */
				drvhdl->lock = curr->next;
			else
				prev->next = curr->next;
			free(curr);
			return EOK;
		}

		prev = curr;
		curr = curr->next;
	}

	return (ENODEV);
}

int
_spi_lock_check(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb)
{
	spi_lock_t	*lock;

	lock = _spi_islock(ctp, device, ocb);

	/*
	 * Deviec is locked
	 */
	if (lock) {
		/*
		 * The device is locked by owner
		 */
		if (lock->owner == ocb) {
			/*
			 * Unlock request
			 */
			if (device & SPI_DEV_UNLOCK)
				return _spi_unlock_dev(ctp, device, ocb);

			/*
			 * The device is accessable
			 */
			return EOK;
		}
		else
			return EPERM;
	}

	/*
	 * Device is not locked
	 */
	if (device & SPI_DEV_LOCK)			/* Lock request */
		return _spi_lock_dev(ctp, device, ocb);
	else if (device & SPI_DEV_UNLOCK)	/* Unlock request */
		return EINVAL;
	else
		return EOK;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_devlock.c $ $Rev: 657836 $" );
