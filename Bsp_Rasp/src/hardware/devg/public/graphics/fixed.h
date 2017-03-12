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




#ifndef _GRAPHICS_FIXED_H_INCLUDED
#define _GRAPHICS_FIXED_H_INCLUDED

#include <stdlib.h>

/*
 * Fixed point types and macros.  Fixed point 3D drivers should define
 * DISP_FIXED_POINT before including this file.
 */
#ifdef DISP_FIXED_POINT
typedef int			disp_real;
#define DISP_REALFX(__r)	(__r)
#else
typedef float			disp_real;
#define DISP_REALFX(__r)	((int)((__r)*0x10000))
#endif

#define DISP_FXHALF		0x8000

#ifdef DISP_FIXED_POINT
#define DISP_REALHALF		DISP_FXHALF
#else
#define DISP_REALHALF		0.5
#endif

#define DISP_INTFX(__i)		((__i)<<16)

#define DISP_FXMUL(__a, __b) \
	((int)(((int64_t)(__a)*(int64_t)(__b)) >> 16))

/* Multiply two real numbers */
#ifdef DISP_FIXED_POINT
#define DISP_REALMUL(__r1, __r2)	DISP_FXMUL((__r1), (__r2))
#else
#define DISP_REALMUL(__r1, __r2)	((__r1) * (__r2))
#endif

/* Divide one real number by another */
#ifdef DISP_FIXED_POINT
#define DISP_REALDIV(__r1, __r2)	DISP_FXDIV((__r1), (__r2))
#else
#define DISP_REALDIV(__r1, __r2)	((__r1) / (__r2))
#endif

/* Macro to convert from fixed point to internal non-integral format */
#ifdef DISP_FIXED_POINT
#define DISP_FXREAL(__fx)	(__fx)
#else
#define DISP_FXREAL(__fx)	((float)(__fx)/0x10000)
#endif

/* Convert an integral value to a real value */
#ifdef DISP_FIXED_POINT
#define DISP_INTREAL(__i)	((disp_real)((__i) << 16))
#else
#define DISP_INTREAL(__i)	((disp_real)(__i))
#endif

/* Convert a real value to an integral value */
#ifdef DISP_FIXED_POINT
#define DISP_REALINT(__r)	((int)(__r) >> 16)
#else
#define DISP_REALINT(__r)	((int)(__r))
#endif

#ifdef __X86__
static __inline__ int
DISP_FXDIV(int a, int b)
{
	int result;

	if (abs(a >> 14) > abs(b)) {
		/* Could cause overflow exception */
		if (((unsigned)a ^ (unsigned)b) & (1<<31))
			return 1<<31;		/* Negative result */
		else
			return 0x7fffffff;
	}

	asm (
		"xorl  %%eax,%%eax      \n"
		"shrdl $16,%%edx,%%eax  \n"
		"sarl  $16,%%edx        \n"
		"idivl %%ecx            \n"

		: "=a" (result)
		: "d" (a), "c" (b)
	);

	return result;
}
#else
#ifdef DISP_FIXED_POINT
#define DISP_FXDIV(__a, __b) \
	((int)(((int64_t)(__a) << 16)/(__b)))
#else
#define DISP_FXDIV(__a, __b) \
	((int)(((float)(__a)*0x10000)/(__b)))
#endif
#endif

#endif /* _GRAPHICS_FIXED_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/fixed.h $ $Rev: 657836 $" )
