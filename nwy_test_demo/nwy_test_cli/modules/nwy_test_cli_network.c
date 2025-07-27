#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

static const char* NwyErrorToString(nwy_error_e code)
{
    switch (code)
    {
        case NWY_SUCCESS:
            return "SUCCESS";
        case NWY_FAIL:
            return "FAIL";
        case NWY_GEN_E_INVALID_PARA:
            return "INVALID_PARA";
        case NWY_GEN_E_SET_FAILED:
            return "SET_FAILED";
        case NWY_GEN_E_GET_FAILED:
            return "GET_FAILED";
        case NWY_GEN_E_SIM_NOT_READY:
            return "SIM_NOT_READY";
        case NWY_GEN_E_SIM_NOT_REG:
            return "SIM_NOT_REG";
        case NWY_GEN_E_ATT_NOT_ACT:
            return "ATT_NOT_ACT";
        case NWY_GEN_E_GPRS_NOT_ACT:
            return "GPRS_NOT_ACT";
        case NWY_GEN_E_AREADY_CONNECT:
            return "ALREADY_CONNECT";
        case NWY_GEN_E_PLAT_NOT_SUPPORT:
            return "PLAT_NOT_SUPPORT";
        case NWY_GEN_E_AREADY_DISCONNECT:
            return "ALREADY_DISCONNECT";
        case NWY_GEN_E_DSNET_NOT_ACTIVE:
            return "DSNET_NOT_ACTIVE";
        case NWY_GEN_E_ARG_NEVER_USED:
            return "ARG_NEVER_USED";
        case NWY_GEN_E_MALLOC_FAIL:
            return "MALLOC_FAIL";
        case NWY_GEN_E_RESOURE_FULL:
            return "RESOURCE_FULL";
        case NWY_GEN_E_TIMEOUT:
            return "TIMEOUT";
        default:
            return "Unknown Error Code";
    }
}

void nwy_test_cli_nw_get_mode(void)
{
    int network_mode = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    ret = nwy_nw_mode_get(simid, &network_mode);
     if (NWY_SUCCESS == ret)
    {
        nwy_test_cli_echo("\r\nCurrent Network mode: 0x%x\r\n", network_mode);
        if(network_mode == NWY_NW_MODE_MASK_AUTO)
        {
            nwy_test_cli_echo("\r\nNot locked, or only support one mode.\r\n");
            return ;
        }
        /* show the comon 3 mode in demo, suggest parsing bits while using */
        if (network_mode & (int)NWY_NW_MODE_MASK_GSM)
        {
            nwy_test_cli_echo("\r\n GSM locked.\r\n");
            network_mode -= (int)NWY_NW_MODE_MASK_GSM;
        }
        if (network_mode & (int)NWY_NW_MODE_MASK_WCDMA)
        {
            nwy_test_cli_echo("\r\nWCDMA locked\r\n");
            network_mode -= (int)NWY_NW_MODE_MASK_WCDMA;
        }
        if (network_mode & (int)NWY_NW_MODE_MASK_LTE)
        {
            nwy_test_cli_echo("\r\nLTE locked\r\n");
            network_mode -= (int)NWY_NW_MODE_MASK_LTE;
        }
        if(network_mode)
            nwy_test_cli_echo("\r\nOther mode locked\r\n");
    }
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
    {
        nwy_test_cli_echo("\r\nGet Network mode Failed, ret: %d\r\n", ret);
    }
}

void nwy_test_cli_nw_set_mode(void)
{
    nwy_nw_rat_type_e mode = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease select network mode(0-AUTO,4-GSM,16-LTE):");
    mode = (nwy_nw_rat_type_e)atoi(p_input);

    ret = nwy_nw_mode_set(simid, mode);
    nwy_test_cli_echo("\r\n%s", NwyErrorToString(ret));
}

void nwy_test_cli_nw_get_priband(void)
{
    nwy_nw_priband_t priband;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1, i = 0;
    char outbuf[30];
    char *p_outbuf = NULL;

    memset(&priband, 0x0, sizeof(nwy_nw_priband_t));
    memset(outbuf, 0x0, 30);
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    p_input = nwy_test_cli_input_gets("\r\nPlease input rat(4:GSM, 16:LTE): ");
    priband.rat = (nwy_nw_mode_mask_e)atoi(p_input);

    ret = nwy_nw_priband_get(simid, &priband);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nGet priband fail.");
    else
    {
        nwy_test_cli_echo("\r\nPriband num: %d", priband.band_nums);
        if(0 != priband.band_nums)
        {
            p_outbuf = &outbuf[0];
            sprintf(p_outbuf, "");
            for(i=0; i<priband.band_nums; i++)
            {
                if(i != priband.band_nums - 1)
                    p_outbuf += sprintf(p_outbuf, "%d,", priband.band[i]);
                else
                    p_outbuf += sprintf(p_outbuf, "%d", priband.band[i]);
            }
            nwy_test_cli_echo("\r\nPriband: %s", outbuf);
        }
    }
}

void nwy_test_cli_nw_set_priband(void)
{
    nwy_nw_priband_t priband;
    char delims[] = ",";
    char *result = NULL;
    int i = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    memset(&priband, 0x0, sizeof(nwy_nw_priband_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    p_input = nwy_test_cli_input_gets("\r\nPlease input mode(0:unlock, 1:lock): ");
    priband.mode = (nwy_nw_priband_mode_e)atoi(p_input);

    if(NWY_NW_PRI_BAND_LOCK == priband.mode)
    {
        p_input = nwy_test_cli_input_gets("\r\nPlease input rat(4:GSM, 16:LTE): ");
        priband.rat = (nwy_nw_mode_mask_e)atoi(p_input);
        p_input = nwy_test_cli_input_gets("\r\nPlease input priband string(ex:1,3,5): ");
        result = strtok(p_input, delims);
        while(result != NULL)
        {
            priband.band[i] = atoi(result);
            result = strtok(NULL, delims);
            i++;
        }
        priband.band_nums = i;
    }

    ret = nwy_nw_priband_set(simid, &priband);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat not support the func,or the rat support.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nSet priband fail.");
    else
        nwy_test_cli_echo("\r\nSet priband success.");
}

void nwy_test_cli_nw_get_ims_mode(void)
{
    uint8 ims_mode = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_ims_get(simid, &ims_mode);
    if( NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nNot support volte, or compile option not open");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nget ims mode error,ret = %d", ret);
    else
        nwy_test_cli_echo("\r\nims mode = %d", ims_mode);
}

void nwy_test_cli_nw_set_ims_mode(void)
{
    uint8 ims_mode = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease input ims_mode(0:disable, 1:enable): ");
    ims_mode = (uint8)atoi(p_input);

    ret = nwy_nw_ims_set(simid, ims_mode);
    if( NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nNot support volte, or compile option not open");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nset ims mode error,ret = %d", ret);
    else
        nwy_test_cli_echo("\r\nset ims mode success");
}

void nwy_test_cli_nw_get_radio_mode(void)
{
    nwy_nw_radio_mode_e radio_mode = NWY_NW_RADIO_NORMAL_MODE;
    int ret =-1;

    ret = nwy_nw_radio_get(&radio_mode);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nget radio mode error,ret = %d", ret);
    else
        nwy_test_cli_echo("\r\nradio mode = %d", radio_mode);
}

void nwy_test_cli_nw_set_radio_mode(void)
{
    nwy_nw_radio_mode_e radio_mode = NWY_NW_RADIO_NORMAL_MODE;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input radio_mode(0-min,1-normal,4-flight): ");
    radio_mode = (nwy_nw_radio_mode_e)atoi(p_input);
    ret= nwy_nw_radio_set(radio_mode);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nset radio mode error,ret = %d", ret);
    else
        nwy_test_cli_echo("\r\nset radio mode success");
    return ;
}

void nwy_test_cli_nw_get_edrx_info(void)
{
    nwy_test_cli_echo("\r\nThe patform can not support the function.");
    return ;
}

void nwy_test_cli_nw_set_edrx_info(void)
{
    nwy_test_cli_echo("\r\nThe patform can not support the function.");
    return ;
}

void nwy_test_cli_nw_get_psm_info(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;
    nwy_nw_psm_info_t psm_info;

    memset(&psm_info, 0x0, sizeof(nwy_nw_psm_info_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    ret = nwy_nw_psm_get(simid, &psm_info);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nget psm info error!");
    else
        nwy_test_cli_echo("\r\npsm mode: %d, T3412: %d, T3324: %d", \
                          psm_info.mode,psm_info.requested_Periodic_tau, \
                          psm_info.requested_active_timer);
}

void nwy_test_cli_nw_set_psm_info(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;
    nwy_nw_psm_info_t psm_info;

    memset(&psm_info, 0x0, sizeof(nwy_nw_psm_info_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease input mode(0-disable,1-enable,2-reset): ");
    psm_info.mode = (nwy_nw_psm_mode_e)atoi(p_input);
    if(NWY_NW_PSM_MODE_ENABLE == psm_info.mode)
    {
        p_input = nwy_test_cli_input_gets("\r\nPlease input T3412(ex.164-4min,170-10min): ");
        psm_info.requested_Periodic_tau = (uint8)atoi(p_input);
        p_input = nwy_test_cli_input_gets("\r\nPlease input T3324(ex.36-4min,42-10min): ");
        psm_info.requested_active_timer = (uint8)atoi(p_input);
    }

    ret = nwy_nw_psm_set(simid, &psm_info);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nset psm info error!");
    else
        nwy_test_cli_echo("\r\nset psm info success!");
}

void nwy_test_cli_nw_get_custom_cfg(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;
    nwy_nw_config_info_u cfg_info = {0};
    nwy_nw_config_type_e cfg_option = 1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease input cfg_option(0-netauto,1-uemode,2-dataonly,4-bandcap,5-nitz): ");
    cfg_option = (nwy_nw_config_type_e)atoi(p_input);
    switch(cfg_option)
    {
        case NWY_NW_CONFIG_RW_NETAUTO:
        {
            ret = nwy_nw_config_get(simid, NWY_NW_CONFIG_RW_NETAUTO, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nget netauto error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nnetauto switch: %d, timer: %d min", \
                cfg_info.netauto.onoff, cfg_info.netauto.timer);

            break;
        }
        case NWY_NW_CONFIG_RW_UEMODE:
        {
            ret = nwy_nw_config_get(simid, NWY_NW_CONFIG_RW_UEMODE, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nget uemode error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nuemode = %d", cfg_info.uemode);
            break;
        }
        case NWY_NW_CONFIG_RW_DATAONLY:
        {
            ret = nwy_nw_config_get(simid, NWY_NW_CONFIG_RW_DATAONLY, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nget dataonly error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\ndataonly switch: %d", cfg_info.dataonly);
            break;
        }
        case NWY_NW_CONFIG_RW_ANTAUX:
        {
            nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            break;
        }
        case NWY_NW_CONFIG_R_BAND_CAP:
        {
            int num =0 ;
            int i = 0,j = 0;
            char echo_str[256] = {0};
            int echo_len = 0;
            ret = nwy_nw_config_get(simid, NWY_NW_CONFIG_R_BAND_CAP, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nget band_capability error,ret = %d", ret);
            else
            {
                if(cfg_info.net_capacity.supported_rat_num > 0)//total rat num
                {
                    for(i = 0; i < NWY_NW_SUPPORTED_RAT_MAX; i++)
                    {
                        if(cfg_info.net_capacity.supported_detail[i].support_rat & NWY_NW_MODE_MASK_LTE)
                        {
                            memset(&echo_str,0,sizeof(echo_str));
                            echo_len = 0;
                            num = cfg_info.net_capacity.supported_detail[i].band_nums;
                            if(num > 0)
                            {
                                echo_len += sprintf(echo_str + echo_len,"Get %d LTE band in total:", num);
                                for(j = 0;j < num; j++)
                                {
                                    if(j == num - 1)
                                    {
                                        echo_len += sprintf(echo_str + echo_len,"%u", cfg_info.net_capacity.supported_detail[i].band[j]);
                                    }
                                    else
                                    {
                                        echo_len += sprintf(echo_str + echo_len,"%u,", cfg_info.net_capacity.supported_detail[i].band[j]);
                                    }
                                }
                                nwy_test_cli_echo("\r\n%s", echo_str);
                            }
                        }
                        else if(cfg_info.net_capacity.supported_detail[i].support_rat & NWY_NW_MODE_MASK_WCDMA)
                        {
                            //other band
                        }
                        else
                        {
                            //error
                        }
                    }
                }
            }
            break;
        }
        case NWY_NW_CONFIG_RW_NITZ_ONOFF:
        {
            ret = nwy_nw_config_get(simid, NWY_NW_CONFIG_RW_NITZ_ONOFF, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nset nitz error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nnitz = %d", cfg_info.nitz_onoff);
            break;
        }
        default:
            nwy_test_cli_echo("\r\nabnormal option");
            break;
    }
}

void nwy_test_cli_nw_set_custom_cfg(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;
    nwy_nw_config_info_u cfg_info = {0};
    nwy_nw_config_type_e cfg_option = 1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease input cfg_option(0-netauto,1-uemode,2-dataonly,5-nitz): ");
    cfg_option = (nwy_nw_config_type_e)atoi(p_input);
    switch(cfg_option)
    {
        case NWY_NW_CONFIG_RW_NETAUTO:
        {
            p_input = nwy_test_cli_input_gets("\r\nPlease input netauto switch(0:disable, 1:enable): ");
            cfg_info.netauto.onoff = (int8)atoi(p_input);
            p_input = nwy_test_cli_input_gets("\r\nPlease input netauto timer(default:3): ");
            cfg_info.netauto.timer = (int8)atoi(p_input);

            ret = nwy_nw_config_set(simid, NWY_NW_CONFIG_RW_NETAUTO, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nset netauto error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nset netauto success!");

            break;
        }
        case NWY_NW_CONFIG_RW_UEMODE:
        {
            p_input = nwy_test_cli_input_gets("\r\nPlease input uemode: ");
            cfg_info.uemode = (nwy_nw_cfg_uemode_type_e)atoi(p_input);
            
            ret = nwy_nw_config_set(simid, NWY_NW_CONFIG_RW_UEMODE, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nset uemode error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nset uemode success!");
            break;
        }
        case NWY_NW_CONFIG_RW_DATAONLY:
        {
            p_input = nwy_test_cli_input_gets("\r\nPlease input dataonly switch(0:disable, 1:enable): ");
            cfg_info.dataonly = (nwy_nw_cfg_switch_type_e)atoi(p_input);

            ret = nwy_nw_config_set(simid, NWY_NW_CONFIG_RW_DATAONLY, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nset dataonly error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nset dataonly success!");
            break;
        }
        case NWY_NW_CONFIG_RW_ANTAUX:
        {
            nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            break;
        }
        case NWY_NW_CONFIG_RW_NITZ_ONOFF:
        {
            p_input = nwy_test_cli_input_gets("\r\nPlease input nitz:(0-disable,1-enable) ");
            cfg_info.nitz_onoff = (nwy_nw_cfg_switch_type_e)atoi(p_input);

            ret = nwy_nw_config_set(simid, NWY_NW_CONFIG_RW_NITZ_ONOFF, &cfg_info);
            if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
                nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
            else if(NWY_SUCCESS != ret)
                nwy_test_cli_echo("\r\nset nitz error,ret = %d", ret);
            else
                nwy_test_cli_echo("\r\nnitz = %d", cfg_info.nitz_onoff);
            break;
        }
        default:
            nwy_test_cli_echo("\r\nabnormal option");
            break;
    }
}

void nwy_test_cli_nw_get_oper_info(void)
{
    nwy_nw_operator_t oper_info;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    memset(&oper_info, 0x0, sizeof(nwy_nw_operator_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_operator_get(simid, &oper_info);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
    {
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
        return;
    }
    else if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget operator info error,ret = %d", ret);
        return ;
    }
    else
    {
        nwy_test_cli_echo("\r\nLong EONS: %s\r\n"
                          "Short EONS: %s\r\n"
                          "MCC and MNC: %s %s\r\n"
                          "SPN: %s\r\n"
                          "SPN ENC: %d\r\n",
                          oper_info.long_eons,
                          oper_info.short_eons,
                          oper_info.mcc,
                          oper_info.mnc,
                          oper_info.spn,
                          oper_info.spn_enc);
    }
}

void nwy_test_cli_nw_get_csq(void)
{
    nwy_nw_get_csq_info_t csq_val;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_csq_get(simid, &csq_val);
    nwy_test_cli_echo("\r\n%s", NwyErrorToString(ret));
    if(ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nCSQ: rssi_level:%d,ber:%d", csq_val.csq_rssi_level, csq_val.ber);
    }
}

void nwy_test_cli_nw_get_signal_info(void)
{
    nwy_nw_signal_info_t info;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    memset(&info, 0x0, sizeof(nwy_nw_signal_info_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_signal_get(simid, &info);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget operator info error,ret = %d", ret);
        return ;
    }
    nwy_test_cli_echo("\r\nCurrent rat = %d", info.rat);
    if(info.rat == NWY_NW_RAT_NONE)
    {
        nwy_test_cli_echo("\r\nnot reg,rat = %d", info.rat);
    }
    if(info.rat == NWY_NW_RAT_GSM)
    {
        nwy_test_cli_echo("\r\nGSM signal info:rssi:%d, ber:%d\r\n",
                          info.rat_signal_info.gsm_signal_info.rssi,
                          info.rat_signal_info.gsm_signal_info.ber);
    }
    if(info.rat == NWY_NW_RAT_WCDMA)
    {
        nwy_test_cli_echo("\r\nWCDMA signal info:rscp:%d, ecio:%d, rssi:%d\r\n",
                          info.rat_signal_info.wcdma_signal_info.rscp,
                          info.rat_signal_info.wcdma_signal_info.ecio,
                          info.rat_signal_info.wcdma_signal_info.rssi);
    }
    if(info.rat == NWY_NW_RAT_LTE)
    {
        nwy_test_cli_echo("\r\nLTE signal info:rsrp:%d, rsrq:%d, rssi:%d, sinr:%d\r\n",
                          info.rat_signal_info.lte_signal_info.rsrp,
                          info.rat_signal_info.lte_signal_info.rsrq,
                          info.rat_signal_info.lte_signal_info.rssi,
                          info.rat_signal_info.lte_signal_info.sinr);
    }
}

void nwy_test_cli_nw_reg_info(void)
{
    
    nwy_nw_regstatus_t reg_info;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    memset(&reg_info, 0x0, sizeof(nwy_nw_regstatus_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_regstatus_get(simid, &reg_info);
    if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget reg info error,ret = %d", ret);
        return ;
    }
    else
    {
        nwy_test_cli_echo("\r\ncurrent rat: %d\r\n", reg_info.ps_regs.rat_type);
        if (1 == reg_info.ps_regs_valid)
        {
            nwy_test_cli_echo("\r\nPS Domain Reg state: %d\r\n"
                              "PS Domain Roam state: %d\r\n"
                              "PS Domain Radio Tech: %d\r\n",
                              reg_info.ps_regs.regs_state,
                              reg_info.ps_regs.roam_state,
                              reg_info.ps_regs.rat_type);
            if(1 == reg_info.reject_info.mm_regs_reject_info_valid)
            {
                nwy_test_cli_echo("\r\nReject code: %d\r\n",
                              reg_info.reject_info.mm_regs_rej_cause);
            }
        }
        if (1 == reg_info.cs_regs_valid)
        {
            nwy_test_cli_echo("\r\nCS Domain Reg state: %d\r\n"
                              "CS Domain Roam state: %d\r\n"
                              "CS Domain Radio Tech: %d\r\n",
                              reg_info.cs_regs.regs_state,
                              reg_info.cs_regs.roam_state,
                              reg_info.cs_regs.rat_type);
            if(1 == reg_info.reject_info.mm_regs_reject_info_valid)
            {
                nwy_test_cli_echo("\r\nReject code: %d\r\n",
                              reg_info.reject_info.mm_regs_rej_cause);
            }
        }
    }
}

void nwy_test_cli_nw_get_cellinfo(void)
{
    nwy_nw_cellinfo_t info;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    nwy_nw_cellinfo_mode_e scan_mode = NWY_NW_GET_ALL_CELL;
    char *p_input = NULL;
    int i = 0, ret =-1;

    memset(&info, 0x0, sizeof(nwy_nw_cellinfo_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0-sim1,1-sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    p_input = nwy_test_cli_input_gets("\r\nPlease input scan_mode:(0-scell,1-all_cell) ");
    scan_mode = (nwy_nw_cellinfo_mode_e)atoi(p_input);

    ret = nwy_nw_cellinfo_get(simid, scan_mode, &info);
    if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget cellinfo error,ret = %d", ret);
        return ;
    }

    nwy_test_cli_echo("\r\ncurrent rat = %d", info.sub_rat);
    switch(info.sub_rat)
    {
        case NWY_RAT_SUB_TDD_LTE:
        case NWY_RAT_SUB_FDD_LTE:
        case NWY_RAT_SUB_CATM:
        case NWY_RAT_SUB_NBIOT:
        case NWY_RAT_SUB_LTE_TO_5GC:
        case NWY_RAT_SUB_CDMA1X_LTE:
            if (info.cell_info.lte_cell.serv_cell.plmn.mnc_includes_pcs_digit)
            {
                nwy_test_cli_echo("\r\nLte Serving cell: mcc:%d,mnc:%03d,tac:%x,earfcn:%d,cellid:%x,pci:%d,band:%d,\
                                bandwidth:%d,isTdd;%d,rsrp:%d,rsrq:%d,rssi:%d,sinr:%d,txpower:%d,rxpower:%d\r\n",\
                             info.cell_info.lte_cell.serv_cell.plmn.mcc,\
                             info.cell_info.lte_cell.serv_cell.plmn.mnc,\
                             info.cell_info.lte_cell.serv_cell.tac,\
                             info.cell_info.lte_cell.serv_cell.earfcn,\
                             info.cell_info.lte_cell.serv_cell.cell_id,\
                             info.cell_info.lte_cell.serv_cell.pci,\
                             info.cell_info.lte_cell.serv_cell.band,\
                             info.cell_info.lte_cell.serv_cell.dlBandwidth,\
                             info.cell_info.lte_cell.serv_cell.isTdd,\
                             info.cell_info.lte_cell.serv_cell.rsrp,\
                             info.cell_info.lte_cell.serv_cell.rsrq,\
                             info.cell_info.lte_cell.serv_cell.rssi,\
                             info.cell_info.lte_cell.serv_cell.sinr,\
                             info.cell_info.lte_cell.serv_cell.TxPower,\
                             info.cell_info.lte_cell.serv_cell.RxPower);
            }
            else
            {
                nwy_test_cli_echo("\r\nLte Serving cell: mcc:%d,mnc:%02d,tac:%x,earfcn:%d,cellid:%x,pci:%d,band:%d,\
                                bandwidth:%d,isTdd;%d,rsrp:%d,rsrq:%d,rssi:%d,sinr:%d,txpower:%d,rxpower:%d\r\n",\
                             info.cell_info.lte_cell.serv_cell.plmn.mcc,\
                             info.cell_info.lte_cell.serv_cell.plmn.mnc,\
                             info.cell_info.lte_cell.serv_cell.tac,\
                             info.cell_info.lte_cell.serv_cell.earfcn,\
                             info.cell_info.lte_cell.serv_cell.cell_id,\
                             info.cell_info.lte_cell.serv_cell.pci,\
                             info.cell_info.lte_cell.serv_cell.band,\
                             info.cell_info.lte_cell.serv_cell.dlBandwidth,\
                             info.cell_info.lte_cell.serv_cell.isTdd,\
                             info.cell_info.lte_cell.serv_cell.rsrp,\
                             info.cell_info.lte_cell.serv_cell.rsrq,\
                             info.cell_info.lte_cell.serv_cell.rssi,\
                             info.cell_info.lte_cell.serv_cell.sinr,\
                             info.cell_info.lte_cell.serv_cell.TxPower,\
                             info.cell_info.lte_cell.serv_cell.RxPower);
            }
            if(NWY_NW_GET_ALL_CELL == scan_mode)
            {
                nwy_test_cli_echo("LTE Ncell num: %d", info.cell_info.lte_cell.lte_nbr_len);
                for (i = 0; i < info.cell_info.lte_cell.lte_nbr_len; i++)
                {
                    if (info.cell_info.lte_cell.lte_nbr_cell[i].plmn.mnc_includes_pcs_digit)
                    {
                        nwy_test_cli_echo("\r\nLte Neighbour cell %d: mcc:%d,mnc:%03d,tac:%x,cellid:%x,pci:%d,\
                                            earfcn:%d,rsrp:%d,rsrq:%d,rssi:%d,sinr:%d,srxlev:%d\r\n", i,
                                     info.cell_info.lte_cell.lte_nbr_cell[i].plmn.mcc,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].plmn.mnc,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].tac,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].cellid,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].pci,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].earfcn,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rsrp,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rsrq,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rssi,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].sinr,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].srxlev);
                    }
                    else
                    {
                        nwy_test_cli_echo("\r\nLte Neighbour cell %d: mcc:%d,mnc:%02d,tac:%x,cellid:%x,pci:%d,\
                                            earfcn:%d,rsrp:%d,rsrq:%d,rssi:%d,sinr:%d,srxlev:%d\r\n", i,
                                     info.cell_info.lte_cell.lte_nbr_cell[i].plmn.mcc,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].plmn.mnc,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].tac,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].cellid,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].pci,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].earfcn,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rsrp,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rsrq,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].rssi,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].sinr,\
                                     info.cell_info.lte_cell.lte_nbr_cell[i].srxlev);
                    }
                }
            }
            break;
        default:
            nwy_test_cli_echo("\r\nCell info Scan failed\r\n");
            break;
    }
}

void nwy_test_cli_nw_set_lock(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    char *mode = NULL;
    char *bandnumber = NULL;
    int ret =-1;
    char *rat = NULL;
    uint16_t band[6] = {0};
    nwy_nw_lock_t p_lock_info;
    char delims[] = ",";
    int i = 0;
    char *result = NULL;

    memset(&p_lock_info, 0x0, sizeof(nwy_nw_lock_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0-sim1,1-sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    mode = nwy_test_cli_input_gets("\r\nPlease input lock mode(0-auto,1-lock_band,2-lock_freq,3-lock_pci,4-unlock_band,5-unlock_freq,6-unlock_pci): ");
    p_lock_info.mode = atoi(mode);
    rat = nwy_test_cli_input_gets("\r\nPlease input lock rat:(16-LTE) ");
    p_lock_info.rat = atoi(rat);
    if(p_lock_info.mode == NWY_NW_LOCK_MODE_UNLOCK || 
       p_lock_info.mode == NWY_NW_LOCK_MODE_UNLOCK_BAND ||
       p_lock_info.mode == NWY_NW_LOCK_MODE_UNLOCK_FREQ)
    {
        ret = nwy_nw_lock_set(simid,&p_lock_info);
        nwy_test_cli_echo("\r\n%s", NwyErrorToString(ret));
    }
    else if(p_lock_info.mode == NWY_NW_LOCK_MODE_LOCK_BAND)
    {
        p_input = nwy_test_cli_input_gets("\r\nPlease input band string(ex:1,3,5): ");
        result = strtok(p_input, delims);
        while(result != NULL)
        {
            p_lock_info.lock_info.lock_band.band[i] = atoi(result);
            result = strtok(NULL, delims);
            i++;
        }
        p_lock_info.lock_info.lock_band.band_nums = i;
        ret = nwy_nw_lock_set(simid,&p_lock_info);
        if(NWY_SUCCESS != ret)
        {
            nwy_test_cli_echo("\r\nset band error,ret = %d", ret);
        }
        else
        {
            nwy_test_cli_echo("\r\nnw_lock_set success");
        }
    }
    else if(p_lock_info.mode == NWY_NW_LOCK_MODE_LOCK_FREQ)
    {
        p_input = nwy_test_cli_input_gets("\r\nPlease input freq string(ex:100,38400): ");
        result = strtok(p_input, delims);
        while(result != NULL)
        {
            p_lock_info.lock_info.lock_freq.freq[i] = atoi(result);
            result = strtok(NULL, delims);
            i++;
        }
        p_lock_info.lock_info.lock_freq.freq_nums = i;

        ret = nwy_nw_lock_set(simid,&p_lock_info);
        if(NWY_SUCCESS != ret)
        {
            nwy_test_cli_echo("\r\nset freq error,ret = %d", ret);
        }
        else
        {
            nwy_test_cli_echo("\r\nnw_lock_set success");
        }
    }
    else
        nwy_test_cli_echo("\r\n unsupport mode value \r\n");
}
void nwy_test_cli_nw_get_lock(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    char *mode = NULL;
    int ret =-1;
    int i;
    nwy_nw_lock_t p_lock_info;
    memset(&p_lock_info, 0x0, sizeof(nwy_nw_lock_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0-sim1,1-sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);
    mode = nwy_test_cli_input_gets("\r\nPlease input lock mode(1-band_lock_info,2-freq_lock_info): ");
    p_lock_info.rat = NWY_NW_MODE_MASK_LTE;
    p_lock_info.mode = atoi(mode);
    char buf1[50] = {0};
    int len = 0;
    ret = nwy_nw_lock_get(simid,&p_lock_info);
    if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget_bandlock error,ret = %d", ret);
    }
    else
    {
        nwy_test_cli_echo("\r\nget_bandlock mode = %d", p_lock_info.mode);
        if (p_lock_info.mode == NWY_NW_LOCK_MODE_LOCK_BAND)
        {   
            if(p_lock_info.lock_info.lock_band.band_nums == 0)
            {
                nwy_test_cli_echo("\r\nbandlock: NONE");
            }
            else
            {
                len = snprintf(buf1,sizeof(buf1)-len,"bandlock: ");
                for (i=0; i<p_lock_info.lock_info.lock_band.band_nums;i++)
                {
                    len += snprintf(buf1+len, sizeof(buf1)-len, "%d,", p_lock_info.lock_info.lock_band.band[i]);
                }
                buf1[len-1] = 0;  
                nwy_test_cli_echo("\r\n%s\r\n", buf1);
            }
        }
        else if(p_lock_info.mode == NWY_NW_LOCK_MODE_LOCK_FREQ)
        {
            if(p_lock_info.lock_info.lock_freq.freq_nums == 0)
            {
                nwy_test_cli_echo("\r\nfreqlock: NONE");
            }
            else
            {
                len = snprintf(buf1,sizeof(buf1)-len,"freqlock: ");
                for (i=0; i<p_lock_info.lock_info.lock_freq.freq_nums;i++)
                {
                    len += snprintf(buf1+len, sizeof(buf1)-len, "%d,", p_lock_info.lock_info.lock_freq.freq[i]);
                }
                buf1[len-1] = 0;  
                nwy_test_cli_echo("\r\n%s\r\n", buf1);
            }
        }
        else
        {
            nwy_test_cli_echo("\r\ninvalid mode");
        }
    }
}

#define TBL_SIZE(tbl) (sizeof(tbl) / sizeof(tbl[0]))
static int scan_finish = 0;
static nwy_nw_scan_rsp_list_t manul_scan_list;


static char *scan_net_status[4] =
{
    "UNKNOWN",
    "AVAILABLE",
    "CURRENT",
    "FORBIDDEN"
};

static const uint8 cmcc_plmn[][6] =
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
static const uint8 unicom_plmn[][6] =
{
    "46001",
    "46006",
    "46009",
    "46010",
};
static const uint8 ct_plmn[][6] =
{
    "46003",
    "46005",
    "46011",
    "46012",
    "46059",
    "45502",
    "45507",
};
static const uint8 cb_plmn[][6] =
{
    "46015",
};

static const char* scan_net_rat_mode(nwy_nw_mode_mask_e mode)
{
    switch (mode)
    {
        case NWY_NW_MODE_MASK_AUTO:
            return "unlocked mode";
        case NWY_NW_MODE_MASK_CDMA_1X:
            return "CDMA 1X";
        case NWY_NW_MODE_MASK_CDMA_HDR:
            return "CDMA HDR";
        case NWY_NW_MODE_MASK_GSM:
            return "GSM";
        case NWY_NW_MODE_MASK_WCDMA:
            return "WCDMA";
        case NWY_NW_MODE_MASK_LTE:
            return "LTE (EUTRAN, Not included CATM1 and NB)";
        case NWY_NW_MODE_MASK_TDSCDMA:
            return "TD-SCDMA";
        case NWY_NW_MODE_MASK_SA:
            return "SA";
        case NWY_NW_MODE_MASK_CATM:
            return "CAT-M1";
        case NWY_NW_MODE_MASK_NB:
            return "NB-IoT";
        case NWY_NW_MODE_MASK_GSM_WCDMA:
            return "GSM + WCDMA";
        case NWY_NW_MODE_MASK_GSM_LTE:
            return "GSM + LTE";
        case NWY_NW_MODE_MASK_WCDMA_LTE:
            return "WCDMA + LTE";
        case NWY_NW_MODE_MASK_GSM_WCDMA_LTE:
            return "GSM + WCDMA + LTE";
        case NWY_NW_MODE_MASK_GSM_SA:
            return "GSM + SA";
        case NWY_NW_MODE_MASK_WCDMA_SA:
            return "WCDMA + SA";
        case NWY_NW_MODE_MASK_LTE_SA_NSA:
            return "LTE + SA + NSA";
        case NWY_NW_MODE_MASK_WCDMA_LTE_SA_NSA:
            return "WCDMA + LTE + SA + NSA";
        case NWY_NW_MODE_MASK_GSM_LTE_SA_NSA:
            return "GSM + LTE + SA + NSA";
        case NWY_NW_MODE_MASK_NSA:
            return "LTE + NSA";
        default:
            return "Unknown network mode";
    }
}

static int nwy_network_util_get_oper_type(char *plmn_buf)
{
    int i;
    if (plmn_buf == NULL)
        return 4;
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

    for (i = 0; i < TBL_SIZE(cb_plmn); i++)
    {
        if (strncmp(plmn_buf, (char *)cb_plmn[i], strlen((char *)cb_plmn[i])) == 0)
        {
            return 3;
        }
    }
    return 4;
}

void nwy_network_test_nw_select(nwy_sim_id_e simid)
{
    char imsi[20] = {0};
    char plmn_str[10] = "";
    int i = 0, j = 0, num = 0;
    int opertor_type = 3;
    char *sub_opt = NULL;
    int id = 0;
    int index[10];
    nwy_nw_select_param_t param = {0};

    if (nwy_sim_imsi_get(simid,imsi,sizeof(imsi)) == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\n imsi: %s",imsi);
        nwy_thread_sleep(1000); //wait for imsi result
        opertor_type = nwy_network_util_get_oper_type(imsi);
        nwy_test_cli_echo("\r\nOperator type: %d(0-CMCC,1-CU,2-CT,3-CB,4-OTHER),Current netwrok list: ", opertor_type);
        for (i = 0; i < manul_scan_list.net_num; i++)
        {
            memset(plmn_str, 0x00, sizeof(plmn_str));
            sprintf(plmn_str, "%s%s", manul_scan_list.net_list[i].net_name.mcc,
                    manul_scan_list.net_list[i].net_name.mnc);
            if (nwy_network_util_get_oper_type(plmn_str) == opertor_type)
            {
                index[num++] = i;
                nwy_test_cli_echo("\r\nIndex:%d Rat:%s", i, scan_net_rat_mode(manul_scan_list.net_list[i].net_rat));
                nwy_test_cli_echo("MCC-MNC:%s-%s", manul_scan_list.net_list[i].net_name.mcc, manul_scan_list.net_list[i].net_name.mnc);
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
        memcpy(param.mcc, manul_scan_list.net_list[id].net_name.mcc, sizeof(param.mcc));
        memcpy(param.mnc, manul_scan_list.net_list[id].net_name.mnc, sizeof(param.mnc));
        param.net_rat = manul_scan_list.net_list[id].net_rat;

        nwy_nw_manual_select(simid, &param);
        nwy_test_cli_echo("\r\nSelect index %d network over!!!", id);
    }
    else
    {
        nwy_test_cli_echo("\r\nGet Current operator type FAILED!!!");
    }
    return;
}

static void nwy_network_test_scan_cb(
    nwy_nw_scan_rsp_list_t *net_list)
{
    int i;

    NWY_SDK_LOG_INFO("%s", __func__);
    if (net_list == NULL)
        return;
    nwy_test_cli_echo("\r\nManual Scan Result %s\r\n", (net_list->result == NWY_SUCCESS) ? "Success" : "Failed");
    if (net_list->result != NWY_SUCCESS)
        return;
    else
    {
        for (i = 0; i < net_list->net_num; i++)
        {
            nwy_test_cli_echo("\r\nIndex: %d\r\n", i);
            nwy_test_cli_echo("MCC-MNC:%s-%s\r\n", net_list->net_list[i].net_name.mcc, net_list->net_list[i].net_name.mnc);
            nwy_test_cli_echo("Long EONS:%s\r\n", net_list->net_list[i].net_name.long_eons);
            nwy_test_cli_echo("Short EONS:%s\r\n", net_list->net_list[i].net_name.short_eons);
            nwy_test_cli_echo("Net Status:%s\r\n", scan_net_status[net_list->net_list[i].net_status]);
            nwy_test_cli_echo("Net Rat:%s\r\n", scan_net_rat_mode(net_list->net_list[i].net_rat));
        }
    }
    nwy_test_cli_echo("\r\nStart Test Select Network...\r\n");
    memset(&manul_scan_list, 0x00, sizeof(manul_scan_list));
    memcpy(&manul_scan_list, net_list, sizeof(nwy_nw_scan_rsp_list_t));
    scan_finish = 1;
}

void nwy_test_cli_nw_manual_scan(void)
{
    int count = 0;
    scan_finish = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    nwy_test_cli_echo("\r\nWaiting Scan result...");
    ret = nwy_nw_scan(simid, nwy_network_test_scan_cb);
    if (NWY_SUCCESS == ret)
    {
        while (1)
        {
            if (scan_finish == 1 || count == 180) //limit 3 minute
                break;
            nwy_thread_sleep(1000);
            count++;
        }

        if (manul_scan_list.result == NWY_SUCCESS)
        {
            nwy_network_test_nw_select(simid);
        }
        else
            nwy_test_cli_echo("\r\nScan network Failed!!!\r\n");
    }
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
        nwy_test_cli_echo("\r\nScan network Failed, ret: %d\r\n", ret);
}

void nwy_test_cli_nw_get_none_eitf(void)
{
    int none_eitf;
    nwy_error_e ret = nwy_dm_nwcfg_get("none_eitf", (void *)&none_eitf);
    if(ret == NWY_SUCCESS)
        nwy_test_cli_echo("\r\nnone_eitf: %d\r\n", none_eitf);
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
        nwy_test_cli_echo("\r\nnone_eitf get failed\r\n");
}

void nwy_test_cli_nw_set_none_eitf(void)
{
    int none_eitf;
    char *p_input = NULL;

    p_input = nwy_test_cli_input_gets("\r\nPlease input none_eitf(0:disable, 1:enable): ");
    none_eitf = (nwy_sim_id_e)atoi(p_input);
    nwy_error_e ret = nwy_dm_nwcfg_set("none_eitf", (void *)&none_eitf);

    if(ret == NWY_SUCCESS)
        nwy_test_cli_echo("\r\nnone_eitf set succeed\r\n");
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
        nwy_test_cli_echo("\r\nnone_eitf set failed\r\n");
}

void nwy_test_cli_nw_get_fplmn_list(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    nwy_nw_plmn_list_t fplmn_list;
    char *p_input = NULL;
    int ret =-1;
    int i = 0;

    memset(&fplmn_list, 0, sizeof(nwy_nw_plmn_list_t));
    p_input = nwy_test_cli_input_gets("\r\nPlease input simid(0:sim1, 1:sim2): ");
    simid = (nwy_sim_id_e)atoi(p_input);

    ret = nwy_nw_get_forbidden_plmn(simid, &fplmn_list);
    if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else if(NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\nget fplmn list error,ret = %d", ret);
        return ;
    }
    else
    {
        nwy_test_cli_echo("\r\nfplmn list num:%d\r\n", fplmn_list.num);
        if(0 != fplmn_list.num)
        {
            for(i=0; i<fplmn_list.num; i++)
            {
                if(0 == fplmn_list.fplmn[i].mnc_includes_pcs_digit)
                {
                    nwy_test_cli_echo("\r\nfplmn[%d]:mcc[%d] mnc[%02d]\r\n", i, fplmn_list.fplmn[i].mcc, fplmn_list.fplmn[i].mnc);
                }
                else if(1 == fplmn_list.fplmn[i].mnc_includes_pcs_digit)
                {
                    nwy_test_cli_echo("\r\nfplmn[%d]:mcc[%d] mnc[%03d]\r\n", i, fplmn_list.fplmn[i].mcc, fplmn_list.fplmn[i].mnc);
                }
            }
        }
    }
}
static const char* get_network_type_to_string(nwy_nw_regs_ind_type_e type)
{
    switch(type)
    {
        case NWY_NW_REGS_DATA_IND:
            return "REGS DATA IND";
        case NWY_NW_REGS_VOICE_IND:
            return "REGS VOICE IND";
        default:
            return "UNKNOW IND";
    }
}

static const char* get_regstate_to_string(nwy_nw_service_type_e type)
{
    switch(type)
    {
        case NWY_NW_SERVICE_NONE:
            return "Out of service";
        case NWY_NW_SERVICE_LIMITED:
            return "limited service,only for emergency call";
        case NWY_NW_SERVICE_FULL:
            return "Service full,reg success";
        default:
            return "Uknown Service type";
            break;
    }
}

static const char* get_rat_to_string(nwy_nw_rat_type_e rat_type)
{
    switch (rat_type)
    {
        case NWY_NW_RAT_NONE:   return "NWY_NW_RAT_NONE";
        case NWY_NW_RAT_GSM:    return "NWY_NW_RAT_GSM";
        case NWY_NW_RAT_WCDMA:  return "NWY_NW_RAT_WCDMA";
        case NWY_NW_RAT_LTE:    return "NWY_NW_RAT_LTE";
        case NWY_NW_RAT_CATM:   return "NWY_NW_RAT_CATM";
        case NWY_NW_RAT_NBIoT:  return "NWY_NW_RAT_NBIoT";
        case NWY_NW_RAT_NR:     return "NWY_NW_RAT_NR";
        case NWY_NW_RAT_CDMA:   return "NWY_NW_RAT_CDMA";
        case NWY_NW_RAT_HDR:    return "NWY_NW_RAT_HDR";
        case NWY_NW_RAT_TDSCDMA: return "NWY_NW_RAT_TDSCDMA";
        default:                return "Unknown RAT type";
    }
}
static const char* get_roam_state_to_string(nwy_nw_roam_state_e roam_state)
{
    switch (roam_state) {
        case NWY_NW_ROAM_STATE_OFF:
            return "NWY_NW_ROAM_STATE_OFF";
        case NWY_NW_ROAM_STATE_ON:
            return "NWY_NW_ROAM_STATE_ON";
        default:
            return "Unknown Roaming State";
    }
}

static const char* get_rat_sub_type_to_string(nwy_rat_sub_type_e rat_sub_type)
{
    switch (rat_sub_type) {
        case NWY_RAT_SUB_NO_SERVICE:       return "NWY_RAT_SUB_NO_SERVICE";
        case NWY_RAT_SUB_GSM:              return "NWY_RAT_SUB_GSM";
        case NWY_RAT_SUB_GPRS:             return "NWY_RAT_SUB_GPRS";
        case NWY_RAT_SUB_EDGE:             return "NWY_RAT_SUB_EDGE";
        case NWY_RAT_SUB_WCDMA:            return "NWY_RAT_SUB_WCDMA";
        case NWY_RAT_SUB_HSDPA:            return "NWY_RAT_SUB_HSDPA";
        case NWY_RAT_SUB_HSUPA:            return "NWY_RAT_SUB_HSUPA";
        case NWY_RAT_SUB_HSDPA_HSUPA:      return "NWY_RAT_SUB_HSDPA_HSUPA";
        case NWY_RAT_SUB_TDSCDMA:          return "NWY_RAT_SUB_TDSCDMA";
        case NWY_RAT_SUB_HSPA_PLUS:        return "NWY_RAT_SUB_HSPA_PLUS";
        case NWY_RAT_SUB_TDD_LTE:          return "NWY_RAT_SUB_TDD_LTE";
        case NWY_RAT_SUB_FDD_LTE:          return "NWY_RAT_SUB_FDD_LTE";
        case NWY_RAT_SUB_LTE_TO_5GC:       return "NWY_RAT_SUB_LTE_TO_5GC";
        case NWY_RAT_SUB_NR_TO_5GC:        return "NWY_RAT_SUB_NR_TO_5GC";
        case NWY_RAT_SUB_NG_RAN:           return "NWY_RAT_SUB_NG_RAN";
        case NWY_RAT_SUB_ENDC:             return "NWY_RAT_SUB_ENDC";
        case NWY_RAT_SUB_NR_TO_EPS:        return "NWY_RAT_SUB_NR_TO_EPS";
        case NWY_RAT_SUB_CDMA1X:           return "NWY_RAT_SUB_CDMA1X";
        case NWY_RAT_SUB_HDR:              return "NWY_RAT_SUB_HDR";
        case NWY_RAT_SUB_CDMA1X_LTE:       return "NWY_RAT_SUB_CDMA1X_LTE";
        case NWY_RAT_SUB_CATM:             return "NWY_RAT_SUB_CATM";
        case NWY_RAT_SUB_NBIOT:            return "NWY_RAT_SUB_NBIOT";
        case NWY_RAT_SUB_MAX:              return "NWY_RAT_SUB_MAX";
        default:                           return "Unknown RAT Sub Type";
    }
}

void nwy_test_network_ind_callback(nwy_nw_regs_ind_type_e ind_type, nwy_nw_info_ind_t *ind_p)
{
    NWY_SDK_LOG_DEBUG("%s enter", __func__);
    nwy_thread_sleep(1000);
    switch(ind_type)
    {
        case NWY_NW_REGS_DATA_IND:
        case NWY_NW_REGS_VOICE_IND:
            if(ind_p != NULL)
            {
                nwy_test_cli_echo("\r\n%s rat_type:%s",get_network_type_to_string(ind_type),get_rat_to_string(ind_p->reg_stu.rat_type));
                nwy_test_cli_echo("\r\n%s regs_state:%s",get_network_type_to_string(ind_type),get_regstate_to_string(ind_p->reg_stu.regs_state));
                nwy_test_cli_echo("\r\n%s roam_state:%s",get_network_type_to_string(ind_type),get_roam_state_to_string(ind_p->reg_stu.roam_state));
                nwy_test_cli_echo("\r\n%s sub_rat:%s",get_network_type_to_string(ind_type),get_rat_sub_type_to_string(ind_p->reg_stu.sub_rat));
            }
            break;
        default:
            nwy_test_cli_echo("\r\nInvalid ind");
            break;
    }
}

void nwy_test_cli_nw_network_ind(void)
{
    char *p_input = NULL;
    int ret =-1;
    int i = 0;
    int reg_option = 0;
    p_input = nwy_test_cli_input_gets("\r\nPlease input ind callback option(0:disable, 1:enable):");
    reg_option = (nwy_sim_id_e)atoi(p_input);

    if(reg_option == 1)
    {
        nwy_test_cli_echo("\r\nEnable network callback %s",(nwy_nw_reg_cb(nwy_test_network_ind_callback) == NWY_SUCCESS) ? "success" : "failed");
    }
    else if(reg_option == 0)
    {
        nwy_test_cli_echo("\r\nDisable network callback %s",(nwy_nw_unreg_cb(nwy_test_network_ind_callback) == NWY_SUCCESS) ? "success" : "failed");
    }
    else
    {
        nwy_test_cli_echo("\r\nInvalid option");
    }
    return;
}

void nwy_test_cli_get_time_zone_switch(void)
{
    nwy_time_zone_switch_e mode = 0;

    int ret =-1;

    ret = nwy_get_time_zone_switch(&mode);
    if (NWY_SUCCESS == ret)
    {
        nwy_test_cli_echo("\r\nCurrent time zone switch: %d\r\n", mode);
    }
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
    {
        nwy_test_cli_echo("\r\nGet Network mode Failed, ret: %d\r\n", ret);
    }
}

void nwy_test_cli_set_time_zone_switch(void)
{
    nwy_time_zone_switch_e mode = 0;
    char *p_input = NULL;
    int ret =-1;

    p_input = nwy_test_cli_input_gets("\r\nPlease set time zone switch(0-Disable time zone update,1-Enable time zone update):");
    mode = (nwy_nw_rat_type_e)atoi(p_input);
    ret = nwy_set_time_zone_switch(mode);
    if(NWY_SUCCESS == ret)
        nwy_test_cli_echo("\r\nSet time zone switch\r\n");
    else if(NWY_GEN_E_PLAT_NOT_SUPPORT == ret)
        nwy_test_cli_echo("\r\nThe plat can not support the function or param.");
    else
        nwy_test_cli_echo("\r\nSet time zone switch fail, ret:%d\r\n", ret);
}
