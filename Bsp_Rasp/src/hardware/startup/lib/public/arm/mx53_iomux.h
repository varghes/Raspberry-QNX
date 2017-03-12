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

#include <sys/srcversion.h>

#ifndef	__ARM_MX53_IOMUX_H_INCLUDED
#define	__ARM_MX53_IOMUX_H_INCLUDED

/*
 * offsets of IOMUXC registers from MX53_IOMUX_SWMUX
 * where MX53_IOMUX_SWMUX = MX53_IOMUXC_BASE + 0x001C
 */

#define SWMUX_GPIO_19				0
#define SWMUX_KEY_COL0				1
#define SWMUX_KEY_ROW0				2
#define SWMUX_KEY_COL1				3
#define SWMUX_KEY_ROW1				4
#define SWMUX_KEY_COL2				5
#define SWMUX_KEY_ROW2				6
#define SWMUX_KEY_COL3				7
#define SWMUX_KEY_ROW3				8
#define SWMUX_KEY_COL4				9
#define SWMUX_KEY_ROW4				10
#define SWMUX_DI0_DISP_CLK			11
#define SWMUX_DI0_PIN15				12
#define SWMUX_DI0_PIN2				13
#define SWMUX_DI0_PIN3				14
#define SWMUX_DI0_PIN4				15
#define SWMUX_DISP0_DAT0			16
#define SWMUX_DISP0_DAT1			17
#define SWMUX_DISP0_DAT2			18
#define SWMUX_DISP0_DAT3			19
#define SWMUX_DISP0_DAT4			20
#define SWMUX_DISP0_DAT5			21
#define SWMUX_DISP0_DAT6			22
#define SWMUX_DISP0_DAT7			23
#define SWMUX_DISP0_DAT8			24
#define SWMUX_DISP0_DAT9			25
#define SWMUX_DISP0_DAT10			26
#define SWMUX_DISP0_DAT11			27
#define SWMUX_DISP0_DAT12			28
#define SWMUX_DISP0_DAT13			29
#define SWMUX_DISP0_DAT14			30
#define SWMUX_DISP0_DAT15			31
#define SWMUX_DISP0_DAT16			32
#define SWMUX_DISP0_DAT17			33
#define SWMUX_DISP0_DAT18			34
#define SWMUX_DISP0_DAT19			35
#define SWMUX_DISP0_DAT20			36
#define SWMUX_DISP0_DAT21			37
#define SWMUX_DISP0_DAT22			38
#define SWMUX_DISP0_DAT23			39
#define SWMUX_CSI0_PIXCLK			40
#define SWMUX_CSI0_MCLK				41
#define SWMUX_CSI0_DATA_EN			42
#define SWMUX_CSI0_VSYNC			43
#define SWMUX_CSI0_DAT4				44
#define SWMUX_CSI0_DAT5				45
#define SWMUX_CSI0_DAT6				46
#define SWMUX_CSI0_DAT7				47
#define SWMUX_CSI0_DAT8				48
#define SWMUX_CSI0_DAT9				49
#define SWMUX_CSI0_DAT10			50
#define SWMUX_CSI0_DAT11			51
#define SWMUX_CSI0_DAT12			52
#define SWMUX_CSI0_DAT13			53
#define SWMUX_CSI0_DAT14			54
#define SWMUX_CSI0_DAT15			55
#define SWMUX_CSI0_DAT16			56
#define SWMUX_CSI0_DAT17			57
#define SWMUX_CSI0_DAT18			58
#define SWMUX_CSI0_DAT19			59
#define SWMUX_EIM_A25				60
#define SWMUX_EIM_EB2				61
#define SWMUX_EIM_D16				62
#define SWMUX_EIM_D17				63
#define SWMUX_EIM_D18				64
#define SWMUX_EIM_D19				65
#define SWMUX_EIM_D20				66
#define SWMUX_EIM_D21				67
#define SWMUX_EIM_D22				68
#define SWMUX_EIM_D23				69
#define SWMUX_EIM_EB3				70
#define SWMUX_EIM_D24				71
#define SWMUX_EIM_D25				72
#define SWMUX_EIM_D26				73
#define SWMUX_EIM_D27				74
#define SWMUX_EIM_D28				75
#define SWMUX_EIM_D29				76
#define SWMUX_EIM_D30				77
#define SWMUX_EIM_D31				78
#define SWMUX_EIM_A24				79
#define SWMUX_EIM_A23				80
#define SWMUX_EIM_A22				81
#define SWMUX_EIM_A21				82
#define SWMUX_EIM_A20				83
#define SWMUX_EIM_A19				84
#define SWMUX_EIM_A18				85
#define SWMUX_EIM_A17				86
#define SWMUX_EIM_A16				87
#define SWMUX_EIM_CS0				88
#define SWMUX_EIM_CS1				89
#define SWMUX_EIM_OE				90
#define SWMUX_EIM_RW				91
#define SWMUX_EIM_LBA				92
#define SWMUX_EIM_EB0				93
#define SWMUX_EIM_EB1				94
#define SWMUX_EIM_DA0				95
#define SWMUX_EIM_DA1				96
#define SWMUX_EIM_DA2				97
#define SWMUX_EIM_DA3				98
#define SWMUX_EIM_DA4				99
#define SWMUX_EIM_DA5				100
#define SWMUX_EIM_DA6				101
#define SWMUX_EIM_DA7				102
#define SWMUX_EIM_DA8				103
#define SWMUX_EIM_DA9				104
#define SWMUX_EIM_DA10				105
#define SWMUX_EIM_DA11				106
#define SWMUX_EIM_DA12				107
#define SWMUX_EIM_DA13				108
#define SWMUX_EIM_DA14				109
#define SWMUX_EIM_DA15				110
#define SWMUX_NANDF_WE_B			111
#define SWMUX_NANDF_RE_B			112
#define SWMUX_EIM_WAIT				113
#define SWMUX_EIM_BCLK				114
#define SWMUX_LVDS1_TX3_P			115
#define SWMUX_LVDS1_TX2_P			116
#define SWMUX_LVDS1_CLK_P			117
#define SWMUX_LVDS1_TX1_P			118
#define SWMUX_LVDS1_TX0_P			119
#define SWMUX_LVDS0_TX3_P			120
#define SWMUX_LVDS0_CLK_P			121
#define SWMUX_LVDS0_TX2_P			122
#define SWMUX_LVDS0_TX1_P			123
#define SWMUX_LVDS0_TX0_P			124
#define SWMUX_GPIO_10				125
#define SWMUX_GPIO_11				126
#define SWMUX_GPIO_12				127
#define SWMUX_GPIO_13				128
#define SWMUX_GPIO_14				129
#define SWMUX_NANDF_CLE				130
#define SWMUX_NANDF_ALE				131
#define SWMUX_NANDF_WP_B			132
#define SWMUX_NANDF_RB0				133
#define SWMUX_NANDF_CS0				134
#define SWMUX_NANDF_CS1				135
#define SWMUX_NANDF_CS2				136
#define SWMUX_NANDF_CS3				137
#define SWMUX_FEC_MDIO				138
#define SWMUX_FEC_REF_CLK			139
#define SWMUX_FEC_RX_ER				140
#define SWMUX_FEC_CRS_DV			141
#define SWMUX_FEC_RXD1				142
#define SWMUX_FEC_RXD0				143
#define SWMUX_FEC_TX_EN				144
#define SWMUX_FEC_TXD1				145
#define SWMUX_FEC_TXD0				146
#define SWMUX_FEC_MDC				147
#define SWMUX_PATA_DIOW				148
#define SWMUX_PATA_DMACK			149
#define SWMUX_PATA_DMARQ			150
#define SWMUX_PATA_BUFFER_EN		151
#define SWMUX_PATA_INTRQ			152
#define SWMUX_PATA_DIOR				153
#define SWMUX_PATA_RESET_B			154
#define SWMUX_PATA_IORDY			155
#define SWMUX_PATA_DA_0				156
#define SWMUX_PATA_DA_1				157
#define SWMUX_PATA_DA_2				158
#define SWMUX_PATA_CS_0				159
#define SWMUX_PATA_CS_1				160
#define SWMUX_PATA_DATA0			161
#define SWMUX_PATA_DATA1			162
#define SWMUX_PATA_DATA2			163
#define SWMUX_PATA_DATA3			164
#define SWMUX_PATA_DATA4			165
#define SWMUX_PATA_DATA5			166
#define SWMUX_PATA_DATA6			167
#define SWMUX_PATA_DATA7			168
#define SWMUX_PATA_DATA8			169
#define SWMUX_PATA_DATA9			170
#define SWMUX_PATA_DATA10			171
#define SWMUX_PATA_DATA11			172
#define SWMUX_PATA_DATA12			173
#define SWMUX_PATA_DATA13			174
#define SWMUX_PATA_DATA14			175
#define SWMUX_PATA_DATA15			176
#define SWMUX_SD1_DATA0				177
#define SWMUX_SD1_DATA1				178
#define SWMUX_SD1_CMD				179
#define SWMUX_SD1_DATA2				180
#define SWMUX_SD1_CLK				181
#define SWMUX_SD1_DATA3				182
#define SWMUX_SD2_CLK				183
#define SWMUX_SD2_CMD				184
#define SWMUX_SD2_DATA3				185
#define SWMUX_SD2_DATA2				186
#define SWMUX_SD2_DATA1				187
#define SWMUX_SD2_DATA0				188
#define SWMUX_GPIO_0				189
#define SWMUX_GPIO_1				190
#define SWMUX_GPIO_9				191
#define SWMUX_GPIO_3				192
#define SWMUX_GPIO_6				193
#define SWMUX_GPIO_2				194
#define SWMUX_GPIO_4				195
#define SWMUX_GPIO_5				196
#define SWMUX_GPIO_7				197
#define SWMUX_GPIO_8				198
#define SWMUX_GPIO_16				199
#define SWMUX_GPIO_17				200
#define SWMUX_GPIO_18				201

/*
 * offsets of IOMUXC registers from MX53_IOMUX_SWPAD
 * where MX53_IOMUX_SWPAD = MX53_IOMUXC_BASE + 0x03F0
 */

#define SWPAD_GPIO_19				0
#define SWPAD_KEY_COL0				1
#define SWPAD_KEY_ROW0				2
#define SWPAD_KEY_COL1				3
#define SWPAD_KEY_ROW1				4
#define SWPAD_KEY_COL2				5
#define SWPAD_KEY_ROW2				6
#define SWPAD_KEY_COL3				7
#define SWPAD_KEY_ROW3				8
#define SWPAD_KEY_COL4				9
#define SWPAD_KEY_ROW4				10
#define SWPAD_NVCC_KEYPAD			11
#define SWPAD_DI0_DISP_CLK			12
#define SWPAD_DI0_PIN15				13
#define SWPAD_DI0_PIN2				14
#define SWPAD_DI0_PIN3				15
#define SWPAD_DI0_PIN4				16
#define SWPAD_DISP0_DAT0			17
#define SWPAD_DISP0_DAT1			18
#define SWPAD_DISP0_DAT2			19
#define SWPAD_DISP0_DAT3			20
#define SWPAD_DISP0_DAT4			21
#define SWPAD_DISP0_DAT5			22
#define SWPAD_DISP0_DAT6			23
#define SWPAD_DISP0_DAT7			24
#define SWPAD_DISP0_DAT8			25
#define SWPAD_DISP0_DAT9			26
#define SWPAD_DISP0_DAT10			27
#define SWPAD_DISP0_DAT11			28
#define SWPAD_DISP0_DAT12			29
#define SWPAD_DISP0_DAT13			30
#define SWPAD_DISP0_DAT14			31
#define SWPAD_DISP0_DAT15			32
#define SWPAD_DISP0_DAT16			33
#define SWPAD_DISP0_DAT17			34
#define SWPAD_DISP0_DAT18			35
#define SWPAD_DISP0_DAT19			36
#define SWPAD_DISP0_DAT20			37
#define SWPAD_DISP0_DAT21			38
#define SWPAD_DISP0_DAT22			39
#define SWPAD_DISP0_DAT23			40
#define SWPAD_CSI0_PIXCLK			41
#define SWPAD_CSI0_MCLK				42
#define SWPAD_CSI0_DATA_EN			43
#define SWPAD_CSI0_VSYNC			44
#define SWPAD_CSI0_DAT4				45
#define SWPAD_CSI0_DAT5				46
#define SWPAD_CSI0_DAT6				47
#define SWPAD_CSI0_DAT7				48
#define SWPAD_CSI0_DAT8				49
#define SWPAD_CSI0_DAT9				50
#define SWPAD_CSI0_DAT10			51
#define SWPAD_CSI0_DAT11			52
#define SWPAD_CSI0_DAT12			53
#define SWPAD_CSI0_DAT13			54
#define SWPAD_CSI0_DAT14			55
#define SWPAD_CSI0_DAT15			56
#define SWPAD_CSI0_DAT16			57
#define SWPAD_CSI0_DAT17			58
#define SWPAD_CSI0_DAT18			59
#define SWPAD_CSI0_DAT19			60
#define SWPAD_NVCC_CSI__0			61
#define SWPAD_JTAG_TMS				62
#define SWPAD_JTAG_MOD				63
#define SWPAD_JTAG_TRSTB			64
#define SWPAD_JTAG_TDI				65
#define SWPAD_JTAG_TCK				66
#define SWPAD_JTAG_TDO				67
#define SWPAD_EIM_A25				68
#define SWPAD_EIM_EB2				69
#define SWPAD_EIM_D16				70
#define SWPAD_EIM_D17				71
#define SWPAD_EIM_D18				72
#define SWPAD_EIM_D19				73
#define SWPAD_EIM_D20				74
#define SWPAD_EIM_D21				75
#define SWPAD_EIM_D22				76
#define SWPAD_EIM_D23				77
#define SWPAD_EIM_EB3				78
#define SWPAD_EIM_D24				79
#define SWPAD_EIM_D25				80
#define SWPAD_EIM_D26				81
#define SWPAD_EIM_D27				82
#define SWPAD_EIM_D28				83
#define SWPAD_EIM_D29				84
#define SWPAD_EIM_D30				85
#define SWPAD_EIM_D31				86
#define SWPAD_NVCC_EIM__1			87
#define SWPAD_EIM_A24				88
#define SWPAD_EIM_A23				89
#define SWPAD_EIM_A22				90
#define SWPAD_EIM_A21				91
#define SWPAD_EIM_A20				92
#define SWPAD_EIM_A19				93
#define SWPAD_EIM_A18				94
#define SWPAD_EIM_A17				95
#define SWPAD_EIM_A16				96
#define SWPAD_EIM_CS0				97
#define SWPAD_EIM_CS1				98
#define SWPAD_EIM_OE				99
#define SWPAD_EIM_RW				100
#define SWPAD_EIM_LBA				101
#define SWPAD_NVCC_EIM__4			102
#define SWPAD_EIM_EB0				103
#define SWPAD_EIM_EB1				104
#define SWPAD_EIM_DA0				105
#define SWPAD_EIM_DA1				106
#define SWPAD_EIM_DA2				107
#define SWPAD_EIM_DA3				108
#define SWPAD_EIM_DA4				109
#define SWPAD_EIM_DA5				110
#define SWPAD_EIM_DA6				111
#define SWPAD_EIM_DA7				112
#define SWPAD_EIM_DA8				113
#define SWPAD_EIM_DA9				114
#define SWPAD_EIM_DA10				115
#define SWPAD_EIM_DA11				116
#define SWPAD_EIM_DA12				117
#define SWPAD_EIM_DA13				118
#define SWPAD_EIM_DA14				119
#define SWPAD_EIM_DA15				120
#define SWPAD_NANDF_WE_B			121
#define SWPAD_NANDF_RE_B			122
#define SWPAD_EIM_WAIT				123
#define SWPAD_EIM_BCLK				124
#define SWPAD_NVCC_EIM__7			125
#define SWPAD_GPIO_10				126
#define SWPAD_GPIO_11				127
#define SWPAD_GPIO_12				128
#define SWPAD_GPIO_13				129
#define SWPAD_GPIO_14				130
#define SWPAD_DRAM_DQM3				131
#define SWPAD_DRAM_SDQS3			132
#define SWPAD_DRAM_SDCKE1			133
#define SWPAD_DRAM_DQM2				134
#define SWPAD_DRAM_SDODT1			135
#define SWPAD_DRAM_SDQS2			136
#define SWPAD_DRAM_RESET			137
#define SWPAD_DRAM_SDCLK_1			138
#define SWPAD_DRAM_CAS				139
#define SWPAD_DRAM_SDCLK_0			140
#define SWPAD_DRAM_SDQS0			141
#define SWPAD_DRAM_SDODT0			142
#define SWPAD_DRAM_DQM0				143
#define SWPAD_DRAM_RAS				144
#define SWPAD_DRAM_SDCKE0			145
#define SWPAD_DRAM_SDQS1			146
#define SWPAD_DRAM_DQM1				147
#define SWPAD_PMIC_ON_REQ			148
#define SWPAD_PMIC_STBY_REQ			149
#define SWPAD_NANDF_CLE				150
#define SWPAD_NANDF_ALE				151
#define SWPAD_NANDF_WP_B			152
#define SWPAD_NANDF_RB0				153
#define SWPAD_NANDF_CS0				154
#define SWPAD_NANDF_CS1				155
#define SWPAD_NANDF_CS2				156
#define SWPAD_NANDF_CS3				157
#define SWPAD_NVCC_NANDF			158
#define SWPAD_FEC_MDIO				159
#define SWPAD_FEC_REF_CLK			160
#define SWPAD_FEC_RX_ER				161
#define SWPAD_FEC_CRS_DV			162
#define SWPAD_FEC_RXD1				163
#define SWPAD_FEC_RXD0				164
#define SWPAD_FEC_TX_EN				165
#define SWPAD_FEC_TXD1				166
#define SWPAD_FEC_TXD0				167
#define SWPAD_FEC_MDC				168
#define SWPAD_NVCC_FEC				169
#define SWPAD_PATA_DIOW				170
#define SWPAD_PATA_DMACK			171
#define SWPAD_PATA_DMARQ			172
#define SWPAD_PATA_BUFFER_EN		173
#define SWPAD_PATA_INTRQ			174
#define SWPAD_PATA_DIOR				175
#define SWPAD_PATA_RESET_B			176
#define SWPAD_PATA_IORDY			177
#define SWPAD_PATA_DA_0				178
#define SWPAD_PATA_DA_1				179
#define SWPAD_PATA_DA_2				180
#define SWPAD_PATA_CS_0				181
#define SWPAD_PATA_CS_1				182
#define SWPAD_NVCC_PATA__2			183
#define SWPAD_PATA_DATA0			184
#define SWPAD_PATA_DATA1			185
#define SWPAD_PATA_DATA2			186
#define SWPAD_PATA_DATA3			187
#define SWPAD_PATA_DATA4			188
#define SWPAD_PATA_DATA5			189
#define SWPAD_PATA_DATA6			190
#define SWPAD_PATA_DATA7			191
#define SWPAD_PATA_DATA8			192
#define SWPAD_PATA_DATA9			193
#define SWPAD_PATA_DATA10			194
#define SWPAD_PATA_DATA11			195
#define SWPAD_PATA_DATA12			196
#define SWPAD_PATA_DATA13			197
#define SWPAD_PATA_DATA14			198
#define SWPAD_PATA_DATA15			199
#define SWPAD_NVCC_PATA__0			200
#define SWPAD_SD1_DATA0				201
#define SWPAD_SD1_DATA1				202
#define SWPAD_SD1_CMD				203
#define SWPAD_SD1_DATA2				204
#define SWPAD_SD1_CLK				205
#define SWPAD_SD1_DATA3				206
#define SWPAD_NVCC_SD1				207
#define SWPAD_SD2_CLK				208
#define SWPAD_SD2_CMD				209
#define SWPAD_SD2_DATA3				210
#define SWPAD_SD2_DATA2				211
#define SWPAD_SD2_DATA1				212
#define SWPAD_SD2_DATA0				213
#define SWPAD_NVCC_SD2				214
#define SWPAD_GPIO_0				215
#define SWPAD_GPIO_1				216
#define SWPAD_GPIO_9				217
#define SWPAD_GPIO_3				218
#define SWPAD_GPIO_6				219
#define SWPAD_GPIO_2				220
#define SWPAD_GPIO_4				221
#define SWPAD_GPIO_5				222
#define SWPAD_GPIO_7				223
#define SWPAD_GPIO_8				224
#define SWPAD_GPIO_16				225
#define SWPAD_GPIO_17				226
#define SWPAD_GPIO_18				227
#define SWPAD_NVCC_GPIO				228
#define SWPAD_POR_B					229
#define SWPAD_BOOT_MODE1			230
#define SWPAD_RESET_IN_B			231
#define SWPAD_BOOT_MODE0			232
#define SWPAD_TEST_MODE				233
#define PAD_GRP_ADDDS				234
#define PAD_GRP_DDRMODE_CTL			235
#define PAD_GRP_TERM_CTL0			236
#define PAD_GRP_DDRPKE				237
#define PAD_GRP_TERM_CTL1			238
#define PAD_GRP_TERM_CTL2			239
#define PAD_GRP_DDRPK				240
#define PAD_GRP_TERM_CTL3			241
#define PAD_GRP_DDRHYS				242
#define PAD_GRP_DDRMODE				243
#define PAD_GRP_B0DS				244
#define PAD_GRP_B1DS				245
#define PAD_GRP_CTLDS				246
#define PAD_GRP_DDR_TYPE			247
#define PAD_GRP_B2DS				248
#define PAD_GRP_B3DS				249

/*
 * offsets of IOMUXC registers from MX53_IOMUX_INPUT
 * where MX53_IOMUX_INPUT = MX53_IOMUXC_BASE + 0x08C4
 */

#define SWINPUT_AUDMUX_P4_DA_AMX                0
#define SWINPUT_AUDMUX_P4_DB_AMX                1
#define SWINPUT_AUDMUX_P4_RXCLK_AMX             2
#define SWINPUT_AUDMUX_P4_RXFS_AMX              3
#define SWINPUT_AUDMUX_P4_TXCLK_AMX             4
#define SWINPUT_AUDMUX_P4_TXFS_AMX              5
#define SWINPUT_AUDMUX_P5_DA_AMX                6
#define SWINPUT_AUDMUX_P5_DB_AMX                7
#define SWINPUT_AUDMUX_P5_RXCLK_AMX             8
#define SWINPUT_AUDMUX_P5_RXFS_AMX              9
#define SWINPUT_AUDMUX_P5_TXCLK_AMX             10
#define SWINPUT_AUDMUX_P5_TXFS_AMX              11
#define SWINPUT_CAN1_IPP_IND_CANRX              12
#define SWINPUT_CAN2_IPP_IND_CANRX              13
#define SWINPUT_CCM_IPP_ASRC_EXT                14
#define SWINPUT_CCM_IPP_DI1_CLK                 15
#define SWINPUT_CCM_PLL1_BYPASS_CLK             16
#define SWINPUT_CCM_PLL2_BYPASS_CLK             17
#define SWINPUT_CCM_PLL3_BYPASS_CLK             18
#define SWINPUT_CCM_PLL4_BYPASS_CLK             19
#define SWINPUT_CSPI_IPP_CSPI_CLK_IN            20
#define SWINPUT_CSPI_IPP_IND_MISO               21
#define SWINPUT_CSPI_IPP_IND_MOSI               22
#define SWINPUT_CSPI_IPP_IND_SS0_B              23
#define SWINPUT_CSPI_IPP_IND_SS1_B              24
#define SWINPUT_CSPI_IPP_IND_SS2_B              25
#define SWINPUT_CSPI_IPP_IND_SS3_B              26
#define SWINPUT_ECSPI1_IPP_CSPI_CLK_IN          27
#define SWINPUT_ECSPI1_IPP_IND_MISO             28
#define SWINPUT_ECSPI1_IPP_IND_MOSI             29
#define SWINPUT_ECSPI1_IPP_IND_SS_B_0           30
#define SWINPUT_ECSPI1_IPP_IND_SS_B_1           31
#define SWINPUT_ECSPI1_IPP_IND_SS_B_2           32
#define SWINPUT_ECSPI1_IPP_IND_SS_B_3           33
#define SWINPUT_ECSPI2_IPP_CSPI_CLK_IN          34
#define SWINPUT_ECSPI2_IPP_IND_MISO             35
#define SWINPUT_ECSPI2_IPP_IND_MOSI             36
#define SWINPUT_ECSPI2_IPP_IND_SS_B_0           37
#define SWINPUT_ECSPI2_IPP_IND_SS_B_1           38
#define SWINPUT_ESAI1_IPP_IND_FSR               39
#define SWINPUT_ESAI1_IPP_IND_FST               40
#define SWINPUT_ESAI1_IPP_IND_HCKR              41
#define SWINPUT_ESAI1_IPP_IND_HCKT              42
#define SWINPUT_ESAI1_IPP_IND_SCKR              43
#define SWINPUT_ESAI1_IPP_IND_SCKT              44
#define SWINPUT_ESAI1_IPP_IND_SDO0              45
#define SWINPUT_ESAI1_IPP_IND_SDO1              46
#define SWINPUT_ESAI1_IPP_IND_SDO2_SDI3         47
#define SWINPUT_ESAI1_IPP_IND_SDO3_SDI2         48
#define SWINPUT_ESAI1_IPP_IND_SDO4_SDI1         49
#define SWINPUT_ESAI1_IPP_IND_SDO5_SDI0         50
#define SWINPUT_ESDHC1_IPP_WP_ON                51
#define SWINPUT_FEC_FEC_COL                     52
#define SWINPUT_FEC_FEC_MDI                     53
#define SWINPUT_FEC_FEC_RX_CLK                  54
#define SWINPUT_FIRI_IPP_IND_RXD                55
#define SWINPUT_GPC_PMIC_RDY                    56
#define SWINPUT_I2C1_IPP_SCL_IN                 57
#define SWINPUT_I2C1_IPP_SDA_IN                 58
#define SWINPUT_I2C2_IPP_SCL_IN                 59
#define SWINPUT_I2C2_IPP_SDA_IN                 60
#define SWINPUT_I2C3_IPP_SCL_IN                 61
#define SWINPUT_I2C3_IPP_SDA_IN                 62
#define SWINPUT_IPU_IPP_DI_0_IND_DISPB_SD_D     63
#define SWINPUT_IPU_IPP_DI_1_IND_DISPB_SD_D     64
#define SWINPUT_IPU_IPP_IND_SENS1_DATA_EN       65
#define SWINPUT_IPU_IPP_IND_SENS1_HSYNC         66
#define SWINPUT_IPU_IPP_IND_SENS1_VSYNC         67
#define SWINPUT_KPP_IPP_IND_COL_5               68
#define SWINPUT_KPP_IPP_IND_COL_6               69
#define SWINPUT_KPP_IPP_IND_COL_7               70
#define SWINPUT_KPP_IPP_IND_ROW_5               71
#define SWINPUT_KPP_IPP_IND_ROW_6               72
#define SWINPUT_KPP_IPP_IND_ROW_7               73
#define SWINPUT_MLB_MLBCLK_IN                   74
#define SWINPUT_MLB_MLBDAT_IN                   75
#define SWINPUT_MLB_MLBSIG_IN                   76
#define SWINPUT_OWIRE_BATTERY_LINE_IN           77
#define SWINPUT_SDMA_EVENTS_14                  78
#define SWINPUT_SDMA_EVENTS_15                  79
#define SWINPUT_SPDIF_SPDIF_IN1                 80
#define SWINPUT_UART1_IPP_UART_RTS_B            81
#define SWINPUT_UART1_IPP_UART_RXD_MUX          82
#define SWINPUT_UART2_IPP_UART_RTS_B            83
#define SWINPUT_UART2_IPP_UART_RXD_MUX          84
#define SWINPUT_UART3_IPP_UART_RTS_B            85
#define SWINPUT_UART3_IPP_UART_RXD_MUX          86
#define SWINPUT_UART4_IPP_UART_RTS_B            87
#define SWINPUT_UART4_IPP_UART_RXD_MUX          88
#define SWINPUT_UART5_IPP_UART_RTS_B            89
#define SWINPUT_UART5_IPP_UART_RXD_MUX          90
#define SWINPUT_USBOH3_IPP_IND_OTG_OC           91
#define SWINPUT_USBOH3_IPP_IND_UH1_OC           92
#define SWINPUT_USBOH3_IPP_IND_UH2_OC           93
#define SWINPUT_USBOH3_IPP_IND_UH2_OC           93

/*
 * Bit definitions for SW_MUX_CTL registers
 */
#define MUX_CTL_SION                            (0x1 << 4)
#define MUX_CTL_MUX_MODE_ALT0                   0
#define MUX_CTL_MUX_MODE_ALT1                   1
#define MUX_CTL_MUX_MODE_ALT2                   2
#define MUX_CTL_MUX_MODE_ALT3                   3
#define MUX_CTL_MUX_MODE_ALT4                   4
#define MUX_CTL_MUX_MODE_ALT5                   5
#define MUX_CTL_MUX_MODE_ALT6                   6
#define MUX_CTL_MUX_MODE_ALT7                   7


/*
 * Bit definitions for SW_PAD_CTL registers
 */

/* GPIO type pads */
#define PAD_CTL_GPIO_DRV_3_3_V                  (0x0 << 13)
#define PAD_CTL_GPIO_DRV_1_8_V                  (0x1 << 13)
#define PAD_CTL_GPIO_HYS_DISABLE                (0x0 << 8)
#define PAD_CTL_GPIO_HYS_ENABLE                 (0x1 << 8)
#define PAD_CTL_GPIO_PKE_DISABLE                (0x0 << 7)
#define PAD_CTL_GPIO_PKE_ENABLE                 (0x1 << 7)
#define PAD_CTL_GPIO_PUE_PULL                   (0x1 << 6)
#define PAD_CTL_GPIO_PUE_KEEP                   (0x0 << 6)
#define PAD_CTL_GPIO_PUS_100K_PD                (0x0 << 4)
#define PAD_CTL_GPIO_PUS_47K_PU                 (0x1 << 4)
#define PAD_CTL_GPIO_PUS_100K_PU                (0x2 << 4)
#define PAD_CTL_GPIO_PUS_22K_PU                 (0x3 << 4)
#define PAD_CTL_GPIO_ODE_DISABLE                (0x0 << 3)
#define PAD_CTL_GPIO_ODE_ENABLE                 (0x1 << 3)
#define PAD_CTL_GPIO_DSE_LOW                    (0x0 << 1)
#define PAD_CTL_GPIO_DSE_MID                    (0x1 << 1)
#define PAD_CTL_GPIO_DSE_HIGH                   (0x2 << 1)
#define PAD_CTL_GPIO_DSE_MAX                    (0x3 << 1)
#define PAD_CTL_GPIO_SRE_SLOW                   (0x0 << 0)
#define PAD_CTL_GPIO_SRE_FAST                   (0x1 << 0)

/* DDR type pads */
#define PAD_CTL_DDR_DRV_3_3_V                   (0x0 << 13)
#define PAD_CTL_DDR_DRV_1_8_V                   (0x1 << 13)
#define PAD_CTL_DDR_TYPE_MOBILE                 (0x0 << 11)
#define PAD_CTL_DDR_TYPE_SDRAM                  (0x1 << 11)
#define PAD_CTL_DDR_TYPE_DDR2                   (0x2 << 11)
#define PAD_CTL_DDR_PKE_DISABLE                 (0x0 << 7)
#define PAD_CTL_DDR_PKE_ENABLE                  (0x1 << 7)
#define PAD_CTL_DDR_ODE_DISABLE                 (0x0 << 3)
#define PAD_CTL_DDR_ODE_ENABLE                  (0x1 << 3)
#define PAD_CTL_DDR_DSE_STD                     (0x0 << 1)
#define PAD_CTL_DDR_DSE_HIGH                    (0x1 << 1)
#define PAD_CTL_DDR_DSE_MAX                     (0x2 << 1)

/*
 * Function prototypes
 */
void pinmux_set_swmux(int pin, int mux_config);
void pinmux_set_padcfg(int pin, int pad_config);
void pinmux_set_input(int pin, int input_config);


#endif	/* __ARM_MX53_IOMUX_H_INCLUDED */

__SRCVERSION("$URL$ $Rev$");

