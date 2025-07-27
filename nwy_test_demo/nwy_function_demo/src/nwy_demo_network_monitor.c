#include "nwy_demo_network_monitor.h"
#include "nwy_demo_data.h"
static int                              g_nwy_csq_flag = 0;
static int                              g_nwy_creg_flag = 0;
static int                              g_nwy_net_con_flag = 0;

nwy_osi_timer_t                         g_network_moitor_timer = NULL;
nwy_osi_timer_t                         g_data_call_moitor_timer = NULL;
extern int                              g_data_handle[NWY_DATA_HANDL_COUNT];

extern nwy_osi_thread_t nwy_network_monitor_thread;
extern nwy_osi_thread_t nwy_main_control_thread;

static int nwy_demo_sim_err_proc()
{
    return 1;
}

static int nwy_demo_reg_err_proc()
{
    return 1;
}

static void nwy_demo_data_call_monitor_cb()
{

}
void nwy_demo_cfun_start(void)
{
    char resp[64] = {0};
    char *ok_fmt = "+CFUN:";
    char *err_fmt = "ERROR";
    char str[]= "AT+CFUN=1";

    char tmp[]= "AT+CFUN?";
    nwy_vir_at_send(tmp, ok_fmt, err_fmt, resp, sizeof(resp));
    if (strstr(resp, "+CFUN: 0") != NULL) {
        nwy_vir_at_send(str, ok_fmt, err_fmt, resp, sizeof(resp));
    }

}

void nwy_demo_cfun_reset_ckeck(int reset_flag)
{
    char resp[64] = {0};
    char *ok_fmt = "+CFUN:";
    char *err_fmt = "ERROR";
    char str[]= "AT+CFUN=0";

    if (reset_flag == 1) {
        g_nwy_creg_flag = 0;
        g_nwy_csq_flag = 0;
        g_nwy_net_con_flag = 0;

        /* release handl and call */
        nwy_demo_data_call_clear();

        nwy_vir_at_send(str, ok_fmt, err_fmt, resp, sizeof(resp));
    }

    /*check cfun?*/
    nwy_demo_cfun_start();
}



void nwy_demo_restart_data_call()
{
    nwy_event_msg_t event;
    nwy_nw_regstatus_t regs_info;


    memset(&regs_info, 0, sizeof(regs_info));
    nwy_nw_regstatus_get(NWY_DEFAULT_SIM_ID, &regs_info);
    if (regs_info.ps_regs_valid == 1 && regs_info.ps_regs.regs_state == NWY_NW_SERVICE_FULL) {
        if (g_data_handle[NWY_DATA_PUBLIC_HANDL] == 0) {
            memset(&event, 0, sizeof(event));
            event.id = NWY_DEMO_RESTART_DATA_CALL_MSG;
            nwy_thread_event_send(nwy_network_monitor_thread, &event, NWY_OSA_NO_SUSPEND);
        }
    }
}

int nwy_demo_data_call_check(void)
{
    int sim_err = 0;
    int reg_err = 0;
    nwy_sim_status_e sim_status = NWY_SIM_UNKNOWN;
    nwy_nw_regstatus_t regs_info;
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_sim_id_e sim_id = NWY_DEFAULT_SIM_ID;

    while (1) {
        NWY_SDK_LOG_DEBUG("sim_status = %d ...",sim_status, 0, 0);
        if (sim_status != NWY_SIM_READY) {
            ret = nwy_sim_status_get(sim_id, &sim_status);
            if (ret == NWY_SUCCESS) {
                sim_err++;

                /* if the times beyond the max retry, customer can add their action */
                if (sim_err >= NWY_SIM_ERR_COUNT_MAX) {
                    nwy_demo_sim_err_proc();
                }
            }

        } else {
            memset(&regs_info, 0, sizeof(regs_info));
            ret = nwy_nw_regstatus_get(sim_id, &regs_info);
            if (ret == NWY_SUCCESS) {

            /* if the times beyond the max retry, customer can add their action */
                if (reg_err >= NWY_REG_ERR_COUNT_MAX) {
                    nwy_demo_reg_err_proc();
                }
            }
            NWY_SDK_LOG_DEBUG("regs_state = %d reg_err = %d...",regs_info.ps_regs.regs_state, reg_err, 0);

            if (regs_info.ps_regs_valid == 1 && regs_info.ps_regs.regs_state == NWY_NW_SERVICE_FULL) {
                return NWY_SUCCESS;
            } else {
                reg_err ++;
                if (reg_err >= NWY_CUFN_RESET_COUNT) {
                    reg_err = 0;
                    nwy_demo_cfun_reset_ckeck(1);
                }

            }
        }
        nwy_thread_sleep(1000);
    }
  return NWY_GEN_E_UNKNOWN;
}

void nwy_demo_check_cfun_reset( )
{
    uint8 csq = 0;
    int cfun_reset_falg = 0;
    nwy_nw_regstatus_t regs_info;

    nwy_nw_csq_get(NWY_DEFAULT_SIM_ID, &csq);
    if (csq < NWY_CSQ_MIN || csq == NWY_CSQ_INVALID_VAL) {
        g_nwy_csq_flag ++;
    } else {
        g_nwy_csq_flag = 0;
    }
    memset(&regs_info, 0, sizeof(regs_info));
    nwy_nw_regstatus_get(NWY_DEFAULT_SIM_ID, &regs_info);


    if (regs_info.ps_regs_valid != 1 || regs_info.ps_regs.regs_state != NWY_NW_SERVICE_FULL)
    {
        g_nwy_creg_flag ++;
    } else {
        g_nwy_creg_flag =0;
    }

    if (0 == g_data_handle[NWY_DATA_PUBLIC_HANDL]
      && (csq > NWY_CSQ_MIN && csq != 99)
      && regs_info.ps_regs.regs_state == NWY_NW_SERVICE_FULL)
    {
        ++g_nwy_net_con_flag;
    }
    else
    {
        g_nwy_net_con_flag = 0;
    }
    NWY_SDK_LOG_DEBUG("g_nwy_creg_flag = %d g_nwy_csq_flag = %d g_nwy_net_con_flag=%d",
      g_nwy_creg_flag, g_nwy_csq_flag, g_nwy_net_con_flag);

    if (g_nwy_creg_flag >= NWY_CUFN_RESET_COUNT || g_nwy_csq_flag >= NWY_CUFN_RESET_COUNT
          || g_nwy_net_con_flag >= NWY_NET_RESET_COUNT) {
          cfun_reset_falg = 1;
    }
    nwy_demo_cfun_reset_ckeck(cfun_reset_falg);
}



void nwy_demo_data_state_process(int profile_id, nwy_data_call_state_e state)
{
    nwy_event_msg_t event;
    nwy_data_callinfo_t info;

    if (state == NWY_DATA_CALL_CONNECTED) {
        memset(&info, 0, sizeof(info));
        nwy_demo_call_info_get(profile_id, &info);

    } else if (state == NWY_DATA_CALL_DISCONNECTED) {

    }

    memset(&event, 0, sizeof(event));
    event.id = NWY_MAIN_DATA_CALL_CHANGE_MSG;
    event.param1 = profile_id;
    event.param2 = state;
    nwy_thread_event_send(nwy_main_control_thread, &event, NWY_OSA_NO_SUSPEND);

}

static void nwy_demo_network_monitor_cb()
{
    nwy_event_msg_t event;

    //nwy_demo_check_cfun_reset();
    memset(&event, 0, sizeof(event));
    event.id = NWY_DEMO_CFUN_RESET_CKECK_MSG;
    nwy_thread_event_send(nwy_network_monitor_thread, &event, NWY_OSA_NO_SUSPEND);

}

void nwy_demo_network_monitor_timer()
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t timer_para;

    if (g_network_moitor_timer == NULL) {
        memset(&timer_para, 0, sizeof(timer_para));

        timer_para.thread_hdl = nwy_network_monitor_thread;
        timer_para.cb = nwy_demo_network_monitor_cb;
        timer_para.cb_para = (nwy_timer_cb_para_t)NULL;
        timer_para.expired_time = 2000;
        timer_para.type = NWY_TIMER_PERIODIC;
        ret = nwy_sdk_timer_create(&g_network_moitor_timer, &timer_para);

        if (g_network_moitor_timer != NULL) {
            nwy_sdk_timer_start(g_network_moitor_timer, &timer_para);

        } else {
            NWY_SDK_LOG_INFO("tiemr create fail", ret, 0, 0);
            return;
        }
    }
}


void nwy_demo_data_call_monitor_timer()
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t timer_para;

    if (g_data_call_moitor_timer == NULL) {
        memset(&timer_para, 0, sizeof(timer_para));

        timer_para.thread_hdl = nwy_network_monitor_thread;
        timer_para.cb = nwy_demo_data_call_monitor_cb;
        timer_para.cb_para = (nwy_timer_cb_para_t)NULL;
        timer_para.expired_time = 2000;
        timer_para.type = NWY_TIMER_PERIODIC;
        nwy_sdk_timer_create(&g_data_call_moitor_timer, &timer_para);
        if (g_data_call_moitor_timer != NULL) {

        } else {
            NWY_SDK_LOG_INFO("tiemr create fail", ret, 0, 0);
        }
    }
}
void nwy_network_monitor_func(void *param)
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_event_msg_t event;

    /*check data environment*/
    nwy_demo_data_call_check();

    /*create and start timer to monitor the network*/
    nwy_demo_network_monitor_timer();

    /* start data call, if it start fail ,the system will auto restart again */
    nwy_demo_data_call_start();

    while (1) {
        memset(&event, 0, sizeof(event));
        ret = nwy_thread_event_wait(nwy_network_monitor_thread, &event, NWY_OSA_SUSPEND);
        if (ret == NWY_SUCCESS) {
            switch (event.id) {
                case NWY_DEMO_DATA_CALL_STA_MSG:
                    nwy_demo_data_state_process(event.param1, event.param2);
                    break;
                case NWY_DEMO_CFUN_RESET_CKECK_MSG:
                    nwy_demo_check_cfun_reset();
                    break;
                case NWY_DEMO_RESTART_DATA_CALL_MSG:
                    nwy_demo_data_call_start();
                    break;
                default:
                    break;
            }
        }
    }
}

