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





#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#define _GRAPHICS_DISPLAY_H_INCLUDED

#ifndef __TYPES_H_INCLUDED
#include <sys/types.h>
#endif

#ifndef _WIN32
#ifdef __QNXNTO__
#ifndef __INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif
#endif
#ifndef __GULLIVER_H_INCLUDED
#include <gulliver.h>
#endif
#ifndef __PCI_H_INCLUDED
#include <hw/pci.h>
#endif
#ifndef __MMAN_H_INCLUDED
#include <sys/mman.h>
#endif
#endif /* _WIN32 */

#ifndef _STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef _WIN32
#ifndef	_PM_H_INCLUDED
#include <sys/pm.h>
#endif
#endif /* _WIN32 */

#ifndef _GRAPHICS_ROP_H_INCLUDED
#include <graphics/rop.h>
#endif

#ifndef _WIN32
#include <semaphore.h>
#endif

#ifdef _WIN32
	#include <gfpr/gfpr.h>
	#ifdef _MSC_VER // Win32 Visual C++
		#ifdef WINSIM_EXPORTS
	    	#define WINSIM_API __declspec(dllexport)
		#else
			#define WINSIM_API __declspec(dllimport)
		#endif
	
		#ifdef SOFT3D_EXPORTS
			#define SOFT3D_API __declspec(dllexport)
		#else
			#define SOFT3D_API __declspec(dllimport)
		#endif
	#else // Win32 mingw32
		#include <stdint.h>
		#define WINSIM_API
  		#define SOFT3D_API
	#endif
	typedef uint32_t		disp_paddr_t;
	typedef uint32_t		pm_power_mode_t;
	typedef uint64_t		paddr_t;
#else
	#define WINSIM_API
  	#define SOFT3D_API
	#ifndef __QNXNTO__
		#include <sys/cdefs.h>
		#ifndef _INTTYPES_H_INCLUDED
			typedef char				int8_t;
			typedef short				int16_t;
			typedef short				int16_t;
			typedef unsigned char			uint8_t;
			typedef unsigned short			uint16_t;
			typedef unsigned long			uint32_t;	/* This line may have to be omitted, depending on which version of the QNX4 header files you build against */
			typedef int8_t				_int8;
			typedef int16_t				_int16;
			typedef uint8_t				_uint8;
			typedef uint16_t			_uint16;
			typedef uint32_t			_uint32;
		#endif
		typedef uint32_t			disp_paddr_t;
		typedef uint32_t			uintptr_t;
	#else
		typedef uint64_t			disp_paddr_t;
	#endif
	#ifndef _HW_INOUT_INCLUDED
		#include <hw/inout.h>
	#endif
#endif /* _WIN32 */

#define DISP_MAX_MODES			72

/*
 * Supported layer formats; note that a layer format is comprised of
 * of one or more surface formats.  Please don't use the most significant bit.
 */
#define DISP_LAYER_FORMAT_PAL8				0x00000000
#define DISP_LAYER_FORMAT_ARGB1555			0x00000001
#define DISP_LAYER_FORMAT_RGB565			0x00000002
#define DISP_LAYER_FORMAT_RGB888			0x00000003
#define DISP_LAYER_FORMAT_ARGB8888			0x00000004
#define DISP_LAYER_FORMAT_RGB556			0x00000005
#define DISP_LAYER_FORMAT_BYTES				0x00000006
#define DISP_LAYER_FORMAT_YUY2				0x00010000	/* Packed YUV, 16bpp: Y8U8Y8V8 */
#define DISP_LAYER_FORMAT_UYVY				0x00010001	/* Packed YUV, 16bpp: U8Y8V8Y8 */
#define DISP_LAYER_FORMAT_YVYU				0x00010002	/* Packed YUV, 16bpp: Y8V8Y8U8 */
#define DISP_LAYER_FORMAT_V422				0x00010003	/* Packed YUV, 16bpp: V8Y8U8Y8 */
#define DISP_LAYER_FORMAT_NV12				0x00010004	/* Y plane immediately followed by packed UV plane, one U sample and one V sample per 2x2 block of Y */
#define DISP_LAYER_FORMAT_AYUV				0x00010005	/* Packed YUV, 32bpp: V8U8Y8A8 */
#define DISP_LAYER_FORMAT_YUY2_INTERLACED	0x00018000	/* Packed YUV, 16bpp: Y8U8Y8V8, interlaced */
#define DISP_LAYER_FORMAT_UYVY_INTERLACED	0x00018001	/* Packed YUV, 16bpp: U8Y8V8Y8, interlaced */
#define DISP_LAYER_FORMAT_YVYU_INTERLACED	0x00018002	/* Packed YUV, 16bpp: Y8V8Y8U8, interlaced */
#define DISP_LAYER_FORMAT_V422_INTERLACED	0x00018003	/* Packed YUV, 16bpp: V8Y8U8Y8, interlaced */
#define DISP_LAYER_FORMAT_NV12_INTERLACED	0x00018004	/* Y plane immediately followed by packed UV plane, one U sample and one V sample per 2x2 block of Y, interlaced */
#define DISP_LAYER_FORMAT_AYUV_INTERLACED	0x00018005	/* Packed YUV, 32bpp: V8U8Y8A8, interlaced */
#define DISP_LAYER_FORMAT_YVU9				0x00020000	/* Planar YUV, one U sample and one V sample per 4x4 block of Y */
#define DISP_LAYER_FORMAT_YV12				0x00020001	/* Planar YUV, one U sample and one V sample per 2x2 block of Y */
#define DISP_LAYER_FORMAT_YUV420			0x00020002	/* Planar YUV, one U sample and one V sample per 2x1 block of Y */
#define DISP_LAYER_FORMAT_YVU9_INTERLACED	0x00028000	/* Planar YUV, one U sample and one V sample per 4x4 block of Y, interlaced */
#define DISP_LAYER_FORMAT_YV12_INTERLACED	0x00028001	/* Planar YUV, one U sample and one V sample per 2x2 block of Y, interlaced */
#define DISP_LAYER_FORMAT_YUV420_INTERLACED	0x00028002	/* Planar YUV, one U sample and one V sample per 2x1 block of Y, interlaced */

/* surface formats - upper byte specifies bits per pixel */
#define DISP_SURFACE_FORMAT_BYTES			0x08000000
#define DISP_SURFACE_FORMAT_PAL8			0x08000001
#define DISP_SURFACE_FORMAT_ARGB1555		0x10000002
#define DISP_SURFACE_FORMAT_RGB565			0x10000003
#define DISP_SURFACE_FORMAT_RGB888			0x18000004
#define DISP_SURFACE_FORMAT_ARGB8888		0x20000005
#define DISP_SURFACE_FORMAT_RGBA8888		0x20000006
#define DISP_SURFACE_FORMAT_PAL4			0x04000006
#define DISP_SURFACE_FORMAT_RGB556			0x10000007

/* Packed YUV formats */
#define DISP_SURFACE_FORMAT_PACKEDYUV_IYU1				0x0c40000a
#define DISP_SURFACE_FORMAT_PACKEDYUV_IYU2				0x1840000b
#define DISP_SURFACE_FORMAT_PACKEDYUV_UYVY				0x1040000c
#define DISP_SURFACE_FORMAT_PACKEDYUV_YUY2				0x1040000d
#define DISP_SURFACE_FORMAT_PACKEDYUV_YVYU				0x1040000e
#define DISP_SURFACE_FORMAT_PACKEDYUV_V422				0x1040000f
#define DISP_SURFACE_FORMAT_PACKEDYUV_CLJR				0x08400010
#define DISP_SURFACE_FORMAT_PACKEDYUV_AYUV				0x20400011
#define DISP_SURFACE_FORMAT_PACKEDYUV_IYU1_INTERLACED	0x0c600012
#define DISP_SURFACE_FORMAT_PACKEDYUV_IYU2_INTERLACED	0x18600013
#define DISP_SURFACE_FORMAT_PACKEDYUV_UYVY_INTERLACED	0x10600014
#define DISP_SURFACE_FORMAT_PACKEDYUV_YUY2_INTERLACED	0x10600015
#define DISP_SURFACE_FORMAT_PACKEDYUV_YVYU_INTERLACED	0x10600016
#define DISP_SURFACE_FORMAT_PACKEDYUV_V422_INTERLACED	0x10600017
#define DISP_SURFACE_FORMAT_PACKEDYUV_CLJR_INTERLACED	0x08600018
#define DISP_SURFACE_FORMAT_PACKEDYUV_AYUV_INTERLACED	0x20600019

/* YUV planar surface types */
#define DISP_SURFACE_FORMAT_YPLANE		0x00400011
#define DISP_SURFACE_FORMAT_UPLANE		0x00400012
#define DISP_SURFACE_FORMAT_VPLANE		0x00400013

/* Useful macros to operate on surface types */
#define DISP_BITS_PER_PIXEL(__fmt)	((__fmt) >> 24)
#define DISP_BYTES_PER_PIXEL(__fmt)	(((__fmt) + 0x7000000) >> 27)
#define DISP_SURFACE_ISPAL(__fmt)	(((__fmt) & 0x00800000) || ((__fmt) ==  DISP_SURFACE_FORMAT_PAL4) || ((__fmt) ==  DISP_SURFACE_FORMAT_PAL8))
#define DISP_SURFACE_ISYUV(__fmt)	((__fmt) & 0x00400000)
#define DISP_SURFACE_IS_INTERLACED(__fmt)	((__fmt) & 0x00200000)

/* RGB true color macros */
#define DISP_TRUE_BLUE(color)	((color) & 0xff)
#define DISP_TRUE_GREEN(color)	(((color) & 0xff00) >> 8)
#define DISP_TRUE_RED(color)	(((color) & 0xff0000) >> 16)
#define DISP_TRUE_ALPHA(color)	(((color) & 0xff000000) >> 24)
#define DISP_MK_TRUECOLOR(a, r, g, b) \
				((a) << 24 | (r) << 16 | (g) << 8 | b)

/* Alpha Blending multiplier origins */
#define DISP_ALPHA_M1_SRC_PIXEL_ALPHA		0x00010000	/* "M1" comes from the source pixels alpha component */
#define DISP_ALPHA_M1_DST_PIXEL_ALPHA		0x00020000	/* "M1" comes from the destination pixels alpha component */
#define DISP_ALPHA_M1_GLOBAL			0x00040000	/* "M1" comes from the global multiplier 1 */
#define DISP_ALPHA_M1_MAP			0x00080000	/* "M1" comes from the Alpha map */
#define DISP_ALPHA_M2_SRC_PIXEL_ALPHA		0x01000000	/* "M2" comes from the source pixels alpha component */
#define DISP_ALPHA_M2_DST_PIXEL_ALPHA		0x02000000	/* "M2" comes from the destination pixels alpha component */
#define DISP_ALPHA_M2_GLOBAL			0x04000000	/* "M2" comes from the global multiplier 2 */
#define DISP_ALPHA_M2_MAP			0x08000000	/* "M2" comes from the Alpha map */
#define DISP_ALPHA_M1_ORIGIN(__mode)		((__mode) & 0x00ff0000)
#define DISP_ALPHA_M2_ORIGIN(__mode)		((__mode) & 0x0f000000)

#define DISP_ALPHA_TEST				0x10000000	/* Alpha test instead of blend */
#define DISP_ALPHA_MERGE			0x20000000	/* Merge two semi-transparent images with per-pixel alpha (experimental) */

/* Alpha Blending Source Factor */
#define DISP_BLEND_SRC_0			0x0000		/* (0,0,0,0) */
#define DISP_BLEND_SRC_M1			0x0100		/* (M1,M1,M1,M1) */
#define DISP_BLEND_SRC_1mM1			0x0200		/* (1-M1,1-M1,1-M1,1-M1) */
#define DISP_BLEND_SRC_1mD 			0x0300		/* (1-M2,1-Rd,1-Gd,1-Bd) */
#define DISP_BLEND_SRC_M2			0x0400		/* (M2,M2,M2,M2) */
#define DISP_BLEND_SRC_D			0x0500		/* (M2,Rd,Gd,Bd) */
#define DISP_BLEND_SRC_1			0x0600		/* (1,1,1,1) */
#define DISP_BLEND_SRC_A1M1			0x0700		/* (1,M1,M1,M1) */
#define DISP_BLEND_SRC_1mM2			0x0800		/* (1-M2,1-M2,1-M2,1-M2) */
#define DISP_BLEND_SRC_1mA1M1			0x0900		/* (1-1,1-M1,1-M1,1-M1) */
#define DISP_BLEND_SRC_A1M2			0x0A00		/* (1,M2,M2,M2) */
#define DISP_BLEND_SRC_1mA1M2			0x0B00		/* (1-1,1-M2,1-M2,1-M2) */
#define DISP_BLEND_SRC_A0M1			0x0C00		/* (0,M1,M1,M1) */
#define DISP_BLEND_SRC_1mA0M1			0x0D00		/* (1-0,1-M1,1-M1,1-M1) */
#define DISP_BLEND_SRC_A0M2			0x0E00		/* (0,M2,M2,M2) */
#define DISP_BLEND_SRC_1mA0M2			0x0F00		/* (1-0,1-M2,1-M2,1-M2) */

#define DISP_BLEND_SRC_M1_ALPHA			DISP_BLEND_SRC_M1	
#define DISP_BLEND_SRC_ONE_MINUS_M1_ALPHA	DISP_BLEND_SRC_1mM1	
#define DISP_BLEND_SRC_M2_ALPHA			DISP_BLEND_SRC_M2
#define DISP_BLEND_SRC_ONE_MINUS_M2_ALPHA	DISP_BLEND_SRC_1mM2
#define DISP_BLEND_SRC_GET(__mode)		((__mode) & 0xff00)

/* Alpha Blending Destination Factor */
#define DISP_BLEND_DST_0			0x00		/* (0,0,0,0) */
#define DISP_BLEND_DST_M1			0x01		/* (M1,M1,M1,M1) */
#define DISP_BLEND_DST_1mM1			0x02		/* (1-M1,1-M1,1-M1,1-M1) */
#define DISP_BLEND_DST_1mS 			0x03		/* (1-M1,1-Rs,1-Gs,1-Bs) */
#define DISP_BLEND_DST_M2			0x04		/* (M2,M2,M2,M2) */
#define DISP_BLEND_DST_S			0x05		/* (M1,Rs,Gs,Bs) */
#define DISP_BLEND_DST_1			0x06		/* (1,1,1,1) */
#define DISP_BLEND_DST_A1M1			0x07		/* (1,M1,M1,M1) */
#define DISP_BLEND_DST_1mM2			0x08		/* (1-M2,1-M2,1-M2,1-M2) */
#define DISP_BLEND_DST_1mA1M1			0x09		/* (1-1,1-M1,1-M1,1-M1) */
#define DISP_BLEND_DST_A1M2			0x0A		/* (1,M2,M2,M2) */
#define DISP_BLEND_DST_1mA1M2			0x0B		/* (1-1,1-M2,1-M2,1-M2) */
#define DISP_BLEND_DST_A0M1			0x0C		/* (0,M1,M1,M1) */
#define DISP_BLEND_DST_1mA0M1			0x0D		/* (1-0,1-M1,1-M1,1-M1) */
#define DISP_BLEND_DST_A0M2			0x0E		/* (0,M2,M2,M2) */
#define DISP_BLEND_DST_1mA0M2			0x0F		/* (1-0,1-M2,1-M2,1-M2) */

#define DISP_BLEND_DST_M1_ALPHA			DISP_BLEND_DST_M1	
#define DISP_BLEND_DST_ONE_MINUS_M1_ALPHA	DISP_BLEND_DST_1mM1	
#define DISP_BLEND_DST_M2_ALPHA			DISP_BLEND_DST_M2
#define DISP_BLEND_DST_ONE_MINUS_M2_ALPHA	DISP_BLEND_DST_1mM2
#define DISP_BLEND_DST_GET(__mode)		((__mode) & 0x00ff)

/* Alpha Tests Pixels are written in destination */
#define DISP_TEST_NEVER				0x00		/* Never */
#define DISP_TEST_ALWAYS			0x01		/* Always */
#define DISP_TEST_LESS_THAN			0x02		/* if M1 < M2 */
#define DISP_TEST_LESS_THAN_OR_EQUAL		0x03		/* if M1 <= M2 */
#define DISP_TEST_EQUAL				0x04		/* if M1 == M2 */
#define DISP_TEST_GREATER_THAN_OR_EQUAL		0x05		/* if M1 >= M2 */
#define DISP_TEST_GREATER_THAN			0x06		/* if M1 > M2*/
#define DISP_TEST_NOT_EQUAL			0x07		/* if M1 != M2 */
#define DISP_TEST_GET(__mode)			((__mode) & 0x00ff)

/* Valid alpha combinations */
#define DISP_ALPHA_CAP_SPP_WITH_GLOBAL		0x00000001	/* Source Per-pixel with Dest global supported */
#define DISP_ALPHA_CAP_GLOBAL_WITH_DPP		0x00000002	/* Source global with Dest Per-pixel supported */
#define DISP_ALPHA_CAP_SPP_WITH_DPP		0x00000004	/* Source per-pixel with Dest per-pixel supported */
#define DISP_ALPHA_CAP_GLOBAL_WITH_GLOBAL	0x00000008	/* Source global with Dest global supported */

#define DISP_ALPHA_BLEND_CAP_SPP_WITH_GLOBAL	DISP_ALPHA_CAP_SPP_WITH_GLOBAL
#define DISP_ALPHA_BLEND_CAP_GLOBAL_WITH_DPP	DISP_ALPHA_CAP_GLOBAL_WITH_DPP
#define DISP_ALPHA_BLEND_CAP_SPP_WITH_DPP	DISP_ALPHA_CAP_SPP_WITH_DPP
#define DISP_ALPHA_BLEND_CAP_GLOBAL_WITH_GLOBAL DISP_ALPHA_CAP_GLOBAL_WITH_GLOBAL

/* Chroma Key operations */
#define DISP_CHROMA_OP_SRC_MATCH		0x00000001	/* Match Color is Source Color */
#define DISP_CHROMA_OP_DST_MATCH		0x00000002	/* Match Color is Destination Color */
#define DISP_CHROMA_OP_DRAW			0x00000004	/* if Match Color == Color, Draw */
#define DISP_CHROMA_OP_NO_DRAW			0x00000008	/* if Match Color == Color, Don't Draw */
#define DISP_CHROMA_OP_UNDEFINED		~(DISP_CHROMA_OP_SRC_MATCH | \
						    DISP_CHROMA_OP_DST_MATCH | \
						    DISP_CHROMA_OP_DRAW | \
						    DISP_CHROMA_OP_NO_DRAW)

typedef uint32_t	disp_color_t;
typedef struct {
	int32_t		x;
	int32_t		y;
} disp_point_t;
typedef int		disp_fx_t;		/* Fixed point: S1:I15:F16 */

#define DISP_ADD_FUNC(tabletype, table, entry, func, limit) \
	if (((size_t)&(((tabletype *)0)->entry)) + sizeof (void (*)()) <= (size_t)(limit)) \
		(table)->entry = (func);

/* disp_adapter.caps */
#define DISP_CAP_MULTI_MONITOR_SAFE	0x00000001
#define DISP_CAP_2D_ACCEL		0x00000002
#define DISP_CAP_3D_ACCEL		0x00000004
#define DISP_CAP_NO_IO_PRIVITY		0x00000008	/* 2D/3D entry points can be called without I/O privity */
#define DISP_CAP_DYNAMIC_MODESWITCH	0x00000010	/* Display modeswitches can occur without damaging any surfaces, or disturbing the layer state  */
#define DISP_CAP_VG_ACCEL		0x00000020	/* driver has some form of VG acceleration support */

/* disp_adapter.bus_type */
#define DISP_BUS_TYPE_UNKNOWN			0x00000000
#define DISP_BUS_TYPE_PCI			0x00000001
#define DISP_BUS_TYPE_AGP			0x00000002
#define DISP_BUS_TYPE_ISA			0x00000004
#define DISP_BUS_TYPE_VL			0x00000010
#define DISP_BUS_TYPE_IS_PCI_OR_AGP(__type)	((__type) & (DISP_BUS_TYPE_PCI|DISP_BUS_TYPE_AGP))

#define	DISP_PADDR_INVALID			(~(disp_paddr_t)0)

/* Commands for disp_adapter_t.callback */
#define DISP_CALLBACK_LOCK			1
#define DISP_CALLBACK_UNLOCK			2
#define DISP_CALLBACK_WAIT_IDLE			3
#define DISP_CALLBACK_ALLOC_SURFACE		4
#define DISP_CALLBACK_FREE_SURFACE		5
#define DISP_CALLBACK_WAIT_VSYNC		6

typedef struct disp_adapter {
	int		size;			/* size of structure */
	void		*gd_ctx;		/* graphics drivers private context */
	void		*ms_ctx;		/* Mode-switchers private context */
	void		*mm_ctx;		/* Memory-managers private context */
	void		*vid_ctx;		/* Video Overlay drivers private context */
	void		*vcap_ctx;		/* Video Capture drivers private context */
	void *		(*callback)(void *handle, unsigned cmd, void *data);
	void		*callback_handle;
	volatile unsigned	*vsync_counter;
	void		*display_update_mutex;
	uintptr_t	reserved;
	void		**hook_context;
	int		hook_index;
	int		irq;			/* Devices irq, set by disp_pci_init; -1 if unable to generate interrupts */
	uintptr_t	reserved1[2];
	int		bus_type;
	union {
		struct {
#ifdef __QNXNTO__
			int		pci_handle;		/* Used internally by disputil */
			void		*pci_dev_handle;	/* Used internally by disputil */
#else
			unsigned	pci_bus;		/* Used internally by disputil */
			unsigned	pci_devfunc;		/* Used internally by disputil */
#endif
			unsigned short	pci_vendor_id;		/* Devices PCI vendor ID (0 if not applicable) */
			unsigned short	pci_device_id;		/* Devices PCI device ID */
			short		pci_index;		/* Together, {pci_vendor_id, pci_deviceid, pci_index} uniquely identify a PCI device in the system */
#ifdef __QNXNTO__
			struct pci_dev_info	*dev_info;
#else
			uintptr_t	reserved;		/* Current flags to pass to pci_attach */
#endif
			disp_paddr_t	base[6];		/* devices PCI aperture base addresses; set by disp_pci_init */
			disp_paddr_t	apsize[6];		/* size of PCI apertures; set by disp_pci_init */
		} pci;
		struct {
			unsigned	reserved1;
			unsigned	reserved2;
			unsigned short	vendor_id;	/* Devices vendor ID (0 if not applicable) */
			unsigned short	device_id;	/* Devices device ID */
			short		index;		/* Together, {vendor_id, deviceid, index} uniquely identify a device in the system */
		} generic;
	} bus;
	unsigned	caps;			/* Adapter capabilities */
	FILE		*dbgfile;		/* Debug sent here if non-NULL */
	uintptr_t	reserved2;		/* interrupts the card can generate */
	uintptr_t	reserved3;
	void		*shmem;

	void		*hydra_shmem;		/* For multi-monitor support */

	struct disp_modefuncs	*modefuncs;
	struct disp_memfuncs	*memfuncs;

	int			adapter_ram;	/* Total memory installed on the adapter */

#if !defined(__QNXNTO__) || defined (__X86__)
	struct vbios_context	*vbios;
#else
	uintptr_t		reservedv;
#endif
	void			*ffb_extension;
	int			pulseprio;
	uintptr_t		reserved4[2];

	unsigned		cbsize;		/* permanetly allocated video capture buffer size */
	unsigned		dflags;		/* debug flags */
} disp_adapter_t;

/* Constants for filling in the disp_module_info structure */
#define	DDK_VERSION_MAJOR	1
#define	DDK_VERSION_MINOR	2
#define	DDK_REVISION		0

typedef struct disp_module_info {
	uint8_t		ddk_version_major;	/* Major version of DDK used to build driver */
	uint8_t		ddk_version_minor;	/* Minor version of DDK used to build driver */
	uint8_t		ddk_rev;		/* Rev. of DDK used to build driver */
	uint8_t		driver_rev;		/* Driver revision number */
	char		*description;
	unsigned	reserved[16];
} disp_module_info_t;

/* Flags for memory management */
#define DISP_PROT_READ			(PROT_READ)
#define DISP_PROT_WRITE			(PROT_WRITE)
#define DISP_PROT_NOCACHE		(PROT_NOCACHE)
#ifdef __QNXNTO__
#define DISP_MAP_PHYS			(MAP_PHYS)
#define DISP_MAP_BELOW16M		(MAP_BELOW16M)
#define DISP_MAP_LAZYWRITE		(MAP_LAZY) /* superceeded by DISP_MAP_WRITECOMBINE */
#define DISP_MAP_LAZY			(DISP_MAP_LAZYWRITE) /* superceeded by DISP_MAP_WRITECOMBINE */
#define DISP_MAP_WRITECOMBINE	0x00000100 /* write combine */
#define DISP_MAP_WRITETHROUGH	0x00000200 /* write through cache hit policy */
#define DISP_MAP_WRITEBACK		0x00000400 /* write back cache hit policy */
#define DISP_MAP_WRITEALLOC		0x00000800 /* write alloc cache miss policy */
#define DISP_MAP_WRITENOALLOC	0x00100000 /* write no alloc cache miss policy */
#else
#define DISP_MAP_PHYS			0x00010000	/* Get physically contiguous RAM */
#define DISP_MAP_BELOW16M		0x00040000	/* Get RAM that is physically below 16 Meg */
#define DISP_MAP_LAZY        		0x00000080	/* Map with write combining (or equivalent caching policy; processor dependant) */
#endif

/* disp_surface.flags */
/* Surface capabilities flags */
#define	DISP_SURFACE_DISPLAYABLE		0x00000001	/* Surface can be displayed via CRT controller */	
#define	DISP_SURFACE_CPU_LINEAR_READABLE	0x00000002	/* CPU can read this surface directly */
#define	DISP_SURFACE_CPU_LINEAR_WRITEABLE	0x00000004	/* CPU can write to this surface directly */
#define	DISP_SURFACE_2D_READABLE		0x00000008	/* surface is read-accessible by 2D engine */
#define	DISP_SURFACE_3D_READABLE		0x00000010	/* surface is read-accessible by 3D engine */
#define	DISP_SURFACE_2D_TARGETABLE		0x00000020	/* 2D engine can render into surface */
#define	DISP_SURFACE_3D_TARGETABLE		0x00000040	/* 3D engine can render into surface */
#define	DISP_SURFACE_SCALER_DISPLAYABLE		0x00000080	/* Surface can be displayed via video overlay scaler */
#define	DISP_SURFACE_VMI_TARGETABLE		0x00000100	/* Video in hardware can write frames into surface */
#define DISP_SURFACE_DMA_SAFE			0x00000200	/* DMA engine can treat the surface memory as one contiguous block */
#define DISP_SURFACE_PAGE_ALIGNED		0x00000400	/* Surface memory starts on a page boundary, and its linear size is a multiple of the page size */
#define DISP_SURFACE_MULTISAMPLED		0x00000800	/* Surface can be used in a multisampled Khronos configuration */
#define DISP_SURFACE_BYTES_REVERSED		0x00001000	/* Byte order is reversed.  Only valid for 16 and 32bpp surfaces */
#define DISP_SURFACE_OTHER_ENDIAN		0x00001000	/* Obsolete - use DISP_SURFACE_BIG_ENDIAN */
#define DISP_SURFACE_NON_CACHEABLE		0x00002000	/* Memory should be mapped non-cachable */
#define DISP_SURFACE_WT_CACHEABLE		0x00004000	/* Memory can be mapped write-through cacheable (but not write-back cacheable) */
#define DISP_SURFACE_PHYS_CONTIG		0x00008000	/* Memory is physically contiguous */
#define DISP_SURFACE_DRIVER_NOT_OWNER		0x00010000	/* Driver did not create/allocate this surface (e.g. images coming from higher-level software) */
#define DISP_SURFACE_ALPHA_MAP			0x00020000	/* Surface can be used as an alpha map for rendering */
#define DISP_SURFACE_RECT			0x00040000
#define DISP_SURFACE_STRIDE_ALIGNED		0x00080000
#define DISP_SURFACE_DRIVER_PRIVATE		0x00100000	/* Has surface been specially mapped for the given process */
#define DISP_SURFACE_VG_READABLE			0x00200000	/* surface is read-accessible by VG engine */
#define DISP_SURFACE_VG_TARGETABLE		0x00400000	/* VG engine can render into surface */
#define DISP_SURFACE_UMA_MEM				0x00800000	/* surface is allocated by UMA GPU through an aperature */

/* Hint flags - mutually exclusive */
#define DISP_SURFACE_SLOW_CPU_ACCESS		0x80000000	/* Surface memory access by CPU is expensive */
#define DISP_SURFACE_SLOW_2D_ENGINE_ACCESS	0x40000000	/* Surface memory access by 2D engine is expensive */
#define DISP_SURFACE_SLOW_3D_ENGINE_ACCESS	0x40000000	/* Surface memory access by 3D engine is expensive */

#define DISP_SURFACE_ZONE_MASK			0x0f000000
#define	DISP_SURFACE_ZONE_SHIFT			24
#define DISP_SURFACE_CAPS_MASK			0x00ffffff

typedef uint8_t*	ch_address;

typedef struct disp_surface {
	int		size;			/* Size of this struct */
	unsigned	pixel_format;
	unsigned	offset;			/* device-dependent address */
	unsigned char	*vidptr;		/* GPU virtual address */
	disp_paddr_t	paddr;			/* physical address */
	unsigned	stride;			/* In bytes */
	unsigned	flags;			/* Surface flags */
	int		width;			/* In pixels */
	int		height;
	disp_color_t	*palette;		/* palette associated with this surface, if any */
	int		palette_valid_entries;	/* size of palette */
	disp_adapter_t	*adapter;		/* the adapter which created this surface (if any) */
	ch_address    **buffer;       /* double index for surface buffer */
	uintptr_t	driver_private;
	unsigned char	*ffb_vidptr;		/* S/W Optimized vidptr, may be the same as vidptr, or NULL. */
	unsigned	ffb_size;		/* Size of memory area referenced by ffb_vidptr. */
	uintptr_t	reserved[5];
} disp_surface_t;

/* Device specific capabilities/requirements that
 * are returned in disp_iomsg_reply_t.u.reg.dev_flags.
 */
#define	DISP_DEV_REQUIRES_LAYER_FLUSHRECT	0x0001

#ifdef DISP_DEBUG
/*
 * This assert deliberately causes a fault upon failure, so that you can
 * obtain a core dump of the process at the time of the assertion failure
 */
#define DISP_ASSERT(x)	if (!(x)) { \
				fprintf(stderr, \
				    "%s, line %d: assert failed: %s\n", \
				    __FILE__, __LINE__, #x); \
				(*(char * volatile)0)++; \
			}
#else
#define DISP_ASSERT(x)
#endif

#ifndef _GRAPHICS_VMEM_H_INCLUDED
#include <graphics/vmem.h>
#endif
#ifndef _GRAPHICS_DRAW_H_INCLUDED
#include <graphics/draw.h>
#endif
#ifndef _GRAPHICS_MODE_H_INCLUDED
#include <graphics/mode.h>
#endif
#ifndef _GRAPHICS_VID_H_INCLUDED
#include <graphics/vid.h>
#endif
#ifndef _GRAPHICS_VCAP_H_INCLUDED
#include <graphics/vcap.h>
#endif
#ifndef _GRAPHICS_DINLINE_H_INCLUDED
#include <graphics/dinline.h>
#endif
#ifndef _GRAPHICS_FIXED_H_INCLUDED
#include <graphics/fixed.h>
#endif

__BEGIN_DECLS

extern int devg_hook_fini(disp_adapter_t *adapter);

__END_DECLS

#endif /* _GRAPHICS_DISPLAY_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/display.h $ $Rev: 657836 $" )
