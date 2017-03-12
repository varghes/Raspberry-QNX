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








#ifndef __DCMD_UCB1400_H_INCLUDED__
#define __DCMD_UCB1400_H_INCLUDED__

#include <sys/dcmd_input.h>

__BEGIN_DECLS

#include <_pack64.h>

/*--------------------------------------------------------------------------------*/

#define UCB1400_DEV_NAME "/dev/ucb1400"  
  
#define	_DCMD_UCB1400            (0x777f)	/* Start of range of UCB1400 devctls in the devctrl space */

typedef struct _ucb1400_read_write_data {   
    _Uint32t n_reg_addr;
    _Uint32t n_reg_data;
} ucb1400_read_write_data_t, * pucb1400_read_write_data_t;
  

#define DEVCTL_UCB1400_VERSION  (0x01)
#define DEVCTL_UCB1400_READ     (0x03)
#define DEVCTL_UCB1400_WRITE    (0x04)


#define DCMD_UCB1400_VERSION __DIOF(_DCMD_UCB1400, DEVCTL_UCB1400_VERSION,   int)
#define DCMD_UCB1400_READ    __DIOTF(_DCMD_UCB1400, DEVCTL_UCB1400_READ,   ucb1400_read_write_data_t)
#define DCMD_UCB1400_WRITE   __DIOT(_DCMD_UCB1400, DEVCTL_UCB1400_WRITE,  ucb1400_read_write_data_t)


/*--------------------------------------------------------------------------------*/


#include <_packpop.h>

__END_DECLS

#endif /*  __DCMD_UCB1400_H_INCLUDED__ */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/public/sys/dcmd_ucb1400.h $ $Rev: 657836 $" )
