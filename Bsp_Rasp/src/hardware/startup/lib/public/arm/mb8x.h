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


/* Core Program Status Register */
#define MB8X_CPSR_MASK				    0x1f
#define MB8X_CPSR_SVC_IF			    0xd3


/* Clock and Reset Generator */
#define MB8X_CRG_CTRL_BASE              0xfffe7000
#define MB8X_CRG_CRPR                   0x00
#define MB8X_CRG_CRWR			        0x08
#define MB8X_CRG_CRSR                   0x0c
#define MB8X_CRG_CRDA                   0x10
#define MB8X_CRG_CRDB                   0x14
#define	MB8X_CRG_CRHA                   0x18
#define	MB8X_CRG_CRPA                   0x1c
#define MB8X_CRG_CRPB                   0x20
#define MB8X_CRG_CRHB                   0x24
#define MB8X_CRG_CRAM                   0x28


/* Remap Boot Controller */
#define MB8X_RBC_BASE                   0xfffe6000
#define MB8X_RBC_REMAP                  0x04
#define MB8X_RBC_VIHA                   0x08
#define MB8X_RBC_ITRA                   0x0c
#define MB8X_RBC_GPR0                   0x20
#define MB8X_RBC_GPR1                   0x24
#define MB8X_RBC_GPR2                   0x28
#define MB8X_RBC_GPR3                   0x2c


/* IRC interrupt controllers */         
#define MB8X_INTR1_BASE                 0xfffe8000
#define MB8X_INTR2_BASE                 0xfffb0000
#define MB8X_INTR_SIZE                  0x400
#define MB8X_INTR_IRQF                  0x00
#define MB8X_INTR_IRQM                  0x04
#define MB8X_INTR_ILM                   0x08
#define MB8X_INTR_ICRMN                 0x0c
#define MB8X_INTR_HRCL                  0x10
#define MB8X_INTR_RESV1                 0x14
#define MB8X_INTR_RESV2                 0x18
#define MB8X_INTR_TBR                   0x1c
#define MB8X_INTR_VCT                   0x20
#define MB8X_INTR_IRQTEST               0x24
#define MB8X_INTR_FIQTEST               0x28
#define MB8X_INTR_RESV3                 0x2c
#define MB8X_INTR_ICR00                 0x30
#define MB8X_INTR_ICR01                 0x34
#define MB8X_INTR_ICR02                 0x38
#define MB8X_INTR_ICR03                 0x3c
#define MB8X_INTR_ICR04                 0x40
#define MB8X_INTR_ICR05                 0x44
#define MB8X_INTR_ICR06                 0x48
#define MB8X_INTR_ICR07                 0x4c
#define MB8X_INTR_ICR08                 0x50
#define MB8X_INTR_ICR09                 0x54
#define MB8X_INTR_ICR10                 0x58
#define MB8X_INTR_ICR11                 0x5c
#define MB8X_INTR_ICR12                 0x60
#define MB8X_INTR_ICR13                 0x64
#define MB8X_INTR_ICR14                 0x68
#define MB8X_INTR_ICR15                 0x6c
#define MB8X_INTR_ICR16                 0x70
#define MB8X_INTR_ICR17                 0x74
#define MB8X_INTR_ICR18                 0x78
#define MB8X_INTR_ICR19                 0x7c
#define MB8X_INTR_ICR20                 0x80
#define MB8X_INTR_ICR21                 0x84
#define MB8X_INTR_ICR22                 0x88
#define MB8X_INTR_ICR23                 0x8c
#define MB8X_INTR_ICR24                 0x90
#define MB8X_INTR_ICR25                 0x94
#define MB8X_INTR_ICR26                 0x98
#define MB8X_INTR_ICR27                 0x9c
#define MB8X_INTR_ICR28                 0xa0
#define MB8X_INTR_ICR29                 0xa4
#define MB8X_INTR_ICR30                 0xa8
#define MB8X_INTR_ICR31                 0xac

/* interrupt mask values */
#define MB8X_INTR_ENABLE                0x07        /* Arbitraly choosen */
#define MB8X_INTR_DISABLE               0x0f        /* Dictated by IRC   */

/* External Bus Interface / Memory controller (MEMC) */
#define MB8X_MEMC_BASE                  0xfffc0000
#define MB8X_MEMC_MCFMODE0              0x0000
#define MB8X_MEMC_MCFMODE2              0x0008
#define MB8X_MEMC_MCFMODE4              0x0010
#define MB8X_MEMC_MCFTIM0               0x0020
#define MB8X_MEMC_MCFTIM2               0x0028
#define MB8X_MEMC_MCFTIM4               0x0030
#define MB8X_MEMC_MCAREA0               0x0040
#define MB8X_MEMC_MCAREA1               0x0044
#define MB8X_MEMC_MCAREA2               0x0048
#define MB8X_MEMC_MCAREA4               0x0050
#define MB8X_MEMC_MCERR                 0x0200

/* NGTBD */


/* DDR2 controler */
#define MB8X_DDR2_BASE                  0xf3000000
#define MB8X_DDR2_DRIC                  0x00
#define MB8X_DDR2_DRIC1                 0x02
#define MB8X_DDR2_DRIC2                 0x04
#define MB8X_DDR2_DRCA                  0x06
#define MB8X_DDR2_DRCM                  0x08
#define MB8X_DDR2_DRCST1                0x0a
#define MB8X_DDR2_DRCST2                0x0c
#define MB8X_DDR2_DRCR                  0x0e
#define MB8X_DDR2_DRCF                  0x20
#define MB8X_DDR2_DRASR                 0x30
#define MB8X_DDR2_DRIMS1                0x42
#define MB8X_DDR2_DRIMS2A1              0x44
#define MB8X_DDR2_DRIMS3A2              0x46
#define MB8X_DDR2_DRIMS4                0x48
#define MB8X_DDR2_DRIMS5                0x4a
#define MB8X_DDR2_DRIMS6                0x4c
#define MB8X_DDR2_DRIMS7D1              0x4e
#define MB8X_DDR2_DRIMS8D2              0x50
#define MB8X_DDR2_DROS                  0x60
#define MB8X_DDR2_DRIBSODT1             0x64
#define MB8X_DDR2_DRIBSOCD              0x66
#define MB8X_DDR2_DRIBSOCD2             0x68
#define MB8X_DDR2_DROABA                0x70
#define MB8X_DDR2_DROBS                 0x84
#define MB8X_DDR2_DRIMR1                0x90
#define MB8X_DDR2_DRIMR2                0x92
#define MB8X_DDR2_DRIMR3                0x94
#define MB8X_DDR2_DRIMR4                0x96
#define MB8X_DDR2_DROISR1               0x98
#define MB8X_DDR2_DROISR2               0x9a


/* Internal RAM */
#define MB8X_INTRA_BASE                 0x01000000
#define MB8X_INTRA_LENGTH               0x10000


/* DMA Controller */
#define MB8X_DMAC_BASE                  0xfffd0000
#define MB8X_DMAC_DMACR                 0x00
#define MB8X_DMAC_DMACA(ch)             ((ch*0x10) + 0x10)
#define MB8X_DMAC_DMACB(ch)             ((ch*0x10) + 0x14)
#define MB8X_DMAC_DMACSA(ch)            ((ch*0x10) + 0x18)
#define MB8X_DMAC_DMACDA(ch)            ((ch*0x10) + 0x1c)


/* Timer */
#define MB8X_TIMER0_BASE                0xfffe0000
#define MB8X_TIMER1_BASE                0xfffe0020
#define MB8X_TIMER_SIZE                 0x20
#define MB8X_LOAD_TIM                   0x00
#define MB8X_READ_TIM                   0x04
#define MB8X_TIMER_CNTL                 0x08
#define MB8X_TIMER_IC                   0x0c
#define MB8X_TIMER_IS                   0x10
#define MB8X_TIMER_IMS                  0x14
#define MB8X_TIMER_BGL                  0x18


/* General-purpose I/O */
#define MB8X_GPIO_BASE                  0xffff9000
#define MB8X_GPIO_PDR0                  0x00
#define MB8X_GPIO_PDR1                  0x04
#define MB8X_GPIO_PDR2                  0x08
#define MB8X_GPIO_DDR0                  0x10
#define MB8X_GPIO_DDR1                  0x14
#define MB8X_GPIO_DDR2                  0x18
#define MB8X_GPIO_PFR0                  0x20
#define MB8X_GPIO_PFR1                  0x24
#define MB8X_GPIO_PFR2                  0x28

/* PWM */
#define MB8X_PWM0_BASE                  0xfff41000
#define MB8X_PWM1_BASE                  0xfff41100
#define MB8X_PWM_BCR                    0x00
#define MB8X_PWM_TPR                    0x04
#define MB8X_PWM_PR                     0x08
#define MB8X_PWM_DR                     0x0c
#define MB8X_PWM_CR                     0x10
#define MB8X_PWM_SR                     0x14
#define MB8X_PWM_CCR                    0x18
#define MB8X_PWM_IR                     0x1c

/* A/D Converter */
#define MB8X_ADC0_BASE                  0xfff52000
#define MB8X_ADC1_BASE                  0xfff53000
#define MB8X_ADC_DATA                   0x00
#define MB8X_ADC_XPD                    0x08
#define MB8X_ADC_CKSEL                  0x10

/* Graphics display controller */

/* Serial audio interface (I2S)*/
#define MB8X_I2S0_BASE                  0xffee0000
#define MB8X_I2S1_BASE                  0xffef0000
#define MB8X_I2S2_BASE                  0xfff00000
#define MB8X_I2S_RXFDAT                 0x00
#define MB8X_I2S_TXFDAT                 0x04
#define MB8X_I2S_CNTREG                 0x08
#define MB8X_I2S_MCR0REG                0x0c
#define MB8X_I2S_MCR1REG                0x10
#define MB8X_I2S_MCR2REG                0x14
#define MB8X_I2S_OPRREG                 0x18
#define MB8X_I2S_SRST                   0x1c
#define MB8X_I2S_INTCNT                 0x20
#define MB8X_I2S_STATUS                 0x24
#define MB8X_I2S_DMAACT                 0x28
#define MB8X_I2S_TSTREG                 0x2c

/* UART */
#define MB8X_UART_1_BASE                0xfffe1000
#define MB8X_UART_2_BASE                0xfffe2000
#define MB8X_UART_3_BASE                0xfff50000
#define MB8X_UART_4_BASE                0xfff51000
#define MB8X_UART_5_BASE                0xfff43000
#define MB8X_UART_6_BASE                0xfff44000
#define MB8X_UART_SIZE                  0x30
#define MB8X_UART_RHR                   0x00
#define MB8X_UART_THR                   0x00
#define MB8X_UART_IER                   0x04
#define MB8X_UART_DLH                   0x04
#define MB8X_UART_IIR                   0x08
#define MB8X_UART_FCR                   0x08
#define MB8X_UART_EFR                   0x08
#define MB8X_UART_LCR                   0x0C
#define MB8X_UART_MCR                   0x10
#define MB8X_UART_XON1                  0x10
#define MB8X_UART_ADDR1                 0x10
#define MB8X_UART_LSR                   0x14
#define MB8X_UART_XON2                  0x14
#define MB8X_UART_ADDR2                 0x14
#define MB8X_UART_MSR                   0x18
#define MB8X_UART_TCR                   0x18
#define MB8X_UART_XOFF1                 0x18
#define MB8X_UART_SCR                   0x1C
#define MB8X_UART_SPR                   0x1C
#define MB8X_UART_TLR                   0x1C
#define MB8X_UART_XOFF2                 0x1C
#define MB8X_UART_DLL                   0x20
#define MB8X_UART_MDR1                  0x20
#define MB8X_UART_DLM 	                0x24
#define MB8X_UART_MDR2	                0x24
#define MB8X_UART_SFLSR	                0x28
#define MB8X_UART_TXFLL	                0x28
#define MB8X_UART_RESUME                0x2C
#define MB8X_UART_TXFLH	                0x2C
#define	MB8X_UART_SFREGL                0x30
#define MB8X_UART_RXFLL	                0x30
#define MB8X_UART_SFREGH                0x34
#define MB8X_UART_RXFLH	                0x34
#define	MB8X_UART_BLR	                0x38
#define MB8X_UART_UASR	                0x38
#define	MB8X_UART_ACREG	                0x3C
#define MB8X_UART_DIV1_6                0x3C
#define	MB8X_UART_SSR	                0x44
#define MB8X_UART_EBLR	                0x48
#define MB8X_UART_OSC_12M_SEL           0x4C
#define MB8X_UART_MVR                   0x50  

/* I2C bus interface */
#define	MB8X_I2C_1_BASE                 0xfff56000
#define	MB8X_I2C_2_BASE                 0xfff57000
#define	MB8X_I2C_SIZE                   0x40

#define	MB8X_I2C_BSR                    0x00
#define	MB8X_I2C_BCR                    0x04
#define	MB8X_I2C_CCR                    0x08
#define	MB8X_I2C_ADR                    0x0c
#define	MB8X_I2C_DAR                    0x10
#define	MB8X_I2C_ECSR                   0x14
#define	MB8X_I2C_BCFR                   0x18
#define	MB8X_I2C_BC2R                   0x1c

/* Bit definition for Bus Status Register */
#define	MB8X_I2C_BSR_FBT                0x01
#define	MB8X_I2C_BSR_GCA                0x02
#define	MB8X_I2C_BSR_AAS                0x04
#define	MB8X_I2C_BSR_TRX                0x08
#define	MB8X_I2C_BSR_LRB                0x10
#define	MB8X_I2C_BSR_AL                 0x20
#define	MB8X_I2C_BSR_RSC                0x40
#define	MB8X_I2C_BSR_BB                 0x80

/* Bit definition for Bus Control Register */
#define	MB8X_I2C_BCR_INT                0x01
#define	MB8X_I2C_BCR_INTE               0x02
#define	MB8X_I2C_BCR_GCAA               0x04
#define	MB8X_I2C_BCR_ACK                0x08
#define	MB8X_I2C_BCR_MSS                0x10
#define	MB8X_I2C_BCR_SCC                0x20
#define	MB8X_I2C_BCR_BEIE               0x40
#define	MB8X_I2C_BCR_BER                0x80

/* Bit definition for CLOCK Control Register */
#define	MB8X_I2C_CCR_CS_MASK            0x1f
#define	MB8X_I2C_CCR_EN                 0x20
#define	MB8X_I2C_CCR_HSM                0x40
#define	MB8X_I2C_CCR_RESV               0x80

/* Bit definition for ADdress Register */
#define	MB8X_I2C_ADR_MASK               0x7f
#define	MB8X_I2C_ADR_RESV               0x80

/* Bit definition for DAta Register */
#define	MB8X_I2C_DAR_MASK               0xff

/* Bit definition for Two Bus Control Register */
#define	MB8X_I2C_BC2R_SCLL              0x01
#define	MB8X_I2C_BC2R_SDAL              0x02
#define	MB8X_I2C_BC2R_RESV1             0x0c
#define	MB8X_I2C_BC2R_SCLS              0x10
#define	MB8X_I2C_BC2R_SDAS              0x20
#define	MB8X_I2C_BC2R_RESV2             0xc0

/* Bit definition for Expension CS Register */
#define	MB8X_I2C_ECSR_CS_MASK           0x3f
#define	MB8X_I2C_ECSR_RESV              0xc0

/* Bit definition for Bus Clock Frequency Register */
#define	MB8X_I2C_BCFR_FS_MASK           0x0f
#define	MB8X_I2C_BCFR_RESV              0xf0


/* Serial peripheral interface */
#define MB8X_SPI_BASE                   0xfff40000
#define MB8X_SPI_SPCR                   0x00
#define MB8X_SPI_SSCR                   0x04
#define MB8X_SPI_SPDR                   0x08
#define MB8X_SPI_SPSR                   0x0c

/* CAN interface */

/* MediaLB interface */

/* USB EHCI host controller */
#define MB8X_EHCI_BASE                  0xfff80000
#define MB8X_EHCI_HSCAPBASE             0x00
#define MB8X_EHCI_HCSPARAMS             0x04
#define MB8X_EHCI_HCCPARAMS             0x08
#define MB8X_EHCI_USBCMD                0x10
#define MB8X_EHCI_USBSTS                0x14
#define MB8X_EHCI_USBINTR               0x18
#define MB8X_EHCI_FRINDEX               0x1c
#define MB8X_EHCI_CTRLDSSEGMENT         0x20
#define MB8X_EHCI_PERIODICLISTBASE      0x24
#define MB8X_EHCI_ASYNCLISTADDR         0x28
#define MB8X_EHCI_CONFIGFFLAG           0x50
#define MB8X_EHCI_PORTSC_1              0x54
#define MB8X_EHCI_INSNREG00             0x90
#define MB8X_EHCI_INSNREG01             0x94
#define MB8X_EHCI_INSNREG02             0x98
#define MB8X_EHCI_INSNREG03             0x9c
#define MB8X_EHCI_INSNREG04             0xa0
#define MB8X_EHCI_INSNREG05             0xa4


/* USB OHCI host controller */
#define MB8X_OHCI_BASE                  0xfff81000
#define MB8X_OHCI_HCREVISION            0x00
#define MB8X_OHCI_HCCONTROL             0x04
#define MB8X_OHCI_HCCMDSTATUS           0x08
#define MB8X_OHCI_HCINTRSTATUS          0x0c
#define MB8X_OHCI_HCINTRENABLE          0x10
#define MB8X_OHCI_HCINTRDISABLE         0x14
#define MB8X_OHCI_HCHCCA                0x18
#define MB8X_OHCI_HCPERIODICCURRENTED   0x1c
#define MB8X_OHCI_HCCONTROLHEADED       0x20
#define MB8X_OHCI_HCCONTROLCURRENTED    0x24
#define MB8X_OHCI_HCBULDHEADED          0x28
#define MB8X_OHCI_HCBULKCURRENTED       0x2c
#define MB8X_OHCI_HCDONEHEAD            0x30
#define MB8X_OHCI_HCFMINTERVAL          0x34
#define MB8X_OHCI_HCFRREMAINING         0x38
#define MB8X_OHCI_HCFMNUMBER            0x3c
#define MB8X_OHCI_HCPERIODICSTART       0x40
#define MB8X_OHCI_HCLSTHRESHOLD         0x44
#define MB8X_OHCI_HCRHDESCRIPTORA       0x48
#define MB8X_OHCI_HCRHDESCRIPTORB       0x4c
#define MB8X_OHCI_HCRHSTATUS            0x50
#define MB8X_OHCI_HCRHPORTSTATUS        0x54

/* USB host controller PHY */
#define MB8X_HC_PHY_BASE                0xfff82000
#define MB8X_HC_PHY_MODE_SETTING        0x00
#define MB8X_HC_PHY_MODE_SETTING1       0x04
#define MB8X_HC_PHY_MODE_SETTING2       0x08


/* USB function controller */

/* IDE host controller */

/* Chip Control Module (CCNT) */
#define MB8X_CCNT_BASE                  0xfff42000
#define MB8X_CCNT_CCID                  0x00
#define MB8X_CCNT_CSRST                 0x04
#define MB8X_CCNT_CIST                  0x10
#define MB8X_CCNT_CISTM                 0x14
#define MB8X_CCNT_CGPIO_IST             0x18
#define MB8X_CCNT_CGPIO_ISTM            0x1c
#define MB8X_CCNT_CGPIO_IP              0x20
#define MB8X_CCNT_CPGPIO_IM             0x24
#define MB8X_CCNT_CAXI_BW               0x28
#define MB8X_CCNT_CAXI_PS               0x2c
#define MB8X_CCNT_CMUX_MD               0x30
#define MB8X_CCNT_CEX_PIN_ST            0x34
#define MB8X_CCNT_CMLB                  0x38
#define MB8X_CCNT_CUSB                  0x40
#define MB8X_CCNT_CDEBUG0               0xe8
#define MB8X_CCNT_CDEBUG1               0xec
#define MB8X_CCNT_CMSR0                 0xf0
#define MB8X_CCNT_CMSR1                 0xf4

/* External interrupt controller */
#define MB8X_EIRC_BASE                  0xfffe4000
#define MB8X_EIRC_SIZE                  0x100
#define MB8X_EIRC_EIENB                 0x00
#define MB8X_EIRC_EIREQ                 0x04
#define MB8X_EIRC_EILVL                 0x08
#define MB8X_EIRC_EILVL_LOW_LEVEL       0x00
#define MB8X_EIRC_EILVL_HI_LEVEL        0x01
#define MB8X_EIRC_EILVL_RISING_EDGE     0x02
#define MB8X_EIRC_EILVL_FALLING_EDGE    0x03


/* SD memory controller */


/* SRAM - On-Chip Static RAM */         
#define MB8X_SRAM_BASE				    0x01000000
#define MB8X_SRAM_LENGTH                0x00010000 	/*  64K */


/* Bit definitions for interrupt enable register  */
#define MB8X_IER_RHR                    0x01
#define MB8X_IER_THR                    0x02
#define MB8X_IER_LS                     0x04
#define MB8X_IER_MS                     0x08
#define MB8X_IER_SLEEP                  0x10
#define MB8X_IER_XOFF                   0x20
#define MB8X_IER_RTS                    0x40
#define MB8X_IER_CTS                    0x80


