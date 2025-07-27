#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_cust.h"

#define NWY_TASK_STACK_SIZE     1024

// Timer demo variables
static nwy_osi_timer_t      s_nwy_test_timer = NULL;
static nwy_osi_timer_t      s_nwy_isr_timer = NULL;
static nwy_osi_timer_t      s_nwy_service_timer = NULL;
static nwy_osi_timer_t      s_nwy_user_timer = NULL;
static nwy_osi_thread_t     s_nwy_user_thread = NULL;
static int                  s_timer_count = 0;
static int                  s_isr_timer_count = 0;
static int                  s_service_timer_count = 0;
static int                  s_user_timer_count = 0;

nwy_osi_semaphore_t         s_nwy_cli_semaphore = NULL;
static nwy_osi_thread_t     s_nwy_task_A = NULL;
static nwy_osi_thread_t     s_nwy_task_B = NULL;
static int                  s_nwy_semaphore_count = 0;

static int s_nwy_at_init_flag = 0;

static nwy_osi_thread_t priority_test_thread = NULL;
static nwy_osi_semaphore_t test_complete_sem = NULL;

static int nwy_cli_data_valid_check(nwy_time_t *time, int timezone)
{
    int monthLen[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (time == NULL) {
        return NWY_GEN_E_INVALID_PARA;
    }

    if ((time->year > 2100 || time->year < 2000) || (time->mon > 12 || time->mon < 1) ||
        ((time->day > monthLen[time->mon-1]) || time->day < 1)) {
            return NWY_GEN_E_INVALID_PARA;
        }

    if ((time->hour > 23 || time->hour < 0) || (time->min > 59 || time->min < 0) ||
        (time->sec > 59 || time->sec < 0)) {
            return NWY_GEN_E_INVALID_PARA;
        }

    if (timezone > 96 || timezone < -96) {
        return NWY_GEN_E_INVALID_PARA;
    }

    return NWY_SUCCESS;
}

/* ISR mode timer callback - non-blocking, fast execution */
static void nwy_test_cli_isr_timer_cb(nwy_timer_cb_para_t ctx)
{
    s_isr_timer_count++;
    /* Execute in interrupt context - cannot call blocking functions */
}

/* Service thread mode timer callback - can be blocking */
static void nwy_test_cli_service_timer_cb(nwy_timer_cb_para_t ctx)
{
    s_service_timer_count++;
    nwy_test_cli_echo("\r\n[ServiceTimer] Count:%d - In timer manager thread context,Can call blocking functions", s_service_timer_count);
    /*Can call blocking functions in service thread */
}

/* User thread mode timer callback - can be blocking */
static void nwy_test_cli_user_timer_cb(nwy_timer_cb_para_t ctx)
{
    s_user_timer_count++;
    nwy_test_cli_echo("\r\n[UserTimer] Count:%d - In user thread context,Can call blocking functions", s_user_timer_count);
    /*Can call blocking functions in user thread */
}

/* User thread main function - waits for timer events */
static void nwy_test_cli_user_thread_cb(void *ctx)
{
    nwy_event_msg_t event;
    
    nwy_test_cli_echo("\r\n[UserThread] User thread started, waiting for timer events");
    
    while(1)
    {
        /* Wait for timer events */
        if (nwy_thread_event_wait(nwy_thread_get_current(), &event, NWY_OSA_SUSPEND) == NWY_SUCCESS) {
            /* Timer events are automatically handled in nwy_thread_event_wait */
            nwy_test_cli_echo("\r\n[UserThread] Timer event received and processed");
        }
    }
}

/* Legacy timer callback for backward compatibility */
static void nwy_test_cli_timer_cb(nwy_timer_cb_para_t ctx)
{
    s_timer_count++;
}

static void nwy_cli_semaphore_taskA_proc(void *ctx)
{
    int i = 0;
    nwy_semaphore_acquire(s_nwy_cli_semaphore, NWY_OSA_SUSPEND);
    for (i = 0; i < 100; i++)
    {
        nwy_test_cli_echo("\r\nTaskA semaphore_count = %d", ++s_nwy_semaphore_count);
        nwy_thread_sleep(10);
    }
    nwy_semahpore_release(s_nwy_cli_semaphore);

    if (s_nwy_task_A != NULL) {
        nwy_thread_suspend(s_nwy_task_A);
    }
}
void nwy_cli_semaphore_taskB_proc(void *ctx)
{
    int i = 0;
    nwy_semaphore_acquire(s_nwy_cli_semaphore, NWY_OSA_SUSPEND);
    for (i = 0; i < 50; i++)
    {
        nwy_test_cli_echo("\r\nTaskB semaphore_count = %d", --s_nwy_semaphore_count);
        nwy_thread_sleep(10);
    }
    nwy_semahpore_release(s_nwy_cli_semaphore);
    if (s_nwy_task_B != NULL) {
        nwy_thread_suspend(s_nwy_task_B);
    }
}
static void nwy_cli_virtual_at_test()
{
    char *sptr = NULL;
    int ret = -1;
    char resp[2048] = {0};
    nwy_at_info_t at_cmd;
    nwy_thread_sleep(500);

    sptr = nwy_test_cli_input_gets("\r\nSend AT cmd :");

    memset(&at_cmd, 0, sizeof(nwy_at_info_t));
    memcpy(at_cmd.at_command, sptr, strlen(sptr));
    at_cmd.at_command[strlen(sptr)] = '\r';
    at_cmd.at_command[strlen(sptr) + 1] = '\n';
    at_cmd.length = strlen(at_cmd.at_command);

    /*** The ASR1803 and UIS8850 platforms require inputs in the "OK" and "ERROR" formats,***/
    /*** while the ASR1605 platform requires only a carriage return.***/
    sptr = nwy_test_cli_input_gets("\r\nAT OK FORMAT:");
    if(strlen(sptr) > 0){
       memcpy(at_cmd.ok_fmt, sptr, strlen(sptr));
    }

    sptr = nwy_test_cli_input_gets("\r\nAT ERROR FORMAT:");
    if(strlen(sptr) > 0){
       memcpy(at_cmd.err_fmt, sptr, strlen(sptr));
    }

    ret = nwy_virt_at_cmd_send(&at_cmd, resp, sizeof(resp), NWY_AT_TIMEOUT_DEFAULT);
    if (ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\n Resp:%s", resp);
    }
    else if (ret == NWY_AT_GET_RESP_TIMEOUT)
    {
        nwy_test_cli_echo("\r\n AT timeout");
    }
    else
    {
        nwy_test_cli_echo("\r\n AT ERROR");
    }
}

static void nwy_at_cmd_process_callback(void *handle, char *atcmd, int type, char *para0, char *para1, char *para2)
{
    switch (type)
    {
    case 0:
        nwy_virt_at_forward_send(handle, "\r\n", 2);
        nwy_virt_at_forward_send(handle, atcmd, strlen(atcmd));
        if (para0)
        {
            nwy_virt_at_forward_send(handle, para0, strlen(para0));
        }

        if (para1)
        {
            nwy_virt_at_forward_send(handle, para1, strlen(para1));
        }

        if (para2)
        {
            nwy_virt_at_forward_send(handle, para2, strlen(para2));
        }

        nwy_virt_at_forward_send(handle, "\r\nOK\r\n", 6);
        break;
    case 1:
        nwy_virt_at_forward_send(handle, "\r\nTEST\r\n", 8);
        break;
    case 2:
        nwy_virt_at_forward_send(handle, "\r\nREAD\r\n", 8);
        break;
    case 3:
        nwy_virt_at_forward_send(handle, "\r\nEXE\r\n", 7);
        break;
    }
}


void nwy_cli_unsol_at_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\n unsolicited, len:%d, data:%s", len, data);
}

void nwy_cli_tcpsetup_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nTCPSETUP: ");
    nwy_test_cli_echo("%s\r\n", data);
}
void nwy_cli_tcprecvs_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nTCPRECV(S): ");
    nwy_test_cli_echo("%s\r\n", data);
}
void nwy_cli_tcp_close_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nTCPCLOSE:");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_cli_tcprecv_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nTCPRECV:");
    nwy_test_cli_echo("%s\r\n", data);
}
void nwy_cli_acpt_close_cb(char *data)
{
    nwy_test_cli_echo("\r\nCLOSECLIENT: ");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_cli_udprecv_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nUDPRECV:");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_cli_client_acpt_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nConnect AcceptSocket=");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_cli_gprs_disconnect_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\nGPRS DISCONNECTION");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_cli_sms_list_resp_cb(uint8 *data, int len)
{
    nwy_test_cli_echo("\r\n+CMGL: ");
    nwy_test_cli_echo("%s\r\n", data);
}

void nwy_test_cli_get_model()
{
    char buff[128] = {0};

    nwy_dm_dev_model_get(buff, sizeof(buff));
    nwy_test_cli_echo("\r\nDev model : %s \r\n", buff);
}

extern char ImeiNumber[20];
void nwy_test_cli_get_imei()
{
    char imei[16] = {0};
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;

    ret = nwy_dm_imei_get(NWY_SIM_ID_SLOT_1, imei, sizeof(imei));
    if (NWY_SUCCESS != ret)
    {
        nwy_test_cli_echo("\r\n Get IMEI error \r\n");
        return;
    }

    nwy_test_cli_echo("\r\nIMEI:%s \r\n", imei);

    strcpy(ImeiNumber, imei);
    
}

void nwy_test_cli_set_app_version()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nPlease input app version: ");

    if(strlen(sptr) == 0 || strlen(sptr) >= 128){
        nwy_test_cli_echo("\r\nInput string is invalid!!\r\n");
        return;
    }

    ret = nwy_dm_app_version_set(sptr,strlen(sptr));

    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nSet APP version success(AT+NAPPCHECK?)!! ver:%s\r\n",sptr);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nSet APP version success failed!\r\n");
    }

}

void nwy_test_cli_get_open_sdk_ver_string()
{
    char ver[64] = {0};
    nwy_error_e ret = NWY_FAIL;
    ret = nwy_dm_open_sdk_version_get(ver, sizeof(ver));
    if(ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nThe open sdk version string in base firmware is :%s\r\n", ver);
        nwy_test_cli_echo("\r\nThe open sdk version string in app is :%s\r\n", NWY_OPEN_SDK_VERSION_STRING);
    }
    else if(ret == NWY_GEN_E_PLAT_NOT_SUPPORT)
    {
        nwy_test_cli_echo("\r\nThis plat do not support this func\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nGet open sdk version failed\r\n");
    }
}


void nwy_test_cli_get_sw_ver()
{
    char ver[64] = {0};

    nwy_dm_inner_version_get(ver, sizeof(ver));

    nwy_test_cli_echo("\r\nThe sw ver is :%s", ver);
}

void nwy_test_cli_get_hw_ver()
{
    char ver[64] = {0};

    nwy_dm_hw_version_get(ver, sizeof(ver));

    nwy_test_cli_echo("\r\nThe hw ver is :%s", ver);
}

void nwy_test_cli_get_heap_info()
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_heapinfo_t  heapinfo;
    memset(&heapinfo,0x00,sizeof(heapinfo));
    ret = nwy_dm_heapinfo(&heapinfo);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\n dev heapinfo: start:%ld,size:%ld,avail_size:%ld,max_block_size:%ld\r\n",
                             heapinfo.start,heapinfo.size,heapinfo.avail_size,heapinfo.max_block_size);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nget dev heapinfo failed!\r\n");
    }
}

void nwy_test_cli_get_cpu_temp()
{
    int ret = NWY_GEN_E_UNKNOWN;
    float value = 0;
    ret = nwy_dm_rftemperature_get(&value);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\n temperature is %.2f\r\n", value);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nget cpu temp failed!\r\n");
    }
}
void nwy_test_cli_start_timer(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t para;

    memset(&para, 0, sizeof(para));
    s_timer_count = 0;

    para.thread_hdl = NWY_TIMER_IN_ISR;
    //para.thread_hdl = NWY_TIMER_IN_SERVICE;
    para.cb = nwy_test_cli_timer_cb;
    para.cb_para = NULL;
    para.expired_time = 1000;
    para.type = NWY_TIMER_PERIODIC;

    if (s_nwy_test_timer == NULL)
    {
        nwy_sdk_timer_create(&s_nwy_test_timer, &para);
        if (s_nwy_test_timer == NULL) {
            nwy_test_cli_echo("\r\nCreate timer fail");
            return;
        }
    }

    ret = nwy_sdk_timer_start(s_nwy_test_timer, &para);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nstart timer fail");
    } else {
        nwy_test_cli_echo("\r\nstart timer succes");
    }
}

/* Demo ISR mode timer - fast, non-blocking */
void nwy_test_cli_start_isr_timer(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t para;

    memset(&para, 0, sizeof(para));
    s_isr_timer_count = 0;

    para.thread_hdl = NWY_TIMER_IN_ISR;  /* Execute in interrupt context */
    para.cb = nwy_test_cli_isr_timer_cb;
    para.cb_para = NULL;
    para.expired_time = 500;  /* 0.5 seconds */
    para.type = NWY_TIMER_PERIODIC;

    if (s_nwy_isr_timer == NULL)
    {
        ret = nwy_sdk_timer_create(&s_nwy_isr_timer, &para);
        if (ret != NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nCreate ISR timer fail");
            return;
        }
    }

    ret = nwy_sdk_timer_start(s_nwy_isr_timer, &para);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nStart ISR timer fail");
    } else {
        nwy_test_cli_echo("\r\nStart ISR timer success - Fast, non-blocking execution");
    }
}

/* Demo Timer service thread mode timer - can be blocking */
void nwy_test_cli_start_service_timer(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t para;

    memset(&para, 0, sizeof(para));
    s_service_timer_count = 0;

    para.thread_hdl = NWY_TIMER_IN_SERVICE;  /* Execute in timer service thread */
    para.cb = nwy_test_cli_service_timer_cb;
    para.cb_para = NULL;
    para.expired_time = 500;  /* 0.5 seconds */
    para.type = NWY_TIMER_PERIODIC;

    if (s_nwy_service_timer == NULL)
    {
        ret = nwy_sdk_timer_create(&s_nwy_service_timer, &para);
        if (ret != NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nCreate service timer fail");
            return;
        }
    }

    ret = nwy_sdk_timer_start(s_nwy_service_timer, &para);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nStart service timer fail");
    } else {
        nwy_test_cli_echo("\r\nStart service timer success - Can call blocking functions");
    }
}

/* Demo user thread mode timer - execute in user specified thread */
void nwy_test_cli_start_user_timer(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_timer_para_t para;

    /* Create user thread */
    if (s_nwy_user_thread == NULL)
    {
        ret = nwy_thread_create(&s_nwy_user_thread, "timer_user_thread", 
                               NWY_OSI_PRIORITY_NORMAL, nwy_test_cli_user_thread_cb, 
                               NULL, 10, 1024*2, NULL);
        if (ret != NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nCreate user thread fail");
            return;
        }
        nwy_test_cli_echo("\r\nUser thread created successfully");
    }

    memset(&para, 0, sizeof(para));
    s_user_timer_count = 0;

    para.thread_hdl = s_nwy_user_thread;  /* Execute in user thread */
    para.cb = nwy_test_cli_user_timer_cb;
    para.cb_para = NULL;
    para.expired_time = 500;  /* 0.5 seconds */
    para.type = NWY_TIMER_PERIODIC;

    if (s_nwy_user_timer == NULL)
    {
        ret = nwy_sdk_timer_create(&s_nwy_user_timer, &para);
        if (ret != NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nCreate user timer fail");
            nwy_thread_exit(s_nwy_user_thread); 
            s_nwy_user_thread = NULL;
            return;
        }
    }

    ret = nwy_sdk_timer_start(s_nwy_user_timer, &para);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nStart user timer fail");
        nwy_sdk_timer_stop(s_nwy_user_timer);
        nwy_sdk_timer_destory(s_nwy_user_timer);
        s_nwy_user_timer = NULL;
        if (s_nwy_user_thread != NULL) {
            nwy_thread_exit(s_nwy_user_thread); 
            s_nwy_user_thread = NULL;
        }
        return;
    }
    nwy_test_cli_echo("\r\nStart user timer success - Executes in user thread context");
}

/* Demo all three modes running simultaneously */
void nwy_test_cli_start_all_timers(void)
{
    nwy_test_cli_echo("\r\n=== Starting all three timer modes ===");
    
    /* Start ISR mode timer */
    nwy_test_cli_start_isr_timer();
    nwy_thread_sleep(100);
    
    /* Start service thread mode timer */
    nwy_test_cli_start_service_timer();
    nwy_thread_sleep(100);
    
    /* Start user thread mode timer */
    nwy_test_cli_start_user_timer();
    
    nwy_test_cli_echo("\r\n=== All timers started - observe different execution contexts ===");
}

void nwy_test_cli_stop_timer(void)
{
    int ret = false;
    ret = nwy_sdk_timer_stop(s_nwy_test_timer);
    if (ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nStop timer sucess,s_timer_count = %d", s_timer_count);
    }
    else
    {
        nwy_test_cli_echo("\r\nStop timer fail");
    }
}

/* Stop all timers and show statistics */
void nwy_test_cli_stop_all_timers(void)
{
    nwy_test_cli_echo("\r\n=== Stopping all timers and showing statistics ===");
    
    /* Stop ISR timer */
    if (s_nwy_isr_timer != NULL) {
        nwy_sdk_timer_stop(s_nwy_isr_timer);
        nwy_sdk_timer_destory(s_nwy_isr_timer);
        s_nwy_isr_timer = NULL;
        nwy_test_cli_echo("\r\nISR timer stopped - Count: %d", s_isr_timer_count);
    }
    
    /* Stop service thread timer */
    if (s_nwy_service_timer != NULL) {
        nwy_sdk_timer_stop(s_nwy_service_timer);
        nwy_sdk_timer_destory(s_nwy_service_timer);
        s_nwy_service_timer = NULL;
        nwy_test_cli_echo("\r\nService timer stopped - Count: %d", s_service_timer_count);
    }
    
    /* Stop user thread timer */
    if (s_nwy_user_timer != NULL) {
        nwy_sdk_timer_stop(s_nwy_user_timer);
        nwy_sdk_timer_destory(s_nwy_user_timer);
        s_nwy_user_timer = NULL;
        nwy_test_cli_echo("\r\nUser timer stopped - Count: %d", s_user_timer_count);
    }
    
    /* Exit user thread */
    if (s_nwy_user_thread != NULL) {
        nwy_thread_exit(s_nwy_user_thread);
        s_nwy_user_thread = NULL;
        nwy_test_cli_echo("\r\nUser thread exited");
    }
    
    nwy_test_cli_echo("\r\n=== All timers stopped ===");
}

void nwy_test_cli_get_time(void)
{
    nwy_time_t julian_time = {0};
    int timezone = 0;
    nwy_date_get(&julian_time, &timezone);
    nwy_test_cli_echo("\r\n%d-%d-%d %d:%d:%d timezone = %d\r\n", julian_time.year, julian_time.mon, julian_time.day, julian_time.hour, julian_time.min, julian_time.sec, timezone);
}

void nwy_test_cli_set_time(void)
{
    char *sptr = NULL;
    int timezone = 0;
    int count = 0;
    nwy_time_t nwy_time = {2020, 3, 19, 20, 33, 30};

    sptr = nwy_test_cli_input_gets("\r\n12Please input data(2020-1-1): ");
    count = sscanf(sptr, "%d-%d-%d", &nwy_time.year, &nwy_time.mon, &nwy_time.day);
    if (count != 3) {
        nwy_test_cli_echo("\r\ninput data format is wrong\r\n");
        return ;
    }

    NWY_CLI_LOG("nwy_time: year = %d mon = %c day = %c ", nwy_time.year, nwy_time.mon, nwy_time.day);

    sptr = nwy_test_cli_input_gets("\r\nPlease input time(1:1:1): ");
    count = sscanf(sptr, "%d:%d:%d", &nwy_time.hour, &nwy_time.min, &nwy_time.sec);
    if (count != 3) {
        nwy_test_cli_echo("\r\ninput time format is wrong\r\n");
        return ;
    }



    sptr = nwy_test_cli_input_gets("\r\nPlease input timezone: ");
    timezone = atoi(sptr);

    if (nwy_cli_data_valid_check(&nwy_time, timezone) != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nInput data invalid\n");
        return;
    }

    nwy_date_set(&nwy_time, timezone);

    nwy_test_cli_echo("\r\nset time sucess\r\n");
    nwy_test_cli_get_time();
}

void nwy_test_cli_test_semp(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    s_nwy_semaphore_count = 0;

    ret = nwy_semaphore_create(&s_nwy_cli_semaphore, 1);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nsem test fail");
        return ;
    }

    if (s_nwy_task_A != NULL) {
        nwy_thread_exit(s_nwy_task_A);
    }

    if (s_nwy_task_B != NULL) {
        nwy_thread_exit(s_nwy_task_B);
    }
    nwy_thread_create(&s_nwy_task_A, "nwy_test_cli_semaphoreA", NWY_OSI_PRIORITY_NORMAL, nwy_cli_semaphore_taskA_proc, NULL, 8, NWY_TASK_STACK_SIZE, NULL);
    nwy_thread_create(&s_nwy_task_B, "nwy_test_cli_semaphoreB", (NWY_OSI_PRIORITY_NORMAL + 1), nwy_cli_semaphore_taskB_proc, NULL, 8, NWY_TASK_STACK_SIZE, NULL);
}

void nwy_test_cli_virt_at_init(void)
{
    int ret = NWY_GEN_E_UNKNOWN;
    
    if (s_nwy_at_init_flag == 0)
    {
        ret = nwy_virt_at_parameter_init();
        if (ret == NWY_SUCCESS) {
            s_nwy_at_init_flag = 1;
            nwy_test_cli_echo("\r\nVirtual AT initialization successful\r\n");
        } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT){
            nwy_test_cli_echo("\r\nPlatform does not support virtual AT function\r\n");
            return;
        } else {
            nwy_test_cli_echo("\r\nVirtual AT initialization failed\r\n");
            return;
        }
    } else {
        nwy_test_cli_echo("\r\nVirtual AT has already been initialized\r\n");
    }
}

void nwy_test_cli_reg_at_unsolicited(void)
{
    char *sptr = NULL;
    int ret = NWY_GEN_E_UNKNOWN;
    
    if (s_nwy_at_init_flag == 0)
    {
        nwy_test_cli_echo("\r\nPlease initialize virtual AT first\r\n");
        return;
    }
    
    sptr = nwy_test_cli_input_gets("\r\nPlease input AT unsolicited command to register,eg:+CREG ");
    
    if (strlen(sptr) == 0 || sptr[0] !='+') {
        nwy_test_cli_echo("\r\nInput err\r\n");
        return;
    }
    
    ret = nwy_virt_at_unsolicited_cb_reg(sptr, nwy_cli_unsol_at_cb);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nSuccessfully registered AT unsolicited: %s\r\n", sptr);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nPlatform does not support this function\r\n");
    } else {
        nwy_test_cli_echo("\r\nFailed to register AT unsolicited\r\n");
    }
}

// Send virtual AT test function
void nwy_test_cli_send_virt_at(void)
{
    if (s_nwy_at_init_flag == 0)
    {
        nwy_test_cli_echo("\r\nPlease initialize virtual AT first\r\n");
        return;
    }

    nwy_cli_virtual_at_test();
}
void nwy_test_cli_reg_at_fwd(void)
{
    int ret = NWY_GEN_E_UNKNOWN;

    ret = nwy_virt_at_forward_cb(1, "+FYTEST1", nwy_at_cmd_process_callback);
    if (ret == NWY_SUCCESS) {

    } else  if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT){
        nwy_test_cli_echo("\r\nnot support\r\n");
        return;
    } else {
        nwy_test_cli_echo("\r\nregister AT failed\r\n");
        return ;
    }
}

void priority_test_callback(void *param)
{
    nwy_error_e ret;
    uint8 current_priority = 0;
    
    // Get initial priority
    ret = nwy_thread_get_priority(priority_test_thread, &current_priority);
    if(ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nInitial priority: %d", current_priority);
    } else {
        nwy_test_cli_echo("\r\nFailed to get initial priority");
    }
    
    // Test setting different priorities
    uint8 test_priorities[] = {
        NWY_OSI_PRIORITY_IDLE,
        NWY_OSI_PRIORITY_LOW, 
        NWY_OSI_PRIORITY_NORMAL,
        NWY_OSI_PRIORITY_ABOVE_NORMAL,
        NWY_OSI_PRIORITY_HIGH
    };
    
    for(int i = 0; i < sizeof(test_priorities)/sizeof(test_priorities[0]); i++) {
        // Set new priority
        ret = nwy_thread_set_priority(priority_test_thread, test_priorities[i]);
        if(ret == NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nSuccessfully set priority to %d", test_priorities[i]);
        } else {
            nwy_test_cli_echo("\r\nFailed to set priority to %d", test_priorities[i]);
        }
        
        // Get and verify new priority
        ret = nwy_thread_get_priority(priority_test_thread, &current_priority);
        if(ret == NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nCurrent priority: %d", current_priority);
            if(current_priority != test_priorities[i]) {
                nwy_test_cli_echo("\r\nWarning: Priority mismatch!");
            }
        } else {
            nwy_test_cli_echo("\r\nFailed to get current priority");
        }
        
        nwy_thread_sleep(100); // Wait a bit between tests
    }
    
    nwy_test_cli_echo("\r\nPriority test completed");
    nwy_semahpore_release(test_complete_sem);
    nwy_thread_exit(priority_test_thread);
}

void nwy_test_cli_thread_priority_test(void)
{
    nwy_test_cli_echo("\r\nStarting thread priority test...");
    
    nwy_semaphore_create(&test_complete_sem, 0);
    
    nwy_thread_create(&priority_test_thread,
        "priority_test",
        NWY_OSI_PRIORITY_ABOVE_NORMAL,
        priority_test_callback,
        NULL,
        8,
        2048,
        NULL);
        
    if(priority_test_thread == NULL) {
        nwy_test_cli_echo("\r\nFailed to create test thread!");
        nwy_semahpore_delete(test_complete_sem);
        return;
    }
    
     nwy_semaphore_acquire(test_complete_sem, NWY_OSA_SUSPEND);//wait test complete and return
    
     nwy_semahpore_delete(test_complete_sem);
    nwy_test_cli_echo("\r\nTest thread completed and cleaned up");
}

// Test configuration parameters
#define NWY_TASK_STACK_SIZE     2048
#define TEST_SENDER_THREAD_COUNT 10    // Number of sender threads
#define TEST_MSG_COUNT 10             // Number of messages per sender thread
#define TEST_EVENT_QUEUE_SIZE 100     // Size of event queue for receiver thread

// Global variables for test
static nwy_osi_thread_t g_receiver_thread = NULL;
static nwy_osi_thread_t g_sender_threads[TEST_SENDER_THREAD_COUNT] = {0};
static uint64_t g_start_times[TEST_SENDER_THREAD_COUNT] = {0};
static uint64_t g_total_times[TEST_SENDER_THREAD_COUNT] = {0};
static int g_received_counts[TEST_SENDER_THREAD_COUNT] = {0};
static bool g_test_complete = false;

// Receiver thread function
static void event_receiver_thread(void *ctx)
{
    nwy_event_msg_t msg;
    nwy_test_cli_echo("\r\nReceiver thread started");  // Debug log
    
    while (!g_test_complete) {
        nwy_error_e ret = nwy_thread_event_wait(g_receiver_thread, &msg, 1000); // Add timeout
        if (ret == NWY_SUCCESS) {
            int sender_idx = msg.param1;
            uint64_t end_time = nwy_uptime_get();
            g_total_times[sender_idx] += (end_time - msg.param2);
            g_received_counts[sender_idx]++;
            
            // Print progress every 10 messages
            if (g_received_counts[sender_idx] % 10 == 0) {
                nwy_test_cli_echo("\r\nReceived %d messages from sender %d", 
                    g_received_counts[sender_idx], sender_idx);
            }
        }
    }
    nwy_test_cli_echo("\r\nReceiver thread exiting");  // Debug log
    nwy_thread_exit(NULL);
}

// Sender thread function
static void event_sender_thread(void *ctx)
{
    int sender_idx = (int)ctx;
    nwy_event_msg_t msg;
    
    // Debug log for thread start
    nwy_test_cli_echo("\r\nSender thread %d started", sender_idx);
    
    for (int i = 0; i < TEST_MSG_COUNT && !g_test_complete; i++) {
        msg.param1 = sender_idx;
        msg.param2 = nwy_uptime_get();
        nwy_error_e ret = nwy_thread_event_send(g_receiver_thread, &msg, 100); // Add timeout
        
        if (ret != NWY_SUCCESS) {
            nwy_test_cli_echo("\r\nSender %d failed to send message %d", sender_idx, i);
        }
        
        nwy_thread_sleep(5);  // Small delay between messages
    }
    
    // Debug log for thread completion
    nwy_test_cli_echo("\r\nSender thread %d completed", sender_idx);
    nwy_thread_exit(NULL);
}

// Main test function for event wait performance testing
void nwy_test_cli_event_wait_performance(void)
{
    char thread_name[32];
    g_test_complete = false;
    
    // Reset statistics
    memset(g_total_times, 0, sizeof(g_total_times));
    memset(g_received_counts, 0, sizeof(g_received_counts));
    memset(g_sender_threads, 0, sizeof(g_sender_threads));
    
    nwy_test_cli_echo("\r\nInitializing event wait performance test...");
    
    // Create receiver thread
    nwy_thread_create(&g_receiver_thread, 
                     "event_receiver",
                     NWY_OSI_PRIORITY_HIGH,
                     event_receiver_thread,
                     NULL,
                     TEST_EVENT_QUEUE_SIZE,  // Large event queue
                     NWY_TASK_STACK_SIZE * 2,  // Double stack size
                     NULL);
                     
    if (!g_receiver_thread) {
        nwy_test_cli_echo("\r\nFailed to create receiver thread!");
        return;
    }
    
    // Wait for receiver thread to initialize
    nwy_thread_sleep(100);
    
    nwy_test_cli_echo("\r\nCreating %d sender threads...", TEST_SENDER_THREAD_COUNT);
    
    // Create sender threads
    for (int i = 0; i < TEST_SENDER_THREAD_COUNT; i++) {
        snprintf(thread_name, sizeof(thread_name), "sender_%d", i);
        nwy_thread_create(&g_sender_threads[i],
                         thread_name,
                         NWY_OSI_PRIORITY_NORMAL,
                         event_sender_thread,
                         (void*)i,
                         8,
                         NWY_TASK_STACK_SIZE,
                         NULL);
                         
        if (!g_sender_threads[i]) {
            nwy_test_cli_echo("\r\nFailed to create sender thread %d!", i);
            g_test_complete = true;
            return;
        }
        
        nwy_thread_sleep(10);  // Give each thread time to start
    }
    
    nwy_test_cli_echo("\r\nAll threads created, waiting for completion...");
    
    // Wait for all messages to be received
    int timeout_count = 0;
    while (1) {
        int total_received = 0;
        for (int i = 0; i < TEST_SENDER_THREAD_COUNT; i++) {
            total_received += g_received_counts[i];
        }
        
        // Show progress periodically
        if (timeout_count % 10 == 0) {
            nwy_test_cli_echo("\r\nProgress: %d/%d messages received", 
                total_received, TEST_SENDER_THREAD_COUNT * TEST_MSG_COUNT);
        }
        
        if (total_received >= TEST_SENDER_THREAD_COUNT * TEST_MSG_COUNT) {
            break;
        }
        
        nwy_thread_sleep(100);
        timeout_count++;
        
        // Timeout protection
        if (timeout_count > 100) {  // 10 second timeout
            nwy_test_cli_echo("\r\nTest timeout!");
            break;
        }
    }
    
    // Test completion and results
    g_test_complete = true;
    nwy_thread_sleep(100);  // Wait for threads to finish
    
    nwy_test_cli_echo("\r\n\nTest Results:");
    nwy_test_cli_echo("\r\nThread Index | Messages Received | Avg Delay (ms)");
    nwy_test_cli_echo("\r\n------------|------------------|---------------");
    
    for (int i = 0; i < TEST_SENDER_THREAD_COUNT; i++) {
        float avg_delay = g_received_counts[i] > 0 ? 
            (float)g_total_times[i] / g_received_counts[i] : 0;
            
        nwy_test_cli_echo("\r\n%11d | %16d | %13.2f", 
                         i, 
                         g_received_counts[i], 
                         avg_delay);
    }
    
    nwy_test_cli_echo("\r\n\nTest completed!");
}

// CLI demo: date <-> timestamp conversion
void nwy_test_cli_date_timestamp(void)
{
    char *sptr = NULL;
    nwy_time_t date = {0};
    nwy_timeval_t timestamp = {0};
    int count = 0;
    int ret = 0;

    nwy_test_cli_echo("\r\n[Date to Timestamp] Please input date (YYYY-MM-DD): ");
    sptr = nwy_test_cli_input_gets("");
    count = sscanf(sptr, "%hu-%hhu-%hhu", &date.year, &date.mon, &date.day);
    if (count != 3) {
        nwy_test_cli_echo("\r\nInput date format is wrong!\r\n");
        return;
    }
    nwy_test_cli_echo("\r\n[Date to Timestamp] Please input time (HH:MM:SS): ");
    sptr = nwy_test_cli_input_gets("");
    count = sscanf(sptr, "%hhu:%hhu:%hhu", &date.hour, &date.min, &date.sec);
    if (count != 3) {
        nwy_test_cli_echo("\r\nInput time format is wrong!\r\n");
        return;
    }
    ret = nwy_date_to_timestamp(&date, &timestamp);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\n[Date to Timestamp] Result: %ld seconds, %ld microseconds\r\n", timestamp.tv_sec, timestamp.tv_usec);
    } else {
        nwy_test_cli_echo("\r\n[Date to Timestamp] Conversion failed!\r\n");
    }

    // Timestamp to date
    nwy_test_cli_echo("\r\n[Timestamp to Date] Please input timestamp seconds: ");
    sptr = nwy_test_cli_input_gets("");
    timestamp.tv_sec = atol(sptr);
    nwy_test_cli_echo("\r\n[Timestamp to Date] Please input timestamp microseconds: ");
    sptr = nwy_test_cli_input_gets("");
    timestamp.tv_usec = atol(sptr);
    memset(&date, 0, sizeof(date));
    ret = nwy_timestamp_to_date(&timestamp, &date);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\n[Timestamp to Date] Result: %04d-%02d-%02d %02d:%02d:%02d\r\n", date.year, date.mon, date.day, date.hour, date.min, date.sec);
    } else {
        nwy_test_cli_echo("\r\n[Timestamp to Date] Conversion failed!\r\n");
    }
}

