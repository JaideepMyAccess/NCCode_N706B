#include "nwy_test_cli_utils.h"
#include "global.h"
#if (defined(FEATURE_NWY_PAHO_MQTT_V5)) || (defined(FEATURE_NWY_PAHO_MQTT_V3))
#include "MQTTClient.h"
#include "nwy_pahomqtt_api.h"
#include "nwy_osi_api.h"
#endif

#ifdef FEATURE_NWY_ALI_MQTT_V4X
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"

char *product_key ="a1cPP8Xe4Ax";
char *device_name ="TEST_T8";
char *device_secret ="dP1UMS0gKlNsvBrPvEfiChBmpYnz2lNI";
char *mqtt_host = "a1cPP8Xe4Ax.iot-as-mqtt.cn-shanghai.aliyuncs.com";

extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;
extern const char *ali_ca_cert;

nwy_osi_thread_t g_mqtt_process_thread = NULL;
nwy_osi_thread_t g_mqtt_recv_thread = NULL;
static uint8_t g_mqtt_process_thread_running = 0;
static uint8_t g_mqtt_recv_thread_running = 0;
void *mqtt_handle = NULL;


/************************** FEATURE_NWY_PAHO_MQTT_V3 (Search) **********************/
int32_t demo_state_logcb(int32_t code, char *message)
{
    //nwy_test_cli_echo("logcb:%s", message);
    return 0;
}

void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
        case AIOT_MQTTEVT_CONNECT: {
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_CONNECT\n");
        }
        break;

        case AIOT_MQTTEVT_RECONNECT: {
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_RECONNECT\n");
        }
        break;

        case AIOT_MQTTEVT_DISCONNECT: {
            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                          ("heartbeat disconnect");
            nwy_test_cli_echo("\r\nAIOT_MQTTEVT_DISCONNECT: %s\n", cause);
        }
        break;

        default: {
        }
    }
}

void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            //nwy_test_cli_echo("heartbeat response\n");
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
            nwy_test_cli_echo("\r\nsuback, res: -0x%04X, packet id: %d, max qos: %d\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            nwy_test_cli_echo("\r\npub, qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            nwy_test_cli_echo("\r\npub, payload: %.*s\n", packet->data.pub.payload_len, packet->data.pub.payload);
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            nwy_test_cli_echo("\r\npuback, packet id: %d\n", packet->data.pub_ack.packet_id);
        }
        break;

        default: {

        }
    }
}
//heart package
void demo_mqtt_process_thread(void *args)
{
    int32_t res = STATE_SUCCESS;
    while(1)
    {
        if(g_mqtt_process_thread_running)
        {
            res = aiot_mqtt_process(mqtt_handle);
            if (res == STATE_USER_INPUT_EXEC_DISABLED) 
            {
                nwy_test_cli_echo("\r\nmqtt handle is NULL\n suspend_thread");
                nwy_suspend_thread(g_mqtt_recv_thread);
            }
        }
        else
            nwy_suspend_thread(g_mqtt_process_thread);
            nwy_thread_sleep(200);
    }
}

void demo_mqtt_recv_thread(void *args)
{
    int32_t res = STATE_SUCCESS;
    while(1)
    {
        if(g_mqtt_recv_thread_running)
        {
            res = aiot_mqtt_recv(mqtt_handle);
            if (res == STATE_USER_INPUT_EXEC_DISABLED)
            {
                nwy_test_cli_echo("\r\nmqtt handle is NULL\n suspend_thread");
                nwy_suspend_thread(g_mqtt_recv_thread);
            }
        }
        else
            nwy_suspend_thread(g_mqtt_recv_thread);
        nwy_thread_sleep(200);
    }
}
void nwy_test_cli_alimqtt_connect()
{
    int32_t     res = STATE_SUCCESS;
    uint16_t    port = 1883;
    aiot_sysdep_network_cred_t cred;

    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
    aiot_state_set_logcb(demo_state_logcb);

    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
    cred.max_tls_fragment = 2048;
    cred.sni_enabled = 1;
    cred.x509_server_cert = ali_ca_cert;
    cred.x509_server_cert_len = strlen(ali_ca_cert);
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        nwy_test_cli_echo("\r\naiot_mqtt_init failed\n");
        return -1;
    }

    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)demo_mqtt_default_recv_handler);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)demo_mqtt_event_handler);

    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        nwy_test_cli_echo("\r\naiot_mqtt_connect failed: -0x%04X", -res);
        nwy_test_cli_echo("\r\nplease check variables like mqtt_host, produt_key, device_name, device_secret in demo\r\n");
        return -1;
    }
    nwy_test_cli_echo("\r\nMQTT construct success\n");
    if(nwy_at_ali_yeild_task_init())
        nwy_test_cli_echo("\r\nnwy_at_ali_yeild_task_init failed\n");
    else
        nwy_test_cli_echo("\r\nnwy_at_ali_yeild_task_init success\n");
}

int nwy_at_ali_yeild_task_init(void)
{
   int32_t res = STATE_SUCCESS;
   g_mqtt_process_thread_running = 1;
    if (g_mqtt_process_thread == NULL)
    {
        res = nwy_create_thread(&g_mqtt_process_thread, 1024 * 2, NWY_OSI_PRIORITY_NORMAL, "neo_ali_process_task", demo_mqtt_process_thread,
                                    NULL , 4);
        if(res)
        {
            nwy_test_cli_echo("\r\ng_mqtt_process_thread create failed");
            return -1;
        }
    }
    else
        nwy_resume_thread(g_mqtt_process_thread);
    nwy_thread_sleep(100);
    g_mqtt_recv_thread_running = 1;
    if (g_mqtt_recv_thread == NULL)
    {
        nwy_create_thread(&g_mqtt_recv_thread, 1024 * 4, NWY_OSI_PRIORITY_NORMAL, "neo_ali_recv_task", demo_mqtt_recv_thread,
                                    NULL , 4);
        if(res)
        {
            nwy_test_cli_echo("\r\g_mqtt_recv_thread create failed");
            return -1;
        }
    }
    else
        nwy_resume_thread(g_mqtt_recv_thread);
    nwy_thread_sleep(100);
    return 0;
}

void nwy_test_cli_alimqtt_pub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    char *pub_payload = "{\"id\":\"12\",\"params\":{\"temperature\":18,\"data\":\"2020/04/03\"}}";
    nwy_test_cli_echo("\r\naiot_mqtt_pub topic %s\n", topic);
    res = aiot_mqtt_pub(mqtt_handle, topic, (uint8_t *)pub_payload, (uint32_t)strlen(pub_payload), 0);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_pub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_pub success");
}

void nwy_test_cli_alimqtt_sub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    nwy_test_cli_echo("\r\naiot_mqtt_sub topic %s\n", topic);
    res = aiot_mqtt_sub(mqtt_handle, topic, NULL, 1, NULL);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_sub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_sub success");
}
void nwy_test_cli_alimqtt_unsub()
{
    int32_t res = STATE_SUCCESS;
    char topic[128] = {0};
    sprintf(topic, "/sys/%s/%s/thing/status/update", product_key, device_name);
    nwy_test_cli_echo("\r\naiot_mqtt_unsub topic %s\n", topic);
    res = aiot_mqtt_unsub(mqtt_handle, topic);
    if (res < 0)
        nwy_test_cli_echo("\r\naiot_mqtt_unsub failed, res: -0x%04X\n", -res);
    else
        nwy_test_cli_echo("\r\naiot_mqtt_unsub success");
}
void nwy_test_cli_alimqtt_state()
{
    int32_t res;
    if( aiot_mqtt_state(mqtt_handle)==STATE_SUCCESS )
        nwy_test_cli_echo("\r\naiot_mqtt_status connected");
    else
        nwy_test_cli_echo("\r\naiot_mqtt_status disconnected");
}
void nwy_test_cli_alimqtt_disconnect()
{
    int32_t res = STATE_SUCCESS;
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        aiot_mqtt_deinit(&mqtt_handle);
        nwy_test_cli_echo("\r\naiot_mqtt_disconnect failed: -0x%04X\n", -res);
        return ;
    }
    g_mqtt_recv_thread_running = 0;
    g_mqtt_process_thread_running = 0;
    res = aiot_mqtt_deinit(&mqtt_handle);
    if (res < STATE_SUCCESS) {
        nwy_test_cli_echo("\r\naiot_mqtt_deinit failed: -0x%04X\n", -res);
        return ;
    }
    nwy_test_cli_echo("\r\naiot_mqtt disconnect ok");
}
void nwy_test_cli_alimqtt_del_kv()
{
    nwy_test_cli_echo("\r\nnot support");
    return ;
}

#endif

#ifdef FEATURE_NWY_ALI_MQTT
#include "mqtt_api.h"
#endif
/**************************ALI MQTT*********************************/
char *sptr;

#ifdef FEATURE_NWY_ALI_MQTT

static void *pclient = NULL;

#define MQTT_PRODUCT_KEY "a1cPP8Xe4Ax"
#define MQTT_DEVICE_NAME "TEST_T8"
#define MQTT_DEVICE_SECRET "dP1UMS0gKlNsvBrPvEfiChBmpYnz2lNI"
#define MSG_LEN_MAX (1024)

#define MQTT_PRODUCT_KEY_AUTH1 "a1TezxwJJ2l"
#define MQTT_DEVICE_NAME_AUTH1 "test_device_sy"
#define MQTT_PRODUCT_SECRET_AUTH1 "gqZAOJ1wsnLln3h4"
#define _KV_FILE_NAME_         "/linkkit_kv.bin"
static char g_empty_string[1] = "";
int auth_mode = 0;

static int mqtt_sub_flag = -1;
static int mqtt_pub_flag = -1;
static int mqtt_unsub_flag = -1;
typedef struct
{
    char topic_name[128];
    iotx_mqtt_event_handle_func_fpt topic_handle_func;
}topic_list_t;
static topic_list_t topic_list[5] = {0};
int check_topic_exit(char *topic_name)
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        if(strlen(topic_list[i].topic_name) && strstr(topic_list[i].topic_name, topic_name))
            return 1;
    }
    return 0;
}
int update_topic_list(char *topic_name, int type)
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        if(type == 1)
        {
            if(strlen(topic_list[i].topic_name) == 0)
            {
                strncpy(topic_list[i].topic_name, topic_name, strlen(topic_name));
                return 1;
            }
        }
        else
        {
            if(strstr(topic_list[i].topic_name, topic_name))
            {
                memset(topic_list[i].topic_name, 0, sizeof(topic_list[i].topic_name));
                return 1;
            }
        }
    }
    nwy_test_cli_echo("update failed\r\n");
    return 0;
}


static void message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt)msg->msg;
    char topic_name[256] = {0};
    strncpy(topic_name, ptopic_info->ptopic, ptopic_info->topic_len);
    nwy_test_cli_echo("\r\ntopic_len:%d, Topic:%s, payload_len:%d, Payload:%s\r\n",
                      ptopic_info->topic_len, topic_name,
                      ptopic_info->payload_len, ptopic_info->payload);
}

static void event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type)
    {
    case IOTX_MQTT_EVENT_UNDEF:
        nwy_test_cli_echo("\r\nundefined event occur.");
        break;

    case IOTX_MQTT_EVENT_DISCONNECT:
        nwy_test_cli_echo("\r\nMQTT disconnect.");
        break;

    case IOTX_MQTT_EVENT_RECONNECT:
        nwy_test_cli_echo("\r\nMQTT reconnect.");
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
        mqtt_sub_flag = 1;
        nwy_test_cli_echo("\r\nsubscribe success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
        mqtt_sub_flag = 0;
        nwy_test_cli_echo("\r\nsubscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
        mqtt_sub_flag = 0;
        nwy_test_cli_echo("\r\nsubscribe nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
        mqtt_unsub_flag = 1;
        nwy_test_cli_echo("\r\nunsubscribe success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
        mqtt_unsub_flag = 0;
        nwy_test_cli_echo("\r\nunsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
        mqtt_unsub_flag = 0;
        nwy_test_cli_echo("\r\nunsubscribe nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
        mqtt_pub_flag = 1;
        nwy_test_cli_echo("\r\npublish success, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
        mqtt_pub_flag = 0;
        nwy_test_cli_echo("\r\npublish timeout, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_NACK:
        mqtt_pub_flag = 0;
        nwy_test_cli_echo("\r\npublish nack, packet-id=%u", (unsigned int)packet_id);
        break;

    case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
        nwy_test_cli_echo("\r\ntopic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
        break;

    default:
        nwy_test_cli_echo("\r\nShould NOT arrive here.");
        break;
    }
}

nwy_osi_thread_t task_id = NULL;

void nwy_ali_cycle(void *ctx)
{
    int ret = 0;

    for (;;)
    {
        while (IOT_MQTT_CheckStateNormal(pclient))
        {
            ret = IOT_MQTT_Yield(pclient, 300);
            nwy_thread_sleep(100);
            if (ret != 0)
            {
                IOT_MQTT_Destroy(&pclient);
                break;
            }
        }
        nwy_suspend_thread(task_id);
    }
}

nwy_osi_thread_t nwy_at_ali_yeild_task_init(void)
{
    if (task_id == NULL)
    {
        nwy_create_thread(&task_id, 1024 * 15, NWY_OSI_PRIORITY_NORMAL, "neo_ali_yeild_task", nwy_ali_cycle,
                                    NULL , 4);
    }
    else
    {
        nwy_resume_thread(task_id);
    }

    nwy_test_cli_echo(" CreateThread neo_ali_yeild_task\n");
    return task_id;
}

int nwy_Snprintf(char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int rc;
    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);
    return rc;
}

int iotx_midreport_topic(char *topic_name, char *topic_head, char *product_key, char *device_name)
{
    int ret;
    /* reported topic name: "/sys/${productKey}/${deviceName}/thing/status/update" */
    int len = strlen(product_key) + strlen(device_name) + 128;
    ret = nwy_Snprintf(topic_name,
                       len,
                       "%s/sys/%s/%s/thing/status/update",
                       topic_head,
                       product_key,
                       device_name);
    return ret;
}

static int dynreg_device_secret(const char *device_secret)
{
    int rc = -1;
    int lenth = strlen(device_secret);
    nwy_test_cli_echo("callback func device secret: %s,len=%d", device_secret,lenth);
    if(lenth > IOTX_DEVICE_SECRET_LEN || lenth <= 0)
    {
        return -1;
    }
    IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)device_secret);
    rc = HAL_Kv_Set(_KV_FILE_NAME_, (void*)device_secret, lenth, 1);
    if(rc < 0)
    {
        nwy_test_cli_echo("dynreg_device_secret to KV_SET ERROR rc= %d",rc);
        return -1;
    }
    return 0;
}


void nwy_test_cli_alimqtt_connect()
{
    int domain_type = IOTX_CLOUD_DOMAIN_SH;
    char topic[128] = {0};
    static iotx_conn_info_pt pconn_info = NULL;
    static iotx_mqtt_param_t mqtt_params;
    nwy_osi_thread_t id = NULL;
    unsigned keepalive = 120000;
    unsigned int clean = 0;
    unsigned int timeout = 30000;
    int ret = 0;
    int dynamic_register = 0;
    iotx_mqtt_topic_info_t topic_info;
    char msg[MSG_LEN_MAX] = {0};
    int device_secret_len = IOTX_DEVICE_SECRET_LEN;
    char device_secret[IOTX_DEVICE_SECRET_LEN] = {0,};
    nwy_test_cli_echo("\r\nalimqtt test");
 
    sptr = nwy_test_cli_input_gets("\r\nPlease input authmode(0/1): ");
    auth_mode = atoi(sptr);
    if (auth_mode > 1 || auth_mode < 0)
    {
        nwy_test_cli_echo("\r\ninput auth_mode error");
        return;
    }
    nwy_authmode_type_set(auth_mode);
    if(1 == auth_mode)
    {
        if(0 == HAL_Kv_Get(_KV_FILE_NAME_, (void*)device_secret, &device_secret_len))
        {
            nwy_test_cli_echo("\r\nHAL_Kv_Get OK device_secret=%s,len=%d",device_secret,device_secret_len);
            dynamic_register = 0;
            //memset(g_nwy_alimqtt_at_param.devorproSecret, 0, IOTX_DEVICE_SECRET_LEN + 1);
            //memcpy(g_nwy_alimqtt_at_param.devorproSecret, device_secret, device_secret_len);
            IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)device_secret);
            IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void *)g_empty_string);
        }
        else
        {
            nwy_test_cli_echo("HAL_Kv_Get NULL do ITE_DYNREG_DEVICE_SECRET");
            dynamic_register = 1;
            IOT_RegisterCallback(ITE_DYNREG_DEVICE_SECRET, dynreg_device_secret);
            IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)g_empty_string);
            IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void*)MQTT_PRODUCT_SECRET_AUTH1);
            strcpy(device_secret, MQTT_PRODUCT_SECRET_AUTH1);
        }
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_KEY, (void *)MQTT_PRODUCT_KEY_AUTH1);
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_NAME, (void *)MQTT_DEVICE_NAME_AUTH1);
        nwy_test_cli_echo("\r\setup with %s,%s,%s\n", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1, device_secret);
    }
    else
    {
        if(2 == auth_mode)
        {
            //nwy_alicloud_X509_conn_init(info->engine);
            nwy_test_cli_echo("\r\nauthmode 2 not support");
            return ;
        }
        dynamic_register = 0;
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)MQTT_DEVICE_SECRET);
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_SECRET, (void *)g_empty_string);
        IOT_Ioctl(IOTX_IOCTL_SET_PRODUCT_KEY, (void *)MQTT_PRODUCT_KEY);
        IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_NAME, (void *)MQTT_DEVICE_NAME);
        strcpy(device_secret, MQTT_DEVICE_SECRET);
        nwy_test_cli_echo("\r\setup with %s,%s,%s\n", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME, device_secret);
    }
    IOT_Ioctl(IOTX_IOCTL_SET_DYNAMIC_REGISTER, (void *)&dynamic_register);
    IOT_Ioctl(IOTX_IOCTL_SET_REGION, (void *)&domain_type);

    //IOT_Ioctl(IOTX_IOCTL_SET_DEVICE_SECRET, (void *)MQTT_DEVICE_SECRET);
    ret = IOT_SetupConnInfo(MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME, device_secret, (void **)&pconn_info);
    if (ret == 0)
    {
        if ((NULL == pconn_info) || (0 == pconn_info->client_id))
        {
            nwy_test_cli_echo("\r\nplease auth fail\n");
            return;
        }
        nwy_test_cli_echo("\r\nIOT_SetupConnInfo SUCCESS\r\n");
        memset(&mqtt_params, 0x0, sizeof(mqtt_params));
        mqtt_params.port = pconn_info->port;
        mqtt_params.host = pconn_info->host_name;
        mqtt_params.client_id = pconn_info->client_id;
        mqtt_params.username = pconn_info->username;
        mqtt_params.password = pconn_info->password;
        mqtt_params.pub_key = pconn_info->pub_key;

        mqtt_params.request_timeout_ms = timeout;
        mqtt_params.clean_session = clean;
        mqtt_params.keepalive_interval_ms = keepalive;
        //mqtt_params.pread_buf = msg_readbuf;
        mqtt_params.read_buf_size = MSG_LEN_MAX;
        //mqtt_params.pwrite_buf = msg_writebuf;
        mqtt_params.write_buf_size = MSG_LEN_MAX;

        mqtt_params.handle_event.h_fp = event_handle;
        mqtt_params.handle_event.pcontext = NULL;
        ret = IOT_MQTT_CheckStateNormal(pclient);
        if (1 == ret)
        {
            nwy_test_cli_echo("\r\nMQTT is connected");
            return;
        }
        pclient = IOT_MQTT_Construct(&mqtt_params);
        if (NULL == pclient)
        {
            nwy_test_cli_echo("\r\nMQTT construct failed\n");
            return;
        }
        else
        {
            nwy_test_cli_echo("\r\nMQTT construct success\n");
            id = nwy_at_ali_yeild_task_init();
            if (NULL == id)
            {
                nwy_test_cli_echo("\r\nMQTT construct failed\n");
                return;
            }
        }
        if(1 == auth_mode)
            iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
        else
            iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
        nwy_test_cli_echo("\r\ntopic = %s\r\n", topic);
        memset(topic_list, 0, sizeof(topic_list));
        ret = IOT_MQTT_Subscribe(pclient, topic, IOTX_MQTT_QOS1, message_arrive, NULL);
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
            return;
        }
        while (1)
        {
            if (1 == mqtt_sub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe OK\r\n");
                update_topic_list(topic, 1);
                break;
            }
            else if (0 == mqtt_sub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error\r\n");
                break;
            }
            nwy_thread_sleep(100);
        }
        /*
        iotx_midreport_reqid(requestId,
         dev->product_key,
         dev->device_name)
        iotx_midreport_payload(msg, )/*/
        strcpy(msg, "{hello word}");
        topic_info.qos = IOTX_MQTT_QOS1;
        topic_info.payload = (void *)msg;
        topic_info.payload_len = strlen(msg);
        topic_info.retain = 0;
        topic_info.dup = 0;
        ret = IOT_MQTT_Publish(pclient, topic, &topic_info);
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
            return;
        }

        while (1)
        {
            if (1 == mqtt_pub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Publish OK");
                break;
            }
            else if (0 == mqtt_pub_flag)
            {
                nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
                break;
            }
            nwy_thread_sleep(100);
        }
    }
    else
        nwy_test_cli_echo("\r\nMQTT auth failed");
}

void nwy_test_cli_alimqtt_pub()
{
    char msg[MSG_LEN_MAX] = {0};
    int ret = 0;
    iotx_mqtt_topic_info_t topic_info;
    char topic[128] = {0};

    if(auth_mode == 1)
    {
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
        strcpy(msg, "{\"LightStatus\":\"true\"}");
    }
    else
    {
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
        strcpy(msg, "{\"id\":\"12\",\"params\":{\"temperature\":18,\"data\":\"2020/04/03\"}}");
    }
    topic_info.qos = IOTX_MQTT_QOS1;
    topic_info.payload = (void *)msg;
    topic_info.payload_len = strlen(msg);
    topic_info.retain = 0;
    topic_info.dup = 0;
    ret = IOT_MQTT_Publish(pclient, topic, &topic_info);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
        return;
    }

    while (1)
    {
        if (1 == mqtt_pub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish OK");
            break;
        }
        else if (0 == mqtt_pub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Publish error");
            break;
        }
        nwy_thread_sleep(100);
    }
}

void nwy_test_cli_alimqtt_sub()
{
    char topic[128] = {0};
    int ret = 0;
    if(auth_mode == 1)
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
    else
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);

    nwy_test_cli_echo("\r\ntopic = %s", topic);
    if(check_topic_exit(topic))
    {
        nwy_test_cli_echo("topic has been subscibe\r\n");
        return ;
    }
    ret = IOT_MQTT_Subscribe(pclient, topic, IOTX_MQTT_QOS1, message_arrive, NULL);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
        return;
    }
    mqtt_sub_flag = -1;

    while (1)
    {
        if (1 == mqtt_sub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe OK");
            update_topic_list(topic, 1);
            break;
        }
        else if (0 == mqtt_sub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Subscribe error");
            break;
        }
        nwy_thread_sleep(100);
    }
}

void nwy_test_cli_alimqtt_unsub()
{
    int rc = 0;
    char topic[128] = {0};
    if(auth_mode == 1)
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY_AUTH1, MQTT_DEVICE_NAME_AUTH1);
    else
        iotx_midreport_topic(topic, "", MQTT_PRODUCT_KEY, MQTT_DEVICE_NAME);
    rc = IOT_MQTT_Unsubscribe(pclient, topic);
    if (rc < 0)
    {
        nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe error");
        return;
    }
    while (1)
    {
        if (1 == mqtt_unsub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe OK");
            update_topic_list(topic, 0);
            break;
        }
        else if (0 == mqtt_unsub_flag)
        {
            nwy_test_cli_echo("\r\nIOT_MQTT_Unsubscribe error");
            break;
        }
        nwy_thread_sleep(100);
    }
}

void nwy_test_cli_alimqtt_state()
{
    int ret = 0;
    ret = IOT_MQTT_CheckStateNormal(pclient);
    nwy_test_cli_echo("\r\nMQTT state is %d", ret);
}

void nwy_test_cli_alimqtt_disconnect()
{
    int ret = 0;
    ret = IOT_MQTT_CheckStateNormal(pclient);
    if (ret == 0)
        nwy_test_cli_echo("\r\nMQTT is disconnected ");
    else
    {
        IOT_MQTT_Destroy(&pclient);
        pclient = NULL;
        nwy_test_cli_echo("\r\nMQTT is Destroy");
    }
}
void nwy_test_cli_alimqtt_del_kv()
{
    HAL_Kv_Del(_KV_FILE_NAME_);
    nwy_test_cli_echo("\r\nALIMQTT dek kv success");
}

#endif
#ifdef FEATURE_NWY_PAHO_MQTT_V3
/**************************MQTT*********************************/
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTClient paho_mqtt_client = {0};
unsigned char g_nwy_paho_readbuf[NWY_PAHO_MSG_LEN_MAX] = {0};
unsigned char g_nwy_paho_writebuf[NWY_PAHO_MSG_LEN_MAX] = {0};
nwy_osi_thread_t nwy_paho_task_id = NULL;
Network paho_network = {0};
#define NWY_EXT_SIO_PER_LEN 1024
char echo_buff[NWY_EXT_SIO_PER_LEN + 1] = {0};
nwy_mqtt_conn_param_t paho_mqtt_at_param = {0};
int g_mqtt_ssl_mode = 0;


// New Custom Code
#define MQTT_BROKER_URL        "auqcj463j9dqt-ats.iot.ap-south-1.amazonaws.com"
#define MQTT_BROKER_PORT       8883
#define MQTT_CLIENT_ID         "VendingMachine"
#define MQTT_USERNAME          "VendingMachine"
#define MQTT_PASSWORD          "VendingMachine"
#define MQTT_SSL_MODE          1
#define MQTT_AUTH_MODE         2
#define MQTT_SSL_VERSION       3
#define MQTT_CLEAN_SESSION     1
#define MQTT_KEEP_ALIVE        60
#define MQTT_WILL_QOS          0
#define MQTT_WILL_RETAINED     0

// Topics

char STPayment[50]; // subscribe topic payment
char STTechConfig[50]; // subscribe topic technical configuration
char STBusinessConfig[50]; // subscribe topic business configuration
char STIMEIConfig[80]; // subscribe topic Initial configuration
char STIMEIConfigFota[80]; // subscribe topic Fota configuration
char STReqConfig[50]; // Subscribe to trigger to send the machine config

char PTDispenseStatus[50]; // publish topic dispense status
char PTDispenseInventory[50]; // publish topic dispense status
char PTIncinCycleMessage[50]; // publish topic incineration cycle message
char PTIncinNapkinMessage[50]; // publish topic incineration napkin message
char PTInitialConfig[50]; // publish topic initial configuration
// char PTReqConfig[50]; // publish topic Trigger send configuration

char PTConfigReq[50] = "POS/INTG/CONFIGREQ";
char PTConfigAck[50] = "POS/INTG/ACK";
char PTReqConfig[50] = "POS/INTG/RESFORCURCONFIG";

extern char ImeiNumber[20];
extern char MAC_ID[10];
extern char MERCHANT_ID[15];
extern char MERCH_KEY[5];
// extern void lcd_clear(void);

// Replace these with actual certificates
const char AWS_CA_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\r\n"
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\r\n"
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\r\n"
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\r\n"
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\r\n"
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\r\n"
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\r\n"
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\r\n"
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\r\n"
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\r\n"
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\r\n"
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\r\n"
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\r\n"
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\r\n"
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\r\n"
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\r\n"
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\r\n"
"rqXRfboQnoZsG4q5WTP468SQvvG5\r\n"
"-----END CERTIFICATE-----\r\n";


const char AWS_CLIENT_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIDWjCCAkKgAwIBAgIVAO4N/T9qPDXsEO7Rt33EZGcT+snNMA0GCSqGSIb3DQEB\r\n"
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\r\n"
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTAzMDMxOTUz\r\n"
"NTFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\r\n"
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC25Eu6aMMK38uI+AlZ\r\n"
"EprYARUGTQRamcJYBFGbloiLt9GcI2tg6HDVkYriHDjpo7bew9TfrZmPz7a4NbVp\r\n"
"rU/o5PC9NpT4Jsy/HpBaJwkEXmENRfe1rhr6LD0HS4KE2VFfyujomONAbtlRHZth\r\n"
"IGupzpnialXa67oqDXIS7blqtnwpcsTmYbl2buFErOcH01r0Xa/yH0A2lOWe7d0m\r\n"
"/fGzmHDR22s5f72FnfWHLD9/uqAd4hdrpLE4ojINSjgeFhR41m83DZfK/DgF1Ada\r\n"
"FGgyN/4NVgzTJeMhgVhL6tT3HWxqYjZNS9Y2T6W5z8MKag6TpGBoIs10QKFFOICu\r\n"
"OANtAgMBAAGjYDBeMB8GA1UdIwQYMBaAFD/0W5fKNnwwr2bFwcAPmj1gHi0LMB0G\r\n"
"A1UdDgQWBBT51Un05MtYuzL0HcOv1T1/lbwSKzAMBgNVHRMBAf8EAjAAMA4GA1Ud\r\n"
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAcUffAiOp2YiZ2cTsERU4D77U\r\n"
"bIc85u6B8tBUWDD2iBbVxT6IYld5TgMMZ4rAHRONK7ySGIgKXVg0ek0AL/ahNJFY\r\n"
"omDHQbitxTgbL0feIoPoUHlJLLvrg3i149tksThyXt7oV+GHxTSf5UddIQAgkag0\r\n"
"LiIxNTOsadrk45Wp3a5RTyDqChzPpRSMUES+p5QOU4f4gVgWHxEKc0J27zj4F6mO\r\n"
"MBnGTyFJAiz0d4CDQ6qIDPm5z1i2vj8q3g7owiUcVPV8uBCA/XeigkjuAQS3hoSG\r\n"
"tJVvEPJQK7G/kUY9EOGFuZw3PI8T6uVNjYEmz3E299QBiMB7RDy4OqM2jpA0Rg==\r\n"
"-----END CERTIFICATE-----\r\n";


const char AWS_CLIENT_PRIVATE_KEY[] = "-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIIEpQIBAAKCAQEAtuRLumjDCt/LiPgJWRKa2AEVBk0EWpnCWARRm5aIi7fRnCNr\r\n"
"YOhw1ZGK4hw46aO23sPU362Zj8+2uDW1aa1P6OTwvTaU+CbMvx6QWicJBF5hDUX3\r\n"
"ta4a+iw9B0uChNlRX8ro6JjjQG7ZUR2bYSBrqc6Z4mpV2uu6Kg1yEu25arZ8KXLE\r\n"
"5mG5dm7hRKznB9Na9F2v8h9ANpTlnu3dJv3xs5hw0dtrOX+9hZ31hyw/f7qgHeIX\r\n"
"a6SxOKIyDUo4HhYUeNZvNw2Xyvw4BdQHWhRoMjf+DVYM0yXjIYFYS+rU9x1samI2\r\n"
"TUvWNk+luc/DCmoOk6RgaCLNdEChRTiArjgDbQIDAQABAoIBAAozK0IHK7GEj65g\r\n"
"3uyXzAj17n0+eFqxLpSIESETQSRBqTADDa8G55wRlORNXKMzHTTJSSr8XI8Xr4IQ\r\n"
"hMCTCNzdP8vdqms4hry18KkGektDFDiQSWNZhWmkY/bvMCVGlXI5N8oZFLt4MyiB\r\n"
"9TNygm6i9hQZiBZAhU5pF46UsX2Q/bH9d7gteBXEefdAJ4o4QnRv605RkqdCsICu\r\n"
"2MnBKEHx8g2peiE+zw3D5PdguqPRtPiZjkach9yb3WEQD4UJpXwBZB3G7nVI8HYM\r\n"
"wrNNBggN33spBJ7altsjKMYv5vpCWPpZDFK94AKtkhJc9c62/aGSzdu33S+0qfzr\r\n"
"aDMv0OECgYEA3Gwck0uiRGLyUE17XkKJA25p658JY8uXrdlLkzmvULc1/rwloHA5\r\n"
"68DScU50D545NNFo0VaLgXUvOX757uXQr/k6yeM26WIjH7PpjoMI0yWfrXh+sAtz\r\n"
"NL91UFHXBkXWaravhCB1l3GKNkUsDP8oQsSNhzeP0wLo2x4m9WYGwIkCgYEA1Glq\r\n"
"hqi+LZ4g5rcHCLWdgN5jSkvhMS3jq2I4okbSy6I6PgBzVnet4SM2xKPnUsr/gfh1\r\n"
"JmmC6lfQr8nRVLMchab60rPKniUXK01HFXKOQWotLz1TNdiiGg4jAsUCSRzoXVZC\r\n"
"7vxI2YVF+mvzwY2wBDj1+AqPsWuKOZwCaCZcisUCgYEApo/fjAKkTM8EUmAqcFEQ\r\n"
"3hHqYk1cOBgZtxozfL4jV3gKikK8oB8N9bNQkqR5GXAzxFDVxxKB+sKFfAoSbU8m\r\n"
"QkOwA+z5iqRI7GT0gWdNHNkab2hVO0x7swlWaepd9PSDEUKZINuyYE1A5r+giPWr\r\n"
"A8EpPVtkCEzzjtibEecWBRkCgYEAkIo7Ru7Emt4jnVummbKcPvkVr5T65DBJ4HGy\r\n"
"ABsZjiASaeZ8lbZSyATiW+T8oEYqoBKmBUF/KGAhTb2TiINpQTljLMXTdtHedkTb\r\n"
"vih5zOGnZaHhYZ7Mj9ZW1Kei6oWVSQ5N9boPCJW8DLAw6uCziewI1IS7SwvWv2T9\r\n"
"7klMZ9ECgYEAq5afCSz3BxjD0QuE06k7DtDXVYgLBB1D7X5THExCfvxIAHVCqjgf\r\n"
"8t5vFVAV2OvVSWUxUF1f1Uh4RMVYZ36//hNxYFy5KjVAWYp/PQFpWyWHWPCjVAEf\r\n"
"v0awvoVQiK8XK8aWCBlb8q95jSFnoPkt03C3Ha8MFjwrly704UR2vMc=\r\n"
"-----END RSA PRIVATE KEY-----\r\n";
#define MAX_TOKENS 20
typedef enum {
  JSMN_UNDEFINED = 0,
  JSMN_OBJECT = 1 << 0,
  JSMN_ARRAY = 1 << 1,
  JSMN_STRING = 1 << 2,
  JSMN_PRIMITIVE = 1 << 3
} jsmntype_t;

// JSMN
typedef struct jsmntok {
  jsmntype_t type;
  int start;
  int end;
  int size;
#ifdef JSMN_PARENT_LINKS
  int parent;
#endif
} jsmntok_t;

// extern JsonParser parse_json();

typedef struct {
    jsmntok_t tokens[MAX_TOKENS];
    char *json;
    int token_count;
} JsonParser;

typedef struct jsmn_parser {
  unsigned int pos;     /* offset in the JSON string */
  unsigned int toknext; /* next token to allocate */
  int toksuper;         /* superior token node, e.g. parent object or array */
} jsmn_parser;

// New
typedef struct {
    jsmntok_t *tokens;
    char *json;
    int token_count;
    int token_capacity;
} DynamicJsonParser;

nwy_ssl_conf_t g_mqtt_ssl = {NWY_VERSION_TLS_V1_2_E,NWY_SSL_AUTH_NONE_E,{NULL,0},{NULL,0},{NULL,0}, 0, NULL};

extern unsigned char cmd_flag[8];

extern JsonParser parse_json();
extern DynamicJsonParser parse_json_dynamic();
// extern find_token_index_recursive(DynamicJsonParser *parser, const char *key);
char *get_json_value(JsonParser *parser, const char *key);
// // Function to parse JSON
// JsonParser parse_json(const char *json_string) {
//     JsonParser parser;
//     jsmn_parser jsmn;
//     jsmn_init(&jsmn);

//     // Allocate memory for JSON copy
//     parser.json = strdup(json_string);
//     parser.token_count = jsmn_parse(&jsmn, parser.json, strlen(parser.json), parser.tokens, MAX_TOKENS);

//     if (parser.token_count < 0) {
//         nwy_test_cli_echo("JSON parsing Fail!\n");
//         free(parser.json);
//         parser.json = NULL;
//     }
//     return parser;
// }

void messageArrived(MessageData *md)
{
    char topic_name[NWY_PAHO_TOPIC_LEN_MAX] = {0};
    int i = 0;
    unsigned int remain_len = 0;
    strncpy(topic_name, md->topicName->lenstring.data, md->topicName->lenstring.len);
    nwy_test_cli_echo("\r\n===Message Received======");
    nwy_test_cli_echo("\r\nPayload Length : %d", md->message->payloadlen);
    nwy_test_cli_echo("\r\nTopic Name: %s:\r\n", topic_name);
    remain_len = md->message->payloadlen;

    // if (md->message->payloadlen > NWY_EXT_SIO_PER_LEN)
    // {
    //     for (i = 0; i < ((md->message->payloadlen / NWY_EXT_SIO_PER_LEN) + 1); i++)
    //     {
    //         memset(echo_buff, 0, sizeof(echo_buff));
    //         strncpy(echo_buff, md->message->payload + i * NWY_EXT_SIO_PER_LEN,
    //                 remain_len > NWY_EXT_SIO_PER_LEN ? NWY_EXT_SIO_PER_LEN : remain_len);
    //         remain_len = md->message->payloadlen - (i + 1) * NWY_EXT_SIO_PER_LEN;
    //         nwy_test_cli_echo(echo_buff);
    //     }
    // }
    // else
    // {
    //     memset(echo_buff, 0, sizeof(echo_buff));
    //     strncpy(echo_buff, md->message->payload, md->message->payloadlen);
    //     nwy_test_cli_echo(echo_buff);
    // }

    memset(echo_buff, 0, sizeof(echo_buff));
    strncpy(echo_buff, md->message->payload, md->message->payloadlen);
    nwy_test_cli_echo(echo_buff);
    nwy_test_cli_echo("\r\n==========\r\n");

    nwy_thread_sleep(1000);
    nwy_test_cli_get_heap_info();
    
    // For Machine Configuration
    if( strcmp(topic_name, STIMEIConfig) == 0 ){
        
        JsonParser parser = parse_json(echo_buff);

        if (parser.json) {
            int mid_idx = find_token_index(&parser, "mid", 0);
            int mnt_idx = find_token_index(&parser, "mnt", 0);

            if (mid_idx != -1) {
                int len = parser.tokens[mid_idx].end - parser.tokens[mid_idx].start;
                strncpy(MAC_ID, parser.json + parser.tokens[mid_idx].start, len);
                MAC_ID[len] = '\0';
            }
            
            if (mnt_idx != -1) {
                int len = parser.tokens[mnt_idx].end - parser.tokens[mnt_idx].start;
                strncpy(MERCHANT_ID, parser.json + parser.tokens[mnt_idx].start, len);
                MERCHANT_ID[len] = '\0';
            }
            
            char config_json[128];
            snprintf(config_json, sizeof(config_json),
                     "{\"mid\":\"%s\",\"mnt\":\"%s\"}", MAC_ID, MERCHANT_ID);            

            int fd = nwy_file_open("machineconfig", NWY_AB_MODE);
            if (fd >= 0) {
                nwy_file_write(fd, config_json, strlen(config_json));
                nwy_file_close(fd);
                nwy_test_cli_echo("MachineConfig saved: %s\n", config_json);

                // send_config_ack(5,2);
                PublishCode = 1;
                PublishCodeA = 5;
                PublishCodeB = 2;
                PublishTrigger = true;

                char machineIDDisplay[22];
                lcd_clear();
                sprintf(machineIDDisplay, " Machine ID : %s", MAC_ID);
                char VersionIDDisplay[22];
                sprintf(VersionIDDisplay, " SW Version : %s", VersionNumber);
                Display(0, PAGE2, 0, VersionIDDisplay);
                Display(0, PAGE3, 0, machineIDDisplay);
                delete_file_by_name("incinConfig");
                delete_file_by_name("businessconfig");
                delete_file_by_name("techconfig");
                nwy_thread_sleep(1000);
                Display(0, PAGE5, 0, "   Please  Restart  ");
                Display(0, PAGE7, 0, "     The Machine    ");
                nwy_thread_sleep(5000);
                nwy_test_cli_echo("Restart the Module\n");
            } else {
                nwy_test_cli_echo("Failed to open machineconfig for writing.\n");
                // send_config_ack(5,3);
                PublishCode = 1;
                PublishCodeA = 5;
                PublishCodeB = 3;
                PublishTrigger = true;

                lcd_clear();
                Display(0, PAGE2, 0, "     Init  Config   ");
                Display(0, PAGE4, 0, "       Update      ");
                Display(0, PAGE6, 0, "       Failed       ");
                
                if(IsInstulationOperation){
                    lcd_insun_last_display_time = nwy_uptime_get() - 5000;
                    nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                    NeedToCheckInsunTime = true;
                    NeededInsuinScreen = true;
                    InterruptForDispense = true;
                }else{
                    lcd_last_display_time = nwy_uptime_get() - 5000;
                    NeededDefaultScreen = true; 
                    InterruptForDispense = false;
                    NeededInsuinScreen = false;
                }
            }
        } else {
            nwy_test_cli_echo("JSON parsing failed for Machine Config Message!\n");
        }

    }

    // For QR Payment Acknowledgement
    if(strcmp(topic_name, STPayment) == 0){
        nwy_test_cli_echo("Payment Message Received\n");

        // Parse JSON once
        JsonParser parser = parse_json(echo_buff);

        if (parser.json) {
        // Extract "oid" value
            char *oid_value = get_json_value(&parser, "oid");
            if (oid_value) {
                nwy_test_cli_echo("Extracted OID: %s\n", oid_value);
                // free(oid_value);
            } else {
                nwy_test_cli_echo("OID key not found!\n");
            }

            // Extract "mid" value
            char *mid_value = get_json_value(&parser, "mid");
            if (mid_value) {
                nwy_test_cli_echo("Extracted MID: %s\n", mid_value);
                // free(mid_value);
            }

            
            lcd_insun_last_display_time = nwy_uptime_get();
            lcd_last_display_time = nwy_uptime_get();
            
            nwy_test_cli_echo("*********************** Motor Turned ON ************************");
            uint8_t new_cmd_flag[8] = { 0x0f, 0x01, 0x10, 0xbb, 0x00, 0x00, 0x00, 0x00};
            memcpy(cmd_flag, new_cmd_flag, sizeof(cmd_flag));
            nwy_i2c_send_data();
            nwy_thread_sleep(100);
            
            PaymentScreenActive = false;
            uint8_t new_cmd_flag3[8] = { 0x04, 0x00, 0x04, 0xBB, 0x00, 0x00, 0x00, 0x00 };
            memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
            nwy_i2c_send_data();
            nwy_thread_sleep(100);
            lcd_clear();
            Display(0, PAGE3, 0, " !!   DISPENSING   !!");
            Display(0, PAGE6, 0, "        NAPKIN       ");

            nwy_thread_sleep(1000);

            nwy_test_cli_echo("*********************** Motor Turned OFF ************************");

            // uint8_t new_cmd_flag2[8] = { 0x0f, 0x00, 0x0f, 0xbb, 0x00, 0x00, 0x00, 0x00};
            // memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
            // nwy_i2c_send_data();
            

            nwy_thread_sleep(2000);


            lcd_clear();
            Display(0, PAGE3, 0, "     COLLECT YOUR     ");
            Display(0, PAGE6, 0, "        NAPKIN       ");

            uint8_t new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
            memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
            nwy_i2c_send_data();
            nwy_thread_sleep(100);

            memset( VendingOidValue, "", sizeof( VendingOidValue ) );
            strcpy(VendingOidValue,oid_value);
            PublishTrigger = true;
            PublishCode = 2;
            // send_dispense_order_message(oid_value, 1, 0, machineReadings.StockStatus, 200, 1, 177, 83); // 1 Represents Offline Mode

            nwy_thread_sleep(2000);

            lcd_clear();
            Display(0, PAGE2, 0, "   THANKS FOR USING  ");
            // Display(0, PAGE4, 0, "      BMC HYGIENE    ");
            Display(0, PAGE4, 0, "    PUREFEM HYGIENE  ");
            Display(0, PAGE6, 0, "       FACILITY      ");
            lcd_last_display_time = nwy_uptime_get() - 5000;
            lcd_insun_last_display_time = nwy_uptime_get() - 5000;

            NeededDefaultScreen = true;
            NeedToCheckInsunTime = true;
            if(IsInstulationOperation){
                InterruptForDispense = true;
            }else{
                InterruptForDispense = false;
            }
        }else{
            nwy_test_cli_echo("JSON parsing failed for Payment Message!\n");
        }
    }

    // For Technical Configuration
    if(strcmp(topic_name, STTechConfig) == 0){
        nwy_test_cli_echo("Tech Config Message Received\n");
        
        // Parse JSON once
        // JsonParser parser = parse_json(echo_buff);
        DynamicJsonParser parser = parse_json_dynamic(echo_buff);
        if (parser.json) {
            int icr_idx = find_token_index_recursive(&parser, "icr", 0);
            if (icr_idx != -1 && parser.tokens[icr_idx].type == JSMN_OBJECT) {
                int ira_idx = find_token_index_recursive(&parser, "ira", icr_idx);
                if (ira_idx != -1 && parser.tokens[ira_idx].type == JSMN_OBJECT) {
                    int sta_idx = find_token_index_recursive(&parser, "sta", ira_idx);
                    int stb_idx = find_token_index_recursive(&parser, "stb", ira_idx);
                    int ham_idx = find_token_index_recursive(&parser, "ham", ira_idx);
                    int hbo_idx = find_token_index_recursive(&parser, "hbo", ira_idx);
                    int bct_idx = find_token_index_recursive(&parser, "bct", ira_idx);
                    int bcc_idx = find_token_index_recursive(&parser, "bcc", ira_idx);
        
                    int act_idx = find_token_index_recursive(&parser, "act", ira_idx);
                    int hur_idx = -1, min_idx = -1;
        
                    if (act_idx != -1 && parser.tokens[act_idx].type == JSMN_OBJECT) {
                        hur_idx = find_token_index_recursive(&parser, "hur", act_idx);
                        min_idx = find_token_index_recursive(&parser, "min", act_idx);
                    }
        
                    // after JSON parsing logic

                    if (sta_idx != -1)
                        sta = atoi(parser.json + parser.tokens[sta_idx].start);
                    if (stb_idx != -1)
                        stb = atoi(parser.json + parser.tokens[stb_idx].start);
                    if(ham_idx != -1)
                        ham = atoi(parser.json + parser.tokens[ham_idx].start);
                    if (hbo_idx != -1)
                        hbo = atoi(parser.json + parser.tokens[hbo_idx].start);
                    if (bct_idx != -1)
                        bct = atoi(parser.json + parser.tokens[bct_idx].start);
                    if (bcc_idx != -1)
                        bcc = atoi(parser.json + parser.tokens[bcc_idx].start);
                    if (hur_idx != -1)
                        hur = atoi(parser.json + parser.tokens[hur_idx].start);
                    if (min_idx != -1)
                        min = atoi(parser.json + parser.tokens[min_idx].start);

                    char config_json[256];
                    snprintf(config_json, sizeof(config_json),
                            "{\"sta\":%d,\"stb\":%d,\"ham\":%d,\"hbo\":%d,\"bct\":%d,\"hur\":%d,\"min\":%d,\"bcc\":%d}",
                            sta, stb, ham, hbo, bct, hur, min, bcc);
                    
                    int fd = nwy_file_open("techconfig", NWY_AB_MODE);
                    if (fd >= 0) {
                        nwy_file_write(fd, config_json, strlen(config_json));
                        nwy_file_close(fd);
                        nwy_test_cli_echo("TechConfig saved to file: %s\n", config_json);

                        lcd_clear();
                        Display(0, PAGE2, 0, "   Technical Config ");
                        Display(0, PAGE4, 0, "       Updated      ");
                        Display(0, PAGE6, 0, "    Successfully    ");
                        PublishCode = 1;
                        PublishCodeA = 1;
                        PublishCodeB = 2;
                        PublishTrigger = true;
                        // send_config_ack(1,2);
                        already_triggered_today = false;
                    } else {
                        nwy_test_cli_echo("Failed to open file techconfig for writing.\n");

                        lcd_clear();
                        Display(0, PAGE2, 0, "   Technical Config ");
                        Display(0, PAGE4, 0, "       Update      ");
                        Display(0, PAGE6, 0, "       Failed       ");
                        PublishCode = 1;
                        PublishCodeA = 1;
                        PublishCodeB = 3;
                        PublishTrigger = true;
                        // send_config_ack(1,3);
                    }   


                    if(IsInstulationOperation){
                        lcd_insun_last_display_time = nwy_uptime_get() - 5000;
                        nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                        NeedToCheckInsunTime = true;
                        NeededInsuinScreen = true;
                        InterruptForDispense = true;
                    }else{
                        lcd_last_display_time = nwy_uptime_get() - 5000;
                        NeededDefaultScreen = true; 
                        InterruptForDispense = false;
                        NeededInsuinScreen = false;
                    }

                }else{
                    nwy_test_cli_echo("ira key not found!\n");
                }
            }else{
                nwy_test_cli_echo("icr key not found!\n");
            }
        }else{
            nwy_test_cli_echo("JSON parsing failed for Tech Config Message!\n");
        }
    }

    // For Business Configuration
    if( strcmp(topic_name, STBusinessConfig) == 0 ){
        nwy_test_cli_echo("Business Config Message Received\n");

        JsonParser parser = parse_json(echo_buff);

        if (parser.json) {
            int iid_idx = find_token_index(&parser, "iid", 0);
            int itp_idx = find_token_index(&parser, "itp", 0);
            int qrb_idx = find_token_index(&parser, "qrb", 0);

            if (iid_idx != -1)
                iid = atoi(parser.json + parser.tokens[iid_idx].start);

            if (itp_idx != -1)
                itp = atoi(parser.json + parser.tokens[itp_idx].start);

            if (qrb_idx != -1) {
                int len = parser.tokens[qrb_idx].end - parser.tokens[qrb_idx].start;
                strncpy(qrb_data, parser.json + parser.tokens[qrb_idx].start, len);
                qrb_data[len] = '\0';
            }

            char config_json[1024];
            snprintf(config_json, sizeof(config_json),
                "{\"iid\":%d,\"itp\":%d,\"qrb\":\"%s\"}", iid, itp, qrb_data);

            int fd = nwy_file_open("businessconfig", NWY_AB_MODE);
            if (fd >= 0) {
                nwy_file_write(fd, config_json, strlen(config_json));
                nwy_file_close(fd);
                nwy_test_cli_echo("BusinessConfig saved to file: %s\n", config_json);
                lcd_clear();
                Display(0, PAGE2, 0, "   Business  Config ");
                Display(0, PAGE4, 0, "       Updated      ");
                Display(0, PAGE6, 0, "    Successfully    ");

                // send_config_ack(3,2);
                   PublishCode = 1;
                PublishCodeA = 3;
                PublishCodeB = 2;
                PublishTrigger = true;
            } else {
                nwy_test_cli_echo("Failed to open file businessconfig for writing.\n");
                lcd_clear();
                Display(0, PAGE2, 0, "   Business  Config ");
                Display(0, PAGE4, 0, "       Update      ");
                Display(0, PAGE6, 0, "       Failed       ");

                // send_config_ack(3,3);
                PublishCode = 1;
                PublishCodeA = 3;
                PublishCodeB = 3;
                PublishTrigger = true;
            }
            ValueChanged = true;
            
            if(IsInstulationOperation){
                lcd_insun_last_display_time = nwy_uptime_get() - 5000;
                nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                NeedToCheckInsunTime = true;
                NeededInsuinScreen = true;
                InterruptForDispense = true;
            }else{
                lcd_last_display_time = nwy_uptime_get() - 5000;
                NeededDefaultScreen = true; 
                InterruptForDispense = false;
                NeededInsuinScreen = false;
            }
        }else{
            nwy_test_cli_echo("JSON parsing failed for Business Config Message!\n");
        }

    }
}
extern bool NetworkDisconnectStatus;
extern bool NetworkConnectStatus;
void nwy_paho_cycle(void *ctx)
{
    while (1)
    {
        while (MQTTIsConnected(&paho_mqtt_client))
        {
            MQTTYield(&paho_mqtt_client, 500);
            nwy_thread_sleep(200);
        }
        nwy_test_cli_echo("\r\nMQTT disconnect Event Customed\n");
        NetworkConnectStatus = false;
        NetworkDisconnectStatus = true;
        nwy_thread_suspend(nwy_paho_task_id);
    }
    nwy_thread_sleep(200);
}
nwy_osi_thread_t nwy_paho_yeild_task_init(void)
{

    if (nwy_paho_task_id == NULL)
    {
        nwy_thread_create(&nwy_paho_task_id, "neo_paho_yeild_task",
                          NWY_OSI_PRIORITY_NORMAL, nwy_paho_cycle, NULL, 4, 1024 * 4, NULL);
    }
    else
        nwy_thread_resume(nwy_paho_task_id);
    return nwy_paho_task_id;
}

void mqtt_subscribe_topic(const char *topic)
{
    int rc = MQTTSubscribe(&paho_mqtt_client, (char *)topic, 0, messageArrived);
    if (rc == SUCCESS)
        nwy_test_cli_echo("\r\nSubscribed to: %s", topic);
    else
        nwy_test_cli_echo("\r\nSubscribe failed: %s (rc=%d)", topic, rc);
}

int nwy_mqtt_publish_data(const char *topic, const char *data)
{
    nwy_test_cli_get_heap_info();
    if (!MQTTIsConnected(&paho_mqtt_client))
    {
        nwy_test_cli_echo("\r\nMQTT not connected");
        return -1;
    }

    if (topic == NULL || data == NULL)
    {
        nwy_test_cli_echo("\r\nTopic or data is NULL");
        return -2;
    }

    if (strlen(data) > 512)
    {
        nwy_test_cli_echo("\r\nPayload too long, max 512 bytes");
        return -3;
    }

    MQTTMessage pubmsg = {0};
    pubmsg.qos = 0;        // Hardcoded QoS
    pubmsg.retained = 0;   // Hardcoded retained
    pubmsg.payload = (void *)data;
    pubmsg.payloadlen = strlen(data);
    pubmsg.dup = 0;

    nwy_test_cli_echo("\r\nPublishing to Topic: %s", topic);
    nwy_test_cli_echo("\r\nPayload: %s", data);

    int rc = MQTTPublish(&paho_mqtt_client, topic, &pubmsg);

    if (rc == SUCCESS)
        nwy_test_cli_echo("\r\nMQTT publish successful\r\n");
    else
        nwy_test_cli_echo("\r\nMQTT publish failed: rc = %d", rc);

    nwy_test_cli_get_heap_info();
    return rc;
}



void nwy_test_cli_mqtt_connect_new()
{
    int rc;
    int len = 0;

    if (MQTTIsConnected(&paho_mqtt_client) == 1)
    {
        nwy_test_cli_echo("\r\npaho mqtt already connected");
        return;
    }

    nwy_test_cli_echo("\r\nnwy_test_cli_mqtt_connect [Hardcoded Mode]\r\n");

    memset(&paho_mqtt_at_param, 0, sizeof(nwy_mqtt_conn_param_t));

    strncpy(paho_mqtt_at_param.clientid, MQTT_CLIENT_ID, sizeof(paho_mqtt_at_param.clientid));
    strncpy(paho_mqtt_at_param.username, MQTT_USERNAME, sizeof(paho_mqtt_at_param.username));
    strncpy(paho_mqtt_at_param.password, MQTT_PASSWORD, sizeof(paho_mqtt_at_param.password));

    g_mqtt_ssl_mode = MQTT_SSL_MODE;

    memset(g_nwy_paho_writebuf, 0, NWY_PAHO_MSG_LEN_MAX);
    memset(g_nwy_paho_readbuf, 0, NWY_PAHO_MSG_LEN_MAX);
    memset(&paho_network, 0, sizeof(Network));
    NetworkInit(&paho_network);

    if (g_mqtt_ssl_mode == 1)
    {
        g_mqtt_ssl.authmode = MQTT_AUTH_MODE;

        if (g_mqtt_ssl.authmode == 2)
        {
            // CA Cert
            len = strlen(AWS_CA_CERT);
            paho_network.tlsConnectParams.ca_cert = (char *)malloc(len + 1);
            if (!paho_network.tlsConnectParams.ca_cert)
            {
                nwy_test_cli_echo("\r\nmalloc failed for ca_cert");
                return;
            }
            memset(paho_network.tlsConnectParams.ca_cert, 0, len + 1);
            strncpy(paho_network.tlsConnectParams.ca_cert, AWS_CA_CERT, len);

            // Client Cert
            len = strlen(AWS_CLIENT_CERT);
            paho_network.tlsConnectParams.client_cert = (char *)malloc(len + 1);
            if (!paho_network.tlsConnectParams.client_cert)
            {
                nwy_test_cli_echo("\r\nmalloc failed for client_cert");
                return;
            }
            memset(paho_network.tlsConnectParams.client_cert, 0, len + 1);
            strncpy(paho_network.tlsConnectParams.client_cert, AWS_CLIENT_CERT, len);

            // Client Key
            len = strlen(AWS_CLIENT_PRIVATE_KEY);
            paho_network.tlsConnectParams.client_key = (char *)malloc(len + 1);
            if (!paho_network.tlsConnectParams.client_key)
            {
                nwy_test_cli_echo("\r\nmalloc failed for client_key");
                return;
            }
            memset(paho_network.tlsConnectParams.client_key, 0, len + 1);
            strncpy(paho_network.tlsConnectParams.client_key, AWS_CLIENT_PRIVATE_KEY, len);
        }

        paho_network.tlsConnectParams.ServerVerificationFlag = g_mqtt_ssl.authmode;
        paho_network.is_SSL = 1;
        paho_network.tlsConnectParams.timeout_ms = 5000;
        paho_network.tlsConnectParams.ssl_version = MQTT_SSL_VERSION;
    }
    else
    {
        nwy_test_cli_echo("\r\nis no-SSL NetworkConnect");
    }

    paho_mqtt_at_param.cleansession = MQTT_CLEAN_SESSION;
    paho_mqtt_at_param.keepalive = MQTT_KEEP_ALIVE;

    nwy_test_cli_echo("\r\nConnecting to %s:%d", MQTT_BROKER_URL, MQTT_BROKER_PORT);
    rc = NetworkConnect(&paho_network, MQTT_BROKER_URL, MQTT_BROKER_PORT);
    if (rc < 0)
    {
        nwy_test_cli_echo("\r\nNetworkConnect failed, rc=%d", rc);
        return;
    }

    MQTTClientInit(&paho_mqtt_client, &paho_network, 10000, g_nwy_paho_writebuf, NWY_PAHO_MSG_LEN_MAX,
                   g_nwy_paho_readbuf, NWY_PAHO_MSG_LEN_MAX);

    paho_mqtt_client.defaultMessageHandler = messageArrived;

    data.clientID.cstring = paho_mqtt_at_param.clientid;
    data.username.cstring = paho_mqtt_at_param.username;
    data.password.cstring = paho_mqtt_at_param.password;
    data.keepAliveInterval = paho_mqtt_at_param.keepalive;
    data.cleansession = paho_mqtt_at_param.cleansession;

    if ((rc = MQTTConnect(&paho_mqtt_client, &data)))
    {
        nwy_test_cli_echo("\r\nMQTT connect failed, return code %d", rc);
    }
    else
    {
        nwy_test_cli_echo("\r\nMQTT connect successful");
        nwy_osi_thread_t task_id = nwy_paho_yeild_task_init();
        if (task_id == NULL)
            nwy_test_cli_echo("\r\npaho yield task creation failed");
        else
            nwy_test_cli_echo("\r\npaho yield task started");

        // Subscribe Topic Assign
        sprintf(STPayment, "HFS/%s/dispense", MAC_ID);
        sprintf(STTechConfig, "HFS/%s/techconfig", MAC_ID);
        sprintf(STBusinessConfig, "HFS/%s/businessconfig", MAC_ID);
        sprintf(STIMEIConfig, "POS/PROD/INTG/INITCONFIG/%s", ImeiNumber);
        sprintf(STIMEIConfigFota, "POS/PROD/INTG/INITCONFIG/%s/Fota", ImeiNumber);
        sprintf(STReqConfig, "POS/INTG/%s/REQFORCURCONFIG", MAC_ID);

        sprintf(PTDispenseInventory, "POS/%s/Inventory", MERCH_KEY);
        sprintf(PTDispenseStatus, "POS/HFS/%s/dispensestatus", MERCH_KEY);
        sprintf(PTIncinCycleMessage, "POS/HFS/%s/dispensestatus", MERCH_KEY);
        sprintf(PTIncinNapkinMessage, "POS/HFS/%s/dispensestatus", MERCH_KEY);
        sprintf(PTInitialConfig, "POS/PROD/INTG/INITCONFIG");

        // --- Subscribe to Topics ---
        mqtt_subscribe_topic(STPayment);
        mqtt_subscribe_topic(STTechConfig);
        mqtt_subscribe_topic(STBusinessConfig);
        mqtt_subscribe_topic(STIMEIConfig);
        mqtt_subscribe_topic(STIMEIConfigFota);
        mqtt_subscribe_topic(STReqConfig);

        nwy_thread_sleep(2000);

        // send_initial_config();
        // nwy_thread_sleep(2000);
        // req_config(1);
        // nwy_thread_sleep(2000);
        // req_config(3);

        if(strcmp(MAC_ID, "") == 0 | strcmp(MERCH_KEY, "") == 0){
            nwy_test_cli_echo("MAC ID and Merchant Key are not set. Waiting for Configuration \n");
            nwy_thread_sleep(1000);
            send_initial_config();
        }else{
            if(!businessConfigFound){
                lcd_last_display_time = nwy_uptime_get();
                lcd_clear();
                Display(0, PAGE2, 0, "     Waiting For       ");
                Display(0, PAGE4, 0, "       Business        ");
                Display(0, PAGE6, 0, "    Configuration      ");
                nwy_thread_sleep(2000);
                req_config(3);
                nwy_thread_sleep(5000);
            }
            if(!technicalConfigFound){
                lcd_last_display_time = nwy_uptime_get();
                lcd_clear();
                Display(0, PAGE2, 0, "     Waiting For       ");
                Display(0, PAGE4, 0, "     Techinacal       ");
                Display(0, PAGE6, 0, "    Configuration      ");
                nwy_thread_sleep(2000);
                req_config(1);
                nwy_thread_sleep(5000);
            }
        }

        // load_business_config_values_temp();
    }
}

// Function to publish initial configuration (for N706B)
void send_initial_config()
{
    if (!MQTTIsConnected(&paho_mqtt_client))
    {
        nwy_test_cli_echo("\nERROR: MQTT not connected! Check connection.\n");
        return;
    }

    char json_payload[256] = {0};

    snprintf(json_payload, sizeof(json_payload),
        "{"
        "\"cvn\":\"%s\","
        "\"cpi\":\"%s\","
        "\"ime\":\"%s\""
        "}", VersionNumber, ImeiNumber, ImeiNumber);

    nwy_mqtt_publish_data(PTInitialConfig, json_payload);  // uses qos=0, retained=0
}

void send_inventory_update_message( int spn, int stl) {
    char json_payload[256]; // Buffer to store JSON data

    // Create JSON payload
    snprintf(json_payload, sizeof(json_payload),
        "{"
        "\"mid\": \"%s\","  // Machine ID
        "\"crc\": 200,"
        "\"key\": \"ABAB\","
        "\"cid\": \"%s\","  // Merchant ID
        "\"icr\": {"
        "   \"sla\": [{"
        "       \"spn\": %d,"   // Spiral Number
        "       \"stl\": %d"    // stl = 0: No Stock, 1: Low Stock, 2: Stock Available
        "   }]"
        "}"
        "}", MAC_ID, MERCHANT_ID, spn, stl);

        nwy_mqtt_publish_data(PTDispenseInventory, json_payload);
}

void send_dispense_order_message(char *oid, int pym, int cin, int sus, int sds, int rot, int ser, int mfb) {

    // JSON payload
    char json_payload[500]; // Buffer to store JSON data
    snprintf(json_payload, sizeof(json_payload),
    "{"
    "\"mid\": \"%s\","       // Machine ID
    "\"oid\": \"%s\","       // Order ID
    "\"pym\": %d,"           // Payment Method: 1 = Online, 2 = Coin
    "\"cin\": %d,"           // Coin Amount
    "\"sta\": 200,"          // Status (fixed)
    "\"der\": 0,"            // Error code (fixed)
    "\"dsa\": [{"
    "   \"spn\": 1,"         // Spring Number (fixed)
    "   \"iid\": %d,"         // Item ID (fixed)
    "   \"sus\": %d,"        // Stock status: 0 = No Stock, 1 = Low, 2 = Available
    "   \"sdq\": 1,"         // Default Quantity?
    "   \"qta\": [{"
    "       \"sds\": %d,"    // Status Code
    "       \"rta\": [{"
    "           \"rot\": %d,"// Rotation Number
    "           \"spn\": 1," // Spring Number (again?)
    "           \"ser\": %d,"// Spring Status: 177 = Success
    "           \"mfb\": %d,"// Motor Feedback: 83 = OK
    "           \"dfb\": 81" // Fixed Value
    "       }]"
    "   }]"
    "}],"
    "\"crc\": 200"
    "}",
    MAC_ID, oid, pym, cin, iid, sus, sds, rot, ser, mfb);


    if(NetworkConnectStatus){
        int rc = nwy_mqtt_publish_data(PTDispenseStatus, json_payload);  // uses qos=0, retained=0
        if (rc == 0) {
            nwy_test_cli_echo("\nMessage Published to Topic: %s\nPayload: %s\n", PTDispenseStatus, json_payload);
        } else {
            nwy_test_cli_echo("\nFailed to Publish to Topic: %s\n", PTDispenseStatus);
            nwy_test_cli_echo("AWS is not connected. Added to napkinOfflineData Log File.\n");
            // append_napkin_data(json_payload);
            napkinOfflineLogDataState = true;
        }

    }else{
        nwy_test_cli_echo("Network is not connected. Added to napkinOfflineData Log File.\n");
        // append_napkin_data(json_payload);
        napkinOfflineLogDataState = true;
    }

}

void req_config(int ctr)
{
    if (!MQTTIsConnected(&paho_mqtt_client))
    {
        nwy_test_cli_echo("\nERROR: MQTT not connected! Check connection.\n");
        return;
    }

    char json_payload[256] = {0}; // Buffer to store JSON data

    // Create JSON payload
    snprintf(json_payload, sizeof(json_payload),
        "{"
        "\"mnt\":\"%s\","
        "\"mid\":\"%s\","
        "\"key\":\"ABAB\","
        "\"crc\":200,"
        "\"ctr\":%d"
        "}", MERCHANT_ID, MAC_ID, ctr);

    // Publish using the helper
    nwy_mqtt_publish_data(PTConfigReq, json_payload);
}

void send_config_ack(int ctr, int sta)
{
    if (!MQTTIsConnected(&paho_mqtt_client))
    {
        nwy_test_cli_echo("\nERROR: MQTT not connected! Check connection.\n");
        return;
    }

    char json_payload[256] = {0}; // Buffer to store JSON data

    // Create JSON payload
    snprintf(json_payload, sizeof(json_payload),
        "{"
        "\"cvn\":\"%s\","
        "\"mnt\":\"%s\","
        "\"mid\":\"%s\","
        "\"key\":\"ABAB\","
        "\"crc\":200,"
        "\"ctr\":%d,"
        "\"sta\":%d"
        "}", VersionNumber, MERCHANT_ID, MAC_ID, ctr, sta);

    // nwy_test_cli_echo(json_payload);
    // nwy_test_cli_echo("/n");
    // Publish using helper
    nwy_mqtt_publish_data(PTConfigAck, json_payload);
}


void nwy_test_cli_mqtt_connect()
{
    int rc;
    char host_name [64] = {0};
    int port = 0;
    int len = 0;
    if (MQTTIsConnected(&paho_mqtt_client) == 1)
    {
        nwy_test_cli_echo("\r\npaho mqtt already connect");
        return;
    }
    nwy_test_cli_echo("\r\nnwy_test_cli_mqtt_connect\r\n");
    memset(&paho_mqtt_at_param, 0, sizeof(nwy_mqtt_conn_param_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input url/ip: ");
    strncpy(host_name, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input port: ");
    port = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input client_id: ");
    strncpy(paho_mqtt_at_param.clientid, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input usrname: ");
    strncpy(paho_mqtt_at_param.username, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input password: ");
    strncpy(paho_mqtt_at_param.password, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input sslmode(1-ssl,0-no ssl): ");
    g_mqtt_ssl_mode = atoi(sptr);
    if (g_mqtt_ssl_mode > 1 || g_mqtt_ssl_mode < 0)
    {
        nwy_test_cli_echo("\r\ninput sslmode error");
        return;
    }

    memset(g_nwy_paho_writebuf, 0, NWY_PAHO_MSG_LEN_MAX);
    memset(g_nwy_paho_readbuf, 0, NWY_PAHO_MSG_LEN_MAX);
    memset(&paho_network, 0, sizeof(Network));
    NetworkInit(&paho_network);

    if (g_mqtt_ssl_mode == 1)
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input auth_mode(0/1/2): ");
        g_mqtt_ssl.authmode = atoi(sptr);
        if (g_mqtt_ssl.authmode > 2 || g_mqtt_ssl.authmode < 0)
        {
            nwy_test_cli_echo("\r\ninput auth_mode error");
            return;
        }
        if (g_mqtt_ssl.authmode == 0)
        {
            paho_network.tlsConnectParams.pRootCALocation = NULL;
            paho_network.tlsConnectParams.pDeviceCertLocation = NULL;
            paho_network.tlsConnectParams.pDevicePrivateKeyLocation = NULL;
        }
        else if (g_mqtt_ssl.authmode == 1)
        {
            //ca
            sptr = nwy_test_cli_input_gets("\r\n input ca cert length(1-4096):");
            len = nwy_cli_str_to_int(sptr);
            if (len < 1 || len > 4096)
            {
                nwy_test_cli_echo("\r\n invalid ca cert size:%d", len);
                return;
            }
            paho_network.tlsConnectParams.ca_cert = (char*)malloc(len + 1);
            if(paho_network.tlsConnectParams.ca_cert == NULL)
            {
                nwy_test_cli_echo("\r\n malloc ca cert failed");
                return;
            }
            memset(paho_network.tlsConnectParams.ca_cert, 0, len +1);
            nwy_cli_get_trans_data(paho_network.tlsConnectParams.ca_cert, len);

        }
        else
        {
            //ca
            sptr = nwy_test_cli_input_gets("\r\n input ca cert length(1-4096):");
            len = nwy_cli_str_to_int(sptr);
            if (len < 1 || len > 4096)
            {
                nwy_test_cli_echo("\r\n invalid ca cert size:%d", len);
                return;
            }
            paho_network.tlsConnectParams.ca_cert = (char*)malloc(len + 1);
            if(paho_network.tlsConnectParams.ca_cert == NULL)
            {
                nwy_test_cli_echo("\r\n malloc ca cert failed");
                return;
            }
            memset(paho_network.tlsConnectParams.ca_cert, 0, len +1);
            nwy_cli_get_trans_data(paho_network.tlsConnectParams.ca_cert, len);

            //client cert
            sptr = nwy_test_cli_input_gets("\r\n input client cert length(1-4096):");
            len = nwy_cli_str_to_int(sptr);
            if (len < 1 || len > 4096)
            {
                nwy_test_cli_echo("\r\n invalid client cert size:%d", len);
                return;
            }
            paho_network.tlsConnectParams.client_cert = (char*)malloc(len + 1);
            if(paho_network.tlsConnectParams.client_cert == NULL)
            {
                nwy_test_cli_echo("\r\n malloc client cert failed");
                return;
            }
            memset(paho_network.tlsConnectParams.client_cert, 0, len +1);
            nwy_cli_get_trans_data(paho_network.tlsConnectParams.client_cert, len);

            //client key
            sptr = nwy_test_cli_input_gets("\r\n input client key length(1-4096):");
            len = nwy_cli_str_to_int(sptr);
            if (len < 1 || len > 4096)
            {
                nwy_test_cli_echo("\r\n invalid client key size:%d", len);
                return;
            }
            paho_network.tlsConnectParams.client_key = (char*)malloc(len + 1);
            if(paho_network.tlsConnectParams.client_key == NULL)
            {
                nwy_test_cli_echo("\r\n malloc client key failed");
                return;
            }
            memset(paho_network.tlsConnectParams.client_key, 0, len +1);
            nwy_cli_get_trans_data(paho_network.tlsConnectParams.client_key, len);

        }
        paho_network.tlsConnectParams.ServerVerificationFlag = g_mqtt_ssl.authmode;
        paho_network.is_SSL = 1;
        paho_network.tlsConnectParams.timeout_ms = 5000;
        sptr = nwy_test_cli_input_gets("\r\nPlease input sslversion: ");
        paho_network.tlsConnectParams.ssl_version = atoi(sptr);
        if (paho_network.tlsConnectParams.ssl_version  > 3 || paho_network.tlsConnectParams.ssl_version  < 0)
        {
            nwy_test_cli_echo("\r\ninput sslversion error");
            return;
        }
    }
    else
        nwy_test_cli_echo("\r\nis no-SSL NetworkConnect");
    sptr = nwy_test_cli_input_gets("\r\nPlease input clean_flag(0/1): ");
    paho_mqtt_at_param.cleansession = atoi(sptr);
    if (paho_mqtt_at_param.cleansession > 1 || paho_mqtt_at_param.cleansession < 0)
    {
        nwy_test_cli_echo("\r\ninput clean_flag error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input keep_alive: ");
    paho_mqtt_at_param.keepalive = atoi(sptr);
    nwy_test_cli_echo("\r\nip:%s, port :%d", host_name, port);
    rc = NetworkConnect(&paho_network, host_name, port);
    if (rc < 0)
    {
        nwy_test_cli_echo("\r\nNetworkConnect err rc=%d", rc);
        return;
    }
    nwy_test_cli_echo("\r\nNetworkConnect ok");
    MQTTClientInit(&paho_mqtt_client, &paho_network, 10000, g_nwy_paho_writebuf, NWY_PAHO_MSG_LEN_MAX,
                   g_nwy_paho_readbuf, NWY_PAHO_MSG_LEN_MAX);
    paho_mqtt_client.defaultMessageHandler = messageArrived;
    data.clientID.cstring = paho_mqtt_at_param.clientid;
    if (0 != strlen((char *)paho_mqtt_at_param.username) && 0 != strlen((char *)paho_mqtt_at_param.password))
    {
        data.username.cstring = paho_mqtt_at_param.username;
        data.password.cstring = paho_mqtt_at_param.password;
    }
    data.keepAliveInterval = paho_mqtt_at_param.keepalive;
    data.cleansession = paho_mqtt_at_param.cleansession;
    if (0 != strlen((char *)paho_mqtt_at_param.willtopic))
    {
        memset(&data.will, 0x0, sizeof(data.will));
        data.willFlag = 1;
        data.will.retained = paho_mqtt_at_param.willretained;
        data.will.qos = paho_mqtt_at_param.willqos;
        if (paho_mqtt_at_param.willmessage_len != 0)
        {
            data.will.topicName.lenstring.data = paho_mqtt_at_param.willtopic;
            data.will.topicName.lenstring.len = strlen((char *)paho_mqtt_at_param.willtopic);
            data.will.message.lenstring.data = paho_mqtt_at_param.willmessage;
            data.will.message.lenstring.len = paho_mqtt_at_param.willmessage_len;
        }
        else
        {
            data.will.topicName.cstring = paho_mqtt_at_param.willtopic;
            data.will.message.cstring = paho_mqtt_at_param.willmessage;
        }
        nwy_test_cli_echo("\r\nMQTT will ready");
    }
    nwy_test_cli_echo("\r\nConnecting MQTT");
    if ((rc = MQTTConnect(&paho_mqtt_client, &data)))
        nwy_test_cli_echo("\r\nFailed to create client, return code %d", rc);
    else
    {
        nwy_test_cli_echo("\r\nMQTT connect ok");
        nwy_osi_thread_t task_id = nwy_paho_yeild_task_init();
        if (task_id == NULL)
            nwy_test_cli_echo("\r\npaho yeid task create failed ");
        else
            nwy_test_cli_echo("\r\npaho yeid task create ok ");
    }
}

void nwy_test_cli_mqtt_pub()
{
    int rc;
    MQTTMessage pubmsg = {0};
    char topic[NWY_PAHO_TOPIC_LEN_MAX] = {0};
    char msg[NWY_PAHO_WILLMSG_LEN_MAX] = {0};
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        pubmsg.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput retained error");
            return;
        }

        pubmsg.retained = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 512): ");
        if (strlen(sptr) > 512)
        {
            nwy_test_cli_echo("\r\nNo more than 512 bytes at a time ");
            return;
        }
        strncpy(msg, sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, msg = %s",
                          pubmsg.retained,
                          pubmsg.qos,
                          topic,
                          msg);
        pubmsg.payload = (void *)msg;
        pubmsg.payloadlen = strlen(msg);
        pubmsg.dup = 0;
        rc = MQTTPublish(&paho_mqtt_client, topic, &pubmsg);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}

void nwy_test_cli_mqtt_sub()
{
    int rc;
    char topic[NWY_PAHO_TOPIC_LEN_MAX] = {0};
    int qos = 0;
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        qos = atoi(sptr);
        rc = MQTTSubscribe(&paho_mqtt_client,
                           (char *)topic,
                           qos,
                           messageArrived);
        if (rc == SUCCESS)
            nwy_test_cli_echo("\r\nMQTT Sub ok");
        else
            nwy_test_cli_echo("\r\nMQTT Sub error:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_unsub()
{
    int rc;
    char topic[NWY_PAHO_TOPIC_LEN_MAX] = {0};
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(topic, sptr, strlen(sptr));
        rc = MQTTUnsubscribe(&paho_mqtt_client, topic);
        if (rc == SUCCESS)
            nwy_test_cli_echo("\r\nMQTT Unsub ok");
        else
            nwy_test_cli_echo("\r\nMQTT Unsub error:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_state()
{
    if (MQTTIsConnected(&paho_mqtt_client))
        nwy_test_cli_echo("\r\nMQTTconnect");
    else
        nwy_test_cli_echo("\r\nMQTT disconnect");
}

void nwy_test_cli_mqtt_disconnect()
{
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        MQTTDisconnect(&paho_mqtt_client);
        NetworkDisconnect(&paho_network);
    }
    nwy_test_cli_echo("\r\nMQTT disconnect ok");
}
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
void nwy_test_cli_mqtt_pub_test(void)
{
    int rc, i;
    MQTTMessage pubmsg = {0};
    if (MQTTIsConnected(&paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput retained error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
        sptr = nwy_test_cli_input_gets("\r\nPlease input 1k str,msg(10k) consists of 10 str");
        if (strlen(sptr) != 1024)
        {
            nwy_test_cli_echo("\r\nmust be 1k message");
            return;
        }
        for (i = 0; i < 10; i++)
            strncpy(paho_mqtt_at_param.message + i * strlen(sptr), sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, strlen is %d",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          strlen(paho_mqtt_at_param.message));
        memset(&pubmsg, 0, sizeof(pubmsg));
        pubmsg.payload = (void *)paho_mqtt_at_param.message;
        pubmsg.payloadlen = strlen(paho_mqtt_at_param.message);
        pubmsg.qos = paho_mqtt_at_param.qos;
        pubmsg.retained = paho_mqtt_at_param.retained;
        pubmsg.dup = 0;
        rc = nwy_MQTTPublish(&paho_mqtt_client, paho_mqtt_at_param.topic, &pubmsg);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}
#endif
#endif
#ifdef FEATURE_NWY_PAHO_MQTT_V5
MQTTClient paho_mqtt_client;
nwy_osi_thread_t nwy_paho_task_id;
#define NWY_EXT_SIO_PER_LEN 1024
char echo_buff[NWY_EXT_SIO_PER_LEN + 1] = {0};
nwy_paho_mqtt_at_param_type paho_mqtt_at_param = {0};
int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
    char topic_name[PAHO_TOPIC_LEN_MAX] = {0};
    int i = 0;
    unsigned int remain_len = 0;
    nwy_test_cli_echo("\r\n===messageArrived======");
    nwy_test_cli_echo("\r\npayloader len is %d", m->payloadlen);
    nwy_test_cli_echo("\r\ntopic name is %s:\r\n", topicName);
    nwy_test_cli_echo("message received is ");
    remain_len = m->payloadlen;
    if (m->payloadlen > NWY_EXT_SIO_PER_LEN)
    {
        for (i = 0; i < ((m->payloadlen / NWY_EXT_SIO_PER_LEN) + 1); i++)
        {
            memset(echo_buff, 0, sizeof(echo_buff));
            strncpy(echo_buff, m->payload + i * NWY_EXT_SIO_PER_LEN,
                    remain_len > NWY_EXT_SIO_PER_LEN ? NWY_EXT_SIO_PER_LEN : remain_len);
            remain_len = m->payloadlen - (i + 1) * NWY_EXT_SIO_PER_LEN;
            nwy_test_cli_echo(echo_buff);
        }
    }
    else
    {
        memset(echo_buff, 0, sizeof(echo_buff));
        strncpy(echo_buff, m->payload, m->payloadlen);
        nwy_test_cli_echo(echo_buff);
    }
    MQTTClient_free(topicName);
    MQTTClient_freeMessage(&m);
    return 1;
}


void nwy_paho_cycle(void *ctx)
{
    while (1)
    {
        while (MQTTClient_isConnected(paho_mqtt_client))
        {
            nwy_thread_sleep(500);
        }
        nwy_test_cli_echo("\r\nMQTT disconnect ,Out paho cycle");
        
        nwy_suspend_thread(nwy_paho_task_id);
    }
    nwy_thread_sleep(200);
}


nwy_osi_thread_t nwy_paho_yeild_task_init(void)
{

    if ( nwy_paho_task_id == NULL)
    {
        nwy_create_thread(&nwy_paho_task_id, 1024 * 2,NWY_OSI_PRIORITY_NORMAL,"neo_paho_yeild_task", nwy_paho_cycle,
                                                     NULL, 4);
    }
    else
        nwy_resume_thread(nwy_paho_task_id);
    return nwy_paho_task_id;
}
void nwy_test_cli_mqtt_connect()
{
    int rc;
    size_t file_size;
    int authmode =0;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions sslopts = MQTTClient_SSLOptions_initializer;
    MQTTClient_init();
    char url[128] = {0};
    nwy_test_cli_echo("\r\nnwy_test_cli_mqtt_connect\r\n");
    if (MQTTClient_isConnected(paho_mqtt_client) == 1)
    {
        nwy_test_cli_echo("\r\npaho mqtt already connect");
        return;
    }
    memset(&paho_mqtt_at_param, 0, sizeof(nwy_paho_mqtt_at_param_type));
    sptr = nwy_test_cli_input_gets("\r\nPlease input url/ip: ");
    strncpy(paho_mqtt_at_param.host_name, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input port: ");
    paho_mqtt_at_param.port = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input client_id: ");
    strncpy(paho_mqtt_at_param.clientID, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input usrname: ");
    strncpy(paho_mqtt_at_param.username, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\nPlease input password: ");
    strncpy(paho_mqtt_at_param.password, sptr, strlen(sptr));
    opts.MQTTVersion = MQTTVERSION_DEFAULT;
    sptr = nwy_test_cli_input_gets("\r\nPlease input sslmode(1-ssl,0-no ssl): ");
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput sslmode error");
        return;
    }
    paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode = atoi(sptr);
    if(paho_mqtt_at_param.paho_ssl_tcp_conf.sslmode == 0)
        sprintf(url, "%s:%d", paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
    else
    {
        sprintf(url, "ssl://%s:%d", paho_mqtt_at_param.host_name, paho_mqtt_at_param.port);
        sptr = nwy_test_cli_input_gets("\r\nPlease input auth_mode(0/1/2): ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput auth_mode error");
            return;
        }
        authmode = atoi(sptr);
        if (authmode == 0)
           sslopts.enableServerCertAuth = 0;
        else if (authmode == 1)
        {
           sslopts.enableServerCertAuth = 1;
           sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
           sslopts.trustStore = paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name;
        }
        else
        {
           sslopts.enableServerCertAuth = 1;
           sptr = nwy_test_cli_input_gets("\r\nPlease input ca: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.cacert.cert_name, 
                    &sslopts.trustStore, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\nload ca error");
                return;
           }
           sptr = nwy_test_cli_input_gets("\r\nPlease input clientcert: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.clientcert.cert_name, 
                    &sslopts.keyStore, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\niload keyStore error");
                return;
           }
           sptr = nwy_test_cli_input_gets("\r\nPlease input clientkey: ");
           strncpy(paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name, sptr, strlen(sptr));
           if(nwy_mbedtls_load_file(paho_mqtt_at_param.paho_ssl_tcp_conf.clientkey.cert_name, 
                    &sslopts.privateKey, &file_size)< 0)
           {
                nwy_test_cli_echo("\r\niload privateKey error");
                return;
           }
        }
        sptr = nwy_test_cli_input_gets("\r\nPlease input ssl version(0/1/2/3): ");
        if (atoi(sptr) > 3 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput sslverison error");
            return;
        }
        sslopts.sslVersion = atoi(sptr);
        sslopts.verify = 1;
        opts.ssl = &sslopts;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input keep_alive: ");
    opts.keepAliveInterval = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input clean_flag(0/1): ");
    if (atoi(sptr) > 1 || atoi(sptr) < 0)
    {
        nwy_test_cli_echo("\r\ninput clean_flag error");
        return;
    }
    opts.cleansession = atoi(sptr);
    opts.username = paho_mqtt_at_param.username;
    opts.password = paho_mqtt_at_param.password;
    opts.connectTimeout = 10;
    opts.serverURIs = url;
    nwy_test_cli_echo("\r\nconnect url :%s\n", url);
    rc = MQTTClient_create(&paho_mqtt_client, url, paho_mqtt_at_param.clientID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    if(rc != MQTTCLIENT_SUCCESS)
    {
        nwy_test_cli_echo("create mqtt client rc : %d\n", rc);
        return ;
    }
    rc = MQTTClient_setCallbacks(paho_mqtt_client, NULL, NULL, messageArrived, NULL);
    if(rc != MQTTCLIENT_SUCCESS)
    {
        nwy_test_cli_echo("set callbacks rc : %d\n", rc);
        return ;
    }
    nwy_test_cli_echo("\r\nConnecting MQTT");
    rc = MQTTClient_connect(paho_mqtt_client, &opts);
    if (rc!=MQTTCLIENT_SUCCESS)
        nwy_test_cli_echo("\r\nFailed to connect client, return code %d", rc);
    else
    {
        nwy_test_cli_echo("\r\nMQTT connect ok");
        nwy_osi_thread_t task_id = nwy_paho_yeild_task_init();
        if (task_id == NULL)
            nwy_test_cli_echo("\r\npaho yeid task create failed ");
        else
            nwy_test_cli_echo("\r\npaho yeid task create ok ");
    }
}

void nwy_test_cli_mqtt_sub()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        rc = MQTTClient_subscribe(paho_mqtt_client, (char *)paho_mqtt_at_param.topic, paho_mqtt_at_param.qos);
        if (rc < 0)
            nwy_test_cli_echo("\r\nMQTT Sub error:%d", rc);
        else
            nwy_test_cli_echo("\r\nMQTT Sub ok");
    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_pub()
{
    int rc;
    MQTTClient_deliveryToken last_token;
    MQTTProperties pub_props = MQTTProperties_initializer;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
        sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 2048): ");
        if (strlen(sptr) > NWY_EXT_SIO_RX_MAX)
        {
            nwy_test_cli_echo("\r\nNo more than 2048 bytes at a time ");
            return;
        }
#else
        sptr = nwy_test_cli_input_gets("\r\nPlease input message(<= 512): ");
        if (strlen(sptr) > 512)
        {
            nwy_test_cli_echo("\r\nNo more than 512 bytes at a time ");
            return;
        }
#endif
        strncpy(paho_mqtt_at_param.message, sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, msg = %s",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          paho_mqtt_at_param.message);
        rc = MQTTClient_publish(paho_mqtt_client, paho_mqtt_at_param.topic, strlen(paho_mqtt_at_param.message),
                paho_mqtt_at_param.message, paho_mqtt_at_param.qos, paho_mqtt_at_param.retained, &last_token);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}

void nwy_test_cli_mqtt_unsub()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        rc = MQTTClient_unsubscribe(paho_mqtt_client, paho_mqtt_at_param.topic);
        if (rc < 0)
            nwy_test_cli_echo("\r\nMQTT unsub error:%d", rc);
        else
            nwy_test_cli_echo("\r\nMQTT unsub ok");


    }
    else
        nwy_test_cli_echo("\r\nMQTT no connect");
}

void nwy_test_cli_mqtt_state()
{
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
        nwy_test_cli_echo("\r\nMQTTconnect");
    else
        nwy_test_cli_echo("\r\nMQTT disconnect");
}

void nwy_test_cli_mqtt_disconnect()
{
    int rc;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        MQTTClient_disconnect(paho_mqtt_client, 0);
        MQTTClient_destroy(&paho_mqtt_client);
    }
    nwy_test_cli_echo("\r\nMQTT disconnect ok");
}
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
void nwy_test_cli_mqtt_pub_test(void)
{
    int rc, i;
    MQTTClient_deliveryToken last_token;
    MQTTProperties pub_props = MQTTProperties_initializer;
    MQTTClient_init();
    if (MQTTClient_isConnected(paho_mqtt_client))
    {
        memset(paho_mqtt_at_param.topic, 0, sizeof(paho_mqtt_at_param.topic));
        sptr = nwy_test_cli_input_gets("\r\nPlease input topic: ");
        strncpy(paho_mqtt_at_param.topic, sptr, strlen(sptr));
        sptr = nwy_test_cli_input_gets("\r\nPlease input qos: ");
        if (atoi(sptr) > 2 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.qos = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\nPlease input retained: ");
        if (atoi(sptr) > 1 || atoi(sptr) < 0)
        {
            nwy_test_cli_echo("\r\ninput qos error");
            return;
        }
        paho_mqtt_at_param.retained = atoi(sptr);
        memset(paho_mqtt_at_param.message, 0, sizeof(paho_mqtt_at_param.message));
        sptr = nwy_test_cli_input_gets("\r\nPlease input 1k str,msg(10k) consists of 10 str");
        if (strlen(sptr) != 1024)
        {
            nwy_test_cli_echo("\r\nmust be 1k message");
            return;
        }
        for (i = 0; i < 10; i++)
            strncpy(paho_mqtt_at_param.message + i * strlen(sptr), sptr, strlen(sptr));
        nwy_test_cli_echo("\r\nmqttpub param retained = %d, qos = %d, topic = %s, strlen is %d",
                          paho_mqtt_at_param.retained,
                          paho_mqtt_at_param.qos,
                          paho_mqtt_at_param.topic,
                          strlen(paho_mqtt_at_param.message));
        rc = MQTTClient_publish(paho_mqtt_client, paho_mqtt_at_param.topic, strlen(paho_mqtt_at_param.message),
                        paho_mqtt_at_param.message, paho_mqtt_at_param.qos, paho_mqtt_at_param.retained, &last_token);
        nwy_test_cli_echo("\r\nmqtt publish rc:%d", rc);
    }
    else
        nwy_test_cli_echo("\r\nMQTT not connect");
}
#endif
#endif
