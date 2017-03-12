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
 
#ifdef __USAGE
%C - SPI driver

Syntax:
%C [-u unit]

Options:
-u unit    Set spi unit number (default: 0).
-d         driver module name
#endif


#include <sys/procmgr.h>
#include <dlfcn.h>
#include "proto.h"


int main(int argc, char *argv[])
{
	spi_dev_t	*head = NULL, *tail = NULL, *dev;
	void		*drventry, *dlhdl;
	siginfo_t	info;
	sigset_t	set;
	int			i, c, devnum = 0;

	if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		perror("ThreadCtl");
		return (!EOK);
	}

	_spi_init_iofunc();

	while ((c = getopt(argc, argv, "u:d:")) != -1) {
		switch (c) {
			case 'u':
				devnum = strtol(optarg, NULL, 0);
				break;
			case 'd':
				if ((drventry = _spi_dlload(&dlhdl, optarg)) == NULL) {
					perror("spi_load_driver() failed");
					return (-1);
				}

				do {
					if ((dev = calloc(1, sizeof(spi_dev_t))) == NULL)
						goto cleanup;
	
					if (argv[optind] == NULL || *argv[optind] == '-')
						dev->opts = NULL;
					else
						dev->opts = strdup(argv[optind]);
					++optind;
					dev->funcs  = (spi_funcs_t *)drventry;
					dev->devnum = devnum++;
					dev->dlhdl  = dlhdl;

					i = _spi_create_instance(dev);
					if (dev->opts)
						free(dev->opts);

					if (i != EOK) {
						perror("spi_create_instance() failed");
						free(dev);
						goto cleanup;
					}

					if (head) {
						tail->next = dev;
						tail = dev;
					}
					else
						head = tail = dev;
				} while (optind < argc && *(optarg = argv[optind]) != '-'); 

				/* 
				 * Now we only support one dll
				 */
				goto start_spi;

				break;
		}
	}

start_spi:
	if (head) {
		/* background the process */
		procmgr_daemon(0, PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL);

		sigemptyset(&set);
		sigaddset(&set, SIGTERM);

		for (;;) {
			if (SignalWaitinfo(&set, &info) == -1)
				continue;

			if (info.si_signo == SIGTERM)
				break;
		}
	}

cleanup:
	dev=head;
	while (dev) {
		if (dev->ctp) {
			dispatch_unblock(dev->ctp);

		}

		if (dev->drvhdl) {
			resmgr_detach(dev->dpp, dev->id, _RESMGR_DETACH_ALL);
			dev->funcs->fini(dev->drvhdl);
		}

		if (dev->dpp) {
			dispatch_destroy(dev->dpp);
		}

		head = dev->next;
		free(dev);
		dev=head;
	}

	dlclose(dlhdl);

	return (EOK);
}


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/spi/master/_spi_main.c $ $Rev: 657836 $" );
