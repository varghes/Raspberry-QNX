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
#include <device_qnx.h>
#include <sys/rsrcdbmgr.h>


static void at91sam9xx_stop(struct ifnet *ifp, int disable);
static void at91sam9xx_destroy(at91sam9xx_dev_t *at91sam9xx, int how);
static void at91sam9xx_reset(at91sam9xx_dev_t *at91sam9xx);
static int at91sam9xx_init(struct ifnet *ifp);
static void at91sam9xx_rx_mbuf_free(at91sam9xx_dev_t *at91sam9xx);

static int at91sam9xx_attach(struct device *, struct device *, void *);
static int at91sam9xx_detach(struct device *, int);

static void at91sam9xx_shutdown(void *);



struct emac_params {
	paddr_t		 paddr;
	unsigned int	irq_present;
	unsigned int	irq_rx;
	unsigned int	mac_present;
	unsigned int	sram_params;
	unsigned char   mac[6];
	unsigned int	phy_present;
	unsigned int	phy_addr;
};

struct at91sam_arg {
	void			*dll_hdl;
	char			*options;
	int			idx;
	uint32_t		iobase;
	struct emac_params	  *emac_p;
};


CFATTACH_DECL(at91sam9xx,
	sizeof(at91sam9xx_dev_t),
	NULL,
	at91sam9xx_attach,
	at91sam9xx_detach,
	NULL);

static  char *at91sam9xx_opts [] = {
	  "receive",		// 0 
	  "transmit",	   // 1
	  "phy_addr",	   // 2
	  "syspage",		// 3
	  "kermask",		// 4
	  "sram_addr",		// 5
	  "sram_size",		// 6
	  NULL
};


//
// called from at91sam9xx_detect()
//
static int  
at91sam9xx_parse_options (at91sam9xx_dev_t *at91sam9xx, char *options, nic_config_t *cfg)
{
	char	*value, *restore, *c;
	int	 opt;
	int	 rc = 0;
	int	 err = EOK;


	restore = NULL;
	while (options && *options != '\0') {
		c = options;
		restore = strchr(options, ',');
		opt = getsubopt (&options, at91sam9xx_opts, &value);

		switch (opt) {
			case  0:
				if (at91sam9xx) {
					at91sam9xx->num_rx_descriptors = strtoul (value, 0, 0);
					}
				break;
			case  1:
				if (at91sam9xx) {
					at91sam9xx->num_tx_descriptors = strtoul (value, 0, 0);
					}
				break;
			case  2:
				if (cfg) {
					cfg->phy_addr = strtoul(value, 0, 0);
					}
				break;
			case  3:
				if (at91sam9xx) {
					at91sam9xx->use_syspage = 1;
					}
				break;
			case  4:
				if (at91sam9xx) {
					if (value) {
						at91sam9xx->kermask = strtoul(value, 0, 0);
						if (at91sam9xx->kermask) {
							at91sam9xx->kermask = 1;
							}
						}
					}
				break;
			case	5:
				if (at91sam9xx) {
					if (value) {
						at91sam9xx->sram_addr = strtoul (value, 0, 0);
						}
					}
				break;
			case	6:
				if (at91sam9xx) {
					if (value) {
						at91sam9xx->sram_size = strtoul (value, 0, 0);
						}
					}
				break;
			default:
				if (nic_parse_options (cfg, value) != EOK) {
					log(LOG_ERR, "%s(): unknown option %s", __FUNCTION__, c);
					err = EINVAL;
					rc = -1;
					}
				break;
			}
		if (restore != NULL)
			*restore = ',';
		}

	errno = err;
	return (rc);
}


//
// convert virtual to physical address
//
static paddr_t 
vtophys (void *addr)
{
	off64_t  offset;

	if (mem_offset64 (addr, NOFD, 1, &offset, 0) == -1) {
		return (-1);
		}
	return (offset);
}

static int 
at91sam9xx_cal_sram(int num_buf)
{
    int offset = SRAM_OFFSET;

    offset += (num_buf * sizeof (mpc_bd_t));
    offset = (offset + 3) & ~0x3;

    offset += (num_buf * sizeof (struct mbuf *));
    offset = (offset + 3) & ~0x3;

    offset += (num_buf * TX_BUFFER_SIZE);
    return offset;
}


static int
at91sam9xx_alloc_sram (at91sam9xx_dev_t *at91sam9xx)
{
    rsrc_alloc_t    alloc;
    rsrc_request_t  rreq;

    at91sam9xx->sram_addr += SRAM_OFFSET;

    /* 
     * The layout of the SRAM
     *  SRAM_OFFSET     TODO: is it necessory?
     *  BDs:            at91sam9xx->num_tx_descriptors * sizeof (mpc_bd_t)
     *  mbuf pointers:  at91sam9xx->num_tx_descriptors * sizeof(struct mbuf *)
     *  tx Buffers:     at91sam9xx->num_tx_descriptors * TX_BUFFER_SIZE
    */

    // If the size of SRAM is not large enough, use smaller number of transmission buffers
    if ( at91sam9xx_cal_sram(at91sam9xx->num_tx_descriptors) > at91sam9xx->sram_size) {

        int num_buf = (at91sam9xx->sram_size - SRAM_OFFSET) / (sizeof (mpc_bd_t) + sizeof (struct mbuf *) + TX_BUFFER_SIZE);

        // Just to make sure 
        if (at91sam9xx_cal_sram(num_buf) > at91sam9xx->sram_size)
            num_buf -= 1;

        slogf (_SLOGC_NETWORK, _SLOG_WARNING, "SRAM is not large enough to hold %d transmission buffers, using %d instead", at91sam9xx->num_tx_descriptors, num_buf);

        at91sam9xx->num_tx_descriptors = num_buf;
        }

    /* 
     * Seed the SRAM, a way to make sure the SRAM won't be shared 
     * by other instances of devnp-at91sam9xx.so 
     */
    alloc.start = at91sam9xx->sram_addr;
    alloc.end = at91sam9xx->sram_addr + at91sam9xx->sram_size;
    alloc.flags = RSRCDBMGR_FLAG_NAME;
    alloc.name = "sram_buffer";
    if (rsrcdbmgr_create (&alloc, 1) == -1) {
        slogf (_SLOGC_NETWORK, _SLOG_WARNING, "Unable to seed SRAM - Start 0x%llx - End 0x%llx, won't use SRAM as the transmission buffers", alloc.start, alloc.end);

        return -1;
    } 

    /* Reserve the SRAM memory for this process */
    memset ((char *) &rreq, 0, sizeof (rreq));
    rreq.length = at91sam9xx->sram_size;
    rreq.start = at91sam9xx->sram_addr;
    rreq.end = at91sam9xx->sram_addr + at91sam9xx->sram_size;
    rreq.flags = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_RANGE;
    rreq.name = "sram_buffer";
    if (rsrcdbmgr_attach (&rreq, 1) != EOK) {
        rsrcdbmgr_destroy(&alloc, 1);
        slogf (_SLOGC_NETWORK, _SLOG_WARNING, "Unable to reserve SRAM - Start 0x%llx - End 0x%llx, won't use SRAM as the transmission buffers", alloc.start, alloc.end);

        return -1;
        } 

    slogf (_SLOGC_NETWORK, _SLOG_INFO, "Setting SRAM for TX buffers- Start 0x%x - End 0x%x, %d TX buffers", 
        at91sam9xx->sram_addr, at91sam9xx->sram_addr + at91sam9xx->sram_size - SRAM_OFFSET - 1, at91sam9xx->num_tx_descriptors);
    return 0;
}

//
// called from at91sam9xx_detect() to allocate resources
//

static int
at91sam9xx_attach(struct device *parent, struct device *self, void *aux)
{
	at91sam9xx_dev_t		*at91sam9xx;
	nic_config_t	*cfg;
	int							 rc, idx;
	struct				  ifnet *ifp;
	size_t				  size;
	char					*options;
	uint32_t				iobase;
	struct  _iopkt_self	 *iopkt;
	struct at91sam_arg	  *at91sam_arg;
	int					 single;
	struct emac_params	  *emac_params;
    int offset = 0;

	iopkt = iopkt_selfp;
	at91sam9xx = (at91sam9xx_dev_t *)self;
	at91sam_arg = aux;
	emac_params = at91sam_arg->emac_p;

	options = at91sam_arg->options;
	idx = at91sam_arg->idx;
	iobase = emac_params->paddr;

	at91sam9xx->sram_buffers = emac_params->sram_params;
	at91sam9xx->dev.dv_dll_hdl = at91sam_arg->dll_hdl;
	at91sam9xx->iopkt = iopkt;
	at91sam9xx->iid_rx = -1;

	ifp = &at91sam9xx->ecom.ec_if;
	ifp->if_softc = at91sam9xx;

	cfg = &at91sam9xx->cfg;

	if (cfg->verbose > 3) {
		log(LOG_ERR, "%s(): starting: idx %d\n", __FUNCTION__, idx);
		}

	if ((at91sam9xx->sdhook = shutdownhook_establish(at91sam9xx_shutdown,
									at91sam9xx)) == NULL) {
		return ENOMEM;
		}

	at91sam9xx->iobase = at91sam9xx->phy_base = (uintptr_t) iobase;
	cfg->irq[0] = emac_params->irq_rx;

	// set default mac address
	cfg->current_address[0] = emac_params->mac[0];
	cfg->current_address[1] = emac_params->mac[1];
	cfg->current_address[2] = emac_params->mac[2];
	cfg->current_address[3] = emac_params->mac[3];
	cfg->current_address[4] = emac_params->mac[4];
	cfg->current_address[5] = emac_params->mac[5];

	/* set some defaults for the command line options */
	//	  cfg->flags = NIC_FLAG_MULTICAST;
	cfg->media_rate = cfg->duplex = -1;
	cfg->priority = IRUPT_PRIO_DEFAULT;
	cfg->iftype = IFT_ETHER;
	cfg->lan = -1;
	strcpy((char *)cfg->uptype, "en");
	cfg->verbose = 0;  // XXX debug - set verbose=0 for normal output

	at91sam9xx->num_tx_descriptors = DEFAULT_NUM_TX_DESCRIPTORS;
	at91sam9xx->num_rx_descriptors = DEFAULT_NUM_RX_DESCRIPTORS;

	at91sam9xx->kermask = 1;  // XXX debug

	cfg->device_index = 0xFFFFFFFF;
	at91sam9xx->sram_addr = 0xffffffff;
	at91sam9xx->sram_size = 0xffffffff;
	if ((rc = at91sam9xx_parse_options(at91sam9xx, options, cfg))) {
		log(LOG_ERR, "%s(): at91sam9xx_parse_options() failed: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 1);
		return rc;
		}

    //
    // Because the start addr and size are different in different at91sam9xx procrssors
    // Care needs to be taken when start the driver with sram_addr or sram_size option
    //
    if (at91sam9xx->sram_addr != 0xffffffff || at91sam9xx->sram_size != 0xffffffff) {
        if (at91sam9xx->sram_addr == 0xffffffff) {
            at91sam9xx->sram_addr = DEFAULT_SRAM_ADDR;
            slogf (_SLOGC_NETWORK, _SLOG_WARNING, "%s(): sram_addr not specified, setting default SRAM address to 0x%x",
                __FUNCTION__, at91sam9xx->sram_addr);
            }    
        else if (at91sam9xx->sram_size == 0xffffffff) {
            at91sam9xx->sram_size = DEFAULT_SRAM_SIZE;
            slogf (_SLOGC_NETWORK, _SLOG_WARNING, "%s(): sram_size not specified, setting default SRAM size to 0x%x",
                __FUNCTION__, at91sam9xx->sram_size);
            }

        at91sam9xx->sram_buffers = at91sam9xx_alloc_sram (at91sam9xx) ? 0 : 1;
		}

	cfg->lan = at91sam9xx->dev.dv_unit;
	single = (cfg->device_index != 0xFFFFFFFF);
	cfg->device_index = idx;

	if (cfg->verbose) {
		log(LOG_ERR, "%s(): IF %d: Base register 0x%08X",
						__FUNCTION__, idx, at91sam9xx->iobase);
		log(LOG_ERR, "%s(): IF %d: IRQ %2d",
						__FUNCTION__, idx, cfg->irq[0]);
		log(LOG_ERR, "%s(): IF %d: PHY address 0x%08x",
						__FUNCTION__, idx, at91sam9xx->phy_base);
		}

	// set defaults - only used by nicinfo to display mtu
	cfg->mtu = ETH_MAX_PKT_LEN;
	//cfg->mtu = cfg->mru = ETH_MAX_PKT_LEN;
//	cfg->mru = RX_BUF_SIZE;
	cfg->mru = ETH_MAX_PKT_LEN;

	cfg->media = NIC_MEDIA_802_3;
	cfg->mac_length = ETH_MAC_LEN;
	at91sam9xx->force_advertise = -1;

	// did user specify either of speed or duplex on the cmd line?
	if ((cfg->media_rate != -1) || (cfg->duplex != -1)) {

		if (cfg->media_rate == -1) {
			log(LOG_ERR, "%s(): must also specify speed when duplex is specified", __FUNCTION__);
			at91sam9xx_destroy(at91sam9xx, 2);
			return EINVAL;
			}
		if (cfg->duplex == -1) {
			log(LOG_ERR, "%s(): must also specify duplex when speed is specified", __FUNCTION__);
			at91sam9xx_destroy(at91sam9xx, 2);
			return EINVAL;
			}

		// we get here, we know both media_rate and duplex are set

		switch(cfg->media_rate) {
			case 0:
				at91sam9xx->force_advertise = 0;  // disable link
				break;

			case 10*1000:
				at91sam9xx->force_advertise = cfg->duplex ? MDI_10bTFD : MDI_10bT;
				break;

			case 100*1000:
				at91sam9xx->force_advertise = cfg->duplex ? MDI_100bTFD : MDI_100bT;
				break;

			case 1000*1000:
				at91sam9xx->force_advertise = cfg->duplex ? MDI_1000bTFD : MDI_1000bT;
				break;

			default:
				log(LOG_ERR, "%s(): invalid speed: %d", __FUNCTION__, cfg->media_rate/1000);
				at91sam9xx_destroy(at91sam9xx, 2);
				return EINVAL;
				break;
			}
		}

	// initialize - until mii callback says we have a link ...
	cfg->flags |= NIC_FLAG_LINK_DOWN;

	if (cfg->num_mem_windows == 0) {
		cfg->num_mem_windows = 1;
		}
	cfg->mem_window_base[0] = at91sam9xx->iobase;
	cfg->mem_window_size[0] = 0x1000;

	strcpy((char *)cfg->device_description, "AT91SAM9XX EMAC");

	at91sam9xx->cachectl.fd = NOFD;

	if (cache_init(0, &at91sam9xx->cachectl, NULL) == -1) {
		log(LOG_ERR, "%s(): at91sam9xx detect: cache_init: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 3);
		return rc;
		}

	cfg->revision = NIC_CONFIG_REVISION;

	if (cfg->verbose) {
		nic_dump_config(cfg);
		}

	// map nic registers into virtual memory
	if ((at91sam9xx->reg = mmap_device_memory (NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_NOCACHE,
								MAP_SHARED, at91sam9xx->iobase)) == MAP_FAILED) {
		log(LOG_ERR, "%s(): mmap regs failed: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 3);
		return rc;
		}

	at91sam9xx->phy_reg = at91sam9xx->reg;

	// alloc rx descr ring
	if ((at91sam9xx->rx_bd = mmap (NULL, sizeof (mpc_bd_t) * at91sam9xx->num_rx_descriptors,
								PROT_READ | PROT_WRITE | PROT_NOCACHE , MAP_PRIVATE | MAP_ANON | MAP_PHYS, NOFD, 0)) == MAP_FAILED) {
		log(LOG_ERR, "%s(): mmap rxd failed: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 4);
		return rc;
		}

	// alloc tx descr ring
	if(at91sam9xx->sram_buffers) {
		at91sam9xx->tx_bd = (mpc_bd_t *) mmap_device_io (sizeof (mpc_bd_t) * at91sam9xx->num_tx_descriptors, at91sam9xx->sram_addr);
        offset = at91sam9xx->num_tx_descriptors * sizeof (mpc_bd_t);
        offset = (offset + 3) & ~0x3;
	} else
		at91sam9xx->tx_bd = mmap (NULL, sizeof (mpc_bd_t) * at91sam9xx->num_tx_descriptors,
				 PROT_READ | PROT_WRITE | PROT_NOCACHE , MAP_PRIVATE | MAP_ANON | MAP_PHYS, NOFD, 0);

	if (at91sam9xx->tx_bd == MAP_FAILED) {
		log(LOG_ERR, "%s(): mmap txd failed: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 5);
		return rc;
		}

	// alloc mbuf pointer array, corresponding to tx descr ring
	size = sizeof(struct mbuf *) * at91sam9xx->num_tx_descriptors;
	if(at91sam9xx->sram_buffers) {
		at91sam9xx->tx_pkts = (struct mbuf **) mmap_device_io(size, at91sam9xx->sram_addr + offset);

        offset += at91sam9xx->num_tx_descriptors * sizeof (struct mbuf *);
        offset = (offset + 3) & ~0x3;
	} else
	 	at91sam9xx->tx_pkts = malloc(size, M_DEVBUF, M_NOWAIT);

	if (at91sam9xx->tx_pkts == NULL) {
		rc = ENOBUFS;
		log(LOG_ERR, "%s(): malloc tx_pkts failed", __FUNCTION__);
		at91sam9xx_destroy(at91sam9xx, 6);
		return rc;
		}
	memset(at91sam9xx->tx_pkts, 0x00, size);

	// alloc mbuf pointer array, corresponding to rx descr ring
	size = sizeof(struct mbuf *) * at91sam9xx->num_rx_descriptors;
	at91sam9xx->rx_pkts = malloc(size, M_DEVBUF, M_NOWAIT);
	if (at91sam9xx->rx_pkts == NULL) {
		rc = ENOBUFS;
		log(LOG_ERR, "%s(): malloc rx_pkts failed", __FUNCTION__);
		at91sam9xx_destroy(at91sam9xx, 7);
		return rc;
		}
	memset(at91sam9xx->rx_pkts, 0x00, size);

	if(at91sam9xx->sram_buffers)
		at91sam9xx->tx_buffers = (void *) mmap_device_io(( 
            at91sam9xx->num_tx_descriptors * TX_BUFFER_SIZE), at91sam9xx->sram_addr + offset);

    if (at91sam9xx->tx_buffers == MAP_FAILED) {
        rc = ENOBUFS;
        log(LOG_ERR, "%s(): mmap_device_io tx_buffers failed", __FUNCTION__);
        at91sam9xx_destroy(at91sam9xx, 8);
        return rc;
        }
	else
		at91sam9xx->tx_phys = vtophys (at91sam9xx->tx_buffers);

	// one hardware interrupt on EMAC
	if ((rc = interrupt_entry_init(&at91sam9xx->inter_rx, 0, NULL,
									cfg->priority)) != EOK) {
		log(LOG_ERR, "%s(): interrupt_entry_init(rx) failed: %d", __FUNCTION__, rc);
		at91sam9xx_destroy(at91sam9xx, 9);
		return rc;
		}

	// hook up so media devctls work
	bsd_mii_initmedia(at91sam9xx);

	// interface setup - entry points, etc
	strcpy(ifp->if_xname, at91sam9xx->dev.dv_xname);
	ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST;
	ifp->if_ioctl = at91sam9xx_ioctl;
	ifp->if_start = at91sam9xx_start;
	//	  ifp->if_watchdog = at91sam9xx_watchdog;
	ifp->if_init = at91sam9xx_init;
	ifp->if_stop = at91sam9xx_stop;
	IFQ_SET_READY(&ifp->if_snd);

	if_attach(ifp);
	ether_ifattach(ifp, at91sam9xx->cfg.current_address);

	// rx interrupt can either hit nic or use kermask
	at91sam9xx->inter_rx.func = at91sam9xx_process_interrupt;
	if (at91sam9xx->kermask) {
		at91sam9xx->inter_rx.enable = at91sam9xx_enable_rx_kermask;
		at91sam9xx->isrp_rx = at91sam9xx_isr_rx_kermask;
	} else {
		at91sam9xx->inter_rx.enable = at91sam9xx_enable_rx;
		at91sam9xx->isrp_rx = at91sam9xx_isr_rx;
		}
	at91sam9xx->inter_rx.arg = at91sam9xx;

	/* Setup a timer for housekeeping */
	callout_init (&at91sam9xx->hk_callout);

	if (cfg->verbose > 3) {
		log(LOG_ERR, "%s(): ending: idx %d\n", __FUNCTION__, idx);
		}

	return EOK;
}


//
// called from at91sam9xx_create_instance() and at91sam9xx_destroy() 
//
static void
at91sam9xx_destroy(at91sam9xx_dev_t *at91sam9xx, int how)
{
	struct ifnet		*ifp;
	struct _iopkt_self	*iopkt;
    rsrc_alloc_t    alloc;
    rsrc_request_t  rreq;

	ifp = &at91sam9xx->ecom.ec_if;
	iopkt = at91sam9xx->iopkt;

	/* FALLTHROUGH all of these */
	switch (how) {

		//
		// called from at91sam9xx_destroy()
		//
		case -1:
			/*
			 * Don't init() while we're dying.  Yes it can happen:
			 * ether_ifdetach() calls bridge_ifdetach() which
			 * tries to take us out of promiscuous mode with an
			 * init().
			 */
			at91sam9xx->dying = 1;

			at91sam9xx_stop(ifp, 1);

			at91sam9xx_rx_mbuf_free(at91sam9xx);

			ether_ifdetach(ifp);
			if_detach(ifp);

			//
			// called from at91sam9xx_create_instance()
			//
		case 9:
			interrupt_entry_remove(&at91sam9xx->inter_rx, NULL);

		case 8:
			free(at91sam9xx->rx_pkts, M_DEVBUF);

            if(at91sam9xx->sram_buffers)
			    munmap_device_io((uintptr_t)at91sam9xx->tx_buffers, (at91sam9xx->num_tx_descriptors * TX_BUFFER_SIZE));

		case 7:
            if(at91sam9xx->sram_buffers)
			    munmap_device_io((uintptr_t)at91sam9xx->tx_pkts, (size_t)M_DEVBUF);
            else
			    free(at91sam9xx->tx_pkts, M_DEVBUF);

		case 6:
            if(at91sam9xx->sram_buffers)
    			munmap_device_io((uintptr_t)at91sam9xx->tx_bd, sizeof(mpc_bd_t) * at91sam9xx->num_tx_descriptors);
            else
    			munmap(at91sam9xx->tx_bd, sizeof(mpc_bd_t) * at91sam9xx->num_tx_descriptors);

		case 5:
			munmap(at91sam9xx->rx_bd, sizeof(mpc_bd_t) * at91sam9xx->num_rx_descriptors);

		case 4:
			munmap_device_memory(at91sam9xx->reg, 0x1000);

		case 3:
			cache_fini(&at91sam9xx->cachectl);

		case 2:
            if(at91sam9xx->sram_buffers) {
                memset ((char *) &rreq, 0, sizeof (rreq));
                rreq.length = at91sam9xx->sram_size;
                rreq.start = at91sam9xx->sram_addr;
                rreq.end = at91sam9xx->sram_addr + at91sam9xx->sram_size;
                rreq.flags = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_RANGE;
                rreq.name = "sram_buffer";
                rsrcdbmgr_detach (&rreq, 1);

                memset ((char *) &alloc, 0, sizeof (rsrc_alloc_t));
                alloc.start = at91sam9xx->sram_addr;
                alloc.end = at91sam9xx->sram_addr + at91sam9xx->sram_size;
                alloc.flags = RSRCDBMGR_FLAG_NAME;
                alloc.name = "sram_buffer";
                rsrcdbmgr_destroy(&alloc, 1);
            }

		case 1:
			shutdownhook_disestablish(at91sam9xx->sdhook);
			break;
	}
}


//
// io-pkt entry point
//

static void
at91sam9xx_shutdown(void *arg)
{
	at91sam9xx_dev_t	*at91sam9xx;

	at91sam9xx = arg;

	at91sam9xx_stop(&at91sam9xx->ecom.ec_if, 1);

}

static int
at91sam9xx_detach(struct device *dev, int flags)
{
	at91sam9xx_dev_t	*at91sam9xx;

	at91sam9xx = (at91sam9xx_dev_t *)dev;
	at91sam9xx_destroy(at91sam9xx, -1);

	return EOK;
}

//
// called from at91sam9xx_init()
//
static void
set_phys_addr (at91sam9xx_dev_t *at91sam9xx)
{
	uint32_t		*base = at91sam9xx->reg;
	int			 i;
	uint8_t		 *mac_add = at91sam9xx->cfg.current_address;
	nic_config_t		*cfg = &at91sam9xx->cfg;

	if (cfg->verbose) {
		log(LOG_ERR, "%s(): MAC %02X:%02X:%02X:%02X:%02X:%02X",
				__FUNCTION__,
			mac_add[0],mac_add[1],mac_add[2],mac_add[3],mac_add[4],mac_add[5]);
	}

	for(i = 0; i < 4; i++) {
		*(base + EMAC_SA1B + (i)) = 0;
		*(base + EMAC_SA1T + (i)) = 0;
	}

	*(base + EMAC_SA1B) = *(uint32_t *)&mac_add[0];
	*(base + EMAC_SA1T) = *(uint16_t *)&mac_add[4];
}

void at91sam9xx_rx_set_mode(at91sam9xx_dev_t *at91sam9xx)
{
	volatile uint32_t   *base = at91sam9xx->reg;
	nic_config_t		*config = &at91sam9xx->cfg;
	uint32_t			cfg;

	cfg = *(base + EMAC_NCFG);

	cfg &= ~(EMAC_NCFG_PAE | EMAC_NCFG_SPD | EMAC_NCFG_FD | EMAC_NCFG_MTI | EMAC_NCFG_NBC);

	if(config->flags & NIC_FLAG_PROMISCUOUS)
		cfg |= EMAC_NCFG_CAF;

	if(config->flags & NIC_FLAG_BROADCAST)
		cfg |= EMAC_NCFG_NBC;

	if(config->duplex)
		cfg |= EMAC_NCFG_FD;

	if(config->media_rate == 100 * 1000)
		cfg |= EMAC_NCFG_SPD;

	*(base + EMAC_NCFG) = cfg;
}

//
// called from at91sam9xx_init() and at91sam9xx_stop()
//
static void	
at91sam9xx_reset (at91sam9xx_dev_t *at91sam9xx)
{
	volatile uint32_t	*base = at91sam9xx->reg;
	uint32_t			cfg;
	int	 			i;

	// mask interrupt in nic register
	*(base + EMAC_IDR)= ~0;

	*(base + EMAC_NCR) = 0;

	*(base + EMAC_NCR) = (EMAC_NCR_CLRSTAT);

	// Clear all status bits in the receive status register.
	*(base + EMAC_RSR) = ~0;

	// Clear all status bits in the transmit status register
	*(base + EMAC_TSR) = ~0;

	*(base + EMAC_ISR);

	// ignore broadcasts, and don't copy FCS.
	cfg = *(base + EMAC_NCFG);

	cfg |= (EMAC_NCFG_PAE | EMAC_NCFG_CLK64);

	*(base + EMAC_NCFG) = cfg;

	//clear statistics register   //
	for( i = EMAC_FTO; i <= EMAC_STE ; i++)
		*(base + i); 
}
 

//
// io-pkt entry point - initialize hardware re-entrantly
//
// N.B.  Must ensure that this function, and everything it calls
// is REALLY ok to call over and over again, because the stack
// will call us MORE THAN ONCE
//
static int
at91sam9xx_init (struct ifnet *ifp)
{
	at91sam9xx_dev_t		*at91sam9xx = ifp->if_softc;
	nic_config_t			*cfg = &at91sam9xx->cfg;
	volatile uint32_t		*base = at91sam9xx->reg;
	int						err, i;
	struct mbuf				*m;
	mpc_bd_t				*bd;

	if (cfg->verbose > 3) {
		log(LOG_ERR, "%s(): starting: idx %d\n", 
				__FUNCTION__, cfg->device_index);
	}

	if (at91sam9xx->dying) {
		log(LOG_ERR, "%s(): dying", __FUNCTION__);
		return 1;
	}

	// clean up and reset nic
	at91sam9xx_stop(ifp, 0);  

	// init tx descr ring
	for (i=0; i<at91sam9xx->num_tx_descriptors; i++) {

		bd = &at91sam9xx->tx_bd[i];
		bd->status = EMAC_TBD_FREE;
		
		if(at91sam9xx->sram_buffers)
			bd->buffer = vtophys ((char *)at91sam9xx->tx_buffers +  i * TX_BUFFER_SIZE);
	
		// free any previously txd mbufs
		if ((m = at91sam9xx->tx_pkts[i]) != NULL) {
			m_freem(m);
			at91sam9xx->tx_pkts[i] = NULL;
		}
		if (i == (at91sam9xx->num_tx_descriptors - 1)) {
			bd->status |= EMAC_TBD_WRAP;
		} 
	}
	at91sam9xx->tx_pidx = at91sam9xx->tx_cidx = at91sam9xx->tx_descr_inuse = 0;

	// init rx descr ring
	for (i=0; i<at91sam9xx->num_rx_descriptors; i++) {
		bd = &at91sam9xx->rx_bd[i];
		bd->status = EMAC_TBD_FREE;

		// assign an mbuf to each rx descriptor
		if ((m = at91sam9xx->rx_pkts[i]) != NULL) {
			// reuse previously allocated mbuf
		} else {
			if ((m = m_gethdr(M_DONTWAIT, MT_DATA)) == NULL) {
				return ENOMEM;
			}
			MCLGET(m, M_DONTWAIT);
			if ((m->m_flags & M_EXT) == 0) {
				m_freem(m);
				return ENOMEM;
			}
			at91sam9xx->rx_pkts[i] = m;
		}

		bd->buffer = pool_phys(m->m_data, m->m_ext.ext_page);
		CACHE_INVAL(&at91sam9xx->cachectl, m->m_data, bd->buffer, m->m_ext.ext_size);
		if (i == (at91sam9xx->num_rx_descriptors - 1)) {
			bd->buffer |= EMAC_RBD_WRAP;	/* Wrap indicator */
		}
	}

	at91sam9xx->rx_cidx = 0;

	*(base + EMAC_TBQP)  = vtophys ((void *) at91sam9xx->tx_bd);
	*(base + EMAC_RBQP)  = vtophys ((void *) at91sam9xx->rx_bd);

	*(base + EMAC_NCR) |= EMAC_NCR_THALT;

	at91sam9xx_clear_stats(at91sam9xx);

	// Enable RMII
	*(base + EMAC_USRIO) = (EMAC_USRIO_CLKEN | EMAC_USRIO_RMII); 
	if ((err = at91sam9xx_init_phy(at91sam9xx)) == -1)
		return (ENODEV);

	at91sam9xx->cfg.phy_addr = err;

	// ask for call for detecting PHY link state changes, cleaning up tx descr
	callout_msec(&at91sam9xx->mii_callout, 2 * 1000, at91sam9xx_MDI_MonitorPhy, at91sam9xx);


	if (ifp->if_capenable_rx & ~(IFCAP_CSUM_IPv4 | IFCAP_CSUM_TCPv4 | IFCAP_CSUM_UDPv4) ) {
		return ENOTSUP;  // should never happen
	}
	at91sam9xx->rx_cap_mask = 0;
	if (ifp->if_capenable_rx & IFCAP_CSUM_IPv4) {
		at91sam9xx->rx_cap_mask |= M_CSUM_IPv4 | M_CSUM_IPv4_BAD;
	}
	if (ifp->if_capenable_rx & IFCAP_CSUM_TCPv4) {
		at91sam9xx->rx_cap_mask |= M_CSUM_TCPv4 | M_CSUM_TCP_UDP_BAD;
	}
	if (ifp->if_capenable_rx & IFCAP_CSUM_UDPv4) {
		at91sam9xx->rx_cap_mask |= M_CSUM_UDPv4 | M_CSUM_TCP_UDP_BAD;
	}
	if (at91sam9xx->cfg.verbose) {
		log(LOG_ERR,"at91sam9xx->rx_cap_mask: 0x%X\n", at91sam9xx->rx_cap_mask);
	}

	//
	// set tx capabilites
	//
	if (ifp->if_capenable_tx & ~(IFCAP_CSUM_IPv4 | IFCAP_CSUM_TCPv4 | IFCAP_CSUM_UDPv4) ) {
		return ENOTSUP;  // should never happen
	}
	at91sam9xx->tx_cap_mask = 0;
	if (ifp->if_capenable_rx & IFCAP_CSUM_IPv4) {
		at91sam9xx->tx_cap_mask |= M_CSUM_IPv4 | M_CSUM_IPv4_BAD;
	}
	if (ifp->if_capenable_rx & IFCAP_CSUM_TCPv4) {
		at91sam9xx->tx_cap_mask |= M_CSUM_TCPv4 | M_CSUM_TCP_UDP_BAD;
	}
	if (ifp->if_capenable_rx & IFCAP_CSUM_UDPv4) {
		at91sam9xx->tx_cap_mask |= M_CSUM_UDPv4 | M_CSUM_TCP_UDP_BAD;
	}
	if (at91sam9xx->cfg.verbose) {
		log(LOG_ERR,"at91sam9xx->tx_cap_mask: 0x%X\n", at91sam9xx->tx_cap_mask);
	}



	// Attach to rx EMAC hardware interrupts
	if (at91sam9xx->iid_rx == -1) {
		if ((err = InterruptAttach_r(at91sam9xx->cfg.irq[AT91SAM9xx_RX_INT], at91sam9xx->isrp_rx,
						at91sam9xx, sizeof(*at91sam9xx), _NTO_INTR_FLAGS_TRK_MSK)) < 0) {
			err = -err;
			log(LOG_ERR, "%s(): InterruptAttach_r(rx) failed: %d", __FUNCTION__, err);
			return err;
		}
		at91sam9xx->iid_rx = err;
	}

    /* After all initialization, start the EMAC device */
	set_phys_addr(at91sam9xx); 
	at91sam9xx_rx_set_mode(at91sam9xx);
	*(base + EMAC_NCR) |= (EMAC_NCR_TE | EMAC_NCR_RE | EMAC_NCR_MPE);
	*(base + EMAC_IER) |= AT91SAM9xx_ENABLED_IMASK;

	NW_SIGLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
	ifp->if_flags_tx |= IFF_RUNNING;
	NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
	ifp->if_flags |= IFF_RUNNING;

	callout_msec (&at91sam9xx->hk_callout, 10 * 1000, at91sam9xx_hk_callout, at91sam9xx);

	if (cfg->verbose > 3) {
		log(LOG_ERR, "%s(): ending: idx %d\n", 
				__FUNCTION__, cfg->device_index);
	}
	return EOK;
}


//
// called from at91sam9xx_mii_callback() and at91sam9xx_ioctl()
//
void	
at91sam9xx_speeduplex (at91sam9xx_dev_t *at91sam9xx)
{
	nic_config_t		*cfg		= &at91sam9xx->cfg;
	volatile uint32_t   *base		= at91sam9xx->reg;

	if (cfg->media_rate == 100 * 1000) { // 100
		*(base + EMAC_NCFG) |= EMAC_NCFG_SPD;
	}
	if (cfg->duplex) {
		*(base + EMAC_NCFG) |= EMAC_NCFG_FD;
	} else {
		*(base + EMAC_NCFG) &= ~EMAC_NCFG_FD;
	}
}


//
// io-pkt entry point.  Also called from at91sam9xx_destroy()
// and at91sam9xx_init()
//
static void
at91sam9xx_stop(struct ifnet *ifp, int disable)
{
	at91sam9xx_dev_t	*at91sam9xx = ifp->if_softc;
	int				i;

	// shut down mii probing
	callout_stop(&at91sam9xx->mii_callout);
	callout_stop(&at91sam9xx->hk_callout);

	if (at91sam9xx->mdi) {
		MDI_DeRegister(&at91sam9xx->mdi);
		at91sam9xx->mdi = NULL;
	}
	// stop tx and rx and reset nic
	at91sam9xx_reset(at91sam9xx);

	/* Lock out the transmit side */
	NW_SIGLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
	for (i = 0; i < 10; i++) {
		if ((ifp->if_flags_tx & IFF_OACTIVE) == 0)
			break;
		NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
		delay(50);
		NW_SIGLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
	}

	if (i < 10) {
		ifp->if_flags_tx &= ~IFF_RUNNING;
		NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
	} else {
		/* Heavy load or bad luck.  Try the big gun. */
		quiesce_all();
		ifp->if_flags_tx &= ~IFF_RUNNING;
		unquiesce_all();
	}

	/*
	 * The only other transmit entry point is now locked out.
	 * All others are only used by the stack thread which is
	 * currently right here.
	 */

	if (at91sam9xx->iid_rx != -1) {
		InterruptDetach(at91sam9xx->iid_rx);
		at91sam9xx->iid_rx = -1;
	}

	/* Mark the interface as down and cancel the watchdog timer. */
	ifp->if_flags &= ~(IFF_RUNNING | IFF_OACTIVE);
	ifp->if_timer = 0;
}


//
// called from at91sam9xx_destroy()
//
static void
at91sam9xx_rx_mbuf_free(at91sam9xx_dev_t *at91sam9xx)
{
	int			i;
	struct mbuf		*m;

	for (i = 0; i < at91sam9xx->num_rx_descriptors; i++) {
		if ((m = at91sam9xx->rx_pkts[i]) != NULL) {
			m_freem(m);
			at91sam9xx->rx_pkts[i] = NULL;
		}
	}
}

//					  
// called from at91sam9xx_detect() below
//					  
static unsigned
at91sam9xx_get_syspage_params(struct emac_params *emac, unsigned last)
{	   
	hwi_tag			*tag;
	unsigned		off;
	unsigned		item;
	char			*name;
	int			sram_p = 0;

	/* Find network hardware information. */
	item = hwi_find_item(last, HWI_ITEM_DEVCLASS_NETWORK, "emac", NULL);

	if (item == HWI_NULL_OFF) {
		item = hwi_find_item(last, HWI_ITEM_DEVCLASS_NETWORK, "emac-sram", NULL);
		if (item == HWI_NULL_OFF) {
			return(item);
		}
		else
			sram_p = 1;
		}							   
		
	off = item;					 
	emac->paddr = 0;				
	emac->irq_present = 0;
	emac->mac_present = 0;
	emac->sram_params = sram_p;
	while((off = hwi_next_tag(off, 1)) != HWI_NULL_OFF) { 
		tag = hwi_off2tag(off);
		name = __hwi_find_string(((hwi_tag *)tag)->prefix.name);
		
		if(strcmp(name, HWI_TAG_NAME_location) == 0) {
				emac->paddr = (paddr_t)tag->location.base;
		} else if(strcmp(name, HWI_TAG_NAME_irq) == 0) {
				emac->irq_rx = tag->irq.vector;
		} else if (strcmp(name, HWI_TAG_NAME_nicaddr) == 0) {
				if (tag->nicaddr.len == 6) {
					memcpy(emac->mac, tag->nicaddr.addr, 6);
					emac->mac_present = 1;
					}
				}
		}

	emac->irq_present = 1;
	return(item);
}
//
// called from at91sam9xx_entry() in at91sam9xx.c
//
int
at91sam9xx_detect(void *dll_hdl, struct _iopkt_self *iopkt, char *options)
{
	at91sam9xx_dev_t		*at91sam9xx;			// dummy entry used for parsing and detecting only
	nic_config_t			*cfg;
	int				idx=0;
	int				rc=EOK, single;
	uint32_t				iobase = 0;
	struct at91sam_arg		at91sam_arg;
	struct device			*dev;

	if ((at91sam9xx = malloc(sizeof(*at91sam9xx), M_TEMP, M_NOWAIT | M_ZERO)) == NULL) {
		return ENOMEM;
	}
	cfg = &at91sam9xx->cfg;
	at91sam9xx->use_syspage = 0;
	cfg->device_index = 0xffffffff;

	if (options) {
		if (at91sam9xx_parse_options(at91sam9xx, options, cfg)) {
			free(at91sam9xx, M_TEMP);
			return EINVAL;
		}
	}

	if (at91sam9xx->use_syspage) {
		unsigned		config_offset = HWI_NULL_OFF;
		struct emac_params	emac;

		config_offset = at91sam9xx_get_syspage_params(&emac,config_offset);
		if (config_offset == HWI_NULL_OFF) {
			log(LOG_ERR, "%s(): No syspage hardware configuration information found", __FUNCTION__);
			free(at91sam9xx, M_TEMP);
			return ENODEV;
		}

		if (emac.paddr && emac.irq_present) {
			/* If device index specified, ignore all other device cfgs. */
			if ((cfg->device_index == idx) || (cfg->device_index == 0xffffffff)) {
				single = 0;
				if (cfg->device_index == idx) {
					/* Configured required device... No need to look for
					 * more. */
					single = 1;
				}

				at91sam_arg.dll_hdl = dll_hdl;
				at91sam_arg.options = options;
				at91sam_arg.idx = idx;
				at91sam_arg.iobase = iobase;
				at91sam_arg.emac_p = &emac;

				single = 1;
				dev = NULL; /* No parent */

				if ((rc = dev_attach("emac", options, &at91sam9xx_ca, &at91sam_arg, &single, &dev, NULL)) != EOK) {
					log(LOG_ERR, "%s(): cmd line dev_attach(%d) failed: %d\n", __FUNCTION__, idx, rc);
					free(at91sam9xx, M_TEMP);
					return (ENODEV);
				}

			}
		}
		//
		// all cfg information will be auto-determined or specified on command line
		//

	}
	free(at91sam9xx, M_TEMP);
	return (rc);
}


__SRCVERSION( "$URL$ $REV$" )
