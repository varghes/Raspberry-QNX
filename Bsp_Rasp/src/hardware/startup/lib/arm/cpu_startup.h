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



#include <arm/inout.h>
#include <arm/inline.h>
#include <arm/mmu.h>

/********************************************************************
*********************************************************************

	These definitions are required for the system independent code.

*********************************************************************
********************************************************************/

#define KERCALL_SEQUENCE(name)	uint32_t name[] = {			\
			0xef000000,		/* swi (syscall no. in ip)	*/	\
			0xe7ffffef		/* undefined instruction	*/	\
}

#define CPU_SYSPAGE_TYPE	SYSPAGE_ARM

struct cpu_local_syspage {
	SYSPAGE_SECTION(arm_boxinfo);
	SYSPAGE_SECTION(arm_cpu);
};

#define BOOTSTRAPS_RUN_ONE_TO_ONE	0

#define CPU_COMMON_OPTIONS_STRING	"w:"

/********************************************************************
*********************************************************************

	Everything below is specific to the ARM.

*********************************************************************
********************************************************************/

/*
 * ------------------------------------------------------------------
 * Generic ARM support
 * ------------------------------------------------------------------
 */

extern void				board_cpu_startup(void);
extern void				board_cpu_startnext(void);

extern void				(*vstart)(uintptr_t sysp, unsigned pc, unsigned cpu);
extern void				vstart_v4(uintptr_t sysp, unsigned pc, unsigned cpu);
extern void				vstart_v6(uintptr_t sysp, unsigned pc, unsigned cpu);
extern void				vstart_v7(uintptr_t sysp, unsigned pc, unsigned cpu);

extern unsigned long	arm_cpuspeed();
extern paddr_t			arm_map(uintptr_t va, paddr_t pa, size_t sz, int flags);
extern void				arm_pdmap(uintptr_t va);
extern void				arm_scmap(paddr_t va, paddr_t pa, int flags);
extern void				arm_scunmap(uintptr_t va);
extern void				arm_map_cpu(int cpu, uintptr_t va, paddr_t pa, int flags);

extern unsigned			trap_vectors;
extern unsigned			mmu_cr_set;
extern unsigned			mmu_cr_clr;
extern int				cycles_per_loop;
extern paddr_t			L1_paddr;
extern paddr_t			L1_vaddr;
extern paddr_t			L2_paddr;
extern paddr_t			startup_base;
extern unsigned			startup_size;
extern int				arm_v6_mmu;

#define	ARM_MAP_SYSPAGE	0xffffffff		/* flags to arm_map() for syspage */
#define ARM_MAP_NOEXEC	0x80000000		/* flags to arm_map() for I/O mem */

/*
 * ------------------------------------------------------------------
 * Cache/MMU/CPU configuration information
 * ------------------------------------------------------------------
 */
extern unsigned long	arm_cpuspeed();
/*
 * CPU cache configuration used if there is no CP15 cache type register
 */
struct arm_cache_config {
	unsigned					flags;
	unsigned					line_size;
	unsigned					num_lines;
};

/*
 * CPU cache callout and configuration
 */
struct armv_cache {
	const struct arm_cache_config	*dcache_config;
	const struct callout_rtn		*dcache_rtn;
	const struct arm_cache_config	*icache_config;
	const struct callout_rtn		*icache_rtn;
};

extern void		arm_add_cache(struct cpuinfo_entry *cpu, const struct armv_cache *cache);

extern const struct armv_cache	armv_cache_720;
extern const struct armv_cache	armv_cache_920;
extern const struct armv_cache	armv_cache_925;
extern const struct armv_cache	armv_cache_926;
extern const struct armv_cache	armv_cache_1020;
extern const struct armv_cache	armv_cache_sa1100;
extern const struct armv_cache	armv_cache_xscale;
extern const struct armv_cache	armv_cache_1136;
extern const struct armv_cache	armv_cache_mp11;
extern const struct armv_cache	armv_cache_xsc3;
extern const struct armv_cache	armv_cache_a8;
extern const struct armv_cache	armv_cache_sheeva_v6;
extern const struct armv_cache	armv_cache_sheeva_v7;
extern const struct armv_cache	armv_cache_a9up;
extern const struct armv_cache	armv_cache_a9mp;
extern const struct armv_cache	armv_cache_a15;

/*
 * MMU PTE details
 */
struct armv_pte {
	unsigned short		upte_ro;	// user RO mappings
	unsigned short		upte_rw;	// user RW mappings
	unsigned short		kpte_ro;	// kern RO mappings
	unsigned short		kpte_rw;	// kern RW mappings
	unsigned short		mask_nc;	// bits to clear for uncached mappings
	unsigned short		l1_pgtable;	// bits to set for L1 page table entry
	unsigned			kscn_ro;	// kern RO section mapping
	unsigned			kscn_rw;	// kern RW section mapping
	unsigned			kscn_cb;	// cacheable section mapping
};

extern const struct armv_pte	*armv_pte;
extern void						arm_pte_setup();
extern int						arm_altpte;

extern const struct armv_pte	armv_pte_v4wb;
extern const struct armv_pte	armv_pte_v4wt;
extern const struct armv_pte	armv_pte_v5wa;
extern const struct armv_pte	armv_pte_v5wb;
extern const struct armv_pte	armv_pte_v5wt;
extern const struct armv_pte	armv_pte_ixp2xxx;
extern const struct armv_pte	armv_pte_v6wa;
extern const struct armv_pte	armv_pte_v6wb;
extern const struct armv_pte	armv_pte_v6wt;
extern const struct armv_pte	armv_pte_v6mp;
extern const struct armv_pte	armv_pte_xsc3;
extern const struct armv_pte	armv_pte_v7wa;
extern const struct armv_pte	armv_pte_v7wb;
extern const struct armv_pte	armv_pte_v7wt;
extern const struct armv_pte	armv_pte_v7mp;

/*
 * CPU specific configuration information
 */
struct armv_chip {
	unsigned					cpuid;		// bits 4-15 of CP15 C1
	const char					*name;
	unsigned					mmu_cr_set;	// CP15 C0 values to set
	unsigned					mmu_cr_clr;	// CP15 C0 values to clear
	int							cycles;		// cycles per arm_cpuspeed() loop
	const struct armv_cache		*cache;		// cache configuration
	const struct callout_rtn	*power;		// idle callout
	const struct callout_rtn	*flush;		// page_flush callout
	const struct callout_rtn	*deferred;	// page_flush_deferred callout
	const struct armv_pte		*pte;		// page table entries
	const struct armv_pte		*pte_wa;	// for -wa option
	const struct armv_pte		*pte_wb;	// for -wb option
	const struct armv_pte		*pte_wt;	// for -wt option
	void						(*setup)(struct cpuinfo_entry *cpu, unsigned cpuid);
	const struct armv_chip		*(*detect)(void);
	unsigned short				ttb_attr;
	unsigned short				pte_attr;
};

extern const struct armv_chip	*armv_list[];
extern const struct armv_chip	*armv_chip_detect();
extern const struct armv_chip	*armv_chip;

extern const struct armv_chip	armv_chip_720;
extern const struct armv_chip	armv_chip_920;
extern const struct armv_chip	armv_chip_922;
extern const struct armv_chip	armv_chip_925;
extern const struct armv_chip	armv_chip_926;
extern const struct armv_chip	armv_chip_1020;
extern const struct armv_chip	armv_chip_1022;
extern const struct armv_chip	armv_chip_sa1100;
extern const struct armv_chip	armv_chip_sa1110;
extern const struct armv_chip	armv_chip_ixp1200;
extern const struct armv_chip	armv_chip_80200;
extern const struct armv_chip	armv_chip_pxa250a;
extern const struct armv_chip	armv_chip_pxa250b;
extern const struct armv_chip	armv_chip_pxa255;
extern const struct armv_chip	armv_chip_pxa210b;
extern const struct armv_chip	armv_chip_pxa210c;
extern const struct armv_chip	armv_chip_pxa270;
extern const struct armv_chip	armv_chip_ixp2400;
extern const struct armv_chip	armv_chip_ixp2800;
extern const struct armv_chip	armv_chip_ixp23xx;
extern const struct armv_chip	armv_chip_ixp425;
extern const struct armv_chip	armv_chip_ixp465;
extern const struct armv_chip	armv_chip_mp11;
extern const struct armv_chip	armv_chip_1136;
extern const struct armv_chip	armv_chip_1176;
extern const struct armv_chip	armv_chip_a8;
extern const struct armv_chip	armv_chip_sheeva;
extern const struct armv_chip	armv_chip_sheeva_v6;
extern const struct armv_chip	armv_chip_sheeva_v7;
extern const struct armv_chip	armv_chip_a9;
extern const struct armv_chip	armv_chip_a9up;
extern const struct armv_chip	armv_chip_a9mp;
extern const struct armv_chip	armv_chip_a15;

/*
 * Operations supported for multiple cores
 */

extern struct callout_rtn	power_cp15_wfi;
extern struct callout_rtn	power_v7_wfi;
extern void					armv_setup_v6(struct cpuinfo_entry *, unsigned, unsigned);
extern void					armv_setup_v7(struct cpuinfo_entry *, unsigned, unsigned);

/*
 * ARM Configuration register manipulation
 */

static inline unsigned
arm_v6_cp15_auxcr_get()
{
	unsigned	auxcr;
	__asm__ __volatile__("mrc	p15, 0, %0, c1, c0, 1" : "=r"(auxcr));
	return auxcr;
}

static inline void
arm_v6_cp15_auxcr_set(unsigned val)
{
	__asm__ __volatile__("mcr	p15, 0, %0, c1, c0, 1" : : "r"(val));
}

#define	ARM_V6_CPACR_CP0	(3 << 0)
#define	ARM_V6_CPACR_CP1	(3 << 2)
#define	ARM_V6_CPACR_CP2	(3 << 4)
#define	ARM_V6_CPACR_CP3	(3 << 6)
#define	ARM_V6_CPACR_CP4	(3 << 8)
#define	ARM_V6_CPACR_CP5	(3 << 10)
#define	ARM_V6_CPACR_CP6	(3 << 12)
#define	ARM_V6_CPACR_CP7	(3 << 14)
#define	ARM_V6_CPACR_CP8	(3 << 16)
#define	ARM_V6_CPACR_CP9	(3 << 18)
#define	ARM_V6_CPACR_CP10	(3 << 20)
#define	ARM_V6_CPACR_CP11	(3 << 22)
#define	ARM_V6_CPACR_CP12	(3 << 24)
#define	ARM_V6_CPACR_CP13	(3 << 26)
#define	ARM_V6_CPACR_CP14	(3 << 28)

#define ARM_V6_CPACR_WMMX2	(ARM_V6_CPACR_CP0  | ARM_V6_CPACR_CP1)
#define ARM_V6_CPACR_VFP	(ARM_V6_CPACR_CP10 | ARM_V6_CPACR_CP11)

static inline unsigned
arm_v6_cp15_cpacr_get()
{
	unsigned	cpacr;
	__asm__ __volatile__("mrc	p15, 0, %0, c1, c0, 2" : "=r"(cpacr));
	return cpacr;
}

static inline void
arm_v6_cp15_cpacr_set(unsigned val)
{
	__asm__ __volatile__("mcr	p15, 0, %0, c1, c0, 2" : : "r"(val));
}


/*
 * ------------------------------------------------------------------
 * SA-1100/1110 Processor Support
 * ------------------------------------------------------------------
 */

extern void	init_sa1100(unsigned, const char *, const char *);
extern void	put_sa1100(int);
extern void	init_qtime_sa1100(void);

extern unsigned				sa11x1_base;
extern unsigned				sa1100_grer;
extern unsigned				sa1100_gfer;

extern struct callout_rtn	reboot_sa1100;
extern struct callout_rtn	power_sa1100;

extern struct callout_rtn	interrupt_id_sa1100;
extern struct callout_rtn	interrupt_eoi_sa1100;
extern struct callout_rtn	interrupt_mask_sa1100;
extern struct callout_rtn	interrupt_unmask_sa1100;
extern struct callout_rtn	interrupt_id_sa1100_gpio;
extern struct callout_rtn	interrupt_eoi_sa1100_gpio;
extern struct callout_rtn	interrupt_mask_sa1100_gpio;
extern struct callout_rtn	interrupt_unmask_sa1100_gpio;

extern struct callout_rtn	display_char_sa1100;
extern struct callout_rtn	poll_key_sa1100;
extern struct callout_rtn	break_detect_sa1100;

extern struct callout_rtn	timer_load_sa1100;
extern struct callout_rtn	timer_value_sa1100;
extern struct callout_rtn	timer_reload_sa1100;

extern struct callout_rtn	cache_sa_i;
extern struct callout_rtn	cache_sa_d;

extern struct callout_rtn	page_flush_sa;
extern struct callout_rtn	page_flush_deferred_sa;

extern struct callout_rtn	interrupt_id_sa1101;
extern struct callout_rtn	interrupt_eoi_sa1101;
extern struct callout_rtn	interrupt_mask_sa1101;
extern struct callout_rtn	interrupt_unmask_sa1101;

extern struct callout_rtn	interrupt_id_sa1111;
extern struct callout_rtn	interrupt_eoi_sa1111;
extern struct callout_rtn	interrupt_mask_sa1111;
extern struct callout_rtn	interrupt_unmask_sa1111;

extern unsigned long		rtc_time_sa1100(unsigned);

/*
 * ------------------------------------------------------------------
 * Xscale processor support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	power_xscale;

extern struct callout_rtn	cache_xscale_i;
extern struct callout_rtn	cache_xscale_d;

extern struct callout_rtn	page_flush_xscale;
extern struct callout_rtn	page_flush_deferred_xscale;

extern void					armv_setup_xscale(struct cpuinfo_entry *, unsigned);

extern struct callout_rtn	cache_xsc3_i;
extern struct callout_rtn	cache_xsc3_d;

extern struct callout_rtn	page_flush_xsc3;
extern struct callout_rtn	page_flush_deferred_xsc3;

/*
 * ------------------------------------------------------------------
 * ARM720T processor support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	cache_720;

extern struct callout_rtn	page_flush_720;
extern struct callout_rtn	page_flush_deferred_720;

/*
 * ------------------------------------------------------------------
 * ARM920T processor support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	cache_920_i;
extern struct callout_rtn	cache_920_d;

extern struct callout_rtn	page_flush_920;
extern struct callout_rtn	page_flush_deferred_920;

extern struct callout_rtn	cache_925_i;
extern struct callout_rtn	cache_925_d;

extern struct callout_rtn	page_flush_925;
extern struct callout_rtn	page_flush_deferred_925;

extern struct callout_rtn	power_926;
extern struct callout_rtn	cache_926_i;
extern struct callout_rtn	cache_926_d;

/*
 * ------------------------------------------------------------------
 * ARM1020T processor support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	cache_1020_i;
extern struct callout_rtn	cache_1020_d;

extern struct callout_rtn	page_flush_1020;
extern struct callout_rtn	page_flush_deferred_1020;

/*
 * ------------------------------------------------------------------
 * PrimeCell UART/RTC support
 * ------------------------------------------------------------------
 */
extern void	init_primecell(unsigned, const char *, const char *);
extern void	put_primecell(int);

extern struct callout_rtn	display_char_primecell;
extern struct callout_rtn	poll_key_primecell;
extern struct callout_rtn	break_detect_primecell;

extern unsigned long		rtc_time_primecell(unsigned);

/*
 * ------------------------------------------------------------------
 * PXA250 support
 * ------------------------------------------------------------------
 */

extern void	init_pxa250(unsigned, const char *, const char *);
extern void	put_pxa250(int);
extern void	init_qtime_pxa250(void);

extern struct callout_rtn	display_char_pxa250;
extern struct callout_rtn	poll_key_pxa250;
extern struct callout_rtn	break_detect_pxa250;

extern struct callout_rtn	timer_load_pxa250;
extern struct callout_rtn	timer_value_pxa250;
extern struct callout_rtn	timer_reload_pxa250;

extern struct callout_rtn	interrupt_id_pxa250;
extern struct callout_rtn	interrupt_eoi_pxa250;
extern struct callout_rtn	interrupt_mask_pxa250;
extern struct callout_rtn	interrupt_unmask_pxa250;
extern struct callout_rtn	interrupt_id_pxa250_gpio;
extern struct callout_rtn	interrupt_eoi_pxa250_gpio;
extern struct callout_rtn	interrupt_mask_pxa250_gpio;
extern struct callout_rtn	interrupt_unmask_pxa250_gpio;

extern unsigned long		rtc_time_pxa250(unsigned);

/*
 * ------------------------------------------------------------------
 * PXA270 support
 * ------------------------------------------------------------------
 */

extern void	init_qtime_pxa270(void);

extern struct callout_rtn	interrupt_id_pxa270;
extern struct callout_rtn	interrupt_eoi_pxa270;
extern struct callout_rtn	interrupt_mask_pxa270;
extern struct callout_rtn	interrupt_unmask_pxa270;
extern struct callout_rtn	interrupt_id_pxa270_gpio;
extern struct callout_rtn	interrupt_eoi_pxa270_gpio;
extern struct callout_rtn	interrupt_mask_pxa270_gpio;
extern struct callout_rtn	interrupt_unmask_pxa270_gpio;

/*
 * ------------------------------------------------------------------
 * HY7201 support
 * ------------------------------------------------------------------
 */

extern void	init_qtime_hy7201(void);

extern struct callout_rtn	reboot_hy7201;
extern struct callout_rtn	power_hy7201;

extern struct callout_rtn	timer_load_hy7201;
extern struct callout_rtn	timer_value_hy7201;
extern struct callout_rtn	timer_reload_hy7201;

extern struct callout_rtn	interrupt_id_hy7201;
extern struct callout_rtn	interrupt_eoi_hy7201;
extern struct callout_rtn	interrupt_mask_hy7201;
extern struct callout_rtn	interrupt_unmask_hy7201;
extern struct callout_rtn	interrupt_id_hy7201_gpioa;
extern struct callout_rtn	interrupt_eoi_hy7201_gpioa;
extern struct callout_rtn	interrupt_mask_hy7201_gpioa;
extern struct callout_rtn	interrupt_unmask_hy7201_gpioa;
extern struct callout_rtn	interrupt_id_hy7201_gpiob;
extern struct callout_rtn	interrupt_eoi_hy7201_gpiob;
extern struct callout_rtn	interrupt_mask_hy7201_gpiob;
extern struct callout_rtn	interrupt_unmask_hy7201_gpiob;
extern struct callout_rtn	interrupt_id_hy7201_gpioc;
extern struct callout_rtn	interrupt_eoi_hy7201_gpioc;
extern struct callout_rtn	interrupt_mask_hy7201_gpioc;
extern struct callout_rtn	interrupt_unmask_hy7201_gpioc;
extern struct callout_rtn	interrupt_id_hy7201_gpiod;
extern struct callout_rtn	interrupt_eoi_hy7201_gpiod;
extern struct callout_rtn	interrupt_mask_hy7201_gpiod;
extern struct callout_rtn	interrupt_unmask_hy7201_gpiod;

extern unsigned long		rtc_time_hy7201();

/*
 * ------------------------------------------------------------------
 * IXP1200 support
 * ------------------------------------------------------------------
 */

extern void	init_ixp1200(unsigned, const char *, const char *);
extern void	put_ixp1200(int);
extern void	init_qtime_ixp1200(void);

extern struct callout_rtn	reboot_ixp1200;

extern struct callout_rtn	display_char_ixp1200;
extern struct callout_rtn	poll_key_ixp1200;
extern struct callout_rtn	break_detect_ixp1200;

extern struct callout_rtn	timer_load_ixp1200;
extern struct callout_rtn	timer_value_ixp1200;
extern struct callout_rtn	timer_reload_ixp1200;

extern struct callout_rtn	interrupt_id_ixp1200;
extern struct callout_rtn	interrupt_eoi_ixp1200;
extern struct callout_rtn	interrupt_mask_ixp1200;
extern struct callout_rtn	interrupt_unmask_ixp1200;
extern struct callout_rtn	interrupt_id_ixp1200_pci;
extern struct callout_rtn	interrupt_eoi_ixp1200_pci;
extern struct callout_rtn	interrupt_mask_ixp1200_pci;
extern struct callout_rtn	interrupt_unmask_ixp1200_pci;

/*
 * ------------------------------------------------------------------
 * IXP2400 support
 * ------------------------------------------------------------------
 */

extern void	init_ixp2400(unsigned, const char *, const char *);
extern void	put_ixp2400(int);
extern void	init_qtime_ixp2400(void);
extern void ixp2400_init_raminfo(void);

extern struct callout_rtn	reboot_ixp2400;

extern struct callout_rtn	display_char_ixp2400;
extern struct callout_rtn	poll_key_ixp2400;
extern struct callout_rtn	break_detect_ixp2400;

extern struct callout_rtn	timer_load_ixp2400;
extern struct callout_rtn	timer_value_ixp2400;
extern struct callout_rtn	timer_reload_ixp2400;

extern struct callout_rtn	interrupt_id_ixp2400;
extern struct callout_rtn	interrupt_eoi_ixp2400;
extern struct callout_rtn	interrupt_mask_ixp2400;
extern struct callout_rtn	interrupt_unmask_ixp2400;

extern struct callout_rtn	interrupt_id_ixp2400_pci;
extern struct callout_rtn	interrupt_eoi_ixp2400_pci;
extern struct callout_rtn	interrupt_mask_ixp2400_pci;
extern struct callout_rtn	interrupt_unmask_ixp2400_pci;

/*
 * ------------------------------------------------------------------
 * IXP23XX support
 * ------------------------------------------------------------------
 */

extern void	init_qtime_ixp23xx(void);

extern struct callout_rtn	reboot_ixp23xx;

extern struct callout_rtn	timer_load_ixp23xx;
extern struct callout_rtn	timer_value_ixp23xx;
extern struct callout_rtn	timer_reload_ixp23xx;

extern struct callout_rtn	interrupt_id_ixp23xx;
extern struct callout_rtn	interrupt_eoi_ixp23xx;
extern struct callout_rtn	interrupt_mask_ixp23xx;
extern struct callout_rtn	interrupt_unmask_ixp23xx;

extern struct callout_rtn	interrupt_id_ixdp2351_cpld;
extern struct callout_rtn	interrupt_eoi_ixdp2351_cpld;
extern struct callout_rtn	interrupt_mask_ixdp2351_cpld;
extern struct callout_rtn	interrupt_unmask_ixdp2351_cpld;

/*
 * ------------------------------------------------------------------
 * IXP425 support (also IXC1100)
 * ------------------------------------------------------------------
 */

extern void init_qtime_ixp425(void);


extern struct callout_rtn   interrupt_id_ixp425;
extern struct callout_rtn   interrupt_eoi_ixp425;
extern struct callout_rtn   interrupt_mask_ixp425;
extern struct callout_rtn   interrupt_unmask_ixp425;

extern struct callout_rtn   timer_load_ixp425;
extern struct callout_rtn   timer_value_ixp425;
extern struct callout_rtn   timer_reload_ixp425;

/*
 * ------------------------------------------------------------------
 * IXP465 support (similar to IXP425)
 * ------------------------------------------------------------------
 */
extern struct callout_rtn   interrupt_id_ixp465;
extern struct callout_rtn   interrupt_eoi_ixp465;
extern struct callout_rtn   interrupt_mask_ixp465;
extern struct callout_rtn   interrupt_unmask_ixp465;

/*
 * ------------------------------------------------------------------
 * OMAP support
 * ------------------------------------------------------------------
 */

extern void init_omap(unsigned channel, const char *init, const char *defaults);
extern void put_omap(int);
extern void omap5912_init_raminfo();

extern struct callout_rtn	reboot_omap;

extern struct callout_rtn	interrupt_id_omap_pri;
extern struct callout_rtn	interrupt_eoi_omap_pri;
extern struct callout_rtn	interrupt_mask_omap_pri;
extern struct callout_rtn	interrupt_unmask_omap_pri;

extern struct callout_rtn	interrupt_id_omap_sec;
extern struct callout_rtn	interrupt_eoi_omap_sec;
extern struct callout_rtn	interrupt_mask_omap_sec;
extern struct callout_rtn	interrupt_unmask_omap_sec;

extern struct callout_rtn	interrupt_id_omap_gpio1;
extern struct callout_rtn	interrupt_eoi_omap_gpio1;
extern struct callout_rtn	interrupt_mask_omap_gpio1;
extern struct callout_rtn	interrupt_unmask_omap_gpio1;

extern struct callout_rtn	interrupt_id_omap_mpuio;
extern struct callout_rtn	interrupt_eoi_omap_mpuio;
extern struct callout_rtn	interrupt_mask_omap_mpuio;
extern struct callout_rtn	interrupt_unmask_omap_mpuio;

extern struct callout_rtn	timer_load_omap;
extern struct callout_rtn	timer_value_omap;
extern struct callout_rtn	timer_reload_omap;


/*
 * ------------------------------------------------------------------
 * OMAP2420 support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	reboot_omap2420;

extern struct callout_rtn	interrupt_id_omap2420;
extern struct callout_rtn	interrupt_eoi_omap2420;
extern struct callout_rtn	interrupt_mask_omap2420;
extern struct callout_rtn	interrupt_unmask_omap2420;

extern struct callout_rtn	interrupt_id_omap2420_gpio;
extern struct callout_rtn	interrupt_eoi_omap2420_gpio;
extern struct callout_rtn	interrupt_mask_omap2420_gpio;
extern struct callout_rtn	interrupt_unmask_omap2420_gpio;

extern struct callout_rtn	timer_load_omap2420;
extern struct callout_rtn	timer_value_omap2420;
extern struct callout_rtn	timer_reload_omap2420;

extern void	init_qtime_omap2420(void);

/*
 * ------------------------------------------------------------------
 * Samsung S3C2410 support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn   interrupt_id_s3c2410;
extern struct callout_rtn   interrupt_eoi_s3c2410;
extern struct callout_rtn   interrupt_mask_s3c2410;
extern struct callout_rtn   interrupt_unmask_s3c2410;

extern struct callout_rtn   interrupt_id_s3c2410_u0;
extern struct callout_rtn   interrupt_eoi_s3c2410_u0;
extern struct callout_rtn   interrupt_mask_s3c2410_u0;
extern struct callout_rtn   interrupt_unmask_s3c2410_u0;

extern struct callout_rtn   interrupt_id_s3c2410_u1;
extern struct callout_rtn   interrupt_eoi_s3c2410_u1;
extern struct callout_rtn   interrupt_mask_s3c2410_u1;
extern struct callout_rtn   interrupt_unmask_s3c2410_u1;

extern struct callout_rtn   interrupt_id_s3c2410_u2;
extern struct callout_rtn   interrupt_eoi_s3c2410_u2;
extern struct callout_rtn   interrupt_mask_s3c2410_u2;
extern struct callout_rtn   interrupt_unmask_s3c2410_u2;

extern struct callout_rtn   interrupt_id_s3c2410_adc_tc;
extern struct callout_rtn   interrupt_eoi_s3c2410_adc_tc;
extern struct callout_rtn   interrupt_mask_s3c2410_adc_tc;
extern struct callout_rtn   interrupt_unmask_s3c2410_adc_tc;

extern struct callout_rtn   interrupt_id_s3c2410_ext_4_7;
extern struct callout_rtn   interrupt_eoi_s3c2410_ext_4_7;
extern struct callout_rtn   interrupt_mask_s3c2410_ext_4_7;
extern struct callout_rtn   interrupt_unmask_s3c2410_ext_4_7;

extern struct callout_rtn   interrupt_id_s3c2410_ext_8_23;
extern struct callout_rtn   interrupt_eoi_s3c2410_ext_8_23;
extern struct callout_rtn   interrupt_mask_s3c2410_ext_8_23;
extern struct callout_rtn   interrupt_unmask_s3c2410_ext_8_23;

extern struct callout_rtn   timer_load_s3c2410;
extern struct callout_rtn   timer_value_s3c2410;
extern struct callout_rtn   timer_reload_s3c2410;

/*
 * ------------------------------------------------------------------
 * ARM11 MPcore support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	page_flush_mp11;
extern struct callout_rtn	page_flush_deferred_mp11;

extern struct callout_rtn	cache_mp11_i;
extern struct callout_rtn	cache_mp11_d;

extern struct callout_rtn	interrupt_id_mpcore;
extern struct callout_rtn	interrupt_eoi_mpcore;
extern struct callout_rtn	interrupt_mask_mpcore;
extern struct callout_rtn	interrupt_unmask_mpcore;
extern struct callout_rtn	interrupt_config_mpcore;

extern struct callout_rtn	sendipi_mpcore;

extern void					armv_setup_mp11(struct cpuinfo_entry *, unsigned);

extern unsigned				mpcore_scu_base;

/*
 * ------------------------------------------------------------------
 * ARM1136/1176 support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	page_flush_1136;
extern struct callout_rtn	page_flush_deferred_1136;

extern struct callout_rtn	cache_1136_i;
extern struct callout_rtn	cache_1136_d;

extern void					armv_setup_1136(struct cpuinfo_entry *, unsigned);
extern void					armv_setup_1176(struct cpuinfo_entry *, unsigned);

/*
 * ------------------------------------------------------------------
 * PrimeCell PL011 UART support
 * ------------------------------------------------------------------
 */

extern void init_pl011(unsigned, const char *, const char *);
extern void put_pl011(int);

extern struct callout_rtn	display_char_pl011;
extern struct callout_rtn	poll_key_pl011;
extern struct callout_rtn	break_detect_pl011;

/*
 * ------------------------------------------------------------------
 * MC9328MX1 support
 * ------------------------------------------------------------------
 */

extern void init_mx1(unsigned channel, const char *init, const char *defaults);
extern void put_mx1(int);

extern struct callout_rtn	display_char_mx1;
extern struct callout_rtn	poll_key_mx1;
extern struct callout_rtn	break_detect_mx1;

extern struct callout_rtn	interrupt_id_aitc;
extern struct callout_rtn	interrupt_eoi_aitc;
extern struct callout_rtn	interrupt_mask_aitc;
extern struct callout_rtn	interrupt_unmask_aitc;

/*
 * ------------------------------------------------------------------
 * Fujitsu MB8X System-On-Chip support
 * ------------------------------------------------------------------
 */

extern struct callout_rtn	reboot_mb8x;
 
extern struct callout_rtn	interrupt_id_mb8x_irc;
extern struct callout_rtn	interrupt_eoi_mb8x_irc;
extern struct callout_rtn	interrupt_mask_mb8x_irc;
extern struct callout_rtn	interrupt_unmask_mb8x_irc;

extern struct callout_rtn	interrupt_id_mb8x_eirc;
extern struct callout_rtn	interrupt_eoi_mb8x_eirc_src_0;
extern struct callout_rtn	interrupt_eoi_mb8x_eirc_src_1;
extern struct callout_rtn	interrupt_eoi_mb8x_eirc_src_2;
extern struct callout_rtn	interrupt_eoi_mb8x_eirc_src_3;
extern struct callout_rtn	interrupt_mask_mb8x_eirc;
extern struct callout_rtn	interrupt_unmask_mb8x_eirc;

extern struct callout_rtn	timer_load_mb8x;
extern struct callout_rtn	timer_value_mb8x;
extern struct callout_rtn	timer_reload_mb8x;

/*
 * ------------------------------------------------------------------
 * Atmel AT92SAM9263 System-On-Chip support
 * ------------------------------------------------------------------
 */
void 						init_at91sam9263(unsigned, const char *, const char *);
void 						put_at91sam9263(int);

extern struct callout_rtn	display_char_at91sam9263;
extern struct callout_rtn	poll_key_at91sam9263;
extern struct callout_rtn	break_detect_at91sam9263;

extern struct callout_rtn	interrupt_id_at91sam9263_aic;
extern struct callout_rtn	interrupt_eoi_at91sam9263_aic;
extern struct callout_rtn	interrupt_mask_at91sam9263_aic;
extern struct callout_rtn	interrupt_unmask_at91sam9263_aic;

extern struct callout_rtn	timer_load_at91sam9263;
extern struct callout_rtn	timer_value_at91sam9263;
extern struct callout_rtn	timer_reload_at91sam9263;

/*
 * ------------------------------------------------------------------
 * i.MX31 support
 * ------------------------------------------------------------------
 */
extern void					init_cacheattr_mx3x(int);
extern struct callout_rtn	cache_imx3x_l2;

/*
 * ------------------------------------------------------------------
 * Cortex A8 support
 * ------------------------------------------------------------------
 */
extern struct callout_rtn	page_flush_a8;
extern struct callout_rtn	page_flush_deferred_a8;

extern struct callout_rtn	cache_a8_i;
extern struct callout_rtn	cache_a8_d;

extern void					armv_setup_a8(struct cpuinfo_entry *, unsigned);

/*
 * ------------------------------------------------------------------
 * Marvell Sheeva support
 * ------------------------------------------------------------------
 */
extern const struct armv_chip	*armv_detect_sheeva(void);

extern struct callout_rtn	page_flush_sheeva_v6;
extern struct callout_rtn	page_flush_deferred_sheeva_v6;
extern struct callout_rtn	cache_sheeva_i_v6;
extern struct callout_rtn	cache_sheeva_d_v6;
extern struct callout_rtn	power_sheeva_v6;
extern void					armv_setup_sheeva_v6(struct cpuinfo_entry *, unsigned);

extern struct callout_rtn	page_flush_sheeva_v7;
extern struct callout_rtn	page_flush_deferred_sheeva_v7;
extern struct callout_rtn	cache_sheeva_i_v7;
extern struct callout_rtn	cache_sheeva_d_v7;
extern struct callout_rtn	power_sheeva_v7;
extern void					armv_setup_sheeva_v7(struct cpuinfo_entry *, unsigned);

extern void	armv_sheeva_wmmx_detect(struct cpuinfo_entry *, unsigned);
extern void	armv_sheeva_errata_check(unsigned);

/*
 * ------------------------------------------------------------------
 * Cortex A9 support
 * ------------------------------------------------------------------
 */
extern const struct armv_chip	*armv_detect_a9(void);

extern struct callout_rtn	page_flush_a9up;
extern struct callout_rtn	page_flush_deferred_a9up;
extern struct callout_rtn	cache_a9up_i;
extern struct callout_rtn	cache_a9up_d;
extern void					armv_setup_a9up(struct cpuinfo_entry *, unsigned);

extern struct callout_rtn	page_flush_a9mp;
extern struct callout_rtn	page_flush_deferred_a9mp;
extern struct callout_rtn	cache_a9mp_i;
extern struct callout_rtn	cache_a9mp_d;
extern void					armv_setup_a9mp(struct cpuinfo_entry *, unsigned);

extern struct callout_rtn	cache_pl310;
extern struct callout_rtn	smp_spin_pl310;

/*
 * ------------------------------------------------------------------
 * Cortex A15 support
 * ------------------------------------------------------------------
 */
extern const struct armv_chip	*armv_detect_a15(void);

extern struct callout_rtn	page_flush_a15;
extern struct callout_rtn	page_flush_deferred_a15;
extern struct callout_rtn	cache_a15_i;
extern struct callout_rtn	cache_a15_d;
extern void					armv_setup_a15(struct cpuinfo_entry *, unsigned);

/*
 * ------------------------------------------------------------------
 * ARM Generic Interrupt Controller Support
 * ------------------------------------------------------------------
 */
extern struct callout_rtn	interrupt_id_gic;
extern struct callout_rtn	interrupt_eoi_gic;
extern struct callout_rtn	interrupt_mask_gic;
extern struct callout_rtn	interrupt_unmask_gic;
extern struct callout_rtn	interrupt_config_gic;

extern struct callout_rtn	sendipi_gic;

extern paddr_t				gic_dist_base;
extern paddr_t				gic_cpu_base;
extern void					arm_gic_init(paddr_t, paddr_t);
extern void					arm_gic_cpu_init(void);

/*
 * ------------------------------------------------------------------
 * Dummy debug callout support
 * ------------------------------------------------------------------
 */
void 						init_null(unsigned, const char *, const char *);
void 						put_null(int);
extern struct callout_rtn	display_char_null;
extern struct callout_rtn	poll_key_null;
extern struct callout_rtn	break_detect_null;
 
/*
 * ------------------------------------------------------------------
 * Primecell SP804 timer support
 * ------------------------------------------------------------------
 */
extern struct callout_rtn	timer_load_sp804;
extern struct callout_rtn	timer_value_sp804;
extern struct callout_rtn	timer_reload_sp804;

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/startup/lib/arm/cpu_startup.h $ $Rev: 657836 $" )
