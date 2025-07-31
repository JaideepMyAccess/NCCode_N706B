#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_func_def.h"
#include "nwy_cust.h"
#include "jsmn.h"
#include "global.h"
#include "Image1.c"
#include "InitialLogo.c"
#include "Bar.c"
static nwy_osi_thread_t nwy_test_cli_thread = NULL;
static nwy_osi_thread_t nwy_test_cli_thread2 = NULL;
static nwy_osi_thread_t nwy_test_cli_thread3 = NULL;
static const char APP_VERSION[65] = "NWY_APP_V1.0.1";

extern void nwy_test_cli_menu_display();
extern void nwy_test_cli_menu_back();
extern void nwy_test_cli_menu_select(int opt);

extern void nwy_test_cli_get_iccid_new();
extern void nwy_test_cli_get_model();
extern void nwy_test_cli_get_sw_ver();
extern void nwy_test_cli_get_imei();
extern void update_temperature();

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
int sta = 950;
int stb = 650;
int ham = 900;
int hbo = 600;
int bct = 40;
int bcc = 10;
int hur = 18;
int min = 0;

// ==============================
// 🏢 4. Business Config Data
// ==============================
int iid = 8;
int itp = 10;
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
char MAC_ID[10] = "RZ1954";
char MERCHANT_ID[15] = "ZEST250507";
// char MAC_ID[10] = "";
// char MERCHANT_ID[15] = "";
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
bool NetworkConnectStatus = false;

int64_t lcd_last_display_time = 0;
int64_t lcd_insun_last_display_time = 0;

bool NetworkDisconnectStatus = false;
char CurrentTimeString[22];
char IncinBatchID[50];

// Custom Code Trigger
bool CustomCode = true;
extern unsigned char cmd_flag[8];

static void customGpio(int param)
{
    nwy_test_cli_echo("\r\n//-----------------------------------\r\nInterrupt Triggered\r\n-----------------------------------//\r\n");
    i2c_state_change = true;
}

static void nwy_test_cli_main_func3(void *param)
{
    while(1){
        nwy_thread_sleep(1000);
        if(NetworkConnectStatus){

        }else{
            nwy_test_cli_echo("\r\n ********** Network Connection Process Started **********");

            if(NetworkDisconnectStatus){
                nwy_test_cli_echo("\nDisconnecting from Profile Data Connection & AWS MQTT ");
                nwy_test_cli_mqtt_disconnect();
                nwy_thread_sleep(100);
                nwy_test_cli_data_stop_fixed();
                nwy_thread_sleep(100);
            }

            DispBarImage(image_data_circle_Image);
            // For Data Connection
            nwy_test_cli_set_profile_new();
            nwy_thread_sleep(100);
            nwy_test_cli_data_start_new();
            nwy_thread_sleep(1000);
          
            
            // nwy_data_test();
            nwy_test_cli_echo("\r\n ********** Network Connection Process Ended **********");
        
            nwy_test_cli_echo("\r\n ********** Current Time Fetch Process Started **********");
        
            nwy_time_t julian_time = {0};
            char timezone =0;
            nwy_date_get(&julian_time, &timezone);
            nwy_test_cli_echo("\r\n%d-%d-%d %d:%d:%d", julian_time.year,julian_time.mon,julian_time.day, julian_time.hour,julian_time.min,julian_time.sec);
        
            // // Incin Batch ID
            // FetchCurrentTimeF2();
            // snprintf(IncinBatchID, sizeof(IncinBatchID), "CM%sIN%s", MAC_ID, CurrentTimeString);
            
        
            nwy_test_cli_echo("\r\n ********** Current Time Fetch Process Ended **********");
        
        
            // nwy_test_cli_echo("\r\n ********** HTTPS Process Started **********");
        
            // nwy_http_test();
        
            // nwy_https_get_with_cert("SF1001");
        
            // nwy_test_cli_echo("\r\n ********** HTTPS Process Ended **********");
            
            
            nwy_test_cli_echo("\r\n ********** AWS MQTT Connection Process Started **********");
            
            // if(NetworkDisconnectStatus){
            //     nwy_test_cli_echo("\nDisconnecting from AWS MQTT");
            //     nwy_test_cli_mqtt_disconnect();
            //     nwy_thread_sleep(100);
            //     nwy_test_cli_data_stop_fixed();
            //     nwy_thread_sleep(100);
            // }
            
            nwy_thread_sleep(3000);
            nwy_test_cli_echo("\nConnecting to AWS MQTT");
            
            nwy_test_cli_mqtt_connect_new();

            if(NetworkConnectStatus){
                nwy_test_cli_echo("\r\n Network Connection Established!");
                DispBarImage(image_data_bar_Image);

                // send_incinerator_info_status(
                //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn);

                uint8_t new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
                memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);

                // send_inventory_update_message(1,machineReadings.StockStatus);
                UpdateStockToServer = false;

                // Fota Response
                // send_config_ack(4,2);
            }else{
                DispBarImage(image_data_clear_Image);
                nwy_test_cli_echo("\r\n Network Connection Failed!");
            }
        

            nwy_test_cli_echo("\r\n ********** AWS MQTT Connection Process Ended **********");
            nwy_thread_sleep(10000);
        }
    }
}
static void nwy_test_cli_main_func2(void *param)
{

while(1){
        if(i2c_state_change && !IsInstulationOperation){
            nwy_test_cli_echo("***************** Sending I2C Command to Fetch the Triggred Data from STM32 ***************\n");
            nwy_thread_sleep(100);
            uint8_t new_cmd_flag[8] = { 0x18, 0x18, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00};
            memcpy(cmd_flag, new_cmd_flag, sizeof(cmd_flag));
            nwy_i2c_send_data();
            i2c_state_change = false;
        }
        if(IncinTriggerBySchedule){
            IncinTriggerState = 3;
            // heater_process();
            already_triggered_today = true;
            IncinTriggerBySchedule = false;
        }
        if(IncinTriggerByResume){
            // IncinTriggeredByRestart = true;
            // heater_process();
            // IncinTriggeredByRestart = false;
            IncinTriggerByResume = false;
        }
        if(IncinTriggerByInitial){
            IncinTriggerState = 2;
            // IncinTriggeredByRestart = false;
            // heater_process();
            IncinTriggerByInitial = false;
        }
        if((lcd_last_display_time + 10000 < nwy_uptime_get()) & NeededDefaultScreen & !InterruptForDispense ){
            lcd_init();
            lcd_clear();
            DispNewImage(image_data_Image);
            if(NetworkConnectStatus){
                DispBarImage(image_data_bar_Image);
            }
            char received_amount[3];
            sprintf(received_amount, "%-2d", Incin_cycle);
            Display(0, PAGE8, 116, received_amount);
            NeededDefaultScreen = false;
            PaymentScreenActive = false;
            uint8_t new_cmd_flag3[8] = { 0x04, 0x00, 0x04, 0xBB, 0x00, 0x00, 0x00, 0x00 };
            memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
            nwy_i2c_send_data();
            nwy_thread_sleep(100);
            
        }
        nwy_thread_sleep(100);
    }

}

void check_and_run_scheduled_task() {
    nwy_time_t current_time = {0};
    char timezone = 0;

    nwy_date_get(&current_time, &timezone);

    // Check if current time is between 18:00 to 18:04
    if (current_time.hour == hur && current_time.min >= min && current_time.min <= min + 1) {
        if (!already_triggered_today) {
            nwy_test_cli_echo("\r\n Incineration Triggered Today ******************************* \n");
            IncinTriggerBySchedule = true;
        }
    }

    // Reset the flag after midnight
    if (current_time.hour == 0 && current_time.min >= 0 && current_time.min <= 4 ) {
        already_triggered_today = false;
    }
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
    nwy_test_cli_get_heap_info();


    if(CustomCode){
        // Starts From Here
        nwy_thread_sleep(100);
        nwy_test_cli_echo("\r\n ********** Initial Configuration Process Started **********");

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

        nwy_thread_sleep(100);


        nwy_test_cli_echo("\r\n ********** Initial Configuration Process Ended **********");

        // GPIO Interrupt for I2C
        int data = nwy_gpio_irq_register(I2C_IO_Pin,1,2,customGpio,NULL);
        if (!data)
        {
            nwy_test_cli_echo("\r\nGpio isr register success! --- ");
        }
        else
        {
            nwy_test_cli_echo("\r\nGpio isr register failed! --- ");
        }

        int ret2 = nwy_gpio_irq_enable(I2C_IO_Pin);

        if(0 == ret2)
            nwy_test_cli_echo(" Gpio enable isr success!\r\n");
        else
            nwy_test_cli_echo(" Gpio enable isr fail!\r\n");

        // I2C Init
        nwy_i2c_init_process();

        // Display Init
        lcd_init();

        nwy_thread_sleep(100);
        lcd_clear();
        nwy_thread_sleep(100);
        Display(0, PAGE2, 0, " PF Version    : V1.0 ");
        // Display(0, PAGE2, 0, " PF Version    : V1.0 ");
        Display(0, PAGE3, 0, " Setup Version : V1.0 ");
        DispNewImage2(image_data_initial_logo);
        nwy_thread_sleep(2000);

        lcd_clear();
        nwy_thread_sleep(100);
        char machineIDDisplay[22];
        sprintf(machineIDDisplay, " Machine ID : %s", MAC_ID);
        char VersionIDDisplay[22];
        sprintf(VersionIDDisplay, " SW Version : %s", VersionNumber);
        Display(0, PAGE2, 0, VersionIDDisplay);
        Display(0, PAGE3, 0, machineIDDisplay);
        DispNewImage2(image_data_initial_logo);
        nwy_thread_sleep(2000);
        // nwy_i2c_test_bus2();
    
    
        lcd_clear();
        DispNewImage(image_data_Image);
        char received_amount[3];
        sprintf(received_amount, "%-2d", Incin_cycle);
        Display(0, PAGE8, 116, received_amount);
        lcd_last_display_time = nwy_uptime_get();
        
        nwy_test_cli_get_heap_info();
        nwy_thread_create(&nwy_test_cli_thread3, "test-cli3", NWY_OSI_PRIORITY_NORMAL, nwy_test_cli_main_func3, NULL, 16, 1024 * 32, NULL);
        nwy_test_cli_get_heap_info();

        nwy_thread_sleep(2000);

        uint8_t new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
        memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
        nwy_i2c_send_data();
        nwy_thread_sleep(100);

        // nwy_test_i2c();
        // nwy_test_lcd();

       

            if(isMachineConfigFound){
        nwy_test_cli_echo("\r\n ********** MAIN Loop Process Started **********");
        
        if(isIncinerationPaused){
            nwy_test_cli_echo("\n Incinerator Process Resuming Needed \n");
            // nwy_test_cli_echo("Incin Start Time String: %s, Incin Start Time Integer: %d",IncinStartTime,atoi(IncinStartTime));
            int epoch = atoi(IncinStartTime);
            current_epoch = get_epoch_from_nwy_time();
            nwy_test_cli_echo("Incin Epoch Time: %d, Current Epoch Time: %d", epoch, current_epoch);
            if((epoch + (bct * 60)) >= current_epoch){
                nwy_test_cli_echo("\n Incinerator Process Resuming \n");
                lcd_clear();
                Display(0, PAGE2, 0, "     INCINERATOR    ");
                Display(0, PAGE4, 0, "       PROCESS      ");
                Display(0, PAGE6, 0, "      RESUMING      ");

                nwy_thread_sleep(2000);
                IncinTriggerByResume = true;
                
            }else{
                nwy_test_cli_echo("\n Incinerator Process Resume Not Needed \n");


                FetchCurrentTimeF1();
                sprintf(IncinEndTime, "%s", CurrentTimeString);

                update_temperature();
                update_temperature();
                // send_incinerator_cycle_message(
                // IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                // IncinEndTime, chamberA_disp, chamberB_disp, 0, 0, IncinTriggerState, 202);

                // Incin Batch ID
                FetchCurrentTimeF2();
                snprintf(IncinBatchID, sizeof(IncinBatchID), "CM%sIN%s", MAC_ID, CurrentTimeString);

                // send_incinerator_cycle_message(
                // IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                // IncinEndTime, chamberA_disp, chamberB_disp, 0, 0, IncinTriggerState, 202);

                // To Update in FLASH Memory
                Incin_cycle = 0;
                isIncinerationPaused = 0;
                current_epoch = get_epoch_from_nwy_time();
                sprintf(IncinStartTime, "%d", current_epoch);
                save_incin_config_values();
            }

        }

        if((Incin_cycle >= bcc) && (bcc != 0)){
            IncinTriggerByInitial = true;
        }

        while(1){
            if(coin_pulse != 0 && coin_pulse <= itp && CoinStateChanged){
                CoinStateChanged = false;
                char received_amount[22];
                char total_amount[22];
                sprintf(total_amount, "        Rs. %-3d       ", itp);
                sprintf(received_amount, "        Rs. %-3d       ", coin_pulse);
                lcd_clear();
                Display(0, PAGE1, 0, "     Napkin Amount    ");
                Display(0, PAGE3, 0, total_amount);
                Display(0, PAGE5, 0, "    Received Amount   ");
                Display(0, PAGE7, 0, received_amount);
    
                // Screen Update
                lcd_last_display_time = nwy_uptime_get()  + 10000;
                lcd_insun_last_display_time = nwy_uptime_get()  + 10000;
                
                NeededDefaultScreen = true;
                NeedToCheckInsunTime = true;
                if(IsInstulationOperation){
                    InterruptForDispense = true;
                }else{
                    InterruptForDispense = false;
                }
    
                if(coin_pulse >= itp){
                    nwy_thread_sleep(1000);
                    CoinStateChanged = true;
                }
            }
            if(coin_pulse >= itp && CoinStateChanged){
                lcd_last_display_time = nwy_uptime_get();
                lcd_insun_last_display_time = nwy_uptime_get();
                coin_pulse = 0;
                CoinStateChanged = false;
                nwy_thread_sleep(500);
                nwy_test_cli_echo("*********************** Motor Turned ON ************************");
    
                uint8_t new_cmd_flag[8] = { 0x0f, 0x01, 0x10, 0xbb, 0x00, 0x00, 0x00, 0x00};
                memcpy(cmd_flag, new_cmd_flag, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);

                PaymentScreenActive = false;
                uint8_t new_cmd_flag3[8] = { 0x04, 0x00, 0x04, 0xBB, 0x00, 0x00, 0x00, 0x00 };
                memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);

                lcd_clear();
                Display(0, PAGE3, 0, " !!   DISPENSING   !!");
                Display(0, PAGE6, 0, "        NAPKIN       ");
        
                nwy_thread_sleep(1000);
                nwy_test_cli_echo("*********************** Motor Turned OFF ************************");
        
                // uint8_t new_cmd_flag2[8] = { 0x0f, 0x00, 0x0f, 0xbb, 0x00, 0x00, 0x00, 0x00};
                // memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                // nwy_i2c_send_data();
                
                nwy_thread_sleep(2000);
                
                lcd_clear();
                Display(0, PAGE3, 0, "     COLLECT YOUR     ");
                Display(0, PAGE6, 0, "        NAPKIN       ");
    
                char oid_value[50];
                FetchCurrentTimeF1();
                snprintf(oid_value, sizeof(oid_value), "CM%sCO%s", MAC_ID, CurrentTimeString);
                nwy_test_cli_echo("\nGenerated OID: %s\n", oid_value);


                uint8_t new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
                memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);
                    
                // send_dispense_order_message(oid_value, 2, itp, machineReadings.StockStatus, 200, 1, 177, 83); // 1 Represents Offline Mode
    
                nwy_thread_sleep(2000);
                
                lcd_clear();
                Display(0, PAGE2, 0, "   THANKS FOR USING  ");
                Display(0, PAGE4, 0, "      BMC HYGIENE    ");
                // Display(0, PAGE4, 0, "    PUREFEM HYGIENE  ");
                Display(0, PAGE6, 0, "       FACILITY      ");
                lcd_last_display_time = nwy_uptime_get()  - 5000;
                lcd_insun_last_display_time = nwy_uptime_get() - 5000;

                NeededDefaultScreen = true;
                NeedToCheckInsunTime = true;
                if(IsInstulationOperation){
                    InterruptForDispense = true;
                }else{
                    InterruptForDispense = false;
                }
                coin_pulse = 0;
            }
            if(UpdateStockToServer){
                UpdateStockToServer = false;
                // send_inventory_update_message(1,machineReadings.StockStatus);
            }
            if(ValueChanged){
                ValueChanged = false;
                load_business_config_values();
            }
            if(!I2C_Connected){
                nwy_test_cli_echo("***************I2C Bus is not connected, Running In Main Loop******************\n");
                nwy_i2c_init_process();
                // return;
            }
            check_and_run_scheduled_task();
            if(FotaUpdate){
                nwy_test_cli_echo("FOTA Process started \n");
            }
            nwy_thread_sleep(300);
        }  
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
    nwy_thread_create(&nwy_test_cli_thread2, "test-cli2", NWY_OSI_PRIORITY_NORMAL, nwy_test_cli_main_func2, NULL, 16, 1024 * 32, NULL);
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

