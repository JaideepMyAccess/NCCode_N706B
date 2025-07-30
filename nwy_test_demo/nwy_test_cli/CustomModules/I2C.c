// #include "nwy_test_cli_utils.h"
// #include "nwy_test_cli_adpt.h"
// #include "nwy_test_cli_func_def.h"
// #include "nwy_i2c_api.h"
#include "global.h"
#include "Bar.c"
int bus;
unsigned char i2c_DEV_ADDR = 0x32;
unsigned char read_flag[32] = {0};
unsigned char cmd_flag[8] = { 0x01, 0x01, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00};
//bool I2C_Connected = false;
// char *i2c_bus_new = "I2C1";
char *i2c_bus_new = "I2C2";
extern bool NetworkConnectStatus;
// extern MachineReadingStruct machineReadings;
// extern ChipConfigStruct chipConfig;
// extern AWSTopicsStruct awsTopic;

void nwy_test_i2c(){

    nwy_test_cli_echo("\r\nPrint from I2C.c\r\n");
    nwy_thread_sleep(100);
    nwy_test_cli_echo("\r\nPrint 2 from I2C.c\r\n");

    nwy_i2c_init_process();

    while(1){
        char *sptr = "";
        sptr = nwy_test_cli_input_gets("\r\n Click Enter \r\n");

        unsigned char new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
        memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
        nwy_i2c_send_data();
        // nwy_test_lcd();
    }
    
}


void nwy_i2c_init_process(){
    // nwy_i2c_deinit(bus);
    // nwy_thread_sleep(100);
    int bps = 0;    // 0-100Kbps,1-400Kbps or 2-3.5Mbps
    // Initialize I2C2

    bus = nwy_i2c_init(i2c_bus_new, "NWY_I2C_BPS_100K");

    if (0 > bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", i2c_bus_new);
        I2C_Connected = false;
    }
    else{
        nwy_test_cli_echo("\r\nI2c : bus:%s init success\r\n", i2c_bus_new);
        I2C_Connected = true;
    }
        
}


void nwy_i2c_send_data() {

    int64_t I2cTimeoutValue = nwy_uptime_get();
    while(I2cBusy){
        nwy_thread_sleep(10);
        if((I2cTimeoutValue + 1) < nwy_uptime_get()){
            nwy_test_cli_echo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ I2c Timeout ~~~~~~~~~~~~~~~~~~~~\n");
            break;
        }
    };
    I2cBusy = true;
    nwy_test_cli_echo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  True  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");


    int ret;

    int cmd_size = sizeof(cmd_flag);

    nwy_test_cli_echo("I2C Write Data --- Write Address : 0x%02X, Bus Name:%s, Data: 0x%02X ", ((i2c_DEV_ADDR << 1) & 0xFE), i2c_bus_new, 0xAA);
    for (int i = 0; i < cmd_size; i++) {
        nwy_test_cli_echo("0x%02X ", cmd_flag[i]);
    }
    nwy_test_cli_echo("\n");

    ret = nwy_i2c_raw_put_byte(bus, (i2c_DEV_ADDR << 1) & 0xFE, 1, 0);
    // nwy_test_cli_echo("\nWrite Address :0x%02x ", ((i2c_DEV_ADDR << 1) & 0xFE));

    ret = nwy_i2c_write(bus, i2c_DEV_ADDR, 0xAA, cmd_flag, cmd_size);


    if (ret >=0) {
        nwy_test_cli_echo("I2C Success: Bus Name:%s, Write Success \n", i2c_bus_new);
        I2C_Connected = true;
        nwy_thread_sleep(80);
        nwy_i2c_receive_data();
        nwy_thread_sleep(100);
    }else{
        nwy_test_cli_echo("I2C Error: Bus Name:%s, write failed \n", i2c_bus_new);
        I2C_Connected = false;
        I2cBusy = false;
        nwy_test_cli_echo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  I2c Write Failed  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
    }
}
 void update_temperature(void){
    chamberA_temp = 0;
    chamberB_temp = 0;
    chamberA_disp = 0;
    chamberB_disp = 0;

    uint8_t retries = 3;
    bool valid_data = false;

    while (retries-- > 0) {
        uint8_t new_cmd_flag1[8] = { 0x0b, 0x0b, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
        memcpy(cmd_flag, new_cmd_flag1, sizeof(new_cmd_flag1));
        nwy_i2c_send_data();
        // nwy_sleep(100);  // wait for conversion

        // Check if 15th bit is not set (data valid)
        bool chamberA_valid = (read_flag[2] & 0x80) == 0;
        bool chamberB_valid = (read_flag[4] & 0x80) == 0;

        if (chamberA_valid && chamberB_valid) {
            valid_data = true;
            break;
        }else{
            nwy_test_cli_echo("*********** ThermoCoupler Data is Not Valid ************\n");
        }
    }

    if (!valid_data) {
        nwy_test_cli_echo("Thermocouple read failed after 5 retries.\n");
        // return;  // Exit the function if invalid data
    }

    chamberA_temp = ((read_flag[2] << 8) | read_flag[3]) * 0.7;
    chamberB_temp = ((read_flag[4] << 8) | read_flag[5]) * 0.7;

    // Chamber A Manipulation
    if (chamberA_temp <= 200) {
        chamberA_disp = chamberA_temp;
    } else if (chamberA_temp <= 300) {
        chamberA_disp = chamberA_temp + 100;
    } else if (chamberA_temp <= 400) {
        chamberA_disp = chamberA_temp + 200;
    } else if (chamberA_temp <= 650) {
        chamberA_disp = chamberA_temp + 300;
    } else {
        chamberA_disp = chamberA_temp + 300;
    }

    // Chamber B Manipulation
    if (chamberB_temp <= 200) {
        chamberB_disp = chamberB_temp;
    } else if (chamberB_temp <= 300) {
        chamberB_disp = chamberB_temp + 100;
    } else if (chamberB_temp <= 450) {
        chamberB_disp = chamberB_temp + 200;
    } else {
        chamberB_disp = chamberB_temp + 200;
    }

    nwy_test_cli_echo("Chamber A Combined (Integer) : %d , Manipulated : %d\n", chamberA_temp, chamberA_disp);
    nwy_test_cli_echo("Chamber B Combined (Integer) : %d , Manipulated : %d\n", chamberB_temp, chamberB_disp);
}
static void heater_process(void){
    
    IsInstulationOperation = true;

    bool inciner_complete = false;
    heaterA_status = false;
    heaterB_status = false;

    update_temperature();
    update_temperature();


    // Turn OFF Coin Acceptor
    PaymentScreenActive = false;
    uint8_t new_cmd_flag3[8] = { 0x04, 0x00, 0x04, 0xBB, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
    nwy_i2c_send_data();
    nwy_thread_sleep(100);

    // Heater A ON
    uint8_t new_cmd_flag[8] = { 0x16, 0x01, 0x17, 0xBB, 0x00, 0x00, 0x00, 0x00};
    memcpy(cmd_flag, new_cmd_flag, sizeof(cmd_flag));
    nwy_i2c_send_data();
    heaterA_status = true;
    nwy_thread_sleep(100);

    lcd_init();
    lcd_clear();
    nwy_thread_sleep(10);


    InterruptForDispense = true;
    NeedToCheckInsunTime = false;
    isIncinerationPaused = 1;

    // To Update in FLASH Memory
    current_epoch = get_epoch_from_nwy_time();
    sprintf(IncinStartTime, "%d", current_epoch);
    save_incin_config_values();

    FetchCurrentTimeF1();
    sprintf(IncinStartTime, "%s", CurrentTimeString);
    // send_incinerator_cycle_message(
    //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
    //     "0", chamberA_disp, chamberB_disp, 1, 0, IncinTriggerState, 1);

  

    insun_burn_time = nwy_uptime_get() / 1000;
    bool isTimeOut = false;
    while(!inciner_complete){
        // nwy_test_cli_echo("Stage 1 \n");
        
        // nwy_test_cli_echo("Stage 2 \n");
        if((incinTime + 1500) < nwy_uptime_get()){
            nwy_test_cli_echo("\n\n############################## Loop Running ##############################\n");

            nwy_test_cli_echo("Current Time : %lld, Insun Burn Time : %lld for Timeout  \n", (nwy_uptime_get() / 1000000), (insun_burn_time + (bct * 60 )));
            if((insun_burn_time + (bct * 60 )) < (nwy_uptime_get() / 1000)){
                nwy_test_cli_echo("bct value : %d Minutes \n", bct);
                nwy_test_cli_echo("************************** Timeout For Incinerator Process **********88\n");
                isTimeOut = true;


                // For Incinerator to TURN OFF
                uint8_t new_cmd_flag2[8] = { 0x16, 0x00, 0x16, 0xBB, 0x00, 0x00, 0x00, 0x00};
                memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);
                // Heater B OFF
                uint8_t new_cmd_flag3[8] = { 0x17, 0x00, 0x17, 0xBB, 0x00, 0x00, 0x00, 0x00};
                memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                nwy_i2c_send_data();
                nwy_thread_sleep(100);

                heaterA_status = false;
                heaterB_status = false;
                inciner_complete = true;
                isIncinerationPaused = 0;
                IsInstulationOperation = false;
                break;
            }
            if(!I2C_Connected){
                nwy_test_cli_echo("***************I2C Bus is not connected******************\n");
                // return;
            }
        
            // nwy_test_cli_echo("**************************Stage 3 ****************************88\n");

            update_temperature();

            if(chamberA_temp > 1500 | chamberB_temp > 1500){
                nwy_thread_sleep(1000);
                nwy_test_cli_echo("Returning Because Temperature is too high\n");
                // continue; 
            }


            
            char status_buffer1[13]= {0};
            char status_buffer2[13]= {0};
            char status_buffer3[13]= {0};
            char status_buffer4[13]= {0};

            sprintf(status_buffer1, "    %s    ", heaterA_status ? "ON":"OFF");
            sprintf(status_buffer2, "    %s    ", heaterB_status ? "ON":"OFF");
            sprintf(status_buffer3, "   %3d C   ", chamberA_disp );
            sprintf(status_buffer4, "   %3d C   ", chamberB_disp );

            nwy_thread_sleep(10);
            if(NeedToCheckInsunTime){
                if(lcd_insun_last_display_time + 10000 < nwy_uptime_get()){
                    nwy_test_cli_echo("**************** Re Init Screen Running *******************");
                    nwy_thread_sleep(100);


                    PaymentScreenActive = false;
                    uint8_t new_cmd_flag3[8] = { 0x04, 0x00, 0x04, 0xBB, 0x00, 0x00, 0x00, 0x00 };
                    memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    lcd_init();
                    lcd_clear();
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);
                    NeedToCheckInsunTime = false;
                    InterruptForDispense = true;
                    nwy_thread_sleep(1000);
                }
            }else{
                if(InterruptForDispense ){
                    nwy_test_cli_echo("**************** Screen Update Running *******************");
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);

                }
            }


            // nwy_ext_input_gets("\r\nPlease send start: ");
            if( chamberA_disp > hbo){ //400 // 150
                if(!heaterB_status ){
                    // Heater B ON
                    nwy_thread_sleep(100);
                    // Read Temperature
                    uint8_t new_cmd_flag2[8] = { 0x17, 0x01, 0x18, 0xBB, 0x00, 0x00, 0x00, 0x00};
                    memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    heaterB_status = true;
                    nwy_thread_sleep(1000);
                    lcd_init();
                    lcd_clear();
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);
                    // send_incinerator_cycle_message(
                    //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                    //     "0", chamberA_disp, chamberB_disp, heaterA_status?1:0, heaterB_status?1:0 , IncinTriggerState, 206);
                }
                
                if(chamberA_disp > sta & heaterA_status){ // 650 //250
                    // Heater A OFF
                    uint8_t new_cmd_flag2[8] = { 0x16, 0x00, 0x16, 0xBB, 0x00, 0x00, 0x00, 0x00};
                    memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    heaterA_status = false;
                    lcd_init();
                    lcd_clear();
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);
                    // send_incinerator_cycle_message(
                    //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                    //     "0", chamberA_disp, chamberB_disp, heaterA_status?1:0, heaterB_status?1:0 , IncinTriggerState, 206);

                }
                if(chamberA_disp < ham & !heaterA_status){ // 600 //240
                    // Heater A ON
                    uint8_t new_cmd_flag2[8] = { 0x16, 0x01, 0x16, 0xBB, 0x00, 0x00, 0x00, 0x00};
                    memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    heaterA_status = true;
                    lcd_init();
                    lcd_clear();
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);
                    // send_incinerator_cycle_message(
                    //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                    //     "0", chamberA_disp, chamberB_disp, heaterA_status?1:0, heaterB_status?1:0 , IncinTriggerState, 206);
                }
                if(chamberB_disp > stb & heaterB_status){//450 // 175
                    // nwy_thread_sleep(1000);
                    // Heater A OFF

                    uint8_t new_cmd_flag2[8] = { 0x16, 0x00, 0x16, 0xBB, 0x00, 0x00, 0x00, 0x00};
                    memcpy(cmd_flag, new_cmd_flag2, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    // Heater B OFF
                    uint8_t new_cmd_flag3[8] = { 0x17, 0x00, 0x17, 0xBB, 0x00, 0x00, 0x00, 0x00};
                    memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    heaterA_status = false;
                    heaterB_status = false;
                    inciner_complete = true;
                    isIncinerationPaused = 0;
                    IsInstulationOperation = false;
                    lcd_init();
                    lcd_clear();
                    Update_Screen(status_buffer1, status_buffer2, status_buffer3, status_buffer4);
                    // send_incinerator_cycle_message(
                    //     IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
                    //     "0", chamberA_disp, chamberB_disp, heaterA_status?1:0, heaterB_status?1:0 , IncinTriggerState, 206);
                }
            }
            incinTime = nwy_uptime_get(); 
        }
        
        if(i2c_state_change == true){
            nwy_test_cli_echo("I2C State Change from Insulator Mode \n");
            i2c_state_change = false;
            nwy_thread_sleep(100);
            uint8_t new_cmd_flag[8] = { 0x18, 0x18, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00};
            memcpy(cmd_flag, new_cmd_flag, sizeof(cmd_flag));
            nwy_i2c_send_data();
            // nwy_thread_sleep(100);
        }

        nwy_thread_sleep(100);  
         
    }
    nwy_thread_sleep(500);  
    if(isTimeOut){
        nwy_test_cli_echo("****************** Timeout For Incinerator Process **********\n");
        lcd_init();
        lcd_clear();
        Display(0, PAGE2, 0, "       BURNING        ");
        Display(0, PAGE4, 0, "        CYCLE         ");
        Display(0, PAGE7, 0, "       TIMEOUT        ");
        lcd_last_display_time = nwy_uptime_get() - 5000;
        nwy_thread_sleep(1000);
        
        FetchCurrentTimeF1();
        sprintf(IncinEndTime, "%s", CurrentTimeString);
        // send_incinerator_cycle_message(
        // IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
        // IncinEndTime, chamberA_disp, chamberB_disp, 0, 0, IncinTriggerState, 201);
    }else{
        nwy_test_cli_echo("****************** Incinerator Process Completed **********\n");
        lcd_init();
        lcd_clear();
        Display(0, PAGE2, 0, "       BURNING        ");
        Display(0, PAGE4, 0, "        CYCLE         ");
        Display(0, PAGE7, 0, "      COMPLETED       ");
        lcd_last_display_time = nwy_uptime_get() - 5000;
        nwy_thread_sleep(1000);
        
        FetchCurrentTimeF1();
        sprintf(IncinEndTime, "%s", CurrentTimeString);
        // send_incinerator_cycle_message(
        // IncinBatchID, Incin_cycle, IncinTotalNapkinBurn, IncinStartTime, 
        // IncinEndTime, chamberA_disp, chamberB_disp, 0, 0, IncinTriggerState, 200);
    }
    

    // Incin Batch ID
    FetchCurrentTimeF2();
    snprintf(IncinBatchID, sizeof(IncinBatchID), "CM%sIN%s", MAC_ID, CurrentTimeString);

    Incin_cycle = 0;
    NeededDefaultScreen = true;
    InterruptForDispense = false;

    // To Update in FLASH Memory
    current_epoch = get_epoch_from_nwy_time();
    sprintf(IncinStartTime, "%d", current_epoch);
    // save_incin_config_values();
}


void Update_Screen(char *status_buffer1, char *status_buffer2, char *status_buffer3, char *status_buffer4){
    Display(0, PAGE1, 0, "    INCINERATOR ON");
    Display(0, PAGE2, 0, "  Treatment Chamber ");
    Display(0, PAGE4, 0, status_buffer1);
    Display(0, PAGE4, 64, status_buffer3);
    Display(0, PAGE6, 0, "   Burning Chamber  ");
    Display(0, PAGE8, 0, status_buffer2);
    Display(0, PAGE8, 64, status_buffer4);
}


void nwy_i2c_receive_data(){

    int ret;
    ret = nwy_i2c_raw_put_byte(bus, (i2c_DEV_ADDR << 1) | 0x01, 1, 0);
    I2cBusy = false;
    if (ret >= 0) {

        nwy_test_cli_echo("\r\nI2C Received Data --- Read Address: 0x%02x, Bus Name:%s, Data: ", ((i2c_DEV_ADDR << 1) & 0xFE), i2c_bus_new);

        int len = 9;
        for(int i=0; i<len; i++)
        {
            if(i == (len - 1)){
                ret = nwy_i2c_raw_get_byte(bus, &read_flag[i], 0, 1); // send cmd and data
                I2C_Connected = true;
            }
            else{
                ret = nwy_i2c_raw_get_byte(bus, &read_flag[i], 0, 0); // send cmd and data
                I2C_Connected = true;
            }
                
            if(ret >=0)
            {
                nwy_test_cli_echo("0x%02x ", read_flag[i]);
                // I2C_Connected = false; 
                // goto error;
            }else{
                nwy_test_cli_echo("\r\n I2c Error: Bus Name:%s Read Failed", i2c_bus_new);
                I2C_Connected = false; 
            }
    
        }  
        if(I2C_Connected){
            nwy_test_cli_echo("\r\nI2C Success: Bus Name:%s, Read Success \n", i2c_bus_new);
        }

            if(read_flag[0] == 0xAA ){
            switch (read_flag[1])
            {
            case 0x01:
                /* code */
                nwy_test_cli_echo("Received Pong Response \r\n");
                break;
            case 0x05:
                // /* code */
                nwy_test_cli_echo("Received Stock Level Response\r\n");
                if(read_flag[2] == 0xc8 & read_flag[3] == 0xc9){
                    nwy_test_cli_echo("Stock Available \r\n");
                    machineReadings.StockStatus = 2;
                }else if(read_flag[2] == 0xc9 & read_flag[3] == 0xc9){
                    nwy_test_cli_echo("Low Available \r\n");
                    machineReadings.StockStatus = 1;
                }else if(read_flag[2] == 0xc9 & read_flag[3] == 0xc8){
                    nwy_test_cli_echo("No Stock Available \r\n");
                    machineReadings.StockStatus = 0;
                }else{
                    nwy_test_cli_echo("Invalid Response \r\n");
                    machineReadings.StockStatus = 2;
                }
                if(machineReadings.StockStatus != machineReadings.PStockStatus){
                    machineReadings.PStockStatus = machineReadings.StockStatus;
                    UpdateStockToServer = true;
                }
                break;
            

            case 0x30:

                // char TempLineData[22];
                // lcd_init();
                // lcd_clear();
                // Display(0, PAGE1, 0, "    CONFIGURATION    ");
                // sprintf(TempLineData, "IMEI: %s", ImeiNumber);
                // Display(0, PAGE2, 0, TempLineData);
                // sprintf(TempLineData, "PRICE: %2d  ID: %d", itp, iid);
                // Display(0, PAGE3, 0, TempLineData);

                    // A total of 20 seconds for user to insert coin or scan QR code
                    if(IsInstulationOperation){
                    lcd_insun_last_display_time = nwy_uptime_get();
                    nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                    NeedToCheckInsunTime = true;
                    NeededInsuinScreen = true;
                    InterruptForDispense = true;
                }else{
                    lcd_last_display_time = nwy_uptime_get();
                    NeededDefaultScreen = true; 
                    InterruptForDispense = false;
                    NeededInsuinScreen = false;
                }

                break;

            case 0x13:
                /* code */
                nwy_test_cli_echo("Received Switch Response \r\n");
                if(read_flag[2] ){
                    // lcd_init();
                    // To Fetch No Stock Status
                    // uint8_t new_cmd_flag3[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
                    // memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                    // nwy_i2c_send_data();
                    // nwy_thread_sleep(100);

                    uint8_t new_cmd_flag4[8] = { 0x05, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00 };
                    memcpy(cmd_flag, new_cmd_flag4, sizeof(cmd_flag));
                    nwy_i2c_send_data();
                    nwy_thread_sleep(100);
                    
                    if(machineReadings.StockStatus == 0){
                        lcd_init();
                        lcd_clear();
                        Display(0, PAGE2, 0, "        SORRY        ");
                        Display(0, PAGE4, 0, "      NO  STOCK      ");
                        Display(0, PAGE7, 0, "      AVAILABLE      ");

                         // A total of 20 seconds for user to insert coin or scan QR code
                         if(IsInstulationOperation){
                            lcd_insun_last_display_time = nwy_uptime_get();
                            nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                            NeedToCheckInsunTime = true;
                            NeededInsuinScreen = true;
                            InterruptForDispense = true;
                        }else{
                            lcd_last_display_time = nwy_uptime_get();
                            NeededDefaultScreen = true; 
                            InterruptForDispense = false;
                            NeededInsuinScreen = false;
                        }
                    }else{
                        if(!PaymentScreenActive){
                            PaymentScreenActive = true;
                            uint8_t new_cmd_flag3[8] = { 0x04, 0x01, 0x05, 0xBB, 0x00, 0x00, 0x00, 0x00 };
                            memcpy(cmd_flag, new_cmd_flag3, sizeof(cmd_flag));
                            nwy_i2c_send_data();
                            nwy_thread_sleep(100);
                            coin_pulse = 0;
                        }else{
                            coin_pulse = 0;
                        }

                        char status_buffer1[13]= {0};
                        char status_buffer2[25]= {0};

                        sprintf(status_buffer1, "  Rs. %2d   ", itp );
                        sprintf(status_buffer2, "        Rs. %2d        ", itp );
                        if(NetworkConnectStatus){
                            lcd_init();
                            lcd_clear();
                            DispQRImage(qrb_array);
                            DispBarImage(image_data_bar_Image);
                            Display(0, PAGE2, 62, "Napkin Cost");
                            Display(0, PAGE3, 62, status_buffer1);
                            Display(0, PAGE5, 62, "Insert Coin");
                            Display(0, PAGE6, 62, "    (or)   ");
                            Display(0, PAGE7, 62, "  Scan QR  ");
                            if(machineReadings.StockStatus == 1){
                            Display(0, PAGE8, 62, " LOW STOCK ");
                            }
                        }else{
                            lcd_init();
                            lcd_clear();
                            Display(0, PAGE2, 0, "      Napkin Cost     ");
                            Display(0, PAGE4, 0, status_buffer2);
                            Display(0, PAGE7, 0, "      Insert Coin     ");
                            if(machineReadings.StockStatus == 1){
                            Display(0, PAGE8, 0, "       LOW STOCK      ");
                            }
                        }   
                        
                        // A total of 20 seconds for user to insert coin or scan QR code
                        if(IsInstulationOperation){
                            lcd_insun_last_display_time = nwy_uptime_get() + 10000;
                            nwy_test_cli_echo("**************** Insuin Screen State True *******************");
                            NeedToCheckInsunTime = true;
                            NeededInsuinScreen = true;
                            InterruptForDispense = true;
                        }else{
                            lcd_last_display_time = nwy_uptime_get() + 10000;
                            NeededDefaultScreen = true; 
                            InterruptForDispense = false;
                            NeededInsuinScreen = false;
                        }
                    }
                }
               
            break;
            case 0x15:
                /* code */
                nwy_test_cli_echo("Received Heater Limit Switch Response \r\n");
                if(read_flag[2]){
                    Incin_cycle++;
                    IncinTotalNapkinBurn++;
                    if(!IsInstulationOperation){
                        char received_amount[3];
                        sprintf(received_amount, "%-2d", Incin_cycle);
                        Display(0, PAGE8, 116, received_amount);
                    }
                    nwy_test_cli_echo("Incinerator Cycle : %d \r\n", Incin_cycle);
                    // send_incinerator_info_status(
                    // IncinBatchID, Incin_cycle, IncinTotalNapkinBurn);
                    lcd_insun_last_display_time = nwy_uptime_get();
                    // save_incin_config_values();
                    if((Incin_cycle >= bcc) && !IsInstulationOperation){
                        IncinTriggerState = 2;
                        // IncinTriggeredByRestart = false;
                        heater_process();
                    }
                }
            break;
            case 0x14:
                /* code */
                nwy_test_cli_echo("Received Incinerator Response \r\n");
                if(read_flag[2] == 0x01){
                    lcd_insun_last_display_time = nwy_uptime_get();
                    // lcd_insun_last_display_time = nwy_uptime_get();
                    IncinTriggerState = 1;
                    // IncinTriggeredByRestart = false;
                    heater_process();
                }
            break;
            
            
            // Commited because coin response is from Neoway N58
            case 0x0e:
            /* code */
                nwy_test_cli_echo("!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received Coin Inserted Response !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
                if(PaymentScreenActive){
                    nwy_test_cli_echo("Payment Active True --------------------- \n");
                    // Temp 
                    nwy_test_cli_echo("##########################################################Received Coin Pulse Value : %d \n", read_flag[2]);
                    if(read_flag[2] < 21){
                        coin_pulse = coin_pulse + read_flag[2];
                        CoinStateChanged = true;
                    }
                }else{
                    nwy_test_cli_echo("Payment Active False --------------------- \n");
                }
               
            break;
            
            case 0x04:
                // nwy_test_cli_echo("Dispensing Complete Response \r\n");
                // lcd_clear();
                // Display(0, PAGE3, 0, "     COLLECT YOUR     ");
                // Display(0, PAGE6, 0, "        NAPKIN       ");
                // nwy_thread_sleep(2000);
                // lcd_clear();
                // Display(0, PAGE2, 0, "   THANKS FOR USING  ");
                // Display(0, PAGE4, 0, "    PUREFEM HYGIENE  ");
                // Display(0, PAGE6, 0, "       FACILITY      ");
            break;
            
            default:
                nwy_test_cli_echo(" Received Unknown Response \r\n");
                break;
            }
        }

    }else{
  
        nwy_test_cli_echo("\r\n I2C Error: Bus Name:%s Read Address Failed", i2c_bus_new);
    
    }
}

