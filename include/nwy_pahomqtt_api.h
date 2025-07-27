/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_pahomqtt_api.h
 *Function description: MQTT API interface, each cid supports the creation of three HTTP clients
 * Author       : nwy
 * Created      : 2024-11
 * Description  : mqtt API
 ******************************************************************************/
#ifndef __NWY_PAHOMQTT_API_H__
#define __NWY_PAHOMQTT_API_H__

#include "nwy_common.h"
#include "nwy_ssl_config.h"

#define NWY_PAHO_HOST_ADDRESS_LEN    (256)
#define NWY_PAHO_CLIENT_ID_LEN       (256)
#define NWY_PAHO_USER_NAME_LEN       (512)
#define NWY_PAHO_PASSWORD_LEN        (256)

#define NWY_PAHO_WILLMSG_LEN_MAX        (1024)
#define NWY_PAHO_MSG_LEN_MAX   (1024)
#define NWY_PAHO_PLAYOAD_LEN_MAX   (NWY_PAHO_MSG_LEN_MAX*10+256)

#define NWY_PAHO_TOPIC_LEN_MAX 128

typedef enum {
    NWY_MQTT_VERSION_DEFAULT = 0,
    NWY_MQTT_VERSION_3_1_1 = 4,
}nwy_mqtt_version_e;


typedef enum {
    NWY_MQTT_QOS0_V2,
    NWY_MQTT_QOS1_V2,
    NWY_MQTT_QOS2_V2
}nwy_mqtt_qos_e;

typedef void * nwy_mqtt_handl_t;

typedef struct mqtt_message
{
    int qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    char *topicName;
    char *payload;
    unsigned int payloadlen;
    nwy_mqtt_handl_t mqtt_handle;
} mqtt_message_t;


typedef void (*connect_lost_cb)(nwy_mqtt_handl_t mqtt_handle);
typedef int (*message_arrived_cb)(mqtt_message_t* mqtt_message);

typedef struct{
    char               url_port[NWY_PAHO_HOST_ADDRESS_LEN + 6];
    char               clientid[NWY_PAHO_CLIENT_ID_LEN+1];
    char               username[NWY_PAHO_USER_NAME_LEN+1];
    char               password[NWY_PAHO_PASSWORD_LEN+1] ;
    unsigned int      keepalive;
    unsigned int      cleansession;
    unsigned int      will_flag;
    unsigned int      willretained;
    unsigned int      willqos;
    char               willtopic[NWY_PAHO_TOPIC_LEN_MAX+1];
    char*              willmessage;
    int                willmessage_len;
    nwy_mqtt_version_e mqtt_version;
    int conntimeout_ms;
    message_arrived_cb message_cb;
    connect_lost_cb connlost_cb;
	int max_read_buff_size;
	int max_write_buff_size;
}nwy_mqtt_conn_param_t;

typedef struct nwy_mqtt_pub_msg
{
    nwy_mqtt_qos_e qos;
    unsigned char retained;
    unsigned char dup;
    char *payload;
    unsigned int payloadlen;
} nwy_mqtt_pub_msg_t;

/*
*****************************************************************************
* Prototype     : nwy_mqtt_connect
* Description   : Connect mqtt
* Input         :
                 nwy_mqtt_conn_param_t *param:Mqtt connection parameters
                 nwy_mqtt_ssl_conf_t *ssl_config:SSL mqtt connection parameters, write NULL for unencrypted mqtt

* Return Value  :
    Failure: NULL
    Connection successful: return mqtt handle
*****************************************************************************
*/
nwy_mqtt_handl_t nwy_mqtt_connect(  int cid, nwy_mqtt_conn_param_t *param,nwy_ssl_conf_t *ssl_config);
/*
*****************************************************************************
* Prototype     : nwy_mqtt_pub
* Description   : Mqtt push message interface
* Input         :
                 nwy_mqtt_handl_t mqtt_handl:Mqtt client handle
                 nwy_mqtt_pub_msg_t *pubmsg:Push message parameters
* Return Value  :
    Failure: other values
    Connection successful: return NWY_SUCCESS
*****************************************************************************
*/
int nwy_mqtt_pub(nwy_mqtt_handl_t mqtt_handl, char *topic, nwy_mqtt_pub_msg_t *pubmsg);
/*
*****************************************************************************
* Prototype     : nwy_mqtt_sub
* Description   : MQTT subscription message interface
* Input         :
                 nwy_mqtt_handl_t mqtt_handl:Mqtt client handle
                 char *topic:Subscribed topics
                 nwy_mqtt_qos_e qos:QOS level (0, 1, 2)
* Return Value  :
    Successfully returned 0
    Failure returns other values
*****************************************************************************
*/
int nwy_mqtt_sub(nwy_mqtt_handl_t mqtt_handl,char *topic, nwy_mqtt_qos_e qos);
/*
*****************************************************************************
* Prototype     : nwy_mqtt_unsub_v2
* Description   : Mqtt unsubscribe message
* Input         :
                 nwy_mqtt_handl_v2_t mqtt_handl:Mqtt client handle
                 char *topic:Subscribed topics
* Return Value  :
    Successfully returned NWY_SUCCESS
    Failure returns other values
*****************************************************************************
*/
int nwy_mqtt_unsub(nwy_mqtt_handl_t mqtt_handl,char *topic);

/*
*****************************************************************************
* Prototype     : nwy_mqtt_disconnect
* Description   : Mqtt disconnect interface
* Input         :
                 nwy_mqtt_handl_t mqtt_handl:Mqtt client handle
* Return Value  :
    Successfully returned 0
    Failure returns other values
*****************************************************************************
*/
int nwy_mqtt_disconnect(nwy_mqtt_handl_t mqtt_handl);

/*
*****************************************************************************
* Prototype     : nwy_mqtt_isconnected
* Description   : Mqtt disconnect interface
* Input         :
                 nwy_mqtt_handl_t mqtt_handl:Mqtt client handle
* Return Value  :
    connected returned 1
    disconnected returns 0
*****************************************************************************
*/
int nwy_mqtt_isconnected(nwy_mqtt_handl_t mqtt_handl);

#endif
