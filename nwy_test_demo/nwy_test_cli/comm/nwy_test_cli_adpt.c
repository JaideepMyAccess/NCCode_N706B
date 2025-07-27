#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_func_def.h"
#include "nwy_cust.h"
static nwy_osi_thread_t nwy_test_cli_thread = NULL;
static const char APP_VERSION[65] = "NWY_APP_V1.0.1";

extern void nwy_test_cli_menu_display();
extern void nwy_test_cli_menu_back();
extern void nwy_test_cli_menu_select(int opt);

void nwy_test_cli_dbg(const char *func, int line, char *fmt, ...)
{
    static char buf[1024];
    va_list args;
    int len = 0;

    memset(buf, 0, sizeof(buf));

    sprintf(buf, "NWY_CLI %s[%d]:", func, line);
    len = strlen(buf);
    va_start(args, fmt);

    vsnprintf(&buf[len], sizeof(buf) - len - 1, fmt, args);
    va_end(args);

    NWY_SDK_LOG_DEBUG("%s", buf);

}

int nwy_test_cli_wait_select()
{
    nwy_event_msg_t event;

    while (1)
    {
        memset(&event, 0, sizeof(event));

        nwy_thread_event_wait(nwy_test_cli_thread, &event, NWY_OSA_SUSPEND);

        if (event.id == NWY_EXT_INPUT_RECV_MSG)
        {
            return 1;
        }
    }
}

void nwy_test_cli_select_enter()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    event.id = NWY_EXT_INPUT_RECV_MSG;
    nwy_thread_event_send(nwy_test_cli_thread, &event, NWY_OSA_SUSPEND);
}

void nwy_test_cli_send_trans_end()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    event.id = NWY_EXT_DATA_REC_END_MSG;
    nwy_thread_event_send(nwy_test_cli_thread, &event, NWY_OSA_SUSPEND);
}

int nwy_test_cli_wait_trans_end()
{
    nwy_event_msg_t event;

    memset(&event, 0, sizeof(event));
    nwy_thread_event_wait(nwy_test_cli_thread, &event, NWY_OSA_SUSPEND);
    if (event.id == NWY_EXT_DATA_REC_END_MSG)
    {
        return 1;
    }
    return 0;
}

void nwy_test_cli_get_version(void)
{
    char base_sdk_version_buf[100] = {0};
    if(nwy_dm_open_sdk_version_get(base_sdk_version_buf, sizeof(base_sdk_version_buf)) != NWY_SUCCESS)//get sdk version in base firmware
    {
        NWY_CLI_LOG("[%s]app get sdkversion fail",__func__);
        return;
    }
    nwy_test_cli_echo("\r\n base version: %s\r\n", base_sdk_version_buf);
    nwy_test_cli_echo("\r\n app version: %s\r\n", APP_VERSION);
}

void nwy_get_base_fota_result()
{
    int fota_flag;
    int fd;
    int wsize;
    
    fd = nwy_file_open("nwy_base_fota_flag", NWY_RB_MODE);
    if(fd < 0)
    {
        return;
    }
    nwy_file_seek(fd, 0, SEEK_SET);
    wsize = nwy_file_read(fd, &fota_flag, sizeof(fota_flag));
    if(wsize != sizeof(fota_flag))
    {
        nwy_file_close(fd);
        return;
    }
    nwy_file_close(fd);

    if(fota_flag == 1)
        nwy_test_cli_echo("\r\nbase version fota success\r\n");

    nwy_file_remove("nwy_base_fota_flag");
}

static void nwy_test_cli_main_func(void *param)
{
    char *sptr = "";
    char chip_platform[21] = {0};
    nwy_error_e ret;
      
    nwy_usb_serial_reg_recv_cb((nwy_sio_recv_cb_t)nwy_test_cli_sio_data_proc);
    ret = nwy_dm_soc_model_get(chip_platform, 20);
    if(NWY_SUCCESS == ret && 0 == strcmp(chip_platform, "ASR1605S"));
    {
        nwy_get_base_fota_result();
    }


    nwy_thread_sleep(100);

    // Custom Code 1 Execution
    sptr = nwy_test_cli_input_gets("\r\n Send data to start the Execution of the Program: \r\n");
    nwy_test_cli_echo("\r\n Hardware Version: N706B-CN-10, SDK Firmware: 009, Software Firmware: V1.00 \r\n");

    // Custom Code 2 Execution
    // nwy_test_i2c();
    // nwy_test_lcd();

    // For Data Connection
    nwy_test_cli_set_profile_new();
    nwy_thread_sleep(1000);
    nwy_test_cli_data_start_new();


    // -------------- Default Code ---------------
    // while (1)
    // {
    //     nwy_test_cli_menu_display();
    //     sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");
    //     if (sptr[0] == 'q' || sptr[0] == 'Q')
    //     {
    //         nwy_test_cli_menu_back();
    //     }
    //     else
    //     {
    //         nwy_test_cli_menu_select(atoi(sptr));
    //     }

    // }
}
bool compare_version_prefix(const char *ver1, const char *ver2)//check string before last '-'
{
    const char *last_dash1 = strrchr(ver1, '-');
    const char *last_dash2 = strrchr(ver2, '-');
    size_t cmp_len = 0;
    size_t ver1_len = 0;
    size_t ver2_len = 0;
    int result = 0;
    if (!last_dash1 || !last_dash2)
    {
        return false; 
    }
    ver1_len = last_dash1 - ver1;
    ver2_len= last_dash2 - ver2;
    if(ver1_len != ver2_len)
    {
        return false;
    }
    cmp_len = ver1_len;
    result = strncmp(ver1, ver2, cmp_len);
    return (bool)(result == 0);
}
bool nwy_opensdk_version_check(void)//check sdkversion between app and base firmware
{
    char base_sdk_version_buf[100] = {0};
    if(nwy_dm_open_sdk_version_get(base_sdk_version_buf, sizeof(base_sdk_version_buf)) != NWY_SUCCESS)//get sdk version in base firmware
    {
        NWY_CLI_LOG("[%s]app get sdkversion fail",__func__);
        return false;
    }

    NWY_CLI_LOG("[%s]app get base sdkversion %s,app sdkversion %s",__func__,base_sdk_version_buf,NWY_OPEN_SDK_VERSION_STRING);

    if(!compare_version_prefix(base_sdk_version_buf,NWY_OPEN_SDK_VERSION_STRING))//compare sdkversion in app and sdkversion in base firmware
    {
        NWY_CLI_LOG("[%s]base sdkversion not match app sdkversion",__func__);
        return false;
    }
    NWY_CLI_LOG("[%s]SDK version check pass",__func__);
    return true;
}

#ifdef FEATURE_NWY_ASR_PLAT
int nwy_open_app_entry()
#else
int appimg_enter(void *param)
#endif
{
    nwy_thread_sleep(10 * 1000);
	char APP_BUILD_TIME[65]= {0};
    char version[70]={0};
    nwy_error_e ret = NWY_FAIL;

    sprintf(version,"\"%s\"", APP_VERSION);
    sprintf(APP_BUILD_TIME,"\"%s,%s\"", __DATE__,__TIME__);
    NWY_CLI_LOG("nwy_open_app_enter ...");

    ret = nwy_dm_app_version_set(version,strlen(version));

    if (ret == NWY_SUCCESS)
    {
        NWY_CLI_LOG("Set APP version success,use (AT+NAPPCHECK?) to check ver:%s",version);
    }
    else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT)
    {
        NWY_CLI_LOG("This plat do not support set app version!");
    }
    else
    {
        NWY_CLI_LOG("Set APP version failed!");
    }
/*
 *Need to be used in conjunction with FEATURE-NWY-POEN_SDK_VER-NOBIND
*/
/*
    if(!nwy_opensdk_version_check())
    {
        NWY_CLI_LOG("[%s]base sdkversion not match app sdkversion,quit app",__func__);
        return -1;
    }
*/
    nwy_thread_create(&nwy_test_cli_thread, "test-cli", NWY_OSI_PRIORITY_NORMAL, nwy_test_cli_main_func, NULL, 16, 1024 * 32, NULL);
    return 0;
}

void appimg_exit(void)
{

    NWY_CLI_LOG("application image exit");
}

int nwy_test_cli_check_uart_mode(uint8_t mode)
{
    return 0;
}

void _gpioisropen(void *param)
{
    NWY_CLI_LOG("nwy gpio isr set success");
}

