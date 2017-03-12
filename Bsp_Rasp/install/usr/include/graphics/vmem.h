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





#ifndef _GRAPHICS_VMEM_H_INCLUDED
#define _GRAPHICS_VMEM_H_INCLUDED

#ifndef _GRAPHICS_DISPLAY_H_INCLUDED
#include <graphics/display.h>
#endif

/* hint_flags argument to "alloc_surface" */
#define DISP_VMEM_HINT_USAGE(__flags)		((__flags) & 0x0000000f)
#define DISP_VMEM_HINT_USAGE_CPU		0x00000001
#define DISP_VMEM_HINT_USAGE_2D			0x00000002
#define DISP_VMEM_HINT_USAGE_3D			0x00000003
#define DISP_VMEM_HINT_USAGE_3D_BACKBUFFER	0x00000004
#define DISP_VMEM_HINT_USAGE_VG			0x00000005

/* disp_aperture.flags */
#define DISP_APER_NOCACHE			0x00000001	/* Aperture must be mapped with PROT_NOCACHE */
#define DISP_APER_LAZYWRITE			0x00000002	/* Aperture supports write combining -- superceeded by DISP_APER_WRITECOMBINE */
#define DISP_APER_WRITECOMBINE		0x00000004	/* Aperture write combine */
#define DISP_APER_WRITETHROUGH		0x00000008	/* Aperture write through cache hit policy */
#define DISP_APER_WRITEBACK			0x00000010	/* Aperture write back cache hit policy */
#define DISP_APER_WRITEALLOC		0x00000020	/* Aperture write alloc cache miss policy */
#define DISP_APER_WRITENOALLOC		0x00000040	/* Aperture write no alloc cache miss policy */

#define	DISP_MAX_APERTURES			8

typedef struct disp_aperture {
	uint64_t	base;			/* Physical address */
	unsigned	size;
	void		*vaddr;
	unsigned	flags;
	unsigned	reserved[5];
} disp_aperture_t;

typedef struct disp_surface_info {
	int		aperture_index;
	unsigned	offset;
	unsigned	reserved[6];
} disp_surface_info_t;

#define DISP_ALLOCINFO_NO_EXTERNAL_ALLOC	0x00000001	/* Surface memory allocation on client side not supported */

typedef struct disp_alloc_info {
	unsigned	start_align;
	unsigned	end_align;
	unsigned	min_stride;
	unsigned	max_stride;
	unsigned	stride_gran;
	unsigned	prot_flags;
	unsigned	map_flags;
	unsigned	surface_flags;
	unsigned	alloc_flags;
	unsigned	row_gran;
	unsigned	reserved[2];
} disp_alloc_info_t;

__BEGIN_DECLS

typedef struct disp_memfuncs {
	int (*init)(disp_adapter_t *adapter, char *optstring);
	void (*fini)(disp_adapter_t *adapter);
	void (*module_info)(disp_adapter_t *adapter, disp_module_info_t *info);
	int (*reset)(disp_adapter_t *adapter, disp_surface_t *surf);
	disp_surface_t * (*alloc_surface)(disp_adapter_t *adapter, int width,
	    int height, unsigned format, unsigned sflags, unsigned hint_flags);
	int (*free_surface)(disp_adapter_t *adapter, disp_surface_t *surf);
	unsigned long (*mem_avail)(disp_adapter_t *adapter, unsigned sflags);
	disp_surface_t * (*alloc_layer_surface)(disp_adapter_t *adp,
	    int dispno[], int layer_idx[],
	    int nlayers, unsigned format, int surface_index,
	    int width, int height, unsigned sflags, unsigned hint_flags);
	int (*set_power_mode)(disp_adapter_t *adapter, pm_power_mode_t mode);
	int (*query_apertures)(disp_adapter_t *adapter, disp_aperture_t *apertures);
	int (*query_surface)(disp_adapter_t *adapter,
	    disp_surface_t *surf, disp_surface_info_t *info);
	int (*get_alloc_info)(disp_adapter_t *adapter, int width,
	    int height, unsigned format,
	    unsigned sflags, unsigned hint_flags, disp_alloc_info_t *info);
	int (*get_alloc_layer_info)(disp_adapter_t *adp,
	    int dispno[], int layer_idx[], int nlayers, unsigned format,
	    int surface_index, int width, int height,
	    unsigned sflags, unsigned hint_flags, disp_alloc_info_t *info);
   	int (*submit_alloced_info)(disp_adapter_t *adapter, disp_surface_t *surf, unsigned flags);
   	void	(*reserved[4])(void);
} disp_memfuncs_t;

/* Main memory manager entry point */
typedef int (*get_memfuncs_t)(disp_adapter_t *adapter,
    disp_memfuncs_t *funcs, int tabsize);

extern WINSIM_API int devg_get_memfuncs(disp_adapter_t *adapter,
    disp_memfuncs_t *funcs, int tabsize);

__END_DECLS

#endif /* _GRAPHICS_VMEM_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/vmem.h $ $Rev: 657836 $" )
