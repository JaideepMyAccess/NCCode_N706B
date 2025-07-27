#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

nwy_sms_recv_info_type_t sms_data = {0};
extern unsigned int nwy_test_del_sms_tab[NWY_OPEN_DEL_SMS_NUM];

/**************************SIM*********************************/
void nwy_test_cli_get_sim_status()
{
    nwy_sim_status sim_status = NWY_SIM_STATUS_GET_ERROR;
    char *sptr = NULL;
    nwy_sim_id_t simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    sim_status = nwy_sim_get_card_status(simid);
    switch (sim_status)
    {
    case NWY_SIM_STATUS_READY:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_READY!\r\n");
        break;
    case NWY_SIM_STATUS_NOT_INSERT:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_NOT_INSERT!\r\n");
        break;
    case NWY_SIM_STATUS_PIN1:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_PIN1!\r\n");
        break;
    case NWY_SIM_STATUS_PUK1:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_PUK1!\r\n");
        break;
    case NWY_SIM_STATUS_BUSY:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_BUSY!\r\n");
        break;
    default:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_GET_ERROR!\r\n");
        break;
    }

    return;
}

void nwy_test_cli_verify_pin()
{
    char *sptr = NULL;
    char pin[10] = {0};
    nwy_sim_id_t simid = NWY_SIM_ID_MAX;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input pin string: ");
    memcpy(pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_verify_pin simid =%d,  pin=%s\r\n", simid, pin);

    ret = nwy_sim_verify_pin(simid, pin);
    if (0 != ret)
        nwy_test_cli_echo("\r\nnwy verify pin fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy verify pin success!\r\n");

    return;
}

void nwy_test_cli_get_pin_mode()
{
#ifdef NWY_OPEN_GET_PINMODE_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sptr = NULL;
    nwy_pin_mode_type pin_mode = NWY_SIM_PIN_MAX;
    nwy_sim_id_t simid = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    pin_mode = nwy_sim_get_pin_mode(simid);
    if (NWY_SIM_PIN_MAX != pin_mode)
        nwy_test_cli_echo("\r\npin_mode: %d\r\n", pin_mode);
    else
        nwy_test_cli_echo("\r\nget pin mode fail!\r\n");
#endif
    return;
}

void nwy_test_cli_set_pin_mode()
{
    char *sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    char pin[12] = {0};
    int action = 0;
    int simid = 0;
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input pin action:(0-disable/1-enable) ");
    action = atoi(sptr);
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input pin string: ");
    memcpy(pin, sptr, strlen(sptr));
    
    nwy_test_cli_echo("\r\nnwy_test_cli_set_pin_mode simid =%d, action=%d, pin=%s\r\n", simid, action, pin);
    if (action == 0) {
        result = nwy_sim_disable_pin(simid, pin);
    } else if (action == 1) {
        result = nwy_sim_enable_pin(simid, pin);
    }

    if (result == NWY_SUCESS) {
        nwy_test_cli_echo("\r\nset pin mode success!\r\n");
    } else {
        nwy_test_cli_echo("\r\nset pin mode fail!\r\n");
    }

    return;


}

void nwy_test_cli_change_pin()
{
    char *sptr = NULL;
    char pin[10] = {0};
    
    char new_pin[10] = {0};
    nwy_sim_id_t simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input old pin string: ");
    memcpy(pin, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input new pin string: ");
    memcpy(new_pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_change_pin simid =%d, pin=%s, new_pin=%s\r\n", simid, pin, new_pin);

    ret = nwy_sim_change_pin(simid, pin, new_pin);
    if (0 != ret)
        nwy_test_cli_echo("\r\nnwy change pin fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy change pin success!\r\n");

    return;

}

void nwy_test_cli_verify_puk()
{
    char *sptr = NULL;
    char puk[10] = {0};
    char new_pin[10] = {0};
    nwy_sim_id_t simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input puk string: ");
    memcpy(puk, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input new pin string: ");
    memcpy(new_pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_verify_puk  simid =%d, puk=%d, new_pin=%s\r\n", simid, puk, new_pin);

    ret = nwy_sim_unblock(simid, puk, new_pin);
    if (0 != ret)
        nwy_test_cli_echo("\r\nnwy verify puk fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy verify puk success!\r\n");

    return;
}

void nwy_test_cli_get_imsi()
{
    int result = NWY_GEN_E_UNKNOWN;
    char imsi[21] = {0};
    char *sptr = NULL;
    nwy_sim_id_t simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    result = nwy_sim_get_imsi(simid, imsi, sizeof(imsi));
    if (NWY_SUCESS == result)
        nwy_test_cli_echo("\r\nimsi: %s\r\n", imsi);
    else
        nwy_test_cli_echo("\r\nget imsi fail!!\r\n");

    return;
}

void nwy_test_cli_get_iccid()
{
    int result = NWY_GEN_E_UNKNOWN;
    char iccid[21] = {0};
    char *sptr = NULL;
    nwy_sim_id_t simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    result = nwy_sim_get_iccid(simid, iccid, sizeof(iccid));
    NWY_CLI_LOG("++++ nwy_test_cli_get_iccid %d %s ++++++", result, iccid);
    if (NWY_SUCESS == result)
    {
        nwy_sleep(1000);
        nwy_test_cli_echo("\r\niccid: %s\r\n", iccid);
    }
    else
        nwy_test_cli_echo("\r\nget sim iccid fail!!\r\n");

    return;
}

void nwy_test_cli_get_msisdn()
{
#ifdef NWY_OPEN_GET_MSISDN_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sptr = NULL;
    char msisdn[128] = {0};
    int ret = -1;
    nwy_sim_id_t simid;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);
    ret = nwy_sim_get_msisdn(simid, msisdn, 128);
    if (0 != ret)
        nwy_test_cli_echo("\r\nget msisdn fail!!\r\n");
    else
        nwy_test_cli_echo("\r\nmsisdn: %s\r\n", msisdn);
#endif
    return;
}

void nwy_test_cli_set_msisdn()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_get_pin_puk_times()
{
#ifdef NWY_OPEN_TEST_PINPUK_TIMES_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sptr = NULL;
    uint8 pin_time = 0;
    uint8 puk_time = 0;

    nwy_sim_id_t simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_t)atoi(sptr);

    ret = nwy_sim_get_pin_puk_retry_times(simid, &pin_time, &puk_time);
    if (0 != ret)
        nwy_test_cli_echo("\r\nnwy get retry time  fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy get retry time success. pin:%d,puk:%d!\r\n", pin_time, puk_time);

    return;
#endif
}

void nwy_test_cli_get_sim_slot()
{
#ifdef NWY_OPEN_TEST_SIM_SWTICH
    uint8 nSwitchSimID = 0;

    nSwitchSimID = nwy_sim_get_simid();

    nwy_test_cli_echo("\r\n simid: %d \r\n", nSwitchSimID);

    return;
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#endif
}

void nwy_test_cli_set_sim_slot()
{
#ifdef NWY_OPEN_TEST_SIM_SWTICH
    char *sptr = NULL;
    uint8 nSwitchSimID = 0;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input switch simid(0-1): ");
    nSwitchSimID = atoi(sptr);

    ret = nwy_sim_set_simid(nSwitchSimID);
    if (0 != ret)
        nwy_test_cli_echo("\r\nnwy set switch simid fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set switch simid success!\r\n");
    return;

#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#endif
}


void nwy_test_cli_set_csim()
{
#ifdef NWY_OPEN_TEST_CSIM_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#else
    char *sptr = NULL;
    uint8 length = 0;
    nwy_sim_id_t simid = 0;
    char csim_cmd[512] = {0};
    char csim_resp[512] = {0};
    int ret = NWY_RES_OK;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input data len: ");
    length = atoi(sptr);
    if ((length > 520) || (length < 10) || (length % 2)) {
        nwy_test_cli_echo("\r\ninput len invalid!\r\n");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input command string: ");
    if (strlen(sptr) != length) {
        nwy_test_cli_echo("\r\ninput csim cmd invalid!\r\n");
        return;
    }
    memcpy(csim_cmd, sptr, strlen(sptr));

    ret = nwy_sim_csim(simid, csim_cmd, length, csim_resp, sizeof(csim_resp));

    if (NWY_RES_OK != ret)
        nwy_test_cli_echo("\r\nnwy set csim fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set csim success resp:%s\r\n", csim_resp);

    return;
#endif
}

/**************************DATA*********************************/
static int ppp_state[10] = {0};
static void nwy_test_cli_data_cb_fun(int hndl,nwy_data_call_state_t ind_state)
{
    NWY_CLI_LOG("=DATA= hndl=%d,ind_state=%d", hndl, ind_state);
    if (hndl > 0 && hndl <= 8)
    {
        ppp_state[hndl - 1] = ind_state;
        nwy_test_cli_echo("\r\nData call status update, handle_id:%d,state:%d\r\n", hndl, ind_state);
    }
}

int nwy_test_cli_check_data_connect()
{
    int i = 0;
    for (i = 0; i < NWY_DATA_CALL_MAX_NUM; i++)
    {
        if (ppp_state[i] == NWY_DATA_CALL_CONNECTED)
        {
            return 1;
        }
    }
    return 0;
}

void nwy_test_cli_data_create()
{
    int hndl = 0;
    hndl = nwy_data_get_srv_handle(nwy_test_cli_data_cb_fun);
    if (hndl > 0)
    {
        nwy_test_cli_echo("\r\nCreate a Resource Handle id: %d success\r\n", hndl);
    }
    else
    {
        nwy_test_cli_echo("\r\nCreate a Resource Handle id: %d fail\r\n", hndl);
    }
    NWY_CLI_LOG("=DATA=  hndl= %d", hndl);
}

void nwy_test_cli_get_profile()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_profile_info_t profile_info;

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile index: (1-7)");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    memset(&profile_info, 0, sizeof(nwy_data_profile_info_t));
    ret = nwy_data_get_profile(profile_id, NWY_DATA_PROFILE_3GPP, &profile_info);
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

    memset(&profile_info, 0, sizeof(nwy_data_profile_info_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: auth_proto <0-2>\r\n0:NONE, 1:PAP, 2:CHAP\r\n");
    int auth_proto = atoi(sptr);
    if ((auth_proto < 0) || (auth_proto > 2))
    {
        nwy_test_cli_echo("\r\nInvaild auth_proto value: %d\r\n", auth_proto);
        return;
    }
    profile_info.auth_proto = (nwy_data_auth_proto_t)auth_proto;

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: pdp_type <1-3,6>\r\n1:IPV4, 2:IPV6, 3:IPV4V6\r\n");
    int pdp_type = atoi(sptr);
    if ((pdp_type != 1) && (pdp_type != 2) && (pdp_type != 3))
    {
        nwy_test_cli_echo("\r\nInvaild pdp_type value: %d\r\n", pdp_type);
        return;
    }
    profile_info.pdp_type = (nwy_data_pdp_type_t)pdp_type;

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: apn (length 0-%d)\r\n", NWY_APN_MAX_LEN);
    if (strlen(sptr) > NWY_APN_MAX_LEN)
    {
        nwy_test_cli_echo("\r\nInvaild apn len\r\n");
        return;
    }
    memcpy(profile_info.apn, sptr, sizeof(profile_info.apn));

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: user name (length 0-%d)\r\n", NWY_APN_USER_MAX_LEN);
    if (strlen(sptr) > NWY_APN_USER_MAX_LEN)
    {
        nwy_test_cli_echo("\r\nInvaild user name len\r\n");
        return;
    }
    memcpy(profile_info.user_name, sptr, sizeof(profile_info.user_name));

    sptr = nwy_test_cli_input_gets("\r\nPlease input profile info: password(length 0-%d)\r\n", NWY_APN_PWD_MAX_LEN);
    if (strlen(sptr) > NWY_APN_PWD_MAX_LEN)
    {
        nwy_test_cli_echo("\r\nInvaild password len\r\n");
        return;
    }
    memcpy(profile_info.pwd, sptr, sizeof(profile_info.pwd));

    ret = nwy_data_set_profile(profile_id, NWY_DATA_PROFILE_3GPP, &profile_info);
    NWY_CLI_LOG("=DATA=  nwy_data_set_profile ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nSet profile %d info fail, result<%d>\r\n", profile_id, ret);
    else
        nwy_test_cli_echo("\r\nSet profile %d info success\r\n", profile_id);
}

void nwy_test_cli_data_start()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    nwy_data_start_call_v02_t param_t;
    memset(&param_t, 0, sizeof(nwy_data_start_call_v02_t));

    sptr = nwy_test_cli_input_gets("\r\nPlease select resource handle id <1-%d>", NWY_DATA_CALL_MAX_NUM);
    int hndl = atoi(sptr);
    if ((hndl <= 0) || (hndl > NWY_DATA_CALL_MAX_NUM))
    {
        nwy_test_cli_echo("\r\nInvaild resource handle id: %d\r\n", hndl);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease select profile: profile_id <1-7>");
    int profile_id = atoi(sptr);
    if ((profile_id <= 0) || (profile_id > 7))
    {
        nwy_test_cli_echo("\r\nInvaild profile id: %d\r\n", profile_id);
        return;
    }
    param_t.profile_idx = profile_id;

    /* Begin: Add by YJJ for support auto re_connect in 2020.05.20*/
    sptr = nwy_test_cli_input_gets("\r\nPlease set auto_connect: 0 Disable, 1 Enable");
    int is_auto_recon = atoi(sptr);
    if ((is_auto_recon != 0) && (is_auto_recon != 1))
    {
        nwy_test_cli_echo("\r\nInvaild auto_connect: %d\r\n", is_auto_recon);
        return;
    }
    param_t.is_auto_recon = is_auto_recon;

    if (is_auto_recon == 1)
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease set auto_connect maximum times: 0 Always Re_connect, [1-65535] maximum_times");
        int auto_re_max = atoi(sptr);
        if ((auto_re_max < 0) || (auto_re_max > 65535))
        {
            nwy_test_cli_echo("\r\nInvaild auto_connect maximum times : %d\r\n", auto_re_max);
            return;
        }
        param_t.auto_re_max = auto_re_max;

        sptr = nwy_test_cli_input_gets("\r\nPlease set auto_connect interval (ms): [60000,86400000]");
        int auto_interval_ms = atoi(sptr);
        if ((auto_interval_ms < 60000) || (auto_interval_ms > 86400000))
        {
            nwy_test_cli_echo("\r\nInvaild auto_connect interval: %d\r\n", auto_interval_ms);
            return;
        }
        param_t.auto_interval_ms = auto_interval_ms;
    }

    /* End: Add by YJJ for auto re_connect in 2020.05.20*/

    ret = nwy_data_start_call(hndl, &param_t);
    NWY_CLI_LOG("=DATA=  nwy_data_start_call ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nStart data call fail, result<%d>\r\n", ret);
    else
        nwy_test_cli_echo("\r\nStart data call ...\r\n");
}

void nwy_test_cli_data_info()
{
    char *sptr = NULL;
    int ret = NWY_GEN_E_UNKNOWN;
    int len = 0;
    nwy_data_addr_t_info addr_info;

    sptr = nwy_test_cli_input_gets("\r\nPlease select resource handle id <1-%d>", NWY_DATA_CALL_MAX_NUM);
    int hndl = atoi(sptr);
    if ((hndl <= 0) || (hndl > NWY_DATA_CALL_MAX_NUM))
    {
        nwy_test_cli_echo("\r\nInvaild resource handle id: %d\r\n", hndl);
        return;
    }

    memset(&addr_info, 0, sizeof(nwy_data_addr_t_info));
    NWY_CLI_LOG("=DATA=  addr_info size= %d", sizeof(nwy_data_addr_t_info));
    ret = nwy_data_get_ip_addr(hndl, &addr_info, &len);
    NWY_CLI_LOG("=DATA=  nwy_data_get_ip_addr = %d|len%d", ret, len);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGet data info fail, result<%d>\r\n", ret);
        return;
    }
    nwy_test_cli_echo("\r\nGet data info success\r\nIface address: %s,%s\r\n",
                      nwy_ip4addr_ntoa(&addr_info.iface_addr_s.ip_addr),
                      nwy_ip6addr_ntoa(&addr_info.iface_addr_s.ip6_addr));
    nwy_test_cli_echo("Dnsp address: %s,%s\r\n",
                      nwy_ip4addr_ntoa(&addr_info.dnsp_addr_s.ip_addr),
                      nwy_ip6addr_ntoa(&addr_info.dnsp_addr_s.ip6_addr));
    nwy_test_cli_echo("Dnss address: %s,%s\r\n",
                      nwy_ip4addr_ntoa(&addr_info.dnss_addr_s.ip_addr),
                      nwy_ip6addr_ntoa(&addr_info.dnss_addr_s.ip6_addr));
}

void nwy_test_cli_data_stop()
{
    char *sptr = NULL;
    int ret = NWY_GEN_E_UNKNOWN;

    sptr = nwy_test_cli_input_gets("\r\nPlease select resource handle id <1-%d>", NWY_DATA_CALL_MAX_NUM);
    int hndl = atoi(sptr);
    if ((hndl <= 0) || (hndl > NWY_DATA_CALL_MAX_NUM))
    {
        nwy_test_cli_echo("\r\nInvaild resource handle id: %d\r\n", hndl);
        return;
    }
    ret = nwy_data_stop_call(hndl);
    NWY_CLI_LOG("=DATA=  nwy_data_stop_call ret= %d", ret);
    if (ret != NWY_SUCCESS)
        nwy_test_cli_echo("\r\nStop data call fail, result<%d>\r\n", ret);
    else
        nwy_test_cli_echo("\r\nStop data call ...\r\n");
}

void nwy_test_cli_data_release()
{
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nPlease select resource handle id <1-%d>", NWY_DATA_CALL_MAX_NUM);
    int hndl = atoi(sptr);
    if ((hndl < 0) || (hndl > NWY_DATA_CALL_MAX_NUM))
    {
        nwy_test_cli_echo("\r\nInvaild resource handle id: %d\r\n", hndl);
        return;
    }

    nwy_data_release_srv_handle(hndl);
    NWY_CLI_LOG("=DATA=  nwy_data_relealse_srv_handle hndl= %d", hndl);
    nwy_test_cli_echo("\r\nRelease resource handle id %d\r\n", hndl);
}

void nwy_test_cli_data_get_flowcalc()
{
#ifdef NWY_OPEN_TEST_FLOW_S
    nwy_data_flowcalc_info_t flowcalc_info;
    if(0 == nwy_data_get_flowcalc_info(&flowcalc_info))
    {
        nwy_test_cli_echo("\r\ntx_bytes: %llu\r\n"
                              "rx_bytes: %llu\r\n"
                              "tx_packets: %lu\r\n"
                              "rx_packets: %lu\r\n",
                              flowcalc_info.tx_bytes,
                              flowcalc_info.rx_bytes,
                              flowcalc_info.tx_packets,
                              flowcalc_info.rx_packets);
    }
    else
    {
        nwy_test_cli_echo("\r\nget cfgdftpdn failed.\r\n");
    }
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif /*NWY_OPEN_TEST_FLOW_S*/
}

void nwy_test_cli_set_dns_server()
{
#ifndef NWY_OPEN_TEST_DNSERVER_NS
    int index;
    char *sptr = NULL;
    sptr = nwy_test_cli_input_gets("\r\nPlease input index info: index <1,2>");
    index = atoi(sptr);
    if(index !=1 && index != 2)
    {
        nwy_test_cli_echo("\r\nInvaild index value: %d\r\n", index);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input dns adder: dns (length 0-%d)\r\n", 40);
    if (strlen(sptr) > 40)
    {
        nwy_test_cli_echo("\r\nInvaild dns len\r\n");
        return;
    }
    if(nwy_set_dns_server(index,sptr) == 0)
    {
        nwy_test_cli_echo("\r\nnwy_set_dns_server success!!!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nnwy_set_dns_server fail!!!\r\n");
    }
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif
}

void nwy_test_cli_get_dns_server()
{
#ifndef NWY_OPEN_TEST_DNSERVER_NS
    char dns_addr[255];
    if(nwy_get_dns_server(dns_addr) == 0)
    {
        nwy_test_cli_echo("\r\nwy_test_cli_get_dns_server success!!!");
        nwy_test_cli_echo("\r\n dns_addr: %s",dns_addr);
    }
    else
    {
        nwy_test_cli_echo("\r\nwy_test_cli_get_dns_server fail!!!\r\n");
    }
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif

}
void nwy_test_cli_nwy_set_netsharemode()
{
#ifdef NWY_OPEN_TEST_NETSHARE_S
    int mode;
    char *sptr = NULL;
    sptr = nwy_test_cli_input_gets("\r\nPlease input mode info: mode <1: ecm, 0: rndis>");
    mode = atoi(sptr);
    if(nwy_nw_set_netsharemode(mode) ==0)
    {
        nwy_test_cli_echo("\r\nset_netshare mode success!!!\r\n");
        nwy_test_cli_echo("\r\nreseting the device takes effect!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nset_netshare mode fail!!!\r\n");
    }
#else
            nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif 
}
void nwy_test_cli_nwy_set_netshareact()
{
#ifdef NWY_OPEN_TEST_NETSHARE_S

    char *sptr = NULL;
    int ucid;
    int action;
    int auto_connect =0;
    int pdpType;
    int authType;
    int ret = -1;
    nwy_nw_cfgdftpdn_t cfgdftpdn_info;
    memset(&cfgdftpdn_info, 0, sizeof(nwy_nw_cfgdftpdn_t));
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input cid: cid <2~15>");
    ucid = atoi(sptr);
    if (ucid < 2 || ucid > 15)
    {
        nwy_test_cli_echo("\r\nInvaild cid value: %d\r\n", ucid );
        return ;
    }
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input action: action < 0 : stop,1 : start >");
    action = atoi(sptr);
    if ((action != 0) && (action !=1))
    {
        nwy_test_cli_echo("\r\nInvaild action value: %d\r\n", action );
        return ;
    }
    if (action == 0 )
    {
        pdpType = 1;
        auto_connect = 0;
        authType = 0;
        ret = nwy_nw_set_netshareact(ucid,action, auto_connect,cfgdftpdn_info.apn,cfgdftpdn_info.userName,
                                                            cfgdftpdn_info.password,authType,pdpType);
        if ( ret == 0 )
        {
            nwy_test_cli_echo("\r\nnetshareact successful!\r\n");
        }
        else
        {
            nwy_test_cli_echo("\r\nnetshareact fail!\r\n");
        }
    }
    else
    {
        sptr = nwy_test_cli_input_gets("\r\nPlease input auto_connect: auto_connect < 0 : manual,1 : auto >");
        auto_connect = atoi(sptr);
        if ((auto_connect != 0) && (auto_connect !=1))
        {
            nwy_test_cli_echo("\r\nInvaild auto_connect value: %d\r\n", auto_connect );
            return ;
        }

        sptr = nwy_test_cli_input_gets("\r\nPlease select apn maxlen is 99:");
        if (strlen(sptr) > 99)
        {
            nwy_test_cli_echo("\r\nInvaild apn len\r\n");
            return ;
        }
        memcpy(cfgdftpdn_info.apn, sptr, strlen(sptr));
        
        sptr = nwy_test_cli_input_gets("\r\nPlease select username maxlen is 64:");
        if (strlen(sptr) > 64)
        {
            nwy_test_cli_echo("\r\nInvaild username len\r\n");
            return ;
        }
        memcpy(cfgdftpdn_info.userName, sptr, strlen(sptr));
        
        sptr = nwy_test_cli_input_gets("\r\nPlease select password maxlen is 64:");
        if (strlen(sptr) > 64)
        {
            nwy_test_cli_echo("\r\nInvaild password len\r\n");
            return ;
        }
        memcpy(cfgdftpdn_info.password, sptr, strlen(sptr));

        sptr = nwy_test_cli_input_gets("\r\nPlease select auth mode (0 - NONE, 1 - PAP , 2 - CHAP):");
        authType = atoi(sptr);
        if ((authType < 0) || (authType > 2))
        {
            nwy_test_cli_echo("\r\nInvaild auth_type value: %d\r\n",authType);
            return ;
        }
        cfgdftpdn_info.authProt = authType;

        sptr = nwy_test_cli_input_gets("\r\nPlease select pdpType mode (1 - IP, 2 - IPV6, 3 - IPV4V6):");
        pdpType = atoi(sptr);
        if ((pdpType != 3) && (pdpType !=1) && (pdpType !=2))
        {
            nwy_test_cli_echo("\r\nInvaild pdp_type value: %d\r\n", pdpType );
            return ;
        }
        cfgdftpdn_info.pdpType = pdpType;
        
        ret = nwy_nw_set_netshareact(ucid,action, auto_connect,cfgdftpdn_info.apn,cfgdftpdn_info.userName,
                                                            cfgdftpdn_info.password,authType,pdpType);
        if ( ret == 0 )
        {
            nwy_test_cli_echo("\r\nnetshareact successful!\r\n");
        }
        else
        {
            nwy_test_cli_echo("\r\nnetshareact fail!\r\n");
        }
    }
#else
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif 

}
/**************************NW*********************************/
static int scan_finish = 0;
static nwy_nw_net_scan_list_t nw_scan_list;
#define TBL_SIZE(tbl) (sizeof(tbl) / sizeof(tbl[0]))
extern uint32_t nwy_test_band_lock_tab[NWY_BAND_LOCK_NUM];
extern uint32_t nwy_test_freq_lock_tab[NWY_FREQ_LOCK_NUM];

static const byte cmcc_plmn[][6] =
    {
        "46000",
        "46002",
        "46004",
        "46007",
        "46008",
        "46013",
        "46020",
        "45412",
        "45413",
};
static const byte unicom_plmn[][6] =
    {
        "46001",
        "46006",
        "46009",
        "46010",
};
static const byte ct_plmn[][6] =
    {
        "46003",
        "46005",
        "46011",
        "46012",
        "46059",
        "45502",
        "45507",
};
void nwy_nw_register_callback_cb(nwy_nw_regs_ind_type_t ind_type, void *ind_struct)
{
    nwy_nw_common_regs_type_t *regs_info = (nwy_nw_common_regs_type_t*)ind_struct;

    switch (ind_type) 
    {
        case NWY_NW_REGS_VOICE_IND:
            nwy_test_cli_echo("\n");
            nwy_test_cli_echo("Network voice: register state=%d, radio_tech=%d, roam_state=%d\n",
                regs_info->regs_state,regs_info->radio_tech, regs_info->roam_state);
            break;
        case NWY_NW_REGS_DATA_IND:
            nwy_test_cli_echo("\n");
            nwy_test_cli_echo("Network data: register state=%d, radio_tech=%d, roam_state=%d\n",
                regs_info->regs_state, regs_info->radio_tech, regs_info->roam_state);
            break;
        default:
            break;
    }
}

void nwy_test_cli_nw_get_mode()
{
    nwy_nw_mode_type_t network_mode = 0;
    if (NWY_SUCCESS == nwy_nw_get_network_mode(&network_mode))
    {
        if (network_mode == NWY_NW_MODE_GSM)
            nwy_test_cli_echo("\r\nCurrent Network mode: GSM (%d)\r\n", network_mode);
        if (network_mode == NWY_NW_MODE_LTE)
            nwy_test_cli_echo("\r\nCurrent Network mode: LTE (%d)\r\n", network_mode);
    }
    else
    {
        nwy_test_cli_echo("\r\nGet Network mode Failed!!!\r\n");
    }
}

void nwy_test_cli_nw_set_mode()
{
#ifdef NWY_OPEN_TEST_SET_NWMODE_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sub_opt = NULL;
    int mode = 0;
    sub_opt = nwy_test_cli_input_gets("\r\nPlease select network mode (0 - AUTO,2 - GSM,4 - LTE):");
    mode = atoi(sub_opt);
    if ((mode == 0) || (mode == 2) || (mode == 4))
    {
        nwy_nw_set_network_mode(mode);
        nwy_test_cli_echo("\r\nSet network mode over\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nInput Wrong network mode!!!\r\n");
    }
#endif
}

void nwy_test_cli_nw_get_fplmn()
{
#ifdef NWY_OPEN_TEST_FPLMN_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    nwy_nw_fplmn_list_t fplmn_list = {0};
    int i = 0;
    memset(&fplmn_list, 0x00, sizeof(fplmn_list));
    if (NWY_SUCCESS == nwy_nw_get_forbidden_plmn(&fplmn_list))
    {
        nwy_test_cli_echo("\r\nGet FPLMN list %d", fplmn_list.num);
        for (i = 0; i < fplmn_list.num; i++)
        {
            nwy_test_cli_echo("\r\n%s - %s", fplmn_list.fplmn[i].mcc, fplmn_list.fplmn[i].mnc);
        }
        nwy_test_cli_echo("\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nGet FPLMN List Failed!!!\r\n");
    }
#endif
}

/*
@func
    nwy_network_util_get_oper_type
@desc
    get current operator type from plmn string
@param
    plmn_buf: input plmn string
@return
    0 : CMCC
    1 : CU
    2 : CT
    3 : OTHER
@other
*/
static int nwy_network_util_get_oper_type(char *plmn_buf)
{
    int i;
    if (plmn_buf == NULL)
        return 3;
    for (i = 0; i < TBL_SIZE(cmcc_plmn); i++)
    {
        if (strncmp(plmn_buf, (char *)cmcc_plmn[i], strlen((char *)cmcc_plmn[i])) == 0)
        {
            return 0;
        }
    }

    for (i = 0; i < TBL_SIZE(unicom_plmn); i++)
    {
        if (strncmp(plmn_buf, (char *)unicom_plmn[i], strlen((char *)unicom_plmn[i])) == 0)
        {
            return 1;
        }
    }

    for (i = 0; i < TBL_SIZE(ct_plmn); i++)
    {
        if (strncmp(plmn_buf, (char *)ct_plmn[i], strlen((char *)ct_plmn[i])) == 0)
        {
            return 2;
        }
    }
    return 3;
}

/*
@func
    nwy_network_test_scan_cb
@desc
    network manual scan callback func
    call by nwy_nw_manual_network_scan()
@param
    net_list: output network scan result point
@return
    NONE
@other
*/
static char *scan_net_rat[9] =
{
    "GSM",
    "GSM compact",
    "UTRAN",
    "GSM w/EGPRS",
    "UTRAN w/HSDPA",
    "UTRAN w/HSUPA",
    "UTRAN w/HSDPA and HSUPA",
    "E-UTRAN",
    "EC-GSM-Iot(A/Gb mode)"
};

static char *scan_net_status[4] =
{
    "UNKNOWN",
    "AVAILABLE",
    "CURRENT",
    "FORBIDDEN"
};

static void nwy_network_test_scan_cb(
    nwy_nw_net_scan_list_t *net_list)
{
    int i;
    if (net_list == NULL)
        return;
    if (net_list->result != 1)
        nwy_test_cli_echo("\r\nManual network Scan failed\r\n");
    else
    {
        nwy_test_cli_echo("\r\nManual Scan Result: %d", net_list->num);
        for (i = 0; i < net_list->num; i++)
        {
            nwy_test_cli_echo("\r\nIndex: %d\r\n", i);
            nwy_test_cli_echo("MCC-MNC:%s-%s\r\n", net_list->net_list[i].net_name.mcc, net_list->net_list[i].net_name.mnc);
            nwy_test_cli_echo("Long EONS:%s\r\n", net_list->net_list[i].net_name.long_eons);
            nwy_test_cli_echo("Short EONS:%s\r\n", net_list->net_list[i].net_name.short_eons);
            nwy_test_cli_echo("Net Status:%s\r\n", scan_net_status[net_list->net_list[i].net_status]);
            nwy_test_cli_echo("Net Rat:%s\r\n", scan_net_rat[net_list->net_list[i].net_rat]);
        }
    }
    nwy_test_cli_echo("\r\nStart Test Select Network...\r\n");
    memset(&nw_scan_list, 0x00, sizeof(nw_scan_list));
    memcpy(&nw_scan_list, net_list, sizeof(nwy_nw_net_scan_list_t));
    scan_finish = 1;
}

/*
@func
    nwy_network_test_nw_select
@desc
    network manual select function.
@param
    NONE
@return
    NONE
@other
    User can only choose insert SIM/USIM operator's network,
    and if choose not support network will cause UE crash.
*/
void nwy_network_test_nw_select()
{
    char imsi[20] = {0};
    char plmn_str[10] = "";
    int i = 0, j = 0, num = 0;
    int opertor_type = 3;
    char *sub_opt = NULL;
    int id = 0;
    int index[10];
    nwy_nw_net_select_param_t param = {0};
/*Begin: Modify by huangsangen in 2022.11.2 for dualSim */
    #ifdef NWY_OPEN_TEST_POC_DS
    nwy_sim_id_t nSim = NWY_SIM_ID_MAX;
    nSim = (nwy_sim_id_t) nwy_poc_dsds_get_mastercard_card();

    if(nwy_sim_get_imsi(nSim,imsi,sizeof(imsi)) == NWY_SUCCESS)
    #else
    // 1. Get current sim imsi for operator type
    if (nwy_sim_get_imsi(NWY_SIM_ID_SLOT_1,imsi,sizeof(imsi)) == NWY_SUCCESS)
    #endif /* NWY_OPEN_TEST_POC_DS */
/*End: Modify by huangsangen in 2022.11.2 for dualSim */
    {
        nwy_test_cli_echo("\r\n imsi: %s",imsi);
        nwy_sleep(1000); //wait for imsi result
        //1.1 Phase imsi to Operator Type.
        opertor_type = nwy_network_util_get_oper_type(imsi);
        nwy_test_cli_echo("\r\nOperator type: %d(0-CMCC,1-CU,2-CT,3-OTHER),Current netwrok list: ", opertor_type);
        //2. Get scan result's operator type and compare with current operator type.
        for (i = 0; i < nw_scan_list.num; i++)
        {
            memset(plmn_str, 0x00, sizeof(plmn_str));
            sprintf(plmn_str, "%s%s", nw_scan_list.net_list[i].net_name.mcc,
                    nw_scan_list.net_list[i].net_name.mnc);
            //2.1 only show user available select netwrok list.
            if (nwy_network_util_get_oper_type(plmn_str) == opertor_type)
            {
                index[num++] = i;
                nwy_test_cli_echo("\r\nIndex:%d Rat:%d ", i, nw_scan_list.net_list[i].net_rat);
                nwy_test_cli_echo("MCC-MNC:%s-%s", nw_scan_list.net_list[i].net_name.mcc, nw_scan_list.net_list[i].net_name.mnc);
            }
        }
        sub_opt = nwy_test_cli_input_gets("\r\nPlease select index at before table:");
        id = atoi(sub_opt);
        for (j = 0; j < num; j++)
        {
            if (id == index[j])
                break;
        }
        if (j == num)
        {
            nwy_test_cli_echo("\r\nSelect index %d error!!!", id);
            return;
        }
        memcpy(param.mcc, nw_scan_list.net_list[id].net_name.mcc, sizeof(param.mcc));
        memcpy(param.mnc, nw_scan_list.net_list[id].net_name.mnc, sizeof(param.mnc));
        param.net_rat = nw_scan_list.net_list[id].net_rat;
        //3. start manual select network.
        nwy_nw_manual_network_select(&param);
        nwy_test_cli_echo("\r\nSelect index %d network over!!!", id);
    }
    else
    {
        nwy_test_cli_echo("\r\nGet Current operator type FAILED!!!");
    }
    return;
}

void nwy_test_cli_nw_manual_scan()
{
#ifdef NWY_OPEN_TEST_NET_SCAN_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    int count = 0;
    scan_finish = 0;
    nwy_test_cli_echo("\r\nWaiting Scan result...");
    if (NWY_SUCCESS == nwy_nw_manual_network_scan(nwy_network_test_scan_cb))
    {
        while (1)
        {
            if (scan_finish == 1 || count == 180) //limit 3 minute
                break;
            nwy_sleep(1000);
            count++;
        }

        if (nw_scan_list.result == true)
        {
            nwy_network_test_nw_select();
        }
        else
            nwy_test_cli_echo("\r\nScan network Failed!!!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nScan network Failed!!!\r\n");
    }
#endif
}

void nwy_test_cli_nw_band_lock()
{
    char *sub_opt = NULL;
    char set_band[64] = {0};
    int act = 0;
    int mode = 0;

    //1. Get rat mode
    sub_opt = nwy_test_cli_input_gets("\r\nPlease set rat mode(0-AUTO, 1-GSM, 2-WCDMA, 3-LTE): ");
    mode = atoi(sub_opt);
    if(mode < 0 || mode > NWY_BAND_LOCK_NUM-1)
    {
      nwy_test_cli_echo("\r\nUnsupport rat mode.\r\n");
      return;
    }
    act = nwy_test_band_lock_tab[mode];
    if(NWY_OPEN_LOCK_NS == act)
    {
      nwy_test_cli_echo("\r\nUnsupport rat mode.\r\n");
      return;
    }
    //Get bandlock mask
    sub_opt = nwy_test_cli_input_gets("\r\nPlease set bandlock mask(limit 256bits): ");
    memset(set_band, 0, sizeof(set_band));
    memcpy(set_band, sub_opt, strlen((char *)sub_opt));

    //Set BADNLOCK
    if(0 == nwy_nw_band_lock(act, set_band))
    {
      if (act == NWY_OPEN_LOCK_AUTO)
        nwy_test_cli_echo("\r\nBand unlock success.\r\n");
      else
        nwy_test_cli_echo("\r\nBand lock success.\r\n");
    }
    else
      nwy_test_cli_echo("\r\nBand lock failed.\r\n");
}

void nwy_test_cli_nw_freq_lock()
{
    char *sub_opt = NULL;
    int freq_num = 0;
    nwy_open_freq_t freq_info[9] = {0};
    int i = 0;
    int mode = 0;
    //get num of freq
    sub_opt = nwy_test_cli_input_gets("\r\nPlease enter freq count(unlock: 0, lock:1-9):");
    freq_num = atoi(sub_opt);
    if(freq_num == 0)
    {
        if(0 == nwy_nw_freq_lock(NULL, 0))
        {
            nwy_test_cli_echo("\r\nfreq unlock ok\r\n");
        }
        else
        {
            nwy_test_cli_echo("\r\nfreq lock fail\r\n");
        }
        return;
    }
    else if (freq_num > 9 || freq_num < 0)
    {
        nwy_test_cli_echo("\r\ninput number of freq is error!\r\n");
        return;
    }

    for (i = 0; i < freq_num; i++)
    {
        sub_opt = nwy_test_cli_input_gets("\r\nPlease enter rat mode(1-GSM, 2-WCDMA, 3-LTE): ");
        mode = atoi(sub_opt);
        if ( mode < 1 || mode > NWY_FREQ_LOCK_NUM )
        {
          nwy_test_cli_echo("\r\nUnsupport rat mode.\r\n");
          return;
        }
        else
          freq_info[i].net = nwy_test_freq_lock_tab[mode - 1];
        if(NWY_OPEN_LOCK_NS == freq_info[i].net)
        {
          nwy_test_cli_echo("\r\nUnsupport rat mode.\r\n");
          return;
        }
        sub_opt = nwy_test_cli_input_gets("\r\nPlease enter band: ");
        freq_info[i].band = atoi(sub_opt);
        sub_opt = nwy_test_cli_input_gets("\r\nPlease enter freq number(max,65535): ");
        if (65535 > atoi(sub_opt))
            freq_info[i].freq = atoi(sub_opt);
        else
            freq_info[i].freq = 65535;
    }
    if (NWY_SUCCESS == nwy_nw_freq_lock(freq_info,freq_num))
        nwy_test_cli_echo("\r\nlock freq success\r\n");
    else
        nwy_test_cli_echo("\r\nlock freq fail\r\n");
}

void nwy_test_cli_nw_get_ims_st()
{
#ifdef NWY_OPEN_TEST_IMS_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    uint8_t ims_enable = 0;
    nwy_nw_get_ims_state(&ims_enable);
    nwy_test_cli_echo("\r\nIMS state %d", ims_enable);
#endif
}

void nwy_test_cli_nw_set_ims_st()
{
#ifdef NWY_OPEN_TEST_IMS_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sub_opt = NULL;
    sub_opt = nwy_test_cli_input_gets("\r\nPlease set IMS state(0-off,1-on):");
    //1. before set IMS open  need set LTE only mode first.
    if (atoi(sub_opt) == 1)
    {
        nwy_nw_set_network_mode(4);
        nwy_sleep(2000);
    }
    // set IMS state
    uint8_t sub_switch = atoi(sub_opt);
    if (NWY_SUCCESS == nwy_nw_set_ims_state(sub_switch)) /*update by gaohe for Eliminate the warning in 2021/12/1*/
    {
        nwy_test_cli_echo("\r\nIMS SET Success!!!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nIMS SET Failed!!!\r\n");
    }
#endif
}

void nwy_test_cli_nw_get_def_pdn()
{
    char *apn = nwy_nw_get_default_pdn_apn();
    if (apn == NULL)
        nwy_test_cli_echo("\r\nGet Default Pdn Info Failed!!\r\n");
    else
        nwy_test_cli_echo("\r\nDefault Pdn Apn:%s\r\n", apn);
}

void nwy_test_cli_nw_set_def_pdn()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_nw_get_radio_st()
{
    int nFM = 0;
    if (nwy_nw_get_radio_st(&nFM) != 0)
    {
        nwy_test_cli_echo("\r\nget radio state error!\r\n");
        return;
    }

    nwy_test_cli_echo("\r\nradio state:%d\r\n", nFM);
}

void nwy_test_cli_nw_set_radio_st()
{
    char *sub_opt = NULL;

    sub_opt = nwy_test_cli_input_gets("\r\nPlease set radio state(0-off,1-on):");
    int fun = atoi(sub_opt);
    if (nwy_nw_set_radio_st(fun) == 0)
        nwy_test_cli_echo("\r\nSet radio state OK!\r\n");
    else
        nwy_test_cli_echo("\r\nSet radio state error!\r\n");
}

void nwy_test_cli_nw_get_radio_sign()
{
    uint8_t csq_val = 0;
    nwy_nw_get_signal_csq(&csq_val);
    nwy_test_cli_echo("\r\nCSQ is %d \r\n", csq_val);
}

void nwy_test_cli_nw_cs_st()
{
#ifdef NWY_OPEN_TEST_CS_NS
      nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    int status = 0;

    if (nwy_nw_get_cs_st(&status) == 0)
    {
        switch (status)
        {
        case 0:
            nwy_test_cli_echo("\r\n CS STATE: %d (not register)\r\n", status);
            break;
        case 1:
            nwy_test_cli_echo("\r\n CS STATE: %d (register,homework)\r\n", status);
            break;
        case 2:
            nwy_test_cli_echo("\r\n CS STATE: %d (searching)\r\n", status);
            break;
        case 3:
            nwy_test_cli_echo("\r\n CS STATE: %d (register denied)\r\n", status);
            break;
        case 4:
            nwy_test_cli_echo("\r\n CS STATE: %d (unknow)\r\n", status);
            break;
        case 5:
            nwy_test_cli_echo("\r\n CS STATE: %d (register roaming)\r\n", status);
            break;
        case 6:
            nwy_test_cli_echo("\r\n CS STATE: %d (register for \"SMS only\",home network)\r\n", status);
            break;
        case 7:
            nwy_test_cli_echo("\r\n CS STATE: %d (register for \"SMS only\",roaming)\r\n", status);
            break;
        default:
            nwy_test_cli_echo("\r\n CS STATE: %d (unknow state)\r\n", status);
            break;
        }
    }
    else
        nwy_test_cli_echo("\r\n CS STATE ERROR!\r\n");
    return;
#endif
}

void nwy_test_cli_nw_ps_st()
{
    int status = 0;

    if (nwy_nw_get_ps_st(&status) == 0)
    {
        switch (status)
        {
        case 0:
            nwy_test_cli_echo("\r\n PS STATE: %d (not register)\r\n", status);
            break;
        case 1:
            nwy_test_cli_echo("\r\n PS STATE: %d (register,homework)\r\n", status);
            break;
        case 2:
            nwy_test_cli_echo("\r\n PS STATE: %d (searching)\r\n", status);
            break;
        case 3:
            nwy_test_cli_echo("\r\n PS STATE: %d (register denied)\r\n", status);
            break;
        case 4:
            nwy_test_cli_echo("\r\n PS STATE: %d (unknow)\r\n", status);
            break;
        case 5:
            nwy_test_cli_echo("\r\n PS STATE: %d (register roaming)\r\n", status);
            break;
        case 6:
            nwy_test_cli_echo("\r\n PS STATE: %d (register for \"SMS only\",home network)\r\n", status);
            break;
        case 7:
            nwy_test_cli_echo("\r\n PS STATE: %d (register for \"SMS only\",roaming)\r\n", status);
            break;
        default:
            nwy_test_cli_echo("\r\n PS STATE: %d (unknow state)\r\n", status);
            break;
        }
    }
    else
        nwy_test_cli_echo("\r\n PS STATE ERROR!\r\n");
    return;
}

void nwy_test_cli_nw_lte_st()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_nw_operator_info()
{
    nwy_nw_regs_info_type_t reg_info = {0};
    nwy_nw_operator_name_t opt_name = {0};
    uint8_t csq_val = 0;
    char lac[16] = {0};
    char cid[16] = {0};

    memset(&reg_info, 0x00, sizeof(reg_info));
    if (NWY_SUCCESS == nwy_nw_get_register_info(&reg_info))
    {
        if (reg_info.data_regs_valid == 1)
        {
            nwy_test_cli_echo("\r\nNetwork Data Reg state: %d\r\n"
                              "Network Data Roam state: %d\r\n"
                              "Network Data Radio Tech: %d\r\n",
                              reg_info.data_regs.regs_state,
                              reg_info.data_regs.roam_state,
                              reg_info.data_regs.radio_tech);
        }
        if (reg_info.voice_regs_valid == 1)
        {
            nwy_test_cli_echo("\r\nNetwork Voice Reg state: %d\r\n"
                              "Network Voice Roam state: %d\r\n"
                              "Network Voice Radio Tech: %d\r\n",
                              reg_info.voice_regs.regs_state,
                              reg_info.voice_regs.roam_state,
                              reg_info.voice_regs.radio_tech);
        }

        // Get operator name
        if (reg_info.data_regs.regs_state != NWY_NW_SERVICE_NONE || \
            reg_info.voice_regs.regs_state != NWY_NW_SERVICE_NONE)
        {
            memset(&opt_name, 0x00, sizeof(opt_name));
            if (NWY_SUCCESS == nwy_nw_get_operator_name(&opt_name))
            {
                nwy_test_cli_echo("\r\nLong EONS: %s\r\n"
                                  "Short EONS: %s\r\n"
                                  "MCC and MNC: %s %s\r\n"
                                  "SPN: %s\r\n",
                                  (char *)opt_name.long_eons,
                                  (char *)opt_name.short_eons,
                                  opt_name.mcc,
                                  opt_name.mnc,
                                  opt_name.spn);
            }
        }

        //Get CSQ
        nwy_nw_get_signal_csq(&csq_val);
        nwy_test_cli_echo("\r\nCSQ is %d \r\n", csq_val);
        //Get Lac and CELL ID
#ifndef NWY_OPEN_TEST_CELL_ID_NS
        nwy_nw_get_lacid(lac, cid);
        nwy_test_cli_echo("\r\nLAC: %s, CELL_ID: %s \r\n", lac, cid);
#endif
    }
    else
    {
        nwy_test_cli_echo("\r\nGet Register Information Failed!!!\r\n");
    }
}

void nwy_test_cli_nw_get_ehplmn()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_nw_get_signal_rssi()
{
    uint8_t rssi = 0;
    if (NWY_SUCCESS == nwy_nw_get_signal_rssi(&rssi))
        nwy_test_cli_echo("\r\nRSSI: %d\r\n", rssi);
    else
        nwy_test_cli_echo("Get RSSI Failed!");
}

static void nwy_cellinfo_scan_cb (void *infor, int num)
{
    nwy_scanned_locator_info_t *cellinfo = (nwy_scanned_locator_info_t *)infor;

    int i;
    if (cellinfo->num == 0)
    {
        nwy_test_cli_echo("\r\nCell info Scan failed\r\n");
        return;
    }
    if (cellinfo->curr_rat == 4)
    {
        nwy_test_cli_echo("\r\nLte Serving cell: mcc:%x,mnc:%d,band:%d,tac:%x,cellid:%x,pci:%d,rsrp:%d,rsrq:%d,sinr:%d\r\n",
                         cellinfo->scell_info.lte_scell_info.mcc, cellinfo->scell_info.lte_scell_info.mnc,
                         cellinfo->scell_info.lte_scell_info.bandInfo, 
                         cellinfo->scell_info.lte_scell_info.tac, cellinfo->scell_info.lte_scell_info.cellId,
                         cellinfo->scell_info.lte_scell_info.pcid, cellinfo->scell_info.lte_scell_info.rsrp,
                         cellinfo->scell_info.lte_scell_info.rsrq, cellinfo->scell_info.lte_scell_info.SINR);
        for (i = 0; i < cellinfo->ncell.lte_ncell.lteInterNcell.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nLte interNeighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,tac:%x,cellid:%x,pci:%d,rsrp:%d,rsrq:%d\r\n", i,
                         cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].mcc, cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].mnc,
                         cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].frequency, cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].tac,
                         cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].cellId, cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].pcid,
                         cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].rsrp, cellinfo->ncell.lte_ncell.lteInterNcell.lteNcell[i].rsrq);
        }
        for (i = 0; i < cellinfo->ncell.lte_ncell.lteIntraNcell.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nLte intraNeighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,tac:%x,cellid:%x,pci:%d,rsrp:%d,rsrq:%d\r\n", i,
                         cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].mcc, cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].mnc,
                         cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].frequency, cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].tac,
                         cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].cellId, cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].pcid,
                         cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].rsrp, cellinfo->ncell.lte_ncell.lteIntraNcell.lteNcell[i].rsrq);
        }
        for (i = 0; i < cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nLte umts Neighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,tac:%x,cellid:%x,rscp:%d,\r\n", i,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].mcc, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].mnc,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].Arfcn, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].Lac,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].ci, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATUtra.umtsNcell[i].rscp);
        }
        for (i = 0; i < cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nLte gsm Neighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x,rssi:%d\r\n", i,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].mcc, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].mnc,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].Arfcn, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].lac,
                         cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].Cellid, cellinfo->ncell.lte_ncell.lteInterRatinfo.interRATGsm.gsmsNcell[i].rssi);
        }
    }
    else if (cellinfo->curr_rat == 3)
    {
        nwy_test_cli_echo("\r\numts Serving cell: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x,rssi:%d,Rscp:%d\r\n",
                         cellinfo->scell_info.umts_scell_info.mcc, cellinfo->scell_info.umts_scell_info.mnc,
                          cellinfo->scell_info.umts_scell_info.Arfcn,cellinfo->scell_info.umts_scell_info.Lac,
                         cellinfo->scell_info.umts_scell_info.ci, cellinfo->scell_info.umts_scell_info.rssi,
                         cellinfo->scell_info.umts_scell_info.rscp);
        for (i = 0; i < cellinfo->ncell.umts_ncell.umtsInterNcell.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\numts interNeighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x\r\n", i,
                         cellinfo->ncell.umts_ncell.umtsInterNcell.umtsNcell[i].mcc, cellinfo->ncell.umts_ncell.umtsInterNcell.umtsNcell[i].mnc,
                         cellinfo->ncell.umts_ncell.umtsInterNcell.umtsNcell[i].Arfcn, cellinfo->ncell.umts_ncell.umtsInterNcell.umtsNcell[i].Lac,
                         cellinfo->ncell.umts_ncell.umtsInterNcell.umtsNcell[i].ci);
        }
        for (i = 0; i < cellinfo->ncell.umts_ncell.umtsIntraNcell.ncell_num; i++)
        {
        nwy_test_cli_echo("\r\numts inrarNeighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x\r\n", i,
                         cellinfo->ncell.umts_ncell.umtsIntraNcell.umtsNcell[i].mcc, cellinfo->ncell.umts_ncell.umtsIntraNcell.umtsNcell[i].mnc,
                         cellinfo->ncell.umts_ncell.umtsIntraNcell.umtsNcell[i].Arfcn, cellinfo->ncell.umts_ncell.umtsIntraNcell.umtsNcell[i].Lac,
                         cellinfo->ncell.umts_ncell.umtsIntraNcell.umtsNcell[i].ci);
        }
        for (i = 0; i < cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatLte.ncell_num; i++)
        {
        nwy_test_cli_echo("\r\numts lte Neighbour cell %d: arfcn:%d,pcid:%x,rsrp:%d,rsrq:%d\r\n", i,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatLte.lteNcell[i].frequency, cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatLte.lteNcell[i].pcid,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatLte.lteNcell[i].rsrp, cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatLte.lteNcell[i].rsrq);
        }
        for (i = 0; i < cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.ncell_num; i++)
        {
        nwy_test_cli_echo("\r\numts gsm Neighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x,bsic:%d,rssi:%d\r\n", i,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].mcc, cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].mnc,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].Arfcn, cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].lac,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].Cellid, cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].Bsic,
                         cellinfo->ncell.umts_ncell.umtsInterRatinfo.interRatGsm.gsmsNcell[i].rssi);
        }
    }
    else if (cellinfo->curr_rat == 2)
    {
        nwy_test_cli_echo("\r\nGsm Serving cell: mcc:%x,mnc:%x,band:%d,arfcn:%d,lac:%x,cellid:%x,bsic:%d,RxQualFull:%d\r\n",
                         cellinfo->scell_info.gsm_scell_info.mcc, cellinfo->scell_info.gsm_scell_info.mnc,
                         cellinfo->scell_info.gsm_scell_info.CurrBand, cellinfo->scell_info.gsm_scell_info.Arfcn,
                         cellinfo->scell_info.gsm_scell_info.Lac, cellinfo->scell_info.gsm_scell_info.Cellid,
                         cellinfo->scell_info.gsm_scell_info.Bsic,
                         cellinfo->scell_info.gsm_scell_info.RxQualFull);
        nwy_test_cli_echo("\r\n gsm ncell num is %d\n",cellinfo->ncell.gsm_ncell.gsm_ncell_info.ncell_num);
        for (i = 0; i < cellinfo->ncell.gsm_ncell.gsm_ncell_info.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nGsm Neighbour cell %d: mcc:%x,mnc:%x,arfcn:%d,lac:%x,cellid:%x,bsic:%d,\r\n", i,
                         cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].mcc, cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].mnc,
                         cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].Arfcn, cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].lac,
                         cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].Cellid, cellinfo->ncell.gsm_ncell.gsm_ncell_info.gsmsNcell[i].Bsic);
        }
        for (i = 0; i < cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatLTE.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nGsm lte Neighbour cell %d: arfcn:%d,pcid:%x,rsrp:%d,rsrq:%d\r\n", i,
                         cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatLTE.lteNcell[i].frequency, cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatLTE.lteNcell[i].pcid,
                         cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatLTE.lteNcell[i].rsrp, cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatLTE.lteNcell[i].rsrq);
        }
        for (i = 0; i < cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatUTRA.ncell_num; i++)
        {
            nwy_test_cli_echo("\r\nGsm umts Neighbour cell %d: arfcn:%d,rscp:%d\r\n", i,
                         cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatUTRA.umtsNcell[i].Arfcn, cellinfo->ncell.gsm_ncell.gsmInterRatinfo.interRatUTRA.umtsNcell[i].rscp);
        }
    }
    else
        nwy_test_cli_echo("\r\nCell info Scan failed\r\n");
}

void nwy_test_cli_nw_get_cellmsg()
{
    nwy_test_cli_echo("\r\n========================================\r\n");
    if (NWY_SUCCESS != nwy_nw_get_neighborLocatorInfo( nwy_cellinfo_scan_cb ))
    {
      nwy_test_cli_echo("\r\ncell scan failed!\r\n");
      return;
    }
    nwy_test_cli_echo("\r\n========================================\r\n");

}


void nwy_test_cli_nw_get_netmsg()
{
    nwy_test_cli_echo("\r\n start get netmsg!Expansion of the stack0\r\n");
    nwy_serving_cell_info pNetmsg;
    memset(&pNetmsg,0,sizeof(nwy_serving_cell_info));
    int ret ;
    ret = nwy_nw_get_netmsg(&pNetmsg);
    nwy_test_cli_echo("demo test is %d \n",ret);
	if (0 == ret)
	{
	    nwy_test_cli_echo("test rat is %d \n",pNetmsg.curr_rat);
		if (pNetmsg.curr_rat == 4)
             nwy_test_cli_echo("\r\nLTE NETMSG:rat = %d, CELL_ID=%x,PCI=%d, RSRQ=%d, RSRP=%d, SINR=%d,dlBler=%d, ulBler=%d\r\n",pNetmsg.curr_rat,pNetmsg.cellId,pNetmsg.pcid,pNetmsg.rsrq, pNetmsg.rsrp, pNetmsg.SINR, pNetmsg.dlBler, pNetmsg.ulBler);
            /*nwy_test_cli_echo("\r\nLTE NETMSG: CELL_ID=%X, PCI=%d, RSRQ=%d, RSRP=%d, SINR=%d, dlBler=%d, ulBler=%d\r\n", pNetmsg.cellId,
                              pNetmsg.pcid, pNetmsg.rsrq, pNetmsg.rsrp, pNetmsg.SINR, pNetmsg.dlBler, pNetmsg.ulBler);*/
        else if (pNetmsg.curr_rat == 3)
            nwy_test_cli_echo("\r\nGSM NETMSG: rat = %d,CELL_ID=%x,PCI=%d, TxPwr=%d, RxPwr=%d\r\n",pNetmsg.curr_rat,pNetmsg.cellId,pNetmsg.pcid,pNetmsg.txPower, pNetmsg.rxPower);
        else if (pNetmsg.curr_rat == 2)
            nwy_test_cli_echo("\r\nGSM NETMSG: rat = %d,CELL_ID=%x,PCI=%d, TxPwr=%d, RxPwr=%d, Bler=%d\r\n",pNetmsg.curr_rat,pNetmsg.cellId,pNetmsg.pcid,pNetmsg.txPower, pNetmsg.rxPower, pNetmsg.ulBler);
            /*nwy_test_cli_echo("\r\nGSM NETMSG: CELL_ID=%X, PCI=%d, TxPwr=%d, RxPwr=%d, Bler=%d\r\n", pNetmsg.cellId, pNetmsg.pcid,
                              pNetmsg.txPower, pNetmsg.rxPower, pNetmsg.ulBler);*/
        else
            nwy_test_cli_echo("\r\nGet NETMSG Failed!!!\r\n");
	}
	else
	{
		nwy_test_cli_echo("\r\nGet NETMSG Failed!!!\r\n");
	}
}


void nwy_test_cli_nw_get_cfgdftpdn_info(void)
{
#ifdef NWY_OPEN_TEST_CFGDFTPDN_S
    nwy_nw_cfgdftpdn_t cfgdftpdn_info;
/*Begin by zhaoyating to solve TD98876 20230321*/
    cfgdftpdn_info.pdpType = 1;
    cfgdftpdn_info.authProt = 1 ;
    memset(cfgdftpdn_info.apn, 0, sizeof(cfgdftpdn_info.apn));
    memset(cfgdftpdn_info.userName, 0, sizeof(cfgdftpdn_info.userName));
    memset(cfgdftpdn_info.password, 0, sizeof(cfgdftpdn_info.password));
/*End by zhaoyating to solve TD98876 20230321*/
    if(0 == nwy_nw_get_cfgdftpdn_info(&cfgdftpdn_info))
    {
        nwy_test_cli_echo("\r\ncfgdftpdn pdp type: %d\r\n"
                              "cfgdftpdn auth mode: %d\r\n"
                              "cfgdftpdn apn: %s\r\n"
                              "cfgdftpdn username: %s\r\n"
                              "cfgdftpdn password: %s\r\n",
                              cfgdftpdn_info.pdpType,
                              cfgdftpdn_info.authProt,
                              cfgdftpdn_info.apn,
                              cfgdftpdn_info.userName,
                              cfgdftpdn_info.password);
    }
    else
    {
        nwy_test_cli_echo("\r\nget cfgdftpdn failed.\r\n");
    }
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif /*NWY_OPEN_TEST_CFGDFTPDN_S*/
}

void nwy_test_cli_nw_set_cfgdftpdn_info(void)
{
#ifdef NWY_OPEN_TEST_CFGDFTPDN_S
    char *sub_opt = NULL;
    int pdpType,authType;
    nwy_nw_cfgdftpdn_t cfgdftpdn_info;
    memset(&cfgdftpdn_info, 0, sizeof(nwy_nw_cfgdftpdn_t));

    /* get parameter */
    sub_opt = nwy_test_cli_input_gets("\r\nPlease select pdpType mode (1 - IP, 2 - IPV6, 3 - IPV4V6):");
    pdpType = atoi(sub_opt);
    if ((pdpType != 3) && (pdpType !=1) && (pdpType !=2))
    {
        nwy_test_cli_echo("\r\nInvaild pdp_type value: %d\r\n", pdpType );
        return ;
    }
    cfgdftpdn_info.pdpType = pdpType;

    sub_opt = nwy_test_cli_input_gets("\r\nPlease select auth mode (0 - NONE, 1 - PAP , 2 - CHAP):");
    authType = atoi(sub_opt);
    if ((authType < 0) || (authType > 2))
    {
        nwy_test_cli_echo("\r\nInvaild auth_type value: %d\r\n",authType);
        return ;
    }
    cfgdftpdn_info.authProt = authType;

    sub_opt = nwy_test_cli_input_gets("\r\nPlease select apn maxlen is 99:");
    if (strlen(sub_opt) > 99)
    {
        nwy_test_cli_echo("\r\nInvaild apn len\r\n");
        return ;
    }
    memcpy(cfgdftpdn_info.apn, sub_opt, strlen(sub_opt));

    sub_opt = nwy_test_cli_input_gets("\r\nPlease select username maxlen is 64:");
    if (strlen(sub_opt) > 64)
    {
        nwy_test_cli_echo("\r\nInvaild username len\r\n");
        return ;
    }
    memcpy(cfgdftpdn_info.userName, sub_opt, strlen(sub_opt));

    sub_opt = nwy_test_cli_input_gets("\r\nPlease select password maxlen is 64:");
    if (strlen(sub_opt) > 64)
    {
        nwy_test_cli_echo("\r\nInvaild password len\r\n");
        return ;
    }
    memcpy(cfgdftpdn_info.password, sub_opt, strlen(sub_opt));
    if(0 == nwy_nw_set_cfgdftpdn_info(cfgdftpdn_info))
    {
        nwy_test_cli_echo("\r\nset cfgdftpdn success.\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nset cfgdftpdn failed.\r\n");
    }
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif /*NWY_OPEN_TEST_CFGDFTPDN_S*/
}

/**************************VOICE*********************************/
#ifdef NWY_OPEN_TEST_VOICE
extern void nwy_test_cli_send_virt_at(void);

void nwy_test_cli_voice_call_start()
{
    char *sptr = NULL;
    int sim_id = 0x00;
    char phone_num[32] = {0};
    int ret = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input dest phone number: ");

    memcpy(phone_num, sptr, strlen(sptr));
    if (strlen(phone_num) > 128)
    {
        return;
    }
    nwy_test_cli_echo("\r\n input dest phone number:%s\r\n", phone_num);

    ret = nwy_voice_call_start(sim_id, phone_num);
    if (ret == 0)
    {
        nwy_test_cli_echo("\r\n nwy call success!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy call fail!\r\n");
    }
}

void nwy_test_cli_voice_call_end()
{
    int sim_id = 0x00;
    int query_cnt = 0;

    while (query_cnt < 20)
    {
        nwy_voice_call_end(sim_id);
        query_cnt++;
        nwy_sleep(20);
    }
}

void nwy_test_cli_voice_auto_answ()
{
    int query_cnt = 0;

    while (query_cnt < 50)
    {
        nwy_voice_call_autoanswver();
        query_cnt++;
        nwy_sleep(20);
    }
}

void nwy_test_cli_voice_volte_set()
{
    char *sptr = NULL;
    int sim_id = 0x00;
    char phone_num[32] = {0};
    int setvolte = 0;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\n Please set volte 0-close 1-open:");
    setvolte = atoi(sptr);

    nwy_test_cli_echo("\r\n set volte value :%d", setvolte);

    ret = nwy_voice_setvolte(sim_id, setvolte);

    if (ret == 0)
    {
        nwy_test_cli_echo("\r\n nwy set volte success!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy set volte fail!\r\n");
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input dest phone number: ");

    memcpy(phone_num, sptr, strlen(sptr));
    if (strlen(phone_num) > 128)
    {
        return;
    }
    nwy_test_cli_echo("\r\n input dest phone number:%s\r\n", phone_num);

    ret = nwy_voice_call_start(sim_id, phone_num);
    if (ret == 0)
    {
        nwy_test_cli_echo("\r\n nwy volte call success!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy volte call fail!\r\n");
    }
}

void voice_test_state_handler(uint32_t call_id, char *phone_num,
                                     nwy_vc_call_state_t state, void *contextPtr)
{
    int is_audio_active = 0;
    int is_volte = 0;
    int index = 0;
    int call_count =0;

    nwy_test_cli_echo("\nvoice_test_state_handler run\n");
    nwy_voice_call_record_type_t *record = NULL;

    if (contextPtr != NULL) {
        record = (nwy_voice_call_record_type_t*)contextPtr;
        {
            nwy_test_cli_echo("call id:%d; state:%d; number:%s; direction:%d; mode:%d; call_type:%d;\n",
            record->call_id,
            record->state,
            record->number,
            record->direction,
            record->call_mode,
            record->call_type);
       }
    } else {
        nwy_test_cli_echo("\nvoice_test_state_handler contextPtr is null\n");
    }

    switch (state) {
        case NWY_VOICE_CALL_STATE_INCOMING:
            nwy_test_cli_echo("voice call incomming\n");
            break;

        case NWY_VOICE_CALL_STATE_DIALING:
            nwy_test_cli_echo("voice call dialing\n");
            break;

        case NWY_VOICE_CALL_STATE_ALERTNG:
            nwy_test_cli_echo("voice call alerting\n");
            break;

        case NWY_VOICE_CALL_STATE_ACTIVE:
            nwy_test_cli_echo("voice call active\n");
            break;

        case NWY_VOICE_CALL_STATE_HOLDING:
            nwy_test_cli_echo("voice call holding\n");
            break;
        case NWY_VOICE_CALL_STATE_WAITING:
            nwy_test_cli_echo("voice call waiting\n");
            break;

        case NWY_VOICE_CALL_STATE_END:
            nwy_test_cli_echo("voice call end\n");
            break;

        default:
            break;
    }
}

void nwy_test_cli_voice_state_change()
{
    nwy_test_cli_echo("\r\n nwy_test_cli_voice_state_change \r\n");
    nwy_voice_add_statehandler((nwy_voice_statehandlerfunc)voice_test_state_handler);
}

void nwy_test_cli_voice_caller_id()
{
    nwy_test_cli_echo("\r\n please open clip at+clip = 1 \r\n");

    nwy_test_cli_send_virt_at();
}

void nwy_test_cli_voice_call_hold()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_voice_call_unhold()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}
#endif
/**************************SMS*********************************/
#ifdef NWY_OPEN_TEST_SMS
void nwy_test_cli_sms_send()
{
    char *sptr = NULL;
    nwy_sms_info_type_t sms_data = {0};
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input dest phone number: ");
    if (strlen(sptr) > NWY_SMS_MAX_ADDR_LENGTH) {
        nwy_test_cli_echo("\r\nnwy send sms fail by phone number oversize\r\n");
        return;
    }
    memcpy(sms_data.phone_num, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input msg len: ");
    sms_data.msg_context_len = atoi(sptr);
    if (sms_data.msg_context_len > 140) {
        nwy_test_cli_echo("\r\nnwy send sms fail by len > 140!\r\n");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input msg data: ");
    if (strlen(sptr) > 140) {
        nwy_test_cli_echo("\r\nnwy send sms fail by data oversize\r\n");
        return;
    }
    memcpy(sms_data.msg_contxet, sptr, strlen(sptr));
    if (strlen((char *)sms_data.msg_contxet) > sms_data.msg_context_len || sms_data.msg_context_len > 140) 
	{
        nwy_test_cli_echo("\r\nnwy send sms fail by len!\r\n");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input msg format(0:GSM7 2:UNICODE): ");
    sms_data.msg_format = (nwy_sms_mag_dcs_type_e)atoi(sptr);
    if (sms_data.msg_format != 0 && sms_data.msg_format != 2) {
        nwy_test_cli_echo("\r\nnwy send sms fail by invalid msg format!\r\n");
        return;
    }

    ret = nwy_sms_send_message(&sms_data);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy send sms fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy send sms success!\r\n");

    return;
}

void nwy_test_cli_sms_del()
{
    char *sptr = NULL;
    uint16_t nindex;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms index: ");
    nindex = atoi(sptr);

    ret = nwy_sms_delete_message(nindex);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy del sms fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy del sms success!\r\n");

    return;
}

void nwy_test_cli_sms_get_sca()
{
    char sca[21] = {0};
    int ret = 0;

    ret = nwy_sms_get_sca(sca);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy get sms sca fail!\r\n");
    else
        nwy_test_cli_echo("\r\nsca: %s \r\n", sca);

    return;
}

void nwy_test_cli_sms_set_sca()
{
    char *sptr = NULL;
    char sca[21] = {0};
    unsigned tosca;
    int ret = 0;
    int i = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input sca number: ");
    if (strlen(sptr) > 20) {
        nwy_test_cli_echo("\r\nnwy send sms fail by sca oversize\r\n");
        return;
    }
    memcpy(sca, sptr, strlen(sptr));
    for (i = 0; i < strlen(sca); i++) {
        if (sca[i] < '0' || sca[i] > '9') {
            if (i != 0) {
                nwy_test_cli_echo("nwy set SMS sca number wrong!\n");
                return;
            }
        }
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input sca type(129,145): ");
    tosca = atoi(sptr);
    if (tosca != 129 && tosca != 145) {
        nwy_test_cli_echo("\r\nnwy set SMS tosca fail!\r\n");
        return;
    }
    ret = nwy_sms_set_sca(sca, tosca);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy set SMS sca fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set SMS sca success!\r\n");

    return;
}

void nwy_test_cli_sms_set_storage()
{
    char *sptr = NULL;
    nwy_sms_storage_type_e sms_storage;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms storage(1-ME or 2-SM): ");
    sms_storage = (nwy_sms_storage_type_e)atoi(sptr);

    ret = nwy_sms_set_storage(sms_storage);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy set sms storage fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set sms storage success!\r\n");

    return;
}

void nwy_test_cli_sms_get_storage()
{
    nwy_sms_storage_type_e sms_storage;
    sms_storage = nwy_sms_get_storage();
    nwy_test_cli_echo("\r\nsms_storage: %d", sms_storage);

    return;
}

void nwy_test_cli_sms_set_report_md()
{
    char *sptr = NULL;
    uint8_t mode = 0;
    uint8_t mt = 0;
    uint8_t bm = 0;
    uint8_t ds = 0;
    uint8_t bfr = 0;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms mode(0-2): ");
    if (sptr[0] != '0' && sptr[0] != '1' && sptr[0] != '2') 
	{
        nwy_test_cli_echo("\r\nset report mode invalid mode input\r\n");
        return;
    }
    mode = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms mt(0-3): ");
    if (sptr[0] != '0' && sptr[0] != '1' && sptr[0] != '2' && sptr[0] != '3') 
	{
        nwy_test_cli_echo("\r\nset report mode invalid mt input\r\n");
        return;
    }
    mt = atoi(sptr);

    ret = nwy_set_report_option(mode, mt, bm, ds, bfr);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy set sms report mode fail!\r\n");
    else
        nwy_test_cli_echo("\r\nSet sms report mode success!\r\n");

    return;
}

void nwy_test_cli_sms_read()
{
    char *sptr = NULL;
    nwy_sms_recv_info_type_t sms_data = {0};
    unsigned index = 0;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms index: ");
    index = atoi(sptr);

    ret = nwy_sms_read_message(index, &sms_data);
    if (NWY_SUCESS != ret)
        nwy_test_cli_echo("\r\nnwy read sms fail!\r\n");
    else
    {
        NWY_CLI_LOG("=SMS= finish to read sms\n");
        nwy_sleep(2000); //wait for sms_data
        nwy_test_cli_echo("\r\nread one sms index:%d StorageId:%d oa:%s time %02d-%02d-%02d %02d:%02d:%02d+%d", sms_data.nIndex, sms_data.nStorageId, sms_data.oa, sms_data.scts.uYear,\
            sms_data.scts.uMonth, sms_data.scts.uDay, sms_data.scts.uHour, sms_data.scts.uMinute, sms_data.scts.uSecond, sms_data.scts.iZone);
        nwy_test_cli_echo("\r\nnwy recv sms sms_data :%s\r\n", sms_data.pData);
    }

    NWY_CLI_LOG("=SMS= end to test read sms\n");

    return;
}

void nwy_echo_sms_list_with_type(nwy_sms_msg_list_t sms_info)
{
    char unread_index[500] = {0};
    char read_index[500] = {0};
    char unsent_index[500] = {0};
    char sent_index[500] = {0};
    int temp = 0;
    for (temp = 0; (temp < sms_info.count); temp++)
    {
        NWY_CLI_LOG("\r\n =SMS= index=%d, tpye =%d", sms_info.sms_info[temp].index, sms_info.sms_info[temp].type);
        if (sms_info.sms_info[temp].type == NWY_SMS_STORED_CMGL_STATUS_UNREAD)
        {
            sprintf(unread_index + strlen(unread_index), "%d,", sms_info.sms_info[temp].index);
        }
        if (sms_info.sms_info[temp].type == NWY_SMS_STORED_CMGL_STATUS_READ)
        {
            sprintf(read_index + strlen(read_index), "%d,", sms_info.sms_info[temp].index);
    
        }
        if (sms_info.sms_info[temp].type == NWY_SMS_STORED_CMGL_STATUS_UNSENT)
        {
            sprintf(unsent_index + strlen(unsent_index), "%d,", sms_info.sms_info[temp].index);
        }
        if (sms_info.sms_info[temp].type == NWY_SMS_STORED_CMGL_STATUS_SENT)
        {
            sprintf(sent_index + strlen(sent_index), "%d,", sms_info.sms_info[temp].index);
        }
    }
    unread_index[strlen(unread_index) - 1] = '\0';
    read_index[strlen(read_index) - 1] = '\0';
    unsent_index[strlen(unsent_index) - 1] = '\0';
    sent_index[strlen(sent_index) - 1] = '\0';
    nwy_test_cli_echo("\r\n unread message index: %s", unread_index);
    nwy_test_cli_echo("\r\n read message index: %s", read_index);
    nwy_test_cli_echo("\r\n unsent message index: %s", unsent_index);
    nwy_test_cli_echo("\r\n sent message index: %s", sent_index);
    NWY_CLI_LOG("=SMS= end to test read sms\n");

}
void nwy_test_cli_sms_list()
{

    int ret = 0;
    nwy_sms_msg_list_t sms_info = {0};
    int i = 0;
    char temp[200] = {0};
    char temp_index[5] = {0};

    ret = nwy_sms_read_message_list(&sms_info);
    if (NWY_SUCESS != ret)
    {
        nwy_test_cli_echo("\r\n nwy read sms list fail!\r\n");
        return;
    }

    if(0 == sms_info.count)
    {
        nwy_test_cli_echo("\r\n no sms in storage!\r\n");
    }
    else
    {
        NWY_CLI_LOG("=SMS= finish to read sms list\n");
        if (sms_info.sms_info[0].type == NWY_SMS_STORED_CMGL_STATUS_UNKNOWN)
        {
            sprintf(temp, "nwy sms list index(no type): ");
            for (i = 0; i < sms_info.count; i++) {
                sprintf(temp_index, "%d,", sms_info.sms_info[i].index);
                strcat(temp, temp_index);
                memset(temp_index, 0, sizeof(temp_index));
            }
            temp[strlen(temp) - 1] = '\0';
            nwy_test_cli_echo("\r\n%s.\r\n", temp);
        }
        else
        {
            nwy_test_cli_echo("\r\n sms number=%d", sms_info.count);
            nwy_echo_sms_list_with_type(sms_info);
        }
    }

    NWY_CLI_LOG("=SMS= end to test read sms\n");

    return;
}

void nwy_test_cli_sms_del_type()
{
    char *sptr = NULL;
    int ret = 0;
    nwy_sms_msg_dflag_e delflag;
    int temp = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input delete sms type(1,read;2,read_send;3,send_unsend;4,all): ");
    temp = atoi(sptr);
    if(temp < NWY_OPEN_DEL_SMS_READ || temp > NWY_OPEN_DEL_SMS_ALL)
    {
        nwy_test_cli_echo("\r\n Input sms type  is invalid!");
        return;
    }
    delflag = nwy_test_del_sms_tab[temp-1];
    if (delflag == NWY_OPEN_UART_CHANNEL_NS)
    {
        nwy_test_cli_echo("\r\n Input sms type  not supported!");
        return;
    }

    ret =  nwy_sms_delete_message_by_type(delflag);

    if (NWY_SUCESS != ret) 
	{
        nwy_test_cli_echo("\r\nnwy delete sms by type fail!\r\n");
    } else {
        nwy_test_cli_echo("\r\nnwy finish to delete sms by type success\n");
    }

    NWY_CLI_LOG("=SMS= end to test delete  sms by type\n");
    return;
}

void nwy_sms_test_recv_cb(void *data, size_t size)
{
    memset(&sms_data, 0, sizeof(sms_data));
    nwy_sms_recv_message(&sms_data);

    nwy_test_cli_echo("\r\nnwy recv sms sms_data.cnmi_mt=%d\r\n", sms_data.cnmi_mt);
    if (sms_data.cnmi_mt == 1) {
        nwy_test_cli_echo("\r\nnwy recv sms sms_data.nIndex=%d, mt=%d\r\n", sms_data.nIndex, sms_data.cnmi_mt);
        nwy_test_cli_echo("\r\nnwy recv sms sms_data.nStorageId=%d\r\n", sms_data.nStorageId);
    } else if (sms_data.cnmi_mt == 2) {
        nwy_test_cli_echo("\r\nnwy recv sms oa len =%d,sms_data.oa=%s, dcs=%d, mt=%d, time %02d-%02d-%02d %02d:%02d:%02d+%d\r\n", \
            sms_data.oa_size, sms_data.oa, sms_data.dcs, sms_data.cnmi_mt,sms_data.scts.uYear, sms_data.scts.uMonth, sms_data.scts.uDay, \
            sms_data.scts.uHour, sms_data.scts.uMinute, sms_data.scts.uSecond, sms_data.scts.iZone);
        nwy_test_cli_echo("\r\nnwy recv sms pdata len =%d,sms_data.pdata=%s\r\n", sms_data.nDataLen, sms_data.pData);
    } else {
        nwy_test_cli_echo("\r\nnwy recv sms sms_data.cnmi_mt=%d invalid\r\n", sms_data.cnmi_mt);
    }

    nwy_test_cli_echo("=SMS= end to test recv sms\n");

    return;
}


#endif

#ifdef NWY_OPEN_TEST_POC_DS
int test_get_switch_poc_dsds()
{
    nwy_poc_dsds_onoff_type_e  on_off = NWY_POC_DSDS_TYPE_MAX;
    int ret = NWY_SUCCESS;

    on_off = nwy_poc_dsds_get_on_off();
    if(NWY_POC_DSDS_TYPE_MAX == on_off)
    {
        NWY_CLI_LOG("nwy switch poc dsds fail");
        return -1;
    }

    nwy_test_cli_echo("\r\n poc dsds switch on/off:%d", on_off);

    return 0;
}

int test_set_switch_poc_dsds()
{
    char *sptr = NULL;

    nwy_poc_dsds_onoff_type_e switch_type;
    int ret = NWY_SUCCESS;
    sptr = nwy_test_cli_input_gets("\r\nPlease input switch type: ");

    switch_type = atoi(sptr);

    ret = nwy_poc_dsds_set_on_off(switch_type);
    if(NWY_SUCCESS != ret)
    {
        NWY_CLI_LOG("nwy set poc dsds switch fail");
        return -1;
    }

    return 0;
}

int test_get_default_card()
{
    nwy_poc_dsds_defaultcard_type_e  default_card = NWY_POC_DSDS_DEFAULT_MAX;
    int ret = NWY_SUCCESS;

    default_card = nwy_poc_dsds_get_default_card();
    if(NWY_POC_DSDS_DEFAULT_MAX == default_card)
    {
        NWY_CLI_LOG("nwy get deault card fail");
        return -1;
    }

    nwy_test_cli_echo("\r\n get default card:%d", default_card);

    return 0;
}

int test_set_default_card()
{
    char* sptr = NULL;
    nwy_poc_dsds_defaultcard_type_e default_card;
    int ret = NWY_SUCCESS;
    sptr = nwy_test_cli_input_gets("\r\nPlease input default card: ");

    default_card = atoi(sptr);

    ret = nwy_poc_dsds_set_default_card(default_card);
    if(NWY_SUCCESS != ret)
    {
        NWY_CLI_LOG("nwy set default card fail");
        return -1;
    }

    return 0;
}

int test_get_auto_switch_card()
{
    nwy_poc_dsds_auto_switch_type_e  auto_switch = NWY_POC_DSDS_AUTO_SWITCH_MAX;
    int ret = NWY_SUCCESS;

    auto_switch = nwy_poc_dsds_get_auto_switch_card();
    if(NWY_POC_DSDS_AUTO_SWITCH_MAX == auto_switch)
    {
        NWY_CLI_LOG("nwy get auto switch card fail");
        return -1;
    }

    nwy_test_cli_echo("\r\n get auto switch card:%d", auto_switch);

    return 0;
}

int test_set_auto_switch_card()
{
    char* sptr = NULL;
    nwy_poc_dsds_auto_switch_type_e auto_switch;
    int ret = NWY_SUCCESS;
    sptr = nwy_test_cli_input_gets("\r\nPlease input auto switch: ");

    auto_switch = atoi(sptr);

    ret = nwy_poc_dsds_set_auto_switch_card(auto_switch);
    if(NWY_SUCCESS != ret)
    {
        NWY_CLI_LOG("nwy set auto switch card fail");
        return -1;
    }

    return 0;
}

int test_get_master_card()
{
    nwy_poc_dsds_mastercard_type_e  master_card = NWY_POC_DSDS_MASTER_NONE;
    int ret = NWY_SUCCESS;

    master_card = nwy_poc_dsds_get_mastercard_card();
    if(NWY_POC_DSDS_MASTER_NONE == master_card)
    {
        NWY_CLI_LOG("nwy get master card fail");
        return -1;
    }

    nwy_test_cli_echo("\r\n get master card:%d", master_card);

    return 0;
}

int test_set_master_card()
{
    char* sptr = NULL;
    nwy_poc_dsds_mastercard_type_e master_card;
    int ret = NWY_SUCCESS;
    sptr = nwy_test_cli_input_gets("\r\nPlease input master card: ");

    master_card = atoi(sptr);

    ret = nwy_poc_dsds_set_mastercard_card(master_card);
    if(NWY_SUCCESS != ret)
    {
        NWY_CLI_LOG("nwy set master card fail");
        return -1;
    }

    return 0;
}


void nwy_poc_dsds_test()
{
    char* sptr = NULL;
    int ret = 0;

    nwy_test_cli_echo("\r\nPlease select an option to test poc dsds below. \r\n");
    nwy_test_cli_echo("1. get poc dsds func on/off \r\n");
    nwy_test_cli_echo("2. set poc dsds func on/off \r\n");
    nwy_test_cli_echo("3. get poc dsds default card \r\n");
    nwy_test_cli_echo("4. set poc dsds default card \r\n");
    nwy_test_cli_echo("5. get poc dsds auto switch card \r\n");
    nwy_test_cli_echo("6. set poc dsds auto switch card \r\n");
    nwy_test_cli_echo("7. get poc dsds master card \r\n");
    nwy_test_cli_echo("8. set poc dsds master card \r\n");

    sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");

    switch(atoi(sptr))
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            ret = test_get_switch_poc_dsds();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n get switch poc dsds fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n get switch poc dsds success \r\n");
            }
            break;
        }
        case 2:
        {
            ret = test_set_switch_poc_dsds();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n set switch poc dsds fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n set switch poc dsds success \r\n");
            }
            break;
        }
        case 3:
        {
            ret = test_get_default_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n get default card fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n get default card success \r\n");
            }
            break;
        }
        case 4:
        {
            ret = test_set_default_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n set default card fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n set default card success \r\n");
            }
            break;
        }
        case 5:
        {
            ret = test_get_auto_switch_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n get auto switch card fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n get auto switch card success \r\n");
            }
            break;
        }
        case 6:
        {
            ret = test_set_auto_switch_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n set auto switch card fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n set auto switch card success \r\n");
            }
            break;
        }
        case 7:
        {
            ret = test_get_master_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n get master fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n get master success \r\n");
            }
            break;
        }
        case 8:
       {
            ret = test_set_master_card();
            if(0 != ret)
            {
                nwy_test_cli_echo("\r\n set master fail \r\n");
            }
            else
            {
                nwy_test_cli_echo("\r\n set master success \r\n");
            }
            break;
        }

        default:
            nwy_test_cli_echo("\r\n invalid input \r\n");
            break;
    }

}
#endif

void nwy_poc_pocds_test()
{
#ifndef NWY_OPEN_TEST_POC_DS
    nwy_test_cli_echo("\r\n Not support! \r\n");
#else
    nwy_poc_dsds_test();
#endif
    return;
}
