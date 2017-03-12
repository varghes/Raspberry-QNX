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


static unsigned char emac_waitphy(at91sam9xx_dev_t *at91sam9xx, unsigned int retry )
{ 
	unsigned int retry_count = 0;
	volatile uint32_t   *base = at91sam9xx->reg;
	
	while((*(base + EMAC_NSR) & EMAC_NSR_IDLE) == 0) {

		if (retry == 0) {
			continue;
		}

		retry_count++;
		if(retry_count >= retry) {
		   	log(LOG_ERR, "%s(): ERROR: Wait PHY time out\n",  __FUNCTION__);
			return 0; 
		}
	}   
		
	return 1;
}	

//
// drvr lib callback to read phy register
//
uint16_t emac_readphy(void *handle, uint8_t phy_add, uint8_t reg_add)
{		
	at91sam9xx_dev_t	*at91sam9xx = (at91sam9xx_dev_t *) handle;
	volatile uint32_t	*base = at91sam9xx->phy_reg;
	int					timeout = EMAC_TIMEOUT, pval;

	*(base + EMAC_MAN) = (EMAC_SOF & (0x01 << 30))
			| (EMAC_CODE & (2 << 16)) 
			| (EMAC_RW & (2 << 28))
			| (EMAC_PHYA & ((phy_add & 0x1f) << 23))
			| (EMAC_REGA & (reg_add << 18));
		
	if ( emac_waitphy(at91sam9xx, timeout) == 0 ) {
		log(LOG_ERR, "%s(): TimeOut emac_readphy()\n",  __FUNCTION__);
		return 0;
		}	   
				
	pval = (*(base + EMAC_MAN) & 0x0000ffff );
	return pval;	   
}

//
// drvr lib callback to write phy register
//
void emac_writephy(void *handle, uint8_t phy_add, uint8_t reg_add, uint16_t data)
{	   
	at91sam9xx_dev_t	*at91sam9xx = (at91sam9xx_dev_t *) handle;
	volatile uint32_t	*base = at91sam9xx->phy_reg;
	int					timeout = EMAC_TIMEOUT;

	*(base + EMAC_MAN) = (EMAC_SOF & (0x01 << 30))
							  | (EMAC_CODE & (2 << 16))
							  | (EMAC_RW & (1 << 28))
							  | (EMAC_PHYA & ((phy_add & 0x1f) << 23))
							  | (EMAC_REGA & (reg_add << 18))
							  | (EMAC_DATA & data) ;
	if ( emac_waitphy(at91sam9xx, timeout) == 0 ) {
		log(LOG_ERR, "%s(): TimeOut emac_writephy()\n",  __FUNCTION__);
		return;
	}

}

//
// drvr lib callback when PHY link state changes
//
void
at91sam9xx_mii_callback(void *handle, uchar_t phy, uchar_t newstate)
{
	at91sam9xx_dev_t	*at91sam9xx = handle;
	nic_config_t		*cfg  = &at91sam9xx->cfg;
	char				*s;
	int					i;
	int					mode;
	struct ifnet		*ifp = &at91sam9xx->ecom.ec_if;

	switch(newstate) {
	case MDI_LINK_UP:
		if ((i = MDI_GetActiveMedia(at91sam9xx->mdi, cfg->phy_addr, &mode)) != MDI_LINK_UP) {
			log(LOG_INFO, "%s(): MDI_GetActiveMedia() failed: %x", __FUNCTION__, i);
			mode = 0;  // force default case below - all MDI_ macros are non-zero
		}

		switch(mode) {
		case MDI_10bTFD:
			s = "10 BaseT Full Duplex";
			cfg->duplex = 1;
			cfg->media_rate = 10 * 1000L;
			break;
		case MDI_10bT:
			s = "10 BaseT Half Duplex";
			cfg->duplex = 0;
			cfg->media_rate = 10 * 1000L;
			break;
		case MDI_100bTFD:
			s = "100 BaseT Full Duplex";
			cfg->duplex = 1;
			cfg->media_rate = 100 * 1000L;
			break;
		case MDI_100bT:
			s = "100 BaseT Half Duplex";
			cfg->duplex = 0;
			cfg->media_rate = 100 * 1000L;
			break;
		case MDI_100bT4:
			s = "100 BaseT4";
			cfg->duplex = 0;
			cfg->media_rate = 100 * 1000L;
			break;
		default:
			log(LOG_INFO,"%s(): unknown link mode 0x%X",__FUNCTION__,mode);
			s = "Unknown";
			cfg->duplex = 0;
			cfg->media_rate = 0L;
			break;
		}

		// immediately set new speed and duplex in nic config registers
		at91sam9xx_speeduplex(at91sam9xx);

		if (cfg->media_rate) {  
			cfg->flags &= ~NIC_FLAG_LINK_DOWN;
			at91sam9xx->linkup = 1;
			if_link_state_change (ifp, LINK_STATE_UP);
			if (cfg->verbose) {
				log(LOG_INFO, "%s(): link up lan %d idx %d (%s)", 
				  __FUNCTION__, cfg->lan, cfg->device_index, s);
			}
			break;
		} else {
			// fall through to link down handling
		}

	case MDI_LINK_DOWN:
		cfg->media_rate = cfg->duplex = -1;
		cfg->flags |= NIC_FLAG_LINK_DOWN;
		at91sam9xx->linkup = 0;
		if_link_state_change (ifp, LINK_STATE_DOWN);

		if (cfg->verbose) {
			log(LOG_INFO, 
			  "%s(): Link down lan %d idx %d, calling MDI_AutoNegotiate()", 
			  __FUNCTION__, cfg->lan, cfg->device_index);
		}
		MDI_AutoNegotiate(at91sam9xx->mdi, cfg->phy_addr, NoWait);

		break;

	default:
		break;
	}
}


//
// periodically called by stack to probe phy state
// and to clean out tx descriptor ring
//
void
at91sam9xx_MDI_MonitorPhy (void *arg)
{
	at91sam9xx_dev_t	*at91sam9xx	= arg;
	nic_config_t		*cfg  		= &at91sam9xx->cfg;


	//
	// we will probe the PHY if:
	//   the user has forced it from the cmd line, or
	//   we have not rxd any packets since the last time we ran, or
	//   the link is considered down
	//
	if (at91sam9xx->probe_phy ||
		!at91sam9xx->rxd_pkts   ||
		cfg->media_rate <= 0 ||		
		cfg->flags & NIC_FLAG_LINK_DOWN) {
		if (cfg->verbose > 4) {
			log(LOG_ERR, "%s(): calling MDI_MonitorPhy()\n",  __FUNCTION__);
		}
		//
		// directly call drvr lib to probe PHY link state which in turn
		// will call at91sam9xx_mii_callback() above if link state changes
		//
		MDI_MonitorPhy(at91sam9xx->mdi);  

	} else {
		if (cfg->verbose > 4) {
			log(LOG_ERR, "%s(): NOT calling MDI_MonitorPhy()\n",  __FUNCTION__);
		}
	}
	at91sam9xx->rxd_pkts = 0;  // reset for next time we are called

	// restart timer to call us again in 2 seconds
	callout_msec(&at91sam9xx->mii_callout, 2 * 1000, at91sam9xx_MDI_MonitorPhy, at91sam9xx);
}


//
// called from at91sam9xx_init()
//
int	at91sam9xx_init_phy(at91sam9xx_dev_t *at91sam9xx)

{
nic_config_t	*cfg = &at91sam9xx->cfg;
int				phy_idx = cfg->phy_addr;
uint32_t		*base = at91sam9xx->reg;
int				timeout = 10;
int				an_capable;
uint16_t		reg;
uint32_t		rc = 0;
int				i, status;

	if (cfg->verbose) {
		log(LOG_ERR, "%s(): media_rate: %d, duplex: %d, PHY: %d", 
		__FUNCTION__, cfg->media_rate, cfg->duplex, phy_idx);
	}

	//Enable MDIO
	*(base + EMAC_NCR) = (*(base + EMAC_NCR) | EMAC_NCR_MPE);

	
	if (at91sam9xx->mdi) {
		MDI_DeRegister(&at91sam9xx->mdi);   // repetitively called by if_init
	}

	status = MDI_Register_Extended (at91sam9xx, emac_writephy, emac_readphy,
		at91sam9xx_mii_callback, (mdi_t **)&at91sam9xx->mdi, NULL, 0, 0);
	if (status != MDI_SUCCESS) {
		log(LOG_ERR, "%s(): MDI_Register_Extended() failed: %d", __FUNCTION__ ,status);
		at91sam9xx->mdi = NULL;
		return -1;
	}

	callout_init(&at91sam9xx->mii_callout);

	for (phy_idx = 0; phy_idx < 32; phy_idx++) {
		rc = emac_readphy(at91sam9xx, phy_idx, MDI_PHYID_1);
		if(rc == 0 ) {
			log(LOG_ERR, "%s(): Cannot find an active PHY  %d.",__FUNCTION__,phy_idx);
			}
		if (rc == 0x0181) {
			rc = phy_idx;
			break;
			}
		}
	
	rc = emac_readphy(at91sam9xx, phy_idx, MDI_PHYID_1);
	
	if (phy_idx == 32) {
		log(LOG_ERR, "%s(): devnp-at91sam9xx: Cannot find an active PHY",__FUNCTION__);
		return -1;
		}
	if (at91sam9xx->cfg.verbose)
		log(LOG_ERR, "%s(): devnp-at91sam9xx: MII transceiver found at address %d.",__FUNCTION__,phy_idx);

	if (MDI_InitPhy(at91sam9xx->mdi, phy_idx) != MDI_SUCCESS) {
		log(LOG_ERR, "%s(): devnp-at91sam9xx: Cannot init the PHY status",__FUNCTION__);
		return -1;
		}

	if (at91sam9xx->force_advertise == 0) { // the user has forced the link down
		MDI_IsolatePhy(at91sam9xx->mdi, phy_idx);
		MDI_DisableMonitor(at91sam9xx->mdi);  // neuter MDI_MonitorPhy()
		return phy_idx;
		} else {  // forced or auto-neg  
		// in case the user previously forced the link
		// down, bring it back up again
		MDI_DeIsolatePhy(at91sam9xx->mdi, phy_idx);
		delay(10);
		}

	//RESET BMCR 
	emac_writephy(at91sam9xx, phy_idx , MDI_BMCR, BMCR_RESET);

	rc = emac_readphy(at91sam9xx, phy_idx, MDI_BMCR);

	do {
		rc = emac_readphy(at91sam9xx, phy_idx, MDI_BMCR);
		timeout--;
		} while ((rc & BMCR_RESET) && timeout);

	an_capable = emac_readphy(at91sam9xx, phy_idx, MDI_BMSR);

	if (at91sam9xx->force_advertise != -1 || !an_capable) {
		reg = emac_readphy(at91sam9xx, phy_idx, MDI_BMCR);
 
		reg &= ~(BMCR_RESTART_AN | BMCR_SPEED_100 | BMCR_SPEED_1000 | BMCR_FULL_DUPLEX);
 
		if (an_capable && at91sam9xx->force_advertise != 0) {
			/*
			 * If we force the speed, but the link partner
			 * is autonegotiating, there is a greater chance
			 * that everything will work if we advertise with
			 * the speed that we are forcing to.
			 */
			MDI_SetAdvert(at91sam9xx->mdi, phy_idx, at91sam9xx->force_advertise);
 
			reg |= BMCR_RESTART_AN | BMCR_AN_ENABLE;
 
			if (cfg->verbose) {
				log(LOG_ERR, "%s(): restricted autonegotiate (%dMbps only)", 
					__FUNCTION__, cfg->media_rate / 1000);
			}
		} else {
			reg &= ~BMCR_AN_ENABLE;
 
			if (cfg->verbose) {
				log(LOG_ERR, "%s(): forcing the link", __FUNCTION__);
			}
		}
	
	if (cfg->duplex > 0) 
		reg |= BMCR_FULL_DUPLEX;
		
	if (cfg->media_rate == 100 * 1000) 
		reg |= BMCR_SPEED_100;

	emac_writephy(at91sam9xx, phy_idx , MDI_BMCR, reg);

	if (reg & BMCR_AN_ENABLE) {
		if ((i = MDI_EnableMonitor(at91sam9xx->mdi, 1)) != MDI_SUCCESS) {
			log(LOG_ERR, "%s(): MDI_EnableMonitor(forced) failed for phy %d: %d", __FUNCTION__, phy_idx, i);
			}
		}

	} else {  // normal auto-negotiation mode

		cfg->flags |= NIC_FLAG_LINK_DOWN;
		if ((i = MDI_AutoNegotiate(at91sam9xx->mdi, phy_idx, NoWait)) != MDI_SUCCESS) {
			log(LOG_ERR, "%s(): MDI_AutoNegotiate() failed for phy %d: %d", __FUNCTION__, phy_idx, i);
		}

		if ((i = MDI_EnableMonitor(at91sam9xx->mdi, 1)) != MDI_SUCCESS) {
			log(LOG_ERR, "%s(): MDI_EnableMonitor(auto) failed for phy %d: %d", __FUNCTION__, phy_idx, i);
			}
		}

	cfg->connector = NIC_CONNECTOR_MII;

	return phy_idx;
}


__SRCVERSION( "$URL$ $REV$" )
