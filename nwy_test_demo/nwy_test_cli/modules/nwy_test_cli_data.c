#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

static int nwy_data_sim_id = 0;
static int current_open_dial_action[7] = {0};
#define NWY_DATA_MAX_DNS_LEN 64
static int is_trigger_by_app[7] = {0};
/**************************DATA*********************************/
/**************************CUSTOM CODE*********************************/
extern bool NetworkDisconnectStatus;
extern bool NetworkConnectStatus;
static void nwy_test_cli_data_cb_fun(int profile_idx, nwy_data_call_state_e ind_state)
{

    nwy_sim_slot_get((nwy_sim_id_e *)&nwy_data_sim_id);

    NWY_CLI_LOG("=DATA= profile_idx=%d,ind_state=%d", profile_idx, ind_state);
    if (profile_idx > 0 && profile_idx < 8)
    {
        nwy_test_cli_echo("\r\nData call status update, profile_id:%d,state:%d\r\n", profile_idx, ind_state);
        if(ind_state == NWY_DATA_CALL_DISCONNECTED_STATE)
        {
            if(is_trigger_by_app[profile_idx-1]==1)
            {
                // if the data callback is trigger by current app , unreg the cb function
                if(current_open_dial_action[profile_idx-1]==ind_state)
                {
                    nwy_test_cli_echo("\r\nData call status NWY_DATA_CALL_DISCONNECTED_STATE\r\n");
                    nwy_data_unreg_cb(nwy_data_sim_id, profile_idx,nwy_test_cli_data_cb_fun);        
                }else{
                        nwy_test_cli_echo("\r\nStart data call failed\r\n");
                    }
            }else{
                nwy_test_cli_echo("\r\nData call status NWY_DATA_CALL_DISCONNECTED_STATE\r\n");
            }
            NetworkConnectStatus = false;
        }
        else
        {
            if(is_trigger_by_app[profile_idx-1]==1)
            {
                nwy_test_cli_echo("\r\nStart data call success\r\n");
            }
            nwy_test_cli_echo("\r\nData call status NWY_DATA_CALL_CONNECTED_STATE\r\n");
                // nwy_thread_sleep(1500);
            NetworkConnectStatus = true;
            NetworkDisconnectStatus = false;

            nwy_test_cli_echo("\r\n *** From Data Callback Function *** \r\n");
            nwy_test_cli_data_info_new();
        }
        current_open_dial_action[profile_idx-1] = ind_state;
        is_trigger_by_app[profile_idx-1] = 0;
    }
}

void nwy_test_cli_set_profile_new()
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_profile_info_t profile_info;
    memset(&profile_info, 0, sizeof(nwy_data_profile_info_t));

    int sim_id = 0; // Hardcoded SIM ID
    int profile_id = 1; // Hardcoded Profile ID
    int auth_proto = 0; // 0: NONE
    int pdp_type = 1; // 1: IPV4

    // Assigning hardcoded values
    profile_info.auth_proto = (nwy_data_auth_proto_e)auth_proto;
    profile_info.pdp_type = (nwy_data_pdp_type_e)pdp_type;
    strncpy(profile_info.apn, "airtelgprs.com", sizeof(profile_info.apn));
    strncpy(profile_info.user_name, "", sizeof(profile_info.user_name));
    strncpy(profile_info.pwd, "", sizeof(profile_info.pwd));

    // Logging the configuration steps
    ret = nwy_data_profile_set(sim_id, profile_id, &profile_info);
    NWY_CLI_LOG("=DATA=  nwy_data_set_profile ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nSet profile %d info fail, result<%d>\r\n", profile_id, ret);
    else
        nwy_test_cli_echo("\r\nSet profile %d info success\r\n", profile_id);

}

void nwy_test_cli_data_start_new()
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_start_call_t param_t = {0};
    int pre_open_dial_action = 0;

    int sim_id = 0;            // Hardcoded SIM ID
    int profile_id = 1;        // Hardcoded Profile ID
    int is_auto_recon = 0;     // Hardcoded auto-connect (0 = disabled)

    param_t.cid = profile_id;
    param_t.call_auto_type = is_auto_recon;
    param_t.action = NWY_DATA_CALL_ACT;
    param_t.trigger_type = NWY_DATA_TRIGGER_OPEN;
    param_t.if_internal_call = 1;

    // Use minimum/default placeholders; actual profile already configured
    param_t.set_profile.pdp_type = NWY_DATA_PDP_TYPE_MIN;
    param_t.set_profile.auth_proto = NWY_DATA_AUTH_PROTO_MIN;

    // Step 1: Register data callback
    ret = nwy_data_reg_cb(sim_id, profile_id, nwy_test_cli_data_cb_fun);
    NWY_CLI_LOG("=DATA=  nwy_data_reg_cb ret= %d", ret);

    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nStart data call fail, result<%d>\r\n", ret);
        return;
    }

    // Step 2: Backup current state and set open dial action
    pre_open_dial_action = current_open_dial_action[profile_id - 1];
    current_open_dial_action[profile_id - 1] = 1;

    // Step 3: Start the data call
    ret = nwy_data_call_start(sim_id, &param_t);
    NWY_CLI_LOG("=DATA=  nwy_data_call_start ret= %d", ret);

    if (ret == NWY_GEN_E_AREADY_CONNECT)
    {
        nwy_test_cli_echo("\r\nData call in cid %d already connected\r\n", profile_id);
        return;
    }

    if (ret != NWY_SUCCESS)
    {
        current_open_dial_action[profile_id - 1] = pre_open_dial_action;
        nwy_data_unreg_cb(sim_id, profile_id, nwy_test_cli_data_cb_fun);
        nwy_test_cli_echo("\r\nStart data call fail, result<%d>\r\n", ret);
    }
    else
    {
        is_trigger_by_app[profile_id - 1] = 1;
        nwy_test_cli_echo("\r\nStart data call ...\r\n");
    }
}

void nwy_test_cli_data_info_new()
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_callinfo_t addr_info = {0};

    int profile_id = 1; // Hardcoded Profile ID (CID)

    memset(&addr_info, 0, sizeof(nwy_data_callinfo_t));
    NWY_CLI_LOG("=DATA=  addr_info size= %d", sizeof(nwy_data_callinfo_t));

    ret = nwy_data_call_info_get(profile_id, &addr_info);
    NWY_CLI_LOG("=DATA=  nwy_data_call_info_get = %d", ret);

    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet data info fail, result<%d>\r\n", ret);
        return;
    }

    // Simulated / expected CLI Output based on log
    nwy_test_cli_echo("\r\nstatus:%d\r\n", addr_info.state);                  // 1
    nwy_test_cli_echo("\r\npdp_type:%d\r\n", addr_info.profile.pdp_type);    // 1
    nwy_test_cli_echo("\r\nipv4:%s\r\n", addr_info.ipv4_str);                // 10.25.158.170
    nwy_test_cli_echo("\r\nipv6:%s\r\n", addr_info.ipv6_str);                // (empty)
}
/**************************CUSTOM CODE*********************************/
void nwy_test_cli_get_profile()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_profile_info_t profile_info;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile index: (1-7)");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    memset(&profile_info, 0, sizeof(nwy_data_profile_info_t));
    ret = nwy_data_profile_get(sim_id, profile_id, &profile_info);
    NWY_CLI_LOG("=DATA=  nwy_data_get_profile ret= %d", ret);
    NWY_CLI_LOG("=DATA=  profile= %d|%d", profile_info.pdp_type, profile_info.auth_proto);
	
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nRead profile %d info fail, result%d\r\n", profile_id, ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nProfile %d info: <pdp_type>,<auth_proto>,<apn>,<user_name>,<password>\r\n%d,%d,%s,%s,%s\r\n", profile_id, profile_info.pdp_type,
                          profile_info.auth_proto, profile_info.apn, profile_info.user_name, profile_info.pwd);
    }
}

void nwy_test_cli_set_profile()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_profile_info_t profile_info;

    nwy_data_capability_rsp_t rsp;
    int range_min =0;
    int range_max = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    memset(&profile_info, 0, sizeof(nwy_data_profile_info_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    //get auth type range 
    memset(&rsp,0,sizeof(rsp));
    if(NWY_SUCCESS == nwy_data_get_capability(REQ_AUTH_TYPE_RANGE, &rsp))
    {
       range_min = rsp.data.range.min;
       range_max = rsp.data.range.max;
    }else
    {
        range_min = 0;
        range_max = 2;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: auth_proto <%d-%d>\r\n0:NONE, 1:PAP, 2:CHAP\r\n",range_min,range_max);
    int auth_proto = atoi(sptr);
    if ((auth_proto < range_min) || (auth_proto > range_max))
    {
        nwy_test_cli_echo("\r\nInvaild auth_proto value: %d\r\n", auth_proto);
        return;
    }
    profile_info.auth_proto = (nwy_data_auth_proto_e)auth_proto;

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: pdp_type <1-3,6>\r\n1:IPV4, 2:IPV6, 3:IPV4V6\r\n");
    int pdp_type = atoi(sptr);
    if ((pdp_type != 1) && (pdp_type != 2) && (pdp_type != 3))
    {
        nwy_test_cli_echo("\r\nInvaild pdp_type value: %d\r\n", pdp_type);
        return;
    }
    profile_info.pdp_type = (nwy_data_pdp_type_e)pdp_type;

    //!!! attention : max apn length is 50 in asr chipset
    //get apn length range 
    memset(&rsp,0,sizeof(rsp));
    if(NWY_SUCCESS == nwy_data_get_capability(REQ_APN_MAX_LENGTH, &rsp))
    {
       range_min = rsp.data.range.min;
       range_max = rsp.data.range.max;
    }else
    {
        range_min = 0;
        range_max = 64;
    }    
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: apn (length %d-%d)\r\n",range_min,range_max);
    if (strlen(sptr) > range_max)
    {
        nwy_test_cli_echo("\r\nInvaild apn len\r\n");
        return;
    }
    memcpy(profile_info.apn, sptr, sizeof(profile_info.apn));
    //!!! attention : max user_name length is 50 in asr chipset
    memset(&rsp,0,sizeof(rsp));
    if(NWY_SUCCESS == nwy_data_get_capability(REQ_USERNAME_MAX_LENGTH, &rsp))
    {
       range_min = rsp.data.range.min;
       range_max = rsp.data.range.max;
    }else
    {
        range_min = 0;
        range_max = 64;
    }    
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: user name (length %d-%d)\r\n", range_min,range_max);
    if (strlen(sptr) > range_max)
    {
        nwy_test_cli_echo("\r\nInvaild user name len\r\n");
        return;
    }
    memcpy(profile_info.user_name, sptr, sizeof(profile_info.user_name));

    //!!! attention : max password length is 50 in asr chipset
    memset(&rsp,0,sizeof(rsp));
    if(NWY_SUCCESS == nwy_data_get_capability(REQ_PASSWORD_MAX_LENGTH, &rsp))
    {
       range_min = rsp.data.range.min;
       range_max = rsp.data.range.max;
    }else
    {
        range_min = 0;
        range_max = 64;
    }    
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: password(length %d-%d)\r\n", range_min,range_max);
    if (strlen(sptr) > range_max)
    {
        nwy_test_cli_echo("\r\nInvaild password len\r\n");
        return;
    }
    memcpy(profile_info.pwd, sptr, sizeof(profile_info.pwd));

    ret = nwy_data_profile_set(sim_id, profile_id, &profile_info);
    NWY_CLI_LOG("=DATA=  nwy_data_set_profile ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nSet profile %d info fail, result<%d>\r\n", profile_id, ret);
    else
        nwy_test_cli_echo("\r\nSet profile %d info success\r\n", profile_id);
}

void nwy_test_cli_auto_connect_set()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    char *token = NULL;
    int i = 0;
    nwy_data_auto_connect_cfg_t cfg = {0};
    sptr = nwy_test_cli_input_gets("\r\nPlease select profile: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
    }
    memset(&cfg, 0x00, sizeof(nwy_data_auto_connect_cfg_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input max retry num: (1-65535)");
    cfg.retry_max = atoi(sptr);
    if ((cfg.retry_max < 1) || (cfg.retry_max > 65535))
    {
        nwy_test_cli_echo("\r\nInvaild max retry time: %d\r\n", cfg.retry_max);
        return;
    }
    nwy_test_cli_echo("\r\nPlease input time intervals,unit is 1s:");
    nwy_test_cli_echo("\r\n(Format:value_1,value_2...value_n,0<n<17,0<value_n<256)");
    sptr = nwy_test_cli_input_gets("\r\nValue_1 is must required,others are optional:");
    token = strtok(sptr, ",");
    while (token != NULL && i < 16) {
            cfg.interval[i] = atoi(token);
            if((cfg.interval[i] < 1) || (cfg.interval[i] > 255))
            {
                nwy_test_cli_echo("\r\nInvaild time interval value%d: %s\r\n",i,token);
                return;
            }
            i++;
            token = strtok(NULL, ",");
    }


    ret = nwy_data_auto_connect_set(profile_id,&cfg);
    NWY_CLI_LOG("=DATA=  nwy_data_auto_connect_set ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nSet auto connect info fail, result<%d>\r\n", ret);
    else
        nwy_test_cli_echo("\r\nSet auto connect info success\r\n");
}

void nwy_test_cli_data_start()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_start_call_t param_t = {0};
    int pre_open_dial_action = 0 ;
    memset(&param_t, 0, sizeof(nwy_data_start_call_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease select profile: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    param_t.cid = profile_id;
    sptr = nwy_test_cli_input_gets("\r\nPlease set auto_connect: 0 Disable, 1 Enable");
    int is_auto_recon = atoi(sptr);
    if ((is_auto_recon != 0) && (is_auto_recon != 1))
    {
        nwy_test_cli_echo("\r\nInvaild auto_connect: %d\r\n", is_auto_recon);
        return;
    }
    param_t.cid = profile_id;
    param_t.call_auto_type = is_auto_recon;
    param_t.action = NWY_DATA_CALL_ACT;
    param_t.trigger_type = NWY_DATA_TRIGGER_OPEN;
    param_t.if_internal_call = 1;
    param_t.set_profile.pdp_type = NWY_DATA_PDP_TYPE_MIN ;
    param_t.set_profile.auth_proto = NWY_DATA_AUTH_PROTO_MIN ;    
    
    ret = nwy_data_reg_cb(sim_id, profile_id, nwy_test_cli_data_cb_fun);
    NWY_CLI_LOG("=DATA=  nwy_data_reg_cb ret= %d", ret);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nStart data call fail, result<%d>\r\n", ret);
        return ;
    }
    pre_open_dial_action = current_open_dial_action[profile_id-1];
    current_open_dial_action[profile_id-1]= 1;
    ret = nwy_data_call_start(sim_id, &param_t);
    NWY_CLI_LOG("=DATA=  nwy_data_call_start ret= %d", ret);
    if(ret == NWY_GEN_E_AREADY_CONNECT)
    {
        nwy_test_cli_echo("\r\n Data call in cid %d already connected\r\n",profile_id);
        return;
    }
    
    if (ret != NWY_SUCCESS)
    {
        current_open_dial_action[profile_id-1] = pre_open_dial_action;
        ret = nwy_data_unreg_cb(sim_id, profile_id, nwy_test_cli_data_cb_fun);
        nwy_test_cli_echo("\r\nStart data call fail, result<%d>\r\n", ret);
    }
    else
    {
        is_trigger_by_app[profile_id-1] = 1;
        nwy_test_cli_echo("\r\nStart data call ...\r\n");
    }
}

void nwy_test_cli_data_info()
{
    char *sptr = NULL;
    int ret = NWY_GEN_E_UNKNOWN;
    //uint8 *v4_ip = NULL;
    //uint8 *v6_ip = NULL;
    nwy_data_callinfo_t addr_info = {0};
    sptr = nwy_test_cli_input_gets("\r\nPlease select profile: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }

    memset(&addr_info, 0, sizeof(nwy_data_callinfo_t));
    NWY_CLI_LOG("=DATA=  addr_info size= %d", sizeof(nwy_data_callinfo_t));
    ret = nwy_data_call_info_get(profile_id, &addr_info);
    NWY_CLI_LOG("=DATA=  nwy_data_call_info_get = %d", ret);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet data info fail, result<%d>\r\n", ret);
        return;
    }

    nwy_test_cli_echo("\r\nstatus:%d\r\n", addr_info.state);
    nwy_test_cli_echo("\r\npdp_type:%d\r\n", addr_info.profile.pdp_type);
    nwy_test_cli_echo("\r\nipv4:%s\r\n", addr_info.ipv4_str);
    nwy_test_cli_echo("\r\nipv6:%s\r\n", addr_info.ipv6_str);
}

void nwy_test_cli_data_stop_fixed(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_start_call_t param = {0};
    int pre_open_dial_action = 0;

    // Hardcoded values
    int sim_id = 0;
    int profile_id = 1;
    int is_auto_recon = 0;

    nwy_data_sim_id = sim_id;

    pre_open_dial_action = current_open_dial_action[profile_id - 1];
    current_open_dial_action[profile_id - 1] = 0;

    param.cid = profile_id;
    param.call_auto_type = is_auto_recon;

    ret = nwy_data_call_stop(sim_id, &param);

    NWY_CLI_LOG("=DATA=  nwy_data_call_stop ret= %d", ret);

    if (ret == NWY_GEN_E_AREADY_DISCONNECT)
    {
        nwy_test_cli_echo("\r\nData call in cid %d already disconnected\r\n", profile_id);
        return;
    }

    if (ret != NWY_SUCCESS)
    {
        current_open_dial_action[profile_id - 1] = pre_open_dial_action;
        nwy_test_cli_echo("\r\nStop data call fail, result<%d>\r\n", ret);
        return;
    }

    is_trigger_by_app[profile_id - 1] = 1;
    nwy_test_cli_echo("\r\nStop data call ...\r\n");
}


void nwy_test_cli_data_stop()
{
    char *sptr = NULL;
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_start_call_t param = {0};
    int pre_open_dial_action = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    nwy_data_sim_id = sim_id;
    sptr = nwy_test_cli_input_gets("\r\nPlease select profile: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease set auto_connect: 0 Disable, 1 Enable");
    int is_auto_recon = atoi(sptr);
    if ((is_auto_recon != 0) && (is_auto_recon != 1))
    {
        nwy_test_cli_echo("\r\nInvaild auto_connect: %d\r\n", is_auto_recon);
        return;
    }
    pre_open_dial_action = current_open_dial_action[profile_id-1];

    current_open_dial_action[profile_id-1]= 0;
    param.cid = profile_id;
    param.call_auto_type = is_auto_recon;

    ret = nwy_data_call_stop(sim_id,&param);

    NWY_CLI_LOG("=DATA=  nwy_data_call_stop ret= %d", ret);
    if(ret == NWY_GEN_E_AREADY_DISCONNECT)
    {
        nwy_test_cli_echo("\r\n Data call in cid %d already disconnected\r\n",profile_id);
        return;
    }
    
    if (ret != NWY_SUCCESS)
    {
        current_open_dial_action[profile_id-1] = pre_open_dial_action;
        nwy_test_cli_echo("\r\nStop data call fail, result<%d>\r\n", ret);
        return ;
    }
    is_trigger_by_app[profile_id-1] = 1;
    nwy_test_cli_echo("\r\nStop data call ...\r\n");

}

void nwy_test_cli_get_dhcp_info()
{
    int ret = NWY_GEN_E_UNKNOWN;
    
    nwy_data_dhcp_t dhcp_info;

    memset(&dhcp_info, 0, sizeof(nwy_data_dhcp_t));    
    ret = nwy_data_dhcp_get(&dhcp_info);
    NWY_CLI_LOG("=DATA=  nwy_data_get_dhcp_info ret= %d", ret);
    NWY_CLI_LOG("=DATA=  dhcp_info= %s|%s", dhcp_info.gw, dhcp_info.netmask);
    
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet dhcp config fail, result %d\r\n", ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nDhcp config info: <gw>,<netmask>,<start_ip>,<end_ip>,<lease_time>\r\n%s,%s,%s,%s,%d\r\n",dhcp_info.gw, dhcp_info.netmask,
                          dhcp_info.start_ip, dhcp_info.end_ip, dhcp_info.lease_time);
    }
}

void nwy_test_cli_set_dhcp_info()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    
    nwy_data_dhcp_t dhcp_info;
    memset(&dhcp_info, 0, sizeof(nwy_data_dhcp_t));    

    sptr = nwy_test_cli_input_gets("\r\nPlease input dhcp gw info(length 7-15),ipv4 string format\r\n");
    if (strlen(sptr) > 15 || strlen(sptr) < 7 )
    {
        nwy_test_cli_echo("\r\nInvaild dhcp gw len\r\n");
        return;
    }
    memcpy(dhcp_info.gw, sptr, sizeof(dhcp_info.gw));


    sptr = nwy_test_cli_input_gets("\r\nPlease input dhcp netmask info(length 7-15),ipv4 string format\r\n");
    if (strlen(sptr) > 15 || strlen(sptr) < 7 )
    {
        nwy_test_cli_echo("\r\nInvaild dhcp netmask len\r\n");
        return;
    }
    memcpy(dhcp_info.netmask, sptr, sizeof(dhcp_info.netmask));

    sptr = nwy_test_cli_input_gets("\r\nPlease input dhcp start_ip info(length 7-15),ipv4 string format\r\n");
    if (strlen(sptr) > 15 || strlen(sptr) < 7 )
    {
        nwy_test_cli_echo("\r\nInvaild dhcp start_ip len\r\n");
        return;
    }
    memcpy(dhcp_info.start_ip, sptr, sizeof(dhcp_info.start_ip));

    sptr = nwy_test_cli_input_gets("\r\nPlease input dhcp end_ip info(length 7-15),ipv4 string format\r\n");
    if (strlen(sptr) > 15 || strlen(sptr) < 7 )
    {
        nwy_test_cli_echo("\r\nInvaild dhcp end_ip len\r\n");
        return;
    }
    memcpy(dhcp_info.end_ip, sptr, sizeof(dhcp_info.end_ip));


    sptr = nwy_test_cli_input_gets("\r\nPlease input dhcp lease time,max value is 4294967294,ASR not support it\r\n");
    unsigned int lease_time = atoi(sptr);
    if(lease_time> 4294967294)
    {
        nwy_test_cli_echo("\r\nInvaild dhcp lease time value\r\n");
        return;
    }

    dhcp_info.lease_time = 0;

    ret = nwy_data_dhcp_set(&dhcp_info);
    NWY_CLI_LOG("=DATA=  nwy_data_set_dhcp ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nSet dhcp config fail, result<%d>\r\n", ret);
    else
        nwy_test_cli_echo("\r\nSet dhcp config success\r\n");
}

void nwy_test_cli_get_folw_info()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_flow_t flow_info;
    int clear_flag = 0;

    
    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile index: (1-7)");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input clear flag: (0 not clear,1 clear)");
    clear_flag = atoi(sptr);
    if ((clear_flag < 0) || (clear_flag > 1))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", clear_flag);
        return;
    }

    memset(&flow_info, 0, sizeof(nwy_data_flow_t));

    flow_info.cid = profile_id;

    ret = nwy_data_flow_info_get(sim_id, profile_id, &flow_info,clear_flag);
    NWY_CLI_LOG("=DATA=  nwy_data_get_profile ret= %d", ret);
    NWY_CLI_LOG("=DATA=  flow_info= %d tx packet %lld| rx packet %lld", flow_info.cid,flow_info.tx, flow_info.rx);
    
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet flow_info cid %d fail, result%d\r\n", profile_id, ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nGet flow_info: <cid>,<tx_packet>,<rx_packet>\r\n%d,%lld,%lld\r\n", flow_info.cid, flow_info.tx,flow_info.rx);
    }
}

void nwy_test_cli_get_eps_apn()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    char apn[100] ={0};
    nwy_data_pdp_type_e ip_type = NWY_DATA_PDP_TYPE_IPV4V6;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    ret = nwy_data_eps_dyn_apn_get(sim_id, &ip_type,apn);
    NWY_CLI_LOG("=DATA=  ip_type %d apn %s", ip_type, apn);
    NWY_CLI_LOG("=DATA=  nwy_test_cli_get_eps_apn ret= %d", ret);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nRead eps apn info fail, result%d\r\n", ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nEps apn info: pdp_type:%d(1-IPV4,2-IPV6,3-IPV4V6),apn:%s\r\n", ip_type, apn);
    }
}


void nwy_test_cli_get_dns_info()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    char dns1Str[NWY_DATA_MAX_DNS_LEN] = {0};
    char dns2Str[NWY_DATA_MAX_DNS_LEN] = {0};
    char v6_dns1Str[NWY_DATA_MAX_DNS_LEN] = {0};
    char v6_dns2Str[NWY_DATA_MAX_DNS_LEN] = {0};

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim id: (0-1)");
    int sim_id = atoi(sptr);
    if ((sim_id != 0) && (sim_id != 1))
    {
        nwy_test_cli_echo("\r\nInvaild sim id: %d\r\n", sim_id);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile index: (1-7)");
    
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }

    ret = nwy_data_dns_info_get(sim_id,profile_id,dns1Str,dns2Str,v6_dns1Str,v6_dns2Str);
    
    NWY_CLI_LOG("=DATA=  ipv4 dns pri %s sec %s",dns1Str,dns2Str);
    NWY_CLI_LOG("=DATA=  ipv6 dns pri %s sec %s",v6_dns1Str,v6_dns2Str);

    if (ret == NWY_GEN_E_GET_FAILED)
    {
        nwy_test_cli_echo("\r\nGet dns info failed,please ensure data activated before getting dns info\r\n");
        return;
    }
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet dns info failed, result%d\r\n", ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nGet dns info cid %d successfully:\r\n", profile_id);
        nwy_test_cli_echo("\r\nIpv4 dns pri %s sec %s\r\n", dns1Str, dns2Str);
        nwy_test_cli_echo("\r\nIpv6 dns pri %s sec %s\r\n", v6_dns1Str, v6_dns2Str);
    }
}


