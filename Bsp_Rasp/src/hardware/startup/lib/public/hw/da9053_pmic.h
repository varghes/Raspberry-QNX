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
 * Dialog Semiconductor DA9053 Power Management Interface Controller (PMIC)
 *    This file contains generic definitions for register locations
 */

#ifndef	_DA9053_PMIC_H_INCLUDED
#define	_DA9053_PMIC_H_INCLUDED


//#######################################################
// THIS SECTOIN CONTAINS INFO ABOUT THE DEVICE REGISTERS
//#######################################################


//----------------------------------------------
// The default 2-wire bus addresses & max speed
//----------------------------------------------
#define DA9052_DEFAULT_2WIRE_ADDRESS         0x48
#define DA9052_DEFAULT_2WIRE_HS_ADDRESS      0x49
#define DA9053_MAX_2WIRE_BUS_SPEED         400000

//---------------------------------------------
// Register Page Address Offsets
// If 4-wire comms, set both to zero
//---------------------------------------------
#define DA9053_PAGE_0_OFFSET      0
#define DA9053_PAGE_1_OFFSET    128

//---------------------------------------------
// Page Control page register address
// This must only be used in 4-wire mode
//---------------------------------------------
#define DA9053_PAGE_CON         0

//---------------------------------------------
// System control register addresses
//---------------------------------------------
#define DA9053_STATUS_A       (  1 + DA9053_PAGE_0_OFFSET)
#define DA9053_STATUS_B       (  2 + DA9053_PAGE_0_OFFSET)
#define DA9053_STATUS_C       (  3 + DA9053_PAGE_0_OFFSET)
#define DA9053_STATUS_D       (  4 + DA9053_PAGE_0_OFFSET)
#define DA9053_EVENT_A        (  5 + DA9053_PAGE_0_OFFSET)
#define DA9053_EVENT_B        (  6 + DA9053_PAGE_0_OFFSET)
#define DA9053_EVENT_C        (  7 + DA9053_PAGE_0_OFFSET)
#define DA9053_EVENT_D        (  8 + DA9053_PAGE_0_OFFSET)
#define DA9053_FAULT_LOG      (  9 + DA9053_PAGE_0_OFFSET)
#define DA9053_IRQ_MASK_A     ( 10 + DA9053_PAGE_0_OFFSET)
#define DA9053_IRQ_MASK_B     ( 11 + DA9053_PAGE_0_OFFSET)
#define DA9053_IRQ_MASK_C     ( 12 + DA9053_PAGE_0_OFFSET)
#define DA9053_IRQ_MASK_D     ( 13 + DA9053_PAGE_0_OFFSET)
#define DA9053_CONTROL_A      ( 14 + DA9053_PAGE_0_OFFSET)
#define DA9053_CONTROL_B      ( 15 + DA9053_PAGE_0_OFFSET)
#define DA9053_CONTROL_C      ( 16 + DA9053_PAGE_0_OFFSET)
#define DA9053_CONTROL_D      ( 17 + DA9053_PAGE_0_OFFSET)
#define DA9053_PD_DIS         ( 18 + DA9053_PAGE_0_OFFSET)
#define DA9053_INTERFACE      ( 19 + DA9053_PAGE_0_OFFSET)
#define DA9053_RESET          ( 20 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// GPIO control register addresses
//---------------------------------------------
#define DA9053_GPIO_00_01     ( 21 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_02_03     ( 22 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_04_05     ( 23 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_06_07     ( 24 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_08_09     ( 25 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_10_11     ( 26 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_12_13     ( 27 + DA9053_PAGE_0_OFFSET)
#define DA9053_GPIO_14_15     ( 28 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Power sequencer control register addresses
//---------------------------------------------
#define DA9053_ID_00_01       ( 29 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_02_03       ( 30 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_04_05       ( 31 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_06_07       ( 32 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_08_09       ( 33 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_10_11       ( 34 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_02_13       ( 35 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_04_15       ( 36 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_06_17       ( 37 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_08_19       ( 38 + DA9053_PAGE_0_OFFSET)
#define DA9053_ID_20_21       ( 39 + DA9053_PAGE_0_OFFSET)
#define DA9053_SEQ_STATUS     ( 40 + DA9053_PAGE_0_OFFSET)
#define DA9053_SEQ_A          ( 41 + DA9053_PAGE_0_OFFSET)
#define DA9053_SEQ_B          ( 42 + DA9053_PAGE_0_OFFSET)
#define DA9053_SEQ_TIMER      ( 43 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Power supply control register addresses
//---------------------------------------------
#define DA9053_BUCK_A         ( 44 + DA9053_PAGE_0_OFFSET)
#define DA9053_BUCK_B         ( 45 + DA9053_PAGE_0_OFFSET)
#define DA9053_BUCKCORE       ( 46 + DA9053_PAGE_0_OFFSET)
#define DA9053_BUCKPRO        ( 47 + DA9053_PAGE_0_OFFSET)
#define DA9053_BUCKMEM        ( 48 + DA9053_PAGE_0_OFFSET)
#define DA9053_BUCKPERI       ( 49 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO1           ( 50 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO2           ( 51 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO3           ( 52 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO4           ( 53 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO5           ( 54 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO6           ( 55 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO7           ( 56 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO8           ( 57 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO9           ( 58 + DA9053_PAGE_0_OFFSET)
#define DA9053_LDO10          ( 59 + DA9053_PAGE_0_OFFSET)
#define DA9053_SUPPLY         ( 60 + DA9053_PAGE_0_OFFSET)
#define DA9053_PULLDOWN       ( 61 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Charging control register addresses
//---------------------------------------------
#define DA9053_CHG_BUCK       ( 62 + DA9053_PAGE_0_OFFSET)
#define DA9053_WAIT_CONT      ( 63 + DA9053_PAGE_0_OFFSET)
#define DA9053_ISET           ( 64 + DA9053_PAGE_0_OFFSET)
#define DA9053_BAT_CHG        ( 65 + DA9053_PAGE_0_OFFSET)
#define DA9053_CHG_CONT       ( 66 + DA9053_PAGE_0_OFFSET)
#define DA9053_INPUT_CONT     ( 67 + DA9053_PAGE_0_OFFSET)
#define DA9053_CHG_TIME       ( 68 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Backup battery charging control register addresses
//---------------------------------------------
#define DA9053_BBAT_CONT      ( 69 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Boost and LED driver control register addresses
//---------------------------------------------
#define DA9053_BOOST          ( 70 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED_CONT       ( 71 + DA9053_PAGE_0_OFFSET)
#define DA9053_LEDMIN         ( 72 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED1_CONF      ( 73 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED2_CONF      ( 74 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED3_CONF      ( 75 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED1_CONT      ( 76 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED2_CONT      ( 77 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED3_CONT      ( 78 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED4_CONT      ( 79 + DA9053_PAGE_0_OFFSET)
#define DA9053_LED5_CONT      ( 80 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// General purpose ADC control register addresses
//---------------------------------------------
#define DA9053_ADC_MAN        ( 81 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADC_CONT       ( 82 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADC_RES_L      ( 83 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADC_RES_H      ( 84 + DA9053_PAGE_0_OFFSET)
#define DA9053_VDD_RES        ( 85 + DA9053_PAGE_0_OFFSET)
#define DA9053_VDD_MON        ( 86 + DA9053_PAGE_0_OFFSET)
#define DA9053_ICHG_AV        ( 87 + DA9053_PAGE_0_OFFSET)
#define DA9053_ICHG_THD       ( 88 + DA9053_PAGE_0_OFFSET)
#define DA9053_ICHG_END       ( 89 + DA9053_PAGE_0_OFFSET)
#define DA9053_TBAT_RES       ( 90 + DA9053_PAGE_0_OFFSET)
#define DA9053_TBAT_HIGHP     ( 91 + DA9053_PAGE_0_OFFSET)
#define DA9053_TBAT_HIGHN     ( 92 + DA9053_PAGE_0_OFFSET)
#define DA9053_TBAT_LOW       ( 93 + DA9053_PAGE_0_OFFSET)
#define DA9053_T_OFFSET       ( 94 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADCIN4_RES     ( 95 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO4_HIGH     ( 96 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO4_LOW      ( 97 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADCIN5_RES     ( 98 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO5_HIGH     ( 99 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO5_LOW      (100 + DA9053_PAGE_0_OFFSET)
#define DA9053_ADCIN6_RES     (101 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO6_HIGH     (102 + DA9053_PAGE_0_OFFSET)
#define DA9053_AUTO6_LOW      (103 + DA9053_PAGE_0_OFFSET)
#define DA9053_TJUNC_RES      (104 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Touch Screen Interface control register addresses
//---------------------------------------------
#define DA9053_TSI_CONT_A     (105 + DA9053_PAGE_0_OFFSET)
#define DA9053_TSI_CONT_B     (106 + DA9053_PAGE_0_OFFSET)
#define DA9053_TSI_X_MSB      (107 + DA9053_PAGE_0_OFFSET)
#define DA9053_TSI_Y_MSB      (108 + DA9053_PAGE_0_OFFSET)
#define DA9053_TSI_LSB        (109 + DA9053_PAGE_0_OFFSET)
#define DA9053_TSI_Z_MSB      (110 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// RTC and calendar register addresses
//---------------------------------------------
#define DA9053_COUNT_S        (111 + DA9053_PAGE_0_OFFSET)
#define DA9053_COUNT_MI       (112 + DA9053_PAGE_0_OFFSET)
#define DA9053_COUNT_H        (113 + DA9053_PAGE_0_OFFSET)
#define DA9053_COUNT_D        (114 + DA9053_PAGE_0_OFFSET)
#define DA9053_COUNT_MO       (115 + DA9053_PAGE_0_OFFSET)
#define DA9053_COUNT_Y        (116 + DA9053_PAGE_0_OFFSET)
#define DA9053_ALARM_MI       (117 + DA9053_PAGE_0_OFFSET)
#define DA9053_ALARM_H        (118 + DA9053_PAGE_0_OFFSET)
#define DA9053_ALARM_D        (119 + DA9053_PAGE_0_OFFSET)
#define DA9053_ALARM_MO       (120 + DA9053_PAGE_0_OFFSET)
#define DA9053_ALARM_Y        (121 + DA9053_PAGE_0_OFFSET)
#define DA9053_SECOND_A       (122 + DA9053_PAGE_0_OFFSET)
#define DA9053_SECOND_B       (123 + DA9053_PAGE_0_OFFSET)
#define DA9053_SECOND_C       (124 + DA9053_PAGE_0_OFFSET)
#define DA9053_SECOND_D       (125 + DA9053_PAGE_0_OFFSET)

//---------------------------------------------
// Customer OTP register addresses
//---------------------------------------------
#define DA9053_CHP_ID         (  1 + DA9053_PAGE_1_OFFSET)
#define DA9053_CONFIG_ID      (  2 + DA9053_PAGE_1_OFFSET)
#define DA9053_OTP_CONT       (  3 + DA9053_PAGE_1_OFFSET)
#define DA9053_OSC_TRIM       (  4 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_0         (  5 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_1         (  6 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_2         (  7 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_3         (  8 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_4         (  9 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_5         ( 10 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_6         ( 11 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_7         ( 12 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_8         ( 13 + DA9053_PAGE_1_OFFSET)
#define DA9053_GPID_9         ( 14 + DA9053_PAGE_1_OFFSET)




// Page Control Register settings  (only used in  4-wire mode)
#define DA9053_PAGE_SELECT_SHIFT                7
#define DA9053_PAGE_SELECT_MASK                (0x01 << DA9053_PAGE_SELECT_SHIFT)

// Status A Register settings
#define DA9053_STATUSA_DCIN_DETECT             (0x01 << 0)
#define DA9053_STATUSA_VBUS_DETECT             (0x01 << 1)
#define DA9053_STATUSA_DCIN_REMOVE             (0x01 << 2)
#define DA9053_STATUSA_VBUS_REMOVE             (0x01 << 3)
#define DA9053_STATUSA_VDD_LOW                 (0x01 << 4)
#define DA9053_STATUSA_RTC_ALARM               (0x01 << 5)
#define DA9053_STATUSA_SEQ_STOP                (0x01 << 6)
#define DA9053_STATUSA_1V2_COMPARE             (0x01 << 7)

// Status B Register settings
#define DA9053_STATUSB_ONKEY                   (0x01 << 0)
#define DA9053_STATUSB_ID_FLOAT                (0x01 << 1)
#define DA9053_STATUSB_ID_GROUND               (0x01 << 2)
#define DA9053_STATUSB_CHG_COMPLETE            (0x01 << 3)
#define DA9053_STATUSB_BAT_TEMP                (0x01 << 4)
#define DA9053_STATUSB_ADC_READY               (0x01 << 5)
#define DA9053_STATUSB_PEN_DOWN                (0x01 << 6)
#define DA9053_STATUSB_TSI_READY               (0x01 << 7)

// Status C Register settings
#define DA9053_STATUSC_GPIO_0                  (0x01 << 0)
#define DA9053_STATUSC_GPIO_1                  (0x01 << 1)
#define DA9053_STATUSC_GPIO_2                  (0x01 << 2)
#define DA9053_STATUSC_GPIO_3                  (0x01 << 3)
#define DA9053_STATUSC_GPIO_4                  (0x01 << 4)
#define DA9053_STATUSC_GPIO_5                  (0x01 << 5)
#define DA9053_STATUSC_GPIO_6                  (0x01 << 6)
#define DA9053_STATUSC_GPIO_7                  (0x01 << 7)

// Status D Register settings
#define DA9053_STATUSD_GPIO_8                  (0x01 << 0)
#define DA9053_STATUSD_GPIO_9                  (0x01 << 1)
#define DA9053_STATUSD_GPIO_10                 (0x01 << 2)
#define DA9053_STATUSD_GPIO_11                 (0x01 << 3)
#define DA9053_STATUSD_GPIO_12                 (0x01 << 4)
#define DA9053_STATUSD_GPIO_13                 (0x01 << 5)
#define DA9053_STATUSD_GPIO_14                 (0x01 << 6)
#define DA9053_STATUSD_GPIO_15                 (0x01 << 7)

// Event A Register settings
#define DA9053_EVENTA_DCIN_DETECT              (0x01 << 0)
#define DA9053_EVENTA_VBUS_DETECT              (0x01 << 1)
#define DA9053_EVENTA_DCIN_REMOVE              (0x01 << 2)
#define DA9053_EVENTA_VBUS_REMOVE              (0x01 << 3)
#define DA9053_EVENTA_VDD_LOW                  (0x01 << 4)
#define DA9053_EVENTA_RTC_ALARM                (0x01 << 5)
#define DA9053_EVENTA_SEQ_STOP                 (0x01 << 6)
#define DA9053_EVENTA_1V2_COMPARE              (0x01 << 7)

// Event B Register settings
#define DA9053_EVENTB_ONKEY                    (0x01 << 0)
#define DA9053_EVENTB_ID_FLOAT                 (0x01 << 1)
#define DA9053_EVENTB_ID_GROUND                (0x01 << 2)
#define DA9053_EVENTB_CHG_COMPLETE             (0x01 << 3)
#define DA9053_EVENTB_BAT_TEMP                 (0x01 << 4)
#define DA9053_EVENTB_ADC_READY                (0x01 << 5)
#define DA9053_EVENTB_PEN_DOWN                 (0x01 << 6)
#define DA9053_EVENTB_TSI_READY                (0x01 << 7)

// Event C Register settings
#define DA9053_EVENTC_GPIO_0                   (0x01 << 0)
#define DA9053_EVENTC_GPIO_1                   (0x01 << 1)
#define DA9053_EVENTC_GPIO_2                   (0x01 << 2)
#define DA9053_EVENTC_GPIO_3                   (0x01 << 3)
#define DA9053_EVENTC_GPIO_4                   (0x01 << 4)
#define DA9053_EVENTC_GPIO_5                   (0x01 << 5)
#define DA9053_EVENTC_GPIO_6                   (0x01 << 6)
#define DA9053_EVENTC_GPIO_7                   (0x01 << 7)

// Event D Register settings
#define DA9053_EVENTD_GPIO_8                   (0x01 << 0)
#define DA9053_EVENTD_GPIO_9                   (0x01 << 1)
#define DA9053_EVENTD_GPIO_10                  (0x01 << 2)
#define DA9053_EVENTD_GPIO_11                  (0x01 << 3)
#define DA9053_EVENTD_GPIO_12                  (0x01 << 4)
#define DA9053_EVENTD_GPIO_13                  (0x01 << 5)
#define DA9053_EVENTD_GPIO_14                  (0x01 << 6)
#define DA9053_EVENTD_GPIO_15                  (0x01 << 7)

// IRQ Mask A Register settings
#define DA9053_IRQMASKA_DCIN_DETECT_DISABLE    (0x01 << 0)
#define DA9053_IRQMASKA_VBUS_DETECT_DISABLE    (0x01 << 1)
#define DA9053_IRQMASKA_DCIN_REMOVE_DISABLE    (0x01 << 2)
#define DA9053_IRQMASKA_VBUS_REMOVE_DISABLE    (0x01 << 3)
#define DA9053_IRQMASKA_VDD_LOW_DISABLE        (0x01 << 4)
#define DA9053_IRQMASKA_RTC_ALARM_DISABLE      (0x01 << 5)
#define DA9053_IRQMASKA_SEQ_STOP_DISABLE       (0x01 << 6)
#define DA9053_IRQMASKA_1V2_COMPARE_DISABLE    (0x01 << 7)

// IRQ Mask B Register settings
#define DA9053_IRQMASKB_ONKEY_DISABLE          (0x01 << 0)
#define DA9053_IRQMASKB_ID_FLOAT_DISABLE       (0x01 << 1)
#define DA9053_IRQMASKB_ID_GROUND_DISABLE      (0x01 << 2)
#define DA9053_IRQMASKB_CHG_COMPLETE_DISABLE   (0x01 << 3)
#define DA9053_IRQMASKB_BAT_TEMP_DISABLE       (0x01 << 4)
#define DA9053_IRQMASKB_ADC_READY_DISABLE      (0x01 << 5)
#define DA9053_IRQMASKB_PEN_DOWN_DISABLE       (0x01 << 6)
#define DA9053_IRQMASKB_TSI_READY_DISABLE      (0x01 << 7)

// IRQ Mask C Register settings
#define DA9053_IRQMASKC_GPIO_0_DISABLE         (0x01 << 0)
#define DA9053_IRQMASKC_GPIO_1_DISABLE         (0x01 << 1)
#define DA9053_IRQMASKC_GPIO_2_DISABLE         (0x01 << 2)
#define DA9053_IRQMASKC_GPIO_3_DISABLE         (0x01 << 3)
#define DA9053_IRQMASKC_GPIO_4_DISABLE         (0x01 << 4)
#define DA9053_IRQMASKC_GPIO_5_DISABLE         (0x01 << 5)
#define DA9053_IRQMASKC_GPIO_6_DISABLE         (0x01 << 6)
#define DA9053_IRQMASKC_GPIO_7_DISABLE         (0x01 << 7)

// IRQ Mask D Register settings
#define DA9053_IRQMASKD_GPIO_8_DISABLE         (0x01 << 0)
#define DA9053_IRQMASKD_GPIO_9_DISABLE         (0x01 << 1)
#define DA9053_IRQMASKD_GPIO_10_DISABLE        (0x01 << 2)
#define DA9053_IRQMASKD_GPIO_11_DISABLE        (0x01 << 3)
#define DA9053_IRQMASKD_GPIO_12_DISABLE        (0x01 << 4)
#define DA9053_IRQMASKD_GPIO_13_DISABLE        (0x01 << 5)
#define DA9053_IRQMASKD_GPIO_14_DISABLE        (0x01 << 6)
#define DA9053_IRQMASKD_GPIO_15_DISABLE        (0x01 << 7)

// GPIO Register settings
#define DA9053_GPIO_EVEN_SHIFT          0
#define DA9053_GPIO_ODD_SHIFT           4
#define DA9053_GPIO_PIN_SHIFT           0
#define DA9053_GPIO_PIN_MASK           (0x03 << DA9053_GPIO_PIN_SHIFT)
#define DA9053_GPIO_TYPE_SHIFT          2
#define DA9053_GPIO_TYPE_MASK          (0x01 << DA9053_GPIO_TYPE_SHIFT)
#define DA9053_GPIO_MODE_SHIFT          3
#define DA9053_GPIO_MODE_MASK          (0x01 << DA9053_GPIO_MODE_SHIFT)

// TSI Control Register A settings
#define DA9053_TSI_AUTO_EN_SHIFT        0
#define DA9053_TSI_AUTO_EN_MASK        (0x01 << DA9053_TSI_AUTO_EN_SHIFT)
#define DA9053_TSI_PEN_DET_EN_SHIFT     1
#define DA9053_TSI_PEN_DET_EN_MASK     (0x01 << DA9053_TSI_PEN_DET_EN_SHIFT)
#define DA9053_TSI_MODE_SHIFT           2
#define DA9053_TSI_MODE_MASK           (0x01 << DA9053_TSI_MODE_SHIFT)
#define DA9053_TSI_SKIP_SHIFT           3
#define DA9053_TSI_SKIP_MASK           (0x07 << DA9053_TSI_SKIP_SHIFT)
#define DA9053_TSI_DELAY_SHIFT          6
#define DA9053_TSI_DELAY_MASK          (0x03 << DA9053_TSI_DELAY_SHIFT)

// TSI Control Register B settings
#define DA9053_TSI_SEL_0_SHIFT          0
#define DA9053_TSI_SEL_0_MASK          (0x01 << DA9053_TSI_SEL_0_SHIFT)
#define DA9053_TSI_SEL_1_SHIFT          1
#define DA9053_TSI_SEL_1_MASK          (0x01 << DA9053_TSI_SEL_1_SHIFT)
#define DA9053_TSI_SEL_2_SHIFT          2
#define DA9053_TSI_SEL_2_MASK          (0x01 << DA9053_TSI_SEL_2_SHIFT)
#define DA9053_TSI_SEL_3_SHIFT          3
#define DA9053_TSI_SEL_3_MASK          (0x01 << DA9053_TSI_SEL_3_SHIFT)
#define DA9053_TSI_MUX_SHIFT            4
#define DA9053_TSI_MUX_MASK            (0x03 << DA9053_TSI_MUX_SHIFT)
#define DA9053_TSI_MAN_SHIFT            6
#define DA9053_TSI_MAN_MASK            (0x01 << DA9053_TSI_MAN_SHIFT)
#define DA9053_TSI_ADCREF_SHIFT         7
#define DA9053_TSI_ADCREF_MASK         (0x01 << DA9053_TSI_ADCREF_SHIFT)

// TSI X/Y/Z Reading Register settings
#define DA9053_TSI_MSB_SHIFT            2
#define DA9053_TSI_LSB_X_SHIFT          0
#define DA9053_TSI_LSB_X_MASK          (0x03 << DA9053_TSI_LSB_X_SHIFT)
#define DA9053_TSI_LSB_Y_SHIFT          2
#define DA9053_TSI_LSB_Y_MASK          (0x03 << DA9053_TSI_LSB_X_SHIFT)
#define DA9053_TSI_LSB_Z_SHIFT          4
#define DA9053_TSI_LSB_Z_MASK          (0x03 << DA9053_TSI_LSB_X_SHIFT)
#define DA9053_TSI_LSB_PENDOWN_SHIFT    6
#define DA9053_TSI_LSB_PENDOWN_MASK    (0x01 << DA9053_TSI_LSB_PENDOWN_SHIFT)



//#################################################################################
//  THIS SECTION CONTAINS IMX53QSB PMIC DRIVER IMPLEMENTATION SPECIFIC INFORMATION
//#################################################################################


// Flags used in DCMD_DA9053_STATUS_READ and DCMD_DA9053_EVENT_PULSE_REGISTER calls
#define DA9053_STATUS_DCIN_DETECT             (0x01 <<  0)
#define DA9053_STATUS_VBUS_DETECT             (0x01 <<  1)
#define DA9053_STATUS_DCIN_REMOVE             (0x01 <<  2)
#define DA9053_STATUS_VBUS_REMOVE             (0x01 <<  3)
#define DA9053_STATUS_VDD_LOW                 (0x01 <<  4)
#define DA9053_STATUS_RTC_ALARM               (0x01 <<  5)
#define DA9053_STATUS_SEQ_STOP                (0x01 <<  6)
#define DA9053_STATUS_1V2_COMPARE             (0x01 <<  7)
#define DA9053_STATUS_ONKEY                   (0x01 <<  8)
#define DA9053_STATUS_ID_FLOAT                (0x01 <<  9)
#define DA9053_STATUS_ID_GROUND               (0x01 << 10)
#define DA9053_STATUS_CHG_COMPLETE            (0x01 << 11)
#define DA9053_STATUS_BAT_TEMP                (0x01 << 12)
#define DA9053_STATUS_ADC_READY               (0x01 << 13)
#define DA9053_STATUS_PEN_DOWN                (0x01 << 14)
#define DA9053_STATUS_TSI_READY               (0x01 << 15)
#define DA9053_STATUS_GPIO_0                  (0x01 << 16)
#define DA9053_STATUS_GPIO_1                  (0x01 << 17)
#define DA9053_STATUS_GPIO_2                  (0x01 << 18)
#define DA9053_STATUS_GPIO_3                  (0x01 << 19)
#define DA9053_STATUS_GPIO_4                  (0x01 << 20)
#define DA9053_STATUS_GPIO_5                  (0x01 << 21)
#define DA9053_STATUS_GPIO_6                  (0x01 << 22)
#define DA9053_STATUS_GPIO_7                  (0x01 << 23)
#define DA9053_STATUS_GPIO_8                  (0x01 << 24)
#define DA9053_STATUS_GPIO_9                  (0x01 << 25)
#define DA9053_STATUS_GPIO_10                 (0x01 << 26)
#define DA9053_STATUS_GPIO_11                 (0x01 << 27)
#define DA9053_STATUS_GPIO_12                 (0x01 << 28)
#define DA9053_STATUS_GPIO_13                 (0x01 << 29)
#define DA9053_STATUS_GPIO_14                 (0x01 << 30)
#define DA9053_STATUS_GPIO_15                 (0x01 << 31)


//--------------------------------------
// Structures used in PMIC devctl comms
//--------------------------------------
#define DA9053_EVENT_CALLBACK_FUNC(a)    int (*a)(uint32_t, void *)


// Read / Write a single register
typedef struct
{
	uint32_t reg_number;
	uint32_t reg_data;
} da9053_send_recv_t;

// Read a sequential block of registers
#define DA9053_MAX_SEND_RECV_BLOCK_SIZE      16
typedef struct
{
	uint32_t start_reg_number;
	uint32_t num_registers;
	uint32_t reg_data[DA9053_MAX_SEND_RECV_BLOCK_SIZE];
} da9053_send_recv_block_t;

// Get driver version
typedef struct
{
	unsigned int version_major;
	unsigned int version_minor;
} da9053_version_info_t;

// Manage pulses sent to clients by the driver
typedef struct
{
	uint32_t eventMask;                    // (in) The 32-bit PMIC event bitmask to check
	int      ownerId;                      // (in) The pulse recipient process ID
	int      channelId;                    // (in) The pulse recipient channel ID
	int      priority;                     // (in) The pulse priority
	int      code;                         // (in) The 8-bit pulse code to send back
	uint32_t guid;                         // (out) A GUID identifying the pulse registration
										   //       Used in cancelling the pulse
} da9053_pulse_info_t;

#define DA9053_DEVICE_NAME               "/dev/pmic"
#define DA9053_DRIVER_VERSION_MAJOR       1
#define DA9053_DRIVER_VERSION_MINOR       0

// IOCTL CALLS FOR PMIC DEVICE
#include <devctl.h>
#define _DCMD_DA9053                      _DCMD_MISC

#define DCMD_DA9053_DRIVER_INFO           __DIOF  (_DCMD_DA9053,  1, da9053_version_info_t)
#define DCMD_DA9053_LOCK                  __DIOT  (_DCMD_DA9053,  2, uint32_t)
#define DCMD_DA9053_UNLOCK                __DIOT  (_DCMD_DA9053,  3, uint32_t)
#define DCMD_DA9053_REGISTER_READ         __DIOTF (_DCMD_DA9053,  4, da9053_send_recv_t)
#define DCMD_DA9053_REGISTER_READ_BLOCK   __DIOTF (_DCMD_DA9053,  5, da9053_send_recv_block_t)
#define DCMD_DA9053_REGISTER_WRITE        __DIOT  (_DCMD_DA9053,  6, da9053_send_recv_t)
#define DCMD_DA9053_REGISTER_WRITE_BLOCK  __DIOTF (_DCMD_DA9053,  7, da9053_send_recv_block_t)
#define DCMD_DA9053_STATUS_READ           __DIOF  (_DCMD_DA9053,  8, uint32_t)
#define DCMD_DA9053_EVENT_PULSE_REGISTER  __DIOTF (_DCMD_DA9053,  9, da9053_pulse_info_t)
#define DCMD_DA9053_EVENT_PULSE_CANCEL    __DIOT  (_DCMD_DA9053, 10, uint32_t)
#define DCMD_DA9053_EVENT_POLL            __DIOT  (_DCMD_DA9053, 11, uint32_t)
#define DCMD_DA9053_ATOD_RESOLUTION       __DIOT  (_DCMD_DA9053, 12, uint32_t)


#endif   // _DA9053_PMIC_H_INCLUDED
