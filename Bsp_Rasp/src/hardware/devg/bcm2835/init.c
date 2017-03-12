/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems.
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
/*
 * Copyright 2016, Varghese Alphonse.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <x86/priv.h>
#include <sys/platform.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
//#include "at91sam9xx.h"
#include "bcm2835.h"


int
bcm2835_init(disp_adapter_t *adapter, char *optstring)
{
	at91sam9xx_context_t		*at91sam9xx;
	  uintptr_t mailboxbase;
	  unsigned int ra;
	  unsigned long  i=0;
	  unsigned int mailptraddr;

	slogf(_SLOGC_GRAPHICS, _SLOG_INFO, "at91sam9xx_init");

   if (adapter->shmem == NULL) {
            if ((at91sam9xx = calloc(1, sizeof (*at91sam9xx))) == NULL)
                    return -1;
            at91sam9xx->free_context = 1;
    } else {
            at91sam9xx = adapter->shmem;
            memset(at91sam9xx, 0, sizeof (*at91sam9xx));
    }
	adapter->ms_ctx = at91sam9xx;
	at91sam9xx->adapter = adapter;
	adapter->vsync_counter = &at91sam9xx->vsync_counter;
		
	if (disp_register_adapter(adapter) == -1) {
            if (at91sam9xx->free_context){
                    free(at91sam9xx);
            }
            return -1;	
	}
	
	get_config_data(at91sam9xx, optstring);
		
	at91sam9xx->dc_regptr = disp_mmap_device_memory(0x40040000, 0x30,
			PROT_READ | PROT_WRITE | PROT_NOCACHE, 0);
	
	if (at91sam9xx->dc_regptr == MAP_FAILED) 
		goto fail;

	*dc_rptr(0x0)= at91sam9xx->width; /* #0 Physical Width */
	*dc_rptr(0x4)= at91sam9xx->height;//	out32(at91sam9xx->dc_regptr+0x4, 480); /* #4 Physical Height */
	*dc_rptr(0x8)= at91sam9xx->width;;//	out32(at91sam9xx->dc_regptr+0x8, 800); /* #8 Virtual Width */
	*dc_rptr(0xc)= at91sam9xx->height;;//	out32(at91sam9xx->dc_regptr+0x0C, 480); /* #12 Virtual Height */
	*dc_rptr(0x10)= 0;//	out32(at91sam9xx->dc_regptr+0x10, 0); /* #16 GPU - Pitch */
	*dc_rptr(0x14)= 24;//	out32(at91sam9xx->dc_regptr+0x14, 32); /* #20 Bit Depth */ //fixme 24 or 32?
	*dc_rptr(0x18)= 0;//	out32(at91sam9xx->dc_regptr+0x18, 0); /* #24 X */
	*dc_rptr(0x1c)= 0;//	out32(at91sam9xx->dc_regptr+0x1C, 0); /* #28 Y */
	*dc_rptr(0x20)= 0;//	out32(at91sam9xx->dc_regptr+0x20, 0); /* #32 GPU - Pointer */
	*dc_rptr(0x24)= 0;//	out32(at91sam9xx->dc_regptr+0x24, 0); /* #36 GPU - Size */

	at91sam9xx->mailboxptr = disp_mmap_device_memory(0x2000B880, 0x30,    //  mailbox mem address
					PROT_READ | PROT_WRITE | PROT_NOCACHE, 0);
	if (at91sam9xx->mailboxptr== MAP_FAILED)
				goto fail;
    //Mailbox write
	while(1)
	{
	    ra=*mailbox_rptr(0x18);
        if((ra&0x80000000)==0) break;
    }
    *mailbox_rptr(0x20)=  0x40040001;

    //Mail box Read
    while(1)
	{
		  while(1)
		  {
		      ra=*mailbox_rptr(0x18);
		      if((ra&0x40000000)==0) break;
		   }
		   ra=*mailbox_rptr(0x0);
		   if((ra&0xF)==1) break;
	 }
  /* fprintf(stderr, "Register Dump\n");
	 fprintf(stderr, "registers :0x0 :%d \n",*dc_rptr(0x0));
	 fprintf(stderr, "registers :0x4 :%d \n",*dc_rptr(0x4));
	 fprintf(stderr, "registers :0x8 :%d\n",*dc_rptr(0x8));
	 fprintf(stderr, "registers :0xc :%d\n",*dc_rptr(0xc));
	 fprintf(stderr, "registers :0x10 :%d\n",*dc_rptr(0x10));
	 fprintf(stderr, "registers :0x14 :%d\n",*dc_rptr(0x14));
	 fprintf(stderr, "registers :0x18 :%d\n",*dc_rptr(0x18));
	 fprintf(stderr, "registers :0x1c :%d\n",*dc_rptr(0x1c));
	 fprintf(stderr, "registers vbase :0x20 :%d\n",*dc_rptr(0x20));
	 fprintf(stderr, "registers size  :0x24 :%d\n",*dc_rptr(0x24));*/

	 at91sam9xx->vidbase= *dc_rptr(0x20);// in32(at91sam9xx->dc_regptr+0x20);
	 at91sam9xx->vidsize=at91sam9xx->width*at91sam9xx->height*3;
	 at91sam9xx->vidbase=at91sam9xx->vidbase;
			//	test screen
	/* at91sam9xx->testscr=disp_mmap_device_memory(at91sam9xx->vidbase, at91sam9xx->vidsize,
					DISP_PROT_READ | DISP_PROT_WRITE, DISP_MAP_LAZY);
			//808000FF red
	 for( i=0;i<1536000;)
	 {
		 *Tstscr_rptr(i)=0x80002066;
		 i=i+4;
	 }
	 sleep(5);*/

	adapter->adapter_ram = at91sam9xx->vidsize;
	at91sam9xx->vidptr = disp_mmap_device_memory(at91sam9xx->vidbase, at91sam9xx->vidsize,
			DISP_PROT_READ | DISP_PROT_WRITE, DISP_MAP_LAZY);
	if (at91sam9xx->vidptr == MAP_FAILED)
		goto fail;
	return 0;
	
fail:
	if (at91sam9xx->dc_regptr)
		disp_munmap_device_memory(at91sam9xx->dc_regptr, 0x30);
	if (at91sam9xx->vidptr)
		disp_munmap_device_memory(at91sam9xx->vidptr, at91sam9xx->vidsize);
		
	disp_unregister_adapter(adapter);
	free(adapter->gd_ctx);
	 if (at91sam9xx->free_context){
     	free(at91sam9xx);
     }
	return -1;
}

/*
 * Called when there's no more drawing to be done (e.g. a mode-switch is
 * about to occur or the driver is about to be unloaded).
 */
void
bcm2835_fini(disp_adapter_t *adapter)
{
	slogf(_SLOGC_GRAPHICS, _SLOG_INFO, "at91sam9xx_fini");
	at91sam9xx_context_t	*at91sam9xx = adapter->ms_ctx;
	
	if (adapter->ms_ctx == NULL)
		return;

	if (at91sam9xx->vidptr)
		disp_munmap_device_memory(at91sam9xx->vidptr, at91sam9xx->vidsize);
	if (at91sam9xx->dc_regptr)
		disp_munmap_device_memory(at91sam9xx->dc_regptr, 0x30);
	
    disp_unregister_adapter(adapter);
	free(adapter->gd_ctx);
	if (at91sam9xx->free_context){
        free(at91sam9xx);
    }
	
	adapter->ms_ctx = NULL;	
}

