// #include "nwy_test_cli_utils.h"
// #include "nwy_test_cli_adpt.h"
// #include "nwy_test_cli_func_def.h"
// #include "nwy_i2c_api.h"
int bus;
unsigned char i2c_DEV_ADDR = 0x32;
unsigned char read_flag[32] = {0};
unsigned char cmd_flag[8] = { 0x01, 0x01, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00};
//bool I2C_Connected = false;
// char *i2c_bus_new = "I2C1";
char *i2c_bus_new = "I2C2";

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
    // nwy_sleep(100);
    int bps = 0;    // 0-100Kbps,1-400Kbps or 2-3.5Mbps
    // Initialize I2C2

    bus = nwy_i2c_init(i2c_bus_new, "NWY_I2C_BPS_100K");

    if (0 > bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", i2c_bus_new);
    }
    else
        nwy_test_cli_echo("\r\nI2c : bus:%s init success\r\n", i2c_bus_new);
}


void nwy_i2c_send_data() {

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
        
        // I2C_Connected = true;

        nwy_thread_sleep(80);

        nwy_i2c_receive_data();

        nwy_thread_sleep(100);
    }else{
        nwy_test_cli_echo("I2C Error: Bus Name:%s, write failed \n", i2c_bus_new);
        // I2C_Connected = false;
        nwy_test_cli_echo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  I2c Write Failed  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
    }
}


void nwy_i2c_receive_data(){

    int ret;
    ret = nwy_i2c_raw_put_byte(bus, (i2c_DEV_ADDR << 1) | 0x01, 1, 0);
    // I2cBusy = false;
    if (ret >= 0) {

        nwy_test_cli_echo("\r\nI2C Received Data --- Read Address: 0x%02x, Bus Name:%s, Data: ", ((i2c_DEV_ADDR << 1) & 0xFE), i2c_bus_new);

        int len = 9;
        for(int i=0; i<len; i++)
        {
            if(i == (len - 1)){
                ret = nwy_i2c_raw_get_byte(bus, &read_flag[i], 0, 1); // send cmd and data
                // I2C_Connected = true;
            }
            else{
                ret = nwy_i2c_raw_get_byte(bus, &read_flag[i], 0, 0); // send cmd and data
                // I2C_Connected = true;
            }
                
            if(ret >=0)
            {
                nwy_test_cli_echo("0x%02x ", read_flag[i]);
                // I2C_Connected = false; 
                // goto error;
            }else{
                nwy_test_cli_echo("\r\n I2c Error: Bus Name:%s Read Failed", i2c_bus_new);
            }
    
        }  

        nwy_test_cli_echo("\r\nI2C Success: Bus Name:%s, Read Success \n", i2c_bus_new);

    }else{
  
        nwy_test_cli_echo("\r\n I2C Error: Bus Name:%s Read Address Failed", i2c_bus_new);
    
    }
}

