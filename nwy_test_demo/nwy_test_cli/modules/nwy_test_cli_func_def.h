/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_test_cli_adpt.h
 * Author       : hujun
 * Created      : 2023-5-22
 * Description  : test������ API ��������
 *
 *****************************************************************************
 */
#ifndef __NWY_TEST_CLI_FUNC_DEF_H__
#define __NWY_TEST_CLI_FUNC_DEF_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */

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
* Prototype     : nwy_test_cli_start_timer
* Description   : timer start������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_start_timer(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_stop_timer
* Description   : timer stop������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_stop_timer(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_time
* Description   : ��ȡϵͳʱ�������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_time(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_set_time
* Description   : ����ϵͳʱ�������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_set_time(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_test_semp
* Description   : �ź���������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_test_semp(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_model
* Description   : ��ȡ�汾��Ϣ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_model();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_imei
* Description   : ��ȡimei��Ϣ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_imei();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_set_app_version
* Description   : ����app�汾�Ų�����
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_set_app_version();

void nwy_test_cli_get_open_sdk_ver_string(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_sw_ver
* Description   : ��ȡ�����汾������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_sw_ver();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_set_app_version
* Description   : ��ȡӲ���汾������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_hw_ver();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_heap_info
* Description   : ��ȡϵͳheap infor������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_heap_info();

/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_cpu_temp
* Description   : ��ȡcpu�¶Ȳ�����
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_get_cpu_temp();
/*
*****************************************************************************
* Prototype     : nwy_test_cli_thread_priority_test
* Description   : Test set & get thread priority.
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Lu Chenyang
*****************************************************************************
*/
void nwy_test_cli_thread_priority_test(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_event_wait_performance
* Description   : Test send & receive thread event.
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Lu Chenyang
*****************************************************************************
*/
void nwy_test_cli_event_wait_performance(void);

void nwy_test_cli_date_timestamp(void);

/**************************FS*********************************/
/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_open
* Description   : file open������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_open(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_open
* Description   : file д�������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_write(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_open
* Description   : �ļ���ȡ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_read(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_open
* Description   : fs size��ȡ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_fsize(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_ftell
* Description   : file tell
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Lu Chenyang
*****************************************************************************
*/
void nwy_test_cli_fs_ftell(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_seek
* Description   : file seek
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_seek(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_sync
* Description   : fs ͬ��������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_sync(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_fstate
* Description   : file stat��ȡ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_fstate(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_trunc
* Description   : file ��ȡ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_trunc(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_close
* Description   : file close������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_close(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_remove
* Description   : file ɾ��������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_remove(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_rename
* Description   : fs ������������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_rename(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_open
* Description   : dir open������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_open(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_read
* Description   : read dir ������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_read(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_tell
* Description   : tell dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_tell(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_seek
* Description   : seek dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_seek(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_rewind
* Description   : rewind dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_rewind(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_close
* Description   : �ر�dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_close(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_mk
* Description   : ����dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_mk(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_dir_remove
* Description   : ɾ��dir������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_dir_remove(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_fs_free_size
* Description   : ��ȡ�ļ�ϵͳfree size������
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
void nwy_test_cli_fs_free_size(void);

/*
*****************************************************************************
* Prototype     : nwy_test_cli_spi_flash_mount
* Description   : ext spi flash mount
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : zhaoyong
*****************************************************************************
*/
void nwy_test_cli_spi_flash_mount(void);

/**************************PM*********************************/
/*
*****************************************************************************
* Prototype     : nwy_test_cli_pm_pwr_off
* Description   : control module power off or reset
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Charlie
*****************************************************************************
*/
void nwy_test_cli_pm_pwr_off(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_pm_save_md
* Description   : control module sleep or wakeup
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Charlie
*****************************************************************************
*/
void nwy_test_cli_pm_save_md(void);
/*
*****************************************************************************
* Prototype     : nwy_test_cli_get_boot_cause
* Description   : get module boot reason
* Input         : NA
* Output        : NA
* Return Value  : NA
* Author        : Charlie
*****************************************************************************
*/
void nwy_test_cli_get_boot_cause(void);
void nwy_test_cli_pm_power_switch();
void nwy_test_cli_pm_level_set();
void nwy_test_cli_power_state();
void nwy_test_cli_power_voltage();

/**************************IO*********************************/

/**************************UART*********************************/
void nwy_test_cli_uart_open(void);
void nwy_test_cli_uart_read(void);
void nwy_test_cli_uart_write(void);
void nwy_test_cli_uart_dcb_set(void);
void nwy_test_cli_uart_dcb_get(void);
void nwy_test_cli_uart_close(void);
void nwy_test_cli_uart_rx_cb_register(void);
void nwy_test_cli_uart_tx_cb_register(void);
void nwy_test_cli_uart_rx_frame_timeout_set(void);
/**************************GPIO*********************************/
void nwy_test_cli_gpio_direction_get(void);
void nwy_test_cli_gpio_direction_set(void);
void nwy_test_cli_gpio_value_get(void);
void nwy_test_cli_gpio_value_set(void);
void nwy_test_cli_gpio_pull_get(void);
void nwy_test_cli_gpio_pull_set(void);
void nwy_test_cli_gpio_irq_register(void);
void nwy_test_cli_gpio_irq_enable(void);
void nwy_test_cli_gpio_irq_disable(void);
void nwy_test_cli_gpio_irq_wakeup_enable(void);
void nwy_test_cli_gpio_irq_wakeup_disable(void);
void nwy_test_cli_gpio_close(void);
/**************************SPI*********************************/
void nwy_test_cli_spi_init(void);
void nwy_test_cli_spi_transfer(void);
void nwy_test_cli_spi_deinit(void);
/**************************I2C*********************************/
void nwy_test_cli_i2c_init(void);
void nwy_test_cli_i2c_read(void);
void nwy_test_cli_i2c_write(void);
void nwy_test_cli_i2c_deinit(void);
/**************************ADC*********************************/
void nwy_test_cli_adc_get(void);

/**************************ALARM*********************************/
void nwy_test_cli_boot_alarm_set(void);
void nwy_test_cli_boot_alarm_del(void);

/**************************TCP*********************************/
void nwy_test_cli_tcp_setup(void);
void nwy_test_cli_tcp_send(void);
void nwy_test_cli_tcp_close(void);
void nwy_test_cli_ping(void);
void nwy_test_cli_ip_packet(void);

/**************************TCP server*********************************/
void nwy_test_cli_tcp_server(void);


/**************************UDP*********************************/
void nwy_test_cli_udp_setup(void);
void nwy_test_cli_udp_send(void);
void nwy_test_cli_udp_close(void);

/**************************TCP EX*********************************/
void nwy_test_cli_tcp_setup_ex(void);
void nwy_test_cli_tcp_send_ex(void);
void nwy_test_cli_tcp_close_ex(void);

/**************************TCP server EX*********************************/
void nwy_test_cli_tcp_server_ex(void);


/**************************UDP EX*********************************/
void nwy_test_cli_udp_setup_ex(void);
void nwy_test_cli_udp_send_ex(void);
void nwy_test_cli_udp_close_ex(void);


/**************************FTP*********************************/
void nwy_test_cli_ftp_param(void);
void nwy_test_cli_ftp_login(void);
void nwy_test_cli_ftp_get(void);
void nwy_test_cli_ftp_put(void);
void nwy_test_cli_ftp_filesize(void);
void nwy_test_cli_ftp_list(void);
void nwy_test_cli_ftp_mkdir(void);
void nwy_test_cli_ftp_rmdir(void);
void nwy_test_cli_ftp_delete(void);
void nwy_test_cli_ftp_logout(void);
/**************************HTTP*********************************/
void nwy_test_cli_http_setup(void);
void nwy_test_cli_http_get(void);
void nwy_test_cli_http_head(void);
void nwy_test_cli_http_post(void);
void nwy_test_cli_http_put(void);
void nwy_test_cli_http_open_mode(void);
void nwy_test_cli_http_close(void);

void nwy_test_cli_sntp_get_time(void);

#ifdef NWY_TEST_MBEDTLS_AES_SUPPORT
void nwy_mbedtls_aes_test(void);
#endif

void nwy_test_cli_ssltcp_init(void);
void nwy_test_cli_ssltcp_setup(void);
void nwy_test_cli_ssltcp_send(void);
void nwy_test_cli_ssltcp_close(void);
void nwy_test_cli_get_sslinfo(void);

/**************************virtual at*********************************/
void nwy_test_cli_virt_at_init(void);
void nwy_test_cli_reg_at_unsolicited(void);
void nwy_test_cli_send_virt_at(void);
void nwy_test_cli_reg_at_fwd(void);

/***************************Network*******************************************/
void nwy_test_cli_nw_get_mode(void);
void nwy_test_cli_nw_set_mode(void);
void nwy_test_cli_nw_set_lock(void);
void nwy_test_cli_nw_get_lock(void);
void nwy_test_cli_nw_get_priband(void);
void nwy_test_cli_nw_set_priband(void);
void nwy_test_cli_nw_get_ims_mode(void);
void nwy_test_cli_nw_set_ims_mode(void);
void nwy_test_cli_nw_get_radio_mode(void);
void nwy_test_cli_nw_set_radio_mode(void);
void nwy_test_cli_nw_get_edrx_info(void);
void nwy_test_cli_nw_set_edrx_info(void);
void nwy_test_cli_nw_get_psm_info(void);
void nwy_test_cli_nw_set_psm_info(void);
void nwy_test_cli_nw_get_custom_cfg(void);
void nwy_test_cli_nw_set_custom_cfg(void);
void nwy_test_cli_nw_get_oper_info(void);
void nwy_test_cli_nw_get_csq(void);
void nwy_test_cli_nw_get_signal_info(void);
void nwy_test_cli_nw_reg_info(void);
void nwy_test_cli_nw_manual_scan(void);
void nwy_test_cli_nw_get_cellinfo(void);
void nwy_test_cli_nw_manual_scan(void);
void nwy_test_cli_nw_get_none_eitf(void);
void nwy_test_cli_nw_set_none_eitf(void);
void nwy_test_cli_nw_get_fplmn_list(void);
void nwy_test_cli_nw_network_ind(void);
void nwy_test_cli_get_time_zone_switch(void);
void nwy_test_cli_set_time_zone_switch(void); 

/***************************SIM*******************************************/
void nwy_test_cli_get_sim_status(void);
void nwy_test_cli_verify_pin(void);
void nwy_test_cli_get_pin_mode(void);
void nwy_test_cli_set_pin_mode(void);
void nwy_test_cli_change_pin(void);
void nwy_test_cli_verify_puk(void);
void nwy_test_cli_get_imsi(void);
void nwy_test_cli_get_iccid(void);
void nwy_test_cli_get_msisdn(void);
void nwy_test_cli_set_msisdn(void);
void nwy_test_cli_get_pin_puk_times(void);
void nwy_test_cli_get_sim_slot(void);
void nwy_test_cli_set_sim_slot(void);
void nwy_test_cli_set_csim(void);
void nwy_poc_pocds_test();
void nwy_test_cli_set_sim_detect();
void nwy_test_cli_set_sim_power_mode();
void nwy_test_cli_sim_recv_cb(nwy_sim_id_e sim_id, nwy_sim_urc_type_e urc_type);
void nwy_test_cli_sim_reg(void);
/***************************SMS*******************************************/
void nwy_test_cli_sms_init();
void nwy_test_cli_sms_send(void);
void nwy_test_cli_sms_del(void);
void nwy_test_cli_sms_get_sca(void);
void nwy_test_cli_sms_set_sca(void);
void nwy_test_cli_sms_set_storage(void);
void nwy_test_cli_sms_get_storage(void);
void nwy_test_cli_sms_set_report_md(void);
void nwy_test_cli_sms_read(void);
void nwy_test_cli_sms_list(void);
void nwy_test_cli_sms_del_type(void);
void nwy_test_cli_sms_reg(void);
/***************************SIM*******************************************/
void nwy_test_cli_get_sim_status();
void nwy_test_cli_verify_pin();
void nwy_test_cli_get_pin_mode();
void nwy_test_cli_set_pin_mode();
void nwy_test_cli_change_pin();
void nwy_test_cli_verify_puk();
void nwy_test_cli_get_imsi();
void nwy_test_cli_get_iccid();
void nwy_test_cli_get_iccid_new();
void nwy_test_cli_get_msisdn();
void nwy_test_cli_set_msisdn();
void nwy_test_cli_get_pin_puk_times();
void nwy_test_cli_get_sim_slot();
void nwy_test_cli_set_sim_slot();
void nwy_test_cli_set_csim();
/********************************Location************************************/
void nwy_test_cli_navigation_start();
void nwy_test_cli_navigation_stop();
void nwy_test_cli_loc_mode_set();
void nwy_test_cli_startup_mode_set();
void nwy_test_cli_loc_rate_set();
void nwy_test_cli_nmea_data_get();
void nwy_test_cli_nmea_data_report_reg();
void nwy_test_cli_nmea_data_parse();
void nwy_test_cli_agnss_start();

/***************************virtual AT*******************************************/
void nwy_test_cli_send_virt_at(void);
void nwy_test_cli_reg_at_fwd(void);
/**************************************************************************/
void nwy_test_cli_pwm_init();
void nwy_test_cli_pwm_start();
void nwy_test_cli_pwm_stop();
void nwy_test_cli_pwm_deinit();
/***************************DATA*******************************************/
void nwy_test_cli_get_profile();
void nwy_test_cli_set_profile();
void nwy_test_cli_auto_connect_set();
void nwy_test_cli_data_start();
void nwy_test_cli_data_info();
void nwy_test_cli_data_stop();
void nwy_test_cli_get_dhcp_info();
void nwy_test_cli_set_dhcp_info();
void nwy_test_cli_get_folw_info();
void nwy_test_cli_get_eps_apn();
void nwy_test_cli_get_dns_info();

/***************************FOTA*******************************************/
void nwy_test_cli_fota_local_update(void);
void nwy_test_cli_ftp_fota_update();
void nwy_test_cli_fota_result(void);
void nwy_test_cli_fota_base_update(void);
/***************************USB*******************************************/
void nwy_test_cli_usb_net_switch(void);
void nwy_test_cli_usb_net_get(void);
/***************************WIFI*******************************************/
void nwy_cli_test_wifi_get_st();
void nwy_cli_test_wifi_enable();
void nwy_cli_test_wifi_set_work_md();
void nwy_cli_test_wifi_set_ap_para();
void nwy_cli_test_wifi_set_ap_para_adv();
void nwy_cli_test_wifi_get_clit_info();
void nwy_cli_test_wifi_sta_scan();
void nwy_cli_test_wifi_sta_scan_ret();
void nwy_cli_test_wifi_sta_connt();
void nwy_cli_test_wifi_sta_disconnt();
void nwy_cli_test_wifi_sta_get_hostpot_info();
void nwy_cli_test_wifi_disable();

/***************************LBS*******************************************/

/***************************MQTT*******************************************/
#ifdef FEATURE_NWY_PAHO_MQTT_V3
void nwy_test_cli_mqtt_connect();
void nwy_test_cli_mqtt_pub();
void nwy_test_cli_mqtt_sub();
void nwy_test_cli_mqtt_unsub();
void nwy_test_cli_mqtt_state();
void nwy_test_cli_mqtt_disconnect();
#endif
/***************************MQTT*******************************************/

void nwy_test_cli_gnss_open_base();
void nwy_test_cli_wifi_open_base();
/***************************STK*******************************************/

void nwy_test_cli_recv_info_stk();
void nwy_test_cli_set_switch_stk();
void nwy_test_cli_get_switch_stk();
void nwy_test_cli_terminal_response_stk();
void nwy_test_cli_envelope_command_stk();
void nwy_test_cli_set_profile_stk();
void nwy_test_cli_get_profile_stk();

void nwy_test_cli_start_isr_timer(void);

void nwy_test_cli_start_service_timer(void);

void nwy_test_cli_start_user_timer(void);


void nwy_test_cli_start_all_timers(void);


void nwy_test_cli_stop_all_timers(void);

#endif
