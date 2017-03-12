/*
 * $QNXLicenseC: 
 * Copyright 2011, QNX Software Systems.  
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


#ifndef	__ARM_MX6X_IOMUX_H_INCLUDED
#define	__ARM_MX6X_IOMUX_H_INCLUDED

#include "mx6dq_iomux.h"
#include "mx6sdl_iomux.h"

/*
 * Bit definitions for SW_MUX_CTL registers
 */
#define MUX_CTL_SION						(0x1 << 4)
#define MUX_CTL_MUX_MODE_ALT0					0
#define MUX_CTL_MUX_MODE_ALT1					1
#define MUX_CTL_MUX_MODE_ALT2					2
#define MUX_CTL_MUX_MODE_ALT3					3
#define MUX_CTL_MUX_MODE_ALT4					4
#define MUX_CTL_MUX_MODE_ALT5					5
#define MUX_CTL_MUX_MODE_ALT6					6
#define MUX_CTL_MUX_MODE_ALT7					7


/*
 * Bit definitions for SW_PAD_CTL registers
 */

#define PAD_CTL_HYS_DISABLE				(0x0 << 16)
#define PAD_CTL_HYS_ENABLE				(0x1 << 16)

#define PAD_CTL_PUS_100K_PD				(0x0 << 14)
#define PAD_CTL_PUS_47K_PU				(0x1 << 14)
#define PAD_CTL_PUS_100K_PU				(0x2 << 14)
#define PAD_CTL_PUS_22K_PU				(0x3 << 14)

#define PAD_CTL_PUE_PULL				(0x1 << 13)
#define PAD_CTL_PUE_KEEP				(0x0 << 13)

#define PAD_CTL_PKE_DISABLE				(0x0 << 12)
#define PAD_CTL_PKE_ENABLE				(0x1 << 12)

#define PAD_CTL_ODE_DISABLE				(0x0 << 11)
#define PAD_CTL_ODE_ENABLE				(0x1 << 11)

#define PAD_CTL_ODT
#define PAD_CTL_SPEED_TBD				(0x0 << 6)
#define PAD_CTL_SPEED_LOW				(0x1 << 6)
#define PAD_CTL_SPEED_MEDIUM				(0x2 << 6)
#define PAD_CTL_SPEED_MAX				(0x3 << 6)

#define PAD_CTL_DSE_DISABLE				(0x0 << 3)
#define PAD_CTL_DSE_240_OHM				(0x1 << 3)
#define PAD_CTL_DSE_120_OHM				(0x2 << 3)
#define PAD_CTL_DSE_80_OHM				(0x3 << 3)
#define PAD_CTL_DSE_60_OHM				(0x4 << 3)
#define PAD_CTL_DSE_48_OHM				(0x5 << 3)
#define PAD_CTL_DSE_40_OHM				(0x6 << 3)
#define PAD_CTL_DSE_34_OHM				(0x7 << 3)

#define PAD_CTL_SRE_SLOW				(0x0 << 0)
#define PAD_CTL_SRE_FAST				(0x1 << 0)

/*
 * Function prototypes
 */
void pinmux_set_swmux(int pin, int mux_config);
void pinmux_set_padcfg(int pin, int pad_config);
void pinmux_set_input(int pin, int input_config);


#endif	/* __ARM_MX6X_IOMUX_H_INCLUDED */

__SRCVERSION("$URL$ $Rev$");

