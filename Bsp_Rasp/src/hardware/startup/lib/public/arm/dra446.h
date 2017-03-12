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
 * TI Jacinto (DRA446) EVM with ARM 926 core
 */

/*
 * Device configuration
 */
#define	DRA446_CFG_BASE			0x01C48000

#define	DRA446_CFG_DSPBOOTADDR	0x08
#define	DRA446_CFG_SUSPSRC		0x0C
#define	DRA446_CFG_BOOTCFG		0x14
#define	DRA446_CFG_DEVICE_ID	0x28
#define	DRA446_CFG_VLYNQ		0x30
#define	DRA446_CFG_AUDSSMUX		0x34
#define	DRA446_CFG_SHAREAUX_CTL0	0x38
#define	DRA446_CFG_SHAREAUX_CMD0	0x3C
#define	DRA446_CFG_SHAREAUX_STAT0	0x40
#define	DRA446_CFG_DSP_INTCMD	0x80
#define	DRA446_CFG_DSP_INTSTAT	0x84
#define	DRA446_CFG_PINMUX0		0x100
#define	DRA446_CFG_PINMUX1		0x104
#define	DRA446_CFG_PINMUX2		0x108
#define	DRA446_CFG_PULLCTL		0x10C
#define	DRA446_CFG_IO_PWDN		0x118
#define	DRA446_CFG_OBSMUX		0x11C
#define	DRA446_CFG_HPI_CTL		0x124
#define	DRA446_CFG_USBPHY_CTL	0x128
#define	DRA446_CFG_VPSS_CLKCTL	0x12C
#define	DRA446_CFG_INTDMA_MUX0	0x134
#define	DRA446_CFG_MISC_CTL0	0x138
#define	DRA446_CFG_INTDMA_MUX1	0x13C
#define	DRA446_CFG_MSTPRI0		0x140
#define	DRA446_CFG_MSTPRI1		0x144
#define	DRA446_CFG_MPER_MAP		0x148
#define	DRA446_CFG_ARMAUX_CTL0	0x14C
#define	DRA446_CFG_ARMAUX_CMD0	0x150
#define	DRA446_CFG_ARMAUX_STAT0	0x154
#define	DRA446_CFG_VERR_FADR	0x160
#define	DRA446_CFG_VERR_FCMD	0x164
#define	DRA446_CFG_ARM_INTCMD	0x180
#define	DRA446_CFG_ARM_INTSTAT	0x184




/* PLL CONTROLLER */

/* TIMERS */
#define DRA446_RTI_SIZE			0x400
#define DRA446_RTI0_BASE		0x01CA2400
#define DRA446_RTI1_BASE		0x01C82800
#define DRA446_RTI2_BASE		0x01C44800

#define	DRA446_RTIGCTRL			0x00
#define	DRA446_RTICAPCTRL		0x08
#define	DRA446_RTICOMPCTRL		0x0C
#define	DRA446_RTIFRC0			0x10
#define	DRA446_RTIUC0			0x14
#define	DRA446_RTICPUC0			0x18
#define	DRA446_RTICAFRC0		0x20
#define	DRA446_RTICAUC0			0x24
#define	DRA446_RTIFRC1			0x30
#define	DRA446_RTIUC1			0x34
#define	DRA446_RTICPUC1			0x38
#define	DRA446_RTICAFRC1		0x40
#define	DRA446_RTICAUC1			0x44
#define	DRA446_RTICOMP0			0x50
#define	DRA446_RTIUDCP0			0x54
#define	DRA446_RTICOMP1			0x58
#define	DRA446_RTIUDCP1			0x5C
#define	DRA446_RTICOMP2			0x60
#define	DRA446_RTIUDCP2			0x64
#define	DRA446_RTICOMP3			0x68
#define	DRA446_RTIUDCP3			0x6C
#define	DRA446_RTISETINT		0x80
#define	DRA446_RTICLEARINT		0x84
#define	DRA446_RTIINTFLAG		0x88
#define	DRA446_RTIDWDCTRL		0x90
#define	DRA446_RTIDWDPRLD		0x94
#define	DRA446_RTIWDSTATUS		0x98
#define	DRA446_RTIWDKEY			0x9C
// #define	DRA446_RTIDWDCNTR		0xA0

/* INTERRUPT CONTROLLER */
#define DRA446_INTR_BASE		0x01CA1800
#define DRA446_INTR_FIQ0		0x00
#define DRA446_INTR_FIQ1		0x04
#define DRA446_INTR_IRQ0		0x08
#define DRA446_INTR_IRQ1		0x0C
#define DRA446_INTR_FIQENTRY	0x10
#define DRA446_INTR_IRQENTRY	0x14
#define DRA446_INTR_EINT0		0x18
#define DRA446_INTR_EINT1		0x1C
#define DRA446_INTR_INTCTL		0x20
#define DRA446_INTR_EABASE		0x24
#define DRA446_INTR_INTPRI0		0x30
#define DRA446_INTR_INTPRI1		0x34
#define DRA446_INTR_INTPRI2		0x38
#define DRA446_INTR_INTPRI3		0x3C
#define DRA446_INTR_INTPRI4		0x40
#define DRA446_INTR_INTPRI5		0x44
#define DRA446_INTR_INTPRI6		0x48
#define DRA446_INTR_INTPRI7		0x4C
#define DRA446_INTR_IRQ32		32

/* UARTs */
#define DRA446_UART_SIZE		0x40
#define DRA446_UART0_BASE		0x01C20000
#define DRA446_UART1_BASE		0x01C20800
#define DRA446_UART2_BASE		0x01C20C00
#define DRA446_UART3_BASE		0x01C20400

/* I2Cs */
#define DRA446_I2C_SIZE			0x40
#define DRA446_I2C0_BASE		0x01C21000
#define DRA446_I2C1_BASE		0x01C21400
#define DRA446_I2C2_BASE		0x01C21800
#define DRA446_I2C_ICOAR            0x00
#define DRA446_I2C_ICIMR            0x04
    #define DRA446_I2C_ICIMR_AL     (1 << 0)
    #define DRA446_I2C_ICIMR_NACK       (1 << 1)
    #define DRA446_I2C_ICIMR_ARDY       (1 << 2)
    #define DRA446_I2C_ICIMR_ICRRDY     (1 << 3)
    #define DRA446_I2C_ICIMR_ICXRDY     (1 << 4)
    #define DRA446_I2C_ICIMR_SCD        (1 << 5)
    #define DRA446_I2C_ICIMR_AAS        (1 << 6)
#define DRA446_I2C_ICSTR            0x08
    #define DRA446_I2C_ICSTR_SDIR       (1 << 14)
    #define DRA446_I2C_ICSTR_NACKSNT    (1 << 13)
    #define DRA446_I2C_ICSTR_BB     (1 << 12)
    #define DRA446_I2C_ICSTR_RSFULL     (1 << 11)
    #define DRA446_I2C_ICSTR_XSMT       (1 << 10)
    #define DRA446_I2C_ICSTR_AAS        (1 << 9)
    #define DRA446_I2C_ICSTR_AD0        (1 << 8)
    #define DRA446_I2C_ICSTR_SCD        (1 << 5)
    #define DRA446_I2C_ICSTR_ICXRDY     (1 << 4)
    #define DRA446_I2C_ICSTR_ICRRDY     (1 << 3)
    #define DRA446_I2C_ICSTR_ARDY       (1 << 2)
    #define DRA446_I2C_ICSTR_NACK       (1 << 1)
    #define DRA446_I2C_ICSTR_AL     (1 << 0)
#define DRA446_I2C_ICCLKL           0x0C
#define DRA446_I2C_ICCLKH           0x10
#define DRA446_I2C_ICCNT            0x14
#define DRA446_I2C_ICDRR            0x18
#define DRA446_I2C_ICSAR            0x1C
#define DRA446_I2C_ICDXR            0x20
#define DRA446_I2C_ICMDR            0x24
    #define DRA446_I2C_ICMDR_STT        (1 << 13)
    #define DRA446_I2C_ICMDR_STP        (1 << 11)
    #define DRA446_I2C_ICMDR_MST        (1 << 10)
    #define DRA446_I2C_ICMDR_TRX        (1 << 9)
    #define DRA446_I2C_ICMDR_XA     (1 << 8)
    #define DRA446_I2C_ICMDR_IRS        (1 << 5)
    #define DRA446_I2C_ICMDR_STB        (1 << 4)
#define DRA446_I2C_ICIVR            0x28
    #define DRA446_I2C_ICIVR_AL     0x1
    #define DRA446_I2C_ICIVR_NACK       0x2
    #define DRA446_I2C_ICIVR_ARDY       0x3
    #define DRA446_I2C_ICIVR_ICRRDY     0x4
    #define DRA446_I2C_ICIVR_ICXRDY     0x5
    #define DRA446_I2C_ICIVR_SCD        0x6
    #define DRA446_I2C_ICIVR_AAS        0x7
#define DRA446_I2C_ICEMDR           0x2C
#define DRA446_I2C_ICPSC            0x30
#define DRA446_I2C_ICPID1           0x34
#define DRA446_I2C_ICPID2           0x38

/* GPIOs */
#define DRA446_GPIO_SIZE		0x88
#define DRA446_GPIO_BASE		0x01C41000

/* Registers, offset from GPIO base */
#define	DRA446_GPIO_PID				0x00
#define	DRA446_GPIO_BINTEN			0x08
#define	DRA446_GPIO_DIR(x)			(0x10 + (x >> 1) * 0x28)
#define	DRA446_GPIO_OUT_DATA(x)		(0x14 + (x >> 1) * 0x28)
#define	DRA446_GPIO_SET_DATA(x)		(0x18 + (x >> 1) * 0x28)
#define	DRA446_GPIO_CLR_DATA(x)		(0x1C + (x >> 1) * 0x28)
#define	DRA446_GPIO_IN_DATA(x)		(0x20 + (x >> 1) * 0x28)
#define	DRA446_GPIO_SET_RIS_TRIG(x)	(0x24 + (x >> 1) * 0x28)
#define	DRA446_GPIO_CLR_RIS_TRIG(x)	(0x28 + (x >> 1) * 0x28)
#define	DRA446_GPIO_SET_FAL_TRIG(x)	(0x2C + (x >> 1) * 0x28)
#define	DRA446_GPIO_CLR_FAL_TRIG(x)	(0x30 + (x >> 1) * 0x28)
#define	DRA446_GPIO_INTSTAT(x)		(0x34 + (x >> 1) * 0x28)


/*
 * McASPs
 */
#define	DRA446_McASP_SIZE		0x800
#define	DRA446_McASP0_BASE		0x01C80000
#define	DRA446_McASP1_BASE		0x01C80800
#define	DRA446_McASP2_BASE		0x01C81000
#define	DRA446_McASP3_BASE		0x01C81800

#define	DRA446_McASP_PID		0x00
#define	DRA446_McASP_PWRDEMU	0x04
#define	DRA446_McASP_PFUNC		0x10
#define	DRA446_McASP_PDIR		0x14
#define	DRA446_McASP_PDOUT		0x18
#define	DRA446_McASP_PDIN		0x1C
#define	DRA446_McASP_PDSET		0x1C
#define	DRA446_McASP_PDCLR		0x20


/*
 * SPIs
 */
#define DRA446_SPI_SIZE			0x80
#define	DRA446_SPI0_BASE		0x01C4CC00
#define	DRA446_SPI1_BASE		0x01C24400
#define	DRA446_SPI2_BASE		0x01C24800
#define	DRA446_SPI3_BASE		0x01C24C00

/*
 * SPI registers, Offset from SPI base
 */
#define DRA446_SPI_GCR0			0x00		/* SPI Global Control Register 0 */
	#define DRA446_SPIGCR0_RESET	(1 << 0)
#define DRA446_SPI_GCR1			0x04		/* SPI Global Control Register 1 */
	#define DRA446_SPIGCR1_MASTER	(1 << 0)
	#define DRA446_SPIGCR1_CLKMOD	(1 << 1)
	#define DRA446_SPIGCR1_LOOPBACK	(1 << 16)
	#define DRA446_SPIGCR1_SPIENA	(1 << 24)
#define DRA446_SPI_INT			0x08		/* SPI Interrupt Register */
	#define DRA446_SPIINT_BITERRENA	(1 << 4)
	#define DRA446_SPIINT_OVRNINTEN	(1 << 6)
	#define DRA446_SPIINT_RXINTEN	(1 << 8)
	#define DRA446_SPIINT_DMAREQEN	(1 << 16)
#define DRA446_SPI_LVL			0x0C		/* SPI Interrupt Level Register */
#define DRA446_SPI_FLG			0x10		/* SPI Flag Register */
#define DRA446_SPI_PC0			0x14		/* SPI Pin Control Register 0 */
#define DRA446_SPI_PC1			0x18		/* SPI Pin Control Register 1 */
#define DRA446_SPI_PC2			0x1C		/* SPI Pin Control Register 2 */
#define DRA446_SPI_PC3			0x20		/* SPI Pin Control Register 3 */
#define DRA446_SPI_PC4			0x24		/* SPI Pin Control Register 4 */
#define DRA446_SPI_PC5			0x28		/* SPI Pin Control Register 5 */
#define DRA446_SPI_PC6			0x2C		/* SPI Pin Control Register 6 */
	#define DRA446_SPIPC_EN0	(1 << 0)
	#define DRA446_SPIPC_EN1	(1 << 1)
	#define DRA446_SPIPC_ENAFUN	(1 << 8)
	#define DRA446_SPIPC_CLK	(1 << 9)
	#define DRA446_SPIPC_DO		(1 << 10)
	#define DRA446_SPIPC_DI		(1 << 11)
#define DRA446_SPI_DAT0			0x38		/* SPI Shift Register 0 */
#define DRA446_SPI_DAT1			0x3C		/* SPI Shift Register 1 */
	#define	DRA446_SPIDAT1_CSHOLD	(1 << 28)
	#define	DRA446_SPIDAT1_CSNR(c)	((c) << 16)
	#define	DRA446_SPIDAT1_DFSEL(d)	((d) << 24)
#define DRA446_SPI_BUF			0x40		/* SPI Buffer Register */
	#define	DRA446_SPIBUF_RXEMPTY	(1 << 31)
	#define	DRA446_SPIBUF_RXOVR		(1 << 30)
	#define	DRA446_SPIBUF_TXFULL	(1 << 29)
	#define	DRA446_SPIBUF_BITERR	(1 << 28)
#define DRA446_SPI_EMU			0x44		/* SPI Emulation Register */
#define DRA446_SPI_DELAY		0x48		/* SPI Delay Register */
	#define	DRA446_SPIDELAY_C2TDELAY(d)	((d) << 24)
	#define	DRA446_SPIDELAY_T2CDELAY(d)	((d) << 16)
#define DRA446_SPI_DEF			0x4C		/* SPI Default Chip Select Register */
	#define	DRA446_SPIDEF_EN1DEF	(1 << 1)
	#define	DRA446_SPIDEF_EN0DEF	(1 << 0)
#define DRA446_SPI_FMT0			0x50		/* SPI Data Format Register 0 */
#define DRA446_SPI_FMT1			0x54		/* SPI Data Format Register 1 */
#define DRA446_SPI_FMT2			0x58		/* SPI Data Format Register 2 */
#define DRA446_SPI_FMT3			0x5C		/* SPI Data Format Register 3 */
	#define	DRA446_SPIFMT_CLEN(l)		((l) & 0x1F)
	#define	DRA446_SPIFMT_PRESCALE(p)	(((p) & 0xFF) << 8)
	#define	DRA446_SPIFMT_PHASE1		(1 << 16)
	#define	DRA446_SPIFMT_POLARITY1		(1 << 17)
	#define	DRA446_SPIFMT_SHIFTLSB		(1 << 20)
#define	DRA446_SPI_MIBSPIE		0x70		/* MibSPI Enable Register */

/*
 * EDMA
 */
#define DRA446_EDMA_BASE		0x01C00000
#define DRA446_EDMA_SIZE		0x5000

/*
 * EMIF
 */
#define	DRA446_EMIFA_CTRL_BASE	0x18000000
#define	DRA446_EMIFB_CTRL_BASE	0x18001000
#define	DRA446_EMIF_CTRL_SIZE	0x100

/*
 * EMIF control registers, Offset from EMIF control base
 */
#define	DRA446_EMIF_REVID		0x00
#define	DRA446_EMIF_SDCR		0x08
#define	DRA446_EMIF_SDRCR		0x0C
#define	DRA446_EMIF_SDTIMER1	0x10
#define	DRA446_EMIF_SDTIMER2	0x14
#define	DRA446_EMIF_PBBPR		0x20
#define	DRA446_EMIF_ACS2CR		0x80
#define	DRA446_EMIF_ACS3CR		0x84
#define	DRA446_EMIF_ACS4CR		0x88
#define	DRA446_EMIF_ACS5CR		0x8C
#define	DRA446_EMIF_AWCCR		0xA0

/*
 * EMIF memory map
 */
#define	DRA446_EMIFA_CS0_BASE	0x40000000
#define	DRA446_EMIFA_CS2_BASE	0x20000000
#define	DRA446_EMIFA_CS3_BASE	0x24000000
#define	DRA446_EMIFA_CS4_BASE	0x28000000
#define	DRA446_EMIFA_CS5_BASE	0x2C000000

#define	DRA446_EMIFB_CS0_BASE	0x60000000


