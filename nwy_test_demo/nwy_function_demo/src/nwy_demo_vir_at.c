#include "nwy_demo_vir_at.h"

nwy_osi_mutex_t     g_nwy_vir_at_mutex = NULL;

nwy_error_e nwy_vir_at_test_init()
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    do {
        ret = nwy_sdk_mutex_create(&g_nwy_vir_at_mutex);
        if (ret != NWY_SUCCESS) {
            break;;
        }

        ret = nwy_virt_at_parameter_init();
    }while(0);

    return ret;
}

nwy_error_e nwy_vir_at_send(char *at_command, char *ok_fmt, char *err_fmt, char *resp, int len)
{
	nwy_at_info_t info;
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;

    if (at_command == NULL || ok_fmt == NULL || err_fmt == NULL || resp ==NULL) {
        return NWY_GEN_E_UNKNOWN;
    }
    memset(&info, 0, sizeof(info));

    memcpy(info.at_command, at_command, strlen(at_command));
    memcpy(info.ok_fmt, ok_fmt, strlen(ok_fmt));
    memcpy(info.err_fmt, err_fmt, strlen(err_fmt));
    info.at_command[strlen(at_command)] = '\r';
    info.at_command[strlen(at_command) + 1] = '\n';

    info.length = strlen(at_command) + 2;

    ret = nwy_sdk_mutex_lock(g_nwy_vir_at_mutex, NWY_OSA_SUSPEND);
    if (ret != NWY_SUCCESS) {
        NWY_SDK_LOG_ERROR("get lock fail",0,0,0);
        return ret;
    }

    ret = nwy_virt_at_cmd_send(&info, resp, len, NWY_AT_TIMEOUT_DEFAULT);

    nwy_sdk_mutex_unlock(g_nwy_vir_at_mutex);
    NWY_SDK_LOG_INFO("vir send at = %s resp = %s len = %d",at_command,resp,len);
    return ret;

}
