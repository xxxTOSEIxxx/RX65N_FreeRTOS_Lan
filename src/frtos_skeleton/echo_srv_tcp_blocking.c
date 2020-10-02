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
* File Name     : echo_srv_tcp_blocking.c
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
*               : 21.06.2010 1.00    First Release
*               : 30.08.2011 1.01    Clean up source code
*               : 01.04.2014 2.00    Release for V.2.00
*               : 10.12.2018 2.08    Release for V.2.08
*               : 20.05.2019 2.09    Release for V.2.09
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "r_t4_itcpip.h"
#include "r_ether_rx_if.h"
#include "r_sys_time_rx_if.h"
#include "echo_srv.h"
#include "r_t4_rx_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#if(T4_CFG_SYSTEM_CHANNEL_NUMBER == 2)

#if(APPLICATION_T4_BLOCKING_TYPE == 0)
/* Blocking echo server */
#define CEPID_BEGIN           (1)
#define CEPID_END             (2)
#define CEP_NUM               (2)

/* cepid=1 : repid=1 : callback=Not set : lan_port_number=0                    */
/* cepid=2 : repid=2 : callback=Not set : lan_port_number=1                    */
/* cepid=3 : It is not used in this sample program.                            */
/* cepid=4 : It is not used in this sample program.                            */
/* cepid=5 : It is not used in this sample program.                            */
/* cepid=6 : It is not used in this sample program.                            */

#elif(APPLICATION_T4_BLOCKING_TYPE == 1)
/* Non-Blocking echo server */
#define CEPID_BEGIN           (3)
#define CEPID_END             (6)
#define CEP_NUM               (4)

/* cepid=1 : It is not used in this sample program.                            */
/* cepid=2 : It is not used in this sample program.                            */
/* cepid=3 : repid=1 : callback=tcp_nonblocking_callback() : lan_port_number=0 */
/* cepid=4 : repid=2 : callback=tcp_nonblocking_callback() : lan_port_number=0 */
/* cepid=5 : repid=3 : callback=tcp_nonblocking_callback() : lan_port_number=1 */
/* cepid=6 : repid=4 : callback=tcp_nonblocking_callback() : lan_port_number=1 */

#endif /* (APPLICATION_T4_BLOCKING_TYPE == 1) */
#endif /* #if(T4_CFG_SYSTEM_CHANNEL_NUMBER == 2) */

#if(T4_CFG_SYSTEM_CHANNEL_NUMBER == 1)
#if(APPLICATION_T4_BLOCKING_TYPE == 0)
/* Blocking echo server */
#define CEPID_BEGIN           (1)
#define CEPID_END             (1)
#define CEP_NUM               (1)

/* cepid=1 : repid=1 : callback=Not set : lan_port_number=0                    */
/* cepid=2 : It is not used in this sample program.                            */
/* cepid=3 : It is not used in this sample program.                            */
/* cepid=4 : It is not used in this sample program.                            */
/* cepid=5 : It is not used in this sample program.                            */
/* cepid=6 : It is not used in this sample program.                            */

#elif(APPLICATION_T4_BLOCKING_TYPE == 1)
/* Non-Blocking echo server */
#define CEPID_BEGIN           (2)
#define CEPID_END             (3)
#define CEP_NUM               (2)

/* cepid=1 : It is not used in this sample program.                            */
/* cepid=2 : repid=1 : callback=tcp_nonblocking_callback() : lan_port_number=0 */
/* cepid=3 : repid=2 : callback=tcp_nonblocking_callback() : lan_port_number=0 */
/* cepid=4 : It is not used in this sample program.                            */
/* cepid=5 : It is not used in this sample program.                            */
/* cepid=6 : It is not used in this sample program.                            */

#endif /* (APPLICATION_T4_BLOCKING_TYPE == 1) */
#endif /* #if(T4_CFG_SYSTEM_CHANNEL_NUMBER == 1) */

#define T4_CLOSED        (0)
#define T4_ACCEPT        (1)
#define T4_ESTABLISHED   (2)
#define T4_CLOSE_WAIT    (3)
#define T4_LAST_ACK      (4)
#define T4_FIN_WAIT1     (5)
#define T4_TIME_WAIT     (6)
#define T4_CANCELING     (7)
#define T4_SENDING       (8)
#define T4_RESET_WAIT    (255)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct _CEP
{
    uint32_t    status;
    T_IPV4EP    dst_addr;
    T_IPV4EP    src_addr;
    int32_t     current_rcv_data_len;
    int32_t     total_rcv_len;
    UB          rcv_buf[1460];
    UB          snd_buf[1460];
    int32_t     _1sec_counter;
    int32_t     _1sec_timer;
    int32_t     pre_1sec_timer;
} st_cep_t;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static st_cep_t cep[CEP_NUM];
static UB guc_event[T4_CFG_SYSTEM_CHANNEL_NUMBER];
static DHCP* gpt_dhcp[T4_CFG_SYSTEM_CHANNEL_NUMBER];

/******************************************************************************
* Function Name     : echo_srv
* Description       : echo server main function.
* Argument          : none
* Return Value      : none
******************************************************************************/
void echo_srv(void)
{
    ID cepid;
    ID repid;
    UB interface;
    ER ercd;
    uint32_t i;
    uint32_t k;
    uint32_t n;

    /* initialize cep status */
    /* WAIT_LOOP */
    for (i = 0; i < CEP_NUM; i++)
    {
        cep[i].status = T4_CLOSED;
    }

    /* refer to tcp_crep[0] */
    n = 0;

    /* WAIT_LOOP */
    while (n < CEP_NUM)
    {
        /* dhcp use*/
        if(1 == _t4_dhcp_enable)
        {
            /* check all connections */
            /* WAIT_LOOP */
            for (i = 0; i < _t4_channel_num; i++)
            {
                interface = i;
                /* t4 reset request */
                if (DHCP_EV_PLEASE_RESET == guc_event[interface])
                {
                    /* reset callback event */
                    guc_event[interface] = 0;
#if defined(DEBUG_PRINT)
                    printf("main:call tcpudp_reset(channel:%d)\n", interface);
#endif /*#if defined(DEBUG_PRINT)*/
                    /* reset DHCP use interface */
                    tcpudp_reset(interface);

                    /* reset application data same as interface. */
                    /* WAIT_LOOP */
                    for (k = 0; k < CEP_NUM; k++)
                    {
                        if (interface == tcp_ccep[k + CEPID_BEGIN - 1].lan_port_number)
                        {
                            memset(&cep[k], 0, sizeof(st_cep_t));
                            cep[k].status = T4_CLOSED;
                        }
                    }
                }
            }
        }

        cepid = n + CEPID_BEGIN;
        repid = n + 1;
        if((1 != _t4_dhcp_enable) || (DHCP_EV_LEASE_IP == guc_event[n]) || (DHCP_EV_APIPA == guc_event[n]))
        {
            ercd = tcp_acp_cep(cepid, repid, &cep[n].dst_addr, TMO_FEVR);
            if(E_OK == ercd)
            {
                /* WAIT_LOOP */
                while(1)
                {
                    ercd = tcp_rcv_dat(cepid, cep[n].rcv_buf, sizeof(cep[n].rcv_buf), TMO_FEVR);
                    if(ercd <= 0)
                    {
                        break;
                    }
                    ercd = tcp_snd_dat(cepid, cep[n].rcv_buf, ercd, TMO_FEVR);
                    if(ercd < 0)
                    {
                        break;
                    }
                }
                tcp_sht_cep(cepid);
                ercd = tcp_cls_cep(cepid, TMO_FEVR);
                if(ercd == E_OK)
                {
                    n++;
                }
            }
        }
    }
}

/******************************************************************************
* Function Name     : system_callback
* Description       : Ethernet callback.
* Argument          : UB        channel ;    RJ45 interface
*                   : UW        eventid ;    Event code
*                   : VP        param   ;    Parameter block
* Return Value      : ER                ;    always 0 (not in use)
******************************************************************************/
ER system_callback(UB channel, UW eventid, VP param)
{
#if defined(DEBUG_PRINT)
    uint8_t*    ev_tbl[] =
    {
        "ETHER_EV_LINK_OFF",
        "ETHER_EV_LINK_ON",
        "ETHER_EV_COLLISION_IP",
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
        "DHCP_EV_LEASE_IP",
        "DHCP_EV_LEASE_OVER",
        "DHCP_EV_INIT",
        "DHCP_EV_INIT_REBOOT",
        "DHCP_EV_APIPA",
        "DHCP_EV_NAK",
        "DHCP_EV_FATAL_ERROR",
        "DHCP_EV_PLEASE_RESET"
    };
    printf("^^>>>user_cb<<< ch:%d,eventID = %s\n", channel, ev_tbl[eventid]);
#endif /*#if defined(DEBUG_PRINT)*/
    guc_event[channel] = eventid;
    switch(eventid)
    {
        case ETHER_EV_LINK_OFF:
        {
            /* Do Nothing. */
        }
        break;

        case ETHER_EV_LINK_ON:
        {
            /* Do Nothing. */
        }
        break;

        case ETHER_EV_COLLISION_IP:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_LEASE_IP:
        {
            /* cast from VP to DHCP* */
            gpt_dhcp[channel] = (DHCP*)param;
#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
#endif /*#if defined(DEBUG_PRINT)*/
        }
        break;

        case DHCP_EV_LEASE_OVER:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_INIT:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_INIT_REBOOT:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_APIPA:
        {
            /* cast from VP to DHCP* */
            gpt_dhcp[channel] = (DHCP*)param;
#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
#endif /*#if defined(DEBUG_PRINT)*/
        }
        break;

        case DHCP_EV_NAK:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_FATAL_ERROR:
        {
            /* Do Nothing. */
        }
        break;

        case DHCP_EV_PLEASE_RESET:
        {
            /* Do Nothing. */
        }
        break;

        default:
        {
            /* Do Nothing. */
        }
        break;
    }
    return 0;
}
