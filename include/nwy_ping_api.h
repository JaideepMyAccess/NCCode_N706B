/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_ping_api.h
 * Author       : shuiying
 * Created      : 2024-12
 * Description  : PING API
 ******************************************************************************/
#ifndef __NWY_PING_API_H__
#define __NWY_PING_API_H__
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

#define NWY_MAX_PING_HOST_LEN 		128



/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
	NWY_PING_ST_RUN_START,
	NWY_PING_ST_DNS_ERR,
	NWY_PING_ST_RUNNING,
	NWY_PING_ST_REPLY_RECV,
	NWY_PING_ST_ALL_FINISH,
	NWY_PING_ST_REPLY_TIMEOUT,
	NWY_PING_ST_SOCKET_ERR,
	NWY_PING_ST_SEND_ERR,
	NWY_PING_ST_OTHER_ERR
}nwy_ping_st_e_type;


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

typedef struct
{
	char ping_host[NWY_MAX_PING_HOST_LEN + 1];
	int cnt;
	int delay;
	int tout;
	int data_len;
	int ip_type; //4-IPv4 6-IPv6
	int cid;
}nwy_ping_api_param_t;

typedef struct
{
	uint32_t  min_rtt;	/* Minimum RTT so far, in millisecs 		  */
	uint32_t  max_rtt;	/* Maximum RTT so far, in millisecs 		  */
	uint32_t  avg_rtt;	/* Average RTT so far, in millisecs 		  */
	uint32_t  num_pkts_sent;  /* Number of pings sent so far		  */
	uint32_t  num_pkts_recvd; /* Number of responses recieved so far  */
	uint32_t  num_pkts_lost;  /* Number of responses not received	  */
}nwy_ping_fin_stats_type;

typedef struct
{
	uint32_t  num_data_bytes;		   /* Data byte size of the ping packet    */
	uint32_t  ping_interval_time;	   /* Interval between each ping, ms	   */
	uint32_t  ping_response_time_out;  /* Wait time for ping response, ms	   */
	uint32_t  cookie;				   /* Internal field					   */
	uint32_t  num_pings;			   /* Number of times to ping			   */
	uint32_t  ttl;					   /* Time To Live for the ping packet	   */
} nwy_ping_config_type;

typedef struct
{
	nwy_ping_st_e_type status;
	uint32_t ping_send_cnt;
	uint32_t elapsed;
	char reply_addr[65];
	nwy_ping_fin_stats_type fin_stats_info;   //ping statistics info, only for PING_ST_ALL_FINISH
	nwy_ping_config_type ping_config;
}nwy_ping_cb_msg;

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

typedef void (*nwy_ping_app_cb)(void *arg);


/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */


/*
*****************************************************************************
* Prototype     : nwy_ping_start
* Description   : ping url
* Input         : ping_param : url, cnt, delay,ip type etc
				  ping_cb: callback func
* Output        :
* Return Value  : NWY_SUCCESS or others
* Author        : hujun
*****************************************************************************
*/

nwy_error_e nwy_ping_start(nwy_ping_api_param_t *ping_param, nwy_ping_app_cb ping_cb);


#endif
