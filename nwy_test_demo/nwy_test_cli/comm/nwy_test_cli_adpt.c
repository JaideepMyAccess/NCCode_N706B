#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_func_def.h"
#include "nwy_cust.h"
#include "jsmn.h"
#include "global.h"
static nwy_osi_thread_t nwy_test_cli_thread = NULL;
static const char APP_VERSION[65] = "NWY_APP_V1.0.1";

extern void nwy_test_cli_menu_display();
extern void nwy_test_cli_menu_back();
extern void nwy_test_cli_menu_select(int opt);

extern void nwy_test_cli_get_iccid_new();
extern void nwy_test_cli_get_model();
extern void nwy_test_cli_get_sw_ver();
extern void nwy_test_cli_get_imei();

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
    nwy_test_cli_echo("\r\nBase version: %s\r\n", base_sdk_version_buf);
    nwy_test_cli_echo("\r\nApp version: %s\r\n", APP_VERSION);
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

// Code Declarations Starts

#define I2C_IO_Pin 87



// // ==============================
// // 📦 1. Struct Declarations
// // ==============================
// typedef struct {
//     int LTEStatus;
//     int I2CStatus;
//     int AWSStatus;
// } ChipConfigStruct;

// typedef struct {
//   int StockStatus;
//   int PStockStatus;
//   int HeaterATemp;
//   int HeaterBTemp;
//   int HeaterAStatus;
//   int HeaterBStatus;
// } MachineReadingStruct;

// typedef struct {
//     int techConfig;
// } AWSTopicsStruct;

// typedef struct {
//     int IncinBatchID;
//     int IncinBurnNapkinTime;
// } IncinDataStruct;

// ==============================
// 🧾 2. Global Struct Variables
// ==============================
MachineReadingStruct machineReadings = {
    .StockStatus = 2,
    .PStockStatus = 2,
    .HeaterATemp = 25,
    .HeaterBTemp = 25,
    .HeaterAStatus = 1,
    .HeaterBStatus = 1
};

ChipConfigStruct chipConfig = {
    .I2CStatus = 0,
    .LTEStatus = 0,
    .AWSStatus = 0
};

AWSTopicsStruct awsTopic = {
    .techConfig = 0
};

// ==============================
// 🔧 3. Technical Config Data
// ==============================
int sta = 0;
int stb = 0;
int ham = 0;
int hbo = 0;
int bct = 0;
int bcc = 0;
int hur = 0;
int min = 0;

// ==============================
// 🏢 4. Business Config Data
// ==============================
int iid = 0;
int itp = 0;
char qrb_data[1024] = {0};
uint8_t qrb_array[512];

// ==============================
// 🔥 5. Incinerator Config & Status
// ==============================
char IncinBatchID[50];
int Incin_cycle = 0;
int IncinTriggerState = 0;
int IncinTotalNapkinBurn = 0;
char IncinStartTime[30];
char IncinEndTime[20];
int isIncinerationPaused = 0;
bool IncinTriggeredByRestart = false;
bool IncinTriggerBySchedule = false;
bool IncinTriggerByResume = false;
bool IncinTriggerByInitial = false;

// ==============================
// ⏱️ 6. Time & Logging
// ==============================
char CurrentTimeString[22];
char CurrentTime[30];
time_t current_epoch = 0;
int64_t incinTime = 0;
int64_t insun_burn_time = 0;

// ==============================
// 💡 7. Hardware/Peripheral Status
// ==============================
bool I2C_Connected = false;
bool I2cBusy = false;
bool i2c_state_change = false;
bool heaterA_status = false;
bool heaterB_status = false;

// ==============================
// 🌐 8. Network & Coin Status
// ==============================
bool NetworkStateChanged = true;
bool CoinStateChanged = false;
int coin_pulse = 0;
bool PaymentScreenActive = false;

// ==============================
// ⚙️ 9. System State Flags
// ==============================
// bool CustomCode = true;
bool technicalConfigFound = false;
bool businessConfigFound = false;
bool isMachineConfigFound = false;
bool ValueChanged = false;
bool already_triggered_today = false;

// ==============================
// 📦 10. Stock & Monitoring
// ==============================
int StockLevel = 0;
int TBackNoStock = 0;
int TBackLowStock = 0;
bool UpdateStockToServer = false;

// ==============================
// 🌡️ 11. Chamber Sensors
// ==============================
int chamberA_temp = 0;
int chamberA_disp = 0;
int chamberB_temp = 0;
int chamberB_disp = 0;

// ==============================
// 🧾 12. Identity and Metadata
// ==============================
// char MAC_ID[10] = "RZ1954";
// char MERCHANT_ID[15] = "ZEST250507";
char MAC_ID[10] = "";
char MERCHANT_ID[15] = "";
char MERCH_KEY[5] = "INTG";
char VersionNumber[6] = "2.0";
char ImeiNumber[20]= {0};

// ==============================
// 📁 13. FTP Configuration
// ==============================
char FTP_PATH[128];
char FTP_USER[64];
char FTP_PASS[64];
char FTP_IP[64];

// ==============================
// 📝 14. Utilities
// ==============================
char json_line[1024] = {0};
bool napkinOfflineLogDataState = false;
bool incineratorOfflineLogDataState = false;

bool NeedToCheckInsunTime = false;
bool NeededDefaultScreen = false;
bool NeededInsuinScreen = false;
bool InterruptForDispense = true;
bool FotaUpdate = false;
bool IsInstulationOperation = false;

int64_t lcd_last_display_time = 0;
int64_t lcd_insun_last_display_time = 0;

// Custom Code Trigger
bool CustomCode = true;
static void customGpio(int param)
{
    nwy_test_cli_echo("\r\n//-----------------------------------\r\nInterrupt Triggered\r\n-----------------------------------//\r\n");
    // i2c_state_change = true;
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
    sptr = nwy_test_cli_input_gets("\r\nSend data to start the Execution of the Program: \r\n");
    nwy_test_cli_echo("\r\nHardware Version: N706B-CN-10, SDK Firmware: 009, Software Firmware: V1.00 \r\n");
    nwy_test_cli_get_model();
    nwy_test_cli_get_imei();
    nwy_test_cli_get_iccid_new();
    // nwy_test_cli_get_sw_ver();
    nwy_test_cli_get_version();
    // Custom Code 2 Execution

    if(CustomCode){
        nwy_thread_sleep(100);
        // GPIO Interrupt for I2C
        // int data = nwy_gpio_irq_register(I2C_IO_Pin,1,2,customGpio,NULL);
        // if (!data)
        // {
        //     nwy_test_cli_echo("\r\nGpio isr register success! --- ");
        // }
        // else
        // {
        //     nwy_test_cli_echo("\r\nGpio isr register failed! --- ");
        // }

        // int ret2 = nwy_gpio_irq_enable(I2C_IO_Pin);

        // if(0 == ret2)
        //     nwy_test_cli_echo(" Gpio enable isr success!\r\n");
        // else
        //     nwy_test_cli_echo(" Gpio enable isr fail!\r\n");

        // int64_t now_us = nwy_uptime_get();
        // nwy_test_cli_echo("Current uptime: %lld us\n", now_us);

        // if (load_machine_config_values()) {
            // Use the values...
            nwy_test_cli_echo("\r\nMachineConfig Load Success!\r\n");
            nwy_test_cli_echo("\r\nMAC ID: %s\r\n", MAC_ID);
            nwy_test_cli_echo("\r\nMERCHANT ID: %s\r\n", MERCHANT_ID);
            isMachineConfigFound = true;
            
        // }else{
        //     nwy_test_cli_echo("\r\nMachineConfig Load Failed!");
        //     // 2nd Time
        //     if (load_machine_config_values()) {
        //         // Use the values...
        //         nwy_test_cli_echo("\r\nMachineConfig Load 2nd time Success!\r\n");
        //         nwy_test_cli_echo("\r\nMAC ID: %s\r\n", MAC_ID);
        //         nwy_test_cli_echo("\r\nMERCHANT ID: %s\r\n", MERCHANT_ID);
        //         isMachineConfigFound = true;
                
        //     }else{
        //         nwy_test_cli_echo("\r\nMachineConfig Load 2nd time Failed!");
        //         isMachineConfigFound = false;
        
        //         lcd_init();
        //         nwy_thread_sleep(100);
        //         lcd_clear();
        //         nwy_thread_sleep(100);
        //         Display(0, PAGE2, 0, " Waiting For Machine   ");
        //         Display(0, PAGE4, 0, "    Configuration      ");
        //     }
        // }

        // nwy_test_i2c();
        // nwy_test_lcd();

        // For Data Connection
        nwy_test_cli_set_profile_new();
        nwy_thread_sleep(1000);
        nwy_test_cli_data_start_new();
        nwy_thread_sleep(1000);
        nwy_thread_sleep(1000);
        nwy_test_cli_mqtt_connect_new();

        while(1){
            nwy_test_cli_echo("\r\n===Message Received======");
            nwy_thread_sleep(1000);
        }

    }else{
        // -------------- Default Code ---------------
        while (1)
        {
            nwy_test_cli_menu_display();
            sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");
            if (sptr[0] == 'q' || sptr[0] == 'Q')
            {
                nwy_test_cli_menu_back();
            }
            else
            {
                nwy_test_cli_menu_select(atoi(sptr));
            }

        }
    }
    
   
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

