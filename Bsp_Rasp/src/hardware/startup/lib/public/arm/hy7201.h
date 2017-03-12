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
 * Hyundai GMS30C7201 processor
 *

 *
 */

#ifndef	__ARM_HY7201_H_INCLUDED
#define	__ARM_HY7201_H_INCLUDED

/*
 * -------------------------------------------------------------------------
 * Power Manager/PLL
 * -------------------------------------------------------------------------
 */
#define	HY7201_PMU_BASE		0x80001000		/* physical address */
#define	HY7201_PMU_SIZE		0x0000003c

/*
 * Offsets from HY7201_PMU_BASE
 */
#define	HY7201_PMU_PMUMODE			0x00	/* power mode */
#define	HY7201_PMU_PICCOLO			0x08	/* piccolo dsp enable */
#define	HY7201_PMU_ID				0x10	/* id register */
#define	HY7201_PMU_BUS				0x18	/* bus retract control */
#define	HY7201_PMU_RESET			0x20	/* reset status */
#define	HY7201_PMU_CLKCTL			0x28	/* clock control */

/*
 * PMUMODE register values
 */
#define	HY7201_PMU_PMUMODE_SLOW			0x00
#define	HY7201_PMU_PMUMODE_RUN			0x01
#define	HY7201_PMU_PMUMODE_IDLE			0x02
#define	HY7201_PMU_PMUMODE_SLEEP		0x03
#define	HY7201_PMU_PMUMODE_DEEP_SLEEP	0x03

/*
 * PICCOLO register values
 */
#define	HY7201_PMU_PICCOLO_PICENABLE	0x01	/* enable piccolo dsp */

/*
 * RESET_STATUS register values
 */
#define	HY7201_PMU_RESET_WARM_RESET		0x2000	/* force warm reset */


/*
 * -------------------------------------------------------------------------
 * 16550 compatible UART
 * Registers are on 4-byte boundaries (register shift = 2)
 * -------------------------------------------------------------------------
 */
#define	HY7201_UART1_BASE		0x80020000		/* physical address */
#define	HY7201_UART2_BASE		0x80021000		/* physical address */
#define	HY7201_UART_SIZE		0x00000020


/*
 * -------------------------------------------------------------------------
 * GPIO
 * -------------------------------------------------------------------------
 */
#define	HY7201_GPIO_BASE		0x80023000		/* physical address */
#define	HY7201_GPIO_SIZE		0x00000080

/*
 * Offsets from HY7201_GPIO_BASE
 */
#define	HY7201_GPIO_PADR		0x00			/* port A data */
#define	HY7201_GPIO_PADDR		0x04			/* port A direction */
#define	HY7201_GPIO_PAIM		0x08			/* port A interrupt mask */
#define	HY7201_GPIO_PAIS		0x0c			/* port A interrupt status */
#define	HY7201_GPIO_PAIE		0x10			/* port A interrupt edge */
#define	HY7201_GPIO_PAIC		0x14			/* port A interrupt clear */
#define	HY7201_GPIO_PAIP		0x18			/* port A interrupt polarity */

#define	HY7201_GPIO_PBDR		0x20			/* port B data */
#define	HY7201_GPIO_PBDDR		0x24			/* port B direction */
#define	HY7201_GPIO_PBIM		0x28			/* port B interrupt mask */
#define	HY7201_GPIO_PBIS		0x2c			/* port B interrupt status */
#define	HY7201_GPIO_PBIE		0x30			/* port B interrupt edge */
#define	HY7201_GPIO_PBIC		0x34			/* port B interrupt clear */
#define	HY7201_GPIO_PBIP		0x38			/* port B interrupt polarity */
#define	HY7201_GPIO_PCDR		0x40			/* port C data */
#define	HY7201_GPIO_PCDDR		0x44			/* port C direction */
#define	HY7201_GPIO_PCIM		0x48			/* port C interrupt mask */
#define	HY7201_GPIO_PCIS		0x4c			/* port C interrupt status */
#define	HY7201_GPIO_PCIE		0x50			/* port C interrupt edge */
#define	HY7201_GPIO_PCIC		0x54			/* port C interrupt clear */
#define	HY7201_GPIO_PDDR		0x60			/* port D data */
#define	HY7201_GPIO_PDDDR		0x64			/* port D direction */
#define	HY7201_GPIO_PDIM		0x68			/* port D interrupt mask */
#define	HY7201_GPIO_PDIS		0x6c			/* port D interrupt status */
#define	HY7201_GPIO_PDIE		0x60			/* port D interrupt edge */
#define	HY7201_GPIO_PDIC		0x64			/* port D interrupt clear */
#define	HY7201_GPIO_PDIP		0x68			/* port D interrupt polarity */
#define	HY7201_GPIO_PMPSB		0x3c			/* port B multifunction pins */
#define	HY7201_GPIO_PMPSD		0x7c			/* port D multifunction pins */

/*
 * -------------------------------------------------------------------------
 * Interrupt controller
 * -------------------------------------------------------------------------
 */
#define	HY7201_INTC_BASE		0x80024000		/* physical address */
#define	HY7201_INTC_SIZE		0x00000014

/*
 * Offsets from HY7201_INTC_BASE
 */
#define	HY7201_INTC_ENABLE		0x08		/* enable: 0=disable, 1=enable */
#define	HY7201_INTC_DIRECTION	0x0c		/* direction: 0=IRQ,  1=FIQ */
#define	HY7201_INTC_STATUS		0x10		/* status */

/*
 * Interrupt register bits
 */
#define	HY7201_INTC_PMU			0x000001
#define	HY7201_INTC_DMA			0x000002
#define	HY7201_INTC_LCD			0x000004
#define	HY7201_INTC_VGA			0x000008
#define	HY7201_INTC_PCMCIA1		0x000010
#define	HY7201_INTC_PCMCIA2		0x000020
#define	HY7201_INTC_AFE			0x000040
#define	HY7201_INTC_AIC			0x000080
#define	HY7201_INTC_KBD			0x000100
#define	HY7201_INTC_TIMER		0x000200
#define	HY7201_INTC_RTC			0x000400
#define	HY7201_INTC_SOUND		0x000800
#define	HY7201_INTC_USB			0x001000
#define	HY7201_INTC_IRDA		0x002000
#define	HY7201_INTC_UART1		0x004000
#define	HY7201_INTC_UART2		0x008000
#define	HY7201_INTC_SPI			0x010000
#define	HY7201_INTC_GPIOA		0x020000
#define	HY7201_INTC_GPIOB		0x040000
#define	HY7201_INTC_GPIOC		0x080000
#define	HY7201_INTC_GPIOD		0x100000
#define	HY7201_INTC_COMMRX		0x200000
#define	HY7201_INTC_COMMTX		0x400000
#define	HY7201_INTC_SWINT		0x800000

/*
 * -------------------------------------------------------------------------
 * TIMER
 * -------------------------------------------------------------------------
 */
#define	HY7201_TIMER_BASE		0x80025000		/* physical address */
#define	HY7201_TIMER_SIZE		0x00000068

/*
 * Offsets from HY7201_TIMER_BASE
 */
#define	HY7201_TIMER_BASE0		0x00			/* timer 0 target count   */
#define	HY7201_TIMER_CNT0		0x08			/* timer 0 counter */
#define	HY7201_TIMER_CTL0		0x10			/* timer 0 control */
#define	HY7201_TIMER_BASE1		0x20			/* timer 1 target count   */
#define	HY7201_TIMER_CNT1		0x28			/* timer 1 counter */
#define	HY7201_TIMER_CTL1		0x30			/* timer 1 control */
#define	HY7201_TIMER_BASE2		0x40			/* timer 2 target count   */
#define	HY7201_TIMER_CNT2		0x48			/* timer 2 counter */
#define	HY7201_TIMER_CTR2		0x50			/* timer 2 control */
#define	HY7201_TIMER_TOPCTL		0x60			/* timer top control */
#define	HY7201_TIMER_STATUS		0x64			/* timer status */

/*
 * CNTL0/1/2 register bits
 */
#define	HY7201_TIMER_CTL_START	0x01
#define	HY7201_TIMER_CTL_REPEAT	0x02
#define	HY7201_TIMER_CTL_RESET	0x04

/*
 * TOPCTL register bits
 */
#define	HY7201_TIMER_TOPCTL_T0INT	0x01		/* enable timer 0 interrupt */
#define	HY7201_TIMER_TOPCTL_T1INT	0x02		/* enable timer 1 interrupt */
#define	HY7201_TIMER_TOPCTL_T2INT	0x04		/* enable timer 2 interrupt */
#define	HY7201_TIMER_TOPCTL_ENABLE	0x08		/* enable timers */
#define	HY7201_TIMER_TOPCTL_64BIT	0x10		/* t1 acts as clock for t2 */

/*
 * STATUS register bits
 */
#define	HY7201_TIMER_STATUS_T0INT	0x01		/* timer 0 interrupt */
#define	HY7201_TIMER_STATUS_T1INT	0x02		/* timer 1 interrupt */
#define	HY7201_TIMER_STATUS_T2INT	0x04		/* timer 2 interrupt */

/*
 * -------------------------------------------------------------------------
 * RTC
 * -------------------------------------------------------------------------
 */
#define	HY7201_RTC_BASE			0x80028000		/* physical address */
#define	HY7201_RTC_SIZE			0x00000014

/*
 * Offsets from HY7201_RTC_BASE
 */
#define	HY7201_RTC_RTCDR		0x00			/* data register */
#define	HY7201_RTC_RTCMR		0x04			/* match register */
#define	HY7201_RTC_RTCS			0x08			/* status register */
#define	HY7201_RTC_RTCDV		0x0c			/* clock divider register */
#define	HY7201_RTC_RTCCR		0x10			/* control register */

#endif	/* __ARM_HY7201_H_INCLUDED */

/* __SRCVERSION("hy7201.h $Rev: 657836 $"); */
