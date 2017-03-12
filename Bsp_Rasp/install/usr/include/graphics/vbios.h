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

#ifndef _GRAPHICS_VBIOS_H_INCLUDED
#define _GRAPHICS_VBIOS_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

typedef struct vbios_context {
	disp_adapter_t	*adp;
	unsigned	xfer_area_seg;		/* Real-mode segment */
	unsigned	xfer_area_off;		/* Real-mode offset */
	unsigned char	*xfer_area_ptr;		/* virtual address */

	/* Multiple controller support */
	uint8_t		*zeropage;		/* Drivers copy */
	uint8_t		*rom;			/* Drivers copy */
	int		romsize;
	uint8_t		*c0000_ptr;		/* Pointer to linear c0000 */

#ifndef __QNXNTO__
	void		*legacy_regs;
#endif
} vbios_context_t;

/* Flags passed to vbios_register */
#define VBIOS_FLAG_FORCE_EMU86	1
#define VBIOS_FLAG_FORCE_V86	2

/* Larget amount of data that can be transferred to/from BIOS code */
#define VBIOS_TRANSBUF_MAX	2048

#ifdef __QNXNTO__
#ifdef __X86__
#ifndef __V86_H_INCLUDED
#include <x86/v86.h>
#endif
typedef struct _v86reg	vbios_regs_t;
#endif
#else	/* !__QNXNTO__ */
typedef struct {
	long	edi, esi, ebp, exx, ebx, edx, ecx, eax;
	long	eip, cs, efl;
	long	esp, ss, es, ds, fs, gs;
} vbios_regs_t;
#endif

__BEGIN_DECLS

int vbios_register_noemu(disp_adapter_t *adp, unsigned flags);
int vbios_register(disp_adapter_t *adp, unsigned flags);
void vbios_unregister(vbios_context_t *ctx);
int vbios_int(vbios_context_t *ctx, int inum, vbios_regs_t *regs, int xfer_size);
int vbios_call(vbios_context_t *ctx, int seg, int offs, vbios_regs_t *regs, int xfer_size);
int vbios_dowarmboot(vbios_context_t *ctx, unsigned ax_val, unsigned char *biosptr, int bios_size);
void *vbios_get_realptr(vbios_context_t *ctx, unsigned seg, unsigned off);

int vbios_store_bios(disp_adapter_t *adp, vbios_context_t *vbios);
int vbios_retrieve_bios(disp_adapter_t *adp, uint8_t **zeropage, uint8_t **rom, int *romsize);

int vbios_acquire_bios_resources(vbios_context_t *ctx);
void vbios_release_bios_resources(vbios_context_t *ctx);

int vbios_set_text_mode3(vbios_context_t *ctx);

__END_DECLS

#endif /* __X86__ */

#endif /* _GRAPHICS_VBIOS_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/vbios.h $ $Rev: 657836 $" )
