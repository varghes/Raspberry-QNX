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








#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <sys/devi.h>
#include "usb.h"

void                   devi_init(pCommon_callbacks_t pCallbacks);
int                    begin(int, char**);

/* buffer */

buffer_t *        	buff_create(unsigned, unsigned);
int 			buff_append(buffer_t *, char *, int );
int 			buff_delete(buffer_t *, char *, int );
int 			buff_flush(buffer_t *);
int 			buff_putc(buffer_t *, char );
int 			buff_getc(buffer_t *, char *);
int 			buff_waiting(buffer_t *);


/* Event Bus */

event_bus_line_t	*bus_create(void);
int		bus_add(event_bus_line_t *, input_module_t *);
int 		bus_complete(event_bus_line_t *line);
void		bus_dump(void);
int		bus_interrogate(int, input_module_t **);
int		bus_reset(void);


/* base */

int 		options(int argc, char *argv[], uint32_t *, unsigned int *);


/* util */

int         devi_get_coid();
void		init_sclk(void);
void 		clk_get(struct timespec *);


/* module */

input_module_t	*module_lookup(char *name);
input_module_t	*module_filter_lookup(int);

int  start_photon_interface(uint32_t);

int  resmgr_create_device(event_bus_line_t *line);

void resmgr_init(int);
void resmgr_enqueue_packet(event_bus_line_t *, char *data, int size);

int  photon_raw_enqueue(unsigned type, char *dptr, int size);

int  gfi_register(int input_group);

/* Dispatch services      */
dispatch_t * devi_get_dispatch_handle();
void devi_set_dispatch_handle(dispatch_t * pdp);

/* Initializes USB driver */
void devi_usb_init();

/* Initializes USB client  */
int devi_usb_server_connect(char * serv_path_name);

/* Disconnect USB server   */
void devi_usb_server_disconnect();

/* This function attach client to USB sub-system.  Register your client from
 * reset function of  device module. If nDev >= 0, it specifies device number.
 * Function returns local module handler
*/
void *  devi_usb_register_client(input_module_t * pInput_module, int nDev,
				 _uint32 nVendor, _uint32 nClass, _uint32 nSubClass);

/* Detach input module from USB driver */
void devi_unregister_usb_client(void * h);

/* Get device status (see usb.h)       */
int  devi_usb_device_status(void * pHandle, _uint8 * pnStatus);


/* Initializes HID driver              */
void  devi_hid_init();

/* Connects to USB HID server          */
int devi_hid_server_connect(char * serv_path_name);

/* Disconnects client from USB HID server */
void devi_hid_server_disconnect();

/* This function attach client to HID USB sub-system.  Register your client from
 * reset function of each device module. If nDev >= 0, it specifies device number.
 * Function returns local module handler
*/
void *  devi_hid_register_client(input_module_t *module, int nDev);

/* Detach input module from HID driver  */ 
void devi_unregister_hid_client(void * h);

/* HID devctrl processor                */ 
int devi_hid_devctrl(void * pHandle, int event, void *ptr, 
    int nDev /*HIDD_CONNECT_WILDCARD if all devices connected to this module*/);

/* Register pulse                       */
int devi_register_pulse(input_module_t *module, int code, 
    int (*func)(message_context_t *, int, unsigned, void *));

/* Register interrupt                   */
int devi_register_interrupt(int intr, int prio, int *code, input_module_t *module, struct sigevent *evp, unsigned);

/* Register timer                       */
timer_t	devi_register_timer(input_module_t *module, int prio, int *code, struct sigevent *evp);

/* Request input/output space mapping   */
uintptr_t 	devi_request_iorange(unsigned start, unsigned len, unsigned flags);

/* Engue data package                   */
int  devi_enqueue_packet(input_module_t *, char *, unsigned);

/* Convert scan code(s) to USB keyboard usage code */
int devi_scan_to_usb(unsigned char * buf, int * pnBytes, USBKCode * usbCodes, 
		     unsigned char * makeBreakes, int * pnLen);

/* Convert USB keyboard usage code to scan code(s) */
int devi_usb_to_scan(USBKCode usbCode, int nMakeBreak, _uint8 * buf, int * pnLen);


// This function sends control data to the device control endpoint    
int devi_output_control(void * pHandle, pControlRequest pRequest, void * data, int nLen);

/* from main */
void input_shutdown(void);

#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/proto.h $ $Rev: 657836 $" )
