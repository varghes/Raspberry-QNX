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
 * TI DM816x processor with ARMv7 Cortex-A8 core
 * TI DM814x processor with ARMv7 Cortex-A8 core
 *
 */

#ifndef    __ARM_DM816X_H_INCLUDED
#define    __ARM_DM816X_H_INCLUDED

#define SRAM0_START                   0x40300000

#define DM816X_UART0_BASE             0x48020000
#define DM816X_UART1_BASE             0x48022000
#define DM816X_UART2_BASE             0x48024000
#define DM816X_UART3_BASE             0x481A6000

/* DM Timer base addresses */
#define DM816X_TIMER_SIZE             0x1000
#define DM816X_TIMER0_BASE            0x4802C000
#define DM816X_TIMER1_BASE            0x4802E000
#define DM816X_TIMER2_BASE            0x48040000
#define DM816X_TIMER3_BASE            0x48042000
#define DM816X_TIMER4_BASE            0x48044000
#define DM816X_TIMER5_BASE            0x48046000
#define DM816X_TIMER6_BASE            0x48048000
#define DM816X_TIMER7_BASE            0x4804A000

/* GPIO Base address */
#define DM816X_GPIO0_BASE             0x48032000
#define DM816X_GPIO1_BASE             0x4804C000
#define DM816X_GPIO2_BASE             0x481AC000
#define DM816X_GPIO3_BASE             0x481AE000

/* BCH Error Location Module */
#define ELM_BASE                      0x48080000

/* Watchdog Timer */
#define WD1_BASE                      0x480C2000

/* Control Module Base Address */
#define CTRL_BASE                     0x48140000

/* PRCM Base Address */
#define PRCM_BASE                     0x48180000

/* SATA Base Address */
#define SATA_BASE			0x4A140000

/* EMIF Base address */
#define EMIF4_0_CFG_BASE              0x4C000000
#define EMIF4_1_CFG_BASE              0x4D000000
#define DMM_BASE                      0x4E000000

/* GPMC Base address */
#define GPMC_BASE                     0x50000000

/* IVAHD Config space */
#define IVAHD0_CONFIG_BASE            0x58000000
#define IVAHD1_CONFIG_BASE            0x5A000000
#define IVAHD2_CONFIG_BASE            0x53000000

#define BIT(x)                        (1 << x)
#define CL_BIT(x)                     (0 << x)

/* TODO: VB__This has to be the ID for TI816X which can be read from some register */
#define CPU_TI816X                      816

/* Timer registers */
#define DM816X_TIMER_TIDR               0x00        /* Timer Identification Register */
#define DM816X_TIMER_THWINFO            0x04
#define DM816X_TIMER_TIOCP_CFG          0x10        /* Timer OCP Configuration Register */
#define DM816X_TIMER_IRQ_EOI            0x20        /* Timer IRQ EOI Register */
#define DM816X_TIMER_IRQSTATUS_RAW      0x24        /* Timer IRQ STATUS RAW Register */
#define DM816X_TIMER_IRQSTATUS          0x28        /* Timer IRQ STATUS Register */
#define DM816X_TIMER_IRQENABLE_SET      0x2C        /* Timer IRQ ENABLE SET Register*/
#define DM816X_TIMER_IRQENABLE_CLR      0x30        /* Timer IRQ ENABLE CLR Register*/
#define DM816X_TIMER_IRQWAKEEN          0x34        /* Timer IRQ WAKEUP ENABLE Register*/
#define DM816X_TIMER_TCLR               0x38        /* Timer Control Register */
#define DM816X_TIMER_TCRR               0x3C        /* Timer Counter Register */
#define DM816X_TIMER_TLDR               0x40        /* Timer Load Value Register*/
#define DM816X_TIMER_TTGR               0x44        /* Timer Trigger Register */
#define DM816X_TIMER_TWPS               0x48        /* Timer Write Posted Status Register */
#define DM816X_TIMER_TMAR               0x4C        /* Timer Match Register*/
#define DM816X_TIMER_TCAR1              0x50        /* Timer Capture Register 1 */
#define DM816X_TIMER_TSICR              0x54        /* Timer Synchronous Interface Control Register */
#define DM816X_TIMER_TCAR2              0x58        /* Timer Capture Register 2 */

/* Timer register bits */
#define DM816X_TIMER_MAT_EN_FLAG        BIT(0)        /* IRQ bit for Match */
#define DM816X_TIMER_OVF_EN_FLAG        BIT(1)        /* IRQ bit for Overflow */
#define DM816X_TIMER_TCAR_EN_FLAG       BIT(2)        /* IRQ bit for Compare */

#define DM816X_TIMER_TCLR_ST            BIT(0)        /* Start=1 Stop=0 */
#define DM816X_TIMER_TCLR_AR            BIT(1)        /* Auto reload */
#define DM816X_TIMER_TCLR_PTV_SHIFT        (2)            /* Pre-scaler shift value */
#define DM816X_TIMER_TCLR_PRE           BIT(5)        /* Pre-scaler enable for the timer input clk */
#define DM816X_TIMER_TCLR_PRE_DISABLE   CL_BIT(5)    /* Pre-scalar disable for the timer input clk */

/* Control */
#define CONTROL_STATUS                  (CTRL_BASE + 0x40)

/* device type */
#define DEVICE_MASK                     (BIT(8) | BIT(9) | BIT(10))
#define TST_DEVICE                      0x0
#define EMU_DEVICE                      0x1
#define HS_DEVICE                       0x2
#define GP_DEVICE                       0x3

/* This gives the status of the boot mode pins on the evm
 */
#define SYSBOOT_MASK                   (BIT(0) | BIT(1) | BIT(2) |BIT(3) |BIT(4))

/* PLL related registers */
#define MAINPLL_CTRL                   (CTRL_BASE + 0x0400)
#define MAINPLL_PWD                    (CTRL_BASE + 0x0404)
#define MAINPLL_FREQ1                  (CTRL_BASE + 0x0408)
#define MAINPLL_DIV1                   (CTRL_BASE + 0x040C)
#define MAINPLL_FREQ2                  (CTRL_BASE + 0x0410)
#define MAINPLL_DIV2                   (CTRL_BASE + 0x0414)
#define MAINPLL_FREQ3                  (CTRL_BASE + 0x0418)
#define MAINPLL_DIV3                   (CTRL_BASE + 0x041C)
#define MAINPLL_FREQ4                  (CTRL_BASE + 0x0420)
#define MAINPLL_DIV4                   (CTRL_BASE + 0x0424)
#define MAINPLL_FREQ5                  (CTRL_BASE + 0x0428)
#define MAINPLL_DIV5                   (CTRL_BASE + 0x042C)
#define MAINPLL_DIV6                   (CTRL_BASE + 0x0434)
#define MAINPLL_DIV7                   (CTRL_BASE + 0x043C)

#define DDRPLL_CTRL                    (CTRL_BASE + 0x0440)
#define DDRPLL_PWD                     (CTRL_BASE + 0x0444)
/* #define DDRPLL_FREQ1                0x04 */
#define DDRPLL_DIV1                    (CTRL_BASE + 0x044C)
#define DDRPLL_FREQ2                   (CTRL_BASE + 0x0450)
#define DDRPLL_DIV2                    (CTRL_BASE + 0x0454)
#define DDRPLL_FREQ3                   (CTRL_BASE + 0x0458)
#define DDRPLL_DIV3                    (CTRL_BASE + 0x045C)
#define DDRPLL_FREQ4                   (CTRL_BASE + 0x0460)
#define DDRPLL_DIV4                    (CTRL_BASE + 0x0464)
#define DDRPLL_FREQ5                   (CTRL_BASE + 0x0468)
#define DDRPLL_DIV5                    (CTRL_BASE + 0x046C)

#define DDR_RCD                        (CTRL_BASE + 0x070C)


#define VIDEOPLL_CTRL                  (CTRL_BASE + 0x0470)
#define VIDEOPLL_PWD                   (CTRL_BASE + 0x0474)
#define VIDEOPLL_FREQ1                 (CTRL_BASE + 0x0478)
#define VIDEOPLL_DIV1                  (CTRL_BASE + 0x047C)
#define VIDEOPLL_FREQ2                 (CTRL_BASE + 0x0480)
#define VIDEOPLL_DIV2                  (CTRL_BASE + 0x0484)
#define VIDEOPLL_FREQ3                 (CTRL_BASE + 0x0488)
#define VIDEOPLL_DIV3                  (CTRL_BASE + 0x048C)

#define AUDIOPLL_CTRL                  (CTRL_BASE + 0x04A0)
#define AUDIOPLL_PWD                   (CTRL_BASE + 0x04A4)
/*
#define AUDIOPLL_FREQ1        0x04
#define AUDIOPLL_DIV1         0x04
This is not be meddled with as it supplies clk to the secure wdt
Fixed at 32kHz
 */
#define AUDIOPLL_FREQ2                 (CTRL_BASE + 0x04B0)
#define AUDIOPLL_DIV2                  (CTRL_BASE + 0x04B4)
#define AUDIOPLL_FREQ3                 (CTRL_BASE + 0x04B8)
#define AUDIOPLL_DIV3                  (CTRL_BASE + 0x04BC)
#define AUDIOPLL_FREQ4                 (CTRL_BASE + 0x04C0)
#define AUDIOPLL_DIV4                  (CTRL_BASE + 0x04C4)
#define AUDIOPLL_FREQ5                 (CTRL_BASE + 0x04C8)
#define AUDIOPLL_DIV5                  (CTRL_BASE + 0x04CC)

/* PRCM */
#define CM_DPLL_OFFSET                 (PRCM_BASE + 0x0300)

/* Registers for selecting the PRCM divider values */
#define CM_SYSCLK1_CLKSEL              (CM_DPLL_OFFSET + 0x00)
#define CM_SYSCLK2_CLKSEL              (CM_DPLL_OFFSET + 0x04)
#define CM_SYSCLK3_CLKSEL              (CM_DPLL_OFFSET + 0x08)
#define CM_SYSCLK4_CLKSEL              (CM_DPLL_OFFSET + 0x0C)
#define CM_SYSCLK7_CLKSEL              (CM_DPLL_OFFSET + 0x18)
#define CM_SYSCLK8_CLKSEL              (CM_DPLL_OFFSET + 0x1C)
#define CM_SYSCLK9_CLKSEL              (CM_DPLL_OFFSET + 0x20)
#define CM_SYSCLK10_CLKSEL             (CM_DPLL_OFFSET + 0x24)
#define CM_SYSCLK11_CLKSEL             (CM_DPLL_OFFSET + 0x2C)
#define CM_SYSCLK12_CLKSEL             (CM_DPLL_OFFSET + 0x30)
#define CM_SYSCLK13_CLKSEL             (CM_DPLL_OFFSET + 0x34)
#define CM_SYSCLK15_CLKSEL             (CM_DPLL_OFFSET + 0x38)
#define CM_VPB3_CLKSEL                 (CM_DPLL_OFFSET + 0x40)
#define CM_VPC1_CLKSEL                 (CM_DPLL_OFFSET + 0x44)
#define CM_VPD1_CLKSEL                 (CM_DPLL_OFFSET + 0x48)
#define CM_SYSCLK19_CLKSEL             (CM_DPLL_OFFSET + 0x4C)
#define CM_SYSCLK20_CLKSEL             (CM_DPLL_OFFSET + 0x50)
#define CM_SYSCLK21_CLKSEL             (CM_DPLL_OFFSET + 0x54)
#define CM_SYSCLK22_CLKSEL             (CM_DPLL_OFFSET + 0x58)
#define CM_APA_CLKSEL                  (CM_DPLL_OFFSET + 0x5C)
#define CM_SYSCLK14_CLKSEL             (CM_DPLL_OFFSET + 0x70)
#define CM_SYSCLK16_CLKSEL             (CM_DPLL_OFFSET + 0x74)
#define CM_SYSCLK18_CLKSEL             (CM_DPLL_OFFSET + 0x78)
#define CM_AUDIOCLK_MCASP0_CLKSEL      (CM_DPLL_OFFSET + 0x7C)
#define CM_AUDIOCLK_MCASP1_CLKSEL      (CM_DPLL_OFFSET + 0x80)
#define CM_AUDIOCLK_MCASP2_CLKSEL      (CM_DPLL_OFFSET + 0x84)
#define CM_AUDIOCLK_MCBSP_CLKSEL       (CM_DPLL_OFFSET + 0x88)
#define CM_TIMER0_CLKSEL               (CM_DPLL_OFFSET + 0x8C)
#define CM_TIMER1_CLKSEL               (CM_DPLL_OFFSET + 0x90)
#define CM_TIMER2_CLKSEL               (CM_DPLL_OFFSET + 0x94)
#define CM_TIMER3_CLKSEL               (CM_DPLL_OFFSET + 0x98)
#define CM_TIMER4_CLKSEL               (CM_DPLL_OFFSET + 0x9C)
#define CM_TIMER5_CLKSEL               (CM_DPLL_OFFSET + 0xA0)
#define CM_TIMER6_CLKSEL               (CM_DPLL_OFFSET + 0xA4)
#define CM_TIMER7_CLKSEL               (CM_DPLL_OFFSET + 0xA8)
#define CM_HDMI_CLKSEL                 (CM_DPLL_OFFSET + 0xAC)
#define CM_SYSCLK23_CLKSEL             (CM_DPLL_OFFSET + 0xB0)
#define CM_SYSCLK24_CLKSEL             (CM_DPLL_OFFSET + 0xB4)


/* Timers */
#define CM_ALWON_TIMER_0_CLKCTRL       (PRCM_BASE + 0x156C)
#define CM_ALWON_TIMER_1_CLKCTRL       (PRCM_BASE + 0x1570)
#define CM_ALWON_TIMER_2_CLKCTRL       (PRCM_BASE + 0x1574)
#define CM_ALWON_TIMER_3_CLKCTRL       (PRCM_BASE + 0x1578)
#define CM_ALWON_TIMER_4_CLKCTRL       (PRCM_BASE + 0x157C)
#define CM_ALWON_TIMER_5_CLKCTRL       (PRCM_BASE + 0x1580)
#define CM_ALWON_TIMER_6_CLKCTRL       (PRCM_BASE + 0x1584)
#define CM_ALWON_TIMER_7_CLKCTRL       (PRCM_BASE + 0x1588)

/* RTC */
#define CM_ALWON_RTC_CLKSTCTRL         (PRCM_BASE + 0x142C)
#define CM_ALWON_RTC_CLKCTRL           (PRCM_BASE + 0x15F0)

/* rtc */
#define DM816X_RTC_BASE                0x480C0000
#define    DM816X_RTC_SIZE             0x1000

#define DM816X_RTC_SECONDS             0x00
#define DM816X_RTC_MINUTES             0x04
#define DM816X_RTC_HOURS               0x08
#define DM816X_RTC_DAYS                0x0c
#define DM816X_RTC_MONTHS              0x10
#define DM816X_RTC_YEARS               0x14
#define DM816X_RTC_WEEKS               0x18
#define DM816X_RTC_ALARM_SECONDS       0x20
#define DM816X_RTC_ALARM_MINUTES       0x24
#define DM816X_RTC_ALARM_HOURS         0x28
#define DM816X_RTC_ALARM_DAYS          0x2c
#define DM816X_RTC_ALARM_MONTHS        0x30
#define DM816X_RTC_ALARM_YEARS         0x34
#define DM816X_RTC_CTRL                0x40
#define		DM816X_RTC_CTRL_RTC_DISABLE		0x40
#define		DM816X_RTC_CTRL_SET_32_CNTR		0x20
#define		DM816X_RTC_CTRL_TEST_MODE		0x10
#define		DM816X_RTC_CTRL_MODE_12_24		0x08
#define		DM816X_RTC_CTRL_AUTO_COMP		0x04
#define		DM816X_RTC_CTRL_ROUND_30S		0x02
#define		DM816X_RTC_CTRL_STOP_RTC		0x01
#define DM816X_RTC_STATUS              0x44
#define 	DM816X_RTC_STATUS_ALARM			0x40
#define 	DM816X_RTC_STATUS_1D_EVENT		0x20
#define 	DM816X_RTC_STATUS_1H_EVENT		0x10
#define 	DM816X_RTC_STATUS_1M_EVENT		0x08
#define 	DM816X_RTC_STATUS_1S_EVENT		0x04
#define 	DM816X_RTC_STATUS_RUN			0x02
#define 	DM816X_RTC_STATUS_BUSY			0x01
#define DM816X_RTC_INTERRUPTS          0x48
#define DM816X_RTC_COMP_LSB            0x4c
#define DM816X_RTC_COMP_MSB            0x50
#define DM816X_RTC_OSC                 0x54
#define 	DM816X_RTC_OSC_SWRESET			0x20
#define 	DM816X_RTC_OSC_OSC32KPWRDNR		0x10
#define 	DM816X_RTC_OSC_SWRESPROG_MASK	0x0F
#define DM816X_RTC_SCRATCH0            0x60
#define DM816X_RTC_SCRATCH1            0x64
#define DM816X_RTC_SCRATCH2            0x68
#define DM816X_RTC_KICK0R              0x6C
#define DM816X_RTC_KICK1R              0x70
#define DM816X_RTC_REVISION            0x74
#define DM816X_RTC_SYSCONFIG           0x78
#define DM816X_RTC_IRQWAKEEN_0         0x7A

#define CM_ALWON_WDTIMER_CLKCTRL             (PRCM_BASE + 0x158C)
/* WDTIMER register is RO. Cant disable it. Needs a kick */
#define CM_ALWON_CONTROL_CLKCTRL             (PRCM_BASE + 0x15C4)
#define CM_ALWON_ETH0_CLKCTRL                (PRCM_BASE + 0x15D4)
#define CM_ALWON_ETH1_CLKCTRL                (PRCM_BASE + 0x15D8)

#define CM_ALWON_L3_SLOW_CLKSTCTRL           (PRCM_BASE + 0x1400)

#define CM_ALWON_CUST_EFUSE_CLKCTRL          (PRCM_BASE + 0x1628)
#define CM_ALWON_GPIO_0_CLKCTRL              (PRCM_BASE + 0x155c)
#define CM_ALWON_GPIO_1_CLKCTRL              (PRCM_BASE + 0x1560)
#define CM_ALWON_GPIO_0_OPTFCLKEN_DBCLK      (PRCM_BASE + 0x155c)

/* Ethernet */
#define CM_ETHERNET_CLKSTCTRL                (PRCM_BASE + 0x1404)
#define CM_ALWON_ETHERNET_0_CLKCTRL          (PRCM_BASE + 0x15D4)
#define CM_ALWON_ETHERNET_1_CLKCTRL          (PRCM_BASE + 0x15D8)

#define CM_ALWON_MCASP0_CLKCTRL              (PRCM_BASE + 0x1540)
#define CM_ALWON_MCASP1_CLKCTRL              (PRCM_BASE + 0x1544)
#define CM_ALWON_MCASP2_CLKCTRL              (PRCM_BASE + 0x1548)
#define CM_ALWON_MCBSP_CLKCTRL               (PRCM_BASE + 0x154C)

/* UARTs */
#define CM_ALWON_UART_0_CLKCTRL              (PRCM_BASE + 0x1550)
#define CM_ALWON_UART_1_CLKCTRL              (PRCM_BASE + 0x1554)
#define CM_ALWON_UART_2_CLKCTRL              (PRCM_BASE + 0x1558)
#define CM_ALWON_UART_3_CLKCTRL              (PRCM_BASE + 0x1580)
#define CM_ALWON_UART_4_CLKCTRL              (PRCM_BASE + 0x1584)

/* I2C */
#define CM_ALWON_I2C_0_CLKCTRL               (PRCM_BASE + 0x1564)
#define CM_ALWON_I2C_1_CLKCTRL               (PRCM_BASE + 0x1568)

/* SPI */
#define CM_ALWON_SPI_CLKCTRL                 (PRCM_BASE + 0x1590)

/*S D/MMC */
#define CM_ALWON_SDIO_CLKCTRL                (PRCM_BASE + 0x15b0)
#define CM_ALWON_SDIO_0_CLKCTRL              (PRCM_BASE + 0x161C)
#define CM_ALWON_SDIO_1_CLKCTRL              (PRCM_BASE + 0x1620)
#define CM_ALWON_SDIO_2_CLKCTRL              (PRCM_BASE + 0x1624)

/*ATA */
#define CM_ALWON_PATA_CLKCTRL                (PRCM_BASE + 0x1578)

/*SATA */
#define CM_DEFAULT_L3_MED_CLKSTCTRL	(PRCM_BASE + 0x0504)
#define CM_ALWON2_SATA_CLKCTRL		(PRCM_BASE + 0x0560)

#define SERDES_REFCLK_CTL			(CTRL_BASE + 0x0E24)
#define		SERDES_REFCLK_CTL_PWRDN_SE	(1<<1)
#define		SERDES_REFCLK_CTL_PWRDN		(1<<0)

/* USB */
#define RM_ALWON2_RSTCTRL                    (PRCM_BASE + 0x0b10)
#define    CM_L3_SLOW_CLKSTCTRL              (PRCM_BASE + 0x0514)
#define CM_ALWON2_USB_CLKCTRL                (PRCM_BASE + 0x0558)

#define RM_ALWON2_RSTCTRL_USB_LRST           0xffffff9f
#define CM_ALWON2_USB_CLKCTRL_IDLEST         (0x3<<16)
#define CM_L3_SLOW_CLKSTCTRL_CLKACTIVE       (1<<8)

/* UART registers. FIXME: hardcoding for UART2 */
#define UART_SYSCFG                          (UART2_BASE + 0x54)
#define UART_SYSSTS                          (UART2_BASE + 0x58)
#define UART_LCR                             (UART2_BASE + 0x0C)
#define UART_EFR                             (UART2_BASE + 0x08)
#define UART_MCR                             (UART2_BASE + 0x10)
#define UART_SCR                             (UART2_BASE + 0x40)
#define UART_TCR                             (UART2_BASE + 0x18)
#define UART_FCR                             (UART2_BASE + 0x08)
#define UART_DLL                             (UART2_BASE + 0x00)
#define UART_DLH                             (UART2_BASE + 0x04)
#define UART_MDR                             (UART2_BASE + 0x20)

/* SATA registers. */
#define SATA_PLLCFG0		(CTRL_BASE + 0x0720)
#define SATA_PLLCFG1		(CTRL_BASE + 0x0724)
#define SATA_PLLCFG2		(CTRL_BASE + 0x0728)
#define SATA_PLLCFG3		(CTRL_BASE + 0x072C)
#define SATA_PLLCFG4		(CTRL_BASE + 0x0730)
#define SATA_PLLSTATUS		(CTRL_BASE + 0x0734)

#define IDLE_OFFSET		(SATA_BASE + 0x1100)

#define CFGRX0_OFFSET		(SATA_BASE + 0x1104)
#define CFGRX1_OFFSET		(SATA_BASE + 0x1108)
#define CFGRX2_OFFSET		(SATA_BASE + 0x110C)
#define CFGRX3_OFFSET		(SATA_BASE + 0x1110)
#define CFGRX4_OFFSET		(SATA_BASE + 0x1114)
#define STSRX_OFFSET		(SATA_BASE + 0x1118)

#define CFGTX0_OFFSET		(SATA_BASE + 0x111C)
#define CFGTX1_OFFSET		(SATA_BASE + 0x1120)
#define CFGTX2_OFFSET		(SATA_BASE + 0x1124)
#define CFGTX3_OFFSET		(SATA_BASE + 0x1128)
#define CFGTX4_OFFSET		(SATA_BASE + 0x112C)
#define STSTX_OFFSET		(SATA_BASE + 0x1130)

/* AHCI Registers */
#define AHCI_CAP                    (SATA_BASE+0x00)// Host Capabilities
	#define AHCI_CAP_S64A			0x80000000		// Supports 64-bit Addressing
	#define AHCI_CAP_SNCQ			0x40000000		// Supports Native Command Queuing
	#define AHCI_CAP_SIS			0x10000000		// Supports Interlock Switch
	#define AHCI_CAP_SSS			0x08000000		// Supports Staggered Spin-up
	#define AHCI_CAP_SALP			0x04000000		// Supports Aggressive Link Power Management
	#define AHCI_CAP_SAL			0x02000000		// Supports Activity LED
	#define AHCI_CAP_SCLO			0x01000000		// Supports Command List Override
	#define AHCI_CAP_ISS_MSK		0x00f00000		// Interface Speed Support
	#define AHCI_CAP_ISS( _cap )	( ( (_cap) & AHCI_CAP_ISS_MSK ) >> 20 )
	#define AHCI_CAP_SNZO			0x00080000		// Supports Non-Zero DMA Offsets
	#define AHCI_CAP_SAM			0x00040000		// Supports AHCI mode only
	#define AHCI_CAP_SPM			0x00020000		// Supports Port Multiplier
	#define AHCI_CAP_PMD			0x00008000		// PIO Multiple DRQ Block
	#define AHCI_CAP_SSC			0x00004000		// Slumber State Capable
	#define AHCI_CAP_PSC			0x00002000		// Partial State Capable
	#define AHCI_CAP_NCS( _cap )	( ( (_cap) >> 8 ) & 0x1f ) // Number of Command Slots
	#define AHCI_CAP_NP( _cap )		( (_cap) & 0x1f )
	#define AHCI_PI_MASK			0xffffffff
	#define AHCI_NP_MAX				32

#define AHCI_GHC					(SATA_BASE+0x04)// Global Host Control
	#define AHCI_GHC_AE				0x80000000		// AHCI Enable
	#define AHCI_GHC_IE				0x00000002		// Interupt Enable
	#define AHCI_GHC_HR				0x00000001		// HBA Reset

#define AHCI_IS						(SATA_BASE+0x08)// Interrupt Status
#define AHCI_PI						(SATA_BASE+0x0c)// Port Implemented
#define AHCI_VS						(SATA_BASE+0x10)// Version
#define AHCI_CCC_CTL				(SATA_BASE+0x14)// Command Completion Coalescing Control
#define AHCI_CCC_PORTS				(SATA_BASE+0x18)// Command Completion Coalescing Ports
#define AHCI_EM_LOC					(SATA_BASE+0x1c)// Enclosure Management Location
#define AHCI_EM_CTL					(SATA_BASE+0x20)// Enclosure Management Control


#define AHCI_CLB( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x100 ))
#define AHCI_CLBU( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x104 ))
#define AHCI_FB( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x108 ))
#define AHCI_FBU( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x10c ))
#define AHCI_PIS( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x110 ))
#define AHCI_PIE( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x114 ))
	#define AHCI_ISE_CPD			0x80000000	// Cold Port Detect
	#define AHCI_ISE_TFE			0x40000000	// Task FIle Error Status
	#define AHCI_ISE_HBF			0x20000000	// Host Bus Fatal Error Status
	#define AHCI_ISE_HBD			0x10000000	// Host Bus Data Error Status
	#define AHCI_ISE_IF				0x08000000	// Interface Fatal Error Status
	#define AHCI_ISE_INF			0x04000000	// Interface Non-fatal Error Status
	#define AHCI_ISE_OF				0x01000000	// Overflow Status
	#define AHCI_ISE_IPM			0x00800000	// Incorrect Port Multiplier Status
	#define AHCI_ISE_PRC			0x00400000	// PhyRdy Change Status
	#define AHCI_ISE_DI				0x00000080	// Device Interlock Status
	#define AHCI_ISE_PC				0x00000040	// Port Connect Change Status
	#define AHCI_ISE_DP				0x00000020	// Descriptor Processed
	#define AHCI_ISE_UF				0x00000010	// Unknown FIS Interrupt
	#define AHCI_ISE_SDB			0x00000008	// Set Device Bits Interrupt
	#define AHCI_ISE_DS				0x00000004	// DMA Setup FIS Interrupt
	#define AHCI_ISE_PS				0x00000002	// PIO Setup FIS Interrupt
	#define AHCI_ISE_DHR			0x00000001	// Device to Host Register FIS Interrupt
	#define AHCI_ISE_FATAL_MSK 		( AHCI_ISE_HBF | AHCI_ISE_HBD | AHCI_ISE_TFE |	\
									  AHCI_ISE_IF )
	#define AHCI_ISE_IEN			( AHCI_ISE_CPD | AHCI_ISE_TFE | AHCI_ISE_HBF |	\
							  		  AHCI_ISE_HBD | AHCI_ISE_IF | AHCI_ISE_OF |	\
									  AHCI_ISE_PRC | AHCI_ISE_PC | AHCI_ISE_DP |	\
									  AHCI_ISE_UF | AHCI_ISE_SDB | AHCI_ISE_DS |	\
						  			  AHCI_ISE_PS | AHCI_ISE_DHR )

#define AHCI_CMD( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x118 ))
	#define AHCI_CMD_ICC_MSK		0xf0000000
	//#define AHCI_CMD_ICC( _icc )	( ((_icc) >> 28) & 0xf )
	#define AHCI_CMD_SLUMBER		0x60000000
	#define AHCI_CMD_PARTIAL		0x20000000
	#define AHCI_CMD_ACTIVE			0x10000000
	#define AHCI_CMD_NOOP			0x00000000
	#define AHCI_CMD_ASP			0x08000000
	#define AHCI_CMD_ALPE			0x04000000
	#define AHCI_CMD_DLAE			0x02000000
	#define AHCI_CMD_ATAPI			0x01000000
	#define AHCI_CMD_CLO			0x00000008
	#define AHCI_CMD_POD			0x00000004
	#define AHCI_CMD_SUD			0x00000002
	#define AHCI_CMD_ST				0x00000001
	#define AHCI_CMD_CPD			0x00100000
	#define AHCI_CMD_ISP			0x00080000
	#define AHCI_CMD_HPCP			0x00040000
	#define AHCI_CMD_PMA			0x00020000
	#define AHCI_CMD_CPS			0x00010000
	#define AHCI_CMD_CR				0x00008000
	#define AHCI_CMD_FR				0x00004000
	#define AHCI_CMD_ISS			0x00002000
	#define AHCI_CMD_CCS_MSK		0x00001f00
	#define AHCI_CMD_CCS( _cmd )	( ( (_cmd) >> 8 ) & 0x1f )
	#define AHCI_CMD_FRE			0x00000010

#define AHCI_TFD( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x120 ))
	#define AHCI_TFD_STS	0
	#define AHCI_TFD_ERR	1
#define AHCI_SIG( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x124 ))
	#define AHCI_SIG_LBAH	3
	#define AHCI_SIG_LBAM	2
	#define AHCI_SIG_LBAL	1
	#define AHCI_SIG_SCNT	0
#define AHCI_SSTS( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x128 ))
#define AHCI_SCTL( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x12c ))
#define AHCI_SERR( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x130 ))
#define AHCI_SACT( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x134 ))
#define AHCI_CI( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x138 ))
#define AHCI_SNTF( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x13c ))
#define AHCI_VSP( _port )			(SATA_BASE+( ( _port ) * 0x80 + 0x170 ))


/*DMM & EMIF4 MMR Declaration*/
#define DMM_REVISION						 (DMM_BASE + 0x00)
#define DMM_LISA_MAP(n)						 (DMM_BASE + 0x40 + 4*(n))
#define DMM_LISA_MAP__0                      DMM_LISA_MAP(0)
#define DMM_LISA_MAP__1                      DMM_LISA_MAP(1)
#define DMM_LISA_MAP__2                      DMM_LISA_MAP(2)
#define DMM_LISA_MAP__3                      DMM_LISA_MAP(3)
#define DMM_LISA_LOCK						 (DMM_BASE + 0x01C)
#define DMM_PAT_BASE_ADDR                    (DMM_BASE + 0x460)

#define EMIF4_0_SDRAM_CONFIG                 (EMIF4_0_CFG_BASE + 0x08)
#define EMIF4_0_SDRAM_CONFIG2                (EMIF4_0_CFG_BASE + 0x0C)
#define EMIF4_0_SDRAM_REF_CTRL               (EMIF4_0_CFG_BASE + 0x10)
#define EMIF4_0_SDRAM_REF_CTRL_SHADOW        (EMIF4_0_CFG_BASE + 0x14)
#define EMIF4_0_SDRAM_TIM_1                  (EMIF4_0_CFG_BASE + 0x18)
#define EMIF4_0_SDRAM_TIM_1_SHADOW           (EMIF4_0_CFG_BASE + 0x1C)
#define EMIF4_0_SDRAM_TIM_2                  (EMIF4_0_CFG_BASE + 0x20)
#define EMIF4_0_SDRAM_TIM_2_SHADOW           (EMIF4_0_CFG_BASE + 0x24)
#define EMIF4_0_SDRAM_TIM_3                  (EMIF4_0_CFG_BASE + 0x28)
#define EMIF4_0_SDRAM_TIM_3_SHADOW           (EMIF4_0_CFG_BASE + 0x2C)
#define EMIF4_0_DDR_PHY_CTRL_1               (EMIF4_0_CFG_BASE + 0xE4)
#define EMIF4_0_DDR_PHY_CTRL_1_SHADOW        (EMIF4_0_CFG_BASE + 0xE8)

#define EMIF4_1_SDRAM_CONFIG                 (EMIF4_1_CFG_BASE + 0x08)
#define EMIF4_1_SDRAM_CONFIG2                (EMIF4_1_CFG_BASE + 0x0C)
#define EMIF4_1_SDRAM_REF_CTRL               (EMIF4_1_CFG_BASE + 0x10)
#define EMIF4_1_SDRAM_REF_CTRL_SHADOW        (EMIF4_1_CFG_BASE + 0x14)
#define EMIF4_1_SDRAM_TIM_1                  (EMIF4_1_CFG_BASE + 0x18)
#define EMIF4_1_SDRAM_TIM_1_SHADOW           (EMIF4_1_CFG_BASE + 0x1C)
#define EMIF4_1_SDRAM_TIM_2                  (EMIF4_1_CFG_BASE + 0x20)
#define EMIF4_1_SDRAM_TIM_2_SHADOW           (EMIF4_1_CFG_BASE + 0x24)
#define EMIF4_1_SDRAM_TIM_3                  (EMIF4_1_CFG_BASE + 0x28)
#define EMIF4_1_SDRAM_TIM_3_SHADOW           (EMIF4_1_CFG_BASE + 0x2C)
#define EMIF4_1_DDR_PHY_CTRL_1               (EMIF4_1_CFG_BASE + 0xE4)
#define EMIF4_1_DDR_PHY_CTRL_1_SHADOW        (EMIF4_1_CFG_BASE + 0xE8)

#define		EMIF4_SDRAM_CONFIG_TYPE_MASK		(0xe0000000)
#define		EMIF4_SDRAM_CONFIG_TYPE_DDR2		(0x40000000)
#define		EMIF4_SDRAM_CONFIG_TYPE_DDR3		(0x60000000)

/*EMIF4 PRCM Defintion*/
#define CM_DEFAULT_L3_FAST_CLKSTCTRL         (PRCM_BASE + 0x0508)
#define CM_DEFAULT_EMIF_0_CLKCTRL            (PRCM_BASE + 0x0520)
#define CM_DEFAULT_EMIF_1_CLKCTRL            (PRCM_BASE + 0x0524)
#define CM_DEFAULT_DMM_CLKCTRL               (PRCM_BASE + 0x0528)
#define CM_DEFAULT_FW_CLKCTRL                (PRCM_BASE + 0x052C)

/* IVAHD related register addresses */
#define CM_IVAHD0_CLKSTCTRL                  (PRCM_BASE + 0x0600)
#define CM_IVAHD0_IVAHD_CLKCTRL              (PRCM_BASE + 0x0620)
#define CM_IVAHD0_SL2_CLKCTRL                (PRCM_BASE + 0x0624)
#define PM_IVAHD0_PWRSTST                    (PRCM_BASE + 0x0C04)
#define RM_IVAHD0_RSTCTRL                    (PRCM_BASE + 0x0C10)
#define RM_IVAHD0_RSTST                      (PRCM_BASE + 0x0C14)

#define CM_IVAHD1_CLKSTCTRL                  (PRCM_BASE + 0x0700)
#define CM_IVAHD1_IVAHD_CLKCTRL              (PRCM_BASE + 0x0720)
#define CM_IVAHD1_SL2_CLKCTRL                (PRCM_BASE + 0x0724)
#define PM_IVAHD1_PWRSTST                    (PRCM_BASE + 0x0D04)
#define RM_IVAHD1_RSTCTRL                    (PRCM_BASE + 0x0D10)
#define RM_IVAHD1_RSTST                      (PRCM_BASE + 0x0D14)

#define CM_IVAHD2_CLKSTCTRL                  (PRCM_BASE + 0x0800)
#define CM_IVAHD2_IVAHD_CLKCTRL              (PRCM_BASE + 0x0820)
#define CM_IVAHD2_SL2_CLKCTRL                (PRCM_BASE + 0x0824)
#define PM_IVAHD2_PWRSTST                    (PRCM_BASE + 0x0E04)
#define RM_IVAHD2_RSTCTRL                    (PRCM_BASE + 0x0E10)
#define RM_IVAHD2_RSTST                      (PRCM_BASE + 0x0E14)

#define PRCM_IVAHD0_ICONT_RST_CNTL_ADDR      (PRCM_BASE + 0x0C10)
#define PRCM_IVAHD1_ICONT_RST_CNTL_ADDR      (PRCM_BASE + 0x0D10)
#define PRCM_IVAHD2_ICONT_RST_CNTL_ADDR      (PRCM_BASE + 0x0E10)

#define IVAHD0_ICONT1_ITCM_BASE              (IVAHD0_CONFIG_BASE + 0x08000)
#define IVAHD0_ICONT2_ITCM_BASE              (IVAHD0_CONFIG_BASE + 0x18000)
#define IVAHD1_ICONT1_ITCM_BASE              (IVAHD1_CONFIG_BASE + 0x08000)
#define IVAHD1_ICONT2_ITCM_BASE              (IVAHD1_CONFIG_BASE + 0x18000)
#define IVAHD2_ICONT1_ITCM_BASE              (IVAHD2_CONFIG_BASE + 0x08000)
#define IVAHD2_ICONT2_ITCM_BASE              (IVAHD2_CONFIG_BASE + 0x18000)

#define IVAHD_LENGTH_BOOT_CODE               14

/* Smartreflex Registers */
#define TI816X_SMRT_SCALE_ADDR               (CTRL_BASE + 0x06A0)
#define TI816X_SMRT_OPP_SVT_ADDR             (CTRL_BASE + 0x06A8)
#define TI816X_SMRT_OPP_HVT_ADDR             (CTRL_BASE + 0x06AC)


/* ALWON PRCM */
#define CM_ALWON_OCMC_0_CLKSTCTRL            (PRCM_BASE + 0x1414)
#define CM_ALWON_OCMC_1_CLKSTCTRL            (PRCM_BASE + 0x1418)
#define CM_ALWON_OCMC_0_CLKCTRL              (PRCM_BASE + 0x15B4)
#define CM_ALWON_OCMC_1_CLKCTRL              (PRCM_BASE + 0x15B8)
#define CM_ALWON_GPMC_CLKCTRL                (PRCM_BASE + 0x15D0)

/* OCMC */
#define SRAM0_SIZE                           (0x40000)
#define SRAM_GPMC_STACK_SIZE                 (0x40)

/* GPMC related */
#define GPMC_CONFIG_CS0                      (0x60)
#define GPMC_CONFIG_CS0_BASE                 (GPMC_BASE + GPMC_CONFIG_CS0)
#define GPMC_CONFIG1                         (0x00)
#define GPMC_CONFIG2                         (0x04)
#define GPMC_CONFIG3                         (0x08)
#define GPMC_CONFIG4                         (0x0C)
#define GPMC_CONFIG5                         (0x10)
#define GPMC_CONFIG6                         (0x14)
#define GPMC_CONFIG7                         (0x18)

/* PAD configuration register offsets and values for gpmc address
 * lines a12 - a26
 */

#if 0
#define TIM7_OUT       (CTRL_BASE + 0xb34)    /* a12 */
#define SC1_DATA       (CTRL_BASE + 0xb18)    /* a13 */
#define SC1_CLK        (CTRL_BASE + 0xb14)    /* a14 */
#define SC1_RST        (CTRL_BASE + 0xb10)  /* a15 */
#define SC1_DET        (CTRL_BASE + 0xb0c)  /* a16 */
#define UART0_RIN      (CTRL_BASE + 0xacc)  /* a17 */
#define UART0_DCDN     (CTRL_BASE + 0xac8)  /* a18 */
#define UART0_DSRN     (CTRL_BASE + 0xac4)  /* a19 */
#define UART0_DTRN     (CTRL_BASE + 0xac0)  /* a20 */
#define SC1_VCCEN      (CTRL_BASE + 0xb20)  /* a21 */
#define SC0_RST        (CTRL_BASE + 0xaf4)  /* a22 */
#define SC0_VCCEN      (CTRL_BASE + 0xb04)  /* a23 */
#define TIM6_OUT       (CTRL_BASE + 0xb30)  /* a24 */
#define SC0_DATA       (CTRL_BASE + 0xafc)  /* a25 */
#define GPMC_A27       (CTRL_BASE + 0xba0)  /* a27 */
#else
#define TIM7_OUT       (CTRL_BASE + 0xb34)    /* a12 */
#define UART1_CTSN     (CTRL_BASE + 0xadc)    /* a13 */
#define UART1_RTSN     (CTRL_BASE + 0xad8)    /* a14 */
#define UART2_RTSN     (CTRL_BASE + 0xae8)  /* a15 */
#define SC1_RST        (CTRL_BASE + 0xb10)  /* a15 */
#define UART2_CTSN     (CTRL_BASE + 0xaec)  /* a16 */
#define UART0_RIN      (CTRL_BASE + 0xacc)  /* a17 */
#define UART0_DCDN     (CTRL_BASE + 0xac8)  /* a18 */
#define UART0_DSRN     (CTRL_BASE + 0xac4)  /* a19 */
#define UART0_DTRN     (CTRL_BASE + 0xac0)  /* a20 */
#define SPI_SCS3       (CTRL_BASE + 0xaa4)  /* a21 */
#define SPI_SC2        (CTRL_BASE + 0xaa0)  /* a22 */
#define GPO_IO6        (CTRL_BASE + 0xca0)  /* a23 */
#define TIM6_OUT       (CTRL_BASE + 0xb30)  /* a24 */
#define SC0_DATA       (CTRL_BASE + 0xafc)  /* a25 */
#define GPMC_A27       (CTRL_BASE + 0xba0)    /* a27 */

#define GPMC_A12 TIM7_OUT
#define GPMC_A13 UART1_CTSN
#define GPMC_A14 UART1_RTSN
#define GPMC_A15 UART2_RTSN
#define GPMC_A16 UART2_CTSN
#define GPMC_A17 UART0_RIN
#define GPMC_A18 UART0_DCDN
#define GPMC_A19 UART0_DSRN
#define GPMC_A20 UART0_DTRN
#define GPMC_A21 SPI_SCS3
#define GPMC_A22 SPI_SC2
#define GPMC_A23 GPO_IO6
#define GPMC_A24 TIM6_OUT
#define GPMC_A25 SC0_DATA

#endif


#endif
