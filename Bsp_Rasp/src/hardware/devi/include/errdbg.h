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








#ifndef _ERRDBG_H_INCLUDED
#define _ERRDBG_H_INCLUDED

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* 
 * examples of the DEBUG macro in action
 *
 * DBG(printf("This val should be: %d\n", val););
 *
 * DBG(assert(x == someval););
 *
 * DBG(if (x == someval) 
         print("Oops");
       else 
         x = newval;
   );
 *
 *
 */

#ifdef DEBUG
#define DBG(code_fragment) do { code_fragment } while (0)
#else
#define DBG(code_fragment)  // debug: do nothing
#endif

#define VBOSE(a) if(verbose) printf("%s\n",(a))

#define err_abort(code, text) do { \
		     fprintf (stderr, "%s at \"%s\":%d %s\n", \
			      text, __FILE__, __LINE__, strerror(code)); \
		     abort(); \
		     } while (0)

#define errno_abort(text) do { \
		     fprintf (stderr, "%s at \"%s\":%d: %s\n", \
			      text, __FILE__, __LINE__, strerror(errno)); \
		     abort(); \
		     } while (0)


#define errno_exit(text) do { \
		     fprintf (stderr, "%s at \"%s\":%d: %s\n", \
			      text, __FILE__, __LINE__, strerror(errno)); \
		     exit(EXIT_FAILURE); \
		     } while (0)


#define errno_print(text) do { \
		     fprintf (stderr, "%s at \"%s\":%d: %s\n", \
			      text, __FILE__, __LINE__, strerror(errno)); \
		     } while (0)




#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/include/errdbg.h $ $Rev: 657836 $" )
