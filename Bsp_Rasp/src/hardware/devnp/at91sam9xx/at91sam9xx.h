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




#ifndef NIC_AT91SAM9xx_H_INCLUDED
#define NIC_AT91SAM9xx_H_INCLUDED

// Atmel AT91 EMAC Ethernet Controller

#include <hw/sysinfo.h>

#include <io-pkt/iopkt_driver.h>
#include <sys/syslog.h>
#include <sys/cache.h>
#include <sys/slogcodes.h>

#include <drvr/mdi.h>
#include <drvr/eth.h>
#include <drvr/nicsupport.h>
#include <drvr/common.h>


#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_ether.h>
#include <net/if_types.h>

#include <quiesce.h>

#include <sys/device.h>
#include <net/if_media.h>
#include <dev/mii/miivar.h>

#if 0
// make LOG output to console
#define log fprintf
#undef LOG_ERR
#define LOG_ERR stderr
#undef LOG_INFO
#define LOG_INFO stderr
#endif

//
// We will arbitrarily transmit a packet with
// a maximum of 32 fragments (descriptors).  
//
#define AT91SAM9xx_MAX_FRAGS		16

//
// We will arbitrarily leave a buffer of
// 4 empty tx descriptors - we will not try
// to entirely fill the tx descr ring
//
#define AT91SAM9xx_UNUSED_DESCR	4

#define MIN_NUM_RX_DESCRIPTORS  16 * 32
#define MIN_NUM_TX_DESCRIPTORS  64

#define DEFAULT_NUM_RX_DESCRIPTORS  32 * 12   // 1M total cost - 2k mbuf per rx descr
#define DEFAULT_NUM_TX_DESCRIPTORS  32  // 8k total cost - cheap

#define MAX_NUM_RX_DESCRIPTORS  2048
#define MAX_NUM_TX_DESCRIPTORS  2048

#define ETH_MAX_PKT_LEN         1514
#define TX_BUFFER_SIZE              1536

#define EMAC_TIMEOUT     10000

/* Default SRAM address */
#define DEFAULT_SRAM_ADDR           0x00300000
#define DEFAULT_SRAM_SIZE           0x00100000

// TODO: Not sure why leave this memory at the beginning of SRAM
#define SRAM_OFFSET                 0x800

/* Transmit/receive buffer descriptor */

typedef struct {
    uint32_t    buffer;
    uint32_t    status;
} mpc_bd_t;

#define NEXT_TX(x)      ((x + 1) % at91sam9xx->num_tx_descriptors)
#define NEXT_RX(x)      ((x + 1) % at91sam9xx->num_rx_descriptors)

#include <_pack1.h>

//EMAC registers

#define EMAC_NCR 			0x00		/* Network Control Register */
#define EMAC_NCFG			0x01		/* Network Configuration Register */
#define EMAC_NSR			0x02		/* Network Status Register */
#define EMAC_TSR			0x05		/* Transmit Status Register */
#define EMAC_RBQP           0x06		/* Receive Buffer Queue Pointer Register */
#define EMAC_TBQP           0x07    	/* Transmit Buffer Queue Pointer Register */
#define EMAC_RSR			0x08		/* Receive Status Register */
#define EMAC_ISR			0x09		/* Interrupt Status Register */
#define EMAC_IER		 	0x0a		/* Interrupt Enable Register */
#define EMAC_IDR		 	0x0b		/*Interrupt Disable Register */
#define EMAC_IMR			0x0c		/* Interrupt Mask Register */
#define EMAC_MAN			0x0d		/* Phy Maintenance Register */
#define EMAC_PTR			0x0e		/* Pause Time Register */
#define EMAC_PFR			0x0f		/* Pause Frames Received Register */
#define EMAC_FTO			0x10		/* Frames Transmitted OK Register */
#define EMAC_SCF			0x11		/* Single Collision Frames Register */
#define EMAC_MCF			0x12		/* Multiple Collision Frames Register */
#define EMAC_FRO			0x13		/* Frames Received Ok Register */
#define EMAC_FCSE			0x14		/* Frames Check Sequence Errors Register */
#define EMAC_ALE			0x15		/* Alignment Errors Register */
#define EMAC_DTF			0x16		/* Deferred Transmission Frames Register */
#define EMAC_LCOL			0x17		/* Late Collisions Register */
#define EMAC_ECOL			0x18		/* Excessive Collisions Register */
#define EMAC_TUND			0x19		/* Transmit Underrun Errors Register */
#define EMAC_CSE			0x1a		/* Carrier Sense Errors Register */
#define EMAC_RRE			0x1b		/* Receive Resources Errors Register */
#define EMAC_ROV			0x1c		/* Receive Overrun Errors Register */
#define EMAC_RSE			0x1d		/* Receive Symbol Errors Register */
#define EMAC_ELE			0x1e		/* Excessive Length Errors Register */
#define EMAC_RJA			0x1f		/* Receive Jabbers Register */
#define EMAC_USF			0x20		/* Undersize Frames Register */
#define EMAC_STE			0x21		/* SQE Test Errors Register */
#define EMAC_RLE			0x22		/* Received Length Field Mismatch Register */
#define EMAC_HRB			0x24		/* Hash Register Bottom [31 : 0] Register */
#define EMAC_HRT			0x25		/* Hash Register Top [63 : 32] Register */
#define EMAC_SA1B			0x26		/* Specific Address 1 Bottom Register */
#define EMAC_SA1T			0x27		/* Specific Address 1 Top Register */
#define EMAC_SA2B			0x28		/* Specific Address 2 Bottom Register */
#define EMAC_SA2T			0x29		/* Specific Address 2 Top Register */ 
#define EMAC_SA3B			0x2a		/* Specific Address 3 Bottom Register */
#define EMAC_SA3T			0x2b		/* Specific Address 3 Top Register */
#define EMAC_SA4B                       0x2c            /* Specific Address 4 Bottom Register */
#define EMAC_SA4T                       0x2d            /* Specific Address 4 Top Register */
#define EMAC_TID                       	0x2e            /* Type ID Checking Register */
#define EMAC_USRIO                      0x30            /* User Input/Output Register */



/* EMAC Network Control Register Bits definations */
#define EMAC_NCR_LB                     (1 << 0)
#define EMAC_NCR_LLB                    (1 << 1)
#define EMAC_NCR_RE                     (1 << 2)
#define EMAC_NCR_TE                     (1 << 3)
#define EMAC_NCR_MPE                    (1 << 4)
#define EMAC_NCR_CLRSTAT                (1 << 5)
#define EMAC_NCR_INCSTAT                (1 << 6)
#define EMAC_NCR_WESTAT                 (1 << 7)
#define EMAC_NCR_BP                     (1 << 8)
#define EMAC_NCR_TSTAT                  (1 << 9)
#define EMAC_NCR_THALT                  (1 << 10)

/* EMAC Network Status Register Bits definations */
#define EMAC_NSR_MDIO                   (1 << 1)
#define EMAC_NSR_IDLE                   (1 << 2)

/* EMAC Interrupt status/enable/disable/mask register bits defination */
#define EMAC_MFD                        (1 << 0)
#define EMAC_RCOMP                      (1 << 1)
#define EMAC_RXUBR                      (1 << 2)
#define EMAC_TXUBR                      (1 << 3)
#define EMAC_TUN                        (1 << 4)
#define EMAC_RTLE                       (1 << 5)
#define EMAC_TXERR                      (1 << 6)
#define EMAC_TCOMP                      (1 << 7)
#define EMAC_ROVR                       (1 << 10)
#define EMAC_HRESP                      (1 << 11)
#define EMAC_PAFR                       (1 << 12)
#define EMAC_PTZ                        (1 << 13)

/* EMAC Receive status register bits defination */
#define EMAC_RSR_BNA                    (1 << 0)
#define EMAC_RSR_REC                    (1 << 1)
#define EMAC_RSR_OVR                    (1 << 2)

/* EMAC Transmit status register bits defination */
#define EMAC_TSR_UBR                    (1 << 0)
#define EMAC_TSR_COL                    (1 << 1)
#define EMAC_TSR_RLE                    (1 << 2)
#define EMAC_TSR_TGO                    (1 << 3)
#define EMAC_TSR_BEX                    (1 << 4)
#define EMAC_TSR_COMP                   (1 << 5)
#define EMAC_TSR_UND                    (1 << 6)

/* EMAC Network configuration register bits defination */
#define EMAC_NCFG_SPD                   (1 << 0)
#define EMAC_NCFG_FD                    (1 << 1)
#define EMAC_NCFG_JFRAME                (1 << 3)
#define EMAC_NCFG_CAF                   (1 << 4)
#define EMAC_NCFG_NBC                   (1 << 5)
#define EMAC_NCFG_MTI                   (1 << 6)
#define EMAC_NCFG_UNI                   (1 << 7)
#define EMAC_NCFG_BIG                   (1 << 8)
#define EMAC_NCFG_CLK8                  (0 << 10)
#define EMAC_NCFG_CLK16                 (1 << 10)
#define EMAC_NCFG_CLK32                 (2 << 10)
#define EMAC_NCFG_CLK64                 (3 << 10)
#define EMAC_NCFG_RTY                   (1 << 12)
#define EMAC_NCFG_PAE                   (1 << 13)
#define EMAC_NCFG_RBOF0                 (0 << 14)
#define EMAC_NCFG_RBOF1                 (1 << 14)
#define EMAC_NCFG_RBOF2                 (2 << 14)
#define EMAC_NCFG_RBOF3                 (3 << 14)
#define EMAC_NCFG_RLCE                  (1 << 16)
#define EMAC_NCFG_DRFCS                 (1 << 17)
#define EMAC_NCFG_EFRHD                 (1 << 18)
#define EMAC_NCFG_IRXFCS                (1 << 19)

/* User Input/Output Register Bits definition */
#define EMAC_USRIO_RMII			(1 << 0)
#define EMAC_USRIO_CLKEN		(1 << 1)
		


#define EMAC_RBD_DONE             	(1 << 0)
#define EMAC_RBD_WRAP             	(1 << 1)
#define EMAC_RBD_SOF              	(1 << 14)
#define EMAC_RBD_EOF              	(1 << 15)

#define EMAC_TBD_FREE             	(1 << 31)
#define EMAC_TBD_WRAP             	(1 << 30)
#define	EMAC_TBD_RETRY				(1 << 29)
#define EMAC_TBD_UN 	       	  	(1 << 28)
#define	EMAC_TBD_BUFFERS			(1 << 27)
#define EMAC_TBD_LAST             	(1 << 15)
#define EMAC_TBD_LENGTH(len)      	(len & ((1 << 11) - 1))
#define	TX_ERROR	(EMAC_TBD_RETRY | EMAC_TBD_UN | EMAC_TBD_BUFFERS)

#define EMAC_RBD_BCAST    		(1 << 31)
#define EMAC_RBD_MCAST    		(1 << 30)
#define EMAC_RBD_UCAST    		(1 << 29)
#define EMAC_RBD_LEN_MASK 		0x7FF

#define RX_BUF_SIZE	          	128
#define EMAC_MAX_FRAME_LEN       	1520
#define EMAC_NUM_TX_BD            	64
#define EMAC_DEFRAG_PACKET            	0x1000000

#define NIC_PRIORITY                	21
#define NIC_MDI_PRIORITY            	10

#define EMAC_DATA       		(0xFFFF <<  0)
#define EMAC_CODE       		(0x3 << 16)
#define EMAC_REGA       		(0x1F << 18)
#define EMAC_PHYA       		(0x1F << 23)
#define EMAC_RW         		(0x3 << 28)
#define EMAC_SOF        		(0x3 << 30)

#include <_packpop.h>


typedef struct _nic_at91sam9xx_ext {
	struct device			dev;
	struct ethercom         	ecom;

	struct _iopkt_inter     	inter_rx;
	int							iid_rx;
	int							irq;
	void                        *sdhook;
	void                        *isrp_rx;

	nic_config_t				cfg;
	nic_stats_t     			stats;

	struct _iopkt_self			*iopkt;

	//
	// rx
	//
	int							rxd_pkts; 	
	int							num_rx_descriptors;
    mpc_bd_t        			*rx_bd;
    int							rx_cidx;
	struct mbuf					**rx_pkts;
	int							rx_cap_mask;			// hardware checksumming


	//
	// cmd line args and state variables
	//
	int							kermask;
    	uint32_t    				use_syspage;            // Whether or not the system page should
                                            			// information.
	int							dying;
#ifndef NDEBUG
	int							spurious;
#endif
    uintptr_t     				iobase;
	struct cache_ctrl       cachectl;
    uintptr_t       			phy_base;
    uint32_t        			*reg;
    uint32_t        			*phy_reg;

	//
	// mii
	//
	struct callout          	mii_callout;
	struct callout				hk_callout;
	int							start_running;
	int							hk;
	int							force_advertise;
	int							linkup;
	mdi_t						*mdi;
    	uint32_t        			probe_phy;  
	struct mii_data				bsd_mii;        		// for media devctls

	//
	// tx variables - hopefully a cache line or two away from
	// the rx variables above
	//
	int							num_tx_descriptors;		// size of tx ring
    mpc_bd_t 					*tx_bd;					// ptr to mmap tx descr
    int             			tx_descr_inuse;			// number of filled descr
    int    					    tx_pidx;				// descr producer index
    int             			tx_cidx;				// descr consumer index
	struct mbuf					**tx_pkts;				// array of mbufs for each descr
	void 						*tx_buffers;
	uint32_t					tx_phys;
	uint32_t					tx_fcb_phys;			// physical address of fcb array
	int							tx_cap_mask;			// hardware checksumming
	int							sram_buffers;
	uint32_t					sram_addr;
	uint32_t					sram_size;
} at91sam9xx_dev_t;



// these are the interrupts which we unmask and handle.  Note no tx complete interrupt
//#define AT91SAM9xx_ENABLED_IMASK  (EMAC_RCOMP | EMAC_RXUBR | EMAC_TUN | EMAC_RTLE | EMAC_TXERR | EMAC_TCOMP | EMAC_ROVR | EMAC_HRESP)
#define AT91SAM9xx_ENABLED_IMASK  (EMAC_RCOMP | EMAC_RXUBR | EMAC_RTLE | EMAC_ROVR | EMAC_HRESP)

// indexes into cfg.irq 
#define AT91SAM9xx_RX_INT  0

// event.c
const struct sigevent * at91sam9xx_isr_rx(void *, int);
const struct sigevent * at91sam9xx_isr_rx_kermask(void *, int);
int at91sam9xx_enable_rx(void *arg);
int at91sam9xx_enable_rx_kermask(void *arg);
int at91sam9xx_process_interrupt(void *arg, struct nw_work_thread *);
void at91sam9xx_hk_callout(void *arg);

// detect.c
int at91sam9xx_detect(void *dll_hdl, struct _iopkt_self *iopkt, char *options);
void at91sam9xx_speeduplex(at91sam9xx_dev_t *);
void at91sam9xx_rx_set_mode(at91sam9xx_dev_t *at91sam9xx);

/* devctl.c */
int at91sam9xx_ioctl(struct ifnet *, unsigned long, caddr_t);

// transmit.c
void at91sam9xx_start(struct ifnet *);
void at91sam9xx_transmit_complete(at91sam9xx_dev_t *);

// receive.c
void at91sam9xx_receive(at91sam9xx_dev_t *, struct nw_work_thread *);

// mii.c
void at91sam9xx_MDI_MonitorPhy(void *);
int at91sam9xx_init_phy(at91sam9xx_dev_t *);

// stats.c
void at91sam9xx_update_stats(at91sam9xx_dev_t *);
void at91sam9xx_clear_stats(at91sam9xx_dev_t *);

// bsd_media.c
void bsd_mii_initmedia(at91sam9xx_dev_t *);

#endif

__SRCVERSION( "$URL$ $REV$" )
