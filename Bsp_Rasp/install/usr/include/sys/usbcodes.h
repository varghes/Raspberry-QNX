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







/*
 *  usbcodes.h
 *
 *  This file includes USB usages (Usb page 0x07 - keyboard/keypad)
 */

#ifndef _USBCODES_H
#define _USBCODES_H

#ifndef	__PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

typedef _Uint16t USBKCode;      /* USB key code                          */
#define         KS_None         0x00

#define         KS_a            0x04
#define 	KS_b            0x05
#define   	KS_c            0x06
#define   	KS_d            0x07
#define   	KS_e            0x08
#define   	KS_f            0x09
#define   	KS_g            0x0a
#define   	KS_h            0x0b
#define   	KS_i            0x0c
#define   	KS_j            0x0d
#define   	KS_k            0x0e
#define   	KS_l            0x0f
#define   	KS_m            0x10
#define   	KS_n            0x11
#define   	KS_o            0x12
#define   	KS_p            0x13
#define   	KS_q            0x14
#define   	KS_r            0x15
#define   	KS_s            0x16
#define   	KS_t            0x17
#define   	KS_u            0x18
#define   	KS_v            0x19
#define   	KS_w            0x1a
#define   	KS_x            0x1b
#define   	KS_y            0x1c
#define   	KS_z            0x1d
#define    	KS_1            0x1e
#define         KS_exclam       (KS_1)      /* upper case */
#define    	KS_2            0x1f
#define    	KS_at           (KS_2)      /* upper case */
#define    	KS_3            0x20
#define    	KS_numbersign   (KS_3)      /* upper case */
#define    	KS_4            0x21
#define    	KS_dollar       (KS_4)      /* upper case */
#define    	KS_5            0x22 
#define    	KS_percent      (KS_5)      /* upper case */
#define    	KS_6            0x23
#define    	KS_asciicircum  (KS_6)      /* upper case */
#define    	KS_7            0x24	
#define    	KS_ampersand    (KS_6)      /* upper case */
#define    	KS_8	        0x25
#define    	KS_asterisk     (KS_8)      /* upper case */
#define    	KS_9	        0x26
#define    	KS_parenleft    (KS_9)      /* upper case */
#define    	KS_0            0x27
#define    	KS_parenright   (KS_0)      /* upper case */ 
#define    	KS_Enter        0x28
#define    	KS_Cmd_Debugger (KS_6)      /* upper case */
#define    	KS_Escape       0x29 
#define    	KS_BackSpace    0x2a
#define    	KS_Tab          0x2b
#define    	KS_space        0x2c
#define    	KS_minus        0x2d
#define    	KS_underscore   (KS_minus)   /* upper case */
#define    	KS_equal	0x2e
#define    	KS_plus         (KS_equal)   /* upper case */
#define    	KS_bracketleft  0x2f
#define    	KS_braceleft    (KS_bracketleft)  /* upper case */
#define    	KS_bracketright 0x30
#define    	KS_braceright   (KS_bracketright) /* upper case */
#define    	KS_backslash	0x31
#define    	KS_bar          (KS_backslash)    /* upper case */
#define    	KS_NonUSNumsign 0x32
#define    	KS_asciitilde   (KS_NonUSNumsign)   /* upper case */
#define    	KS_semicolon    0x33
#define    	KS_colon        (KS_semicolumn)   /* upper case */
#define    	KS_apostrophe   0x34
#define    	KS_quotedbl     (KS_apostrophe)   /* upper case */
#define    	KS_grave        0x35
#define    	KS_tilde        (KS_grave)   /* upper case */
#define    	KS_comma        0x36
#define    	KS_less         (KS_comma)   /* upper case */
#define    	KS_period       0x37
#define    	KS_greater      (KS_period)  /* upper case */
#define    	KS_slash        0x38
#define    	KS_question     (KS_slash)   /* upper case */
#define    	KS_Caps_Lock    0x39
#define    	KS_Cmd_Screen0  (KS_Caps_Lock)    /* upper case */
#define    	KS_f1           0x3a
#define    	KS_Cmd_Screen1  (KS_f1)      /* upper case */
#define    	KS_f2           0x3b
#define    	KS_Cmd_Screen2	(KS_f2)      /* upper case */
#define    	KS_f3           0x3c
#define    	KS_Cmd_Screen3  (KS_f3)      /* upper case */
#define    	KS_f4           0x3d
#define    	KS_Cmd_Screen4  (KS_f4)      /* upper case */
#define    	KS_f5           0x3e
#define    	KS_Cmd_Screen5	(KS_f5)      /* upper case */
#define    	KS_f6           0x3f
#define    	KS_Cmd_Screen6  (KS_f6)      /* upper case */
#define    	KS_f7           0x40
#define    	KS_Cmd_Screen7  (KS_f7)      /* upper case */
#define    	KS_f8           0x41
#define    	KS_Cmd_Screen8  (KS_f8)      /* upper case */
#define    	KS_f9           0x42
#define    	KS_Cmd_Screen9  (KS_f9)      /* upper case */
#define    	KS_f10          0x43
#define    	KS_f11          0x44
#define    	KS_f12          0x45
#define    	KS_Print_Screen 0x46
#define    	KS_Hold_Screen  0x47
#define    	KS_Pause        0x48
#define    	KS_Insert       0x49
#define    	KS_Home         0x4a
#define    	KS_PgUp         0x4b
#define    	KS_Delete       0x4c
#define    	KS_End          0x4d
#define    	KS_PgDn         0x4e
#define    	KS_Right        0x4f
#define    	KS_Left         0x50
#define    	KS_Down         0x51
#define    	KS_Up           0x52
#define    	KS_Num_Lock     0x53
#define    	KS_KP_Divide    0x54
#define    	KS_KP_Multiply  0x55
#define    	KS_KP_Subtract  0x56
#define    	KS_KP_Add       0x57
#define    	KS_KP_Enter     0x58
#define    	KS_KP_End       0x59
#define    	KS_KP_1         (KS_KP_End)   /* Num Lock ON */
#define    	KS_KP_Down      0x5a
#define    	KS_KP_2         (KS_KP_Down)  /* Num Lock ON */
#define    	KS_KP_Next	0x5b
#define    	KS_KP_3         (KS_KP_Next)  /* Num Lock ON */
#define    	KS_KP_Left	0x5c
#define    	KS_KP_4         (KS_KP_Left)  /* Num Lock ON */
#define    	KS_KP_Begin	0x5d
#define    	KS_KP_5         (KS_KP_Begin) /* Num Lock ON */
#define    	KS_KP_Right     0x5e
#define    	KS_KP_6         (KS_KP_right) /* Num Lock ON */
#define    	KS_KP_Home      0x5f
#define    	KS_KP_7         (KS_KP_Home)  /* Num Lock ON */
#define    	KS_KP_Up        0x60
#define    	KS_KP_8         (KS_KP_Up)    /* Num Lock ON */
#define    	KS_KP_Prior     0x61
#define    	KS_KP_9         (KS_KP_Prior) /* Num Lock ON */
#define    	KS_KP_Insert    0x62
#define    	KS_KP_0         (KS_KP_Insert)/* Num Lock ON */
#define    	KS_KP_Delete    0x63
#define    	KS_KP_Decimal   (KS_KP_Delete)/* Num Lock ON */
#define    	KS_NonUSSlash   0x64
#define    	KS_NonUSBar     (KS_NonUSSlash)/* upper case */
#define    	KS_Application  0x65
#define    	KS_Power        0x66
#define    	KS_Keypad_Eq    0x67
#define    	KS_f13          0x68
#define    	KS_f14          0x69
#define    	KS_f15          0x6a
#define    	KS_f16          0x6b
#define    	KS_f17          0x6c
#define    	KS_f18          0x6d
#define    	KS_f19          0x6e
#define    	KS_f20          0x6f
#define    	KS_f21          0x70
#define    	KS_f22          0x71
#define    	KS_f23          0x72
#define    	KS_f24          0x73
#define    	KS_Execute      0x74
#define    	KS_Help         0x75
#define    	KS_Menu         0x76
#define    	KS_Select       0x77
#define    	KS_Stop         0x78
#define    	KS_Again        0x79
#define    	KS_Undo         0x7a
#define    	KS_Cut          0x7b
#define    	KS_Copy         0x7c
#define    	KS_Paste        0x7d
#define    	KS_Find         0x7e
#define    	KS_Mute         0x7f
#define    	KS_Volume_Up    0x80
#define    	KS_Volume_Down  0x81
#define    	KS_LockCpsLock  0x82
#define    	KS_LockNumLock  0x83
#define    	KS_LockScrLock  0x84
#define    	KS_KP_Comma     0x85
#define    	KS_KP_Equal     0x86
#define    	KS_Internat1    0x87
#define    	KS_Internat2    0x88
#define    	KS_Internat3    0x89
#define    	KS_Internat4    0x8a
#define    	KS_Internat5    0x8b
#define    	KS_Internat6    0x8c
#define    	KS_Internat7    0x8d
#define    	KS_Internat8    0x8e
#define    	KS_Internat9    0x8f
#define    	KS_Lang1        0x90
#define    	KS_Lang2        0x91
#define    	KS_Lang3        0x92
#define    	KS_Lang4        0x93
#define    	KS_Lang5        0x94
#define    	KS_Lang6        0x95
#define    	KS_Lang7        0x96
#define    	KS_Lang8        0x97
#define    	KS_Lang9        0x98
#define    	KS_AltErase     0x99
#define    	KS_SysReq       0x9a
#define    	KS_Cancel       0x9b
#define    	KS_Clear        0x9c
#define    	KS_Prior        0x9d
#define    	KS_Return       0x9e    /* Don't confuse with Enter ! */
#define    	KS_Separator    0x9f
#define    	KS_Out          0xa0
#define    	KS_Oper         0xa1
#define    	KS_ClearAgain   0xa2
#define    	KS_CrSel_Props  0xa3
#define    	KS_ExSel        0xa4
/* 0xa5 - 0xdf : reserved     */
/* ... many unmapped keys ... */
#define    	KS_Control_L    0xe0
#define         KS_Cmd1         (KS_Control_L)
#define    	KS_Shift_L      0xe1
#define    	KS_Alt_L        0xe2
#define    	KS_Cmd2         (KS_Alt_L)
#define         KS_LeftGUI      0xe3
#define    	KS_Meta_L       (KS_LeftGUI)
#define    	KS_Control_R    0xe4
#define    	KS_Shift_R      0xe5
#define    	KS_Alt_R        0xe6
#define    	KS_Multi_key    (KS_Alt_R)
#define         KS_RightGUI     0xe7
#define    	KS_Meta_R       (KS_RightGUI)
/* 0xe8-0xffff : reserved     */


#endif   /*  _USBCODES_H  */

#ifdef __QNXNTO__
#include <sys/srcversion.h>
__SRCVERSION( "$URL: http://svn/product/tags/public/bsp/nto650/ATMEL-AT91SAM9G45-EKES-650/latest/src/hardware/devi/public/sys/usbcodes.h $ $Rev: 657836 $" )
#endif
