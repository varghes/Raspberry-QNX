/*
 * $QNXLicenseC: 
 * Copyright 2007, 2008 QNX Software Systems.  
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
 * begin.c
 *
 * Initial entry point into the devi runtime system.  Parses the command
 * line, starts up the photon interface (if needed), sets up signal
 * handling, and the thread pool used to receive pulses and messages.
 *
 */

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/devi.h>
#include "proto.h"

static void sig_install (int *table, void (*function) (int));
static void termination_hndlr (int sig_num);

extern input_module_t absolute_filter;
extern input_module_t fd;
extern input_module_t uart;
extern input_module_t relative_filter;
extern input_module_t keyboard_filter;
extern input_module_t joystick_filter;
//extern input_module_t gamepad_filter;
extern input_module_t control_filter;

input_module_t *ModuleTable[MODULE_TABLE_SIZE] = {
	&uart,
	&fd,
	&absolute_filter,
	&relative_filter,
	&keyboard_filter,
	&joystick_filter,
	&control_filter
};

extern input_module_t *modules[];

static int signal_table[] = {
	SIGILL,
	SIGTERM,
	SIGHUP,
	SIGQUIT,
	SIGINT,
	SIGABRT,
	0
};

common_callbacks_t commonCallbacks = {
	4,
	NULL,													// Pre-init
	NULL,													// Post-init
	NULL,													// Pre-shutdown
	NULL													// Post-shutdown
};

extern int raw_enqueue (unsigned type, char *dptr, int size);
static void *
playback_thread (void *data)
{
	struct
	{
		struct timespec t;
		int type;
		unsigned size;
	}
	rec;

	long long last_stamp_msec = 0, cur_time_msec;
	char buf[1024];
	long loop_pos = 0;

	printf ("initial delay(%d)\n", delayMsec);

	delay (delayMsec);

loop:
	printf ("starting playback...\n");

	// timestamp, type(unsigned), data_size(unsigned), data
	while (1 == fread (&rec, sizeof (rec), 1, playFile)) {
		if (rec.size > sizeof (buf)) {
			printf ("playback buf is not big enough, new size = %d\n", rec.size);
			input_shutdown ();
		}
		if (1 != fread (buf, rec.size, 1, playFile)) {
			perror ("fread()");
			input_shutdown ();
		}
		cur_time_msec = rec.t.tv_sec * 1000 + rec.t.tv_nsec / 1000000L;
		if (last_stamp_msec != 0 && cur_time_msec - last_stamp_msec > 1) {
			delay (cur_time_msec - last_stamp_msec);
		}
		last_stamp_msec = cur_time_msec;
		raw_enqueue (rec.type & DEVI_CLASS_MASK, buf, rec.size);

		if ((rec.type & DEVI_CLASS_MASK) == DEVI_CLASS_KBD && loop_pos == 0) {
			struct packet_kbd *kbuf = (struct packet_kbd *) buf;

			if (kbuf->key.key_cap == KEYCODE_MENU) {
				loop_pos = ftell (playFile);
				printf ("loop position assigned %ld\n", loop_pos);
			}
		}
	}

	printf ("playback complete\n");

	if (!ferror (playFile) && loopPlayback) {
		fseek (playFile, loop_pos, SEEK_SET);
		goto loop;
	}

	input_shutdown ();						// TODO: Do we need to exit?
	return NULL;
}

void
set_abs_pos (int x, int y)
{
	struct packet_abs abs = { 0 };

	abs.flags |= ABSOLUTE;
	abs.flags &= ~ABS_UNCALIBRATED;
	abs.x = x;
	abs.y = y;
	devi_enqueue_packet (&absolute_filter, (char *) &abs, sizeof (abs));
}

int
begin (int argc, char *argv[])
{
	int i, j;
	uint32_t ph_flags = 0;
	thread_pool_attr_t tp_attr;
	static void *tpp;
	dispatch_t *dpp;

	dpp = dispatch_create ();
	if (dpp == NULL) {
		char *pMsgTxt = "Error: cannot create dispatch interface\n";

		fprintf (stderr, pMsgTxt);
		slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt);
		return (EXIT_FAILURE);
	}
	devi_set_dispatch_handle (dpp);
	/* set up the module table */
	if ((void *)modules != NULL) {

		for (i = 0; i < MODULE_TABLE_SIZE && ModuleTable[i] != NULL; i++);
		/* add extra modules to end of ModuleTable */

		for (j = 0; j < (MODULE_TABLE_SIZE - i) && modules[j] != NULL; j++, i++)
			ModuleTable[i] = modules[j];
	}

	if (ThreadCtl (_NTO_TCTL_IO, 0) == -1) {
		errno_print ("ThreadCtl");
		return (EXIT_FAILURE);
	}
	// Call global callback if it was specified        
	if (commonCallbacks.pre_init && commonCallbacks.pre_init ())
		return (EXIT_FAILURE);
	if (options (argc, argv, &ph_flags, &OptFlags) < 0)
		return (EXIT_FAILURE);
	// Call global callback if it was specified        
	if (commonCallbacks.post_init && commonCallbacks.post_init ())
		return (EXIT_FAILURE);
	sig_install (signal_table, termination_hndlr);

	if (procmgr_daemon (0, PROCMGR_DAEMON_NODEVNULL |
											PROCMGR_DAEMON_NOCLOSE) < 0) {
		errno_print ("procmgr_daemon");
		return (EXIT_FAILURE);
	}


	if (start_photon_interface (ph_flags) < 0) {
		if (!(OptFlags & OPT_NO_PHOTON)) {
			errno_print ("unable to start photon interface");
			return (EXIT_FAILURE);
		}
	}

	gfi_register (IpGroup);

	memset (&tp_attr, 0, sizeof (tp_attr));
	tp_attr.handle = devi_get_dispatch_handle ();
	tp_attr.context_alloc = dispatch_context_alloc;
	tp_attr.block_func = dispatch_block;
	tp_attr.handler_func = dispatch_handler;
	tp_attr.context_free = dispatch_context_free;
	/* We'd rather not to use thread pool for device driver - it could 
	   cause desynchronizing and data losing                       */
	tp_attr.lo_water = 1;					/* min amount threads waiting blocked */
	tp_attr.hi_water = 1;					/* max amount threads waiting blocked */
	tp_attr.increment = 1;
	tp_attr.maximum = 1;

	tpp = thread_pool_create (&tp_attr, POOL_FLAG_USE_SELF);
	if (tpp == NULL) {
		errno_print ("thread_pool_create");
		return (EXIT_FAILURE);
	}

	if (logFile) {
		set_abs_pos (0, 0);

	} else if (playFile) {
		pthread_create (NULL, NULL, playback_thread, NULL);

	}

	thread_pool_start (tpp);

	return (EOK);
}


static void
sig_install (int *table, void (*function) (int))
{
	struct sigaction act;
	sigset_t valid_signals;
	int *signalp;

	sigemptyset (&valid_signals);

	for (signalp = table; *signalp; signalp++)
		sigaddset (&valid_signals, *signalp++);

	for (signalp = table; *signalp; signalp++) {
		act.sa_flags = 0;
		act.sa_mask = valid_signals;
		act.sa_handler = function;
		if (sigaction (*signalp, &act, NULL) < 0)
			fprintf (stderr, "Error: sigaction for %d failed: %s\n", *signalp,
							 strerror (errno));
	}
}

static void
termination_hndlr (int sig_num)
{
	char *pMsgTxt = "Hit with signal: %d, shutting down\n";

	if (verbosity)
		fprintf (stderr, pMsgTxt, sig_num);
	slogf (_SLOG_SETCODE (_SLOGC_INPUT, 0), _SLOG_ERROR, pMsgTxt, sig_num);

	input_shutdown ();
}


void
input_shutdown (void)
{
	event_bus_line_t *line;
	input_module_t *module;
	static int in_shutdown;

	if (in_shutdown++) {
		return;
	}
	// Call global callback if it was specified        
	if (commonCallbacks.pre_shutdown)
		commonCallbacks.pre_shutdown ();
	line = Bus;
	do {
		module = line->bottom;
		do {
			if (module && module->shutdown) {
				(module->shutdown) (module, 0);
			}
		} while ((module = module->down));
	} while ((line = line->next));

	--in_shutdown;

	// Call global callback if it was specified        
	if (commonCallbacks.post_shutdown && commonCallbacks.post_shutdown ())
		_exit (EXIT_FAILURE);

	_exit (EXIT_SUCCESS);
}


void
devi_init (pCommon_callbacks_t pCommonCallbacks)
{
	int i = 0;

	if (NULL == pCommonCallbacks)
		return;
	commonCallbacks.nCallbacks = pCommonCallbacks->nCallbacks;
	if (pCommonCallbacks->nCallbacks < ++i)
		return;
	commonCallbacks.pre_init = pCommonCallbacks->pre_init;
	if (pCommonCallbacks->nCallbacks < ++i)
		return;
	commonCallbacks.post_init = pCommonCallbacks->post_init;
	if (pCommonCallbacks->nCallbacks < ++i)
		return;
	commonCallbacks.pre_shutdown = pCommonCallbacks->pre_shutdown;
	if (pCommonCallbacks->nCallbacks < ++i)
		return;
	commonCallbacks.post_shutdown = pCommonCallbacks->post_shutdown;

}

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/lib/begin.c $ $Rev: 657836 $" );
