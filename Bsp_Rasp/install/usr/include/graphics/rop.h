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





#ifndef _GRAPHICS_ROP_H_INCLUDED
#define _GRAPHICS_ROP_H_INCLUDED

/*
 * Raster Op Defines
 *
 * Reverse Polish Notation Defines of ROPS
 * This follows Microsofts 256 raster operation
 * definitions
 *
 * D = Dest Bit
 * S = Source Bit
 * P = Pattern Bit
 * o = bitwise or operation
 * a = bitwise and operation
 * x = bitwise exclusive or operation
 * n = bitwise not operation
 *
 * Truth Table
 *	P	S	D	ropbit
 *	0	0	0	0
 *	0	0	1	1
 * 	0	1	0	2
 *	0	1	1	3
 *	1	0	0	4
 *	1	0	1	5
 *	1	1	0	6
 *	1	1	1	7
 *
 * if the condition in the table is true for 
 * each test turned on then dst bit=1, else 0
 *
 * So: DrawMode CC =  Source Copy
 * 		if ((conditions 2 | 3 | 6 | 7) == 1) on current bits, dst bit=1, else 0
 *	   DrawMode 66 = S ^ D
 *		if ((conditions 1 | 2 | 5 | 6) == 1) on current bits, dst bit=1, else 0
 *	   DrawMode 03 = ~ (P | S)
 *      if ((conditions 0 | 1) == 1) on current bits, dst bit=1, else 0
 *
 * It's a little confusing at first, but it does make sense once you wrap
 * your brain around it.
 *
 *          Name                Rop #            Operation
 *--------------------------------------------------------------------------
 */
#define DrawModeZERO			0x00	/* 0 */
#define DrawModeDPSoon			0x01	/* ~ ( (D | (P | S) ) ) */
#define DrawModeDPSona			0x02	/* D & ( ~ (P | S) ) */
#define DrawModePSon			0x03	/* ~ (P | S) */
#define DrawModeSDPona			0x04	/* S & ( ~ (D | P) ) */
#define DrawModeDPon			0x05	/* ~ (D | P) */
#define DrawModePDSxnon			0x06	/* ~ (P | (~ (D ^ S))) */
#define DrawModePDSaon			0x07	/* ~ (P | (D & S)) */
#define DrawModeSDPnaa			0x08	/* S & (D & (~ P)) */
#define DrawModePDSxon			0x09	/* ~ (P | (D ^ S)) */
#define DrawModeDPna			0x0A	/* D & (~ P) */
#define DrawModePSDnaon			0x0B	/* ~ (P | (S & (~ D))) */
#define DrawModeSPna			0x0C	/* S & (~ P) */
#define DrawModePDSnaon			0x0D	/* ~ (P | (D & (~ S))) */
#define DrawModePDSonon			0x0E	/* ~ (P | (~(D | S))) */
#define DrawModePn			0x0F	/* ~ P */
#define DrawModePDSona			0x10	/* P & (~ (D | S)) */
#define DrawModeDSon			0x11	/* ~ (D | S) */
#define DrawModeSDPxnon			0x12	/* ~ (S | (~ (D ^ P))) */
#define DrawModeSDPaon			0x13	/* ~ (S | (D & P)) */
#define DrawModeDPSxnon			0x14	/* ~ (D | (~ (P ^ S))) */
#define DrawModeDPSaon			0x15	/* ~ (D | (P & S)) */
#define DrawModePSDPSanaxx		0x16	/* P ^ (S ^ (D & (~ (P & S)))) */
#define DrawModeSSPxDSxaxn		0x17	/* ~ (S ^ ((S ^ P) & (D ^ S))) */
#define DrawModeSPxPDxa			0x18	/* (S ^ P) & (P ^ D) */
#define DrawModeSDPSanaxn		0x19	/* ~ (S ^ (D & (~ (P & S)))) */
#define DrawModePDSPaox			0x1A	/* P ^ (D | (S & P)) */
#define DrawModeSDPSxaxn		0x1B	/* ~ (S ^ (D & (P ^ S)))) */
#define DrawModePSDPaox			0x1C	/* P ^ (S | (D & P))) */
#define DrawModeDSPDxaxn		0x1D	/* ~ (D ^ (S & (P ^ D))) */
#define DrawModePDSox			0x1E	/* P ^ (D | S) */
#define DrawModePDSoan			0x1F	/* ~ (P & (D | S)) */
#define DrawModeDPSnaa			0x20	/* D & (P & (~S)) */
#define DrawModeSDPxon			0x21	/* ~ (S | (D ^ P)) */
#define DrawModeDSna			0x22	/* D & (~S) */
#define DrawModeSPDnaon			0x23	/* ~ (S | (P & (~D))) */
#define DrawModeSPxDSxa			0x24	/* (S ^ P) & (D ^ S) */
#define DrawModePDSPanaxn		0x25	/* ~ (P ^ (D & (~ (S & P)))) */
#define DrawModeSDPSaox			0x26	/* S ^ (D | (P & S)) */
#define DrawModeSDPSxnox		0x27	/* S ^ (D | (~ (P ^ S))) */
#define DrawModeDPSxa			0x28	/* D & (P ^ S) */
#define DrawModePSDPSaoxxn		0x29	/* ~ (P ^ (S ^ (D | (P & S)))) */
#define DrawModeDPSana			0x2A	/* D & (~ (P & S)) */
#define DrawModeSSPxPDxaxn		0x2B	/* ~ (S ^ ((S ^ P) & (P ^ D)) */
#define DrawModeSPDSoax			0x2C	/* S ^ (P & (D | S)) */
#define DrawModePSDnox			0x2D	/* P ^ (S | (~D)) */
#define DrawModePSDPxox			0x2E	/* P ^ (S | (D ^ P)) */
#define DrawModePSDnoan			0x2F	/* ~ (P & (S | (~D))) */
#define DrawModePSna			0x30	/* P & (~ S) */
#define DrawModeSDPnaon			0x31	/* ~ (S | (D & (~P))) */
#define DrawModeSDPSoox			0x32	/* S ^ (D | (P | S)) */
#define DrawModeSn			0x33	/* ~S */
#define DrawModeSPDSaox			0x34	/* S ^ (P | (D & S)) */
#define DrawModeSPDSxnox		0x35	/* S ^ (P | (~ (D ^ S))) */
#define DrawModeSDPox			0x36	/* S ^ (D | P) */
#define DrawModeSDPoan			0x37	/* ~ (S & (D | P)) */
#define DrawModePSDPoax			0x38	/* P ^ (S & (D | P))) */
#define DrawModeSPDnox			0x39	/* S ^ (P | (~ D))) */
#define DrawModeSPDSxox			0x3A	/* S ^ (P | (D ^ S)) */
#define DrawModeSPDnoan			0x3B	/* ~ (S & (P | (~ D))) */
#define DrawModePSx			0x3C	/* P ^ S */
#define DrawModeSPDSonox		0x3D	/* S ^ (P | (~ (D | S))) */
#define DrawModeSPDSnaox		0x3E	/* S ^ (P | (D & (~ S))) */
#define DrawModePSan			0x3F	/* ~ (P & S) */
#define DrawModePSDnaa			0x40	/* P & (S & (~ D)) */
#define DrawModeDPSxon			0x41	/* ~ (D | (P ^ S) */
#define DrawModeSDxPDxa			0x42	/* (S ^ D) & (P ^ D) */
#define DrawModeSPDSanaxn		0x43	/* ~ (S ^ (P & (~ (D & S)))) */
#define DrawModeSDna			0x44	/* S & (~ D) */
#define DrawModeDPSnaon			0x45	/* ~ (D | (P & (~ S))) */
#define DrawModeDSPDaox			0x46	/* D ^ (S | (P & D))) */
#define DrawModePSDPxaxn		0x47	/* ~ (P ^ (S & (D ^ P))) */
#define DrawModeSDPxa			0x48	/* S & (D ^ P)) */
#define DrawModePDSPDoaxxn		0x49	/* ~ (P ^ (D ^ (S & (P | D)))) */
#define DrawModeDPSDoax			0x4A	/* D ^ (P & (S | D)) */
#define DrawModePDSnox			0x4B	/* P ^ (D | (~ S)) */
#define DrawModeSDPana			0x4C	/* S & (~ (D & P)) */
#define DrawModeSSPxDSxoxn		0x4D	/* ~ (S ^ ((S ^ P) | (D ^ S))) */
#define DrawModePDSPxox			0x4E	/* P ^ (D | (S ^ P)) */
#define DrawModePDSnoan			0x4F	/* ~ (P & (D | (~S))) */
#define DrawModePDna			0x50	/* (~ D) & P */
#define DrawModeDSPnaon			0x51	/* ~ (D | (S & (~P))) */
#define DrawModeDPSDaox			0x52	/* D ^ (P | (S & D)) */
#define DrawModeSPDSxaxn		0x53	/* ~ (S ^ (P & (D ^ S))) */
#define DrawModeDPSonon			0x54	/* ~ (D | (~ (P | S))) */
#define DrawModeDn			0x55	/* ~ D */
#define DrawModeDPSox			0x56	/* D ^ (P | S) */
#define DrawModeDPSoan			0x57	/* ~ (D & (P | S)) */
#define DrawModePDSPoax			0x58	/* P ^ (D & (S | P)) */
#define DrawModeDPSnox			0x59	/* D ^ (P | (~ S)) */
#define DrawModeDPx			0x5A	/* D ^ P */
#define DrawModeDPSDonox		0x5B	/* D ^ (P | (~ (S | D))) */
#define DrawModeDPSDxox			0x5C	/* D ^ (P | (S ^ D)) */
#define DrawModeDPSnoan			0x5D	/* ~ (D & (P | (~ S))) */
#define DrawModeDPSDnaox		0x5E	/* D ^ (P | (S & (~ D))) */
#define DrawModeDPan			0x5F	/* ~ (D & P) */
#define DrawModePDSxa			0x60	/* P & (D ^ S) */
#define DrawModeDSPDSaoxxn		0x61	/* ~ (D ^ (S ^ (P | (D & S)))) */
#define DrawModeDSPDoax			0x62	/* D ^ (S & (P | D)) */
#define DrawModeSDPnox			0x63	/* S ^ (D | (~ P)) */
#define DrawModeSDPSoax			0x64	/* S ^ (D & (P | S)) */
#define DrawModeDSPnox			0x65	/* D ^ (S | (~ P)) */
#define DrawModeDSx			0x66	/* D ^ S */
#define DrawModeSDPSonox		0x67	/* S ^ (D | (~ (P | S))) */
#define DrawModeDSPDSonoxxn		0x68	/* ~ (D ^ (S ^ (P | (~ (D | S))))) */
#define DrawModePDSxxn			0x69	/* ~ (P ^ (D ^ S)) */
#define DrawModeDPSax			0x6A	/* D ^ (P & S) */
#define DrawModePSDPSoaxxn		0x6B	/* ~ (P ^ (S ^ (D & (P | S)))) */
#define DrawModeSDPax			0x6C	/* S ^ (D & P) */
#define DrawModePDSPDoaxx		0x6D	/* P ^ (D ^ (S & (P | D))) */
#define DrawModeSDPSnoax		0x6E	/* S ^ (D & (P | (~ S))) */
#define DrawModePDSxnan			0x6F	/* ~ (P & (~ (D ^ S))) */
#define DrawModePDSana			0x70	/* P & (~ (D & S)) */
#define DrawModeSSDxPDxaxn		0x71	/* ~ (S ^ ((S ^ D) & (P ^ D))) */
#define DrawModeSDPSxox			0x72	/* S ^ (D | (P ^ S)) */
#define DrawModeSDPnoan			0x73	/* ~ (S & (D | (~P))) */
#define DrawModeDSPDxox			0x74	/* D ^ (S | (P ^ D)) */
#define DrawModeDSPnoan			0x75	/* ~ (D & (S | (~ P))) */
#define DrawModeSDPSnaox		0x76	/* S ^ (D | (P & (~ S))) */
#define DrawModeDSan			0x77	/* ~ (D & S) */
#define DrawModePDSax			0x78	/* P ^ (D & S) */
#define DrawModeDSPDSoaxxn		0x79	/* ~ (D ^ (S ^ (P & (D | S)))) */
#define DrawModeDPSDnoax		0x7A	/* D ^ (P & (S | (~ D))) */
#define DrawModeSDPxnan			0x7B	/* ~ (S & (~ (D ^ P))) */
#define DrawModeSPDSnoax		0x7C	/* S ^ (P & (D | (~ S))) */
#define DrawModeDPSxnan			0x7D	/* ~ (D & (~ (P ^ S))) */
#define DrawModeSPxDSxo			0x7E	/* (S ^ P) | (D ^ S) */
#define DrawModeDPSaan			0x7F	/* ~ (D & (P & S)) */
#define DrawModeDPSaa			0x80	/* D & (P & S) */
#define DrawModeSPxDSxon		0x81	/* ~ ((P ^ S) | (D ^ S)) */
#define DrawModeDPSxna			0x82	/* D & (~ (P ^ S)) */
#define DrawModeSPDSnoaxn		0x83	/* ~ (S ^ (P & (D | (~ S)))) */
#define DrawModeSDPxna			0x84	/* S & (~ (D ^ P)) */
#define DrawModePDSPnoaxn		0x85	/* ~ (P ^ (D & (S | (~ P)))) */
#define DrawModeDSPDSoaxx		0x86	/* D ^ (S ^ (P & (D | S))) */
#define DrawModePDSaxn			0x87	/* ~ (P ^ (D & S)) */
#define DrawModeDSa			0x88	/* D & S */
#define DrawModeSDPSnaoxn		0x89	/* ~ (S ^ (D | (P & (~ S)))) */
#define DrawModeDSPnoa			0x8A	/* D & (S | (~ P)) */
#define DrawModeDSPDxoxn		0x8B	/* ~ (D ^ (S | (P ^ D))) */
#define DrawModeSDPnoa			0x8C	/* S & (D | (~ P)) */
#define DrawModeSDPSxoxn		0x8D	/* ~ (S ^ (D | (P ^ S))) */
#define DrawModeSSDxPDxax		0x8E	/* S ^ ((S ^ D) & (P ^ D)) */
#define DrawModePDSanan			0x8F	/* ~ (P & (~ (D & S))) */
#define DrawModePDSxna			0x90	/* P & (~ (D ^ S)) */
#define DrawModeSDPSnoaxn		0x91	/* ~ (S ^ (D & (P | (~ S)))) */
#define DrawModeDPSDPoaxx		0x92	/* D ^ (P ^ (S & (D | P))) */
#define DrawModeSPDaxn			0x93	/* ~ (S ^ (P & D)) */
#define DrawModePSDPSoaxx		0x94	/* P ^ (S ^ (D & (P | S))) */
#define DrawModeDPSaxn			0x95	/* ~ (D ^ (P & S)) */
#define DrawModeDPSxx			0x96	/* D ^ (P ^ S) */
#define DrawModePSDPSonoxx		0x97	/* P ^ (S ^ (D | (~ (P | S)))) */
#define DrawModeSDPSonoxn		0x98	/* ~ (S ^ (D | (~ (P | S)))) */
#define DrawModeDSxn			0x99	/* ~ (D ^ S) */
#define DrawModeDPSnax			0x9A	/* D ^ (P & (~ S)) */
#define DrawModeSDPSoaxn		0x9B	/* ~ (S ^ (D & (P | S))) */
#define DrawModeSPDnax			0x9C	/* S ^ (P & (~ D)) */
#define DrawModeDSPDoaxn		0x9D	/* ~ (D ^ (S & (P | D))) */
#define DrawModeDSPDSaoxx		0x9E	/* D ^ (S ^ (P | (D & S))) */
#define DrawModePDSxan			0x9F	/* ~ (P & (D ^ S)) */
#define DrawModeDPa			0xA0	/* (D & P) */
#define DrawModePDSPnaoxn		0xA1	/* ~ (P ^ (D | (S & (~ P)))) */
#define DrawModeDPSnoa			0xA2	/* D & (P | (~ S)) */
#define DrawModeDPSDxoxn		0xA3	/* ~ (D ^ (P | (S ^ D))) */
#define DrawModePDSPonoxn		0xA4	/* ~ (P ^ (D | (~ (S | P)))) */
#define DrawModePDxn			0xA5	/* ~ (P ^ D) */
#define DrawModeDSPnax			0xA6	/* D ^ (S & (~ P)) */
#define DrawModePDSPoaxn		0xA7	/* ~ (P ^ (D & (S | P))) */
#define DrawModeDPSoa			0xA8	/* D & (P | S) */
#define DrawModeDPSoxn			0xA9	/* ~ (D ^ (P | S)) */
#define DrawModeD			0xAA	/* D */
#define DrawModeDPSono			0xAB	/* D | (~ (P | S)) */
#define DrawModeSPDSxax			0xAC	/* S ^ (P & (D ^ S)) */
#define DrawModeDPSDaoxn		0xAD	/* ~ (D ^ (P | (S & D))) */
#define DrawModeDSPnao			0xAE	/* D | (S & (~ P)) */
#define DrawModeDPno			0xAF	/* D | (~ P) */
#define DrawModePDSnoa			0xB0	/* P & (D | (~ S)) */
#define DrawModePDSPxoxn		0xB1	/* ~ (P ^ (D | (S ^ P))) */
#define DrawModeSSPxDSxox		0xB2	/* S ^ ((S ^ P) | (D ^ S)) */
#define DrawModeSDPanan			0xB3	/* ~ (S & (~ (D & P))) */
#define DrawModePSDnax			0xB4	/* P ^ (S & (~ D)) */
#define DrawModeDPSDoaxn		0xB5	/* ~ (D ^ (P & (S | D))) */
#define DrawModeDPSDPaoxx		0xB6	/* D ^ (P ^ (S | (D & P))) */
#define DrawModeSDPxan			0xB7	/* ~ (S & (D ^ P)) */
#define DrawModePSDPxax			0xB8	/* P ^ (S & (D ^ P)) */
#define DrawModeDSPDaoxn		0xB9	/* ~ (D ^ (S | (P & D))) */
#define DrawModeDPSnao			0xBA	/* D | (P & (~ S)) */
#define DrawModeDSno			0xBB	/* D | (~ S) */
#define DrawModeSPDSanax		0xBC	/* S ^ (P & (~ (D & S))) */
#define DrawModeSDxPDxan		0xBD	/* ~ ((S ^ D) & (P ^ D)) */
#define DrawModeDPSxo			0xBE	/* D | (P ^ S) */
#define DrawModeDPSano			0xBF	/* D | (~ (P & S)) */
#define DrawModePSa			0xC0	/* P & S */
#define DrawModeSPDSnaoxn		0xC1	/* ~ (S ^ (P | (D & (~ S)))) */
#define DrawModeSPDSonoxn		0xC2	/* ~ (S ^ (P | (~ (D | S)))) */
#define DrawModePSxn			0xC3	/* ~ (P ^ S) */
#define DrawModeSPDnoa			0xC4	/* S & (P | (~ D)) */
#define DrawModeSPDSxoxn		0xC5	/* ~ (S ^ (P | (D ^ S))) */
#define DrawModeSDPnax			0xC6	/* S ^ (D & (~ P)) */
#define DrawModePSDPoaxn		0xC7	/* ~ (P ^ (S & (D | P))) */
#define DrawModeSDPoa			0xC8	/* S & (D | P) */
#define DrawModeSPDoxn			0xC9	/* ~ (S ^ (P | D)) */
#define DrawModeDPSDxax			0xCA	/* D ^ (P & (S ^ D)) */
#define DrawModeSPDSaoxn		0xCB	/* ~ (S ^ (P | (D & S))) */
#define DrawModeS			0xCC	/* S */
#define DrawModeSDPono			0xCD	/* S | (~ (D | P)) */
#define DrawModeSDPnao			0xCE	/* S | (D & (~ P)) */
#define DrawModeSPno			0xCF	/* S | (~ P) */
#define DrawModePSDnoa			0xD0	/* P & (S | (~ D)) */
#define DrawModePSDPxoxn		0xD1	/* ~ (P ^ (S | (D ^ P))) */
#define DrawModePDSnax			0xD2	/* P ^ (D & (~ S)) */
#define DrawModeSPDSoaxn		0xD3	/* ~ (S ^ (P & (D | S))) */
#define DrawModeSSPxPDxax		0xD4	/* S ^ ((S ^ P) & (P ^ D)) */
#define DrawModeDPSanan			0xD5	/* ~ (D & (~ (P & S))) */
#define DrawModePSDPSaoxx		0xD6	/* P ^ (S ^ (D | (P & S))) */
#define DrawModeDPSxan			0xD7	/* ~ (D & (P ^ S)) */
#define DrawModePDSPxax			0xD8	/* P ^ (D & (S ^ P)) */
#define DrawModeSDPSaoxn		0xD9	/* ~ (S ^ (D | (P & S))) */
#define DrawModeDPSDanax		0xDA	/* D ^ (P & (~ (S & D))) */
#define DrawModeSPxDSxan		0xDB	/* ~ ((S ^ P) & (D ^ S)) */
#define DrawModeSPDnao			0xDC	/* S | (P & (~ D)) */
#define DrawModeSDno			0xDD	/* S | (~ D) */
#define DrawModeSDPxo			0xDE	/* S | (D ^ P) */
#define DrawModeSDPano			0xDF	/* S | (~ (D & P)) */
#define DrawModePDSoa			0xE0	/* P & (D | S) */
#define DrawModePDSoxn			0xE1	/* ~ (P ^ (D | S)) */
#define DrawModeDSPDxax			0xE2	/* D ^ (S & (P ^ D)) */
#define DrawModePSDPaoxn		0xE3	/* ~ (P ^ (S | (D & P))) */
#define DrawModeSDPSxax			0xE4	/* S ^ (S & (P ^ S)) */
#define DrawModePDSPaoxn		0xE5	/* ~ (P ^ (D | (S & P))) */
#define DrawModeSDPSanax		0xE6	/* S ^ (D & (~ (P & S))) */
#define DrawModeSPxDPxan		0xE7	/* ~ ((S ^ P) & (D ^ P)) */
#define DrawModeSSPxDSxax		0xE8	/* S ^ ((S ^ P) & (D ^ S)) */
#define DrawModeDSPDSanaxxn		0xE9	/* ~ (D ^ (S ^ (P & (~ (D & S))))) */
#define DrawModeDPSao			0xEA	/* D | (P & S) */
#define DrawModeDPSxno			0xEB	/* D | (~ (P ^ S)) */
#define DrawModeSDPao			0xEC	/* S | (D & P) */
#define DrawModeSDPxno			0xED	/* S | (~ (D ^ P)) */
#define DrawModeDSo			0xEE	/* D | S */
#define DrawModeSDPnoo			0xEF	/* S | (D | (~ P)) */
#define DrawModeP			0xF0	/* P */
#define DrawModePDSono			0xF1	/* P | (~ (D | S)) */
#define DrawModePDSnao			0xF2	/* P | (D & (~ S)) */
#define DrawModePSno			0xF3	/* P | (~ S) */
#define DrawModePSDnao			0xF4	/* P | (S & (~ D)) */
#define DrawModePDno			0xF5	/* P | (~ D) */
#define DrawModePDSxo			0xF6	/* P | (D ^ S) */
#define DrawModePDSano			0xF7	/* P | (~ (D & S)) */
#define DrawModePDSao			0xF8	/* P | (D & S) */
#define DrawModePDSxno			0xF9	/* P | (~ (D ^ S)) */
#define DrawModeDPo			0xFA	/* D | P */
#define DrawModeDPSnoo			0xFB	/* D | (P | (~ S)) */
#define DrawModePSo			0xFC	/* P | S */
#define DrawModePSDnoo			0xFD	/* P | (S | (~ D)) */
#define DrawModeDPSoo			0xFE	/* D | (P | S) */
#define DrawModeONE			0xFF	/* 1 */

/* Convenience Defines */
#define DrawModeBLACK			DrawModeZERO
#define DrawModeWHITE			DrawModeONE

#define DrawModeSRCCOPY			DrawModeS
#define DrawModeSRCINVERT		DrawModeSn

#define DrawModePATCOPY			DrawModeP
#define DrawModePATINVERT		DrawModePn

#define DrawModeNOP			DrawModeD
#define DrawModeDSTINVERT		DrawModeDn

#define DrawModeSRCXOR			DrawModeDSx
#define DrawModePATXOR			DrawModeDPx
#define DrawModeSRCAND			DrawModeDSa
#define DrawModePATAND			DrawModeDPa
#define DrawModeSRCOR			DrawModeDSo
#define DrawModePATOR			DrawModeDPo

/* Simple ROP defines */
#define DrawModeOpaque			DrawModeSRCCOPY
#define DrawModeXOR			DrawModeSRCXOR
#define DrawModeAND			DrawModeSRCAND
#define DrawModeOR			DrawModeSRCOR

/* Defines for interpreting ternary Raster OPs (ROP3) */
#define DISP_ROP_STUPID(__rop)		((__rop) == 0 | (__rop) == 0xff)
#define DISP_ROP_NO_PATTERN(__rop)	(((__rop) & 0xf) == \
					    (((__rop) >> 4) & 0xf))
#define DISP_ROP_NO_SOURCE(__rop)	(((__rop) & 0x33) == \
					    (((__rop) >> 2) & 0x33))
#define DISP_ROP_NO_DEST(__rop)		(((__rop) & 0x55) == \
					    (((__rop) >> 1) & 0x55))

/* Rastor op conversion macros */
#define DISP_ROP_SRC_TO_PAT(__rop)	(((__rop) & 0xc0) | 		\
					    ((__rop) & 0xc) << 2 |	\
					    ((__rop) & 0x30) >> 2 |	\
					    ((__rop) & 0x3))
#define DISP_ROP_PAT_TO_SRC(__rop)	(((__rop) & 0xc0) |		\
					    ((__rop) & 0x30) >> 2 |	\
					    ((__rop) & 0xc) << 2 |	\
					    ((__rop) & 0x3))

#endif /* _GRAPHICS_ROP_H_INCLUDED */

__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devg/public/graphics/rop.h $ $Rev: 657836 $" )
