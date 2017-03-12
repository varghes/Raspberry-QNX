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
 * hw/acpi.h:	ACPI manifests
 *

 *
 */

#ifndef _HW_ACPI_H_INCLUDED
#define _HW_ACPI_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

/* Argh, some of the ACPI structures aren't naturally aligned :-( */
#include _NTO_HDR_(_pack1.h)

typedef struct {
	_Uint8t			Address_Space_ID;
	_Uint8t			Register_Bit_Width;
	_Uint8t			Register_Bit_Offset;
	_Uint8t			reserved;
	union {
		_Paddr64t	sm;
		_Paddr64t	sio;
		struct {
			_Uint16t	offset;
			_Uint16t	pci_function_number;
			_Uint16t	pci_device_number;
			_Uint16t	reserved;
		}			pcs;
	}				Address;
} acpi_gas;

#define ACPI_GAS_ASID_SYSTEM_MEMORY				0
#define ACPI_GAS_ASID_SYSTEM_IO					1
#define ACPI_GAS_ASID_PCI_CONFIGURATION_SPACE	2



typedef struct {
	_Uint8t		Signature[8];
	_Uint8t		Checksum;
	_Uint8t		OEMID[6];
	_Uint8t		Revision;
	_Paddr32t	RsdtAddress;
} acpi_rsdp1_0;

typedef struct {
	acpi_rsdp1_0	v1;
	_Uint32t		Length;
	_Paddr64t		XsdtAddress;
	_Uint8t			Extended_Checksum;
	_Uint8t			reserved[3];
} acpi_rsdp2_0;

typedef acpi_rsdp2_0	acpi_rsdp;

#define ACPI_RSDP_SIGNATURE		"RSD PTR "

#define ACPI_RSDP_REVISION_1_0	0
#define ACPI_RSDP_REVISION_2_0	2


typedef struct {
	_Uint32t		Signature;
	_Uint32t		Length;
	_Uint8t			Revision;
	_Uint8t			Checksum;
	_Uint8t			OEMID[6];
	_Uint8t			OEM_Table_ID[8];
	_Uint32t		OEM_Revision;
	_Uint32t		Creator_ID;
	_Uint32t		Creator_Revison;
} acpi_description_header;

#define ACPI_SIGNATURE(c1, c2, c3, c4)	\
		(((c1)<<0)|((c2)<<8)|((c3)<<16)|((c4)<<24))


#define ACPI_RSDT_SIGNATURE	ACPI_SIGNATURE('R','S','D','T')
typedef struct {
	acpi_description_header		hdr;
	_Paddr32t					Entry[1]; /* Variably sized */
} acpi_rsdt;


#define ACPI_XSDT_SIGNATURE	ACPI_SIGNATURE('X','S','D','T')
typedef struct {
	acpi_description_header		hdr;
	_Paddr64t					Entry[1]; /* Variably sized */
} acpi_xsdt;


#define ACPI_FADT_SIGNATURE	ACPI_SIGNATURE('F','A','C','P')
typedef struct {
	acpi_description_header		hdr;
	_Paddr32t					FIRMWARE_CTRL;
	_Paddr32t					DSDT;
	_Uint8t						reserved;
	_Uint8t						Perferred_PM_Profile;
	_Uint16t					SCI_INT;
	_Paddr32t					SMI_CMD;
	_Uint8t						ACPI_ENABLE;
	_Uint8t						ACPI_DISABLE;
	_Uint8t						S4BIOS_REQ;
	_Uint8t						PSTATE_CNT;
	_Paddr32t					PM1a_EVT_BLK;
	_Paddr32t					PM1b_EVT_BLK;
	_Paddr32t					PM1a_CNT_BLK;
	_Paddr32t					PM1b_CNT_BLK;
	_Paddr32t					PM2_CNT_BLK;
	_Paddr32t					PM_TMR_BLK;
	_Paddr32t					GPE0_BLK;
	_Paddr32t					GPE1_BLK;
	_Uint8t						PM1_EVT_LEN;
	_Uint8t						PM1_CNT_LEN;
	_Uint8t						PM2_CNT_LEN;
	_Uint8t						PM_TMR_LEN;
	_Uint8t						GPE0_BLK_LEN;
	_Uint8t						GPE1_BLK_LEN;
	_Uint8t						GPE1_BASE;
	_Uint8t						CST_CNT;
	_Uint16t					P_LVL2_LAT;
	_Uint16t					P_LVL3_LAT;
	_Uint16t					FLUSH_SIZE;
	_Uint16t					FLUSH_STRIDE;
	_Uint8t						DUTY_OFFSET;
	_Uint8t						DUTY_WIDTH;
	_Uint8t						DAY_ALRM;
	_Uint8t						MON_ALRM;
	_Uint8t						CENTURY;
	_Uint16t					IAPC_BOOT_ARCH;
	_Uint8t						reserved1;
	_Uint32t					Flags;
	acpi_gas					RESET_REG;
	_Uint8t						RESET_VALUE;
	_Uint8t						reserved2[3];
	_Paddr64t					X_FIRMWARE_CTRL;
	_Paddr64t					X_DSDT;
	acpi_gas					X_PM1a_EVT_BLK;
	acpi_gas					X_PM1b_EVT_BLK;
	acpi_gas					X_PM1a_CNT_BLK;
	acpi_gas					X_PM1b_CNT_BLK;
	acpi_gas					X_PM2_CNT_BLK;
	acpi_gas					X_PM_TMR_BLK;
	acpi_gas					X_GPE0_BLK;
	acpi_gas					X_GPE1_BLK;
} acpi_fadt;

#define ACPI_FADT_PPMP_UNSPECIFIED		0
#define ACPI_FADT_PPMP_DESKTOP			1
#define ACPI_FADT_PPMP_MOBILE			2
#define ACPI_FADT_PPMP_WORKSTATION		3
#define ACPI_FADT_PPMP_ENTERPRISE_SERVER 4
#define ACPI_FADT_PPMP_SOHO_SERVER		5
#define ACPI_FADT_PPMP_APPLIANCE_PC		6

#define ACPI_FADT_FLAGS_WBINVD			_ONEBIT32L(0)
#define ACPI_FADT_FLAGS_WBINVD_FLUSH	_ONEBIT32L(1)
#define ACPI_FADT_FLAGS_PROC_C1			_ONEBIT32L(2)
#define ACPI_FADT_FLAGS_P_LVL2_UP		_ONEBIT32L(3)
#define ACPI_FADT_FLAGS_PWR_BUTTON		_ONEBIT32L(4)
#define ACPI_FADT_FLAGS_SLP_BUTTON		_ONEBIT32L(5)
#define ACPI_FADT_FLAGS_FIX_RTC			_ONEBIT32L(6)
#define ACPI_FADT_FLAGS_RTC_S4			_ONEBIT32L(7)
#define ACPI_FADT_FLAGS_TMR_VAL_EXT		_ONEBIT32L(8)
#define ACPI_FADT_FLAGS_DCK_CAP			_ONEBIT32L(9)
#define ACPI_FADT_FLAGS_RESET_REG_SUP	_ONEBIT32L(10)
#define ACPI_FADT_FLAGS_SEALED_CASE		_ONEBIT32L(11)
#define ACPI_FADT_FLAGS_HEADLESS		_ONEBIT32L(12)
#define ACPI_FADT_FLAGS_CPU_SW_SLP		_ONEBIT32L(13)

#define ACPI_FADT_IBA_LEGACY_DEVICES	_ONEBIT32L(0)
#define ACPI_FADT_IBA_8042				_ONEBIT32L(1)


#define ACPI_FACS_SIGNATURE	ACPI_SIGNATURE('F','A','C','S')
typedef struct {
	_Uint32t					Signature;
	_Uint32t					Length;
	_Uint32t					Hardware_Signature;
	_Paddr32t					Firmware_Waking_Vector;
	_Uint32t					Global_Lock;
	_Uint32t					Flags;
	_Paddr64t					X_Firmware_Waking_Vector;
	_Uint8t						Version;
	_Uint8t						reserved[31];
} acpi_facs;

#define ACPI_FACS_S4BIOS_F		_ONEBIT32L(0)

#define ACPI_FACS_GL_PENDING	_ONEBIT32B(0)
#define ACPI_FACS_GL_OWNED		_ONEBIT32B(1)


#define ACPI_DSDT_SIGNATURE	ACPI_SIGNATURE('D','S','D','T')
typedef struct {
	acpi_description_header		hdr;
	_Uint8t						Definition_Block[1]; /* Variably Sized */
} acpi_dsdt;


#define ACPI_SSDT_SIGNATURE	ACPI_SIGNATURE('S','S','D','T')
typedef struct {
	acpi_description_header		hdr;
	_Uint8t						Definition_Block[1]; /* Variably Sized */
} acpi_ssdt;


/* Only used in ACPI 1.0 */
#define ACPI_PSDT_SIGNATURE	ACPI_SIGNATURE('P','S','D','T')
typedef acpi_ssdt	acpi_psdt;


#define ACPI_MADT_TYPE_PROCESSOR_LOCAL_APIC			0
#define ACPI_MADT_TYPE_IO_APIC						1
#define ACPI_MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE 	2
#define ACPI_MADT_TYPE_NONMASKABLE_INTERRUPT_SOURCE	3
#define ACPI_MADT_TYPE_LOCAL_APIC_NMI				4
#define ACPI_MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE	5
#define ACPI_MADT_TYPE_IO_SAPIC						6
#define ACPI_MADT_TYPE_LOCAL_SAPIC					7
#define ACPI_MADT_TYPE_PLATFORM_INTERRUPT_SOURCES	8

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
	_Uint8t			ACPI_Processor_ID;
	_Uint8t			APIC_ID;
	_Uint32t		Flags;
} acpi_madt_processor_local_apic;

#define ACPI_MADT_PLA_FLAGS_ENABLED		_ONEBIT32L(0)

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
	_Uint8t			IO_APIC_ID;
	_Uint8t			reserved;
	_Paddr32t		IO_APIC_Address;
	_Uint32t		Global_System_Interrupt_Base;
} acpi_madt_io_apic;

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
	_Uint8t			Bus;
	_Uint8t			Source;
	_Uint32t		Global_System_Interrupt;
	_Uint16t		Flags;
} acpi_madt_interrupt_source_overrides;

#define ACPI_MADT_ISO_FLAGS_POLARITY_MASK		_BITFIELD16L(0,0x3)
#define ACPI_MADT_ISO_FLAGS_POLARITY_SHIFT		0
#define ACPI_MADT_ISO_FLAGS_POLARITY_CONFORMS	_BITFIELD16L(0,0x0)
#define ACPI_MADT_ISO_FLAGS_POLARITY_HIGH		_BITFIELD16L(0,0x1)
#define ACPI_MADT_ISO_FLAGS_POLARITY_LOW		_BITFIELD16L(0,0x3)
#define ACPI_MADT_ISO_FLAGS_TRIGGER_MASK		_BITFIELD16L(2,0x3)
#define ACPI_MADT_ISO_FLAGS_TRIGGER_SHIFT		2
#define ACPI_MADT_ISO_FLAGS_TRIGGER_CONFORMS	_BITFIELD16L(2,0x0)
#define ACPI_MADT_ISO_FLAGS_TRIGGER_EDGE		_BITFIELD16L(2,0x1)
#define ACPI_MADT_ISO_FLAGS_TRIGGER_LEVEL		_BITFIELD16L(0,0x3)

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
	_Uint16t		Flags;
	_Uint32t		Global_System_Interrupt;
} acpi_madt_nmi_interrupt_sources;

#define ACPI_MADT_NIS_FLAGS_POLARITY_MASK		_BITFIELD16L(0,0x3)
#define ACPI_MADT_NIS_FLAGS_POLARITY_SHIFT		0
#define ACPI_MADT_NIS_FLAGS_POLARITY_CONFORMS	_BITFIELD16L(0,0x0)
#define ACPI_MADT_NIS_FLAGS_POLARITY_HIGH		_BITFIELD16L(0,0x1)
#define ACPI_MADT_NIS_FLAGS_POLARITY_LOW		_BITFIELD16L(0,0x3)
#define ACPI_MADT_NIS_FLAGS_TRIGGER_MASK		_BITFIELD16L(2,0x3)
#define ACPI_MADT_NIS_FLAGS_TRIGGER_SHIFT		2
#define ACPI_MADT_NIS_FLAGS_TRIGGER_CONFORMS	_BITFIELD16L(2,0x0)
#define ACPI_MADT_NIS_FLAGS_TRIGGER_EDGE		_BITFIELD16L(2,0x1)
#define ACPI_MADT_NIS_FLAGS_TRIGGER_LEVEL		_BITFIELD16L(0,0x3)

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
	_Uint8t			ACPI_Processor_ID;
	_Uint16t		Flags;
	_Uint8t			Local_APIC_LINT;
} acpi_madt_local_apic_nmi;

#define ACPI_MADT_LAN_FLAGS_POLARITY_MASK		_BITFIELD16L(0,0x3)
#define ACPI_MADT_LAN_FLAGS_POLARITY_SHIFT		0
#define ACPI_MADT_LAN_FLAGS_POLARITY_CONFORMS	_BITFIELD16L(0,0x0)
#define ACPI_MADT_LAN_FLAGS_POLARITY_HIGH		_BITFIELD16L(0,0x1)
#define ACPI_MADT_LAN_FLAGS_POLARITY_LOW		_BITFIELD16L(0,0x3)
#define ACPI_MADT_LAN_FLAGS_TRIGGER_MASK		_BITFIELD16L(2,0x3)
#define ACPI_MADT_LAN_FLAGS_TRIGGER_SHIFT		2
#define ACPI_MADT_LAN_FLAGS_TRIGGER_CONFORMS	_BITFIELD16L(2,0x0)
#define ACPI_MADT_LAN_FLAGS_TRIGGER_EDGE		_BITFIELD16L(2,0x1)
#define ACPI_MADT_LAN_FLAGS_TRIGGER_LEVEL		_BITFIELD16L(0,0x3)

typedef struct {
	_Uint8t			Type;
	_Uint8t			Length;
} acpi_madt_header;

typedef struct {
	acpi_madt_header	hdr;
	_Uint16t			reserved;
	_Paddr64t			Local_APIC_Address;
} acpi_madt_local_apic_address_override;

typedef struct {
	acpi_madt_header	hdr;
	_Uint8t				IO_APIC_ID;
	_Uint32t			Global_System_Interrupt_Base;
	_Paddr64t			IO_SAPIC_Address;
} acpi_madt_io_sapic;

typedef struct {
	acpi_madt_header	hdr;
	_Uint8t				ACPI_Processor_ID;
	_Uint8t				Local_SAPIC_ID;
	_Uint8t				Local_SAPIC_EID;
	_Uint8t				reserved[3];
	_Uint32t			Flags;
} acpi_madt_local_sapic;

#define ACPI_MADT_LS_FLAGS_ENABLED		_ONEBIT32L(0)

typedef struct {
	acpi_madt_header	hdr;
	_Uint16t			Flags;
	_Uint8t				Interrupt_Type;
	_Uint8t				ProcessorID;
	_Uint8t				ProcessorEID;
	_Uint8t				IO_SAPIC_Vector;
	_Uint32t			Global_System_Interrupt;
	_Uint32t			reserved;
} acpi_madt_platform_interrupt_sources;

#define ACPI_MADT_PIS_TYPE_PMI						1
#define ACPI_MADT_PIS_TYPE_INIT						2
#define ACPI_MADT_PIS_TYPE_CORRECTED_PLAFORM_ERROR	3

typedef union {
	acpi_madt_header						hdr;
	acpi_madt_processor_local_apic			processor_local_apic;
	acpi_madt_io_apic						io_apic;
	acpi_madt_interrupt_source_overrides	interrupt_sources_override;
	acpi_madt_nmi_interrupt_sources			nmi_interrupt_sources;
	acpi_madt_local_apic_nmi				local_apic_nmi;
	acpi_madt_local_apic_address_override	local_apic_address_override;
	acpi_madt_io_sapic						io_sapic;
	acpi_madt_local_sapic					local_sapic;
	acpi_madt_platform_interrupt_sources	platform_interrupt_sources;
} acpi_madt_generic;

#define ACPI_MADT_SIGNATURE	ACPI_SIGNATURE('A','P','I','C')
typedef struct {
	acpi_description_header		hdr;
	_Paddr32t					Local_APIC_Address;
	_Uint32t					Flags;
	acpi_madt_generic 			APIC[1]; /* Variably Sized */
} acpi_madt;


#define ACPI_SBST_SIGNATURE	ACPI_SIGNATURE('S','B','S','T')
typedef struct {
	acpi_description_header		hdr;
	_Uint32t					Warning_Energy_Level;
	_Uint32t					Low_Energy_Level;
	_Uint32t					Critical_Energy_Level;
} apci_sbst;


#define ACPI_ECDT_SIGNATURE	ACPI_SIGNATURE('E','C','D','T')
typedef struct {
	acpi_description_header		hdr;
	acpi_gas					EC_CONTROL;
	acpi_gas					EC_DATA;
	_Uint32t					UID;
	_Uint8t						GPE_BIT;
	_Uint8t						EC_ID[1]; /* Variably sized */
} acpi_ecdt;

typedef union {
	acpi_description_header		hdr;
	acpi_rsdt					rsdt;
	acpi_xsdt					xsdt;
	acpi_fadt					fadt;
	acpi_facs					facs;
	acpi_dsdt					dsdt;
	acpi_ssdt					ssdt;
	acpi_psdt					psdt;
	acpi_madt					madt;
	apci_sbst					sbst;
	acpi_ecdt					ecdt;
} acpi_generic;

#include _NTO_HDR_(_packpop.h)

#endif

/* __SRCVERSION("acpi.h $Rev: 657836 $"); */
