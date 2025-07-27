/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_common.h
 * Author       : hujun
 * Created      : 2023-5-10
 * Description  : sdk公共定义
 *
 *****************************************************************************
 */

#ifndef __NWY_COMMON_H__
#define __NWY_COMMON_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nwy_log_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NWY_BPV2_SDK_VERSION "SDK-V1.1.1"

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_APP_EVENT_ID_BASE 2100
#define NWY_APP_EVENT_ID_END  2200

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
    NWY_SUCCESS                      = 0,
    NWY_FAIL                         = -1,
    NWY_GEN_E_UNKNOWN                = -1,
    NWY_GEN_E_INVALID_PARA           = -2,
    NWY_GEN_E_SET_FAILED             = -3,
    NWY_GEN_E_GET_FAILED             = -4,
    NWY_GEN_E_SIM_NOT_READY          = -5,
    NWY_GEN_E_SIM_NOT_REG            = -6,
    NWY_GEN_E_ATT_NOT_ACT            = -7,
    NWY_GEN_E_GPRS_NOT_ACT           = -8,
    NWY_GEN_E_AREADY_CONNECT         = -9,
    NWY_GEN_E_PLAT_NOT_SUPPORT       = -10,
    NWY_GEN_E_AREADY_DISCONNECT      = -11,
    NWY_GEN_E_DSNET_NOT_ACTIVE       = -12,
    NWY_GEN_E_ARG_NEVER_USED         = -13,
    NWY_GEN_E_MALLOC_FAIL            = -14,
    NWY_GEN_E_RESOURE_FULL           = -15,
    NWY_GEN_E_TIMEOUT                = -16,
    NWY_GEN_E_STATCK_SIZE_SMALL      = -17,
    NWY_GEN_E_NULL_PTR               = -18,

    NWY_AT_GEN_ERR                   = -100,
    NWY_AT_MSG_QUEUE_NULL            = -101,
    NWY_AT_GET_RESP_TIMEOUT          = -102,
    NWY_AT_RESP_TIMEOUT_PARA_ERR     = -103,
    NWY_AT_MALLOC_ERR                = -104,
    NWY_AT_MSG_PUT_ERR               = -105,
    NWY_AT_FORWARD_REG_FULL          = -106,
    NWY_AT_UNISOL_REG_FULL           = -107,
    NWY_AT_WAIT_RESP_FAIL            = -108,
	NWY_AT_BUSY		   			     = -109,

    NWY_WIFI_GEN_ERR                 = -200,
    NWY_WIFI_OPEN_FAILED             = -201,
    NWY_WIFI_SCAN_FAILED             = -202,
    NWY_WIFI_SCAN_TIMEOUT            = -203,
    NWY_WIFI_GETINFO_FAILED          = -204,
    NWY_WIFI_FAILED                  = -205,

    NWY_FTP_GEN_ERR                  = -300,
    NWY_FTP_LOGGED                   = -301,
    NWY_FTP_NOT_LOGGED               = -302,
    NWY_FTP_DATA_ALREADY_OPEND       = -303,
    NWY_FTP_DSNET_NOT_ACTIVE         = -304,

    NWY_FS_GEN_ERR                   = -400,
    NWY_FS_PATH_ERR                  = -401,
    NWY_FS_ACTION_FAILED             = -402,
    NWY_FS_NOT_EXIST                 = -403,
    NWY_FS_OPEN_FAIL                 = -404,
    NWY_FS_SPACE_NOT_ENOUGH          = -405,

    NWY_SOCKET_GEN_FAILED            = -500,
    NWY_SOCKET_ACTION_FAILED         = -501,

    NWY_FOTA_WRITE_FAILED            = -600,
    NWY_FOTA_CHECK_FAILED            = -601,
    NWY_FOTA_UPDATE_RESULT_FALSE     = -602,
    NWY_FOTA_WRITE_FLAG_FAIL         = -603,

    NWY_SNTP_UPDATETIME_DNS_ERR      = -700,
    NWY_SNTP_UPDATETIME_QUERY_FAIL   = -701,
    NWY_SNTP_UPDATETIME_SOCKET_ERR   = -702,
    NWY_SNTP_BUSY   = -703,

    NWY_LBS_BUSY   = -712,

}nwy_error_e;

typedef enum {
    NWY_DISABLE_STATUS = 0,     //disable status
    NWY_ENABLE_STATUS  = 1,     //enable status
}nwy_status_e;


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
#ifndef _UINT32_DEFINED
/** Unsigned 32-bit value.*/
typedef  unsigned int  uint32;
#define _UINT32_DEFINED
#endif

#ifndef _UINT16_DEFINED
/** Unsigned 16-bit value.*/
typedef  unsigned short     uint16;
#define _UINT16_DEFINED
#endif

#ifndef _UINT8_DEFINED
/** Unsigned 8-bit value. */
typedef  unsigned char      uint8;
#define _UINT8_DEFINED
#endif

#ifndef _INT32_DEFINED
/** Signed 32-bit value.*/
typedef  signed long int    int32;
#define _INT32_DEFINED
#endif

#ifndef _S32_DEFINED
/** Signed short 32-bit value.*/
typedef  signed int         s32;
#define _S32_DEFINED
#endif

#ifndef _INT16_DEFINED
/** Signed 16-bit value.*/
typedef  signed short       int16;
#define _INT16_DEFINED
#endif

#ifndef _INT8_DEFINED
/** Signed 8-bit value.*/
typedef  signed char        int8;
#define _INT8_DEFINED
#endif

#ifndef _INT64_DEFINED
/** Signed 64-bit value.*/
typedef long long     int64;
#define _INT64_DEFINED
#endif
#ifndef _UINT64_DEFINED
/** Unsigned 64-bit value.*/
typedef  unsigned long long  uint64;
#define _UINT64_DEFINED
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if defined(SELF_TESTING)
#include "cmockery_mem.h"
#endif

#ifdef __cplusplus
   }
#endif

#endif
