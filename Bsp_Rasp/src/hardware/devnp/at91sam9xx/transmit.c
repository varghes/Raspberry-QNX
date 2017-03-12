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



#include "bpfilter.h"

#include "at91sam9xx.h"

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

static struct mbuf *defrag_packet (struct mbuf *m)
{
struct mbuf *m2;

	// the entire packet should fit into one cluster

	MGET(m2, M_DONTWAIT, MT_DATA);
	if (m2 == NULL) {
		m_freem(m);
		return NULL;
	}

	M_COPY_PKTHDR(m2, m);

	MCLGET(m2, M_DONTWAIT);
	if ((m2->m_flags & M_EXT) == 0) {
		m_freem(m);
		m_freem(m2);
		return NULL;
	}

	// this is NOT paranoid - this can happen with jumbo packets bigger
	// than a single cluster - should be handled above
	if (m->m_pkthdr.len > m2->m_ext.ext_size) {
		m_freem(m);
		m_freem(m2);
		return NULL;
	}

	m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
	m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;

	m_freem(m);

	return m2;
}

//
//
//  io-pkt entry point for transmit
//
//  Called with ifp->if_snd_mutex held and IFF_OACTIVE not
//  yet set in ifp->if_flags_tx.
//
void	at91sam9xx_start(struct ifnet *ifp)

{
	at91sam9xx_dev_t	*at91sam9xx = ifp->if_softc;
	struct mbuf			*m;
	struct mbuf			*m2, *m_org;
	mpc_bd_t			*tx_bd = NULL;          // compiler
	volatile mpc_bd_t	*tx_bd_first;		    // warnings
	int					idx, didx;
	int					num_frag;
	int					i, cnt;
	int					desc_avail;
	uint32_t			temp;
	int					length;
	int					total_len=0;
	volatile uint32_t   *base = at91sam9xx->reg;


	if ((ifp->if_flags_tx & IFF_RUNNING) == 0) {
		NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
		return;
		}
	if (!at91sam9xx->linkup) {
		NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
		return;
		}

	at91sam9xx->start_running = 1;

	for (;;) {

		tx_bd_first = NULL;
		desc_avail = at91sam9xx->num_tx_descriptors - at91sam9xx->tx_descr_inuse;


		//
		// Ensure that there is enough room in the tx descriptor
		// ring for the most fragmented packet we will attempt to tx
		//
		// Also, every N times we are called, clean up old tx descr
		// where N is the number of tx descr divided by 8 ie shifted
		// right 3 bits.  Under load, this has the effect of cleaning
		// up old descriptors 8 times, each trip around the ring.  
		//
		// Here are some values for N:
		//
		// With the default  256 tx descr, we clean up every  256/8 =  32 calls
		// With the maximum 2048 tx descr, we clean up every 2048/8 = 256 calls
		// With the minimum   32 tx descr, we clean up every   32/8 =   4 calls
		//
		// The bigger the ring, the more mbufs we will let sit in the tx
		// descriptor ring until we harvest them, for maximum performance 
		// under load: we will allow an enormous burst of packets (eg 256)
		// to be transmitted without stopping to smell the flowers
		//
		// The smaller the ring, the fewer mbufs we will let sit in the tx
		// descriptor ring until we harvest them, for minimum mbuf use
		// and resulting minimal memory consumption
		//
		// NB: if no one transmits packets in a long time, the periodic
		// timer (2 seconds) detects this and harvests the txd mbufs
		// out of the tx descriptor ring
		//

		if (desc_avail < AT91SAM9xx_UNUSED_DESCR) {

			cnt = 0;
			while (1) {
				// see if we can clean out txd pkts from descr ring
				at91sam9xx_transmit_complete(at91sam9xx);

				// did we free any up?
				desc_avail = at91sam9xx->num_tx_descriptors - at91sam9xx->tx_descr_inuse;

				if (desc_avail < AT91SAM9xx_UNUSED_DESCR) {
					if (at91sam9xx->cfg.verbose)
						slogf (_SLOGC_NETWORK, _SLOG_ERROR, "desc_avail %d - tx_descr_inuse %d",
							desc_avail, at91sam9xx->tx_descr_inuse);
					if (++cnt < 3) {	/* Allow some time for buffers to clear */
						nanospin_ns (100000);
						continue;
						}
					ifp->if_flags_tx |= IFF_OACTIVE;
					at91sam9xx->stats.tx_failed_allocs++;
					goto done; // not enough tx descriptors, try later
					}
				else
					break;
				}
			}

		//
		// ok, now we know that we have room for at least one heavily
		// fragmented packet, get the next mbuf for tx
		//
		IFQ_DEQUEUE(&ifp->if_snd, m);

		if (!m) {
			goto done;  // nothing to tx, we go home now
			}

		m_org = m;
		// count up mbuf fragments
		for (total_len = 0, num_frag = 0, m2 = m; m2; num_frag++) {
			total_len += m2->m_len;
			m2 = m2->m_next;
			}
		if (!at91sam9xx->sram_buffers) {
			if((total_len < 1520) && (num_frag > 1)) {
				if ((m = defrag_packet (m)) == NULL) {
					goto done;
					}
				num_frag = 1;
				}
			}

		if (at91sam9xx->cfg.verbose > 6) {
			log(LOG_ERR, 
			  "%s(): num_frag %d  desc_avail %d  tx_pidx %d  tx_cidx %d  tx_descr_inuse %d num_tx_descriptors %d\n", 
			  __FUNCTION__, num_frag, desc_avail, at91sam9xx->tx_pidx, at91sam9xx->tx_cidx, at91sam9xx->tx_descr_inuse, 
			  at91sam9xx->num_tx_descriptors);
		}

		// dump header of txd packets if user requested it with verbose=8 cmd line option
		if (at91sam9xx->cfg.verbose > 7) {
			unsigned char *p;
			int len = m->m_len;
			int i;

			log(LOG_ERR,"Txd dev_idx %d  num_frag %d  first descr bytes %d", 
			  at91sam9xx->cfg.device_index, num_frag, len);

				log(LOG_ERR,"\n\tPacket:\n");

			p = (unsigned char *)m->m_data;
			for (i = 0; i < min (len, 64); i++) {
				log(LOG_ERR,"%02X ",*p++);
				if ((i + 1) % 16 == 0) {
					log(LOG_ERR,"\n");
				}
			}
			if (i < 64) {
				log(LOG_ERR,"\n");
			}
		}

		// load up descriptors
		for (i = 0, m2 = m, idx = at91sam9xx->tx_pidx; i < num_frag && m2; i++, m2 = m2->m_next) {

			// skip over zero length fragments
			if (m2->m_len == 0) {
				continue;
			}

			// calc pointer to next descriptor we should load
			tx_bd = &at91sam9xx->tx_bd[idx];

			if (!(tx_bd->status & EMAC_TBD_FREE)) {
				//
				// this should NEVER happen - see AT91SAM9xx_UNUSED_DESCR.  Reset chip?
				//
				log(LOG_ERR, "%s(): Big Problem: out of sync with nic!!", __FUNCTION__);
				at91sam9xx->stats.un.estats.internal_tx_errors++;
				ifp->if_oerrors++;
				m_freem(m);
				goto done;
			}

			if(at91sam9xx->sram_buffers) {
				m2 = m_org;
				didx = idx * TX_BUFFER_SIZE;
				length = 0;
				while (1) {
					memcpy(((char *)at91sam9xx->tx_buffers + didx), m2->m_data,  m2->m_len);
					didx += m2->m_len;
					length += m2->m_len;
					m2 = m2->m_next;
					if (m2 == NULL)
						break;
					}
				tx_bd->buffer = at91sam9xx->tx_phys + (idx * TX_BUFFER_SIZE);
				num_frag = 1;
				}
			else {
				uint32_t  mbuf_phys_addr = mbuf_phys(m2);
				length = m2->m_len;
				CACHE_INVAL(&at91sam9xx->cachectl, m2->m_data, mbuf_phys_addr, m2->m_len);
				tx_bd->buffer = mbuf_phys_addr;
				}
					
			temp = tx_bd->status;
			temp &= EMAC_TBD_WRAP;

			temp |= EMAC_TBD_LENGTH(length) | EMAC_TBD_LAST;

			tx_bd->status = temp;
			tx_bd_first = tx_bd;

			if (at91sam9xx->cfg.verbose > 6) {
				log(LOG_ERR,"normal:	  idx %d  len %d  stat 0x%x", idx, length, tx_bd->status);
				}
			// we have loaded this descriptor, onto the next
			idx = NEXT_TX(idx);
			if (at91sam9xx->sram_buffers)
				break;
			}

		// remember the number of tx descriptors used this time
		
		at91sam9xx->tx_descr_inuse += (num_frag);

		// remember mbuf pointer for after tx.  For multiple descriptor
		// transmissions, middle and last descriptors have a zero mbuf ptr
		at91sam9xx->tx_pkts[at91sam9xx->tx_pidx] = m;

		// advance producer index to next unused descriptor, using modulo macro above
		at91sam9xx->tx_pidx = idx;

		//
		// start transmission of this packet by:
		//
		// 1) setting LAST  bit in last descriptor, and
		// 2) setting READY bit in first descriptor
		//
		// at the completion of the loop, tx_bd_first
		// is set, and tx_bd points to last descr filled
		//
	
		if (num_frag == 1) {  // single descriptor

    	    tx_bd->status |= EMAC_TBD_LAST;
			if (at91sam9xx->cfg.verbose > 6) {
				log(LOG_ERR,"enable: single: stat 0x%x", tx_bd->status);
			}
		} else { // multiple descriptors

    	    tx_bd->status &= ~EMAC_TBD_LAST;
			if (at91sam9xx->cfg.verbose > 6) {
				log(LOG_ERR,"enable: multi: first stat 0x%X  last stat 0x%X", tx_bd_first->status, tx_bd->status);
			}
		}
		/* Force transmitter to start */
		temp = (*(base + EMAC_NCR) | EMAC_NCR_TSTAT);
		*(base + EMAC_NCR) =  temp;
	
#if NBPFILTER > 0
		// Pass the packet to any BPF listeners
		if (ifp->if_bpf) {
			bpf_mtap(ifp->if_bpf, m);
		}
#endif

	} // for

done:

	at91sam9xx->start_running = 0;
	if (!at91sam9xx->hk) {
		NW_SIGUNLOCK(&ifp->if_snd_ex, at91sam9xx->iopkt);
		}
}


//
// process completed tx descriptors - call with if_snd_ex
//
void	at91sam9xx_transmit_complete(at91sam9xx_dev_t *at91sam9xx)

{
	int				idx, desc_avail;
	mpc_bd_t		*bd;
	uint32_t		status, tsr;
	struct mbuf		*m;
	struct ifnet	*ifp = &at91sam9xx->ecom.ec_if;
	volatile uint32_t	   *base = at91sam9xx->reg;
	

	if (at91sam9xx->cfg.verbose > 5) {
		log(LOG_ERR, "%s(): starting: tx_pidx %d  tx_cidx %d  tx_descr_inuse %d", 
		  __FUNCTION__, at91sam9xx->tx_pidx, at91sam9xx->tx_cidx, at91sam9xx->tx_descr_inuse);
	}

	tsr = *(base + EMAC_TSR);
	*(base + EMAC_TSR) = tsr;
	while (at91sam9xx->tx_pidx != at91sam9xx->tx_cidx) {
		idx = at91sam9xx->tx_cidx;
		bd = &at91sam9xx->tx_bd[idx];

		status = bd->status;
		if (!(status & EMAC_TBD_FREE)) {
			break; // nic still owns this descriptor
		}

		/* If a transmit error occurs, the transmitter resets to the first descriptor */
		if (status & TX_ERROR || tsr & EMAC_TSR_BEX) {
			at91sam9xx->tx_pidx = 0;
			at91sam9xx->tx_cidx = 0;
			at91sam9xx->tx_descr_inuse = 0;
			for (idx = 0; idx < at91sam9xx->num_tx_descriptors; idx++) {
				if ((m = at91sam9xx->tx_pkts[idx])) {		
					m_freem(m);
					at91sam9xx->tx_pkts[idx] = NULL;
					ifp->if_opackets++;
					}
				}
			log(LOG_ERR, "%s(): tx error: descr addr 0x%X length - status %x", 
				__FUNCTION__, bd->buffer, bd->status);
			return;
			}

		if (at91sam9xx->cfg.verbose > 6) {
			log(LOG_ERR,"tx done: idx %d  stat 0x%X", idx, status);
		}

		//
		// only FIRST descriptor will have corresponding mbuf pointer
		//
		if ((m = at91sam9xx->tx_pkts[idx]) != NULL) {
			m_freem(m);
			at91sam9xx->tx_pkts[idx] = NULL;
			ifp->if_opackets++;
		}

		// this tx descriptor is available for use now
		at91sam9xx->tx_descr_inuse--;
		at91sam9xx->tx_cidx = NEXT_TX(idx);
	}

	if (ifp->if_flags_tx & IFF_OACTIVE) {
		desc_avail = at91sam9xx->num_tx_descriptors - at91sam9xx->tx_descr_inuse;
		if (desc_avail > AT91SAM9xx_UNUSED_DESCR) {
			ifp->if_flags_tx &= ~IFF_OACTIVE;
			if (!at91sam9xx->start_running)
				at91sam9xx_start (ifp);
			}
		}
		
	if (at91sam9xx->cfg.verbose > 5) {
		log(LOG_ERR, "%s(): ending:   tx_pidx %d  tx_cidx %d  tx_descr_inuse %d \n", 
		  __FUNCTION__, at91sam9xx->tx_pidx, at91sam9xx->tx_cidx, at91sam9xx->tx_descr_inuse);
	}
}

__SRCVERSION( "$URL$ $REV$" )
