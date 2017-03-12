/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems.
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



#ifndef		_MMCSD_INCLUDED
#define		_MMCSD_INCLUDED

/* MultiMediaCard Command definitions */
/* An 'S' after the definition means that this command is available in SPI mode */
/* SPI commands are a subset of the MMC definition */

#define	MMC_GO_IDLE_STATE			0		/* S */
#define	MMC_SEND_OP_COND			1		/* S */
#define	MMC_ALL_SEND_CID			2
#define	MMC_SET_RELATIVE_ADDR		3
#define	MMC_SET_DSR					4
#define	MMC_SET_DSR					4
#define	MMC_SWITCH					6
	#define MMC_SWITCH_MODE_WRITE	0x3
	#define MMC_SWITCH_MODE_CLR		0x2
	#define MMC_SWITCH_MODE_SET		0x1
	#define MMC_SWITCH_MODE_CMD_SET	0x0
	#define MMC_SWITCH_CMDSET_DFLT	0x1
#define	MMC_SEL_DES_CARD			7
#define	MMC_IF_COND					8
#define	MMC_SEND_EXT_CSD			8
#define	MMC_SEND_CSD				9		/* S */
#define	MMC_SEND_CID				10		/* S */
#define	MMC_READ_DAT_UNTIL_STOP		11
#define	MMC_STOP_TRANSMISSION		12
#define	MMC_SEND_STATUS				13		/* S */
#define	MMC_GO_INACTIVE_STATE		15
#define	MMC_SET_BLOCKLEN			16		/* S */
#define	MMC_READ_SINGLE_BLOCK		17		/* S */
#define	MMC_READ_MULTIPLE_BLOCK		18
#define	MMC_WRITE_DAT_UNTIL_STOP	20
#define	MMC_WRITE_BLOCK				24		/* S */
#define	MMC_WRITE_MULTIPLE_BLOCK	25
#define	MMC_PROGRAM_CID				26
#define	MMC_PROGRAM_CSD				27		/* S */
#define	MMC_SET_WRITE_PROT			28		/* S */
#define	MMC_CLR_WRITE_PROT			29		/* S */
#define	MMC_SEND_WRITE_PROT			30		/* S */
#define	MMC_TAG_SECTOR_START		32		/* S */
#define	MMC_TAG_SECTOR_END			33		/* S */
#define	MMC_UNTAG_SECTOR			34		/* S */
#define	MMC_TAG_ERASE_GROUP_START	35		/* S */
#define	MMC_TAG_ERASE_GROUP_END		36		/* S */
#define	MMC_UNTAG_ERASE_GROUP		37		/* S */
#define	MMC_ERASE					38		/* S */
	#define MMC_ERASE_TRIM				0x00000001
	#define MMC_ERASE_GARBAGE_COLLECT	0x80008001
	#define MMC_ERASE_SECURE_TRIM		0x80000001
#define	MMC_FAST_IO					39
#define	MMC_GO_IRQ_STATE			40
#define	MMC_LOCK_UNLOCK				42		/* S */
#define	MMC_APP_CMD					55		/* S */
#define	MMC_GEN_CMD					56		/* S */
#define	MMC_READ_OCR				58		/* S */
#define	MMC_CRC_ON_OFF				59		/* S */

#define	SD_SET_BUS_WIDTH			6
#define	SD_SEND_OP_COND				41
#define	SD_SEND_SCR					51


/* Card Status Response Bits */

#define	MMC_OUT_OF_RANGE			(1 << 31)
#define	MMC_ADDRESS_ERROR			(1 << 30)
#define	MMC_BLOCK_LEN_ERROR			(1 << 29)
#define	MMC_ERASE_SEQ_ERROR			(1 << 28)
#define	MMC_ERASE_PARAM				(1 << 27)
#define	MMC_WP_VIOLATION			(1 << 26)
#define	MMC_CARD_IS_LOCKED			(1 << 25)
#define	MMC_LOCK_UNLOCK_FAILED		(1 << 24)
#define	MMC_COM_CRC_ERROR			(1 << 23)
#define	MMC_ILLEGAL_COMMAND			(1 << 22)
#define	MMC_CARD_ECC_FAILED			(1 << 21)
#define	MMC_CC_ERROR				(1 << 20)
#define	MMC_ERROR					(1 << 19)
#define	MMC_UNDERRUN				(1 << 18)
#define	MMC_OVERRUN					(1 << 17)
#define	MMC_CID_CSD_OVERWRITE		(1 << 16)
#define	MMC_WP_ERASE_SKIP			(1 << 15)
#define	MMC_CARD_ECC_DISABLED		(1 << 14)
#define	MMC_ERASE_RESET				(1 << 13)
/* Bits 9-12 define the CURRENT_STATE */
#define	MMC_IDLE					(0 << 9)
#define	MMC_READY					(1 << 9)
#define	MMC_IDENT					(2 << 9)
#define	MMC_STANDBY					(3 << 9)
#define	MMC_TRAN					(4 << 9)
#define	MMC_DATA					(5 << 9)
#define	MMC_RCV						(6 << 9)
#define	MMC_PRG						(7 << 9)
#define	MMC_DIS						(8 << 9)
/* End CURRENT_STATE */
#define	MMC_READY_FOR_DATA			(1 << 8)
#define	MMC_SWITCH_ERROR			(1 << 7)
#define	MMC_URGENT_BKOPS			(1 << 6)
#define	MMC_APP_CMD_S				(1 << 5)

/* SPI Mode Response R1 format */

#define	MMC_PARAM_ERROR				(1 << 6)
#define	MMC_SADDRESS_ERROR			(1 << 5)
#define	MMC_SERASE_SEQ_ERROR		(1 << 4)
#define	MMC_SCOM_CRC_ERROR			(1 << 3)
#define	MMC_SILLEGAL_COMMAND		(1 << 2)
#define	MMC_SERASE_RESET			(1 << 1)
#define	MMC_IDLE_STATE				(1 << 0)

/* SPI Mode Response R2 format */
/* First byte is the same as R1 format */
#define	MMC_SOUT_OF_RANGE			(1 << 7)
#define	MMC_SERASE_PARAM			(1 << 6)
#define	MMC_SWP_VIOLATION			(1 << 5)
#define	MMC_SCARD_ECC_FAILED		(1 << 4)
#define	MMC_SCC_ERROR				(1 << 3)
#define	MMC_SERROR					(1 << 2)
#define	MMC_SWP_ERASE_SKIP			(1 << 1)
#define	MMC_SCARD_IS_LOCKED			(1 << 0)

/* I/O Definitions for target ID */
#define	MMC_TARGET_MMC				0
#define	MMC_TARGET_MAS				1

/* I/O Flag definitions */
#define	MMC_DIR_NONE			0
#define	MMC_DIR_IN				(1 << 0)
#define	MMC_DIR_OUT				(1 << 1)	
#define	MMC_CRC7				(1 << 2)
#define	MMC_CRC16				(1 << 3)
#define	MMC_DIR_MASK			(MMC_DIR_IN | MMC_DIR_OUT)

/* I/O Status definitions */
#define	MMC_SUCCESS				0
#define	MMC_FAILURE				1
#define	MMC_DATA_OVERRUN		2
#define	MMC_BAD_FLAG			3
#define	MMC_NOT_PRESENT			4
#define	MMC_TIMEOUT				5
#define	MMC_ALLOC_FAILED		6
#define	MMC_INVALID_HANDLE		7
#define	MMC_COMMAND_FAILURE		8
#define	MMC_READ_ERROR			9
#define	MMC_WRITE_ERROR			10

/* MMC I/O structure */

typedef	struct	_mmc_cmd_t {
	uint8_t		opcode;			// command code
	uint8_t		resp;			// response type
	uint16_t	eflags;			// flags
#define	MMC_CMD_NONE		(0 << 0)	// nothing special
#define	MMC_CMD_INIT		(1 << 0)	// initialize sequence required
#define	MMC_CMD_PPL			(1 << 1)	// Push-pull command
#define	MMC_CMD_DATA		(1 << 4)	// command expect data
#define	MMC_CMD_DATA_MULTI	(1 << 5)	// multi-blocks data expected
#define	MMC_CMD_DATA_IN		(1 << 6)	// data read
#define	MMC_CMD_DATA_DMA	(1 << 7)	// DMA is expected for data transfer
#define	MMC_CMD_INTR		(1 << 8)	// command complete interrupt expected
	uint32_t	argument;		// command argument
} mmc_cmd_t;


#define	MMC_RSP_PRESENT	(1 << 0)
#define	MMC_RSP_136		(1 << 1)	/* 136 bit response */
#define	MMC_RSP_CRC		(1 << 2)	/* expect valid crc */
#define	MMC_RSP_BUSY	(1 << 3)	/* card may send busy */
#define	MMC_RSP_OPCODE	(1 << 4)	/* response contains opcode */

#define	MMC_RSP_NONE	(0)
#define	MMC_RSP_R1		(MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)
#define	MMC_RSP_R1B		(MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE | MMC_RSP_BUSY)
#define	MMC_RSP_R2		(MMC_RSP_PRESENT | MMC_RSP_136 | MMC_RSP_CRC)
#define	MMC_RSP_R3		(MMC_RSP_PRESENT)
#define	MMC_RSP_R6		(MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)
#define	MMC_RSP_R7		(MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)


typedef struct _sd_cid_t {
	uint8_t		mid;		// Manufacture ID
	uint8_t		oid[3];		// OEM/Application ID
	uint8_t		pnm[6];		// Product name
	uint8_t		prv;		// Product revision
	uint32_t	psn;		// Product serial number
	uint16_t	mdt;		// Manufacture date
} sd_cid_t;

typedef struct _mmc_cid_t {
	uint32_t	mid;		// Manufacture ID
	uint16_t	oid;		// OEM ID
	uint8_t		pnm[8];		// Product name
	uint8_t		hwr;		// HW revision
	uint8_t		fwr;		// FW revision
	uint32_t	psn;		// Product serial number
	uint8_t		mcd;		// Month code
	uint16_t	ycd;		// Year code
} mmc_cid_t;

typedef struct _sd_csd_t {
	uint8_t		csd_structure;	// CSD structure
	uint8_t		taac;
	uint8_t		nsac;
	uint8_t		tran_speed;
	uint16_t	ccc;
	uint8_t		read_bl_len;
	uint8_t		read_bl_partial;
	uint8_t		write_blk_misalign;
	uint8_t		read_blk_misalign;
	uint8_t		dsr_imp;
	union {
		struct {
			uint16_t	c_size;
			uint8_t		vdd_r_curr_min;
			uint8_t		vdd_r_curr_max;
			uint8_t		vdd_w_curr_min;
			uint8_t		vdd_w_curr_max;
			uint8_t		c_size_mult;
		} csd_ver1;
		struct {
			uint32_t	c_size;
		} csd_ver2;
	}csd;
	uint8_t		erase_blk_en;
	uint8_t		sector_size;
	uint8_t		wp_grp_size;
	uint8_t		wp_grp_enable;
	uint8_t		r2w_factor;
	uint8_t		write_bl_len;
	uint8_t		write_bl_partial;
	uint8_t		file_format_grp;
	uint8_t		copy;
	uint8_t		perm_write_protect;
	uint8_t		tmp_write_protect;
	uint8_t		file_format;
} sd_csd_t;

/*
 * EXT_CSD fields
 */
#define EXT_CSD_SIZE	512	

#define EXT_CSD_GP_SIZE				143
#define EXT_CSD_GP1_SIZE			143
#define EXT_CSD_GP2_SIZE			146
#define EXT_CSD_GP3_SIZE			149
#define EXT_CSD_GP4_SIZE			152
#define EXT_CSD_PARTITION_SETTING	155
	#define EXT_CSD_PS_CMP			0x01
#define EXT_CSD_PARTITIONS_ATTR		156
	#define EXT_CSD_PA_ENH_4		0x10
	#define EXT_CSD_PA_ENH_3		0x08
	#define EXT_CSD_PA_ENH_2		0x04
	#define EXT_CSD_PA_ENH_1		0x02
	#define EXT_CSD_PA_ENH_USR		0x01
#define EXT_CSD_PARTITIONING_SUP	160
	#define EXT_CSD_PS_EN			0x01
#define EXT_CSD_RPMB_SIZE_MULT		168
#define EXT_CSD_USER_WP				171
#define EXT_CSD_ERASE_GRP_DEF		175
	#define EXT_CSD_EGD_EN			0x01
#define EXT_CSD_PART_CONFIG			179	/* R/W */
	#define EXT_CSD_PC_ACCESS_MSK	0x7
#define EXT_CSD_BUS_WIDTH			183	/* R/W */
#define EXT_CSD_HS_TIMING			185	/* R/W */
#define EXT_CSD_CARD_TYPE			196	/* RO */
#define EXT_CSD_REV					192	/* RO */
#define EXT_CSD_SEC_CNT				212	/* RO, 4 bytes */
#define EXT_CSD_HC_WP_GRP_SIZE		221
#define EXT_CSD_ERASE_GRP_SIZE		224
#define EXT_CSD_BOOT_SIZE_MULT		226
#define EXT_CSD_SEC_FEATURE_SUPPORT	231
	#define EXT_CSD_SEC_GB_CL_EN	0x10 /* TRIM support */
	#define EXT_CSD_SEC_BD_BLK_EN	0x04 /* Secure purge Bad blk mgnt support */
	#define EXT_CSD_SEC_ER_EN		0x01 /* Secure purge support */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

#define EXT_CSD_CARD_TYPE_26		(1<<0)	/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52		(1<<1)	/* Card can run at 52MHz */
#define EXT_CSD_CARD_TYPE_DDR_52	(1<<2)	/* Card can run at 52MHz 1.8V, 3.0V */
#define EXT_CSD_CARD_TYPE_DDR_52_LV	(1<<3)	/* Card can run at 52MHz 1.2V */

#define EXT_CSD_USER_WP_US_PWR_WP_EN	(1<<0)
#define EXT_CSD_USER_WP_US_PERM_WP_EN	(1<<2)
#define EXT_CSD_USER_WP_US_PWR_WP_DIS	(1<<3)
#define EXT_CSD_USER_WP_US_PERM_WP_DIS	(1<<4)
#define EXT_CSD_USER_WP_CD_PERM_WP_DIS	(1<<6)
#define EXT_CSD_USER_WP_PERM_PSWD_DIS	(1<<7)

#define EXT_CSD_BUS_WIDTH_1	0	/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	1	/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	2	/* Card is in 8 bit mode */

typedef struct _mmc_ext_csd {
	uint32_t		hs_max_dtr;
	uint32_t		sectors;
	uint8_t			erase_grp_def;
    uint8_t			hc_erase_group_size;
    uint8_t			hc_wp_grp_size;
    uint8_t			user_wp;
} mmc_ext_csd_t;

typedef struct _mmc_csd_t {
	uint8_t		csd_structure;	// CSD structure
#define	CSD_VERSION_10	0   /* 1.0 - 1.2 */
#define	CSD_VERSION_11	1   /* 1.4 -  */
#define	CSD_VERSION_12	2   /* 2.0 - 2.2 */
#define	CSD_VERSION_13	3   /* 3.1 - 3.2 - 3.31 */
#define	CSD_VERSION_14	4   /* 4.0 - 4.1 */
	uint8_t		mmc_prot;
	uint8_t		taac;
	uint8_t		nsac;
	uint8_t		tran_speed;
	uint16_t	ccc;
	uint8_t		read_bl_len;
	uint8_t		read_bl_partial;
	uint8_t		write_blk_misalign;
	uint8_t		read_blk_misalign;
	uint8_t		dsr_imp;
	uint16_t	c_size;
	uint8_t		vdd_r_curr_min;
	uint8_t		vdd_r_curr_max;
	uint8_t		vdd_w_curr_min;
	uint8_t		vdd_w_curr_max;
	uint8_t		c_size_mult;
	union {
		struct {	/* MMC system specification version 3.1 */
			uint8_t	erase_grp_size;
			uint8_t	erase_grp_mult;
		} mmc_v31;
		struct {	/* MMC system specification version 2.2 */
			uint8_t	sector_size;
			uint8_t	erase_grp_size;
		} mmc_v22;
	} erase;
	mmc_ext_csd_t ext_csd;
	uint8_t		wp_grp_size;
	uint8_t		wp_grp_enable;
	uint8_t		r2w_factor;
	uint8_t		write_bl_len;
	uint8_t		write_bl_partial;
//	uint8_t		file_format_grp;
	uint8_t		copy;
	uint8_t		perm_write_protect;
	uint8_t		tmp_write_protect;
	uint8_t		ecc;
} mmc_csd_t;

typedef struct _sd_scr_t {
	uint8_t		scr_structure;	// SCR structure
	uint8_t		sd_spec;		// Physical layer specification
	uint8_t		data_stat_after_erase;
	uint8_t		sd_security;
	uint8_t		sd_bus_width;
} sd_scr_t;

#define	SD_SCR_SIZE				8

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/mmcsd.h $ $Rev: 657836 $" )
