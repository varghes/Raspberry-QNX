/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
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





#ifndef _NW_DATASTRUCT_H_INCLUDED
#define _NW_DATASTRUCT_H_INCLUDED

#ifndef __IO_PKT_H_INCLUDED
#include <sys/io-pkt.h>
#endif

#include <sys/pool.h>
#include <net/cacheline.h>
#include <ucontext.h>
#include <setjmp.h>
#include <sys/queue.h>
#include <sys/dispatch.h>

extern char iopkt_instance[30];

struct _thread_local_storage;
struct proc;
struct ipflow_threadinfo;
struct bridge_threadinfo;
#include <sys/resourcevar.h>

struct prio_ent {
	struct prio_ent *nxt;    /* nxt as first member! */
	struct prio_ent **prv;
	int prio;
};

struct prio_ends {
	struct prio_ent *head;
	struct prio_ent *tail;
};

struct prio {
	struct prio_ends *prio_prios;
	struct prio_ends prio_all;
};

struct proc_alloc {
	struct proc *procs;
	char        *stacks;
};


struct mbuf_zone {
	struct mbuf *p;
	int avail;
	int max;
};

struct nw_oob_ctl {
	int init;
	int prio;
	int chid;
	int coid;

	int noob;
	struct oob_info {
		struct sigevent *ev;
		struct _iopkt_inter *inter;
	} *oob_infop;
};


struct nw_work_thread {
	int			wt_critical;
	int			wt_intr_pending;
	int                       intr_sighot;
	unsigned                  am_stack;
	unsigned                  blocking;
	unsigned                  inreceive;

	/*
	 * The _real_ tidx == NW_TID_TO_TIDX(pthread_self()).
	 * Also the index into stk_ctl.work_threads.
	 */
	int                       tidx_wt;

	/*
	 * Interrupt callouts are only to be performed by core
	 * and oob threads:
	 * - Processing interrupts stored off
	 *   _iopkt_selfp->inter_threads[x].inter_head
	 *
	 * The core threads are always created first.  To
	 * remove the restriction that oob threads' tidx_wt
	 * must follow those of core threads, we introduce
	 * tidx_irupt to allow holes between them.  The
	 * index into iopkt_selfp->inter_threads.
	 */
	int                       tidx_irupt;

	/*
	 * ipflow wants the max number of threads
	 * at startup.  Non NULL if this thread
	 * has registered to flow.
	 */
	struct ipflow_threadinfo  *flowctl;

	/* Similarly for bridge */
	union {
		struct bridge_threadinfo	*wt_bru_brctl;
		int				*wt_bru_bridx;
	} wt_bru;
#define wt_bridx wt_bru.wt_bru_bridx
#define wt_brctl wt_bru.wt_bru_brctl

	struct nw_oob_ctl         *wt_oob;

	struct mbuf_zone          wt_mzones[2];
#define wt_zone_mbuf	wt_mzones[0]
#define wt_zone_packet	wt_mzones[1]

	mcontext_t                *jp;
	char                      *intr_stack_tos;
	uint32_t                  saved_sp;
	struct _thread_local_storage *tls;

	union	{
		jmp_buf           rx_loop_jmp_buf;
		mcontext_t        rx_loop_ctxt;
	} rx_loop_ctxt;
	void                      *saved_stackbase;

	char                      *intr_stack_base;

	int                       poke_stack;  /* for oob threads */

	int                       flags;
#define WT_CORE		0x00000001
#define WT_OOB 		0x00000002
#define WT_IRUPT	0x00000004
#define WT_FLOW		0x00000008
#define WT_BRIDGE	0x00000010
#define WT_DYING	0x00000020
#define WT_COREFLAGS	WT_CORE | WT_IRUPT | WT_FLOW | WT_BRIDGE
	void                      (*quiesce_callout)(void *, int);
	void                      *quiesce_arg;
	void                      *wtp_alloc;
	/* For dedicated threads (eg ppp) to use as they wish */
	union sigval              wt_specialized;
	int                       unused[4];
} __attribute__((aligned (NET_CACHELINE_SIZE)));

struct nw_oob_msg {
	short type;
	int noob;
	struct oob_info *oob_infop;
};

struct nw_stk_ctl {
	struct _iopkt_self              *iopkt;
	int                             do_cache;
	/*
	 * All manipulation by stack of recv_*
	 * must be completed before a proc
	 * blocks (ltsleep()).  This is to avoid
	 * window recv_* could be accessed by
	 * stack and thread setting them up for
	 * MsgReceive() at the same instance in
	 * time.
	 */
	int                             recv_max;
	int                             recv_avail;
	iov_t                           *recv_iov;

	struct mbuf                     **recv_mbuf;
	int                             recv_start;

	struct proc                     *recv_loaded_proc;
	struct proc                     *recv_procp;

	int                             chid;
	int                             coid;
	dispatch_t                      *dpp;
	int                             timer_int_id;

	int                             fastforward;

	int                             stacksize;

	int				rx_prio;

	int                             proc_alloc_tot;
	int                             proc_alloc_used;
	struct proc_alloc               *allprocs;
	struct proc 			*proc0;

	struct proc                     *freeprocs;
	int                             nprocs_cur_max;
	int                             nprocs_used;

	int                             nprocs_min;
	int                             nprocs_incr;
	int                             nprocs_max;

	unsigned                        nthreads_core;
	unsigned                        nthreads_core_max;
	unsigned                        nthreads_oob;
	unsigned                        nthreads_oob_max;
	unsigned                        nthreads_flow_max;
	/*
	 * Max number we're aware of.  Not the max tid we've
	 * seen (shim may create threads we know nothing about).
	 */
	unsigned                        nthreads_cur;

	unsigned                        nthreads_untracked;

	/*
	 * This one's size is dynamic.
	 * Indexed by work threads by tidx_wt member.
	 */
	unsigned                        nwork_threads; /* Size of our array */
	struct nw_work_thread           **work_threads;

	int                             quiesce_count;

	pthread_mutex_t                 stack_ex;
	int                             stack_inuse;
	int                             thread_inreceive;
	int                             msg_outstanding;

	pthread_mutex_t                 pkt_ex;
	struct ifqueue                  *pkt_rx_q;

	pthread_key_t                   work_thread_key;

	struct prio                     proc_prio;

	struct pstats                   pstats;
	struct plimit                   plimit;
	/* Put at bottom to avoid requiring drivers which use macros (e.g. WTP) which access this structure to be rebuilt */
#ifndef USE_TIMER_INTR
	/* Note: When !USE_TIMER_INTR, timer_int_id is not an interrupt id, it's a timer id! */
	struct sigevent                 timer_ev;
	int				timer_pulse_prio;
#endif
};


extern struct nw_stk_ctl stk_ctl;      /* main.c */
extern struct nw_oob_ctl oob_ctl_low;  /* main.c  ifdef OOB_THREAD_LOW  */
extern struct nw_oob_ctl oob_ctl_high; /* main.c  ifdef OOB_THREAD_HIGH */

extern int init_procs(struct nw_stk_ctl *sctlp, int);
extern int add_procs(struct nw_stk_ctl *sctlp, int nprocs, int nstacks);
extern int pcreat(struct proc *, void (*)(void *), void *);
extern void startproc(void *arg);
extern void iopkt_func_init(struct _iopkt_self *iopkt, struct nw_stk_ctl *sctlp);

#endif /* !_NW_DATASTRUCT_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/io-pkt/sys/nw_datastruct.h $ $Rev: 657826 $" )
