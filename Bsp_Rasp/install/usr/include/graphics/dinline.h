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



#ifdef _MSC_VER
#define __inline__
#define __attribute__(a)
typedef unsigned short uint16_t;
#endif


/* Optimized inline routines for low-level graphics operations */

#ifndef _GRAPHICS_DINLINE_H_INCLUDED
#define _GRAPHICS_DINLINE_H_INCLUDED

/*
 * The following macros are designed to allow, for example,
 * DISP_MR_SWAP32(ptr++); to work without undesirable side effects.
 */
#ifdef __PPC__
static __inline__ uint32_t __attribute__((__unused__))
disp_mr_swap32(volatile uintptr_t __addr)
{
	unsigned ret;

	__asm__ __volatile__("lwbrx %0,0,%1" :
	    "=r" (ret) : "r" (__addr), "m" (__addr));
	return ret;
}

static __inline__ uint16_t __attribute__((__unused__))
disp_mr_swap16(volatile uintptr_t __addr)
{
	int		ret;

	__asm__ __volatile__("lhbrx %0,0,%1" :
	    "=r" (ret) : "r" (__addr), "m" (__addr));

	return ret;
}

static __inline__ void __attribute__((__unused__))
disp_mw_swap32(volatile uintptr_t __addr, _Uint32t __data)
{
	__asm__ __volatile__("stwbrx %1,0,%2" :
	    "=m" (__addr) : "r" (__data), "r" (__addr));
}

static __inline__ void __attribute__((__unused__))
disp_mw_swap16(volatile uintptr_t __addr, _Uint16t __data)
{
	__asm__ __volatile__("sthbrx %1,0,%2" :
	    "=m" (__addr) : "r" (__data), "r" (__addr));
}

/*
 * Bury these functions inside of macros, to work around a gcc 2.95.3 bug,
 * which doesn't occur if the arg to disp_m* is a uintptr_t, instead of
 * an actual pointer.
 */
#define	DISP_MR_SWAP32(__addr)		disp_mr_swap32((uintptr_t)(__addr))
#define	DISP_MR_SWAP16(__addr)		disp_mr_swap16((uintptr_t)(__addr))
#define	DISP_MW_SWAP32(__addr, __val)	disp_mw_swap32((uintptr_t)(__addr), __val)
#define	DISP_MW_SWAP16(__addr, __val)	disp_mw_swap16((uintptr_t)(__addr), __val)
#else
static __inline__ uint32_t __attribute__((__unused__))
DISP_MR_SWAP32(void *__ptr)
{
	uint32_t	__val = *(volatile uint32_t *)(__ptr);

	return (((__val) >> 24) | (((__val) >> 8) & 0xff00) |
		(((__val) & 0xff00) << 8) | ((__val) << 24));
}

static __inline__ uint16_t __attribute__((__unused__))
DISP_MR_SWAP16(void *__ptr)
{
	uint16_t	__val = *(volatile uint16_t *)(__ptr);

	return (((__val) >> 8) | ((__val) << 8));
}

static __inline__ void __attribute__((__unused__))
DISP_MW_SWAP32(void *__ptr, uint32_t __v)
{
	uint32_t	__val = (__v);

	__val = (__val >> 24 | (__val & 0xff0000) >> 8 |
		(__val & 0xff00) << 8 | __val << 24);

	*(volatile uint32_t *)(__ptr) = __val;
}

static __inline__ void __attribute__((__unused__))
DISP_MW_SWAP16(void *__ptr, uint16_t __v)
{
	uint16_t	__val = (__v);

	__val = (__val >> 8 | __val << 8);

	*(volatile uint16_t *)(__ptr) = __val;
}
#endif

#ifdef __LITTLEENDIAN__
#define DISP_MR_LE32(__ptr)		(*(volatile uint32_t *)(__ptr))
#define DISP_MR_BE32(__ptr)		DISP_MR_SWAP32(__ptr)
#define DISP_MW_LE32(__ptr, __val)	(*(volatile uint32_t *)(__ptr) = (__val))
#define DISP_MW_BE32(__ptr, __val)	DISP_MW_SWAP32(__ptr, __val)
#define DISP_MR_LE16(__ptr)		(*(volatile uint16_t *)(__ptr))
#define DISP_MR_BE16(__ptr)		DISP_MR_SWAP16(__ptr)
#define DISP_MW_LE16(__ptr, __val)	(*(volatile uint16_t *)(__ptr) = (__val))
#define DISP_MW_BE16(__ptr, __val)	DISP_MW_SWAP16(__ptr, __val)
#else
#define DISP_MR_LE32(__ptr)		DISP_MR_SWAP32(__ptr)
#define DISP_MR_BE32(__ptr)		(*(volatile uint32_t *)(__ptr))
#define DISP_MW_LE32(__ptr, __val)	DISP_MW_SWAP32(__ptr, __val)
#define DISP_MW_BE32(__ptr, __val)	(*(volatile uint32_t *)(__ptr) = (__val))
#define DISP_MR_LE16(__ptr)		DISP_MR_SWAP16(__ptr)
#define DISP_MR_BE16(__ptr)		(*(volatile uint16_t *)(__ptr))
#define DISP_MW_LE16(__ptr, __val)	DISP_MW_SWAP16(__ptr, __val)
#define DISP_MW_BE16(__ptr, __val)	(*(volatile uint16_t *)(__ptr) = (__val))
#endif

__BEGIN_DECLS

#define __USE_C_FALLBACKS

#if defined (__GNUC__) || defined(__INTEL_COMPILER)

#if defined (__X86__)

#undef __USE_C_FALLBACKS

static inline void disp_memcpy(void *dst, void *src, int len)
{
	__asm__ __volatile__ (
		".align 4					\n\t"
		"cld						\n\t"
		"cmpl	$8, %%edx				\n\t"
		"jl		0f				\n\t"
	"3:							\n\t"
		".align 4					\n\t"
		"testl	$3, %%edi				\n\t"
		"jz		1f				\n\t"
		"movsb						\n\t"
		"decl	%%edx					\n\t"
		"jz		2f				\n\t"
		"jnz	3b					\n\t"
	"0:							\n\t"
		".align 4					\n\t"
		"movl	%%edx, %%ecx				\n\t"
		"rep						\n\t"
		"movsb						\n\t"
		"jmp	2f					\n\t"
	"1:							\n\t"
		".align 4					\n\t"
		"movl	%%edx, %%ecx				\n\t"
		"shrl	$2, %%ecx				\n\t"
		"rep						\n\t"
		"movsl						\n\t"
	"4:							\n\t"
		".align 4					\n\t"
		"andl	$3, %%edx				\n\t"
		"jz	2f					\n\t"
		"movsb						\n\t"
		"decb	%%dl					\n\t"
		"jmp	4b					\n\t"
	"2:							\n\t"
		".align 4					\n\t"
	: "=D"(dst), "=S"(src), "=d"(len) : "D"(dst), "S"(src), "d"(len) :
	"ecx", "memory");
}

static inline void disp_memcpy_r(void *dst, void *src, int len)
{
	__asm__ __volatile__ (
		"std					\n\t"
		"cmpl	$8, %%edx			\n\t"
		"jge	0f				\n\t"
		"movl 	%%edx, %%ecx			\n\t"
		"rep					\n\t"
		"movsb					\n\t"
		"jmp	1f				\n\t"
	/* Move (edi+1) & 3 bytes with movsb. */
	"0:						\n\t"
		".align 4				\n\t"
		"movl	$3, %%eax			\n\t"
		"movl	%%edi, %%ecx			\n\t"
		"incl	%%ecx				\n\t"
		"andl	%%eax, %%ecx			\n\t"
		"subl	%%ecx, %%edx			\n\t"
		"rep					\n\t"
		"movsb					\n\t"
	/* Adjust edi & esi to point to *start* of nearest long. */
	/* Move (edx / 4) longs with movsd. */
	/* Adjust edi & esi to point back to end of long. */
		"subl	%%eax, %%edi			\n\t"
		"subl	%%eax, %%esi			\n\t"
		"movl	%%edx, %%ecx			\n\t"
		"shrl	$2, %%ecx			\n\t"
		"rep					\n\t"
		"movsl					\n\t"
		"addl	%%eax, %%edi			\n\t"
		"addl	%%eax, %%esi			\n\t"
	/* Move remaining bytes. */
		"movl	%%edx, %%ecx			\n\t"
		"andl	%%eax, %%ecx			\n\t"
		"jz		1f			\n\t"
		"rep					\n\t"
		"movsb					\n\t"
	"1:						\n\t"
		".align 4					\n\t"
		"cld					\n\t"
	: "=D"(dst), "=S"(src), "=d"(len) : "D"(dst), "S"(src), "d"(len) :
	"eax", "ecx", "memory");
}

static inline void disp_memset16(void *dst, uint16_t val, int len)
{
	__asm__ __volatile__ (
		"cmpl	$8, %%edx				\n\t"
		"jle		0f				\n\t"
		"movl	%%eax, %%ecx				\n\t"
		"rorl	$16, %%eax				\n\t"
		"movw	%%cx, %%ax				\n\t"
	"2:							\n\t"
		".align 4					\n\t"
		"testl	$3, %%edi				\n\t"
		"jz		1f				\n\t"
		"stosw						\n\t"
		"decl	%%edx					\n\t"
		"jmp	2b					\n\t"
	"1:							\n\t"
		".align 4					\n\t"
		"movl	%%edx, %%ecx				\n\t"
		"shrl	$1, %%ecx				\n\t"
		"andl	$1, %%edx				\n\t"
		"rep						\n\t"
		"stosl						\n\t"
	"0:							\n\t"
		".align 4					\n\t"
		"testl	%%edx, %%edx				\n\t"
		"jz		3f				\n\t"
		"movl	%%edx, %%ecx				\n\t"
		"rep						\n\t"
		"stosw						\n\t"
	"3:							\n\t"
		".align 4					\n\t"
	: "=D"(dst), "=a"(val), "=d"(len) : "D"(dst), "a"(val), "d"(len) :
	"ecx", "memory");
};

#endif	/* __X86__ */

#elif	defined (__WATCOMC__)

#undef __USE_C_FALLBACKS

void disp_memcpy(void *dst, void *src, int len);
#pragma aux disp_memcpy =						\
"	cmp		edx, 8					",\
"	jl		QByteCopy				",\
"StartTest:							",\
"	test	edi, 3						",\
"	jz		EvenStart				",\
"	movsb							",\
"	dec		edx					",\
"	jz		Done					",\
"	jmp		StartTest				",\
"QByteCopy:							",\
"	mov		ecx, edx				",\
"QByteCopy2:							",\
"	rep 	movsb						",\
"	jmp		Done					",\
"EvenStart:							",\
"	mov		ecx, edx				",\
"	shr		ecx, 2					",\
"DoubleCopy:							",\
"	rep 	movsd						",\
"ByteCopy:							",\
"	and		edx, 3					",\
"	jz		Done					",\
"	movsb							",\
"	dec		dl					",\
"	jmp		ByteCopy				",\
"Done:								"\
	parm nomemory [edi] [esi] [edx]				\
	modify [edi esi ecx edx];

void disp_memcpy_r(void *dst, void *src, int len);
#pragma aux disp_memcpy_r = 					\
"	std							",\
"	cmp		edx, 8					",\
"	jge		StartTest				",\
								\
"	mov		ecx, edx				",\
"	rep 	movsb						",\
"	jmp		Done					",\
 /* move (edi+1) & 3 bytes with movsb */			\
"StartTest:							",\
"	mov		eax, 3					",\
"	mov		ecx,edi					",\
"	inc		ecx					",\
"	and		ecx,eax					",\
"	sub		edx,ecx					",\
"	rep 	movsb						",\
 /* adjust edi & esi to point to */				\
 /* *start* of nearest long */					\
 /* move (edx / 4) longs with movsd */				\
 /* adjust edi & esi to point back to end of long */		\
"	sub		edi, eax				",\
"	sub		esi, eax				",\
"	mov		ecx, edx				",\
"	shr		ecx, 2					",\
"	rep		movsd					",\
"	add		edi, eax				",\
"	add		esi, eax				",\
 /* move remaining bytes */					\
"	mov		ecx, edx				",\
"	and		ecx, eax				",\
"	jz		Done					",\
"	rep 	movsb						",\
"Done:								",\
"	cld							",\
	parm nomemory [edi] [esi] [edx]				\
	modify [edi esi eax ecx edx];

void disp_memset16(void *dst, unsigned short val, int len);
#pragma aux disp_memset16 = 					\
"	cmp		edx, 8					",\
"	jle		ByteCopy				",\
"	mov		ecx, eax				",\
"	ror		eax, 16					",\
"	mov		ax, cx					",\
"StartTest:							",\
"	test		edi, 3					",\
"	jz		EvenStart				",\
"	stosw							",\
"	dec		edx					",\
"	jmp		StartTest				",\
"EvenStart:							",\
"	mov		ecx, edx				",\
"	shr		ecx, 1					",\
"	and		edx, 1					",\
"	rep		stosd					",\
"ByteCopy:							",\
"	test		edx, edx				",\
"	jz		Done					",\
"	mov		ecx, edx				",\
"	rep 		stosw					",\
"Done:								"\
	parm nomemory [edi] [ax] [edx]				\
	modify [edi eax ecx edx];

#endif	/* __WATCOMC__ */

#ifdef __USE_C_FALLBACKS
#define disp_memcpy(dst, src, len)	memcpy(dst, src, len)

extern void disp_memcpy_r(void *dst, void *src, int len);
extern void disp_memset16(void *dst, disp_color_t color, int len);
#endif /* __USE_C_FALLBACKS */

__END_DECLS

#endif /* _GRAPHICS_DINLINE_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/dinline.h $ $Rev: 657836 $" )
