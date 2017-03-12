/*
 * $QNXLicenseC: 
 * Copyright 2009, QNX Software Systems.  
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
 
 
#ifndef _AT91SAM9G45_H__
#define _AT91SAM9G45_H__

/* Internal Memory Mapping */
#define AT91SAM9G45_BOOT_BASE		0x00000000
#define AT91SAM9G45_ITCM_BASE		0x00100000
#define AT91SAM9G45_DTCM_BASE		0x00200000
#define AT91SAM9G45_SRAM_BASE		0x00300000
#define AT91SAM9G45_ROM_BASE		0x00400000
#define AT91SAM9G45_LCD_BASE		0x00500000
#define AT91SAM9G45_UDP_RAM_BASE	0x00600000

/* External Bus Interface Mapping */
#define AT91SAM9G45_EBI0_CS0_BASE               0x10000000
#define AT91SAM9G45_EBI0_CS1_BASE               0x20000000  
#define AT91SAM9G45_EBI0_CS2_BASE               0x30000000
#define AT91SAM9G45_EBI0_CS3_BASE               0x40000000  
#define AT91SAM9G45_EBI0_CS4_BASE               0x50000000  
#define AT91SAM9G45_EBI0_CS5_BASE               0x60000000  
#define AT91SAM9G45_DDR2LPDDR_CS_BASE           0x70000000  

/* Peripheral Mapping */
#define AT91SAM9G45_TC_BASE                     0xfff78000
#define AT91SAM9G45_MCI_BASE                    0xfff80000
#define AT91SAM9G45_TWI0_BASE                   0xfff84000
#define AT91SAM9G45_TWI1_BASE                   0xfff84000
#define AT91SAM9G45_USART0_BASE                 0xfff8c000
#define AT91SAM9G45_USART1_BASE                 0xfff90000
#define AT91SAM9G45_USART2_BASE                 0xfff94000
#define AT91SAM9G45_USART3_BASE                 0xfff98000
#define AT91SAM9G45_SSC0_BASE                   0xfff9c000
#define AT91SAM9G45_SSC1_BASE                   0xfffa0000
#define AT91SAM9G45_PWMC_BASE                   0xfffb8000
#define AT91SAM9G45_EMAC_BASE                   0xfffbc000
#define AT91SAM9G45_SPI0_BASE                   0xfffa4000
#define AT91SAM9G45_SPI1_BASE                   0xfffa8000
#define AT91SAM9G45_UDPHS_BASE                  0xfff78000
#define AT91SAM9G45_AC97C_BASE                  0xfffac000


/* System Controller Mapping Mapping */
#define AT91SAM9G45_DMAC_BASE                   0xffffec00
#define AT91SAM9G45_ECC_BASE                    0xffffe200
#define AT91SAM9G45_DDR2_SDRAMC_BASE            0xffffe600
#define AT91SAM9G45_SMC_BASE                    0xffffe800
#define AT91SAM9G45_MATRIX_BASE                 0xffffea00
#define AT91SAM9G45_AIC_BASE                    0xfffff000
#define AT91SAM9G45_DBGU_BASE                   0xffffee00
#define AT91SAM9G45_PIOA_BASE                   0xfffff200
#define AT91SAM9G45_PIOB_BASE                   0xfffff400
#define AT91SAM9G45_PIOC_BASE                   0xfffff600
#define AT91SAM9G45_PIOD_BASE                   0xfffff800
#define AT91SAM9G45_PIOE_BASE                   0xfffffa00
#define AT91SAM9G45_PMC_BASE                    0xfffffc00
#define AT91SAM9G45_RSTC_BASE                   0xfffffd00
#define AT91SAM9G45_SHDWC_BASE                  0xfffffd10
#define AT91SAM9G45_RTCC0_BASE                  0xfffffd20
#define AT91SAM9G45_PIT_BASE                    0xfffffd30
#define AT91SAM9G45_WDTC_BASE                   0xfffffd40
#define AT91SAM9G45_SCKCR_BASE                  0xfffffd50
#define AT91SAM9G45_GPBR_BASE                   0xfffffd60
#define AT91SAM9G45_RTCC1_BASE                  0xfffffdb0

/* Chip Configuration */
#define AT91SAM9G45_CCFG_SIZE                   0x0f0
#define AT91SAM9G45_CCFG_MATRIX_TCMR            0x004
#define AT91SAM9G45_CCFG_EBI_CSA                0x128

/* On-chip 32KHz RC oscillator startup time [us] */
#define AT91SAM9G45_RC_STARTUP_US		75

/* DMAC */
#define AT91SAM9G45_INTR_DMAC                   21

#define AT91SAM9G45_INTR_EMAC                   25

#include <arm/at91sam9xx.h>

#endif /* #ifndef _AT91SAM9G45_H__ */
