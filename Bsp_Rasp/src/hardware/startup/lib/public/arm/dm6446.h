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
 * TI Davinci (DM6446) EVM with ARM 926 core
 */

/* PLL CONTROLLER */
#define DM6446_PLL_BASE				0x01C40800
#define DM6446_PLL_PLLDIV2			0x11C
#define DM6446_PLL_PLLM				0x110
#define DM6446_PLL_INPUT_CLK		27000000

/* TIMERS */
#define DM6446_TIMER_SIZE			0x30
#define DM6446_TMR_1_REGS			0x01C21400
#define DM6446_TMR_2_REGS			0x01C21400
#define DM6446_TMR_3_REGS			0x01C21800
#define DM6446_TMR_4_REGS			0x01C21800
#define DM6446_WDT_1_REGS			0x01C21C00
#define DM6446_TMR_PID12			0x00
#define DM6446_TMR_EMUMGT			0x04
#define DM6446_TMR_TIM12			0x10
#define DM6446_TMR_TIM34			0x14
#define DM6446_TMR_PRD12			0x18
#define DM6446_TMR_PRD34			0x1C
#define DM6446_TMR_TCR				0x20
	#define DM6446_TCR_ENA34		(0x2 << 22)
	#define DM6446_TCR_ENA12		(0x2 << 6)
#define DM6446_TMR_TGCR				0x24
	#define DM6446_TGCR_TIM34RS		0x2
	#define DM6446_TGCR_TIM12RS		0x1
	#define DM6446_TGCR_TIMMODE		(0x1 << 2)
#define DM6446_TMR_WDTCR			0x28

/* INTERRUPT CONTROLLER */
#define DM6446_INTR_BASE			0x01C48000
#define DM6446_INTR_FIQ0			0x00
#define DM6446_INTR_FIQ1			0x04
#define DM6446_INTR_IRQ0			0x08
#define DM6446_INTR_IRQ1			0x0C
#define DM6446_INTR_FIQENTRY			0x10
#define DM6446_INTR_IRQENTRY			0x14
#define DM6446_INTR_EINT0			0x18
#define DM6446_INTR_EINT1			0x1C
#define DM6446_INTR_INTCTL			0x20
#define DM6446_INTR_EABASE			0x24
#define DM6446_INTR_INTPRI0			0x30
#define DM6446_INTR_INTPRI1			0x34
#define DM6446_INTR_INTPRI2			0x38
#define DM6446_INTR_INTPRI3			0x3C
#define DM6446_INTR_INTPRI4			0x40
#define DM6446_INTR_INTPRI5			0x44
#define DM6446_INTR_INTPRI6			0x48
#define DM6446_INTR_INTPRI7			0x4C
#define DM6446_INTR_IRQ32			32

/* I2C */
#define DM6446_I2C_BASE				0x01c21000
#define DM6446_I2C_SIZE				0x3C
#define DM6446_I2C_ICOAR			0x00
#define DM6446_I2C_ICIMR			0x04
	#define DM6446_I2C_ICIMR_AL		(1 << 0)
	#define DM6446_I2C_ICIMR_NACK		(1 << 1)
	#define DM6446_I2C_ICIMR_ARDY		(1 << 2)
	#define DM6446_I2C_ICIMR_ICRRDY		(1 << 3)
	#define DM6446_I2C_ICIMR_ICXRDY		(1 << 4)
	#define DM6446_I2C_ICIMR_SCD		(1 << 5)
	#define DM6446_I2C_ICIMR_AAS		(1 << 6)
#define DM6446_I2C_ICSTR			0x08
	#define DM6446_I2C_ICSTR_SDIR		(1 << 14)
	#define DM6446_I2C_ICSTR_NACKSNT	(1 << 13)
	#define DM6446_I2C_ICSTR_BB		(1 << 12)
	#define DM6446_I2C_ICSTR_RSFULL		(1 << 11)
	#define DM6446_I2C_ICSTR_XSMT		(1 << 10)
	#define DM6446_I2C_ICSTR_AAS		(1 << 9)
	#define DM6446_I2C_ICSTR_AD0		(1 << 8)
	#define DM6446_I2C_ICSTR_SCD		(1 << 5)
	#define DM6446_I2C_ICSTR_ICXRDY		(1 << 4)
	#define DM6446_I2C_ICSTR_ICRRDY		(1 << 3)
	#define DM6446_I2C_ICSTR_ARDY		(1 << 2)
	#define DM6446_I2C_ICSTR_NACK		(1 << 1)
	#define DM6446_I2C_ICSTR_AL		(1 << 0)
#define DM6446_I2C_ICCLKL			0x0C
#define DM6446_I2C_ICCLKH			0x10
#define DM6446_I2C_ICCNT			0x14
#define DM6446_I2C_ICDRR			0x18
#define DM6446_I2C_ICSAR			0x1C
#define DM6446_I2C_ICDXR			0x20
#define DM6446_I2C_ICMDR			0x24
	#define DM6446_I2C_ICMDR_STT		(1 << 13)
	#define DM6446_I2C_ICMDR_STP		(1 << 11)
	#define DM6446_I2C_ICMDR_MST		(1 << 10)
	#define DM6446_I2C_ICMDR_TRX		(1 << 9)
	#define DM6446_I2C_ICMDR_XA		(1 << 8)
	#define DM6446_I2C_ICMDR_IRS		(1 << 5)
	#define DM6446_I2C_ICMDR_STB		(1 << 4)
#define DM6446_I2C_ICIVR			0x28
	#define DM6446_I2C_ICIVR_AL		0x1
	#define DM6446_I2C_ICIVR_NACK		0x2
	#define DM6446_I2C_ICIVR_ARDY		0x3
	#define DM6446_I2C_ICIVR_ICRRDY		0x4
	#define DM6446_I2C_ICIVR_ICXRDY		0x5
	#define DM6446_I2C_ICIVR_SCD		0x6
	#define DM6446_I2C_ICIVR_AAS		0x7
#define DM6446_I2C_ICEMDR			0x2C
#define DM6446_I2C_ICPSC			0x30
#define DM6446_I2C_ICPID1			0x34
#define DM6446_I2C_ICPID2			0x38

/*
 * SPI
 */
#define DM6446_SPI_BASE			0x01C66800
#define DM6446_SPI_SIZE			0x80

/*
 * SPI registers, Offset from SPI base
 */
#define DM6446_SPI_GCR0			0x00		/* SPI Global Control Register 0 */
	#define DM6446_SPIGCR0_RESET	(1 << 0)
#define DM6446_SPI_GCR1			0x04		/* SPI Global Control Register 1 */
	#define DM6446_SPIGCR1_MASTER	(1 << 0)
	#define DM6446_SPIGCR1_CLKMOD	(1 << 1)
	#define DM6446_SPIGCR1_LOOPBACK	(1 << 16)
	#define DM6446_SPIGCR1_SPIENA	(1 << 24)
#define DM6446_SPI_INT			0x08		/* SPI Interrupt Register */
	#define DM6446_SPIINT_BITERRENA	(1 << 4)
	#define DM6446_SPIINT_OVRNINTEN	(1 << 6)
	#define DM6446_SPIINT_RXINTEN	(1 << 8)
	#define DM6446_SPIINT_DMAREQEN	(1 << 16)
#define DM6446_SPI_LVL			0x0C		/* SPI Interrupt Level Register */
#define DM6446_SPI_FLG			0x10		/* SPI Flag Register */
#define DM6446_SPI_PC0			0x14		/* SPI Pin Control Register 0 */
#define DM6446_SPI_PC2			0x1C		/* SPI Pin Control Register 2 */
	#define DM6446_SPIPC_EN0	(1 << 0)
	#define DM6446_SPIPC_EN1	(1 << 1)
	#define DM6446_SPIPC_CLK	(1 << 9)
	#define DM6446_SPIPC_DO		(1 << 10)
	#define DM6446_SPIPC_DI		(1 << 11)
#define DM6446_SPI_DAT1			0x3C		/* SPI Shift Register */
	#define	DM6446_SPIDAT1_CSHOLD	(1 << 28)
	#define	DM6446_SPIDAT1_CSNR(c)	((c) << 16)
	#define	DM6446_SPIDAT1_DFSEL(d)	((d) << 24)
#define DM6446_SPI_BUF			0x40		/* SPI Buffer Register */
	#define	DM6446_SPIBUF_RXEMPTY	(1 << 31)
	#define	DM6446_SPIBUF_RXOVR		(1 << 30)
	#define	DM6446_SPIBUF_TXFULL	(1 << 29)
	#define	DM6446_SPIBUF_BITERR	(1 << 28)
#define DM6446_SPI_EMU			0x44		/* SPI Emulation Register */
#define DM6446_SPI_DELAY		0x48		/* SPI Delay Register */
	#define	DM6446_SPIDELAY_C2TDELAY(d)	((d) << 24)
	#define	DM6446_SPIDELAY_T2CDELAY(d)	((d) << 16)
#define DM6446_SPI_DEF			0x4C		/* SPI Default Chip Select Register */
	#define	DM6446_SPIDEF_EN1DEF	(1 << 1)
	#define	DM6446_SPIDEF_EN0DEF	(1 << 0)
#define DM6446_SPI_FMT0			0x50		/* SPI Data Format Register 0 */
#define DM6446_SPI_FMT1			0x54		/* SPI Data Format Register 1 */
#define DM6446_SPI_FMT2			0x58		/* SPI Data Format Register 2 */
#define DM6446_SPI_FMT3			0x5C		/* SPI Data Format Register 3 */
	#define	DM6446_SPIFMT_CLEN(l)		((l) & 0x1F)
	#define	DM6446_SPIFMT_PRESCALE(p)	(((p) & 0xFF) << 8)
	#define	DM6446_SPIFMT_PHASE1		(1 << 16)
	#define	DM6446_SPIFMT_POLARITY1		(1 << 17)
	#define	DM6446_SPIFMT_SHIFTLSB		(1 << 20)


/*
 * EDMA
 */
#define DM6446_EDMA_BASE		0x01C00000
#define DM6446_EDMA_SIZE		0x5000

/*
 * Registers, Offset from EDMA base
 */
#define	DM6446_EDMA_PID			0x00
#define	DM6446_EDMA_CCCFG		0x04

/* Global registers, offset from EDMA base */
#define	DM6446_EDMA_QCHMAP(c)	(0x200 + (c) * 4)
#define	DM6446_EDMA_DMAQNUM(c)	(0x240 + (c) * 4)
#define	DM6446_EDMA_QDMAQNUM	0x260
#define	DM6446_EDMA_QUEPRI		0x284
#define	DM6446_EDMA_EMR			0x300
#define	DM6446_EDMA_EMRH		0x304
#define	DM6446_EDMA_EMCR		0x308
#define	DM6446_EDMA_EMCRH		0x30C
#define	DM6446_EDMA_QEMR		0x310
#define	DM6446_EDMA_QEMCR		0x314
#define	DM6446_EDMA_CCERR		0x318
#define	DM6446_EDMA_CCERRCLR	0x31C
#define	DM6446_EDMA_EEVAL		0x320
#define	DM6446_EDMA_DRAE(c)		(0x340 + (c) * 8)
#define	DM6446_EDMA_DRAEH(c)	(0x344 + (c) * 8)
#define	DM6446_EDMA_QRAE(c)		(0x380 + (c) * 4)
#define	DM6446_EDMA_QRAE(c)		(0x380 + (c) * 4)
#define	DM6446_EDMA_Q0E(c)		(0x400 + (c) * 4)
#define	DM6446_EDMA_Q1E(c)		(0x440 + (c) * 4)
#define	DM6446_EDMA_QSTAT0		0x600
#define	DM6446_EDMA_QSTAT1		0x604
#define	DM6446_EDMA_QWMTHRA		0x620
#define	DM6446_EDMA_CCSTAT		0x640

/* Channel registers, offset from EDMA base */
#define	DM6446_EDMA_GLOBAL		0x1000
#define	DM6446_EDMA_REGION0		0x2000
#define	DM6446_EDMA_REGION1		0x2200
#define	DM6446_EDMA_REGION2		0x2400
#define	DM6446_EDMA_REGION3		0x2600
	#define	DM6446_EDMA_ER		0x00
	#define	DM6446_EDMA_ERH		0x04
	#define	DM6446_EDMA_ECR		0x08
	#define	DM6446_EDMA_ECRH	0x0C
	#define	DM6446_EDMA_ESR		0x10
	#define	DM6446_EDMA_ESRH	0x14
	#define	DM6446_EDMA_CER		0x18
	#define	DM6446_EDMA_CERH	0x1C
	#define	DM6446_EDMA_EER		0x20
	#define	DM6446_EDMA_EERH	0x24
	#define	DM6446_EDMA_EECR	0x28
	#define	DM6446_EDMA_EECRH	0x2C
	#define	DM6446_EDMA_EESR	0x30
	#define	DM6446_EDMA_EESRH	0x34
	#define	DM6446_EDMA_SER		0x38
	#define	DM6446_EDMA_SERH	0x3C
	#define	DM6446_EDMA_SECR	0x40
	#define	DM6446_EDMA_SECRH	0x44
	#define	DM6446_EDMA_IER		0x50
	#define	DM6446_EDMA_IERH	0x54
	#define	DM6446_EDMA_IECR	0x58
	#define	DM6446_EDMA_IECRH	0x5C
	#define	DM6446_EDMA_IESR	0x60
	#define	DM6446_EDMA_IESRH	0x64
	#define	DM6446_EDMA_IPR		0x68
	#define	DM6446_EDMA_IPRH	0x6C
	#define	DM6446_EDMA_ICR		0x70
	#define	DM6446_EDMA_ICRH	0x74
	#define	DM6446_EDMA_IEVAL	0x78
	#define	DM6446_EDMA_QER		0x80
	#define	DM6446_EDMA_QEER	0x84
	#define	DM6446_EDMA_QEECR	0x88
	#define	DM6446_EDMA_QEESR	0x8C
	#define	DM6446_EDMA_QSER	0x90
	#define	DM6446_EDMA_QSECR	0x94

/* Parameter RAM base, offser from EDMA base */
#define	DM6446_EDMA_PARAM_BASE	0x4000
