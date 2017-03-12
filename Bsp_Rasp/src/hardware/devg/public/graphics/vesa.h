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




#ifdef __X86__

#ifndef _GRAPHICS_VESA_H_INCLUDED
#define _GRAPHICS_VESA_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif
#ifndef _GRAPHICS_VBIOS_H_INCLUDED
#include <graphics/vbios.h>
#endif

#include <_pack1.h>

struct VESATimingDetail {
	uint16_t PixClock;
	uint8_t HALow;
	uint8_t HBLow;
	uint8_t HABHigh;
	uint8_t VALow;
	uint8_t VBLow;
	uint8_t VABHigh;
	uint8_t HSyncOLow;
	uint8_t HSyncWLow;
	uint8_t VSyncOffWidLow;
	uint8_t SyncOffWidHigh;
	uint8_t HSizeLow;
	uint8_t VSizeLow;
	uint8_t SizeHigh;
	uint8_t HBorder;
	uint8_t VBorder;
	uint8_t Flags;
};

struct vesa_edid {
	uint8_t b0;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	uint8_t b4;
	uint8_t b5;
	uint8_t b6;
	uint8_t b7;
	uint16_t Manufacturer;
	uint16_t Product;
	uint32_t SerialNum;
	uint8_t MfgrWeek;
	uint8_t MfgrYear;
	uint8_t EDIDVersion;
	uint8_t EDIDRevision;
	uint8_t VidInDef;
	uint8_t MaxHSize;
	uint8_t MaxVSize;
	uint8_t Gamma;
	uint8_t DPMSSupport;
	uint8_t RGLow;
	uint8_t BWLow;
	uint8_t RedX;
	uint8_t RedY;
	uint8_t GreenX;
	uint8_t GreenY;
	uint8_t BlueX;
	uint8_t BlueY;
	uint8_t WhiteX;
	uint8_t WhiteY;
	uint8_t EstTiming1;
	uint8_t EstTiming2;
	uint8_t MfgrTiming;
	uint16_t StdTiming[8];
	struct VESATimingDetail Detail[4];
	uint8_t ExtensionFlag;
	uint8_t Checksum;
	uint8_t spare[101];
};

#define CIB_FLAG_DSCAN 0x01
#define CIB_FLAG_NO_DSCAN 0
#define CIB_FLAG_INTERLACED 0x02
#define CIB_FLAG_HSYNC_PLUS 0
#define CIB_FLAG_HSYNC_MINUS 0x04
#define CIB_FLAG_VSYNC_PLUS 0
#define CIB_FLAG_VSYNC_MINUS 0x08

typedef struct {
	uint16_t	HorizontalTotal;
	uint16_t	HorizontalSyncStart;
	uint16_t	HorizontalSyncEnd;
	uint16_t	VerticalTotal;
	uint16_t	VerticalSyncStart;
	uint16_t	VerticalSyncEnd;
	uint8_t		Flags;
	uint32_t	PixelClock;
	uint16_t	RefreshRate;
	uint8_t		Reserved[40];
} CRTCInfoBlock_t;

typedef struct {
	uint32_t	Signature;
	uint16_t	EntryPoint;
	uint16_t	Initialize;
	uint16_t	BIOSDataSelector;
	uint16_t	A0000Selector;
	uint16_t	B0000Selector;
	uint16_t	B8000Selector;
	uint16_t	CodeSegSelector;
	uint8_t	InProtectMode;
	uint8_t	Checksum;
} PMInfoBlock_t;

typedef struct {
	int32_t		Mid;
	int16_t		XRes;			
	int16_t		YRes;			
	int16_t		BitsPerPixel;	
	int16_t		MemModel;		
} ModeInfo_t;

typedef struct {	
	uint16_t	Offset;
	uint16_t	Seg;
} BIOSFarPointer;

typedef struct {
	uint32_t	VESASignature;
	uint16_t	VESAVersion;
	BIOSFarPointer	OEMStringPtr;
	uint32_t	Capabilities;
	BIOSFarPointer	VideoModePtr;
	uint16_t	TotalMemory;

	/* VESA2 */
 	uint16_t 	VBESoftwareRev;
	BIOSFarPointer	OEMVendorNamePtr;
	BIOSFarPointer	OEMProductNamePtr;
	BIOSFarPointer	OEMProductRevPtr;
	uint8_t		Reserved[222];
	uint8_t		OEMData[256];
} VESAInfoBlockStruct;

typedef struct {
	uint16_t	ModeAttributes;
	uint8_t		WinAAttributes;
	uint8_t		WinBAttributes;
	uint16_t	WinGranularity;
	uint16_t	WinSize;
	uint16_t	WinASegment;
	uint16_t	WinBSegment;
	BIOSFarPointer	WinFuncPtr;			/* Set to 0 */
	uint16_t	BytesPerScanLine;
	uint16_t	XResolution;
	uint16_t	YResolution;
	uint8_t		XCharSize;
	uint8_t		YCharSize;
	uint8_t		NumberOfPlanes;
	uint8_t		BitsPerPixel;
	uint8_t		NumberOfBanks;
	uint8_t		MemoryModel;
	uint8_t		BankSize;
	uint8_t		NumberOfImagePages;
	uint8_t		Reserved;			/* Set to 1 */
	
	uint8_t		RedMaskSize, RedFieldPosition;
	uint8_t		GreenMaskSize, GreenFieldPosition;
	uint8_t		BlueMaskSize, BlueFieldPosition;
	uint8_t		RsvdMaskSize, RsvdFieldPosition;
	uint8_t		DirectColorModeInfo;

	/* VESA2 */
	uint32_t	PhysBasePtr;
	uint32_t	OffScreenMemOffset;
	uint16_t	OffScreenMemSize;

	/* VBE 3.0 (optional) */
	uint16_t	LinBytesPerScanLine;
	uint8_t		BnkNumberOfImagePages;
	uint8_t		LinNumberOfImagePages;
	uint8_t		LinRedMaskSize;
	uint8_t		LinRedFieldPosition;
	uint8_t		LinGreenMaskSize;
	uint8_t		LinGreenFieldPosition;
	uint8_t		LinBlueMaskSize;
	uint8_t		LinBlueFieldPosition;
	uint8_t		LinRsvdMaskSize;
	uint8_t		LinRsvdFieldPosition;

	/* VBE 3.0 (mandatory) */
	uint32_t	MaxPixelClock;

	uint8_t		cur_reserved[189];
} VESAModeInfoStruct;

#include <_packpop.h>

/* frame buffer memory models */
#define	VESA_MM_TextMode			0x0
#define	VESA_MM_CGAGraphics			0x1
#define VESA_MM_HGCGraphics			0x2
#define VESA_MM_MonoGraphics			0x2
#define VESA_MM_VGA4Plane			0x3
#define VESA_MM_VGA8Packed			0x4
#define	VESA_MM_DirectColor			0x6
#define	VESA_MM_DirectYUV			0x7

__BEGIN_DECLS

struct vesa_edid *vesa_get_edid(vbios_context_t *vbios, struct vesa_edid *edid);
int vesa_HaveDPMS(vbios_context_t *vbios);
int vesa_HaveSetDisplayOffset(vbios_context_t *vbios);
VESAInfoBlockStruct *vesa_InfoBlock(vbios_context_t *vbios,
	VESAInfoBlockStruct *VIB);
uint8_t *vesa_OEMString(vbios_context_t *vbios,
	VESAInfoBlockStruct *VIPtr, uint8_t *str);
uint16_t *vesa_ModeList(vbios_context_t *vbios,
	VESAInfoBlockStruct *VIPtr, uint16_t *list);
VESAModeInfoStruct *vesa_ModeInfo(vbios_context_t *vbios,
	int mode, VESAModeInfoStruct *buffer);
int vesa_SetDisplayOffset(vbios_context_t *vbios, int x, int y, int wait_vsync);
int vesa_SetDPMSMode(vbios_context_t *vbios, int mode);
int vesa_BIOSSetPalette(vbios_context_t *vbios,
	disp_color_t *pal, int idx, int num);

__END_DECLS

#endif /* _GRAPHICS_VESA_H_INCLUDED */
#endif /* __X86__ */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/vesa.h $ $Rev: 657836 $" )
