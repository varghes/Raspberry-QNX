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








#ifndef EXTERN_H_INCLUDED
#ifndef EXT
#define EXT extern
#endif

#define EXTERN_H_INCLUDED

#include <sys/devi.h>

extern int		optind, opterr, optopt;
extern char		*optarg;

EXT int			verbosity;

EXT event_bus_line_t	*Bus;	/* pointer to list of event bus lines 
				   managed by devi */

EXT int			IpGroup; /* input group (used by photon i/f) */
EXT char		*IpTargetDev; /* name of photon device to open */
EXT unsigned int        OptFlags; /* global options */
EXT int			IpCurrentFd; /* fd used to send messages via photon */
EXT input_module_t	*ModuleTable[MODULE_TABLE_SIZE]; /* table of currently
							    supported modules */
EXT common_callbacks_t commonCallbacks; /* Global callback functions     */
EXT char                *pServ_name;
EXT char                *pFull_process_name; // Full process name
EXT int					loopPlayback;
EXT FILE				*playFile;
EXT FILE				*logFile;
EXT	int					delayMsec;
EXT int         xRes; 
EXT int         yRes; 
#endif


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/externs.h $ $Rev: 657836 $" )
