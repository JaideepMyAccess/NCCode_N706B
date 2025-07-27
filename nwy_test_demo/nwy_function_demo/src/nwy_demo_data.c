#include "nwy_demo_network_monitor.h"
#include "nwy_demo_utility.h"
#include "nwy_demo_data.h"
int g_data_handle[NWY_DATA_HANDL_MAX] = {0}; //

extern nwy_osi_thread_t nwy_network_monitor_thread;

static void nwy_demo_data_call_cb(int profile_idx, nwy_data_call_state_e ind_state)
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));

    NWY_SDK_LOG_INFO("=DATA= profile_idx=%d,ind_state=%d", profile_idx, ind_state, 0);
    if (profile_idx > 0 && profile_idx < 8)
    {
        if(ind_state == NWY_DATA_CALL_DISCONNECTED)
        {
            ret = nwy_data_unreg_cb(NWY_DEFAULT_SIM_ID, profile_idx);
            NWY_SDK_LOG_INFO("=DATA=  nwy_data_unreg_cb ret= %d", ret, 0, 0);

             g_data_handle[profile_idx -1] = 0;
        } else if (ind_state == NWY_DATA_CALL_CONNECTED) {
            g_data_handle[profile_idx -1] = profile_idx;
        }

        event.id = NWY_DEMO_DATA_CALL_STA_MSG;
        event.param1 = profile_idx;
        event.param2 = ind_state;
        nwy_thread_event_send(nwy_network_monitor_thread, &event, NWY_OSA_SUSPEND);
    }
}

nwy_error_e nwy_demo_call_info_get(int profile_idx, nwy_data_callinfo_t *info)
{
    uint8 *v4_ip = NULL;
    uint8 *v6_ip = NULL;
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    if (info == NULL ) {
        return NWY_GEN_E_INVALID_PARA;
    }
    ret = nwy_data_call_info_get(profile_idx, info);
    if (ret < 0) {
        NWY_SDK_LOG_ERROR("get data call info error %d", ret, 0, 0);
    } else {
        v4_ip = (uint8 *)&(info->v4_info.public_ip.s_addr);
        v6_ip = info->v6_info.public_ip_v6.u6_addr8;
        nwy_demo_uart_echo("Iface address: %d.%d.%d.%d\r\n",
                          v4_ip[0], v4_ip[1], v4_ip[2], v4_ip[3]);
        nwy_demo_uart_echo("Iface address_v6: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n",
                          v6_ip[0], v6_ip[1], v6_ip[2], v6_ip[3],
                          v6_ip[4], v6_ip[5], v6_ip[6], v6_ip[7],
                          v6_ip[8], v6_ip[9], v6_ip[10], v6_ip[11],
                          v6_ip[12], v6_ip[13], v6_ip[14], v6_ip[15]);
        v4_ip = (uint8 *)&(info->v4_info.primary_dns.s_addr);
        v6_ip = info->v6_info.primary_dns_v6.u6_addr8;
        nwy_demo_uart_echo("Dnsp address: %d.%d.%d.%d\r\n",
                          v4_ip[0], v4_ip[1], v4_ip[2], v4_ip[3]);
        nwy_demo_uart_echo("Dnsp address_v6: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n",
                          v6_ip[0], v6_ip[1], v6_ip[2], v6_ip[3],
                          v6_ip[4], v6_ip[5], v6_ip[6], v6_ip[7],
                          v6_ip[8], v6_ip[9], v6_ip[10], v6_ip[11],
                          v6_ip[12], v6_ip[13], v6_ip[14], v6_ip[15]);
        v4_ip = (uint8 *)&(info->v4_info.primary_dns.s_addr);
        v6_ip = info->v6_info.primary_dns_v6.u6_addr8;
        nwy_demo_uart_echo("Dnss address: %d.%d.%d.%d\r\n",
                          v4_ip[0], v4_ip[1], v4_ip[2], v4_ip[3]);
        nwy_demo_uart_echo("Dnss address_v6: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n",
                          v6_ip[0], v6_ip[1], v6_ip[2], v6_ip[3],
                          v6_ip[4], v6_ip[5], v6_ip[6], v6_ip[7],
                          v6_ip[8], v6_ip[9], v6_ip[10], v6_ip[11],
                          v6_ip[12], v6_ip[13], v6_ip[14], v6_ip[15]);
    }

    return ret;
}

void nwy_demo_data_call_clear()
{
    int i = 0;

    for (i = 0; i <NWY_DATA_HANDL_MAX; i ++) {

    NWY_SDK_LOG_INFO("g_data_handle[%d] = %d", i, g_data_handle[i], 0);
        if (g_data_handle[i] != 0) {
            nwy_data_call_stop(NWY_DEFAULT_SIM_ID, g_data_handle[i]);
        }
    }

    return;
}

nwy_error_e nwy_demo_data_call_start(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_sim_id_e sim_id = NWY_DEFAULT_SIM_ID;
    nwy_data_profile_info_t profile_info;
    nwy_data_start_call_t param_t;
    nwy_data_auto_connect_cfg_t cfg;

    memset(&profile_info,0,sizeof(profile_info));
    memset(&param_t, 0, sizeof(param_t));
    memset(&cfg, 0, sizeof(cfg));

    do {
        if (g_data_handle[NWY_DATA_PUBLIC_HANDL] <= 0) {

            /* set profile info */
            profile_info.auth_proto = NWY_DATA_AUTH_PROTO_NONE;
            profile_info.pdp_type = NWY_DATA_PDP_TYPE_IPV4;
            memcpy(profile_info.apn, NWY_PUBLIC_NET_APN_NAME, strlen(NWY_PUBLIC_NET_APN_NAME));
            memcpy(profile_info.pwd, NWY_PUBLIC_NET_APN_PWD, strlen(NWY_PUBLIC_NET_APN_PWD));
            ret = nwy_data_profile_set(sim_id, 1,&profile_info);
            if (ret != NWY_SUCCESS) {
                NWY_SDK_LOG_DEBUG("nwy set apn  fail ret = %d", ret, 0, 0);
                break;
            }

            param_t.profile_idx = 1;
            param_t.auto_connect = 1;

            /* auto connect timer set */
            cfg.interval[0] = NWY_DATA_AUTO_CONNET_INTERVAL;
            cfg.retry_max = NWY_DATA_AUTO_CONNET_TIME;
            ret = nwy_data_auto_connect_set(&cfg);
            if (ret != NWY_SUCCESS) {
                NWY_SDK_LOG_DEBUG("nwy auto connect set  fail ret = %d", ret,0, 0);
                break;
            }


            ret = nwy_data_reg_cb(sim_id, param_t.profile_idx, nwy_demo_data_call_cb);
            if (ret != NWY_SUCCESS) {
                NWY_SDK_LOG_DEBUG("nwy data call reg cb fail ret = %d", ret,0, 0);
                break;
            }

            ret = nwy_data_call_start(sim_id, &param_t);
            if (ret != NWY_SUCCESS) {
                NWY_SDK_LOG_DEBUG("nwy data call start fail ret = %d", ret,0, 0);
                break;
            }

        }
    }while(0);

    return ret;
}

