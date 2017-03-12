/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */


#ifndef _SPI_MASTER_LIB_H_INCLUDED
#define _SPI_MASTER_LIB_H_INCLUDED

#include <sys/iofunc.h>
#include <sys/dispatch.h>

#ifndef	__TYPES_H_INCLUDED
#include <sys/types.h>
#endif

#ifndef	_INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#include <sys/iomsg.h>

#define	SPI_VERSION_MAJOR	1
#define	SPI_VERMAJOR_SHIFT	16
#define	SPI_VERSION_MINOR	0
#define	SPI_VERMINOR_SHIFT	8
#define SPI_REVISION		0
#define	SPI_VERREV_SHIFT	0


/*
 * SPI driver information
 */
typedef struct {
	uint32_t	version;
	char		name[16];	/* Driver name */
	uint32_t	feature;
#define	SPI_FEATURE_DMA			(1 << 31)
#define	SPI_FEATURE_DMA_ALIGN	0xFF		/* DMA buffer alignment mask, alignment = 2^alignment */
} spi_drvinfo_t;


typedef struct {
	uint32_t	mode;
#define	SPI_MODE_CHAR_LEN_MASK	(0xFF)		/* Charactor length */
#define	SPI_MODE_CKPOL_HIGH		(1 <<  8)
#define	SPI_MODE_CKPHASE_HALF	(1 <<  9)
#define	SPI_MODE_BODER_MSB		(1 << 10)
#define	SPI_MODE_CSPOL_MASK		(1 << 11)	/* Chip select polarity */
#define	SPI_MODE_CSPOL_HIGH		(1 << 11)
#define	SPI_MODE_CSSTAT_HIGH	(1 << 12)
#define	SPI_MODE_CSHOLD_HIGH	(1 << 13)
#define	SPI_MODE_RDY_MASK		(3 << 14)	/* Ready signal control */
#define	SPI_MODE_RDY_NONE		(0 << 14)
#define	SPI_MODE_RDY_EDGE		(1 << 14)	/* Falling edge signal */
#define	SPI_MODE_RDY_LEVEL		(2 << 14)	/* Low level signal */
#define	SPI_MODE_IDLE_INSERT	(1 << 16)

#define	SPI_MODE_LOCKED			(1 << 31)	/* The device is locked by another client */

	uint32_t	clock_rate;
} spi_cfg_t;

#define	SPI_DEV_ID_MASK			0xFFFF
#define	SPI_DEV_ID_NONE			SPI_DEV_ID_MASK

/* For SPI API interface */
#define	SPI_DEV_DEFAULT			(1 << 31)	/* Default device, use by spi_setcfg()/spi_getdevinfo() call */
#define	SPI_DEV_LOCK			(1 << 30)	/* Lock device, for spi_read()/spi_write()/spi_exchange() */
#define	SPI_DEV_UNLOCK			(1 << 29)	/* Unlock device, for spi_read()/spi_write()/spi_exchange() */

/* For SPI driver interface */
#define SPI_DEV_XFER_MASK		3
#define SPI_DEV_XFER_SHIFT		16
#define SPI_DEV_EXCHANGE		3
#define SPI_DEV_READ			1
#define SPI_DEV_WRITE			2

typedef struct {
	uint32_t	device;		/* Device ID */
	char		name[16];	/* Device description */
	spi_cfg_t	cfg;		/* Device configuration */
} spi_devinfo_t;


/*
 * Resource Manager Interface
 */
#define	_IOMGR_SPI	(_IOMGR_PRIVATE_BASE + 0x01)

typedef struct {
	io_msg_t	msg_hdr;
#define	_SPI_IOMSG_READ			0x0001
#define	_SPI_IOMSG_WRITE		0x0002
#define	_SPI_IOMSG_EXCHANGE		0x0003
#define	_SPI_IOMSG_CMDREAD		0x0004
#define	_SPI_IOMSG_DMAXCHANGE	0x0005
	uint32_t	device;
	int32_t		xlen;
} spi_msg_t;

/*
 * DMA buffer address : physical address
 */
typedef struct {
	uint64_t	rpaddr;
	uint64_t	wpaddr;
} spi_dma_paddr_t;


/*
 * The following devctls are used by a client application
 * to control the SPI interface.
 */

#include <devctl.h>
#define _DCMD_SPI				_DCMD_MISC

#define DCMD_SPI_SET_CONFIG		__DIOT (_DCMD_SPI, 0x11, spi_cfg_t)
#define	DCMD_SPI_GET_DRVINFO	__DIOF (_DCMD_SPI, 0x12, spi_drvinfo_t)
#define	DCMD_SPI_GET_DEVINFO	__DIOF (_DCMD_SPI, 0x13, spi_devinfo_t)


/*
 * SPI API calls
 */
int	spi_open(const char *path);
int spi_close(int fd);
int spi_setcfg(int fd, uint32_t device, spi_cfg_t *cfg);
int spi_getdevinfo(int fd, uint32_t device, spi_devinfo_t *devinfo);
int spi_getdrvinfo(int fd, spi_drvinfo_t *drvinfo);
int spi_read(int fd, uint32_t device, void *buf, int len);
int spi_write(int fd, uint32_t device, void *buf, int len);
int spi_xchange(int fd, uint32_t device, void *wbuf, void *rbuf, int len);
int spi_cmdread(int fd, uint32_t device, void *cbuf, int16_t clen, void *rbuf, int rlen);
int spi_dma_xchange(int fd, uint32_t device, void *wbuf, void *rbuf, int len);



/*
 * For SPI driver interface
 */

/*
 * Hardware interface for low level driver
 */
typedef struct {
	/* size of this structure */
	size_t	size;

	/*
	 * Initialize master interface.
	 * Returns a handle associated with this driver
	 * Returns:
	 * !NULL    success
	 * NULL     failure
	 */
	void*	(*init)(void *hdl, char *options);

	/*
	 * Clean up driver.
	 * Frees memory associated with "hdl".
	 */
	void	(*fini)(void *hdl);

	/*
	 * Get driver information
	 */
	int		(*drvinfo)(void *hdl, spi_drvinfo_t *info);

	/*
	 * Get device information
	 */
	int		(*devinfo)(void *hdl, uint32_t device, spi_devinfo_t *info);

	/*
	 * Set SPI configuration
	 */
	int		(*setcfg)(void *hdl, uint16_t device, spi_cfg_t *cfg);

	/*
	 * xfer function calls
	 */
	void*	(*xfer)(void *hdl, uint32_t device, uint8_t *buf, int *len);

	/*
	 * DMA xfer function calls
	 */
	int		(*dma_xfer)(void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len);
} spi_funcs_t;

/*
 * Low-level entry, has to be at the beginning of low-level handle
 */
typedef struct _spidev_entry_t {
	iofunc_attr_t	attr;
	void			*hdl;		/* Pointer to high-level handle */
	void			*lock;		/* Pointer to lock list */
} SPIDEV;


#endif


__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/spi-master/public/hw/spi-master.h $ $Rev: 219996 $" )
