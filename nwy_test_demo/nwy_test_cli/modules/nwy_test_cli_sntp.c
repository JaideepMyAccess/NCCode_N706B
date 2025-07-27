#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"


static char * test_get_sntp_resp(NWY_SNTP_EVENT_E event)
{
    switch(event)
    {
        case     NWY_SNTP_SUCCESS:
           return "SNTP_SUCCESS";
        case     NWY_SNTP_FAIL:
            return "SNTP_FAIL";
        case     NWY_SNTP_DNS_ERROR:
            return "SNTP_DNS_ERROR";
        case     NWY_SNTP_NET_ERROR:
             return "SNTP_NET_ERROR";
        case     NWY_SNTP_TIMEOUT:
            return "SNTP_TIMEOUT";
        default:
            return "unknown error";
    }

}

static void nwy_cli_sntp_result_cb(nwy_sntp_result_type *event)
{
    char resp[128] = {0};

    if(event == NULL)
    {
        nwy_test_cli_echo(" \r\nsntp cb error ");
        return;
    }
    nwy_test_cli_echo("\r\nsntp status:%s ",test_get_sntp_resp(event->event));

    if(event->event == NWY_SNTP_SUCCESS)
    {
        snprintf(resp,sizeof(resp),"date:%d/%d/%d/ %d:%d:%d",event->update_time.year,event->update_time.mon,event->update_time.day,event->update_time.hour,event->update_time.min,event->update_time.sec);
        nwy_test_cli_echo("\r\n%s",resp);
    }
    return;
}


void nwy_test_cli_sntp_get_time(void)
{
    char *sptr = NULL;
    nwy_error_e ret;
    nwy_sntp_param_t sntp_param;
    memset(&sntp_param, 0, sizeof(nwy_sntp_param_t));
    sptr = nwy_test_cli_input_gets("\r\nPlease input channel(1-7):");
    sntp_param.cid = atoi(sptr);
    if ((sntp_param.cid < 1) || (sntp_param.cid > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel id: %d\r\n", sntp_param.cid);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input server addr:");
    if (strlen(sptr) > (NWY_SNTP_URL_MAX_LEN -1) )
    {
        nwy_test_cli_echo("\r\nServer addr len is too long\r\n");
        return;
    }
    strncpy(sntp_param.url, sptr, strlen(sptr));

    sptr = nwy_test_cli_input_gets("\r\nPlease input timeout(s):");
    sntp_param.timeout = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input retry_times:");
    sntp_param.retry_times = atoi(sptr);

    sptr = nwy_test_cli_input_gets("\r\nPlease input tz(E0-13,W0-12),e.g:\"E8\":");
    if (strlen(sptr) > (NWY_SNTP_TZ_MAX_LEN -1) )
    {
        nwy_test_cli_echo("\r\nServer addr len is too long\r\n");
        return;
    }
    strncpy(sntp_param.tz, sptr, strlen(sptr));

    ret = nwy_sntp_get_time(&sntp_param, nwy_cli_sntp_result_cb);
    if(ret == NWY_SUCCESS)
        nwy_test_cli_echo("\r\nnwy_sntp_get_time success\r\n");
    else
        nwy_test_cli_echo("\r\nnwy_sntp_get_time fail %d\r\n", ret);
}

