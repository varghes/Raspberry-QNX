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
_spi_devctl(resmgr_context_t *ctp, io_devctl_t *msg, spi_ocb_t *ocb)
{
	int			status;
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_dev_t	*dev = drvhdl->hdl;

	status = iofunc_devctl_default(ctp, msg, &ocb->hdr);
	if (status != _RESMGR_DEFAULT)
		return status;

	switch (msg->i.dcmd) {
		case DCMD_SPI_SET_CONFIG:
		{
			spi_cfg_t	*cfg;
			uint32_t	device;

			if (msg->i.nbytes < (sizeof(spi_cfg_t) + sizeof(uint32_t)))
				return EINVAL;

			device = *(uint32_t *)_DEVCTL_DATA(msg->i);
			cfg    = (spi_cfg_t *)((uint8_t *)_DEVCTL_DATA(msg->i) + sizeof(uint32_t));

			if ((device & SPI_DEV_ID_MASK) == SPI_DEV_ID_NONE)
				return EINVAL;

			/*
			 * Client wants to change default device
			 */
			if (device & SPI_DEV_DEFAULT)
				ocb->chip = device;

			/*
			 * Wants to set configuration for this device?
			 */
			if (cfg->mode != 0) {
				if (dev->funcs->setcfg == NULL)
					return ENOTSUP;
			
				status = dev->funcs->setcfg(drvhdl, device, cfg);
				if (status != EOK)
					return EINVAL;
			}

			memset(&msg->o, 0, sizeof(msg->o));
			msg->o.ret_val = 0;
			msg->o.nbytes = 0;
			return _RESMGR_PTR(ctp, msg, sizeof(msg->o));
		}

		case DCMD_SPI_GET_DEVINFO:
		{
			uint32_t		device;
			spi_devinfo_t	*info;
			spi_lock_t		*lock;

			if (msg->i.nbytes < sizeof(uint32_t))
				return EINVAL;

			if (dev->funcs->devinfo == NULL)
				return ENOTSUP;

			device = *((uint32_t *)_DEVCTL_DATA(msg->i));

			info = (spi_devinfo_t *)_DEVCTL_DATA(msg->o);
			status = dev->funcs->devinfo(drvhdl, device, info);
			if (status != EOK)
				return EINVAL;

			if ((lock = _spi_islock(ctp, device, ocb)) != NULL) {
				if (lock->owner != ocb)
					info->cfg.mode |= SPI_MODE_LOCKED;
			}

			msg->o.nbytes = sizeof(spi_devinfo_t);

			return _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(spi_devinfo_t));
		}

		case DCMD_SPI_GET_DRVINFO:
		{
			spi_drvinfo_t	*info;

			if (dev->funcs->drvinfo == NULL)
				return ENOTSUP;

			info = (spi_drvinfo_t *)_DEVCTL_DATA(msg->o);

			status = dev->funcs->drvinfo(drvhdl, info);
			if (status != EOK)
				return EINVAL;

			msg->o.nbytes = sizeof(spi_drvinfo_t);
			return _RESMGR_PTR(ctp, msg, sizeof(msg->o) + sizeof(spi_drvinfo_t));
		}
	}

	return ENOSYS;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_devctl.c $ $Rev: 657836 $" );
