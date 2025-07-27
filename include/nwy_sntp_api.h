/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_sntp_api.h
 * Author       : shuiying
 * Created      : 2023-11-27
 * Description  : sntp API
 ******************************************************************************/
#ifndef __NWY_SNTP_API_H__
#define __NWY_SNTP_API_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_osi_api.h"

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

#define NWY_SNTP_URL_MAX_LEN 128
#define NWY_SNTP_TZ_MAX_LEN 10


/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */


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

typedef enum
{
    NWY_SNTP_SUCCESS = 0,
    NWY_SNTP_FAIL,
    NWY_SNTP_DNS_ERROR,
    NWY_SNTP_NET_ERROR,
    NWY_SNTP_TIMEOUT
}NWY_SNTP_EVENT_E;

typedef struct
{
  int cid;
  char url[NWY_SNTP_URL_MAX_LEN];
  int timeout;
  int retry_times;
  char tz[NWY_SNTP_TZ_MAX_LEN];
  unsigned char dst;
}nwy_sntp_param_t;

typedef struct{
    NWY_SNTP_EVENT_E event;                     /* event id */
    nwy_time_t update_time;
}nwy_sntp_result_type;

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

typedef void (*nwy_sntp_resultcb)(nwy_sntp_result_type *event);

/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */

/*
 *****************************************************************************
 * Prototype     : nwy_sntp_get_time
 * Description   : sntp get time
 * Input         : *sntp_param:sntp param
                    int cid://Data channels (1-7)
                    char url[NWY_SNTP_URL_MAX_LEN];NTP server
                    int timeout:Timeout, measured in seconds
                    int retry_times:Number of attempts to request
                    char tz:Time zone, formatted as "E/W digits", defaults to East 8 zone
                    E: Indicates the Eastern Time Zone and supports the Eastern 0-13 zones
                    W: Indicates the Western Time Zone and supports the Western 0-12 zones
                    0: represents the zero hour zone. e.g:"E8"
                    unsigned char dst:Summer time, not supported
                   cb_func: ind func
 * Output        : None
 * Return Value  : 0-success,other-fail
 * Author        : shuiying
 *****************************************************************************
*/
nwy_error_e nwy_sntp_get_time(nwy_sntp_param_t *sntp_param, nwy_sntp_resultcb cb_func);

#endif
