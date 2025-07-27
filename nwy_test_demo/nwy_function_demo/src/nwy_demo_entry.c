#include "nwy_demo_utility.h"

nwy_osi_thread_t nwy_main_control_thread = NULL;
nwy_osi_thread_t nwy_network_monitor_thread = NULL;

extern void nwy_main_control_func(void *param);
extern void nwy_network_monitor_func(void *param);

static nwy_error_e nwy_demo_init()
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    ret = nwy_vir_at_test_init();
    if (ret != NWY_SUCCESS) {
        return ret;
    }
    return ret;
}

#ifdef FEATURE_NWY_ASR_PLAT
int nwy_open_app_entry()
#else
int appimg_enter(void *param)
#endif
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;

    nwy_thread_sleep(5 * 1000);
    NWY_SDK_LOG_DEBUG("nwy_open_app_enter ...",0, 0, 0);
    ret = nwy_demo_init();
    if (ret != NWY_SUCCESS) {
        NWY_SDK_LOG_DEBUG("nwy_demo_init faile ret = %d ...",ret, 0, 0);
        return ret;
    }
    nwy_thread_create(&nwy_main_control_thread, "nwy_main_control", NWY_OSI_PRIORITY_NORMAL, nwy_main_control_func, NULL, 32, 1024 * 4, NULL);

    nwy_thread_create(&nwy_network_monitor_thread, "nwy_net_monitor", NWY_OSI_PRIORITY_NORMAL, nwy_network_monitor_func, NULL, 16, 1024 * 4, NULL);
    return 0;
}

void appimg_exit(void)
{
    NWY_SDK_LOG_DEBUG("application image exit",0, 0, 0);
}


