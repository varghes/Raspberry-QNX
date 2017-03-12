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
/*
 * Register Offsets for the Cirrus Logic EP93xx (with ARM 920T processor)
 * internal devices
 */

#ifndef	_EP93xx_H_
#define	_EP93xx_H_

/*
 * ------------------------------------------------------------------------
 * DMA Controllers
 * ------------------------------------------------------------------------
*/
#define EP93xx_DMA_M2P_CHAN0	0x80000000U
#define EP93xx_DMA_M2P_CHAN1	0x80000040U
#define EP93xx_DMA_M2P_CHAN2	0x80000080U
#define EP93xx_DMA_M2P_CHAN3	0x800000C0U
#define EP93xx_DMA_M2P_CHAN4	0x80000240U
#define EP93xx_DMA_M2P_CHAN5	0x80000200U
#define EP93xx_DMA_M2P_CHAN6	0x800002C0U
#define EP93xx_DMA_M2P_CHAN7	0x80000280U
#define EP93xx_DMA_M2P_CHAN8	0x80000340U
#define EP93xx_DMA_M2P_CHAN9	0x80000300U

#define EP93xx_DMA_CHAN_ARB		0x0380U
#define EP93xx_DMA_GBL_INTR		0x03C0U

/*
 * ------------------------------------------------------------------------
 * Ethernet Controller
 * ------------------------------------------------------------------------
*/
#define EP93xx_ENET_BASE		0x80010000U
#define EP93xx_ENET_SIZE		0x5100U

/*
 * ------------------------------------------------------------------------
 * System Controller
 * ------------------------------------------------------------------------
*/
#define EP93xx_SYSCTRL_PWRSTS		0x0000U
#define EP93xx_SYSCTRL_PWRCNT		0x0004U
#define EP93xx_SYSCTRL_HALT			0x0008U
#define EP93xx_SYSCTRL_STANDBY		0x000CU
#define EP93xx_SYSCTRL_TEOI			0x0018U
#define EP93xx_SYSCTRL_STFCLR		0x001CU
#define EP93xx_SYSCTRL_CLKSET1		0x0020U
#define EP93xx_SYSCTRL_CLKSET2		0x0024U
#define EP93xx_SYSCTRL_SCRATCH0		0x0040U
#define EP93xx_SYSCTRL_SCRATCH1		0x0044U
#define EP93xx_SYSCTRL_APBWAIT		0x0050U
#define EP93xx_SYSCTRL_BUSMSTRARB	0x0054U
#define EP93xx_SYSCTRL_BOOTMODECLR	0x0058U
#define EP93xx_SYSCTRL_DEVICECFG	0x0080U
#define EP93xx_SYSCTRL_VIDCLKDIV	0x0084U
#define EP93xx_SYSCTRL_MIRCLKDIV	0x0088U
#define EP93xx_SYSCTRL_I2SCLKDIV	0x008CU
#define EP93xx_SYSCTRL_KEYTCHCLKDIV	0x0090U
#define EP93xx_SYSCTRL_CHIPID		0x0094U
#define EP93xx_SYSCTRL_SYSCFG		0x009CU
#define EP93xx_SYSCTRL_SYSSWLOCK	0x00C0U


/*
 * ------------------------------------------------------------------------
 * Clock/Timer Controller offsets
 * ------------------------------------------------------------------------
*/

/* the following offsets are relative to the per timer base address */
#define	EP93xx_TIMER_LOAD			0x0000U
#define	EP93xx_TIMER_VALUE			0x0004U
#define	EP93xx_TIMER_CTRL			0x0008U
#define EP93xx_TIMER_CLR			0x000CU

/* bit masks for the various timer registers */
#define EP93xx_TIMER_CTRL_ENABLE		(1U << 7)		// 1 == enabled
#define EP93xx_TIMER_CTRL_CLKSEL(_c_)	(((_c_) & 0x1) << 3)	// 1 == 508 KHz, 0 == 2 KHz
#define EP93xx_TIMER_CTRL_CLKMODE(_m_)	(((_m_) & 0x1) << 6)	// 1 == periodic, 0 == free run

#define EP93xx_TIMER_CTRL_CLKMODE_PERIODIC		EP93xx_TIMER_CTRL_CLKMODE(1)
#define	EP93xx_TIMER_CTRL_CLKMODE_FREERUN		EP93xx_TIMER_CTRL_CLKMODE(0)

/*
 * ------------------------------------------------------------------------
 * SDRAM
 * ------------------------------------------------------------------------
*/
#define EP93xx_SDRAM_GL_CONFIG  	0x0004U
#define EP93xx_SDRAM_REFRESH_TIMER	0x0008U
#define EP93xx_SDRAM_BOOT_STATUS  	0x000CU
#define EP93xx_SDRAM_DEV_CONFIG0  	0x0010U
#define EP93xx_SDRAM_DEV_CONFIG1  	0x0014U
#define EP93xx_SDRAM_DEV_CONFIG2  	0x0018U
#define EP93xx_SDRAM_DEV_CONFIG3  	0x001CU

/*
 * ------------------------------------------------------------------------
 * UART REGISTERS
 * 
 * unless otherwise noted, these register offsets apply to all 3 uarts.
 * There are additional modem/hdlc registers for UART1 and UART3 however
 * they are not used for the debug interface
 * ------------------------------------------------------------------------
*/
#define EP93xx_UART_DATA			0x0000U
#define EP93xx_UART_RXSTS			0x0004U
#define EP93xx_UART_LNCTRL_H		0x0008U
#define EP93xx_UART_LNCTRL_M		0x000CU
#define EP93xx_UART_LNCTRL_L		0x0010U
#define EP93xx_UART_CTRL			0x0014U
#define EP93xx_UART_FLAGS			0x0018U
#define EP93xx_UART_INTR			0x001CU
#define EP93xx_UART_IRDALOWPWRCNTR	0x0020U		// only for UART2 and UART3
#define EP93xx_UART_DMA_CTRL		0x0028U

/* some register bit masks needed by startup */ 
#define EP93xx_UART_CTRL_ENABLE		0x00000001U

#define EP93xx_UART_FLAGS_TXFE		0x00000080U
#define EP93xx_UART_FLAGS_RXFE		0x00000010U
#define EP93xx_UART_FLAGS_TXFF		0x00000020U
#define EP93xx_UART_FLAGS_BUSY		0x00000008U

/* LNCTRL_H register value for 8N1, no fifo, no break */
#define EP93xx_UART_LNCTRL_H_8N1	0x00000060U

/*
 * ------------------------------------------------------------------------
 * INTERRUPT CONTROLLER
 * ------------------------------------------------------------------------
*/
#define EP93xx_VIC_IRQ_STATUS		0x0000U
#define EP93xx_VIC_FIQ_STATUS		0x0004U
#define EP93xx_VIC_INT_RAW			0x0008U
#define EP93xx_VIC_INT_SELECT		0x000CU
#define EP93xx_VIC_INT_ENABLE		0x0010U
#define EP93xx_VIC_INT_CLEAR		0x0014U
#define EP93xx_VIC_SWINT_ENABLE		0x0018U
#define EP93xx_VIC_SWINT_CLEAR		0x001CU
#define EP93xx_VIC_PROTECTION		0x0020U
#define EP93xx_VIC_VEC_ADDR_CURR	0x0030U
#define EP93xx_VIC_VEC_ADDR_DFLT	0x0034U
/* offset to each of the 15 vector address registers */
#define EP93xx_VIC_VEC_ADDR(n)		(0x0100U + (((n) & 15) * 4))
/* offset to each of the 15 vector control registers */
#define EP93xx_VIC_VEC_CTRL(n)		(0x0200U + (((n) & 15) * 4))

#define EP93xx_VIC_PERIPHERAL_ID0	0x0FE0U
#define EP93xx_VIC_PERIPHERAL_ID1	0x0FE4U
#define EP93xx_VIC_PERIPHERAL_ID2	0x0FE8U
#define EP93xx_VIC_PERIPHERAL_ID3	0x0FECU

/*
 * ------------------------------------------------------------------------
 * GPIO
 * ------------------------------------------------------------------------
*/
#define EP93xx_GPIO_PORTA_INT_CLEAR		0x0098U
#define EP93xx_GPIO_PORTA_INT_ENABLE	0x009CU
#define EP93xx_GPIO_PORTA_INT_STATUS	0x00A0U

#define EP93xx_GPIO_PORTB_INT_CLEAR		0x00B4U
#define EP93xx_GPIO_PORTB_INT_ENABLE	0x00B8U
#define EP93xx_GPIO_PORTB_INT_STATUS	0x00BCU

#define EP93xx_GPIO_PORTF_INT_CLEAR		0x0054U
#define EP93xx_GPIO_PORTF_INT_ENABLE	0x0058U
#define EP93xx_GPIO_PORTF_INT_STATUS	0x005CU

#endif	/* _EP93xx_H_ */

/* __SRCVERSION( "$URL$ $Rev$" ); */
