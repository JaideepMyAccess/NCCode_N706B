/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_data_api.h
 * Author       : liufeifan
 * Created      : 2023-5-18
 * Description  : data api function declaration
 *
 *****************************************************************************
 */

#ifndef __NWY_DATA_API_H__
#define __NWY_DATA_API_H__

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */

#include "nwy_common.h"
#include "nwy_sim_api.h"

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
#define NWY_APP_APN_MAX_LEN 63
#define NWY_APP_USER_MAX_LEN 64
#define NWY_APP_PWD_MAX_LEN 64
#define NWY_APP_DATA_CALL_MAX_NUM 7
#define NWY_APP_IP_STR_MAX_LEN 60
#define NW_APP_CALL_CID_MIN   1
#define NW_APP_CALL_CID_MAX   6

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
 typedef enum {
    NWY_DATA_PDP_TYPE_MIN =  -1,                  //IPv4 dial
    NWY_DATA_PDP_TYPE_IPV4 = 1,                  //IPv4 dial
    NWY_DATA_PDP_TYPE_IPV6 = 2,                      //IPv6 dial
    NWY_DATA_PDP_TYPE_IPV4V6 = 3,                    //IPv4/v6 dial
    NWY_DATA_PDP_TYPE_PPP = 6,                       //PPP dial
} nwy_data_pdp_type_e;

typedef enum {
    NWY_DATA_AUTH_PROTO_MIN = -1,
    NWY_DATA_AUTH_PROTO_NONE =  0,                    //none
    NWY_DATA_AUTH_PROTO_PAP,                     //PAP
    NWY_DATA_AUTH_PROTO_CHAP,                    //CHAP
    NWY_DATA_AUTH_PROTO_PAP_CHAP
} nwy_data_auth_proto_e;

typedef enum {
    NWY_DATA_CALL_INVALID_STATE = -1,
    NWY_DATA_CALL_DISCONNECTED_STATE = 0,                  //data call is disconnected
    NWY_DATA_CALL_CONNECTED_STATE = 1                     //data call is connected
} nwy_data_call_state_e;

typedef enum {
    NWY_DATA_TYPE_NORMAL = 0x0,
    NWY_DATA_TYPE_TETHER,  /*PC get LAN ip in usbnetshare state*/
    NWY_DATA_TYPE_IPPT,  /*PC get WAN ip in usbnetshare state*/
    NWY_DATA_TYPE_NORMAL_TETHER,
    NWY_DATA_TYPE_NORMAL_IPPT,
} nwy_data_type_e;

typedef enum {
    NWY_DATA_STATE_INVALID = 0x0,
    NWY_DATA_STATE_CONNECT,
    NWY_DATA_STATE_DISCONNECT,
    NWY_DATA_STATE_DISCONNECT_RETRY_MAX,   /*reach max redial num*/
} nwy_data_state_e;

typedef enum {
    NWY_DATA_CALL_DEACT = 0x0,
    NWY_DATA_CALL_ACT,
    NWY_DATA_CALL_MAX
} nwy_data_call_action_type_e;

typedef enum
{
  NWY_DATA_TRIGGER_NONE,
  NWY_DATA_TRIGGER_XIIC,
  NWY_DATA_TRIGGER_MYNETACT,
  NWY_DATA_TRIGGER_QIACT,
  NWY_DATA_TRIGGER_OPEN,
  NWY_DATA_TRIGGER_MYUSBNETACT,
  NWY_DATA_TRIGGER_NWNETACT,
  NWY_DATA_TRIGGER_NETSHAREACT,
  NWY_DATA_TRIGGER_NWNETSHAREACT,
  NWY_DATA_TRIGGER_NWMUXSHAREACT,
  NWY_DATA_TRIGGER_MAX
}nwy_data_trigger_type_e;

typedef enum
{
  NWY_DATA_SHARE_MODE_RNDIS = 0,
  NWY_DATA_SHARE_MODE_ECM,
  NWY_DATA_SHARE_MODE_NCM,
  NWY_DATA_SHARE_MODE_RNDIS_IP_PASSTHROUGH,
  NWY_DATA_SHARE_MODE_ECM_IP_PASSTHROUGH,
  NWY_DATA_SHARE_MODE_NCM_IP_PASSTHROUGH,
  NWY_DATA_SHARE_MODE_MAX
}nwy_data_share_mode_e;

typedef enum
{
  NWY_DATA_CALL_AUTO_TYPE_DISABLE = 0,
  NWY_DATA_CALL_AUTO_TYPE_ENABLE,
  NWY_DATA_CALL_AUTO_TYPE_MAX
}nwy_data_call_auto_type_e;

// Definition callback function type
typedef enum {
    NWY_DATA_CB_TYPE_LEGACY = 0,    // callback function type in legecy version
    NWY_DATA_CB_TYPE_EX = 1         // callback function type in new version
} nwy_data_cb_type_e;

/* Request types (extensible) */
typedef enum {
    REQ_AUTH_TYPE_RANGE   = 0x01,  // Authentication type min/max
    REQ_APN_MAX_LENGTH, // Maximum APN length
    REQ_USERNAME_MAX_LENGTH , 
    REQ_PASSWORD_MAX_LENGTH , 
    REQ_DEVICE_MODEL,  // Device model string
    /* Add future request types here (0x04, 0x05...) */
} nwy_data_cap_request_type_e;


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

/* Response structure */
typedef struct {
    nwy_error_e result_code;  // Operation status
    nwy_data_cap_request_type_e req_type;   // Echoes the requested type
    union {
        struct {                    // For RANGE
            int32 min;
            int32 max;
        } range;
        int64 max_or_min_value;       // For MAXMIN_VALUE
        struct {                    // For variable-length strings
            char *buffer;           // Caller-provided buffer
            //deal with data eg:"4800,9200,19200,38400,blabla"
            uint16 buffer_size;   // Buffer capacity
            uint16 data_length;   // Actual data length (excl. null terminator)
        } string_data;
    } data;
} nwy_data_capability_rsp_t;


typedef struct {
    nwy_data_pdp_type_e pdp_type;                //PPP,IPv4,
    char apn[NWY_APP_APN_MAX_LEN + 1];                 //Access Point Name
    nwy_data_auth_proto_e auth_proto;            //authentication protocol
    char user_name[NWY_APP_USER_MAX_LEN + 1];      //user name
    char pwd[NWY_APP_PWD_MAX_LEN + 1];            //password
} nwy_data_profile_info_t;

typedef struct {
    uint16 retry_max;  /*max retry num*/
    uint8 interval[16];  /*unit:1s,interval is non-zero num,others no limitation.if interval[n] equal to 0 , set interval[n] = interval[n-1] */
}nwy_data_auto_connect_cfg_t;

typedef struct {
    int cid;
    nwy_data_call_action_type_e action;
    nwy_data_trigger_type_e trigger_type;
    nwy_data_call_auto_type_e call_auto_type;
    nwy_data_profile_info_t set_profile;
    int if_internal_call;
}nwy_data_start_call_t;

typedef struct {
    uint32 s_addr;
}nwy_in4_addr_t;

typedef struct {
    uint8 u6_addr8[16];
    uint16 u6_addr16[8];
    uint32 u6_addr32[4];
}nwy_in6_addr_t;

typedef struct {
    nwy_in4_addr_t public_ip;       /**< IP address. */
    nwy_in4_addr_t primary_dns;     /**< Primary domain name service (DNS) IP address. */
    nwy_in4_addr_t secondary_dns;   /**< Secondary DNS IP address. */
}v4_info_t;

typedef struct {
    nwy_in6_addr_t public_ip_v6;       /**< IPv6 address. */
    nwy_in6_addr_t primary_dns_v6;     /**< Primary domain name service (DNS) IPv6 address. */
    nwy_in6_addr_t secondary_dns_v6;   /**< Secondary DNS IPv6 address. @newpagetable */
}v6_info_t;

typedef struct {
    char ipv4_str[NWY_APP_IP_STR_MAX_LEN];
    char ipv6_str[NWY_APP_IP_STR_MAX_LEN];
    nwy_data_call_state_e state;
    nwy_data_profile_info_t profile;
}nwy_data_callinfo_t;

typedef struct {
    char gw[20];
    char netmask[20];
    char start_ip[20];
    char end_ip[20];
    unsigned int lease_time;
}nwy_data_dhcp_t;

typedef struct {
    int profile_idx;
    nwy_data_call_state_e ind_state;
    nwy_data_pdp_type_e ip_type;
    nwy_data_trigger_type_e trigger_type;
} nwy_data_cb_info_t;

typedef void (*nwy_data_cb_func)(int profile_idx, nwy_data_call_state_e ind_state);

typedef void (*nwy_data_cb_func_ex)(nwy_data_cb_info_t *cb_info);

typedef struct {
    nwy_data_cb_type_e type;        // callback function type
    union {
        nwy_data_cb_func legacy;    // callback function type in legecy version
        nwy_data_cb_func_ex ex;     // callback function type in new version
    } func;
} nwy_data_cb_wrapper_t;

typedef struct {
    int cid;
    uint64 tx;
    uint64 rx;
}nwy_data_flow_t;
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
 
 /*
 *****************************************************************************
 * Prototype	 : nwy_data_profile_set
 * Description	 : set profile info
 * Input		 : sim_id: sim id
                   profile_idx: cid
                   profile_info: profile info
 * Output		 : NULL
 * Return Value  : nwy_error_e
 * Author		 : liufeifan 20230615
 *****************************************************************************
 */
nwy_error_e nwy_data_profile_set(nwy_sim_id_e sim_id, int profile_idx, nwy_data_profile_info_t *profile_info);

/*
 *****************************************************************************
 * Prototype	 : nwy_data_profile_get
 * Description	 : get profile info
 * Input		 : sim_id: sim id
                   profile_idx: cid
 * Output		 : profile_info: profile info
 * Return Value  : nwy_error_e
 * Author		 : liufeifan 20230615
 *****************************************************************************
 */
nwy_error_e nwy_data_profile_get(nwy_sim_id_e sim_id, int profile_idx, nwy_data_profile_info_t *profile_info);

/*
*****************************************************************************
* Prototype     : nwy_data_reg_cb
* Description   : register callback function
* Input         : sim_id: sim id
                  profile_idx: cid
                  cb_func: callback function
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_reg_cb(nwy_sim_id_e sim_id, int profile_idx, nwy_data_cb_func cb_func);

/*
*****************************************************************************
* Prototype     : nwy_data_unreg_cb
* Description   : delete callback function
* Input         : sim_id: sim id
                  profile_idx: cid
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_unreg_cb(nwy_sim_id_e sim_id, int profile_idx,nwy_data_cb_func cb_func);

/*
*****************************************************************************
* Prototype     : nwy_data_reg_cb_ex
* Description   : register callback function
* Input         : sim_id: sim id
                  profile_idx: cid
                  cb_func: callback function
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : chenxiaokang 20250304
*****************************************************************************
*/
nwy_error_e nwy_data_reg_cb_ex(nwy_sim_id_e sim_id, int profile_idx, nwy_data_cb_func_ex cb_func);

/*
*****************************************************************************
* Prototype     : nwy_data_unreg_cb_ex
* Description   : delete callback function
* Input         : sim_id: sim id
                  profile_idx: cid
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : chenxiaokang 20250304
*****************************************************************************
*/
nwy_error_e nwy_data_unreg_cb_ex(nwy_sim_id_e sim_id, int profile_idx, nwy_data_cb_func_ex cb_func);
/*
*****************************************************************************
* Prototype     : nwy_data_auto_connect_set
* Description   : Set parameters for automatic connection
* Input         : cfg: automatic connection params
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_auto_connect_set(int cid,nwy_data_auto_connect_cfg_t *cfg);

/*
*****************************************************************************
* Prototype     : nwy_data_call_start
* Description   : start dial
* Input         : sim_id: sim id
                  param: dial params
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_call_start(nwy_sim_id_e sim_id, nwy_data_start_call_t *param);

/*
*****************************************************************************
* Prototype     : nwy_data_call_stop
* Description   : stop dial
* Input         : sim_id: sim id
                  profile_idx: cid
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/

nwy_error_e  nwy_data_call_stop(nwy_sim_id_e sim_id, nwy_data_start_call_t *param);


/*
*****************************************************************************
* Prototype     : nwy_data_call_info_get
* Description   : get dial info
* Input         : profile_idx: cid
* Output        : info_ptr: dial info
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_call_info_get(int profile_idx, nwy_data_callinfo_t * info_ptr);

/*
*****************************************************************************
* Prototype     : nwy_data_dhcp_set
* Description   : set dhcp params
* Input         : dhcp: dhcp params
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_dhcp_set(nwy_data_dhcp_t *dhcp);

/*
*****************************************************************************
* Prototype     : nwy_data_dhcp_get
* Description   : get dhcp params
* Input         : NULL
* Output        : dhcp: dhcp params
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_dhcp_get(nwy_data_dhcp_t *dhcp);

/*
*****************************************************************************
* Prototype     : nwy_data_type_set
* Description   : set the dial type
* Input         : profile_idx: cid
                  type: dial type
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liufeifan 20230615
*****************************************************************************
*/
nwy_error_e nwy_data_type_set(int profile_idx, nwy_data_type_e type);

nwy_error_e  nwy_data_type_get(int profile_idx, nwy_data_type_e *type);

/*
*****************************************************************************
* Prototype     : nwy_data_get_internal_call_single_cid
* Description   : get single internal protocol call cid
* Input         : NULL
* Output        : cid
* Return Value  : nwy_error_e
* Author        : cuilisha
*****************************************************************************
*/
nwy_error_e nwy_data_get_internal_call_single_cid(int *cid);

/*
*****************************************************************************
* Prototype     : nwy_data_set_share_mode
* Description   : set network_share_mode
* Input         : share_mode nwy_data_share_mode_e
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : cuilisha
*****************************************************************************
*/
nwy_error_e nwy_data_set_share_mode(nwy_data_share_mode_e          share_mode);


/*
*****************************************************************************
* Prototype     : nwy_data_get_share_mode
* Description   : get network_share_mode
* Input         : NULL
* Output        : share_mode nwy_data_share_mode_e
* Return Value  : nwy_error_e
* Author        : cuilisha
*****************************************************************************
*/
nwy_error_e nwy_data_get_share_mode(nwy_data_share_mode_e      *    share_mode);



/*
*****************************************************************************
* Prototype     : nwy_data_get_share_act_cid
* Description   : get share_act_cid
* Input         : NULL
* Output        : cid
* Return Value  : nwy_error_e
* Author        : cuilisha
*****************************************************************************
*/
nwy_error_e nwy_data_get_share_act_cid(int *cid);



/*****************************************************************************
* Prototype     : nwy_data_get_share_act_auto_type
* Description   : get share_act_auto_type
* Input         : NULL
* Output        : auto_type nwy_data_call_auto_type_e
* Return Value  : nwy_error_e
* Author        : cuilisha
*****************************************************************************
*/
nwy_error_e nwy_data_get_share_act_auto_type(int cid, nwy_data_call_auto_type_e *auto_type);

/*****************************************************************************
* Prototype     : nwy_data_share_status_get
* Description   : get external data share status
* Input         : profile_id
* Output        : status
* Return Value  : nwy_error_e
* Author        : gehongru
*****************************************************************************
*/
nwy_error_e  nwy_data_share_status_get(nwy_sim_id_e sim_id,int profile_id, nwy_data_call_state_e *status);

/*****************************************************************************
* Prototype     : nwy_data_share_act
* Description   : active/deactive profile_id pdp context
* Input         : profile_id,action
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : liucheng
*****************************************************************************
*/
nwy_error_e nwy_data_share_act(nwy_sim_id_e sim_id, nwy_data_start_call_t *param);

/*****************************************************************************
* Prototype     : nwy_data_pdp_info_set
* Description   : set pdp information
* Input         : sim_id, profile_id ,ip_type, apn
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : gehongru
*****************************************************************************
*/
nwy_error_e  nwy_data_pdp_info_set(nwy_sim_id_e sim_id, int profile_id, nwy_data_pdp_type_e ip_type, char *apn);

/*****************************************************************************
* Prototype     : nwy_data_pdp_info_get
* Description   : get pdp information
* Input         : sim_id, profile_id
* Output        : ip_type, apn
* Return Value  : nwy_error_e
* Author        : gehongru
*****************************************************************************
*/
nwy_error_e  nwy_data_pdp_info_get(nwy_sim_id_e sim_id, int profile_id, nwy_data_pdp_type_e *ip_type, char *apn);

/*****************************************************************************
* Prototype     : nwy_data_auth_info_set
* Description   : set auth information
* Input         : sim_id, profile_id, auth: authtype, user: username, pwd: password
* Output        : NULL
* Return Value  : nwy_error_e
* Author        : gehongru
*****************************************************************************
*/
nwy_error_e nwy_data_auth_info_set(nwy_sim_id_e sim_id, int profile_id, nwy_data_auth_proto_e auth, char *user, char *pwd);

/*****************************************************************************
* Prototype     : nwy_data_auth_info_get
* Description   : get auth information
* Input         : sim_id, profile_id
* Output        : auth: auth_type, user: username, pwd: password
* Return Value  : nwy_error_e
* Author        : gehongru
*****************************************************************************
*/
nwy_error_e nwy_data_auth_info_get(nwy_sim_id_e sim_id, int profile_id, nwy_data_auth_proto_e *auth, char *user, char *pwd);

/*****************************************************************************
* Prototype     : nwy_data_flow_info_get
* Description   : get DS flow info
* Input         : sim_id, profile_id, flag
* Output        : flow info
* Return Value  : nwy_error_e
* Author        : liucheng
*****************************************************************************
*/
nwy_error_e nwy_data_flow_info_get(nwy_sim_id_e sim_id, int profile_id, nwy_data_flow_t *flow_info, int clear_flag);
nwy_error_e nwy_data_set_arp_proxy(nwy_sim_id_e sim_id, int enable);
nwy_error_e nwy_data_get_arp_proxy(nwy_sim_id_e sim_id, int *enable);

/*
 *****************************************************************************
 * Prototype	 : nwy_data_get_capability
 * Description	 : get data range on different platform
 * Input		 : req_type
 * Output		 : rsp, user should initialize buffer if string_data requested
 * Return Value  : nwy_error_e
 * Author		 : liufeifan 20230615
 *****************************************************************************
 */
nwy_error_e nwy_data_get_capability(nwy_data_cap_request_type_e req_type, nwy_data_capability_rsp_t *rsp);

/*
 *****************************************************************************
 * Prototype	 : nwy_data_eps_dyn_apn_get
 * Description	 : get default eps bear context apn info 
 * Input		 : sim_id
 * Output		 : ip_type,apn
 * Return Value  : nwy_error_e
 * Author		 : zhaoyating 20250428
 *****************************************************************************
 */

nwy_error_e nwy_data_eps_dyn_apn_get(nwy_sim_id_e sim_id,nwy_data_pdp_type_e *ip_type, char *apn);

/*
 *****************************************************************************
 * Prototype     : nwy_data_dns_info_get
 * Description     : get dns info in spec cid 
 * Input         : sim_id,cid
 * Output         : dns1,dns2,v6dns1,v6dns2
 * Return Value  : nwy_error_e
 * Author         : zhaoyating 20250604
 *****************************************************************************
 */
nwy_error_e nwy_data_dns_info_get(nwy_sim_id_e sim_id,int cid, char *dns1,char *dns2,char *v6dns1,char *v6dns2);

/*
 *****************************************************************************
 * Prototype     : nwy_data_dns_info_set
 * Description     : set dns info in spec cid 
 * Input         : sim_id,cid,dns1,dns2,v6dns1,v6dns2
 * Output         : 
 * Return Value  : nwy_error_e
 * Author         : zhaoyating 20250604
 *****************************************************************************
 */
nwy_error_e nwy_data_dns_info_set(nwy_sim_id_e sim_id,int cid, char *dns1,char *dns2,char *v6dns1,char *v6dns2);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

