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



/*
 *  io-char.h     Character device library interface.
 *

 */

#ifndef __IO_CHAR_H_INCLUDED
#define __IO_CHAR_H_INCLUDED


struct ttydev_entry;

#ifdef __IOFUNC_H_INCLUDED
#error sys/iofunc.h already included
#endif
#define IOFUNC_ATTR_T		struct ttydev_entry
#include <sys/iofunc.h>
#include <sys/ioctl.h>
#include <sys/pm.h>
#include <ps/ps.h>
#include <pthread.h>

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#ifndef _TERMIOS_H_INCLUDED
#error 'termios.h' should be included first
#endif

__BEGIN_DECLS

/* Flag definitions */
#define EVENT_QUEUED		0x0000001
#define EVENT_SIGINT		0x0000002
#define EVENT_SIGHUP		0x0000004
#define EVENT_TTO			0x0000008
#define EVENT_READ			0x0000010
#define EVENT_WRITE			0x0000020
#define EVENT_DRAIN			0x0000040
#define EVENT_TIMEOUT		0x0000080
#define EVENT_NOTIFY_INPUT	0x0000100
#define EVENT_NOTIFY_OUTPUT	0x0000200
#define EVENT_NOTIFY_OBAND	0x0000400
#define EVENT_CARRIER		0x0000800
#define EVENT_SIGQUIT		0x0004000
#define EVENT_SIGSUSP		0x0008000

#define OHW_PAGED			0x0001000	/* OHW_PAGED and OSW_PAGED flags share the same bits */
#define IHW_PAGED			0x0002000	/* IHW_PAGED and ISW_PAGED flags share the same bits */
#define OSW_PAGED			0x0001000
#define ISW_PAGED			0x0002000
#define EDIT_INSERT			0x0010000
#define EDIT_PREFIX			0x0020000
#define OBAND_DATA			0x0040000
#define LOSES_TX_INTR		0x0080000
#define TIMER_ACTIVE		0x0100000
#define TIMER_KEEP			0x0200000
#define NOTTY				0x0400000	/* Used by Pty's */
#define NL_INSERT			0x0800000	
#define ISAPTY				0x1000000	/* Is a pty */
#define DEV_ACTIVE			0x2000000	/* Used by pty's to indicate device history */
#define LITERAL				0x4000000
#define FIRST_TIME_ALONE	0x8000000	/* Many->One reader transition (see wait.c) */

/* Extra/Extended flags definitions */
#define  OSW_PAGED_OVERRIDE  0x0000001   /* Software flow control override (when set allow transmit of one byte when output is suspended) */
#define  MODEM_DISCONNECT    0x0000002   /* Used to prevent read/write while in a disconnected state */ 
#define  IBUF_TRANSITION     0x0000004	 /* Used to flag when the input buffer has changed from empty to non-empty */
#define  SERCTL_INPROG		 0x0000008	 /* Used to hold of write requests until current SERCTL operation is complete */

/* Logging Flags */
#define  LOG_LAST_IPAGED     0x0000001    /* Last logged state for input flow control (set/cleared) */
#define  LOG_LAST_OPAGED     0x0000002    /* Last logged state for output flow control (set/cleared) */
#define  LOG_BREAK           0x0000010
#define  LOG_INTR            0x0000020
#define  LOG_TX_DATA         0x0000040    /* Log TX data to file */
#define  LOG_RX_DATA         0x0000080	  /* Log RX data to file */

/* Event Masks */
#define  IS_ERROR_EVENT      0x000000f

/* Event flags */
#define  EVENT_PARITY      0x0000001    /* Parity Error  */
#define  EVENT_FRAME       0x0000002    /* Framing Error */
#define  EVENT_OVERRUN     0x0000004    /* HW fifo Overrun Error */
#define  EVENT_OVERRUN_SW  0x0000008    /* SW buffer Overrun Error */
#define  EVENT_IPAGED      0x0000010    /* Change in input flow control state */
#define  EVENT_OPAGED      0x0000020    /* Change in output flow control state */
#define  EVENT_OPEN        0x0000040    /* Open event to service clients on the waiting_open list */
#define  EVENT_TIMER_QUEUE 0x0000080	/* Queue a timer */
#define  EVENT_CUSTOM      0x0000100	/* Custom driver event callout */
#define  EVENT_FLUSH_TX_LOG 0x0000200    /* Write logged TX data to file */
#define  EVENT_FLUSH_RX_LOG 0x0000400    /* Write logged RX data to file */

/* Verbosity Levels  */
#define  EVENT 3
#define  POWER 5


/* These codes must start above an 8 bit character code. */
enum {
	TTI_BREAK    = 0x100,
	TTI_OVERRUN  = 0x200,
	TTI_FRAME    = 0x300,
	TTI_PARITY   = 0x400,
	TTI_CARRIER  = 0x500,
	TTI_HANGUP   = 0x600,
	TTI_OHW_STOP = 0x700,
	TTI_OHW_CONT = 0x800,
	TTI_OSW_STOP = 0x900,
	TTI_OSW_CONT = 0xa00,
	TTI_QUIT     = 0xb00,
	TTI_SUSP     = 0xc00,
	TTI_INTR     = 0xd00
	};

enum {
	TTO_DATA,
	TTO_STTY,
	TTO_CTRL,
	TTO_EVENT,
	TTO_LINESTATUS
	};

enum {
	TTC_INIT_PROC,
	TTC_INIT_CC,
	TTC_INIT_START,
	TTC_INIT_RAW,
	TTC_INIT_EDIT,
	TTC_INIT_ATTACH,
	TTC_SET_OPTION,
	TTC_TIMER_QUEUE,
	TTC_INIT_PTY,
	TTC_INIT_TTYNAME,
	TTC_TTY_DETACH,
	TTC_INIT_POWER
	};

/*TTC_INIT_ATTACH flags */
#define NUMBER_DEV_FROM_INODE	0x80000000	/*Device has number appended to it from inode (0based)*/
#define NUMBER_DEV_1_BASED		0x40000000	/*Device has number from inode incremented by 1*/
#define NUMBER_DEV_FROM_USER	0x20000000	/*Device has a number specified by the user*/
#define USE_GENERIC_CLASS		0x00800000	/*Use the generic "device" class*/

#if 0 /* This is off until we are more sure the dev specific locks work */
#define dev_lock(dev)		(InterruptLock(&(dev)->lock))
#define dev_unlock(dev)		(InterruptUnlock(&(dev)->lock))
#else
#define dev_lock(dev)		(InterruptLock(&ttyctrl.lock))
#define dev_unlock(dev)		(InterruptUnlock(&ttyctrl.lock))
#endif

#define IO_CHAR_COMMON_OPTIONS	"veEfFsSC:I:O:o:"
#define IO_CHAR_SERIAL_OPTIONS	IO_CHAR_COMMON_OPTIONS "b:"
#define IO_CHAR_DEFAULT_BITSIZE 12
#define IO_CHAR_PMM_NAME_MAX	256 	
#define IO_CHAR_USEC_PER_SEC 	1000000  /* Number of useconds in a second */

#define SET_NAME_NUMBER(x)		(((x) & 0x3ff) << 10)	/* 10 bit name number */
#define GET_NAME_NUMBER(x)		(((x) >> 10) & 0x3ff)

/* Define flags for power init */
#define PWR_INIT_DRVREG			0x0000001 	/* Register as a power managed device */
#define PWR_INIT_DRVPSTORE		0x0000002 	/* Driver wants its own persistent storage object */
#define PWR_INIT_DRVSAVE		0x0000004 	/* Driver wants to store data for HWVOL or NORAM */
#define PWR_INIT_DRVOVERRIDE	0x0000008 	/* Driver will assume all setpower functionality */
#define PWR_INIT_DRVNOFLOWCTL	0x0000010 	/* Driver should delay before power down for flow control */
#define PWR_INIT_NOPMM			0x0001000 	/* Indicates that there is not power manager running on init*/

/* Define power flags */
#define PWR_WAITING_DRAIN		0x0000001 	/* Power change is waiting for device to drain */
#define PWR_FLOW_PAGED			0x0000002 	/* Device is HW or SW paged due to power */
#define PWR_EVENT_DEVCTL		0x0001000 	/* Check devctl blocking queue */
#define PWR_EVENT_DEVWAKEUP		0x0002000 	/* Device wants to wakeup due to an external event such as a device wakeup event (this is NOT CPU wakeup) */

/* Define flags for hardware power mode capabilities */
#define PM_MODE_HW_NONE			0x0000000 	/* power mode does not support any hw access */
#define PM_MODE_HW_TX			0x0000001 	/* power mode allows transmit */
#define PM_MODE_HW_RX			0x0000002 	/* power mode allows receive */
#define PM_MODE_HW_CTRL			0x0000004 	/* power mode allows control over RTS/DTR lines */
#define PM_MODE_HW_REGREAD		0x0000008 	/* power mode allows register read */
#define PM_MODE_HW_REGWRITE		0x0000010 	/* power mode allows register write */
#define PM_MODE_HW_ALL 			0xFFFFFFF	/* power mode allows full device access */
#define PM_MODE_HW_RXTX			(PM_MODE_HW_RX | PM_MODE_HW_TX)
#define PM_MODE_HW_REG 			(PM_MODE_HW_REGREAD | PM_MODE_HW_REGWRITE)
#define PM_MODE_HW_DRAIN		(PM_MODE_HW_REGREAD | PM_MODE_HW_TX)
#define PM_MODE_HW_OFF 			PM_MODE_HW_NONE	 	/* OFF modes should not allow any HW access */
#define PM_MODE_HW_ACTIVE		PM_MODE_HW_ALL 		/* ACTIVE modes should allow full HW access */

#define OBUF_SAFETY   5  /* extra obuf space to allow injection of software flow control characters */

typedef struct ttyinit_entry {
	_Paddr64t		port;
	unsigned		port_shift;
	unsigned		intr;
	int				baud;
	int				isize;
	int				osize;
	int				csize;
	int				c_cflag;
	int				c_iflag;
	int				c_lflag;
	int				c_oflag;
	int				fifo;
	int				clk;
	int				div;
	char			name[TTY_NAME_MAX];
	char			*pmm_parent; 	/* power manager namespace */
	unsigned		pmflags;		/* common power management flags */	
	int      		verbose; /* Driver verbosity */
	int      		highwater;	/* ibuf highwater mark for flow control */
	char    		logging_path[_POSIX_PATH_MAX];
	unsigned		lflags;  		/* Logging flags */
} TTYINIT;

typedef struct pm_dev {
    struct pm_dev   		*next;
	struct ttydev_entry 	*dev;
} PM_DEV;

typedef struct ttybuf_entry {
	int			 	cnt;
	int			 	size;
	unsigned char	*buff;
	unsigned char	*head;
	unsigned char	*tail;
} TTYBUF; 

#define  DEBUG_BREAK         0x0000001
#define  NODAEMONIZE         0x0000002

typedef struct chario_entry {
	dispatch_t		*dpp;
	int				coid;
	int				timerid;
	int 			pm_pulse;
	unsigned		max_devs;
	unsigned		num_devs;
	struct sigevent	event;
	struct sigevent	timer;
	struct ttydev_entry	*timer_list;
	unsigned		num_events;
	struct ttydev_entry	**event_queue;
	intrspin_t		lock;
	PM_DEV  		*pm_dev_list;
	unsigned		flags;
	unsigned		perm;
	pthread_mutex_t timer_mutex;
} TTYCTRL;

typedef struct ttywait_entry {
	struct ttywait_entry 	*next;
	int			rcvid;
	int			scoid;
	int			coid;
	int			offset;
	int			nbytes;
	pid_t		pid;
	int			tid;
} TTYWAIT;

typedef struct ttyinitpty_entry {
	TTYWAIT *(*wait_add)(TTYWAIT **, resmgr_context_t *, int);
} TTYINITPTY;

/* io-char's data to save in persistent storage */
typedef struct psdata_entry {
	int						c_cflag;
	int						c_iflag;
	int						c_lflag;
	int						c_oflag;
    cc_t			 		c_cc[NCCS];
	int			 			baud;
} PSDATA;

typedef struct ttyinitpower_entry {
	pmd_setpower_t          setpower;	/* driver specific setpower */
	void 					(*devstate)(struct ttydev_entry *ttydev); /* save driver state */
	pmd_mode_attr_t         *modes;		/* list of power modes supported by driver */
	int                     nmodes;		/* number of power modes supported by driver */
	pm_power_mode_t         init_mode;	/* driver's initial power mode */
	char					*pmm_parent; /* power manager namespace */
	uint64_t 				psunique;	/* unique value to generate pstore id - use port */ 
	void 					*psdata;	/* driver specific persistent storage */
	size_t 					pssize;		/* size of driver specific persistent storage */
} TTYINITPOWER;

typedef struct ttypstore_entry {
	ps_handle_t 			*hdl;		/* persistent storage handle */
	int						instance;	/* persistent storage instance */
	void 					*data;		/* data to store */
	size_t 					size;		/* size of data to store */
} TTYPSTORE;

typedef struct ttypower_entry {
	pmd_attr_t              pmd;		/* device's power management structure */
	struct sigevent 		event;		/* event for power pulse handler */
	pmd_setpower_t 			iochar_setpower; /* io-char's generic setpower */
	pmd_setpower_t 			driver_setpower; /* driver specific setpower */
	void 					(*devstate)(struct ttydev_entry *ttydev); /* save driver state */
	pmd_mode_attr_t 		*modes; 	/* list of power modes supported by driver */
	int 					nmodes; 	/* number of power modes supported by driver */
	pm_power_mode_t 		init_mode;	/* driver's initial power mode */
	unsigned 				init_flags;	/* power management init flags */
	volatile unsigned 		flags;		/* power management flags */
	char					*pmm_parent; /* power manager namespace */
	char					pmm_name[IO_CHAR_PMM_NAME_MAX]; /* power manager path name */
	ps_objid_t 				psid;		/* unique persistent storage id */
	TTYPSTORE				*pschar; 	/* io-char persistent storage */
	TTYPSTORE				*psdev;		/* device's persistent storage */
} TTYPOWER; 

typedef struct ttydev_entry {
	iofunc_attr_t			attr;
	iofunc_mount_t			mount;
	TTYPOWER 				power;
	TTYWAIT					*waiting_read;
	TTYWAIT					*waiting_write;
	TTYWAIT					*waiting_drain;
	TTYWAIT					*waiting_devctl;
	int						c_cflag;
	int						c_iflag;
	int						c_lflag;
	int						c_oflag;
	volatile unsigned		flags;
	volatile unsigned		xflags;  /* Extended flags, for miscellaneous flags */
	volatile unsigned		eflags;  /* Event flags, extention to the events in the flags variable */
	volatile unsigned		lflags;  /* Logging flags */
	int						verbose; /* Driver verbosity */
	unsigned int			bcnt;
	unsigned int			fwdcnt;
	struct ttydev_entry		*timer;
	int			 			timeout;
	int			 			timeout_reset;
	union {
		/* Note: io-char requires members of the union are the same size */
		uint64_t			tmrs;
		struct {
			char 			drn_tmr;	/* drain timer */
			char 			tx_tmr;		/* loses tx interrupt timer */
			char 			brk_tmr;	/* break timer */
			char 			dtr_tmr;	/* dtr line timer */
			char 			spare_tmr;	/* spare timer for driver use ONLY */
			char 			dsr_tmr;	/* for device side driver where DSR is an output */
			char 			dcd_tmr;	/* for device side driver where DCD is an output */
			char 			rsvd3; 		/* Reserved for more timers to be added */
		}	s;
	} 		un;
	pid_t					brkpgrp;
	pid_t			 		huppid;
    cc_t			 		c_cc[NCCS];
	unsigned char		 	fifo;
	unsigned char		 	fwd;
	unsigned char		 	prefix_cnt;
	unsigned char		 	oband_data;
	int			 			highwater;
	int			 			baud;
	struct winsize			winsize;
	TTYBUF	 				obuf;
	TTYBUF	 				obuf_log;
	TTYBUF	 				ibuf;
	TTYBUF	 				ibuf_log;
	TTYBUF	 				cbuf;
	iofunc_notify_t		 	notify[3];
	struct ttydev_entry		*extra;
	TTYWAIT					*waiting_open;
	int						linkid;			/* id returned from resmgr_attach */
	void					*reserved2;		/* reserved for use by io-char */
	int						(*io_devctlext)(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);
	int						(*custom_event_handler) (struct ttydev_entry *tty);
	char			 		name[TTY_NAME_MAX];
	char                    *logging_path;
	int                     tx_log_fd;
	int                     rx_log_fd;
	/* supress flooding slog with errors */
	volatile unsigned 		shush; 
	} TTYDEV;

extern int  drain_check(TTYDEV *dev, uintptr_t *cnt);

extern int	ttc(int type, void *ptr, int arg);
extern int 	tti(TTYDEV *dev, unsigned c);
extern int 	tti2(TTYDEV *dev, unsigned char *char_buf, int len, uint16_t err_code);
extern int 	tto(TTYDEV *dev, int action, int arg);
extern void kick(TTYDEV *dev);
extern void tx_inject(TTYDEV *dev, unsigned char c);
extern int iochar_regdrv_power(TTYDEV *dev, TTYINITPOWER *init, unsigned flags);
extern int devctl_wait_power(resmgr_context_t *ctp, TTYDEV *dev, int nonblock, uint32_t drvflag);
extern void iochar_send_event(TTYDEV *dev);
extern TTYWAIT *wait_add(TTYWAIT **queue, resmgr_context_t *ctp, int offset);
extern int wait_remove_rcvid(TTYWAIT **head, int rcvid, int *offset);
extern unsigned char tto_getchar( TTYDEV *dev);
extern int tto_write_block(TTYDEV *dev, char *buf, int buf_size, int *byte_cnt);
extern int tto_checkclients(TTYDEV *dev);
extern void timer_queue(TTYDEV *dev);
extern void timer_remove(TTYDEV *dev);

extern resmgr_io_funcs_t		iochar_io_funcs;
extern resmgr_connect_funcs_t	iochar_connect_funcs;

__END_DECLS

#endif



__SRCVERSION( "$URL: http://svn/product/tags/internal/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/1.0.0/latest/lib/io-char/public/sys/io-char.h $ $Rev: 642129 $" )
