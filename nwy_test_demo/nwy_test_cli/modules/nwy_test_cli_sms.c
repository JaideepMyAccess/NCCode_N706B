#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

/**************************SMS*********************************/
void nwy_test_cli_sms_init()
{
    char *sptr = NULL;
    int ret = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    ret = nwy_sms_option_init(simid);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy init sms fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy init sms success!\r\n");

    return;
}

void nwy_test_cli_sms_send()
{
    char *sptr = NULL;
    nwy_sms_info_type_t sms_data = {0};
    int dcs = 0;
    int ret = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

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
    memcpy(sms_data.msg_context, sptr, strlen(sptr));
    if (strlen((char *)sms_data.msg_context) > sms_data.msg_context_len || sms_data.msg_context_len > 140) 
	{
        nwy_test_cli_echo("\r\nnwy send sms fail by len!\r\n");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\nPlease input msg format(1:GSM7 2:8BIT 3:UCS2): ");

    sms_data.msg_format = (nwy_sms_msg_format_type_e)atoi(sptr);
    if (sms_data.msg_format < NWY_SMS_MSG_FORMAT_GSM7 || sms_data.msg_format > NWY_SMS_MSG_FORMAT_UCS2) {
        nwy_test_cli_echo("\r\nnwy send sms fail by invalid msg format!\r\n");
        return;
    }

    ret = nwy_sms_msg_send(simid, &sms_data);
    if (NWY_SUCCESS != ret)
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms index: ");
    nindex = atoi(sptr);

    ret = nwy_sms_msg_del(simid, nindex);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy del sms fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy del sms success!\r\n");

    return;
}

void nwy_test_cli_sms_get_sca()
{
    nwy_sms_sca_t sca = {0};
    int ret = 0;
    char *sptr = NULL;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    ret = nwy_sms_sca_get(simid, &sca);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy get sms sca fail!\r\n");
    else
        nwy_test_cli_echo("\r\nsca: %s,%u \r\n", sca.sca, sca.tosca);

    return;
}

void nwy_test_cli_sms_set_sca()
{
    char *sptr = NULL;
    nwy_sms_sca_t sca = {0};
    unsigned tosca;
    int ret = 0;
    int i = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input sca number: ");
    if (strlen(sptr) > 20) {
        nwy_test_cli_echo("\r\nnwy send sms fail by sca oversize\r\n");
        return;
    }
    memcpy(sca.sca, sptr, strlen(sptr));
    for (i = 0; i < strlen(sca.sca); i++) {
        if (sca.sca[i] < '0' || sca.sca[i] > '9') {
            if (i != 0) {
                nwy_test_cli_echo("nwy set SMS sca number wrong!\n");
                return;
            }
        }
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input sca type(129,145): ");
    sca.tosca = atoi(sptr);
    if (sca.tosca != 129 && sca.tosca != 145) {
        nwy_test_cli_echo("\r\nnwy set SMS tosca fail!\r\n");
        return;
    }
    ret = nwy_sms_sca_set(simid, sca);
    if (NWY_SUCCESS != ret)
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms storage(0-SM or 1-ME): ");
    sms_storage = (nwy_sms_storage_type_e)atoi(sptr);

    ret = nwy_sms_storage_set(simid, sms_storage);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy set sms storage fail!\r\n");
    else
        nwy_test_cli_echo("\r\nnwy set sms storage success!\r\n");

    return;
}

void nwy_test_cli_sms_get_storage()
{
    char *sptr = NULL;
    nwy_sms_storage_type_e sms_storage;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    int ret = 0;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    ret = nwy_sms_storage_get(simid, &sms_storage);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nsms_storage: %d", sms_storage);
    } else {
        nwy_test_cli_echo("\r\nget sms_storage fail");
    }
    return;
}

void nwy_test_cli_sms_set_report_md()
{
    char *sptr = NULL;
    int ret = 0;
    nwy_sms_report_para_t report_para = {0};
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms mode(0-2): ");
    if (sptr[0] != '3' && sptr[0] != '1' && sptr[0] != '2') 
	{
        nwy_test_cli_echo("\r\nset report mode invalid mode input\r\n");
        return;
    }
    report_para.transfer_online_mode = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms transfer_type(0(TRANSFER_AND_STORE)-1(TRANSFER_ONLY): ");
    if (sptr[0] != '0' && sptr[0] != '1') 
	{
        nwy_test_cli_echo("\r\nset report mode invalid mt input\r\n");
        return;
    }
    report_para.transfer_type = (0 == atoi(sptr)) ? NWY_SMS_TRANSFER_AND_STORE : NWY_SMS_TRANSFER_ONLY;

    ret = nwy_sms_report_set(simid, report_para);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy set sms report mode fail!\r\n");
    else
        nwy_test_cli_echo("\r\nSet sms report mode success!\r\n");

    return;
}
nwy_sms_recv_info_type_t g_sms_data = {0};
void nwy_test_cli_sms_read()
{
    char *sptr = NULL;
    //nwy_sms_recv_info_type_t g_sms_data = {0};
    unsigned index = 0;
    int ret = 0;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms index: ");
    index = atoi(sptr);

    ret = nwy_sms_msg_read(simid, index, &g_sms_data);
    if (NWY_SUCCESS != ret)
        nwy_test_cli_echo("\r\nnwy read sms fail!\r\n");
    else
    {
        NWY_CLI_LOG("=SMS= finish to read sms\n");
        nwy_test_cli_echo("\r\nread one sms index:%d StorageId:%d phone_number:%s time %02d-%02d-%02d %02d:%02d:%02d+%d", index, g_sms_data.storage_type, g_sms_data.source_phone_num, g_sms_data.date.uYear,\
            g_sms_data.date.uMonth, g_sms_data.date.uDay, g_sms_data.date.uHour, g_sms_data.date.uMinute, g_sms_data.date.uSecond, g_sms_data.date.iZone);
        nwy_test_cli_echo("\r\nnwy read sms sms_data :%s\r\n", g_sms_data.msg_content);
    }

    NWY_CLI_LOG("=SMS= end to test read sms\n");

    return;
}


void nwy_test_cli_sms_list()
{

    int ret = 0;
    nwy_sms_msg_list_t sms_info = {0};
    int i = 0;
    char temp[200] = {0};
    char *sptr = NULL;
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    ret = nwy_sms_msg_list_read(simid, &sms_info);
    if (NWY_SUCCESS != ret) {
        nwy_test_cli_echo("\r\n nwy read sms list fail!\r\n");
        return;
    }

    if (0 == sms_info.len) {
        nwy_test_cli_echo("\r\n no sms in storage!\r\n");
    } else {
        nwy_test_cli_echo("\r\nsms storage. index num: %d", sms_info.len);
        for (i = 0; i < sms_info.len; i++) {
            sprintf(temp + strlen(temp), "%d,", sms_info.indices[i]);
        }
        temp[strlen(temp) - 1] = '\0';
        nwy_test_cli_echo("\r\n%s\r\n", temp);
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
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input delete sms type(1,read;2,read_send;3,read_send_unsend;4,all): ");
    temp = atoi(sptr);
    if(temp < NWY_SMS_MSG_DFLAG_READ || temp > NWY_SMS_MSG_DFLAG_ALL)
    {
        nwy_test_cli_echo("\r\n Input sms type  is invalid!");
        return;
    }
    delflag = (nwy_sms_msg_dflag_e)temp;

    ret =  nwy_sms_msg_del_ext(simid, delflag);

    if (NWY_SUCCESS != ret) 
	{
        nwy_test_cli_echo("\r\nnwy delete sms by type fail!\r\n");
    } else {
        nwy_test_cli_echo("\r\nnwy finish to delete sms by type success\n");
    }

    NWY_CLI_LOG("=SMS= end to test delete  sms by type\n");
    return;
}


void nwy_sms_test_recv_cb(nwy_sim_id_e simid, nwy_mt_sms_event_e urc_type, nwy_sms_info_ind_t *ind_struct)
{
    nwy_sms_info_ind_t *sms_data = (nwy_sms_info_ind_t *)ind_struct;
    if(NWY_SMS_PP_IND == urc_type)
    {
        if(sms_data->transfer_type == NWY_SMS_TRANSFER_AND_STORE)
        {
            nwy_test_cli_echo("\r\nrecv one sms transfer and store\r\n");
            if(NWY_SMS_STORAGE_TYPE_UIM == sms_data->sms_info.storage_type)
            {
                nwy_test_cli_echo("\r\nrecv sms nIndex:%d, storage:SIM\r\n", sms_data->sms_info.nIndex);
            }
            else if(NWY_SMS_STORAGE_TYPE_NV == sms_data->sms_info.storage_type)
            {
                nwy_test_cli_echo("\r\nrecv sms nIndex:%d, storage:ME\r\n", sms_data->sms_info.nIndex);
            }
        }
        else if(sms_data->transfer_type == NWY_SMS_TRANSFER_ONLY)
        {
            nwy_test_cli_echo("\r\nrecv one sms transfer only\r\n");
            nwy_test_cli_echo("\r\nrecv sms number:%s, msg_format:%d, time %02d-%02d-%02d %02d:%02d:%02d+%d\r\n", \
                sms_data->sms_info.source_phone_num, sms_data->sms_info.msg_format, sms_data->sms_info.date.uYear, sms_data->sms_info.date.uMonth, sms_data->sms_info.date.uDay, \
                sms_data->sms_info.date.uHour, sms_data->sms_info.date.uMinute, sms_data->sms_info.date.uSecond, sms_data->sms_info.date.iZone);
            nwy_test_cli_echo("\r\nrecv sms data len:%d,sms data:%s\r\n", sms_data->sms_info.msg_content_len, sms_data->sms_info.msg_content);
        }
        else
        {
            nwy_test_cli_echo("\r\nnwy recv sms transfer type invalid\r\n");
        }
    }

    return;
}
void nwy_test_cli_sms_reg(void)
{
    nwy_sim_id_e simid = NWY_SIM_ID_MAX;
    int reg_flag = 0;
    char *sptr = NULL;
    sptr = nwy_test_cli_input_gets("\r\nPlease input simid: ");
    simid = (nwy_sim_id_e)atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input sms reg option: (0 - reg / 1 - unreg)");
    reg_flag = atoi(sptr);
    if(0 == reg_flag)
    {
        nwy_test_cli_echo("\r\nreg sms ind %s",(nwy_sms_recv_cb_reg(simid, nwy_sms_test_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else if(1 == reg_flag)
    {
        nwy_test_cli_echo("\r\nunreg sms ind %s",(nwy_sms_recv_cb_unreg(simid, nwy_sms_test_recv_cb) == NWY_SUCCESS) ? "success" : "failed");
    }
    else
    {
        nwy_test_cli_echo("\r\nwrong sms reg option\n");
    }

    return;
}