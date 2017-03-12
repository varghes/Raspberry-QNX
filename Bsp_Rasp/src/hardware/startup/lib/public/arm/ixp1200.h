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
 * IXP1200 processor
 *

 *
 */

#ifndef	__ARM_IXP1200_H_INCLUDED
#define	__ARM_IXP1200_H_INCLUDED

/*
 * -----------------------------------------------------------------------
 * PLL Configuration Register
 * -----------------------------------------------------------------------
 */
#define	IXP1200_PLL_CFG_BASE	0x90000c00
#define	IXP1200_PLL_CFG_SIZE	4

/*
 * PLL_CFG register CCF mask
 */
#define	IXP1200_PLL_CFG_CCF		0x1f

/*
 * -----------------------------------------------------------------------
 * GPIO
 * -----------------------------------------------------------------------
 */
#define	IXP1200_GPIO_BASE		0x90001800
#define	IXP1200_GPIO_SIZE		0x800

/*
 * Register offsets from IXP1200_GPIO_BASE
 */
#define	IXP1200_GPIO_EN			0x000
#define	IXP1200_GPIO_DATA		0x400

/*
 * GPIO_EN register bits
 */
#define	IXP1200_GPIO_EN_SA		0x00	/* SA owns GPIO 0-3 */
#define	IXP1200_GPIO_EN_IX_0	0x10	/* SA owns GPIO 1-3, IX owns GPIO 0 */
#define	IXP1200_GPIO_EN_SA_0	0x20	/* SA owns GPIO 0, IX owns GPIO 1-3 */
#define	IXP1200_GPIO_EN_IX		0x30	/* IX owns GPIO 0-3 */
#define	IXP1200_GPIO_EN_0_IN	0x00	/* GPIO 0 is input  */
#define	IXP1200_GPIO_EN_0_OUT	0x01	/* GPIO 0 is output */
#define	IXP1200_GPIO_EN_1_IN	0x00	/* GPIO 1 is input  */
#define	IXP1200_GPIO_EN_1_OUT	0x02	/* GPIO 1 is output */
#define	IXP1200_GPIO_EN_2_IN	0x00	/* GPIO 2 is input  */
#define	IXP1200_GPIO_EN_2_OUT	0x04	/* GPIO 2 is output */
#define	IXP1200_GPIO_EN_3_IN	0x00	/* GPIO 3 is input  */
#define	IXP1200_GPIO_EN_3_OUT	0x08	/* GPIO 3 is output */

/*
 * GPIO_DATA bits
 */
#define	IXP1200_GPIO_DATA_0		0x01
#define	IXP1200_GPIO_DATA_1		0x02
#define	IXP1200_GPIO_DATA_2		0x04
#define	IXP1200_GPIO_DATA_3		0x08

/*
 * -----------------------------------------------------------------------
 * RTC
 * -----------------------------------------------------------------------
 */
#define	IXP1200_RTC_BASE		0x90002000
#define	IXP1200_RTC_SIZE		0x1400

/*
 * Offsets from IXP1200_RTC_BASE
 */
#define	IXP1200_RTC_DIV		0x0000
#define	IXP1200_RTC_TINT	0x0400
#define	IXP1200_RTC_TVAL	0x0800
#define	IXP1200_RTC_CNTR	0x0c00
#define	IXP1200_RTC_ALM		0x1000

/*
 * RTC_DIV bits
 */
#define	IXP1200_RTC_DIV_RDIV(x)		((x) & 0xffff)
#define	IXP1200_RTC_DIV_WEN			0x10000		/* write enable */
#define	IXP1200_RTC_DIV_IEN			0x20000		/* interrupt enable */
#define	IXP1200_RTC_DIV_IRST		0x40000		/* reset RTC interrupt */
#define	IXP1200_RTC_DIV_IRQS		0x80000		/* 0: FIQ, 1: IRQ */

/*
 * RTC_TINT bits
 */
#define	IXP1200_RTC_TINT_RTINT(x)	((x) & 0xffff)

/*
 * RTC_TVAL bits
 */
#define	IXP1200_RTC_TVAL_TVAL(x)	((x) & 0xffff)
#define	IXP1200_RTC_TVAL_LD			0x10000		/* load RTC registers */
#define	IXP1200_RTC_TVAL_PRE		0x20000		/* 0: sysclk/128, 1: sysclk */

/*
 * -----------------------------------------------------------------------
 * UART
 * -----------------------------------------------------------------------
 */
#define	IXP1200_UART_BASE		0x90003400
#define	IXP1200_UART_SIZE		0xc00

/*
 * Register offsets from IXP1200_UART_BASE
 */
#define	IXP1200_UART_SR			0x000
#define	IXP1200_UART_CR			0x400
#define	IXP1200_UART_DR			0x800

/*
 * UART_SR bits
 */
#define	IXP1200_UART_SR_RPE		0x01		/* parity error in next char */
#define	IXP1200_UART_SR_RFE		0x02		/* framing error in next char */
#define	IXP1200_UART_SR_TXR		0x04		/* tx fifo ready */
#define	IXP1200_UART_SR_ROR		0x08		/* rx overrun */
#define	IXP1200_UART_SR_RXR		0x10		/* rx fifo ready */
#define	IXP1200_UART_SR_TXE		0x20		/* tx fifo empty */
#define	IXP1200_UART_SR_RXF		0x40		/* rx fifo full */
#define IXP1200_UART_SR_TXF		0x80		/* tx fifo full */

/*
 * UART_CR bits
 */
#define	IXP1200_UART_CR_BRK		0x00001		/* assert break */
#define	IXP1200_UART_CR_PE		0x00002		/* enable parity */
#define	IXP1200_UART_CR_PS		0x00004		/* 0=even parity, 1=odd parity */
#define	IXP1200_UART_CR_SBS		0x00008		/* 0=1 stop bit, 1=2 stop bits */
#define	IXP1200_UART_CR_RIE		0x00010		/* rx interrupt enable */
#define	IXP1200_UART_CR_DSS_5	0x00000		/* 5 data bits */
#define	IXP1200_UART_CR_DSS_6	0x00020		/* 5 data bits */
#define	IXP1200_UART_CR_DSS_7	0x00040		/* 5 data bits */
#define	IXP1200_UART_CR_DSS_8	0x00060		/* 5 data bits */
#define	IXP1200_UART_CR_UE		0x00080		/* uart enable */
#define	IXP1200_UART_CR_XIE		0x00100		/* tx interrupt enable */
#define	IXP1200_UART_CR_UIS		0x00200		/* 0=use IRQ, 1=use FIQ */

#define	IXP1200_UART_CR_BRD(x)	((x) << 16)	/* set UART_CR_BRD value */

/*
 * UART_DR bits
 */
#define	IXP1200_UART_DR_DATA(x)	((x) & 0xff)	/* mask off data */
#define	IXP1200_UART_DR_PRE		0x100			/* parity error in data */
#define	IXP1200_UART_DR_FRE		0x200			/* framing error in data */
#define	IXP1200_UART_DR_ROR		0x400			/* overrun error after data */

/*
 * -----------------------------------------------------------------------
 * Timer registers
 * -----------------------------------------------------------------------
 */
#define	IXP1200_TIMER_BASE		0x42000300
#define	IXP1200_TIMER_SIZE		0x70

/*
 * Register offsets from IXP1200_TIMER_BASE
 */
#define	IXP1200_TIMER_1_LOAD	0x00
#define	IXP1200_TIMER_1_VALUE	0x04
#define	IXP1200_TIMER_1_CONTROL	0x08
#define	IXP1200_TIMER_1_CLEAR	0x0c
#define	IXP1200_TIMER_2_LOAD	0x20
#define	IXP1200_TIMER_2_VALUE	0x24
#define	IXP1200_TIMER_2_CONTROL	0x28
#define	IXP1200_TIMER_2_CLEAR	0x2c
#define	IXP1200_TIMER_3_LOAD	0x40
#define	IXP1200_TIMER_3_VALUE	0x44
#define	IXP1200_TIMER_3_CONTROL	0x48
#define	IXP1200_TIMER_3_CLEAR	0x4c
#define	IXP1200_TIMER_4_LOAD	0x60
#define	IXP1200_TIMER_4_VALUE	0x64
#define	IXP1200_TIMER_4_CONTROL	0x68
#define	IXP1200_TIMER_4_CLEAR	0x6c

/*
 * TIMER_x_CONTROL bits
 */
#define	IXP1200_TIMER_CONTROL_STP_1		0x00	/* use core frequency */
#define	IXP1200_TIMER_CONTROL_STP_16	0x04	/* use core/16 */
#define	IXP1200_TIMER_CONTROL_STP_256	0x08	/* use core/256 */
#define	IXP1200_TIMER_CONTROL_MODE		0x40
#define	IXP1200_TIMER_CONTROL_FREE		0x00	/* free running 24-bit */
#define	IXP1200_TIMER_CONTROL_PERIODIC	0x40	/* periodic */
#define	IXP1200_TIMER_CONTROL_EN		0x80	/* enable timer */

/*
 * -----------------------------------------------------------------------
 * Interrupt Controller
 * -----------------------------------------------------------------------
 */
#define	IXP1200_IRQ_BASE		0x90001400		/* physical address */
#define	IXP1200_IRQ_SIZE		0x00000008

/*
 * IRQ register bits
 */
#define	IXP1200_IRQ_PCI		0x004		/* PCI unit interrupt */
#define	IXP1200_IRQ_CINT	0x008		/* CINT interrupt */
#define	IXP1200_IRQ_UENG	0x010		/* microengine interrupt */
#define	IXP1200_IRQ_SRAM	0x020		/* SRAM unit interrupt */
#define	IXP1200_IRQ_RTC		0x040		/* RTC interrupt */
#define	IXP1200_IRQ_SDRAM	0x080		/* SDRAM unit interrupt */
#define	IXP1200_IRQ_UART	0x100		/* UART interrupt */

/*
 * -----------------------------------------------------------------------
 * PCI Address Space and Registers
 * -----------------------------------------------------------------------
 */
#define	IXP1200_PCI_CFG_BASE	0x42000000		/* physical address */
#define	IXP1200_PCI_CFG_SIZE	0x400

#define	IXP1200_PCI_TYPE0_CONFIG_BASE	0x53000000
#define	IXP1200_PCI_TYPE1_CONFIG_BASE	0x52000000
#define	IXP1200_PCI_IO_BASE				0x54000000
#define	IXP1200_PCI_MEM_BASE			0x60000000

/*
 * Offsets from IXP1200_PCI_CFG_BASE
 */
#define	IXP1200_PCI_VEN_DEV_ID			0x00
#define	IXP1200_PCI_CMD_STAT			0x04
#define	IXP1200_PCI_REV_CLASS			0x08
#define	IXP1200_PCI_CACHE_LAT_HDR_BIST	0x0c
#define	IXP1200_PCI_MEM_BAR				0x10
#define	IXP1200_PCI_IO_BAR				0x14
#define	IXP1200_PCI_IO_DRAM_BAR			0x18
#define	IXP1200_PCI_SUBSYS				0x2c
#define	IXP1200_PCI_CAP_PTR				0x34
#define	IXP1200_PCI_INT_LAT				0x3c
#define	IXP1200_CAP_PTR_EXT				0x70
#define	IXP1200_PWR_MGMT				0x74
#define	IXP1200_IXP1200_RESET			0x7c
#define	IXP1200_CSR_BASE_ADDR_MASK		0xf8
#define	IXP1200_DRAM_BASE_ADDR_MASK		0x100
#define	IXP1200_SA_CONTROL				0x13c
#define	IXP1200_PCI_ADDR_EXT			0x140

/*
 * PCI Interrupt Controller (offsets from IXP1200_PCI_CFG_BASE)
 */
#define	IXP1200_IRQ_STATUS				0x180
#define	IXP1200_IRQ_RAW_STATUS			0x184
#define	IXP1200_IRQ_ENABLE				0x188
#define	IXP1200_IRQ_ENABLE_SET			0x188
#define	IXP1200_IRQ_ENABLE_CLEAR		0x18c
#define	IXP1200_IRQ_SOFT				0x190
#define	IXP1200_FIQ_STATUS				0x280
#define	IXP1200_FIQ_RAW_STATUS			0x284
#define	IXP1200_FIQ_ENABLE				0x288
#define	IXP1200_FIQ_ENABLE_SET			0x288
#define	IXP1200_FIQ_ENABLE_CLEAR		0x28c
#define	IXP1200_FIQ_SOFT				0x290

/*
 * PCI Interrupt Controller bits
 */
#define	IXP1200_IRQ_SI			0x00000001	/* s/w interrupt */
#define	IXP1200_IRQ_T1			0x00000010	/* timer 1 */
#define	IXP1200_IRQ_T2			0x00000020	/* timer 2 */
#define	IXP1200_IRQ_T3			0x00000040	/* timer 3 */
#define	IXP1200_IRQ_T4			0x00000080	/* timer 4 */
#define	IXP1200_IRQ_DFH			0x00008000	/* doorbell from host */
#define	IXP1200_IRQ_DMA1		0x00010000	/* DMA channel 1 */
#define	IXP1200_IRQ_DMA2		0x00020000	/* DMA channel 2 */
#define	IXP1200_IRQ_PIL			0x00040000	/* PCI_INT pin */
#define	IXP1200_IRQ_DMA1NB		0x00100000	/* DMA channel 1 not busy */
#define	IXP1200_IRQ_DMA2NB		0x00200000	/* DMA channel 2 not busy */
#define	IXP1200_IRQ_SB			0x00400000	/* start BIST */
#define	IXP1200_IRQ_RSERR		0x00800000	/* received SERR */
#define	IXP1200_IRQ_IIP			0x02000000	/* I20 inbound post_list */
#define	IXP1200_IRQ_PWRM		0x04000000	/* power management */
#define	IXP1200_IRQ_DTE			0x08000000	/* discard timer expired */
#define	IXP1200_IRQ_DPED		0x10000000	/* data parity error */
#define	IXP1200_IRQ_RMA			0x20000000	/* received master abort */
#define	IXP1200_IRQ_RTA			0x40000000	/* received target abort */
#define	IXP1200_IRQ_DPE			0x80000000	/* PCI parity error */

/*
 * SA_CONTROL bits
 */
#define	IXP1200_SA_CONTROL_PCF0		0x80000000
#define	IXP1200_SA_CONTROL_PCF1		0x40000000
#define	IXP1200_SA_CONTROL_BE_BEO	0x00040000
#define	IXP1200_SA_CONTROL_BE_DEO	0x00020000
#define	IXP1200_SA_CONTROL_BE_BEI	0x00010000
#define	IXP1200_SA_CONTROL_BE_DEI	0x00008000
#define	IXP1200_SA_CONTROL_WE		0x00002000
#define	IXP1200_SA_CONTROL_PNR		0x00000200
#define	IXP1200_SA_CONTROL_DTX		0x00000100
#define	IXP1200_SA_CONTROL_RS		0x00000008
#define	IXP1200_SA_CONTROL_PCF1N	0x00000004
#define	IXP1200_SA_CONTROL_AS		0x00000002
#define	IXP1200_SA_CONTROL_IC		0x00000001	

/*
 * -----------------------------------------------------------------------
 * FBI Registers
 * -----------------------------------------------------------------------
 */
#define	IXP1200_FBI_BASE		0xb0040000		/* physical address */
#define	IXP1200_FBI_SIZE		0x1000

/*
 * Offsets from IXP1200_FBI_BASE
 */
#define	IXP1200_IREG				0x1e0		/* FBI interrupts */
#define	IXP1200_SOP_SEQ1			0x020
#define	IXP1200_SOP_SEQ2			0x024
#define	IXP1200_ENQUEUE_SEQ1		0x028
#define	IXP1200_ENQUEUE_SEQ2		0x02c
#define	IXP1200_INTER_THD_SIG		0x1c0
#define	IXP1200_THREAD_DONE_REG0	0x008
#define	IXP1200_THREAD_DONE_REG1	0x00c
#define	IXP1200_RCV_RDY_CNT			0x0c0
#define	IXP1200_RCV_RDY_HI			0x0c8
#define	IXP1200_RCV_RDY_LO			0x0c4
#define	IXP1200_RCV_RDY_CTL			0x0f8
#define	IXP1200_RCV_REQ				0x01c
#define	IXP1200_RCV_CNTL			0x018
#define	IXP1200_REC_FASTPORT_CTL	0x030
#define	IXP1200_FP_READY_WAIT		0x1a4
#define	IXP1200_RFIFO_ADDR			0x19c
#define	IXP1200_FLOWCTL_MASK		0x194
#define	IXP1200_RDYBUS_TEMPLATE_CTL			0x180
#define	IXP1200_RDYBUS_TEMPLATE_PROG3		0x18c
#define	IXP1200_RDYBUS_TEMPLATE_PROG2		0x188
#define	IXP1200_RDYBUS_TEMPLATE_PROG1		0x184
#define	IXP1200_RDYBUS_SYNCH_COUNT_DEFAULT	0x190
#define	IXP1200_SELF_DESTRUCT				0x1d4
#define	IXP1200_HASH_MULTIPLIER_64_HI		0x144
#define	IXP1200_HASH_MULTIPLIER_64_LO		0x140
#define	IXP1200_HASH_MULTIPLIER_48_HI		0x14c
#define	IXP1200_HASH_MULTIPLIER_48_LO		0x148
#define	IXP1200_SEND_CMD					0x198
#define	IXP1200_GET_CMD						0x1b0
#define	IXP1200_XMIT_RDY_LO					0x0e4
#define	IXP1200_XMIT_RDY_HI					0x0e8
#define	IXP1200_XMIT_RDY_CTL				0x0fc
#define	IXP1200_XMIT_PTR					0x0e0

/*
 * IREG bits
 */
#define	IXP1200_IREG_THD0		0x00000001		/* uengine interrupt status */
#define	IXP1200_IREG_THD1		0x00000002		/* uengine interrupt status */
#define	IXP1200_IREG_THD2		0x00000004		/* uengine interrupt status */
#define	IXP1200_IREG_THD3		0x00000008		/* uengine interrupt status */
#define	IXP1200_IREG_THD4		0x00000010		/* uengine interrupt status */
#define	IXP1200_IREG_THD5		0x00000020		/* uengine interrupt status */
#define	IXP1200_IREG_THD6		0x00000040		/* uengine interrupt status */
#define	IXP1200_IREG_THD7		0x00000080		/* uengine interrupt status */
#define	IXP1200_IREG_THD8		0x00000100		/* uengine interrupt status */
#define	IXP1200_IREG_THD9		0x00000200		/* uengine interrupt status */
#define	IXP1200_IREG_THD10		0x00000400		/* uengine interrupt status */
#define	IXP1200_IREG_THD11		0x00000800		/* uengine interrupt status */
#define	IXP1200_IREG_THD12		0x00001000		/* uengine interrupt status */
#define	IXP1200_IREG_THD13		0x00002000		/* uengine interrupt status */
#define	IXP1200_IREG_THD14		0x00004000		/* uengine interrupt status */
#define	IXP1200_IREG_THD15		0x00008000		/* uengine interrupt status */
#define	IXP1200_IREG_THD16		0x00010000		/* uengine interrupt status */
#define	IXP1200_IREG_THD17		0x00020000		/* uengine interrupt status */
#define	IXP1200_IREG_THD18		0x00040000		/* uengine interrupt status */
#define	IXP1200_IREG_THD19		0x00080000		/* uengine interrupt status */
#define	IXP1200_IREG_THD20		0x00100000		/* uengine interrupt status */
#define	IXP1200_IREG_THD21		0x00200000		/* uengine interrupt status */
#define	IXP1200_IREG_THD22		0x00400000		/* uengine interrupt status */
#define	IXP1200_IREG_THD23		0x00800000		/* uengine interrupt status */
#define	IXP1200_IREG_DBINT0		0x01000000		/* debug interrupt */
#define	IXP1200_IREG_DBINT1		0x02000000		/* debug interrupt */
#define	IXP1200_IREG_DBINT2		0x04000000		/* debug interrupt */
#define	IXP1200_IREG_CINT		0x08000000		/* CINT status */
#define	IXP1200_IREG_CINTIRQ	0x10000000		/* CINT IRQ enable */
#define	IXP1200_IREG_CINTFIQ	0x20000000		/* CINT FIQ enable */
#define	IXP1200_IREG_UEIRQ		0x40000000		/* uengine IRQ enable */
#define	IXP1200_IREG_UEFIQ		0x80000000		/* uengine FIQ enable */

/*
 * -----------------------------------------------------------------------
 * Microengine Registers
 * -----------------------------------------------------------------------
 */
/* FIXME: to be done */

/*
 * -----------------------------------------------------------------------
 * SRAM Registers
 * -----------------------------------------------------------------------
 */
/* FIXME: to be done */

/*
 * -----------------------------------------------------------------------
 * SDRAM Registers
 * -----------------------------------------------------------------------
 */
/* FIXME: to be done */

#endif	/* __ARM_IXP1200_H_INCLUDED */

/* __SRCVERSION("ixp1200.h $Rev: 657836 $"); */
