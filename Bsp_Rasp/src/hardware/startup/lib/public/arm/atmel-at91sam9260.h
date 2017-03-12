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
 
 
#ifndef _AT91_H__
#define _AT91_H__

 /* Internal Memory Mapping */
#define AT91_BOOT_BASE		0x00000000
#define AT91_ITCM_BASE		0x00100000
#define AT91_DTCM_BASE		0x00200000
#define AT91_SRAM_BASE		0x00300000
#define AT91_ROM_BASE		0x00400000
#define AT91_LCD_BASE		0x00500000
#define AT91_UDP_RAM_BASE	0x00600000

/* External Bus Interface Mapping */
#define AT91_EBI0_CS0_BASE               0x10000000
#define AT91_EBI0_CS1_BASE               0x20000000  
#define AT91_EBI0_CS2_BASE               0x30000000
#define AT91_EBI0_CS3_BASE               0x40000000  
#define AT91_EBI0_CS4_BASE               0x50000000  
#define AT91_EBI0_CS5_BASE               0x60000000  

/* Peripheral Mapping */
#define AT91_TC_BASE			 0xfffa0000
#define AT91_UDP_BASE			 0xfffa4000
#define AT91_MCI_BASE                    0xfffa8000
#define AT91_TWI0_BASE                   0xfffac000
#define AT91_USART0_BASE                 0xfffb0000
#define AT91_USART1_BASE                 0xfffb4000
#define AT91_USART2_BASE                 0xfffb8000
#define AT91_SSC_BASE			 0xfffbc000
#define AT91_ISI_BASE			 0xfffc0000
#define AT91_EMAC_BASE                   0xfffc4000
#define AT91_SPI0_BASE                   0xfffc8000
#define AT91_SPI1_BASE                   0xfffcc000
#define AT91_USART3_BASE                 0xfffd0000
#define AT91_USART4_BASE                 0xfffd4000
#define AT91_USART5_BASE                 0xfffd8000
#define AT91_TCx_BASE			 0xfffdc000
#define AT91_ADC_BASE			 0xfffe0000
#define AT91_SYSC_BASE			 0xffffc000


/* System Controller Mapping Mapping */
#define AT91_DMAC_BASE			 0xffffe600
#define AT91_ECC_BASE			 0xffffe800
#define AT91_SDRAMC_BASE                 0xffffea00
#define AT91_SMC_BASE                    0xffffec00
#define AT91_MATRIX_BASE                 0xffffee00
#define AT91_AIC_BASE                    0xfffff000
#define AT91_DBGU_BASE                   0xfffff200
#define AT91_PIOA_BASE                   0xfffff400
#define AT91_PIOB_BASE                   0xfffff600
#define AT91_PIOC_BASE                   0xfffff800
#define AT91_PIOD_BASE                   0xfffffa00
#define AT91_PMC_BASE                    0xfffffc00
#define AT91_RSTC_BASE                   0xfffffd00
#define AT91_SHDWC_BASE                  0xfffffd10
#define AT91_RTCC0_BASE                   0xfffffd20
#define AT91_PIT_BASE                    0xfffffd30
#define AT91_WDTC_BASE                    0xfffffd40
#define AT91_SCKCR_BASE                    0xfffffd50
#define AT91_GPBR_BASE                   0xfffffd60
#define AT91_RTCC1_BASE                   0xfffffe00

/*
  * System Controller Register Declaration.
  */

  /* Static Memory Controller (SMC) */
#define AT91_SMC_SIZE                    0x80
#define AT91_SMC_SETUP(CS_NUM)           (0x00 + CS_NUM*0x10)
#define         AT91_SMC_NWESETUP       (0x3f << 0)                     /* NWE Setup Length */
#define                 AT91_SMC_NWESETUP_(x)   ((x) << 0)
#define         AT91_SMC_NCS_WRSETUP    (0x3f << 8)                     /* NCS Setup Length in Write Access */
#define                 AT91_SMC_NCS_WRSETUP_(x)        ((x) << 8)
#define         AT91_SMC_NRDSETUP       (0x3f << 16)                    /* NRD Setup Length */
#define                 AT91_SMC_NRDSETUP_(x)   ((x) << 16)
#define         AT91_SMC_NCS_RDSETUP    (0x3f << 24)                    /* NCS Setup Length in Read Access */
#define                 AT91_SMC_NCS_RDSETUP_(x)        ((x) << 24)

#define AT91_SMC_PULSE(CS_NUM)           (0x04 + CS_NUM*0x10)
#define         AT91_SMC_NWEPULSE       (0x7f <<  0)                    /* NWE Pulse Length */
#define                 AT91_SMC_NWEPULSE_(x)   ((x) << 0)
#define         AT91_SMC_NCS_WRPULSE    (0x7f <<  8)                    /* NCS Pulse Length in Write Access */
#define                 AT91_SMC_NCS_WRPULSE_(x)((x) << 8)
#define         AT91_SMC_NRDPULSE       (0x7f << 16)                    /* NRD Pulse Length */
#define                 AT91_SMC_NRDPULSE_(x)   ((x) << 16)
#define         AT91_SMC_NCS_RDPULSE    (0x7f << 24)                    /* NCS Pulse Length in Read Access */
#define                 AT91_SMC_NCS_RDPULSE_(x)((x) << 24)

#define AT91_SMC_CYCLE(CS_NUM)           (0x08 + CS_NUM*0x10)
#define         AT91_SMC_NWECYCLE       (0x1ff << 0 )                   /* Total Write Cycle Length */
#define                 AT91_SMC_NWECYCLE_(x)   ((x) << 0)
#define         AT91_SMC_NRDCYCLE       (0x1ff << 16)                   /* Total Read Cycle Length */
#define                 AT91_SMC_NRDCYCLE_(x)   ((x) << 16)

#define AT91_SMC_MODE(CS_NUM)            (0x0c + CS_NUM*0x10)
#define         AT91_SMC_READMODE       (1 <<  0)                       /* Read Mode */
#define         AT91_SMC_WRITEMODE      (1 <<  1)                       /* Write Mode */
#define         AT91_SMC_EXNWMODE       (3 <<  4)                       /* NWAIT Mode */
#define                 AT91_SMC_EXNWMODE_DISABLE       (0 << 4)
#define                 AT91_SMC_EXNWMODE_FROZEN        (2 << 4)
#define                 AT91_SMC_EXNWMODE_READY         (3 << 4)
#define         AT91_SMC_BAT            (1 <<  8)                       /* Byte Access Type */
#define                 AT91_SMC_BAT_SELECT             (0 << 8)
#define                 AT91_SMC_BAT_WRITE              (1 << 8)
#define         AT91_SMC_DBW            (3 << 12)                       /* Data Bus Width */
#define                 AT91_SMC_DBW_8                  (0 << 12)
#define                 AT91_SMC_DBW_16                 (1 << 12)
#define                 AT91_SMC_DBW_32                 (2 << 12)
#define         AT91_SMC_TDF            (0xf << 16)                     /* Data Float Time. */
#define                 AT91_SMC_TDF_(x)                ((x) << 16)
#define         AT91_SMC_TDFMODE        (1 << 20)                       /* TDF Optimization - Enabled */
#define         AT91_SMC_PMEN           (1 << 24)                       /* Page Mode Enabled */
#define         AT91_SMC_PS             (3 << 28)                       /* Page Size */
#define                 AT91_SMC_PS_4                   (0 << 28)
#define                 AT91_SMC_PS_8                   (1 << 28)
#define                 AT91_SMC_PS_16                  (2 << 28)
#define                 AT91_SMC_PS_32                  (3 << 28)


/* Bus Matrix */
#define AT91_MATRIX_SIZE                 0x110
#define AT91_MATRIX_MCFG0                0x000
#define AT91_MATRIX_MCFG1                0x004
#define AT91_MATRIX_MCFG2                0x008
#define AT91_MATRIX_MCFG3                0x00c
#define AT91_MATRIX_MCFG4                0x010
#define AT91_MATRIX_MCFG5                0x014
#define AT91_MATRIX_MCFG6                0x018
#define AT91_MATRIX_MCFG7                0x01c
#define AT91_MATRIX_MCFG8                0x020
#define AT91_MATRIX_SCFG0                0x40
#define AT91_MATRIX_SCFG1                0x44
#define AT91_MATRIX_SCFG2                0x48
#define AT91_MATRIX_SCFG3                0x4c
#define AT91_MATRIX_SCFG4                0x50
#define AT91_MATRIX_SCFG5                0x54
#define AT91_MATRIX_SCFG6                0x58
#define AT91_MATRIX_SCFG7                0x5c
#define AT91_MATRIX_PRAS0                0x080
#define AT91_MATRIX_PRBS0                0x084
#define AT91_MATRIX_PRAS1                0x088
#define AT91_MATRIX_PRBS1                0x08c
#define AT91_MATRIX_PRAS2                0x090
#define AT91_MATRIX_PRBS2                0x094
#define AT91_MATRIX_PRAS3                0x098
#define AT91_MATRIX_PRBS3                0x09c
#define AT91_MATRIX_PRAS4                0x0a0
#define AT91_MATRIX_PRBS4                0x0a4
#define AT91_MATRIX_PRAS5                0x0a8
#define AT91_MATRIX_PRBS5                0x0ac
#define AT91_MATRIX_PRAS6                0x0b0
#define AT91_MATRIX_PRBS6                0x0b4
#define AT91_MATRIX_PRAS7                0x0b8
#define AT91_MATRIX_PRBS7                0x0bc
#define AT91_MATRIX_MRCR                 0x100

/* Chip Configuration */
#define AT91_CCFG_SIZE                   0xf0
#define AT91_CCFG_MATRIX_TCMR            0x04
#define AT91_CCFG_EBI_CSA                0x11C

/* (DBGU) */
#define AT91_DBGU_SIZE                   0x4c
#define AT91_DBGU_CR                     0x00
#define AT91_DBGU_MR                     0x04
#define AT91_DBGU_IER                    0x08
#define AT91_DBGU_IDR                    0x0c
#define AT91_DBGU_IMR                    0x10
#define AT91_DBGU_SR                     0x14
#define AT91_DBGU_RHR                    0x18
#define AT91_DBGU_THR                    0x1c
#define AT91_DBGU_BRGR                   0x20
#define AT91_DBGU_CIDR                   0x40
#define AT91_DBGU_EXID                   0x44
#define AT91_DBGU_FNR                    0x48

/* Advanced Interrupt Controller */
#define AT91_AIC_SIZE                    0x200
#define AT91_AIC_SMR(x)                  (0x4*x)
#define AT91_AIC_SMR0                    AT91_AIC_SMR(0 )
#define AT91_AIC_SMR1                    AT91_AIC_SMR(1 )
#define AT91_AIC_SMR2                    AT91_AIC_SMR(2 )
#define AT91_AIC_SMR3                    AT91_AIC_SMR(3 )
#define AT91_AIC_SMR4                    AT91_AIC_SMR(4 )
#define AT91_AIC_SMR5                    AT91_AIC_SMR(5 )
#define AT91_AIC_SMR6                    AT91_AIC_SMR(6 )
#define AT91_AIC_SMR7                    AT91_AIC_SMR(7 )
#define AT91_AIC_SMR8                    AT91_AIC_SMR(8 )
#define AT91_AIC_SMR9                    AT91_AIC_SMR(9 )
#define AT91_AIC_SMR10                   AT91_AIC_SMR(10)
#define AT91_AIC_SMR11                   AT91_AIC_SMR(11)
#define AT91_AIC_SMR12                   AT91_AIC_SMR(12)
#define AT91_AIC_SMR13                   AT91_AIC_SMR(13)
#define AT91_AIC_SMR14                   AT91_AIC_SMR(14)
#define AT91_AIC_SMR15                   AT91_AIC_SMR(15)
#define AT91_AIC_SMR16                   AT91_AIC_SMR(16)
#define AT91_AIC_SMR17                   AT91_AIC_SMR(17)
#define AT91_AIC_SMR18                   AT91_AIC_SMR(18)
#define AT91_AIC_SMR19                   AT91_AIC_SMR(19)
#define AT91_AIC_SMR20                   AT91_AIC_SMR(20)
#define AT91_AIC_SMR21                   AT91_AIC_SMR(21)
#define AT91_AIC_SMR22                   AT91_AIC_SMR(22)
#define AT91_AIC_SMR23                   AT91_AIC_SMR(23)
#define AT91_AIC_SMR24                   AT91_AIC_SMR(24)
#define AT91_AIC_SMR25                   AT91_AIC_SMR(25)
#define AT91_AIC_SMR26                   AT91_AIC_SMR(26)
#define AT91_AIC_SMR27                   AT91_AIC_SMR(27)
#define AT91_AIC_SMR28                   AT91_AIC_SMR(28)
#define AT91_AIC_SMR29                   AT91_AIC_SMR(29)
#define AT91_AIC_SMR30                   AT91_AIC_SMR(30)
#define AT91_AIC_SMR31                   AT91_AIC_SMR(31)
#define AT91_AIC_SVR(x)                  (0x4*x + 0x80)
#define AT91_AIC_SVR0                    AT91_AIC_SVR(0 )
#define AT91_AIC_SVR1                    AT91_AIC_SVR(1 )
#define AT91_AIC_SVR2                    AT91_AIC_SVR(2 )
#define AT91_AIC_SVR3                    AT91_AIC_SVR(3 )
#define AT91_AIC_SVR4                    AT91_AIC_SVR(4 )
#define AT91_AIC_SVR5                    AT91_AIC_SVR(5 )
#define AT91_AIC_SVR6                    AT91_AIC_SVR(6 )
#define AT91_AIC_SVR7                    AT91_AIC_SVR(7 )
#define AT91_AIC_SVR8                    AT91_AIC_SVR(8 )
#define AT91_AIC_SVR9                    AT91_AIC_SVR(9 )
#define AT91_AIC_SVR10                   AT91_AIC_SVR(10)
#define AT91_AIC_SVR11                   AT91_AIC_SVR(11)
#define AT91_AIC_SVR12                   AT91_AIC_SVR(12)
#define AT91_AIC_SVR13                   AT91_AIC_SVR(13)
#define AT91_AIC_SVR14                   AT91_AIC_SVR(14)
#define AT91_AIC_SVR15                   AT91_AIC_SVR(15)
#define AT91_AIC_SVR16                   AT91_AIC_SVR(16)
#define AT91_AIC_SVR17                   AT91_AIC_SVR(17)
#define AT91_AIC_SVR18                   AT91_AIC_SVR(18)
#define AT91_AIC_SVR19                   AT91_AIC_SVR(19)
#define AT91_AIC_SVR20                   AT91_AIC_SVR(20)
#define AT91_AIC_SVR21                   AT91_AIC_SVR(21)
#define AT91_AIC_SVR22                   AT91_AIC_SVR(22)
#define AT91_AIC_SVR23                   AT91_AIC_SVR(23)
#define AT91_AIC_SVR24                   AT91_AIC_SVR(24)
#define AT91_AIC_SVR25                   AT91_AIC_SVR(25)
#define AT91_AIC_SVR26                   AT91_AIC_SVR(26)
#define AT91_AIC_SVR27                   AT91_AIC_SVR(27)
#define AT91_AIC_SVR28                   AT91_AIC_SVR(28)
#define AT91_AIC_SVR29                   AT91_AIC_SVR(29)
#define AT91_AIC_SVR30                   AT91_AIC_SVR(30)
#define AT91_AIC_SVR31                   AT91_AIC_SVR(31)
#define AT91_AIC_IVR                     0x100
#define AT91_AIC_FVR                     0x104
#define AT91_AIC_ISR                     0x108
#define AT91_AIC_IPR                     0x10c
#define AT91_AIC_IMR                     0x110
#define AT91_AIC_CISR                    0x114
#define AT91_AIC_IECR                    0x120
#define AT91_AIC_IDCR                    0x124
#define AT91_AIC_ICCR                    0x128
#define AT91_AIC_ISCR                    0x12c
#define AT91_AIC_EOICR                   0x130
#define AT91_AIC_SPU                     0x134
#define AT91_AIC_DCR                     0x138
#define AT91_AIC_FFER                    0x140
#define AT91_AIC_FFDR                    0x144
#define AT91_AIC_FFSR                    0x148

/* PIO Controller */
#define AT91_PIO_SIZE                    0x200
#define AT91_PIO_PER                     0x00
#define AT91_PIO_PDR                     0x04
#define AT91_PIO_PSR                     0x08
#define AT91_PIO_OER                     0x10
#define AT91_PIO_ODR                     0x14
#define AT91_PIO_OSR                     0x18
#define AT91_PIO_IFER                    0x20
#define AT91_PIO_IFDR                    0x24
#define AT91_PIO_IFSR                    0x28
#define AT91_PIO_SODR                    0x30
#define AT91_PIO_CODR                    0x34
#define AT91_PIO_ODSR                    0x38
#define AT91_PIO_PDSR                    0x3c
#define AT91_PIO_IER                     0x40
#define AT91_PIO_IDR                     0x44
#define AT91_PIO_IMR                     0x48
#define AT91_PIO_ISR                     0x4c
#define AT91_PIO_MDER                    0x50
#define AT91_PIO_MDDR                    0x54
#define AT91_PIO_MDSR                    0x58
#define AT91_PIO_PUDR                    0x60
#define AT91_PIO_PUER                    0x64
#define AT91_PIO_PUSR                    0x68
#define AT91_PIO_ASR                     0x70
#define AT91_PIO_BSR                     0x74
#define AT91_PIO_ABSR                    0x78
#define AT91_PIO_OWER                    0xa0
#define AT91_PIO_OWDR                    0xa4
#define AT91_PIO_OWSR                    0xa8

/* Power Management Controller */
#define AT91_PMC_SIZE                    0x84
#define AT91_PMC_SCER                    0x00
#define AT91_PMC_SCDR                    0x04
#define AT91_PMC_SCSR                    0x08
#define AT91_PMC_PCER                    0x10
#define AT91_PMC_PCDR                    0x14
#define AT91_PMC_PCSR                    0x18
#define AT91_PMC_MOR                     0x20
#define AT91_PMC_MCFR                    0x24
#define AT91_PMC_PLLAR                   0x28
#define AT91_PMC_PLLBR                   0x2c
#define AT91_PMC_MCKR                    0x30
#define AT91_PMC_PCK0                    0x40
#define AT91_PMC_PCK1                    0x44
#define AT91_PMC_IER                     0x60
#define AT91_PMC_IDR                     0x64
#define AT91_PMC_SR                      0x68
#define AT91_PMC_IMR                     0x6c
#define AT91_PMC_PLLICPR                 0x80

/* Reset Controller (RSTC) */
#define AT91_RSTC_SIZE                   0x0C
#define AT91_RSTC_CR                     0x00
#define AT91_RSTC_SR                     0x04
#define AT91_RSTC_MR                     0x08

/* Shutdown Controller */
#define AT91_SHDWC_SIZE                  0x0c
#define AT91_SHDWC_CR                    0x00
#define AT91_SHDWC_MR                    0x04
#define AT91_SHDWC_SR                    0x08

/* Realtime Timer */
#define AT91_RTT_SIZE                    0x10
#define AT91_RTT_MR                      0x00
#define AT91_RTT_AR                      0x04
#define AT91_RTT_VR                      0x08
#define AT91_RTT_SR                      0x0c

/* Periodic Interval Timer */
#define AT91_PIT_SIZE                    0x10
#define AT91_PIT_MR                      0x00
#define AT91_PIT_SR                      0x04
#define AT91_PIT_PIVR                    0x08
#define AT91_PIT_PIIR                    0x0c

/* Watchdog Timer */
#define AT91_WDT_SIZE                    0x0c
#define AT91_WDT_CR                      0x00
#define AT91_WDT_MR                      0x04
#define AT91_WDT_SR                      0x08

/* DBGU bit defination */

#define AT91_DBGU_CR_RSTSTA                  0x00000100
#define AT91_DBGU_CR_TXDIS                   0x00000080
#define AT91_DBGU_CR_TXEN                    0x00000040
#define AT91_DBGU_CR_RXDIS                   0x00000020
#define AT91_DBGU_CR_RXEN                    0x00000010
#define AT91_DBGU_CR_RSTTX                   0x00000008
#define AT91_DBGU_CR_RSTRX                   0x00000004

#define AT91_DBGU_MR_CHMODE(mode)            ((mode & 3) << 14)
#define AT91_DBGU_MR_CHMODE_MASK             AT91_DBGU_MR_CHMODE(3)
#define AT91_DBGU_MR_CHMODE_NORMAL           AT91_DBGU_MR_CHMODE(0)
#define AT91_DBGU_MR_CHMODE_AUTO_ECHO        AT91_DBGU_MR_CHMODE(1)
#define AT91_DBGU_MR_CHMODE_LOCAL_LOOPB      AT91_DBGU_MR_CHMODE(2)
#define AT91_DBGU_MR_CHMODE_REMOTE_LOOPB     AT91_DBGU_MR_CHMODE(3)

#define AT91_DBGU_MR_PAR(par)                ((par  & 7) << 9)
#define AT91_DBGU_MR_PAR_MASK               AT91_DBGU_MR_PAR(7)
#define AT91_DBGU_MR_PAR_EVEN               AT91_DBGU_MR_PAR(0)
#define AT91_DBGU_MR_PAR_ODD                AT91_DBGU_MR_PAR(1)
#define AT91_DBGU_MR_PAR_SPACE              AT91_DBGU_MR_PAR(2)
#define AT91_DBGU_MR_PAR_MARK               AT91_DBGU_MR_PAR(3)
#define AT91_DBGU_MR_PAR_NONE               AT91_DBGU_MR_PAR(4)

#define AT91_DBGU_IE_SR_COMMRX               0x80000000
#define AT91_DBGU_IE_SR_COMMTX               0x40000000
#define AT91_DBGU_IE_SR_RXBUFF               0x00001000
#define AT91_DBGU_IE_SR_TXBUFFE              0x00000800
#define AT91_DBGU_IE_SR_TXEMPTY              0x00000200
#define AT91_DBGU_IE_SR_PARE                 0x00000080
#define AT91_DBGU_IE_SR_FRAME                0x00000040
#define AT91_DBGU_IE_SR_OVERE                0x00000020
#define AT91_DBGU_IE_SR_ENDTX                0x00000010
#define AT91_DBGU_IE_SR_ENDRX                0x00000008
#define AT91_DBGU_IE_SR_TXRDY                0x00000002
#define AT91_DBGU_IE_SR_RXRDY                0x00000001

#define AT91_DBGU_RHR_THR_CHAR_MASK          0x000000ff

#define AT91_DBGU_BRGR_CD_MASK               0x0000ffff
#define AT91_DBGU_BRGR_CD_DISABLE            0x00000000
#define AT91_DBGU_BRGR_CD_MCK                0x00000001

#define AT91_DBGU_CIDR_EXT                   0x80000000
#define AT91_DBGU_CIDR_NVPTYP                0x70000000
#define AT91_DBGU_CIDR_ARCH                  0x0ff00000
#define AT91_DBGU_CIDR_SRAMSIZ               0x000f0000
#define AT91_DBGU_CIDR_NVPSIZ2               0x0000f000
#define AT91_DBGU_CIDR_NVPSIZ                0x00000f00
#define AT91_DBGU_CIDR_EPROC                 0x000000e0
#define AT91_DBGU_CIDR_VERSION               0x0000001f

#define AT91_DBGU_EXID_MASK                  0xffffffff

#define AT91_DBGU_FNR_FNTRST                 0x00000001

#define AT91_INTR_EMAC                       21

#endif /* #ifndef _AT91_H__ */
