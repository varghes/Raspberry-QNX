/*
 * bcm2835.h
 *
 *  Created on: May 1, 2016
 *      Author: Varghese
 */

#ifndef BCM2835_H_
#define BCM2835_H_


#define BCM2835_GPIO_BASE  0x20200000
#define BCM2835_GPPUD      GPIO_BASE + 0x94
#define BCM2835_GPPUDCLK0  GPIO_BASE + 0x98


#define BCM2835_GPIO_GPFSEL0 0x00
#define BCM2835_GPIO_GPFSEL1 0x04
#define BCM2835_GPIO_GPFSEL2 0x08
#define BCM2835_GPIO_GPFSEL3 0x0C
#define BCM2835_GPIO_GPFSEL4 0x10
#define BCM2835_GPIO_GPFSEL5 0x14

#define BCM2835_GPIO_GPSET0 0x1C
#define BCM2835_GPIO_GPSET1 0x20

#define BCM2835_GPIO_GPCLR0 0x28
#define BCM2835_GPIO_GPCLR1 0x2C

#define BCM2835_GPIO_GPLEV0 0x34
#define BCM2835_GPIO_GPLEV1 0x38

#define BCM2835_GPIO_GPEDS0 0x40
#define BCM2835_GPIO_GPEDS1 0x44

#define BCM2835_GPIO_GPREN0 0x4C
#define BCM2835_GPIO_GPREN1 0x50

#define BCM2835_GPIO_GPFEN0 0x58
#define BCM2835_GPIO_GPFEN1 0x5C

#define BCM2835_GPIO_GPHEN0 0x64
#define BCM2835_GPIO_GPHEN1 0x68

#define BCM2835_GPIO_GPLEN0 0x70
#define BCM2835_GPIO_GPLEN1 0x74

#define BCM2835_GPIO_GPAREN0 0x7C
#define BCM2835_GPIO_GPAREN1 0x80

#define BCM2835_GPIO_GPAFEN0 0x88
#define BCM2835_GPIO_GPAFEN1 0x8C

#define BCM2835_GPIO_GPPUD 0x94
#define BCM2835_GPIO_GPPUDCLK0 0x98
#define BCM2835_GPIO_GPPUDCLK1 0x9C


#define BCM2835_SPI0_BASE 0x20204000
#define BCM2835_SPI0_CS     0x00
#define BCM2835_SPI0_FIFO   0x04
#define BCM2835_SPI0_CLK    0x08
#define BCM2835_SPI0_DLEN   0x0C
#define BCM2835_SPI0_LTOH   0x10
#define BCM2835_SPI0_DC     0x14

#define AUX_PERIPHL_BASE 0x20215000
#define AUX_ENABLES     0x04
#define AUX_MU_IO_REG   0x40
#define AUX_MU_IER_REG  0x44
#define AUX_MU_IIR_REG  0x48
#define AUX_MU_LCR_REG  0x4C
#define AUX_MU_MCR_REG  0x50
#define AUX_MU_LSR_REG  0x54
#define AUX_MU_MSR_REG  0x58
#define AUX_MU_SCRATCH  0x5C
#define AUX_MU_CNTL_REG 0x60
#define AUX_MU_STAT_REG 0x64
#define AUX_MU_BAUD_REG 0x68



#define BCM2835_UART0_BASE  0x20201000
#define BCM2835_UART_SIZE    0x90
// The offsets for reach register for the UART.
#define BCM2835_UART0_DR     0x00
#define BCM2835_UART0_RSRECR 0x04
#define BCM2835_UART0_FR     0x18
#define BCM2835_UART0_ILPR   0x20
#define BCM2835_UART0_IBRD   0x24
#define BCM2835_UART0_FBRD   0x28
#define BCM2835_UART0_LCRH   0x2C
#define BCM2835_UART0_CR     0x30
#define BCM2835_UART0_IFLS   0x34
#define BCM2835_UART0_IMSC   0x38
#define BCM2835_UART0_RIS    0x3C
#define BCM2835_UART0_MIS    0x40
#define BCM2835_UART0_ICR    0x44
#define BCM2835_UART0_DMACR  0x48
#define BCM2835_UART0_ITCR   0x80
#define BCM2835_UART0_ITIP   0x84
#define BCM2835_UART0_ITOP   0x88
#define BCM2835_UART0_TDR    0x8C

#define BCM2835_DBGU_IE_SR_RXEMTY   0x10
#define BCM2835_DBGU_IE_SR_TXRDY    0x20

//interrupt mask.
#define BCM2835_UART0_IMSC_RX   0x10

//#define BCM2835_DBGU_IE_SR_RXRDY 0x40

#define IRQ_OVRR_BIT			(1 << 10)				// Overrun error irq
#define IRQ_BRK_BIT				(1 << 9)				// break irq
#define IRQ_PARE_BIT			(1 << 8)				// Parity error irq
#define IRQ_FRME_BIT			(1 << 7)				// Frame error irq
#define IRQ_RCVTO_BIT			(1 << 6)				// Recv timeout error irq
#define IRQ_TX_BIT				(1 << 5)				// Transmit irq
#define IRQ_RX_BIT				(1 << 4)				// Receive irq
// bits 2 and 3 reserved
#define IRQ_MODEM_BIT			(1 << 1)				// Modem error irq mask
// bit 1 reserved


#define BCM2835_TIMER_SIZE          0x22
#define BCM2835_TIMER_BASE          0x20003000 // changing the timer
#define BCM2835_TIMER_CS 	  0x00
#define BCM2835_TIMER_VALUE_LOWER 	  0x04
#define BCM2835_TIMER_VALUE_HIGHER 	  0x08
#define BCM2835_TIMER0_CMP 	  0x0C
#define BCM2835_TIMER1_CMP 	  0x10
#define BCM2835_TIMER2_CMP 	  0x14
#define BCM2835_TIMER3_CMP 	  0x18

#define TIMER_CS_MATCH_0	(1<<0)	//  Set when compare reg 0 matches
#define TIMER_CS_MATCH_1	(1<<1)	//  Set when compare reg 1 matches
#define TIMER_CS_MATCH_2	(1<<2)	//  Set when compare reg 2 matches
#define TIMER_CS_MATCH_3	(1<<3)	//  Set when compare reg 3 matches

#define ARM_IRQCTLR_BASEOFF		0x2000B000
#define ARM_AIC_SIZE            0x28

#define IRQC_BASICPEND_OFF		(0x00000200)   //
#define IRQC_IRQPEND1_OFF		(0x00000204)   //
#define IRQC_IRQPEND2_OFF		(0x00000208)   //
#define IRQC_FIQCTRL_OFF		(0x0000020C)   //
#define IRQC_ENABIRQ1_OFF		(0x00000210)   //
#define IRQC_ENABIRQ2_OFF		(0x00000214)   //
#define IRQC_ENABBASIC_OFF		(0x00000218)   //
#define IRQC_DISABIRQ1_OFF		(0x0000021C)   //
#define IRQC_DISABIRQ2_OFF		(0x00000220)   //
#define IRQC_DISABBASIC_OFF		(0x00000224)   //

// Bit defs for base pending reg
#define  BCM2835_BASE_ARMTIMER	(1 << 0)		//    1
#define  BCM2835_BASE_ARMMAILBOX	(1 << 1)		// 2
#define  BCM2835_BASE_ARMDOORBEL0	(1 << 2)		//4
#define  BCM2835_BASE_ARMDOORBEL1	(1 << 3)		//8
#define  BCM2835_BASE_GPU0HALT	(1 << 4)		//
#define  BCM2835_BASE_GPU1HALT	(1 << 5)		//
#define  BCM2835_BASE_BADACCESS1	(1 << 6)		//
#define  BCM2835_BASE_BADACCESS0	(1 << 7)		//

#define  BCM2835_BASE_JPEG		(1 << 10)		//GPU_IRQ07
#define  BCM2835_BASE_USB			(1 << 11)		//GPU_IRQ09
#define  BCM2835_BASE_3D			(1 << 12)		//GPU_IRQ10
#define  BCM2835_BASE_DMA2		(1 << 13)		//GPU_IRQ18
#define  BCM2835_BASE_DMA3		(1 << 14)		//GPU_IRQ19
#define  BCM2835_BASE_I2C			(1 << 15)		//GPU_IRQ53
#define  BCM2835_BASE_SPI			(1 << 16)		//GPU_IRQ54
#define  BCM2835_BASE_I2SPCM		(1 << 17)		//GPU_IRQ55
#define  BCM2835_BASE_SDIO		(1 << 18)		//GPU_IRQ56
#define  BCM2835_BASE_UART0		(1 << 19)		//GPU_IRQ57
#define  BCM2835_BASE_ARASANSDIO	(1 << 20)		//GPU_IRQ62

// Bit defs for first IRQ reg
#define  BCM2835_IRQR1_TIMER0			(1 << 0)
#define  BCM2835_IRQR1_TIMER1			(1 << 1)
#define  BCM2835_IRQR1_TIMER2			(1 << 2)
#define  BCM2835_IRQR1_TIMER3			(1 << 3)
#define  BCM2835_IRQR1_CODEC0			(1 << 4)
#define  BCM2835_IRQR1_CODEC1			(1 << 5)
#define  BCM2835_IRQR1_CODEC2			(1 << 6)
#define  BCM2835_IRQR1_VCJPEG			(1 << 7)   // Use the bit in the base reg for pending irq check
#define  BCM2835_IRQR1_ISP			(1 << 8)
#define  BCM2835_IRQR1_VC_USB			(1 << 9)   // Use the bit in the base reg for pending irq check
#define  BCM2835_IRQR1_VC_3D			(1 << 10)  // Use the bit in the base reg for pending irq check
#define  BCM2835_IRQR1_TRANSPOSER		(1 << 11)
#define  BCM2835_IRQR1_MCSYNC0		(1 << 12)
#define  BCM2835_IRQR1_MCSYNC1		(1 << 13)
#define  BCM2835_IRQR1_MCSYNC2		(1 << 14)
#define  BCM2835_IRQR1_MCSYNC3		(1 << 15)
#define  BCM2835_IRQR1_DMA0			(1 << 16)
#define  BCM2835_IRQR1_DMA1			(1 << 17)
#define  BCM2835_IRQR1_VC_DMA2		(1 << 18)  // Use the bit in the base reg for pending irq check
#define  BCM2835_IRQR1_VC_DMA3		(1 << 19)  // Use the bit in the base reg for pending irq check
#define  BCM2835_IRQR1_DMA4			(1 << 20)
#define  BCM2835_IRQR1_DMA5			(1 << 21)
#define  BCM2835_IRQR1_DMA6			(1 << 22)
#define  BCM2835_IRQR1_DMA7			(1 << 23)
#define  BCM2835_IRQR1_DMA8			(1 << 24)
#define  BCM2835_IRQR1_DMA9			(1 << 25)
#define  BCM2835_IRQR1_DMA10			(1 << 26)
#define  BCM2835_IRQR1_DMA11			(1 << 27)
#define  BCM2835_IRQR1_DMA12			(1 << 28)
#define  BCM2835_IRQR1_AUX			(1 << 29)
#define  BCM2835_IRQR1_ARM			(1 << 30)
#define  BCM2835_IRQR1_VPUDMA			(1 << 31)

// Bit defs for second IRQ reg
#define  BCM2835_IRQR2_HOSTPORT		(1 << 0) //GPU32
#define  BCM2835_IRQR2_VIDSCALER		(1 << 1) //GPU33
#define  BCM2835_IRQR2_CCP2TX			(1 << 2) //GPU34
#define  BCM2835_IRQR2_SDC			(1 << 3) //GPU35
#define  BCM2835_IRQR2_DSIO			(1 << 4) //GPU36
#define  BCM2835_IRQR2_AVE			(1 << 5) //GPU37
#define  BCM2835_IRQR2_CAM0			(1 << 6) //GPU38
#define  BCM2835_IRQR2_CAM1			(1 << 7) //GPU39
#define  BCM2835_IRQR2_HDMI0			(1 << 8) //GPU40
#define  BCM2835_IRQR2_HDMI1			(1 << 9) //GPU41
#define  BCM2835_IRQR2_PIXELVALV1		(1 << 10) //GPU42
#define  BCM2835_IRQR2_I2CSPISLAVE	(1 << 11) //GPU43
#define  BCM2835_IRQR2_DSI1			(1 << 12) //GPU44
#define  BCM2835_IRQR2_PWA0			(1 << 13) //GPU45
#define  BCM2835_IRQR2_PWA1			(1 << 14) //GPU46
#define  BCM2835_IRQR2_CPR			(1 << 15) //GPU47
#define  BCM2835_IRQR2_SMI			(1 << 16) //GPU48
#define  BCM2835_IRQR2_GPIO0			(1 << 17) //GPU49
#define  BCM2835_IRQR2_GPIO1			(1 << 18) //GPU50
#define  BCM2835_IRQR2_GPIO2			(1 << 19) //GPU51
#define  BCM2835_IRQR2_GPIO3			(1 << 20) //GPU52
#define  BCM2835_IRQR2_VC_I2C			(1 << 21) //GPU53  // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_VC_SPI			(1 << 22) //GPU54  // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_I2SPCM			(1 << 23) //GPU55  // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_SDIO			(1 << 24) //GPU56  // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_UART0			(1 << 25) //GPU57  // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_SLIMBUS		(1 << 26) //GPU58
#define  BCM2835_IRQR2_VEC			(1 << 27) //GPU59
#define  BCM2835_IRQR2_CPG			(1 << 28) //GPU60
#define  BCM2835_IRQR2_RNG			(1 << 29) //GPU61
#define  BCM2835_IRQR2_VC_ARASANSDIO	(1 << 30) //GPU62 // Use bit in base reg for pending irq check
#define  BCM2835_IRQR2_AVSPMON		(1 << 31) //GPU63



#define  BCM2835_PHYSIRQ_TIMER0		0
#define  BCM2835_PHYSIRQ_TIMER1		1
#define  BCM2835_PHYSIRQ_TIMER2		2
#define  BCM2835_PHYSIRQ_TIMER3		3
#define  BCM2835_PHYSIRQ_CODEC0		4
#define  BCM2835_PHYSIRQ_CODEC1		5
#define  BCM2835_PHYSIRQ_CODEC2		6
#define  BCM2835_PHYSIRQ_VCJPEG		7
#define  BCM2835_PHYSIRQ_ISP			8
#define  BCM2835_PHYSIRQ_VC_USB		9
#define  BCM2835_PHYSIRQ_VC_3D		10
#define  BCM2835_PHYSIRQ_TRANSPOSER	11
#define  BCM2835_PHYSIRQ_MCSYNC0		12
#define  BCM2835_PHYSIRQ_MCSYNC1		13
#define  BCM2835_PHYSIRQ_MCSYNC2		14
#define  BCM2835_PHYSIRQ_MCSYNC3		15
#define  BCM2835_PHYSIRQ_DMA0			16
#define  BCM2835_PHYSIRQ_DMA1			17
#define  BCM2835_PHYSIRQ_VC_DMA2		18
#define  BCM2835_PHYSIRQ_VC_DMA3		19
#define  BCM2835_PHYSIRQ_DMA4			20
#define  BCM2835_PHYSIRQ_DMA5			21
#define  BCM2835_PHYSIRQ_DMA6			22
#define  BCM2835_PHYSIRQ_DMA7			23
#define  BCM2835_PHYSIRQ_DMA8			24
#define  BCM2835_PHYSIRQ_DMA9			25
#define  BCM2835_PHYSIRQ_DMA10		26
#define  BCM2835_PHYSIRQ_DMA11		27
#define  BCM2835_PHYSIRQ_DMA12		28
#define  BCM2835_PHYSIRQ_AUX			29
#define  BCM2835_PHYSIRQ_ARM			30
#define  BCM2835_PHYSIRQ_VPUDMA		31

#define  BCM2835_PHYSIRQ_HOSTPORT		32
#define  BCM2835_PHYSIRQ_VIDSCALER	33
#define  BCM2835_PHYSIRQ_CCP2TX		34
#define  BCM2835_PHYSIRQ_SDC			35
#define  BCM2835_PHYSIRQ_DSIO			36
#define  BCM2835_PHYSIRQ_AVE			37
#define  BCM2835_PHYSIRQ_CAM0			38
#define  BCM2835_PHYSIRQ_CAM1			39
#define  BCM2835_PHYSIRQ_HDMI0		40
#define  BCM2835_PHYSIRQ_HDMI1		41
#define  BCM2835_PHYSIRQ_PIXELVALV1	42
#define  BCM2835_PHYSIRQ_I2CSPISLAVE	43
#define  BCM2835_PHYSIRQ_DSI1			44
#define  BCM2835_PHYSIRQ_PWA0			45
#define  BCM2835_PHYSIRQ_PWA1			46
#define  BCM2835_PHYSIRQ_CPR			47
#define  BCM2835_PHYSIRQ_SMI			48
#define  BCM2835_PHYSIRQ_GPIO0		49
#define  BCM2835_PHYSIRQ_GPIO1		50
#define  BCM2835_PHYSIRQ_GPIO2		51
#define  BCM2835_PHYSIRQ_GPIO3		52
#define  BCM2835_PHYSIRQ_VC_I2C		53
#define  BCM2835_PHYSIRQ_VC_SPI		54
#define  BCM2835_PHYSIRQ_I2SPCM		55
#define  BCM2835_PHYSIRQ_SDIO			56
#define  BCM2835_PHYSIRQ_UART0		57
#define  BCM2835_PHYSIRQ_SLIMBUS		58
#define  BCM2835_PHYSIRQ_VEC			59
#define  BCM2835_PHYSIRQ_CPG			60
#define  BCM2835_PHYSIRQ_RNG			61
#define  BCM2835_PHYSIRQ_VC_ARASANSDIO 62
#define  BCM2835_PHYSIRQ_AVSPMON		63


#endif /* BCM2835_H_ */
