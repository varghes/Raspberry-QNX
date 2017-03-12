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
 * AT91RM9200 processor
 */

#ifndef	__ARM_AT91RM9200_H_INCLUDED
#define	__ARM_AT91RM9200_H_INCLUDED

/*---------------------------------------
          Internal Memory Mapping
  ---------------------------------------*/ 
#define	AT91RM9200_BOOT_BASE	0x00000000
#define	AT91RM9200_ROM_BASE		0x00100000
#define	AT91RM9200_SRAM_BASE	0x00200000
#define	AT91RM9200_USBH_BASE	0x00300000

/*---------------------------------------
            User Peripheral
  ---------------------------------------*/ 

/*
 * Timer Counter (TC)
 */
#define	AT91RM9200_TC0_BASE		0xFFFA0000
#define	AT91RM9200_TC3_BASE		0xFFFA4000
#define	AT91RM9200_TC_SIZE		0x100

#define	AT91RM9200_TC_CCR		0x00
#define	AT91RM9200_TC_CMR		0x04
#define	AT91RM9200_TC_CV		0x10
#define	AT91RM9200_TC_RA		0x14
#define	AT91RM9200_TC_RB		0x18
#define	AT91RM9200_TC_RC		0x1C
#define	AT91RM9200_TC_SR		0x20
#define	AT91RM9200_TC_IER		0x24
#define	AT91RM9200_TC_IDR		0x28
#define	AT91RM9200_TC_IMR		0x2C

#define	AT91RM9200_TC_BCR		0xC0
#define	AT91RM9200_TC_BMR		0xC4


#define	AT91RM9200_TC_CCR_CLKEN		(1 << 0)
#define	AT91RM9200_TC_CCR_CLKDIS	(1 << 1)
#define	AT91RM9200_TC_CCR_SWTRG		(1 << 2)

#define	AT91RM9200_TC_CMR_CPCTRG	(1 << 14)

/*
 * USB Device Port (UDP)
 */
#define	AT91RM9200_UDP_BASE		0xFFFB0000

/*
 * Multimedia Card Interface (MCI)
 */
#define	AT91RM9200_MCI_BASE		0xFFFB4000

/*
 * Two Wire Interface (TWI)
 */
#define	AT91RM9200_TWI_BASE		0xFFFB8000

/*
 * Ethernet MAC (EMAC)
 */
#define	AT91RM9200_EMAC_BASE	0xFFFBC000
#define	AT91RM9200_EMAC_SIZE	0xC0

/*
 * Universal Synchronous Asynchronous Receiver Transceiver (USART)
 */
#define	AT91RM9200_USART0_BASE	0xFFFC0000
#define	AT91RM9200_USART1_BASE	0xFFFC4000
#define	AT91RM9200_USART2_BASE	0xFFFC8000
#define	AT91RM9200_USART3_BASE	0xFFFCC000

#define	AT91USART_SIZE			0x100

/*
 * --------------------------------------------------------------------------
 * Atmel USAR0 Registers
 * --------------------------------------------------------------------------
 */
#define	AT91USART_CR			0x00	/* Control Register */
#define	AT91USART_MR			0x04	/* Mode Register */
#define	AT91USART_IER			0x08	/* Interrupt Enable Register */
#define	AT91USART_IDR			0x0C	/* Interrupt Disable Register */
#define	AT91USART_IMR			0x10	/* Interrupt Mask Register */
#define	AT91USART_CSR			0x14	/* Channel Status Register */
#define	AT91USART_RHR			0x18	/* Receiver Holding Register */
#define	AT91USART_THR			0x1C	/* Transmitter Holding Register */
#define	AT91USART_BRGR			0x20	/* Baud Rate Generator Register */
#define	AT91USART_RTOR			0x24	/* Receiver Time-out Register */
#define	AT91USART_TTGR			0x28	/* Transmitter Timeguard Register */
#define	AT91USART_FIDI			0x40	/* FIDI Ratio Register */
#define	AT91USART_NER			0x44	/* Number of Errors Register */
#define	AT91USART_IF			0x4C	/* IrDA Filter Register */


/*
 * CR register bits
 */
#define	AT91USART_CR_RSTRX		(1 << 2)	/* Reset Receiver */
#define	AT91USART_CR_RSTTX		(1 << 3)	/* Reset Transmitter */
#define	AT91USART_CR_RXEN		(1 << 4)	/* Receiver Enable */
#define	AT91USART_CR_RXDIS		(1 << 5)	/* Receiver Disable */
#define	AT91USART_CR_TXEN		(1 << 6)	/* Transmitter Enable */
#define	AT91USART_CR_TXDIS		(1 << 7)	/* Transmitter Disable */
#define	AT91USART_CR_RSTSTA		(1 << 8)	/* Reset Status Bits */
#define	AT91USART_CR_STTBRK		(1 << 9)	/* Start Break */
#define	AT91USART_CR_STPBRK		(1 << 10)	/* Stop Break */
#define	AT91USART_CR_STTTO		(1 << 11)	/* Start Time-out */
#define	AT91USART_CR_SENDA		(1 << 12)	/* Send Address */
#define	AT91USART_CR_RSTIT		(1 << 13)	/* Reset Iterations */
#define	AT91USART_CR_RSTNACK	(1 << 14)	/* Reset Non Acknowledge */
#define	AT91USART_CR_RETTO		(1 << 15)	/* Rearm Time-out */
#define	AT91USART_CR_DTREN		(1 << 16)	/* DTE Enable */
#define	AT91USART_CR_DTRDIS		(1 << 17)	/* DTE Disable */
#define	AT91USART_CR_RTSEN		(1 << 18)	/* RTS Enable */
#define	AT91USART_CR_RTSDIS		(1 << 19)	/* RTS Disable */

/*
 * Interrupt Enable/Disable/Mask/Status register
 */
#define	AT91USART_INT_RXRDY		(1 << 0)	/* Receiver Ready */
#define	AT91USART_INT_TXRDY		(1 << 1)	/* Transmitter Ready */
#define	AT91USART_INT_RXBRK		(1 << 2)	/* Break Received/End of Break */
#define	AT91USART_INT_ENDRX		(1 << 3)	/* End of Receive Transfer */
#define	AT91USART_INT_ENDTX		(1 << 4)	/* End of Transmitter Transfer */
#define	AT91USART_INT_OVRE		(1 << 5)	/* Overrun Error */
#define	AT91USART_INT_FRAME		(1 << 6)	/* Framing Error */
#define	AT91USART_INT_PARE		(1 << 7)	/* Parity Error */
#define	AT91USART_INT_TIMEOUT	(1 << 8)	/* Receive Timeout */
#define	AT91USART_INT_TXEMPTY	(1 << 9)	/* Transmitter Empty */
#define	AT91USART_INT_ITERATION	(1 << 10)	/* Max Number of Repetitions reached */
#define	AT91USART_INT_TXBUFE	(1 << 11)	/* Transmission Buffer Empty */
#define	AT91USART_INT_RXBUFF	(1 << 12)	/* Receive Buffer Full */
#define	AT91USART_INT_NACK		(1 << 13)	/* Receive Buffer Full */
#define	AT91USART_INT_RIIC		(1 << 16)	/* Ring Indicator Input Change */
#define	AT91USART_INT_DSRIC		(1 << 17)	/* Data Set Ready Input Change */
#define	AT91USART_INT_DCDIC		(1 << 18)	/* Data Carrier Detect Input Change */
#define	AT91USART_INT_CTSIC		(1 << 19)	/* Clear to Send Input Change */

/*
 * CSR register bits
 */
#define	AT91USART_CSR_RI		(1 << 20)	/* RI Input */
#define	AT91USART_CSR_DSR		(1 << 21)	/* DSR Input */
#define	AT91USART_CSR_DCD		(1 << 22)	/* DCD Input */
#define	AT91USART_CSR_CTS		(1 << 23)	/* CTS Input */


/*
 * Serial Synchronous Controller (SSC)
 */
#define	AT91RM9200_SSC0_BASE	0xFFFD0000
#define	AT91RM9200_SSC1_BASE	0xFFFD4000
#define	AT91RM9200_SSC2_BASE	0xFFFD8000

/*
 * Serial Peripheral Interface (SPI)
 */
#define	AT91RM9200_SPI_BASE		0xFFFE0000


/*---------------------------------------
            System Peripheral
  ---------------------------------------*/ 
/*
 * Advanced Interrupt Controller (AIC)
 */
/* AIC Base address */
#define	AT91RM9200_AIC_BASE		0xFFFFF000

/* AIC Registers, offset from base address */
#define	AT91RM9200_AIC_SMR(x)	(0x04 * (x))
#define	AT91RM9200_AIC_SVR(x)	(0x80 + 0x04 * (x))
#define	AT91RM9200_AIC_IVR		0x100
#define	AT91RM9200_AIC_FVR		0x104
#define	AT91RM9200_AIC_ISR		0x108
#define	AT91RM9200_AIC_IPR		0x10C
#define	AT91RM9200_AIC_IMR		0x110
#define	AT91RM9200_AIC_CISR		0x114
#define	AT91RM9200_AIC_IECR		0x120
#define	AT91RM9200_AIC_IDCR		0x124
#define	AT91RM9200_AIC_ICCR		0x128
#define	AT91RM9200_AIC_ISCR		0x12C
#define	AT91RM9200_AIC_EIOCR	0x130
#define	AT91RM9200_AIC_SPU		0x134
#define	AT91RM9200_AIC_DCR		0x138

/*
 * Debug port
 */
#define	AT91RM9200_DBGU_BASE	0xFFFFF200

/*
 * PIO A,B,C,D
 */
#define	AT91RM9200_PIOA_BASE	0xFFFFF400
#define	AT91RM9200_PIOB_BASE	0xFFFFF600
#define	AT91RM9200_PIOC_BASE	0xFFFFF800
#define	AT91RM9200_PIOD_BASE	0xFFFFFA00

/* PIO Registers, offset from base address */
#define	AT91RM9200_PIO_PER		0x00
#define	AT91RM9200_PIO_PDR		0x04
#define	AT91RM9200_PIO_PSR		0x08
#define	AT91RM9200_PIO_OER		0x10
#define	AT91RM9200_PIO_ODR		0x14
#define	AT91RM9200_PIO_OSR		0x18
#define	AT91RM9200_PIO_IFER		0x20
#define	AT91RM9200_PIO_IFDR		0x24
#define	AT91RM9200_PIO_IFSR		0x28
#define	AT91RM9200_PIO_SODR		0x30
#define	AT91RM9200_PIO_CODR		0x34
#define	AT91RM9200_PIO_ODSR		0x38
#define	AT91RM9200_PIO_PDSR		0x3C
#define	AT91RM9200_PIO_IER		0x40
#define	AT91RM9200_PIO_IDR		0x44
#define	AT91RM9200_PIO_IMR		0x48
#define	AT91RM9200_PIO_ISR		0x4C
#define	AT91RM9200_PIO_MDER		0x50
#define	AT91RM9200_PIO_MDDR		0x54
#define	AT91RM9200_PIO_MDSR		0x58
#define	AT91RM9200_PIO_PUDR		0x60
#define	AT91RM9200_PIO_PUER		0x64
#define	AT91RM9200_PIO_PUSR		0x68
#define	AT91RM9200_PIO_ASR		0x70
#define	AT91RM9200_PIO_BSR		0x74
#define	AT91RM9200_PIO_ABSR		0x78
#define	AT91RM9200_PIO_OWER		0xA0
#define	AT91RM9200_PIO_OWDR		0xA4
#define	AT91RM9200_PIO_OWSR		0xA8


/*
 * Power Management Controller (PMC)
 */
#define	AT91RM9200_PMC_BASE		0xFFFFFC00

/* PMC Registers, offset from base address */
#define	AT91RM9200_PMC_SCER		0x00
#define	AT91RM9200_PMC_SCDR		0x04
#define	AT91RM9200_PMC_SCSR		0x08
#define	AT91RM9200_PMC_PCER		0x10
#define	AT91RM9200_PMC_PCDR		0x14
#define	AT91RM9200_PMC_PCSR		0x18
#define	AT91RM9200_CKFR_MOR		0x20
#define	AT91RM9200_CKFR_MCFR	0x24
#define	AT91RM9200_CKFR_PLLAR	0x28
#define	AT91RM9200_CKFR_PLLBR	0x2C
#define	AT91RM9200_PMC_MCKR		0x30
#define	AT91RM9200_PMC_PCK0		0x40
#define	AT91RM9200_PMC_PCK1		0x44
#define	AT91RM9200_PMC_PCK2		0x48
#define	AT91RM9200_PMC_PCK3		0x4C
#define	AT91RM9200_PMC_IER		0x60
#define	AT91RM9200_PMC_IDR		0x64
#define	AT91RM9200_PMC_SR		0x68
#define	AT91RM9200_PMC_IMR		0x6C


/*
 * System Timer
 */
#define	AT91RM9200_ST_BASE		0xFFFFFD00

#define	AT91RM9200_ST_SIZE		0x28

/* ST Registers, offset from base address */
#define	AT91RM9200_ST_CR		0x00
#define	AT91RM9200_ST_PIMR		0x04
#define	AT91RM9200_ST_WDMR		0x08
#define	AT91RM9200_ST_RTMR		0x0C
#define	AT91RM9200_ST_SR		0x10
#define	AT91RM9200_ST_IER		0x14
#define	AT91RM9200_ST_IDR		0x18
#define	AT91RM9200_ST_IMR		0x1C
#define	AT91RM9200_ST_RTAR		0x20
#define	AT91RM9200_ST_CRAR		0x24

/*
 * WDMR register bits
 */
#define	AT91RM9200_WDMR_RSTEN	(1 << 16)
#define	AT91RM9200_WDMR_EXTEN	(1 << 17)

/*
 * Real Time Clock (RTC)
 */
#define	AT91RM9200_RTC_BASE		0xFFFFFE00

/*
 * Memory Controller
 */
#define	AT91RM9200_MC_BASE		0xFFFFFF00


#endif	/* __ARM_AT91RM9200_H_INCLUDED */

/* __SRCVERSION("at91rm9200.h $Rev: 657836 $"); */
