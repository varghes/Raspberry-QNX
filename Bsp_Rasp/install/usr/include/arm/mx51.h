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
 * i.MX51 processor
 *
 */

#ifndef	__ARM_MX51_H_INCLUDED
#define	__ARM_MX51_H_INCLUDED


/*
 * Enhanced Secured Digital Host Controllers
 */
#define	MX51_ESDHC1_BASE		0x70004000
#define	MX51_ESDHC2_BASE		0x70008000
#define	MX51_ESDHC3_BASE		0x70020000
#define	MX51_ESDHC4_BASE		0x70024000
#define	MX51_ESDHC_SIZE			0x4000

/*
 * UARTs
 */
#define	MX51_UART1_BASE			0x73FBC000
#define	MX51_UART2_BASE			0x73FC0000

#define MX51_UART1_INTR			31
#define MX51_UART2_INTR			32

#define MX51_UART_SIZE                  0xE0
/*
 * USB Base Address
 */
#define MX51_USB_BASE                   0x73F80000	
#define MX51_USBH1_BASE                 0x73F80300      
#define MX51_USB_INTR                   14
#define MX51_USB_SIZE                   0x1000


/*
 * Enhanced Configurable Serial Peripheral Interfaces
 */
#define MX51_ECSPI1_BASE                0x70010000
#define MX51_ECSPI2_BASE                0x83fac000
#define MX51_ECSPI1_SIZE                0x4000
#define MX51_ECSPI2_SIZE                0x4000
#define MX51_ECSPI1_IRQ                 36
#define MX51_ECSPI2_IRQ                 37

#define MX51_CSPI1_BASE                 0x83FC0000
#define MX51_CSPI1_IRQ                  38
#define MX51_CSPI_SIZE                  0x20

/*
 * Audio device
 */
#define MX51_AUDMUX_BASE                0x83FD0000
#define MX51_AUDMUX_SIZE                0x40

/* Port offset */
#define MX51_AUDMUX_PTCR1               0x00
#define MX51_AUDMUX_PDCR1               0x04
#define MX51_AUDMUX_PTCR2               0x08
#define MX51_AUDMUX_PDCR2               0x0C
#define MX51_AUDMUX_PTCR3               0x10
#define MX51_AUDMUX_PDCR3               0x14
#define MX51_AUDMUX_PTCR4               0x18
#define MX51_AUDMUX_PDCR4               0x1C
#define MX51_AUDMUX_PTCR5               0x20
#define MX51_AUDMUX_PDCR5               0x24
#define MX51_AUDMUX_PTCR6               0x28
#define MX51_AUDMUX_PDCR6               0x2C
#define MX51_AUDMUX_PTCR7               0x30
#define MX51_AUDMUX_PDCR7               0x34
#define MX51_AUDMUX_CNMCR               0x38
#define MX51_AUDMUX_CNMCR               0x38

 #define CCM_BASE                        0x73FD4000

/* CCM Registers, offset from base address */
#define MX51_CCM_CCR			0x00
#define MX51_CCM_CCDR			0x04
#define MX51_CCM_CSR			0x08
#define MX51_CCM_CCSR			0x0C
#define MX51_CCM_CACCR			0x10
#define MX51_CCM_CBCDR			0x14
#define MX51_CCM_CBCMR			0x18
#define MX51_CCM_CSCMR1			0x1c
#define MX51_CCM_CSCMR2			0x20
#define MX51_CCM_CSCDR1			0x24
#define MX51_CCM_CS1CDR			0x28
#define MX51_CCM_CS2CDR			0x2c
#define MX51_CCM_CDCDR			0x30
#define MX51_CCM_CSCDR2			0x38
#define MX51_CCM_CSCDR3			0x3c
#define MX51_CCM_CSCDR4			0x40
#define MX51_CCM_CWDR			0x44
#define MX51_CCM_CDHIPR			0x48
#define MX51_CCM_CDCR			0x4c
#define MX51_CCM_CTOR			0x50

#define MX51_CCM_CLPCR			0x54
#define MX51_CCM_CISR			0x58
#define MX51_CCM_CIMR			0x5c
#define MX51_CCM_CCOSR			0x60
#define MX51_CCM_CGPR			0x64
#define MX51_CCM_CCGR0			0x68
#define MX51_CCM_CCGR1			0x6c
#define MX51_CCM_CCGR2			0x70
#define MX51_CCM_CCGR3			0x74
#define MX51_CCM_CCGR4			0x78
#define MX51_CCM_CCGR5			0x7c
#define MX51_CCM_CCGR6			0x80
#define MX51_CCM_CMEOR			0x84

#define MX51_CCM_CLKOSEL(x)             (x & 0xF)
#define MX51_CLKOSEL_ARMCLOCK   0x0
#define MX51_CLKOSEL_AUDIOCLOCK 0x02
#define MX51_AUDIOCLOCK         24576000
#define MX51_CCM_CLK2EN                 (1 << 24)
#define MX51_CCM_CLKOEN                 (1 << 7)
#define MX51_CCM_CLK2_DIV1(x)           ((x&0x8) << 4)
#define MX51_CCM_CLKO_DIV1(x)           ((x&0x01) << 6)
#define MX51_CCM_CLKO_DIV(x)            ((x&0x3F) << 10)

/* DIGITAL AUDIO MULTIPLEXER (AUDMUX) */
#define AUDMUX_SYNC             (1<<11)
#define AUDMUX_TFS_DIROUT       (1<<31)
#define AUDMUX_TFS_PORT4        (3<<27)
#define AUDMUX_TFS_PORT5        (4<<27)
#define AUDMUX_TCLK_DIROUT      (1<<26)
#define AUDMUX_TCLK_PORT4       (3<<22)
#define AUDMUX_TCLK_PORT5       (4<<22)
#define AUDMUX_RFS_DIROUT       (1<<21)
#define AUDMUX_RFS_PORT4        (3<<17)
#define AUDMUX_RFS_PORT5        (4<<17)
#define AUDMUX_RCLK_DIROUT      (1<<16)
#define AUDMUX_RCLK_PORT4       (3<<12)
#define AUDMUX_RCLK_PORT5       (4<<12)
#define AUDMUX_RXDSEL_PORT1     (0<<13)
#define AUDMUX_RXDSEL_PORT2     (1<<13)
#define AUDMUX_RXDSEL_PORT4     (3<<13)
#define AUDMUX_RXDSEL_PORT5     (4<<13)
#define AUDMUX_RXDSEL_MSK       (7<<13)

/*
 * Synchronous Serial Interfaces
 */
#define MX51_SSI1_BASE                  0x83FCC000
#define MX51_SSI2_BASE                  0x70014000
#define MX51_SSI_SIZE                   0x4000

#define MX51_SSI1_IRQ			29
#define MX51_SSI2_IRQ			30

/*
 * Sony/Philips Digital Interface Transmitter
 */
#define	MX51_SPDIF_BASE			0x70028000
#define	MX51_SPDIF_SIZE			0x4000

/*
 * I2C
 */
#define MX51_I2C1_BASE                	0x83FC8000
#define MX51_I2C1_IRQ		      	62

#define MX51_I2C2_BASE                	0x83FC4000
#define MX51_I2C2_IRQ                 	63

#define MX51_I2C_SIZE                	0x4000
/*
 * High Speed Inter IC
 */
#define	MX51_HS_I2C_BASE		0x70038000
#define	MX51_HS_I2C_SIZE		0x4000

/* Fast Ethernet Controller */
#define MX51_FEC_BASE                   0x83FEC000
#define MX51_FEC_INTR                   87

/*
 * GPIOs
 */
#define	MX51_GPIO1_BASE			0x73F84000
#define	MX51_GPIO2_BASE			0x73F88000
#define	MX51_GPIO3_BASE			0x73F8C000
#define	MX51_GPIO4_BASE			0x73F90000
#define	MX51_GPIO_SIZE			0x4000

/* GPIO Registers, offset from base address */
#define MX51_GPIO_DR                    0x00
#define MX51_GPIO_GDIR                  0x04
#define MX51_GPIO_PSR                   0x08
#define MX51_GPIO_ICR1                  0x0C
#define MX51_GPIO_ICR2                  0x10
#define MX51_GPIO_IMR                   0x14
#define MX51_GPIO_ISR                   0x18
#define MX51_GPIO_EDR                   0x1C

/*
 * SDMA Base Address
 */
#define MX51_SDMA_BASE                  0x83FB0000
#define MX51_SDMA_INTRU                 6
#define MX51_SDMA_SIZE                  0x4000

/* SDMA Registers, offset from base address */
#define	MX51_SDMA_MC0PTR		0x00	/* AP (MCU) Channel 0 Pointer */
#define	MX51_SDMA_INTR			0x04	/* Channel Interrupts */
#define	MX51_SDMA_STOP_STAT		0x08	/* Channel Stop / Channel Status */
#define	MX51_SDMA_HSTART		0x0C	/* Channel Start */
#define	MX51_SDMA_EVTOVR		0x10	/* Channel Event Override */
#define	MX51_SDMA_DSPOVR		0x14	/* Channel DSP (BP) Override */
#define	MX51_SDMA_HOSTOVR		0x18	/* Channel AP Override */
#define	MX51_SDMA_EVTPEND		0x1C	/* Channel Event Pending */
#define	MX51_SDMA_RESET			0x24	/* Reset Register */
#define	MX51_SDMA_EVTERR		0x28	/* DMA Request Error Register */
#define	MX51_SDMA_INTRMASK		0x2C	/* Channel AP Interrupt Mask */
#define	MX51_SDMA_PSW			0x30	/* Schedule Status */
#define	MX51_SDMA_EVTERRDBG		0x34	/* DMA Request Error Register */
#define	MX51_SDMA_CONFIG		0x38	/* Configuration Register */
#define	MX51_SDMA_ONCE_ENB		0x40	/* OnCE Enable */
#define	MX51_SDMA_ONCE_DATA		0x44	/* OnCE Data Register */
#define	MX51_SDMA_ONCE_INSTR	0x48	/* OnCE Instruction Register */
#define	MX51_SDMA_ONCE_STAT		0x4C	/* OnCE Status Register */
#define	MX51_SDMA_ONCE_CMD		0x50	/* OnCE Command Register */
#define	MX51_SDMA_EVT_MIRROT	0x54	/* DMA Request */
#define	MX51_SDMA_ILLINSTADDR	0x58	/* Illegal Instruction Trap Address */
#define	MX51_SDMA_CHN0ADDR		0x5C	/* Channel 0 Boot Address */
#define	MX51_SDMA_XTRIG_CONF1	0x70	/* Cross-Trigger Events Configuration Register 1 */
#define	MX51_SDMA_XTRIG_CONF2	0x74	/* Cross-Trigger Events Configuration Register 2 */
#define MX51_SDMA_CHNPRI(n)     (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */
#define MX51_SDMA_CHNENBL(n)    (0x200 + ((n)<<2))   /* Channel Enable n = 0 to 47 */

/*
 * Keypad port
 */
#define	MX51_KPP_BASE			0x73F94000
#define	MX51_KPP_SIZE			0x4000

/*
 * Watchdogs
 */
#define	MX51_WDOG1_BASE			0x73F98000
#define	MX51_WDOG2_BASE			0x73F9C000
#define	MX51_WDOG_SIZE			0x4000

/*
 * Watchdog Timer Registers, offset from base address 
 */
#define MX51_WDOG_CR                    0x00
        #define WDE                     (0x01 << 2)
        #define WT                      (0xf0 << 8)
        #define WDT                     (0x01 << 3)
#define MX51_WDOG_SR                    0x02
#define MX51_WDOG_RSR                   0x04
#define MX51_WDOG_ICR                   0x06
#define MX51_WDOG_MCR                   0x08

/*
 * General Purpose Timer
 */
#define	MX51_GPT_BASE			0x73FA0000
#define	MX51_GPT_SIZE			0x4000

/*
 * Secure Real Time Clock
 */
#define	MX51_SRTC_BASE			0x73FA4000
#define	MX51_SRTC_SIZE			0x4000

/*
 * I/O MUX Controller
 */
#define	MX51_IOMUXC_BASE		0x73FA8000
#define	MX51_IOMUXC_SIZE		0x4000

/* IOMUX Registers, offset from base address */
#define MX51_IOMUX_GPR0                         0x0000
#define MX51_IOMUX_GPR1                         0x0004
#define MX51_IOMUXC_OBSERVE_MUX_0               0x0008
#define MX51_IOMUXC_OBSERVE_MUX_1               0x000c
#define MX51_IOMUXC_OBSERVE_MUX_2               0x0010
#define MX51_IOMUXC_OBSERVE_MUX_3               0x0014
#define MX51_IOMUXC_OBSERVE_MUX_4               0x0018

#define MX51_IOMUX_SWMUX    		0x001C
#define MX51_IOMUX_SWPAD                0x03F0
#define MX51_IOMUX_SWINPUT              0x08C4

/*
 * Enhanced Periodic Timers
 */
#define	MX51_EPIT1_BASE			0x73FAC000
#define	MX51_EPIT2_BASE			0x73FB0000
#define	MX51_EPIT_SIZE			0x4000

/* EPIT Registers, off set from base address */
#define MX51_EPIT_CR                    0x00    /* Control Register */
#define MX51_EPIT_SR                    0x04    /* Status Register */
#define MX51_EPIT_LR                    0x08    /* Load Register */
#define MX51_EPIT_CMPR                  0x0C    /* Compare Register */
#define MX51_EPIT_CNR                   0x10    /* Counter Register */

/*SDRAM*/
#define MX51_SDRAM_BASE     0x90000000
#define MX51_SDRAM_SIZE		 512


#endif	/* __ARM_MX51_H_INCLUDED */

/* __SRCVERSION("mx51.h $Rev: 169789 $"); */
