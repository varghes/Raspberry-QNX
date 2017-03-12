/*
 * $QNXLicenseC: 
 * Copyright 2009,2012 QNX Software Systems.  
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
 
 
#ifndef _AT91SAM9M10_H__
#define _AT91SAM9M10_H__

#include <arm/at91sam9xx.h>

 /* Internal Memory Mapping */
#define AT91_BOOT_BASE					0x00000000
#define AT91_ITCM_BASE					0x00100000
#define AT91_DTCM_BASE					0x00200000
#define AT91_SRAM_BASE					0x00300000
#define AT91_ROM_BASE					0x00400000
#define AT91_LCD_BASE					0x00500000
#define AT91_UDP_RAM_BASE				0x00600000
#define AT91_UHP_OHCI_BASE				0x00700000
#define AT91_UHP_EHCI_BASE				0x00800000
#define AT91_VDEC_BASE					0x00900000

/* External Bus Interface Mapping */
#define AT91_EBI0_CS0_BASE				0x10000000
#define AT91_EBI0_CS1_BASE				0x20000000  
#define AT91_EBI0_CS2_BASE				0x30000000
#define AT91_EBI0_CS3_BASE				0x40000000  
#define AT91_EBI0_CS4_BASE				0x50000000  
#define AT91_EBI0_CS5_BASE				0x60000000  
#define AT91_DDR2LPDDR_CS_BASE			0x70000000  

/* Peripheral Mapping */
#define AT91_PERIPHERAL_BASE			0xf0000000
#define AT91_UDPHS_BASE					0xfff78000
#define AT91_TC012_BASE					0xfff7C000
#define AT91_MCI0_BASE					0xfff80000
#define AT91_TWI0_BASE					0xfff84000
#define AT91_TWI1_BASE					0xfff88000
#define AT91_USART0_BASE				0xfff8c000
#define AT91_USART1_BASE				0xfff90000
#define AT91_USART2_BASE				0xfff94000
#define AT91_USART3_BASE				0xfff98000
#define AT91_SSC0_BASE					0xfff9c000
#define AT91_SSC1_BASE					0xfffa0000
#define AT91_SPI0_BASE					0xfffa4000
#define AT91_SPI1_BASE					0xfffa8000
#define AT91_AC97C_BASE					0xfffac000
#define AT91_PWMC_BASE					0xfffb8000
#define AT91_EMAC_BASE					0xfffbc000
#define AT91_MCI1_BASE					0xfffd0000
#define AT91_TC345_BASE					0xfffd4000

/* System Controller Mapping Mapping */
#define AT91_SYSTEM_CONTROLLER_BASE		0xffffc000
#define AT91_ECC_BASE			 		0xffffe200
#define AT91_DDR2_SDRAMC_BASE			0xffffe600
#define AT91_SMC_BASE					0xffffe800
#define AT91_MATRIX_BASE				0xffffea00
#define AT91_DMAC_BASE			 		0xffffec00
#define AT91_DBGU_BASE					0xffffee00
#define AT91_AIC_BASE					0xfffff000
#define AT91_PIOA_BASE					0xfffff200
#define AT91_PIOB_BASE					0xfffff400
#define AT91_PIOC_BASE					0xfffff600
#define AT91_PIOD_BASE					0xfffff800
#define AT91_PIOE_BASE					0xfffffa00
#define AT91_PMC_BASE					0xfffffc00
#define AT91_SHDWC_BASE					0xfffffd10
#define AT91_RTCC0_BASE					0xfffffd20
#define AT91_PIT_BASE					0xfffffd30
#define AT91_WDTC_BASE					0xfffffd40
#define AT91_SCKCR_BASE					0xfffffd50
#define AT91_GPBR_BASE					0xfffffd60
#define AT91_RTCC1_BASE					0xfffffdb0

/* Chip Configuration */
#define AT91_CCFG_SIZE					0xf0
#define AT91_CCFG_TCMR					0x110
#define AT91_CCFG_VMRCR					0x118
#define AT91_CCFG_EBI_CSA				0x128

/* Peripheral Identifier */
#define AT91_INTR_AIC_FIQ				0
#define AT91_INTR_SYSC					1
#define AT91_INTR_PIOA					2
#define AT91_INTR_PIOB					3
#define AT91_INTR_PIOC					4
#define AT91_INTR_PIOD_E				5
#define AT91_INTR_TRNG					6
#define AT91_INTR_USART0				7
#define AT91_INTR_USART1				8
#define AT91_INTR_USART2				9
#define AT91_INTR_MCI0					11
#define AT91_INTR_TWI0					12
#define AT91_INTR_TWI1					13
#define AT91_INTR_SPI0					14
#define AT91_INTR_SPI1					15
#define AT91_INTR_SSC0					16
#define AT91_INTR_SSC1					17
#define AT91_INTR_TC					18
#define AT91_INTR_PWM					19
#define AT91_INTR_TSADCC				20
#define AT91_INTR_DMAC					21
#define AT91_INTR_UHPHS					22
#define AT91_INTR_LCDC					23
#define AT91_INTR_AC97C					24
#define AT91_INTR_EMAC					25
#define AT91_INTR_ISI					26
#define AT91_INTR_UDPHS					27
#define AT91_INTR_MCI1					29
#define AT91_INTR_VDEC					30
#define AT91_INTR_AIC 					31

#endif /* #ifndef _AT91SAM9M10_H__ */
