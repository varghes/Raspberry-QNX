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


#ifndef	__ARM_TUAREG_H_INCLUDED
#define	__ARM_TUAREG_H_INCLUDED




/*
 * -------------------------------------------------------------------------
 * SDRAM registers
 * -------------------------------------------------------------------------
 */

#define	TUAREG_SDRAM_BASE		0xf0000000


/* 
 * Register offset from TUAREG_SDRAM_BASE
 */

#define TUAREG_SDRAM_MEMCONFIG	0x30

/*
 *  Memconfig register bits
 */

#define TUAREG_MEMCONFIG_BANK1	(1 << 8)
#define TUAREG_MEMCONFIG_BANK2  (1 << 9)
#define TUAREG_MEMCONFIG_BANK3  (1 << 10)
#define TUAREG_MEMCONFIG_BANK4  (1 << 11)


/*
 * -------------------------------------------------------------------------
 * Extended Function Timer
 * -------------------------------------------------------------------------
 */

#define	TUAREG_TIMER0_BASE		0xe0007000	/* Timer 0 BASE */
#define	TUAREG_TIMER1_BASE		0xe0008000	/* Timer 1 BASE */

#define TUAREG_TIMER_SIZE		0x00000020

/*
 * Register offsets from TUAREG_TIMER_BASE
 */
#define	TUAREG_TIMER_ICAR		0x00		/* Input Capture A			 */
#define	TUAREG_TIMER_ICBR		0x04		/* Input Capture B			 */
#define	TUAREG_TIMER_OCAR		0x08		/* Output Compare A			 */
#define	TUAREG_TIMER_OCBR		0x0C		/* Output Compare B			 */
#define	TUAREG_TIMER_CNTR		0x10		/* Counter Value			 */
#define	TUAREG_TIMER_CR1		0x14		/* Control Register 1		 */
#define TUAREG_TIMER_CR2		0x18		/* Control Register 2		 */
#define TUAREG_TIMER_STATUS		0x1c		/* Status Register			 */


/*
 *  Control Register 1 bits
 */

#define TUAREG_CR1_EN			(1<<15)

/*
 *  Control Register 2 bits
 */

#define TUAREG_CR2_OCAIE		(1<<14)


/*
 *  Status Register bits
 */

#define TUAREG_STATUS_OCFA		(1<<14)

/*
 * -------------------------------------------------------------------------
 * EMI Registers
 * -------------------------------------------------------------------------
 */
#define	TUAREG_EMI_BASE			0xf0000400
#define	TUAREG_EMI_SIZE			0x14



/* 
 * EMI Registers, offset from base address 
 */                
#define	TUAREG_EMI_BCON1		0x04		/* BCON1 config				*/
#define	TUAREG_EMI_BBASE1		0x10		/* Base address for BCON1	*/


/*
 * -------------------------------------------------------------------------
 * RTC Registers
 * -------------------------------------------------------------------------
 */
#define	TUAREG_RTC_BASE			0xf0002800
#define	TUAREG_RTC_SIZE			0x22

/* 
 * RTC Registers, offset from base address 
 */
#define	TUAREG_RTC_RTCCR		0x00		/* RTC Control				*/
#define	TUAREG_RTC_RTCCNT_L		0x04		/* RTC Counter				*/
#define TUAREG_RTC_RTCCNT_M		0x08		/* RTC Counter				*/
#define	TUAREG_RTC_RTCCNT_H		0x0C		/* RTC Counter				*/
#define	TUAREG_RTC_RTCA_L		0x10		/* RTC Alarm				*/
#define TUAREG_RTC_RTCA_M		0x14		/* RTC Alarm				*/
#define TUAREG_RTC_RTCA_H		0x18		/* RTC Alarm				*/
#define	TUAREG_RTC_RTCP_L		0x1C		/* RTC Periodic value		*/
#define TUAREG_RTC_RTCP_H		0x20		/* RTC Periodic value		*/

/*
 * 	RTC RTCCR bits
 */

#define TUAREG_RTCCR_CNF		(1<<4)
#define TUAREG_RTCCR_RTOFF		(1<<5)

/*
 * 	RTC clock frequency
 */

#define TUAREG_RTC_CLOCK				32768



/*
 * -------------------------------------------------------------------------
 * Serial ports
 * -------------------------------------------------------------------------
 */

/* UART0 base address */
#define	TUAREG_UART0_BASE		0xe0005000

/* UART1 base address */
#define TUAREG_UART1_BASE		0xe0006000

#define	TUAREG_UART_SIZE		0x26

/* UART registers, offset from base address */


#define	TUAREG_UART_BAUDRATE	0x00		/* Baud rate		 */
#define	TUAREG_UART_TX			0x04		/* TX				 */
#define	TUAREG_UART_RX			0x08		/* RX				 */
#define	TUAREG_UART_CNTL		0x0c		/* Control			 */
#define	TUAREG_UART_INTEN		0x10		/* Interrupt Enable  */
#define	TUAREG_UART_STATUS		0x14		/* Status			 */
#define	TUAREG_UART_TIMEOUT		0x1C		/* Timeout			 */
#define	TUAREG_UART_TXRESET		0x20		/* TX Reset			 */
#define	TUAREG_UART_RXRESET		0x24		/* RX Reset			 */


/* 
 * Control Register bits
 */
#define	TUAREG_CNTL_FIFO_EN		(1<<10)		/* Enable FIFO mode			 */
#define	TUAREG_CNTL_RX_EN		(1<<8)		/* Enable RX				 */
#define	TUAREG_CNTL_RUN			(1<<7)		/* Enable BAUD rate generator*/
#define	TUAREG_CNTL_PAR_ODD		(1<<5)		/* ODD parity				 */
#define	TUAREG_CNTL_STOP05		(0<<3)		/* 0.5 stop bits			 */
#define	TUAREG_CNTL_STOP10		(1<<3)		/* 1   stop bits			 */
#define	TUAREG_CNTL_STOP15		(2<<3)		/* 1.5 stop bits			 */
#define	TUAREG_CNTL_STOP20		(3<<3)		/* 2   stop bits			 */

#define	TUAREG_CNTL_MODE8BITS	1			/* 8 bits					 */
#define	TUAREG_CNTL_MODE7BITS	3			/* 7 bits + parity			 */
#define	TUAREG_CNTL_MODE9BITS	4			/* 9 bits					 */
#define	TUAREG_CNTL_MODE8BITS_W	5			/* 8 bits + wake up bit		 */



/* 
 * Interrupt enable Register  bits
 */
#define	TUAREG_INTEN_RXHALF			(1<<8)		/* RX half full			 */
#define	TUAREG_INTEN_TIMEIDLE		(1<<7)		/* Timeout idle			 */
#define	TUAREG_INTEN_TIMENOTEMPTY	(1<<6)		/* Timeout not empty	 */
#define	TUAREG_INTEN_OVERRUN		(1<<5)		/* Overrun error		 */
#define	TUAREG_INTEN_FRAME			(1<<4)		/* Framing error		 */
#define	TUAREG_INTEN_PARITY			(1<<3)		/* Parity error			 */
#define	TUAREG_INTEN_TXHALF			(1<<2)		/* TX half full			 */
#define	TUAREG_INTEN_TXEMPTY		(1<<1)		/* TX empty				 */
#define	TUAREG_INTEN_RXNOTEMPTY		1			/* RX not empty			 */


/*
 * Status Register bits
 */


#define	TUAREG_STATUS_TXFULL		(1<<9)		/* TX full				 */
#define	TUAREG_STATUS_RXHALF		(1<<8)		/* RX half full			 */
#define	TUAREG_STATUS_TIMEIDLE		(1<<7)		/* Timeout idle			 */
#define	TUAREG_STATUS_TIMENOTEMPTY	(1<<6)		/* Timeout not empty	 */
#define	TUAREG_STATUS_OVERRUN		(1<<5)		/* Overrun error		 */
#define	TUAREG_STATUS_FRAME			(1<<4)		/* Framing error		 */
#define	TUAREG_STATUS_PARITY		(1<<3)		/* Parity error			 */
#define	TUAREG_STATUS_TXHALF		(1<<2)		/* TX half full			 */
#define	TUAREG_STATUS_TXEMPTY		(1<<1)		/* TX empty				 */
#define	TUAREG_STATUS_RXNOTEMPTY	1			/* RX not empty			 */


/*
 * Uart Interrupts
 */

#define TUAREG_EIC_UART0			(1<<5)
#define TUAREG_EIC_UART1 			(1<<6)


/*
 * -------------------------------------------------------------------------
 * Enhanced Interrupt Controller
 * -------------------------------------------------------------------------
 */


/* EIC base address */
#define	TUAREG_EIC_BASE		0xf0003c00


#define	TUAREG_EIC_SIZE		0x224

/* EIC registers, offset from base address */

#define TUAREG_EIC_ICR		0x00	/* Interrupt Control register			*/
#define TUAREG_EIC_CICR		0x04	/* Current Interrupt Channel register	*/
#define TUAREG_EIC_CIPR		0x08	/* Current Interrupt Priority register	*/
#define TUAREG_EIC_IVR		0x18	/* Interrupt Vector register			*/
#define TUAREG_EIC_FIR		0x1c	/* Fast Interrupt register				*/
#define TUAREG_EIC_IER0		0x20	/* Interrupt Enable 0 register			*/
#define TUAREG_EIC_IPR0		0x40	/* Interrupt Pending 0 register			*/
#define TUAREG_EIC_SIR0		0x60	/* Source Interrupt 0 register          */
#define TUAREG_EIC_SIR1		0x64	/* Source Interrupt 1 register          */
#define TUAREG_EIC_SIR2		0x68	/* Source Interrupt 2 register          */
#define TUAREG_EIC_SIR3		0x6c	/* Source Interrupt 3 register          */
#define TUAREG_EIC_SIR4		0x70	/* Source Interrupt 4 register          */
#define TUAREG_EIC_SIR5		0x74	/* Source Interrupt 5 register          */
#define TUAREG_EIC_SIR6		0x78	/* Source Interrupt 6 register          */
#define TUAREG_EIC_SIR7		0x7c	/* Source Interrupt 7 register          */
#define TUAREG_EIC_SIR8		0x80	/* Source Interrupt 8 register          */
#define TUAREG_EIC_SIR9		0x84	/* Source Interrupt 9 register          */
#define TUAREG_EIC_SIR10	0x88	/* Source Interrupt 10 register         */
#define TUAREG_EIC_SIR11	0x8c	/* Source Interrupt 11 register         */
#define TUAREG_EIC_SIR12	0x90	/* Source Interrupt 12 register         */
#define TUAREG_EIC_SIR13	0x94	/* Source Interrupt 13 register         */
#define TUAREG_EIC_SIR14	0x98	/* Source Interrupt 14 register         */
#define TUAREG_EIC_SIR15	0x9c	/* Source Interrupt 15 register         */
#define TUAREG_EIC_SIR16	0xa0	/* Source Interrupt 0 register          */
#define TUAREG_EIC_SIR17	0xa4	/* Source Interrupt 1 register          */
#define TUAREG_EIC_SIR18	0xa8	/* Source Interrupt 2 register          */
#define TUAREG_EIC_SIR19	0xac	/* Source Interrupt 3 register          */
#define TUAREG_EIC_SIR20	0xb0	/* Source Interrupt 4 register          */
#define TUAREG_EIC_SIR21	0xb4	/* Source Interrupt 5 register          */
#define TUAREG_EIC_SIR22	0xb8	/* Source Interrupt 6 register          */
#define TUAREG_EIC_SIR23	0xbc	/* Source Interrupt 7 register          */
#define TUAREG_EIC_SIR24	0xc0	/* Source Interrupt 8 register          */
#define TUAREG_EIC_SIR25	0xc4	/* Source Interrupt 9 register          */
#define TUAREG_EIC_SIR26	0xc8	/* Source Interrupt 10 register         */
#define TUAREG_EIC_SIR27	0xcc	/* Source Interrupt 11 register         */
#define TUAREG_EIC_SIR28	0xd0	/* Source Interrupt 12 register         */
#define TUAREG_EIC_SIR29	0xd4	/* Source Interrupt 13 register         */
#define TUAREG_EIC_SIR30	0xd8	/* Source Interrupt 14 register         */
#define TUAREG_EIC_SIR31	0xdc	/* Source Interrupt 15 register         */


/*
 * TUAREG_EIC_ICR bits
 */

#define TUAREG_ICR_IRQEN	(1<<0)
 

/*
 * Interrupt definitions
 */


#define TUAREG_INTR_CLASS_EXTERNAL				0x0
#define TUAREG_INTR_CLASS_EXTERNAL_4			0x1 	 	      
#define	TUAREG_INTR_CLASS_WAKE					0x2
#define TUAREG_INTR_CLASS_EFT1					0x3
#define TUAREG_INTR_CLASS_EFT2					0x4
#define TUAREG_INTR_CLASS_UART1					0x5
#define TUAREG_INTR_CLASS_UART2					0x6
#define TUAREG_INTR_CLASS_DMA					0x7            
#define TUAREG_INTR_CLASS_DMA0					0x8
#define TUAREG_INTR_CLASS_DMA1					0x9
#define TUAREG_INTR_CLASS_USB_H					0xa
#define TUAREG_INTR_CLASS_USB_L					0xb 
#define TUAREG_INTR_CLASS_CAN					0xc 
#define TUAREG_INTR_CLASS_SPI1					0xd
#define TUAREG_INTR_CLASS_SPI2					0xe
#define TUAREG_INTR_CLASS_IDE1					0xf 
#define TUAREG_INTR_CLASS_RTC					0x10
#define	TUAREG_INTR_CLASS_GPS_SYNC				0x11
#define TUAREG_INTR_CLASS_GPS_SLEEP				0x12
#define TUAREG_INTR_CLASS_GPS_100TICK			0x13
#define TUAREG_INTR_CLASS_GPS_20TICK			0x14
#define TUAREG_INTR_CLASS_GPS_1TICK				0x15  
#define TUAREG_INTR_CLASS_GPS_DATA				0x16
#define TUAREG_INTR_CLASS_AD					0x17 
#define TUAREG_INTR_CLASS_AUDIO_DA				0x18
#define TUAREG_INTR_CLASS_SERIALAUDIO_FRAME		0x19  
#define TUAREG_INTR_CLASS_SERIALAUDIO_OU		0x1a
#define TUAREG_INTR_CLASS_SERIALAUDIO_RDATA		0x1b 
#define TUAREG_INTR_CLASS_SERIALAUDIO_TDATA		0x1c
#define TUAREG_INTR_CLASS_EFT2_COMPAREA			0x1d  
#define TUAREG_INTR_CLASS_EFT2_COMPAREB			0x1e  
#define TUAREG_INTR_CLASS_WATCHDOG				0x1f

/*
 * -------------------------------------------------------------------------
 * Wakeup / Interrupt Unit (WIU)
 * -------------------------------------------------------------------------
 */

#define TUAREG_PCG1_PUR1_WAKEWIU	0x400
#define TUAREG_WIU_BASE				0xf0002400
#define TUAREG_WIU_SIZE				0x10

/* WIU Registers offset from Base address */
#define TUAREG_WIU_WUCTRL			0x00
#define TUAREG_WIU_WUMR				0x04
#define TUAREG_WIU_WUTR				0x08
#define TUAREG_WIU_WUPR				0x0c

/* Register Bit Definitions */
#define TUAREG_WIU_WUCTRL_WKUPINT	(1<<0)
#define TUAREG_WIU_WUCTRL_INTEN		(1<<1)

#define TUAREG_ETH_TRIG				(1<<2)


/*
 * -------------------------------------------------------------------------
 * General Purpose IO
 * -------------------------------------------------------------------------
 */


/* GPIO base address */
#define	TUAREG_GPIO_BASE	0xe0002000		


#define	TUAREG_GPIO_SIZE		0x10


#define TUAREG_GPIO3_BASE	0xf0001400
#define TUAREG_GPIO4_BASE	0xf0001800

/* GPIO registers, offset from base address */

#define	TUAREG_GPIO_PC0		0x00
#define	TUAREG_GPIO_PC1		0x04
#define	TUAREG_GPIO_PC2		0x08
#define	TUAREG_GPIO_PD		0x0c


/*
 *  PC0 bits
 */

#define TUAREG_PC0_C00		1
#define TUAREG_PC0_C01		(1<<1)
#define TUAREG_PC0_C02		(1<<2)
#define TUAREG_PC0_C03		(1<<3)


/*
 *  PC1 bits
 */

#define TUAREG_PC1_C00		1
#define TUAREG_PC1_C01		(1<<1)
#define TUAREG_PC1_C02		(1<<2)
#define TUAREG_PC1_C03		(1<<3)

/*
 *  PC2 bits
 */

#define TUAREG_PC2_C00		1
#define TUAREG_PC2_C01		(1<<1)
#define TUAREG_PC2_C02		(1<<2)
#define TUAREG_PC2_C03		(1<<3)


/*
 * -------------------------------------------------------------------------
 * Miscellanea Registers
 * -------------------------------------------------------------------------
 */



/* CGC base address */
#define	TUAREG_CGC_BASE		0xf0002c00


#define	TUAREG_CGC_SIZE		0x8

/* CGC registers, offset from base address */

#define TUAREG_CGC_PCG1		0x00
#define TUAREG_CGC_PUR1		0x04
#define TUAREG_CGC_PCG2		0x0c
#define TUAREG_CGC_PUR2		0x10



/*
 *  wakeup bits
 */


#define	TUAREG_PCG1_PUR1_WAKEUART0	0x21
#define	TUAREG_PCG1_PUR1_WAKEUART1	0x41
#define TUAREG_PCG1_PUR1_WAKEATA	0x08
#define TUAREG_PCG2_PUR2_WAKEATA	0x02



/*
 *  RTC wakeup bits
 */

#define TUAREG_PCG1_PUR1_WAKERTC	0x800
#define TUAREG_PCG1_PUR1_WAKERTC	0x800


/*
 * -------------------------------------------------------------------------
 * RCCU Registers
 * -------------------------------------------------------------------------
 */

/* RCCU base address */
#define	TUAREG_RCCU_BASE		0xf0001000


#define	TUAREG_RCCU_SIZE		0x18

/* RCCU registers, offset from base address */

#define TUAREG_RCCU_PLLCONF			0x0
#define TUAREG_RCCU_DIVCONF			0x4
#define TUAREG_RCCU_CLKFLAG 		0x8
#define TUAREG_RCCU_CLKCTL			0xC
#define TUAREG_RCCU_MSKCTL			0x10
#define TUAREG_RCCU_SYSPROT			0x14


/*
 * PLLCONF bits
 */


#define TUAREG_PLLCONF_PLLOFF		(1<<9)

#define TUAREG_PLLCONF_DIVMASK		0xc0
#define TUAREG_PLLCONF_DIV1			0x0
#define TUAREG_PLLCONF_DIV2			0x40
#define TUAREG_PLLCONF_DIV4			0x80
#define TUAREG_PLLCONF_DIV8			0xc0

#define TUAREG_PLLCONF_MULTMASK		0x3f

/*
 * DIVCONF bits
 */

#define TUAREG_DIVCONF_APBMASK		0x3
#define TUAREG_DIVCONF_APBDIV2		0x0
#define TUAREG_DIVCONF_APBDIV4		0x1
#define TUAREG_DIVCONF_APBDIV8		0x2
#define TUAREG_DIVCONF_APBDIV16		0x3


/*
 * SYSPROT bits
 */

#define TUAREG_SYSPROT_UNLOCK		(1<<0)


/*
 * CLKCTL
 */

#define TUAREG_CLKCTL_SWRST			(1<<3)



/*
 * Input clock
 */

#define TUAREG_INPUT_CLOCK			49107000  /* HZ	*/


/*
 * -------------------------------------------------------------------------
 * ATAPI configuration space registers
 * -------------------------------------------------------------------------
 */

#define TUAREG_CONFIG_ATA_BASE		0xc0000100

#define TUAREG_CONFIG_ATA_SIZE		0x100

/* configuration register offsets	*/


#define	TUAREG_CONFIG_ATA_XCMD		0x4
#define	TUAREG_CONFIG_ATA_PIDETIM	0x40
#define	TUAREG_CONFIG_ATA_SLIDETIM	0x44

/*
 *	 XCMD bits
 */

#define TUAREG_XCMD_IO						0x1


/*
 *  PIEDTIM bits
 */

#define	TUAREG_PIDETIM_IDEDECODE	1<<15
#define	TUAREG_PIDETIM_SLAVETIMING	1<<14



#endif	/* __ARM_TUAREG_H_INCLUDED */

/* __SRCVERSION("tuareg.h $Rev: 657836 $"); */
