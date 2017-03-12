/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008 QNX Software Systems.  
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




#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <sys/queue.h>
#include <sys/usbdi.h>

#include <sys/devi.h>


#define MAX_TIME_WAIT    (1)		// Time (in seconds) for sinchronization lock



// This are the macros for support device common blocks 

typedef struct _module_data
{
	LIST_ENTRY (_module_data) lst_conn;	// List connector
	input_module_t *pInput_module;	// Pointer to module descriptor
	int nDev;											// Device number or USB_CONNECT_WILDCARD of module doesn't care
	_uint32 nVendor;							// Vendor ID
	_uint32 nClass;								// Device class
	_uint32 nSubClass;						// Device subclass
	_uint32 flags;								// Device status flags (see usb.h and devi_usb_device_status)
	// USB device  parameters 
	usbd_device_instance_t *pInstance;
	struct usbd_device *pDevice;
	_uint32 status;
	struct usbd_pipe *ep_cntl;		// Control pipe
	struct usbd_pipe *ep_int;			// Interrupt pipe
	struct usbd_urb *urb;					// Request Block
	uint32_t ep_int_size;					// Transfer buffer size (interrupt)
	_uint8 *ep_int_buf;						// Transfer buffer  (interrupt)
	uint32_t ep_cnt_size;					// Transfer buffer size (control)
	_uint8 *ep_cnt_buf;						// Transfer buffer (control)
	// End of USB device  parameters  
	void *pData;									// report private data area (depends on report) 
}
module_data_t, *pModule_data_t;



// Static variables
// 
static struct usbd_connection *pGlbConnection;

static
LIST_HEAD (_modList, _module_data)
	modList;											// List of the registred modules
		 static pthread_mutex_t mod_mutex;	// Use this mutex for safe modules list modification


// Prototypes+
// 
		 static void insertion (struct usbd_connection *pConnection,
														usbd_device_instance_t * pInstance);
		 static void removal (struct usbd_connection *pConnection,
													usbd_device_instance_t * pInstance);

		 static void usb_dev_int (struct usbd_urb *urb, struct usbd_pipe *pipe,
															void *hdl);
		 static int parse_descriptors (pModule_data_t pModule);

#define LIST_FIRST_ITEM(head) ((head) -> lh_first)
#define LIST_NEXT_ITEM(elm, field) ((elm == NULL) ? NULL : (elm) -> field.le_next)
#define LIST_PREV_ITEM(elm, field) ((elm == NULL) ? NULL : (elm) -> field.le_prev)



/* Description: this function initializes USB client                                */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : Must be called before any other devi_usb... function                */
		 void devi_usb_init ()
{
	LIST_INIT (&modList);
	pGlbConnection = NULL;

	if (EOK != pthread_mutex_init (&mod_mutex, NULL)) {
		char *pMsgTxt =
			"Error: USB stack initialization error. Driver is terminating\n";
		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		exit (-1);
	}
}



/* Description: this function initializes USB client                                */
/* Input      : char * serv_path_name - server path name (if NULL, takes default    */
/*              (/dev/usb/)                                                         */
/* Output     : None                                                                */
/* Return     : EOK if OK or negative error value                                   */
/* Comment    : Must be called after registration of all input modules for USB      */
/*              devices ( see devi_register_hid_client)                             */
int
devi_usb_server_connect (char *serv_path_name)
{
	int rc = EOK;
	usbd_funcs_t funcs = { _USBDI_NFUNCS, insertion, removal, NULL };
	usbd_device_ident_t interest =
		{ USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD,
USBD_CONNECT_WILDCARD, USBD_CONNECT_WILDCARD };
	usbd_connect_parm_t parm =
		{ NULL, USB_VERSION, USBD_VERSION, 0, 0, NULL, 0, &interest, &funcs,
USBD_CONNECT_WAIT };


	parm.path = serv_path_name;

	if (EOK != (rc = usbd_connect (&parm, &pGlbConnection))) {
		char *pMsgTxt =
			"Error: cannot connect to USB server (error code = %d).\nDriver is terminating\n";
		fprintf (stderr, pMsgTxt, rc);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, rc);
		exit (-2);
	}

	return EOK;
}

/* Description: this function detaches USB client from server                       */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void
devi_usb_server_disconnect ()
{
	if (NULL != pGlbConnection)
		usbd_disconnect (pGlbConnection);
	pGlbConnection = NULL;
}


/* Description: This function attach input module to USB sub-system.                */
/* Input      : input_module_t * pInput_module - module that requests registration  */
/*              int nDev - optional, USB device number (see comment)                */
/*              _uint32 nVendor - vendor ID (optional;default-USBD_CONNECT_WILDCARD)*/
/*              _uint32 nClass - dev class (optional;default-USBD_CONNECT_WILDCARD) */
/* Output     : None                                                                */
/* Return     : local module handle or NULL if error                                */
/* Comment    : Register your client from reset function of each device module. If  */
/*              nDev >= 0, it specifies device number.                              */
void *
devi_usb_register_client (input_module_t * pInput_module, int nDev,
													_uint32 nVendor, _uint32 nClass, _uint32 nSubClass)
{
	pModule_data_t pNewMod;
	struct timespec t;

	assert (pInput_module);

	clock_gettime (CLOCK_REALTIME, &t);
	t.tv_sec += MAX_TIME_WAIT;
	if (EOK != pthread_mutex_timedlock (&mod_mutex, &t))
		return NULL;

	if (NULL != (pNewMod = malloc (sizeof (module_data_t)))) {
		LIST_INSERT_HEAD (&modList, pNewMod, lst_conn);
		pNewMod->pInput_module = pInput_module;
		pNewMod->nDev = nDev;
		pNewMod->nVendor = nVendor;
		pNewMod->nClass = nClass;
		pNewMod->flags = 0;
	}

	pthread_mutex_unlock (&mod_mutex);

	return pNewMod;
}

/* Description: This function detach input module from  USB sub-system.             */
/* Input      : void * h - local module handle that was returned by                 */
/*              devi_register_usb_client function                                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : Must be called before any other devi_hid... function                */
void
devi_unregister_usb_client (void *h)
{
	int rc;
	pModule_data_t pModule = (pModule_data_t) h;
	struct timespec t;

	assert (pModule);


	if (pModule->ep_int) {
		usbd_reset_pipe (pModule->ep_int);
		usbd_abort_pipe (pModule->ep_int);
	}

	if (pModule->ep_cntl)
		usbd_abort_pipe (pModule->ep_cntl);

	if (pModule->urb)
		usbd_free_urb (pModule->urb);

	if (pModule->ep_int_buf)
		usbd_free (pModule->ep_int_buf);

	if (pModule->ep_cnt_buf)
		usbd_free (pModule->ep_cnt_buf);

	sleep (1);

	if (EOK != (rc = usbd_detach (pModule->pDevice))) {
		if (verbosity)
			fprintf (stderr, "Detach device: error %i\n", rc);
	}

	clock_gettime (CLOCK_REALTIME, &t);
	t.tv_sec += MAX_TIME_WAIT;
	if (EOK != pthread_mutex_timedlock (&mod_mutex, &t))
		return;

	LIST_REMOVE (pModule, lst_conn);

	pthread_mutex_unlock (&mod_mutex);

	free (pModule);

}


/* Description: Callback function; invoked by USB driver when new device is attached*/
/* Input      : struct usbd_connection *pConnection - USB connection handler        */
/*              usbd_device_instance_t *pInstance - connecting device instance      */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
insertion (struct usbd_connection *pConnection,
					 usbd_device_instance_t * pInstance)
{
	int rc;
	struct usbd_device *pDevice;
	pModule_data_t pModule, *ppModule;

	if (verbosity)
		printf ("Try to inser device\n");
	if (EOK !=
			(rc =
			 usbd_attach (pConnection, pInstance, sizeof (module_data_t **),
										&pDevice))) {
		char *pMsgTxt =
			"Error: cannot attach USB device #%i (error code is %i)\n";
		fprintf (stderr, pMsgTxt, (int) (pInstance->devno), rc);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt,
					 (int) (pInstance->devno), rc);
		return;
	}
	ppModule = (pModule_data_t *) usbd_device_extra (pDevice);


	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule;
			 pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		// Check device number if specified
		if ((USBD_CONNECT_WILDCARD != pModule->nDev)
				&& (pModule->nDev != pInstance->devno))
			continue;

		if ((USBD_CONNECT_WILDCARD != pModule->nVendor)
				&& (pModule->nVendor != pInstance->ident.vendor))
			continue;									// Wrong vendor - ignore

		if ((USBD_CONNECT_WILDCARD != pModule->nClass)
				&& (pModule->nClass != pInstance->ident.dclass))
			continue;

		if ((USBD_CONNECT_WILDCARD != pModule->nSubClass)
				&& (pModule->nSubClass != pInstance->ident.subclass))
			continue;

		pModule->pInstance = pInstance;
		pModule->pDevice = pDevice;
		if (EOK == parse_descriptors (pModule)) {
			if ((pModule->ep_int_buf = usbd_alloc (pModule->ep_int_size))) {
				if ((pModule->urb = usbd_alloc_urb (NULL))) {
					// Initialize request block
					usbd_setup_interrupt (pModule->urb,
																URB_DIR_IN | URB_SHORT_XFER_OK,
																pModule->ep_int_buf, pModule->ep_int_size);
					// Initialize Interrupt callback function
					if (EOK == usbd_io (pModule->urb,
															pModule->ep_int,
															usb_dev_int, pModule, USBD_TIME_INFINITY)) {
						pModule->flags |= USB_DEVICE_ON | USB_DEVICE_PRESENT;
						*ppModule = pModule;
						if (verbosity)
							printf ("Attach device\n");
						return;
					} else {
						if (verbosity)
							printf ("Cannot attach device\n");
					}
				}
				usbd_free (pModule->ep_int_buf);
			}
			usbd_detach (pDevice);
		}
	}
}

/* Description: Callback function; invoked by USB driver when new device is gonna be*/
/*              removed                                                             */
/* Input      : struct usbd_connection *pConnection - USB connection handler        */
/*              usbd_device_instance_t *pInstance - connecting device instance      */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
removal (struct usbd_connection *pConnection,
				 usbd_device_instance_t * pInstance)
{
	struct usbd_device *pDevice;
	pModule_data_t pModule;
	int rc;

	if (NULL != (pDevice = usbd_device_lookup (pConnection, pInstance))) {
		pModule = usbd_device_extra (pDevice);
		if (pModule->ep_int) {			// Stop the Interrupt In endpoint
			usbd_abort_pipe (pModule->ep_int);	// Abort pipe
			usbd_free_urb (pModule->urb);	// Free request block
			usbd_free (pModule->ep_int_buf);
			if ((rc = usbd_detach (pModule->pDevice)))
				fprintf (stderr, "usbd_detach %i\n", rc);
		}
	}
}


/* Description: Callback function; invoked by USB driver when new device is gonna be*/
/*              removed                                                             */
/* In/Out     : pModule_data_t pModule - module to probe (and to connect if it is   */
/*              a device we are going to connect                                    */
/* Output     : None                                                                */
/* Return     : None                                                                */
int
parse_descriptors (pModule_data_t pModule)
{
	_uint32 eix;
	usbd_descriptors_t *pDesc;
	struct usbd_desc_node *ifc, *ept;

	if (verbosity >= 3)
		printf ("Try to get interface descriptors %i, %i, %i\n",
						(int) (pModule->pInstance->config),
						(int) (pModule->pInstance->iface),
						(int) (pModule->pInstance->alternate));
	if (usbd_interface_descriptor (pModule->pDevice, pModule->pInstance->config, pModule->pInstance->iface, pModule->pInstance->alternate, &ifc)) {	// Trace interfaces
		for (eix = 0;
				 (pDesc =
					usbd_parse_descriptors (pModule->pDevice, ifc, USB_DESC_ENDPOINT,
																	eix, &ept)) != NULL; ++eix) {
			switch (pDesc->endpoint.bmAttributes) {
			case USB_ATTRIB_CONTROL:
				usbd_open_pipe (pModule->pDevice, pDesc, &(pModule->ep_cntl));
				pModule->ep_cnt_size = pDesc->endpoint.wMaxPacketSize;	// Size of input buffer
				pModule->ep_cnt_buf = usbd_alloc (pModule->ep_cnt_size);
				break;

			case USB_ATTRIB_ISOCHRONOUS:
				break;

			case USB_ATTRIB_BULK:
				break;

			case USB_ATTRIB_INTERRUPT:
				// Can have two endpoints: IN & OUT
				switch (pDesc->endpoint.bEndpointAddress & USB_ENDPOINT_IN) {
				case USB_ENDPOINT_OUT:
					break;
				case USB_ENDPOINT_IN:
					if (EOK == usbd_open_pipe (pModule->pDevice,
																		 pDesc, &(pModule->ep_int))) {
						pModule->ep_int_size = pDesc->endpoint.wMaxPacketSize;	// Size of input buffer
					}
					break;
				}
				break;
			}
		}
	}
	return ((pModule->ep_int && pModule->ep_cntl) ? EOK : ENODEV);
}


/* Description: Callback function; invoked by USB driver when input data come       */
/* In/Out     : struct usbd_urb *pUrb - pointer to request block                    */
/*              struct usbd_pipe *pPipe - pointer to input/output block             */
/*              void *hdl - pointer to user defined handeler (pModule in this case) */
/* Output     : None                                                                */
/* Return     : None                                                                */
void
usb_dev_int (struct usbd_urb *pUrb, struct usbd_pipe *pPipe, void *pHandle)
{
	pModule_data_t pModule = (pModule_data_t) pHandle;
	_uint32 status;
	_uint32 len;

	if (verbosity >= 3)
		printf ("Device sent data\n");

	// extract completion status and data transfer length from Request Block
	usbd_urb_status (pUrb, &status, &len);

	if ((status & USBD_URB_STATUS_MASK) != USBD_STATUS_CMP) {
		// check for a stall condition and clear it
		if ((status & USBD_USB_STATUS_MASK) == USBD_STATUS_STALL) {
			usbd_reset_pipe (pPipe);
		}
		if (verbosity)
			fprintf (stderr, "Interrupt transfer status %x\n", status);
	} else												// Process data
	{
		input_module_t *pInput_module =	// Pointer to input module descriptor
			pModule->pInput_module;

		(pInput_module->input) (pInput_module, len, pModule->ep_int_buf);
	}

	if (pModule->flags & USB_DEVICE_ON) {	// re-hook interrupt endpoint
		usbd_setup_interrupt (pModule->urb,
													URB_DIR_IN | URB_SHORT_XFER_OK,
													pModule->ep_int_buf, pModule->ep_int_size);
		if (usbd_io (pModule->urb,
								 pModule->ep_int, usb_dev_int, pModule, USBD_TIME_INFINITY))
			pModule->flags &= ~(USB_DEVICE_ON | USB_DEVICE_PRESENT);
	}
}


/* Description: This function sends control data to the device control endpoint     */
/* Input      : input_module_t * pInput_module - module that requests registration  */
/*              void * data - data to transfer                                      */
/*              int nLen - data buffer length                                       */
/* Output     : None                                                                */
/* Return     : EOK if ok, error code on error                                      */
int
devi_output_control (void *pHandle, pControlRequest pRequest, void *data,
										 int nLen)
{
	int rc = EOK;
	struct usbd_urb *pUrb;
	void *pLocalData = NULL;
	pModule_data_t pModule = (pModule_data_t) pHandle;

	if (NULL == pModule->ep_cntl)
		return EINVAL;

	if ((pRequest->dir & URB_DIR_OUT) && (nLen > pModule->ep_cnt_size))	// Too big buffer
		return EINVAL;

	if ((pUrb = usbd_alloc_urb (NULL)))	// Allocate request block
	{
		if ((pRequest->dir & URB_DIR_OUT) && (NULL != data) && (nLen > 0))
			memcpy (pModule->ep_cnt_buf, data, nLen);	// Take data to USB -allocated buffer
		// Setup vendor request block
		usbd_setup_vendor (pUrb, pRequest->dir,	// Direction
											 pRequest->req,	// Device specific request
											 pRequest->type | pRequest->rec,	// Type | recipient
											 pRequest->value,	// Request specific
											 pRequest->index,	// Request specific 
											 pModule->ep_cnt_buf, nLen);
		rc = usbd_io (pUrb, pModule->ep_cntl, NULL, pModule, USBD_TIME_INFINITY);
		// Delete request block
		usbd_free_urb (pUrb);

		if ((pRequest->dir & URB_DIR_IN) && (EOK == rc))	// Take data from USB -allocated buffer
			memcpy (data, pLocalData, min (nLen, pModule->ep_cnt_size));
	} else
		rc = ENOMEM;

	return (rc);

}


/* Description: This function returns USB device status                             */
/* Input      : void * pHandle - local module handle                                */
/* Output     : _uint8 * pnStatus - device status (see possible values in usb.h)    */
/* Return     : EOK if ok, otherwise error code                                     */
int
devi_usb_device_status (void *pHandle, _uint8 * pnStatus)
{
	pModule_data_t pModule = (pModule_data_t) pHandle;

	if (NULL == pModule)
		return EINVAL;

	*pnStatus = (pModule->flags & USB_DEVICE_STATUS_FLAGS);
	return EOK;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/usb.c $ $Rev: 657836 $" );
