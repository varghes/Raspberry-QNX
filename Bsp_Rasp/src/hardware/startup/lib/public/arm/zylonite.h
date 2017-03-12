/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008, QNX Software Systems.  
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
 * Marvell Zylonite Development System specifics
 *
 *
 *
 */
#ifndef ARM_ZYLONITE_H_INCLUDED
#define ARM_ZYLONITE_H_INCLUDED

/*
 * -------------------------------------------------------------------------
 * Serial ports
 * -------------------------------------------------------------------------
 */

/* UART1 base address */
#define	ZYLONITE_UART1_BASE		0x40100000

/* UART2 base address */
#define ZYLONITE_UART2_BASE		0x40200000

/* UART3 base address */
#define ZYLONITE_UART3_BASE		0x40700000

#define	ZYLONITE_UART_SIZE		0x30

/* UART registers, offset from base address */
#define	ZYLONITE_UART_FFRBR			0x00		/* RX buffer,DLAB=0			 */
#define	ZYLONITE_UART_FFTHR			0x00		/* TX holding,DLAB=0		 */
#define	ZYLONITE_UART_FFIER			0x04		/* Interrupt Enable,DLAB=0	 */
#define	ZYLONITE_UART_FFIIR			0x08		/* Interrupt ID				 */
#define	ZYLONITE_UART_FFFCR			0x08		/* FIFO Control				 */
#define	ZYLONITE_UART_FFLCR			0x0C		/* Line Control  			 */
#define	ZYLONITE_UART_FFMCR			0x10		/* Modem Control			 */
#define	ZYLONITE_UART_FFLSR			0x14		/* Line Status				 */
#define	ZYLONITE_UART_FFMSR			0x18		/* Modem Status				 */
#define	ZYLONITE_UART_FFSPR			0x1C		/* Scratch pad				 */
#define	ZYLONITE_UART_FFDLL			0x00		/* Divisor Latch low,DLAB=1	 */
#define	ZYLONITE_UART_FFDLH			0x04		/* Divisor Latch high,DLAB=1 */
#define	ZYLONITE_UART_FFISR			0x20		/* Serial IR			 	 */
#define	ZYLONITE_UART_FFFOR			0x24		/* Fifo Occupancy 			 */
#define	ZYLONITE_UART_FFABR			0x28		/* Autobaud Control			 */
#define	ZYLONITE_UART_FFACR			0x2C		/* Autobaud Count			 */


/* 
 * Fifo Control Register bits
 */
#define	ZYLONITE_CNTL_ITL1BYTE		(0<<6)		/* 1 byte in Fifo cause Intr  		*/
#define	ZYLONITE_CNTL_ITL8BYTE		(1<<6)		/* 8 byte in Fifo cause Intr  		*/
#define	ZYLONITE_CNTL_ITL16BYTE		(2<<6)		/* 16 byte in Fifo cause Intr 		*/
#define	ZYLONITE_CNTL_ITL32BYTE		(3<<6)		/* 32 byte in Fifo cause Intr  		*/
#define	ZYLONITE_CNTL_BUS			(1<<5)		/* 32 bit peripheral bus			*/
#define	ZYLONITE_CNTL_TRAIL			(1<<4)		/* Trailing bytes removed by dmac   */
#define	ZYLONITE_CNTL_TIL			(1<<3)		/* Intr/dma req when fifo empty		*/
#define	ZYLONITE_CNTL_RESETTF		(1<<2)		/* Clear TX FIFO				 	*/
#define	ZYLONITE_CNTL_RESETRF		(1<<1)		/* Clear RX FIFO			 		*/
#define	ZYLONITE_CNTL_TRFIFOE		(1<<0)		/* Enable TX/RX	FIFO mode			*/


/* 
 * Line Control Register bits
 */
#define	ZYLONITE_CNTL_DLAB			(1<<7)		/* Access Divisor Latch   	 */
#define	ZYLONITE_CNTL_SB			(1<<6)		/* Force TXD to 0	 		 */
#define	ZYLONITE_CNTL_STKYP			(1<<5)		/* Force parity	opp to EPS	 */
#define	ZYLONITE_CNTL_EPS			(1<<4)		/* Even parity				 */
#define	ZYLONITE_CNTL_PEN			(1<<3)		/* Parity Enable			 */
#define	ZYLONITE_CNTL_STB			(1<<2)		/* 2 stop bits			 	 */
#define	ZYLONITE_CNTL_WLS0			(0<<0)		/* 7 bits Word Length Select */
#define	ZYLONITE_CNTL_WLS1			(1<<0)		/* 7 bits Word Length Select */
#define	ZYLONITE_CNTL_WLS2			(2<<0)		/* 7 bits Word Length Select */
#define	ZYLONITE_CNTL_WLS3			(3<<0)		/* 8 bits Word Length Select */


/* 
 * Interrupt enable Register  bits
 */
#define	ZYLONITE_INTEN_DMAE				(1<<7)		/* DMA req enable			 */
#define	ZYLONITE_INTEN_UUE				(1<<6)		/* UART unit enable	 		 */
#define	ZYLONITE_INTEN_NRZE				(1<<5)		/* NRZ coding enabled		 */
#define	ZYLONITE_INTEN_RTOIE			(1<<4)		/* RX Timeout		 		 */
#define	ZYLONITE_INTEN_MIE				(1<<3)		/* Modem Status Intr En		 */
#define	ZYLONITE_INTEN_RLSE				(1<<2)		/* RX Line Status Intr En	 */
#define	ZYLONITE_INTEN_TIE				(1<<1)		/* TX data req Intr En		 */
#define	ZYLONITE_INTEN_RAVIE		     1			/* RX data available Intr En */


/*
 * Line Status Register bits
 */
#define	ZYLONITE_STATUS_FIFOE			(1<<7)		/* Fifo Error Status	 */
#define	ZYLONITE_STATUS_TXEMPTY			(1<<6)		/* TX Eempty			 */
#define	ZYLONITE_STATUS_TDRQ			(1<<5)		/* TX transmit data req	 */
#define	ZYLONITE_STATUS_BI				(1<<4)		/* Break signal		 	 */
#define	ZYLONITE_STATUS_FRAME			(1<<3)		/* Framing error		 */
#define	ZYLONITE_STATUS_PARITY			(1<<2)		/* Parity error			 */
#define	ZYLONITE_STATUS_OVERRUN			(1<<1)		/* Overrun error		 */
#define	ZYLONITE_STATUS_DR		 		 1			/* Data available in RBR */


/*
 * Uart Interrupts (*to check)
 */
#define ZYLONITE_EIC_UART0			(1<<5)
#define ZYLONITE_EIC_UART1 			(1<<6)


/*
 * -------------------------------------------------------------------------
 * General Purpose IO
 * -------------------------------------------------------------------------
 */

/* GPIO base address */
#define	ZYLONITE_GPIO_BASE		0x40E00000		

#define	ZYLONITE_GPIO_SIZE		0x4B0

/* GPIO registers, offset from base address */
#define	ZYLONITE_GPIO_GPLR0		0x00
#define	ZYLONITE_GPIO_GPLR1		0x04
#define	ZYLONITE_GPIO_GPLR2		0x08
#define	ZYLONITE_GPIO_GPLR3		0x100

#define	ZYLONITE_GPIO_GPDR0		0x0C
#define	ZYLONITE_GPIO_GPDR1		0x10
#define	ZYLONITE_GPIO_GPDR2		0x14
#define	ZYLONITE_GPIO_GPDR3		0x10C	

#define	ZYLONITE_GPIO_GPSR0		0x18
#define	ZYLONITE_GPIO_GPSR1		0x1C
#define	ZYLONITE_GPIO_GPSR2		0x20
#define	ZYLONITE_GPIO_GPSR3		0x118

#define	ZYLONITE_GPIO_GPCR0		0x24
#define	ZYLONITE_GPIO_GPCR1		0x28
#define	ZYLONITE_GPIO_GPCR2		0x2c
#define	ZYLONITE_GPIO_GPCR3		0x124

#define	ZYLONITE_GPIO_GRER0		0x30
#define	ZYLONITE_GPIO_GRER1		0x34
#define	ZYLONITE_GPIO_GRER2		0x38
#define	ZYLONITE_GPIO_GRER3		0x130

#define	ZYLONITE_GPIO_GFER0		0x3C
#define	ZYLONITE_GPIO_GFER1		0x40
#define	ZYLONITE_GPIO_GFER2		0x44
#define	ZYLONITE_GPIO_GFER3		0x13C

#define	ZYLONITE_GPIO_GEDR0		0x48
#define	ZYLONITE_GPIO_GEDR1		0x4C
#define	ZYLONITE_GPIO_GEDR2		0x50
#define	ZYLONITE_GPIO_GEDR3		0x148

#define	ZYLONITE_GPIO_GSDR0		0x400
#define	ZYLONITE_GPIO_GSDR1		0x404
#define	ZYLONITE_GPIO_GSDR2		0x408
#define	ZYLONITE_GPIO_GSDR3		0x40C

#define	ZYLONITE_GPIO_GCDR0		0x420
#define	ZYLONITE_GPIO_GCDR1		0x424
#define	ZYLONITE_GPIO_GCDR2		0x428
#define	ZYLONITE_GPIO_GCDR3		0x42C

#define	ZYLONITE_GPIO_GSRER0	0x440
#define	ZYLONITE_GPIO_GSRER1	0x444
#define	ZYLONITE_GPIO_GSRER2	0x448
#define	ZYLONITE_GPIO_GSRER3	0x44C

#define	ZYLONITE_GPIO_GCRER0	0x460
#define	ZYLONITE_GPIO_GCRER1	0x464
#define	ZYLONITE_GPIO_GCRER2	0x468
#define	ZYLONITE_GPIO_GCRER3	0x46C

#define	ZYLONITE_GPIO_GSFER0	0x480
#define	ZYLONITE_GPIO_GSFER1	0x484
#define	ZYLONITE_GPIO_GSFER2	0x488
#define	ZYLONITE_GPIO_GSFER3	0x48C

#define	ZYLONITE_GPIO_GCFER0	0x4A0
#define	ZYLONITE_GPIO_GCFER1	0x4A4
#define	ZYLONITE_GPIO_GCFER2	0x4A8
#define	ZYLONITE_GPIO_GCFER3	0x4AC


/*
 *  GPIO Register bit definitions
 */
#define ZYLONITE_GPLRx_HIGH		1
#define ZYLONITE_GPDRx_OP		1
#define ZYLONITE_GSDRx_OP		1
#define ZYLONITE_GCDRx_IP		1
#define ZYLONITE_GPSRx_HIGH		1
#define ZYLONITE_GPCRx_LOW		1
#define ZYLONITE_GRERx_RSDET	1
#define ZYLONITE_GSRERx_SET		1
#define ZYLONITE_GCRERx_CLR		1
#define ZYLONITE_GFERx_FLDET	1
#define ZYLONITE_GSFERx_SET		1
#define ZYLONITE_GSFERx_CLR		1
#define ZYLONITE_GEDRx_EGDET	1

/*
 * Static memory controller register values
 */
#define CSMSADRCFG_value	0x2
#define CSADRCFG0_value		0x00020000
#define CSADRCFG1_value		0x00020000
#define CSADRCFG2_value		0x0032091D
#define CSADRCFG3_value		0x003E080B
#define MSC1_value			0x7FFC7FF8

/*
 * Data Flash Controller register values
 */
#define DFC_ND_RNB_value	0x00001900
#define DFC_ND_NWE_value	0x00001901
#define DFC_ND_CS0_value	0x00001901
#define DFC_ND_CS1_value	0x00001901
#define DFC_ND_NRE_value	0x00001901


#endif // ARM_ZYLONITE_H_INCLUDED

