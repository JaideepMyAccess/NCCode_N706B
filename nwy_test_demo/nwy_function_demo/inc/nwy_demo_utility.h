
/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_network_monitor.h
 * Author       : hujun
 * Created      : 2017-3-29
 * Description  : osi API º¯ÊýÉùÃ÷
 *
 *****************************************************************************
 */

#ifndef _NWY_DEMO_UTILITY_H_
#define _NWY_DEMO_UTILITY_H_

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"

#include "nwy_common.h"
#include "nwy_osi_api.h"
#include "nwy_usb_serial.h"
#include "nwy_file_api.h"
#include "nwy_socket_api.h"
#include "nwy_sim_api.h"
#include "nwy_vir_at_api.h"
#include "nwy_data_api.h"
#include "nwy_network_api.h"

#include "nwy_demo_vir_at.h"
#include "nwy_demo_tcp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
    NWY_DEMO_DATA_CALL_STA_MSG = NWY_APP_EVENT_ID_BASE + 1,
    NWY_DEMO_CFUN_RESET_CKECK_MSG,
    NWY_DEMO_RESTART_DATA_CALL_MSG,
    NWY_MAIN_DATA_CALL_CHANGE_MSG,
}nwy_demo_msg_e;


/*
 *****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************
 */




/*
 *****************************************************************************
 * 5 STRUCT Type Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 6 UNION Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */
void nwy_demo_uart_echo(char *fmt, ...);


#ifdef __cplusplus
}
#endif


#endif






