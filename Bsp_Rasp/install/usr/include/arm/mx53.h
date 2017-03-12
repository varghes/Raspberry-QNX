/*
 * $QNXLicenseC:
 * Copyright 2010, QNX Software Systems. 
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
 * i.MX53 processor
 *
 */

#ifndef	__ARM_MX53_H_INCLUDED
#define	__ARM_MX53_H_INCLUDED


/*
 * Enhanced Secured Digital Host Controllers
 */
#define MX53_ESDHC1_BASE                        0x50004000
#define MX53_ESDHC2_BASE                        0x50008000
#define MX53_ESDHC3_BASE                        0x50020000
#define MX53_ESDHC4_BASE                        0x50024000
#define MX53_ESDHC_SIZE                         0x4000

/*
 * UARTs
 */
#define MX53_UART1_BASE                         0x53FBC000
#define MX53_UART2_BASE                         0x53FC0000
#define MX53_UART3_BASE                         0x5000C000
#define MX53_UART4_BASE                         0x53FF0000
#define MX53_UART5_BASE                         0x63F90000

#define MX53_UART1_INTR                         31
#define MX53_UART2_INTR                         32
#define MX53_UART3_INTR                         33
#define MX53_UART4_INTR                         13
#define MX53_UART5_INTR                         86

#define MX53_UART_SIZE                          0xE0
/*
 * USB Base Address
 */
#define MX53_USB_BASE                           0x53F80000
#define MX53_USBH1_BASE                         0x53F80300
#define MX53_USBH2_BASE                         0x53F80500
#define MX53_USBH3_BASE                         0x53F80700
#define MX53_USBOTG_BASE                        0x53F80100

#define MX53_USBH1_INTR                         14
#define MX53_USBH2_INTR                         16
#define MX53_USBH3_INTR                         17
#define MX53_USBOTG_INTR                        18

#define MX53_USB_SIZE                           0x1000


/*
 * Enhanced Configurable Serial Peripheral Interfaces
 */
#define MX53_ECSPI1_BASE                        0x50010000
#define MX53_ECSPI2_BASE                        0x63fac000

#define MX53_ECSPI1_IRQ                         36
#define MX53_ECSPI2_IRQ                         37

#define MX53_ECSPI1_SIZE                        0x4000
#define MX53_ECSPI2_SIZE                        0x4000

#define MX53_CSPI1_BASE                         0x63FC0000
#define MX53_CSPI1_IRQ                          38
#define MX53_CSPI_SIZE                          0x20

/*
 * Audio device
 */
#define MX53_AUDMUX_BASE                        0x63FD0000
#define MX53_AUDMUX_SIZE                        0x40

/* Port offset */
#define MX53_AUDMUX_PTCR1                       0x00
#define MX53_AUDMUX_PDCR1                       0x04
#define MX53_AUDMUX_PTCR2                       0x08
#define MX53_AUDMUX_PDCR2                       0x0C
#define MX53_AUDMUX_PTCR3                       0x10
#define MX53_AUDMUX_PDCR3                       0x14
#define MX53_AUDMUX_PTCR4                       0x18
#define MX53_AUDMUX_PDCR4                       0x1C
#define MX53_AUDMUX_PTCR5                       0x20
#define MX53_AUDMUX_PDCR5                       0x24
#define MX53_AUDMUX_PTCR6                       0x28
#define MX53_AUDMUX_PDCR6                       0x2C
#define MX53_AUDMUX_PTCR7                       0x30
#define MX53_AUDMUX_PDCR7                       0x34
#define MX53_AUDMUX_CNMCR                       0x38
#define MX53_AUDMUX_CNMCR                       0x38

#define CCM_BASE                                0x53FD4000

/* CCM Registers, offset from base address */
#define MX53_CCM_CCR                            0x00
#define MX53_CCM_CCDR                           0x04
#define MX53_CCM_CSR                            0x08
#define MX53_CCM_CCSR                           0x0C
#define MX53_CCM_CACCR                          0x10
#define MX53_CCM_CBCDR                          0x14
#define MX53_CCM_CBCMR                          0x18
#define MX53_CCM_CSCMR1                         0x1c
#define MX53_CCM_CSCMR2                         0x20
#define MX53_CCM_CSCDR1                         0x24
#define MX53_CCM_CS1CDR                         0x28
#define MX53_CCM_CS2CDR                         0x2c
#define MX53_CCM_CDCDR                          0x30
#define MX53_CCM_CSCDR2                         0x38
#define MX53_CCM_CSCDR3                         0x3c
#define MX53_CCM_CSCDR4                         0x40
#define MX53_CCM_CWDR                           0x44
#define MX53_CCM_CDHIPR                         0x48
#define MX53_CCM_CDCR                           0x4c
#define MX53_CCM_CTOR                           0x50

#define MX53_CCM_CLPCR                          0x54
#define MX53_CCM_CISR                           0x58
#define MX53_CCM_CIMR                           0x5c
#define MX53_CCM_CCOSR                          0x60
#define MX53_CCM_CGPR                           0x64
#define MX53_CCM_CCGR0                          0x68
#define MX53_CCM_CCGR1                          0x6c
#define MX53_CCM_CCGR2                          0x70
#define MX53_CCM_CCGR3                          0x74
#define MX53_CCM_CCGR4                          0x78
#define MX53_CCM_CCGR5                          0x7c
#define MX53_CCM_CCGR6                          0x80
#define MX53_CCM_CCGR7                          0x84
#define MX53_CCM_CMEOR                          0x88

#define MX53_CCM_CLKOSEL(x)                     (x & 0xF)
#define MX53_CLKOSEL_ARMCLOCK                   0x0
#define MX53_CLKOSEL_AUDIOCLOCK                 0x02
#define MX53_AUDIOCLOCK                         24576000
#define MX53_CCM_CLK2EN                         (1 << 24)
#define MX53_CCM_CLKOEN                         (1 << 7)
#define MX53_CCM_CLK2_DIV1(x)                   ((x&0x8) << 4)
#define MX53_CCM_CLKO_DIV1(x)                   ((x&0x01) << 6)
#define MX53_CCM_CLKO_DIV(x)                    ((x&0x3F) << 10)

/* DIGITAL AUDIO MULTIPLEXER (AUDMUX) */
#define AUDMUX_SYNC                             (1<<11)
#define AUDMUX_TFS_DIROUT                       (1<<31)
#define AUDMUX_TFS_PORT4                        (3<<27)
#define AUDMUX_TFS_PORT5                        (4<<27)
#define AUDMUX_TCLK_DIROUT                      (1<<26)
#define AUDMUX_TCLK_PORT4                       (3<<22)
#define AUDMUX_TCLK_PORT5                       (4<<22)
#define AUDMUX_RFS_DIROUT                       (1<<21)
#define AUDMUX_RFS_PORT4                        (3<<17)
#define AUDMUX_RFS_PORT5                        (4<<17)
#define AUDMUX_RCLK_DIROUT                      (1<<16)
#define AUDMUX_RCLK_PORT4                       (3<<12)
#define AUDMUX_RCLK_PORT5                       (4<<12)
#define AUDMUX_RXDSEL_PORT1                     (0<<13)
#define AUDMUX_RXDSEL_PORT2                     (1<<13)
#define AUDMUX_RXDSEL_PORT4                     (3<<13)
#define AUDMUX_RXDSEL_PORT5                     (4<<13)
#define AUDMUX_RXDSEL_MSK                       (7<<13)

/*
 * Synchronous Serial Interfaces
 */
#define MX53_SSI1_BASE                          0x63FCC000
#define MX53_SSI2_BASE                          0x50014000
#define MX53_SSI_SIZE                           0x4000

#define MX53_SSI1_IRQ                           29
#define MX53_SSI2_IRQ                           30

/*
 * Sony/Philips Digital Interface Transmitter
 */
#define MX53_SPDIF_BASE                         0x50028000
#define MX53_SPDIF_SIZE                         0x4000

/*
 * I2C
 */
#define MX53_I2C1_BASE                          0x63FC8000
#define MX53_I2C2_BASE                          0x63FC4000
#define MX53_I2C3_BASE                          0x53FEC000

#define MX53_I2C1_IRQ                           62
#define MX53_I2C2_IRQ                           63
#define MX53_I2C3_IRQ                           64

#define MX53_I2C_SIZE                           0x4000

/*
 * Fast Ethernet Controller
 */
#define MX53_FEC_BASE                           0x63FEC000
#define MX53_FEC_INTR                           87

/*
 * External Interface Module (EIM)
 */
#define MX53_EIM_BASE                           0x63FDC000
#define MX53_EIM_CS0GCR1                        0x00
#define MX53_EIM_CS0GCR2                        0x04
#define MX53_EIM_CS0RCR1                        0x08
#define MX53_EIM_CS0RCR2                        0x0C
#define MX53_EIM_CS0WCR1                        0x10
#define MX53_EIM_CS0WCR2                        0x14
#define MX53_EIM_CS1GCR1                        0x18
#define MX53_EIM_CS1GCR2                        0x1C
#define MX53_EIM_CS1RCR1                        0x20
#define MX53_EIM_CS1RCR2                        0x24
#define MX53_EIM_CS1WCR1                        0x28
#define MX53_EIM_CS1WCR2                        0x2C
#define MX53_EIM_CS2GCR1                        0x30
#define MX53_EIM_CS2GCR2                        0x34
#define MX53_EIM_CS2RCR1                        0x38
#define MX53_EIM_CS2RCR2                        0x3C
#define MX53_EIM_CS2WCR1                        0x40
#define MX53_EIM_CS2WCR2                        0x44
#define MX53_EIM_CS3GCR1                        0x48
#define MX53_EIM_CS3GCR2                        0x4C
#define MX53_EIM_CS3RCR1                        0x50
#define MX53_EIM_CS3RCR2                        0x54
#define MX53_EIM_CS3WCR1                        0x58
#define MX53_EIM_CS3WCR2                        0x5C
#define MX53_EIM_CS4GCR1                        0x60
#define MX53_EIM_CS4GCR2                        0x64
#define MX53_EIM_CS4RCR1                        0x68
#define MX53_EIM_CS4RCR2                        0x6C
#define MX53_EIM_CS4WCR1                        0x70
#define MX53_EIM_CS4WCR2                        0x74
#define MX53_EIM_CS5GCR1                        0x78
#define MX53_EIM_CS5GCR2                        0x7C
#define MX53_EIM_CS5RCR1                        0x80
#define MX53_EIM_CS5RCR2                        0x84
#define MX53_EIM_CS5WCR1                        0x88
#define MX53_EIM_CS5WCR2                        0x8C
#define MX53_EIM_WCR                            0x90
#define MX53_EIM_WIAR                           0x94
#define MX53_EIM_EAR                            0x98

/*
 * CAN
 */
#define MX53_CAN1_PORT                          0x53FC8000
#define MX53_CAN1_MEM                           0x53FC8080
#define MX53_CAN1_IRQ                           82

#define MX53_CAN2_PORT                          0x53FCC000
#define MX53_CAN2_MEM                           0x53FCC080
#define MX53_CAN2_IRQ                           83

#define MX53_CAN_SIZE                           0x4000

/*
 * GPIOs
 */
#define MX53_GPIO1_BASE                         0x53F84000
#define MX53_GPIO2_BASE                         0x53F88000
#define MX53_GPIO3_BASE                         0x53F8C000
#define MX53_GPIO4_BASE                         0x53F90000
#define MX53_GPIO5_BASE                         0x53FDC000
#define MX53_GPIO6_BASE                         0x53FE0000
#define MX53_GPIO7_BASE                         0x53FE4000
#define MX53_GPIO_SIZE                          0x4000

/* GPIO Registers, offset from base address */
#define MX53_GPIO_DR                            0x00
#define MX53_GPIO_GDIR                          0x04
#define MX53_GPIO_PSR                           0x08
#define MX53_GPIO_ICR1                          0x0C
#define MX53_GPIO_ICR2                          0x10
#define MX53_GPIO_IMR                           0x14
#define MX53_GPIO_ISR                           0x18
#define MX53_GPIO_EDR                           0x1C

/*
 * SDMA Base Address
 */
#define MX53_SDMA_BASE                          0x63FB0000
#define MX53_SDMA_INTRU                         6
#define MX53_SDMA_SIZE                          0x4000

/* SDMA Registers, offset from base address */
#define MX53_SDMA_MC0PTR                        0x00	/* AP (MCU) Channel 0 Pointer */
#define MX53_SDMA_INTR                          0x04	/* Channel Interrupts */
#define MX53_SDMA_STOP_STAT                     0x08	/* Channel Stop / Channel Status */
#define MX53_SDMA_HSTART                        0x0C	/* Channel Start */
#define MX53_SDMA_EVTOVR                        0x10	/* Channel Event Override */
#define MX53_SDMA_DSPOVR                        0x14	/* Channel DSP (BP) Override */
#define MX53_SDMA_HOSTOVR                       0x18	/* Channel AP Override */
#define MX53_SDMA_EVTPEND                       0x1C	/* Channel Event Pending */
#define MX53_SDMA_RESET                         0x24	/* Reset Register */
#define MX53_SDMA_EVTERR                        0x28	/* DMA Request Error Register */
#define MX53_SDMA_INTRMASK                      0x2C	/* Channel AP Interrupt Mask */
#define MX53_SDMA_PSW                           0x30	/* Schedule Status */
#define MX53_SDMA_EVTERRDBG                     0x34	/* DMA Request Error Register */
#define MX53_SDMA_CONFIG                        0x38	/* Configuration Register */
#define MX53_SDMA_ONCE_ENB                      0x40	/* OnCE Enable */
#define MX53_SDMA_ONCE_DATA                     0x44	/* OnCE Data Register */
#define MX53_SDMA_ONCE_INSTR                    0x48	/* OnCE Instruction Register */
#define MX53_SDMA_ONCE_STAT                     0x4C	/* OnCE Status Register */
#define MX53_SDMA_ONCE_CMD                      0x50	/* OnCE Command Register */
#define MX53_SDMA_EVT_MIRROT                    0x54	/* DMA Request */
#define MX53_SDMA_ILLINSTADDR                   0x58	/* Illegal Instruction Trap Address */
#define MX53_SDMA_CHN0ADDR                      0x5C	/* Channel 0 Boot Address */
#define MX53_SDMA_XTRIG_CONF1                   0x70	/* Cross-Trigger Events Configuration Register 1 */
#define MX53_SDMA_XTRIG_CONF2                   0x74	/* Cross-Trigger Events Configuration Register 2 */
#define MX53_SDMA_CHNPRI(n)                     (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */
#define MX53_SDMA_CHNENBL(n)                    (0x200 + ((n)<<2))   /* Channel Enable n = 0 to 47 */

/*
 * Keypad port
 */
#define MX53_KPP_BASE                           0x53F94000
#define MX53_KPP_SIZE                           0x4000

/*
 * Watchdogs
 */
#define MX53_WDOG1_BASE                         0x53F98000
#define MX53_WDOG2_BASE                         0x53F9C000
#define MX53_WDOG_SIZE                          0x4000

/*
 * Watchdog Timer Registers, offset from base address 
 */
#define MX53_WDOG_CR                            0x00
        #define WDE                             (0x01 << 2)
        #define WT                              (0xf0 << 8)
        #define WDT                             (0x01 << 3)
#define MX53_WDOG_SR                            0x02
#define MX53_WDOG_RSR                           0x04
#define MX53_WDOG_ICR                           0x06
#define MX53_WDOG_MCR                           0x08

/*
 * General Purpose Timer
 */
#define MX53_GPT_BASE                           0x53FA0000
#define MX53_GPT_SIZE                           0x4000

/*
 * Secure Real Time Clock
 */
#define MX53_SRTC_BASE                          0x53FA4000
#define MX53_SRTC_SIZE                          0x4000

/*
 * I/O MUX Controller
 */
#define MX53_IOMUXC_BASE                        0x53FA8000
#define MX53_IOMUXC_SIZE                        0x4000

/*
 * IOMUX Registers, offset from base address
 */
#define MX53_IOMUX_GPR0                         0x0000
#define MX53_IOMUX_GPR1                         0x0004
#define MX53_IOMUX_GPR2                         0x0008
#define MX53_IOMUXC_OBSERVE_MUX_0               0x000c
#define MX53_IOMUXC_OBSERVE_MUX_1               0x0010
#define MX53_IOMUXC_OBSERVE_MUX_2               0x0014
#define MX53_IOMUXC_OBSERVE_MUX_3               0x0018
#define MX53_IOMUXC_OBSERVE_MUX_4               0x001c

#define MX53_IOMUX_SWMUX                        0x0020
#define MX53_IOMUX_SWPAD                        0x0348
#define MX53_IOMUX_SWINPUT                      0x0730

/*
 * Enhanced Periodic Timers
 */
#define MX53_EPIT1_BASE                         0x53FAC000
#define MX53_EPIT2_BASE                         0x53FB0000
#define MX53_EPIT_SIZE                          0x4000

/*
 * EPIT Registers, off set from base address
 */
#define MX53_EPIT_CR                            0x00    /* Control Register */
#define MX53_EPIT_SR                            0x04    /* Status Register */
#define MX53_EPIT_LR                            0x08    /* Load Register */
#define MX53_EPIT_CMPR                          0x0C    /* Compare Register */
#define MX53_EPIT_CNR                           0x10    /* Counter Register */

/*
 * SDRAM
 */
#define MX53_SDRAM_BASE                         0x70000000
#define MX53_SDRAM_SIZE                         512

/*
 * MLB
 */
#define MX53_MLB_BASE                           0x63FE4000
#define MX53_MLB_SIZE                           0x4000
#define MX53_MLB_IRQ                            65

/*
 * Multimedia Peripherals
 */

/* Graphics */
#define MX53_GPU2D_BASE                         0x20000000
#define MX53_GPU2D_SIZE                         0x10000000
#define MX53_GPU3D_BASE                         0x30000000
#define MX53_GPU3D_SIZE                         0x10000000

#define MX53_GPU3D_IRQ                          12
#define MX53_GPU3D_IDLE_IRQ                     102

/* Image Processing: IPUv3M */
#define MX53_IPU_BASE                           0x18000000
#define MX53_IPU_SIZE                           0x8000000
#define MX53_IPU_ERR_IRQ                        10
#define MX53_IPU_SYNC_IRQ                       11

/* TV Encoder: TVEv2.1 */
#define MX53_TVE_BASE                           0x63FF0000
#define MX53_TVE_SIZE                           0x4000

/* Video Processing: VPU */
#define MX53_VPU_BASE                           0x63FF4000
#define MX53_VPU_SIZE                           0x4000
#define MX53_VPU_IRQ                            9
#define MX53_VPU_IDLE_IRQ                       100

/* IPU init values */
#define BACKLIGHT_BIT                           0x20000000
#define BACKLIGHT_ONOFF                         0x00000002
#define LVDS_PWR_EN                             0x00100000

/*
 * TrustZone Interrupt Controller (TZIC)
 */
#define MX53_TZIC_BASE                          0x0FFFC000

#define MX53_TZIC_INTCTRL                       0x0
#define MX53_TZIC_INTTYPE                       0x4
#define MX53_TZIC_PRIOMASK                      0xC
#define MX53_TZIC_SYNCCTRL                      0x10
#define MX53_TZIC_DSMINT                        0x14
#define MX53_TZIC_INTSEC(x)                     (0x80+((x)*0x4))   //x = 0 to 3
#define MX53_TZIC_ENSET(x)                      (0x100+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_ENCLEAR(x)                    (0x180+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_SRCSET(x)                     (0x200+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_SRCCLEAR(x)                   (0x280+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_PRIORITY0(x)                  (0x400+((x)*0x4))  //x = 0 to 31
#define MX53_TZIC_PEND(x)                       (0xD00+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_HIPND(x)                      (0xD80+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_WAKEUP(x)                     (0xE00+((x)*0x4))  //x = 0 to 3
#define MX53_TZIC_SWINT                         (0xF00+((x)*0x4))  //x = 0 to 3

/*
 * IC Identification Module (IIM)
 */
#define MX53_IIM_BASE                           0x63F98000

#define MX53_IIM_STAT                           0x0
#define MX53_IIM_STATM                          0x4
#define MX53_IIM_ERR                            0x8
#define MX53_IIM_EMASK                          0xC
#define MX53_IIM_FCTL                           0x10
#define MX53_IIM_UA                             0x14
#define MX53_IIM_LA                             0x18
#define MX53_IIM_SDAT                           0x1C
#define MX53_IIM_PREV                           0x20
#define MX53_IIM_SREV                           0x24
#define MX53_IIM_PREG_P                         0x28
#define MX53_IIM_SCS0                           0x2C
#define MX53_IIM_SCS2                           0x34
#define MX53_IIM_SCS3                           0x38

#endif	/* __ARM_MX53_H_INCLUDED */

/* __SRCVERSION("mx53.h $Rev: 169789 $"); */
