/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_bt_ble_api.h
 * Author       : zhujunbo
 * Created      : 2025-05-07
 * Description  : bt/ble API
 ******************************************************************************/
#ifndef __NWY_BT_BLE_API_H__
#define __NWY_BT_BLE_API_H__
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

#define BLE_HOST_SCAN_MAX_NUM       20

#define BLE_ADDRESS_SIZE            6
#define BLE_CREATE_SRV_MAX_NUM      2
#define BLE_CREATE_CHAR_MAX_NUM     10
#define BLE_SEND_MAX_LEN            1024

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
    NWY_BLE_ADC_MODE_IDLE,
    NWY_BLE_ADC_MODE_ADVERTISING,
    NWY_BLE_ADC_MODE_UPDATEING,
    NWY_BLE_ADC_MODE_UNORMAL,
} nwy_ble_adc_mode_type;

typedef enum
{
    NWY_BLE_MODE_SLAVE,
    NWY_BLE_MODE_HOST,
} nwy_ble_mode_type;

typedef enum
{
    NWY_BLE_DATA_MODE_ASCII,
    NWY_BLE_DATA_MODE_HEX,
} nwy_ble_data_mode_type;

typedef enum
{
    NWY_BLE_MODE_DISCONNECTED,
    NWY_BLE_MODE_CONNECTED,
} nwy_ble_conn_status_type;

typedef enum
{
    NWY_BLE_CONNECTED,
    NWY_BLE_DISCONNECT,
    NWY_BLE_DATA_RECVED,
    NWY_BLE_UNKOWN,
}nwy_ble_event_e;

//ble cb flag
typedef enum
{
    NWY_BLE_READ_FLAG,
    NWY_BLE_WRITE_FLAG,
    NWY_BLE_ADD_WRITE_FLAG,
}nwy_ble_cb_flag_type;

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
    uint8_t ser_index;
    uint8_t char_index;
    uint8_t char_uuid[16];
    uint8_t char_des; 
    uint8_t char_per; 
    uint16_t permisssion;
    uint16_t char_cp;
}nwy_ble_char_info;

typedef struct
{
    uint32_t ser_index;
    uint8_t ser_uuid[16];
    uint8_t char_num;
    uint8_t p;
    nwy_ble_char_info ser_ble_char[BLE_CREATE_CHAR_MAX_NUM];
}nwy_ble_service_info;

typedef struct
{
    uint8_t ser_id;
    uint8_t char_id;
    uint8_t data[BLE_SEND_MAX_LEN];
    uint16_t datalen;
    uint8_t op;
}nwy_ble_send_info;

typedef struct
{
    uint8_t ser_id;
    uint8_t char_id;
    uint8_t data[244];
    uint16_t datalen;
}nwy_ble_recv_info;

typedef struct
{
    uint8_t ser_id;
    uint8_t char_id;
    uint8_t rw_flag;
}nwy_ble_read_info;

typedef struct
{
    uint8_t ser_id;
    uint8_t char_id;
    uint8_t read_rsp[244];
    uint8_t rsp_len;
}nwy_ble_read_rsp;

typedef struct
{
    int conn_id;
    int connected;
    uint8_t ble_addr[BLE_ADDRESS_SIZE];
}nwy_ble_conn_info;


typedef struct {
    uint8_t uuid[16];
    uint8_t major[2];
    uint8_t minor[2];
    int8_t  txpwr;
}nwy_ble_beacon_info;


typedef struct nwy_ble_cb_t
{
  nwy_ble_event_e event;      /* event id */
  uint32 data_len;            /* data length */
  char* data;                 /* data */
}nwy_ble_cb_t;

typedef struct
{
    int index;
    char addr[18];
    char name[18];
    int type;
    int rssi;
    char adv_data[64];
    uint8_t adv_len;
}nwy_ble_scan_info_t;


typedef struct
{
    int num;
    nwy_ble_scan_info_t ap_list[BLE_HOST_SCAN_MAX_NUM];
}nwy_ble_scan_list_t;



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

typedef void (*nwy_ble_cb)(nwy_ble_cb_t *event);  //notify events
typedef void (*nwy_ble_scanresult_cb)(nwy_ble_scan_list_t* scan_result);  //notify events

/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */

//BLE slave api
/*
 *****************************************************************************
 * Prototype     : nwy_ble_enable
 * Description   : set enable ble 
 * Input         :
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/

nwy_error_e nwy_ble_enable(void);
/*
 *****************************************************************************
 * Prototype     : nwy_ble_disable
 * Description   : set disable ble 
 * Input         :
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_disable(void);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_status
 * Description   : get ble state enable or disable
 * Input         :
 * Output        : status: 0-enable,1-disable
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_status(uint8_t *status);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_version
 * Description   : get ble soft version
 * Input         :
 * Output        : version: version string
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_version(char *version);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_device_name
 * Description   : set ble device name
 * Input         : name：ble device name string (name len 1~24)
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_device_name(char *name);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_device_name
 * Description   : get ble device name
 * Input         : 
 * Output        : name：ble device name string
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_device_name(char *name);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_mac_addr
 * Description   : set ble mac address
 * Input         : mac_addr: ble mac address string(hexstring,len=12)
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_mac_addr(char *mac_addr);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_mac_addr
 * Description   : get ble mac address
 * Input         : 
 * Output        : mac_addr: ble mac address string
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_mac_addr(char *mac_addr);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_mode
 * Description   : set ble mode slave or host
 * Input         : ble_mode: 0:slave 1:host 
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_mode(uint8_t ble_mode);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_mode
 * Description   : get ble mode slave or host
 * Input         : 
 * Output        : ble_mode: 0:slave 1:host  
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_mode(uint8_t *ble_mode);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_adv
 * Description   : set ble advertising enable or disable
 * Input         : enable: 0-disable,1-enable
 * Output        :
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_adv(uint8_t enable);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_adv
 * Description   : get ble advertising state enable or disable
 * Input         : 
 * Output        : enable: 0-disable,1-enable
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_adv(uint8_t *enable);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_adv_interval
 * Description   : set ble advertising param interval time
 * Input         : min_time,max_time: range 32~16384, unit 0.625ms, adv interval 20ms~10.28s
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_adv_interval(int min_time, int max_time);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_adv_interval
 * Description   : get ble advertising param interval time
 * Input         : 
 * Output        : min_time,max_time: adv interval time ,range 32~16384, unit 0.625ms
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_adv_interval(int *min_time, int *max_time);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_conn_rssi
 * Description   : when ble connected, get ble master device RSSI
 * Input         : 
 * Output        : rssi: unit dBm
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_conn_rssi(int *rssi);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_conn_status
 * Description   : get ble connect state
 * Input         : 
 * Output        : status: ble state  0:disconnect 1:connect 2:connect fail 3:smp start
 *                         4:smp success 5:smp fail (3 4 5 when smp mode=1 support)
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_conn_status(uint8_t *status);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_disconnect
 * Description   : when ble is connect, disconnect link
 * Input         : 
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_disconnect(void);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_client_macaddr
 * Description   : when ble is connect, get master device mac address
 * Input         : 
 * Output        : conn_info: conn_id: 0,connected: 0 or 1, ble_addr: mac addr
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_client_macaddr(nwy_ble_conn_info* conn_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_add_service
 * Description   : add ble server
 * Input         : serv_info: ser_uuid: 128bit uuid hex, char_num: server char num(1~10)
 * Output        : serv_info: ser_index: server id (1~2)
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_add_service(nwy_ble_service_info* serv_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_add_charcter
 * Description   : add ble server charcter
 * Input         : char_info: ser_index: server id , ser_uuid: 128bit uuid hex, 
                   char cp: char Permissions(0:write 1:read 2:notify 3:indify 4:write | notify 5:read | notify 6:all)  
 * Output        : char_info: char_index: char id (0~9)
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_add_charcter(nwy_ble_char_info* char_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_start_service
 * Description   : start add ble server
 * Input         : ser_id: server id(1~2)
 * Output        :  
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_start_service(uint8 ser_id);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_add_recv_data
 * Description   : receive add ble server data, when nwy_ble_register_cb, in callback call
 * Input         : 
 * Output        : recv_data: ser_id(1~2),char_id(0~9),data buf[244],datalen
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_add_recv_data(nwy_ble_recv_info *recv_data);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_recv_data
 * Description   : receive deafult ble server data, when nwy_ble_register_cb, in callback call
 * Input         : 
 * Output        : data buf[244],datalen
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_recv_data(uint16_t *datalen, uint8_t *data);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_add_send_data
 * Description   : send data to add ble server
 * Input         : send_data: ser_id(1~2),char_id(0~9),data buf,datalen, op 0 notify 1:indify
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_add_send_data(nwy_ble_send_info* send_data);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_send_data
 * Description   : send data notify to default ble server
 * Input         : data:data buf, datalen: data buf len
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_send_data(uint16_t datalen, uint8_t *data);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_send_indify_data
 * Description   : send data indify to default ble server
 * Input         : data:data buf, datalen: data buf len
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_send_indify_data(uint16_t datalen, uint8_t *data);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_read_req_info
 * Description   : when ble is connect, master read char report
 * Input         : 
 * Output        : read_req_info:ser_id(0~2),char_id(0~9)
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_read_req_info(nwy_ble_read_info* read_req_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_read_rsp_info
 * Description   : when ble is connect, write char, master read get write data
 * Input         : read_rsp:read_req_info:ser_id(0~2),char_id(0~9),read_srp(data buf), rsp_len(data buf len) 
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_read_rsp_info(nwy_ble_read_rsp* read_rsp);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_del_service
 * Description   : delete add ble service (must disconnect and adv disable)
 * Input         : ser_id(1~2)
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/

nwy_error_e nwy_ble_del_service(uint8_t ser_id);
/*
 *****************************************************************************
 * Prototype     : nwy_ble_del_charcter
 * Description   : delete add ble service charcter (must disconnect and adv disable)
 * Input         : ser_id(1~2),char_id(0~9)
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_del_charcter(uint8_t ser_id, uint8_t char_id);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_scanrsp_data
 * Description   : set ble scan response data
 * Input         : Scanrsp_Data:response data buf,scanrsp_len: data len
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_scanrsp_data(uint8_t Scanrsp_Data[31], uint8_t scanrsp_len);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_adv_data
 * Description   : set ble adv data
 * Input         : Adv_Data:adv data buf,adv_len: data len
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_adv_data(uint8_t Adv_Data[31], uint8_t adv_len);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_update_conn
 * Description   : set ble update connect param
 * Input         : handle:connection index(0), intervalMin,intervalMax: minmum and maxmum connection interval. unit: 1.25ms
                   slaveLatency:number of connection events that slave would like to skip 
                   timeoutMulti:timeout before connection is dropped. unit: 10ms
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_update_conn(uint16_t handle, uint16_t intervalMin, uint16_t intervalMax, uint16_t slaveLatency, uint16_t timeoutMulti);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_beacon
 * Description   : set ble beacon mode data
 * Input         : beacon_info: uuid 16byte hex, major 2byte hex,minor 2byte hex, txpwr tx power -127~20(fefault -59)
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_beacon(nwy_ble_beacon_info *beacon_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_beacon
 * Description   : get ble beacon mode data,if not set, get fail
 * Input         : 
 * Output        : beacon_info: uuid 16byte hex, major 2byte hex,minor 2byte hex, txpwr tx power -127~20
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/

nwy_error_e nwy_ble_get_beacon(nwy_ble_beacon_info *beacon_info);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_set_smp
 * Description   : set ble smp mode, need delete master app band info
 * Input         : mode:0 smp disable,1 smp enable. passkey: 0~999999, default 123456
 * Output        : 
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_set_smp(uint8_t mode, uint32_t passkey);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_get_smp
 * Description   : get ble smp mode 
 * Input         : 
 * Output        : mode:0 smp disable,1 smp enable. passkey: 0~999999
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_get_smp(uint8_t *mode, uint32_t *passkey);

/*
 *****************************************************************************
 * Prototype     : nwy_ble_conn_status_cb
 * Description   : register connect state callback,when connect state change ble report state
 * Input         : ble_conn_cb: calback fun, use nwy_ble_get_conn_status get state 
                   and use nwy_ble_get_client_macaddr get master device address
 * Output        :  
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_conn_status_cb(void (*ble_conn_cb)());


/*
 *****************************************************************************
 * Prototype     : nwy_ble_register_cb
 * Description   : register ble read write callback, ble report data
 * Input         : ble_reg_cb: calback fun, flag: event flag
                   NWY_BLE_READ_FLAG:use nwy_ble_read_req_info api get read info;
                   NWY_BLE_WRITE_FLAG:use nwy_ble_recv_data api get reveive default server date;
                   NWY_BLE_ADD_WRITE_FLAG:use nwy_ble_add_recv_data api get reveive add server date;
 * Output        :  
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e nwy_ble_register_cb(void (*ble_reg_cb)(),uint8 flag);


//ble client api
/*
 *****************************************************************************
 * Prototype     : nwy_ble_client_start_scan
 * Description   : register ble master scan callback, get ble scan data
 * Input         : time: scan time unit s, cb_func: calback fun get scan result
 * Output        :  
 * Return Value  : 0-success,other-fail
 *****************************************************************************
*/
nwy_error_e  nwy_ble_client_start_scan(int time, nwy_ble_scanresult_cb cb_func);

#endif

