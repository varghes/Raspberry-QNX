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




/*
 * SA-1111 Companion Chip
 *
 * NOTE: The SA-1111 address is determined by board-specific chip selects.
 * The register addresses here are _offsets_ from that board-specific address.
 *

 *
 */

#ifndef	__ARM_SA1111_H_INCLUDED
#define	__ARM_SA1111_H_INCLUDED

/*
 * -------------------------------------------------------------------------
 * System Bus Interface
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_SKCR		0x00000000		/* control register */
#define	SA1111_SMCR		0x00000004		/* shared memory controller register */
#define	SA1111_SKID		0x00000008		/* ID register */

/*
 * SA1111_SKCR bits
 */
#define	SA1111_SKCR_PLL_Bypass	0x0001	/* Enable on-chip PLL */
#define	SA1111_SKCR_RCLKEn		0x0002	/* Enable internal system bus clocks */
#define	SA1111_SKCR_Sleep		0x0004	/* Enter sleep mode */
#define	SA1111_SKCR_Doze		0x0008	/* Enter doze mode */
#define	SA1111_SKCR_VCO_Off		0x0010	/* Disable system PLL */
#define	SA1111_SKCR_RdyEn		0x0080	/* Enable RDY response */
#define	SA1111_SKCR_SeLAC		0x0100	/* Select AC Link/I2S audio */
#define	SA1111_SKCR_OEn			0x2000	/* Enable nOE for DMA accesses */

/*
 * SA1111_SMCR bits
 */
#define	SA1111_SMCR_DTIM		0x0001	/* Type of DRAM (1 == SDRAM) */
#define	SA1111_SMCR_MBGE		0x0002	/* Shared memory grant enable */
#define	SA1111_SMCR_DRAC_MASK	0x001c
#define	SA1111_SMCR_DRAC_9		0x0000	/* 9 DRAM row address bits */
#define	SA1111_SMCR_DRAC_10		0x0004	/* 10 DRAM row address bits */
#define	SA1111_SMCR_DRAC_11		0x0008	/* 11 DRAM row address bits */
#define	SA1111_SMCR_DRAC_12		0x000c	/* 12 DRAM row address bits */
#define	SA1111_SMCR_DRAC_13		0x0010	/* 13 DRAM row address bits */
#define	SA1111_SMCR_DRAC_14		0x0014	/* 14 DRAM row address bits */
#define	SA1111_SMCR_DRAC_15		0x0018	/* 15 DRAM row address bits */
#define	SA1111_SMCR_CLAT		0x0020	/* SDRAM CAS latency */

/*
 * SA1111_SKID bits
 */
#define	SA1111_SKID_REV_MASK	0x000001ff	/* mask revision number */
#define	SA1111_SKID_ID			0x690cc200	/* chip id */

/*
 * -------------------------------------------------------------------------
 * System Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_SKPCR	0x00000200		/* Power control register */
#define	SA1111_SKCDR	0x00000204		/* Clock divider register */
#define	SA1111_SKAUD	0x00000208		/* Audio clock divider register */
#define	SA1111_SKPMC	0x0000020c		/* PS/2 mouse clock divider register */
#define	SA1111_SKPTC	0x00000210		/* PS/2 trackpad clock divider register */
#define	SA1111_SKPEN0	0x00000214		/* PWM0 enable register */
#define	SA1111_SKPWM0	0x00000218		/* PWM0 clock register */
#define	SA1111_SKPEN1	0x0000021c		/* PWM1 enable register */
#define	SA1111_SKPWM1	0x00000220		/* PWM1 clock register */

/*
 * SA1111_SKPCR bits
 */
#define	SA1111_SKPCR_UCLKEn		0x0001	/* Enable USB host controller clock */
#define	SA1111_SKPCR_ACCLKEn	0x0002	/* Enable audio AC Link clock */
#define	SA1111_SKPCR_I2SCLKEn	0x0004	/* Enable audio I2S clock */
#define	SA1111_SKPCR_L3CLKEn	0x0008	/* Enable audio L3 clock */
#define	SA1111_SKPCR_SCLKEn		0x0010	/* Enable SSP clock */
#define	SA1111_SKPCR_PMCLKEn	0x0020	/* Enable PS/2 mouse clock */
#define	SA1111_SKPCR_PTCLKEn	0x0040	/* Enable PS/2 trackpad clock */
#define	SA1111_SKPCR_DCLKEn		0x0080	/* SMC/DMA bus clock */
#define	SA1111_SKPCR_PWMCLKEn	0x0100	/* PWM clock enable */

/*
 * -------------------------------------------------------------------------
 * USB Host Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */

/*
 * -------------------------------------------------------------------------
 * Serial Audio Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define SA1111_SAC          0x0000600

#define SA1111_SACR0        0x00     /* Serial Audio Common Control Register  */
#define SA1111_SACR1        0x04     /* Serial Audio Alternate Mode (I2C/MSB-Justified) Control Register */
#define SA1111_SACR2        0x08     /* Serial Audio AC-Link Control Register */
#define SA1111_SASR0        0x0C     /* Serial Audio I2C/MSB-Justified Interface and FIFO status Register */
#define SA1111_SASR1        0x10     /* Serial Audio AC-Link Interface and FIFO status Register */
#define SA1111_SASCR        0x18     /* Serial Audio Status Clear Register    */
#define SA1111_L3CAR        0x1C     /* L3 Control Bus Address Register       */
#define SA1111_L3CDR        0x20     /* L3 Control Bus Data Register          */
#define SA1111_ACCAR        0x24     /* AC-Link Command Address Register      */
#define SA1111_ACCDR        0x28     /* AC-Link Command Data Register         */
#define SA1111_ACSAR        0x2C     /* AC-Link Status Address Register       */
#define SA1111_ACSDR        0x30     /* AC-Link Status Data Register          */
#define SA1111_SADTCS       0x34     /* Serial Audio DMA Transmit Control/Status Register */
#define SA1111_SADTSA       0x38     /* Serial Audio DMA Transmit Buffer Start Address Register A */
#define SA1111_SADTCA       0x3C     /* Serial Audio DMA Transmit Buffer Count Register A */
#define SA1111_SADTSB       0x40     /* Serial Audio DMA Transmit Buffer Start Address Register B */
#define SA1111_SADTCB       0x44     /* Serial Audio DMA Transmit Buffer Count Register B */
#define SA1111_SADRCS       0x48     /* Serial Audio DMA Receive Control/Status Register */
#define SA1111_SADRSA       0x4C     /* Serial Audio DMA Receive Buffer Start Address Register A */
#define SA1111_SADRCA       0x50     /* Serial Audio DMA Transmit Buffer Count Register A */
#define SA1111_SADRSB       0x54     /* Serial Audio DMA Receive Buffer Start Address Register B */
#define SA1111_SADRCB       0x58     /* Serial Audio DMA Transmit Buffer Count Register B */
#define SA1111_SAITR        0x5C     /* Serial Audio Interrupt Test Register  */
#define SA1111_SADR         0x80     /* Serial Audio Data Register (a block of 32-bit 16 locations - up to 0x00006BC) */
#define SA1111_SAC_SIZE     (SA1111_SADR - SA1111_SACR0 + 4)


/*
 * SA1111_SA1111_SACR0 bits
 */
#define	SA1111_SACR0_ENB	0x0001		/* SAC Enable                                  */
#define	SA1111_SACR0_BCKD	0x0004		/* BIT_CLC Pin direction (0- output, 1 - input */
#define	SA1111_SACR0_RST        0x0008          /* Reset SAC registers (except this one)       */
#define	SA1111_SACR0_TFTH       0x00f0          /* Transmit FIFO interrupt or DMA threshold    */
#define	SA1111_SACR0_RFTH       0x0f00          /* Receive  FIFO interrupt or DMA threshold    */


/*
 * SA1111_SA1111_SACR2 bits
 */
#define	SA1111_SACR2_TS3V	0x0001		/* Transmit slot 3 vaid                        */
#define	SA1111_SACR2_TS4V       0x0002		/* Transmit slot 4 vaid                        */
#define	SA1111_SACR2_WKUP       0x0004		/* Wake Up AC'97 codec                         */
#define	SA1111_SACR2_DREC       0x0008		/* Disable Recording function of AC-Link interface */
#define	SA1111_SACR2_DRPL       0x0010		/* Disable Replying function of AC-Link interface */
#define	SA1111_SACR2_ENLBF      0x0020		/* Enable AC-Link interface Loopback function (for test only) */
#define	SA1111_SACR2_RESETOFF   0x0040		/* Specify reset sygnal to SYS_CLK output      */

/*
 * SA1111_SA1111_SASR1 bits
 */
#define	SA1111_SASR1_TNF	0x0001		/* Transmit FIFO not full                      */
#define	SA1111_SASR1_RNE	0x0002		/* Receive FIFO not full                       */
#define	SA1111_SASR1_BSY	0x0004		/* SAC Busy                                    */
#define	SA1111_SASR1_TFS	0x0008		/* Transmit FIFO Service Request               */
#define	SA1111_SASR1_RFS	0x0010		/* Receive  FIFO Service Request               */
#define	SA1111_SASR1_TUR	0x0020		/* Transmit FIFO under-run                     */
#define	SA1111_SASR1_ROR	0x0040		/* Receive  FIFO overrun                       */
#define	SA1111_SASR1_TFL	0x0f00		/* Transmit FIFO level                         */
#define	SA1111_SASR1_RFL	0xf000		/* Receive  FIFO level                         */
#define	SA1111_SASR1_CADT	0x10000		/* AC-Link Command Address and Data are Received is done */
#define	SA1111_SASR1_SADR	0x20000		/* AC-Link Command Address and Data are Received is done */
#define	SA1111_SASR1_RSTO       0x40000		/* Read status timeout                                   */
#define	SA1111_SASR1_CLPM       0x80000		/* Command for AC'97 Codec Low Power Mode is detected    */
#define	SA1111_SASR1_CRDY       0x100000	/* AC'97 Codec is ready for normal operation   */
#define	SA1111_SASR1_RS3V       0x200000	/* Received slot 3 is valid                    */
#define	SA1111_SASR1_RS4V       0x200000	/* Received slot 4 is valid                    */

/*
 * SA1111_SA1111_SASCR bits
 */
#define	SA1111_SASCR_TUR	0x0020		/* Clears transmit FIFO under-run status bit   */
#define	SA1111_SASCR_ROR	0x0040		/* Clears receive  FIFO overrun status bit     */
#define	SA1111_SASCR_DTS	0x10000		/* Clears L3C/AC-Link Data Sent  status bit    */
#define	SA1111_SASCR_RDD	0x20000		/* Clears L3C/AC-Link Data Read done  status bit   */
#define	SA1111_SASCR_STO	0x40000		/* Clears AC-Link Read Status Time Out  status bit */



/*
 * -------------------------------------------------------------------------
 * SSP Serial Port
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */

/*
 * -------------------------------------------------------------------------
 * PS/2 Trackpad and Mouse Interfaces
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_PS2_TRACKPAD		0x00000a00
#define	SA1111_PS2_MOUSE		0x00000c00
#define	SA1111_PS2_SIZE			0x00000014

/*
 * Register offsets from trackpad/mouse register base
 */
#define	SA1111_KBDCR		0x00	/* Control register */
#define	SA1111_KBDSTAT		0x04	/* Status register */
#define	SA1111_KBDDATA		0x08	/* Data register */
#define	SA1111_KBDCLKDIV	0x0c	/* Clock divider register */
#define	SA1111_KBDPRECNT	0x10	/* Clock precount register */

/*
 * SA1111_KBDCR bits
 */
#define	SA1111_KBDCR_FKC	0x0001		/* Force CLK line low */
#define	SA1111_KBDCR_FKD	0x0002		/* Force DATA line low */
#define	SA1111_KBDCR_ENA	0x0008		/* Enable interface */

/*
 * SA1111_KBDSTAT bits
 */
#define	SA1111_KBDSTAT_KBC	0x0001		/* CLK pin value */
#define	SA1111_KBDSTAT_KBD	0x0002		/* DATA pin value */
#define	SA1111_KBDSTAT_RXP	0x0004		/* Parity for last received byte */
#define	SA1111_KBDSTAT_ENA	0x0008		/* Interface enabled */
#define	SA1111_KBDSTAT_RXB	0x0010		/* Rx busy */
#define	SA1111_KBDSTAT_RXF	0x0020		/* Rx register full */
#define	SA1111_KBDSTAT_TXB	0x0040		/* Tx busy */
#define	SA1111_KBDSTAT_TXE	0x0080		/* Tx register empty */
#define	SA1111_KBDSTAT_STP	0x0100		/* Stop bit error */

/*
 * SA1111_KBDCLKDIV values
 */
#define	SA1111_KBDCLKDIV_8		0		/* Divide by 8 (KbdClk is 8MHz) */
#define	SA1111_KBDCLKDIV_4		1		/* Divide by 4 (KbdClk is 4MHz) */
#define	SA1111_KBDCLKDIV_2		2		/* Divide by 2 (KbdClk is 2MHz) */

/*
 * -------------------------------------------------------------------------
 * GPIO Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_GPIO			0x00001000
#define	SA1111_GPIO_SIZE	0x00000030

/*
 * Register offsets from SA1111_GPIO
 */
#define	SA1111_PA_DDR	0x00		/* GPIO block A data direction */
#define	SA1111_PA_DRR	0x04		/* GPIO block A data read */
#define	SA1111_PA_DWR	0x04		/* GPIO block A data write */
#define	SA1111_PA_SDR	0x08		/* GPIO block A sleep direction */
#define	SA1111_PA_SSR	0x0c		/* GPIO Block A sleep state */
#define	SA1111_PB_DDR	0x10		/* GPIO block B data direction */
#define	SA1111_PB_DRR	0x14		/* GPIO block B data read */
#define	SA1111_PB_DWR	0x14		/* GPIO block B data write */
#define	SA1111_PB_SDR	0x18		/* GPIO block B sleep direction */
#define	SA1111_PB_SSR	0x1c		/* GPIO Block B sleep state */
#define	SA1111_PC_DDR	0x20		/* GPIO block C data direction */
#define	SA1111_PC_DRR	0x24		/* GPIO block C data read */
#define	SA1111_PC_DWR	0x24		/* GPIO block C data write */
#define	SA1111_PC_SDR	0x28		/* GPIO block C sleep direction */
#define	SA1111_PC_SSR	0x2c		/* GPIO Block C sleep state */

/*
 * -------------------------------------------------------------------------
 * Interrupt Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_INTR		0x00001600

/*
 * Register offsets from SA1111_INTR
 */
#define	SA1111_INTTEST0		0x00
#define	SA1111_INTTEST1		0x04
#define	SA1111_INTEN0		0x08
#define	SA1111_INTEN1		0x0c
#define	SA1111_INTPOL0		0x10
#define	SA1111_INTPOL1		0x14
#define	SA1111_INTTSTSEL	0x18
#define	SA1111_INTSTATCLR0	0x1c
#define	SA1111_INTSTATCLR1	0x20
#define	SA1111_INTSET0		0x24
#define	SA1111_INTSET1		0x28
#define	SA1111_WAKE_EN0		0x2c
#define	SA1111_WAKE_EN1		0x30
#define	SA1111_WAKE_POL0	0x34
#define	SA1111_WAKE_POL1	0x38
#define SA1111_INTR_SIZE        (SA1111_WAKE_POL1 - SA1111_INTEN0 + 4)

/*
 * SA1111 Interrupt Numbers
 */

#define SA1111_INTR_SAC_ACLINK_SENT         40
#define SA1111_INTR_SAC_ACLINK_READ_DONE    41
#define SA1111_INTR_SAC_ACLINK_READ_TIMEOUT 42


/*
 * -------------------------------------------------------------------------
 * PCMCIA Controller
 *
 * NB: values are _offsets_ from SA-1111 base address
 * -------------------------------------------------------------------------
 */
#define	SA1111_PCMCIA		0x00001800
#define	SA1111_PCMCIA_SIZE	0x0000000c

/*
 * Register offsets from SA1111_PCMCIA
 */
#define	SA1111_PCCR			0x00
#define	SA1111_PCSSR		0x04
#define	SA1111_PCSR			0x08

/*
 * SA1111_PCCR bits
 */
#define	SA1111_PCCR_S0_RST	0x0001			/* Socket 0 reset */
#define	SA1111_PCCR_S1_RST	0x0002			/* Socket 1 reset */
#define	SA1111_PCCR_S0_FLT	0x0004			/* Socket 0 float control lines */
#define	SA1111_PCCR_S1_FLT	0x0008			/* Socket 1 float control lines */
#define	SA1111_PCCR_S0_PWE	0x0010			/* Socket 0 wait enable */
#define	SA1111_PCCR_S1_PWE	0x0020			/* Socket 1 wait enable */
#define	SA1111_PCCR_S0_PSE	0x0040			/* Socket 0 5V select */
#define	SA1111_PCCR_S1_PSE	0x0080			/* Socket 1 5V select */

/*
 * SA1111_PCSR bits
 */
#define	SA1111_PCSR_S0_RDY	0x0001			/* Socket 0 RDY_nIRQ */
#define	SA1111_PCSR_S1_RDY	0x0002			/* Socket 1 RDY_nIRQ */
#define	SA1111_PCSR_S0_CD	0x0004			/* Socket 0 CD1/CD2 */
#define	SA1111_PCSR_S1_CD	0x0008			/* Socket 1 CD1/CD2 */
#define	SA1111_PCSR_S0_VS1	0x0010			/* Socket 0 VS1 */
#define	SA1111_PCSR_S0_VS2	0x0020			/* Socket 0 VS2 */
#define	SA1111_PCSR_S1_VS1	0x0040			/* Socket 1 VS1 */
#define	SA1111_PCSR_S1_VS2	0x0080			/* Socket 1 VS2 */
#define	SA1111_PCSR_S0_WP	0x0100			/* Socket 0 WP */
#define	SA1111_PCSR_S1_WP	0x0200			/* Socket 1 WP */
#define	SA1111_PCSR_S0_BVD1	0x0400			/* Socket 0 BVD1 */
#define	SA1111_PCSR_S0_BVD2	0x0800			/* Socket 0 BVD2 */
#define	SA1111_PCSR_S1_BVD1	0x1000			/* Socket 1 BVD1 */
#define	SA1111_PCSR_S1_BVD2	0x2000			/* Socket 1 BVD2 */

#endif	/* __ARM_SA1111_H_INCLUDED */

/* __SRCVERSION("sa1111.h $Rev: 657836 $"); */
