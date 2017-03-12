/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems. 
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

#include <sim_mmc.h>

#ifdef MMCSD_VENDOR_ESDHC

#include <sim_esdhc.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_85xx.h>
#include <hw/sysinfo.h>

//#define ESDHC_DEBUG

int esdhc_dump_regs( SIM_HBA *hba )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;

	mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: DSADDR %x, BLKATTR %x, CMDARG %x, XFERTYP %x, CMDRSP0 %x, CMDRSP1 %x, CMDRSP2 %x, CMDRSP3 %x, PRSSTAT %x, PROTCTL %x, SYSCTL %x, IRQSTAT %x, IRQSTATEN %x, IRQSIGEN %x, AUTOC12ERR %x, HOSTCAPBLT %x, WML %x, FEVT %x, HOSTVER %x, SCR %x", __FUNCTION__,
		in32( base + ESDHC_DSADDR ), in32( base + ESDHC_BLKATTR ),
		in32( base + ESDHC_CMDARG ), in32( base + ESDHC_XFERTYP ),
		in32( base + ESDHC_CMDRSP0 ), in32( base + ESDHC_CMDRSP1 ),
		in32( base + ESDHC_CMDRSP2 ), in32( base + ESDHC_CMDRSP3 ),
		in32( base + ESDHC_PRSSTAT ), in32( base + ESDHC_PROCTL ),
		in32( base + ESDHC_SYSCTL ), in32( base + ESDHC_IRQSTAT ),
		in32( base + ESDHC_IRQSTATEN ), in32( base + ESDHC_IRQSIGEN ),
		in32( base + ESDHC_AUTOC12ERR ), in32( base + ESDHC_HOSTCAPBLT ),
		in32( base + ESDHC_WML ), in32( base + ESDHC_FEVT ),
		in32( base + ESDHC_HOSTVER ), in32( base + ESDHC_SCR ) );
	return( MMC_SUCCESS );
}

int esdhc_prsstat( SIM_HBA *hba, uint32_t value, uint32_t mask, uint32_t timeo )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		prsstat;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;

	for( ; timeo && ( ( prsstat = in32( base + ESDHC_PRSSTAT ) ) & mask ) != value; timeo-- ) {
		nanospin_ns( 100 );
	} 
	return( timeo ? MMC_SUCCESS : MMC_FAILURE );
}

int esdhc_detect( SIM_HBA *hba )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;

	ext   = (SIM_MMC_EXT *)hba->ext;
	esdhc = (esdhc_ext_t *)ext->handle;
	base  = esdhc->base;

	if( !( in32( base + ESDHC_PRSSTAT ) & ESDHC_PRSSTAT_CINS ) ) {
		return( MMC_FAILURE );
	}

	return( esdhc_prsstat( hba, ESDHC_PRSSTAT_CDPL | ESDHC_PRSSTAT_CINS, ESDHC_PRSSTAT_CDPL | ESDHC_PRSSTAT_CINS, ESDHC_PRSSTAT_DETECT_TIMEOUT ) );
}
			
int esdhc_reset( SIM_HBA *hba, uint32_t rst, uint32_t restore )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		cnt;
	uint32_t		wml;
	uint32_t		imsk;
	uint32_t		proctl;
	uint32_t		sysctl;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;
	wml		= imsk = proctl = 0;

	if( restore ) {
		wml		= in32( base + ESDHC_WML );
		imsk	= in32( base + ESDHC_IRQSIGEN );
		proctl	= in32( base + ESDHC_PROCTL );
	}

		// wait up to 100 ms for reset to complete
	out32( base + ESDHC_SYSCTL, in32( base + ESDHC_SYSCTL ) | rst );
	for( cnt = 100; cnt && ( ( sysctl = in32( base + ESDHC_SYSCTL ) ) & rst ); cnt-- ) {
		delay( 1 );
	}

	if( restore ) {
		out32( base + ESDHC_IRQSIGEN, imsk );
		out32( base + ESDHC_PROCTL, proctl );
		out32( base + ESDHC_WML, wml );
	}

	return( cnt ? MMC_SUCCESS : MMC_FAILURE );
}

int esdhc_clock( SIM_HBA *hba, int *clock, int hs )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		sysctl;
	uint32_t		max_clk;
    int				dvs;
    int				sdclkfs;

	ext			= (SIM_MMC_EXT *)hba->ext;
	esdhc		= (esdhc_ext_t *)ext->handle;
	base		= esdhc->base;
	max_clk		= esdhc->clock;
	hs			= hs;
	sdclkfs		= 2;

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: requested clock %d, %s speed", __FUNCTION__, *clock, hs ? "high" : "low" );
	}
	if(*clock>max_clk){
		*clock = max_clk;
	}

	if(*clock < (max_clk / 256)){
		*clock = max_clk / 256;
	}

	// calc sdclkfs (min is div 2)
	if( max_clk / 16 > *clock ) {
		for( sdclkfs = 2; sdclkfs < 256; sdclkfs *= 2 ) {
			if( ( max_clk / sdclkfs ) < ( *clock * 16 ) ) {
				break;
			}
		}
	}

		// calc dvs using above sdclkfs
	for( dvs = 1; dvs <= 16; dvs++ ) {
		if( ( max_clk / ( dvs * sdclkfs ) ) <= *clock ) {
			break;
		}
	}

	sdclkfs >>= 1;
	dvs -= 1;

	sysctl = in32( base + ESDHC_SYSCTL ) & ~( ESDHC_SYSCTL_SDCLKFS_MASK | ESDHC_SYSCTL_DVS_MASK );
	sysctl |= ( dvs << ESDHC_SYSCTL_DVS_SHIFT ) | ( sdclkfs << ESDHC_SYSCTL_SDCLKFS_SHIFT );
	out32( base + ESDHC_SYSCTL, sysctl | ESDHC_SYSCTL_DTOCV_MAX );
	out32( base + ESDHC_SYSCTL, sysctl | ESDHC_SYSCTL_PEREN | ESDHC_SYSCTL_DTOCV_MAX );

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: SYSCTL %x, dvs %x, sdclkfs %x", __FUNCTION__, in32( base + ESDHC_SYSCTL ), dvs, sdclkfs );
	}
	return( MMC_SUCCESS );
}

int _esdhc_clock( SIM_HBA *hba, int clock )
{
	return( esdhc_clock( hba, &clock, 0 ) );
}

int esdhc_interrupt( SIM_HBA *hba, int irq, int rtype, uint32_t *resp )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		imsk;
	uint32_t		istat;
	uint32_t		acstat;
	int				intr;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;
	istat	= in32( base + ESDHC_IRQSTAT );
	imsk	= in32( base + ESDHC_IRQSTATEN );
	acstat	= in32( base + ESDHC_AUTOC12ERR );
	intr	= 0;

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: IRQSTAT %x, IRQSTATEN %x, IRQSIGEN %x, AUTOC12ERR %x, PRSSTAT %x", __FUNCTION__, istat, imsk, in32( base + ESDHC_IRQSIGEN ), acstat, in32( base + ESDHC_PRSSTAT ) );
	}

	if( imsk & ESDHC_IRQ_CC ) {			// command complete interrupt enabled
		if( istat & ESDHC_IRQ_CTOE ) {	// command timeout
			intr |= MMC_ERR_CMD_TO | MMC_INTR_ERROR | MMC_INTR_COMMAND;

			mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: CTOE blks remaining %d", __FUNCTION__, ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) ? ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) : 0 ); 

			if( esdhc_reset( hba, ESDHC_SYSCTL_RSTD | ESDHC_SYSCTL_RSTC, CAM_TRUE ) ) {
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ESDHC_SYSCTL_RSTD/ESDHC_SYSCTL_RSTC timeout", __FUNCTION__ );
			}
		}
		else if( istat & ESDHC_IRQ_CC ) {
			intr |= MMC_INTR_COMMAND;
			if( istat & ( ESDHC_IRQ_CCE | ESDHC_IRQ_CEBE | ESDHC_IRQ_CIE ) ) {
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: CCE/CEBE/CIE blks remaining %d", __FUNCTION__, ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) ? ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) : 0 ); 
				intr |= MMC_INTR_ERROR;	// Other errors
			}
			resp[0] = in32( base + ESDHC_CMDRSP0 );
			resp[1] = in32( base + ESDHC_CMDRSP1 );
			resp[2] = in32( base + ESDHC_CMDRSP2 );
			resp[3] = in32( base + ESDHC_CMDRSP3 );
			if( rtype & MMC_RSP_136 ) {
				// CRC is not included in the response register,
				// shift to match the 128 CID/CSD structure
				resp[3] = ( resp[3] << 8 ) | ( resp[2] >> 24 );
				resp[2] = ( resp[2] << 8 ) | ( resp[1] >> 24 );
				resp[1] = ( resp[1] << 8 ) | ( resp[0] >> 24 );
				resp[0] = ( resp[0] << 8 );
			}
		}
	}

	if( istat & ESDHC_IRQ_DINT ) {					// dma interrupt
		out32( base + ESDHC_DSADDR, in32( base + ESDHC_DSADDR ) );
#if 0
			// wait for transfer complete 
		while( !( istat & ESDHC_IRQ_TC ) && !( istat & ESDHC_IRQ_ERROR_MASK ) ) {
			nanospin_ns( 100 );
			istat	= in32( base + ESDHC_IRQSTAT );
		}
#endif
	}

	if( imsk & ( ESDHC_IRQ_TC | ESDHC_IRQ_BRR | ESDHC_IRQ_BWR ) ) {
		if( istat & ESDHC_IRQ_BRR ) {
			out32( base + ESDHC_IRQSTATEN, in32( base + ESDHC_IRQSTATEN ) & ~ESDHC_IRQ_BRR );
			intr |= MMC_INTR_RBRDY;
		}
		else if( istat & ESDHC_IRQ_BWR ) {
			out32( base + ESDHC_IRQSTATEN, in32( base + ESDHC_IRQSTATEN ) & ~ESDHC_IRQ_BWR );
			intr |= MMC_INTR_WBRDY;
		}
		if( istat & ESDHC_IRQ_TC ) {		// data done
#ifdef BLKATTR_BUG
			intr |= MMC_INTR_DATA;
#else
			if( ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) && ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) ) {
				// errata ignore TC
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: TC blocks remaining %d, PSTAT %x dsaddr %x", __FUNCTION__, ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ), in32( base + ESDHC_PRSSTAT ), in32( base + ESDHC_DSADDR ) );
			}
			else {
				intr |= MMC_INTR_DATA;
			}
#endif
		}
		else if( istat & ESDHC_IRQ_ERROR_MASK ) {		// errors
			intr |= MMC_ERR_DATA_TO | MMC_INTR_ERROR;

			mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: DATA error blks remaining %d (%x %x)", __FUNCTION__, ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) ? ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) : 0, imsk, istat ); 

			if( esdhc_reset( hba, ESDHC_SYSCTL_RSTD | ESDHC_SYSCTL_RSTC, CAM_TRUE ) ) {
				mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ESDHC_SYSCTL_RSTD/ESDHC_SYSCTL_RSTC timeout", __FUNCTION__ );
			}
		}
	}

	if( istat & ESDHC_IRQ_CINS ) { 			// card inserted
		intr |= MMC_INTR_CARDINS;
		out32( base + ESDHC_IRQSTATEN, ( in32( base + ESDHC_IRQSTATEN ) & ~ESDHC_IRQ_CINS ) | ESDHC_IRQ_CRM );
		out32( base + ESDHC_IRQSIGEN, ( in32( base + ESDHC_IRQSIGEN ) & ~ESDHC_IRQ_CINS ) | ESDHC_IRQ_CRM );
	}

	if( istat & ESDHC_IRQ_CRM ) {			// card removed
		intr |= MMC_INTR_CARDRMV;
		out32( base + ESDHC_IRQSTATEN, ( in32( base + ESDHC_IRQSTATEN ) & ~ESDHC_IRQ_CRM ) | ESDHC_IRQ_CINS );
		out32( base + ESDHC_IRQSIGEN, ( in32( base + ESDHC_IRQSIGEN ) & ~ESDHC_IRQ_CRM ) | ESDHC_IRQ_CINS );
	}

	out32( base + ESDHC_IRQSTAT, istat );	// clear interrupts

	return( intr );
}

int esdhc_setup_xfer( SIM_HBA *hba, int xlen, int xmode, paddr_t paddr )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		blkcnt;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;
	blkcnt	= xlen / esdhc->blksz;

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: %s %s %d blocks, paddr %x", __FUNCTION__, ( xmode & ESDHC_XFERTYP_DMAEN ) ? "DMA" : "PIO", ( xmode & ESDHC_XFERTYP_DTD_RD ) ? "in" : "out", blkcnt, paddr );
	}

	if( blkcnt == 0 ) {
		return( 0 );
	}

	if( esdhc_prsstat( hba, 0, ESDHC_PRSSTAT_DLA, ESDHC_PRSSTAT_DATA_TIMEOUT ) ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: DLA timeout prstat %x", __FUNCTION__, in32( base + ESDHC_PRSSTAT ) ); 
	}

	if( blkcnt > 1 ) {
		xmode |= ESDHC_XFERTYP_MSBSEL | ESDHC_XFERTYP_BCEN;
		if( ext->hccap & MMC_HCCAP_ACMD12 ) {
			xmode |= ESDHC_XFERTYP_AC12EN;
		}
	}

	if( xmode & ESDHC_XFERTYP_DMAEN ) {
		// Note:  WML is set in esdhc_block_size()
		out32( base + ESDHC_DSADDR, paddr );
	}

	esdhc->xmode = xmode;
	out32( base + ESDHC_BLKATTR, esdhc->blksz | ( blkcnt << ESDHC_BLKATTR_BLKCNT_SHIFT ) );

	return( xlen );
}

int esdhc_setup_dma( SIM_HBA *hba, paddr_t paddr, int len, int dir )
{
	int		xmode;

	xmode = ESDHC_XFERTYP_DMAEN;
	if( dir == MMC_DIR_IN ) xmode |= ESDHC_XFERTYP_DTD_RD;
	return( esdhc_setup_xfer( hba, len, xmode, paddr ) );
}

int esdhc_setup_pio( SIM_HBA *hba, int len, int dir )
{
	return( esdhc_setup_xfer( hba, len, ( dir == MMC_DIR_IN ) ? ESDHC_XFERTYP_DTD_RD : 0, 0 ) );
}

int esdhc_dma_done( SIM_HBA *hba, int dir )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: %s blks remaining %d, dsaddr %x", __FUNCTION__, dir == MMC_DIR_IN ? "in" : "out", ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) ? ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) : 0, in32( base + ESDHC_DSADDR ) ); 
	}

	return( MMC_SUCCESS );
}

int esdhc_pio_done( SIM_HBA *hba, char *buf, int len, int dir )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		cnt;
	uint32_t		*pbuf;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;
	pbuf	= (uint32_t *)buf;

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: %s len %d, blks remaining %d, prsstat %x", __FUNCTION__, dir == MMC_DIR_IN ? "in" : "out", len, ( esdhc->xmode & ESDHC_XFERTYP_BCEN ) ? ( in32( base + ESDHC_BLKATTR ) >> ESDHC_BLKATTR_BLKCNT_SHIFT ) : 0, in32( base + ESDHC_PRSSTAT ) ); 
	}

	if( len < esdhc->blksz ) {
		return( 0 );
	}

	if( dir == MMC_DIR_IN ) {
		if( esdhc_prsstat( hba, ESDHC_PRSSTAT_BREN, ESDHC_PRSSTAT_BREN, 1 /*ESDHC_PRSSTAT_DATA_TIMEOUT*/ ) ) {
			if( in32( base + ESDHC_PRSSTAT ) & ESDHC_PRSSTAT_RTA ) {
				out32( base + ESDHC_IRQSTATEN, in32( base + ESDHC_IRQSTATEN ) | ESDHC_IRQ_BRR );
			}
			return( 0 );
		}
		for( cnt = esdhc->blksz / 4; cnt; cnt--, pbuf++ ) {
			*pbuf = ENDIAN_LE32( in32( base + ESDHC_DATPORT ) );
			nanospin_ns( ESDHC_DATPORT_DELAY );
		}
	}
	else {
		if( esdhc_prsstat( hba, ESDHC_PRSSTAT_BWEN, ESDHC_PRSSTAT_BWEN, 1 /*ESDHC_PRSSTAT_DATA_TIMEOUT*/) ) {
			if( in32( base + ESDHC_PRSSTAT ) & ESDHC_PRSSTAT_WTA ) {
				out32( base + ESDHC_IRQSTATEN, in32( base + ESDHC_IRQSTATEN ) | ESDHC_IRQ_BWR );
			}
			return( 0 );
		}
		for( cnt = esdhc->blksz / 4; cnt; cnt--, pbuf++ ) {
			out32( base + ESDHC_DATPORT, ENDIAN_LE32( *pbuf ) );
			nanospin_ns( ESDHC_DATPORT_DELAY );
		}
	}

	return( esdhc->blksz );
}

int esdhc_command( SIM_HBA *hba, mmc_cmd_t *cmd )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		mask;
	uint32_t		command;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;
	mask	= ESDHC_PRSSTAT_CIHB;
	command	= ESDHC_XFERTYP_CMDINX( cmd->opcode );

	if( cmd->resp & MMC_RSP_PRESENT ) {
		if( cmd->resp & MMC_RSP_OPCODE )	command |= ESDHC_XFERTYP_CIEN;
		if( cmd->resp & MMC_RSP_CRC )		command |= ESDHC_XFERTYP_CCCEN;
		if( cmd->resp & MMC_RSP_BUSY ) {		// must be R1b
			mask	|= ESDHC_PRSSTAT_CDIHB;
			command |= ESDHC_XFERTYP_RSPLEN48BSY;
		}
		else if( cmd->resp & MMC_RSP_136 ) { 	// must be R2
			command |= ESDHC_XFERTYP_RSPLEN136;
		}
		else {
			command |= ESDHC_XFERTYP_RSPLEN48;
		}
	}

	if( cmd->eflags & MMC_CMD_DATA ) {
		mask	|= ESDHC_PRSSTAT_CDIHB;
		command	|= ESDHC_XFERTYP_DPSEL;		// data present select
	}

		// CMD0, CMD12, CMD13 and CMD52 can be
		// isused when SDHC_DAT lines are busy
	if( cmd->opcode == MMC_STOP_TRANSMISSION ) {
		mask	&= ~ESDHC_PRSSTAT_CDIHB;
		command |= ESDHC_XFERTYP_CMDTYP_ABORT;
	}

	if( esdhc_prsstat( hba, 0, mask, ESDHC_PRSSTAT_CMD_TIMEOUT ) ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: Timeout PRSSTAT %x", __FUNCTION__, in32( base + ESDHC_PRSSTAT ) );
		return( MMC_FAILURE );
	}

	mask = ESDHC_IRQ_ERROR_MASK | ESDHC_IRQ_CRM;
	if( cmd->eflags & MMC_CMD_DATA ) {
		command |= esdhc->xmode;
		if( cmd->eflags & MMC_CMD_DATA_DMA ) {
			mask |= ESDHC_IRQ_DINT | ESDHC_IRQ_TC;
				// disable buffer buffer read/write ready interrupts
			mask &= ~( ESDHC_IRQ_BRR | ESDHC_IRQ_BWR );
			out32( base + ESDHC_IRQSIGEN, in32( base + ESDHC_IRQSIGEN ) & ~( ESDHC_IRQ_BRR | ESDHC_IRQ_BWR ) );
		}
		else {
			if( cmd->eflags & MMC_CMD_DATA_IN ) {
				mask |= ESDHC_IRQ_BRR;
			}
			else {
				mask |= ESDHC_IRQ_BWR | ESDHC_IRQ_TC;
			}
			out32( base + ESDHC_IRQSIGEN, in32( base + ESDHC_IRQSIGEN ) | ( ESDHC_IRQ_BRR | ESDHC_IRQ_BWR ) );
		}
	}
	else {
		mask |= ESDHC_IRQ_CC;
	}

	if(hba->verbosity > 3){
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: cmd %x, msk %x, prstat %x, istat %x", __FUNCTION__, command, mask, in32( base + ESDHC_PRSSTAT ), in32( base + ESDHC_IRQSTAT ) );
	}

	out32( base + ESDHC_IRQSTATEN, mask );
	out32( base + ESDHC_CMDARG, cmd->argument );
	out32( base + ESDHC_XFERTYP, command );

	return( MMC_SUCCESS );
}

int esdhc_cfg_bus( SIM_HBA *hba, int width, int mmc )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		proctl;

	ext   = (SIM_MMC_EXT *)hba->ext;
	esdhc = (esdhc_ext_t *)ext->handle;
	base  = esdhc->base;

	proctl = in32( base + ESDHC_PROCTL ) & ~ESDHC_PROCTL_DTW_MASK;
	switch( width ) {
		case 8:
			proctl |= ESDHC_PROCTL_DTW_8; break;
		case 4:
			proctl |= ESDHC_PROCTL_DTW_4; break;
		default:
			break;
	}

	out32( base + ESDHC_PROCTL, proctl );

	return( MMC_SUCCESS );
}

int esdhc_block_size( SIM_HBA *hba, int blksz )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;
	uint32_t		wml_rd;
	uint32_t		wml_wr;

	ext   = (SIM_MMC_EXT *)hba->ext;
	esdhc = (esdhc_ext_t *)ext->handle;
	base  = esdhc->base;

	if( blksz > 4095 ) {
		return( MMC_FAILURE );
	}

	esdhc->blksz	= blksz;
	wml_rd			= min( 0x10, esdhc->blksz / 4 );
	wml_wr			= min( 0x80, esdhc->blksz / 4 );
	out32( base + ESDHC_WML, ( wml_wr << 16 ) | wml_rd );
	out32( base + ESDHC_BLKATTR, blksz );
	return( MMC_SUCCESS );
}

int esdhc_powerup( SIM_HBA *hba )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;

	ext			= (SIM_MMC_EXT *)hba->ext;
	esdhc		= (esdhc_ext_t *)ext->handle;
	base		= esdhc->base;
	ext->eflags	&= ~MMC_EFLAG_WP;

	if( !( in32( base + ESDHC_PRSSTAT ) & ESDHC_PRSSTAT_WPSPL ) ) {
		ext->eflags |= MMC_EFLAG_WP;
	}

	if( esdhc_reset( hba, ESDHC_SYSCTL_RSTA, CAM_FALSE ) ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ESDHC_SYSCTL_RSTA timeout", __FUNCTION__ );
	}

	if( esdhc_reset( hba, ESDHC_SYSCTL_INITA, CAM_FALSE ) ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: ESDHC_SYSCTL_INITA timeout", __FUNCTION__ );
	}

	_esdhc_clock( hba, 400000 );

		// config signals and enable removal interrupt
	out32( base + ESDHC_IRQSIGEN, ESDHC_IRQ_MASK | ESDHC_IRQ_CRM );
	out32( base + ESDHC_IRQSTATEN, ESDHC_IRQ_CRM );

	return( MMC_SUCCESS );
}

int esdhc_powerdown( SIM_HBA *hba )
{
	SIM_MMC_EXT		*ext;
	esdhc_ext_t		*esdhc;
	uintptr_t		base;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;
	base	= esdhc->base;

// need clocks to receive insertion interrupts
//	out32( base + ESDHC_SYSCTL, 0 );		// disable clocks
	return( MMC_SUCCESS );
}

int esdhc_shutdown( SIM_HBA *hba )
{
	SIM_MMC_EXT			*ext;
	esdhc_ext_t			*esdhc;

	ext		= (SIM_MMC_EXT *)hba->ext;
	esdhc	= (esdhc_ext_t *)ext->handle;

	esdhc_reset( hba, ESDHC_SYSCTL_RSTA, CAM_FALSE );
	esdhc_powerdown( hba );
	munmap_device_memory( (void *)esdhc->base, hba->cfg.MemLength[0] );
	free( esdhc );

	return( MMC_SUCCESS );
}

uint32_t mpc_immr( )
{
	struct asinfo_entry		*asinfo;
	int32_t					num;
	char					*name;

	num		= _syspage_ptr->asinfo.entry_size / sizeof( *asinfo );
	asinfo	= SYSPAGE_ENTRY( asinfo );

	for( ; num; num-- ) {
		name = __hwi_find_string( asinfo->name );
		if( strcmp( name, "immr" ) ) {
			asinfo++;
			continue;
		}
		return( (uint32_t)asinfo->start );
	}
	return( 0 );
}

int esdhc_init_mux( SIM_HBA *hba )
{
	uintptr_t	base;

	if( (void *)( base = (uintptr_t)mmap_device_memory( NULL, 0x4, PROT_READ | PROT_WRITE |
			PROT_NOCACHE, MAP_SHARED, mpc_immr( ) + MPC_PMUXCR ) ) == MAP_FAILED ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: mmap_device_memory failed", __FUNCTION__ );
		return( MMC_FAILURE );
	}

	InterruptDisable( );
	out32( base, in32( base ) | MPC_PMUXCR_SDHC_CD | MPC_PMUXCR_SDHC_WP );
	InterruptEnable( );

	munmap_device_memory( (void *)base, 0x4 );
	return( MMC_SUCCESS );
}

int	esdhc_init( SIM_HBA *hba )
{
	CONFIG_INFO			*cfg;
	SIM_MMC_EXT			*ext;
	esdhc_ext_t			*esdhc;
	uintptr_t			base;
	uint32_t			cap;
	uint32_t			ver;
	unsigned			hwi_off;
	hwi_tag				*loc_tag;

	ext = (SIM_MMC_EXT *)hba->ext;
	cfg = (CONFIG_INFO *)&hba->cfg;

	if( ( esdhc = calloc( 1, sizeof( esdhc_ext_t ) ) ) == NULL ) {
		return( MMC_FAILURE );
	}

	hwi_off = hwi_find_device( PPC85xx_HWI_SDHC, 0 );
	loc_tag = hwi_tag_find( hwi_off, HWI_TAG_NAME_location, NULL );
	cfg->IRQRegisters[0]	= ( hwi_off != HWI_NULL_OFF ) ? hwitag_find_ivec( hwi_off, NULL ) : 56;
	cfg->NumIRQs			= 1;
	cfg->MemBase[0]			= loc_tag ? loc_tag->location.base : mpc_immr( ) + MPC8536_ESDHC_BASE;
	cfg->MemLength[0]		= loc_tag ? loc_tag->location.len : MPC8536_ESDHC_LEN;
	cfg->NumMemWindows		= 1;

	if( (void *)( base = (uintptr_t)mmap_device_memory( NULL, cfg->MemLength[0],
			PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, cfg->MemBase[0] ) ) == MAP_FAILED ) {
		mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "%s: mmap_device_memory %s", __FUNCTION__, strerror( errno ) );
		return( MMC_FAILURE );
	}

	ver	= in32( base + ESDHC_HOSTVER );
	mmc_slogf( hba, _SLOGC_SIM_MMC, _SLOG_ERROR, 1, "Freescale ESDHC (VNN-%x, SVN-%x)", ( ver >> 8 ) & 0xff, ver & 0xff );

	cap = in32( base + ESDHC_HOSTCAPBLT );
	if( cap & ESDHC_HOSTCAPBLT_VS18 ) ext->hccap |= MMC_HCCAP_18V;
	if( cap & ESDHC_HOSTCAPBLT_VS30) ext->hccap |= MMC_HCCAP_30V;
	if( cap & ESDHC_HOSTCAPBLT_VS33 ) ext->hccap |= MMC_HCCAP_33V;
	if( cap & ESDHC_HOSTCAPBLT_DMAS ) ext->hccap |= MMC_HCCAP_DMA;
	if( cap & ESDHC_HOSTCAPBLT_HSS ) ext->hccap |= MMC_HCCAP_HS;
	ext->hccap		|=	MMC_HCCAP_BW1 | MMC_HCCAP_BW4 | MMC_HCCAP_BW8 |
						MMC_HCCAP_CD_INTR;// | MMC_HCCAP_ACMD12;
//	ext->maxio		= ESDHC_MAXIO;
	esdhc->clock	= MPC8536_CCB_CLOCK;
	esdhc->base		= base;
	ext->handle		= esdhc;
	ext->clock		= esdhc->clock;
	ext->detect		= esdhc_detect;
	ext->powerup	= esdhc_powerup;
	ext->powerdown	= esdhc_powerdown;
	ext->cfg_bus	= esdhc_cfg_bus;
	ext->set_clock	= esdhc_clock;
	ext->set_blksz	= esdhc_block_size;
	ext->interrupt	= esdhc_interrupt;
	ext->command	= esdhc_command;
	ext->setup_dma	= esdhc_setup_dma;
	ext->dma_done	= esdhc_dma_done;
	ext->setup_pio	= esdhc_setup_pio;
	ext->pio_done	= esdhc_pio_done;
	ext->shutdown	= esdhc_shutdown;

	esdhc_init_mux( hba );

	out32( base + ESDHC_PROCTL, ESDHC_PROCTL_EMODE_AI );
	out32( base + ESDHC_SCR, in32( base + ESDHC_SCR ) | ESDHC_SCR_SNOOP );
	_esdhc_clock( hba, 400000 );
	out32( base + ESDHC_SYSCTL, in32( base + ESDHC_SYSCTL ) | ESDHC_SYSCTL_HCKEN | ESDHC_SYSCTL_IPGEN );

		// configure signals and enable insertion interrrupt
	out32( base + ESDHC_IRQSTAT, in32( base + ESDHC_IRQSTAT ) );
	out32( base + ESDHC_IRQSIGEN, ESDHC_IRQ_MASK | ESDHC_IRQ_CINS );
	out32( base + ESDHC_IRQSTATEN, ESDHC_IRQ_CINS );

	strncpy( cfg->Description, "Freescale eSDHC", sizeof( cfg->Description ) );

	return( MMC_SUCCESS );
}

#endif

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devb/mmcsd/chipsets/sim_esdhc.c $ $Rev: 657836 $" );
