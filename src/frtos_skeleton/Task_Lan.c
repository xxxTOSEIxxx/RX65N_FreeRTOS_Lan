/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "task_function.h"
//#include "platform.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdbool.h>
#include "r_t4_itcpip.h"
#include "r_sys_time_rx_if.h"
#include "Pin.h"
#include "echo_srv.h"



/******************************************************************************
Macro definitions
******************************************************************************/
/* Maximum size used in echo server sample program */
#if (APPLICATION_T4_CHANNEL_NUM  == (1))
#define T4_WORK_SIZE (14800)
#else
#define T4_WORK_SIZE (29600)
#endif


/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/



/******************************************************************************
Private global variables and functions
******************************************************************************/
static UW tcpudp_work[(T4_WORK_SIZE / 4) + 1];


void Task_Lan(void * pvParameters)
{
    ER 				ercd;
    W    			size;
    sys_time_err_t 	systime_ercd;
    char    		ver[128];


    PORTA.PDR.BIT.B6 = 1;
    PORTA.PODR.BIT.B6 = 0;
    vTaskDelay(200);
    PORTA.PODR.BIT.B6 = 1;
    vTaskDelay(200);

    /* cast from uint8_t to char* */
    strcpy(ver, (char*)R_t4_version.library);
    systime_ercd = R_SYS_TIME_Open();
    if (systime_ercd != SYS_TIME_SUCCESS)
    {
        /* Specifies that the integer is of type bool. */
        /* WAIT_LOOP */
        while ((bool)1)
        {
            /* Do Nothing. */
        };
    }

    R_Pins_Create();

    /* start LAN controller */
    ercd = lan_open();
    if (ercd != E_OK)
    {
        /* Specifies that the integer is of type bool. */

        /* WAIT_LOOP */
        while ((bool)1)
        {
            /* Do Nothing. */
        };
    }



    /* initialize TCP/IP */
    size = tcpudp_get_ramsize();
    if (size > (sizeof(tcpudp_work)))
    {
        /* Specifies that the integer is of type bool. */
        /* WAIT_LOOP */
        while ((bool)1)
        {
            /* Do Nothing. */
        };
    }

    ercd = tcpudp_open(tcpudp_work);
    if (ercd != E_OK)
    {
        /* Specifies that the integer is of type bool. */
        /* WAIT_LOOP */
        while ((bool)1)
        {
            /* Do Nothing. */
        };
    }

    /* start echo server */
    echo_srv();

    /* end TCP/IP */
    tcpudp_close();
    lan_close();
    R_SYS_TIME_Close();

    /* Specifies that the integer is of type bool. */
    /* WAIT_LOOP */
    while ((bool)1)
    {
        /* Do Nothing. */
    };
}
