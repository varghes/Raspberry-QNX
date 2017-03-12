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
 * TI OMAP 1510 / 5910 processor with ARM 925 core
 *

 *
 */

/* Core Program Status Register */
#define OMAP_CPSR_MASK				0x1f
#define OMAP_CPSR_SVC_IF			0xd3

/* SDRAM Controller  */
#define OMAP_TC_BASE				0xfffecc00
#define OMAP_IMIF_PRIO				0x00
#define OMAP_OMAP_EMIFS_PRIO		0x04
#define OMAP_OMAP_EMIFF_PRIO		0x08
#define OMAP_EMIFS_CONFIG_REG		0x0c
#define OMAP_EMIFS_CS0_CONFIG		0x10
#define OMAP_EMIFS_CS1_CONFIG		0x14
#define OMAP_EMIFS_CS2_CONFIG		0x18
#define OMAP_EMIFS_CS3_CONFIG		0x1c
#define OMAP_EMIFF_SDRAM_CONFIG		0x20
#define OMAP_EMIFF_MRS				0x24
#define OMAP_TC_TIMEOUT1			0x28
#define OMAP_TC_TIMEOUT2			0x2c
#define OMAP_TC_TIMEOUT3			0x30
#define OMAP_TC_ENDIANISM			0x34
#define OMAP_EMIFF_SDRAM_CONFIG2	0x3c
#define OMAP_EMIFS_CFG_DYN_WAIT		0x40
#define OMAP_EMIFS_ABORT_ADDR		0x44
#define OMAP_EMIFS_ABORT_TYPE		0x48
#define OMAP_EMIFS_ABORT_TIMEOUT	0x4c
#define OMAP_EMIFS_ADV_CS0_CONFIG	0x50
#define OMAP_EMIFS_ADV_CS1_CONFIG	0x54
#define OMAP_EMIFS_ADV_CS2_CONFIG	0x58
#define OMAP_EMIFS_ADV_CS3_CONFIG	0x5c
#define OMAP_EMIFF_MRS_NEW			0x70
#define OMAP_EMIFF_EMRS1			0x78
#define OMAP_EMIFF_SDRAM_OP			0x80
#define OMAP_EMIFF_SDRAM_MAN_CMD	0x84
#define OMAP_DLL_URD_CTRL			0xc0
#define OMAP_DLL_LRD_CTRL			0xcc
#define OMAP_DLL_WRT_CTRL			0x64


/* SRAM - On-Chip Static RAM */
#define OMAP_SRAM_BASE				0x20000000
#define OMAP5905_SRAM_LENGTH        0x00014000 	/*  80K */
#define OMAP5910_SRAM_LENGTH        0x00030000 	/* 192K */
#define OMAP1610_SRAM_LENGTH        0x0003E800 	/* 250K */


/* interrupt controllers */
#define OMAP_INTR1_BASE				0xfffecb00
#define OMAP_INTR2_BASE				0xfffe0000
#define OMAP_INTR_SIZE				0x400
#define OMAP_INTR_ITR				0x00
#define OMAP_INTR_MIR				0x04
#define OMAP_INTR_SIR_IRQ_CODE		0x10
#define OMAP_INTR_CONTROL_REG		0x18
#define OMAP_INTR_ILR0_OFF			0x1c

/* OMAP_MPUIO */
#define OMAP_MPUIO_BASE				0xfffb5000
#define OMAP_MPUIO_INPUT_LATCH		0x00
#define OMAP_MPUIO_OUTPUT_REG		0x04
#define OMAP_MPUIO_IO_CNTL			0x08
#define OMAP_MPUIO_KBR_LATCH		0x10
#define OMAP_MPUIO_KBC_REG			0x14
#define OMAP_MPUIO_EVENT_MODE_REG	0x18
#define OMAP_MPUIO_INT_EDGE_REG		0x1c
#define OMAP_MPUIO_KBD_INT			0x20
#define OMAP_MPUIO_INT				0x24
#define OMAP_MPUIO_KBD_MASKIT		0x28
#define	OMAP_MPUIO_MASKIT			0x2c
#define OMAP_MPUIO_DEBOUNCING_REG	0x30
#define OMAP_MPUIO_LATCH_REG		0x34

/* OMAP ULPD */
#define OMAP_ULPD_REGISTER_BASE				0xfffe0800
#define ULPD_COUNTER_32_LSB_REG				0x00
#define ULPD_COUNTER_32_MSB_REG				0x04
#define ULPD_COUNTER_HIGH_FREQ_LSB_REG		0x08
#define ULPD_COUNTER_HIGH_FREQ_MSB_REG		0x0c
#define ULPD_GAUGING_CTRL_REG				0x10
#define ULPD_IT_STATUS_REG					0x14
#define ULPD_SETUP_ANALOG_CELL3_ULPD1_REG	0x24
#define ULPD_SETUP_ANALOG_CELL2_ULPD1_REG	0x28
#define ULPD_SETUP_ANALOG_CELL1_ULPD1_REG	0x2c
#define ULPD_CLOCK_CTRL_REG					0x30
#define ULPD_SOFT_REQ_REG					0x34
#define ULPD_COUNTER_32_FIQ_REG				0x38
#define ULPD_DPLL_CTRL_REG					0x3c
#define ULPD_STATUS_REQ_REG					0x40
#define ULPD_PLL_DIV_REG					0x44
#define ULPD_APLL_CTRL_REG					0x4c
#define	ULPD_POWER_CTRL_REG					0x50
#define ULPD_STATUS_REQ_REG2				0x54
#define ULPD_SLEEP_STATUS					0x58
#define ULPD_SOFT_DISABLE_REQ_REG			0x68
#define ULPD_RESET_STATUS					0x6c
#define ULPD_REVISION_NUMBER				0x70
#define ULPD_CAM_CLK_CTRL_REG				0x7c

/* Bit definitions for ULPD_CLOCK_CTRL_REG */
#define UPLD_CKCTL_DIS_USB_PVCI_CLK 		0x0020 
#define UPLD_CKCTL_USB_MCLK_EN 				0x0010
#define UPLD_CKCTL_MODEM_32K_EN 			0x0001

/* Bit definitions for ULPD_POWER_CTRL_REG */
#define	ULPD_PWRCTL_OSC_STOP_EN				0x0200
#define	ULPD_PWRCTL_LDO_CTL_EN				0x0080
#define ULPD_PWRCTL_DS_EN 					0x0010
#define UPLD_PWRCTL_LOW_PWR_EN 				0x0001

/* Bit definitions for ULPD_APLL_CTRL_REG */
#define UPLD_APLLCTL_SEL 					0x0008
#define UPLD_APLLCTL_APLL_SWITCH 			0x0001


/* OMAP_GPIO */
#define OMAP_GPIO_BASE				0xfffce000
#define GPIO_DATA_INPUT_REG			0x00
#define GPIO_DATA_OUTPUT_REG		0x04
#define GPIO_DIRECTION_CONTROL_REG	0x08
#define	GPIO_INTERRUPT_CONTROL_REG	0x0c
#define GPIO_INTERRUPT_MASK_REG		0x10
#define GPIO_INTERRUPT_STATUS_REG	0x14
#define GPIO_PIN_CONTROL_REG		0x18
#define GPIO_PIN_CONTROL_STATUS_REG	0x18

/* OMAP 1610/5912 GPIO */
#define OMAP_GPIO1_BASE				0xfffbe400
#define OMAP_GPIO2_BASE				0xfffbec00
#define OMAP_GPIO3_BASE				0xfffbb400
#define OMAP_GPIO4_BASE				0xfffbbc00
#define OMAP_GPIO_REVISION			0x00
#define OMAP_GPIO_SYSCONFIG			0x10
#define OMAP_GPIO_SYSSTATUS			0x14
#define OMAP_GPIO_IRQSTATUS1		0x18
#define OMAP_GPIO_IRQENABLE1		0x1c
#define OMAP_GPIO_IRQSTATUS2		0x20
#define OMAP_GPIO_IRQENABLE2		0x24
#define OMAP_GPIO_WAKEUPENABLE		0x28
#define OMAP_GPIO_DATAIN			0x2c
#define OMAP_GPIO_DATAOUT			0x30
#define OMAP_GPIO_DIRECTION			0x34
#define OMAP_GPIO_EDGE_CTRL1		0x38
#define OMAP_GPIO_EDGE_CTRL2		0x3c
#define OMAP_GPIO_CLEAR_IRQENABLE1	0x9c
#define OMAP_GPIO_CLEAR_IRQENABLE2	0xa4
#define OMAP_GPIO_CLEAR_WAKEUPENA	0xa8
#define OMAP_GPIO_CLEAR_DATAOUT		0xb0
#define OMAP_GPIO_SET_IRQENABLE1	0xdc
#define OMAP_GPIO_SET_IRQENABLE2	0xe4
#define OMAP_GPIO_SET_WAKEUPENA		0xe8
#define OMAP_GPIO_SET_DATAOUT		0xf0


/* Local Bus MMU Registers */
#define OMAP_LB_REG_BASE			0xfffec000
#define OMAP_LB_CLOCK_DIV			0x10c
#define OMAP_LB_MMU_CNTL_REG		0x208
#define OMAP_LB_MMU_LOCK_REG		0x224
#define OMAP_LB_MMU_LD_TLB_REG		0x228
#define OMAP_LB_MMU_CAM_H_REG		0x22C
#define OMAP_LB_MMU_CAM_L_REG		0x230
#define OMAP_LB_MMU_RAM_H_REG		0x234
#define OMAP_LB_MMU_RAM_L_REG		0x238

/* OMAP_I2Cs */
#define	OMAP_I2C_1_BASE				0xfffb3800
#define	OMAP_I2C_2_BASE				0xfffb7800
#define	OMAP_I2C_3_BASE				0xfffb8800
#define	OMAP_I2C_SIZE				0x40

#define	OMAP_I2C_REV				0x00
#define	OMAP_I2C_IE					0x04
#define	OMAP_I2C_STAT				0x08
#define	OMAP_I2C_IV					0x0c

#define	OMAP_I2C_SYSS				0x10
#define	OMAP_I2C_BUF				0x14
#define	OMAP_I2C_CNT				0x18
#define	OMAP_I2C_DATA				0x1c
#define	OMAP_I2C_SYSC				0x20

#define	OMAP_I2C_CON				0x24
#define	OMAP_I2C_OA					0x28
#define	OMAP_I2C_SA					0x2c
#define	OMAP_I2C_PSC				0x30
#define	OMAP_I2C_SCLL				0x34
#define	OMAP_I2C_SCLH				0x38
#define	OMAP_I2C_SYSTEST			0x3c

#define	OMAP_I2C_OA_MASK			0x03FF

/* Bit defination for Interrupt Enable register */
#define	OMAP_I2C_IE_GC				0x0020
#define	OMAP_I2C_IE_XRDY			0x0010
#define	OMAP_I2C_IE_RRDY			0x0008
#define	OMAP_I2C_IE_ARDY			0x0004
#define	OMAP_I2C_IE_NACK			0x0002
#define	OMAP_I2C_IE_AL				0x0001

/* Bit defination for Status register */
#define	OMAP_I2C_STAT_SBD			0x8000
#define	OMAP_I2C_STAT_BB			0x1000
#define	OMAP_I2C_STAT_ROVR			0x0800
#define	OMAP_I2C_STAT_XUDF			0x0400
#define	OMAP_I2C_STAT_AAS			0x0200
#define	OMAP_I2C_STAT_AD0			0x0100
#define	OMAP_I2C_STAT_GC			0x0020
#define	OMAP_I2C_STAT_XRDY			0x0010
#define	OMAP_I2C_STAT_RRDY			0x0008
#define	OMAP_I2C_STAT_ARDY			0x0004
#define	OMAP_I2C_STAT_NACK			0x0002
#define	OMAP_I2C_STAT_AL			0x0001

/* Bit defination for Control register */
#define	OMAP_I2C_CON_EN				0x8000
#define	OMAP_I2C_CON_BE				0x4000
#define	OMAP_I2C_CON_STB			0x0800
#define	OMAP_I2C_CON_MST			0x0400
#define	OMAP_I2C_CON_TRX			0x0200
#define	OMAP_I2C_CON_XA				0x0100
#define	OMAP_I2C_CON_RM				0x0004
#define	OMAP_I2C_CON_STP			0x0002
#define	OMAP_I2C_CON_STT			0x0001

/* Bit defination for System Status register */
#define	OMAP_I2C_SYSS_RDONE			0x0001

#define	OMAP_I2C_REVMAJOR(rev)		(((rev) >> 4) & 0xf)
#define	OMAP_I2C_REVMINOR(rev)		((rev) & 0xf)


/* OMAP_UARTs */
#define OMAP_UART_1_BASE			0xfffb0000
#define OMAP_UART_2_BASE			0xfffb0800
#define OMAP_UART_3_BASE			0xfffb9800
#define OMAP_UART_SIZE				0x54
#define OMAP_UART_RHR	 			0x00
#define OMAP_UART_THR	 			0x00
#define OMAP_UART_DLL 				0x00
#define OMAP_UART_IER 				0x04
#define OMAP_UART_DLH 				0x04
#define OMAP_UART_IIR 				0x08
#define OMAP_UART_FCR 				0x08
#define OMAP_UART_EFR 				0x08
#define OMAP_UART_LCR	 			0x0C
#define OMAP_UART_MCR 				0x10
#define OMAP_UART_XON1 				0x10
#define OMAP_UART_ADDR1				0x10
#define OMAP_UART_LSR 				0x14
#define OMAP_UART_XON2				0x14
#define OMAP_UART_ADDR2				0x14
#define OMAP_UART_MSR 				0x18
#define OMAP_UART_TCR 				0x18
#define OMAP_UART_XOFF1				0x18
#define OMAP_UART_SPR 				0x1C
#define OMAP_UART_TLR 				0x1C
#define OMAP_UART_XOFF2				0x1C
#define OMAP_UART_MDR1				0x20
#define OMAP_UART_MDR2				0x24
#define OMAP_UART_SFLSR				0x28
#define OMAP_UART_TXFLL				0x28
#define OMAP_UART_RESUME			0x2C
#define OMAP_UART_TXFLH				0x2C
#define	OMAP_UART_SFREGL			0x30
#define OMAP_UART_RXFLL				0x30
#define OMAP_UART_SFREGH			0x34
#define OMAP_UART_RXFLH				0x34
#define	OMAP_UART_BLR				0x38
#define OMAP_UART_UASR				0x38
#define	OMAP_UART_ACREG				0x3C
#define OMAP_UART_DIV1_6			0x3C
#define OMAP_UART_SCR				0x40
#define	OMAP_UART_SSR				0x44
#define OMAP_UART_EBLR				0x48
#define OMAP_UART_OSC_12M_SEL 		0x4C
#define OMAP_UART_MVR				0x50  

/* Bit definitions for interrupt identification */
#define OMAP_II_NOINTR   0x01
#define OMAP_II_MS       0x00
#define OMAP_II_TX       0x02
#define OMAP_II_RX       0x04
#define OMAP_II_RXTO     0x0C
#define OMAP_II_LS       0x06
#define OMAP_II_MASK     0x3F
#define OMAP_II_FIFO     0x80

/* Bit definitions for line control */
#define OMAP_LCR_BITS_MASK   0x03
#define OMAP_LCR_STB2        0x04
#define OMAP_LCR_PEN         0x08
#define OMAP_LCR_EPS         0x10
#define OMAP_LCR_SPS         0x20
#define OMAP_LCR_BREAK       0x40
#define OMAP_LCR_DLAB        0x80

/* Bit definitions for modem control */
#define OMAP_MCR_DTR         0x01
#define OMAP_MCR_RTS         0x02
#define OMAP_MCR_CDSTSCH     0x08
#define OMAP_MCR_LOOPBACK    0x10
#define OMAP_MCR_XON         0x20
#define OMAP_MCR_TCRTLR      0x40
#define OMAP_MCR_CLKSEL      0x80

/* Bit definitions for line status */
#define OMAP_LSR_RXRDY       0x01
#define OMAP_LSR_OE          0x02
#define OMAP_LSR_PE          0x04
#define OMAP_LSR_FE          0x08
#define OMAP_LSR_BI          0x10
#define OMAP_LSR_TXRDY       0x20
#define OMAP_LSR_TSRE        0x40
#define OMAP_LSR_RCV_FIFO    0x80

/* Bit definitions for modem status  */
#define OMAP_MSR_DCTS        0x01
#define OMAP_MSR_DDSR        0x02
#define OMAP_MSR_DRING       0x04
#define OMAP_MSR_DDCD        0x08
#define OMAP_MSR_CTS         0x10
#define OMAP_MSR_DSR         0x20
#define OMAP_MSR_RING        0x40
#define OMAP_MSR_DCD         0x80

/* Bit definitions for interrupt enable register  */
#define OMAP_IER_RHR         0x01
#define OMAP_IER_THR         0x02
#define OMAP_IER_LS          0x04
#define OMAP_IER_MS          0x08
#define OMAP_IER_SLEEP       0x10
#define OMAP_IER_XOFF        0x20
#define OMAP_IER_RTS         0x40
#define OMAP_IER_CTS         0x80

/* Bit definitions for fifo control register  */
#define OMAP_FCR_ENABLE      0x01
#define OMAP_FCR_RXCLR       0x02
#define OMAP_FCR_TXCLR       0x04
#define OMAP_FCR_DMA         0x08

/* Bit definitions for supplementary status register  */
#define OMAP_SSR_TXFULL      0x01
#define OMAP_SSR_WU_STS      0x02

/* Bit definitions for enhanced feature register  */
#define OMAP_EFR_ENHANCED    0x10
#define OMAP_EFR_AUTO_RTS    0x40
#define OMAP_EFR_AUTO_CTS    0x80

/* Mode settings for mode definition register 1  */
#define OMAP_MDR1_MODE_16X        0x00
#define OMAP_MDR1_MODE_AUTOBAUD   0x02
#define OMAP_MDR1_MODE_13X        0x03
#define OMAP_MDR1_MODE_DISABLE    0x07
#define OMAP_MDR1_MODE_MSK        0x07

#define OMAP_UART_LCR_DLAB 			0x80
#define OMAP_UART_LSR_THRE 			0x20
#define OMAP_UART_LSR_BI   			0x10
#define OMAP_UART_LSR_DR   			0x01
#define OMAP_UART_FIFO_ENABLE 		0x07

/* OMAP_FPGA interrupts */
#define OMAP_FPGA_BASE				0x08000000
#define OMAP_FPGA_INT_STAT_LO		0x06
#define OMAP_FPGA_INT_STAT_HI		0x07
#define OMAP_FPGA_INT_MASK_LO		0x08
#define OMAP_FPGA_INT_MASK_HI		0x09
#define OMAP_FPGA_PER_RESET			0x0b

/* system timers */
#define OMAP_TIMER0_BASE			0xfffec500
#define OMAP_TIMER1_BASE			0xfffec600
#define OMAP_TIMER2_BASE			0xfffec700
#define OMAP_TIMER_SIZE				0x300
#define OMAP_TIMER_CNTL				0x00
#define OMAP_LOAD_TIM				0x04
#define OMAP_READ_TIM				0x08

/* 32KHz timer */
#define OMAP_32KHZ_TIMER_BASE		0xfffb9000
#define OMAP_32KHZ_TIMER_SIZE		0x0c
#define OMAP_32KHZ_TIMER_TVR		0x00
#define OMAP_32KHZ_TIMER_TCR		0x04
#define OMAP_32KHZ_TIMER_CR			0x08

/* rtc */
#define OMAP_RTC_BASE				0xfffb4800
#define	OMAP_RTC_SIZE				0x54
#define OMAP_RTC_SECONDS			0x00
#define OMAP_RTC_MINUTES			0x04
#define OMAP_RTC_HOURS				0x08
#define OMAP_RTC_DAYS				0x0c
#define OMAP_RTC_MONTHS				0x10
#define OMAP_RTC_YEARS				0x14
#define OMAP_RTC_WEEKS				0x18
#define OMAP_RTC_ALARM_SECONDS		0x20
#define OMAP_RTC_ALARM_MINUTES		0x24
#define OMAP_RTC_ALARM_HOURS		0x28
#define OMAP_RTC_ALARM_DAYS			0x2c
#define OMAP_RTC_ALARM_MONTHS		0x30
#define OMAP_RTC_ALARM_YEARS		0x34
#define OMAP_RTC_CTRL				0x40
#define OMAP_RTC_STATUS				0x44
#define OMAP_RTC_INTERRUPTS			0x48
#define OMAP_RTC_COMP_LSB			0x4c
#define OMAP_RTC_COMP_MSB			0x50


/* watchdog timer */
#define OMAP_WDT_BASE				0xfffec800
#define OMAP_WDT_CNTL				0x00
#define OMAP_WDT_LOAD				0x04
#define OMAP_WDT_READ				0x04
#define OMAP_WDT_MODE				0x08

/* Secure Watchdog */
#define OMAP_SWDT_BASE				0xfffeb000
#define OMAP_SWDT_WIDR				0x00
#define OMAP_SWDT_WD_SYSCONFIG		0x10
#define OMAP_SWDT_WD_SYSSTATUS		0x14
#define OMAP_SWDT_WCLR				0x24
#define OMAP_SWDT_WCRR				0x28
#define OMAP_SWDT_WLDR				0x2c
#define OMAP_SWDT_WTGR				0x30
#define OMAP_SWDT_WWPS				0x34
#define OMAP_SWDT_WSPR				0x48

/* Bit definitions for SWDT_WWPS */
#define OMAP_SWDT_WWPS_WCLR 		0x01
#define OMAP_SWDT_WWPS_WCRR 		0x02
#define OMAP_SWDT_WWPS_WLDR 		0x04
#define OMAP_SWDT_WWPS_WTGR 		0x08
#define OMAP_SWDT_WWPS_WSPR 		0x10


/* clock, reset, and power mode control registers */
#define OMAP_CRPM_CTRL_BASE			0xfffece00
#define OMAP_ARM_CKCTL				0x00
#define OMAP_ARM_IDLECT1			0x04
#define OMAP_ARM_IDLECT2			0x08
#define OMAP_ARM_EWUPCT				0x0c
#define OMAP_ARM_RSTCT1				0x10
#define OMAP_ARM_RSTCT2				0x14
#define	OMAP_ARM_SYSST				0x18
#define OMAP_ARM_IDLECT3			0x24

/* Bit definitions for ARM_RSTCT1 */
#define OMAP_ARM_RSTCT1_SW_RST 		0x0008
#define OMAP_ARM_RSTCT1_DSP_RST 	0x0004
#define OMAP_ARM_RSTCT1_DSP_EN 		0x0002
#define OMAP_ARM_RSTCT1_ARM_RST 	0x0001

/* Bit definitions for ARM_RSTCT2 */
#define OMAP_ARM_RSTCT2_PER_EN 		0x0001

/* Bit definitions for ARM_CKCTL */
#define OMAP_ARM_CKCTL_EN_DSPCK 	0x2000

/* Bit definitions for ARM_IDLECT1 */
#define OMAP_ARM_IDLECT1_IDL_CLKOUT 	0x1000
#define OMAP_ARM_IDLECT1_SETARM_IDLE 	0x0800
#define OMAP_ARM_IDLECT1_WKUP_MODE 		0x0400
#define OMAP_ARM_IDLECT1_IDLTIM  		0x0200
#define OMAP_ARM_IDLECT1_IDLAPI 		0x0100
#define OMAP_ARM_IDLECT1_IDLDPLL 		0x0080
#define OMAP_ARM_IDLECT1_IDLIF   		0x0040
#define OMAP_ARM_IDLECT1_IDLPER  		0x0004
#define OMAP_ARM_IDLECT1_IDLXORP 		0x0002
#define OMAP_ARM_IDLECT1_IDLWDT  		0x0001

/* Bit definitions for ARM_IDLECT2 */
#define OMAP_ARM_IDLECT2_EN_CKOUT 		0x0800
#define OMAP_ARM_IDLECT2_EN_GPIOCK 		0x0200
#define OMAP_ARM_IDLECT2_DMACK_REQ 		0x0100
#define OMAP_ARM_IDLECT2_EN_TIMCK 		0x0080
#define OMAP_ARM_IDLECT2_EN_APICK 		0x0040
#define OMAP_ARM_IDLECT2_EN_LBCK 		0x0010
#define OMAP_ARM_IDLECT2_EN_LCDCK 		0x0008
#define OMAP_ARM_IDLECT2_EN_PERCK 		0x0004
#define OMAP_ARM_IDLECT2_EN_XORPCK 		0x0002
#define OMAP_ARM_IDLECT2_EN_WDTCK 		0x0001

/* Bit definitions for ARM_IDLECT3 */
#define OMAP_ARM_IDLECT3_EN_IDLTC2 		0x0020
#define OMAP_ARM_IDLECT3_EN_TC2_CK 		0x0010
#define OMAP_ARM_IDLECT3_EN_IDLTC1 		0x0008
#define OMAP_ARM_IDLECT3_EN_TC1_CK 		0x0004
#define OMAP_ARM_IDLECT3_EN_IDLOCPI		0x0002
#define OMAP_ARM_IDLECT3_EN_OCPI_CK		0x0001

/* PLL */
#define OMAP_PLL_CLKREF				12000000
#define OMAP_DPLL_CTL_REG			0xfffecf00

/* Bit definitions for DPLL_CTL_REG */
#define OMAP_DPLL_CTL_IOB 			0x2000
#define OMAP_DPLL_CTL_PLL_ENABLE 	0x0010
#define OMAP_DPLL_CTL_BYPASS_DIV 	0x000C
#define OMAP_DPLL_CTL_LOCK 			0x0001

/* OMAP Configuration Registers */
#define OMAP_CFG_BASE				0xfffe1000
#define OMAP_FUNC_MUX_CTRL_0		0x00
#define OMAP_FUNC_MUX_CTRL_1		0x04
#define OMAP_FUNC_MUX_CTRL_2		0x08
#define OMAP_COMP_MODE_CTRL_0		0x0c
#define OMAP_FUNC_MUX_CTRL_3		0x10
#define OMAP_FUNC_MUX_CTRL_4		0x14
#define OMAP_FUNC_MUX_CTRL_5		0x18
#define OMAP_FUNC_MUX_CTRL_6		0x1c
#define OMAP_FUNC_MUX_CTRL_7		0x20
#define OMAP_FUNC_MUX_CTRL_8		0x24
#define OMAP_FUNC_MUX_CTRL_9		0x28
#define OMAP_FUNC_MUX_CTRL_A		0x2c
#define OMAP_FUNC_MUX_CTRL_B		0x30
#define OMAP_FUNC_MUX_CTRL_C		0x34
#define OMAP_FUNC_MUX_CTRL_D		0x38
#define OMAP_PULL_DWN_CTRL_0		0x40
#define OMAP_PULL_DWN_CTRL_1		0x44
#define OMAP_PULL_DWN_CTRL_2		0x48
#define OMAP_PULL_DWN_CTRL_3		0x4c
#define OMAP_GATE_INH_CTRL_0		0x50
#define OMAP_VOLTAGE_CTRL_0			0x60
#define OMAP_USB_TRANSCEIVER_CTRL	0x64
#define OMAP_TEST_DBG_CTRL_0		0x70
#define OMAP_MOD_CONF_CTRL_0		0x80
#define OMAP_FUNC_MUX_CTRL_E		0x90
#define OMAP_FUNC_MUX_CTRL_F		0x94
#define OMAP_FUNC_MUX_CTRL_10		0x98
#define OMAP1610_RESET_CONTROL 		0x140  

/* Configuration Registers of OMAP5905 are different */
#define OMAP5905_MUXCR0				0x00
#define OMAP5905_MUXCR1				0x04
#define OMAP5905_MUXCR2				0x08
#define OMAP5905_DSPDMACR			0x0c
#define OMAP5905_SPDIFCR			0x10
#define OMAP5905_MODECR				0x14
#define OMAP5905_PULLCR				0x18
#define OMAP5905_CONFCR				0x1c
#define OMAP5905_SECCR				0x20
#define OMAP5905_CONFSR				0x24
#define OMAP5905_CONFREV			0x28
#define OMAP5905_LDOCR				0x2c
#define OMAP5905_TESTDBGCR			0x30

/* Bit definitions for MUXCR1 Register */
#define OMAP5905_MUXCR1_SCI1GPIO2	0x0100000
#define OMAP5905_MUXCR1_HECC2SPI3	0x4000000

/* Split Power Logic Registers */
#define	OMAP5905_SPL_WAKENCR		0x40
#define	OMAP5905_SPL_WAKSR			0x44
#define	OMAP5905_SPL_WAKCNT			0x48
#define	OMAP5905_SPL_PINCR1			0x4C
#define	OMAP5905_SPL_PINCR2			0x50
#define	OMAP5905_SPL_GPIO1MUXCR		0x54
#define	OMAP5905_SPL_SPSCR			0x58
#define	OMAP5905_SPL_ULPDCNT		0x60


/* Bit definitions for RESET_CONTROL */
#define OMAP1610_RESET_CTL_RNGIDLE 	0x40

/* Bit definitions for FUNC_MUX_CTRL_7 */
#define OMAP_FMCTL_7_ARMIO5 		0x7000
#define OMAP_FMCTL_7_ARMIO5_LOW_PWR 0x1000  /* Select low_pwr signal on armio_5 */


/* OMAP DSP Clock/Reset/Power Mode Control Registers */
#define OMAP_DSP_CTRL_BASE			0xe1008000
#define	OMAP_DSP_CKCTL				0x00
#define	OMAP_DSP_IDLECT1			0x04
#define	OMAP_DSP_IDLECT2			0x08
#define	OMAP_DSP_RSTCT2				0x14
#define	OMAP_DSP_SYSST				0x18

/* Bit definitions for DSP_IDLECT1 */
#define OMAP_DSP_IDLECT1_IDLTIM		0x0100
#define OMAP_DSP_IDLECT1_WKUP_MODE	0x0040
#define OMAP_DSP_IDLECT1_IDLDPLL	0x0020
#define OMAP_DSP_IDLECT1_IDLIF 		0x0010
#define OMAP_DSP_IDLECT1_IDLPER 	0x0004
#define OMAP_DSP_IDLECT1_IDLXORP	0x0002
#define OMAP_DSP_IDLECT1_IDLWDT		0x0001

/* Bit definitions for DSP_IDLECT2 */
#define OMAP_DSP_IDLECT2_EN_TIMCK 	0x0020
#define OMAP_DSP_IDLECT2_EN_PERCK 	0x0004
#define OMAP_DSP_IDLECT2_EN_XORPCK	0x0002
#define OMAP_DSP_IDLECT2_EN_WDTCK	0x0001

/* USB OTG */
#define OMAP_OTG_BASE				0xfffb0400
#define OTG_REV						0x00
#define OTG_SYSCON_1				0x04
#define OTG_SYSCON_2				0x08
#define OTG_CTRL					0x0c
#define OTG_IRQ_EN					0x10
#define OTG_IRQ_SRC					0x14
#define OTG_VC						0xfc

/* Bit definitions for OTG_SYSCON_2 */
#define OTG_SYSCON_2_UHOST_EN 		0x100


/* USB */
#define OMAP1610_USB_BASE 			0xfffb4000
#define OMAP1610_USB_SYSCON1 		0x18
#define OMAP1610_USB_SYSCON2 		0x1C

/* Bit definitions for USB_SYSCON1 */
#define OMAP1610_USB_SYSCON1_SOFF_DIS 	0x02

/* OCPI */
#define OMAP_OCPI_BASE				0xfffec320
#define OCPI_ADDR_FAULT				0x00
#define OCPI_MASTER_CMD_FAULT		0x04
#define OCPI_SINT_0					0x08
#define OCPI_ABORT_TYPE				0x0c
#define OCPI_SINT_1					0x10
#define OCPI_PROT					0x14
#define OCPI_SEC_MODE				0x18

/* OMAP5905 CAN */
#define OMAP5905_CAN1_REG_BASE 		0xfffba800
#define OMAP5905_CAN2_REG_BASE 		0xfffbc000
#define OMAP5905_CAN1_MEM_BASE 		0xfffbac00
#define OMAP5905_CAN2_MEM_BASE 		0xfffbc400
#define OMAP5905_CAN_REG_SIZE_HECC	0x80
#define OMAP5905_CAN_MEM_SIZE_HECC	0x200
#define OMAP5905_CAN_MEM_SIZE_SCC	0x100
#define OMAP5905_CANLAM_MEM_SIZE	0x80 
#define OMAP5905_CANMOTS_MEM_SIZE	0x80
#define OMAP5905_CANMOT0_MEM_SIZE	0x80
#define OMAP5905_CAN_NUM_MAILBOX_SCC	16
#define OMAP5905_CAN_NUM_MAILBOX_HECC	32	
#define OMAP5905_CAN_CLK_12MHZ 		12000000
#define OMAP5905_CAN_CLK_48MHZ 		48000000

/* OMAP5905 CAN Register Offsets */
#define OMAP5905_CANME 				0x00
#define OMAP5905_CANMD 				0x04
#define OMAP5905_CANTRS				0x08
#define OMAP5905_CANTRR		 		0x0C
#define OMAP5905_CANTA 				0x10
#define OMAP5905_CANAA 				0x14
#define OMAP5905_CANRMP 			0x18
#define OMAP5905_CANRML 			0x1C
#define OMAP5905_CANRFP 			0x20
#define OMAP5905_CANGAM				0x24 
#define OMAP5905_CANMC	 			0x28
#define OMAP5905_CANBTC 			0x2C
#define OMAP5905_CANES 				0x30
#define OMAP5905_CANTEC 			0x34
#define OMAP5905_CANREC 			0x38
#define OMAP5905_CANGIF0 			0x3C
#define OMAP5905_CANGIM 			0x40
#define OMAP5905_CANGIF1 			0x44
#define OMAP5905_CANMIM 			0x48
#define OMAP5905_CANMIL 			0x4C
#define OMAP5905_CANOPC 			0x50
#define OMAP5905_CANTIOC 			0x54
#define OMAP5905_CANRIOC			0x58
#define OMAP5905_CANLNT				0x5C
#define OMAP5905_CANTOC 			0x60
#define OMAP5905_CANTOS				0x64
#define OMAP5905_CANLAM				0x80 
#define OMAP5905_CANLAM0_SCC		0x80 /* SCC only */
#define OMAP5905_CANLAM3_SCC		0x8C /* SCC only */
#define OMAP5905_CANMOTS			0x100
#define OMAP5905_CANMOTO			0x180

/* Bit definitions for OMAP5905 CAN Master and Control (CANMC) Register */
#define OMAP5905_CANMC_MBNR_MASK	0x0000001F
#define OMAP5905_CANMC_SRES			0x00000020
#define OMAP5905_CANMC_STM			0x00000040
#define OMAP5905_CANMC_ABO			0x00000080
#define OMAP5905_CANMC_CDR			0x00000100
#define OMAP5905_CANMC_WUBA			0x00000200
#define OMAP5905_CANMC_DBO			0x00000400
#define OMAP5905_CANMC_PDR			0x00000800
#define OMAP5905_CANMC_CCR			0x00001000
#define OMAP5905_CANMC_SCM			0x00002000
#define OMAP5905_CANMC_LNTM			0x00004000
#define OMAP5905_CANMC_LNTC			0x00008000

/* Bit definitions for OMAP5905 CAN Error and Status (CANES) Register */
#define OMAP5905_CANES_TM			0x00000001
#define OMAP5905_CANES_RM			0x00000002
#define OMAP5905_CANES_PDA			0x00000008
#define OMAP5905_CANES_CCE			0x00000010
#define OMAP5905_CANES_SMA			0x00000020
#define OMAP5905_CANES_EW			0x00010000
#define OMAP5905_CANES_EP			0x00020000
#define OMAP5905_CANES_BO			0x00040000
#define OMAP5905_CANES_ACKE			0x00080000
#define OMAP5905_CANES_SE			0x00100000
#define OMAP5905_CANES_CRCE			0x00200000
#define OMAP5905_CANES_SA1			0x00400000
#define OMAP5905_CANES_BE			0x00800000
#define OMAP5905_CANES_FE			0x01000000

/* Bit definitions for OMAP5905 CAN Global Interrupt Mask (CANGIM) Register */
#define OMAP5905_CANGIM_I0EN		0x00000001
#define OMAP5905_CANGIM_I1EN		0x00000002
#define OMAP5905_CANGIM_SIL			0x00000004
#define OMAP5905_CANGIM_WLIM		0x00000100
#define OMAP5905_CANGIM_EPIM		0x00000200
#define OMAP5905_CANGIM_BOIM		0x00000400
#define OMAP5905_CANGIM_RMLIM		0x00000800
#define OMAP5905_CANGIM_WUIM		0x00001000
#define OMAP5905_CANGIM_WDIM		0x00002000
#define OMAP5905_CANGIM_AAIM		0x00004000
#define OMAP5905_CANGIM_TCOIM		0x00010000
#define OMAP5905_CANGIM_MAIM		0x00020000

/* Bit definitions for OMAP5905 CAN Global Interrupt Flag (CANGIF) Register */
#define OMAP5905_CANGIF_MIV_HECC	0x0000001F
#define OMAP5905_CANGIF_MIV_SCC		0x0000000F
#define OMAP5905_CANGIF_WLIF		0x00000100
#define OMAP5905_CANGIF_EPIF		0x00000200
#define OMAP5905_CANGIF_BOIF		0x00000400
#define OMAP5905_CANGIF_RMLIF		0x00000800
#define OMAP5905_CANGIF_WUIF		0x00001000
#define OMAP5905_CANGIF_WDIF		0x00002000
#define OMAP5905_CANGIF_AAIF		0x00004000
#define OMAP5905_CANGIF_GMIF		0x00008000
#define OMAP5905_CANGIF_TCOIF		0x00010000
#define OMAP5905_CANGIF_MAIF		0x00020000

/* Bit definitions for OMAP5905 CAN Transmit I/O Control (CANTIOC) Register */
#define OMAP5905_CANTIOC_TXIN		0x00000001
#define OMAP5905_CANTIOC_TXOUT		0x00000002
#define OMAP5905_CANTIOC_TXDIR		0x00000004
#define OMAP5905_CANTIOC_TXFUNC		0x00000008

/* Bit definitions for OMAP5905 CAN Transmit I/O Control (CANRIOC) Register */
#define OMAP5905_CANRIOC_RXIN		0x00000001
#define OMAP5905_CANRIOC_RXOUT		0x00000002
#define OMAP5905_CANRIOC_RXDIR		0x00000004
#define OMAP5905_CANRIOC_RXFUNC		0x00000008

/* Bit definitions for OMAP5905 CAN Bit-timing Configuration (CANBTC) Register */
#define OMAP5905_CANBTC_TSEG2_MASK		0x00000007
#define OMAP5905_CANBTC_TSEG1_MASK		0x00000078
#define OMAP5905_CANBTC_SAM				0x00000080
#define OMAP5905_CANBTC_SJW_MASK		0x00000300
#define OMAP5905_CANBTC_ERM				0x00000400
#define OMAP5905_CANBTC_BRP_MASK		0x00FF0000
#define OMAP5905_CANBTC_BRP_MAXVAL		0xFF
#define OMAP5905_CANBTC_BRP_SHIFT		16	
#define OMAP5905_CANBTC_SJW_MAXVAL		0x3
#define OMAP5905_CANBTC_SJW_SHIFT		8	
#define OMAP5905_CANBTC_TSEG2_MAXVAL	0x7
#define OMAP5905_CANBTC_TSEG2_SHIFT		0	
#define OMAP5905_CANBTC_TSEG1_MAXVAL	0xF
#define OMAP5905_CANBTC_TSEG1_SHIFT		3	
#define OMAP5905_CANBTC_DECREMENT		1			/* All values stored in CANBTC must be adjusted */
#define OMAP5905_CANBTC_SAM_BRP_MIN		5			/* Minimum BRP value before SAM can be enabled */
#define OMAP5905_CAN_BITRATE_MAX		100000000 	/* 1Mbps max bitrate, depending on bus and transceiver */

/* Bit definitions for OMAP5905 CAN Message Control Field (CANMCF) Register */
#define OMAP5905_CANMCF_DLC_BYTE0	0x0
#define OMAP5905_CANMCF_DLC_BYTE1	0x1
#define OMAP5905_CANMCF_DLC_BYTE2	0x2
#define OMAP5905_CANMCF_DLC_BYTE3	0x3
#define OMAP5905_CANMCF_DLC_BYTE4	0x4
#define OMAP5905_CANMCF_DLC_BYTE5	0x5
#define OMAP5905_CANMCF_DLC_BYTE6	0x6
#define OMAP5905_CANMCF_DLC_BYTE7	0x7
#define OMAP5905_CANMCF_DLC_BYTE8	0x8
#define OMAP5905_CANMCF_RTR			0x00000010
#define OMAP5905_CANMCF_TPL_SHIFT	8			/* Bit 8 */
#define OMAP5905_CANMCF_TPL_MAXVAL	0x3F
#define OMAP5905_CANMCF_TPL_MASK	0x00003F00

/* Bit definitions for OMAP5905 CAN Message Identifier (CANMID) Register */
#define OMAP5905_CANMID_MASK_STD	0x1FFC0000
#define OMAP5905_CANMID_MASK_EXT	0x1FFFFFFF
#define OMAP5905_CANMID_AAM			0x20000000
#define OMAP5905_CANMID_AME			0x40000000
#define OMAP5905_CANMID_IDE			0x80000000

/* Bit definitions for OMAP5905 CAN Local Acceptance Mask (CANLAM) Register */
#define OMAP5905_CANLAM_LAMI 		0x80000000
#define OMAP5905_CANLAM_MASK		0x1FFFFFFF




/* __SRCVERSION("omap.h $Rev: 657836 $"); */
