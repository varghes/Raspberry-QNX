/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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


#ifndef _AT91SAM9263_H__
#define _AT91SAM9263_H__
/*
 * Atmel AT91SAM9263 SOC with ARM 926ES-J core.
 */


/*
* System On Chip (SOC) Memory Mapping.
*/

 /*
  * Peripheral ID Declaration and Interrupt Number.
  */
#define AT91SAM9263_ID_INUM_FIQ                 0
#define AT91SAM9263_ID_INUM_SYSC                1
#define AT91SAM9263_ID_INUM_PIOA                2
#define AT91SAM9263_ID_INUM_PIOB                3
#define AT91SAM9263_ID_INUM_PIOC                4
#define AT91SAM9263_ID_INUM_PIOD                AT91SAM9263_ID_INUM_PIOC
#define AT91SAM9263_ID_INUM_PIOE                AT91SAM9263_ID_INUM_PIOC
#define AT91SAM9263_ID_INUM_USART0              7
#define AT91SAM9263_ID_INUM_USART1              8
#define AT91SAM9263_ID_INUM_USART2              9
#define AT91SAM9263_ID_INUM_MCI0                10
#define AT91SAM9263_ID_INUM_MCI1                11
#define AT91SAM9263_ID_INUM_CAN                 12
#define AT91SAM9263_ID_INUM_TWI                 13
#define AT91SAM9263_ID_INUM_SPI0                14
#define AT91SAM9263_ID_INUM_SPI1                15
#define AT91SAM9263_ID_INUM_SSC0                16
#define AT91SAM9263_ID_INUM_SSC1                17
#define AT91SAM9263_ID_INUM_AC97C               18
#define AT91SAM9263_ID_INUM_TC0                 19
#define AT91SAM9263_ID_INUM_TC1                 AT91SAM9263_ID_INUM_TC0
#define AT91SAM9263_ID_INUM_TC2                 AT91SAM9263_ID_INUM_TC0
#define AT91SAM9263_ID_INUM_PWMC                20
#define AT91SAM9263_ID_INUM_EMAC                21
#define AT91SAM9263_ID_INUM_2DGE                23
#define AT91SAM9263_ID_INUM_UDP                 24
#define AT91SAM9263_ID_INUM_ISI                 25
#define AT91SAM9263_ID_INUM_LCDC                26
#define AT91SAM9263_ID_INUM_DMA                 27
#define AT91SAM9263_ID_INUM_UHP                 29
#define AT91SAM9263_ID_INUM_IRQ0                30
#define AT91SAM9263_ID_INUM_IRQ1                31
    
  /* Internal Memory Mapping */
#define AT91SAM9263_BOOT_BASE                   0x00000000
#define AT91SAM9263_ITCM_BASE                   0x00100000
#define AT91SAM9263_DTCM_BASE                   0x00200000
#define AT91SAM9263_SRAM_BASE                   0x00300000
#define AT91SAM9263_ROM_BASE                    0x00400000
#define AT91SAM9263_SRAM0_BASE                  0x00500000
#define AT91SAM9263_LCD_CONTROLLER_BASE         0x00700000
#define AT91SAM9263_DMAC_BASE                   0x00800000
#define AT91SAM9263_USB_HOST_BASE               0x00a00000

#define AT91SAM9263_BOOT_SIZE                   0x00100000
#define AT91SAM9263_ITCM_SIZE                   0x00100000
#define AT91SAM9263_DTCM_SIZE                   0x00100000
#define AT91SAM9263_SRAM_SIZE                   0x00100000
#define AT91SAM9263_ROM_SIZE                    0x00100000
#define AT91SAM9263_SRAM0_SIZE                  0x00100000
#define AT91SAM9263_LCD_CONTROLLER_SIZE         0x00100000
#define AT91SAM9263_DMAC_SIZE                   0x00100000
#define AT91SAM9263_USB_HOST_SIZE               0x00100000

  /* External Bus Interface 0 & 1 Mapping*/
#define AT91SAM9263_EBI0_CS0_BASE               0x10000000
#define AT91SAM9263_EBI0_CS1_BASE               0x20000000  /* SDRAMC    */
#define AT91SAM9263_EBI0_CS2_BASE               0x30000000
#define AT91SAM9263_EBI0_CS3_BASE               0x40000000  /* NANDFlash */
#define AT91SAM9263_EBI0_CS4_BASE               0x50000000  /* Compact Flash Slot 0 */
#define AT91SAM9263_EBI0_CS5_BASE               0x60000000  /* Compact Flash Slot 1 */
#define AT91SAM9263_EBI1_CS0_BASE               0x70000000
#define AT91SAM9263_EBI1_CS1_BASE               0x80000000  /* SDRAMC    */
#define AT91SAM9263_EBI1_CS2_BASE               0x90000000  /* NANDFlash */

#define AT91SAM9263_EBIX_CSX_MAX_SIZE           0x10000000  /* 256 MB           */

  /* Peripheral Mapping */
#define AT91SAM9263_UDP_BASE                    0xfff78000
#define AT91SAM9263_TC_BASE                     0xfff7c000
#define AT91SAM9263_MCI0_BASE                   0xfff80000
#define AT91SAM9263_MCI1_BASE                   0xfff84000
#define AT91SAM9263_TWI_BASE                    0xfff88000
#define AT91SAM9263_USART0_BASE                 0xfff8c000
#define AT91SAM9263_USART1_BASE                 0xfff90000
#define AT91SAM9263_USART2_BASE                 0xfff94000
#define AT91SAM9263_SSC0_BASE                   0xfff98000
#define AT91SAM9263_SSC1_BASE                   0xfff9c000
#define AT91SAM9263_AC97C_BASE                  0xfffa0000
#define AT91SAM9263_SPI0_BASE                   0xfffa4000
#define AT91SAM9263_SPI1_BASE                   0xfffa8000
#define AT91SAM9263_CAN0_BASE                   0xfffac000
#define AT91SAM9263_PWMC_BASE                   0xfffb8000
#define AT91SAM9263_EMAC_BASE                   0xfffbc000
#define AT91SAM9263_ISI_BASE                    0xfffc4000
#define AT91SAM9263_2DGC_BASE                   0xfffc8000

  /* System Controller Mapping Mapping */

#define AT91SAM9263_ECC0_BASE                   0xffffe000
#define AT91SAM9263_SDRAMC0_BASE                0xffffe200
#define AT91SAM9263_SMC0_BASE                   0xffffe400
#define AT91SAM9263_ECC1_BASE                   0xffffe600
#define AT91SAM9263_SDRAMC1_BASE                0xffffe800
#define AT91SAM9263_SMC1_BASE                   0xffffea00
#define AT91SAM9263_MATRIX_BASE                 0xffffec00
#define AT91SAM9263_CCFG_BASE                   0xffffed10
#define AT91SAM9263_DBGU_BASE                   0xffffee00
#define AT91SAM9263_AIC_BASE                    0xfffff000
#define AT91SAM9263_PIOA_BASE                   0xfffff200
#define AT91SAM9263_PIOB_BASE                   0xfffff400
#define AT91SAM9263_PIOC_BASE                   0xfffff600
#define AT91SAM9263_PIOD_BASE                   0xfffff800
#define AT91SAM9263_PIOE_BASE                   0xfffffa00
#define AT91SAM9263_PMC_BASE                    0xfffffc00
#define AT91SAM9263_RSTC_BASE                   0xfffffd00
#define AT91SAM9263_SHDWC_BASE                  0xfffffd10
#define AT91SAM9263_RTT0_BASE                   0xfffffd20
#define AT91SAM9263_PIT_BASE                    0xfffffd30
#define AT91SAM9263_WDT_BASE                    0xfffffd40
#define AT91SAM9263_RTT1_BASE                   0xfffffd50
#define AT91SAM9263_GPBR_BASE                   0xfffffd60

 /*
  * Peripheral Register Declaration.
  */

  /* USB Device Port (UDP) */
#define AT91SAM9263_UDP_SIZE                    0x100
#define AT91SAM9263_UDP_FRM_NUM                 0x00
#define AT91SAM9263_UDP_GLB_STAT                0x04
#define AT91SAM9263_UDP_FADDR                   0x08
#define AT91SAM9263_UDP_IER                     0x10
#define AT91SAM9263_UDP_IDR                     0x14
#define AT91SAM9263_UDP_IMR                     0x18
#define AT91SAM9263_UDP_ISR                     0x1c
#define AT91SAM9263_UDP_ICR                     0x20
#define AT91SAM9263_UDP_RST_EP                  0x28
#define AT91SAM9263_UDP_CSR(x)                  (0x30 + 4 * (x-1))
#define AT91SAM9263_UDP_CSR0                    0x30
#define AT91SAM9263_UDP_CSR1                    0x34
#define AT91SAM9263_UDP_CSR2                    0x38
#define AT91SAM9263_UDP_CSR3                    0x3c
#define AT91SAM9263_UDP_CSR4                    0x40
#define AT91SAM9263_UDP_CSR5                    0x44
#define AT91SAM9263_UDP_FDR(x)                  (0x50 + 4 * (x-1))
#define AT91SAM9263_UDP_FDR0                    0x50
#define AT91SAM9263_UDP_FDR1                    0x54
#define AT91SAM9263_UDP_FDR2                    0x58
#define AT91SAM9263_UDP_FDR3                    0x5c
#define AT91SAM9263_UDP_FDR4                    0x60
#define AT91SAM9263_UDP_FDR5                    0x64
#define AT91SAM9263_UDP_TXVC                    0x74

  /* Timer Counter (TC) */
#define AT91SAM9263_TC_SIZE                     0x100
#define AT91SAM9263_TC_CH(x)_OFFSET             (x * 0x40)
#define AT91SAM9263_TC_CH0                      0x00
#define AT91SAM9263_TC_CH1                      0x40
#define AT91SAM9263_TC_CH2                      0x80
#define AT91SAM9263_TC_CHx_CCR                  0x00
#define AT91SAM9263_TC_CHx_CMR                  0x04
#define AT91SAM9263_TC_CHx_CV                   0x10
#define AT91SAM9263_TC_CHx_RA                   0x14
#define AT91SAM9263_TC_CHx_RB                   0x18
#define AT91SAM9263_TC_CHx_RC                   0x1c
#define AT91SAM9263_TC_CHx_SR                   0x20
#define AT91SAM9263_TC_CHx_IER                  0x24
#define AT91SAM9263_TC_CHx_IDR                  0x28
#define AT91SAM9263_TC_CHx_IMR                  0x2c
#define AT91SAM9263_TC_BCR                      0xc0
#define AT91SAM9263_TC_BMR                      0xc4

  /* MultiMedia Card Interface (MCI) */
#define AT91SAM9263_MCI_SIZE                    0x50
#define AT91SAM9263_MCI_CR                      0x00
#define AT91SAM9263_MCI_MR                      0x04
#define AT91SAM9263_MCI_DTOR                    0x08
#define AT91SAM9263_MCI_SDCR                    0x0c
#define AT91SAM9263_MCI_ARGR                    0x10
#define AT91SAM9263_MCI_CMDR                    0x14
#define AT91SAM9263_MCI_BLKR                    0x18
#define AT91SAM9263_MCI_RSPR                    0x20
#define AT91SAM9263_MCI_RSPR_1                  0x24
#define AT91SAM9263_MCI_RSPR_2                  0x28
#define AT91SAM9263_MCI_RSPR_3                  0x2c
#define AT91SAM9263_MCI_RDR                     0x30
#define AT91SAM9263_MCI_TDR                     0x34
#define AT91SAM9263_MCI_SR                      0x40
#define AT91SAM9263_MCI_IER                     0x44
#define AT91SAM9263_MCI_IDR                     0x48
#define AT91SAM9263_MCI_IMR                     0x4c

  /* Two-wire Interface (TWI) */
#define AT91SAM9263_TWI_SIZE                    0x38
#define AT91SAM9263_TWI_CR                      0x00
#define AT91SAM9263_TWI_MMR                     0x04
#define AT91SAM9263_TWI_IADR                    0x0c
#define AT91SAM9263_TWI_CWGR                    0x10
#define AT91SAM9263_TWI_SR                      0x20
#define AT91SAM9263_TWI_IER                     0x24
#define AT91SAM9263_TWI_IDR                     0x28
#define AT91SAM9263_TWI_IMR                     0x2c
#define AT91SAM9263_TWI_RHR                     0x30
#define AT91SAM9263_TWI_THR                     0x34

  /* USART */
#define AT91SAM9263_USART_SIZE                  0x100
#define AT91SAM9263_USART_CR                    0x000
#define AT91SAM9263_USART_MR                    0x004
#define AT91SAM9263_USART_IER                   0x008
#define AT91SAM9263_USART_IDR                   0x00c
#define AT91SAM9263_USART_IMR                   0x010
#define AT91SAM9263_USART_CSR                   0x014
#define AT91SAM9263_USART_RHR                   0x018
#define AT91SAM9263_USART_THR                   0x01c
#define AT91SAM9263_USART_BRGR                  0x020
#define AT91SAM9263_USART_RTOR                  0x024
#define AT91SAM9263_USART_TTGR                  0x028
#define AT91SAM9263_USART_FIDI                  0x040 
#define AT91SAM9263_USART_NER                   0x044
#define AT91SAM9263_USART_IF                    0x04c

  /* Serial Synchronous Controller (SCC) */
#define AT91SAM9263_SSC_SIZE                    0x50
#define AT91SAM9263_SSC_CR                      0x00
#define AT91SAM9263_SSC_CMR                     0x00
#define AT91SAM9263_SSC_RCMR                    0x04
#define AT91SAM9263_SSC_RFMR                    0x10
#define AT91SAM9263_SSC_TCMR                    0x14
#define AT91SAM9263_SSC_TFMR                    0x18
#define AT91SAM9263_SSC_RHR                     0x1c
#define AT91SAM9263_SSC_THR                     0x20
#define AT91SAM9263_SSC_RSHR                    0x24
#define AT91SAM9263_SSC_TSHR                    0x
#define AT91SAM9263_SSC_RCxR(x)                 (0x38 + 4*x)
#define AT91SAM9263_SSC_RC0R                    0x38
#define AT91SAM9263_SSC_RC1R                    0x3c
#define AT91SAM9263_SSC_SR                      0x40
#define AT91SAM9263_SSC_IER                     0x44
#define AT91SAM9263_SSC_IDR                     0x48
#define AT91SAM9263_SSC_IMR                     0x4c

  /* AC'97 Controller (AC97C) */
#define AT91SAM9263_AC97C_SIZE                  0x60
#define AT91SAM9263_AC97C_MR                    0x08
#define AT91SAM9263_AC97C_ICA                   0x10
#define AT91SAM9263_AC97C_OCA                   0x14
#define AT91SAM9263_AC97C_CARHR                 0x20
#define AT91SAM9263_AC97C_CATHR                 0x24
#define AT91SAM9263_AC97C_CASR                  0x28
#define AT91SAM9263_AC97C_CAMR                  0x2c
#define AT91SAM9263_AC97C_CBRHR                 0x30
#define AT91SAM9263_AC97C_CBTHR                 0x34
#define AT91SAM9263_AC97C_CBSR                  0x38
#define AT91SAM9263_AC97C_CBMR                  0x3c
#define AT91SAM9263_AC97C_CORHR                 0x40
#define AT91SAM9263_AC97C_COTHR                 0x44
#define AT91SAM9263_AC97C_COSR                  0x48
#define AT91SAM9263_AC97C_COMR                  0x4c
#define AT91SAM9263_AC97C_SR                    0x50
#define AT91SAM9263_AC97C_IER                   0x54
#define AT91SAM9263_AC97C_IDR                   0x58
#define AT91SAM9263_AC97C_IMR                   0x5c

  /* Serial Peripheral Interface (SPI) */
#define AT91SAM9263_SPI_SIZE                    0x40
#define AT91SAM9263_SPI_CR                      0x00
#define AT91SAM9263_SPI_MR                      0x04
#define AT91SAM9263_SPI_RDR                     0x08
#define AT91SAM9263_SPI_TDR                     0x0c
#define AT91SAM9263_SPI_SR                      0x10
#define AT91SAM9263_SPI_IER                     0x14
#define AT91SAM9263_SPI_IDR                     0x18
#define AT91SAM9263_SPI_IMR                     0x1c
#define AT91SAM9263_SPI_CSR(x)                  (0x30 + x*4)
#define AT91SAM9263_SPI_CSR0                    0x30
#define AT91SAM9263_SPI_CSR1                    0x34
#define AT91SAM9263_SPI_CSR2                    0x38
#define AT91SAM9263_SPI_CSR3                    0x3c

  /* Controller Area Network (CAN) */
#define AT91SAM9263_CAN_SIZE                    0x240
#define AT91SAM9263_CAN_MR                      0x000
#define AT91SAM9263_CAN_IER                     0x004
#define AT91SAM9263_CAN_IDR                     0x008
#define AT91SAM9263_CAN_IMR                     0x00c
#define AT91SAM9263_CAN_SR                      0x010
#define AT91SAM9263_CAN_BR                      0x014
#define AT91SAM9263_CAN_TIM                     0x018
#define AT91SAM9263_CAN_TIMESTP                 0x01c
#define AT91SAM9263_CAN_ECR                     0x020
#define AT91SAM9263_CAN_TCR                     0x024
#define AT91SAM9263_CAN_ACR                     0x028
#define AT91SAM9263_CAN_MMR(x)                  (0x200 + 0x20*x)
#define AT91SAM9263_CAN_MMR0                    0x200
#define AT91SAM9263_CAN_MAM(x)                  (0x204 + 0x20*x)
#define AT91SAM9263_CAN_MAM0                    0x204
#define AT91SAM9263_CAN_MID(x)                  (0x208 + 0x20*x)
#define AT91SAM9263_CAN_MID0                    0x208
#define AT91SAM9263_CAN_MFID(x)                 (0x20c + 0x20*x)
#define AT91SAM9263_CAN_MFID0                   0x20c
#define AT91SAM9263_CAN_MSR(x)                  (0x210 + 0x20*x)
#define AT91SAM9263_CAN_MSR0                    0x210
#define AT91SAM9263_CAN_MDL(x)                  (0x214 + 0x20*x)
#define AT91SAM9263_CAN_MDL0                    0x214
#define AT91SAM9263_CAN_MDH(x)                  (0x218 + 0x20*x)
#define AT91SAM9263_CAN_MDH0                    0x218
#define AT91SAM9263_CAN_MCR(x)                  (0x21c + 0x20*x)
#define AT91SAM9263_CAN_MCR0                    0x21c
#define AT91SAM9263_CAN_MMR1                    0x220
#define AT91SAM9263_CAN_MAM1                    0x224
#define AT91SAM9263_CAN_MID1                    0x228
#define AT91SAM9263_CAN_MFID1                   0x22c
#define AT91SAM9263_CAN_MSR1                    0x230
#define AT91SAM9263_CAN_MDL1                    0x234
#define AT91SAM9263_CAN_MDH1                    0x238
#define AT91SAM9263_CAN_MCR1                    0x23c

  /* Pulse Width Modulation (PWMC) */
#define AT91SAM9263_PWMC_SIZE                   0x234
#define AT91SAM9263_PWMC_MR                     0x000
#define AT91SAM9263_PWMC_ENA                    0x004
#define AT91SAM9263_PWMC_DIS                    0x008
#define AT91SAM9263_PWMC_SR                     0x00c
#define AT91SAM9263_PWMC_IER                    0x010
#define AT91SAM9263_PWMC_IDR                    0x014
#define AT91SAM9263_PWMC_IMR                    0x018
#define AT91SAM9263_PWMC_ISR                    0x01c
#define AT91SAM9263_PWMC_CRM0                   0x200
#define AT91SAM9263_PWMC_CRM(x)                 (0x200 + 0x20*x)
#define AT91SAM9263_PWMC_CDTY0                  0x204
#define AT91SAM9263_PWMC_CDTY(x)                (0x204 + 0x20*x)
#define AT91SAM9263_PWMC_CPRD0                  0x208
#define AT91SAM9263_PWMC_CPRD(x)                (0x208 + 0x20*x)
#define AT91SAM9263_PWMC_CCNT0                  0x20c
#define AT91SAM9263_PWMC_CCNT(x)                (0x20c + 0x20*x)
#define AT91SAM9263_PWMC_CUPD0                  0x210
#define AT91SAM9263_PWMC_CUPD(x)                (0x210 + 0x20*x)
#define AT91SAM9263_PWMC_CRM1                   0x220
#define AT91SAM9263_PWMC_CDTY1                  0x224
#define AT91SAM9263_PWMC_CPRD1                  0x228
#define AT91SAM9263_PWMC_CCNT1                  0x22c
#define AT91SAM9263_PWMC_CUPD1                  0x230

  /* Ethernet MAC (EMAC) */
#define AT91SAM9263_EMAC_SIZE                   0xc8
#define AT91SAM9263_EMAC_NCR                    0x00
#define AT91SAM9263_EMAC_NCFG                   0x04
#define AT91SAM9263_EMAC_NSR                    0x08
#define AT91SAM9263_EMAC_TSR                    0x14
#define AT91SAM9263_EMAC_RBQP                   0x18
#define AT91SAM9263_EMAC_TBQP                   0x1c
#define AT91SAM9263_EMAC_RSR                    0x20
#define AT91SAM9263_EMAC_ISR                    0x24
#define AT91SAM9263_EMAC_IER                    0x28
#define AT91SAM9263_EMAC_IDR                    0x2c
#define AT91SAM9263_EMAC_IMR                    0x30
#define AT91SAM9263_EMAC_MAN                    0x34
#define AT91SAM9263_EMAC_PTR                    0x38
#define AT91SAM9263_EMAC_PFR                    0x3c
#define AT91SAM9263_EMAC_FTO                    0x40
#define AT91SAM9263_EMAC_SCF                    0x44
#define AT91SAM9263_EMAC_MCF                    0x48
#define AT91SAM9263_EMAC_FRO                    0x4c
#define AT91SAM9263_EMAC_FCSE                   0x50
#define AT91SAM9263_EMAC_ALE                    0x54
#define AT91SAM9263_EMAC_DTF                    0x58
#define AT91SAM9263_EMAC_LCOL                   0x5c
#define AT91SAM9263_EMAC_ECOL                   0x60
#define AT91SAM9263_EMAC_TUND                   0x64
#define AT91SAM9263_EMAC_CSE                    0x68
#define AT91SAM9263_EMAC_RRE                    0x6c
#define AT91SAM9263_EMAC_ROV                    0x70
#define AT91SAM9263_EMAC_RSE                    0x74
#define AT91SAM9263_EMAC_ELE                    0x78
#define AT91SAM9263_EMAC_RJA                    0x7c
#define AT91SAM9263_EMAC_USF                    0x80
#define AT91SAM9263_EMAC_STE                    0x84
#define AT91SAM9263_EMAC_RLE                    0x88
#define AT91SAM9263_EMAC_HRB                    0x90
#define AT91SAM9263_EMAC_HRT                    0x94
#define AT91SAM9263_EMAC_SAxB(x)                (0x98 + 8*x)
#define AT91SAM9263_EMAC_SA1B                   0x98
#define AT91SAM9263_EMAC_SAxT(x)                (0x9c + 8*x)
#define AT91SAM9263_EMAC_SA1T                   0x9c
#define AT91SAM9263_EMAC_SA2B                   0xa0
#define AT91SAM9263_EMAC_SA2T                   0xa4
#define AT91SAM9263_EMAC_SA3B                   0xa8
#define AT91SAM9263_EMAC_SA3T                   0xac
#define AT91SAM9263_EMAC_SA4B                   0xb0
#define AT91SAM9263_EMAC_SA4T                   0xb4
#define AT91SAM9263_EMAC_TID                    0xb8
#define AT91SAM9263_EMAC_USRIO                  0xc0

  /* Image Sensor Interface (ISI) */
#define AT91SAM9263_ISI_SIZE                    0x44
#define AT91SAM9263_ISI_CR1                     0x00
#define AT91SAM9263_ISI_CR2                     0x04
#define AT91SAM9263_ISI_SR                      0x08
#define AT91SAM9263_ISI_IER                     0x0c
#define AT91SAM9263_ISI_IDR                     0x10
#define AT91SAM9263_ISI_IMR                     0x14
#define AT91SAM9263_ISI_PSIZE                   0x20
#define AT91SAM9263_ISI_PDECF                   0x24
#define AT91SAM9263_ISI_PPFBD                   0x28
#define AT91SAM9263_ISI_CDBA                    0x2c
#define AT91SAM9263_ISI_Y2R_SET0                0x30
#define AT91SAM9263_ISI_Y2R_SET1                0x34
#define AT91SAM9263_ISI_R2Y_SET0                0x38
#define AT91SAM9263_ISI_R2Y_SET1                0x3c
#define AT91SAM9263_ISI_R2Y_SET2                0x40

  /* 2D Graphics Controller (2DGC) */
#define AT91SAM9263_2DGC_SIZE                   0x208
#define AT91SAM9263_2DGC_BTSXR                  0x000
#define AT91SAM9263_2DGC_BTSYR                  0x004
#define AT91SAM9263_2DGC_SBXR                   0x008
#define AT91SAM9263_2DGC_SBYR                   0x00c
#define AT91SAM9263_2DGC_TEXR                   0x010
#define AT91SAM9263_2DGC_TEYR                   0x014
#define AT91SAM9263_2DGC_LWR                    0x018
#define AT91SAM9263_2DGC_LPR                    0x01c
#define AT91SAM9263_2DGC_CSR                    0x020
#define AT91SAM9263_2DGC_LOR                    0x024
#define AT91SAM9263_2DGC_GOR                    0x028
#define AT91SAM9263_2DGC_EBXR                   0x02c
#define AT91SAM9263_2DGC_EBYR                   0x030
#define AT91SAM9263_2DGC_EEXR                   0x034
#define AT91SAM9263_2DGC_EEYR                   0x038
#define AT91SAM9263_2DGC_ECSR                   0x03c
#define AT91SAM9263_2DGC_CCR                    0x040
#define AT91SAM9263_2DGC_CXMINR                 0x044
#define AT91SAM9263_2DGC_CXMAXR                 0x048
#define AT91SAM9263_2DGC_CYMINR                 0x04c
#define AT91SAM9263_2DGC_CYMAXR                 0x050
#define AT91SAM9263_2DGC_GSR                    0x054
#define AT91SAM9263_2DGC_VSR                    0x058
#define AT91SAM9263_2DGC_FCR                    0x060
#define AT91SAM9263_2DGC_GIR                    0x064
#define AT91SAM9263_2DGC_GIMR                   0x068
#define AT91SAM9263_2DGC_BPPR                   0x070
#define AT91SAM9263_2DGC_CQCR                   0x078
#define AT91SAM9263_2DGC_CQSR                   0x07c
#define AT91SAM9263_2DGC_CQR                    0x080
#define AT91SAM9263_2DGC_VXR                    0x090
#define AT91SAM9263_2DGC_VYR                    0x120
#define AT91SAM9263_2DGC_VOR                    0x200
#define AT91SAM9263_2DGC_DFR                    0x204


 /*
  * System Controller Register Declaration.
  */

  /* Error Correted Code (ECC) */
#define AT91SAM9263_ECC_SIZE                    0x14
#define AT91SAM9263_ECC_CR                      0x00
#define AT91SAM9263_ECC_MR                      0x04
#define AT91SAM9263_ECC_SR                      0x08
#define AT91SAM9263_ECC_PR                      0x0c
#define AT91SAM9263_ECC_NPR                     0x10

  /* SDRAM Controller (SDRAMC) */
#define AT91SAM9263_SDRAMC_SIZE                 0x28
#define AT91SAM9263_SDRAMC_MR                   0x00
#define AT91SAM9263_SDRAMC_TR                   0x04
#define AT91SAM9263_SDRAMC_CR                   0x08
#define AT91SAM9263_SDRAMC_LPR                  0x10
#define AT91SAM9263_SDRAMC_IER                  0x14
#define AT91SAM9263_SDRAMC_IDR                  0x18
#define AT91SAM9263_SDRAMC_IMR                  0x1c
#define AT91SAM9263_SDRAMC_ISR                  0x20
#define AT91SAM9263_SDRAMC_MDR                  0x24

  /* Static Memory Controller (SMC) */
#define AT91SAM9263_SMC_SIZE                    0x80
#define AT91SAM9263_SMC_SETUP(CS_NUM)           (0x00 + CS_NUM*0x10)
#define AT91SAM9263_SMC_PULSE(CS_NUM)           (0x04 + CS_NUM*0x10)
#define AT91SAM9263_SMC_CYCLE(CS_NUM)           (0x08 + CS_NUM*0x10)
#define AT91SAM9263_SMC_MODE(CS_NUM)            (0x0c + CS_NUM*0x10)

  /* Bus Matrix (MATRIX) */
#define AT91SAM9263_MATRIX_SIZE                 0x110
#define AT91SAM9263_MATRIX_MCFG0                0x000
#define AT91SAM9263_MATRIX_MCFG1                0x004
#define AT91SAM9263_MATRIX_MCFG2                0x008
#define AT91SAM9263_MATRIX_MCFG3                0x00c
#define AT91SAM9263_MATRIX_MCFG4                0x010
#define AT91SAM9263_MATRIX_MCFG5                0x014
#define AT91SAM9263_MATRIX_MCFG6                0x018
#define AT91SAM9263_MATRIX_MCFG7                0x01c
#define AT91SAM9263_MATRIX_MCFG8                0x020
#define AT91SAM9263_MATRIX_SCFG0                0x40
#define AT91SAM9263_MATRIX_SCFG1                0x44
#define AT91SAM9263_MATRIX_SCFG2                0x48
#define AT91SAM9263_MATRIX_SCFG3                0x4c
#define AT91SAM9263_MATRIX_SCFG4                0x50
#define AT91SAM9263_MATRIX_SCFG5                0x54
#define AT91SAM9263_MATRIX_SCFG6                0x58
#define AT91SAM9263_MATRIX_SCFG7                0x5c
#define AT91SAM9263_MATRIX_PRAS0                0x080
#define AT91SAM9263_MATRIX_PRBS0                0x084
#define AT91SAM9263_MATRIX_PRAS1                0x088
#define AT91SAM9263_MATRIX_PRBS1                0x08c
#define AT91SAM9263_MATRIX_PRAS2                0x090
#define AT91SAM9263_MATRIX_PRBS2                0x094
#define AT91SAM9263_MATRIX_PRAS3                0x098
#define AT91SAM9263_MATRIX_PRBS3                0x09c
#define AT91SAM9263_MATRIX_PRAS4                0x0a0
#define AT91SAM9263_MATRIX_PRBS4                0x0a4
#define AT91SAM9263_MATRIX_PRAS5                0x0a8
#define AT91SAM9263_MATRIX_PRBS5                0x0ac
#define AT91SAM9263_MATRIX_PRAS6                0x0b0
#define AT91SAM9263_MATRIX_PRBS6                0x0b4
#define AT91SAM9263_MATRIX_PRAS7                0x0b8
#define AT91SAM9263_MATRIX_PRBS7                0x0bc
#define AT91SAM9263_MATRIX_MRCR                 0x100

  /* Chip Configuration (CCFG) */
#define AT91SAM9263_CCFG_SIZE                   0xf0
#define AT91SAM9263_CCFG_MATRIX_TCMR            0x04
#define AT91SAM9263_CCFG_EBI0_CSA               0x10
#define AT91SAM9263_CCFG_EBI1_CSA               0x14

/* (DBGU) */
#define AT91SAM9263_DBGU_SIZE                   0x4c
#define AT91SAM9263_DBGU_CR                     0x00
#define AT91SAM9263_DBGU_MR                     0x04
#define AT91SAM9263_DBGU_IER                    0x08
#define AT91SAM9263_DBGU_IDR                    0x0c
#define AT91SAM9263_DBGU_IMR                    0x10
#define AT91SAM9263_DBGU_SR                     0x14
#define AT91SAM9263_DBGU_RHR                    0x18
#define AT91SAM9263_DBGU_THR                    0x1c
#define AT91SAM9263_DBGU_BRGR                   0x20
#define AT91SAM9263_DBGU_CIDR                   0x40
#define AT91SAM9263_DBGU_EXID                   0x44
#define AT91SAM9263_DBGU_FNR                    0x48

  /* Advanced Interrupt Controller (AIC) */
#define AT91SAM9263_AIC_SIZE                    0x200
#define AT91SAM9263_AIC_SMR(x)                  (0x4*x)
#define AT91SAM9263_AIC_SMR0                    AT91SAM9263_AIC_SMR(0 )
#define AT91SAM9263_AIC_SMR1                    AT91SAM9263_AIC_SMR(1 )
#define AT91SAM9263_AIC_SMR2                    AT91SAM9263_AIC_SMR(2 )
#define AT91SAM9263_AIC_SMR3                    AT91SAM9263_AIC_SMR(3 )
#define AT91SAM9263_AIC_SMR4                    AT91SAM9263_AIC_SMR(4 )
#define AT91SAM9263_AIC_SMR5                    AT91SAM9263_AIC_SMR(5 )
#define AT91SAM9263_AIC_SMR6                    AT91SAM9263_AIC_SMR(6 )
#define AT91SAM9263_AIC_SMR7                    AT91SAM9263_AIC_SMR(7 )
#define AT91SAM9263_AIC_SMR8                    AT91SAM9263_AIC_SMR(8 )
#define AT91SAM9263_AIC_SMR9                    AT91SAM9263_AIC_SMR(9 )
#define AT91SAM9263_AIC_SMR10                   AT91SAM9263_AIC_SMR(10)
#define AT91SAM9263_AIC_SMR11                   AT91SAM9263_AIC_SMR(11)
#define AT91SAM9263_AIC_SMR12                   AT91SAM9263_AIC_SMR(12)
#define AT91SAM9263_AIC_SMR13                   AT91SAM9263_AIC_SMR(13)
#define AT91SAM9263_AIC_SMR14                   AT91SAM9263_AIC_SMR(14)
#define AT91SAM9263_AIC_SMR15                   AT91SAM9263_AIC_SMR(15)
#define AT91SAM9263_AIC_SMR16                   AT91SAM9263_AIC_SMR(16)
#define AT91SAM9263_AIC_SMR17                   AT91SAM9263_AIC_SMR(17)
#define AT91SAM9263_AIC_SMR18                   AT91SAM9263_AIC_SMR(18)
#define AT91SAM9263_AIC_SMR19                   AT91SAM9263_AIC_SMR(19)
#define AT91SAM9263_AIC_SMR20                   AT91SAM9263_AIC_SMR(20)
#define AT91SAM9263_AIC_SMR21                   AT91SAM9263_AIC_SMR(21)
#define AT91SAM9263_AIC_SMR22                   AT91SAM9263_AIC_SMR(22)
#define AT91SAM9263_AIC_SMR23                   AT91SAM9263_AIC_SMR(23)
#define AT91SAM9263_AIC_SMR24                   AT91SAM9263_AIC_SMR(24)
#define AT91SAM9263_AIC_SMR25                   AT91SAM9263_AIC_SMR(25)
#define AT91SAM9263_AIC_SMR26                   AT91SAM9263_AIC_SMR(26)
#define AT91SAM9263_AIC_SMR27                   AT91SAM9263_AIC_SMR(27)
#define AT91SAM9263_AIC_SMR28                   AT91SAM9263_AIC_SMR(28)
#define AT91SAM9263_AIC_SMR29                   AT91SAM9263_AIC_SMR(29)
#define AT91SAM9263_AIC_SMR30                   AT91SAM9263_AIC_SMR(30)
#define AT91SAM9263_AIC_SMR31                   AT91SAM9263_AIC_SMR(31)
#define AT91SAM9263_AIC_SVR(x)                  (0x4*x + 0x80)
#define AT91SAM9263_AIC_SVR0                    AT91SAM9263_AIC_SVR(0 )
#define AT91SAM9263_AIC_SVR1                    AT91SAM9263_AIC_SVR(1 )
#define AT91SAM9263_AIC_SVR2                    AT91SAM9263_AIC_SVR(2 )
#define AT91SAM9263_AIC_SVR3                    AT91SAM9263_AIC_SVR(3 )
#define AT91SAM9263_AIC_SVR4                    AT91SAM9263_AIC_SVR(4 )
#define AT91SAM9263_AIC_SVR5                    AT91SAM9263_AIC_SVR(5 )
#define AT91SAM9263_AIC_SVR6                    AT91SAM9263_AIC_SVR(6 )
#define AT91SAM9263_AIC_SVR7                    AT91SAM9263_AIC_SVR(7 )
#define AT91SAM9263_AIC_SVR8                    AT91SAM9263_AIC_SVR(8 )
#define AT91SAM9263_AIC_SVR9                    AT91SAM9263_AIC_SVR(9 )
#define AT91SAM9263_AIC_SVR10                   AT91SAM9263_AIC_SVR(10)
#define AT91SAM9263_AIC_SVR11                   AT91SAM9263_AIC_SVR(11)
#define AT91SAM9263_AIC_SVR12                   AT91SAM9263_AIC_SVR(12)
#define AT91SAM9263_AIC_SVR13                   AT91SAM9263_AIC_SVR(13)
#define AT91SAM9263_AIC_SVR14                   AT91SAM9263_AIC_SVR(14)
#define AT91SAM9263_AIC_SVR15                   AT91SAM9263_AIC_SVR(15)
#define AT91SAM9263_AIC_SVR16                   AT91SAM9263_AIC_SVR(16)
#define AT91SAM9263_AIC_SVR17                   AT91SAM9263_AIC_SVR(17)
#define AT91SAM9263_AIC_SVR18                   AT91SAM9263_AIC_SVR(18)
#define AT91SAM9263_AIC_SVR19                   AT91SAM9263_AIC_SVR(19)
#define AT91SAM9263_AIC_SVR20                   AT91SAM9263_AIC_SVR(20)
#define AT91SAM9263_AIC_SVR21                   AT91SAM9263_AIC_SVR(21)
#define AT91SAM9263_AIC_SVR22                   AT91SAM9263_AIC_SVR(22)
#define AT91SAM9263_AIC_SVR23                   AT91SAM9263_AIC_SVR(23)
#define AT91SAM9263_AIC_SVR24                   AT91SAM9263_AIC_SVR(24)
#define AT91SAM9263_AIC_SVR25                   AT91SAM9263_AIC_SVR(25)
#define AT91SAM9263_AIC_SVR26                   AT91SAM9263_AIC_SVR(26)
#define AT91SAM9263_AIC_SVR27                   AT91SAM9263_AIC_SVR(27)
#define AT91SAM9263_AIC_SVR28                   AT91SAM9263_AIC_SVR(28)
#define AT91SAM9263_AIC_SVR29                   AT91SAM9263_AIC_SVR(29)
#define AT91SAM9263_AIC_SVR30                   AT91SAM9263_AIC_SVR(30)
#define AT91SAM9263_AIC_SVR31                   AT91SAM9263_AIC_SVR(31)
#define AT91SAM9263_AIC_IVR                     0x100
#define AT91SAM9263_AIC_FVR                     0x104
#define AT91SAM9263_AIC_ISR                     0x108
#define AT91SAM9263_AIC_IPR                     0x10c
#define AT91SAM9263_AIC_IMR                     0x110
#define AT91SAM9263_AIC_CISR                    0x114
#define AT91SAM9263_AIC_IECR                    0x120
#define AT91SAM9263_AIC_IDCR                    0x124
#define AT91SAM9263_AIC_ICCR                    0x128
#define AT91SAM9263_AIC_ISCR                    0x12c
#define AT91SAM9263_AIC_EOICR                   0x130
#define AT91SAM9263_AIC_SPU                     0x134
#define AT91SAM9263_AIC_DCR                     0x138
#define AT91SAM9263_AIC_FFER                    0x140
#define AT91SAM9263_AIC_FFDR                    0x144
#define AT91SAM9263_AIC_FFSR                    0x148

  /* Parallel Input/Output (PIO) Controller */
#define AT91SAM9263_PIO_SIZE                    0x200
#define AT91SAM9263_PIO_PER                     0x00
#define AT91SAM9263_PIO_PDR                     0x04
#define AT91SAM9263_PIO_PSR                     0x08
#define AT91SAM9263_PIO_OER                     0x10
#define AT91SAM9263_PIO_ODR                     0x14
#define AT91SAM9263_PIO_OSR                     0x18
#define AT91SAM9263_PIO_IFER                    0x20
#define AT91SAM9263_PIO_IFDR                    0x24
#define AT91SAM9263_PIO_IFSR                    0x28
#define AT91SAM9263_PIO_SODR                    0x30
#define AT91SAM9263_PIO_CODR                    0x34
#define AT91SAM9263_PIO_ODSR                    0x38
#define AT91SAM9263_PIO_PDSR                    0x3c
#define AT91SAM9263_PIO_IER                     0x40
#define AT91SAM9263_PIO_IDR                     0x44
#define AT91SAM9263_PIO_IMR                     0x48
#define AT91SAM9263_PIO_ISR                     0x4c
#define AT91SAM9263_PIO_MDER                    0x50
#define AT91SAM9263_PIO_MDDR                    0x54
#define AT91SAM9263_PIO_MDSR                    0x58
#define AT91SAM9263_PIO_PUDR                    0x60
#define AT91SAM9263_PIO_PUER                    0x64
#define AT91SAM9263_PIO_PUSR                    0x68
#define AT91SAM9263_PIO_ASR                     0x70
#define AT91SAM9263_PIO_BSR                     0x74
#define AT91SAM9263_PIO_ABSR                    0x78
#define AT91SAM9263_PIO_OWER                    0xa0
#define AT91SAM9263_PIO_OWDR                    0xa4
#define AT91SAM9263_PIO_OWSR                    0xa8

  /* Power Management Controller (PMC) */
#define AT91SAM9263_PMC_SIZE                    0x84
#define AT91SAM9263_PMC_SCER                    0x00
#define AT91SAM9263_PMC_SCDR                    0x04
#define AT91SAM9263_PMC_SCSR                    0x08
#define AT91SAM9263_PMC_PCER                    0x10
#define AT91SAM9263_PMC_PCDR                    0x14
#define AT91SAM9263_PMC_PCSR                    0x18
#define AT91SAM9263_PMC_MOR                     0x20
#define AT91SAM9263_PMC_MCFR                    0x24
#define AT91SAM9263_PMC_PLLAR                   0x28
#define AT91SAM9263_PMC_PLLBR                   0x2c
#define AT91SAM9263_PMC_MCKR                    0x30
#define AT91SAM9263_PMC_PCK0                    0x40
#define AT91SAM9263_PMC_PCK1                    0x44
#define AT91SAM9263_PMC_IER                     0x60
#define AT91SAM9263_PMC_IDR                     0x64
#define AT91SAM9263_PMC_SR                      0x68
#define AT91SAM9263_PMC_IMR                     0x6c
#define AT91SAM9263_PMC_PLLICPR                 0x80

  /* Reset Controller (RSTC) */
#define AT91SAM9263_RSTC_SIZE                   0x0C
#define AT91SAM9263_RSTC_CR                     0x00
#define AT91SAM9263_RSTC_SR                     0x04
#define AT91SAM9263_RSTC_MR                     0x08

  /* Shutdown Controller (SHDWC) */
#define AT91SAM9263_SHDWC_SIZE                  0x0c
#define AT91SAM9263_SHDWC_CR                    0x00
#define AT91SAM9263_SHDWC_MR                    0x04
#define AT91SAM9263_SHDWC_SR                    0x08

  /* Real-time Timer(RTT) */
#define AT91SAM9263_RTT_SIZE                    0x10
#define AT91SAM9263_RTT_MR                      0x00
#define AT91SAM9263_RTT_AR                      0x04
#define AT91SAM9263_RTT_VR                      0x08
#define AT91SAM9263_RTT_SR                      0x0c

  /* Periodic Interval Timer (PIT) */
#define AT91SAM9263_PIT_SIZE                    0x10
#define AT91SAM9263_PIT_MR                      0x00
#define AT91SAM9263_PIT_SR                      0x04
#define AT91SAM9263_PIT_PIVR                    0x08
#define AT91SAM9263_PIT_PIIR                    0x0c

  /* Watchdog Timer (WDT) */
#define AT91SAM9263_WDT_SIZE                    0x0c
#define AT91SAM9263_WDT_CR                      0x00
#define AT91SAM9263_WDT_MR                      0x04
#define AT91SAM9263_WDT_SR                      0x08

 /*
  * System Controller Register's bit Definition.
  */

  /* (DBGU) */
    /* CR */
#define AT91SAM9263_DBGU_CR_RSTSTA                  0x00000100
#define AT91SAM9263_DBGU_CR_TXDIS                   0x00000080
#define AT91SAM9263_DBGU_CR_TXEN                    0x00000040
#define AT91SAM9263_DBGU_CR_RXDIS                   0x00000020
#define AT91SAM9263_DBGU_CR_RXEN                    0x00000010
#define AT91SAM9263_DBGU_CR_RSTTX                   0x00000008
#define AT91SAM9263_DBGU_CR_RSTRX                   0x00000004

    /* MR */
#define AT91SAM9263_DBGU_MR_CHMODE(mode)            ((mode & 3) << 14)
    #define AT91SAM9263_DBGU_MR_CHMODE_MASK             AT91SAM9263_DBGU_MR_CHMODE(3)
    #define AT91SAM9263_DBGU_MR_CHMODE_NORMAL           AT91SAM9263_DBGU_MR_CHMODE(0)
    #define AT91SAM9263_DBGU_MR_CHMODE_AUTO_ECHO        AT91SAM9263_DBGU_MR_CHMODE(1)
    #define AT91SAM9263_DBGU_MR_CHMODE_LOCAL_LOOPB      AT91SAM9263_DBGU_MR_CHMODE(2)
    #define AT91SAM9263_DBGU_MR_CHMODE_REMOTE_LOOPB     AT91SAM9263_DBGU_MR_CHMODE(3)

#define AT91SAM9263_DBGU_MR_PAR(par)                ((par  & 7) << 9)
    #define AT91SAM9263_DBGU_MR_PAR_MASK               AT91SAM9263_DBGU_MR_PAR(7)
    #define AT91SAM9263_DBGU_MR_PAR_EVEN               AT91SAM9263_DBGU_MR_PAR(0)
    #define AT91SAM9263_DBGU_MR_PAR_ODD                AT91SAM9263_DBGU_MR_PAR(1)
    #define AT91SAM9263_DBGU_MR_PAR_SPACE              AT91SAM9263_DBGU_MR_PAR(2)
    #define AT91SAM9263_DBGU_MR_PAR_MARK               AT91SAM9263_DBGU_MR_PAR(3)
    #define AT91SAM9263_DBGU_MR_PAR_NONE               AT91SAM9263_DBGU_MR_PAR(4)

    /* IER, IDR, IMR & SR*/
#define AT91SAM9263_DBGU_IE_SR_COMMRX               0x80000000
#define AT91SAM9263_DBGU_IE_SR_COMMTX               0x40000000
#define AT91SAM9263_DBGU_IE_SR_RXBUFF               0x00001000
#define AT91SAM9263_DBGU_IE_SR_TXBUFFE              0x00000800
#define AT91SAM9263_DBGU_IE_SR_TXEMPTY              0x00000200
#define AT91SAM9263_DBGU_IE_SR_PARE                 0x00000080
#define AT91SAM9263_DBGU_IE_SR_FRAME                0x00000040
#define AT91SAM9263_DBGU_IE_SR_OVERE                0x00000020
#define AT91SAM9263_DBGU_IE_SR_ENDTX                0x00000010
#define AT91SAM9263_DBGU_IE_SR_ENDRX                0x00000008
#define AT91SAM9263_DBGU_IE_SR_TXRDY                0x00000002
#define AT91SAM9263_DBGU_IE_SR_RXRDY                0x00000001

    /* RHR & THR */
#define AT91SAM9263_DBGU_RHR_THR_CHAR_MASK          0x000000ff

    /* BRGR */
#define AT91SAM9263_DBGU_BRGR_CD_MASK               0x0000ffff
    #define AT91SAM9263_DBGU_BRGR_CD_DISABLE            0x00000000
    #define AT91SAM9263_DBGU_BRGR_CD_MCK                0x00000001

    /* CIDR */
#define AT91SAM9263_DBGU_CIDR_EXT                   0x80000000
#define AT91SAM9263_DBGU_CIDR_NVPTYP                0x70000000
#define AT91SAM9263_DBGU_CIDR_ARCH                  0x0ff00000
#define AT91SAM9263_DBGU_CIDR_SRAMSIZ               0x000f0000
#define AT91SAM9263_DBGU_CIDR_NVPSIZ2               0x0000f000
#define AT91SAM9263_DBGU_CIDR_NVPSIZ                0x00000f00
#define AT91SAM9263_DBGU_CIDR_EPROC                 0x000000e0
#define AT91SAM9263_DBGU_CIDR_VERSION               0x0000001f

    /* EXID */
#define AT91SAM9263_DBGU_EXID_MASK                  0xffffffff

    /* FNR */
#define AT91SAM9263_DBGU_FNR_FNTRST                 0x00000001

 /*
  * Two-Wire Interface (TWI)
  */
    /* Control Register (CR) */
#define AT91SAM9263_TWI_CR_SWRST                    0x00000080
#define AT91SAM9263_TWI_CR_MSDIS                    0x00000008
#define AT91SAM9263_TWI_CR_MSEN                     0x00000004
#define AT91SAM9263_TWI_CR_STOP                     0x00000002
#define AT91SAM9263_TWI_CR_START                    0x00000001

    /* Master Mode Register (MMR) */
#define AT91SAM9263_TWI_MMR_DADR                    0x007f0000
#define AT91SAM9263_TWI_MMR_MREAD                   0x00001000
#define AT91SAM9263_TWI_MMR_IADRSZ                  0x00000300

    /* Internal Address Register (IADR) */
#define AT91SAM9263_TWI_IADR0                       0x000000ff
#define AT91SAM9263_TWI_IADR1                       0x0000ff00
#define AT91SAM9263_TWI_IADR2                       0x00ff0000
#define AT91SAM9263_TWI_IADR_SET(byte,val)          ((val & 0xff) << (8 * byte))

    /* Clock Waveform Generator Register (CWGR) */
#define AT91SAM9263_TWI_CWGR_CKDIV                  0x00070000
#define AT91SAM9263_TWI_CWGR_CHDIV                  0x0000ff00
#define AT91SAM9263_TWI_CWGR_CLDIV                  0x000000ff

    /* Status Register (SR )and Interrupt Enable,Disable & Mask Register (IER, IDR & IMR) */
#define AT91SAM9263_TWI_SR_NACK                     0x00000100
#define AT91SAM9263_TWI_SR_TXRDY                    0x00000004
#define AT91SAM9263_TWI_SR_RXRDY                    0x00000002
#define AT91SAM9263_TWI_SR_TXCOMP                   0x00000001

   /* Received & Transmit Holding Register(RHR & THR)*/
#define AT91SAM9263_TWI_HR_DATA                     0x000000ff


 /*
  * Serial Peripheral Interface (SPI)
  */
    /* Control Register (CR) */
#define AT91SAM9263_SPI_CR_LASTXFER                 0x01000000
#define AT91SAM9263_SPI_CR_SWRST                    0x00000080
#define AT91SAM9263_SPI_CR_SPIDIS                   0x00000002
#define AT91SAM9263_SPI_CR_SPIEN                    0x00000001

    /* Mode Register (MR) */
#define AT91SAM9263_SPI_MR_DLYBCS(dlybcs)           ((dlybcs & 0xff) << 24)
#define AT91SAM9263_SPI_MR_PCS(pcs)                 ((pcs & 0xf)     << 16)
#define AT91SAM9263_SPI_MR_LLB                      0x00000080
#define AT91SAM9263_SPI_MR_MODFDIS                  0x00000010
#define AT91SAM9263_SPI_MR_PCSDEC                   0x00000004
#define AT91SAM9263_SPI_MR_PS                       0x00000002
#define AT91SAM9263_SPI_MR_MSTR                     0x00000001

    /* Receive Data Register (RDR) */
#define AT91SAM9263_SPI_RDR_PCS(pcs)                ((pcs & 0xf)     << 16)
#define AT91SAM9263_SPI_RDR_RD                      0x0000ffff

    /* Transmit Data Register (TDR) */
#define AT91SAM9263_SPI_TDR_LASTXFER                0x00010000
#define AT91SAM9263_SPI_TDR_PCS(pcs)                ((pcs & 0xf)     << 16)
#define AT91SAM9263_SPI_TDR_TD                      0x0000ffff

    /* Status Register (SR )and Interrupt Enable,Disable & Mask Register (IER, IDR & IMR) */
#define AT91SAM9263_SPI_SR_SPIENS                   0x00010000      // Status Register Only */
#define AT91SAM9263_SPI_SR_TDEMPTY                  0x00000200
#define AT91SAM9263_SPI_SR_NSSR                     0x00000100
#define AT91SAM9263_SPI_SR_TXBUFE                   0x00000080
#define AT91SAM9263_SPI_SR_RXBUFF                   0x00000040
#define AT91SAM9263_SPI_SR_ENDTX                    0x00000020
#define AT91SAM9263_SPI_SR_ENDRX                    0x00000010
#define AT91SAM9263_SPI_SR_OVRES                    0x00000008
#define AT91SAM9263_SPI_SR_MODF                     0x00000004
#define AT91SAM9263_SPI_SR_TDRE                     0x00000002
#define AT91SAM9263_SPI_SR_RDRF                     0x00000001

    /* Chip Select Register 0, 1, 2 & 3 (CSR0 - CSR3) */
#define AT91SAM9263_SPI_CSRx_DLYBCT(dlybct)         ((dlybct & 0xff) << 24)   
#define AT91SAM9263_SPI_CSRx_DLYBS(dlybs)           ((dlybs  & 0xff) << 16)
#define AT91SAM9263_SPI_CSRx_SCBR(scbr)             ((scbr   & 0xff) << 8)
#define AT91SAM9263_SPI_CSRx_BITS(bits)             ((bits   & 0xf)  << 4)
#define AT91SAM9263_SPI_CSRx_CSAAT                  0x00000008
#define AT91SAM9263_SPI_CSRx_NCPHA                  0x00000002
#define AT91SAM9263_SPI_CSRx_CPOL                   0x00000001

 /*
  * PIO Controller A Register's bit Definition.
  */
#define AT91SAM9263_PIO_PA0_A_MCI0_DA0              (1 << 0) 
#define AT91SAM9263_PIO_PA0_B_SPI0_MISO             (1 << 0)
#define AT91SAM9263_PIO_PA1_A_MCI0_CDA              (1 << 1)
#define AT91SAM9263_PIO_PA1_B_SPI0_MOSI             (1 << 1)
#define AT91SAM9263_PIO_PA2_A_                      (1 << 2)
#define AT91SAM9263_PIO_PA2_B_SPI0_SPCK             (1 << 2)
#define AT91SAM9263_PIO_PA3_A_MCI0_DA1              (1 << 3)
#define AT91SAM9263_PIO_PA3_B_SPI0_NPCS1            (1 << 3)
#define AT91SAM9263_PIO_PA4_A_MCI0_DA2              (1 << 4)
#define AT91SAM9263_PIO_PA4_B_SPI0_NPCS2            (1 << 4)
#define AT91SAM9263_PIO_PA5_A_MCI0_DA3              (1 << 5)
#define AT91SAM9263_PIO_PA5_B_SPI0_NPCS3            (1 << 5)
#define AT91SAM9263_PIO_PA6_A_MCI1_CK               (1 << 6)
#define AT91SAM9263_PIO_PA6_B_                      (1 << 6)
#define AT91SAM9263_PIO_PA7_A_MCI1_CDA              (1 << 7)
#define AT91SAM9263_PIO_PA7_B_                      (1 << 7)
#define AT91SAM9263_PIO_PA8_A_MCI1_DA0              (1 << 8)
#define AT91SAM9263_PIO_PA8_B_                      (1 << 8)
#define AT91SAM9263_PIO_PA9_A_MCI1_DA1              (1 << 9)
#define AT91SAM9263_PIO_PA9_B_                      (1 << 9)
#define AT91SAM9263_PIO_PA10_A_MCI1_DA2             (1 << 10)
#define AT91SAM9263_PIO_PA10_B_                     (1 << 10)
#define AT91SAM9263_PIO_PA11_A_MCI1_DA3             (1 << 11)
#define AT91SAM9263_PIO_PA11_B_                     (1 << 11)
#define AT91SAM9263_PIO_PA12_A_MCI0_CK              (1 << 12)
#define AT91SAM9263_PIO_PA12_B_                     (1 << 12)
#define AT91SAM9263_PIO_PA13_A_CANTX                (1 << 13)
#define AT91SAM9263_PIO_PA13_B_PCK0                 (1 << 13)
#define AT91SAM9263_PIO_PA14_A_CANRX                (1 << 14)
#define AT91SAM9263_PIO_PA14_B_IRQ0                 (1 << 14)
#define AT91SAM9263_PIO_PA15_A_TCLK2                (1 << 15)
#define AT91SAM9263_PIO_PA15_B_IRQ1                 (1 << 15)
#define AT91SAM9263_PIO_PA16_A_MCI0_CDB             (1 << 16)
#define AT91SAM9263_PIO_PA16_B_EBI1_D16             (1 << 16)
#define AT91SAM9263_PIO_PA17_A_MCI0_DB0             (1 << 17)
#define AT91SAM9263_PIO_PA17_B_EBI1_D17             (1 << 17)
#define AT91SAM9263_PIO_PA18_A_MCI0_DB1             (1 << 18)
#define AT91SAM9263_PIO_PA18_B_EBI1_D18             (1 << 18)
#define AT91SAM9263_PIO_PA19_A_MCI0_DB2             (1 << 19)
#define AT91SAM9263_PIO_PA19_B_EBI1_D19             (1 << 19)
#define AT91SAM9263_PIO_PA20_A_MCI0_DB3             (1 << 20)
#define AT91SAM9263_PIO_PA20_B_EBI1_D20             (1 << 20)
#define AT91SAM9263_PIO_PA21_A_MCI1_CDB             (1 << 21)
#define AT91SAM9263_PIO_PA21_B_EBI1_D21             (1 << 21)
#define AT91SAM9263_PIO_PA22_A_MCI1_DB0             (1 << 22)
#define AT91SAM9263_PIO_PA22_B_EBI1_D22             (1 << 22)
#define AT91SAM9263_PIO_PA23_A_MCI1_DB1             (1 << 23)
#define AT91SAM9263_PIO_PA23_B_EBI1_D23             (1 << 23)
#define AT91SAM9263_PIO_PA24_A_MCI1_DB2             (1 << 24)
#define AT91SAM9263_PIO_PA24_B_EBI1_D24             (1 << 24)
#define AT91SAM9263_PIO_PA25_A_MCI1_DB3             (1 << 25)
#define AT91SAM9263_PIO_PA25_B_EBI1_D25             (1 << 25)
#define AT91SAM9263_PIO_PA26_A_TXD0                 (1 << 26)
#define AT91SAM9263_PIO_PA26_B_EBI1_D26             (1 << 26)
#define AT91SAM9263_PIO_PA27_A_RXD0                 (1 << 27)
#define AT91SAM9263_PIO_PA27_B_EBI1_D27             (1 << 27)
#define AT91SAM9263_PIO_PA28_A_RTS0                 (1 << 28)
#define AT91SAM9263_PIO_PA28_B_EBI1_D28             (1 << 28)
#define AT91SAM9263_PIO_PA29_A_CTS0                 (1 << 29)
#define AT91SAM9263_PIO_PA29_B_EBI1_D29             (1 << 29)
#define AT91SAM9263_PIO_PA30_A_SCK0                 (1 << 30)
#define AT91SAM9263_PIO_PA30_B_EBI1_D30             (1 << 30)
#define AT91SAM9263_PIO_PA31_A_DMARQ0               (1 << 31)
#define AT91SAM9263_PIO_PA31_B_EBI1_D31             (1 << 31)

/*
  * PIO Controller B Register's bit Definition.
  */
#define AT91SAM9263_PIO_PB0_A_AC97FS                (1 << 0) 
#define AT91SAM9263_PIO_PB0_B_TF0                   (1 << 0)
#define AT91SAM9263_PIO_PB1_A_AC97CK                (1 << 1)
#define AT91SAM9263_PIO_PB1_B_TK0                   (1 << 1)
#define AT91SAM9263_PIO_PB2_A_AC97TX                (1 << 2)
#define AT91SAM9263_PIO_PB2_B_TD0                   (1 << 2)
#define AT91SAM9263_PIO_PB3_A_AC97RX                (1 << 3)
#define AT91SAM9263_PIO_PB3_B_RD0                   (1 << 3)
#define AT91SAM9263_PIO_PB4_A_TWD                   (1 << 4)
#define AT91SAM9263_PIO_PB4_B_RK0                   (1 << 4)
#define AT91SAM9263_PIO_PB5_A_TWCK                  (1 << 5)
#define AT91SAM9263_PIO_PB5_B_RF0                   (1 << 5)
#define AT91SAM9263_PIO_PB6_A_TF1                   (1 << 6)
#define AT91SAM9263_PIO_PB6_B_DMARQ1                (1 << 6)
#define AT91SAM9263_PIO_PB7_A_TK1                   (1 << 7)
#define AT91SAM9263_PIO_PB7_B_PWM0                  (1 << 7)
#define AT91SAM9263_PIO_PB8_A_TD1                   (1 << 8)
#define AT91SAM9263_PIO_PB8_B_PWM1                  (1 << 8)
#define AT91SAM9263_PIO_PB9_A_RD1                   (1 << 9)
#define AT91SAM9263_PIO_PB9_B_LCDCC                 (1 << 9)
#define AT91SAM9263_PIO_PB10_A_RK1                  (1 << 10)
#define AT91SAM9263_PIO_PB10_B_PCK1                 (1 << 10)
#define AT91SAM9263_PIO_PB11_A_RF1                  (1 << 11)
#define AT91SAM9263_PIO_PB11_B_SPI0_NPCS3           (1 << 11)
#define AT91SAM9263_PIO_PB12_A_SPI1_MISO            (1 << 12)
#define AT91SAM9263_PIO_PB12_B_                     (1 << 12)
#define AT91SAM9263_PIO_PB13_A_SPI1_MOSI            (1 << 13)
#define AT91SAM9263_PIO_PB13_B_                     (1 << 13)
#define AT91SAM9263_PIO_PB14_A_SPI1_SPCK            (1 << 14)
#define AT91SAM9263_PIO_PB14_B_                     (1 << 14)
#define AT91SAM9263_PIO_PB15_A_SPI1_NPCS0           (1 << 15)
#define AT91SAM9263_PIO_PB15_B_                     (1 << 15)
#define AT91SAM9263_PIO_PB16_A_SPI1_NPCS1           (1 << 16)
#define AT91SAM9263_PIO_PB16_B_                     (1 << 16)
#define AT91SAM9263_PIO_PB17_A_SPI1_NPCS2           (1 << 17)
#define AT91SAM9263_PIO_PB17_B_TIOA2                (1 << 17)
#define AT91SAM9263_PIO_PB18_A_SPI1_NPCS3           (1 << 18)
#define AT91SAM9263_PIO_PB18_B_TIOB2                (1 << 18)
#define AT91SAM9263_PIO_PB19_A_                     (1 << 19)
#define AT91SAM9263_PIO_PB19_B_                     (1 << 19)
#define AT91SAM9263_PIO_PB20_A_                     (1 << 20)
#define AT91SAM9263_PIO_PB20_B_                     (1 << 20)
#define AT91SAM9263_PIO_PB21_A_                     (1 << 21)
#define AT91SAM9263_PIO_PB21_B_                     (1 << 21)
#define AT91SAM9263_PIO_PB22_A_                     (1 << 22)
#define AT91SAM9263_PIO_PB22_B_                     (1 << 22)
#define AT91SAM9263_PIO_PB23_A_                     (1 << 23)
#define AT91SAM9263_PIO_PB23_B_                     (1 << 23)
#define AT91SAM9263_PIO_PB24_A_                     (1 << 24)
#define AT91SAM9263_PIO_PB24_B_DMARQ3               (1 << 24)
#define AT91SAM9263_PIO_PB25_A_                     (1 << 25)
#define AT91SAM9263_PIO_PB25_B_                     (1 << 25)
#define AT91SAM9263_PIO_PB26_A_                     (1 << 26)
#define AT91SAM9263_PIO_PB26_B_                     (1 << 26)
#define AT91SAM9263_PIO_PB27_A_                     (1 << 27)
#define AT91SAM9263_PIO_PB27_B_PWM2                 (1 << 27)
#define AT91SAM9263_PIO_PB28_A_                     (1 << 28)
#define AT91SAM9263_PIO_PB28_B_TCLK0                (1 << 28)
#define AT91SAM9263_PIO_PB29_A_                     (1 << 29)
#define AT91SAM9263_PIO_PB29_B_PWM3                 (1 << 29)
#define AT91SAM9263_PIO_PB30_A_                     (1 << 30)
#define AT91SAM9263_PIO_PB30_B_                     (1 << 30)
#define AT91SAM9263_PIO_PB31_A_                     (1 << 31)
#define AT91SAM9263_PIO_PB31_B_                     (1 << 31)

 /*
  * PIO Controller C Register's bit Definition.
  */
#define AT91SAM9263_PIO_PC0_A_LCDVSYNC              (1 << 0) 
#define AT91SAM9263_PIO_PC0_B_                      (1 << 0)
#define AT91SAM9263_PIO_PC1_A_LCDHSYNC              (1 << 1)
#define AT91SAM9263_PIO_PC1_B_                      (1 << 1)
#define AT91SAM9263_PIO_PC2_A_LDCDOTCK              (1 << 2)
#define AT91SAM9263_PIO_PC2_B_                      (1 << 2)
#define AT91SAM9263_PIO_PC3_A_LCDDEN                (1 << 3)
#define AT91SAM9263_PIO_PC3_B_PWM1                  (1 << 3)
#define AT91SAM9263_PIO_PC4_A_LCDD0                 (1 << 4)
#define AT91SAM9263_PIO_PC4_B_LCDD3                 (1 << 4)
#define AT91SAM9263_PIO_PC5_A_LCDD1                 (1 << 5)
#define AT91SAM9263_PIO_PC5_B_LCDD4                 (1 << 5)
#define AT91SAM9263_PIO_PC6_A_LCDD2                 (1 << 6)
#define AT91SAM9263_PIO_PC6_B_LCDD5                 (1 << 6)
#define AT91SAM9263_PIO_PC7_A_LCDD3                 (1 << 7)
#define AT91SAM9263_PIO_PC7_B_LCDD6                 (1 << 7)
#define AT91SAM9263_PIO_PC8_A_LCDD4                 (1 << 8)
#define AT91SAM9263_PIO_PC8_B_LCDD7                 (1 << 8)
#define AT91SAM9263_PIO_PC9_A_LCDD5                 (1 << 9)
#define AT91SAM9263_PIO_PC9_B_LCDD10                (1 << 9)
#define AT91SAM9263_PIO_PC10_A_LCDD6                (1 << 10)
#define AT91SAM9263_PIO_PC10_B_LCDD11               (1 << 10)
#define AT91SAM9263_PIO_PC11_A_LCDD7                (1 << 11)
#define AT91SAM9263_PIO_PC11_B_LCDD12               (1 << 11)
#define AT91SAM9263_PIO_PC12_A_LCDD8                (1 << 12)
#define AT91SAM9263_PIO_PC12_B_LCDD13               (1 << 12)
#define AT91SAM9263_PIO_PC13_A_LCDD9                (1 << 13)
#define AT91SAM9263_PIO_PC13_B_LCDD14               (1 << 13)
#define AT91SAM9263_PIO_PC14_A_LCDD10               (1 << 14)
#define AT91SAM9263_PIO_PC14_B_LCDD15               (1 << 14)
#define AT91SAM9263_PIO_PC15_A_LCDD11               (1 << 15)
#define AT91SAM9263_PIO_PC15_B_LCDD19               (1 << 15)
#define AT91SAM9263_PIO_PC16_A_LCDD12               (1 << 16)
#define AT91SAM9263_PIO_PC16_B_LCDD20               (1 << 16)
#define AT91SAM9263_PIO_PC17_A_LCDD13               (1 << 17)
#define AT91SAM9263_PIO_PC17_B_LCDD21               (1 << 17)
#define AT91SAM9263_PIO_PC18_A_LCDD14               (1 << 18)
#define AT91SAM9263_PIO_PC18_B_LCDD22               (1 << 18)
#define AT91SAM9263_PIO_PC19_A_LCDD15               (1 << 19)
#define AT91SAM9263_PIO_PC19_B_LCDD23               (1 << 19)
#define AT91SAM9263_PIO_PC20_A_LCDD16               (1 << 20)
#define AT91SAM9263_PIO_PC20_B_ETX2                 (1 << 20)
#define AT91SAM9263_PIO_PC21_A_LCDD17               (1 << 21)
#define AT91SAM9263_PIO_PC21_B_ETX3                 (1 << 21)
#define AT91SAM9263_PIO_PC22_A_LCDD18               (1 << 22)
#define AT91SAM9263_PIO_PC22_B_ERX2                 (1 << 22)
#define AT91SAM9263_PIO_PC23_A_LCDD19               (1 << 23)
#define AT91SAM9263_PIO_PC23_B_ERX3                 (1 << 23)
#define AT91SAM9263_PIO_PC24_A_LCDD20               (1 << 24)
#define AT91SAM9263_PIO_PC24_B_ETXER                (1 << 24)
#define AT91SAM9263_PIO_PC25_A_LCDD21               (1 << 25)
#define AT91SAM9263_PIO_PC25_B_ERXDB                (1 << 25)
#define AT91SAM9263_PIO_PC26_A_LCDD22               (1 << 26)
#define AT91SAM9263_PIO_PC26_B_ECOL                 (1 << 26)
#define AT91SAM9263_PIO_PC27_A_LCDD23               (1 << 27)
#define AT91SAM9263_PIO_PC27_B_ERXCK                (1 << 27)
#define AT91SAM9263_PIO_PC28_A_PWM0                 (1 << 28)
#define AT91SAM9263_PIO_PC28_B_TCLK1                (1 << 28)
#define AT91SAM9263_PIO_PC29_A_PCK0                 (1 << 29)
#define AT91SAM9263_PIO_PC29_B_PWM2                 (1 << 29)
#define AT91SAM9263_PIO_PC30_A_DRXD                 (1 << 30)
#define AT91SAM9263_PIO_PC30_B_                     (1 << 30)
#define AT91SAM9263_PIO_PC31_A_DTXD                 (1 << 31)
#define AT91SAM9263_PIO_PC31_B_                     (1 << 31)

 /*
  * PIO Controller D Register's bit Definition.
  */
#define AT91SAM9263_PIO_PD0_A_TXD1                  (1 << 0) 
#define AT91SAM9263_PIO_PD0_B_SPI0_NPCS2            (1 << 0)
#define AT91SAM9263_PIO_PD1_A_RXD1                  (1 << 1)
#define AT91SAM9263_PIO_PD1_B_SPI0_NPCS3            (1 << 1)
#define AT91SAM9263_PIO_PD2_A_TXD2                  (1 << 2)
#define AT91SAM9263_PIO_PD2_B_SPI1_NPCS2            (1 << 2)
#define AT91SAM9263_PIO_PD3_A_RXD2                  (1 << 3)
#define AT91SAM9263_PIO_PD3_B_SPI1_NPCS3            (1 << 3)
#define AT91SAM9263_PIO_PD4_A_FIQ                   (1 << 4)
#define AT91SAM9263_PIO_PD4_B_DMARQ2                (1 << 4)
#define AT91SAM9263_PIO_PD5_A_EBI0_NWAIT            (1 << 5)
#define AT91SAM9263_PIO_PD5_B_RTS2                  (1 << 5)
#define AT91SAM9263_PIO_PD6_A_EBI0_NCS4_CFCS0       (1 << 6)
#define AT91SAM9263_PIO_PD6_B_CTS2                  (1 << 6)
#define AT91SAM9263_PIO_PD7_A_EBI0_NCS5_CFCS1       (1 << 7)
#define AT91SAM9263_PIO_PD7_B_RTS1                  (1 << 7)
#define AT91SAM9263_PIO_PD8_A_EBI0_CFCE1            (1 << 8)
#define AT91SAM9263_PIO_PD8_B_CTS1                  (1 << 8)
#define AT91SAM9263_PIO_PD9_A_EBI0_CFCE2            (1 << 9)
#define AT91SAM9263_PIO_PD9_B_SCK2                  (1 << 9)
#define AT91SAM9263_PIO_PD10_A_                     (1 << 10)
#define AT91SAM9263_PIO_PD10_B_SCK1                 (1 << 10)
#define AT91SAM9263_PIO_PD11_A_EBI0_NCS2            (1 << 11)
#define AT91SAM9263_PIO_PD11_B_TSYNC                (1 << 11)
#define AT91SAM9263_PIO_PD12_A_EBI0_A23             (1 << 12)
#define AT91SAM9263_PIO_PD12_B_TCLK                 (1 << 12)
#define AT91SAM9263_PIO_PD13_A_EBI0_A24             (1 << 13)
#define AT91SAM9263_PIO_PD13_B_TPS0                 (1 << 13)
#define AT91SAM9263_PIO_PD14_A_EBI0_A25_CFRNW       (1 << 14)
#define AT91SAM9263_PIO_PD14_B_TPS1                 (1 << 14)
#define AT91SAM9263_PIO_PD15_A_EBI0_NCS2_NANDCS     (1 << 15)
#define AT91SAM9263_PIO_PD15_B_TPS2                 (1 << 15)
#define AT91SAM9263_PIO_PD16_A_EBI0_D16             (1 << 16)
#define AT91SAM9263_PIO_PD16_B_TPK0                 (1 << 16)
#define AT91SAM9263_PIO_PD17_A_EBI0_D17             (1 << 17)
#define AT91SAM9263_PIO_PD17_B_TP1K                 (1 << 17)
#define AT91SAM9263_PIO_PD18_A_EBI0_D18             (1 << 18)
#define AT91SAM9263_PIO_PD18_B_TPK2                 (1 << 18)
#define AT91SAM9263_PIO_PD19_A_EBI0_D19             (1 << 19)
#define AT91SAM9263_PIO_PD19_B_TPK3                 (1 << 19)
#define AT91SAM9263_PIO_PD20_A_EBI0_D20             (1 << 20)
#define AT91SAM9263_PIO_PD20_B_TPK4                 (1 << 20)
#define AT91SAM9263_PIO_PD21_A_EBI0_D21             (1 << 21)
#define AT91SAM9263_PIO_PD21_B_TPK5                 (1 << 21)
#define AT91SAM9263_PIO_PD22_A_EBI0_D22             (1 << 22)
#define AT91SAM9263_PIO_PD22_B_TPK6                 (1 << 22)
#define AT91SAM9263_PIO_PD23_A_EBI0_D23             (1 << 23)
#define AT91SAM9263_PIO_PD23_B_TPK7                 (1 << 23)
#define AT91SAM9263_PIO_PD24_A_EBI0_D24             (1 << 24)
#define AT91SAM9263_PIO_PD24_B_TPK8                 (1 << 24)
#define AT91SAM9263_PIO_PD25_A_EBI0_D25             (1 << 25)
#define AT91SAM9263_PIO_PD25_B_TPK9                 (1 << 25)
#define AT91SAM9263_PIO_PD26_A_EBI0_D26             (1 << 26)
#define AT91SAM9263_PIO_PD26_B_TPK10                (1 << 26)
#define AT91SAM9263_PIO_PD27_A_EBI0_D27             (1 << 27)
#define AT91SAM9263_PIO_PD27_B_TPK11                (1 << 27)
#define AT91SAM9263_PIO_PD28_A_EBI0_D28             (1 << 28)
#define AT91SAM9263_PIO_PD28_B_TPK12                (1 << 28)
#define AT91SAM9263_PIO_PD29_A_EBI0_D29             (1 << 29)
#define AT91SAM9263_PIO_PD29_B_TPK13                (1 << 29)
#define AT91SAM9263_PIO_PD30_A_EBI0_D30             (1 << 30)
#define AT91SAM9263_PIO_PD30_B_TPK14                (1 << 30)
#define AT91SAM9263_PIO_PD31_A_EBI0_D31             (1 << 31)
#define AT91SAM9263_PIO_PD31_B_TPK15                (1 << 31)

 /*
  * PIO Controller E Register's bit Definition.
  */
#define AT91SAM9263_PIO_PE0_A_                      (1 << 0) 
#define AT91SAM9263_PIO_PE0_B_                      (1 << 0)
#define AT91SAM9263_PIO_PE1_A_                      (1 << 1)
#define AT91SAM9263_PIO_PE1_B_                      (1 << 1)
#define AT91SAM9263_PIO_PE2_A_                      (1 << 2)
#define AT91SAM9263_PIO_PE2_B_                      (1 << 2)
#define AT91SAM9263_PIO_PE3_A_                      (1 << 3)
#define AT91SAM9263_PIO_PE3_B_                      (1 << 3)
#define AT91SAM9263_PIO_PE4_A_                      (1 << 4)
#define AT91SAM9263_PIO_PE4_B_                      (1 << 4)
#define AT91SAM9263_PIO_PE5_A_                      (1 << 5)
#define AT91SAM9263_PIO_PE5_B_                      (1 << 5)
#define AT91SAM9263_PIO_PE6_A_                      (1 << 6)
#define AT91SAM9263_PIO_PE6_B_                      (1 << 6)
#define AT91SAM9263_PIO_PE7_A_                      (1 << 7)
#define AT91SAM9263_PIO_PE7_B_                      (1 << 7)
#define AT91SAM9263_PIO_PE8_A_                      (1 << 8)
#define AT91SAM9263_PIO_PE8_B_                      (1 << 8)
#define AT91SAM9263_PIO_PE9_A_                      (1 << 9)
#define AT91SAM9263_PIO_PE9_B_                      (1 << 9)
#define AT91SAM9263_PIO_PE10_A_                     (1 << 10)
#define AT91SAM9263_PIO_PE10_B_                     (1 << 10)
#define AT91SAM9263_PIO_PE11_A_                     (1 << 11)
#define AT91SAM9263_PIO_PE11_B_                     (1 << 11)
#define AT91SAM9263_PIO_PE12_A_                     (1 << 12)
#define AT91SAM9263_PIO_PE12_B_                     (1 << 12)
#define AT91SAM9263_PIO_PE13_A_                     (1 << 13)
#define AT91SAM9263_PIO_PE13_B_                     (1 << 13)
#define AT91SAM9263_PIO_PE14_A_                     (1 << 14)
#define AT91SAM9263_PIO_PE14_B_                     (1 << 14)
#define AT91SAM9263_PIO_PE15_A_                     (1 << 15)
#define AT91SAM9263_PIO_PE15_B_                     (1 << 15)
#define AT91SAM9263_PIO_PE16_A_                     (1 << 16)
#define AT91SAM9263_PIO_PE16_B_                     (1 << 16)
#define AT91SAM9263_PIO_PE17_A_                     (1 << 17)
#define AT91SAM9263_PIO_PE17_B_                     (1 << 17)
#define AT91SAM9263_PIO_PE18_A_                     (1 << 18)
#define AT91SAM9263_PIO_PE18_B_                     (1 << 18)
#define AT91SAM9263_PIO_PE19_A_                     (1 << 19)
#define AT91SAM9263_PIO_PE19_B_                     (1 << 19)
#define AT91SAM9263_PIO_PE20_A_                     (1 << 20)
#define AT91SAM9263_PIO_PE20_B_                     (1 << 20)
#define AT91SAM9263_PIO_PE21_A_                     (1 << 21)
#define AT91SAM9263_PIO_PE21_B_                     (1 << 21)
#define AT91SAM9263_PIO_PE22_A_                     (1 << 22)
#define AT91SAM9263_PIO_PE22_B_                     (1 << 22)
#define AT91SAM9263_PIO_PE23_A_                     (1 << 23)
#define AT91SAM9263_PIO_PE23_B_                     (1 << 23)
#define AT91SAM9263_PIO_PE24_A_                     (1 << 24)
#define AT91SAM9263_PIO_PE24_B_                     (1 << 24)
#define AT91SAM9263_PIO_PE25_A_                     (1 << 25)
#define AT91SAM9263_PIO_PE25_B_                     (1 << 25)
#define AT91SAM9263_PIO_PE26_A_                     (1 << 26)
#define AT91SAM9263_PIO_PE26_B_                     (1 << 26)
#define AT91SAM9263_PIO_PE27_A_                     (1 << 27)
#define AT91SAM9263_PIO_PE27_B_                     (1 << 27)
#define AT91SAM9263_PIO_PE28_A_                     (1 << 28)
#define AT91SAM9263_PIO_PE28_B_                     (1 << 28)
#define AT91SAM9263_PIO_PE29_A_                     (1 << 29)
#define AT91SAM9263_PIO_PE29_B_                     (1 << 29)
#define AT91SAM9263_PIO_PE30_A_                     (1 << 30)
#define AT91SAM9263_PIO_PE30_B_                     (1 << 30)
#define AT91SAM9263_PIO_PE31_A_                     (1 << 31)
#define AT91SAM9263_PIO_PE31_B_                     (1 << 31)



#endif /* #ifndef _AT91SAM9263_H__ */
