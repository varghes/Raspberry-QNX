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

#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED

#include <pthread.h>
#include <sys/queue.h>
#include <sys/resmgr.h>
#include <sys/syspage.h>

#include <cam.h>
#include <sys/cfg.h>


__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/include/sim.h $ $Rev: 657836 $" )

extern paddr_t mphys( void * );

// Useful TAILQ manifests possibly not in sys/queue.h
#ifndef TAILQ_EMPTY
	#define	TAILQ_EMPTY(head) ((head)->tqh_first == NULL)
#endif
#ifndef TAILQ_FIRST
	#define	TAILQ_FIRST(head) ((head)->tqh_first)
#endif
#ifndef TAILQ_LAST
	#define	TAILQ_LAST(head) ((head)->tqh_last)
#endif
#ifndef TAILQ_NEXT
	#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
#endif
#ifndef TAILQ_PREV
	#define TAILQ_PREV(elm, field) ((elm)->field.tqe_prev)
#endif

typedef struct Config_Info			CONFIG_INFO;
typedef struct _sim_memory_block	SIM_MEM_BLOCK;
typedef struct _sim_memory_pool		SIM_MEM_POOL;
typedef struct _sim_hba_stats		SIM_HBA_STATS;
typedef struct _sim_hba_extension	SIM_HBA_EXT;
typedef struct _sim_hba				SIM_HBA;

// NOTES:
//
//  An initiator may not mix the use of tagged and untagged queuing for I/O
//  processes to a logical unit, except during a contingent allegiance or
//  extended contingent allegiance condition when only untagged initial
//  connections are allowed.
//
//  If the disconnect privilege is not granted in the IDENTIFY message for
//  a tagged I/O process, the target will return BUSY status.
//
//  A series of linked commands constitue a single I/O process.  These
//  commands are assigned the queue tag established in the initial
//  connection.
//
//  The cam_status is | with CAM_SIM_QFRZN.

// state defines
#define SIM_CCB_READY			0x0000		// CCB is not active
#define SIM_CCB_NEXUS			0x0001		// CCB has been dequeued/started
#define SIM_CCB_ARB				0x0002		// CCB is currently arbitrating
#define SIM_CCB_SEL				0x0004		// CCB is currently selecting
#define SIM_CCB_CMD				0x0008		// CCB is in command in phase
#define SIM_CCB_MSGI			0x0010		// CCB is in message in phase
#define SIM_CCB_MSGO			0x0020		// CCB is in message out phase
#define SIM_CCB_DIN				0x0040		// CCB is in data in phase
#define SIM_CCB_DOUT			0x0080		// CCB is in data out phase
#define SIM_CCB_STAT			0x0100		// CCB is in status phase
#define SIM_CCB_DISC			0x0200		// CCB is disconnected
#define SIM_CCB_DONE			0x0400		// CCB has completed
#define SIM_CCB_ABORT			0x8000		// CCB is to be aborted
#define SIM_CCB_RESET			0x4000		// CCB is to be aborted
#define SIM_CCB_MASK			0x3fff		// CCB state mask
#define SIM_CCB_QUERY			0xFFFF		// Query ccb state

typedef struct sim_lun_queue {
	_uint32				frzn_cnt;
	CCB_SCSIIO			*ccb;				// ccbs queued
	CCB_SCSIIO			*nexus;				// ccbs active
	_uint8				ltag_id;			// last tag id given out
	_uint8				tag;				// count of active tagged ccbs
	_uint8				nontag;				// count of active non tagged ccbs
	_uint8				max_tagged;			// maximum tagged per lun
} SIM_LUN_QUEUE;

typedef struct sim_target_queue {
	SIM_LUN_QUEUE		*lque;				// array of lun queues
} SIM_TARGET_QUEUE;

typedef struct sim_queue {
	pthread_t			tid;				// id of controlling thread
	_uint32				busy;				// queue access lock count
	timer_t				timerid;			// timer id
	_uint32				ntargs;				// number of targets
	_uint32				nluns;				// number of logical units
	_uint32				max_tagged;			// maximum tagged per lun
	_uint32				max_non_tagged;		// maximum non tagged per lun
	_uint32				mactive;			// maximum active ccbs
	_uint32				actcnt;				// number of active ccbs
	_uint32				qcnt;				// number of ccbs queued
	_uint32				tindx;				// used for round-robin scheduling
	_uint32				lindx;				// used for round-robin scheduling
	_uint32				timeout;
	SIM_HBA 			*hba;				// pointer to hba structure
	SIM_TARGET_QUEUE	*tque;				// variable length array of targets
	struct qtime_entry	*systime;			// fast access to current time
} SIM_QUEUE;

// This structure must be the first entry in the private data
typedef struct sim_que_data {
	CCB_SCSIIO			*pccb;				// previous ccb pointer
	CCB_SCSIIO			*nccb;				// next ccb pointer
	_uint16				state;				// SCSI state of ccb
	_uint8				rsvd;
	_uint8				tag_id;				// tag id for this ccb
	_uint32				timeout;			// timeout
} SIMQ_DATA;

#define SIMQ_TIMER_DISABLE	0x00
#define SIMQ_TIMER_ENABLE	0x01

#define SIM_TIME_DEFAULT	0x02			// default to a 2 second timeout

#define SIM_TAG_INVALID		0xff

#define SIM_ENQUEUE			0x01
#define SIM_INTERRUPT		0x02
#define SIM_TIMER			0x03
#define SIM_DRVR_TIMER		0x04
#define SIM_DMA_INTERRUPT	0x05

#define SIM_PRIORITY		21

#define SIMARG_VAL( _o, _v ) if( (_v) == NULL || *(_v) == '\0' ) { fprintf( stderr, "sim: Missing argument for '%s'", sim_drvr_opts[ _o ] ); return( EINVAL ); }
#define SIMARG_NOVAL( _o, _v ) if( (_v) != NULL ) { fprintf( stderr, "sim: Unexpected argument for '%s'", sim_drvr_opts[ _o ] ); return( EINVAL ); }
#define SIMARG_NUMBER( _o, _n, _f ) if( ( *(_n) = (_f)) == -1 ) { fprintf( stderr, "sim: Invalid numeric argument for '%s'", sim_drvr_opts[ _o ] ); return( EINVAL ); }
#define SIMARG_MISSING( _o, _a ) if( !(_a) ) { fprintf( stderr, "sim: Missing argument component for '%s'", sim_drvr_opts[ _o ] ); return( EINVAL ); }

#define HBA_FLAG_NORESET			0x0001
#define HBA_FLAG_NOPARITY			0x0002
#define HBA_FLAG_TERMLO				0x0004
#define HBA_FLAG_TERMHI				0x0008
#define HBA_FLAG_SCAM				0x0010

#define HBA_FLAG_ACCESS8			0x0020
#define HBA_FLAG_ACCESS16			0x0040
#define HBA_FLAG_ACCESS32			0x0080

#define HBA_STATE_FAILURE			0x01
#define HBA_STATE_INITIALIZING		0x02
#define HBA_STATE_INITIALIZED		0x04
#define HBA_STATE_RESETING			0x08
#define HBA_STATE_EXPECT_INTR		0x10

struct _sim_hba_stats {
	_uint32				stat_irqs;
	_uint32				stat_timeouts;
	_uint32				stat_selections;
	_uint32				stat_disconnects;
	_uint32				stat_reselections;
	_uint32				stat_bus_resets;
	_uint8				rsvd[36];
};

// sim extentions must include the following members
struct _sim_hba_extension {
	SIM_HBA				*hba;
	_uint32				eflags;
};

struct _sim_memory_block {
	size_t				size;				// This must be first
	SIM_MEM_BLOCK		*next;				// This is only used when free
};

struct _sim_memory_pool {
	paddr_t				paddr;				// Physical address of memory
	char				*vaddr;				// Virtual address of memory
	size_t				size;				// Size of block of memory
	SIM_MEM_BLOCK		*list;				// Pointer to start of free list
};

struct _sim_hba {
	TAILQ_ENTRY( _sim_hba )	hlink;
	SIM_HBA_EXT			*ext;
	SIM_QUEUE			*simq;
	CAM_ENTRY			*cam_funcs;
	CONFIG_INFO			cfg;
	_uint32				verbosity;
	_uint32				hflags;				// see HBA_FLAGS_...
	_uint32				state;				// see HBA_STATE_...
	_uint32				coid;
	_uint32				chid;
	_uint32				iid;
	pthread_t			tid;				// id of event handling thread
	void				*dhdl;				// device handle (pci)
	_int32				pathid;
	_uint32				vpathid[8];			// pathid's for virtual devices (system drives)

	_uint8				stride;				// I/O stride
	_uint8				decode;				// I/O decode
	_uint8				scsiid;
	_uint8				seltimo;			// selection timeout;
	_uint8				ntargets;
	_uint8				nluns;
	_uint8				speed;
	_uint8				nsgelm;				// number of scatter gatter elements

	_uint16				tag_mask;
	_uint16				disc_mask;
	_uint16				sync_mask;
	_uint16				fast_mask;
	_uint16				wide_mask;
	_uint16				ultra_mask;
	_uint16				ultra2_mask;
	_uint16				ultra160_mask;
	_uint16				ultra320_mask;
	_uint16				ultra640_mask;

	_uint32				timeout;			// command completion timeout
	_uint32				rst_settle;			// reset settle time
	SIM_HBA_STATS		stats;
	SIM_MEM_POOL		*pool;
};

#define SIM_MBLOCK_ALLOC( _free, _ptr )	{	pthread_sleepon_lock( ); 		\
											(_ptr) = (_free);  				\
											(_free) = *(void **)(_free);	\
											pthread_sleepon_unlock( ); }

#define SIM_MBLOCK_FREE( _free, _ptr )	{	pthread_sleepon_lock( ); 		\
											*(void **)(_ptr) = (_free); 	\
											(_free) = (_ptr);				\
											pthread_sleepon_unlock( ); }

paddr_t SIM_MPHYS( SIM_MEM_POOL *_pool, void *_ptr );
#define SIM_MPHYS( _pool, _ptr )	((_pool)->paddr + (((char *)_ptr) - (_pool)->vaddr))
#define SIM_VADDR( _pool, _phys )	((_pool)->vaddr + (_phys - (_pool)->paddr))

int simq_ccb_enqueue( SIM_QUEUE *simq, CCB_SCSIIO *ccb );
void simq_ccb_requeue( SIM_QUEUE *simq, CCB_SCSIIO *ccb );
CCB_SCSIIO *simq_ccb_dequeue( SIM_QUEUE *simq );
int simq_ccb_remove( SIM_QUEUE *simq, CCB_SCSIIO *ccb );
CCB_SCSIIO *simq_ccb_locate( SIM_QUEUE *simq, int target, int lun, int tag );
SIM_QUEUE *simq_init( int coid, void *hba, int ntargs, int nluns, int max_non_tagged, int max_tagged, int mactive, int timeout );
int simq_dinit( SIM_QUEUE *simq );
int simq_ccb_state( CCB_SCSIIO *ccb, int state );
void simq_post_ccb( SIM_QUEUE *simq, CCB_SCSIIO *ccb );
void simq_ccb_reset( SIM_QUEUE *simq );
void simq_scsi_reset( SIM_QUEUE *simq );
void simq_reset_dev( SIM_QUEUE *simq, CCB_RESETDEV *ccb );
int simq_ccb_abort( SIM_QUEUE *simq, CCB_ABORT *ccb_abort );
int simq_rel_simq( SIM_QUEUE *simq, CCB_RELSIM *ccb );
void simq_timer( SIM_QUEUE *simq );
void simq_lock( SIM_QUEUE *simq );
void simq_unlock( SIM_QUEUE *simq );
int simq_timer_settime( SIM_QUEUE *simq, int seconds );
void simq_clock_gettime( SIM_QUEUE *simq, struct timespec *t );

SIM_HBA *sim_alloc_hba( int ext_size );
void sim_free_hba( SIM_HBA *sim );
int sim_drvr_options( SIM_HBA *sim, char *options );
int sim_display_config( SIM_HBA *sim );
SIM_HBA *sim_alloc_object( int ext_size );

SIM_MEM_POOL *sim_create_pool( size_t size, int flags );
int sim_destroy_pool( SIM_MEM_POOL *pool );
void *sim_malloc( SIM_MEM_POOL *pool, size_t size );
void sim_free( SIM_MEM_POOL *pool, void *ptr );
void *sim_mem_block( SIM_MEM_POOL *pool, void **free, int num, int size );

#endif
