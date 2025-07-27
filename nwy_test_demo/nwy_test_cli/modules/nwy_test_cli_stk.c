#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"



/**************************STK*********************************/
void nwy_test_cli_recv_info_stk()
{
    nwy_sat_notify_info_type_t sat_data = {0};
    memset(&sat_data,0,sizeof(nwy_sat_notify_info_type_t));

    nwy_sat_recv_data(&sat_data);
    nwy_test_cli_echo("\r\n+STKPCI type: %d", sat_data.pcitype);
    nwy_test_cli_echo("\r\n+STKPCI Len: %d", sat_data.nDataLen);
    nwy_test_cli_echo("\r\n+STKPCI Data: %s", sat_data.pData);

    /* then can process sat data by customer */
    nwy_test_cli_echo("\r\nprocess sat data");
}

void nwy_test_cli_set_switch_stk()
{
    nwy_sat_onoff_type_e switch_type;
    char* sptr = NULL;
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input stk switch: ");
    switch_type = atoi(sptr);

    ret = nwy_sat_set_on_off(switch_type);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\nnwy set stk switch fail");
        return;
    }
    nwy_test_cli_echo("\r\nnwy set stk switch success");

    return;
}

void nwy_test_cli_get_switch_stk()
{
    nwy_sat_onoff_type_e  switch_type = NWY_SAT_TYPE_MAX;
    int ret = 0;

    switch_type = nwy_sat_get_on_off();
    if(NWY_SAT_TYPE_MAX == switch_type)
    {
        nwy_test_cli_echo("\r\nnwy get stk switch fail");
        return;
    }

    nwy_test_cli_echo("\r\n stk switch on/off:%d", switch_type);

    return;
}

void nwy_test_cli_terminal_response_stk()
{
    char* sptr = NULL;
    char data[500] = {0};
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input terminal response data: ");
    memcpy(data, sptr,strlen(sptr));

    ret = nwy_sat_terminal_response(data);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\nnwy send terminal response fail");
        return;
    }
    nwy_test_cli_echo("\r\nnwy send terminal response success");

    return;
}

void nwy_test_cli_envelope_command_stk()
{
    char* sptr = NULL;
    char data[500] = {0};
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input envelope command data: ");
    memcpy(data,sptr,strlen(sptr));

    ret = nwy_sat_envelope_command(data);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\nnwy send envelope cmd fail");
        return;
    }
    nwy_test_cli_echo("\r\nnwy send envelope cmd success");

    return;
}

void nwy_test_cli_set_profile_stk()
{
    char* sptr = NULL;
    char data[500] = {0};
    int ret = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease set profile data: ");
    memcpy(data, sptr,strlen(sptr));

    ret = nwy_sat_set_profile(data);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\nnwy set profile fail");
        return;
    }
    nwy_test_cli_echo("\r\nnwy set profile success");

    return;
}

void nwy_test_cli_get_profile_stk()
{
    char* sptr = NULL;
    char data[500] = {0};
    int ret = 0;

    ret = nwy_sat_get_profile(data);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\nnwy get profile fail");
        return;
    }
    nwy_test_cli_echo("\r\nstk: %s",data);

    return;
}

#if 0
void nwy_test_cli_stk_recv_cb(nwy_sat_notify_info_type_t *ind_struct)
{
    nwy_test_cli_echo("\r\nstk pcitype:%d\r\n", ind_struct->pcitype);
    nwy_test_cli_echo("\r\nstk Len:%d\r\n", ind_struct->nDataLen);
    nwy_test_cli_echo("\r\nstk Data:%s\r\n", ind_struct->pData);

    return;
}

void nwy_test_cli_stk_reg()
{
    int reg_flag = 0;
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nPlease input stk reg option: (0 - reg / 1 - unreg)");
    reg_flag = atoi(sptr);
    if(0 == reg_flag)
    {
        nwy_test_cli_echo("\r\nreg stk ind %s",(nwy_sim_urc_reg(nwy_test_cli_stk_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else if(1 == reg_flag)
    {
        nwy_test_cli_echo("\r\nunreg stk ind %s",(nwy_sim_urc_unreg(nwy_test_cli_stk_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else
    {
        nwy_test_cli_echo("\r\nwrong stk reg option\n");
    }

    return;
}
#endif
