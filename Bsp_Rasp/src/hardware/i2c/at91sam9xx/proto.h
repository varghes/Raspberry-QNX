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




#ifndef __PROTO_H_INCLUDED
#define __PROTO_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <hw/i2c.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

#define AT91SAM_I2C_IDLE    0
#define AT91SAM_I2C_TX      1
#define AT91SAM_I2C_RX      2

typedef struct _at91sam9xx_dev {

	uint8_t             *buf;
	unsigned            buflen;

	unsigned            reglen;
	uintptr_t           regbase;
	unsigned            physbase;

	int                 intr;
	int                 iid;
	struct sigevent     intrevent;
	unsigned            txrx;
	unsigned            status;
	unsigned            cur_len;
	unsigned            tot_len;
	unsigned            stop;

	unsigned            own_addr;
	unsigned            slave_addr;
	unsigned            clk;
	i2c_addrfmt_t       slave_addr_fmt;
	unsigned            restart;
	unsigned            options;
	
	unsigned            speed;
} at91sam9xx_dev_t;

#define AT91SAM9XX_OPT_VERBOSE		0x00000002
#define INVALID_ADDR			0x80

#define AT91SAM9XX_I2C_CTRREG_OFF	0x00
#define CTRREG_SWRST			(1 << 7)
#define CTRREG_SVDIS			(1 << 5)
#define CTRREG_SVEN			(1 << 4)
#define CTRREG_MSDIS			(1 << 3)
#define CTRREG_MSEN			(1 << 2) 
#define CTRREG_STOP			(1 << 1)
#define CTRREG_START			(1 << 0)
#define AT91SAM9XX_I2C_MMREG_OFF	0x04
#define MMREG_MREAD			(1 << 12)
#define AT91SAM9XX_I2C_SMREG_OFF	0x08
#define AT91SAM9XX_I2C_IADDREG_OFF	0x0C
#define AT91SAM9XX_I2C_CLOCKGEN_OFF	0x10
#define AT91SAM9XX_I2C_STSREG_OFF	0x20
#define STSREG_NACK			(1 << 8)
#define STSREG_GACC			(1 << 6)
#define STSREG_SVACC			(1 << 5)
#define STSREG_SVREAD			(1 << 4)
#define STSREG_TXRDY			(1 << 2)
#define STSREG_RXRDY			(1 << 1)
#define STSREG_TXCOMP			(1 << 0)
#define AT91SAM9XX_I2C_IEREG_OFF	0x24
#define AT91SAM9XX_I2C_IDREG_OFF	0x28
#define AT91SAM9XX_I2C_IMREG_OFF	0x2C
#define AT91SAM9XX_I2C_RHREG_OFF	0x30
#define AT91SAM9XX_I2C_THREG_OFF	0x34

#define AT91SAM9XX_I2C_REGLEN		0x100

#define AT91SAM9XX_I2C_REVMAJOR(rev)	(((rev) >> 8) & 0xff)
#define AT91SAM9XX_I2C_REVMINOR(rev)	((rev) & 0xff)

void *at91sam9xx_init(int argc, char *argv[]);
void at91sam9xx_fini(void *hdl);
int at91sam9xx_options(at91sam9xx_dev_t *dev, int argc, char *argv[]);

int at91sam9xx_set_slave_addr(void *hdl, unsigned int addr, i2c_addrfmt_t fmt);
int at91sam9xx_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed);
int at91sam9xx_version_info(i2c_libversion_t *version);
int at91sam9xx_driver_info(void *hdl, i2c_driver_info_t *info);
i2c_status_t at91sam9xx_recv(void *hdl, void *buf, 
		unsigned int len, unsigned int stop);
i2c_status_t at91sam9xx_send(void *hdl, void *buf, 
		unsigned int len, unsigned int stop);
i2c_status_t at91sam9xx_wait_complete(at91sam9xx_dev_t *dev);

int at91sam9xx_attach_intr(at91sam9xx_dev_t *dev);

#endif
