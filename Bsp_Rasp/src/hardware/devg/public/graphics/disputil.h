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





#ifndef _GRAPHICS_DISPUTIL_H_INCLUDED
#define _GRAPHICS_DISPUTIL_H_INCLUDED

#ifndef __TYPES_H_INCLUDED
#include <sys/types.h>
#endif
#ifndef _WIN32
#ifndef __INTTYPES_H_INCLUDED
#ifdef __QNXNTO__
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
#endif

/* Flags for disp_pci_init() */
#ifdef __QNXNTO__
#define	DISP_PCI_INIT_IRQ		(PCI_INIT_IRQ)
#define	DISP_PCI_INIT_ROM		(PCI_INIT_ROM)
#define	DISP_PCI_INIT_BASE0		(PCI_INIT_BASE0)
#define	DISP_PCI_INIT_BASE1		(PCI_INIT_BASE1)
#define	DISP_PCI_INIT_BASE2		(PCI_INIT_BASE2)
#define	DISP_PCI_INIT_BASE3		(PCI_INIT_BASE3)
#define	DISP_PCI_INIT_BASE4		(PCI_INIT_BASE4)
#define	DISP_PCI_INIT_BASE5		(PCI_INIT_BASE5)
#define	DISP_PCI_INIT_MASTER		(PCI_MASTER_ENABLE)
#else
#define	DISP_PCI_INIT_IRQ		0x00010000
#define	DISP_PCI_INIT_ROM		0x00020000
#define	DISP_PCI_INIT_BASE0		0x00040000
#define	DISP_PCI_INIT_BASE1		0x00080000
#define	DISP_PCI_INIT_BASE2		0x00100000
#define	DISP_PCI_INIT_BASE3		0x00200008
#define	DISP_PCI_INIT_BASE4		0x00400000
#define	DISP_PCI_INIT_BASE5		0x00800000
#define	DISP_PCI_INIT_MASTER		0x01000000
#endif
#define	DISP_PCI_INIT_BASES		(DISP_PCI_INIT_BASE0 | \
						DISP_PCI_INIT_BASE1 | \
						DISP_PCI_INIT_BASE2 | \
						DISP_PCI_INIT_BASE3 | \
						DISP_PCI_INIT_BASE4 | \
						DISP_PCI_INIT_BASE5)
#define	DISP_PCI_INIT_ALL		(DISP_PCI_INIT_BASES | \
						DISP_PCI_INIT_ROM | \
						DISP_PCI_INIT_IRQ)

#define DISP_VM_MAX_ZONES	16

typedef struct {
	uint8_t	misc_out;
	uint8_t	seq[5];
	uint8_t	crtc[25];
	uint8_t	gc[9];
	uint8_t	attr[21];
	uint8_t	pal[64*3];
	uint8_t	pad[3];		/* Make struct a multiple of 4 bytes in size */
} disp_vga_state_t;

typedef struct disp_vm_pool *		pool_handle_t;

__BEGIN_DECLS

/* Prototypes for functions in display driver utility lib */
int disp_register_adapter(disp_adapter_t *adp);
int disp_unregister_adapter(disp_adapter_t *adp);
int disp_crtc_calc(disp_crtc_settings_t *display);
int disp_mode_get_entry(disp_adapter_t *adapter, disp_crtc_settings_t *settings,
    const char *fname, int xres, int yres, int refresh);
int disp_check_generic_mode(disp_adapter_t *adapter, disp_mode_info_t *mi,
    int xres, int yres, int refresh);
int disp_acquire_vga_resources(disp_adapter_t *adapter);
int disp_release_vga_resources(disp_adapter_t *adapter);
void disp_perror(disp_adapter_t *adp, char *what);
void disp_printf(disp_adapter_t *adp, const char *fmt, ...);
void disp_wait_condition(disp_adapter_t *adapter,
    int (*check)(disp_adapter_t *, void *arg), void *arg);

/* PCI access */
int disp_pci_init(disp_adapter_t *adapter, unsigned flags);
int disp_pci_shutdown(disp_adapter_t *adapter);
int disp_pci_info(unsigned *lastbus, unsigned *version, unsigned *hardware);
int disp_pci_read_config(disp_adapter_t *adapter, unsigned offset,
    unsigned cnt, size_t size, void *bufptr);
int disp_pci_write_config(disp_adapter_t *adapter, unsigned offset,
    unsigned cnt, size_t size, void *bufptr);
int disp_pci_dev_find(unsigned devid, unsigned venid, unsigned index,
    unsigned *bus, unsigned *devfunc);
int disp_pci_dev_read_config(unsigned bus, unsigned devfunc, unsigned offset,
    unsigned cnt, size_t size, void *bufptr);
int disp_pci_dev_write_config(unsigned bus, unsigned devfunc, unsigned offset,
    unsigned cnt, size_t size, void *bufptr);

/* Memory management */
void *disp_mmap_device_memory(disp_paddr_t base, size_t len, int prot, int flags);
unsigned long disp_mmap_device_io(size_t len, disp_paddr_t base);
void disp_munmap_device_memory(void volatile * addr, size_t len);
int disp_munmap_device_io(uintptr_t io, size_t len);
disp_paddr_t disp_phys_addr(void *addr);
void *disp_getmem(int size, unsigned prot, unsigned flags);
void disp_freemem(void *addr, int size);

/* Video memory management */
disp_surface_t *disp_vm_alloc_surface(pool_handle_t pool, int width, int height,
    int stride, unsigned format, unsigned flags);
int disp_vm_free_surface(disp_adapter_t *adapter, disp_surface_t *surf);
disp_surface_t * disp_vm_alloc_surface_external(disp_adapter_t *adapter,
    int width, int height, unsigned flags);
void disp_vm_free_surface_external(disp_adapter_t *adapter,
    disp_surface_t *surf);
unsigned long disp_vm_mem_avail(pool_handle_t pool);
pool_handle_t disp_vm_create_pool(disp_adapter_t *adapter,
    disp_surface_t *surf, int bytealign);
int disp_vm_destroy_pool(disp_adapter_t *adapter,
    pool_handle_t pool);
pool_handle_t disp_vm_create_pool_with_zones(disp_adapter_t *adapter,
    disp_surface_t *surf, int bytealign, int num_zones, int * zone_sizes);
    
/* Color space conversion utilities */
disp_color_t disp_color_translate(disp_color_t *palette,
    unsigned srcformat, unsigned dstformat, disp_color_t color);

/* Miscellaneous utilities */
void disp_usecspin(unsigned usecs);
void disp_delay(unsigned milliseconds);
char *disp_get_rom_image(disp_adapter_t *adapter, int code_type,
   int *size, int map_pci_base_index, int map_offset);

/* X86/BIOS Multi-head support */
int disp_warm_boot(disp_adapter_t *adapter, uint8_t *rom);
int disp_get_primary_vga(disp_adapter_t *adapter, int *bus, int *devfunc);
int disp_primary_active(disp_adapter_t *adapter, int io_active, int mem_active);
int disp_device_active(disp_adapter_t *adapter, int io_active, int mem_active);
int disp_acquire_vga_resources(disp_adapter_t *adapter);
int disp_release_vga_resources(disp_adapter_t *adapter);

void disp_vga_restore_state(disp_adapter_t *ctx, disp_vga_state_t *state);
void disp_vga_save_state(disp_adapter_t *ctx, disp_vga_state_t *state);

__END_DECLS

#endif /* _GRAPHICS_DISPUTIL_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/disputil.h $ $Rev: 657836 $" )
