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
 * Compaq iPaq Specifics
 *

 *
 */
#ifndef ARM_IPAQ_H_INCLUDED
#define ARM_IPAQ_H_INCLUDED

#include <sys/neutrino.h>
#include <hw/sysinfo.h>
#include <hw/inout.h>
#include <arm/sa1100.h>


#define IPAQ_MACHINE_3100 "ipaq-3100"
#define IPAQ_MACHINE_3600 "ipaq-3600"
#define IPAQ_MACHINE_3800 "ipaq-3800"
#define IPAQ_MACHINE_3900 "ipaq-3900"

/*
 *
 *  iPaq 3100/3600/3700 EGPIO Register and Bit Definitions
 *
 */

#define IPAQ_EGPIO_BASE         0x49000000

#define IPAQ_EGPIO_VPEN         0x00000001  /* Flash Write Enable */
#define IPAQ_EGPIO_CARD_RESET   0x00000002  /* CF/PCMCIA Card Reset */
#define IPAQ_EGPIO_OPT_RESET    0x00000004  /* Expansion Pack Reset */
#define IPAQ_EGPIO_CODEC_RESET  0x00000008  /* Onboard CODEC Reset (low) */
#define IPAQ_EGPIO_OPT_POWER_ON 0x00000010  /* Expansion Pack NVRAM Power */ 
#define IPAQ_EGPIO_OPT_ON       0x00000020  /* Expansion Pack Full Power */ 
#define IPAQ_EGPIO_LCD_ON       0x00000040  /* LCD 3.3v Power Enable */ 
#define IPAQ_EGPIO_RS232_ON     0x00000080  /* RS232 Tranceiver Enable */
#define IPAQ_EGPIO_LCD_PCI      0x00000100  /* LCD Control IC Enable */
#define IPAQ_EGPIO_IR_ON        0x00000200  /* IR Module Enable */
#define IPAQ_EGPIO_AUD_ON       0x00000400  /* Audio Output Amp. Enable */
#define IPAQ_EGPIO_AUD_PWR_ON   0x00000800  /* Audio IC Power Enable */
#define IPAQ_EGPIO_QMUTE        0x00001000  /* Mute Onboard CODEC */
#define IPAQ_EGPIO_IR_FSEL      0x00002000  /* FIR Mode Select 1=FIR,0=SIR */
#define IPAQ_EGPIO_LCD_5V_ON    0x00004000  /* LCD 5v Power Enable */
#define IPAQ_EGPIO_LVDD_ON      0x00008000  /* LCD 9v and -6.5v Power Enable */

/*
 * Default value for EGPIO set by startup program
 */
#define IPAQ_EGPIO_DEFAULT      \
    (                           \
        IPAQ_EGPIO_RS232_ON     \
    )

#define IPAQ_EGPIO_LCD_ENABLE       \
    (                               \
        IPAQ_EGPIO_LCD_ON       |   \
        IPAQ_EGPIO_LCD_PCI      |   \
        IPAQ_EGPIO_LCD_5V_ON    |   \
        IPAQ_EGPIO_LVDD_ON          \
    )


/*
 *
 *  iPaq 3800/3900 ASIC Registers and Bit Definitions
 *
 */

/*
 ASIC1 and ASIC2 share the same address space.
*/
#define IPAQ_ASIC1_BASE         IPAQ_EGPIO_BASE
#define IPAQ_ASIC2_BASE         IPAQ_EGPIO_BASE

/*
 ASIC1 GPIO Control Registers
*/
#define IPAQ_ASIC1_GPIO_MASK         0x1e60  
#define IPAQ_ASIC1_GPIO_DIR          0x1e64  
#define IPAQ_ASIC1_GPIO_OUT          0x1e68  
#define IPAQ_ASIC1_GPIO_LEVELTRI     0x1e6C  
#define IPAQ_ASIC1_GPIO_RISING       0x1e70  
#define IPAQ_ASIC1_GPIO_LEVEL        0x1e74  
#define IPAQ_ASIC1_GPIO_LEVELSTATUS  0x1e78  
#define IPAQ_ASIC1_GPIO_EDGESTATUS   0x1e7C  
#define IPAQ_ASIC1_GPIO_STATE        0x1e80  
#define IPAQ_ASIC1_GPIO_RESET        0x1e84  
#define IPAQ_ASIC1_GPIO_SLEEPMASK    0x1e88  
#define IPAQ_ASIC1_GPIO_SLEEPDIR     0x1e8C  
#define IPAQ_ASIC1_GPIO_SLEEPOUT     0x1e90
#define IPAQ_ASIC1_GPIO_STATUS       0x1e94
#define IPAQ_ASIC1_GPIO_BATFAULTDIR  0x1e98
#define IPAQ_ASIC1_GPIO_BATFAULTOUT  0x1e9C

/*
 ASIC1 GPIO Bit Definitions
*/
#define IPAQ_ASIC1_IR_ON       (1<<0)    /* IrDA Phyiscal Layer On/OFF */
#define IPAQ_ASIC1_SD_PWR_ON   (1<<1)    /* SD/MMC Power On */
#define IPAQ_ASIC1_RS232_ON    (1<<2)    /* Main RS232 (ser3) On */
#define IPAQ_ASIC1_PULSE_GEN   (1<<3)    /* ??? */
#define IPAQ_ASIC1_CH_TIMER    (1<<4)    /* Battery Charging Enable */
#define IPAQ_ASIC1_LCD_5V_ON   (1<<5)    /* LCD 5v On */
#define IPAQ_ASIC1_LCD_ON      (1<<6)    /* LCD On */
#define IPAQ_ASIC1_LCD_PCI     (1<<7)    /* LCD Power On */
#define IPAQ_ASIC1_VGH_ON      (1<<8)    /* ??? */
#define IPAQ_ASIC1_VGL_ON      (1<<9)    /* ??? */
#define IPAQ_ASIC1_FL_PWR_ON   (1<<10)   /* Front Light Power On */
#define IPAQ_ASIC1_BT_PWR_ON   (1<<11)   /* Bluetooth Power On */
#define IPAQ_ASIC1_SPK_ON      (1<<12)   /* Speaker On */
#define IPAQ_ASIC1_EAR_ON      (1<<13)   /* Earphones On */
#define IPAQ_ASIC1_AUD_PWR_ON  (1<<14)   /* Audio Power On */

#define IPAQ_ASIC1_GPIO_DEFAULT \
    ( \
        IPAQ_ASIC1_RS232_ON \
    )

#define IPAQ_ASIC1_GPIO_LCD_ENABLE \
    ( \
        IPAQ_ASIC1_LCD_5V_ON    | \
        IPAQ_ASIC1_LCD_ON       | \
        IPAQ_ASIC1_LCD_PCI      | \
        IPAQ_ASIC1_VGH_ON       | \
        IPAQ_ASIC1_VGL_ON       | \
        IPAQ_ASIC1_FL_PWR_ON      \
    )

/*
 ASIC1 MMC/SD Control Registers
*/
#define IPAQ_ASIC1_MMC_STRSTPCLK     0x1c00
#define IPAQ_ASIC1_MMC_STATUS        0x1c04
#define IPAQ_ASIC1_MMC_CLKRATE       0x1c08
#define IPAQ_ASIC1_MMC_RESV1         0x1c0c
#define IPAQ_ASIC1_MMC_SPIREG        0x1c10
#define IPAQ_ASIC1_MMC_CMDDATCONT    0x1c14
#define IPAQ_ASIC1_MMC_RESPTO        0x1c18
#define IPAQ_ASIC1_MMC_READTO        0x1c1c
#define IPAQ_ASIC1_MMC_BLKLEN        0x1c20
#define IPAQ_ASIC1_MMC_NOB           0x1c24
#define IPAQ_ASIC1_MMC_INTMASK       0x1c34
#define IPAQ_ASIC1_MMC_CMD           0x1c38
#define IPAQ_ASIC1_MMC_ARGH          0x1c3C
#define IPAQ_ASIC1_MMC_ARGL          0x1c40
#define IPAQ_ASIC1_MMC_RESFIFO       0x1c44
#define IPAQ_ASIC1_MMC_DATABUF       0x1c48
#define IPAQ_ASIC1_MMC_BUFPARTFULL   0x1c40


/*
 ASIC2's GPIO Control Registers (offsets from IPAQ_ASIC2_BASE)
*/
#define IPAQ_ASIC2_GPIODIR      0x0000  /* Direction */
#define IPAQ_ASIC2_GPIINTTYP    0x0004  /* Interrupt Type (edge 1, level 0) */
#define IPAQ_ASIC2_GPIINTESEL   0x0008  /* Active Edge ( rising 1, falling 0 ) */
#define IPAQ_ASIC2_GPIINTALSEL  0x000C  /* Active Level ( high 1, low 0 ) */
#define IPAQ_ASIC2_GPIINTCLR    0x0010  /* Clear */
#define IPAQ_ASIC2_GPIINTFLAG   0x0010  /* Active */
#define IPAQ_ASIC2_GPIOPIOD     0x0014  /* Read/Write GPIO Values */
#define IPAQ_ASIC2_GPOBFSTAT    0x0018  /* Battery Fault State */
#define IPAQ_ASIC2_GPIINTSTAT   0x001C  /* Interrupt Enabling */
#define IPAQ_ASIC2_GPIOALT      0x003C  /* Alternate Function Enables */

#define IPAQ_ASIC2_GPIO_INY1      (1<<0)
#define IPAQ_ASIC2_GPIO_INX0      (1<<1)
#define IPAQ_ASIC2_GPIO_INY0      (1<<2)
#define IPAQ_ASIC2_GPIO_INX1      (1<<3)
#define IPAQ_ASIC2_GPIO_BTRST     (1<<4)
#define IPAQ_ASIC2_GPIO_PENIRQ    (1<<5)
#define IPAQ_ASIC2_GPIO_SDDETECT  (1<<6)
#define IPAQ_ASIC2_GPIO_EARIN     (1<<7)
#define IPAQ_ASIC2_GPIO_OPTPCMRST (1<<8)
#define IPAQ_ASIC2_GPIO_OPTRST    (1<<9)
#define IPAQ_ASIC2_GPIO_USBDETECT (1<<10)
#define IPAQ_ASIC2_GPIO_SDCON     (1<<11)
#define IPAQ_ASIC2_GPIO_OJON      (1<<12)
#define IPAQ_ASIC2_GPIO_OPTNVRAM  (1<<13)

#define IPAQ_ASIC2_GPIO_INMASK \
    ( \
        IPAQ_ASIC2_GPIO_INY1 | \
        IPAQ_ASIC2_GPIO_INX0 | \
        IPAQ_ASIC2_GPIO_INY0 | \
        IPAQ_ASIC2_GPIO_INX1   \
    )

#define IPAQ_ASIC2_GPIO_INPUTS \
    ( \
        IPAQ_ASIC2_GPIO_PENIRQ    | \
        IPAQ_ASIC2_GPIO_SDDETECT  | \
        IPAQ_ASIC2_GPIO_EARIN     | \
        IPAQ_ASIC2_GPIO_USBDETECT | \
        IPAQ_ASIC2_GPIO_SDCON       \
    )

/*
 ASIC2's KPIO Control Register (Keypad Inputs)
*/
#define IPAQ_ASIC2_KPIODIR      0x0200
#define IPAQ_ASIC2_KPIINTTYP    0x0204
#define IPAQ_ASIC2_KPIINTESEL   0x0208
#define IPAQ_ASIC2_KPIINTALSEL  0x020C
#define IPAQ_ASIC2_KPIINTCLR    0x0210
#define IPAQ_ASIC2_KPIINTFLAG   0x0210
#define IPAQ_ASIC2_KPIOPIOD     0x0214
#define IPAQ_ASIC2_KPOBFSTAT    0x0218
#define IPAQ_ASIC2_KPIINTSTAT   0x021C
#define IPAQ_ASIC2_KALT         0x023C

#define IPAQ_ASIC2_KPIO_RECORD    (1<<0)
#define IPAQ_ASIC2_KPIO_5WAY1     (1<<1)
#define IPAQ_ASIC2_KPIO_5WAY2     (1<<2)
#define IPAQ_ASIC2_KPIO_5WAY3     (1<<3)
#define IPAQ_ASIC2_KPIO_5WAY4     (1<<4)
#define IPAQ_ASIC2_KPIO_5WAY5     (1<<5)
#define IPAQ_ASIC2_KPIO_KEYLEFT   (1<<6)
#define IPAQ_ASIC2_KPIO_KEYRIGHT  (1<<7)
#define IPAQ_ASIC2_KPIO_APP1      (1<<8)
#define IPAQ_ASIC2_KPIO_APP2      (1<<9)
#define IPAQ_ASIC2_KPIO_APP3      (1<<10)
#define IPAQ_ASIC2_KPIO_APP4      (1<<11)

#define IPAQ_ASIC2_KPIO_INPUTS \
    ( \
        IPAQ_ASIC2_KPIO_RECORD   | \
        IPAQ_ASIC2_KPIO_5WAY1    | \
        IPAQ_ASIC2_KPIO_5WAY2    | \
        IPAQ_ASIC2_KPIO_5WAY3    | \
        IPAQ_ASIC2_KPIO_5WAY4    | \
        IPAQ_ASIC2_KPIO_5WAY5    | \
        IPAQ_ASIC2_KPIO_KEYLEFT | \
        IPAQ_ASIC2_KPIO_KEYRIGHT| \
        IPAQ_ASIC2_KPIO_APP1     | \
        IPAQ_ASIC2_KPIO_APP2     | \
        IPAQ_ASIC2_KPIO_APP3     | \
        IPAQ_ASIC2_KPIO_APP4       \
    )

/*
 ASIC2 uses some of the free upper bits in the KPIO registers for
  interrupt status and clearing of non-kpio bits as well.
*/
#define IPAQ_ASIC2_KPIO_TIMER2   (1<<23)
#define IPAQ_ASIC2_KPIO_TIMER1   (1<<22)
#define IPAQ_ASIC2_KPIO_TIMER0   (1<<21)
#define IPAQ_ASIC2_KPIO_UART1    (1<<20)
#define IPAQ_ASIC2_KPIO_UART0    (1<<19)
#define IPAQ_ASIC2_KPIO_ADC      (1<<18)
#define IPAQ_ASIC2_KPIO_OWM      (1<<17)
#define IPAQ_ASIC2_KPIO_SPI      (1<<16)


/*
 ASIC2 Clock Controler
*/
#define IPAQ_ASIC2_CDEX         0x1000
  #define IPAQ_ASIC2_CDEX_33MHZ         (1<<15)
  #define IPAQ_ASIC2_CDEX_24MHZ         (1<<14)
  #define IPAQ_ASIC2_CDEX_32KHZ         (1<<13)
  #define IPAQ_ASIC2_CDEX_PWM           (1<<7)
  #define IPAQ_ASIC2_CDEX_OWM           (1<<6)
  #define IPAQ_ASIC2_CDEX_SPI           (1<<5)
  #define IPAQ_ASIC2_CDEX_ADC           (1<<4)
  #define IPAQ_ASIC2_CDEX_AUDIO_44Khz   (1<<3)
  #define IPAQ_ASIC2_CDEX_AUDIO_11Khz   (1<<2)
  #define IPAQ_ASIC2_CDEX_AUDIO_48Khz   (1<<1)
  #define IPAQ_ASIC2_CDEX_AUDIO_8Khz    (1<<0)
  #define IPAQ_ASIC2_CDEX_AUDIO_MASK    (0xf)         

#define IPAQ_ASIC2_CDEX_DEFAULT \
    ( \
        IPAQ_ASIC2_CDEX_33MHZ | \
        IPAQ_ASIC2_CDEX_24MHZ | \
        IPAQ_ASIC2_CDEX_32KHZ | \
        IPAQ_ASIC2_CDEX_PWM   | \
        IPAQ_ASIC2_CDEX_OWM   | \
        IPAQ_ASIC2_CDEX_SPI   | \
        IPAQ_ASIC2_CDEX_ADC     \
    )

/*
 ASIC2 UART(s) Registers
*/
#define IPAQ_ASIC2_UART0_BASE   0x0a00
#define IPAQ_ASIC2_UART1_BASE   0x0c00

/*
 ASIC2 SPI Registers
*/
#define IPAQ_ASIC2_SPICR        0x0400  /* control */
    #define IPAQ_ASIC2_SPICR_CPOL       (1<<7)
    #define IPAQ_ASIC2_SPICR_SEL        (1<<6)
    #define IPAQ_ASIC2_SPICR_SPIE       (1<<5)
    #define IPAQ_ASIC2_SPICR_SPE        (1<<4)
    #define IPAQ_ASIC2_SPICR_64KHZ      (0x3)

#define IPAQ_ASIC2_SPIDR        0x0404  /* data */
#define IPAQ_ASIC2_SPIDCS       0x0408  /* chip select disable */

/*
 ASIC2 ADC Registers
*/
#define IPAQ_ASIC2_ADMUX        0x1200  /* multiplexer select */
   #define IPAQ_ASIC2_ADMUX_CLKEN       (1<<3)
   #define IPAQ_ASIC2_ADMUX_AIN4        (0x4)
   #define IPAQ_ASIC2_ADMUX_AIN3        (0x3)
   #define IPAQ_ASIC2_ADMUX_AIN2        (0x2)
   #define IPAQ_ASIC2_ADMUX_AIN1        (0x1)
   #define IPAQ_ASIC2_ADMUX_AIN0        (0x0)

#define IPAQ_ASIC2_ADCSR        0x1204  /* control and status */
   #define IPAQ_ASIC2_ADCSR_ENABLE      (1<<7)
   #define IPAQ_ASIC2_ADCSR_START       (1<<6)
   #define IPAQ_ASIC2_ADCSR_INTEN       (1<<5)
   #define IPAQ_ASIC2_ADCSR_FREERUN     (1<<4)

#define IPAQ_ASIC2_ADCDR        0x1208   /* data */

#define IPAQ_ASIC2_ADC_LIGHTSENSOR   IPAQ_ASIC2_ADMUX_AIN0
#define IPAQ_ASIC2_ADC_IMIN          IPAQ_ASIC2_ADMUX_AIN1
#define IPAQ_ASIC2_ADC_VSMBAT        IPAQ_ASIC2_ADMUX_AIN2
#define IPAQ_ASIC2_ADC_TOUCHx        IPAQ_ASIC2_ADMUX_AIN3
#define IPAQ_ASIC2_ADC_TOUCHy        IPAQ_ASIC2_ADMUX_AIN4

/*
 ASIC2 PWM Registers
*/
#define IPAQ_ASIC2_PWM0_PWMTBS  0x0600  /* time base set */
#define IPAQ_ASIC2_PWM0_PWMPTS  0x0604  /* period time set */
#define IPAQ_ASIC2_PWM0_PWMDTS  0x0608  /* duty time set */

#define IPAQ_ASIC2_PWM1_PWMTBS  0x0700  /* time base set */
#define IPAQ_ASIC2_PWM1_PWMPTS  0x0704  /* period time set */
#define IPAQ_ASIC2_PWM1_PWMDTS  0x0708  /* duty time set */

#define IPAQ_ASIC2_PWMTBS_CLR      (1<<5)
#define IPAQ_ASIC2_PWMTBS_CLKEN    (1<<4)
#define IPAQ_ASIC2_PWMTBS_BASE(a)  (a&0xf)

/*
 ASIC2 LED Registers
*/
#define IPAQ_ASIC2_LED0_LEDTBS  0x0800  /* time base set */
#define IPAQ_ASIC2_LED0_LEDPTS  0x0804  /* period time set */
#define IPAQ_ASIC2_LED0_LEDDTS  0x0808  /* duty time set */
#define IPAQ_ASIC2_LED0_LEDASTC 0x080C  /* auto stop count */

#define IPAQ_ASIC2_LED1_LEDTBS  0x0880  /* time base set */
#define IPAQ_ASIC2_LED1_LEDPTS  0x0884  /* period time set */
#define IPAQ_ASIC2_LED1_LEDDTS  0x0888  /* duty time set */
#define IPAQ_ASIC2_LED1_LEDASTC 0x088C  /* auto stop count */

#define IPAQ_ASIC2_LED2_LEDTBS  0x0900  /* time base set */
#define IPAQ_ASIC2_LED2_LEDPTS  0x0904  /* period time set */
#define IPAQ_ASIC2_LED2_LEDDTS  0x0908  /* duty time set */
#define IPAQ_ASIC2_LED2_LEDASTC 0x090C  /* auto stop count */

#define IPAQ_ASIC_LEDTBS_LEDALW         (1<<6)
#define IPAQ_ASIC_LEDTBS_LEDSTP         (1<<5)
#define IPAQ_ASIC_LEDTBS_LEDEN          (1<<4)

/*
 ASIC2 Interrupt Control Registers
*/
#define IPAQ_ASIC2_INTMASK      0x1600  /* interrupt mask control */
   #define IPAQ_ASIC2_INTMASK_OWM      (1<<7)
   #define IPAQ_ASIC2_INTMASK_TIMER    (1<<6)
   #define IPAQ_ASIC2_INTMASK_UART1    (1<<5)
   #define IPAQ_ASIC2_INTMASK_UART0    (1<<4)
   #define IPAQ_ASIC2_INTMASK_EXTR     (1<<2)
   #define IPAQ_ASIC2_INTMASK_POR      (1<<1)
   #define IPAQ_ASIC2_INTMASK_GLOBAL   (1<<0)

#define IPAQ_ASIC2_INTCPS       0x1604  /* interrupt timer clock pre-scale */
   #define IPAQ_ASIC2_INTCPS_ENABLE    (1<<4)
   #define IPAQ_ASIC2_INTCPS_30us      0xE
   #define IPAQ_ASIC2_INTCPS_61us      0xD
   #define IPAQ_ASIC2_INTCPS_500ms     0x0

#define IPAQ_ASIC2_INTTBS       0x1608  /* interrupt timer set */


/*
 ASIC2 OWM Interface (one-wire bus)
*/
#define IPAQ_ASIC2_OWMCMD       0x1800  /* command register */
#define IPAQ_ASIC2_OWMRXTX      0x1804  /* tx/rx buffer */
#define IPAQ_ASIC2_OWMINT       0x1808  /* interrupt register */
#define IPAQ_ASIC2_OWMINTEN     0x180C  /* interrupt enable */
#define IPAQ_ASIC2_OWMCLKDIV    0x1810  /* clock divisor */

/*
 ASIC2 PIT (8254-esq)
*/
#define IPAQ_ASIC2_PIT          0x0E00  /* command */

/*
 ASIC2 Flash Control
*/
#define IPAQ_ASIC2_FLASH_WP    0x1f00  /* Write Protect (0 WP, 1 R/W) */


/*
 * 
 * SA1110's GPIO to Interrupt mappings
 *
 */

#define IPAQ_POWER_IRQ       SA1100_GPIO_0   /* Main Power Button */
#define IPAQ_MCU_IRQ         SA1100_GPIO_1   /* Microcontroler Signal */
#define IPAQ_CF_SLOT1_IRQ    SA1100_GPIO_10  /* CF/PCMCIA Slot1 IRQ */
#define IPAQ_CF_CARD1_IRQ    SA1100_GPIO_11  /* CF/PCMCIA Card0 IRQ */
#define IPAQ_CF_SLOT0_IRQ    SA1100_GPIO_17  /* CF/PCMCIA Slot0 IRQ */
#define IPAQ_CF_CARD0_IRQ    SA1100_GPIO_21  /* CF/PCMCIA Card0 IRQ */
#define IPAQ_JOY_CENTER_IRQ  SA1100_GPIO_18  /* Joypad Center Pressed */
#define IPAQ_BAT_FAULT_IRQ   SA1100_GPIO_20  /* Battery Fault */
#define IPAQ_EXP_LOCK_IRQ    SA1100_GPIO_22  /* Expansion Pack Locked/Unlocked */
#define IPAQ_EXP_SHARED_IRQ  SA1100_GPIO_24  /* Expansion Pack Shared IRQ */ 
#define IPAQ_EXP_DETECT_IRQ  SA1100_GPIO_27  /* Expansion Detected IRQ */




/*
 * 
 * iPaq 3100/3600/3700 MCU Input Macros
 *
 */

#define IPAQ_MCU_INPUT_GET_X( a )         ( (uint32_t)a & 0x7FF )
#define IPAQ_MCU_INPUT_GET_Y( a )         ( ( (uint32_t)a >> 11 ) & 0x7FF )
#define IPAQ_MCU_INPUT_GET_BUTTONS( a )   ( ( (uint32_t)a >> 22 ) & 0x3FF )

#define IPAQ_MCU_INPUT_X( a )             ( (uint32_t)a & 0x7FF )
#define IPAQ_MCU_INPUT_Y( a )             ( ( (uint32_t)a & 0x7FF ) << 11 )
#define IPAQ_MCU_INPUT_BUTTONS( a )       ( (uint32_t)( (uint32_t)a & 0x3FF ) << 22 )

#define IPAQ_MCU_INPUT( x, y, z ) \
(uint32_t)( (uint32_t)IPAQ_MCU_INPUT_X( x ) | \
            (uint32_t)IPAQ_MCU_INPUT_Y( y ) | \
            (uint32_t)IPAQ_MCU_INPUT_BUTTONS( z ) \
          )


#define IPAQ_MCU_INPUT_BUTTON_PEN         0x001
#define IPAQ_MCU_INPUT_BUTTON_RECORD      0x002
#define IPAQ_MCU_INPUT_BUTTON_CALENDER    0x004
#define IPAQ_MCU_INPUT_BUTTON_PIM         0x008
#define IPAQ_MCU_INPUT_BUTTON_Q           0x010
#define IPAQ_MCU_INPUT_BUTTON_BACK        0x020
#define IPAQ_MCU_INPUT_BUTTON_UP          0x040
#define IPAQ_MCU_INPUT_BUTTON_RIGHT       0x080
#define IPAQ_MCU_INPUT_BUTTON_LEFT        0x100
#define IPAQ_MCU_INPUT_BUTTON_DOWN        0x200


    
/*
 * 
 * HWI Structures and Functions for EGPIO and ASIC2
 *
 */


#define HWI_TAG_NAME_ipaq_egpio "IPAQ_EGPIO"
#define HWI_TAG_ALIGN_ipaq_egpio (sizeof(unsigned))
struct hwi_ipaq_egpio 
{
    struct hwi_item     prefix;
    unsigned            addr;
    unsigned            value;
};

#define HWI_TAG_NAME_ipaq_asic2 "IPAQ_ASIC2"
#define HWI_TAG_ALIGN_ipaq_asic2 (sizeof(unsigned))
struct hwi_ipaq_asic2 
{
    struct hwi_item     prefix;
    unsigned            addr;
    unsigned            value;
};


/*
 * Locate EGPIO from syspage hwinfo structure
 */
static __inline__ struct hwi_ipaq_egpio * __attribute__((__unused__))
ipaq_egpio_attach()
{
    unsigned    off;

    off = hwi_find_item(HWI_NULL_OFF, "ipaq_egpio", NULL);
    return hwi_off2tag(off);
}

/*
 * Set a bit in the EGPIO bank
 */
static __inline__ void __attribute__((__unused__))
ipaq_egpio_set(struct hwi_ipaq_egpio *egpio, unsigned bits)
{
    InterruptDisable();
    egpio->value |= bits;
    out32(egpio->addr, egpio->value);
    InterruptEnable();
}

/*
 * Clear a bit in the EGPIO bank
 */
static __inline__ void __attribute__((__unused__))
ipaq_egpio_clr(struct hwi_ipaq_egpio *egpio, unsigned bits)
{
    InterruptDisable();
    egpio->value &= ~bits;
    out32(egpio->addr, egpio->value);
    InterruptEnable();
}

/*
 * Read EGPIO value
 */
static __inline__ unsigned __attribute__((__unused__))
ipaq_egpio_get(struct hwi_ipaq_egpio *egpio)
{
    return egpio->value;
}



/*
 * Locate ASIC2 from syspage hwinfo structure
 */
static __inline__ struct hwi_ipaq_asic2 * __attribute__((__unused__))
ipaq_asic2_attach()
{
    unsigned    off;

    off = hwi_find_item(HWI_NULL_OFF, "ipaq_asic2", NULL);
    return hwi_off2tag(off);
}

/*
 * Set a bit in the ASIC2 GPIO
 */
static __inline__ void __attribute__((__unused__))
ipaq_asic2_set(struct hwi_ipaq_asic2 *asic2, unsigned bits)
{
    InterruptDisable();
    asic2->value |= bits;
    out32(asic2->addr, asic2->value);
    InterruptEnable();
}

/*
 * Clear a bit in the ASIC2 GPIO
 */
static __inline__ void __attribute__((__unused__))
ipaq_asic2_clr(struct hwi_ipaq_asic2 *asic2, unsigned bits)
{
    InterruptDisable();
    asic2->value &= ~bits;
    out32(asic2->addr, asic2->value);
    InterruptEnable();
}

/*
 * Read ASIC2 value
 */
static __inline__ unsigned __attribute__((__unused__))
ipaq_asic2_get(struct hwi_ipaq_asic2 *asic2)
{
    return asic2->value;
}


#endif


/* __SRCVERSION("ipaq.h $Rev: 657836 $"); */
