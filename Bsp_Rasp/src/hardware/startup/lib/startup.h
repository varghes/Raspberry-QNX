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






//
// Other includes which most everyone needs
//

#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <gulliver.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/image.h>
#include <sys/startup.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <sys/elf.h>
#include <hw/sysinfo.h>
#include <confname.h>


#define PROCESSORS_MAX	32	// set this to match kernel (at least a search will find this now)

/* this ASSERT is intentionally meant to not be conditionally implemented */
#define ASSERT(_c_) \
		do { \
			if (!(_c_)) \
			{ \
				const char *s = strrchr(__FILE__, '/'); \
				crash("%s:%d -- ASSERT(%s) failed!\n", (s != NULL) ? s + 1 : __FILE__, __LINE__, #_c_); \
			} \
		} while(0)

#define SYSPAGE_TYPED_SECTION(type, field)	\
		struct field##_section {			\
			type			*p;				\
			unsigned		size;			\
		}						field

#define SYSPAGE_SECTION(field)	SYSPAGE_TYPED_SECTION(struct field##_entry, field)

struct callout_rtn {
	unsigned	*rw_size;
	void		(*patcher)(paddr32_t paddr, uintptr_t vaddr, unsigned rtn_offset, unsigned rw_offset, void *data, const struct callout_rtn *src);
	unsigned	rtn_size;
	uint8_t		rtn_code[1];
};

struct callout_slot {
	unsigned					offset;
	const struct callout_rtn	*callout;
};

#define CALLOUT_SLOT( name, suffix ) \
	offsetof( struct callout_entry, name ), &name##suffix

#include "cpu_startup.h"
#include "restore_ifs.h"

#if !defined(MAKE_1TO1_PTR)
	#define MAKE_1TO1_PTR(p)	((void *)((p) + shdr->paddr_bias))
#endif

#ifndef __PAGESIZE
  #define __PAGESIZE 4096
#endif

#ifndef CPU_VIRT_PAGESIZE
	#define CPU_VIRT_PAGESIZE __PAGESIZE
#endif

#ifndef CPU_PHYS_PAGESIZE
	#define CPU_PHYS_PAGESIZE 16
#endif

//
// Defines
//
#define NULL_PADDR		(~(paddr_t)0)
#define NULL_PADDR32	(~(paddr32_t)0)
#define MAX_PADDR32     (0xffffffff)

#define TRUNC(_x,_a)  ((paddr_t)(_x) &~ (((paddr_t)(_a))-1))
#define ROUND(_x,_a)  TRUNC(((paddr_t)(_x)) + ((paddr_t)(_a)-1), (paddr_t)(_a))

#define TRUNCPG(_x)		TRUNC((_x), __PAGESIZE)
#define ROUNDPG(_x)		ROUND((_x), __PAGESIZE)

#define KILO(k)			((k) * 1024UL)
#define MEG(m)			((m) * (1024UL * 1024UL))
#define GIG(m)			((m) * (1024UL * 1024UL * 1024UL))

#define TOPTR(x)		((void *)(x))

#define PTR_DIFF(a,b)	((uint8_t *)(a) - (uint8_t *)(b))

#define NUM_ELTS(__array)	(sizeof(__array)/sizeof(__array[0]))

#define COMMON_OPTIONS_STRING   CPU_COMMON_OPTIONS_STRING "AD:F:f:I:i:K:M:N:o:P:R:S:Tvr:j:Z"

struct local_syspage {
	SYSPAGE_SECTION(syspage);
	SYSPAGE_SECTION(system_private);
	SYSPAGE_SECTION(meminfo);
	SYSPAGE_SECTION(asinfo);
	SYSPAGE_SECTION(hwinfo);
	SYSPAGE_SECTION(cpuinfo);
	SYSPAGE_SECTION(cacheattr);
	SYSPAGE_SECTION(qtime);
	SYSPAGE_SECTION(callout);
	SYSPAGE_SECTION(callin);
	SYSPAGE_SECTION(typed_strings);
	SYSPAGE_SECTION(strings);
	SYSPAGE_SECTION(intrinfo);
	SYSPAGE_SECTION(smp);
	SYSPAGE_SECTION(pminfo);
	SYSPAGE_SECTION(mdriver);
	struct cpu_local_syspage	cpu;
};

struct debug_device {
	const char 					*name;
	const char					*defaults[2];
	void						(*init)(unsigned, const char *, const char *);
	void						(*put)(int);
	const struct callout_rtn	*callouts[3];
};

/*
 *NOTE: The first part has to match one-to-one with struct intrinfo_entry
 *      on the syspage (aside from the type of the function pointers changing).
 */
struct startup_intrgen {
	uint16_t			genflags;
	uint16_t			size;
	struct callout_rtn	*rtn;
};

struct startup_intrinfo {
	uint32_t				vector_base;
	uint32_t				num_vectors;
	uint32_t				cascade_vector;
	uint32_t				cpu_intr_base;
	uint16_t				cpu_intr_stride;
	uint16_t				flags;
	struct startup_intrgen	id;
	struct startup_intrgen	eoi;
	struct callout_rtn		*mask;
	struct callout_rtn		*unmask;
	struct callout_rtn		*config;
	/* End of struct intrinfo_entry match */
	void					*patch_data;
};

typedef void	(*output_callout_t(int))(int);

typedef struct {
	paddr_t		base;
	unsigned	shift;
} chip_info;


#ifdef __STRICT_PROTYPES__
/* 
 * This really isn't correct, since there is no 'one' signature for a callout fp,
 * but this silences the compiler's complaints for those who really must compile with
 * -fstrict-prototypes
 */
typedef void (*callout_fp_t)(void);
#else
typedef void (*callout_fp_t)();
#endif
typedef unsigned (cache_rtn)(paddr32_t, unsigned, int, struct cacheattr_entry *, volatile struct syspage_entry *);

//
// Function prototypes
//

//
//These are board specific routines that need to be implemented. There
//may be more syspage sections that need to be implemented for a
//particular CPU. Check the appropriate cpu_startup.h. There may be
//default implementations of these routines in the startup library that
//will work for a particular board, or set of boards.
//
void init_raminfo(void);
void init_intrinfo(void);
void init_qtime(void);
void init_cacheattr(void);
void init_cpuinfo(void);
void init_hwinfo(void);
void init_asinfo(unsigned mem);
void init_nanospin(void);

// Only needed for SMP systems
void init_smp(void);
unsigned board_smp_num_cpu(void);
void board_smp_init(struct smp_entry *, unsigned num_cpus);
int board_smp_start(unsigned cpu, void (*start)(void));
unsigned board_smp_adjust_num(unsigned);
extern void	(*smp_spin_vaddr)(void);

// Only needed on systems with an MMU.
void init_mmu(void);

//
//These are CPU specific routines that are implemented by the startup library.
//Users shouldn't have to call these directly.
//
void cpu_startup(void);
void cpu_one_startup(int __cpu);
void cpu_init_syspage_memory(void);	
struct syspage_entry *cpu_alloc_syspage_memory(paddr32_t *, paddr32_t *, unsigned);
void cpu_write_syspage_memory(paddr32_t, unsigned, unsigned);
void cpu_print_callout(struct callout_entry *call); 
void cpu_startnext(uintptr_t eip,unsigned cpu);
void elf_map(uintptr_t vaddr, paddr32_t paddr,size_t size,int flags);
paddr_t elf_vaddr_to_paddr(uintptr_t vaddr);
uintptr_t load_elf32(paddr32_t addr);
void startnext(void);

extern struct callout_rtn	cache_dummy;

//
// These are CPU/board independent routines that can be used 'as is'.
//
void init_system_private(void);
void load_ifs(paddr32_t);
struct pminfo_entry *init_pminfo(unsigned managed_size);

void copy_memory(paddr32_t dst, paddr32_t src, size_t len);

paddr_t strtopaddr(const char *nptr, char **endptr, int base);
unsigned calc_cksum(const void *start, unsigned nbytes);
void chip_access(paddr_t base, unsigned reg_shift, unsigned mem_mapped, unsigned size);
unsigned chip_read8(unsigned off);
void chip_write8(unsigned off, unsigned val);
void chip_write16(unsigned off, unsigned val) ;
void chip_write32(unsigned off, unsigned val) ;
unsigned chip_read16(unsigned off);
unsigned chip_read32(unsigned off);
uint64_t chip_read64(unsigned off);
void chip_write64(unsigned off, uint64_t val);
void chip_done(void);


void add_callout_array(const struct callout_slot *slots, unsigned size);
void * add_callout(unsigned offset, const struct callout_rtn *callout);
void callout_output_one(int sizing, void *rp);
unsigned output_callouts(int sizing);
void callout_register_data(void *rp, void *data);
void callout_reloc_data(void *base, unsigned len, int diff);

unsigned long timer_ns2tick(unsigned long ns);
unsigned long timer_tick2ns(unsigned long ticks);
unsigned long calc_time_t(const struct tm *tm);
unsigned bcd2bin(unsigned a);
unsigned long rtc_time(void);
unsigned long rtc_time_none(void);
unsigned long rtc_time_mc146818(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_ds1386(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_ds15x1(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_ds1743(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_rtc72423(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_m48t5x(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_m48t37(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);
unsigned long rtc_time_dm816x(paddr_t base, unsigned reg_shift, int mmap, int cent_reg);

void invert_timer_freq(struct qtime_entry *qtime, unsigned freq);
struct qtime_entry * alloc_qtime(void);

void		*ws_alloc(size_t);
void 		add_sysram(void);
void		avoid_ram(paddr32_t, size_t size);
void 		add_ram(paddr_t start, paddr_t size);
void		watch_add_ram(void (*)(paddr_t, paddr_t));
paddr_t		alloc_ram(paddr_t addr, paddr_t size, unsigned align);
paddr_t		find_top_ram(paddr_t size);
paddr_t		find_top_ram_aligned(paddr_t size, unsigned align,
                                     unsigned pagesize);
paddr_t     find_ram_in_range(paddr_t range_start, paddr_t range_end, size_t size,
                              unsigned align, unsigned colour, unsigned mask);
paddr32_t	find_ram(size_t size, unsigned align, unsigned colour, unsigned mask);
paddr32_t	calloc_ram(size_t size, unsigned align);
void            reserve_ram(unsigned size, unsigned align, unsigned pagesize);

int getopt(int argc,char **argv,char *opts);
int cpu_handle_common_option(int opt);
void handle_common_option(int opt);
paddr_t getsize(char *, char **);

void tweak_cmdline(struct bootargs_entry *bap, const char *);
int bootstrap_arg_adjust(struct bootargs_entry *bap, const char *old, const char *new);
int bootstrap_env_adjust(struct bootargs_entry *bap, const char *old, const char *new);

struct tm *_gmtime(const time_t *timer,struct tm *tmbuf);

void kprintf(const char *fmt,... );
void ksprintf(char *buff, const char *fmt,... );
void crash(const char *fmt,... );
void set_print_char(void (*)(int));
void (*get_print_char(void))(int);
void set_debug(unsigned channel, const struct debug_device *dev, const char *options);
void select_debug(const struct debug_device *dev, unsigned size);
void print_syspage(void);

char *ultoa_end(unsigned long int v, char *d, int radix);

void init_syspage_memory(void *, unsigned max_size);
void *grow_syspage_section(void *, unsigned size);
void *set_syspage_section(void *, unsigned size);
void reloc_syspage_memory(void *, unsigned max_size);
void alloc_syspage_memory(void);
void write_syspage_memory(void);

void jtag_store_syspage_addr(void);
void jtag_reserve_syspage_addr(paddr_t jtag_syspage_addr);
void jtag_reserve_memory(paddr_t resmem_addr, size_t resmem_size, uint8_t resmem_flag);

paddr_t	acpi_find_table(unsigned table_name, unsigned *lenp);
void 	*board_find_acpi_rsdp(void);

const void *find_startup_info(const void *start, unsigned type);
int find_typed_string(int type_index);
int del_typed_string(int type_index);
unsigned add_typed_string(int type_index, const char *name);
unsigned add_string(const char *name);
int add_cache(int next, unsigned flags, unsigned line_size, unsigned num_lines, const struct callout_rtn *rtn);
int add_cache_ways(int next, unsigned flags, unsigned line_size, unsigned num_lines, unsigned ways, const struct callout_rtn *rtn);

struct intrinfo_entry *add_interrupt(const struct startup_intrinfo *);
void add_interrupt_array(const struct startup_intrinfo *intrs, unsigned size);
int interrupt_valid(int intr);

#define HWI_TAG_INFO(type)				\
			HWI_TAG_NAME_##type,		\
			sizeof(struct hwi_##type),	\
			HWI_TAG_ALIGN_##type

/* prototypes for generic hwi routines. Note there are specific bus and device
 * add functions defined in startup/lib/public/sys/hwinfo.h */
void		hwi_default(void);
void		*hwi_alloc_tag(const char *tagname, unsigned size, unsigned align);
void		*hwi_alloc_item(const char *tagname, unsigned size, unsigned align, const char *itemname, unsigned owner);
void		*hwi_grow_tag(unsigned offset, unsigned size);

unsigned	hwi_add_synonym(unsigned hwi_off, const char *synonym);
unsigned	hwi_find_as(paddr_t base, int mmap);

/* prototypes for adding buses and devices */
unsigned	hwibus_add(const char *bus_name, unsigned hwi_off);
unsigned	hwidev_add(const char *device_name, unsigned pnp, unsigned bus_hwi_off);

/* prototypes for adding tags */
void		hwitag_add_common(unsigned hwi_off, void *attr);
void 		hwitag_add_location(unsigned hwi_off, paddr_t base, paddr_t len, unsigned reg_shift, unsigned addr_space);
void		hwitag_add_busattr(unsigned hwi_off, struct hwi_busattr *attr);
void 		hwitag_add_irq(unsigned hwi_off, unsigned vector);
unsigned	hwitag_add_irq_mapping(unsigned hwi_off, unsigned default_irq);
void		hwitag_add_specific_irq_mapping(unsigned hwi_off, unsigned offset, unsigned devfunc, int adjust);
void		hwitag_add_irqrange(unsigned hwi_off, unsigned start_vector, unsigned num);
void 		hwitag_add_inputclk(unsigned hwi_off, unsigned clk, unsigned div);
void 		hwitag_add_nicaddr(unsigned hwi_off, const _uint8 *addr, unsigned len);
void 		hwitag_add_nicphyaddr(unsigned hwi_off, const _uint32 addr);
void		hwitag_add_regname(unsigned hwi_off, const char *name, unsigned regoffset);

/* prototypes for setting specific tag values */
int			hwitag_set_nicaddr(unsigned hwi_off, unsigned nic_idx, uint8_t *nic_addr);
int			hwitag_set_phyaddr(unsigned hwi_off, unsigned phy_idx, uint32_t phy_addr);
int			hwitag_set_ivec(unsigned hwi_off, unsigned ivec_idx, uint32_t vector);
int			hwitag_set_inputclk(unsigned hwi_off, unsigned clksrc_idx, struct hwi_inputclk *clk);
int			hwitag_set_busattr(unsigned hwi_off, unsigned busattr_idx, struct hwi_busattr *busattr);
int			hwitag_set_ivecrange(unsigned hwi_off, unsigned ivecrange_idx, unsigned start_vector, unsigned num);
int			hwitag_set_avail_ivec(unsigned hwi_off, int ivec_idx, uint32_t vector);
int			hwitag_set_errata(unsigned hwi_off, unsigned errata_idx, uint32_t errata);

/* prototypes for deprecated hwi functions. Kept for backwards compat until all usage is removed */
void		hwi_add_device(const char *bus, const char *class, const char *name, unsigned pnp);
void 		hwi_add_location(paddr_t base, paddr_t len, unsigned reg_shift, unsigned addr_space);
void 		hwi_add_irq(unsigned vector);
void 		hwi_add_nicaddr(const _uint8 *addr, unsigned len);
void 		hwi_add_nicphyaddr(const _uint32 addr);
unsigned	hwi_add_irq_mapping(unsigned default_irq);
void		hwi_add_specific_irq_mapping(unsigned offset, unsigned devfunc, int adjust);
void 		hwi_add_inputclk(unsigned clk, unsigned div);
void		hwi_add_rtc(const char *name, paddr_t base, unsigned reg_shift, unsigned len, int mmap, int cent_reg);
void		hwi_add_nanospin(unsigned long hundred_loop_time, unsigned long overhead);

#define AS_ATTR_NONE	0
#define AS_ATTR_RAM		(AS_ATTR_READABLE|AS_ATTR_WRITABLE|AS_ATTR_CACHABLE)
#define AS_ATTR_ROM		(AS_ATTR_READABLE|AS_ATTR_CACHABLE)
#define AS_ATTR_DEV		(AS_ATTR_READABLE|AS_ATTR_WRITABLE)
#define AS_OVERLAY_IO	0x80000000

unsigned			as_default(void);
unsigned			as_add(paddr_t start, paddr_t end, unsigned attr, const char *name, unsigned owner);
unsigned			as_add64(paddr64_t start, paddr64_t end, unsigned attr, const char *name, unsigned owner);
unsigned			as_add_containing(paddr_t start, paddr_t end, unsigned attr, const char *name, const char *container);
void				as_set_priority(unsigned as_off, unsigned priority);
void				as_set_checker(unsigned off, const struct callout_rtn *rtn);
struct asinfo_entry	*as_off2info(unsigned offset);
unsigned			as_info2off(const struct asinfo_entry *);
unsigned			as_find(unsigned start, ...);
unsigned			as_find_containing(unsigned off, paddr_t start, paddr_t end, const char *container);

extern unsigned		mdriver_max;
extern void			(*mdriver_check)(void);
extern void			(*mdriver_hook)(void);
int 				mdriver_add(char *name, int intr, int (*handler)(int state, void *data), paddr32_t data_paddr, unsigned data_size);


void uncompress(int type, paddr32_t dst, paddr32_t src);
void uncompress_zlib(uint8_t *dst, int *dstlen, uint8_t *src, int srclen, uint8_t *win);
void uncompress_lzo(uint8_t *dst, uint8_t *src);
void uncompress_ucl(uint8_t *dst, uint8_t *src);

void tulip_reset(paddr_t, int);
void pcnet_reset(paddr_t, int);
void amd8111_reset(paddr_t, int);

#define PROT_READ       0x00000100  /* pages can be read      */
#define PROT_WRITE      0x00000200  /* pages can be written   */
#define PROT_EXEC       0x00000400  /* pages can be executed  */
#define PROT_NOCACHE    0x00000800  /* pages are not cached   */
#define PROT_USER	    0x00001000	/* pages are accessable to user */
#define PROT_CALLOUT	0x80000000	/* internal flag */

uintptr_t	startup_io_map(unsigned size, paddr_t phys);
void		startup_io_unmap(uintptr_t port);
void		*startup_memory_map(unsigned size, paddr_t phys, unsigned prot_flags);
void		startup_memory_unmap(void *);
uintptr_t	callout_io_map(unsigned size, paddr_t phys);
void		*callout_memory_map(unsigned size, paddr_t phys, unsigned prot_flags);
uintptr_t	callout_io_map_indirect(unsigned size, paddr_t *phys);
void		*callout_memory_map_indirect(unsigned size, paddr_t *phys, unsigned prot_flags);

#define MISC_FLAG_SUPPRESS_BOOTTIME		0x0001

//
// Variable prototypes
//
extern unsigned						paddr_bits;
extern paddr32_t					syspage_paddr;
extern int 							debug_flag;
extern chip_info					dbg_device[2];
extern chip_info					timer_chip;
extern unsigned						patch_channel;
extern struct local_syspage			lsp;
extern unsigned long				cpu_freq;
extern unsigned long				cycles_freq;
extern unsigned long				timer_freq;
extern unsigned 					max_cpus;
extern unsigned						reserved_size;
extern unsigned						reserved_align;
extern unsigned						system_icache_idx;
extern unsigned						system_dcache_idx;
extern struct startup_header		*shdr;
extern int							_argc;
extern char							**_argv;
extern char 						*optarg;
extern int 							optind;
extern unsigned 					(*timer_start)(void);
extern unsigned 					(*timer_diff)(unsigned start);
extern void							(*smp_hook_rtn)(void);
extern output_callout_t				*callout_output_rtn;
extern unsigned						misc_flags;
extern uintptr_t 					boot_vaddr_base;
extern uintptr_t 					boot_vaddr_end;

//
// Debugging I/O routines available for many systems
//

/* if the board does not have a UART available for use as a console, use these
 * "dummy" callouts and debug device routines */
void						init_dummy(unsigned channel, const char *init, const char *defaults);
void						put_dummy(int c);
extern struct callout_rtn	display_char_dummy;
extern struct callout_rtn	poll_key_dummy;
extern struct callout_rtn	break_detect_dummy;

void 						init_8250(unsigned, const char *, const char *);
void 						put_8250(int);
extern struct callout_rtn	display_char_8250;
extern struct callout_rtn	poll_key_8250;
extern struct callout_rtn	break_detect_8250;

void 						init_zscc(unsigned, const char *, const char *);
void 						put_zscc(int);
extern struct callout_rtn	display_char_zscc;
extern struct callout_rtn	poll_key_zscc;
extern struct callout_rtn	break_detect_zscc;

void 						init_2681(unsigned, const char *, const char *);
void 						put_2681(int);
extern struct callout_rtn	poll_key_2681;
extern struct callout_rtn	display_char_2681;
extern struct callout_rtn	break_detect_2681;

extern struct callout_rtn	debug_watchdog_e500;

void 						dummy_print_char(int);

void 						crash_done(void);
void						ap_fail(int cpu);

void						board_init(void);

//
// Generic PCI routines
//
uint16_t pci_read_cfg16(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset);
uint8_t pci_read_cfg8(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset);
void pci_write_cfg16(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset, uint16_t value);
void pci_write_cfg8(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset, uint8_t value);

//
// CPU specific PCI routines. Note that 8 and 16 bit variants are generic and
// implemented in terms of these 32 bit variants which are architecture
// specific.
//
uint32_t pci_read_cfg32(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset);
void pci_write_cfg32(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset, uint32_t value);

void init_ringbuffers(const char *name, paddr_t ringbuffer_addr, size_t ringbuffer_size);

/* Temp definition until all the header files are caught up 2008/08/20 */
#ifndef CACHE_FLAG_VIRT_IDX
	#define CACHE_FLAG_VIRT_IDX 0x800
#endif

/* __SRCVERSION("startup.h $Rev: 657836 $"); */
