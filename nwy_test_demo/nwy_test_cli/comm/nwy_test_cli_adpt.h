/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_test_cli_adpt.h
 * Author       : hujun
 * Created      : 2023-5-22
 * Description  : nwy_test_cli_adpt API 函数声明
 *
 *****************************************************************************
 */

#ifndef __NWY_TEST_CLI_ADPT_H__
#define __NWY_TEST_CLI_ADPT_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_osi_api.h"
#include "nwy_usb_serial.h"
#include "nwy_file_api.h"
#include "nwy_socket_api.h"
#include "nwy_sim_api.h"
#include "nwy_sms_api.h"
#include "nwy_loc_api.h"
#include "nwy_vir_at_api.h"
#include "nwy_data_api.h"
#include "nwy_uart_api.h"
#include "nwy_gpio_api.h"
#include "nwy_pm_api.h"
#include "nwy_pwm_api.h"
#include "nwy_network_api.h"
#include "nwy_dm_api.h"
#include "nwy_usb_api.h"
#include "nwy_wifi_api.h"
#include "nwy_ping_api.h"
#include "nwy_dev_api.h"
#ifndef FEATURE_NWY_ASR_TEST_TMP
#include "nwy_network_api.h"
#include "nwy_sms_api.h"
#include "nwy_pm_api.h"
#include "nwy_vir_at_api.h"
#include "nwy_uart_api.h"
#include "nwy_gpio_api.h"
#include "nwy_spi_api.h"
#include "nwy_i2c_api.h"
#include "nwy_adc_api.h"
#include "nwy_ftp_api.h"
#include "nwy_sntp_api.h"
#endif
#include "nwy_sat_api.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 ********************************************   *********************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_OPENC_TEST_UART
#ifdef FEATURE_NWY_ASR_PLAT
#define NWY_OPEN_TEST_PWM
#endif
#define NWY_OPENC_TEST_GPIO
//#define NWY_OPENC_TEST_I2C
//#define NWY_OPENC_TEST_SPI
#define NWY_OPENC_TEST_ADC
#define NWY_TEST_CLI_NOT_SUPPORT      0

#define RS485_GPIO_PORT (2)
#define RS485_DIR_TX (1) //hight level for send
#define RS485_DIR_RX (0) //low level for recv

#define NWY_OPEN_MAX_SPI_CHANNEL  2
#define NWY_OPEN_MAX_UART_CHANNEL 3
#define NWY_OPEN_UART_CHANNEL_NS  0
#define NWY_OPEN_TEST_ALARM
#define NWY_OPEN_TEST_POWER_STATE
#define NWY_OPEN_TEST_PERIHERALS
#define NWY_OPEN_TEST_USB
#define NWY_OPEN_TEST_USB_NET_SWITCH
#define NWY_OPENC_TEST_I2C
#define FEATURE_NWY_PAHO_MQTT_V3
#define NWY_OPENC_TEST_I2C_DEV_ES8311
#define NWY_OPENC_TEST_SPI

#define NWY_OPEN_TEST_PM_POWER_SWITCH_NS
#define NWY_OPEN_TEST_PM_POWER_LEVEL_NS
//#define NWY_OPEN_TEST_UART_NS
//#define NWY_OPEN_TEST_GPIO_NS
#define NWY_OPEN_TEST_PWM_NS
#define NWY_OPENC_TEST_GNSS
//#define NWY_OPEN_TEST_LBS
//#define NWY_OPEN_TEST_WIFI
#define NWY_OPEN_TEST_SMS
#define NWY_OPEN_TEST_HTTP
#define NWY_OPEN_TEST_FTP
#define NWY_OPEN_TEST_MBEDTLS
#define NWY_OPEN_TEST_SPI_FLASH

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************
 */




/*
 *****************************************************************************
 * 5 STRUCT Type Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 6 UNION Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */


/*
*****************************************************************************
* Prototype     : nwy_test_cli_dbg
* Description   : nwy_test_cli打印log信息
* Input         : func：调用函数
                  line：行数
                  fmt：log格式化
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dbg(const char *func, int line, char* fmt, ... );

/*
*****************************************************************************
* Prototype     : nwy_test_cli_wait_select
* Description   : 系统等等客户输入信息处理函数
* Input         : NA
* Output        : NA
* Return Value  : 1：收到输入时间
* Author        : hujun
*****************************************************************************
*/
int nwy_test_cli_wait_select();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_select_enter
* Description   : 发送输入信息事件
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_select_enter();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_select_enter
* Description   : 发送传输信息完毕事件
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_send_trans_end(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_select_enter
* Description   : 等待接收传输信息完毕事件
* Input         : NA
* Output        : NA
* Return Value  : 1：收到输入时间
* Author        : hujun
*****************************************************************************
*/
int nwy_test_cli_wait_trans_end(void);

int nwy_test_cli_check_uart_mode(uint8_t mode);
void _gpioisropen(void *param);
void nwy_test_cli_get_version(void);

#endif
