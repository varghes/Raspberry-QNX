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




#ifndef _I2C_LIB_H_INCLUDED
#define _I2C_LIB_H_INCLUDED

#ifndef	_PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#define I2CLIB_VERSION_MAJOR    1
#define I2CLIB_VERSION_MINOR    0
#define I2CLIB_REVISION         0

#include <_pack1.h>

typedef struct {
    unsigned char   major;
    unsigned char   minor;
    unsigned char   revision;
} i2c_libversion_t;

typedef enum {
    I2C_STATUS_DONE     = 0x0001,   /* transaction completed (with or
                                       without error) */
    I2C_STATUS_ERROR    = 0x0002,   /* unknown error */
    I2C_STATUS_NACK     = 0x0010,   /* slave no-acknowledge */
    I2C_STATUS_ARBL     = 0x0020,   /* lost arbitration */
    I2C_STATUS_BUSY     = 0x0040,   /* timed out */
    I2C_STATUS_ABORT    = 0x0100    /* transaction aborted */
} i2c_status_t;

typedef enum {
    I2C_ADDRFMT_10BIT   = 0x0001,
    I2C_ADDRFMT_7BIT    = 0x0002,
} i2c_addrfmt_t;

typedef enum {
    I2C_SPEED_STANDARD  = 0x0001,   /* up to 100 kbit/s */
    I2C_SPEED_FAST      = 0x0002,   /* up to 400 kbit/s */
    I2C_SPEED_HIGH      = 0x0004    /* up to 3.4 Mbit/s */
} i2c_speed_mode_t;

typedef struct {
    _Uint32t            speed_mode; /* supported speeds: I2C_SPEED_* */
    _Uint32t            addr_mode;  /* supported address fmts: I2C_ADDRFMT_* */
    _Uint32t            reserved[2];
} i2c_driver_info_t;

typedef struct {
    _Uint32t            addr;   /* I2C address */
    _Uint32t            fmt;    /* 7- or 10-bit format */
} i2c_addr_t;

typedef struct {
    i2c_addr_t          slave;  /* slave address */
    _Uint32t            len;    /* length of send data in bytes */
    _Uint32t            stop;   /* send stop when complete? (0=no, 1=yes) */
} i2c_send_t;

typedef struct {
    i2c_addr_t          slave;  /* slave address */
    _Uint32t            len;    /* length of receive data in bytes */
    _Uint32t            stop;   /* send stop when complete? (0=no, 1=yes) */
} i2c_recv_t;

typedef struct {
    i2c_addr_t          slave;      /* slave address */
    _Uint32t            send_len;   /* length of send data in bytes */
    _Uint32t            recv_len;   /* length of receive data in bytes */
    _Uint32t            stop;       /* set stop when complete? */
} i2c_sendrecv_t;

typedef struct {
    _Uint32t            len;    /* length of data to send/recv, in bytes
                                   (not including this header) */
    _Uint32t            stop;   /* send stop when complete? (0=no, 1=yes) */
} i2c_masterhdr_t;

#include <_packpop.h>

/*
 * Resource Manager Interface
 */

/*
 * The following devctls are used by a client application
 * to control the I2C interface.
 */

#include <devctl.h>
#define _DCMD_I2C   _DCMD_MISC

#define DCMD_I2C_SET_SLAVE_ADDR     __DIOT (_DCMD_I2C, 1, i2c_addr_t)
#define DCMD_I2C_SET_BUS_SPEED      __DIOT (_DCMD_I2C, 2, _Uint32t)
#define DCMD_I2C_MASTER_SEND        __DIOT (_DCMD_I2C, 3, i2c_masterhdr_t)
#define DCMD_I2C_MASTER_RECV        __DIOTF(_DCMD_I2C, 4, i2c_masterhdr_t)
#define DCMD_I2C_SEND               __DIOT (_DCMD_I2C, 5, i2c_send_t)
#define DCMD_I2C_RECV               __DIOTF(_DCMD_I2C, 6, i2c_recv_t)
#define DCMD_I2C_SENDRECV           __DIOTF(_DCMD_I2C, 7, i2c_sendrecv_t)
#define DCMD_I2C_LOCK               __DION (_DCMD_I2C, 8)
#define DCMD_I2C_UNLOCK             __DION (_DCMD_I2C, 9)
#define DCMD_I2C_DRIVER_INFO        __DIOF (_DCMD_I2C, 10, i2c_driver_info_t)

#define DCMD_I2C_GET_BUS_SPEED      __DIOF (_DCMD_I2C, 11, _Uint32t)

/*
 * Hardware interface
 */
typedef struct {
    /* size of this structure */
    size_t size;

    /*
     * Return version information
     * Returns:
     * 0    success
     * -1   failure
     */
    int (*version_info)(i2c_libversion_t *version);

    /*
     * Initialize master interface.
     * Returns a handle that is passed to all other functions.
     * Returns:
     * !NULL    success
     * NULL     failure
     */
    void *(*init)(int argc, char *argv[]);

    /*
     * Clean up driver.
     * Frees memory associated with "hdl".
     */
    void (*fini)(void *hdl);

    /*
     * Master send.
     * Parameters:
     * (in)     hdl         Handle returned from init()
     * (in)     buf         Buffer of data to send
     * (in)     len         Length in bytes of buf
     * (in)     stop        If !0, set stop condition when send completes
     * Returns:
     * bitmask of status bits
     */
    i2c_status_t (*send)(void *hdl, void *buf, unsigned int len, 
                         unsigned int stop);
    /*
     * Master receive.
     * Parameters:
     * (in)     hdl         Handle returned from init()
     * (in)     buf         Buffer for received data
     * (in)     len         Length in bytes of buf
     * (in)     stop        If !0, set stop condition when recv completes
     * Returns:
     * bitmask of status bits
     */
    i2c_status_t (*recv)(void *hdl, void *buf, unsigned int len, 
                         unsigned int stop);

    /*
     * Force the master to free the bus.
     * Returns when the stop condition has been sent.
     * Returns:
     * 0    success
     * -1   failure
     */
    int (*abort)(void *hdl, int rcvid);

    /*
     * Specify the target slave address.
     * Returns:
     * 0    success
     * -1   failure
     */
    int (*set_slave_addr)(void *hdl, unsigned int addr, i2c_addrfmt_t fmt);

    /*
     * Specify the bus speed. 
     * If an invalid bus speed is requested, this function should return
     * failure and leave the bus speed unchanged.
     * Parameters:
     * (in)     speed       Bus speed. Units are implementation-defined.
     * (out)    ospeed      Actual bus speed (if NULL, this is ignored)
     * Returns:
     * 0    success
     * -1   failure
     */
    int (*set_bus_speed)(void *hdl, unsigned int speed, unsigned int *ospeed);

    /*
     * Request info about the driver.
     * Returns:
     * 0    success
     * -1   failure
     */
    int (*driver_info)(void *hdl, i2c_driver_info_t *info);

    /*
     * Handle a driver-specific devctl().
     * Parameters:
     * (in)     cmd         Device command
     * (i/o)    msg         Message buffer
     * (in)     msglen      Length of message buffer in bytes
     * (out)    nbytes      Bytes to return (<= msglen)
     * (out)    info        Extra status information returned by devctl
     * Returns:
     * EOK      success
     * errno    failure
     */
    int (*ctl)(void *hdl, int cmd, void *msg, int msglen, 
               int *nbytes, int *info);
} i2c_master_funcs_t;

#define I2C_MASTER_GETFUNCS "i2c_master_getfuncs"

/*
 * Fills in the given table with the hardware-specific functions. 
 * If a function table entry is unimplemented, it should 
 * be left unchanged (and not set to NULL, etc.).
 * "funcs->size" should be set by whomever has allocated the
 * table. Don't change this.
 * Parameters:
 * (i/o)    funcs       Function table
 * (in)     tabsize     size of "funcs" in bytes
 * Returns:
 * 0    success
 * -1   failure
 */
typedef int (i2c_master_getfuncs_f)(i2c_master_funcs_t *funcs, int tabsize);
i2c_master_getfuncs_f   i2c_master_getfuncs;

#define I2C_ADD_FUNC(tabletype, table, entry, func, limit) \
    if (((size_t)&(((tabletype *)0)->entry)) + \
        sizeof (void (*)()) <= (limit)) \
        (table)->entry = (func);

#endif

__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/i2c/public/hw/i2c.h $ $Rev: 501843 $" )
