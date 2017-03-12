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



#include <at91sam9xx.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <device_qnx.h>


//
// this is a callback, made by the bsd media code.  We passed
// a pointer to this function during the ifmedia_init() call
// in bsd_mii_initmedia()
//
void
bsd_mii_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
	at91sam9xx_dev_t *at91sam9xx = ifp->if_softc;

	at91sam9xx->bsd_mii.mii_media_active = IFM_ETHER;
	at91sam9xx->bsd_mii.mii_media_status = IFM_AVALID;

	if (at91sam9xx->force_advertise != -1) {	// link is forced

		if (at91sam9xx->cfg.flags & NIC_FLAG_LINK_DOWN) {
			at91sam9xx->bsd_mii.mii_media_active |= IFM_NONE;
			at91sam9xx->bsd_mii.mii_media_status  = 0;

		} else {	// link is up
			at91sam9xx->bsd_mii.mii_media_status |= IFM_ACTIVE;

			switch(at91sam9xx->cfg.media_rate) {
				case 0:
				at91sam9xx->bsd_mii.mii_media_active |= IFM_NONE;
				break;	
	
				case 1000*10:
				at91sam9xx->bsd_mii.mii_media_active |= IFM_10_T;
				break;
	
				case 1000*100:
				at91sam9xx->bsd_mii.mii_media_active |= IFM_100_TX;
				break;
	
				default:	// this shouldnt really happen, but ...
				at91sam9xx->bsd_mii.mii_media_active |= IFM_NONE;
				break;
			}
	
			if (at91sam9xx->cfg.duplex) {
				at91sam9xx->bsd_mii.mii_media_active |= IFM_FDX;
			}
	#if 0
			// could set flow bits
			at91sam9xx->bsd_mii.mii_media_active |= IFM_FLOW;
			at91sam9xx->bsd_mii.mii_media_active |= IFM_ETH_TXPAUSE;
			at91sam9xx->bsd_mii.mii_media_active |= IFM_ETH_RXPAUSE;
	#endif
		}

	} else if (!(at91sam9xx->cfg.flags & NIC_FLAG_LINK_DOWN)) {  // link is auto-detect and up

		at91sam9xx->bsd_mii.mii_media_status |= IFM_ACTIVE;

		switch(at91sam9xx->cfg.media_rate) {
			case 1000*10:
			at91sam9xx->bsd_mii.mii_media_active |= IFM_10_T;
			break;

			case 1000*100:
			at91sam9xx->bsd_mii.mii_media_active |= IFM_100_TX;
			break;

			default:	// this shouldnt really happen, but ...
			at91sam9xx->bsd_mii.mii_media_active |= IFM_NONE;
			break;
		}

		if (at91sam9xx->cfg.duplex) {
			at91sam9xx->bsd_mii.mii_media_active |= IFM_FDX;
		}
#if 0
		// could set flow bits
		at91sam9xx->bsd_mii.mii_media_active |= IFM_FLOW;
		at91sam9xx->bsd_mii.mii_media_active |= IFM_ETH_TXPAUSE;
		at91sam9xx->bsd_mii.mii_media_active |= IFM_ETH_RXPAUSE;
#endif

		// could move this to mii.c so there was no lag
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_AUTO);

	} else {	// link is auto-detect and down
		at91sam9xx->bsd_mii.mii_media_active |= IFM_NONE;
		at91sam9xx->bsd_mii.mii_media_status = 0;

		// could move this to mii.c so there was no lag
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_NONE);
	}

	// stuff parameter values with hoked-up bsd values
    ifmr->ifm_status = at91sam9xx->bsd_mii.mii_media_status;
    ifmr->ifm_active = at91sam9xx->bsd_mii.mii_media_active;
}


//
// this is a callback, made by the bsd media code.  We passed
// a pointer to this function during the ifmedia_init() call
// in bsd_mii_initmedia().  This function is called when
// someone makes an ioctl into us, we call into the generic
// ifmedia source, and it make this callback to actually 
// force the speed and duplex, just as if the user had
// set the cmd line options
//
int
bsd_mii_mediachange(struct ifnet *ifp)
{
	at91sam9xx_dev_t	*at91sam9xx			= ifp->if_softc;
	int				old_media_rate		= at91sam9xx->cfg.media_rate;
	int				old_duplex			= at91sam9xx->cfg.duplex;  
	struct ifmedia	*ifm				= &at91sam9xx->bsd_mii.mii_media;
	int				user_duplex			= ifm->ifm_media & IFM_FDX ? 1 : 0;
	int				user_media			= ifm->ifm_media & IFM_TMASK;

    if (!(ifp->if_flags & IFF_UP)) {
		log(LOG_WARNING,
		  "%s(): at91sam9xx interface isn't up, ioctl ignored", __FUNCTION__);
	    return 0;
	}

	if (!(ifm->ifm_media & IFM_ETHER)) {
		log(LOG_WARNING,
		  "%s(): at91sam9xx interface - bad media: 0x%X", 
		  __FUNCTION__, ifm->ifm_media);
		return 0;	// should never happen
	}

	switch (user_media) {
		case IFM_AUTO:		// auto-select media
		if (at91sam9xx->force_advertise == -1) {
			// we are already in auto-select mode, noop
			return 0;
		}
		// we are currently in a forced mode (or disabled) - now go auto-select
		at91sam9xx->force_advertise = -1;
		at91sam9xx->cfg.media_rate	 = -1;
		at91sam9xx->cfg.duplex		 = -1;
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_AUTO);
		break;

		case IFM_NONE:		// disable media
		if (at91sam9xx->force_advertise == 0) {
			// we are already disabled, noop
			return 0;
		}
		// we are currently either auto or forced - now disable link
		at91sam9xx->force_advertise = 0;
		at91sam9xx->cfg.media_rate	 = 0;
		at91sam9xx->cfg.duplex		 = 0;
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_NONE);
		break;

		case IFM_10_T:		// force 10baseT
		at91sam9xx->force_advertise = user_duplex ? MDI_10bTFD : MDI_10bT;
		at91sam9xx->cfg.media_rate	 = 10 * 1000;
		at91sam9xx->cfg.duplex		 = user_duplex;
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, 
		  user_duplex ? IFM_ETHER|IFM_10_T|IFM_FDX : IFM_ETHER|IFM_10_T);
		break;

		case IFM_100_TX:	// force 100baseTX
		at91sam9xx->force_advertise = user_duplex ? MDI_100bTFD : MDI_100bT;
		at91sam9xx->cfg.media_rate	 = 100 * 1000;
		at91sam9xx->cfg.duplex		 = user_duplex;
		ifmedia_set(&at91sam9xx->bsd_mii.mii_media, 
		  user_duplex ? IFM_ETHER|IFM_100_TX|IFM_FDX : IFM_ETHER|IFM_100_TX);
		break;

		default:			// should never happen
		log(LOG_WARNING,
		  "%s(): at91sam9xx interface - unknown media: 0x%X", 
		  __FUNCTION__, user_media);
		return 0;
		break;
	}

	// does the user want something different than it already is?
	if ((at91sam9xx->cfg.media_rate  != old_media_rate) || 
		(at91sam9xx->cfg.duplex      != old_duplex) ||
		(at91sam9xx->cfg.flags       &  NIC_FLAG_LINK_DOWN) ){

		// must re-initialize hardware with new parameters
		ifp->if_init(ifp);
	}

    return 0;
}


//
// called from at91sam9xx_create_instance() in detect.c to hook up
// to the bsd media structure.  Not entirely unlike kissing
// a porcupine, we must do so carefully, because we do not
// want to use the bsd mii management structure, because
// this driver uses the io-net2 lib/drvr mii code
//
void
bsd_mii_initmedia(at91sam9xx_dev_t *at91sam9xx)
{
    at91sam9xx->bsd_mii.mii_ifp = &at91sam9xx->ecom.ec_if;

	ifmedia_init(&at91sam9xx->bsd_mii.mii_media, IFM_IMASK, bsd_mii_mediachange,
	  bsd_mii_mediastatus);

	// we do NOT call mii_attach() - we do our own link management

	//
	// must create these entries to make ifconfig media work
	// see lib/socket/public/net/if_media.h for defines
	//

	// ifconfig wm0 none (x22)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_NONE, 0, NULL);

	// ifconfig wm0 auto (x20)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_AUTO, 0, NULL);

	// ifconfig wm0 10baseT (x23 - half duplex)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_10_T, 0, NULL);

	// ifconfig wm0 10baseT-FDX (x100023)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_10_T|IFM_FDX, 0, NULL);

	// ifconfig wm0 100baseTX (x26 - half duplex)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_100_TX, 0, NULL);

	// ifconfig wm0 100baseTX-FDX (x100026 - full duplex)
    ifmedia_add(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_100_TX|IFM_FDX, 0, NULL);

	// would add more here for flow control via ifconfig media

	// link is initially down
	ifmedia_set(&at91sam9xx->bsd_mii.mii_media, IFM_ETHER|IFM_NONE);
}


__SRCVERSION( "$URL$ $REV$" )
