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




#ifndef _SPI_PROTO_H_INCLUDED
#define _SPI_PROTO_H_INCLUDED

struct spi_ocb;
#define IOFUNC_OCB_T    struct spi_ocb

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <hw/spi-master.h>

#define SPI_RESMGR_NPARTS_MIN   2
#define SPI_RESMGR_MSGSIZE_MIN  2048
#define SPI_CLIENTS_MAX         32

#define	SPI_EVENT				1
#define	SPI_PRIORITY			24

#define _SPI_DEV_READ(d)		(d | (SPI_DEV_READ << SPI_DEV_XFER_SHIFT))
#define _SPI_DEV_WRITE(d)		(d | (SPI_DEV_WRITE << SPI_DEV_XFER_SHIFT))
#define _SPI_DEV_EXCHANGE(d)	(d | (SPI_DEV_EXCHANGE << SPI_DEV_XFER_SHIFT))

extern iofunc_funcs_t			_spi_ocb_funcs;
extern resmgr_io_funcs_t		_spi_io_funcs;
extern resmgr_connect_funcs_t	_spi_connect_funcs;
extern iofunc_mount_t			_spi_mount;

typedef struct spi_ocb {
	iofunc_ocb_t		hdr;
	uint32_t			chip;
} spi_ocb_t;

typedef struct spi_lock {
	spi_ocb_t			*owner;		/* Owner */
	uint32_t			device;		/* Device */
	struct spi_lock		*next;
} spi_lock_t;

typedef struct spi_dev {
	dispatch_t			*dpp;
	dispatch_context_t	*ctp;
	int					id;
	spi_funcs_t			*funcs;

	uint8_t				*buf;
	uint8_t				*dmabuf;
	unsigned			buflen;

	void				*drvhdl;
	void				*dlhdl;

	pthread_t			tid;
	int					chid;
	int					coid;

	char				*opts;
	int					devnum;

	void				*next;
} spi_dev_t;


spi_ocb_t  *_spi_ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr);
spi_lock_t *_spi_islock(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb);
void _spi_ocb_free(spi_ocb_t *ocb);
void *_spi_dlload(void **hdl, const char *optarg);
int _spi_create_instance(spi_dev_t *dev);
int _spi_init_iofunc(void);
int _spi_close_ocb(resmgr_context_t *ctp, void *reserved, spi_ocb_t *ocb);
int _spi_devctl(resmgr_context_t *ctp, io_devctl_t *msg, spi_ocb_t *ocb);
int _spi_read(resmgr_context_t *ctp, io_read_t *msg, spi_ocb_t *ocb);
int _spi_write(resmgr_context_t *ctp, io_write_t *msg, spi_ocb_t *ocb);
int _spi_iomsg(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_iomsg_read(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_iomsg_cmdread(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_iomsg_write(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_iomsg_xchange(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_iomsg_dmaxchange(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb);
int _spi_lock_check(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb);
int _spi_unlock_dev(resmgr_context_t *ctp, uint32_t device, spi_ocb_t *ocb);
int _spi_slogf(const char *fmt, ...);


#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/proto.h $ $Rev: 657836 $" )
