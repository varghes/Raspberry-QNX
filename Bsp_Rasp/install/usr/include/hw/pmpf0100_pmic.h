/*
 * $QNXLicenseC:
 * Copyright 2012, QNX Software Systems.
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
 *  Freescale PMPF0100 Power Management Interface Controller (PMIC)
 */

#ifndef	_PMPF0100_PMIC_H_INCLUDED
#define	_PMPF0100_PMIC_H_INCLUDED

//--------------------------------------
// Structures used in PMIC devctl comms
//--------------------------------------
#define PMIC_EVENT_CALLBACK_FUNC(a)    int (*a)(uint32_t, void *)


// Read / Write a single register
typedef struct
{
	uint32_t reg_number;
	uint32_t reg_data;
} pmic_send_recv_t;

// Read a sequential block of registers
#define PMIC_MAX_SEND_RECV_BLOCK_SIZE      16
typedef struct
{
	uint32_t start_reg_number;
	uint32_t num_registers;
	uint32_t reg_data[PMIC_MAX_SEND_RECV_BLOCK_SIZE];
} pmic_send_recv_block_t;

// Get driver version
typedef struct
{
	unsigned int version_major;
	unsigned int version_minor;
} pmic_version_info_t;

// Manage pulses sent to clients by the driver
typedef struct
{
	uint8_t  eventMask;                    // (in) The 8-bit PMIC event bitmask to check
	uint8_t  eventReg;                     // (in) The PMIC event register to check (0 to 4)
	int      ownerId;                      // (in) The pulse recipient process ID
	int      channelId;                    // (in) The pulse recipient channel ID
	int      priority;                     // (in) The pulse priority
	int      code;                         // (in) The 8-bit pulse code to send back
	uint32_t guid;                         // (out) A GUID identifying the pulse registration
										   //       Used in cancelling the pulse
} pmic_pulse_info_t;

#define PMPF0100_DEVICE_NAME               "/dev/pmic"
#define PMPF0100_DRIVER_VERSION_MAJOR       1
#define PMPF0100_DRIVER_VERSION_MINOR       0

// IOCTL CALLS FOR PMIC DEVICE
#include <devctl.h>
#define _DCMD_PMPF0100                      _DCMD_MISC

#define DCMD_PMPF0100_DRIVER_INFO           __DIOF  (_DCMD_PMPF0100,  1, pmic_version_info_t)
#define DCMD_PMPF0100_LOCK                  __DIOT  (_DCMD_PMPF0100,  2, uint32_t)
#define DCMD_PMPF0100_UNLOCK                __DIOT  (_DCMD_PMPF0100,  3, uint32_t)
#define DCMD_PMPF0100_REGISTER_READ         __DIOTF (_DCMD_PMPF0100,  4, pmic_send_recv_t)
#define DCMD_PMPF0100_REGISTER_WRITE        __DIOT  (_DCMD_PMPF0100,  6, pmic_send_recv_t)
#define DCMD_PMPF0100_STATUS_READ           __DIOF  (_DCMD_PMPF0100,  8, uint32_t)
#define DCMD_PMPF0100_EVENT_PULSE_REGISTER  __DIOTF (_DCMD_PMPF0100,  9, pmic_pulse_info_t)
#define DCMD_PMPF0100_EVENT_PULSE_CANCEL    __DIOT  (_DCMD_PMPF0100, 10, uint32_t)
#define DCMD_PMPF0100_EVENT_POLL            __DIOT  (_DCMD_PMPF0100, 11, uint32_t)
#define DCMD_PMPF0100_ATOD_RESOLUTION       __DIOT  (_DCMD_PMPF0100, 12, uint32_t)

// PMPF0100 on recent iMX6X boards
#define PMPF0100_DEFAULT_2WIRE_ADDRESS        0x08
#define PMPF0100_MAX_2WIRE_BUS_SPEED        400000
#define PMPF0100_DEFAULT_2WIRE_BUS               3


//---------------------------------------------
// System control register addresses
//---------------------------------------------
#define PMPF0100_NUM_IRQ_REGS               4

#define PMPF0100_DEVICE_ID         (0)
#define PMPF0100_SILICON_REV       (3)
#define PMPF0100_FAB_ID            (4)
#define PMPF0100_INT_STATUS0       (5)
#define PMPF0100_INT_MASK0         (6)
#define PMPF0100_INT_MASK0_DEF     0x3f

#define PMPF0100_INT_SENSE0        (7)
#define PMPF0100_INT_STATUS1       (8)
#define PMPF0100_INT_MASK1         (9)
#define PMPF0100_INT_MASK1_DEF     0x7f

#define PMPF0100_INT_SENSE1        (0xa)
#define PMPF0100_INT_STATUS3       (0xe)
#define PMPF0100_INT_MASK3         (0xf)
#define PMPF0100_INT_MASK3_DEF     0x81

#define PMPF0100_INT_SENSE3        (0x10)
#define PMPF0100_INT_STATUS4       (0x11)
#define PMPF0100_INT_MASK4         (0x12)
#define PMPF0100_INT_MASK4_DEF     0x3f

#define PMPF0100_INT_SENSE4        (0x13)

#define PMPF0100_COINCELL_CTRL     (0x1a)
#define PMPF0100_POWER_CTRL        (0x1b)
#define PMPF0100_MEMORY_A          (0x1c)
#define PMPF0100_MEMORY_B          (0x1d)
#define PMPF0100_MEMORY_C          (0x1e)
#define PMPF0100_MEMORY_D          (0x1f)
#endif   // _PMPF0100_PMIC_H_INCLUDED
