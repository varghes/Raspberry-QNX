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





#include <mkasmoff.h>
#include <stddef.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <sys/fault.h>
#include <sys/syspage.h>
#include <hw/8250.h>
#include <hw/zscc.h>
#include "startup.h"

/*
	This source file isn't included in the startup library.
	It's only used to build the asmoff.def file.

	CPU independent definitions
*/

VALUE(QT_TIMER_RATE,		offsetof(struct qtime_entry, timer_rate));
VALUE(QT_TIMER_LOAD,		offsetof(struct qtime_entry, timer_load));

VALUE(MS_SYNC,				MS_SYNC);
VALUE(MS_ASYNC,				MS_ASYNC);
VALUE(MS_INVALIDATE,		MS_INVALIDATE);
VALUE(MS_INVALIDATE_ICACHE,	MS_INVALIDATE_ICACHE);

VALUE(CA_LINE_SIZE,			offsetof(struct cacheattr_entry, line_size));
VALUE(CA_NUM_LINES,			offsetof(struct cacheattr_entry, num_lines));

VALUE(REG_LS,				REG_LS);
VALUE(LSR_RXRDY,			LSR_RXRDY);
VALUE(LSR_TXRDY,			LSR_TXRDY);
VALUE(LSR_BI,				LSR_BI);

VALUE(SCC_RR0_TX_EMPTY,		SCC_RR0_TX_EMPTY);
VALUE(SCC_RR0_RX_AVAILABLE,	SCC_RR0_RX_AVAILABLE);
VALUE(SCC_RR0_BREAK,		SCC_RR0_BREAK);

VALUE(DDI_BASE,				offsetof(chip_info, base));
VALUE(DDI_SHIFT,			offsetof(chip_info,shift));
VALUE(SIZEOF_DDI,			sizeof(chip_info));

VALUE(INTR_CONFIG_FLAG_IPI, 		INTR_CONFIG_FLAG_IPI);
VALUE(INTR_CONFIG_FLAG_DISALLOWED,	INTR_CONFIG_FLAG_DISALLOWED);
VALUE(INTR_CONFIG_FLAG_PREATTACH,	INTR_CONFIG_FLAG_PREATTACH);

VALUE(SYSPAGE_SYSTEM_PRIVATE,	offsetof(struct syspage_entry, system_private));
VALUE(SP_PRIVATE_FLAGS,			offsetof(struct system_private_entry, private_flags));
VALUE(SYSTEM_PRIVATE_FLAG_ABNORMAL_REBOOT,SYSTEM_PRIVATE_FLAG_ABNORMAL_REBOOT);

VALUE(SYSPAGE_NUM_CPU,		offsetof(struct syspage_entry, num_cpu));
VALUE(SYSPAGE_SMP,			offsetof(struct syspage_entry, smp));
VALUE(SMP_START_ADDR,		offsetof(struct smp_entry, start_addr));
VALUE(SMP_PENDING,			offsetof(struct smp_entry, pending));

VALUE(SYSPAGE_PMINFO,		offsetof(struct syspage_entry, pminfo));
VALUE(PMINFO_WAKEUP_PENDING,offsetof(struct pminfo_entry, wakeup_pending));
VALUE(PMINFO_WAKEUP_CONDITION,offsetof(struct pminfo_entry, wakeup_condition));
VALUE(PMINFO_MANAGED_STORAGE,offsetof(struct pminfo_entry, managed_storage));

VALUE(SIGBUS,				SIGBUS);
VALUE(BUS_ADRERR,			BUS_ADRERR);
VALUE(BUS_OBJERR,			BUS_OBJERR);
VALUE(FLTACCESS,			FLTACCESS);
VALUE(FLTBOUNDS,			FLTBOUNDS);

VALUE(DEBUG_WATCHDOG_STOP,	DEBUG_WATCHDOG_STOP);
VALUE(DEBUG_WATCHDOG_CONTINUE,	DEBUG_WATCHDOG_CONTINUE);
VALUE(DEBUG_WATCHDOG_FEED,	DEBUG_WATCHDOG_FEED);

/* __SRCVERSION("asmoff.h $Rev: 657836 $"); */
