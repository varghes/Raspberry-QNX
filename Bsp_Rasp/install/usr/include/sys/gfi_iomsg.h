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



#ifndef __GFI_IOMSG_H_INCLUDED
#define __GFI_IOMSG_H_INCLUDED

#ifndef __IOMSG_H_INCLUDED
#include <sys/iomsg.h>
#endif

#ifndef _GF_GFI_H_INCLUDED
#include <gf/gfi.h>
#endif

#ifndef _IOMGR_INPUT
#define _IOMGR_INPUT	0x16
#endif

/*
 * This version number is used to detect version mismatches between the
 * GF lib and devi drivers.  At every incompatible release, the number should
 * be incremented.
 */
#define GFI_VERSION			0x00000001

#define GFI_IOMSG_SIZEOF(u_type)	(sizeof (io_msg_t) + sizeof (u_type))
#define GFI_IOMSG_REP_SIZEOF(u_type)	(sizeof (int) + sizeof (u_type))
#define GFI_GET_EVENT_MAX		32

typedef struct {
	io_msg_t	iomsg;
	union {
#define GFI_REGISTER_LAYER		0x0001
		struct {
			_Uint32t	gfi_version;
			_Int32t		z_pos;
		} reg;
#define GFI_UNREGISTER_LAYER		0x0002
		struct {
			_Int32t		z_pos;
		} unreg;
#define GFI_SET_ATTRS			0x0003
		struct {
			_Int32t		z_pos;
			_Int32t		x1, y1, x2, y2;
			_Uint32t	flags;		/* See gfi_attr */
		} set_attrs;
#define GFI_GET_EVENT			0x0004
		struct {
			_Int32t		nlayers;
			_Int32t		layer_z_pos[GFI_GET_EVENT_MAX];
			_Int32t		block;
		} get_event;
#define GFI_SET_FOCUS			0x0005
		struct {
			_Int32t		z_pos;
		} set_focus;
#define GFI_EMIT_EVENT			0x0006
		struct {
			_Int32t		z_pos;
			gfi_event_t	ev;
		} emit_event;
#define	GFI_SET_PTR_CTRL 			0x0007
		struct {
			_Int32t		flags;
		} set_ptr_ctrl;
#define	GFI_SET_PTR_MATRIX4			0x0008
		struct {
			_Int32t		swap;
			_Int32t		scr_xoffset, scr_yoffset, scr_xscale, scr_yscale;
			_Int32t		coord_xoffset, coord_yoffset, coord_xscale, coord_yscale;
		} set_ptr_matrix4;
#define	GFI_SET_PTR_MATRIX3			0x0009
		struct {
			int64_t		An, Bn, Cn, Dn, En, Fn, Divider;
		} set_ptr_matrix3;
	} u;
} gfi_iomsg_t;

typedef struct {
	union {
		struct {
		} reg;
		struct {
			gfi_event_t	ev;
			int		got_event;
		} get_event;
		struct {
		} dummy;
	} u;
} gfi_iomsg_reply_t;

#endif /* __GFI_IOMSG_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/private/sys/gfi_iomsg.h $ $Rev: 657836 $" )
