#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"



/**************************SIM*********************************/
void nwy_test_cli_get_sim_status()
{
    nwy_sim_status_e sim_status = NWY_SIM_UNKNOWN;
    int ret = 0;
    char *sptr = NULL;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    ret = nwy_sim_status_get(simid, &sim_status);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nget sim status fail\r\n");
        return;
    }
    switch (sim_status)
    {
    case NWY_SIM_READY:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_READY!\r\n");
        break;
    case NWY_SIM_NOT_INSERTED:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_NOT_INSERT!\r\n");
        break;
    case NWY_SIM_PIN_REQ:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_PIN1!\r\n");
        break;
    case NWY_SIM_PUK_REQ:
        nwy_test_cli_echo("\r\nNWY_SIM_STATUS_PUK1!\r\n");
        break;
    case NWY_SIM_BUSY:
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input pin string: ");
    memcpy(pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_verify_pin simid =%d,  pin=%s\r\n", simid, pin);

    ret = nwy_sim_pin_verify(simid, pin);
    if (NWY_SUCCESS != ret)
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
    int ret = 0;
    nwy_sim_pin_mode_e pin_mode = NWY_SIM_PIN_MODE_UNKOWN;
    nwy_sim_id_e simid = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    ret = nwy_sim_pin_mode_get(simid, &pin_mode);
    if (NWY_SUCCESS == ret)
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
    simid = (nwy_sim_id_e)atoi(sptr);
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input pin action:(0-disable/1-enable) ");
    action = atoi(sptr);
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input pin string: ");
    memcpy(pin, sptr, strlen(sptr));
    
    nwy_test_cli_echo("\r\nnwy_test_cli_set_pin_mode simid =%d, action=%d, pin=%s\r\n", simid, action, pin);
    if (action == 0) {
        result = nwy_sim_pin_disable(simid, pin);
    } else if (action == 1) {
        result = nwy_sim_pin_enable(simid, pin);
    }

    if (result == NWY_SUCCESS) {
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
    nwy_sim_id_e simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input old pin string: ");
    memcpy(pin, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input new pin string: ");
    memcpy(new_pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_change_pin simid =%d, pin=%s, new_pin=%s\r\n", simid, pin, new_pin);

    ret = nwy_sim_pin_change(simid, pin, new_pin);
    if (NWY_SUCCESS != ret)
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
    nwy_sim_id_e simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input puk string: ");
    memcpy(puk, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input new pin string: ");
    memcpy(new_pin, sptr, strlen(sptr));
    nwy_test_cli_echo("\r\nnwy_test_cli_verify_puk  simid =%d, puk=%d, new_pin=%s\r\n", simid, puk, new_pin);

    ret = nwy_sim_pin_unlock(simid, puk, new_pin);
    if (NWY_SUCCESS != ret)
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    result = nwy_sim_imsi_get(simid, imsi, sizeof(imsi));
    if (NWY_SUCCESS == result)
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    result = nwy_sim_iccid_get(simid, iccid, sizeof(iccid));
    NWY_CLI_LOG("++++ nwy_test_cli_get_iccid %d %s ++++++", result, iccid);
    if (NWY_SUCCESS == result)
    {
        nwy_thread_sleep(1000);
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
    nwy_sim_id_e simid;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    ret = nwy_sim_msisdn_get(simid, msisdn, 128);
    if (NWY_SUCCESS != ret)
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
    char *sptr = NULL;
    uint8 pin_time = 0;
    uint8 puk_time = 0;

    nwy_sim_id_e simid;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    ret = nwy_sim_pin_times_get(simid, &pin_time, &puk_time);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy get retry time  fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy get retry time success. pin:%d,puk:%d!\r\n", pin_time, puk_time);

    return;
}

void nwy_test_cli_get_sim_slot()
{
//#ifdef NWY_OPEN_TEST_SIM_SWTICH
#if 1
    uint8 nSwitchSimID = 0;
    int ret = 0;
    ret = nwy_sim_slot_get(&nSwitchSimID);
    if (NWY_SUCCESS == ret) {    
        nwy_test_cli_echo("\r\n simid: %d \r\n", nSwitchSimID);    
    } else {
        nwy_test_cli_echo("\r\n get sim slot fail, ret:%d \r\n", ret);    
    }
    return;
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#endif
}

void nwy_test_cli_set_sim_slot()
{
//#ifdef NWY_OPEN_TEST_SIM_SWTICH
#if 1
    char *sptr = NULL;
    uint8 nSwitchSimID = 0;
    int ret = 0;
    uint8 is_save = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input switch simid(0-1): ");
    nSwitchSimID = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input save option(0-1): ");
    is_save = atoi(sptr);

    ret = nwy_sim_slot_set(nSwitchSimID, is_save);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy set switch simid fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set switch simid success!\r\n");
    return;

#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#endif
}


void nwy_test_cli_set_sim_detect()
{
//#ifdef NWY_OPEN_TEST_SIM_DETECT
#if 1
    char *sptr = NULL;
    nwy_error_e ret = 0;
    uint8 simid = 0;
    uint8 detect_mode = 0;
    uint8 trig_mode = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input operate simid(0-1): ");
    simid = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input detect_mode (0-1): ");
    detect_mode = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input trig_mode (0-1): ");
    trig_mode = atoi(sptr);
    ret = nwy_sim_detect_set(simid, detect_mode, trig_mode);
    if (NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThis function can not support!");
    else if (NWY_SUCCESS != ret)
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
    nwy_sim_id_e simid = 0;
    char csim_cmd[512] = {0};
    char csim_resp[512] = {0};
    int ret = 0;

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

    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy set csim fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set csim success resp:%s\r\n", csim_resp);

    return;
#endif
}

void nwy_test_cli_set_sim_power_mode()
{
#ifdef NWY_OPEN_TEST_SIM_POWER_NS
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    return;
#else
    char *sptr = NULL;
    int nPowerMode = 0;
    nwy_sim_id_e simid = 0;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim power mode(0-powerdown/1-powerup/2-reset): ");
    nPowerMode = atoi(sptr);

    if(0 == nPowerMode || 1 == nPowerMode)
    {
        ret = nwy_sim_power_set(simid, nPowerMode);
    }
    else if(2 == nPowerMode)
    {
        ret = nwy_sim_reset(simid);
    }
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy set power mode fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set power mode success!\r\n");
    return;
#endif
}

void nwy_test_cli_sim_recv_cb(nwy_sim_id_e sim_id, nwy_sim_urc_type_e urc_type, nwy_sim_info_ind_t *ind_struct)
{
    nwy_test_cli_echo("\r\nrecv sim ind");
    if(NWY_SIM_URC_TYPE_STATUS == urc_type)
    {
        //do customer process
        if(0 == ind_struct->sim_detect_status)
        {
            nwy_test_cli_echo("\r\nsim absent");
        }
        else if(1 == ind_struct->sim_detect_status)
        {
            nwy_test_cli_echo("\r\nsim present");
        }
        nwy_test_cli_echo("\r\nrecv sim status:%d!\r\n", ind_struct->sim_status);
    }

    return;
}

void nwy_test_cli_sim_reg(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    int reg_flag = 0;
    char *sptr = NULL;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sim reg option: (0 - reg / 1 - unreg)");
    reg_flag = atoi(sptr);
    if(0 == reg_flag)
    {
        nwy_test_cli_echo("\r\nreg sim ind %s",(nwy_sim_urc_reg(simid, nwy_test_cli_sim_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else if(1 == reg_flag)
    {
        nwy_test_cli_echo("\r\nunreg sim ind %s",(nwy_sim_urc_unreg(simid, nwy_test_cli_sim_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else
    {
        nwy_test_cli_echo("\r\nwrong sim reg option\n");
    }

    return;
}