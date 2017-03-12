/*
 * $QNXLicenseC:
 * Copyright 2011, QNX Software Systems. 
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
  * i.MX6 Naming convention:
  * i.MX6 Q (Quad) = i.MX63
  * i.MX6 D (Dual) = i.MX61
  * i.MX6 S (Solo) = i.MX60
  *
  * i.MX6 Q, i.MX D are the same chip except for the difference
  * in number of enabled cores (4 versus 2), as such they share 
  * the same memory map, and IRQs.  The i.MX6 S has a different GPU, and has a 
  * few less peripherals than the dual/quad, however the memory maps, and IRQs are
  * compatible.  This header file can be used for any i.MX6x SOC.
  */

#ifndef	__ARM_MX6X_H_INCLUDED
#define	__ARM_MX6X_H_INCLUDED


/*
 * System Memory Map
 */

/*
 * BOOT ROM (ROMCP)
 */
#define MX6X_BOOTROM_BASE                       0x00000000
#define MX6X_BOOTROM_SILICON_REV                0x48	



#define MX6X_WEIM_CS0_BASE                      0x08000000  /* NOR or SRAM */

/*
 * Enhanced Secured Digital Host Controllers
 */
#define MX6X_USDHC1_BASE                        0x02190000
#define MX6X_USDHC2_BASE                        0x02194000
#define MX6X_USDHC3_BASE                        0x02198000
#define MX6X_USDHC4_BASE                        0x0219C000
#define MX6X_USDHC_SIZE                         0x4000
#define MX6X_USDHC_MAP_SIZE                     0x100

#define MX6X_USDHC1_IRQ                         54
#define MX6X_USDHC2_IRQ                         55
#define MX6X_USDHC3_IRQ                         56
#define MX6X_USDHC4_IRQ                         57

/*
 * Multi Mode DDR Controller (MMDC) 
 */
#define MX6X_MMDC_P0_BASE                       0x021B0000   /* port 0 */
#define MX6X_MMDC_P1_BASE                       0x021B4000   /* port 1 */
#define MX6X_MMDC_SIZE                          0x4000

/*
 * UARTs
 */
#define MX6X_UART1_BASE                         0x02020000
#define MX6X_UART2_BASE                         0x021E8000
#define MX6X_UART3_BASE                         0x021EC000
#define MX6X_UART4_BASE                         0x021F0000
#define MX6X_UART5_BASE                         0x021F4000

#define MX6X_UART1_IRQ                          58
#define MX6X_UART2_IRQ                          59
#define MX6X_UART3_IRQ                          60
#define MX6X_UART4_IRQ                          61
#define MX6X_UART5_IRQ                          62

#define MX6X_UART_SIZE                          0x4000

/*
 * USB Base Address
 */
#define MX6X_USB_BASE                           0x02184000

#define MX6X_USBOTG_BASE                        0x02184000
#define MX6X_USBH1_BASE                         0x02184200
#define MX6X_USBH2_BASE                         0x02184400
#define MX6X_USBH3_BASE                         0x02184600
#define MX6X_USB_OTHER_BASE                     0x02184800

#define MX6X_USB_CMD                            0x140

#define MX6X_USBH1_IRQ                          72
#define MX6X_USBH2_IRQ                          73
#define MX6X_USBH3_IRQ                          74
#define MX6X_USBOTG_IRQ                         75

#define MX6X_USB_SIZE                           0x4000

/* USB non-core registers */
#define MX6X_USB_OTG_CTRL						0x00
#define MX6X_USB_UH1_CTRL						0x04
#define MX6X_USB_UH2_CTRL						0x08
#define MX6X_USB_UH3_CTRL						0x0c
#define MX6X_USB_UH2_HSIC_CTRL					0x10
#define MX6X_USB_UH3_HSIC_CTRL					0x14
#define MX6X_USB_OTG_PHY_CTRL_0					0x18
#define MX6X_USB_UH1_PHY_CTRL_0					0x1c
#define MX6X_USB_UH2_HSIC_DLL_CFG1				0x20
#define MX6X_USB_UH2_HSIC_DLL_CFG2				0x24
#define MX6X_USB_UH2_HSIC_DLL_CFG3				0x28
#define MX6X_USB_UH3_HSIC_DLL_CFG1				0x30
#define MX6X_USB_UH3_HSIC_DLL_CFG2				0x34
#define MX6X_USB_UH3_HSIC_DLL_CFG3				0x38

/*
 * ANATOP
 * Used for configuring PLLs, PFDs
 */
#define MX6X_ANATOP_BASE                        0x020C8000
#define MX6X_ANATOP_SIZE                        0x1000

#define MX6X_ANATOP_PLL1_SYS                    0x00
#define MX6X_ANATOP_PLL3_USB1_OTG               0x10
#define MX6X_ANATOP_PLL7_USB2_HOST              0x20
#define MX6X_ANATOP_PLL2_SYS_BUS                0x30
#define MX6X_ANATOP_PLL4_AUDIO                  0x70
#define MX6X_ANATOP_PLL4_AUDIO_NUM              0x80
#define MX6X_ANATOP_PLL4_AUDIO_DENOM            0x90
#define MX6X_ANATOP_PLL5_VIDEO                  0xA0
#define MX6X_ANATOP_PLL5_VIDEO_NUM              0xB0
#define MX6X_ANATOP_PLL5_VIDEO_DENOM            0xC0
#define MX6X_ANATOP_PLL6_MLB                    0xD0
#define MX6X_ANATOP_PLL8_ENET                   0xE0
#define MX6X_ANATOP_PFD_480                     0xF0
#define MX6X_ANATOP_PFD_480_SET                 0xF4
#define MX6X_ANATOP_PFD_480_CLR                 0xF8
#define MX6X_ANATOP_PFD_480_TOP                 0xFC
#define MX6X_ANATOP_PFD_528                     0x100

#define ANATOP_SETREG_OFFSET                    0x04
#define ANATOP_CLRREG_OFFSET                    0x08
#define ANATOP_TOGGLE_OFFSET                    0x0C

#define MX6X_ANADIG_REG_CORE                    0x140 
#define MX6X_ANADIG_CHIP_INFO			0x260 /* TODO: docs? */	
#define ANADIG_REG_CORE_REG2_TRG_MASK           0x7C0000

#define ANATOP_PLL8_ENET_LOCK 				(1 << 31) 
#define ANATOP_PLL8_ENET_ENABLE_SATA		(1 << 20) 
#define ANATOP_PLL8_ENET_ENABLE_PCIE		(1 << 19) 
#define ANATOP_PLL8_ENET_PFD_OFFSET_EN		(1 << 18) 
#define ANATOP_PLL8_ENET_DITHER_ENABLE		(1 << 17) 
#define ANATOP_PLL8_ENET_BYPASS				(1 << 16) 
#define ANATOP_PLL8_ENET_BYPASS_CLK_OSC		(0x0 << 14) 
#define ANATOP_PLL8_ENET_BYPASS_CLK_ANACLK_1	(0x1 << 14) 
#define ANATOP_PLL8_ENET_BYPASS_CLK_ANACLK_2	(0x2 << 14) 
#define ANATOP_PLL8_ENET_BYPASS_CLK_XOR		(0x3 << 14) 
#define ANATOP_PLL8_ENET_ENABLE				(1 << 13) 
#define ANATOP_PLL8_ENET_POWERDOWN			(1 << 12) 
#define ANATOP_PLL8_ENET_REF_MASK           (0x3 << 0)
#define ANATOP_PLL8_ENET_REF_ENET_25M		(0x0 << 0)
#define ANATOP_PLL8_ENET_REF_ENET_50M		(0x1 << 0)
#define ANATOP_PLL8_ENET_REF_ENET_100M		(0x2 << 0)
#define ANATOP_PLL8_ENET_REF_ENET_125M		(0x3 << 0)

/* 
 * USB PHY0, 1
 */
#define MX6X_USBPHY0_BASE                       0x020C9000
#define MX6X_USBPHY1_BASE                       0x020CA000

#define MX6X_USBPHY_PWD                         0x00
#define MX6X_USBPHY_TX                          0x10
#define MX6X_USBPHY_RX                          0x20
#define MX6X_USBPHY_CTRL                        0x30
#define MX6X_USBPHY_STATUS                      0x40
#define MX6X_USBPHY_DEBUG                       0x50
#define MX6X_USBPHY_DEBUG0_STATUS               0x60
#define MX6X_USBPHY_DEBUG1_STATUS               0x70
#define MX6X_USBPHY_VERSION                     0x80
#define MX6X_USBPHY_IP                          0x90

/*
 * SNVS Secure Non Voltatile Storage
 */
#define MX6X_SNVS_BASE							0x020CC000
#define MX6X_SNVS_SIZE							0x4000
#define MX6X_SNVS_SECURE_IRQ					51		/* Non TrustZone    */
#define MX6X_SNVS_NON_SECURE_IRQ				52		/* TrustZone        */

#define MX6X_SNVS_HPSR							0x14    /* HP Status Register */
#define MX6X_SNVS_LPCR							0x38    /* LP Control Register */
#define MX6X_SNVS_LPSR							0x4C    /* LP Status Register */
#define MX6X_SNVS_LPSRTCMR						0x50    /* LP Secure Real Time Counter MSB Register */
#define MX6X_SNVS_LPSRTCLR						0x54    /* LP Secure Real Time Counter LSB Register */
#define MX6X_SNVS_LPTAR							0x58    /* LP Time Alarm Register */
#define MX6X_SNVS_LPSMCMR						0x5C    /* LP Secure Monotonic Counter MSB Register */
#define MX6X_SNVS_LPSMCLR						0x60    /* LP Secure Monotonic Counter LSB Register */
#define MX6X_SNVS_LPPGDR						0x64    /* LP Power Glitch Detector Register */
#define MX6X_SNVS_LPGPR							0x68    /* LP General Purpose Register */

/*
 * SATA
 */
#define MX6X_SATA_BASE				            0x02200000
#define MX6X_SATA_SIZE				            0x4000

#define MX6X_SATA_CAP							0x00
#define MX6X_SATA_CAP_SSS						(0x1 << 27)
#define MX6X_SATA_GHC							0x04
#define MX6X_SATA_GHC_RESET						(0x1 << 0)
#define MX6X_SATA_PI							0x0C
#define MX6X_SATA_PI_EN							0x1
#define MX6X_TIMER1MS							0xE0
#define MX6X_SATA_SR							0x128

/*
 * Enhanced Configurable Serial Peripheral Interfaces
 */
#define MX6X_ECSPI1_BASE                        0x02008000
#define MX6X_ECSPI2_BASE                        0x0200C000
#define MX6X_ECSPI3_BASE                        0x02010000
#define MX6X_ECSPI4_BASE                        0x02014000
#define MX6X_ECSPI5_BASE                        0x02018000

#define MX6X_ECSPI1_IRQ                         63
#define MX6X_ECSPI2_IRQ                         64
#define MX6X_ECSPI3_IRQ                         65
#define MX6X_ECSPI4_IRQ                         66
#define MX6X_ECSPI5_IRQ                         67

#define MX6X_ECSPI_SIZE                         0x4000

/*
 * Audio MUX
 */
#define MX6X_AUDMUX_BASE                        0x021D8000
#define MX6X_AUDMUX_SIZE                        0x4000

/* Port offset */
#define MX6X_AUDMUX_PTCR1                       0x00
#define MX6X_AUDMUX_PDCR1                       0x04
#define MX6X_AUDMUX_PTCR2                       0x08
#define MX6X_AUDMUX_PDCR2                       0x0C
#define MX6X_AUDMUX_PTCR3                       0x10
#define MX6X_AUDMUX_PDCR3                       0x14
#define MX6X_AUDMUX_PTCR4                       0x18
#define MX6X_AUDMUX_PDCR4                       0x1C
#define MX6X_AUDMUX_PTCR5                       0x20
#define MX6X_AUDMUX_PDCR5                       0x24
#define MX6X_AUDMUX_PTCR6                       0x28
#define MX6X_AUDMUX_PDCR6                       0x2C
#define MX6X_AUDMUX_PTCR7                       0x30
#define MX6X_AUDMUX_PDCR7                       0x34
#define MX6X_AUDMUX_CNMCR                       0x38
#define MX6X_AUDMUX_CNMCR                       0x38

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

#define MX6X_CCM_BASE                           0x020C4000
#define MX6X_CCM_SIZE                           0x4000

/* CCM Registers, offset from base address */
#define MX6X_CCM_CCR                            0x00
#define MX6X_CCM_CCDR                           0x04
#define MX6X_CCM_CSR                            0x08
#define MX6X_CCM_CCSR                           0x0C
#define MX6X_CCM_CACCR                          0x10
#define MX6X_CCM_CBCDR                          0x14
	#define MX6X_CCM_CBCDR_PERIPH_CLK2_PODF_MASK     	(0x7 << 27)
	#define MX6X_CCM_CBCDR_PERIPH_CLK2_PODF_OFFSET   	(27)
	#define MX6X_CCM_CBCDR_PERIPH2_CLK2_SEL          	(1 << 26)
	#define MX6X_CCM_CBCDR_PERIPH_CLK_SEL            	(1 << 25)
	#define MX6X_CCM_CBCDR_MMDC_CH0_PODF_MASK        	(0x7 << 19)
	#define MX6X_CCM_CBCDR_MMDC_CH0_PODF_OFFSET      	(19)
	#define MX6X_CCM_CBCDR_AXI_PODF_MASK         		(0x7 << 16)
	#define MX6X_CCM_CBCDR_AXI_PODF_OFFSET           	(16)
	#define MX6X_CCM_CBCDR_AHB_PODF_MASK        	 	(0x7 << 10)
	#define MX6X_CCM_CBCDR_AHB_PODF_OFFSET           	(10)
	#define MX6X_CCM_CBCDR_IPG_PODF_MASK         		(0x3 << 8)
	#define MX6X_CCM_CBCDR_IPG_PODF_OFFSET           	(8)
	#define MX6X_CCM_CBCDR_AXI_ALT_SEL           		(1 << 7)
	#define MX6X_CCM_CBCDR_AXI_SEL               		(1 << 6)
	#define MX6X_CCM_CBCDR_MMDC_CH1_PODF_MASK        	(0x7 << 3)
	#define MX6X_CCM_CBCDR_MMDC_CH1_PODF_OFFSET      	(3)
	#define MX6X_CCM_CBCDR_PERIPH2_CLK2_PODF_MASK   	(0x7 << 0)
	#define MX6X_CCM_CBCDR_PERIPH2_CLK2_PODF_OFFSET		(0)
#define MX6X_CCM_CBCMR                          0x18
	#define MX6X_CCM_CBCMR_GPU3D_SHADER_PODF_MASK       (0x7 << 29)
	#define MX6X_CCM_CBCMR_GPU3D_SHADER_PODF_OFFSET     (29)
	#define MX6X_CCM_CBCMR_GPU3D_CORE_PODF_MASK      	(0x7 << 26)
	#define MX6X_CCM_CBCMR_GPU3D_CORE_PODF_OFFSET       (26)
	#define MX6X_CCM_CBCMR_GPU2D_CORE_PODF_MASK      	(0x7 << 23)
	#define MX6X_CCM_CBCMR_GPU2D_CORE_PODF_OFFSET       (23)
	#define MX6X_CCM_CBCMR_PRE_PERIPH2_CLK_SEL_MASK     (0x3 << 21)
	#define MX6X_CCM_CBCMR_PRE_PERIPH2_CLK_SEL_OFFSET   (21)
	#define MX6X_CCM_CBCMR_PRE_PERIPH2_CLK2_SEL     	(1 << 20)
	#define MX6X_CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK      (0x3 << 18)
	#define MX6X_CCM_CBCMR_PRE_PERIPH_CLK_SEL_OFFSET    (18)
	#define MX6X_CCM_CBCMR_GPU2D_CLK_SEL_MASK        	(0x3 << 16)
	#define MX6X_CCM_CBCMR_GPU2D_CLK_SEL_OFFSET      	(16)
	#define MX6X_CCM_CBCMR_VPU_AXI_CLK_SEL_MASK     	(0x3 << 14)
	#define MX6X_CCM_CBCMR_VPU_AXI_CLK_SEL_OFFSET       (14)
	#define MX6X_CCM_CBCMR_PERIPH_CLK2_SEL_MASK      	(0x3 << 12)
	#define MX6X_CCM_CBCMR_PERIPH_CLK2_SEL_OFFSET       (12)
	#define MX6X_CCM_CBCMR_VDOAXI_CLK_SEL            	(1 << 11)
	#define MX6X_CCM_CBCMR_PCIE_AXI_CLK_SEL          	(1 << 10)
	#define MX6X_CCM_CBCMR_GPU3D_SHADER_CLK_SEL_MASK    (0x3 << 8)
	#define MX6X_CCM_CBCMR_GPU3D_SHADER_CLK_SEL_OFFSET  (8)
	#define MX6X_CCM_CBCMR_GPU3D_CORE_CLK_SEL_MASK      (0x3 << 4)
	#define MX6X_CCM_CBCMR_GPU3D_CORE_CLK_SEL_OFFSET    (4)
	#define MX6X_CCM_CBCMR_GPU3D_AXI_CLK_SEL         	(1 << 1)
	#define MX6X_CCM_CBCMR_GPU2D_AXI_CLK_SEL         	(1 << 0)
#define MX6X_CCM_CSCMR1                         0x1c
	#define MX6X_CCM_CSCMR1_ACLK_EMI_SLOW_MASK       	(0x3 << 29)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_SLOW_OFFSET     	(29)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_MASK           	(0x3 << 27)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_OFFSET          	(27)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_SLOW_PODF_MASK     (0x7 << 23)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_SLOW_PODF_OFFSET   (23)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_PODF_MASK       	(0x7 << 20)
	#define MX6X_CCM_CSCMR1_ACLK_EMI_PODF_OFFSET     	(20)
	#define MX6X_CCM_CSCMR1_USDHC4_CLK_SEL           	(1 << 19)
	#define MX6X_CCM_CSCMR1_USDHC3_CLK_SEL           	(1 << 18)
	#define MX6X_CCM_CSCMR1_USDHC2_CLK_SEL           	(1 << 17)
	#define MX6X_CCM_CSCMR1_USDHC1_CLK_SEL           	(1 << 16)
	#define MX6X_CCM_CSCMR1_SSI3_CLK_SEL_MASK        	(0x3 << 14)
	#define MX6X_CCM_CSCMR1_SSI3_CLK_SEL_OFFSET      	(14)
	#define MX6X_CCM_CSCMR1_SSI2_CLK_SEL_MASK        	(0x3 << 12)
	#define MX6X_CCM_CSCMR1_SSI2_CLK_SEL_OFFSET      	(12)
	#define MX6X_CCM_CSCMR1_SSI1_CLK_SEL_MASK        	(0x3 << 10)
	#define MX6X_CCM_CSCMR1_SSI1_CLK_SEL_OFFSET      	(10)
	#define MX6X_CCM_CSCMR1_PERCLK_PODF_MASK         	(0x3F)
#define MX6X_CCM_CSCMR2                         0x20
#define MX6X_CCM_CSCDR1                         0x24
    #define MX6X_CCM_CSCDR1_VPU_AXI_PODF_MASK        	(0x7 << 25)
    #define MX6X_CCM_CSCDR1_VPU_AXI_PODF_OFFSET      	(25)
    #define MX6X_CCM_CSCDR1_USDHC4_PODF_MASK         	(0x7 << 22)
    #define MX6X_CCM_CSCDR1_USDHC4_PODF_OFFSET       	(22)
    #define MX6X_CCM_CSCDR1_USDHC3_PODF_MASK         	(0x7 << 19)
    #define MX6X_CCM_CSCDR1_USDHC3_PODF_OFFSET       	(19)
    #define MX6X_CCM_CSCDR1_USDHC2_PODF_MASK         	(0x7 << 16)
    #define MX6X_CCM_CSCDR1_USDHC2_PODF_OFFSET       	(16)
    #define MX6X_CCM_CSCDR1_USDHC1_PODF_MASK         	(0x7 << 11)
    #define MX6X_CCM_CSCDR1_USDHC1_PODF_OFFSET       	(11)
    #define MX6X_CCM_CSCDR1_USBOH3_CLK_PRED_OFFSET   	(8)
    #define MX6X_CCM_CSCDR1_USBOH3_CLK_PRED_MASK     	(0x7 << 8)
    #define MX6X_CCM_CSCDR1_USBOH3_CLK_PODF_OFFSET   	(6)
    #define MX6X_CCM_CSCDR1_USBOH3_CLK_PODF_MASK     	(0x3 << 6)
    #define MX6X_CCM_CSCDR1_UART_CLK_PODF_MASK       	(0x3F)
    #define MX6X_CCM_CSCDR1_UART_CLK_PODF_OFFSET     	(0)
#define MX6X_CCM_CS1CDR                         0x28
#define MX6X_CCM_CS2CDR                         0x2c
    #define MX6X_CCM_CS2CDR_ENFC_CLK_PODF_MASK       	(0x3F << 21)
    #define MX6X_CCM_CS2CDR_ENFC_CLK_PODF_OFFSET     	(21)
    #define MX6X_CCM_CS2CDR_ENFC_CLK_PRED_MASK       	(0x7 << 18)
    #define MX6X_CCM_CS2CDR_ENFC_CLK_PRED_OFFSET     	(18)
    #define MX6X_CCM_CS2CDR_ENFC_CLK_SEL_MASK        	(0x3 << 16)
    #define MX6X_CCM_CS2CDR_ENFC_CLK_SEL_OFFSET      	(16)
    #define MX6X_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK     	(0x7 << 12)
    #define MX6X_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET   	(12)
    #define MX6X_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK     	(0x7 << 9)
    #define MX6X_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET   	(9)
    #define MX6X_CCM_CS2CDR_SSI2_CLK_PRED_MASK       	(0x7 << 6)
    #define MX6X_CCM_CS2CDR_SSI2_CLK_PRED_OFFSET     	(6)
    #define MX6X_CCM_CS2CDR_SSI2_CLK_PODF_MASK       	(0x3F)
    #define MX6X_CCM_CS2CDR_SSI2_CLK_PODF_OFFSET     	(0)
#define MX6X_CCM_CDCDR                          0x30
#define MX6X_CCM_CHSCCDR						0x34
#define MX6X_CCM_CSCDR2                         0x38
	#define MX6X_CCM_CSCDR2_ECSPI_CLK_PODF_MASK     	(0x3F << 19)
	#define MX6X_CCM_CSCDR2_ECSPI_CLK_PODF_OFFSET       (19)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_PRE_CLK_SEL_MASK  (0x7 << 15)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_PRE_CLK_SEL_OFFSET (15)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_PODF_MASK      	(0x7 << 12)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_PODF_OFFSET       (12)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_CLK_SEL_MASK      (0x7 << 9)
	#define MX6X_CCM_CHSCCDR_IPU2_DI1_CLK_SEL_OFFSET    (9)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_PRE_CLK_SEL_MASK  (0x7 << 6)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_PRE_CLK_SEL_OFFSET (6)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_PODF_MASK      	(0x7 << 3)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_PODF_OFFSET       (3)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_CLK_SEL_MASK      (0x7)
	#define MX6X_CCM_CHSCCDR_IPU2_DI0_CLK_SEL_OFFSET    (0)
#define MX6X_CCM_CSCDR3                         0x3c
#define MX6X_CCM_CSCDR4                         0x40
#define MX6X_CCM_CWDR                           0x44
#define MX6X_CCM_CDHIPR                         0x48
#define MX6X_CCM_CDCR                           0x4c
#define MX6X_CCM_CTOR                           0x50
#define MX6X_CCM_CLPCR                          0x54
#define MX6X_CCM_CISR                           0x58
#define MX6X_CCM_CIMR                           0x5c
#define MX6X_CCM_CCOSR                          0x60
#define MX6X_CCM_CGPR                           0x64
#define MX6X_CCM_CCGR0                          0x68
#define MX6X_CCM_CCGR1                          0x6c
	#define MX6X_CCGR1_GPU3D_ENABLE                  (0x3 << 26)
#define MX6X_CCM_CCGR2                          0x70
#define MX6X_CCM_CCGR3                          0x74
#define MX6X_CCM_CCGR4                          0x78
#define MX6X_CCM_CCGR5                          0x7c
#define MX6X_CCM_CCGR6                          0x80
#define MX6X_CCM_CCGR7                          0x84
#define MX6X_CCM_CMEOR                          0x88

/* CCM Clock Gating Reset Values */
#define MX6X_CCM_CCGR0_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR1_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR2_RESET                    0xFC3FFFFF
#define MX6X_CCM_CCGR3_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR4_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR5_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR6_RESET                    0xFFFFFFFF
#define MX6X_CCM_CCGR7_RESET                    0xFFFFFFFF

/* CCM Clock Gating Values */
#define CCGR0_CG14_DTCP                         (0x3 << 28)
#define CCGR0_CG13_DCIC2                        (0x3 << 26)
#define CCGR0_CG12_DCIC1                        (0x3 << 24)
#define CCGR0_CG11_CHEETAH_DBG                  (0x3 << 22)
#define CCGR0_CG10_CAN2_SERIAL                  (0x3 << 20)
#define CCGR0_CG9_CAN2_CLK                      (0x3 << 18)
#define CCGR0_CG8_CAN1_SERIAL                   (0x3 << 16)
#define CCGR0_CG7_CAN1_CLK                      (0x3 << 14)
#define CCGR0_CG6_CAAM_IPG                      (0x3 << 12)
#define CCGR0_CG5_CAAM_ACLK                     (0x3 << 10)
#define CCGR0_CG4_CAAM_SECURE_MEM               (0x3 << 8)
#define CCGR0_CG3_ASRC                          (0x3 << 6)
#define CCGR0_CG2_APBH_DMA_HCLK                 (0x3 << 4)
#define CCGR0_CG1_AIPS_TZ2                      (0x3 << 2)
#define CCGR0_CG0_AIPS_TZ1                      (0x3 << 0)

#define CCGR1_CG13_GPU3D                        (0x3 << 26)
#define CCGR1_CG12_GPU2D                        (0x3 << 24)
#define CCGR1_CG11_GPT_SERIAL                   (0x3 << 22)
#define CCGR1_CG10_GPT                          (0x3 << 20)
#define CCGR1_CG8_ESAI                          (0x3 << 16)
#define CCGR1_CG7_EPIT2                         (0x3 << 14)
#define CCGR1_CG6_EPIT1                         (0x3 << 12)
#define CCGR1_CG5_ENET                          (0x3 << 10)
#define CCGR1_CG4_ECSPI5                        (0x3 << 8)
#define CCGR1_CG3_ECSPI4         				(0x3 << 6)
#define CCGR1_CG2_ECSPI3                        (0x3 << 4)
#define CCGR1_CG1_ECSPI2                        (0x3 << 2)
#define CCGR1_CG0_ECSPI1                        (0x3 << 0)

#define CCGR2_CG13_IPSYNC                       (0x3 << 26)
#define CCGR2_CG12_IPSYNC                       (0x3 << 24)
#define CCGR2_CG11_IPSYNC                       (0x3 << 22)
#define CCGR2_CG10_IPMUX3                       (0x3 << 20)
#define CCGR2_CG9_IPMUX2                        (0x3 << 18)
#define CCGR2_CG8_IPMUX1                        (0x3 << 16)
#define CCGR2_CG7_IOMUX_IPT                     (0x3 << 14)
#define CCGR2_CG6_OCOTP                         (0x3 << 12)
#define CCGR2_CG5_I2C3                          (0x3 << 10)
#define CCGR2_CG4_I2C2                          (0x3 << 8)
#define CCGR2_CG3_I2C1                          (0x3 << 6)
#define CCGR2_CG2_HDMI_TX_ISFR                  (0x3 << 4)
#define CCGR2_CG0_HDMI_TX_IAHB                  (0x3 << 0)

#define CCGR3_CG15_OPENVG_AXI                   (0x3 << 30)
#define CCGR3_CG14_OCRAM                        (0x3 << 28)
#define CCGR3_CG13_MMDC_CORE_IPG_P1             (0x3 << 26)
#define CCGR3_CG12_MMDC_CORE_IPG_P2             (0x3 << 24)
#define CCGR3_CG11_MMDC_CORE_ACLK_FAST_P1       (0x3 << 22)
#define CCGR3_CG10_MMDC_CORE_ACLK_FAST_P0       (0x3 << 20)
#define CCGR3_CG9_MLB                           (0x3 << 18)
#define CCGR3_CG8_MIPI                          (0x3 << 16)
#define CCGR3_CG7_LDB_DI1                       (0x3 << 14)
#define CCGR3_CG6_LDB_DI0                       (0x3 << 12)
#define CCGR3_CG5_IPU2_DI1                      (0x3 << 10)
#define CCGR3_CG4_IPU2_DI0                      (0x3 << 8)
#define CCGR3_CG3_IPU2_CLK                      (0x3 << 6)
#define CCGR3_CG2_IPU1_DI1                      (0x3 << 4)
#define CCGR3_CG1_IPU1_DI0                      (0x3 << 2)
#define CCGR3_CG0_IPU1_CLK                      (0x3 << 0)

#define CCGR4_CG15_RAWNAND_GPMI_INPUT_APB       (0x3 << 30)
#define CCGR4_CG14_RAWNAND_GPMI_BCH_INPUT       (0x3 << 28)
#define CCGR4_CG13_RAWNAND_GPMI_BCH_INPUT       (0x3 << 26)
#define CCGR4_CG12_RAWNAND_BCH_INPUT_APB        (0x3 << 24)
#define CCGR4_CG11_PWM4                         (0x3 << 22)
#define CCGR4_CG10_PWM3                         (0x3 << 20)
#define CCGR4_CG9_PWM2                          (0x3 << 18)
#define CCGR4_CG8_PWM1                          (0x3 << 16)
#define CCGR4_CG7_PL301_PER2_MAINCLK            (0x3 << 14)
#define CCGR4_CG6_PL301_PER1_BCH                (0x3 << 12)
#define CCGR4_CG4_PL301_FAST1_S133              (0x3 << 8)
#define CCGR4_CG0_PCIE                          (0x3 << 0)

#define CCGR5_CG13_UART_SERIAL_CLK              (0x3 << 26)
#define CCGR5_CG12_UART_CLK                     (0x3 << 24)
#define CCGR5_CG11_SSI3                         (0x3 << 22)
#define CCGR5_CG10_SSI2                         (0x3 << 20)
#define CCGR5_CG9_SSI1                          (0x3 << 18)
#define CCGR5_CG7_SPDIF                         (0x3 << 14)
#define CCGR5_CG6_SPBA                          (0x3 << 12)
#define CCGR5_CG3_SDMA                          (0x3 << 6)
#define CCGR5_CG2_SATA                          (0x3 << 4)
#define CCGR5_CG0_ROM                           (0x3 << 0)

#define CCGR6_CG7_VPU                           (0x3 << 14)
#define CCGR6_CG6_VDOAXICLK                     (0x3 << 12)
#define CCGR6_CG5_EMI_SLOW                      (0x3 << 10)
#define CCGR6_CG4_USDHC4                        (0x3 << 8)
#define CCGR6_CG3_USDHC3                        (0x3 << 6)
#define CCGR6_CG2_USDHC2                        (0x3 << 4)
#define CCGR6_CG1_USDHC1                        (0x3 << 2)
#define CCGR6_CG0_USBOH3                        (0x3 << 0)

#define CSCMR1_SSI1_CLK_SEL_MASK                (0x3<<10)
#define CSCMR1_SSI1_CLK_SEL_PLL4                (0x2<<10)

#define CSCMR1_SSI2_CLK_SEL_MASK                (0x3<<12)
#define CSCMR1_SSI2_CLK_SEL_PLL4                (0x2<<12)

#define CSCMR1_SSI3_CLK_SEL_MASK                (0x3<<14)
#define CSCMR1_SSI3_CLK_SEL_PLL4                (0x2<<14)

/*
 * Values passed to SSI clock divisor macros should be the exact
 * divisor value, not the register value.  E.g. SSI_CLK_PRED(1) will
 * divide by 1, which will set the ssiX_clk_pred pre-dividor to bit value 0
 * which corresponds to divide by 1.
 */
#define SSI1_CLK_DIV_MASK                       (0x1ff)
#define SSI1_CLK_PRED(x)                        ((x - 1) << 6)
#define SSI1_CLK_PODF(x)                        ((x - 1) << 0)
#define SSI2_CLK_DIV_MASK                       (0x1ff)
#define SSI2_CLK_PRED(x)                        ((x - 1) << 6)
#define SSI2_CLK_PODF(x)                        ((x - 1) << 0)
#define SSI3_CLK_DIV_MASK                       (0x1ff << 16)
#define SSI3_CLK_PRED(x)                        ((x - 1) << 22)
#define SSI3_CLK_PODF(x)                        ((x - 1) << 16)

/*
 * Synchronous Serial Interfaces
 */
#define MX6X_SSI1_BASE                          0x02028000
#define MX6X_SSI2_BASE                          0x0202C000
#define MX6X_SSI3_BASE                          0x02030000

#define MX6X_SSI_SIZE                           0x4000

#define MX6X_SSI1_IRQ                           78
#define MX6X_SSI2_IRQ                           79
#define MX6X_SSI3_IRQ                           80

#define MX6X_SSI_STCCR                          0x24
#define MX6X_SSI_DIV2                           (1 << 18)
#define MX6X_SSI_WORD_LENGTH_16                 (7 << 13)
#define MX6X_SSI_DC(x)                          ((x & 0x1f) << 8)
#define MX6X_SSI_PM(x)                          ((x & 0xff) << 0)



/*
 * Sony/Philips Digital Interface Transmitter
 */
#define MX6X_SPDIF_BASE                         0x02004000
#define MX6X_SPDIF_SIZE                         0x4000

#define MX6X_SPDIF_IRQ                          84
/*
 * I2C
 */
#define MX6X_I2C1_BASE                          0x021A0000
#define MX6X_I2C2_BASE                          0x021A4000
#define MX6X_I2C3_BASE                          0x021A8000

#define MX6X_I2C1_IRQ                           68
#define MX6X_I2C2_IRQ                           69
#define MX6X_I2C3_IRQ                           70

#define MX6X_I2C_SIZE                           0x4000

/*
 * Fast Ethernet Controller
 */
#define MX6X_FEC_BASE                           0x02188000
#define MX6X_FEC_IRQ                            150

/*
 * External Interface Module (EIM)
 */
#define MX6X_EIM_BASE                           0x021B8000
#define MX6X_EIM_CS0GCR1                        0x00
#define MX6X_EIM_CS0GCR2                        0x04
#define MX6X_EIM_CS0RCR1                        0x08
#define MX6X_EIM_CS0RCR2                        0x0C
#define MX6X_EIM_CS0WCR1                        0x10
#define MX6X_EIM_CS0WCR2                        0x14
#define MX6X_EIM_CS1GCR1                        0x18
#define MX6X_EIM_CS1GCR2                        0x1C
#define MX6X_EIM_CS1RCR1                        0x20
#define MX6X_EIM_CS1RCR2                        0x24
#define MX6X_EIM_CS1WCR1                        0x28
#define MX6X_EIM_CS1WCR2                        0x2C
#define MX6X_EIM_CS2GCR1                        0x30
#define MX6X_EIM_CS2GCR2                        0x34
#define MX6X_EIM_CS2RCR1                        0x38
#define MX6X_EIM_CS2RCR2                        0x3C
#define MX6X_EIM_CS2WCR1                        0x40
#define MX6X_EIM_CS2WCR2                        0x44
#define MX6X_EIM_CS3GCR1                        0x48
#define MX6X_EIM_CS3GCR2                        0x4C
#define MX6X_EIM_CS3RCR1                        0x50
#define MX6X_EIM_CS3RCR2                        0x54
#define MX6X_EIM_CS3WCR1                        0x58
#define MX6X_EIM_CS3WCR2                        0x5C
#define MX6X_EIM_CS4GCR1                        0x60
#define MX6X_EIM_CS4GCR2                        0x64
#define MX6X_EIM_CS4RCR1                        0x68
#define MX6X_EIM_CS4RCR2                        0x6C
#define MX6X_EIM_CS4WCR1                        0x70
#define MX6X_EIM_CS4WCR2                        0x74
#define MX6X_EIM_CS5GCR1                        0x78
#define MX6X_EIM_CS5GCR2                        0x7C
#define MX6X_EIM_CS5RCR1                        0x80
#define MX6X_EIM_CS5RCR2                        0x84
#define MX6X_EIM_CS5WCR1                        0x88
#define MX6X_EIM_CS5WCR2                        0x8C
#define MX6X_EIM_WCR                            0x90
#define MX6X_EIM_WIAR                           0x94
#define MX6X_EIM_EAR                            0x98

/*
 * CAN
 */
#define MX6X_CAN1_PORT                          0x02090000
#define MX6X_CAN1_MEM                           0x02090080
#define MX6X_CAN1_IRQ                           142

#define MX6X_CAN2_PORT                          0x02094000
#define MX6X_CAN2_MEM                           0x02094080
#define MX6X_CAN2_IRQ                           143

#define MX6X_CAN_SIZE                           0x4000

/*
 * GPIOs
 */
#define MX6X_GPIO1_BASE                         0x0209C000
#define MX6X_GPIO2_BASE                         0x020A0000
#define MX6X_GPIO3_BASE                         0x020A4000
#define MX6X_GPIO4_BASE                         0x020A8000
#define MX6X_GPIO5_BASE                         0x020AC000
#define MX6X_GPIO6_BASE                         0x020B0000
#define MX6X_GPIO7_BASE                         0x020B4000
#define MX6X_GPIO_SIZE                          0x4000

/* GPIO Registers, offset from base address */
#define MX6X_GPIO_DR                            0x00
#define MX6X_GPIO_GDIR                          0x04
#define MX6X_GPIO_PSR                           0x08
#define MX6X_GPIO_ICR1                          0x0C
#define MX6X_GPIO_ICR2                          0x10
#define MX6X_GPIO_IMR                           0x14
#define MX6X_GPIO_ISR                           0x18
#define MX6X_GPIO_EDR                           0x1C

/*
 * SDMA Base Address
 */
#define MX6X_SDMA_BASE                          0x020EC000
#define MX6X_SDMA_IRQ                           34
#define MX6X_SDMA_SIZE                          0x4000

/* SDMA Registers, offset from base address */
#define MX6X_SDMA_MC0PTR                        0x00	/* AP (MCU) Channel 0 Pointer */
#define MX6X_SDMA_INTR                          0x04	/* Channel Interrupts */
#define MX6X_SDMA_STOP_STAT                     0x08	/* Channel Stop / Channel Status */
#define MX6X_SDMA_HSTART                        0x0C	/* Channel Start */
#define MX6X_SDMA_EVTOVR                        0x10	/* Channel Event Override */
#define MX6X_SDMA_DSPOVR                        0x14	/* Channel DSP (BP) Override */
#define MX6X_SDMA_HOSTOVR                       0x18	/* Channel AP Override */
#define MX6X_SDMA_EVTPEND                       0x1C	/* Channel Event Pending */
#define MX6X_SDMA_RESET                         0x24	/* Reset Register */
#define MX6X_SDMA_EVTERR                        0x28	/* DMA Request Error Register */
#define MX6X_SDMA_INTRMASK                      0x2C	/* Channel AP Interrupt Mask */
#define MX6X_SDMA_PSW                           0x30	/* Schedule Status */
#define MX6X_SDMA_EVTERRDBG                     0x34	/* DMA Request Error Register */
#define MX6X_SDMA_CONFIG                        0x38	/* Configuration Register */
#define MX6X_SDMA_ONCE_ENB                      0x40	/* OnCE Enable */
#define MX6X_SDMA_ONCE_DATA                     0x44	/* OnCE Data Register */
#define MX6X_SDMA_ONCE_INSTR                    0x48	/* OnCE Instruction Register */
#define MX6X_SDMA_ONCE_STAT                     0x4C	/* OnCE Status Register */
#define MX6X_SDMA_ONCE_CMD                      0x50	/* OnCE Command Register */
#define MX6X_SDMA_EVT_MIRROT                    0x54	/* DMA Request */
#define MX6X_SDMA_ILLINSTADDR                   0x58	/* Illegal Instruction Trap Address */
#define MX6X_SDMA_CHN0ADDR                      0x5C	/* Channel 0 Boot Address */
#define MX6X_SDMA_XTRIG_CONF1                   0x70	/* Cross-Trigger Events Configuration Register 1 */
#define MX6X_SDMA_XTRIG_CONF2                   0x74	/* Cross-Trigger Events Configuration Register 2 */
#define MX6X_SDMA_CHNPRI(n)                     (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */
#define MX6X_SDMA_CHNENBL(n)                    (0x200 + ((n)<<2))   /* Channel Enable n = 0 to 47 */

/*
 * Watchdogs
 */
#define MX6X_WDOG1_BASE                         0x020BC000
#define MX6X_WDOG2_BASE                         0x020C0000
#define MX6X_WDOG_SIZE                          0x4000

/*
 * Watchdog Timer Registers, offset from base address 
 */
#define MX6X_WDOG_CR                            0x00
// Set default time out value to 30 seconds
#define WT_MASK                                 (0xff << 8)
#define WT                                      (0x3B << 8)
#define WDT                                     (0x01 << 3)
#define WDE                                     (0x01 << 2)
#define WDZST                                   (0x01)
#define MX6X_WDOG_SR                            0x02
#define MX6X_WDOG_RSR                           0x04
#define MX6X_WDOG_ICR                           0x06
#define MX6X_WDOG_MCR                           0x08

/*
 * General Purpose Timer
 */
#define MX6X_GPT_BASE                           0x02098000
#define MX6X_GPT_SIZE                           0x4000

/*
 * Enhanced Serial Audio Interface (ESAI)
 */
#define MX6X_ESAI_BASE                           0x02024000
#define MX6X_ESAI_SIZE                           0x4000

/*
 * I/O MUX Controller
 */
#define MX6X_IOMUXC_BASE                        0x020E0000
#define MX6X_IOMUXC_SIZE                        0x4000

/*
 * IOMUX Registers, offset from base address
 */
#define MX6X_IOMUX_GPR0                         0x0000
#define MX6X_IOMUX_GPR1                         0x0004
#define MX6X_IOMUX_GPR2                         0x0008
#define MX6X_IOMUX_GPR3                         0x000C
#define MX6X_IOMUX_GPR4                         0x0010
#define MX6X_IOMUX_GPR5                         0x0014
#define MX6X_IOMUX_GPR6                         0x0018
#define MX6X_IOMUX_GPR7                         0x001C
#define MX6X_IOMUX_GPR8                         0x0020
#define MX6X_IOMUX_GPR9                         0x0024
#define MX6X_IOMUX_GPR10                        0x0028
#define MX6X_IOMUX_GPR11                        0x002C
#define MX6X_IOMUX_GPR12                        0x0030
#define MX6X_IOMUX_GPR13                        0x0034
#define MX6X_IOMUXC_OBSERVE_MUX_0               0x0038
#define MX6X_IOMUXC_OBSERVE_MUX_1               0x003C
#define MX6X_IOMUXC_OBSERVE_MUX_2               0x0040
#define MX6X_IOMUXC_OBSERVE_MUX_3               0x0044
#define MX6X_IOMUXC_OBSERVE_MUX_4               0x0048

#define MX6X_IOMUX_SWMUX                        0x004C
#define MX6X_IOMUX_SWPAD                        0x0360
#define MX6X_IOMUX_SWINPUT                      0x07B0

/* 
 * i.MX6 DualLite/Solo have different IOMUX register offsets
 * than i.MX6 Dual/Quad.
 */
#define MX6SDL_IOMUX_SWMUX                        0x004C
#define MX6SDL_IOMUX_SWPAD                        0x0360
#define MX6SDL_IOMUX_SWINPUT                      0x0790

/*
 * AHB to IP Bridge (Trust Zone) (AIPSTZ)
 */
#define MX6X_AIPS1_CONFIG                      0x0207C000
#define MX6X_AIPS2_CONFIG                      0x0217C000
#define MX6X_AIPS_MPR0                         0x00
#define MX6X_AIPS_MPR1                         0x04
#define MX6X_AIPS_OPACR0                       0x40
#define MX6X_AIPS_OPACR1                       0x44
#define MX6X_AIPS_OPACR2                       0x48
#define MX6X_AIPS_OPACR3                       0x4C
#define MX6X_AIPS_OPACR4                       0x50

/*
 * Enhanced Periodic Timers
 */
#define MX6X_EPIT1_BASE                         0x020D0000
#define MX6X_EPIT2_BASE                         0x020D4000
#define MX6X_EPIT_SIZE                          0x4000

#define MX6X_EPIT1_IRQ                          88
#define MX6X_EPIT2_IRQ                          89

/*
 * EPIT Registers, off set from base address
 */
#define MX6X_EPIT_CR                            0x00    /* Control Register */
#define MX6X_EPIT_SR                            0x04    /* Status Register */
#define MX6X_EPIT_LR                            0x08    /* Load Register */
#define MX6X_EPIT_CMPR                          0x0C    /* Compare Register */
#define MX6X_EPIT_CNR                           0x10    /* Counter Register */

/*
 * SDRAM
 */
#define MX6X_SDRAM_64_BIT_BASE                  0x10000000
#define MX6X_SDRAM_DUAL_FIXED_P0_BASE           0x80000000
#define MX6X_SDRAM_DUAL_FIXED_P1_BASE           0x10000000
#define MX6X_SDRAM_DUAL_INRL_BASE               0x10000000

/*
 * MLB
 */
#define MX6X_MLB_BASE                           0x0218C000
#define MX6X_MLB_SIZE                           0x4000
#define MX6X_MLB_IRQ                            85

/*
 * Multimedia Peripherals
 */

/* Graphics */
#define MX6X_GPU2D_BASE                         0x00134000
#define MX6X_GPU2D_SIZE                         0x4000
#define MX6X_GPU3D_BASE                         0x00130000
#define MX6X_GPU3D_SIZE                         0x4000

#define MX6X_GPU3D_IRQ                          41
#define MX6X_GPU2D_IRQ                          42
#define MX6X_OPENVG_XAQ2                        43


/* Image Processing: IPUv3M */
#define MX6X_IPU1_BASE                          0x02400000

#define MX6X_IPU1_ERR_IRQ                       37
#define MX6X_IPU1_SYNC_IRQ                      38
#define MX6X_IPU2_BASE                          0x02800000

#define MX6X_IPU_SIZE                           0x400000

#define MX6X_IPU2_ERR_IRQ                       39
#define MX6X_IPU2_SYNC_IRQ                      40

/* Video Processing: VPU */
#define MX6X_VPU_BASE                           0x02040000
#define MX6X_VPU_SIZE                           0x3C000
#define MX6X_VPU_IRQ                            44

/*
  * TODO, waiting for next TRM
  * IC Identification Module (IIM)
  */
#define MX6X_IIM_BASE
#define MX6X_IIM_STAT                           0x0
#define MX6X_IIM_STATM                          0x4
#define MX6X_IIM_ERR                            0x8
#define MX6X_IIM_EMASK                          0xC
#define MX6X_IIM_FCTL                           0x10
#define MX6X_IIM_UA                             0x14
#define MX6X_IIM_LA                             0x18
#define MX6X_IIM_SDAT                           0x1C
#define MX6X_IIM_PREV                           0x20
#define MX6X_IIM_SREV                           0x24
#define MX6X_IIM_PREG_P                         0x28
#define MX6X_IIM_SCS0                           0x2C
#define MX6X_IIM_SCS2                           0x34
#define MX6X_IIM_SCS3                           0x38

/*
 * On-Chip eFuse OTP Controller
 */
#define MX6X_OCOTP_BASE                         0x021BC000

/*
  * System Reset Controller (SRC)
  */
#define MX6X_SRC_BASE                           0x020D8000
#define MX6X_SRC_SCR                            0x000
#define MX6X_SRC_SBMR                           0x004
#define MX6X_SRC_SRSR                           0x008
#define MX6X_SRC_SAIAR                          0x00c
#define MX6X_SRC_SAIRAR                         0x010
#define MX6X_SRC_SISR                           0x014
#define MX6X_SRC_SIMR                           0x018
#define MX6X_SRC_SBMR2                          0x01c
#define MX6X_SRC_GPR1                           0x020
#define MX6X_SRC_GPR2                           0x024
#define MX6X_SRC_GPR3                           0x028
#define MX6X_SRC_GPR4                           0x02c
#define MX6X_SRC_GPR5                           0x030
#define MX6X_SRC_GPR6                           0x034
#define MX6X_SRC_GPR7                           0x038
#define MX6X_SRC_GPR8                           0x03c
#define MX6X_SRC_GPR9                           0x040
#define MX6X_SRC_GPR10                          0x044

#define BP_SRC_SCR_CORE0_RST                     13
#define BP_SRC_SCR_CORES_DBG_RST                 21
#define BP_SRC_SCR_CORE1_ENABLE                  22


#endif	/* __ARM_MX6X_H_INCLUDED */

/* __SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/public/arm/mx6x.h $ $Rev: 657836 $" ) */
