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



/*
 *
 * hid.c 
 * 
 * This  is a USB/HID communication module implementation
 */
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <sys/queue.h>

#include <sys/devi.h>
#include "hid.h"
#include "photon.h"

// MOUSE
#define MAX_BUTTONS      (5)

#define MAX_JOYSTICK_BUTTONS 32 
#define MAX_CONTROL_BUTTONS  64

// KEYBOARD
//
#define MAX_KEYS_IN_BUFFER (20) // Actually keyboard doesn't use more than 6 buttons, however we use
                                                                    // this value for future expansion
#define TIME_MASK          (0x7fffffff)
#define REPEAT_FLAG        (0x80000000)


typedef enum
{
	HIDD_KEYBOARD_REPORT = 1,
	HIDD_MOUSE_REPORT,
	HIDD_JOYSTICK_REPORT,
	HIDD_TOUCHSCREEN_REPORT,
	HIDD_CONTROL_REPORT,
	HIDD_GAMEPAD_REPORT,
}
repClasses;

#define MAX_TIME_WAIT    (1)		// Time (in seconds) for sinchronization lock

typedef struct _device_data
{
	LIST_ENTRY (_device_data) lst_conn;	// List connector
	hidd_device_instance_t *pDevInstance;	// Instance of device
	void *pPrivData;							// Private device data
}
device_data_t, *pDevice_data_t;


typedef struct _module_data
{
	LIST_ENTRY (_module_data) lst_conn;	// List connector
	input_module_t *pInput_module;	// Pointer to module descriptor
	int nDev;											// Device number or HIDD_CONNECT_WILDCARD if module doesn't care
	 LIST_HEAD (_devDataList, _device_data)	// List of device's private data - several devices can be connected to each module 
	 devDataList;
	 LIST_HEAD (_inpRepList, _report_data)	// List of input reports for this module
	 inpRepList;
	 LIST_HEAD (_outRepList, _report_data)	// List of output reports for this module
	 outRepList;
	 LIST_HEAD (_featureRepList, _report_data)	// List of feature reports for this module
	 featureRepList;
}
module_data_t, *pModule_data_t;

typedef struct _report_data
{
	LIST_ENTRY (_report_data) lst_conn;	// List connector
	_uint8 nRepType;
	hidd_device_instance_t *pDevInstance;	// Instance of device this report belongs to
	struct hidd_collection *pCollection;	// Collection this report belongs to
	struct hidd_report_instance *pRepInstance;	// opaque report instance handler
	struct hidd_report *pReport;	// opaque report handler
	_uint16 usage_page;						// usage page ID
	_uint16 usage;								// usage ID
	pModule_data_t pModule;				// module descriptor this report belongs to
	void *pPrivData;							// Pointer to device private data block (stored in module devDataLis)
}
report_data_t, *pReport_data_t;


typedef struct _rep_keyboard_data
{
	hid_keyboard_data_t kbdData;
	_uint16 anUsages[MAX_KEYS_IN_BUFFER];	// Usages of keys that are currently pressed
	int nLastPressed;							// Number of key that was pressed last (index in anUssages)
	_uint32 tRep;									// Time (in msecs) when key was pressed
}
rep_keyboard_data_t, *pRep_keyboard_data_t;


/* Device Attributes */
typedef mouse_data_t rep_mouse_data_t, *pRep_mouse_data_t;
typedef joystick_attrib_t rep_joystick_attrib_t, *pRep_joystick_attrib_t;
typedef touch_attrib_t rep_touch_attrib_t, *pRep_touch_attrib_t;
typedef control_attrib_t rep_control_attrib_t, *pRep_control_attrib_t;


// Static variables
// 
static struct hidd_connection *pConnection;

static
LIST_HEAD (_modList, _module_data)
	modList;											// List of the registred modules
		 static pthread_mutex_t mod_mutex;	// Use this mutex for safe modules list modification


// Prototypes+
// 
static void insertion (struct hidd_connection *, hidd_device_instance_t * instance);
static void removal (struct hidd_connection *, hidd_device_instance_t * instance);
static void report (struct hidd_connection *, struct hidd_report *handle, void *report_data, _uint32 report_len, _uint32 flags, void *user);
static void event (struct hidd_connection *, hidd_device_instance_t * instance, _uint16 type);
static void attach_keyboard_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection);
static void attach_mouse_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection);
static void attach_joystick_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection);
static void attach_touch_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection);
static void attach_control_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection);
static int accept_report (struct hidd_collection *pCollection, struct hidd_device_instance *pDevInstance, _uint16 nRepIndex, _uint16 nRepType, _uint16 nConnType, pReport_data_t * ppRepData);
static int attach_input_reports (pModule_data_t pModule, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection, _uint16 nRepClass, void *pPrivData);

static void report_keyboard (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData);
static void report_mouse (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData);
static void report_joystick (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData);
static void report_touch (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData);
static void report_control (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData);

static int kbd_devctrl (pModule_data_t pModule, int event, void *ptr, void *pPrivData);
static int mouse_devctrl (pModule_data_t pModule, int event, void *ptr, void *pPrivData);
//static int  touch_devctrl(pModule_data_t pModule, int event, void *ptr, void * pPrivData);
static void detach_reports (pReport_data_t pRepData);
static void remove_device_reports (hidd_device_instance_t * pInstance);
static void remove_device_reports_from_list (pReport_data_t pFirstRepData, hidd_device_instance_t * pDevInstance);
static void remove_device_data (hidd_device_instance_t * pDevInstance);
static void remove_device_data_from_list (pDevice_data_t pFirstDeviceData, hidd_device_instance_t * pDevInstance);
static void *alloc_device_data (pModule_data_t pModule, hidd_device_instance_t * pDevInstance);
static int is_mouse_report (hidd_device_instance_t * pInstance, struct hidd_collection *pCollection, int *pIsMouse);




/* Description: this function initializes HID client                                */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : Must be called before any other devi_hid... function                */
void devi_hid_init ()
{
	LIST_INIT (&modList);
	pConnection = NULL;

	if (EOK != pthread_mutex_init (&mod_mutex, NULL)) {
		char *pMsgTxt = "System error. Driver is terminating\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		exit (-1);
	}
}


/* Description: this function initializes HID client                                */
/* Input      : char * serv_path_name - server path name (if NULL, takes default    */
/*              (/dev/io-hid/)                                                     */
/* Output     : None                                                                */
/* Return     : EOK if OK or negative error value                                   */
/* Comment    : Must be called after registration of all HID input modules          */
/*              (see devi_register_hid_client)                                      */
int devi_hid_server_connect (char *serv_path_name)
{

	int rc = EOK;
	hidd_connect_parm_t hparm = {NULL, HID_VERSION, HIDD_VERSION, 0, 0, 0, 0, HIDD_CONNECT_WAIT};
	hidd_funcs_t hf = {_HIDDI_NFUNCS, insertion, removal, report, event};
	hidd_device_ident_t interest = {HIDD_CONNECT_WILDCARD, HIDD_CONNECT_WILDCARD, HIDD_CONNECT_WILDCARD};

	hparm.path = serv_path_name;
	hparm.funcs = &hf;
	hparm.device_ident = &interest;

	if (EOK != (rc = hidd_connect (&hparm, &pConnection))) {
		char *pMsgTxt = "Error: cannot connect to HID server (error code = %d).\nDriver is terminating \n";

		fprintf (stderr, pMsgTxt, rc);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, rc);
		exit (-2);
	}

	return EOK;
}

/* Description: this function detach  HID client from server                        */
/* Input      : None                                                                */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void devi_hid_server_disconnect ()
{
	if (NULL != pConnection)
		hidd_disconnect (pConnection);

	pConnection = NULL;
}


/* Description: This function attach input module to HID USB sub-system.            */
/* Input      : input_module_t * pInput_module - module that requests registration  */
/*              int nDev - optional, USB device number (see comment)                */
/* Output     : None                                                                */
/* Return     : local module handler or NULL if error                               */
/* Comment    : Register your client from reset function of each device module. If  */
/*              nDev >= 0, it specifies device number.                              */
void * devi_hid_register_client (input_module_t * pInput_module, int nDev)
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
		LIST_INIT (&pNewMod->devDataList);
		LIST_INIT (&pNewMod->inpRepList);
		LIST_INIT (&pNewMod->outRepList);
		LIST_INIT (&pNewMod->featureRepList);

		pNewMod->pInput_module = pInput_module;
		pNewMod->nDev = nDev;
	}

	pthread_mutex_unlock (&mod_mutex);

	return pNewMod;
}


/* Description: This function detach input module from HID USB sub-system.          */
/* Input      : void * h - local module handler that was returned by                */
/*              devi_register_hid_client function                                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : Must be called before any other devi_hid... function                */
void devi_unregister_hid_client (void *h)
{
	pModule_data_t pMod = (pModule_data_t) h;
	pDevice_data_t pDeviceData;
	struct timespec t;

	assert (pMod);

	clock_gettime (CLOCK_REALTIME, &t);
	t.tv_sec += MAX_TIME_WAIT;

	if (EOK != pthread_mutex_timedlock (&mod_mutex, &t))
		return;

	LIST_REMOVE (pMod, lst_conn);

	detach_reports (LIST_FIRST_ITEM (&(pMod->inpRepList)));
	detach_reports (LIST_FIRST_ITEM (&(pMod->outRepList)));
	detach_reports (LIST_FIRST_ITEM (&(pMod->featureRepList)));

	// Remove private data of all connected devices
	pDeviceData = LIST_FIRST_ITEM (&(pMod->devDataList));

	while (NULL != pDeviceData) {
		pDevice_data_t pTmpDeviceData = LIST_NEXT_ITEM (pDeviceData, lst_conn);

		if (NULL != pDeviceData->pPrivData)
			free (pDeviceData->pPrivData);

		LIST_REMOVE (pDeviceData, lst_conn);
		pDeviceData = pTmpDeviceData;
	}

	free (pMod);

	pthread_mutex_unlock (&mod_mutex);
}

/* Description: This is a callback function; HID driver calls it each time new      */
/*              device, that matches specified filter is detected                   */
/* Input      : struct hidd_connection * - connection handler (we ignore it)        */
/*              hidd_device_instance_t * pInstance - device instance handler        */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void insertion (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance)
{
	struct hidd_collection **pCollections;
	_uint16 usage_page, usage;
	_uint16 nColl;
	int i;
	struct timespec t;
	int is_mouse = 0;

	// Try to switch to REPORT protocol
	hidd_set_protocol (pConnection, pInstance, HID_PROTOCOL_REPORT);

	// get root level collections 
	if (EOK != hidd_get_collections (pInstance, NULL, &pCollections, &nColl))
		return;

	clock_gettime (CLOCK_REALTIME, &t);
	t.tv_sec += MAX_TIME_WAIT;
	if (EOK != pthread_mutex_timedlock (&mod_mutex, &t))
		return;

	if (verbosity >= 3)
		printf ("Device insertion: device instance = %p, device no = %i\n", pInstance, pInstance->devno);

	for (i = 0; i < nColl; ++i) {
		if (EOK != hidd_collection_usage (pCollections[i], &usage_page, &usage))
			break;

		switch (usage_page) {
		case HIDD_PAGE_DESKTOP:
			switch (usage) {
			case HIDD_USAGE_POINTER:
				{
					if (!is_mouse_report (pInstance, pCollections[i], &is_mouse)
							&& !is_mouse) {
						// This doesn't look nice: I don't find any standard touch screen
						// report description. On the contrary, I found an example of Semtech 
						// touch screen which provides reports with pointer usage. In order
						// to sparate this sort device from real mice, I double check type
						// of valueas for coordinate Report (relative for mice, absolute for 
						// touch screens
						attach_touch_reports (pConnection, pInstance, pCollections[i]);
						break;
					}

					attach_mouse_reports (pConnection, pInstance, pCollections[i]);
					break;
				}
			case HIDD_USAGE_MOUSE:
				{
					if (!is_mouse_report (pInstance, pCollections[i], &is_mouse)
							&& !is_mouse) {
						// This doesn't look nice: I don't find any standard touch screen
						// report description. On the contrary, I found an example of Semtech
						// touch screen which provides reports with Mouse usage. In order
						// to sparate this sort device from real mice, I double check type
						// of valueas for coordinate Report (relative for mice, absolute for
						// touch screens
						attach_touch_reports (pConnection, pInstance, pCollections[i]);
						break;
					}

					attach_mouse_reports (pConnection, pInstance, pCollections[i]);
					break;
				}
			case HIDD_USAGE_JOYSTICK:
				attach_joystick_reports (pConnection, pInstance, pCollections[i]);
				break;
			case HIDD_USAGE_GAMEPAD:
attach_joystick_reports (pConnection, pInstance, pCollections[i]);
				break;
				
			case HIDD_USAGE_KEYBOARD:
				attach_keyboard_reports (pConnection, pInstance, pCollections[i]);
				break;
			default:
				// Not support 
				break;
			}
			break;
		case HIDD_PAGE_KEYBOARD:
			break;
		case HIDD_PAGE_CONSUMER:
			switch (usage) {
			case HIDD_USAGE_CONSUMER_CONTROL:
				attach_control_reports (pConnection, pInstance, pCollections[i]);
				break;
			}
			break;
		case HIDD_PAGE_DIGITIZER:
			switch (usage) {
			case HIDD_USAGE_TOUCH_SCREEN:
				attach_touch_reports (pConnection, pInstance, pCollections[i]);
				break;
			default:
				// Not supported
				break;
			}
			break;
		default:
			break;;
		}
	}


	pthread_mutex_unlock (&mod_mutex);

}


/* Description: This is a callback function; HID driver calls it each time when     */
/*              device is removed                                                   */
/* Input      : struct hidd_connection * - connection handler (we ignore it)        */
/*              hidd_device_instance_t * pInstance - device instance handler        */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void removal (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance)
{
	struct timespec t;

	clock_gettime (CLOCK_REALTIME, &t);
	t.tv_sec += MAX_TIME_WAIT;
	
	if (EOK != pthread_mutex_timedlock (&mod_mutex, &t))
		return;

	if (verbosity >= 3)
		printf ("Device Removal: device instance = %p, device no = %i\n", pInstance, pInstance->devno);

	remove_device_reports (pInstance);
	remove_device_data (pInstance);
	hidd_reports_detach (pConnection, pInstance);

	pthread_mutex_unlock (&mod_mutex);
}

/* Description: Service function; attaches report and create control structure      */
/* Input      : hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              _uint16 nRepIndex - report index in collection                      */
/*              _uint16 nRepType - input, output or feature (see hidut.h for        */
/*              manifest constants)                                                 */
/*              _uint16 nConnType - exclusive or receive always (see hidut.h for    */
/*              manifest constants)                                                 */
/* Output     : pReport_data_t * ppRepData - report data block that HID library     */
/*              allocates for this report                                           */
/* Return     : EOK if OK, otherwise negative error code from HID client library    */
/* Comment    : None                                                                */
int accept_report (struct hidd_collection *pCollection, struct hidd_device_instance *pDevInstance, _uint16 nRepIndex, _uint16 nRepType, _uint16 nConnType, pReport_data_t * ppRepData)
{
	int rc = EOK;
	_uint16 nRepLen;
	struct hidd_report_instance *pRepInstance;
	struct hidd_report *pReport;

	if (EOK != (rc = hidd_get_report_instance (pCollection, nRepIndex, nRepType, &pRepInstance)))
		return rc;

	hidd_report_len (pRepInstance, &nRepLen);

	// Attach report and allocate space to store keyboard data
	if (EOK == (rc = hidd_report_attach (pConnection, pDevInstance, pRepInstance, nConnType, nRepLen + sizeof (report_data_t), &pReport))) {
		if (verbosity >= 3)
			printf ("Report attach: pReport = %p, report instance = %p, report data = %p\n", pReport, pRepInstance, *ppRepData);

		*ppRepData = (pReport_data_t) hidd_report_extra (pReport);
		(*ppRepData)->pRepInstance = pRepInstance;
		(*ppRepData)->pReport = pReport;
		(*ppRepData)->pDevInstance = pDevInstance;
		(*ppRepData)->pCollection = pCollection;
	}
	if (EOK != rc) {
		char *pMsgTxt = "hidd_report_attach failed (%i)\n";

		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, rc);
		return rc;
	}

	return rc;
}


/* Description: Service function; can be called while report insertion              */
/* Input      : pModule_data_t pModule - pointer to module descriptor               */
/*            : hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              _uint8 nProtocol - HID_REPORT_TYPE_PROTOCOL or HID_BOOT_TYPEPROTOCOL*/
/*              _uint16 nRepClass - one of supported classes (see repClasses)       */
/* Output     : None                                                                */
/* Return     : EOK if OK or eror code                                              */
/* Comment    : None                                                                */
int attach_input_reports (pModule_data_t pModule, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection, _uint16 nRepClass, void *pPrivData)
{
	pReport_data_t pRepData = NULL;
	struct hidd_collection **pCollections;	// Array of next level collections
	_uint16 usage, usage_page;
	_uint16 nColl;								// Number of subcollections
	int i;
	int rc;

	hidd_collection_usage (pCollection, &usage_page, &usage);

	// As for now, I have no idea what to do with feature report. I just include this code for future
	// Does feature report exist?

	for (i = 0; EOK == (rc = accept_report (pCollection, pInstance, i, HID_INPUT_REPORT, 0,	//HIDD_REPORT_EXCLUSIVE,
																					&pRepData)); ++i) {
		LIST_INSERT_HEAD (&(pModule->inpRepList), pRepData, lst_conn);
		pRepData->nRepType = nRepClass;
		pRepData->usage_page = usage_page;
		pRepData->usage = usage;
		pRepData->pModule = pModule;
		pRepData->pPrivData = pPrivData;
	}

	// Try next level
	if (EOK == hidd_get_collections (NULL, pCollection, &pCollections, &nColl)) {
		// Recursively call to process all embedded collections
		for (i = 0; i < nColl; ++i)
			attach_input_reports (pModule, pInstance, pCollections[i], nRepClass, pPrivData);
	}

	return ((NULL != LIST_FIRST_ITEM (&(pModule->inpRepList))) ? EOK : ENOENT);
}



/* Description: This is a callback function; HID driver calls it each time when     */
/*              USB report comes                                                    */
/* Input      : struct hidd_connection * - connection handler (we ignore it)        */
/*              struct hidd_report *handle - report handle                          */
/*              void *report_data - pointer to raw report data                      */
/*              _uint32 report_len - reportb length                                 */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              void *user - pointer to user data, attached to the report           */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void report (struct hidd_connection *pConnection, struct hidd_report *handle, void *report_data, _uint32 report_len, _uint32 flags, void *user)
{
	pReport_data_t pRepData = (pReport_data_t) user;

	int x;

	pConnection = pConnection;
	
	if (NULL == pRepData)
		return;

	if (verbosity >= 8)
	{
		fprintf (stderr, "Received a HID Report - Handle %p, Data %p, Type %d\n", handle, pRepData, pRepData->nRepType);

		for ( x = 0 ; x < report_len ; x++ )
				    printf("%02x ", *( ((_uint8 * )report_data )+x) );

		printf( "\n");
	}


	switch (pRepData->nRepType) {
	case HIDD_KEYBOARD_REPORT:
		report_keyboard (handle, report_data, report_len, flags, pRepData);
		break;
	case HIDD_MOUSE_REPORT:
		report_mouse (handle, report_data, report_len, flags, pRepData);
		break;
	case HIDD_GAMEPAD_REPORT:
	case HIDD_JOYSTICK_REPORT:
		report_joystick (handle, report_data, report_len, flags, pRepData);
		break;
	case HIDD_TOUCHSCREEN_REPORT:
		report_touch (handle, report_data, report_len, flags, pRepData);
		break;
	case HIDD_CONTROL_REPORT:
		report_control (handle, report_data, report_len, flags, pRepData);
		break;
	default:
		break;
	}

}

/* Description: This is a callback function; HID driver calls it each time when     */
/*              USB report comes                                                    */
/* Input      : struct hidd_connection * - connection handler (we ignore it)        */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              _uint16 type                                                        */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : This is a future extension for the driver events                    */
void event (struct hidd_connection *pConnection, hidd_device_instance_t * instance, _uint16 type)
{
	pConnection = pConnection;
}


/* Description: Service function; can be called while insertion of keyboard report  */
/* Input      : hidd_connection * pConnection - connection                          */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_collection * pCollection - report collection                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void attach_keyboard_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection)
{
	pModule_data_t pModule;				// List of the registred modules
	_uint16 nButtons, nLeds = 0;

	pReport_data_t pLedRepData = NULL, pFeatureRepData = NULL;
	pRep_keyboard_data_t pKeyboardData = NULL;
	struct devctl_setkbd devctl;
	_uint16 nLedState = 0;
	int rc;

	pConnection = pConnection;

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule; pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		if ((HIDD_CONNECT_WILDCARD != pModule->nDev) && (pInstance->devno != pModule->nDev))
			continue;
		if (!(pModule->pInput_module->type & DEVI_CLASS_KBD))
			continue;
		if (verbosity >= 3)
			printf ("Attach keyboard report\n");

		if (NULL == (pKeyboardData = (pRep_keyboard_data_t) alloc_device_data (pModule, pInstance))) {
			if (verbosity)
				fprintf (stderr, "Error: not enough memory\n");
			return;
		}
		// Does output report (LEDs) exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_OUTPUT_REPORT, 0,	// HIDD_REPORT_EXCLUSIVE,
															&pLedRepData)) {
			hidd_num_buttons (pLedRepData->pRepInstance, &nLeds);
			pLedRepData->usage_page = HIDD_PAGE_LEDS;
			pLedRepData->usage = HIDD_USAGE_KEYBOARD;
			pLedRepData->pModule = pModule;
			pLedRepData->pPrivData = pKeyboardData;
			LIST_INSERT_HEAD (&(pModule->outRepList), pLedRepData, lst_conn);
		}
		// As for now, I have no idea what to do with feature report. I just include this code for future
		// Does feature report exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_FEATURE_REPORT, 0,	//HIDD_REPORT_EXCLUSIVE,
															&pFeatureRepData)) {
			pFeatureRepData->usage_page = HIDD_PAGE_UNDEFINED;
			pFeatureRepData->usage = HIDD_USAGE_UNDEFINED;
			pFeatureRepData->pModule = pModule;
			pFeatureRepData->pPrivData = pKeyboardData;
			LIST_INSERT_HEAD (&(pModule->featureRepList), pFeatureRepData,
												lst_conn);
		}
		// And attach all input report
		if (EOK !=
				(rc =
				 attach_input_reports (pModule, pInstance, pCollection,
															 HIDD_KEYBOARD_REPORT, pKeyboardData))) {
			char *pMsgTxt =
				"Cannot attach keyboard input report (error code = %i)\n";
			fprintf (stderr, pMsgTxt, rc);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, rc);
			remove_device_data (pInstance);
			return;
		}
		// Keyboard usually has only one input report - pressed keys
		hidd_num_buttons ((LIST_FIRST_ITEM (&(pModule->inpRepList)))->
											pRepInstance, &nButtons);

		pKeyboardData->kbdData.nLeds = nLeds;
		pKeyboardData->kbdData.nKeys = nButtons;

		pKeyboardData->nLastPressed = -1;
		pKeyboardData->tRep = 0;

		pModule->pInput_module->devctrl (pModule->pInput_module, DEVCTL_GETKBD,
																		 &devctl);
		// Request top level about default keyboard parameters
		// (This can be changed any moment by calling devi_hid_devctl)
//     devctl.delay = pKeyboardData -> kbdData.nDelay;
//     devctl.rate  = pKeyboardData -> kbdData.nRate;
		kbd_devctrl (pModule, DEVCTL_SETKBD, &devctl, pKeyboardData);	// Reset keyboard
		kbd_devctrl (pModule, DEVCTL_LED, &nLedState, pKeyboardData);	// Switch off leds

		break;


	}

}



/* Description: Service function; can be called while insertion of mouse report     */
/* Input      : hidd_connection * pConnection - connection                          */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_collection * pCollection - report collection                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void
attach_mouse_reports (struct hidd_connection *pConnection,
											hidd_device_instance_t * pInstance,
											struct hidd_collection *pCollection)
{
	pModule_data_t pModule;				// List of the registred modules
	pRep_mouse_data_t pMouseData;
	_uint16 nButtons;
	pReport_data_t pFeatureRepData = NULL;
	pReport_data_t pRepData;
	_uint8 nProtocolId;
	int rc;

	pConnection = pConnection;

	// Try to find if any module cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule;
			 pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		if ((HIDD_CONNECT_WILDCARD != pModule->nDev)
				&& (pInstance->devno != pModule->nDev))
			continue;
		if (!(pModule->pInput_module->type & DEVI_CLASS_REL))
			continue;

		if (verbosity >= 3)
			printf ("Attach mouse report\n");

		if (NULL ==
				(pMouseData =
				 (pRep_mouse_data_t) alloc_device_data (pModule, pInstance))) {
			if (verbosity)
				fprintf (stderr, "Error: not enough memory\n");
			return;
		}
		// As for now, I have no idea what to do with feature report. I just include this code for future
		// Does feature report exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_FEATURE_REPORT, 0,	//HIDD_REPORT_EXCLUSIVE,
															&pFeatureRepData)) {
			pFeatureRepData->usage_page = HIDD_PAGE_UNDEFINED;
			pFeatureRepData->usage = HIDD_USAGE_UNDEFINED;
			pFeatureRepData->pModule = pModule;
			pFeatureRepData->pPrivData = pMouseData;
			LIST_INSERT_HEAD (&(pModule->featureRepList), pFeatureRepData,
												lst_conn);
			if (verbosity >= 3)
				printf ("Mouse feature report has been attached\n");
		}
		// And attach all input report
		if (EOK !=
				(rc =
				 attach_input_reports (pModule, pInstance, pCollection,
															 HIDD_MOUSE_REPORT, pMouseData))) {
			char *pMsgTxt = "Cannot attach mouse input report (error code %i)\n";

			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, rc);
			remove_device_data (pInstance);
			return;
		}
		// Find number of buttons.
		// We iterate through input report list because we don't 
		// know in which report are buttons included
		for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList));
				 NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
			hidd_num_buttons (pRepData->pRepInstance, &nButtons);
			if (0 != nButtons) {
				pMouseData->nButtons = nButtons;
				if (verbosity >= 3)
					printf ("Mouse has %i available buttons\n", (int) nButtons);
				break;
			}
		}

		pMouseData->flags = 0;
		if ((EOK ==
				 (rc = hidd_get_protocol (pConnection, pInstance, &nProtocolId)))
				&& (HID_PROTOCOL_REPORT == nProtocolId)) {
			pMouseData->flags = HID_MOUSE_HAS_WHEEL | HID_MOUSE_WHEEL_ON;
		}

		break;
	}


}


/* Description: Service function; can be called while insertion of joystick report  */
/* Input      : hidd_connection * pConnection - connection                          */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_collection * pCollection - report collection                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void attach_joystick_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection)
{
	pModule_data_t pModule;             // List of the registred modules
	pRep_joystick_attrib_t pJoystickAttrib;
	_uint16 nButtons;
	pReport_data_t pRepData;
	pReport_data_t pFeatureRepData = NULL;
	int rc;

	pConnection = pConnection, pInstance = pInstance, pCollection = pCollection;

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule; pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {

		if ((HIDD_CONNECT_WILDCARD != pModule->nDev) && (pInstance->devno != pModule->nDev))
			continue;

		if (!(pModule->pInput_module->type & DEVI_CLASS_JOYSTICK))
			continue;

		if (verbosity >= 3)
			printf ("Attached joystick report\n");

		if (NULL == (pJoystickAttrib = (pRep_joystick_attrib_t) alloc_device_data (pModule, pInstance))) {
			if (verbosity)
				fprintf (stderr, "Error: not enough memory\n");
			return;
		}

		// As for now, I have no idea what to do with feature report. I just include this code for future
		// Does feature report exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_FEATURE_REPORT, 0, //HIDD_REPORT_EXCLUSIVE,
			&pFeatureRepData)) {
			pFeatureRepData->usage_page = HIDD_PAGE_UNDEFINED;
			pFeatureRepData->usage = HIDD_USAGE_UNDEFINED;
			pFeatureRepData->pModule = pModule;
			pFeatureRepData->pPrivData = pJoystickAttrib;
			LIST_INSERT_HEAD (&(pModule->featureRepList), pFeatureRepData, lst_conn);
		}

		// And attach all input report
		if (EOK != (rc = attach_input_reports (pModule, pInstance, pCollection, HIDD_JOYSTICK_REPORT, pJoystickAttrib))) {
			char *pMsgTxt = "Cannot attach input report\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			remove_device_data (pInstance);
			return;
		}

		// Find number of buttons.
		// We iterate through input report list because we don't
		// know in which report are buttons included
		for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList)); NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
			hidd_num_buttons (pRepData->pRepInstance, &nButtons);

			if (0 != nButtons) {
				pJoystickAttrib->nButtons = nButtons;

				if (verbosity >= 3)
					printf ("Joystick has %i available buttons\n", (int) nButtons);

				break;
			}
		}
		
		pJoystickAttrib->flags = 0;
		break;
	}
}



/* Description: Service function; can be called while insertion of joystick report  */
/* Input      : hidd_connection * pConnection - connection                          */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_collection * pCollection - report collection                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void attach_control_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection)
{
	pModule_data_t pModule;             // List of the registred modules
	pRep_control_attrib_t pDeviceAttrib;
	_uint16 nButtons;
	pReport_data_t pRepData;
	pReport_data_t pFeatureRepData = NULL;
	int rc;

	pConnection = pConnection, pInstance = pInstance, pCollection = pCollection;

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule; pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		if ((HIDD_CONNECT_WILDCARD != pModule->nDev) && (pInstance->devno != pModule->nDev))
			continue;

		if (!(pModule->pInput_module->type & DEVI_CLASS_CONTROL))
			continue;

		if (verbosity >= 3)
			printf ("Attached control report\n");

		if (NULL == (pDeviceAttrib = (pRep_control_attrib_t) alloc_device_data (pModule, pInstance))) {
			if (verbosity)
				fprintf (stderr, "Error: not enough memory\n");
			return;
		}

		// As for now, I have no idea what to do with feature report. I just include this code for future
		// Does feature report exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_FEATURE_REPORT, 0, &pFeatureRepData)) {
			pFeatureRepData->usage_page = HIDD_PAGE_UNDEFINED;
			pFeatureRepData->usage = HIDD_USAGE_UNDEFINED;
			pFeatureRepData->pModule = pModule;
			pFeatureRepData->pPrivData = pDeviceAttrib;
			LIST_INSERT_HEAD (&(pModule->featureRepList), pFeatureRepData, lst_conn);
		}

		// And attach all input report
		if (EOK != (rc = attach_input_reports (pModule, pInstance, pCollection, HIDD_CONTROL_REPORT, pDeviceAttrib))) {
			char *pMsgTxt = "Cannot attach input report\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			remove_device_data (pInstance);
			return;
		}

		// Find number of buttons.
		// We iterate through input report list because we don't
		// know in which report are buttons included
		for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList)); NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
			hidd_num_buttons (pRepData->pRepInstance, &nButtons);

			if (0 != nButtons) {
				pDeviceAttrib->nButtons = nButtons;

				if (verbosity >= 3)
					printf ("Control Device has %i available buttons\n", (int) nButtons);

				break;
			}
		}

		break;
	}
}


/* Description: Service function; can be called while inserting touch screen report */
/* Input      : hidd_connection * pConnection - connection                          */
/*              hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/*              hidd_collection * pCollection - report collection                   */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void attach_touch_reports (struct hidd_connection *pConnection, hidd_device_instance_t * pInstance, struct hidd_collection *pCollection)
{
	pModule_data_t pModule;             // List of the registred modules
	pRep_touch_attrib_t pDeviceAttrib;
	_uint16 nButtons;
	pReport_data_t pRepData;
	pReport_data_t pFeatureRepData = NULL;
	int rc;

	pConnection = pConnection;

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule;
			 pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		if ((HIDD_CONNECT_WILDCARD != pModule->nDev)
				&& (pInstance->devno != pModule->nDev))
			continue;

		if (!(pModule->pInput_module->type & DEVI_CLASS_ABS))
			continue;

		if (verbosity >= 3)
			printf ("Attached touch screen report\n");

		if (NULL == (pDeviceAttrib = (pRep_touch_attrib_t) alloc_device_data (pModule, pInstance))) {
			if (verbosity)
				fprintf (stderr, "Error: not enough memory\n");
			return;
		}

		// As for now, I have no idea what to do with feature report. I just include this code for future
		// Does feature report exist?
		if (EOK == accept_report (pCollection, pInstance, 0, HID_FEATURE_REPORT, 0,	//HIDD_REPORT_EXCLUSIVE,
															&pFeatureRepData)) {
			pFeatureRepData->usage_page = HIDD_PAGE_UNDEFINED;
			pFeatureRepData->usage = HIDD_USAGE_UNDEFINED;
			pFeatureRepData->pModule = pModule;
			pFeatureRepData->pPrivData = pDeviceAttrib;
			LIST_INSERT_HEAD (&(pModule->featureRepList), pFeatureRepData, lst_conn);
		}

		// And attach all input report
		if (EOK != (rc = attach_input_reports (pModule, pInstance, pCollection, HIDD_TOUCHSCREEN_REPORT, pDeviceAttrib))) {
			char *pMsgTxt = "Cannot attach input report\n";

			fprintf (stderr, pMsgTxt);
			slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
			remove_device_data (pInstance);
			return;
		}

		// Find number of buttons.
		// We iterate through input report list because we don't
		// know in which report are buttons included
		for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList)); NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
			hidd_num_buttons (pRepData->pRepInstance, &nButtons);

			if (0 != nButtons) {
				pDeviceAttrib->nButtons = nButtons;

				if (verbosity >= 3)
					printf ("Touchscreen has %i available buttons\n", (int) nButtons);

				break;
			}
		}

		break;
	}
}

/* Description: Service function; can be called when any keyboard report comes      */
/* Input      : struct hidd_report * pReport - report handle                        */
/*              void * pReportData - pointer to raw report data                     */
/*              _uint32 nRepLen - report length                                     */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              pReport_data_t pPrivData - pointer to user data, attached to the    */
/*              report                                                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void
report_keyboard (struct hidd_report *pReport, void *pReportData,
								 _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData)
{
	pRep_keyboard_data_t pKbdData;
	_uint16 usages[MAX_KEYS_IN_BUFFER];
	_uint16 pressed[MAX_KEYS_IN_BUFFER + 1];	// We don't use the first item of array for storing usages
	_uint16 released[MAX_KEYS_IN_BUFFER + 1];	// We don't use the first item of array for storing usages
	_uint16 nKeys, nPressed, nRealPressed, nReleased;
	struct timespec timestamp;
	_uint32 tNow;
	int i;
	input_module_t *pInput_module;	// Pointer to input module descriptor

	flags = flags;


	if (NULL == (pKbdData = (pRep_keyboard_data_t) (pPrivData->pPrivData)))
		return;

	nPressed = nReleased = nKeys = ARRAY_SIZE (usages);

	if (EOK != hidd_get_buttons (pPrivData->pRepInstance,
															 pPrivData->pCollection,
															 HIDD_PAGE_KEYBOARD,
															 pReportData, usages, &nKeys))
		return;

	clk_get (&timestamp);					// Current time
	tNow = MSEC (timestamp);			// Convert it to msecs

	/* Europe 1 shares the same make/break codes as the "\ |" key, our lookup table doesn't handle the Europe 1 key correctly, so
	   convert the Usage ID so we can correctly handle this key in the lookup table. */
	for (i = 0; i < nKeys; ++i) {
		if (usages[i] == 0x32)
			usages[i] = 0x31;
	}

	hidd_button_list_diff (usages, pKbdData->anUsages, pressed + 1, &nPressed);	// New pressed buttons
	nRealPressed = nPressed;
	hidd_button_list_diff (pKbdData->anUsages, usages, released + 1, &nReleased);	// Released buttons
	// hidd_button_list_same(usages, &pKey(pPrivData, 0), repeated, &nRepeated);

	if ((verbosity >= 3) && (0 != nKeys)) {
		printf ("keys = %i, pressed = %i, released = %i, keys:", nKeys, nPressed,
						nReleased);
		for (i = 0; i < nKeys; ++i) {
			printf ("%#x ", (int) usages[i]);
		}
		printf ("\n");
	}
	// Implementation of pressed key repeating algorithm

	if (nPressed > 0)							// Take from new preseed keys
	{

		for (i = 0; i < nKeys; ++i) {
			if (pressed[1] == usages[i]) {
				pKbdData->nLastPressed = i;
				pKbdData->tRep = tNow;
				break;
			}
		}
	} else if (pKbdData->nLastPressed >= 0)	// If previous "last pressed" is still pressed, update its index
	{
		int nLastPressed = pKbdData->nLastPressed;

		pKbdData->nLastPressed = -1;
		for (i = 0; i < nKeys; ++i) {
			if (usages[i] == pKbdData->anUsages[nLastPressed])	// Still pressed
			{
				pKbdData->nLastPressed = i;
				break;
			}
		}
	}
	// If there is "repeat pending" key?
	if (pKbdData->nLastPressed >= 0) {
		if (pKbdData->tRep)					// This key is in DELAY state
		{
			if (((unsigned) (pKbdData->tRep) + pKbdData->kbdData.nDelay) < tNow)	// State should be changed
			{
				pKbdData->tRep = 0;			// Now it is in repeat state
			}
		}
		if ((0 == pKbdData->tRep) &&	// key is in REPEAT state
				(0 != pKbdData->kbdData.nRate))	// Otherwise never repeat!
		{
			pressed[++nPressed] = usages[pKbdData->nLastPressed];	// Repeat it
		}
	}


	memset (&(pKbdData->anUsages[0]), 0, sizeof (pKbdData->anUsages));
	if (nKeys > 0)
		memcpy (pKbdData->anUsages, usages, sizeof (usages[0]) * nKeys);
	if ((0 == nPressed) && (0 == nReleased))	// No data to send up!
		return;


	// And send all pressed and released keys to the input module.
	// We use the first item of each array to indicate if this is either
	// an array of pressed or an array of released keys
	pressed[0] = 1;
	released[0] = 0;
	pInput_module = pPrivData->pModule->pInput_module;

	if (verbosity >= 5)
		printf ("Keyboard input report is transferred to the upper level\n");
	if (nPressed > 0) {
		(pInput_module->input) (pInput_module,
														sizeof (pressed[0]) * (nPressed + 1),
														(void *) pressed);
	}
	if (nReleased > 0) {
		(pInput_module->input) (pInput_module,
														sizeof (released[0]) * (nReleased + 1),
														(void *) released);
	}


}


/* Description: Service function; can be called when any mouse report comes         */
/* Input      : struct hidd_report * pReport - report handle                        */
/*              void * pReportData - pointer to raw report data                     */
/*              _uint32 nRepLen - report length                                     */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              pReport_data_t pPrivData - pointer to user data, attached to the    */
/*              report                                                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void
report_mouse (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen,
							_uint32 flags, pReport_data_t pPrivData)
{
	pRep_mouse_data_t pMouseData;
	_uint16 usages[MAX_BUTTONS];
	_uint16 nKeys;
	_uint32 nValue;
	int i;
	static _uint8 aButtonFlags[] = {
		_POINTER_BUTTON_LEFT,				// Left button
		_POINTER_BUTTON_RIGHT,			// Right button
		_POINTER_BUTTON_MIDDLE,			// Middle button
		_POINTER_BUTTON_4,					// First additional button
		_POINTER_BUTTON_5,					// Second additional button
	};
	input_module_t *pInput_module;	// Pointer to input module descriptor
	mouse_raw_data_t mouseRawData;


	flags = flags;

	if (NULL == (pMouseData = (pRep_mouse_data_t) (pPrivData->pPrivData)))
		return;

	// Is there buttons data?
	mouseRawData.btnStates = 0;

	nKeys = sizeof (usages) / sizeof (usages[0]);

	if (EOK == hidd_get_buttons (pPrivData->pRepInstance,
															 pPrivData->pCollection,
															 HIDD_PAGE_BUTTONS,
															 pReportData, usages, &nKeys)) {

		for (i = 0; i < nKeys; ++i) {
			int nInd = usages[i] - 1;	// Button usages start from 1

			assert (nInd < ARRAY_SIZE (aButtonFlags));
			mouseRawData.btnStates |= aButtonFlags[nInd];
		}
	}

	// Is there pointer data?
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_X, pReportData, &nValue))
		mouseRawData.x = (_int16) nValue;
	else
		mouseRawData.x = 0;
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Y, pReportData, &nValue))
		mouseRawData.y = (_int16) nValue;
	else
		mouseRawData.y = 0;

/*
	if ( !hidd_get_scaled_usage_value(pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_HAT_SWITCH, pReportData, &nValue) )
			    fprintf( stderr, "Scaled hatswtch=%d\n", nValue);
*/

	// Is there Wheel data ?
	if ((pMouseData->flags & HID_MOUSE_WHEEL_ON)
	   && ((EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_WHEEL, pReportData, &nValue))
	   || (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Z, pReportData, &nValue)))) {
		mouseRawData.z = (_int16) nValue;
	} else {
		mouseRawData.z = 0;
	}

	if (verbosity >= 3) {
		printf ("Raw mouse data: buttons=%#x, x = %d, y = %d, z = %d\n",
						(int) mouseRawData.btnStates,
						(int) mouseRawData.x, (int) mouseRawData.y, (int) mouseRawData.z);
	}
	// And send mouse data to input module. Mouse data must be transferred in mouse_data_t 
	// structure (see hid.h)
	pInput_module = pPrivData->pModule->pInput_module;
	(pInput_module->input) (pInput_module, sizeof (mouseRawData),
													(void *) &mouseRawData);

}


/* Description: Service function; can be called when any joystick report comes      */
/* Input      : struct hidd_report * pReport - report handle                        */
/*              void * pReportData - pointer to raw report data                     */
/*              _uint32 nRepLen - report length                                     */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              pReport_data_t pPrivData - pointer to user data, attached to the    */
/*              report                                                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */

void report_joystick (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData)
{
	pRep_joystick_attrib_t pMouseData;
	_uint16 usages[32];
	_uint16 nKeys;
	_uint32 nValue;
	int i;

	input_module_t *pInput_module;  // Pointer to input module descriptor
	joystick_raw_data_t raw_data;

	memset (&raw_data, 0, sizeof (raw_data));
	
	flags = flags;

	if (NULL == (pMouseData = (pRep_joystick_attrib_t) (pPrivData->pPrivData))) 
		return;

// Is there buttons data?

	nKeys = sizeof (usages) / sizeof (usages[0]);

	if (EOK == hidd_get_buttons (pPrivData->pRepInstance, pPrivData->pCollection, HIDD_PAGE_BUTTONS, pReportData, usages, &nKeys)) {
		if (nKeys)
			for (i = 0; i < nKeys; ++i) {
				raw_data.button_state |= 1 << (usages[i] - 1);
			}
	}

	/* Fetch positional data */
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_X, pReportData, &nValue))
		raw_data.x = (_int16) nValue;

	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Y, pReportData, &nValue))
		raw_data.y = (_int16) nValue;

	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Z, pReportData, &nValue))
		raw_data.z = (_int16) nValue;

	/* Fetch rotational data */
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_RX, pReportData, &nValue))
		raw_data.Rx = (_int16) nValue;

	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_RY, pReportData, &nValue))
		raw_data.Ry = (_int16) nValue;

	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_RZ, pReportData, &nValue))
		raw_data.Rz = (_int16) nValue;

	/* Fetch slider data */
//	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_SLIDER, pReportData, &nValue))
//		raw_data.slider = (_int16) nValue;

	/* Fetch HAT data */
/*	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_HAT_SWITCH, pReportData, &nValue))
		raw_data.hat_switch = (_int16) nValue;
	if ( !hidd_get_scaled_usage_value(pPrivData->pRepInstance, NULL, 1, HIDD_USAGE_HAT_SWITCH, pReportData, &nValue) )
		fprintf( stderr, "Scaled hatswtch=%d\n", nValue);

*/
	
	if (verbosity >= 3)
		fprintf (stderr, "Raw joystick data: x:%d, y:%d, z:%d, buttons:%llu, Rx:%d, Ry:%d, Rz:%d, Slider:%d, Hat Switch:%d\n",
			raw_data.x, raw_data.y, raw_data.z, raw_data.button_state, raw_data.Rx, raw_data.Ry, raw_data.Rz, raw_data.slider, raw_data.hat_switch);

	// And send data to input module. Joystick data must be transferred _data_t
	// structure (see hid.h)
	pInput_module = pPrivData->pModule->pInput_module;
	(pInput_module->input) (pInput_module, sizeof (raw_data), (void *) &raw_data);

	pReport = pReport, pReportData = pReportData, nRepLen = nRepLen, flags = flags, pPrivData = pPrivData;
}



/* Description: Service function; can be called when touchscreen report comes       */
/* Input      : struct hidd_report * pReport - report handle                        */
/*              void * pReportData - pointer to raw report data                     */
/*              _uint32 nRepLen - report length                                     */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              pReport_data_t pPrivData - pointer to user data, attached to the    */
/*              report                                                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */
void report_touch (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData)
{
	_uint32 nValue;
	_uint16 nKeys;
	_uint16 usages[MAX_BUTTONS];
	//pRep_touch_attrib_t pDeviceAttrib;

	input_module_t *pInput_module;	// Pointer to input module descriptor
	touch_raw_data_t touchRawData;

	memset (&touchRawData, 0, sizeof (touchRawData));

	flags = flags;

//	if (NULL == (pDeviceAttrib = (pRep_touch_attrib_t) (pPrivData->pPrivData)))
//		return;

	// Is there buttons data?
	nKeys = sizeof (usages) / sizeof (usages[0]);

	if (EOK == hidd_get_buttons (pPrivData->pRepInstance, pPrivData->pCollection, HIDD_PAGE_BUTTONS, pReportData, usages, &nKeys)) {
		touchRawData.touched = usages[0] ? _POINTER_BUTTON_LEFT : 0L;
	//} else if (EOK != hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_TIP_SWITCH, pReportData, &nValue)) {
	} 

	/* Digitizer devices sometimes have tip switches */
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DIGITIZER, 0x42, pReportData, &nValue)) {
		touchRawData.touched = nValue ? _POINTER_BUTTON_LEFT : 0L;
	}

	// Is there pointer data?
	if (EOK != hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_X, pReportData, &nValue))
		return;
	touchRawData.x = (_int16) nValue;

	if (EOK != hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Y, pReportData, &nValue))
		return;
	touchRawData.y = (_int16) nValue;

	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Z, pReportData, &nValue))
  	touchRawData.z = (_int16) nValue;

	if (verbosity >= 3) {
		printf ("Raw HID touch screen data: %s, x = %d, y = %d, z = %d\n",
						touchRawData.touched ? "touched" : "released",
						(int) touchRawData.x, (int) touchRawData.y, (int) touchRawData.z);
	}
	// And send mouse data to input module. Mouse data must be transferred in mouse_data_t 
	// structure (see hid.h)
	pInput_module = pPrivData->pModule->pInput_module;
	(pInput_module->input) (pInput_module, sizeof (touchRawData), (void *) &touchRawData);
}


/* Description: Service function; can be called when any joystick report comes      */
/* Input      : struct hidd_report * pReport - report handle                        */
/*              void * pReportData - pointer to raw report data                     */
/*              _uint32 nRepLen - report length                                     */
/*              _uint32 flags - flags (have no idea about their meanings)           */
/*              pReport_data_t pPrivData - pointer to user data, attached to the    */
/*              report                                                              */
/* Output     : None                                                                */
/* Return     : None                                                                */
/* Comment    : None                                                                */

void report_control (struct hidd_report *pReport, void *pReportData, _uint32 nRepLen, _uint32 flags, pReport_data_t pPrivData)
{
	pRep_control_attrib_t pMouseData;
	_uint16 usages[32];
	_uint16 nKeys;
	_uint32 nValue;
	int i;

	input_module_t *pInput_module;  // Pointer to input module descriptor
	control_raw_data_t raw_data;

	memset (&raw_data, 0, sizeof (raw_data));

	flags = flags;

	if (NULL == (pMouseData = (pRep_control_attrib_t) (pPrivData->pPrivData)))
		return;

	// Is there buttons data?

	nKeys = sizeof (usages) / sizeof (usages[0]);

	if (pPrivData->pRepInstance == NULL)
			fprintf (stderr, "Instance is NULL\n");

	if (EOK == hidd_get_buttons (pPrivData->pRepInstance, pPrivData->pCollection, HIDD_PAGE_BUTTONS, pReportData, usages, &nKeys)) {
		if (nKeys)
			for (i = 0; i < nKeys; ++i) {
				raw_data.button_state |= 1 << (usages[i] - 1);
			}
	}

	/* Fetch rotational data */
	if (EOK == hidd_get_usage_value (pPrivData->pRepInstance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_RX, pReportData, &nValue))
		raw_data.Rx = (_int16) nValue;

	if (verbosity >= 3)
		fprintf (stderr, "Raw Control data: buttons:%x, Rx:%d\n", raw_data.button_state, raw_data.Rx);

	// And send data to input module. Joystick data must be transferred _data_t
	// structure (see hid.h)
	pInput_module = pPrivData->pModule->pInput_module;
	(pInput_module->input) (pInput_module, sizeof (raw_data), (void *) &raw_data);

	pReport = pReport, pReportData = pReportData, nRepLen = nRepLen, flags = flags, pPrivData = pPrivData;
}




/* Description: this is a callback function for DEVCTRL command processing          */
/* Input      : void * pHandle - local module handler                               */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/* Output     : None                                                                */
/* Return     : 0 if OK, otherwise negative error code                              */
/* Comment    : None                                                                */
int
devi_hid_devctrl (void *pHandle, int event, void *ptr, int nDev)
{
	pModule_data_t pModule = (pModule_data_t) pHandle;	// List of the registred modules
	int rc = EOK;
	pDevice_data_t pDeviceData;

	assert (pHandle);


	for (pDeviceData = LIST_FIRST_ITEM (&(pModule->devDataList));
			 NULL != pDeviceData;
			 pDeviceData = LIST_NEXT_ITEM (pDeviceData, lst_conn)) {
		if ((nDev != HIDD_CONNECT_WILDCARD)
				&& (nDev != pDeviceData->pDevInstance->devno))
			continue;									// Not for this device
		switch (pModule->pInput_module->type & DEVI_CLASS_MASK) {
		case DEVI_CLASS_KBD:
			rc = kbd_devctrl (pModule, event, ptr, pDeviceData->pPrivData);
			break;
		case DEVI_CLASS_REL:
			rc = mouse_devctrl (pModule, event, ptr, pDeviceData->pPrivData);
			break;
		default:
			rc = ENOTSUP;
		}
		if (EOK != rc)
			break;
	}

	return rc;
}


/* Description: this is a callback function for keyboard DEVCTRL command processing */
/* Input      : pModule_data_t pModule - pointer to module descriptor               */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/*              void * pPrivData - private device data block                        */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise error code                                     */
/* Comment    : None                                                                */
int
kbd_devctrl (pModule_data_t pModule, int event, void *ptr, void *pPrivData)
{
	int rc = EOK;

	pRep_keyboard_data_t pKbdData = (pRep_keyboard_data_t) (pPrivData);

	switch (event) {

	case DEVCTL_GETKBD:
		if (NULL != ptr) {					// Get keyboard parameters - delay & rate 
			struct devctl_getkbd *pDevctl = (struct devctl_getkbd *) ptr;

			pDevctl->rate = pKbdData->kbdData.nRate;
			pDevctl->delay = pKbdData->kbdData.nDelay;
		}
		break;

	case DEVCTL_SETKBD:
		if (NULL != ptr) {					// Set KB rate and delay 
			struct devctl_setkbd *pDevctl = (struct devctl_setkbd *) ptr;

			// Set idle interval (this is a mechanizm of repeat key implementation)
			// Take idle rate for the first input report (this is a pressed keys report)
			pReport_data_t pRepData;

			pKbdData->kbdData.nDelay = pDevctl->delay;
			pKbdData->kbdData.nRate = pDevctl->rate;

			for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList));
					 NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)
				) {
				rc = hidd_set_idle (pRepData->pReport, (_uint16) (pDevctl->rate));
			}
		}
		break;
	case DEVCTL_LED:
		if (NULL != ptr) {
			_uint16 anLedFlags[] =
				{ KEYIND_SCROLL_LOCK, KEYIND_NUM_LOCK, KEYIND_CAPS_LOCK };
			_uint16 anLedUsages[] =
				{ HIDD_USAGE_SCROLL_LOCK, HIDD_USAGE_NUM_LOCK, HIDD_USAGE_CAPS_LOCK };
			int i;
			short nLeds = *(short *) ptr;
			_uint16 nRepLen;

			pReport_data_t pLedReport;
			void *pRepData;

			for (pLedReport = LIST_FIRST_ITEM (&(pModule->outRepList));
					 NULL != pLedReport;
					 pLedReport = LIST_NEXT_ITEM (pLedReport, lst_conn)
				) {
				if (EOK !=
						(rc = hidd_report_len (pLedReport->pRepInstance, &nRepLen)))
					break;

				pRepData = malloc (nRepLen);
				if (NULL == pRepData) {
					rc = ENOMEM;
					break;
				}
				memset (pRepData, 0, nRepLen);
				for (i = 0; i < ARRAY_SIZE (anLedFlags); ++i) {
					rc = hidd_set_usage_value (pLedReport->pRepInstance, NULL,	//pLedReport -> pCollection, 
																		 HIDD_PAGE_LEDS, anLedUsages[i],
																		 (nLeds & anLedFlags[i]) ? 1 : 0,
																		 pRepData, nRepLen);
				}
				rc = hidd_send_report (pLedReport->pReport, pRepData);
				free (pRepData);
			}
		}

		break;

	default:
		rc = ENOTSUP;
		break;
	}

	return (rc);
}



/* Description: this is a callback function for mouse DEVCTRL command processing    */
/* Input      : pModule_data_t pModule - pointer to module descriptor               */
/*              int event  - DEVCTRL command code                                   */
/*              void * ptr - pointer to data exchange block                         */
/*              void * pPrivData - private device data block                        */
/* Output     : None                                                                */
/* Return     : EOK if OK, otherwise error code                                     */
/* Comment    : None                                                                */
int
mouse_devctrl (pModule_data_t pModule, int event, void *ptr, void *pPrivData)
{
	int rc = EOK;

	pRep_mouse_data_t pMouseData = (pRep_mouse_data_t) (pPrivData);

	switch (event) {
	case DEVCTL_GETMOUSETYPE:
		/* Get mouse type - PS2 legacy support               */
		if (NULL != ptr) {
			struct devctl_mouse_types *pDevctl = (struct devctl_mouse_types *) ptr;

			if (!(pMouseData->flags & HID_MOUSE_HAS_WHEEL))
				pDevctl->type = NO_WHEEL_MOUSE;
			if (!(pMouseData->flags & HID_MOUSE_WHEEL_ON))
				pDevctl->curtype = NO_WHEEL_MOUSE;
			if (4 > pMouseData->nButtons)
				pDevctl->curtype = pDevctl->type = WHEEL_3B_MOUSE;
			else
				pDevctl->curtype = pDevctl->type = WHEEL_5B_MOUSE;
		}
		break;
	case DEVCTL_SETMOUSETYPE:
		/* Set mouse type   - PS2 legacy support              */
		if (NULL != ptr) {
			// Do nothing now; I'm still thinking should be implemented this function or no.
		}
	case DEVCTL_GETSAMPLE_RATE:
		/* Get sample rate   - PS2 legacy support              */
		if (NULL != ptr) {
			// Take idle rate for the first input report
			pReport_data_t pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList));
			_uint16 nIdleRate;

			if (NULL == pRepData)			// Report hasn't still attached
				break;
			if (EOK == (rc = hidd_get_idle (pRepData->pReport, &nIdleRate)))
				*(int *) ptr = nIdleRate;
		}
		break;
	case DEVCTL_SETSAMPLE_RATE:
		/* Set sample rate   - PS2 legacy support              */
		if (NULL != ptr) {
			pReport_data_t pRepData;
			_uint8 nNewIdleRate = (_uint8) * (int *) ptr;

			if (nNewIdleRate > 0) {
				// Change idle rate for pointer report
				for (pRepData = LIST_FIRST_ITEM (&(pModule->inpRepList));
						 NULL != pRepData;
						 pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
					if (HIDD_USAGE_POINTER == pRepData->usage) {
						hidd_set_idle (pRepData->pReport, nNewIdleRate);
						break;
					}
				}
			}
		}
		break;

	case DEVCTL_GET_RESOLUTION:
	case DEVCTL_SET_RESOLUTION:
		/* PS2 legacy support              */
		if (NULL != ptr) {
			// Do nothing now; I'm still thinking should be implemented this function or no.
		}
	default:
		rc = ENOTSUP;
	}

	return rc;
}


void
detach_reports (pReport_data_t pRepData)
{
	for (; NULL != pRepData; pRepData = LIST_NEXT_ITEM (pRepData, lst_conn)) {
		hidd_report_detach (pRepData->pReport);
	}
}


void
remove_device_reports (hidd_device_instance_t * pDevInstance)
{
	pModule_data_t pModule;				// List of the registred modules

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule;
			 pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		if (verbosity >= 3)
			printf ("Remove input report - %p\n",
							LIST_FIRST_ITEM (&(pModule->inpRepList)));
		remove_device_reports_from_list (LIST_FIRST_ITEM (&(pModule->inpRepList)),
																		 pDevInstance);
		if (verbosity >= 3)
			printf ("Remove output report - %p\n",
							LIST_FIRST_ITEM (&(pModule->outRepList)));
		remove_device_reports_from_list (LIST_FIRST_ITEM (&(pModule->outRepList)),
																		 pDevInstance);
		if (verbosity >= 3)
			printf ("Remove feature report - %p\n",
							LIST_FIRST_ITEM (&(pModule->featureRepList)));
		remove_device_reports_from_list (LIST_FIRST_ITEM
																		 (&(pModule->featureRepList)),
																		 pDevInstance);
	}
}

void
remove_device_reports_from_list (pReport_data_t pFirstRepData,
																 hidd_device_instance_t * pDevInstance)
{
	pReport_data_t pRepData = pFirstRepData;

	while (NULL != pRepData) {
		if (pRepData->pDevInstance == pDevInstance) {
			pReport_data_t pTmpRepData = LIST_NEXT_ITEM (pRepData, lst_conn);

			LIST_REMOVE (pRepData, lst_conn);
			// Don't remove report data itself - it is a part of memory, allocated in HID library
			pRepData = pTmpRepData;
		} else
			pRepData = LIST_NEXT_ITEM (pRepData, lst_conn);
	}
}

void
remove_device_data (hidd_device_instance_t * pDevInstance)
{
	pModule_data_t pModule;				// List of the registred modules

	// Try to find if somebody cares about keyboard reports
	for (pModule = LIST_FIRST_ITEM (&modList); NULL != pModule;
			 pModule = LIST_NEXT_ITEM (pModule, lst_conn)) {
		remove_device_data_from_list (LIST_FIRST_ITEM (&(pModule->devDataList)),
																	pDevInstance);
	}
}

void
remove_device_data_from_list (pDevice_data_t pFirstDeviceData,
															hidd_device_instance_t * pDevInstance)
{
	pDevice_data_t pDeviceData = pFirstDeviceData;

	while (NULL != pDeviceData) {
		if (pDeviceData->pDevInstance == pDevInstance) {
			pDevice_data_t pTmpDeviceData = LIST_NEXT_ITEM (pDeviceData, lst_conn);

			if (NULL != pDeviceData->pPrivData)
				free (pDeviceData->pPrivData);
			LIST_REMOVE (pDeviceData, lst_conn);
			free (pDeviceData);
			pDeviceData = pTmpDeviceData;
		} else
			pDeviceData = LIST_NEXT_ITEM (pDeviceData, lst_conn);
	}

}

void *alloc_device_data (pModule_data_t pModule, hidd_device_instance_t * pDevInstance)
{
	void *pPrivData = NULL;
	pDevice_data_t pDeviceData = NULL;	// Device data block

	switch (pModule->pInput_module->type & DEVI_CLASS_MASK) {
	case DEVI_CLASS_KBD:
		if (NULL == (pPrivData = malloc (sizeof (rep_keyboard_data_t))))
			return NULL;
		memset (pPrivData, 0, sizeof (rep_keyboard_data_t));
		break;
	case DEVI_CLASS_REL:
		if (NULL == (pPrivData = malloc (sizeof (rep_mouse_data_t))))
			return NULL;
		memset (pPrivData, 0, sizeof (rep_mouse_data_t));
		break;
	case DEVI_CLASS_ABS:
		if (NULL == (pPrivData = malloc (sizeof (rep_touch_attrib_t))))
			return NULL;
		memset (pPrivData, 0, sizeof (rep_touch_attrib_t));
		break;
	case DEVI_CLASS_JOYSTICK:
		if (NULL == (pPrivData = malloc (sizeof (rep_joystick_attrib_t))))
			return NULL;

		memset (pPrivData, 0, sizeof (rep_joystick_attrib_t));
		break;
	case DEVI_CLASS_CONTROL:
		if (NULL == (pPrivData = malloc (sizeof (rep_control_attrib_t))))
			return NULL;
		
		memset (pPrivData, 0, sizeof (rep_control_attrib_t));
		break;
	default:
		break;
	}

	if (NULL == (pDeviceData = malloc (sizeof (device_data_t)))) {
		if (NULL != pPrivData)
			free (pPrivData);
		return NULL;
	}
	pDeviceData->pPrivData = pPrivData;
	LIST_INSERT_HEAD (&(pModule->devDataList), pDeviceData, lst_conn);
	pDeviceData->pDevInstance = pDevInstance;

	return pPrivData;
}



/* Description: Service function; can be called to detect is it really mouse        */
/*              (on the contrary, maybe touch screen)                               */
/* Input      : hidd_device_instance_t * pDevInstance - device instance handler     */
/*              hidd_collection * pCollection - collection this report belongs to   */
/* Output     : int * pIsMouse - 1 if it is mouse, 0 for touch screen               */
/* Return     : 1 if detected or 0 if not detected                                  */
/* Comment    : None                                                                */
int
is_mouse_report (hidd_device_instance_t * pInstance,
								 struct hidd_collection *pCollection, int *pIsMouse)
{

	int i, j;
	struct hidd_report_instance *pRepInstance;
	struct hidd_collection **pCollections;	// Array of next level collections
	hidd_report_props_t *pReport_props;
	_uint16 nNumProps;
	_uint16 nPropsLen;
	_uint16 nColl;								// Number of subcollections

	for (i = 0;
			 EOK == hidd_get_report_instance (pCollection, i, HID_INPUT_REPORT,
																				&pRepInstance); ++i) {
		if ((EOK != hidd_get_num_props (pRepInstance, &nNumProps))
				|| (0 == nNumProps))
			continue;									/* We cannot determine anything */

		nPropsLen = sizeof (hidd_report_props_t) * nNumProps;
		pReport_props = malloc (nPropsLen);

		if (EOK ==
				hidd_get_report_props (pRepInstance, pReport_props, &nPropsLen))
			for (j = 0; j < nNumProps; ++j) {
				if (pReport_props[j].data_properties & HIDD_DATA_RELATIVE) {
					if (verbosity >= 3)
						fprintf (stderr,
										 "Relative data stream detected (Mouse attached)!\n");

					*pIsMouse = 1;
					free (pReport_props);

					return 1;
				}
			}

		free (pReport_props);
	}

	// Try next level
	if (EOK == hidd_get_collections (NULL, pCollection, &pCollections, &nColl)) {
		// Recursively call to process all embedded collections
		for (i = 0; i < nColl; ++i) {
			if (is_mouse_report (pInstance, pCollections[i], pIsMouse))
				return 1;
		}
	}

	return 0;
}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/hid.c $ $Rev: 657836 $" );
