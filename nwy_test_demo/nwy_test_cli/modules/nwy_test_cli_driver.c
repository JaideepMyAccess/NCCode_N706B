#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

#ifndef OSI_ALIGNED
#define OSI_ALIGNED(s) __attribute__((aligned(s)))
#endif

#if 0
#if NWY_TEST_CLI_NOT_SUPPORT

/**************************UART*********************************/
int hd = -1;
static uint8_t uart_mode = NWY_UART_MODE_AT;
extern unsigned int nwy_test_uart_name_tab[NWY_OPEN_MAX_UART_CHANNEL];
void nwy_test_cli_uart_init()
{
    char *opt;
    uint8_t port;
    uint32_t name;
#ifdef NWY_EC618_UART_OPEN_TEST
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(1-URT0, 2-URT1 or 3-URT2):");
#else
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(1-URT1, 2-URT2 or 3-URT3):");
#endif
    port = atoi(opt);
    if(port < 1 || port > NWY_OPEN_MAX_UART_CHANNEL)
    {
        nwy_test_cli_echo("\r\n Input UART id is invalid!");
        return;
    }
    name = nwy_test_uart_name_tab[port-1];
    if (name == NWY_OPEN_UART_CHANNEL_NS)
    {
        nwy_test_cli_echo("\r\n Input UART not supported!");
        return;
    }
    nwy_test_cli_echo("\r\nTest port = %d!\r\n", port); //delet

    opt = nwy_test_cli_input_gets("\r\nPlease input the uart mode(0-AT,1-DATA):");
    uart_mode = atoi(opt);
    nwy_test_cli_echo("\r\nTest mode = %d!\r\n", uart_mode); //delet

    hd = nwy_uart_init(name, (nwy_uart_mode_t)uart_mode);
    if (hd < 0)
        nwy_test_cli_echo("\r\nTest uart error!\r\n"); //delet
    nwy_test_cli_echo("\r\nTest uart success!\r\n");   //delet
}

void nwy_test_cli_uart_set_baud()
{
    char *opt;
    uint32_t baud = 0;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nPlease input the uart baud:");
    baud = atoi(opt);
    nwy_test_cli_echo("\r\nTest baud = %d,hd = %d!\r\n", baud, hd); //delet

    if (1200 <= baud && 8000000 >= baud)
    {
        nwy_uart_set_baud(hd, baud);
        nwy_test_cli_echo("\r\nSet uart baud success!\r\n");
    }
    else
        nwy_test_cli_echo("\r\nSet uart baud param error!\r\n");
}

void nwy_test_cli_uart_get_baud()
{
    uint32_t baud = 0;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_get_baud(hd, &baud);
    nwy_test_cli_echo("\r\nRead uart id = %d, baud = %d!\r\n", hd + 1, baud);
}

void nwy_test_cli_uart_set_para()
{
    nwy_uart_parity_t parity;
    nwy_uart_data_bits_t data_size;
    nwy_uart_stop_bits_t stop_size;
    int flow_ctrl = 0;
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart param:");

    switch (*opt++)
    {
    case 'p':
        nwy_uart_get_para(hd, &parity, &data_size, &stop_size, &flow_ctrl);
        parity = (nwy_uart_parity_t)atoi(opt);
        if (0 <= parity && 2 >= parity)
        {
            nwy_uart_set_para(hd, parity, data_size, stop_size, flow_ctrl);
            nwy_test_cli_echo("\r\nSwitch parity to:%d\r\n", parity);
        }
        else
            nwy_test_cli_echo("\r\nTest invalid parity:%d!\r\n", parity);
        break;

    case 'd':
        nwy_uart_get_para(hd, &parity, &data_size, &stop_size, &flow_ctrl);
        data_size = (nwy_uart_data_bits_t)atoi(opt);
        if (7 <= data_size && 8 >= data_size)
        {
            nwy_uart_set_para(hd, parity, data_size, stop_size, flow_ctrl);
            nwy_test_cli_echo("\r\nSwitch data_size to:%d\r\n", data_size);
        }
        else
            nwy_test_cli_echo("\r\nInvalid data_size:%d\r\n", data_size);
        break;

    case 's':
        nwy_uart_get_para(hd, &parity, &data_size, &stop_size, &flow_ctrl);
        stop_size = (nwy_uart_stop_bits_t)atoi(opt);
        if (1 <= stop_size && 2 >= stop_size)
        {
            nwy_uart_set_para(hd, parity, data_size, stop_size, flow_ctrl);
            nwy_test_cli_echo("\r\nSwitch stop_size to:%d\r\n", stop_size);
        }
        else
            nwy_test_cli_echo("\r\nInvalid stop_size:%d\r\n", stop_size);
        break;

    case 'f':
        nwy_uart_get_para(hd, &parity, &data_size, &stop_size, &flow_ctrl);
        flow_ctrl = atoi(opt);
        if (0 <= parity && 1 >= parity)
        {
            nwy_uart_set_para(hd, parity, data_size, stop_size, flow_ctrl);
            nwy_test_cli_echo("\r\nSwitch flow_ctrl to:%d\r\n", flow_ctrl);
        }
        else
            nwy_test_cli_echo("\r\nInvalid flow_ctrl:%d\r\n", flow_ctrl);
        break;

    default:
        break;
    }
}

void nwy_test_cli_uart_get_para()
{
    nwy_uart_parity_t parity;
    nwy_uart_data_bits_t data_size;
    nwy_uart_stop_bits_t stop_size;
    int flow_ctrl = 0;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_get_para(hd, &parity, &data_size, &stop_size, &flow_ctrl);
    nwy_test_cli_echo("\r\nUart parity = %d\r\n", parity);
    nwy_test_cli_echo("\r\nUart data_size = %d\r\n", data_size);
    nwy_test_cli_echo("\r\nUart stop_size = %d\r\n", stop_size);
    nwy_test_cli_echo("\r\nUart flow_ctrl = %d\r\n", flow_ctrl);
}

void nwy_test_cli_uart_set_tout()
{
    int timeout;
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart receive timeout(default:32ms):");
    timeout = atoi(opt);

    nwy_set_rx_frame_timeout(hd, timeout);
    nwy_test_cli_echo("\r\nSet rx frame timeout = %d\r\n", timeout);
}

void nwy_test_cli_uart_send()
{
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart send data:");

    nwy_uart_send_data(hd, (uint8 *)opt, strlen(opt));
}

static void nwy_uart_recv_handle(const char *str, unsigned int length)
{
    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_send_data(hd, (uint8 *)str, length);
    nwy_test_cli_echo("\r\nUart send data length = %d\r\n", length);
}
#ifdef NWY_EC618_UART_OPEN_TEST
static void nwy_uart_recv0(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, 10);
}
static void nwy_uart_recv1(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, strlen(str));

    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_uartopen_21, P_VALUE,"strlen(str)=%d",strlen(str));
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_uartopen_28, P_VALUE,"strlen(str)=%s",str);
}
#endif

void nwy_test_cli_uart_reg_rx_cb()
{
    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }
#ifdef NWY_EC618_UART_OPEN_TEST
        if(hd == 0)
            nwy_uart_reg_recv_cb(hd, nwy_uart_recv0);
        if(hd == 1)
            nwy_uart_reg_recv_cb(hd, nwy_uart_recv1);
#else
        nwy_uart_reg_recv_cb(hd, nwy_uart_recv_handle);
#endif

}

/*if send completly, the callback func will set RS485 as rx state*/
static void nwy_rs485_direction_switch(int port, int value)
{
    nwy_gpio_set_direction(port, nwy_output);
    nwy_gpio_set_value(port, (nwy_value_t)value);
}
static void nwy_uart_send_complet_handle(int param)
{
    nwy_sleep(10);
    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_RX);
    nwy_test_cli_echo("\r\nUart send complet handle success!\r\n");
}

void nwy_test_cli_uart_reg_tx_cb()
{
    char *pstsnd = "hellors485";

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_RX);

    /*register cb func to uart drv */
    nwy_uart_reg_tx_cb(hd, nwy_uart_send_complet_handle);

    /* for send, set RS485 as tx state */
    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_TX);
    nwy_uart_send_data(hd, (uint8_t *)pstsnd, strlen(pstsnd));
}

void nwy_test_cli_uart_deinit()
{
    int close = 0;
    char *opt = NULL;

    if (hd < 0){
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nSure to close this uart(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
    {
        nwy_uart_deinit(hd);
        hd = -1;
    }
}

/**************************I2C*********************************/
#ifdef NWY_OPEN_TEST_I2C
int i2c_bus;
void nwy_test_cli_i2c_init()
{
#ifdef NWY_EC618_I2C_OPEN_TEST
    char *opt;
    uint8_t port;
    char *name;
    opt = nwy_test_cli_input_gets("\r\nPlease input the I2C id(0-I2C0,1-I2C1 ):");
    port = atoi(opt);
    if (port == 0)
    {
        name = NAME_I2C_BUS_0;
    }
    else if (port == 1)
    {
        name = NAME_I2C_BUS_1;
    }
    else
    {
        nwy_test_cli_echo("\r\n Input I2C id is invalid!");
        return;
    }

    i2c_bus = nwy_i2c_init(name, NWY_I2C_BPS_400K);
    if (NWY_SUCESS > i2c_bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", name);
        return;
    }
#else
    char *opt;
    uint8_t port;
    char *name;
    opt = nwy_test_cli_input_gets("\r\nPlease input the I2C id(1-I2C1,2-I2C2 or 3-I2C3):");
    port = atoi(opt);
    if (port == 1)
    {
        name = NAME_I2C_BUS_1;
    }
    else if (port == 2)
    {
        name = NAME_I2C_BUS_2;
    }
    else if (port == 3)
    {
        name = NAME_I2C_BUS_3;
    }
    else
    {
        nwy_test_cli_echo("\r\n Input I2C id is invalid!");
        return;
    }

    i2c_bus = nwy_i2c_init(name, NWY_I2C_BPS_100K);
    if (NWY_SUCESS > i2c_bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", name);
        return;
    }
#endif
}

#define ES8311_DEV_ADDR 0x18
#define BMA400_DEV_ADDR 0x14
void nwy_test_cli_i2c_read()
{
    uint8_t rtn, sensor_id;
#ifdef NWY_EC618_I2C_OPEN_TEST
    rtn = nwy_i2c_read(i2c_bus, ES8311_DEV_ADDR, 0xfd, &sensor_id, 2);
#else
    rtn = nwy_i2c_read(i2c_bus, BMA400_DEV_ADDR, 0x00, &sensor_id, 1);
#endif
    if (NWY_SUCESS == rtn)
        nwy_test_cli_echo("\r\nNWY get sensor id = 0x%x!\r\n", sensor_id);
    else
        nwy_test_cli_echo("\r\nNWY read I2C error!\r\n");

}


void nwy_test_cli_i2c_write()
{
    uint8_t temp = 0x1c;
    uint8_t *data = &temp;
    uint8_t rtn;
    rtn = nwy_i2c_write(i2c_bus, BMA400_DEV_ADDR, 0xf4, data, 1);
    if (NWY_SUCESS == rtn)
        nwy_test_cli_echo("\r\nNWY write I2C success!\r\n");
    else
        nwy_test_cli_echo("\r\nNWY write I2C error!\r\n");
}

void nwy_test_cli_i2c_put_raw()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_i2c_get_raw()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_i2c_deinit()
{
    int close;
    char *opt;
    opt = nwy_test_cli_input_gets("\r\nSure to close this i2c(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
        nwy_i2c_deinit(i2c_bus);
}
#endif

/**************************SPI*********************************/
#ifdef NWY_OPEN_TEST_SPI
int spi_bus = -1;
extern char* nwy_test_spi_name_tab[NWY_OPEN_MAX_SPI_CHANNEL];
void nwy_test_cli_spi_init()
{
    char *opt;
    uint8_t port;
    char *name;
    opt = nwy_test_cli_input_gets("\r\nPlease input the SPI id(1-spi1,2-spi2):");
    port = atoi(opt);
    if(port < 1 || port > NWY_OPEN_MAX_SPI_CHANNEL)
    {
        nwy_test_cli_echo("\r\n Input SPI id is invalid!");
        return;
    }
    name = nwy_test_spi_name_tab[port-1];
    if (name == NULL)
    {
        nwy_test_cli_echo("\r\n Input SPI not supported!");
        return;
    }

    spi_bus = nwy_spi_init(name, SPI_MODE_0, 1000000, 8);
    if (NWY_SUCESS > spi_bus)
    {
        nwy_test_cli_echo("\r\nSPI Error : bus:%s init fail\r\n", name);
        return;
    }
}

#define SPI_FLASH_MOUNT_POINT "/extn"
void nwy_test_cli_spi_flash_mount(void)
{
    #ifdef NWY_OPEN_TEST_GENERAL_FLASH_MOUNT
    nwy_test_cli_echo("\r\n flash test nwy_spi_flash_mount_test start");
    int opt;
    int ret;
    nwy_block_device_t * block_dev = NULL;
    int mode = nwy_test_cli_input_gets("\r\input option:0-flash capacity <= 16M bytes,1-flash capacity > 16M bytes:");
    opt = atoi(mode);
    if(opt == 1)
        block_dev = nwy_vfs_logical_block_device_create(NAME_SPI_BUS_2, 0, 8192 * 4096);
    if(block_dev == NULL)
    {
        nwy_test_cli_echo("\r\n flash test block_dev create fail");
        return;
    }
    while(1)
    {
        int mount = nwy_test_cli_input_gets("\r\input option:0-fs mount,1-fs format,2-fs write test,3-fs read test,4-fs free size,5-exit:");
        opt = atoi(mount);
        if(opt == 0)
        {
            nwy_test_cli_echo("\r\n flash test nwy_vfs_mount start");
            ret = nwy_vfs_mount(SPI_FLASH_MOUNT_POINT, block_dev);
            if(0 > ret)
            {
                nwy_test_cli_echo("\r\n flash test mount fail");
                ret = nwy_vfs_mkfs(block_dev);
                if(0 > ret)
                {
                    nwy_test_cli_echo("\r\n flash test format fail");
                    continue;
                }
                ret = nwy_vfs_mount(SPI_FLASH_MOUNT_POINT, block_dev);
                if(0 > ret)
                {
                    nwy_test_cli_echo("\r\n flash test remount fail");
                    continue;
                }
            }
            nwy_test_cli_echo("\r\n flash test nwy_vfs_mount success");
        }
        else if(opt == 1)
        {
            ret = nwy_vfs_mkfs(block_dev);
            nwy_test_cli_echo("\r\n flash test format :%d", ret);
            continue;
        }
        else if(opt == 2)
        {
            int i = 1;
            int len, size;
            char file_name[64] = {0};
            while(i)
            {
                memset(file_name, 0, sizeof(file_name));
                sprintf(file_name, "%s/%d", SPI_FLASH_MOUNT_POINT, i);
                int fd = nwy_file_open(file_name, NWY_CREAT | NWY_RDWR | NWY_TRUNC);
                if(fd < 0)
                {
                    nwy_test_cli_echo("\r\nfile open %s fail\r\n", file_name);
                    break;
                }
                else
                {
                    len = size = 0;
                    while(len < (4096 * i))
                    {
                        size = nwy_file_write(fd, file_name, strlen(file_name));
                        if(size <= 0)
                        {
                            nwy_test_cli_echo("\r\nfile write %s fail\r\n", file_name);
                            nwy_file_close(fd);
                            break;
                        }
                        len += size;
                    }
                    nwy_file_close(fd);
                    if(len > 0)
                        nwy_test_cli_echo("\r\nfile write %s size %d success\r\n", file_name, len);
                    else
                        break;
                }
                i++;
                nwy_sleep(100);
            }
        }
        else if(opt == 3)
        {
            int i = 1;
            int len, size;
            char file_name[64] = {0};
            char read_data[64];
            while(i)
            {
                memset(file_name, 0, sizeof(file_name));
                sprintf(file_name, "%s/%d", SPI_FLASH_MOUNT_POINT, i);
                int fd = nwy_file_open(file_name, NWY_RDONLY);
                if(fd < 0)
                {
                    nwy_test_cli_echo("\r\nfile open %s fail\r\n", file_name);
                    break;
                }
                else
                {
                    len = size = 0;
                    while(len < (4096 * i))
                    {
                        memset(read_data, 0, sizeof(read_data));
                        size = nwy_file_read(fd, read_data, strlen(file_name));
                        if(size <= 0)
                        {
                            nwy_test_cli_echo("\r\nfile read %s fail\r\n", file_name);
                            nwy_file_close(fd);
                            break;
                        }
                        if(memcmp(read_data, file_name, size))
                        {
                            nwy_test_cli_echo("\r\nfile read %s data compare fail, read %s dest %s\r\n", file_name, read_data, file_name);
                            nwy_file_close(fd);
                            break;
                        }
                        len += size;
                    }
                    nwy_file_close(fd);
                    if(len > 0)
                        nwy_test_cli_echo("\r\nfile read %s size %d success\r\n", file_name, len);
                    else
                        break;
                }
                i++;
                nwy_sleep(100);
            }
        }
        else if(opt == 4)
        {
            int free_size = nwy_sdk_vfs_free_size(SPI_FLASH_MOUNT_POINT);
            nwy_test_cli_echo("\r\%s free size:%d\r\n", SPI_FLASH_MOUNT_POINT, free_size);
        }
        else if(opt == 5)
            return;
    }
    nwy_test_cli_echo("\r\n flash test end");

#else
    nwy_test_cli_echo("\r\n flash test not support");
#endif
}

void nwy_test_cli_spi_trans()
{
#ifdef NWY_OPEN_TEST_SPI_CFG
    uint8_t OSI_ALIGNED(16) cmd[3] = {0x9F, 0, 0};

    nwy_spi_cs_cfg(spi_bus, SPI_CS_0, true);
    nwy_spi_write(spi_bus, cmd, 1);
    cmd[0] = 0x0;
    nwy_spi_read(spi_bus, cmd, 3);
    nwy_spi_cs_cfg(spi_bus, SPI_CS_0, false);
    nwy_test_cli_echo("\r\nSpi flash read id %02x,%02x,%02x\r\n", cmd[0], cmd[1], cmd[2]);
#else
    uint8_t OSI_ALIGNED(16) Command[4] = {w25x_jedecDeviceID, 0xFF, 0xFF, 0xFF};
    uint8_t OSI_ALIGNED(16) FlashId[4] = {0};
    int rtn = nwy_spi_transfer(spi_bus, SPI_CS_0, Command, FlashId, 4);

    if (SPI_EC_SUCESS == rtn)
        nwy_test_cli_echo("\r\nSpi flash read id %02x,%02x,%02x\r\n", FlashId[1], FlashId[2], FlashId[3]);
    else
        nwy_test_cli_echo("\r\nSpi error:transfer fail!\r\n");
#endif
}

void nwy_test_cli_spi_deinit()
{
    int close;
    char *opt;
    opt = nwy_test_cli_input_gets("\r\nSure to close this spi(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
        nwy_spi_deinit(spi_bus);
    spi_bus = -1;
}
#endif

/**************************GPIO*********************************/
void nwy_test_cli_gpio_set_val()
{
    char *opt;
    uint8_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the gpio value(0-low level,1-high level):");
    vol = atoi(opt);

    nwy_gpio_set_value(port, (nwy_value_t)vol);
}

void nwy_test_cli_gpio_get_val()
{
    char *opt;
    uint32_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    vol = nwy_gpio_get_value(port);
    nwy_test_cli_echo("\r\nGet the GPIO value = %d\r\n", vol);
}

void nwy_test_cli_gpio_set_dirt()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO dir(0-input,1-output):");
    dir = atoi(opt);

    nwy_gpio_set_direction(port, (nwy_dir_mode_t)dir);
}

void nwy_test_cli_gpio_get_dirt()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    dir = nwy_gpio_get_direction(port);
    nwy_test_cli_echo("\r\nGet the GPIO dir = %d\r\n", dir);
}
#ifdef NWY_EC618_GPIO_OPEN_TEST
static void nwy_irq_open_cb24()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_10, P_VALUE,"nwy_irq_open_24");
}
static void nwy_irq_open_cb8()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_11, P_VALUE,"nwy_irq_open_8");
}
static void nwy_irq_open_cb9()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_12, P_VALUE,"nwy_irq_open_9");
}
#endif
void nwy_test_cli_gpio_config_irq()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    char *opt;
    uint8_t port, mode, data;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO irq mode(0-disable,1-low,2-high,3-falling,4-rising):");
    mode = atoi(opt);

    if (port == 9)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb9);
    else if (port == 8)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb8);
    else if (port == 24)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb24);
    else
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, _gpioisropen);
#else
    char *opt;
    uint8_t port, mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO irq mode(0-rising,2-rising&falling,3-high):");
    mode = atoi(opt);

    nwy_close_gpio(port);
    int data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, _gpioisropen);
#endif
    if (data)
    {
        nwy_test_cli_echo("\r\nGpio isr config success!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nGpio isr config failed!\r\n");
    }
}

void nwy_test_cli_gpio_enable_irq()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    nwy_gpio_open_irq_enable(port);

    nwy_test_cli_echo("\r\nGpio enable isr success!\r\n");
}

void nwy_test_cli_gpio_disable_irq()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    nwy_gpio_open_irq_disable(port);

    nwy_test_cli_echo("\r\nGpio enable isr success!\r\n");
}

void nwy_test_cli_gpio_pull()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(0-pull up,1-pull down):");
    mode = atoi(opt);
    nwy_gpio_pullup_or_pulldown(port, mode);
#else
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif
}

void nwy_test_cli_gpio_close()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    nwy_close_gpio(port);
#endif
}

/**************************ADC*********************************/
#ifdef NWY_OPEN_TEST_ADC
void nwy_test_cli_adc_read()
{
    char *opt;
    uint8_t port, mode;
    uint32_t adc_vol;
    opt = nwy_test_cli_input_gets("\r\nChoose the ADC channel(1-CHANNEL1,2-CHANNEL2,3-CHANNEL3,4-VBAT):");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nChoose the ADC scale(0-1V250,1-2V444,2-3V233,3-5V000):");
    mode = atoi(opt);

    adc_vol = nwy_adc_get((nwy_adc_t)port, (nwy_adc_aux_scale_t)mode);
    nwy_test_cli_echo("\r\nAdc get value = %d\r\n", adc_vol);
}
#endif
#ifdef NWY_OPEN_TEST_CAMERA
unsigned short *buff = NULL;
void nwy_test_cli_camera_open()
{
    int ret ;
    nwy_camera_info_t cam_info =
        {
            .img_width = 320,
            .img_height = 240,
            .img_pixel = NWY_CAM_NPIX_QVGA,
        };
    ret = nwy_camera_open(cam_info);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\n camera open failed!\r\n");
    }
    else
        nwy_test_cli_echo("\r\n camera open success!\r\n");
}
void nwy_test_cli_camera_close()
{
    nwy_camera_close();
    nwy_test_cli_echo("\r\n camera close success!\r\n");
}
void nwy_test_cli_camera_get_preview()
{
    nwy_camera_get_preview(&buff);
    if (buff == NULL)
    {
        nwy_test_cli_echo("\r\n buff is NULL\r\n");
    }
    nwy_test_cli_echo("\r\n camera preview success!\r\n");
}
void nwy_test_cli_camera_capture()
{
    int ret = -1;
    int save_mode = 0;
    int image_format = 0;
    char image_path[128] = {0};
    char *sptr;
    save_mode = FS_MODE;
    image_format = YUV_FORMAT;
    strcpy(image_path, "cam.yuv");
    ret = nwy_camera_capture_image(image_format, save_mode, image_path);
    if(0 != ret)
    {
        nwy_test_cli_echo("\r\n test camera capture yuv failed\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n test camera capture yuv success\r\n");
    }
}
#endif

/**************************KEYPAD*********************************/
#ifdef NWY_OPEN_TEST_KEYPAD
static void _openkeypad(nwy_key_t key, nwy_keyState_t evt)
{
    uint8_t status = 0;
    if (evt & key_state_press)
        status = 1;
    if (evt & key_state_release)
        status = 0;

    if (evt == key_state_press)
    {
        NWY_CLI_LOG("\r\nThis key%d is press\r\n", key);
    }
    else
    {
        NWY_CLI_LOG("\r\n key id %d,status %d", key, status);
    }
}

void nwy_test_cli_keypad_reg_cb()
{
    nwy_test_cli_echo("\r\nTest in keypad cb!\r\n");
    reg_nwy_key_cb(_openkeypad);
}

void nwy_test_cli_keypad_set_debouce()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}
#endif
/**************************PWM*********************************/
#ifdef NWY_OPEN_TEST_PWM
nwy_pwm_t *test_p;
void nwy_test_cli_pwm_init()
{
    test_p = nwy_pwm_init(NAME_PWM_1, 100, 40);

    if (test_p == NULL)
    {
        nwy_test_cli_echo("\r\nPWM init failed!\r\n");
    }
    nwy_test_cli_echo("\r\nPWM init success!\r\n");
}

void nwy_test_cli_pwm_start()
{
    nwy_pwm_start(test_p);
    nwy_test_cli_echo("\r\nTest in pwm start!\r\n");
}

void nwy_test_cli_pwm_stop()
{
    nwy_pwm_stop(test_p);
    nwy_test_cli_echo("\r\nTest in pwm stop!\r\n");
}

void nwy_test_cli_pwm_deinit()
{
    nwy_pwm_deinit(test_p);
    nwy_test_cli_echo("\r\nTest in pwm deinit!\r\n");
}
#endif

#ifdef NWY_OPEN_TEST_PWM_EC618

void nwy_test_cli_pwm_init()
{
    char *opt;
    uint32_t pwm_id;
    uint32_t freq;
    uint32_t duty;

    int ret = -1;
    opt = nwy_test_cli_input_gets("\r\n Please input pwm index:");
    pwm_id = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\n Please input pwm freq:");
    freq = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\n Please input pwm duty:");
    duty = atoi(opt);

    ret = nwy_pwm_init(pwm_id, freq, duty);

    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nPWM init failed!\r\n");
    }
    else
    nwy_test_cli_echo("\r\nPWM init success!\r\n");
}

void nwy_test_cli_pwm_start()
{
    char *opt;
    uint32_t pwm_id;
    int ret = -1;
    opt = nwy_test_cli_input_gets("\r\n Please input pwm index:");
    pwm_id = atoi(opt);
    ret = nwy_pwm_start(pwm_id);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nTest in pwm start failed!\r\n");
    }
    else
        nwy_test_cli_echo("\r\nTest in pwm start ok!\r\n");
}

void nwy_test_cli_pwm_stop()
{
    char *opt;
    uint32_t pwm_id;
    int ret = -1;
    opt = nwy_test_cli_input_gets("\r\n Please input pwm index:");
    pwm_id = atoi(opt);
    ret = nwy_pwm_stop(pwm_id);
    if (ret < 0)
    {
        nwy_test_cli_echo("\r\nTest in pwm stop failed!\r\n");
    }
    else
        nwy_test_cli_echo("\r\nTest in pwm stop ok!\r\n");
}

void nwy_test_cli_pwm_deinit()
{
    char *opt;
    uint32_t pwm_id;
    opt = nwy_test_cli_input_gets("\r\n Please input pwm index:");
    pwm_id = atoi(opt);
    nwy_pwm_deinit(pwm_id);
    nwy_test_cli_echo("\r\nTest in pwm deinit!\r\n");
}
#endif
/**************************LCD*********************************/
#ifdef NWY_OPEN_TEST_LCD

#define ROW 128
#define COL 128

#define WIDTH 128
#define HEIGHT 128

#define ASC12_FILE_NAME "/ASC12"
#define ASC16_FILE_NAME "/ASC16"
#define HZK12_FILE_NAME "/HZK12"
#define HZK16_FILE_NAME "/HZK16"

#define LCD_DataWrite_ST7735(Data)                   \
    {                                                \
        while (nwy_lcd_bus_write_data(Data) != true) \
            ;                                        \
    }
#define LCD_CtrlWrite_ST7735(Cmd)                  \
    {                                              \
        while (nwy_lcd_bus_write_cmd(Cmd) != true) \
            ;                                      \
    }

static unsigned short display_buf[WIDTH * HEIGHT];

void nwy_write_data_buf(void *buf, unsigned int len)
{
    int i;
    unsigned char *data = (unsigned char *)buf;

    for (i = 0; i < len; i++)
        LCD_DataWrite_ST7735(data[i]);

    NWY_CLI_LOG("nwy_write_data_buf!");
}

void nwy_lcd_block_write(unsigned char startx, unsigned char starty, unsigned char endx, unsigned char endy)
{
    unsigned char buf[4];
    LCD_CtrlWrite_ST7735(0x2a);
    buf[0] = 0;
    buf[1] = startx;
    buf[2] = 0;
    buf[3] = endx;
    nwy_write_data_buf((unsigned short *)buf, sizeof(buf));

    LCD_CtrlWrite_ST7735(0x2B);
    buf[0] = 0;
    buf[1] = starty;
    buf[2] = 0;
    buf[3] = endy;
    nwy_write_data_buf((unsigned short *)buf, sizeof(buf));

    LCD_CtrlWrite_ST7735(0x2C);

    NWY_CLI_LOG("nwy LCD block write");
}

static void GetFontSize(unsigned int fontSize, unsigned int *GBK_W,
                        unsigned int *GBK_H, unsigned int *ASC_W, unsigned int *ASC_H)
{
    if (fontSize == 12)
    {
        if (GBK_W != NULL)
            *GBK_W = 12;
        if (GBK_H != NULL)
            *GBK_H = 12;
        if (ASC_W != NULL)
            *ASC_W = 6;
        if (ASC_H != NULL)
            *ASC_H = 12;
    }
    else if (fontSize == 16)
    {
        if (GBK_W != NULL)
            *GBK_W = 16;
        if (GBK_H != NULL)
            *GBK_H = 16;
        if (ASC_W != NULL)
            *ASC_W = 8;
        if (ASC_H != NULL)
            *ASC_H = 16;
    }
}

static void GetGbkOneBuf(unsigned int fontSize, unsigned char *gbk, unsigned short *gbk_buf, unsigned short TextColor, unsigned short BackColor)
{
    int qh;
    int wh;
    int offset = 0;
    int i, j, k;
    int flag;
    int n = 0;

    unsigned int GBK_W = 0;
    unsigned int GBK_H = 0;
    unsigned int WORD_SIZE = 0;
    char *filename = NULL;

    GetFontSize(fontSize, &GBK_W, &GBK_H, NULL, NULL);

    unsigned char buf[100] = {0};
    unsigned char key[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    qh = (int)(gbk[0] - 0xa0);
    wh = (int)(gbk[1] - 0xa0);

    if (fontSize == 12)
    {
        filename = HZK12_FILE_NAME;
        WORD_SIZE = 24;
        offset = (int)(94 * (qh - 1) + (wh - 1)) * WORD_SIZE;
    }
    else if (fontSize == 16)
    {
        filename = HZK16_FILE_NAME;
        WORD_SIZE = 32;
        offset = (int)(94 * (qh - 1) + (wh - 1)) * WORD_SIZE;
    }
    else
    {
        NWY_CLI_LOG("unsupport font size:%d", fontSize);
        return;
    }

    int fp = nwy_file_open(filename, NWY_RDONLY);
    if (0 > fp)
    {
        NWY_CLI_LOG("%s open fail\n", filename);
        return;
    }

    nwy_file_seek(fp, offset, NWY_SEEK_SET);
    int read_len = nwy_file_read(fp, buf, WORD_SIZE);

    if (read_len != WORD_SIZE)
    {
        NWY_CLI_LOG("%s read fail read_len=%d\n", filename, read_len);
    }

    nwy_file_close(fp);

    int total_word_size = WORD_SIZE / GBK_H;
    for (k = 0; k < GBK_H; k++)
    {
        for (j = 0; j < total_word_size; j++)
        {
            for (i = 0; i < 8; i++)
            {
                if (j * 8 + i >= GBK_W)
                    break;
                flag = buf[k * total_word_size + j] & key[i];
                if (flag)
                {
                    gbk_buf[n++] = TextColor;
                }
                else
                {
                    gbk_buf[n++] = BackColor;
                }
            }
        }
    }

    //LCD_BlockWrite(0,0, COL-1,ROW-1);
}

static void GetOneCharBuf(unsigned int fontSize, unsigned char ord, unsigned short *char_buf, unsigned short TextColor, unsigned short BackColor) // ord:0~95
{
    unsigned char i, j, k;
    unsigned char dat;
    int n = 0;

    unsigned int ASC_W = 0;
    unsigned int ASC_H = 0;
    unsigned int WORD_SIZE = 0;
    char *filename = NULL;
    unsigned char buf[100] = {0};

    GetFontSize(fontSize, NULL, NULL, &ASC_W, &ASC_H);

    if (fontSize == 12)
    {
        filename = ASC12_FILE_NAME;
        WORD_SIZE = 12;
    }
    else if (fontSize == 16)
    {
        filename = ASC16_FILE_NAME;
        WORD_SIZE = 16;
    }
    else
    {
        NWY_CLI_LOG("unsupport font size:%d", fontSize);
        return;
    }

    int fp = nwy_file_open(filename, NWY_RDONLY);
    if (0 > fp)
    {
        NWY_CLI_LOG("%s open fail\n", filename);
        return;
    }

    nwy_file_seek(fp, ord * WORD_SIZE, NWY_SEEK_SET);
    int read_len = nwy_file_read(fp, buf, WORD_SIZE);
    if (read_len != WORD_SIZE)
    {
        NWY_CLI_LOG("ASC16 read fail read_len=%d\n", read_len);
    }

    nwy_file_close(fp);

    int total_word_size = WORD_SIZE / ASC_H;

    for (k = 0; k < ASC_H; k++)
    {
        for (j = 0; j < total_word_size; j++)
        {
            dat = buf[k * total_word_size + j];
            for (i = 0; i < 8; i++)
            {
                if (j * 8 + i >= ASC_W)
                    break;
                if ((dat << i) & 0x80)
                {
                    char_buf[n++] = TextColor;
                }
                else
                {
                    char_buf[n++] = BackColor;
                }
            }
        }
    }
}

void nwy_dispstrline(unsigned int fontSize, unsigned char *str, unsigned int Xstart, unsigned int Ystart, unsigned short TextColor, unsigned short BackColor)
{
    int i, j;
    static unsigned short tmp_buf[256];
    int line_len;
    int str_index = 0;
    unsigned int Xend = Xstart;
    unsigned int GBK_W = 0;
    unsigned int GBK_H = 0;
    unsigned int ASC_W = 0;
    unsigned int ASC_H = 0;

    GetFontSize(fontSize, &GBK_W, &GBK_H, &ASC_W, &ASC_H);
    line_len = strlen((char *)str) * ASC_W;

    if (line_len + Xstart > WIDTH)
    {
        NWY_CLI_LOG("ERROR:::: line_len is too lone %d\r\n", line_len);
        return;
    }

    while (!(*str == '\0'))
    {
        if (*str > 0x80)
        {
            GetGbkOneBuf(fontSize, str, tmp_buf, TextColor, BackColor);
            for (i = 0; i < GBK_H; i++)
            {
                for (j = 0; j < GBK_W; j++)
                {
                    display_buf[i * line_len + str_index + j] = tmp_buf[i * GBK_W + j];
                }
            }
            str_index += GBK_W;

            if (Xstart > ((COL)-GBK_W))
            {
                Xstart = (COL)-GBK_W;
            }
            else
            {
                Xend = Xend + GBK_W;
            }

            if (Ystart > ((ROW)-GBK_H))
            {
                break;
            }

            str += 2;
        }
        else
        {
            GetOneCharBuf(fontSize, *str++, tmp_buf, TextColor, BackColor);
            for (i = 0; i < ASC_H; i++)
            {
                for (j = 0; j < ASC_W; j++)
                {
                    display_buf[i * line_len + str_index + j] = tmp_buf[i * ASC_W + j];
                }
            }
            str_index += ASC_W;

            if (Xstart > ((COL)-ASC_W * 2))
            {
                Xstart = (COL)-ASC_W * 2;
            }
            else
            {
                Xend = Xend + ASC_H;
            }

            if (Ystart > ((ROW)-ASC_H))
            {
                break;
            }
        }
    }

    nwy_lcd_block_write(Xstart, Ystart, Xstart + line_len - 1, Ystart + (GBK_H - 1));
    nwy_write_data_buf(display_buf, line_len * GBK_H * 2);
}

static inline void prvFillBufferWhiteScreen(uint16_t *buffer, unsigned width, unsigned height)
{
    memset(buffer, 0xff, width * height * sizeof(uint16_t));
}

static void prvLcdClear(void)
{
    prvFillBufferWhiteScreen(display_buf, WIDTH, HEIGHT);
    nwy_lcd_block_write(0, 0, WIDTH - 1, HEIGHT - 1);
    nwy_write_data_buf(display_buf, WIDTH * HEIGHT * 2);
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
/**************************************************************************************/
static void _st7735Init(void)
{
    NWY_CLI_LOG("lcd:  st7735Init ");
    nwy_sleep(200);
    LCD_CtrlWrite_ST7735(0x11);
    nwy_sleep(200);

    LCD_CtrlWrite_ST7735(0xB1);
    LCD_DataWrite_ST7735(0x05);
    LCD_DataWrite_ST7735(0x3C);
    LCD_DataWrite_ST7735(0x3C);

    LCD_CtrlWrite_ST7735(0xB2);
    LCD_DataWrite_ST7735(0x05);
    LCD_DataWrite_ST7735(0x3C);
    LCD_DataWrite_ST7735(0x3C);

    LCD_CtrlWrite_ST7735(0xB3);
    LCD_DataWrite_ST7735(0x05);
    LCD_DataWrite_ST7735(0x3C);
    LCD_DataWrite_ST7735(0x3C);
    LCD_DataWrite_ST7735(0x05);
    LCD_DataWrite_ST7735(0x3C);
    LCD_DataWrite_ST7735(0x3C);

    LCD_CtrlWrite_ST7735(0xB4);
    LCD_DataWrite_ST7735(0x03);

    LCD_CtrlWrite_ST7735(0xC0);
    LCD_DataWrite_ST7735(0x62);
    LCD_DataWrite_ST7735(0x02);
    LCD_DataWrite_ST7735(0x04);

    LCD_CtrlWrite_ST7735(0xC1);
    LCD_DataWrite_ST7735(0xC0);

    LCD_CtrlWrite_ST7735(0xC2);
    LCD_DataWrite_ST7735(0x0D);
    LCD_DataWrite_ST7735(0x00);

    LCD_CtrlWrite_ST7735(0xC3);
    LCD_DataWrite_ST7735(0x8D);
    LCD_DataWrite_ST7735(0x6A);

    LCD_CtrlWrite_ST7735(0xC4);
    LCD_DataWrite_ST7735(0x8D);
    LCD_DataWrite_ST7735(0xEE);

    LCD_CtrlWrite_ST7735(0xC5);
    LCD_DataWrite_ST7735(0x12);
    //turn right 90
    LCD_CtrlWrite_ST7735(0x36);
    LCD_DataWrite_ST7735(0x60);

    LCD_CtrlWrite_ST7735(0xE0);
    LCD_DataWrite_ST7735(0x03);
    LCD_DataWrite_ST7735(0x1B);
    LCD_DataWrite_ST7735(0x12);
    LCD_DataWrite_ST7735(0x11);
    LCD_DataWrite_ST7735(0x3F);
    LCD_DataWrite_ST7735(0x3A);
    LCD_DataWrite_ST7735(0x32);
    LCD_DataWrite_ST7735(0x34);
    LCD_DataWrite_ST7735(0x2F);
    LCD_DataWrite_ST7735(0x2B);
    LCD_DataWrite_ST7735(0x30);
    LCD_DataWrite_ST7735(0x3A);
    LCD_DataWrite_ST7735(0x00);
    LCD_DataWrite_ST7735(0x01);
    LCD_DataWrite_ST7735(0x02);
    LCD_DataWrite_ST7735(0x05);

    LCD_CtrlWrite_ST7735(0xE1);
    LCD_DataWrite_ST7735(0x03);
    LCD_DataWrite_ST7735(0x1B);
    LCD_DataWrite_ST7735(0x12);
    LCD_DataWrite_ST7735(0x11);
    LCD_DataWrite_ST7735(0x32);
    LCD_DataWrite_ST7735(0x2F);
    LCD_DataWrite_ST7735(0x2A);
    LCD_DataWrite_ST7735(0x2F);
    LCD_DataWrite_ST7735(0x2E);
    LCD_DataWrite_ST7735(0x2C);
    LCD_DataWrite_ST7735(0x35);
    LCD_DataWrite_ST7735(0x3F);
    LCD_DataWrite_ST7735(0x00);
    LCD_DataWrite_ST7735(0x00);
    LCD_DataWrite_ST7735(0x01);
    LCD_DataWrite_ST7735(0x05);

    LCD_CtrlWrite_ST7735(0xFC);
    LCD_DataWrite_ST7735(0x8C);

    LCD_CtrlWrite_ST7735(0x3A);
    LCD_DataWrite_ST7735(0x05);

    LCD_CtrlWrite_ST7735(0x29);
}

#define LCD_BACK_LIGHT_POWER NWY_POWER_RGB_IB0
#define LCD_MAIN_POWER NWY_POWER_LCD
#define LCD_BUS_CLK_FREQ (20000000)

static bool lcd_init = false;
void nwy_lcd_init(void)
{
    if (lcd_init)
        return;

    nwy_subpower_switch(LCD_MAIN_POWER, true, true);
    nwy_subpower_switch(NWY_POWER_BACK_LIGHT, true, true);
    nwy_subpower_switch(LCD_BACK_LIGHT_POWER, true, true);
    nwy_sleep(256);
    nwy_lcd_bus_config_t lcd_bus_config =
        {
            .cs = NWY_LCD_BUS_CS_0,
            .cs0Polarity = false,
            .cs1Polarity = false,
            .resetb = true,
            .rsPolarity = false,
            .wrPolarity = false,
            .rdPolarity = false,
            .highByte = false,
            .clk = LCD_BUS_CLK_FREQ,
        };
    nwy_lcd_bus_init(&lcd_bus_config);
    nwy_sleep(32);
    _st7735Init();
    nwy_sleep(32);
    prvLcdClear();

    lcd_init = true;
}

void nwy_lcd_deinit(void)
{
    nwy_lcd_bus_deinit();
    nwy_subpower_switch(NWY_POWER_BACK_LIGHT, false, false);
    nwy_subpower_switch(LCD_BACK_LIGHT_POWER, false, false);
    nwy_subpower_switch(LCD_MAIN_POWER, false, false);

    lcd_init = false;
}


#define BLACK 0x0000
#define NAVY 0x000F
#define DGREEN 0x03E0
#define DCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LGRAY 0xC618
#define DGRAY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

static unsigned short display_buffer[128 * 128];
void nwy_test_cli_lcd_open()
{
    nwy_lcd_init();
    nwy_test_cli_echo("\r\nlcd open success!\r\n");
}

void nwy_test_cli_lcd_draw_line()
{
    int i;
    nwy_lcd_block_write(0, 96, 127, 96);
    for (i = 0; i < 128; i++)
    {
        display_buffer[i] = OLIVE;
    }
    nwy_write_data_buf(display_buffer, 128 * 2);
    nwy_test_cli_echo("\r\nlcd draw line success!\r\n");
}

/* GB18030 chinese code */
static const char chinese_string[] = {
    0xd3, 0xd0, /* �� */
    0xb7, 0xbd, /* �� */
    0xba, 0xba, /* �� */
    0xd7, 0xd6, /* �� */
    0xd1, 0xdd, /* �� */
    0xca, 0xbe, /* ʾ */
    '\0'};

#define MK_COLOR(r, g, b) ((((r)&0x1f) << 11) + (((g)&0x3f) << 5) + (((b)&0x1f) << 0))
#define COLOR_WHITE MK_COLOR(0xff, 0xff, 0xff)
#define COLOR_BLACK MK_COLOR(0, 0, 0)

void nwy_test_cli_lcd_draw_chinese()
{
    nwy_lcd_block_write(16, 19, 16 + 16 * 6, 34);
    nwy_dispstrline(16, (unsigned char *)chinese_string, 16, 19, BLACK, MAGENTA);
    nwy_test_cli_echo("\r\nlcd draw chinese success!\r\n");
}

void nwy_test_cli_lcd_close()
{
    nwy_lcd_deinit();
    nwy_test_cli_echo("\r\nlcd close success!\r\n");
}

void nwy_test_cli_lcd_open_bl()
{
    nwy_subpower_switch(NWY_POWER_BACK_LIGHT, true, true);
    nwy_subpower_switch(LCD_BACK_LIGHT_POWER, true, true);
    nwy_test_cli_echo("\r\nlcd backlight open success!\r\n");
}

void nwy_test_cli_lcd_close_bl()
{
    nwy_subpower_switch(NWY_POWER_BACK_LIGHT, false, false);
    nwy_subpower_switch(LCD_BACK_LIGHT_POWER, false, false);
    nwy_test_cli_echo("\r\nlcd backlight close success!\r\n");
}

void nwy_test_cli_lcd_set_bl_level()
{
    char *sptr;
    sptr = nwy_test_cli_input_gets("\r\n Please input light level[0~63]:");
    int level = atoi(sptr);
    nwy_set_back_light_level(NWY_POWER_RGB_IB0, level);
    nwy_set_back_light_level(NWY_POWER_RGB_IB1, level);
    nwy_set_back_light_level(NWY_POWER_RGB_IB2, level);
    nwy_test_cli_echo("\r\nlcd backlight set success!\r\n");
}
#endif
/**************************SD*********************************/
void nwy_test_cli_sd_get_st()
{
    nwy_test_cli_echo("\r\nsd state:%d\r\n", nwy_read_sdcart_status());
}

void nwy_test_cli_sd_mnt()
{
    nwy_test_cli_echo("\r\nsd mount:%d\r\n", nwy_sdk_sdcard_mount());
}

void nwy_test_cli_sd_unmnt()
{
    nwy_sdk_sdcard_unmount();
    nwy_test_cli_echo("\r\nsd unmount success\r\n");
}
void nwy_test_cli_sd_format()
{
    nwy_format_sdcard();
    nwy_test_cli_echo("\r\nsd format end\r\n");
}
/**************************FLASH*********************************/
void nwy_test_cli_flash_open()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_flash_erase()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_flash_write()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_flash_read()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

/**************************TTS*********************************/
#ifdef FEATURE_NWY_OPEN_TTS
char buff[1024] = {0};
nwy_tts_encode_t encode_type = ENCODE_GBK;
char *hexbuf = "b8b8c4b8d4daa3acb2bbd4b6d3cea3acd3ceb1d8d3d0b7bd";

static void tts_play_callback(void *cb_para, nwy_neoway_result_t result)
{
    switch (result)
    {
    case PLAY_END:
        nwy_test_cli_echo("\r\n tts test down \r\n");
        break;
    default:
        break;
    }
}

void nwy_test_cli_tts_input()
{
    char *sptr;
    memset(buff, 0, 1024);
    sptr = nwy_test_cli_input_gets("\r\nPlease input encode mode(0-gbk,1-utf16le,2-utf16be,3-utf8): ");
    encode_type = (nwy_tts_encode_t)atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\nPlease input content: ");
    strncpy(buff, sptr, strlen(sptr));
}

void nwy_test_cli_tts_play_start()
{
    if (strlen(buff) == 0)
        nwy_tts_playbuf(hexbuf, strlen(hexbuf), ENCODE_GBK, tts_play_callback, NULL);
    else
        nwy_tts_playbuf(buff, strlen(buff), encode_type, tts_play_callback, NULL);
}

void nwy_test_cli_tts_play_stop()
{
    nwy_tts_stop_play();
}
#endif
/**************************FOTA*********************************/
#ifndef NWY_OPEN_TEST_FOTA_NS
nwy_osi_msg_queue_t nwy_download_msg_queue = NULL;
typedef struct
{
    uint32 len;
    void *data;
} nwy_download_queue_msg_t;

static void nwy_cli_recv_callback(unsigned char *data, uint32 length)
{
    nwy_download_queue_msg_t msg;
    if (!data || !length)
        return;
    msg.data = malloc(length);
    if (!msg.data)
        return;
    msg.len = length;
    memcpy(msg.data, data, msg.len);
    if (nwy_download_msg_queue)
        if (NWY_SUCESS != nwy_msg_queue_send(nwy_download_msg_queue, sizeof(msg), &msg, NWY_OSA_SUSPEND))
        {
            free(msg.data);
            NWY_CLI_LOG("put msg que failed drop the data:%d", length);
        }
}

typedef void (*nwy_test_cli_download_callback_t)(unsigned char *data, uint32 length, void *arg);

int nwy_test_cli_download_data(uint32 download_size, uint32 timeout, nwy_test_cli_download_callback_t fn, void *arg)
{
    uint32 size = 0;
    nwy_download_queue_msg_t msg;
    if (!download_size || !fn)
        return size;
    nwy_msg_queue_create(&nwy_download_msg_queue, NULL, sizeof(nwy_download_queue_msg_t), 1024);
    if (!nwy_download_msg_queue)
        return size;
    nwy_test_cli_sio_enter_trans_mode((nwy_sio_trans_cb)nwy_cli_recv_callback);
    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        if (NWY_SUCESS == nwy_msg_queue_recv(nwy_download_msg_queue, sizeof(msg),(uint8 *)&msg, NWY_OSA_SUSPEND))
        {
            if (msg.data && msg.len)
            {
                if ((size + msg.len) > download_size)
                    msg.len = download_size - size;

                fn(msg.data, msg.len, arg);

                free(msg.data);
                size += msg.len;
                if (size >= download_size)
                    break;
            }
        }
        else
            break;
    }

    nwy_test_cli_sio_quit_trans_mode();
    #if 0
    /* clear the rest fifo mem */
    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        if (NWY_SUCESS == nwy_msg_queue_recv(nwy_download_msg_queue, sizeof(msg), (uint8 *)&msg, NWY_OSA_SUSPEND))
        {
            if (msg.data && msg.len)
            {
                free(msg.data);
            }
        }
        else
            break;
    }
    #endif
    nwy_msg_queue_delete(nwy_download_msg_queue);
    nwy_download_msg_queue = NULL;
    return size;
}

void nwy_test_cli_download_sdk_fota_pkt_cb(unsigned char *data, uint32 length, void *arg)
{
    ota_package_t *pkt = arg;
    pkt->data = data;
    pkt->len = length;
    if (!nwy_fota_download_core(pkt))
        pkt->offset += length;
    nwy_test_cli_echo("\r\ndownload pkt size:%d", pkt->offset);
}

void nwy_test_cli_fota_base_ver()
{
    char *sptr;
    int pkt_size;
    sptr = nwy_test_cli_input_gets("\r\n Please input firmware packet size:");
    pkt_size = atoi(sptr);
    if (pkt_size <= 0)
    {
        nwy_test_cli_echo("\r\n Fota Error : invalid packet size:%s", sptr);
        return;
    }
    nwy_test_cli_echo("\r\n Please input firmware:\r\n");
    ota_package_t ota_pkt;
    memset(&ota_pkt, 0, sizeof(ota_pkt));
    ota_pkt.ota_size = pkt_size;
    if (pkt_size <= nwy_test_cli_download_data(pkt_size, 8000, nwy_test_cli_download_sdk_fota_pkt_cb, &ota_pkt))
    {
        nwy_test_cli_echo("\r\n firmware download finish");
        nwy_test_cli_echo("\r\n system will reset for update");
        nwy_sleep(1000);
        nwy_version_core_update(true);
        nwy_test_cli_echo("\r\n firmware wrong");
    }
    nwy_test_cli_echo("\r\n what happened.");
}

void nwy_test_cli_download_app_fota_pkt_cb(unsigned char *data, uint32 length, void *arg)
{
    ota_package_t *pkt = arg;
    pkt->data = data;
    pkt->len = length;

    if (!nwy_fota_dm(pkt))
        pkt->offset += length;
    nwy_test_cli_echo("\r\ndownload pkt size:%d", pkt->offset);
}

void nwy_test_cli_fota_app_ver()
{
    char *sptr;
    int ret, pkt_size;
    sptr = nwy_test_cli_input_gets("\r\n Please input firmware packet size:");
    pkt_size = atoi(sptr);
    if (pkt_size <= 0)
    {
        nwy_test_cli_echo("\r\n Fota Error : invalid packet size:%s", sptr);
        return;
    }
    nwy_test_cli_echo("\r\n Please input firmware:\r\n");
    ota_package_t ota_pkt;
    memset(&ota_pkt, 0, sizeof(ota_pkt));
    ota_pkt.ota_size = pkt_size;
    if (pkt_size <= nwy_test_cli_download_data(pkt_size, 8000, nwy_test_cli_download_app_fota_pkt_cb, &ota_pkt))
    {
        nwy_test_cli_echo("\r\n firmware download finish");
        ret = nwy_package_checksum();
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nchecksum failed");
            return;
        }
        nwy_test_cli_echo("\r\n system will reset for update");
        nwy_sleep(1000);
        ret = nwy_fota_ua();
        if (ret < 0)
        {
            nwy_test_cli_echo("\r\nupdate failed");
            return;
        }
    }
    nwy_test_cli_echo("\r\n what happened.");
}
#endif
/**************************AUDIO*********************************/
#ifdef NWY_OPEN_TEST_AUDIO
static int capture_len = 0;
static uint8_t *audio_buf;
#define AUDIO_RECORDER_DATA_LEN (1024 * 48)
char nwy_ext_sio_recv_buff[NWY_EXT_SIO_RX_MAX + 1] = {0};

static int nwy_player_cb(nwy_player_status state)
{
    nwy_test_cli_echo("nwytest_player_cb state=%d", state);
    return 1;
}

void nwy_test_cli_audio_play()
{
    nwy_test_cli_echo("\r\naudio player is running\r\n");
    int read_index = 0;

    nwy_audio_player_open(nwy_player_cb);
    while (read_index < capture_len)
    {
        nwy_audio_player_play(&audio_buf[read_index], 320);
        read_index += 320;
    }
    nwy_audio_player_stop();
    nwy_audio_player_close();
    if (audio_buf != NULL)
    {
        capture_len = 0;
        free(audio_buf);
        audio_buf = NULL;
    }
    nwy_test_cli_echo("\r\naudio play done\r\n");
}

static int capture_callback(unsigned char *pdata, unsigned int len)
{
    if ((capture_len + len) < (AUDIO_RECORDER_DATA_LEN))
    {
        memcpy(audio_buf + capture_len, pdata, len);
        capture_len += len;
    }
    else
    {
        nwy_test_cli_echo("recorder data is overflow\r\n");
    }
    return 1;
}

void nwy_test_cli_audio_rec()
{
    capture_len = 0;
    audio_buf = (uint8_t *)malloc(AUDIO_RECORDER_DATA_LEN);
    if (NULL == audio_buf)
    {
        nwy_test_cli_echo("audio malloc failed\r\n");
        return;
    }
    memset(audio_buf, 0, AUDIO_RECORDER_DATA_LEN);

    nwy_test_cli_echo("\r\naudio recorder is running\r\n");
    nwy_audio_recorder_open(capture_callback);
    nwy_audio_recorder_start();
    nwy_sleep(500 * 6);
    nwy_audio_recorder_stop();
    nwy_audio_recorder_close();
}

void nwy_test_cli_audio_dtmf()
{
    char *tone = nwy_ext_sio_recv_buff;
    nwy_test_cli_input_gets("\r\nPlease input DTMF: ");
    nwy_audio_tone_play(tone, 200, 15);
}

static void _handleKeyDetct(char key)
{
    nwy_test_cli_echo("\r\nNWY_DTMF: %c", key);
}

void nwy_test_cli_audio_dtmf_enable()
{
    nwy_test_cli_echo("\r\nStart Detect DTMF");
    nwy_audio_tone_detect(1, _handleKeyDetct);
}

void nwy_test_cli_audio_dtmf_disable()
{
    nwy_test_cli_echo("\r\nStop Detect DTMF");
    nwy_audio_tone_detect(0, NULL);
}
#endif
#endif
#endif
/**************************FS*********************************/
#define NWY_FILE_NAME_MAX 64
static int nwy_test_fs_fd = -1;
static char nwy_test_file_name[NWY_FILE_NAME_MAX + 1] = {0};
/*
static void nwy_fs_close(int fd)
{
    nwy_file_close(fd);
    nwy_test_fs_fd = -1;
}
*/
void nwy_test_cli_fs_open(void)
{
    char *sptr;
    if (nwy_test_fs_fd > 0) {
        nwy_test_cli_echo("\r\nclose current file %s first", nwy_test_file_name);
        return;
    }
    memset(nwy_test_file_name, 0, sizeof(nwy_test_file_name));
    sptr = nwy_test_cli_input_gets("\r\nPlease input filename(len < %d): ", NWY_FILE_NAME_MAX);
    if (strlen(sptr) >= NWY_FILE_NAME_MAX)
    {
        nwy_test_cli_echo("\r\nfile name must be less than %d", NWY_FILE_NAME_MAX);
        return;
    }
    strcpy(nwy_test_file_name, sptr);
    nwy_test_fs_fd = nwy_file_open(nwy_test_file_name, NWY_WB_PLUS_MODE);
    if (nwy_test_fs_fd == NWY_FS_PATH_ERR) {
        nwy_test_cli_echo("\r\nfile name can't with path,only support current directory\r\n");
    } else if (nwy_test_fs_fd < 0) {
        nwy_test_cli_echo("\r\nfile %s open error:%d\r\n", nwy_test_file_name, nwy_test_fs_fd);
    } else {
        nwy_test_cli_echo("\r\nfile %s open success:%d\r\n", nwy_test_file_name, nwy_test_fs_fd);
    }
}

void nwy_test_cli_fs_open_filename(const char *filename)
{
    if (nwy_test_fs_fd > 0) {
        nwy_test_cli_echo("\r\nclose current file %s first", nwy_test_file_name);
        // return -1;  // File already open
    }

    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= NWY_FILE_NAME_MAX) {
        nwy_test_cli_echo("\r\nInvalid filename. Must be non-empty and less than %d characters.", NWY_FILE_NAME_MAX);
        // return -2;  // Invalid filename
    }

    memset(nwy_test_file_name, 0, sizeof(nwy_test_file_name));
    strcpy(nwy_test_file_name, filename);
    nwy_test_cli_echo("\r\nfile %s open", nwy_test_file_name);
    nwy_test_fs_fd = nwy_file_open(nwy_test_file_name, NWY_WB_PLUS_MODE);

    if (nwy_test_fs_fd == NWY_FS_PATH_ERR) {
        nwy_test_cli_echo("\r\nfile name can't have path, only support current directory");
        // return -3;  // Path not allowed
    } else if (nwy_test_fs_fd < 0) {
        nwy_test_cli_echo("\r\nfile %s open error: %d", nwy_test_file_name, nwy_test_fs_fd);
        // return -4;  // Generic open error
    } else {
        nwy_test_cli_echo("\r\nfile %s open success: %d", nwy_test_file_name, nwy_test_fs_fd);
        // return 0;   // Success
    }
}


void nwy_test_cli_fs_write(void)
{
    char *sptr;
    int avail_space = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input file write data(len <= 2000): ");
    int len = strlen(sptr);
    if (len > 2000)
    {
        nwy_test_cli_echo("\r\nfile write data can't beyond 2000");
        return;
    }
    int rtn = nwy_file_write(nwy_test_fs_fd, sptr, len);
    if (rtn != len)
        nwy_test_cli_echo("\r\nfile %s write error:%d\r\n", nwy_test_file_name, rtn);
    else
        nwy_test_cli_echo("\r\nfile %s write success:%d\r\n", nwy_test_file_name, rtn);

}

void nwy_test_cli_fs_read(){
    int size, rtn;
    char buffer[128+1] = {0};
    nwy_test_cli_echo("\r\n");
    size = rtn = 0;
    bool error = false;
    if(nwy_test_fs_fd < 0){
        nwy_test_cli_echo("\r\nfile not opened\r\n");
        return;
    }
    nwy_file_seek(nwy_test_fs_fd, 0, NWY_SEEK_SET);

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        rtn = nwy_file_read(nwy_test_fs_fd, buffer, sizeof(buffer)-1);
        NWY_CLI_LOG("read data size:%d", rtn);
        if (rtn < 0) {
            error = true;
            nwy_test_cli_echo("\r\nnwy_file_read error, rtn = %d\r\n", rtn);
            break;
        } else if (rtn == 0) {
            NWY_CLI_LOG("end of file");
            break; // EOF
        }

        buffer[rtn] = '\0'; // Ensure null-termination
        nwy_test_cli_echo(buffer);

        if(size > __INT_MAX__ -rtn){
            nwy_test_cli_echo("\r\nfile too large\r\n");
            error = true;
            break;
        }
        size += rtn;
    }

    if (error) {
        nwy_test_cli_echo("\r\nfile read failed\r\n");
    } else if (size > 0) {
        nwy_test_cli_echo("\r\nfile %s read success:%d\r\n", nwy_test_file_name, size);
    } else {
        nwy_test_cli_echo("\r\nfile %s is empty\r\n", nwy_test_file_name);
    }

}

void nwy_test_cli_fs_fsize()
{
    char *sptr = NULL;
    nwy_test_cli_echo("\r\nfile %s size:%d\r\n", nwy_test_file_name, nwy_file_fd_size(nwy_test_fs_fd));

}

void nwy_test_cli_fs_seek()
{
    char *sptr;
    char buffer[128] = {0};
    int size = 0;

    sptr = nwy_test_cli_input_gets("\r\nPlease input file seek offset: ");
    int offset = atoi(sptr);
    int rtn = nwy_file_seek(nwy_test_fs_fd, offset, NWY_SEEK_SET);
    if (rtn != offset)
        nwy_test_cli_echo("\r\nfile %s seek error:%d\r\n", nwy_test_file_name, rtn);
    else
        nwy_test_cli_echo("\r\nfile %s seek success:%d\r\n", nwy_test_file_name, rtn);
/*
    while (1)
    {
        rtn = nwy_file_read(nwy_test_fs_fd, buffer, sizeof(buffer));
        NWY_CLI_LOG("read data size:%d", rtn);
        if (rtn > 0)
            nwy_test_cli_echo(buffer, rtn);
        else
            break;
        size += rtn;
    }
    if (size)
        nwy_test_cli_echo("\r\nfile %s read success:%d\r\n", nwy_test_file_name, size);
*/
}

void nwy_test_cli_fs_ftell()
{
    nwy_test_cli_echo("\r\nfile %s tell:%d\r\n", nwy_test_file_name, nwy_file_tell(nwy_test_fs_fd));
}

void nwy_test_cli_fs_sync()
{
    int ret = NWY_GEN_E_UNKNOWN;
    ret = nwy_file_sync(nwy_test_fs_fd);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nfile sync sucess!\r\n");
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nfile sync failed!\r\n");
    }
}

void nwy_test_cli_fs_fstate()
{
    nwy_file_stat st;
    int ret = NWY_GEN_E_UNKNOWN;

    memset(&st, 0, sizeof(st));
    ret = nwy_file_fd_stat_get(nwy_test_fs_fd, &st);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nfile %s sta st_size:%d\r\n", nwy_test_file_name, st.st_size);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nfile stat get failed!\r\n");
    }
}

void nwy_test_cli_fs_trunc()
{
    char *sptr;
    nwy_file_seek(nwy_test_fs_fd, 0, NWY_SEEK_SET);
    sptr = nwy_test_cli_input_gets("\r\nPlease input file trunc size: ");
    int size = atoi(sptr);
    int ret = nwy_file_fd_trunc(nwy_test_fs_fd, size);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nfile %s trunc success\r\n", nwy_test_file_name);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nfile %s trunc error:%d\r\n", nwy_test_file_name, ret);
    }
}

void nwy_test_cli_fs_close()
{
    nwy_test_cli_echo("\r\nfile %s close:%d\r\n", nwy_test_file_name, nwy_file_close(nwy_test_fs_fd));
    memset(nwy_test_file_name, 0, sizeof(nwy_test_file_name));
    nwy_test_fs_fd = -1;
}

void nwy_test_cli_fs_remove()
{
    char *sptr;
    sptr = nwy_test_cli_input_gets("\r\nPlease input file name: ");

    if (memcmp(nwy_test_file_name, sptr, strlen(sptr)) == 0 && (strlen(sptr) == strlen(nwy_test_file_name))) {
        nwy_test_cli_echo("\r\nPlease close current open file %s first\r\n", nwy_test_file_name);
        return;
    }
    int rtn = nwy_file_remove(sptr);
    if (rtn != 0)
        nwy_test_cli_echo("\r\nfile %s remove error:%d\r\n", sptr, rtn);
    else
        nwy_test_cli_echo("\r\nfile %s remove success:%d\r\n", sptr, rtn);
}

void nwy_test_cli_fs_rename()
{
    char *sptr;
    char old[64], new[64];
    memset(old, 0, sizeof(old));
    memset(new, 0, sizeof(new));
    sptr = nwy_test_cli_input_gets("\r\nPlease input file old name: ");
    if (memcmp(nwy_test_file_name, sptr, strlen(sptr)) == 0 && (strlen(sptr) == strlen(nwy_test_file_name))) {
        nwy_test_cli_echo("\r\nPlease close current open file %s first\r\n", nwy_test_file_name);
        return;
    }
    strncpy(old, sptr, sizeof(old));
    sptr = nwy_test_cli_input_gets("\r\nPlease input file new name: ");
    strncpy(new, sptr, sizeof(new));
    int rtn = nwy_file_rename(old, new);
    if (rtn != 0)
        nwy_test_cli_echo("\r\nfile %s rename error:%d\r\n", old, rtn);
    else
        nwy_test_cli_echo("\r\nfile %s rename success:%d\r\n", new, rtn);
}

#define NWY_DIR_NAME_MAX 64

static nwy_dir_info_t nwy_test_fs_dir;
static char nwy_test_dir_name[NWY_DIR_NAME_MAX + 1] = {0};

void nwy_test_cli_dir_open()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr;
    sptr = nwy_test_cli_input_gets("\r\nPlease input dir name(len < %d): ", NWY_DIR_NAME_MAX);
    if (strlen(sptr) > NWY_DIR_NAME_MAX)
    {
        nwy_test_cli_echo("\r\ndir name must be less than %d", NWY_DIR_NAME_MAX);
        return;
    }


    if (memcmp(nwy_test_dir_name, sptr, strlen(sptr)) == 0 && (strlen(sptr) == strlen(nwy_test_dir_name))) {
        nwy_test_cli_echo("\r\nDir %s is been opened\r\n", nwy_test_dir_name);
        return;
    } else if (strlen(nwy_test_dir_name) != 0){
        nwy_test_cli_echo("\r\nclose current dir %s first", nwy_test_dir_name);
        return;
    }
        
    memset(nwy_test_dir_name, 0, sizeof(nwy_test_dir_name));
    strcpy(nwy_test_dir_name, sptr);

    ret = nwy_dir_open(nwy_test_dir_name, &nwy_test_fs_dir);
    if (ret != NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s open error\r\n", nwy_test_dir_name);
        memset(nwy_test_dir_name, 0, sizeof(nwy_test_dir_name));
    } else {
        nwy_test_cli_echo("\r\ndir %s open success,id = %d\r\n", nwy_test_dir_name, nwy_test_fs_dir.fd);

    }
}

void nwy_test_cli_dir_read()
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    char rsp[256 + 84];
    nwy_file_stat st;
    nwy_dirent_t info;
    size_t name_len = strlen(nwy_test_dir_name);
    bool trail_slash = (name_len > 0 && nwy_test_dir_name[name_len - 1] == '/');

    nwy_dir_rewind(&nwy_test_fs_dir);

    while ((ret = nwy_dir_read(&nwy_test_fs_dir, &info)) == NWY_SUCCESS)
    {
       if (trail_slash) {
           snprintf(rsp, sizeof(rsp), "%s%s", nwy_test_dir_name, info.d_name);
       }
       else {
           snprintf(rsp, sizeof(rsp), "%s/%s", nwy_test_dir_name, info.d_name);
       }

        nwy_test_cli_echo("\r\n%s", rsp);
    }
    nwy_test_cli_echo("\r\n");

    if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    }
}

void nwy_test_cli_dir_tell()
{
    int ret = NWY_GEN_E_UNKNOWN;
    ret = nwy_dir_tell(&nwy_test_fs_dir);
    if (ret >= 0) {
        nwy_test_cli_echo("\r\ndir %s tell:%d\r\n", nwy_test_dir_name, ret);
    } else {
        nwy_test_cli_echo("\r\ndir tell failed\r\n");
    }

}

void nwy_test_cli_dir_seek()
{
    int ret = NWY_GEN_E_UNKNOWN;
    char *sptr;
    sptr = nwy_test_cli_input_gets("\r\nPlease input dir seek offset: ");
    int offset = atoi(sptr);
    ret = nwy_dir_seek(&nwy_test_fs_dir, offset);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s seek success:%d\r\n", nwy_test_dir_name, offset);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\ndir seek failed\r\n");
    }
}

void nwy_test_cli_dir_rewind()
{
    int ret = NWY_GEN_E_UNKNOWN;
    ret = nwy_dir_rewind(&nwy_test_fs_dir);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s rewind success\r\n", nwy_test_dir_name);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\ndir rewind failed\r\n");
    }

}

void nwy_test_cli_dir_close()
{
    int ret = NWY_GEN_E_UNKNOWN;
    ret = nwy_dir_close(&nwy_test_fs_dir);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s close sucess\r\n", nwy_test_dir_name);
        memset(&nwy_test_fs_dir, 0, sizeof(nwy_test_fs_dir));
        memset(nwy_test_dir_name, 0, sizeof(nwy_test_dir_name));
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\ndir %s close fail\r\n", nwy_test_dir_name);
    }


}

void nwy_test_cli_dir_mk()
{
    char *sptr;
    int ret = NWY_GEN_E_UNKNOWN;

    sptr = nwy_test_cli_input_gets("\r\nPlease input dir name(len <= %d): ", NWY_DIR_NAME_MAX);
    if (strlen(sptr) > NWY_DIR_NAME_MAX)
    {
        nwy_test_cli_echo("\r\ndir name must be less than %d", NWY_DIR_NAME_MAX);
        return;
    }

    ret = nwy_dir_mk(sptr);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s make sucess\r\n", sptr);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\ndir make fail\r\n");
    }
}

void nwy_test_cli_dir_remove()
{
    char *sptr;
    int ret = NWY_GEN_E_UNKNOWN;
    sptr = nwy_test_cli_input_gets("\r\nPlease input dir name(len <= %d): ", NWY_DIR_NAME_MAX);
    if (strlen(sptr) > NWY_DIR_NAME_MAX)
    {
        nwy_test_cli_echo("\r\ndir name must be less than %d", NWY_DIR_NAME_MAX);
        return;
    }

    if (memcmp(nwy_test_dir_name, sptr, strlen(sptr)) == 0 && (strlen(sptr) == strlen(nwy_test_dir_name))) {
        nwy_test_cli_echo("\r\nPlease close current open dir %s first\r\n", nwy_test_dir_name);
        return;
    }

    ret = nwy_dir_rm(sptr);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\ndir %s remove sucess\r\n", sptr);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\ndir remove fail\r\n");
    }
}

void nwy_test_cli_fs_free_size()
{
    int ret = NWY_GEN_E_UNKNOWN;
    unsigned long long free_size = 0;
    ret = nwy_vfs_free_size_get(NULL, &free_size);
    if (ret ==  NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nfs free size:%lld\r\n", free_size);
    } else if (ret == NWY_GEN_E_PLAT_NOT_SUPPORT) {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    } else {
        nwy_test_cli_echo("\r\nget free size failed\r\n");
    }
}


void nwy_test_cli_spi_flash_mount(void)
{
#ifdef NWY_OPEN_TEST_SPI_FLASH
#define SPI_FLASH_MOUNT_POINT "ext_spi_flash"
#define SPI_FLASH_BUS         NAME_SPI_BUS_1
#define SPI_FLASH_SIZE_MAX    (128 * 1024 * 1024)

    static char s_w_buf[4096] = {0};
    static char s_r_buf[4096] = {0};
    int opt;
    int ret;
    nwy_block_device_t * block_dev = NULL;

    nwy_test_cli_echo("\r\n flash test nwy_spi_flash_mount_test start");
    block_dev = nwy_vfs_logical_block_device_create(SPI_FLASH_BUS, 0, SPI_FLASH_SIZE_MAX);
    if(block_dev == NULL)
    {
        nwy_test_cli_echo("\r\n flash test block_dev create fail");
        return;
    }
    while(1)
    {
        int mount = nwy_test_cli_input_gets("\r\ninput option:\r\n0-fs mount\r\n1-fs format\r\n2-fs write test\r\n3-fs read test\r\n4-fs free size\r\n5-exit:");
        opt = atoi(mount);
        if(opt == 0)
        {   // mount
            nwy_test_cli_echo("\r\n flash test nwy_vfs_mount start");
            ret = nwy_vfs_mount(SPI_FLASH_MOUNT_POINT, block_dev);
            if(0 > ret)
            {
                nwy_test_cli_echo("\r\n flash test mount fail");
                ret = nwy_vfs_mkfs(block_dev);
                if(0 > ret)
                {
                    nwy_test_cli_echo("\r\n flash test format fail");
                    continue;
                }
                ret = nwy_vfs_mount(SPI_FLASH_MOUNT_POINT, block_dev);
                if(0 > ret)
                {
                    nwy_test_cli_echo("\r\n flash test remount fail");
                    continue;
                }
            }
            nwy_test_cli_echo("\r\n flash test nwy_vfs_mount success");
        }
        else if(opt == 1)
        {   // format
            ret = nwy_vfs_mkfs(block_dev);
            nwy_test_cli_echo("\r\n flash test format :%d", ret);
            continue;
        }
        else if(opt == 2)
        {   // write
            int i = 1;
            int len, size;
            char file_name[64] = {0};
            int total = 0;
            while(i)
            {
                memset(file_name, 0, sizeof(file_name));
                sprintf(file_name, "%s/rw_test_%d", SPI_FLASH_MOUNT_POINT, i);
                int fd = nwy_file_open(file_name, NWY_CREAT | NWY_RDWR | NWY_TRUNC);
                if(fd < 0)
                {
                    nwy_test_cli_echo("\r\nfile open %s fail\r\n", file_name);
                    break;
                }
                else
                {
                    int op_size = strlen(file_name);
                    int64 op_ts = nwy_uptime_get();
                    len = size = 0;
                    while(len < 4096)
                    {
                        size = ((4096 - len) > op_size) ? op_size : (4096 - len);
                        memcpy((char *)s_w_buf + len, file_name, size);
                        len += size;
                    }
                    len = size = 0;
                    while(len < (4096 * i))
                    {
                        size = nwy_file_write(fd, s_w_buf, 4096);
                        if(size <= 0)
                        {
                            nwy_test_cli_echo("\r\nfile write %s fail\r\n", file_name);
                            nwy_file_close(fd);
                            break;
                        }
                        len += size;
                        if((nwy_uptime_get() - op_ts) >= 1000)
                        {
                            nwy_thread_sleep(20);
                            op_ts = nwy_uptime_get();
                        }
                    }
                    nwy_file_close(fd);
                    if(len >= 4096 * i)
                        nwy_test_cli_echo("\r\nfile write %s size %d success\r\n", file_name, len);
                    else
                        break;
                    total += len;
                    if(total >= 1 * 1024 * 1024)
                    {
                        nwy_test_cli_echo("\r\nfile write test total size %d finish\r\n", total);
                        break;
                    }
                }
                i++;
            }
        }
        else if(opt == 3)
        {   // read
            int i = 1;
            int len, size;
            char file_name[64] = {0};
            int total = 0;
            while(i)
            {
                memset(file_name, 0, sizeof(file_name));
                sprintf(file_name, "%s/rw_test_%d", SPI_FLASH_MOUNT_POINT, i);
                int fd = nwy_file_open(file_name, NWY_RDONLY);
                if(fd < 0)
                {
                    nwy_test_cli_echo("\r\nfile open %s fail\r\n", file_name);
                    break;
                }
                else
                {
                    int op_size = strlen(file_name);
                    int64 op_ts = nwy_uptime_get();
                    len = size = 0;
                    while(len < 4096)
                    {
                        size = ((4096 - len) > op_size) ? op_size : (4096 - len);
                        memcpy((char *)s_w_buf + len, file_name, size);
                        len += size;
                    }
                    len = size = 0;
                    while(len < (4096 * i))
                    {
                        memset(s_r_buf, 0, sizeof(s_r_buf));
                        size = nwy_file_read(fd, s_r_buf, sizeof(s_r_buf));
                        if(size <= 0)
                        {
                            nwy_test_cli_echo("\r\nfile read %s fail\r\n", file_name);
                            nwy_file_close(fd);
                            break;
                        }
                        if(memcmp(s_r_buf, s_w_buf, size))
                        {
                            nwy_test_cli_echo("\r\nfile read %s data compare fail, read %s dest %s %d:%d\r\n", file_name, s_r_buf, file_name, len, size);
                            nwy_file_close(fd);
                            break;
                        }
                        len += size;
                        if((nwy_uptime_get() - op_ts) >= 1000)
                        {
                            nwy_thread_sleep(20);
                            op_ts = nwy_uptime_get();
                        }
                    }
                    nwy_file_close(fd);
                    if(len >= (4096 * i))
                        nwy_test_cli_echo("\r\nfile read %s size %d success\r\n", file_name, len);
                    else
                        break;
                    total += len;
                    if(total >= 1 * 1024 * 1024)
                    {
                        nwy_test_cli_echo("\r\nfile read test total size %d finish\r\n", total);
                        break;
                    }
                }
                i++;
            }
        }
        else if(opt == 4)
        {   // free size
            unsigned long long free_size = 0;
            nwy_vfs_free_size_get(SPI_FLASH_MOUNT_POINT, &free_size);
            nwy_test_cli_echo("\r\%s free size:%d\r\n", SPI_FLASH_MOUNT_POINT, (int)free_size);
        }
        else if(opt == 5)
            return;
    }
    nwy_test_cli_echo("\r\n flash test end");

#else
    nwy_test_cli_echo("\r\n flash test not support");
#endif
}

/**************************PM*********************************/
void nwy_test_cli_pm_pwr_off()
{
    char *opt;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nChoose the power off mode(1-quickly,2-normal,3-reset):");
    mode = atoi(opt);
    nwy_pm_ctrl(mode);
}

void nwy_test_cli_pm_save_md()
{
    char *opt;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nChoose the powersave mode(0-WAKEUP,1-ENTER SLEEP):");
    mode = atoi(opt);
    nwy_pm_state_set(mode);
}

void nwy_test_cli_get_boot_cause()
{
    unsigned int causes = nwy_pm_boot_res();
    char string_buf[256];
    memset(string_buf, 0, sizeof(string_buf));
    int idx = sprintf(string_buf, "boot cause[%02x]:", causes);
    if (causes == NWY_BOOTCAUSE_UNKNOWN)
        idx += sprintf(string_buf + idx, " %s", "UNKNOWN");
    else
    {
        if (causes & NWY_BOOTCAUSE_PWRKEY)
            idx += sprintf(string_buf + idx, " %s", "PWRKEY");
        if (causes & NWY_BOOTCAUSE_PIN_RESET)
            idx += sprintf(string_buf + idx, " %s", "PIN_RESET");
        if (causes & NWY_BOOTCAUSE_ALARM)
            idx += sprintf(string_buf + idx, " %s", "ALARM");
        if (causes & NWY_BOOTCAUSE_CHARGE)
            idx += sprintf(string_buf + idx, " %s", "CHARGE");
        if (causes & NWY_BOOTCAUSE_WDG)
            idx += sprintf(string_buf + idx, " %s", "WDG");
        if (causes & NWY_BOOTCAUSE_PIN_WAKEUP)
            idx += sprintf(string_buf + idx, " %s", "PIN_WAKEUP");
        if (causes & NWY_BOOTCAUSE_DOWNLOAD)
            idx += sprintf(string_buf + idx, " %s", "DOWNLOAD");
        if (causes & NWY_BOOTCAUSE_FACTORY)
            idx += sprintf(string_buf + idx, " %s", "FACTORY");
    }
    nwy_test_cli_echo("\r\n%s\r\n", string_buf);
}

#ifndef NWY_OPEN_TEST_PM_POWER_SWITCH_NS
void nwy_test_cli_pm_power_switch()
{
    char *opt;
    uint8_t power_id, mode;
    opt = nwy_test_cli_input_gets("\r\nPlease input sub power id:(0-LCD,1-KEYLED,2-BACK_LIGHT,3-SD,4-CAMA,5-CAMD,6-SIM1)");
    power_id = atoi(opt);

    nwy_pm_power_switch((nwy_pm_id_e)power_id, true, false);
    nwy_test_cli_echo("\r\nSet the power %d open!\r\n", power_id);

    opt = nwy_test_cli_input_gets("\r\nPlease sure to close this sub power, 0:open,1:closed");
    mode = atoi(opt);

    if (mode)
    {
        nwy_pm_power_switch((nwy_pm_id_e)power_id, false, false);
        nwy_test_cli_echo("\r\nSet the power %d closed!\r\n", power_id);
    }
    else
        nwy_test_cli_echo("\r\nSet the power %d open!\r\n", power_id);
}
#endif

#ifndef NWY_OPEN_TEST_PM_POWER_LEVEL_NS
void nwy_test_cli_pm_level_set()
{
    char *opt;
    uint16_t power_id, level;
    opt = nwy_test_cli_input_gets("\r\nPlease input sub power id:(0-LCD,1-KEYLED,2-BACK_LIGHT,3-SD,4-CAMA,5-CAMD,6-SIM1)");
    power_id = atoi(opt);

    opt = nwy_test_cli_input_gets("\r\nPlease set the sub power level you want(mV):");
    level = atoi(opt);

    nwy_pm_level_set((nwy_pm_id_e)power_id, level);
    nwy_test_cli_echo("\r\nSet the power %d in %d mV!\r\n", power_id, level);
}
#endif

/**************************UART*********************************/
#ifdef NWY_OPENC_TEST_UART
static int hd = -1;
static uint8_t uart_mode = NWY_UART_MODE_AT;
static char* name;

void nwy_test_cli_uart_open()
{
    char *opt;
    uint8_t port;
    uint32_t baudrate;
    uint8_t flowctrl;
    char dev_name_tip[128] = {0};
    char dev_name_tmp[64] = {0};
    char dev_name_list[4][8] = {0};
    int dev_num = 0;
    int i = 0;

    dev_num = nwy_dev_name_list_get(NWY_DEV_TYPE_UART,dev_name_list);
    while(i < dev_num) {
        memset(dev_name_tmp, 0, sizeof(dev_name_tmp));
        sprintf(dev_name_tmp, "%d-%s", i + 1, dev_name_list[i]);
        strcat(dev_name_tip, dev_name_tmp);
        i++;
        if (i != dev_num)
            strcat(dev_name_tip, ", ");
    }
#ifdef NWY_EC618_UART_OPEN_TEST
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(1-URT0, 2-URT1 or 3-URT2):");
#else
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(%s):", dev_name_tip);
#endif
    port = atoi(opt);
    if(port < 1 || port > dev_num)
    {
        nwy_test_cli_echo("\r\n Input UART id is invalid!");
        return;
    }
    name = dev_name_list[port-1];
    nwy_test_cli_echo("\r\nTest port = %d!\r\n", port); //delet

//    opt = nwy_test_cli_input_gets("\r\nPlease input the uart mode(0-AT,1-DATA):");
//    uart_mode = atoi(opt);
//    nwy_test_cli_echo("\r\nTest mode = %d!\r\n", uart_mode); //delet
    opt = nwy_test_cli_input_gets("\r\nPlease input the uart baudrate (4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 2000000):");
    baudrate = atoi(opt);
    nwy_test_cli_echo("\r\n Input UART baudrate = %d!\r\n", baudrate);

    opt = nwy_test_cli_input_gets("\r\nPlease input the uart flowctrl(0-FC_NONE, 1-FC_RTSCTS, 2-FC_XONXOFF):");
    flowctrl = atoi(opt);
    nwy_test_cli_echo("\r\n Input UART flowctrl = %d!\r\n", flowctrl);

    hd = nwy_uart_open(name, baudrate, flowctrl);
    if (hd < 0)
        nwy_test_cli_echo("\r\nTest uart error!\r\n"); //delet
    nwy_test_cli_echo("\r\nTest uart success!\r\n");   //delet
}

void nwy_test_cli_uart_read()
{
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }
//  nwy_test_cli_echo("\r\n Please send data in %s!!!\r\n", name);

//    while(!nwy_uart_read(hd, (uint8 *)opt, strlen(opt)));

//  nwy_test_cli_echo("\r\n Read uart data : %s!!!\r\n", opt);
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");

}

void nwy_test_cli_uart_write()
{
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart send data:");

    nwy_uart_write(hd, (uint8 *)opt, strlen(opt));
}

static nwy_uartdcb_t dcb = {
    .baudrate = 115200,
    .databits = 8,
    .stopbits = 1,
    .parity = PB_NONE,
    .flowctrl = FC_NONE
};
void nwy_test_cli_uart_dcb_set()
{
    unsigned int baudrate;
    unsigned int databits;
    unsigned int stopbits;
    nwy_paritybits_e parity;
    nwy_flowctrl_e flowctrl;

    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart param:(b[115200], p[1 2], d[7 8], s[1 2], f[1 2])");

    switch (*opt++)
    {
        case 'b':
            baudrate = atoi(opt);
            if (300 <= baudrate && 2000000 >= baudrate)
            {
                dcb.baudrate = baudrate;
                nwy_uart_dcb_set(hd, &dcb);
                nwy_test_cli_echo("\r\nSwitch baudrate to:%d\r\n", baudrate);
            }
            else
                nwy_test_cli_echo("\r\nTest invalid baudrate:%d!\r\n", baudrate);
            break;
        case 'p':
            parity = atoi(opt);
            if (0 <= parity && 2 >= parity)
            {
                dcb.parity = parity;
                nwy_uart_dcb_set(hd, &dcb);
                nwy_test_cli_echo("\r\nSwitch parity to:%d\r\n", parity);
            }
            else
                nwy_test_cli_echo("\r\nTest invalid parity:%d!\r\n", parity);
            break;

        case 'd':
            databits = atoi(opt);
            if (7 <= databits && 8 >= databits)
            {
                dcb.databits = databits;
                nwy_uart_dcb_set(hd, &dcb);
                nwy_test_cli_echo("\r\nSwitch data_size to:%d\r\n", databits);
            }
            else
                nwy_test_cli_echo("\r\nInvalid data_size:%d\r\n", databits);
            break;

        case 's':
            stopbits = atoi(opt);
            if (1 <= stopbits && 2 >= stopbits)
            {
                dcb.stopbits = stopbits;
                nwy_uart_dcb_set(hd, &dcb);
                nwy_test_cli_echo("\r\nSwitch stop_size to:%d\r\n", stopbits);
            }
            else
                nwy_test_cli_echo("\r\nInvalid stop_size:%d\r\n", stopbits);
            break;

        case 'f':
            flowctrl = atoi(opt);
            if (0 <= flowctrl && 2 >= flowctrl)
            {
                dcb.flowctrl = flowctrl;
                nwy_uart_dcb_set(hd, &dcb);
                nwy_test_cli_echo("\r\nSwitch flow_ctrl to:%d\r\n", flowctrl);
            }
            else
                nwy_test_cli_echo("\r\nInvalid flow_ctrl:%d\r\n", flowctrl);
            break;

        default:
            break;
    }
}

void nwy_test_cli_uart_dcb_get()
{
    nwy_uartdcb_t dcb;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_dcb_get(hd, &dcb);
    nwy_test_cli_echo("\r\nUart baudrate = %d\r\n", dcb.baudrate);
    nwy_test_cli_echo("\r\nUart parity = %d\r\n", dcb.parity);
    nwy_test_cli_echo("\r\nUart databits = %d\r\n", dcb.databits);
    nwy_test_cli_echo("\r\nUart stopbits = %d\r\n", dcb.stopbits);
    nwy_test_cli_echo("\r\nUart flowctrl = %d\r\n", dcb.flowctrl);
}

void nwy_test_cli_uart_close()
{
    int close = 0;
    char *opt = NULL;

    if (hd < 0){
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nSure to close this uart(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
    {
        nwy_uart_close(hd);
        hd = -1;
    }
}

static void nwy_uart_recv_handle(const char *str, unsigned int length)
{
    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_write(hd, (uint8 *)str, length);
    nwy_test_cli_echo("\r\nUart send data length = %d, data=%s\r\n", length, str);
}
#ifdef NWY_EC618_UART_OPEN_TEST
static void nwy_uart_recv0(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, 10);
}
static void nwy_uart_recv1(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, strlen(str));

    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_uartopen_21, P_VALUE,"strlen(str)=%d",strlen(str));
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_uartopen_28, P_VALUE,"strlen(str)=%s",str);
}
#endif

void nwy_test_cli_uart_rx_cb_register()
{
    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }
#ifdef NWY_EC618_UART_OPEN_TEST
    if(hd == 0)
        nwy_uart_rx_cb_register(hd, nwy_uart_recv0);
    if(hd == 1)
        nwy_uart_rx_cb_register(hd, nwy_uart_recv1);
#else
    nwy_uart_rx_cb_register(hd, nwy_uart_recv_handle);
#endif

}

/*if send completly, the callback func will set RS485 as rx state*/
static void nwy_rs485_direction_switch(int port, int value)
{
    nwy_gpio_direction_set(port, PIN_DIRECTION_OUT);
    nwy_gpio_value_set(port, (nwy_value_e)value);
}
static void nwy_uart_send_complet_handle(int param)
{
    nwy_thread_sleep(10);
    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_RX);
    nwy_test_cli_echo("\r\nUart send complet handle success!\r\n");
}

void nwy_test_cli_uart_tx_cb_register()
{
    char *pstsnd = "hellors485";

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_RX);

    /*register cb func to uart drv */
    nwy_uart_tx_cb_register(hd, nwy_uart_send_complet_handle);

    /* for send, set RS485 as tx state */
    nwy_rs485_direction_switch(RS485_GPIO_PORT, RS485_DIR_TX);
    nwy_uart_write(hd, (uint8_t *)pstsnd, strlen(pstsnd));
}

void nwy_test_cli_uart_rx_frame_timeout_set()
{
    int timeout;
    char *opt;

    if(nwy_test_cli_check_uart_mode(uart_mode))
    {
        nwy_test_cli_echo("\r\nUart Current mode is AT Mode!! Please input at by uart port!!\r\n");
        return;
    }

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart receive timeout(default:32ms):");
    timeout = atoi(opt);

    nwy_uart_rx_frame_timeout_set(hd, timeout);
    nwy_test_cli_echo("\r\nSet rx frame timeout = %d\r\n", timeout);
}
#endif

/**************************GPIO*********************************/
#ifdef NWY_OPENC_TEST_GPIO
void nwy_test_cli_gpio_direction_get()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    dir = nwy_gpio_direction_get(port);
    nwy_test_cli_echo("\r\nGet the GPIO dir = %d\r\n", dir);
}

void nwy_test_cli_gpio_direction_set()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO dir(0-input,1-output):");
    dir = atoi(opt);

    nwy_gpio_direction_set(port, (nwy_dir_mode_e)dir);
}

void nwy_test_cli_gpio_value_get()
{
    char *opt;
    uint32_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    vol = nwy_gpio_value_get(port);
    nwy_test_cli_echo("\r\nGet the GPIO value = %d\r\n", vol);
}

void nwy_test_cli_gpio_value_set()
{
    char *opt;
    uint8_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the gpio value(0-low level,1-high level):");
    vol = atoi(opt);

    nwy_gpio_value_set(port, (nwy_value_e)vol);
}


#ifdef NWY_EC618_GPIO_OPEN_TEST
static void nwy_irq_open_cb24()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_10, P_VALUE,"nwy_irq_open_24");
}
static void nwy_irq_open_cb8()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_11, P_VALUE,"nwy_irq_open_8");
}
static void nwy_irq_open_cb9()
{
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER,nwy_GPIOopen_12, P_VALUE,"nwy_irq_open_9");
}
#endif

void nwy_test_cli_gpio_pull_get(void)
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_gpio_pull_set()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(0-pull up,1-pull down):");
    mode = atoi(opt);
    nwy_gpio_pullup_or_pulldown(port, mode);
#else
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(1-pull up,2-pull down):");
    mode = atoi(opt);

    nwy_gpio_pull_set (port, mode);
#endif
}

void nwy_test_cli_gpio_irq_register()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    char *opt;
    uint8_t port, mode, data;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO irq mode(0-disable,1-low,2-high,3-falling,4-rising):");
    mode = atoi(opt);

    if (port == 9)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb9);
    else if (port == 8)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb8);
    else if (port == 24)
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb24);
    else
        data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, _gpioisropen);
#else
    char *opt;
    uint8_t port, pin_edge, pin_pull;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pin edge(1-rising,2-falling,3-both edge,4-high,5-low):");
    pin_edge = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pin pull(1-pullup,2-pulldown):");
    pin_pull = atoi(opt);

    nwy_gpio_close(port);
    int data = nwy_gpio_irq_register (port, pin_edge, pin_pull, _gpioisropen, NULL);
#endif
    if (!data)
    {
        nwy_test_cli_echo("\r\nGpio isr register success!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\nGpio isr register failed!\r\n");
    }
}

void nwy_test_cli_gpio_irq_enable()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    int ret = nwy_gpio_irq_enable(port);

    if(0 == ret)
        nwy_test_cli_echo("\r\nGpio enable isr success!\r\n");
    else
        nwy_test_cli_echo("\r\nGpio enable isr fail!\r\n");

}

void nwy_test_cli_gpio_irq_disable()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    int ret = nwy_gpio_irq_disable(port);
    if(0 == ret)
        nwy_test_cli_echo("\r\nGpio disenable isr success!\r\n");
    else
        nwy_test_cli_echo("\r\nGpio disenable isr fail!\r\n");
}

void nwy_test_cli_gpio_irq_wakeup_enable(void)
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_gpio_irq_wakeup_disable(void)
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_gpio_close()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    int ret = nwy_gpio_close(port);
    if(0 == ret)
        nwy_test_cli_echo("\r\nGpio close success!\r\n");
    else
        nwy_test_cli_echo("\r\nGpio close fail!\r\n");

#endif
}
#endif
/**************************SPI*********************************/
#ifdef NWY_OPENC_TEST_SPI
int spi_bus = -1;

void nwy_test_cli_spi_init()
{
    char *opt;
    uint8_t port;
    char *name;
    opt = nwy_test_cli_input_gets("\r\nPlease input the SPI id(1-spi1,2-spi2,3-spi3):");
    port = atoi(opt);

    if (port == 1)
    {
        name = NAME_SPI_BUS_1;
    }
    else if (port == 2)
    {
        name = NAME_SPI_BUS_2;
    }
    else
    {
        nwy_test_cli_echo("\r\n Input SPI id is invalid!");
        return;
    }

    spi_bus = nwy_spi_init(name, SPI_MODE_0, 1000000, 8);
    if (NWY_SUCCESS > spi_bus)
    {
        nwy_test_cli_echo("\r\nSPI Error : bus:%s init fail\r\n", name);
    }
    else
    {
        nwy_test_cli_echo("\r\nSPI : bus:%s init success\r\n", name);
    }
    return;
}

void nwy_test_cli_spi_transfer()
{
    uint8_t OSI_ALIGNED(16) Command[4] = {0x9f, 0xFF, 0xFF, 0xFF};
    uint8_t OSI_ALIGNED(16) FlashId[4] = {0};
    int rtn = nwy_spi_transfer(spi_bus, SPI_CS_0, Command, FlashId, 4);

    if (NWY_SUCCESS == rtn)
        nwy_test_cli_echo("\r\nSpi flash read id %02x,%02x,%02x\r\n", FlashId[1], FlashId[2], FlashId[3]);
    else
        nwy_test_cli_echo("\r\nSpi error:transfer fail!\r\n");
}

void nwy_test_cli_spi_deinit()
{
    int close;
    char *opt;
    opt = nwy_test_cli_input_gets("\r\nSure to close this spi(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
    {
        nwy_spi_deinit(spi_bus);
        spi_bus = -1;
    }
}
#endif
/**************************I2C*********************************/
#ifdef NWY_OPENC_TEST_I2C
int i2c_bus;
void nwy_test_cli_i2c_init()
{
#ifdef NWY_EC618_I2C_OPEN_TEST
    char *opt;
    uint8_t port;
    char *name;
    opt = nwy_test_cli_input_gets("\r\nPlease input the I2C id(0-I2C0,1-I2C1 ):");
    port = atoi(opt);
    if (port == 0)
    {
        name = NAME_I2C_BUS_0;
    }
    else if (port == 1)
    {
        name = NAME_I2C_BUS_1;
    }
    else
    {
        nwy_test_cli_echo("\r\n Input I2C id is invalid!");
        return;
    }

    i2c_bus = nwy_i2c_init(name, NWY_I2C_BPS_400K);
    if (NWY_SUCESS > i2c_bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", name);
        return;
    }
#else
    char *opt;
    uint8_t port;
    char *name;
    int bps;
    opt = nwy_test_cli_input_gets("\r\nPlease input the I2C id(1-I2C1,2-I2C2 or 3-I2C3):");
    port = atoi(opt);
    if (port == 1)
    {
        name = NAME_I2C_BUS_1;
    }
    else if (port == 2)
    {
        name = NAME_I2C_BUS_2;
    }
    else if (port == 3)
    {
        name = NAME_I2C_BUS_3;
    }
    else
    {
        nwy_test_cli_echo("\r\n Input I2C id is invalid!");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nPlease input the I2C freq(0-100Kbps,1-400Kbps or 2-3.5Mbps):");
    bps = atoi(opt);
    if(2 < bps)
    {
        nwy_test_cli_echo("\r\n Input I2C freq is invalid!");
        return;
    }

    i2c_bus = nwy_i2c_init(name, (nwy_i2c_mode_e)bps);
    if (0 > i2c_bus)
    {
        nwy_test_cli_echo("\r\nI2c Error : bus:%s init fail\r\n", name);
    }
    else
        nwy_test_cli_echo("\r\nI2c : bus:%s init success\r\n", name);
    return;
#endif
}

#define ES8311_DEV_ADDR 0x18
#define BMA400_DEV_ADDR 0x14
void nwy_test_cli_i2c_read()
{
    int rtn, dev_id;
#if defined(NWY_EC618_I2C_OPEN_TEST) || defined(NWY_OPENC_TEST_I2C_DEV_ES8311)
    rtn = nwy_i2c_read(i2c_bus, ES8311_DEV_ADDR, 0xfd, &dev_id, 2);
#else
    rtn = nwy_i2c_read(i2c_bus, BMA400_DEV_ADDR, 0x00, &dev_id, 1);
#endif
    if (NWY_SUCCESS == rtn)
        nwy_test_cli_echo("\r\nNWY get device id = 0x%x!\r\n", dev_id);
    else
        nwy_test_cli_echo("\r\nNWY read I2C error!\r\n");

}

void nwy_test_cli_i2c_write()
{
    uint8_t temp = 0x1c;
    uint8_t *data = &temp;
    uint8_t rtn;
    rtn = nwy_i2c_write(i2c_bus, BMA400_DEV_ADDR, 0xf4, data, 1);
    if (NWY_SUCCESS == rtn)
        nwy_test_cli_echo("\r\nNWY write I2C success!\r\n");
    else
        nwy_test_cli_echo("\r\nNWY write I2C error!\r\n");
}

void nwy_test_cli_i2c_deinit()
{
    int close;
    char *opt;
    opt = nwy_test_cli_input_gets("\r\nSure to close this i2c(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
    {
        int ret = nwy_i2c_deinit(i2c_bus);
        if (NWY_SUCCESS == ret)
            nwy_test_cli_echo("\r\nNWY close I2C success!\r\n");
        else
            nwy_test_cli_echo("\r\nNWY close I2C error!\r\n");
    }

}
#endif
/**************************ADC*********************************/
#ifdef NWY_OPENC_TEST_ADC
void nwy_test_cli_adc_get()
{
    char *opt;
    uint8_t port, mode;
    uint32_t adc_vol;
    char dev_name_tip[128] = {0};
    char dev_name_tmp[64] = {0};
    char dev_name_list[4][8] = {0};
    int dev_num = 0;
    int i = 0;

    dev_num = nwy_dev_name_list_get(NWY_DEV_TYPE_ADC,dev_name_list);
    while(i < dev_num) {
        memset(dev_name_tmp, 0, sizeof(dev_name_tmp));
        sprintf(dev_name_tmp, "%d-%s", i + 1, dev_name_list[i]);
        strcat(dev_name_tip, dev_name_tmp); 
        i++;
        if (i != dev_num)
            strcat(dev_name_tip, ", ");
    }

    opt = nwy_test_cli_input_gets("\r\nChoose the ADC channel(%s):",dev_name_tip);
    port = atoi(opt);
    if ((port < 1) || (port > dev_num)) {
        nwy_test_cli_echo("\r\nInput ADC channel is invalid!\r\n");
        return;
    }
    opt = nwy_test_cli_input_gets("\r\nChoose the ADC scale(0-1V250,1-2V444,2-3V233,3-5V000):");
    mode = atoi(opt);
    if (mode > 3) {
        nwy_test_cli_echo("\r\nInput ADC scale is invalid!\r\n");
        return;
    }

    adc_vol = nwy_adc_get((nwy_adc_e)port, (nwy_adc_aux_scale_e)mode);
    nwy_test_cli_echo("\r\nAdc get value = %d\r\n", adc_vol);
}
#endif

#ifdef NWY_OPEN_TEST_ALARM
int nwy_bootup_alarm_set(uint32_t sec_in_day);
int nwy_bootup_alarm_del(void);
void nwy_test_cli_boot_alarm_set()
{
    char *opt;
    uint32_t after_now, sec;
    int ret;

    opt = nwy_test_cli_input_gets("\r\nPlease select the boot alarm seconds in day [0] or after now [1]:");
    after_now = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nPlease input the boot alarm seconds:");
    sec = atoi(opt);

    if(after_now)
    {
        nwy_timeval_t now_time;
        nwy_sys_timestamp_get(&now_time);
        sec += now_time.tv_sec % 24 * 3600;
    }

    ret = nwy_bootup_alarm_set(sec);
    if (1 == ret)
        nwy_test_cli_echo("\r\nboot alarm set at %d seconds in day success!\r\n", sec);
    else
        nwy_test_cli_echo("\r\nboot alarm set at seconds in day fail!\r\n");
}

void nwy_test_cli_boot_alarm_del()
{
    int ret;

    ret = nwy_bootup_alarm_del();
    if (1 == ret)
        nwy_test_cli_echo("\r\nboot alarm delete success!\r\n");
    else
        nwy_test_cli_echo("\r\nboot alarm delete fail!\r\n");
}
#endif

#ifdef NWY_OPEN_TEST_POWER_STATE
void nwy_test_cli_power_state()
{
    int ret;

    ret = nwy_power_state();
    if (1 == ret)
        nwy_test_cli_echo("\r\npower state normal!\r\n");
    else
        nwy_test_cli_echo("\r\npower state drop!\r\n");
}

void nwy_test_cli_power_voltage()
{
    int vol = 0;
    int ret;

    ret = nwy_pm_vbat_voltage_get(&vol);
    if (ret == NWY_SUCCESS)
        nwy_test_cli_echo("\r\npower power vbat voltage=%dmV!\r\n", vol);
    else
        nwy_test_cli_echo("\r\npower power vbat voltage get fail!\r\n");
}
#endif

#ifdef NWY_OPEN_TEST_USB_NET_SWITCH
void nwy_test_cli_usb_net_switch(void)
{
    char *opt;
    uint32_t net_type;
    int ret;

    opt = nwy_test_cli_input_gets("\r\nPlease select usb net type: [0]rndis [1]ecm");
    net_type = atoi(opt);
    if(net_type >= NWY_USB_NET_MODE_NONE)
    {
        nwy_test_cli_echo("\r\nusb net type %d not support!\r\n", net_type);
        return;
    }
    ret = nwy_usb_net_mode_set((nwy_usb_net_mode_e)net_type);
    if (NWY_SUCCESS == ret)
        nwy_test_cli_echo("\r\nusb net type switch to %d success!\r\n", net_type);
    else
        nwy_test_cli_echo("\r\nusb net type switch to %d fail!\r\n", net_type);
}

void nwy_test_cli_usb_net_get(void)
{
    nwy_usb_net_mode_e net_type;
    int ret;

    ret = nwy_usb_net_mode_get(&net_type);
    if (NWY_SUCCESS == ret)
        nwy_test_cli_echo("\r\nusb net type [0]rndis [1]ecm :%d!\r\n", net_type);
    else
        nwy_test_cli_echo("\r\nusb net type get fail!\r\n");
}
#endif

