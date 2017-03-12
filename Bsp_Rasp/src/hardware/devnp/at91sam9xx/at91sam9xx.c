/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */




#include "at91sam9xx.h"


static int at91sam9xx_entry(void *dll_hdl, struct _iopkt_self *iopkt, char *options);

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(at91sam9xx) = IOPKT_DRVR_ENTRY_SYM_INIT(at91sam9xx_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms at91sam9xx_syms[] = {
        {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(at91sam9xx)},
        {NULL, NULL}
};
#endif


/*
 * at91sam9xx_entry()
 *
 * Description: main entry point for the driver
 *
 * Returns: 0 if OK else -1 on error with errno set
 *
 */
int
at91sam9xx_entry(void *dll_hdl, struct _iopkt_self *iopkt, char *options)
{
	return  at91sam9xx_detect(dll_hdl, iopkt, options);
}


__SRCVERSION( "$URL$ $REV$" )
