/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010-2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : echo_srv.h
* Version       : 2.09
* Device(s)     : RX family
* Tool-Chain    : C/C++ Compiler Package for RX Family
* OS            :
* H/W Platform  : RX family board
* Description   : T4 sample program
* Operation     : 1. Compile and download the sample code. Click 'Reset Go'
*                    to start the software.
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.05.2019 2.09    Release for V.2.09
******************************************************************************/

#ifndef ECHO_SRV_H
#define ECHO_SRV_H

/******************************************************************************
Macro definitions
******************************************************************************/
/* Please define the same price as  _t4_channel_num variable in config_tcpudp.c. */
/* 0 = BLOCKING, 1 = NON_BLOCKING */
#define APPLICATION_T4_BLOCKING_TYPE (0)

#define DEBUG_PRINT

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern const UB _t4_channel_num;
extern const UB _t4_dhcp_enable;
extern T_TCP_CREP tcp_crep[];
extern T_TCP_CCEP tcp_ccep[];
extern T_UDP_CCEP udp_ccep[];
extern const H __tcprepn;
extern const H __tcpcepn;
extern const H __udpcepn;

void echo_srv(void);
ER tcp_nonblocking_callback(ID cepid, FN fncd , VP p_parblk);
ER system_callback(UB channel, UW eventid, VP param);
void receive_cancel_timer(void);

#endif /* #ifndef ECHO_SRV_H */
