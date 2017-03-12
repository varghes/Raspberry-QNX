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

#ifndef _SCSI_H_INCLUDED
#define _SCSI_H_INCLUDED

#ifndef _CAM_H_INCLUDED
	#include <cam.h>
#endif

#include <_pack1.h>


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/include/ntoscsi.h $ $Rev: 657836 $" )

#define ENDIAN_RET24(__x)		((((__x) >> 16) & 0xff) | \
								((__x) & 0xff00) | \
								(((__x) & 0xff) << 16))

#define ENDIAN_RET48(__x)		((((__x) >> 40) & 0xff) | \
								 (((__x) >> 24) & 0xff00) | \
								 (((__x) >> 8) & 0xff0000) | \
								 (((__x) & 0xff0000) << 8) | \
								 (((__x) & 0xff00) << 24) | \
								 (((__x) & 0xff) << 40))

#define SCSI_RET24(__p)			UNALIGNED_RET24(__p)

#if defined(__LITTLEENDIAN__)
#define ENDIAN_LE24(__x)		(__x)
#define ENDIAN_BE24(__x)		ENDIAN_RET24(__x)
#define ENDIAN_LE48(__x)		(__x)
#define ENDIAN_BE48(__x)		ENDIAN_RET48(__x)

#define UNALIGNED_RET24(__p)	(((_Uint8t volatile *)(__p))[0] | \
								(((_Uint8t volatile *)(__p))[1] << 8) | \
								(((_Uint8t volatile *)(__p))[2] << 16))

#define UNALIGNED_PUT24(__p,__x) (((_Uint8t volatile *)(__p))[0] = (__x) & 0xff, \
								((_Uint8t volatile *)(__p))[1] = ((__x) >> 8) & 0xff, \
								((_Uint8t volatile *)(__p))[2] = ((__x) >> 16) & 0xff)

#define UNALIGNED_RET48(__p)	(((_Uint8t volatile *)(__p))[0] | \
								(((_Uint8t volatile *)(__p))[1] << 8) | \
								(((_Uint8t volatile *)(__p))[2] << 16) | \
								(((_Uint8t volatile *)(__p))[3] << 24) | \
								(((_Uint64t)((_Uint8t volatile *)(__p))[4]) << 32) | \
								(((_Uint64t)((_Uint8t volatile *)(__p))[5]) << 40))

#define UNALIGNED_PUT48(__p,__x) (((_Uint8t volatile *)(__p))[0] = (__x) & 0xff, \
								((_Uint8t volatile *)(__p))[1] = ((__x) >> 8) & 0xff, \
								((_Uint8t volatile *)(__p))[2] = ((__x) >> 16) & 0xff, \
								((_Uint8t volatile *)(__p))[3] = ((__x) >> 24) & 0xff, \
								((_Uint8t volatile *)(__p))[4] = ((__x) >> 32) & 0xff, \
								((_Uint8t volatile *)(__p))[5] = ((__x) >> 48) & 0xff)

#else

#define ENDIAN_LE24(__x)		ENDIAN_RET24(__x)
#define ENDIAN_BE24(__x)		(__x)
#define ENDIAN_LE48(__x)		ENDIAN_RET48(__x)
#define ENDIAN_BE48(__x)		(__x)

#define UNALIGNED_RET24(__p)	((((_Uint8t volatile *)(__p))[0] << 16) | \
								(((_Uint8t volatile *)(__p))[1] << 8) | \
								(((_Uint8t volatile *)(__p))[2]))

#define UNALIGNED_PUT24(__p,__x) (((_Uint8t volatile *)(__p))[0] = ((__x) >> 16) & 0xff, \
								((_Uint8t volatile *)(__p))[1] = ((__x) >> 8) & 0xff, \
								((_Uint8t volatile *)(__p))[2] = (__x) & 0xff)

#define UNALIGNED_RET48(__p)	((((_Uint64t)((_Uint8t volatile *)(__p))[0]) << 40) | \
								(((_Uint64t)((_Uint8t volatile *)(__p))[1]) << 32) | \
								(((_Uint8t volatile *)(__p))[2] << 24) | \
								(((_Uint8t volatile *)(__p))[3] << 16) | \
								(((_Uint8t volatile *)(__p))[4] << 8) | \
								(((_Uint8t volatile *)(__p))[5]))

#define UNALIGNED_PUT48(__p,__x) (((_Uint8t volatile *)(__p))[0] = ((__x) >> 40) & 0xff, \
								((_Uint8t volatile *)(__p))[1] = ((__x) >> 32) & 0xff, \
								((_Uint8t volatile *)(__p))[2] = ((__x) >> 24) & 0xff, \
								((_Uint8t volatile *)(__p))[3] = ((__x) >> 16) & 0xff, \
								((_Uint8t volatile *)(__p))[4] = ((__x) >> 8) & 0xff, \
								((_Uint8t volatile *)(__p))[5] = (__x) & 0xff)
#endif

#define SC_LUN_CDB(__x)			((__x) << 5)	// convert lun to CDB format

#define SC_NO_TIMEOUT_CMD( _cmd )	( (_cmd) == SC_RQ_SENSE || \
									(_cmd) == SC_INQUIRY || \
									(_cmd) == SC_RES_UNIT || \
									(_cmd) == SC_REL_UNIT || \
									(_cmd) == SC_RCV_DIAG || \
									(_cmd) == SC_SND_DIAG || \
									(_cmd) == SC_GET_CONFIGURATION || \
									(_cmd) == SC_GET_EVENT_STATUS )

#define SC_RW_CMD( _cmd )			( (_cmd) == SC_READ || \
									(_cmd) == SC_READ10 || \
									(_cmd) == SC_READ12 || \
									(_cmd) == SC_READ16 || \
									(_cmd) == SC_WRITE || \
									(_cmd) == SC_WRITE10 || \
									(_cmd) == SC_WRITE12 || \
									(_cmd) == SC_WRITE16 )
// SCSI Command Code definitions
// The first byte of the CDB is the operation code.  The top 3 bits
// are the group code field and the bottom 5 bits are the command code
// field.
// Group 0 - six-byte commands
// Group 1 - ten-byte commands
// Group 2 - ten-byte commands
// Group 3 - reserved
// Group 4 - reserved
// Group 5 - twelve-byte commands
// Group 6 - vendor specific
// Group 7 - vendor specific
#define SC_GROUP( _cmd )    ( (_cmd) >> 5 )
// Group 0 Command Codes (00H - 1FH) (six byte commands)
#define SC_UNIT_RDY		0x00	// Test Unit Ready
#define SC_REZERO		0x01	// Rezero Unit
#define SC_REWIND		0x01	// Rewind (sequential-access device)
#define SC_VEN_02		0x02	// Vendor specific
#define SC_RQ_SENSE		0x03	// Request Sense
#define SC_FORMAT		0x04	// Format unit
#define SC_LIMITS		0x05	// Read Block Limits (sequential-access device)
#define SC_VEN_06		0x06	// Vendor specific
#define SC_REASSIGN		0x07	// Reassign Blocks
#define SC_READ			0x08	// Read
#define SC_VEN_09		0x09	// Vendor specific
#define SC_WRITE		0x0a	// Write
#define SC_SEEK			0x0b	// Seek
#define SC_TRACK_SEL	0x0b	// Track Select (sequential-access device)
#define SC_VEN_0C		0x0C	// Vendor specific
#define SC_VEN_0D		0x0D	// Vendor specific
#define SC_VEN_0E		0x0E	// Vendor specific
#define SC_RD_REVERSE	0x0f	// Read Reverse (sequential-access device)
#define SC_WR_FM		0x10	// Write Filemarks (sequential-access device)
#define SC_SPACE		0x11	// Space (sequential-access device)
#define SC_INQUIRY		0x12	// Inquiry
#define SC_VERIFY		0x13	// Verify
#define SC_RECOVER_BD	0x14	// Recover Buffered Data (sequential-access device)
#define SC_MSELECT		0x15	// Mode Select
#define SC_RES_UNIT		0x16	// Reserve Unit
#define SC_REL_UNIT		0x17	// Release Unit
#define SC_COPY			0x18	// Copy
#define SC_ERASE		0x19	// Erase (sequential-access device)
#define SC_MSENSE		0x1a	// Mode Sense
#define SC_SPINDLE		0x1b	// Start/Stop Unit
#define SC_LOAD			0x1b	// Load/Unload (sequential-access device)
#define SC_RCV_DIAG		0x1c	// Receive Diagnostic Results
#define SC_SND_DIAG		0x1d	// Send Diagnostic
#define SC_REMOVAL		0x1e	// Prevent/Allow Medium Removal
#define SC_RES_1F		0x1f	// (reserved)

// Group 1 Command Codes (20H - 3FH) (ten byte commands)
#define SC_VEN_20		0x20	// Vendor specific
#define SC_VEN_21		0x21	// Vendor specific
#define SC_VEN_22		0x22	// Vendor specific
#define SC_VEN_23		0x23	// Vendor specific
#define SC_READ_FORMATED_CAPACITIES		0x23
#define SC_SET_WIN		0x24	// Set Window (scanner device)
#define SC_GET_WIN		0x25	// Get Window (scanner device)
#define SC_RD_CAP		0x25	// Read Capacity
#define SC_VEN_26		0x26	// Vendor specific
#define SC_VEN_27		0x27	// Vendor specific
#define SC_READ10		0x28	// Read 10 byte
#define SC_RD_GEN		0x29	// Read Generation (optical-memory device)
#define SC_WRITE10		0x2a	// Write
#define SC_SEEK10		0x2b	// Seek
#define SC_POSITION		0x2b	// Position to Element
#define SC_LOCATE		0x2b	// Locate (sequential-access)
#define SC_ERASE10		0x2c	// Erase (optical-memory)
#define SC_RD_UB		0x2d	// Read Updated Block (optical-memory)
#define SC_WR_VER		0x2e	// Write with Verify
#define SC_VERIFY10		0x2f	// Verify
#define SC_SRCH_HI		0x30	// Search Data High
#define SC_SRCH_EQ		0x31	// Search Data Equal
#define SC_SRCH_LO		0x32	// Search Data Low
#define SC_LIMITS10		0x33	// Set Limits
#define SC_PREFETCH		0x34	// Pre-fetch (pre-read into cache)
#define SC_RD_POS		0x34	// Read position (sequential-access device)
#define SC_SYNC			0x35	// Synchronize Cache
#define SC_CACHE		0x36	// Lock/Unlock Cache
#define SC_RD_DEF		0x37	// Read Defect Data
#define SC_SCAN			0x38	// Medium Scan (optical-memory)
#define SC_COMPARE		0x39	// Compare
#define SC_CPY_VER		0x3a	// Copy and Verify
#define SC_WR_BUF		0x3b	// Write Buffer
#define SC_RD_BUF		0x3c	// Read Buffer
#define SC_UPD_BLK		0x3d	// Update Block (optical-memory)
#define SC_RD_LONG		0x3e	// Read long
#define SC_WR_LONG		0x3f	// Write long

// Group 2 Command Codes (40H - 5FH) (ten byte commands)
#define SC_CHG_DEF		0x40	// Change definition (SCSI-1, SCSI-2, CCS)
#define SC_WR_SAME		0x41	// Write same
#define SC_UNMAP		0x42	// Unmap (Thin Provisioning)
#define SC_RD_SUBCH		0x42	// Read Sub-Channel (cd-rom)
#define SC_RD_TOC		0x43	// Read TOC (cd-rom)
#define SC_RD_HDR		0x44	// Read Header (cd-rom)
#define SC_REPORT_DENSITY	0x44	// Report Density Support
#define SC_PLAY			0x45	// Play Audio(10) (cd-rom)
#define SC_RES_46		0x46	// (reserved)
#define SC_GET_CONFIGURATION	0x46
#define SC_PLAY_MSF		0x47	// Play Audio MSF (cd-rom)
#define SC_PLAY_TI		0x48	// Play Audio Track Index (cd-rom)
#define SC_PLAY_TR		0x49	// Play Track Relative(10) (cd-rom)
#define SC_RES_4A		0x4a	// (reserved)
#define SC_GET_EVENT_STATUS	0x4a	// Get Event Status Notification
#define SC_PAUSE		0x4b	// Pause Resume (cd-rom)
#define SC_LSELECT		0x4c	// Log Select
#define SC_LSENSE		0x4d	// Log Sense
#define SC_RES_4E		0x4e	// (reserved)
#define SC_STOP_PLAY_SCAN 0x4e	// Stop Play/Scan (ATAPI)
#define SC_RES_4F		0x4f	// (reserved)
#define SC_READ_DISC_INFORMATION	0x51
#define SC_READ_TRACK_INFORMATION	0x52
#define SC_MSELECT10	0x55	// Mode Select
#define SC_MSENSE10		0x5a	// Mode Sense
#define SC_PER_RES_IN   0x5e	// Persistent Reserve In
#define SC_PER_RES_OUT  0x5f	// Persistent Reserve Out

// Group 3 and 4, command codes 60H-9FH are reserved
#define SC_ATA_PT16                     0x85	// ATA Pass-through
#define SC_READ16						0x88	// Read
#define SC_WRITE16						0x8a	// Write
#define SC_WR_VER16						0x8e	// Write and Verify
#define SC_SERVICE_ACTION_IN12          0xab
#define SC_SERVICE_ACTION_OUT12         0xa9
#define SC_SERVICE_ACTION_IN16          0x9e
#define SC_SERVICE_ACTION_OUT16         0x9f
	#define SAI_SERVICE_ACTION_MSK      0x1f
	#define SAI_READ_CAPACITY_16        0x10
	#define SAI_READ_LONG_16            0x11
	#define SAI_GET_LBA_STATUS          0x12
#define SC_WR_SAME16					0x93	// Write same

// Group 5 command codes (A0H - BFH) (twelve byte commands)
#define SC_RES_A0		0xa0	// (reserved)
#define SC_RES_A1		0xa1	// (reserved)
#define SC_RES_A2		0xa2	// (reserved)
#define SC_RES_A3		0xa3	// (reserved)
#define SC_SENDKEY		0xa3	// DVD CSS
#define SC_REPORTKEY	0xa4	// DVD CSS
#define SC_MOVE			0xa5	// Move Medium (media-changer)
#define SC_PLAY12		0xa5	// Play Audio (cd-rom)
#define SC_EXCHANGE		0xa6	// Exchange Medium (media-changer)
#define SC_RES_A7		0xa7	// (reserved)
#define SC_GET_MSG		0xa8	// Get Message (communications device)
#define SC_READ12		0xa8	// Read (cd-rom)
#define SC_PLAY_TR12	0xa9	// Play Track Relative (cd-rom)
#define SC_SEND_MSG		0xaa	// Send Message (communications device)
#define SC_WRITE12		0xaa	// Write (optical-memory)
#define SC_RES_AB		0xab	// (reserved)
#define SC_ERASE12		0xac	// Erase (optical-memory)
#define SC_READDVDSTRUCT 0xad
#define SC_WR_VER12		0xae	// Write and Verify (optical-memory)
#define SC_VERIFY12		0xaf	// Verify (optical-memory)
#define SC_SRCH_HI12	0xb0	// Search Data High (cd-rom)
#define SC_SRCH_EQ12	0xb1	// Search Data Equal (cd-rom)
#define SC_SRCH_LO12	0xb2	// Search Data Low (cd-rom)
#define SC_LIMITS12		0xb3	// Set Limits (cd-rom)
#define SC_CD_SPEED		0xbb	// Set CD Speed (ATAPI)
#define SC_PLAY_CD		0xbc	// Play CD (ATAPI)
#define SC_READ_CD_MSF	0xb9	// Read CD MSF (ATAPI)
#define SC_CD_SCAN		0xba	// Scan (ATAPI)
#define SC_MECHANISM_STATUS	0xbd	// Mechanism Status (ATAPI)
#define SC_READ_CD		0xbe	// Read CD (ATAPI)

// Group 6 and 7, command codes C0H-FFH are vendor specific (Some WORMs)
#define SC_STANDBY		0xce	// Standby Check
#define SC_LOAD_G6		0xcf	// Load/Unload Unit
#define SC_RWD_DCB		0xd2	// Read DCB
#define SC_DREAD		0xee	// Disk Read
#define SC_DWRITE		0xef	// Disk Write

// Command Descriptor Block structure definitions

// CDB Flags
#define CF_LINK			0x01	// Linked-command indication
#define CF_FLAG			0x02	// Linked-command with flag bit
#define CF_VENDOR0		0x40	// Vendor unique bits
#define CF_VENDOR1		0x80

#define CF_FUA			0x08
#define CF_DPO			0x10

typedef struct _mode_sense	MODE_SENSE;
typedef struct _mode_select	MODE_SELECT;

typedef union _cdb {
	// generic 6 byte command descriptor block
	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba_byte1;
		uchar_t		lba_byte0;				// LSB
		uchar_t		transfer_len;
		uchar_t		control;
	} gen6;

	// generic 10 byte command descriptor block
	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba_byte3;
		uchar_t		lba_byte4;
		uchar_t		lba_byte1;
		uchar_t		lba_byte0;
		uchar_t		rsvd;
		uchar_t		transfer_len[2];
		uchar_t		control;
	} gen10;

	// generic 12 byte command descriptor block
	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba_byte3;
		uchar_t		lba_byte4;
		uchar_t		lba_byte1;
		uchar_t		lba_byte0;
		uchar_t		transfer_len[4];
		uchar_t		rsvd10;
		uchar_t		control;
	} gen12;

	struct {
		uchar_t		opcode;
#define INQ_OPT_EVPD			0x01
		uchar_t		lun_opt;
#define INQ_PC_VPD_SUPPORTED						0x00
/* 0x1 - 0x7f ASCII Information */
#define INQ_PC_UNIT_SERIAL_NUMBER					0x80
#define INQ_PC_DEVICE_IDENTIFICATION				0x83
#define INQ_PC_SOFTWARE_INTERFACE_IDENTIFICATION	0x84
#define INQ_PC_MANAGEMENT_NETWORK_ADDRESSES			0x85
#define INQ_PC_EXTENDED_INQUIRY_DATA				0x86
#define INQ_PC_MODE_PAGE_POLICY						0x87
#define INQ_PC_SCSI_PORTS							0x88
#define INQ_PC_BLOCK_LIMITS							0xb0
#define INQ_PC_BLOCK_DEVICE_CHARACTERISTICS			0xb1
#define INQ_PC_THIN_PROVISIONING					0xb2
		uchar_t		pc;					// Page Control
		uchar_t		rsvd2;
		uchar_t		transfer_len;
		uchar_t		control;
	} inquiry;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd1;
		uchar_t		rsvd2;
		uchar_t		transfer_len;
		uchar_t		control;
	} request_sense;

	struct _format_unit {
        uchar_t         op_code;
#define FU_RSVD0        0xc0                    // reserved bits
#define FU_FMTDAT       0x10
#define FU_CMPLIST      0x08
        uchar_t         defect_list_fmt;
        uchar_t         track_num;
        ushort_t        interleave;
        uchar_t         rsvd1[7];
	} format_unit;

	struct _format_unit_old {
        uchar_t         op_code;
        uchar_t         rsvd0;
        uchar_t         medium_type_code;
        uchar_t         rsvd1;
        uchar_t         interleave;
        uchar_t         rsvd2;
#define FMT_RSVD3               0x80
#define FMT_SECT_SIZE_CD        0x70
#define FMT_IMMED               0x08
#define FMT_HEAD                0x04
#define FMT_ST                  0x02
#define FMT_CERT                0x01
        uchar_t         cert;
        uchar_t         track_addr;
        uchar_t         rsvd4[4];
	} format_unit_old;

#define RW_OPT_RELADR	0x01
#define RW_OPT_CORRCT	0x02					// Disable Corrections
#define RW_OPT_FUA		0x08					// Force Unit Access
#define RW_OPT_DPO		0x10					// Disable Page Out
	struct {
		uchar_t		opcode;
		uchar_t		lun_lba;
		uchar_t		lba[2];
		uchar_t		transfer_len;
		uchar_t		control;
	} read_write6;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba[4];
		uchar_t		rsvd2;
		uchar_t		transfer_len[2];
		uchar_t		control;
	} read_write10;

	struct {
		uchar_t		opcode;
#define WV_OPT_BYTCHK	0x02
		uchar_t		lun_opt;
		uchar_t		lba[4];
		uchar_t		group;
		uchar_t		transfer_len[2];
		uchar_t		control;
	} write_verify10;

	struct {
		uchar_t		opcode;
#define WS_OPT_UNMAP	0x08
#define WS_OPT_PBDATA	0x04
#define WS_OPT_LBDATA	0x02
		uchar_t		opt;
		uchar_t		lba[4];
		uchar_t		group;
		uchar_t		transfer_len[2];
		uchar_t		control;
	} write_same10;

	struct {
		uchar_t		opcode;
		uchar_t		rsvd1_5[5];
		uchar_t		group_number;
		uchar_t		param_length[2];
		uchar_t		control;
	} unmap;

	struct {
		uchar_t		opcode;
#define WS_OPT_UNMAP	0x08
#define WS_OPT_PBDATA	0x04
#define WS_OPT_LBDATA	0x02
		uchar_t		opt;
		uchar_t		lba[8];
		uchar_t		transfer_len[4];
		uchar_t		group;
		uchar_t		control;
	} write_same16;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba[4];
		uchar_t		transfer_len[4];
		uchar_t		group;
		uchar_t		control;
	} read_write12;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba[8];
		uchar_t		transfer_len[4];
		uchar_t		group;
		uchar_t		control;
	} read_write16;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba[4];
		uchar_t		transfer_len[4];
		uchar_t		rsvd2;
		uchar_t		control;
	} write_verify12;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		lba[8];
		uchar_t		transfer_len[4];
		uchar_t		rsvd2;
		uchar_t		control;
	} write_verify16;

#define MSEL_OPT_PF		0x10			// Page Format
#define MSEL_OPT_SP		0x01			// Save Page
	struct _mode_select {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		param_length;
		uchar_t		control;
	} mode_select;

	struct _mode_select10 {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		param_length[2];
		uchar_t		control;
	} mode_select10;

	struct {
		uchar_t		opcode;
#define LS_OPT_SP		0x01			// Save Parameters
#define LS_OPT_PCR		0x02			// Parameter Code Reset
		uchar_t		lun_opt;
#define LS_PC_CUR_THRESHOLD		0x00
#define LS_PC_CUR_CUMULATIVE	0x01
#define LS_PC_DFLT_THRESHOLD	0x02
#define LS_PC_DFLT_CUMULATIVE	0x03
		uchar_t		pc;					// Page Control
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		param_length[2];
		uchar_t		control;
	} log_select;

	struct _mode_sense {
		uchar_t		opcode;
#define MSNS_OPT_DBD	0x08			// Disable Block Descriptors
		uchar_t		lun_opt;
#define PC_CURRENT		0x00
#define PC_CHANGEABLE	0x40
#define PC_DEFAULT		0x80
#define PC_SAVED		0xC0
#define PC_MSK			0xC0
#define PAGE_MSK		0x3f
		uchar_t		pc_page;
		uchar_t		subpage;
		uchar_t		allocation_length;
		uchar_t		control;
	} mode_sense;

	struct _mode_sense10 {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		pc_page;
		uchar_t		subpage;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} mode_sense10;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		pc_page;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		parameter_pointer[2];
		uchar_t		allocation_length[2];
		uchar_t		control;
	} log_sense;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		prevent;
		uchar_t		control;
	} removal;

	struct {
		uchar_t		opcode;
#define LD_OPT_IMMED	0x01
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
#define LD_CMD_START	0x01
#define LD_CMD_LOEJ		0x02
#define LD_CMD_STOP		0x00
#define LD_CMD_EJECT	0x02
#define LD_CMD_LOAD		0x03

// Sequential-Access
#define LD_CMD_SA_HOLD		0x08
#define LD_CMD_SA_EOT		0x04
#define LD_CMD_SA_RT		0x02			// re-tension
#define LD_CMD_SA_LOEJ		0x01

// Block
#define LD_CMD_PC_MSK		0xf0
#define LD_CMD_PC_NC		0
#define LD_CMD_PC_ACTIVE	1
#define LD_CMD_PC_IDLE		2
#define LD_CMD_PC_STANDBY	3
#define LD_CMD_PC_SLEEP		5

		uchar_t		cmd;
		uchar_t		control;
	} load;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
#define SC_OPT_RELADR	0x01
#define SC_OPT_IMMED	0x02
		uchar_t		lba[4];
		uchar_t		num_blocks[2];
		uchar_t		control;
	} synchronize_cache;

// cdrom commands
	struct {
		uchar_t		opcode;
		uchar_t		rsvd1;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} read_disc_information;

	struct {
		uchar_t		opcode;
#define RTI_TYPE_LBA		0x00
#define RTI_TYPE_TRK		0x01
#define RTI_TYPE_BORDER		0x02
		uchar_t		type;
		uchar_t		address_number[4];
		uchar_t		rsvd3;
		uchar_t		allocation_length[2];
		uchar_t		control;
		uchar_t		rsvd10;
		uchar_t		rsvd11;
	} read_track_information;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		rsvd7;
#define PR_PAUSE			0x00
#define PR_RESUME			0x01
		uchar_t		resume;
		uchar_t		control;
	} pause_resume;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		start_minute;
		uchar_t		start_second;
		uchar_t		start_frame;
		uchar_t		end_minute;
		uchar_t		end_second;
		uchar_t		end_frame;
		uchar_t		control;
	} play_audio_msf;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		start_track;
		uchar_t		start_index;
		uchar_t		rsvd6;
		uchar_t		end_track;
		uchar_t		end_index;
		uchar_t		control;
	} play_audio_ti;

	struct {
		uchar_t		opcode;
#define CD_SCAN_DIR_FORWARD		0x00
#define CD_SCAN_DIR_REVERSE		0x10
		uchar_t		opt;
		uchar_t		start_address[4];
#define CD_SCAN_TYPE_LBA		0x00
#define CD_SCAN_TYPE_MSF		0x40
#define CD_SCAN_TYPE_TRK		0x80
#define CD_SCAN_TYPE_MSK		0xc0
		uchar_t		rsvd6;
		uchar_t		rsvd7;
		uchar_t		rsvd8;
		uchar_t		type;
		uchar_t		rsvd10;
		uchar_t		rsvd11;
	} cd_scan;

	struct {
		uchar_t		opcode;
#define RTOC_OPT_MSF	0x02
		uchar_t		lun_opt;
#define RTOC_FMT_TOC		0x0
#define RTOC_FMT_SESSION	0x1
#define RTOC_FMT_QSUBCODE	0x2
#define RTOC_FMT_QSUBCHNL	0x3
#define RTOC_FMT_ATIP		0x4
#define RTOC_FMT_CDTEXT		0x5
		uchar_t		format;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		start_track;
		uchar_t		allocation_length[2];
#define RTOC_CNTL_FMT_SESSION	0x40
		uchar_t		control_format;
	} read_toc;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2[6];
		uchar_t		allocation_length[2];
		uchar_t		rsvd3[2];
	} mechanism_status;

	struct {
		uchar_t		opcode;
#define EXCHANGE_OPT_IMMED	0x01
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
#define EXCHANGE_CMD_START	0x01
#define EXCHANGE_CMD_LOEJ	0x02
		uchar_t		cmd;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		rsvd7;
		uchar_t		slot;
		uchar_t		rsvd9;
		uchar_t		rsvd10;
		uchar_t		rsvd11;
	} exchange;

	struct {
		uchar_t		opcode;
		uchar_t		rt;
		uchar_t		feature_number[2];
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} get_configuration;

	struct {
		uchar_t		opcode;
#define GE_OPT_POLLED			0x01
		uchar_t		opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
#define NCR_OPERATIONAL_CHANGE	0x02
#define NCR_POWER_MANAGEMENT	0x04
#define NCR_EXTERNAL_REQUEST	0x08
#define NCR_MEDIA				0x10
#define NCR_MULTI_INITIATOR		0x20
#define NCR_DEVICE_BUSY			0x40
		uchar_t		ncr;         // notification class request
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} get_event;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2;
		uchar_t		rsvd3;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		rsvd6;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} read_formated_capacities;

	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		read_speed[2];
		uchar_t		write_speed[2];
		uchar_t		rsvd2[6];
	} cd_speed;		

	struct {
		uchar_t		opcode;
#define RSCHNL_OPT_MSF		0x02
		uchar_t		lun_opt;
#define RSCHNL_DATA_SUBQ	0x40
		uchar_t		data;
		uchar_t		data_format;
		uchar_t		rsvd4;
		uchar_t		rsvd5;
		uchar_t		track;
		uchar_t		allocation_length[2];
		uchar_t		control;
	} read_subchannel;

#define CD_FRAME_SYNC_SIZE         12
#define CD_FRAME_HDR_SIZE           4
#define CD_FRAME_SUB_HDR_SIZE       8
#define CD_FRAME_EDC_SIZE           4
#define CD_FRAME_ECC_SIZE         276
#define CD_FRAME_AUX_SIZE           8
#define CD_FRAME_ZERO_SIZE          8
#define CD_FRAME_SPARE_SIZE         4
#define CD_FRAME_C2_ERR_SIZE      294
#define CD_FRAME_BLOCK_ERR_SIZE     2

	struct {
		uchar_t		opcode;
		uchar_t		lun_stype;
// expected sector type
#define RDCD_EST_ANY_SECTOR				(0 << 2)
#define RDCD_EST_CDDA_SECTOR			(1 << 2)
#define RDCD_EST_YELLOW_MODE1_SECTOR	(2 << 2)
#define RDCD_EST_YELLOW_MODE2_SECTOR	(3 << 2)
#define RDCD_EST_XA_SECTOR				(4 << 2)
#define RDCD_EST_XA_FORM2_SECTOR		(5 << 2)
#define RDCD_EST_MSK					(7 << 2)
		uchar_t		lba[4];
		uchar_t		transfer_len[3];
		uchar_t		flags;
#define RDCD_FLG_ALL			0xf8
#define RDCD_FLG_SYNC			0x80
#define RDCD_FLG_UDATA			0x10
#define RDCD_FLG_ECC			0x08
#define RDCD_FLG_CD_ERR			0x02
#define RDCD_FLG_CD_BLOCK_ERR	0x04
#define RDCD_FLG_HC_NONE		( 0x00 << 5 )
#define RDCD_FLG_HC_HDR			( 0x01 << 5 )
#define RDCD_FLG_HC_SUBHEADER	( 0x02 << 5 )
#define RDCD_FLG_HC_ALL_HEADERS	( 0x03 << 5 )
		uchar_t		subch_selection;
		uchar_t		rsvd3;
	} read_cd;

	struct {
		uchar_t		opcode;
		uchar_t		lun_stype;
		uchar_t		rsvd2;
		uchar_t		start_minute;
		uchar_t		start_second;
		uchar_t		start_frame;
		uchar_t		end_minute;
		uchar_t		end_second;
		uchar_t		end_frame;
		uchar_t		flags;
		uchar_t		subch_selection;
		uchar_t		rsvd11;
	} read_cd_msf;

	struct {
		uchar_t		opcode;
		uchar_t		media_type;
		uchar_t		address[4];
		uchar_t		layer;
#define RDS_FMT_PHYSICAL		0x00
#define RDS_FMT_COPYRIGHT		0x01
#define RDS_FMT_DISCKEY			0x02
#define RDS_FMT_BCA				0x03
#define RDS_FMT_MANUFACTURER	0x04
#define RDS_FMT_CGMS_CPM		0x05
		uchar_t		format;
		uchar_t		allocation_length[2];
		uchar_t		agid;
		uchar_t		control;
	} read_dvd_structure;

	struct _ata_pass_thru {
		uchar_t		opcode;
#define ATA_PROTO_MSK				0x1e
#define ATA_PROTO_RESPONSE			(15 << 1)
#define ATA_PROTO_FPDMA				(12 << 1)
#define ATA_PROTO_UDMA_DATA_OUT		(11 << 1)
#define ATA_PROTO_UDMA_DATA_IN		(10 << 1)
#define ATA_PROTO_DEVICE_RESET		(9 << 1)
#define ATA_PROTO_DEVICE_DIAGNOSTIC	(8 << 1)
#define ATA_PROTO_DMA_QUEUED		(7 << 1)
#define ATA_PROTO_DMA				(6 << 1)
#define ATA_PROTO_PIO_DATA_OUT		(5 << 1)
#define ATA_PROTO_PIO_DATA_IN		(4 << 1)
#define ATA_PROTO_DATA_NONE			(3 << 1)
#define ATA_PROTO_SRST				(1 << 1)
#define ATA_PROTO_HRST				(0 << 1)
#define ATA_PROTO_EXTEND			0x01
		uchar_t		protocol;      // multiple count, protocol
#define ATA_MCOUNT_MSK				0xe0

#define ATA_FLG_CK_COND				0x20
#define ATA_FLG_T_DIR				0x08		// data from device
#define ATA_FLG_BYT_BLOK			0x04
#define ATA_FLG_TLEN_STPSIU			0x03
#define ATA_FLG_TLEN_SECTOR_COUNT	0x02
#define ATA_FLG_TLEN_FEATURE		0x01
		uchar_t		flags;

		uchar_t		efeatures;
		uchar_t		features;
		uchar_t		esector_count;
		uchar_t		sector_count;
		uchar_t		elba_low;
		uchar_t		lba_low;
		uchar_t		elba_mid;
		uchar_t		lba_mid;
		uchar_t		elba_high;
		uchar_t		lba_high;
		uchar_t		device;
		uchar_t		command;
		uchar_t		control;
	} ata_pass_thru;

	struct {
		uchar_t		opcode;
		uchar_t		rsvd[1];
		uchar_t		lba[2];
		uchar_t		rsvd6_7[2];
		uchar_t		pmi;
		uchar_t		control;
	} read_capacity10;

	struct {
		uchar_t		opcode;
		uchar_t		service_action;
		uchar_t		lba[8];
		uchar_t		allocation_length[4];
		uchar_t		pmi;
		uchar_t		control;
	} read_capacity16;

// sequential access commands
	struct {
		uchar_t		opcode;
#define ERASE_OPT_LONG	0x01
		uchar_t		opt;
		uchar_t		rsvd[3];
		uchar_t		control;
	} erase;

	struct {
		uchar_t		opcode;
		uchar_t		opt;
		uchar_t		lba[4];
		uchar_t		rsvd[1];
		uchar_t		transfer_len[2];
		uchar_t		control;
	} erase10;

	struct {
		uchar_t		opcode;
		uchar_t		opt;
		uchar_t		lba[4];
		uchar_t		transfer_len[4];
		uchar_t		rsvd[1];
		uchar_t		control;
	} erase12;

	struct {
		uchar_t		opcode;
#define LOCATE_OPT_CP	0x2
#define LOCATE_OPT_BT	0x4
		uchar_t		opt;
		uchar_t		rsvd2;
		uchar_t		ba[4];			// block address
		uchar_t		rsvd7;
		uchar_t		partition;
		uchar_t		control;
	} locate;

	struct {
		uchar_t		opcode;
		uchar_t		opt;
		uchar_t		rsvd2[3];
		uchar_t		control;
	} read_block_limits;

#define RP_OPT_BT	0x01			// block address type
#define RP_OPT_LNG	0x02			// long format
#define RP_OPT_TCLP	0x04			// total current logical position
	struct {
		uchar_t		opcode;
		uchar_t		lun_opt;
		uchar_t		rsvd2[7];
		uchar_t		control;
	} read_position;

#define SRW_OPT_FIXED	0x01
#define SRW_OPT_SILI	0x02
	struct {
		uchar_t		opcode;
		uchar_t		opt;
		uchar_t		transfer_len[3];
		uchar_t		control;
	} sa_read_write;

	struct {
		uchar_t		opcode;
		uchar_t		opt;
		uchar_t		rsvd[3];
		uchar_t		control;
	} rewind;

	struct {
		uchar_t		opcode;
#define SPACE_CODE_BLOCKS		0x00
#define SPACE_CODE_FMRKS		0x01
#define SPACE_CODE_SEQ_FMRKS	0x02
#define SPACE_CODE_EOD			0x03
#define SPACE_CODE_SMRKS		0x04
#define SPACE_CODE_SEQ_SMRKS	0x05
		uchar_t		lun_code;
		uchar_t		count[3];
		uchar_t		control;
	} space;

	struct {
		uchar_t		opcode;
#define WF_OPT_IMMED	0x01
#define WF_OPT_WSMK		0x02
		uchar_t		opt;
		uchar_t		transfer_length[3];
		uchar_t		control;
	} write_filemarks;

	struct {
		uchar_t		opcode;
#define RD_OPT_MEDIA	0x01
		uchar_t		opt;
		uchar_t		rsvd[5];
		uchar_t		allocation_length[2];
		uchar_t		control;
	} report_density;

	struct {
		uchar_t		opcode;
#define FM_OPT_IMMED	0x01
#define FM_OPT_VERIFY	0x02
		uchar_t		opt;
#define FM_FMT_DFLT				0x00
#define FM_FMT_PARTITION		0x01
#define FM_FMT_FORMAT_PARTITION	0x02
		uchar_t		format;
		uchar_t		transfer_length[2];
		uchar_t		control;
	} format_media;
} CDB;

// SCSI status bytes
// (as per X3T9.2 revision 10 -- Table 27)
#define SCS_MASK		0x3e	// Mask valid bits
#define SCS_GOOD		0x00	// Good
#define SCS_CHECK		0x02	// Check Condition
#define SCS_MET			0x04	// Condition Met/Good
#define SCS_BUSY		0x08	// Target/LUN is Busy
#define SCS_INTER		0x10	// Intermediate/Good
#define SCS_INTERMET	0x14	// Intermediate/Condition Met/Good
#define SCS_CONFLICT	0x18	// Reservation Conflict
#define SCS_TERM		0x21	// Command Terminated
#define SCS_QUEUEFULL	0x28	// Queue Full

// SCSI Message Description
// (as per X3T9.2 revision 10 -- Table 10)
// The first byte in the message determines the message format.
// 		One-Byte Message (COMMAND COMPLETE)		0x00
//		Extended Message						0x01
//		One-Byte Message						0x02 - 0x1F
//		Two-Byte Message						0x20 - 0x2F
//		Reserved								0x30 - 0x7F
//		One-Byte Message (IDENTIFY)				0x80 - 0xFF
#define SCM_COMMAND_COMPLETE		0x00
#define SCM_EXTENDED_MSG			0x01
	#define SCM_MODIFY_DATA_POINTER	0x00
	#define SCM_SYNC_XFER_REQUEST	0x01
	#define SCM_EXTENDED_IDENTIFY	0x02
	#define SCM_WIDE_XFER_REQUEST	0x03
	#define SCM_WIDE_LEN			0x02
	#define SCM_SYNC_LEN			0x03
	#define SCM_MODIFY_LEN			0x05
#define SCM_SAVE_DATA_POINTER		0x02
#define SCM_RESTORE_POINTERS		0x03
#define SCM_DISCONNECT				0x04
#define SCM_INITIATOR_DETECTED_ERR	0x05
#define SCM_ABORT					0x06
#define SCM_MESSAGE_REJECT			0x07
#define SCM_NO_OPERATION			0x08
#define SCM_MESSAGE_PARITY_ERROR	0x09
#define SCM_LINKED_CMD_COMPLETE		0x0a
#define SCM_LINKED_CMD_COMPLETE_FL	0x0b
#define SCM_BUS_DEVICE_RESET		0x0c
#define SCM_ABORT_TAG				0x0d
#define SCM_CLEAR_QUEUE				0x0e
#define SCM_INITIATE_RECOVERY		0x0f
#define SCM_RELEASE_RECOVERY		0x10
#define SCM_TERM_IO_PROCESS			0x11
#define SCM_SIMPLE_QUEUE_TAG		0x20
#define SCM_HEAD_OF_QUEUE_TAG		0x21
#define SCM_ORDERED_QUEUE_TAG		0x22
#define SCM_IGNORE_WIDE_RESIDUE		0x23
#define SCM_IDENTIFY				0x80
	#define SCM_DISC_PRIV			0x40


// Definition of the SCSI-2 INQUIRY data format
#define INQUIRY_SHORT_LEN   36
typedef struct _scsi_inquiry {
	uchar_t peripheral;				// Peripheral Qualifier/Device Type
	uchar_t	rmb;					// Removable Medium
	uchar_t	version;				// Versions (ISO, ECMA, ANSI)
	uchar_t	rdf;					// Response Data Format/Flags
	uchar_t	adlen;					// Additional Data Length
	uchar_t	rsvd1[2];
	uchar_t	flags;
	uchar_t	vend_id[8];				// Vendor identification
	uchar_t	prod_id[16];			// Product identification
	uchar_t	prod_rev[4];			// Product revision level
	uchar_t	vend_spc[20];			// Vendor Specific
	uchar_t	rsvd3[40];
} SCSI_INQUIRY;

#define INQ_DTYPE_MSK		0x1f
#define INQ_DTYPE_WILDCARD	0xff

// INQUIRY Data flags
#define INQ_QUAL_MSK		0xe0
#define INQ_QUAL_AVAIL		0x00
#define INQ_QUAL_NOTAVAIL	0x20
#define INQ_QUAL_RESERVED	0x40
#define INQ_QUAL_NOTSUP		0x60
#define INQ_QUAL_VENDOR		0x80
#define INQ_RMB				0x80	// Removable Medium Bit
#define INQ_VANSI			0x07	// ANSI Approved Version
#define INQ_VECMA			0x38	// ECMA
#define INQ_VISO			0xc0	// ISO
#define INQ_AENC			0x80	// Asynchronous Event Notificaton Capability
#define INQ_TRM_IOP			0x40	// Terminate I/O Process bit
#define INQ_RDF				0x0f	// Response Data Format mask
#define INQ_REL_ADR			0x80	// Relative Addressing bit
#define INQ_WBUS32			0x40	// Wide Bus 32 (32 bit bus) support
#define INQ_WBUS16			0x20	// Wide Bus 16 (16 bit bus) support
#define INQ_SYNC			0x10	// Synchronous Data Transfers supported
#define INQ_LINKED			0x08	// Linked commands supported
#define INQ_RES				0x04	// (reserved)
#define INQ_CMD_QUE			0x02	// Tagged Command Queuing supported
#define INQ_SFT_RST			0x01	// Soft RESET alternative supported

#define INQ_VER_SPC3		0x05

// Sense Keys
#define SK_MSK			0x0F	// mask to sd_sense field for key

#define SK_NO_SENSE		0		// No sense data (no error)
		#define ASCQ_FILEMARK_DETECTED			0x01
		#define ASCQ_EOPM_DETECTED				0x02	// End of Partition/Medium Detected
		#define ASCQ_SETMARK_DETECTED			0x03
		#define ASCQ_BOPM_DETECTED				0x04	// Beginning of Partition/Medium Detected

#define SK_RECOVERED	1		// Recovered error
		#define ASC_ATA_PASS_THRU					0x00
			#define ASCQ_ATA_PASS_THRU_INFO_AVAIL	0x1d

#define SK_NOT_RDY		2		// Device not ready
	#define ASC_NO_SEEK_COMPLETE				0x02
	#define ASC_NOT_READY						0x04
		#define ASCQ_CAUSE_NOT_REPORTABLE			0x00
		#define ASCQ_BECOMING_READY					0x01
		#define ASCQ_INIT_COMMAND_REQUIRED			0x02
		#define ASCQ_MANUAL_INTERVENTION_REQUIRED	0x03
		#define ASCQ_FORMAT_IN_PROGRESS				0x04
		#define ASCQ_UNKNOWN_CHANGED				0xff	// NTO extension for fdc's
	#define ASC_MEDIA_FORMAT					0x30		// bad format
	#define ASC_MEDIA_NOT_PRESENT				0x3a
	#define ASC_NOT_CONFIGURED					0x3e

#define SK_MEDIUM		3		// Medium error
	#define ASC_WRITE_ERROR					0x0c
	#define ASC_UNRECOVERABLE_READ_ERROR	0x11
	#define ASC_RECORD_NOT_FOUND			0x14
		#define ASCQ_RECORD_NOT_FOUND		0x01
	#define ASC_UNABLE_TO_RECOVER_TOC		0x57
	#define ASC_INCOMPATIBLE_MEDIUM			0x64

#define SK_HARDWARE		4		// Hardware error
	#define ASC_INTERNAL_TARGET_FAILURE		0x44
	#define ASC_MEDIA_LOAD_EJECT_FAILURE	0x53
		#define ASCQ_UNRECOVERABLE_CIRC				0x06

#define SK_ILLEGAL		5		// Illegal Request (bad command)
	#define ASC_INVALID_COMMAND			0x20
	#define ASC_INVALID_FIELD			0x24
	#define ASC_INVALID_FIELD_PARAMETER	0x26
	#define ASC_COMMAND_SEQUENCE_ERROR	0x2c
		#define ASCQ_READ_SCRAMBLED		0x03
	#define ASC_ILLEGAL_MODE			0x64
	#define ASC_COPY_PROTECTION			0x6f

#define SK_UNIT_ATN		6		// Unit Attention
	#define ASC_MEDIUM_CHANGED					0x28
	#define ASC_BUS_RESET						0x29
	#define ASC_INSUFFICIENT_TIME_FOR_OPERATION	0x2e
	#define ASC_OPERATOR_REQUEST				0x5a
		#define ASCQ_OPERATOR_MEDIUM_REMOVAL	0x01

#define SK_DATA_PROT	7		// Data Protect
	#define ASC_WRITE_PROTECTED			0x27

#define SK_BLNK_CHK		8		// Blank Check
#define SK_VENDOR		9		// Vendor Specific
#define SK_CPY_ABORT	10		// Copy Aborted
#define SK_CMD_ABORT	11		// Aborted Command
#define SK_EQUAL		12		// Equal
#define SK_VOL_OFL		13		// Volume Overflow
#define SK_MISCMP		14		// Miscompare
#define SK_RESERVED		15		// Reserved

// SENSE error member
#define SENSE_EVALID	0x80		// Error Valid
#define SENSE_ECODE		0x7f		// Error Code Mask

// SENSE sense member
#define SENSE_FILEMARK	0x80		// Filemark
#define SENSE_EOM		0x40		// End-of-Medium (end/beginning of partition)
#define SENSE_ILI		0x20		// Incorect Length Indicator

// SENSE sks member
#define SENSE_BP		0x07		// Bit Pointer
#define SENSE_BPV		0x08		// Bit Pointer Valid
#define SENSE_CD		0x40		// Command Data

#define SENSE_SKSV					0x80

#define SENSE_ERROR					0x70
#define SENSE_ERROR_TYPE_DEFERRED	0x01
#define SENSE_DATA_FMT_DESCRIPTOR	0x02

// Fixed Format Sense Data Structure
// Note: The field "error" has the following format:
//        bit    7	- Address valid bit
//		  bits 6-4  - Error class
//		  bits 3-0  - Error code 
//
// Error classes 0-6 are vendor unique and also indicate that the
// sense data is in _nonextended_ format. (i.e. not usually used)
//		struct _scsi_nonextended_sense {
//			uchar_t     sd_err;
//			uint32_t    sd_block_address;
//		};
//
//	An error class of 7 and an error code of 0 (70H) indicate SCSI-1
//	extended sense data format (or SCSI-2 sense data format).
//
//	An error class of 7 and an error code of 1 (71H) indicate SCSI-2
//	deferred errors.
//
//	Error codes 74H to 7EH are reserved and error code 7FH indicates
//	a vendor-specific sense data format.
typedef struct _scsi_sense {
	uchar_t		error;				// Error Code
	uchar_t		segment;			// Segment number
	uchar_t		sense;				// Sense key/flags
	uchar_t		info[4];			// Information (32bit big-endian value)
	uchar_t		asl;				// Additional Sense Length
	uchar_t		csinfo[4];			// Command-Specific Information
	uchar_t		asc;				// Additional Sense Code
	uchar_t		ascq;				// Additional Sense Code Qualifier
	uchar_t		fruc;				// Field Replaceable Unit Code
	uchar_t		sks;				// Sense Key Specific
	ushort_t	sks_data;			// Sense Key Specific Data (16bit big-endian)
	ushort_t	asb;				// Additional Sense uchar_ts (Max 256-18)
} SCSI_SENSE;

// Descriptor Format Sense Data Structure
//	error code of 72 current, 73 deferred
//	extended sense data format (or SCSI-2 sense data format).
typedef struct _scsi_sense_descriptor {
	uchar_t		error;				// Error Code
	uchar_t		sense;				// Sense key/flags
	uchar_t		asc;				// Additional Sense Code
	uchar_t		ascq;				// Additional Sense Code Qualifier
	uchar_t		rsvd[3];
	uchar_t		asl;				// Additional Sense Length
} SCSI_SENSE_DESCRIPTOR;

typedef struct _scsi_sense_desriptor_header {
	uchar_t			descriptor_type;
	uchar_t			descriptor_len;
} SCSI_SENSE_DESCRIPTOR_HEADER;

#define SENSE_DTYPE_INFORMATION		0x00
#define SENSE_DTYPE_CSI				0x01	// Command Specific Information
#define SENSE_DTYPE_SKS				0x02	// Sense Key Specific
#define SENSE_DTYPE_FRU				0x03	// Field Replaceable Unit
#define SENSE_DTYPE_STREAM			0x04
#define SENSE_DTYPE_BLOCK			0x05
#define SENSE_DTYPE_OSD_OBJ_IDENT	0x06	// OSD Object Identification
#define SENSE_DTYPE_OSD_INTEGRITY	0x07	// OSD Response Integrity Check Value
#define SENSE_DTYPE_OSD_ATR_IDENT	0x08	// OSD Attribute Identification
#define SENSE_DTYPE_ATA				0x09
typedef struct _ata_status_descriptor {
	uchar_t			descriptor_type;
#define ATA_SD_DLEN					0x0c
	uchar_t			descriptor_len;			/* 0xc */
#define ATA_SD_FLG_EXTEND			0x01
	uchar_t			flags;
	uchar_t			error;
	uchar_t			esector_count;			/* (15:8) */
	uchar_t			sector_count;			/* (7:0) */
	uchar_t			elba_low;				/* (15:8) */
	uchar_t			lba_low;				/* (7:0) */
	uchar_t			elba_mid;				/* (15:8) */
	uchar_t			lba_mid;				/* (7:0) */
	uchar_t			elba_high;				/* (15:8) */
	uchar_t			lba_high;				/* (7:0) */
	uchar_t			device;
	uchar_t			status;
} ATA_STATUS_DESCRIPTOR;


// Vital Product Data header
typedef struct _vpd_header {
	uchar_t		peripheral;			// 4:4 qualifier:type
	uchar_t		page_code;
	ushort_t	page_length;
} VPD_HEADER;

typedef struct _identification_descriptor_header {
#define CS_IDENTIFIER_BINARY		0x01
#define CS_IDENTIFIER_ASCII			0x02
#define CS_IDENTIFIER_UTF8			0x03
	uchar_t		proto_codeset;
#define IDT_VENDOR					0x00
#define IDT_T10						0x01
#define IDT_EUI64					0x02
#define IDT_NAA						0x03
#define IDT_REL_TGT_PORT			0x04
#define IDT_TGT_PORT_GROUP			0x05
#define IDT_LOG_UNIT_GROUP			0x06
#define IDT_MD5_LOG_UNIT			0x07
#define IDT_SCSI_NAME_STR			0x08
	uchar_t		assoc_idtype;
	uchar_t		rsvd2;
	ushort_t	identifier_length;
} ID_HEADER;

typedef struct _identification_descriptor {
#define CS_IDENTIFIER_BINARY		0x01
#define CS_IDENTIFIER_ASCII			0x02
#define CS_IDENTIFIER_UTF8			0x03
	uchar_t		proto_codeset;
#define IDT_VENDOR					0x00
#define IDT_T10						0x01
#define IDT_EUI64					0x02
#define IDT_NAA						0x03
#define IDT_REL_TGT_PORT			0x04
#define IDT_TGT_PORT_GROUP			0x05
#define IDT_LOG_UNIT_GROUP			0x06
#define IDT_MD5_LOG_UNIT			0x07
#define IDT_SCSI_NAME_STR			0x08
	uchar_t		assoc_idtype;
	uchar_t		rsvd2;
	ushort_t	identifier_length;
} IDENTIFICATION_DESCRIPTOR;

typedef struct _block_device_characteristics {
#define BDC_NON_ROTATING			1
	ushort_t	medium_rotation_rate;
	uchar_t		rsvd6;
	uchar_t		nominal_form_factor;
	uchar_t		rsvd8_63[56];
} BLOCK_DEVICE_CHARACTERISTICS;

typedef struct _block_limits {
	uchar_t		rsvd4_5[2];
	ushort_t	optimal_xfer_len_granularity;
	uint32_t	max_xfer_len;
	uint32_t	optimal_xfer_len;
	uint32_t	max_prefetch_len;
	uint32_t	max_unmap_lba_count;
	uint32_t	max_unmap_desc_count;
	uint32_t	optimal_unmap_granularity;
	uint32_t	unmap_granularity_alignment;
	uchar_t		rsvd36_63[28];
} BLOCK_LIMITS;

// Mode Page Codes
#define MP_RW_ERR			0x01	// Read-Write error recovery page
#define MP_CONNECT			0x02	// Disconnect-reconnect page
#define MP_FMT_DEV			0x03	// Format device page
#define MP_GEOMETRY			0x04	// Rigid disk geometry page
#define MP_FLEX_DSK			0x05	// Flexible disk page
#define MP_RESERVED			0x06	// (reserved)
#define MP_VFY_ERR			0x07	// Verify error recovery page
#define MP_CACHING			0x08	// Caching page
#define MP_PERIPHERAL		0x09	// Peripheral device page
#define MP_CONTROL			0x0a	// Control mode page
	#define MSP_CONTROL_PATA	0xf1
#define MP_MEDIUM			0x0b	// Medium types supported page
#define MP_PARTITION		0x0c	// Notch and Partition page
#define MP_CD				0x0d	// CD-ROM Parameters page
#define MP_AUDIO_CONTROL	0x0e	// Audio Control page
#define MP_POWER_CONDITION	0x1a	// Power Condition Page
#define MP_TIMEOUT_PROTECT	0x1d	// Timeout and Protect Page
#define MP_CD_CAP        	0x2a	// CD Capabilities and Mechanical Status
#define MP_SMART			0x2f	// SMART page
#define MP_ALL				0x3f	// Return all pages ("mode sense" only)
#define MP_DATA_COMPRESSION		0x0f
#define MP_DEVICE_CONFIGURATION	0x10
#define MP_MEDIUM_PARTITION		0x11

// Device Specific
#define MP_DS_WP			0x80	// Write Protected
	// Sequential Access
#define MP_DS_SA_BM_MSK		0x30	// Buffered Mode Mask
#define MP_DS_SA_BM_NONE	0x00	// return GOOD status after data xferd to media
#define MP_DS_SA_BM_TGT		0x10    // return GOOD status when data in target buffer

// Mode parameter header (6)
typedef struct _mode_param_header_6 {
	uchar_t		data_len;
	uchar_t		medium_type;
	uchar_t		device_specific;
	uchar_t		block_desc_length;
} MODE_PARM_HEADER6;

// Mode sense parameter header (10)
typedef struct _mode_parm_header_10 {
	uchar_t		data_len[2];
	uchar_t		medium_type;
	uchar_t		device_specific;
	uchar_t		zero1;
	uchar_t		zero2;
	ushort_t	block_desc_length;
} MODE_PARM_HEADER10;

#define MP_BD_DENSITY_NOCHNG	0x7f
#define MP_BD_LENGTH			0x08
// Mode sense block descriptor
typedef struct _mode_block_descriptor {
	uchar_t		density;
	uchar_t		num_blocks[3];
	uchar_t		zero1;
	uchar_t		block_len[3];
} MODE_BLOCK_DESCRIPTOR;

#define MPAGE_PAGE_MSK		0x3f

typedef struct _mode_page_power_condition {
	uchar_t		pc_page;
	uchar_t		page_length;
	uchar_t		rsvd2;
#define MP_PC_IDLE_VALID		0x2
#define MP_PC_STANDBY_VALID		0x1
	uchar_t		byte3;
	uchar_t		idle_timer[4];
	uchar_t		standby_timer[4];
} MPAGE_POWER_CONDITION;

typedef struct _mode_page_timeout_protect {
	uchar_t		pc_page;
	uchar_t		page_length;
	uchar_t		rsvd2;
	uchar_t		rsvd3;
#define MP_TP_SWPP		0x01		// Software Write Protect
#define MP_TP_DISP		0x02		// Logical Unit Unavailable
#define MP_TP_TMOE		0x04		// Timeout Enable
#define MP_TP_G3ENABLE	0x08		// Enable Group 3 Timeout
	uchar_t		byte4;
	uchar_t		rsvd5;
	uchar_t		group1_min_timeout[2];
	uchar_t		group2_min_timeout[2];
	uchar_t		group3_min_timeout[2];
} MPAGE_TIMEOUT_PROTECT;

typedef struct _mode_page_caching {
#define MP_PC_SAVE		0x80
	uchar_t		pc_page;
	uchar_t		page_length;
	uchar_t		flags;
#define MP_CACHE_RCD	0x01		// Read Cache Disable
#define MP_CACHE_MF		0x02		// Multiplication Factor
#define MP_CACHE_WCE	0x04		// Write Cache Enable
	uchar_t		retension_prio;
	uchar_t		disable_prefetch[2];
	uchar_t		min_prefetch[2];
	uchar_t		max_prefetch[2];
	uchar_t		max_prefetch_ceiling[2];
	uchar_t		flags2;
	uchar_t		num_cache_segments;
	uchar_t		cache_segement_size[2];
	uchar_t		rsvd16;
	uchar_t		obsolete[3];
} MPAGE_CACHING;

typedef struct _mode_page_flexible_disk {
	uchar_t		pc_page;
	uchar_t		page_length;
	uchar_t		transfer_rate[2];
	uchar_t		number_heads;
	uchar_t		sectors_track;
	ushort_t	bytes_per_sector;
	ushort_t	number_cylinders;
	uchar_t		write_precomp[2];			// starting cylinder-write precompensation
	uchar_t		write_current[2];			// starting cylinder-reduced write current
	uchar_t		step_rate[2];				// drive step rate
	uchar_t		pulse_width;				// drive step pulse width
	uchar_t		head_settle[2];				// head settle delay
	uchar_t		motor_on_delay;				// motor on delay
	uchar_t		motor_off_delay;			// motor off delay
	uchar_t		rsvd1					:5,
				mo						:1,	// motor on
				ssn						:1,	// start sector number
				trdy					:1;	// true ready
	uchar_t		spc						:4,	// step pulse per cylinder
				rsvd2					:4;
	uchar_t		write_compensation;
	uchar_t		head_load;					// head load delay
	uchar_t		head_unload;				// head unload delay
	uchar_t		pin2					:4,
				pin34					:4;
	uchar_t		pin1					:4,
				pin4					:4;
	uchar_t		medium_rate[2];				// medium rotation rate
	uchar_t		rsvd3[2];
} MPAGE_FLEXIBLE_DISK;

typedef struct _mode_page_device_configuration {
	uchar_t		pc_page;
	uchar_t		page_length;
#define MP_DC_CAP		0x40				// change active partition
#define MP_DC_CAF		0x20				// change active format
	uchar_t		format;
	uchar_t		partition;					// active partition
	uchar_t		write_bfr;					// write buffer full ratio
	uchar_t		read_bfr;					// read buffer full ratio
	uchar_t		write_delay[2];				// write delay time

#define MP_DC_DBR		0x80				// data buffer recovery
#define MP_DC_BIS		0x40				// block identifiers supported
#define MP_DC_RSMK		0x20				// report setmarks
#define MP_DC_AVC		0x10				// automatic velocity control
#define MP_DC_SOCF_MSK	0x0c				// stop on consecutive filemarks mask
#define MP_DC_RBO		0x02				// recover buffer order
#define MP_DC_REW		0x01				// report early warning
	uchar_t		byte8;
	uchar_t		gap_size;					// gap size
#define MP_DC_EOD_MSK	0xe0				// end of data mask
#define MP_DC_EEG		0x10				// enable EOD generation (EEG)
#define MP_DC_SEW		0x08				// synchronize early warning
	uchar_t		byte10;
	uchar_t		ewarning[3];				// buffer size at early warning
	uchar_t		compression;				// select data compression algorithm
	uchar_t		rsvd15;
} MPAGE_DEVICE_CONFIGURATION;

typedef struct _partition_size_descriptor {
	uchar_t		partition_size[2];
} PARTITION_SIZE_DESCRIPTOR;

typedef struct _mode_page_medium_partition {
	uchar_t						pc_page;
	uchar_t						page_length;
	uchar_t						maximum_partitions;
	uchar_t						partitions_defined;
#define MP_MP_FDP		0x80				// fixed data partitions
#define MP_MP_SDP		0x40				// select data partitions
#define MP_MP_IDP		0x20				// initiator defined partitions
#define MP_MP_PSUM_MSK	0x18				// partition size unit of measure
	#define MP_PSUM_BYTES	0x00
	#define MP_PSUM_KBYTES	0x08
	#define MP_PSUM_MBYTES	0x10
	uchar_t						byte4;
	uchar_t						recognition;		// medium format recognition
	uchar_t						rsvd6;
	uchar_t						rsvd7;
	PARTITION_SIZE_DESCRIPTOR	partition_sizes[64];
} MPAGE_MEDIUM_PARTITION;

typedef struct _mode_page_data_compression {
	uchar_t		pc_page;
	uchar_t		page_length;
#define MP_DC_DCE		0x80				// data compression enable
#define MP_DC_DCC		0x40				// data compression capable
	uchar_t		byte2;
#define MP_DC_DDE		0x80				// data decompression enable
	uchar_t		byte3;
#define MP_DC_CMPR_DFLT				0x01
#define MP_DC_CMPR_IBM_ALDC_512		0x03
#define MP_DC_CMPR_IBM_ALDC_1024	0x04
#define MP_DC_CMPR_IBM_ALDC_2048	0x05
#define MP_DC_CMPR_IBM_IDRC			0x10
#define MP_DC_CMPR_DCLZ				0x20
	uint32_t	compression;				// compression algorithm
	uint32_t	decompression;				// decompression algorithm	
	uchar_t		rsvd[4];
} MPAGE_DATA_COMPRESSION;

typedef struct _mode_page_control_pata {
	uchar_t		pc_page;
	uchar_t		subpage;
	uchar_t		page_length[2];
	uchar_t		mwdma_pio;
	uchar_t		udma;
	uchar_t		rsvd6;
	uchar_t		rsvd7;
} MPAGE_CONTROL_PATA;

typedef struct _mode_page_audio_control {
	uchar_t		pc_page;
	uchar_t		page_length;
#define MP_AC_SOTC			0x02		// stop on track crossing
#define MP_AC_IMMED			0x04
	uchar_t		byte2;
	uchar_t		rsvd3;
	uchar_t		rsvd4;
	uchar_t		rsvd5;
	uchar_t		rsvd6;
	uchar_t		rsvd7;
	uchar_t		port0;
	uchar_t		volume0;
	uchar_t		port1;
	uchar_t		volume1;
	uchar_t		port2;
	uchar_t		volume2;
	uchar_t		port3;
	uchar_t		volume3;
} MPAGE_AUDIO_CONTROL;

typedef struct _mode_page_cd_cap {
	uchar_t		pc_page;
	uchar_t		page_length;
#define MP_CAP_DVD_RAM_RD	0x20
#define MP_CAP_DVD_R_RD		0x10
#define MP_CAP_DVD_ROM_RD	0x08
#define MP_CAP_METHOD2		0x04
#define MP_CAP_CD_RW_RD		0x02
#define MP_CAP_CD_R_RD		0x01
	uchar_t		byte2;
#define MP_CAP_DVD_RAM_WR	0x20
#define MP_CAP_DVD_R_WR		0x10
#define MP_CAP_TEST_WR		0x04
#define MP_CAP_CD_RW_WR		0x02
#define MP_CAP_CD_R_WR		0x01
	uchar_t		byte3;
#define MP_CAP_BUF				0x80
#define MP_CAP_MULTISESSION		0x40
#define MP_CAP_MODE2_FORM2		0x20
#define MP_CAP_MODE2_FORM1		0x10
#define MP_CAP_DIGITAL_PORT0	0x08
#define MP_CAP_DIGITAL_PORT1	0x04
#define MP_CAP_COMPOSITE		0x02
#define MP_CAP_AUDIO_PLAY		0x01
	uchar_t		byte4;
#define MP_CAP_READ_BAR_CODE	0x80
#define MP_CAP_UPC				0x40
#define MP_CAP_ISRC				0x20
#define MP_CAP_C2_POINTERS		0x10
#define MP_CAP_RW_CORRECTED		0x08
#define MP_CAP_RW_SUPPORT		0x04
#define MP_CAP_CDDA_ACCURATE	0x02
#define MP_CAP_CDDA_SUPPORTED	0x01
	uchar_t		byte5;
#define MP_CAP_MECH_CADDY		0x00
#define MP_CAP_MECH_TRAY		0x20
#define MP_CAP_MECH_POPUP		0x40
#define MP_CAP_MECH_CHNG		0x80
#define MP_CAP_MECH_CHNG_MAG	0xa0
#define MP_CAP_EJECT			0x08
#define MP_CAP_PREVENT_JUMPER	0x04
#define MP_CAP_LOCK_STATE		0x02
#define MP_CAP_LOCK				0x01
	uchar_t		byte6;
#define MP_CAP_RW_LEADIN		0x20
#define MP_CAP_SIDE_CHNG_CAP	0x10
#define MP_CAP_SLOT_SELECTION	0x08
#define MP_CAP_CHNG_DISC		0x04
#define MP_CAP_CHNL_MUTES		0x02
#define MP_CAP_VOLUME_LEVELS	0x01
	uchar_t		byte7;
	uchar_t		obsolete_maximum_read_speed[2];
	uchar_t		volume_levels[2];
	uchar_t		buffer_size[2];
	uchar_t		obsolete_current_read_speed[2];
	uchar_t		rsvd16;
#define MP_CAP_WL_32			0x00
#define MP_CAP_WL_16			0x10
#define MP_CAP_WL_24			0x20
#define MP_CAP_WL_24_I2S		0x30
#define MP_CAP_LSBF				0x08
#define MP_CAP_RCK				0x04
#define MP_CAP_BCKF				0x02
	uchar_t		byte17;
	uchar_t		obsolete_maximum_write_speed[2];
	uchar_t		obsolete_current_write_speed[2];
	uchar_t		copy_management[2];
	uchar_t		rsvd24[3];
	uchar_t		rotation_control;
	uchar_t		current_write_speed[2];
	uchar_t		num_write_descriptors[2];
} MPAGE_CD_CAP;

typedef struct _mode_page_cd {
	uchar_t		pc_page;
	uchar_t		page_length;
	uchar_t		reserved1;
	uchar_t		multiplier;				// Inactivity Time Multiplier
	uchar_t		msf_s[2];				// Number of MSF - S Units per MSF - M Unit
	uchar_t		msf_f[2];				// Number of MSF - F Units per MSF - S Unit
} MPAGE_CD;

typedef struct _mode_page_format_device {
	uchar_t		pc_page;
	uchar_t		page_length;
	ushort_t	tracks;						// tracks per zone
	ushort_t	sectors;					// sectors per zone
	ushort_t	alt_tracks;					// alternate tracks per zone
	ushort_t	alt_sectors;				// alternate sectors per zone
	ushort_t	sectors_per_track;
	ushort_t	bytes_per_sector;
	ushort_t	interleave;
	ushort_t	track_skew_factor;
	ushort_t	cylinder_skew_factor;
	uchar_t		flags;						// surf, rmb, hsec, ssec
	uchar_t		rsvd2[3];
} MPAGE_FORMAT_DEVICE;

// Log Sense Page
#define LP_TAPE_CAPACITY		0x31

typedef struct _log_parm_header {
	uchar_t		parameter_code[2];
#define LP_PC_DU		0x80		// disable update
#define LP_PC_DS		0x40		// disable save
#define LP_PC_TDS		0x20		// target disable save
#define LP_PC_ETC		0x10		// enable threshold comparison
#define LP_PC_TMC_MSK	0xc0
	#define LP_PC_TMC_GREATER	0xc0
	#define LP_PC_TMC_NOT_EQUAL	0x80
	#define LP_PC_TMC_EQUAL		0x40
	#define LP_PC_TMC_EVERY		0x00
#define LP_PC_LP		0x01		// list paramter
	uchar_t		parameter_control;
	uchar_t		parameter_length;
} LOG_PARM_HEADER;

typedef struct _lpage_tape_capacity {
	uchar_t		pc_page;
	uchar_t		rsvd;
	uchar_t		page_length[2];
	LOG_PARM_HEADER	parm1;
	uchar_t		remaining_capactity_part0;
	LOG_PARM_HEADER	parm2;
	uchar_t		remaining_capactity_part1;
	LOG_PARM_HEADER	parm3;
	uchar_t		maximum_capactity_part0;
	LOG_PARM_HEADER	parm4;
	uchar_t		maximum_capactity_part1;
} LPAGE_TAPE_CAPACITY;

typedef struct _read_capacity {
	uint32_t	lba;
	uint32_t	blk_size;
} READ_CAPACITY;

typedef struct _read_capacity16 {
	uint64_t	lba;
	uint32_t	blk_size;
#define RD_CAP_PROT_EN		0x01
	uint8_t		ptype_prot;
#define RD_CAP_PI_EXP( _e )		( (_e) >> 4 )
#define RD_CAP_LB_PB_EXP( _e )	( (_e) & 0xf )
	uint8_t		exponents;			// pi and logical blocks per physical block exponents
#define RD_CAP_TPE	0x80			// Thin Provisioning enabled (aka TRIM)
#define RD_CAP_TPRZ	0x40			// Thin Provisioning Read Zero
	uint8_t		tp_lalba[2];		// TP flags and Lowest Aligned LBA
	uchar_t		rsvd16_31[16];
} READ_CAPACITY16;

typedef struct _read_header_data {
	uchar_t		data_mode;
	uchar_t		rsvd[3];
	ushort_t	addr;
} READ_HEADER_DATA;

typedef struct _disc_information_block {
	ushort_t	disc_information_len;
#define DIB_DS_ERASABLE				0x10
#define DIB_DS_DISC_STATUS( _status )		( ( _status ) & 0x3 )
#define DIB_DS_EMPTY_DISC			0x00
#define DIB_DS_INCOMPLETE_DISC		0x01
#define DIB_DS_FINALIZED_DISC		0x02
#define DIB_DS_OTHERS_DISC			0x03

#define DIB_DS_SESSION_STATE( _state )		( ( ( _state ) >> 2 ) & 0x3 )
#define DIB_DS_EMPTY_SESSION		0x00
#define DIB_DS_INCOMPLETE_SESSION	0x01
#define DIB_DS_DAMAGED_SESSION		0x02
#define DIB_DS_COMPLETE_SESSION		0x03
	uchar_t		disc_status;
	uchar_t		ftrk_disc;
	uchar_t		num_sessions_lsb;
	uchar_t		ftrk_session_lsb;
	uchar_t		ltrk_session_lsb;
	uchar_t		flags_bfg;
#define DIB_DID_V		0x80
#define DIB_DBC_V		0x40
#define DIB_URU			0x20
#define DIB_DIRTY		0x04
	uchar_t		disc_type;
	uchar_t		num_sessions_msb;
	uchar_t		ftrk_session_msb;
	uchar_t		ltrk_session_msb;
	uint32_t	disc_identification;
	uint32_t	session_lead_in;
	uint32_t	session_lead_out;
	uint32_t	disc_bar_code;
	uchar_t		rsvd32;
	uchar_t		num_opc_tables;
/*	uchar_t		opc_table_entries[]; */
} DISC_INFORMATION_BLOCK;

typedef struct _read_track_information {
	ushort_t	track_information_len;
    uchar_t     track_number_lsb;
	uchar_t     session_number_lsb;
	uchar_t     rsvd4;
	uchar_t     track_mode;
	uchar_t     data_mode;
#define RTI_VALID_LRA	0x02
#define RTI_VALID_NWA	0x01
	uchar_t     valid;
	uchar_t     track_start_address[4];
	uchar_t     next_writable_address[4];
	uchar_t     free_blocks[4];
	uchar_t     fixed_packet_size[4];
	uchar_t     track_size[4];
	uchar_t     last_recorded_address[4];
    uchar_t     track_number_msb;
	uchar_t     session_number_msb;
	uchar_t     rsvd34_35;
} READ_TRACK_INFORMATION;

typedef struct _dvd_struc_hdr {
	ushort_t	struc_data_len;
	uint8_t		rsvd1;
	uint8_t		rsvd2;
} DVD_STRUC_HDR;

typedef struct _dvd_physical_information {
	uchar_t		bt_pv;					// book type, part version
	uchar_t		ds_mr;					// disc size, maximum rate
	uchar_t		nl_tp_lt;				// num layers, track path, layer type
	uchar_t		ld_td;					// layer density, track density
	uchar_t		rsvd4;
	uchar_t		sphys_sect_data[3];
	uchar_t		rsvd8;
	uchar_t		ephys_sect_data[3];
	uchar_t		rsvd12;
	uchar_t		ephys_sect_layer[3];
	uchar_t		bca;
	uchar_t		media_specific[2030];
} DVD_PHYSICAL_INFORMATION;

typedef struct _dvd_copyright_information {
	uchar_t		copyright_type;
	uchar_t		region_information;
	uchar_t		rsvd[2];
} DVD_COPYRIGHT_INFORMATION;

typedef struct capacity_list_header {
	uchar_t		rsvd[3];
	uchar_t		capacity_list_length;
} CAPACITY_LIST_HEADER;

#define FEATURE_RANDOM_WRITABLE	0x20
#define FCD_UNFORMATTED			0x01
#define FCD_FORMATTED			0x02
#define FCD_NO_MEDIA			0x03
#define FCD_MSK					0x03
typedef struct formatted_capacity_descriptor {
	uint32_t	num_blocks;
	uchar_t		descriptor_type;
	uchar_t		block_length[3];
} FORMATED_CAPACITY_DESCRIPTOR;

#define RWD_RT_SFN				0x02
#define RWD_CURRENT				0x01
#define RWD_PERSISTENT			0x02
#define RWD_VERSION_MASK		0x3c
typedef struct random_writable_descriptor {
	ushort_t			feature_code;
	uchar_t				flags;
	uchar_t				additional_length;
	uchar_t				lba[4];
	uint32_t			blk_size;
	ushort_t			blocking;
	uchar_t				rsvd14;
	uchar_t				rsvd15_pp;
} RANDOM_WRITABLE_DESCRIPTOR;

typedef struct feature_header {
	uchar_t			data_length[4];
	uchar_t			rsvd4;
	uchar_t			rsvd5;
	uchar_t			current_profile[2];
} FEATURE_HEADER;

// ATAPI changer
#define MSH_CHANGER_FAULT			(0x1 << 7)
#define MSH_CHANGER_READY			(0x0 << 5)
#define MSH_CHANGER_LOADING			(0x1 << 5)
#define MSH_CHANGER_UNLOADING		(0x2 << 5)
#define MSH_CHANGER_INITIALIZING	(0x3 << 5)

#define MSH_MECHANISM_DOOR_OPEN		(0x1 << 4)
#define MSH_MECHANISM_IDLE			(0x0 << 5)
#define MSH_MECHANISM_AUDIO			(0x1 << 5)
#define MSH_MECHANISM_AUDIO_SCAN	(0x2 << 5)
#define MSH_MECHANISM_HOST			(0x3 << 5)
#define MSH_MECHANISM_UNKNOWN		(0x7 << 5)
typedef struct _mechanism_status_header {
	uchar_t		changer_state_slot;
	uchar_t		mech_state;
	uchar_t		current_lba[3];
	uchar_t		num_slots;
	uchar_t		slot_table_len[2];
} MECHANISM_STATUS_HEADER;

#define STR_DISC_PRESENT  0x80
#define STR_DISC_CHANGED  0x01
typedef struct _slot_table_response {
	uchar_t		flags;
	uchar_t		rsvd[3];
} SLOT_TABLE_RESPONSE;

typedef struct _event_header {
	uchar_t		event_descriptor_len[2];
#define EH_NEA				0x80
#define NC_OPERATIONAL_CHANGE	0x01
#define NC_POWER_MANAGEMENT		0x02
#define NC_EXTERNAL_REQUEST		0x03
#define NC_MEDIA				0x04
#define NC_MULTI_INITIATOR		0x05
#define NC_DEVICE_BUSY			0x06
	uchar_t		notification_class;
	uchar_t		supported_event_classes;
} EVENT_HEADER;

typedef struct _event_operational_change_descriptor {
#define EC_NO_CHANGE			0
#define EC_CHANGE				2
	uchar_t		event_code;
	uchar_t		operational_status;	
#define OP_CHANGE_NONE			0
#define OP_CHANGE_FEATURE		1
	uchar_t		operational_change[2];
} EVENT_OCD_DESCRIPTOR;

typedef struct _event_power_management_descriptor {
#define EC_NO_CHANGE			0
#define EC_CHANGE_SUCCESS		1
#define EC_CHANGE_FAIL			2
	uchar_t		event_code;
#define PS_ACTIVE				1
#define PS_IDLE					2
#define PS_STANDBY				3
#define PS_SLEEP				4
	uchar_t		power_status;	
	uchar_t		rsvd2;
	uchar_t		rsvd3;
} EVENT_PM_DESCRIPTOR;

typedef struct _event_external_descriptor {
#define EC_NO_CHANGE			0
#define EC_KEY_UP				1
#define EC_KEY_DOWN				2
#define EC_ERN					3		// External Request Notification
	uchar_t		event_code;
	uchar_t		external_request_status;	
#define ER_NONE					0
#define ER_OVERRUN				1
#define ER_PLAY					0x101
#define ER_REWIND				0x102
#define ER_FORWARD				0x103
#define ER_PAUSE				0x104
#define ER_STOP					0x106
#define ER_ASCII_BUTTON_MSK		0x2ff
	uchar_t		external_request[2];
} EVENT_EXTERNAL_DESCRIPTOR;

typedef struct _event_media_descriptor {
#define EC_NO_CHNAGE			0
#define EC_EJECT_REQUEST		1
#define EC_NEW_MEDIA			2
#define EC_MEDIA_REMOVAL		3
#define EC_MEDIA_CHANGED		4
#define EC_BG_FORMAT_COMPLETE	5
#define EC_BG_FORMAT_RESTARTED	6
	uchar_t		event_code;
#define MS_MEDIA_PRESENT		0x02
#define MS_DOOR_OPEN			0x01
	uchar_t		media_status;
	uchar_t		start_slot;
	uchar_t		end_slot;
} EVENT_MEDIA_DESCRIPTOR;

typedef struct _read_position_data_shrt {
#define RPD_BOP		0x80		// begining of partition
#define RPD_EOP		0x40		// end of partition
#define RPD_BPU		0x04		// block position unknown
	uchar_t		flags;
	uchar_t		partition_number;
	uchar_t		rsvd2;
	uchar_t		rsvd3;
	uint32_t	first_block_location;
	uint32_t	last_block_location;
	uchar_t		rsvd12;
	uchar_t		number_blocks_buffer[3];
	uint32_t	number_bytes_buffer;
} READ_POSITION_DATA_SHRT;

typedef struct _read_position_data_lng {
#define RPD_BOP		0x80		// begining of partition
#define RPD_EOP		0x40		// end of partition
#define RPD_BPU		0x04		// block position unknown
	uchar_t		flags;
	uchar_t		rsvd1;
	uchar_t		rsvd2;
	uchar_t		rsvd3;
	uchar_t		partition_number[4];
	uchar_t		block_number[8];
	uchar_t		file_number[8];
	uchar_t		set_number[8];
} READ_POSITION_DATA_LNG;

typedef struct _density_support_header {
	ushort_t	density_support_length;
	uchar_t		rsvd1;
	uchar_t		rsvd2;
} DENSITY_SUPPORT_HEADER;

typedef struct _density_support_descriptor {
	uchar_t		primary_density_code;
	uchar_t		secondary_density_code;
#define DSD_CAP_WRTOK		0x80
#define DSD_CAP_DUP			0x40
#define DSD_CAP_DEFLT		0x20
	uchar_t		capabilities;
	uchar_t		rsvd3;
	uchar_t		rsvd4;
	uchar_t		bits_per_mm[3];
	uchar_t		media_width[2];
	uchar_t		tracks[2];
	uchar_t		capacity[4];
	uchar_t		assigning_organization[8];
	uchar_t		density_name[8];
	uchar_t		description[20];
} DENSITY_SUPPORT_DESCRIPTOR;

typedef struct _read_block_limits_data {
	uchar_t		granularity;
	uchar_t		maximum_block_length[3];
	uchar_t		minimum_block_length[2];
} READ_BLOCK_LIMITS_DATA;

typedef struct _unmap_parm_header {
	uchar_t		unmap_data_length[2];
	uchar_t		unmap_block_desc_length[2];
	uchar_t		rsvd4_7[4];
} UNMAP_PARM_HEADER;

// unmap block descriptor
typedef struct _unmap_block_descriptor {
	uchar_t		unmap_lba[8];
	uchar_t		unmap_nlba[4];
	uchar_t		rsvd12_15[4];
} UNMAP_BLOCK_DESCRIPTOR;

#include <_packpop.h>

#endif
