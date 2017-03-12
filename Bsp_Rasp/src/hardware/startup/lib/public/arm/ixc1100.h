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
 * IXC1100 processor (IXP425 core are well)
 *

 *
 */

#ifndef	__ARM_IXC1100_H_INCLUDED
#define	__ARM_IXC1100_H_INCLUDED


/*
 * -----------------------------------------------------------------------
 * UART
 * -----------------------------------------------------------------------
 */


#define	IXP425_UART_BASE		0xc8000000
#define	IXP425_UART_SIZE		0x20

/*
 * Register offsets from IXP2400_UART_BASE (registers that differ from 8250)
 */

#define	IXP425_UART_IER		0x04

/*
 * UART_IER bits
 */

#define	IXP425_UART_IER_RTOIEN	0x10	/* receiver data time out */
#define	IXP425_UART_IER_NRZE	0x20	/* NRZ coding enable */
#define	IXP425_UART_IER_UUE		0x40	/* uart unit enable */



/*
 * -----------------------------------------------------------------------
 * Timer registers
 * -----------------------------------------------------------------------
 */

#define IXP425_TIMER_BASE			0xc8005000
#define IXP425_TIMER_SIZE  			0x24


/*
 * Register offsets from IXP425_TIMER_BASE
 */

#define IXP425_TIMER0_VAL				0x4
#define IXP425_TIMER0_RELOAD  			0x8
#define IXP425_TIMER1_VAL				0xc
#define IXP425_TIMER1_RELOAD  			0x10
#define IXP425_TIMER_STATUS				0x20

/*
 * TIMER_STATUS bits
 */

#define	IXP425_TIMER_STATUS_OST_BITS	0x1f
#define	IXP425_TIMER_STATUS_OST_TIMER0  0x01

/*
 *  TIMERx_RELOAD
 */

#define IXP425_TIMER0_RELOAD_EN			0x01

/*
 * -----------------------------------------------------------------------
 * Interrupt Controller
 * -----------------------------------------------------------------------
 */

#define IXP425_IRQ_SIZE			0x20
#define IXP425_IRQ_BASE			0xC8003000

#define IXP465_IRQ_SIZE			0x38
/*
 * IRQ register
 */


#define IXP425_IRQ_ENABLE	0x4
#define IXP465_IRQ_ENABLE2	0x24
#define IXP425_IRQ_STATUS	0xC
#define IXP465_IRQ_STATUS2 0x2C
#define IXP425_IRQ_HIPRIO	0x18


/*
 *  IRQ register bits (IXP425_IRQ_STATUS)
 */

#define IXP425_IRQ_WAN			0
#define IXP425_IRQ_ETHNPEA	    1
#define IXP425_IRQ_ETHNPEB	    2
#define IXP425_IRQ_QUEUE1	    3
#define IXP425_IRQ_QUEUE2	    4
#define IXP425_IRQ_TIMERS	    5
#define IXP425_IRQ_GPIO0	    6
#define IXP425_IRQ_GPIO1	    7
#define IXP425_IRQ_PCIINT		8
#define IXP425_IRQ_PCIDMA1		9
#define IXP425_IRQ_PCIDMA2		10
#define IXP425_IRQ_USB			12
#define IXP425_IRQ_CUART		13   /* docs says 15, seems wrong */
#define IXP425_IRQ_TIMERSTAMP	14 
#define IXP425_IRQ_FUART		15   /* docs says 13, seems wrong */
#define IXP425_IRQ_TIMERWATCH   16
#define IXP425_IRQ_PERFMON		17
#define IXP425_IRQ_XPERFMON		18
#define IXP425_IRQ_GPIO2	    19
#define IXP425_IRQ_GPIO3	    20
#define IXP425_IRQ_GPIO4	    21
#define IXP425_IRQ_GPIO5	    22
#define IXP425_IRQ_GPIO6	    23
#define IXP425_IRQ_GPIO7	    24
#define IXP425_IRQ_GPIO8	    25
#define IXP425_IRQ_GPIO9	    26
#define IXP425_IRQ_GPIO10	    27
#define IXP425_IRQ_GPIO11	    28
#define IXP425_IRQ_GPIO12	    29
#define IXP425_IRQ_SW0		    30
#define IXP425_IRQ_SW1		    31



/*
 * -----------------------------------------------------------------------
 * GPIO Controller
 * -----------------------------------------------------------------------
 */

#define IXP425_GPIO_SIZE	0x20
#define IXP425_GPIO_BASE	0xC8004000

/*
 * GPIO register   (offset from base)
 */

#define IXP425_GPIO_GPISR	0xC
#define IXP425_GPIO_GPIT1R	0x10
#define IXP425_GPIO_GPIT2R	0x14



/*
 * -----------------------------------------------------------------------
 * PCI Address Space and Registers
 * -----------------------------------------------------------------------
 */

#define IXC1100_PCI_MEM_BASE           0x48000000
#define IXC1100_PCI_MEM_LEN             0x03FFFFFF
#define IXC1100_PCI_IO_BASE               0x4c000000
#define IXC1100_PCI_IO_LEN                 0X00FFFFFF

/*
 * PCI Controller CSRs Base
 */
#define IXC1100_PCI_CSR_BASE		0xC0000000
#define IXC1100_PCI_CSR_LEN			0x70

/*
 * PCI Configuration registers
 */
#define IXC1100_PCI_DIDVID			0x00
#define IXC1100_PCI_SRCR			0x04
#define IXC1100_PCI_CCRID			0x08
#define IXC1100_PCI_BHLC			0x0c
#define IXC1100_PCI_BAR0			0x10
#define IXC1100_PCI_BAR1			0x14
#define IXC1100_PCI_BAR2			0x18
#define IXC1100_PCI_BAR3			0x1c
#define IXC1100_PCI_BAR4			0x20
#define IXC1100_PCI_BAR5			0x24
#define IXC1100_PCI_SIDSVID			0x2C
#define IXC1100_PCI_LATINT			0x3C
#define IXC1100_PCI_RTOTTO			0x40

/*
 * PCI Controller CSRs
 */
#define IXC1100_PCI_NP_AD			0x00
#define IXC1100_PCI_NP_CBE			0x04
#define IXC1100_PCI_NP_WDATA		0x08
#define IXC1100_PCI_NP_RDATA		0x0c
#define IXC1100_PCI_CRP_AD_CBE		0x10
#define IXC1100_PCI_CRP_WDATA		0x14
#define IXC1100_PCI_CRP_RDATA		0x18
#define IXC1100_PCI_CSR				0x1C
#define IXC1100_PCI_ISR				0x20
#define IXC1100_PCI_INTEN			0x24
#define IXC1100_PCI_DMACTRL			0x28
#define IXC1100_PCI_AHBMEMBASE		0x2C
#define IXC1100_PCI_AHBIOBASE		0x30
#define IXC1100_PCI_PCIMEMBASE		0x34
#define IXC1100_PCI_AHBDOORBELL		0x38
#define IXC1100_PCI_PCIDOORBELL		0x3C
#define IXC1100_PCI_ATPDMA0_AHBADDR	0x40
#define IXC1100_PCI_ATPDMA0_PCIADDR	0x44
#define IXC1100_PCI_ATPDMA0_LENGTH	0x48
#define IXC1100_PCI_ATPDMA1_AHBADDR	0x4C
#define IXC1100_PCI_ATPDMA1_PCIADDR	0x50
#define IXC1100_PCI_ATPDMA1_LENGTH	0x54
#define IXC1100_PCI_PTADMA0_AHBADDR	0x58
#define IXC1100_PCI_PTADMA0_PCIADDR	0x5C
#define IXC1100_PCI_PTADMA0_LENGTH	0x60
#define IXC1100_PCI_PTADMA1_AHBADDR	0x64
#define IXC1100_PCI_PTADMA1_PCIADDR	0x68
#define IXC1100_PCI_PTADMA1_LENGTH	0x6C

#define IXC1100_PCI_CRP_CMD_WRITE	1<<16
#define IXC1100_PCI_CRP_CMD_READ	0<<16
#define IXC1100_PCI_CRP_BE_WORD		0x0<<20

#define IXC1100_PCI_NP_CMD_WRITE	0x0000000b
#define IXC1100_PCI_NP_CMD_READ		0x0000000a
#define IXC1100_PCI_NP_BE_WORD		0x0 << 4

#endif				/* __ARM_IXC1100_H_INCLUDED */

/* __SRCVERSION("ixc1100.h $Rev: 657836 $"); */
