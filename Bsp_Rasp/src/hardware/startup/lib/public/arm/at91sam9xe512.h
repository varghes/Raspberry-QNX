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
 
 
#ifndef _AT91SAM9XE512_H__
#define _AT91SAM9XE512_H__

#include <arm/at91sam9xx.h>

/* External Bus Interface Mapping */
#define AT91_EBI0_CS0_BASE              0x10000000
#define AT91_EBI0_CS1_BASE              0x20000000  
#define AT91_EBI0_CS2_BASE              0x30000000
#define AT91_EBI0_CS3_BASE              0x40000000  
#define AT91_EBI0_CS4_BASE              0x50000000  
#define AT91_EBI0_CS5_BASE              0x60000000  
#define AT91_EBI0_CS6_BASE              0x70000000  
#define AT91_EBI0_CS7_BASE              0x80000000  

/* Peripheral Mapping */
#define AT91_TC_BASE			        0xfffa0000
#define AT91_UDP_BASE                   0xfffa4000
#define AT91_MCI_BASE                   0xfffa8000
#define AT91_TWI0_BASE                  0xfffac000
#define AT91_USART0_BASE                0xfffb0000
#define AT91_USART1_BASE                0xfffb4000
#define AT91_USART2_BASE                0xfffb8000
#define AT91_SSC_BASE			        0xfffbc000
#define AT91_ISI_BASE			        0xfffc0000
#define AT91_EMAC_BASE                  0xfffc4000
#define AT91_SPI0_BASE                  0xfffc8000
#define AT91_SPI1_BASE                  0xfffcc000
#define AT91_USART3_BASE                0xfffd0000
#define AT91_USART4_BASE                0xfffd4000
#define AT91_TWI1_BASE                  0xfffd8000
#define AT91_TCx_BASE			        0xfffdc000
#define AT91_ADC_BASE			        0xfffe0000
#define AT91_SYSC_BASE			        0xffffc000

/* System Controller Mapping Mapping */
#define AT91_ECC_BASE			        0xffffe800
#define AT91_SDRAMC_BASE                0xffffea00
#define AT91_SMC_BASE                   0xffffec00
#define AT91_MATRIX_BASE                0xffffee00
#define AT91_AIC_BASE                   0xfffff000
#define AT91_DBGU_BASE                  0xfffff200
#define AT91_PIOA_BASE                  0xfffff400
#define AT91_PIOB_BASE                  0xfffff600
#define AT91_PIOC_BASE                  0xfffff800
#define AT91_EEFC_BASE                  0xfffffa00
#define AT91_PMC_BASE                   0xfffffc00
#define AT91_RSTC_BASE                  0xfffffd00
#define AT91_SHDC_BASE                  0xfffffd10
#define AT91_RTCC_BASE                  0xfffffd20
#define AT91_PITC_BASE                  0xfffffd30
#define AT91_WDTC_BASE                  0xfffffd40
#define AT91_GPBR_BASE                  0xfffffd60

/* Peripheral Identifier */
#define AT91_INTR_AIC_FIQ                   0
#define AT91_INTR_SYSC                      1
#define AT91_INTR_PIOA                      2
#define AT91_INTR_PIOB                      3
#define AT91_INTR_PIOC                      4
#define AT91_INTR_ADC                       5
#define AT91_INTR_USART0                    6
#define AT91_INTR_USART1                    7
#define AT91_INTR_USART2                    8
#define AT91_INTR_MCI                       9
#define AT91_INTR_UDP                       10
#define AT91_INTR_TWI0                      11
#define AT91_INTR_SPI0                      12
#define AT91_INTR_SPI1                      13
#define AT91_INTR_SSC                       14
#define AT91_INTR_TC0                       17
#define AT91_INTR_TC1                       18
#define AT91_INTR_TC9                       19
#define AT91_INTR_UHP                       20      
#define AT91_INTR_EMAC                      21
#define AT91_INTR_ISI                       22
#define AT91_INTR_USART3                    23
#define AT91_INTR_USART4                    24
#define AT91_INTR_TWI1                      25
#define AT91_INTR_TC3                       26    
#define AT91_INTR_TC4                       27
#define AT91_INTR_TC5                       28
#define AT91_INTR_AIC_IRQ0                  29    
#define AT91_INTR_AIC_IRQ1                  30
#define AT91_INTR_AIC_IRQ2                  31

#endif /* #ifndef _AT91SAM9XE512_H__ */
