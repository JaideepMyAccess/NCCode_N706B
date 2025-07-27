#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_func_def.h"

#define TEST_CLI_MENU_STACK_MAX 5
typedef void (*item_proc_func)();

struct item
{
    const char *name_str;
    struct item *sub_menu;
    item_proc_func func;
};

typedef struct item cli_menu_item;

/*******************************************************************************
                        1. Mobile Network
********************************************************************************/

static cli_menu_item nw_menu_arry[] =
    {
        {" get network mode", NULL, nwy_test_cli_nw_get_mode},
        {" set network mode", NULL, nwy_test_cli_nw_set_mode},
        {" get lock info", NULL, nwy_test_cli_nw_get_lock},
        {" set lock info", NULL, nwy_test_cli_nw_set_lock},
        {" get ims status", NULL, nwy_test_cli_nw_get_ims_mode},
        {" set ims status", NULL, nwy_test_cli_nw_set_ims_mode},
        {" get radio mode", NULL, nwy_test_cli_nw_get_radio_mode},
        {" set radio mode", NULL, nwy_test_cli_nw_set_radio_mode},
        {" get edrx info", NULL, nwy_test_cli_nw_get_edrx_info},
        {" set edrx info", NULL, nwy_test_cli_nw_set_edrx_info},
        {" get customer feature config", NULL, nwy_test_cli_nw_get_custom_cfg},
        {" set customer feature config", NULL, nwy_test_cli_nw_set_custom_cfg},
        {" get operator info", NULL, nwy_test_cli_nw_get_oper_info},
        {" get csq", NULL, nwy_test_cli_nw_get_csq},
        {" get signal info", NULL, nwy_test_cli_nw_get_signal_info},
        {" get reg info", NULL, nwy_test_cli_nw_reg_info},
        {" Get cell info", NULL, nwy_test_cli_nw_get_cellinfo},
        {" manul scan and select net", NULL, nwy_test_cli_nw_manual_scan},
        {" get high priority band", NULL, nwy_test_cli_nw_get_priband},
        {" set high priority band", NULL, nwy_test_cli_nw_set_priband},
        {" get psm info", NULL, nwy_test_cli_nw_get_psm_info},
        {" set psm info", NULL, nwy_test_cli_nw_set_psm_info},
        {" get none_eitf", NULL, nwy_test_cli_nw_get_none_eitf},
        {" set none_eitf", NULL, nwy_test_cli_nw_set_none_eitf},
        {" get fplmn list", NULL, nwy_test_cli_nw_get_fplmn_list},
        {" Reg or Unreg network ind callback", NULL, nwy_test_cli_nw_network_ind},
        {" Get time zone switch", NULL, nwy_test_cli_get_time_zone_switch},
        {" Set time zone switch", NULL, nwy_test_cli_set_time_zone_switch},
        {NULL, NULL, NULL}};

// 1.Sim
static cli_menu_item sim_menu_arry[] =
    {
        {" Sim status", NULL, nwy_test_cli_get_sim_status},
        {" Verify PIN", NULL, nwy_test_cli_verify_pin},
        {" Get PIN mode", NULL, nwy_test_cli_get_pin_mode},
        {" PIN disable/enable", NULL, nwy_test_cli_set_pin_mode},
        {" Change PIN", NULL, nwy_test_cli_change_pin},
        {" Verify PUK", NULL, nwy_test_cli_verify_puk},
        {" Get IMSI", NULL, nwy_test_cli_get_imsi},
        {" Get ICCID", NULL, nwy_test_cli_get_iccid},
        {" Get MSISDN", NULL, nwy_test_cli_get_msisdn},
        {" Set MSISDN", NULL, nwy_test_cli_set_msisdn},
        {" Get pin puk times", NULL, nwy_test_cli_get_pin_puk_times},
        {" Set CSIM", NULL, nwy_test_cli_set_csim},
#ifndef FEATURE_NWY_ASR_TEST_TMP
        {" Get SIM Slot", NULL, nwy_test_cli_get_sim_slot},
        {" Set SIM Slot", NULL, nwy_test_cli_set_sim_slot},
        {" Set SIM Detect", NULL, nwy_test_cli_set_sim_detect},
#endif
        {" Set SIM power mode", NULL, nwy_test_cli_set_sim_power_mode},
        {" Reg SIM ind", NULL, nwy_test_cli_sim_reg},
        //{"15. Test SIM POCDS", NULL, nwy_poc_pocds_test},
        {NULL, NULL, NULL}};

        //5. SMS
#ifdef NWY_OPEN_TEST_SMS
static cli_menu_item sms_menu_arry[] =
    {
        {" Init SMS", NULL, nwy_test_cli_sms_init},
        {" Send SMS", NULL, nwy_test_cli_sms_send},
        {" Delete SMS", NULL, nwy_test_cli_sms_del},
        {" Get SCA", NULL, nwy_test_cli_sms_get_sca},
        {" Set SCA", NULL, nwy_test_cli_sms_set_sca},
        {" Set storage", NULL, nwy_test_cli_sms_set_storage},
        {" Get storage", NULL, nwy_test_cli_sms_get_storage},
        {" Set report mode", NULL, nwy_test_cli_sms_set_report_md},
        {" Read SMS by index", NULL, nwy_test_cli_sms_read},
        {" Read SMS list", NULL, nwy_test_cli_sms_list},
        {" Delete SMS by type", NULL, nwy_test_cli_sms_del_type},
        {" Reg SMS receive", NULL, nwy_test_cli_sms_reg},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const sms_menu_arry = NULL;
#endif
// 2. Data connection
static cli_menu_item data_menu_arry[] =
    {
        {" Get profile", NULL, nwy_test_cli_get_profile},
        {" Set profile", NULL, nwy_test_cli_set_profile},
        {" Get Eps Apn Info", NULL, nwy_test_cli_get_eps_apn},
        {" Set auto_connect", NULL, nwy_test_cli_auto_connect_set},
        {" Start connection", NULL, nwy_test_cli_data_start},
        {" Get connections info", NULL, nwy_test_cli_data_info},
        {" Stop connection", NULL, nwy_test_cli_data_stop},
        {" Get dhcp info", NULL, nwy_test_cli_get_dhcp_info},
        {" Set dhcp info", NULL, nwy_test_cli_set_dhcp_info},
        {" Get flow info", NULL, nwy_test_cli_get_folw_info},
        {" Get dns info", NULL, nwy_test_cli_get_dns_info},
        {NULL, NULL, NULL}};

//3 Location
#ifdef NWY_OPENC_TEST_GNSS
static cli_menu_item location_menu_arry[] =
    {
        {" Open GNSS", NULL, nwy_test_cli_navigation_start},
        {" Close GNSS", NULL, nwy_test_cli_navigation_stop},
        {" Set Loc Mode", NULL, nwy_test_cli_loc_mode_set},
        {" Set Startup Mode", NULL, nwy_test_cli_startup_mode_set},
        {" Set Loc Rate Type", NULL, nwy_test_cli_loc_rate_set},
        {" Get NMEA data", NULL, nwy_test_cli_nmea_data_get},
        {" Report NMEA data", NULL, nwy_test_cli_nmea_data_report_reg},
        {" NMEA data parse", NULL, nwy_test_cli_nmea_data_parse},
        {" Open AGNSS", NULL, nwy_test_cli_agnss_start},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const location_menu_arry = NULL;
#endif
static cli_menu_item stk_menu_arry[] =
	{
		{" Set stk on/off", NULL, nwy_test_cli_set_switch_stk},
		{" Get stk on/off", NULL, nwy_test_cli_get_switch_stk},
		{" Send stk terminal response", NULL, nwy_test_cli_terminal_response_stk},
		{" Send stk envelope command", NULL, nwy_test_cli_envelope_command_stk},
		{" Set stk profile", NULL, nwy_test_cli_set_profile_stk},
		{" Get stk profile", NULL, nwy_test_cli_get_profile_stk},
		{" Report stk info", NULL, nwy_test_cli_recv_info_stk},
		{NULL, NULL, NULL}};
static cli_menu_item mobile_menu_arry[] =
    {
        {" Network", nw_menu_arry, NULL},
        {" SIM", sim_menu_arry, NULL},

        {" DATA", data_menu_arry, NULL},

        {" Location", location_menu_arry, NULL},
#ifndef FEATURE_NWY_ASR_TEST_TMP
        {" SMS", sms_menu_arry, NULL},

#endif
        {" STK", stk_menu_arry, NULL},
        {NULL, NULL, NULL}};

/*******************************************************************************
						2. Driver & Kernel
********************************************************************************/
//1 Peripherals
//1.1 UART
#ifdef NWY_OPENC_TEST_UART
static cli_menu_item uart_menu_arry[] =
	{
		{" Uart open", NULL, nwy_test_cli_uart_open},
		{" Uart read", NULL, nwy_test_cli_uart_read},
		{" Uart write", NULL, nwy_test_cli_uart_write},
		{" Uart set para", NULL, nwy_test_cli_uart_dcb_set},
		{" Uart get para", NULL, nwy_test_cli_uart_dcb_get},
		{" Uart close", NULL, nwy_test_cli_uart_close},
		{" Uart rx callback register", NULL, nwy_test_cli_uart_rx_cb_register},
		{" Uart tx callback register", NULL, nwy_test_cli_uart_tx_cb_register},
		{" Uart rx frame timeout set", NULL, nwy_test_cli_uart_rx_frame_timeout_set},
		{NULL, NULL, NULL}};
#else
		static cli_menu_item * const uart_menu_arry = NULL;
#endif

//1.2 I2C
#ifdef NWY_OPENC_TEST_I2C
static cli_menu_item i2c_menu_arry[] =
	{
		{" I2C Init", NULL, nwy_test_cli_i2c_init},
		{" I2C Read(reg/mem)", NULL, nwy_test_cli_i2c_read},
		{" I2C Write(reg/mem)", NULL, nwy_test_cli_i2c_write},
		{" I2C Deinit", NULL, nwy_test_cli_i2c_deinit},
		{NULL, NULL, NULL}};
#else
static cli_menu_item * const i2c_menu_arry = NULL;
#endif

//1.3 SPI
#ifdef NWY_OPENC_TEST_SPI
static cli_menu_item spi_menu_arry[] =
	{
		{" SPI Init", NULL, nwy_test_cli_spi_init},
		{" SPI Transfer", NULL, nwy_test_cli_spi_transfer},
		{" SPI Deinit", NULL, nwy_test_cli_spi_deinit},
		{NULL, NULL, NULL}};
#else
static cli_menu_item * const spi_menu_arry = NULL;
#endif

//1.4 GPIO
#ifdef NWY_OPENC_TEST_GPIO
static cli_menu_item gpio_menu_arry[] =
	{
		{" GPIO direction get", NULL, nwy_test_cli_gpio_direction_get},
		{" GPIO direction set", NULL, nwy_test_cli_gpio_direction_set},
		{" GPIO value get", NULL, nwy_test_cli_gpio_value_get},
		{" GPIO value set", NULL, nwy_test_cli_gpio_value_set},
		{" GPIO pull get", NULL, nwy_test_cli_gpio_pull_get},
		{" GPIO pull set", NULL, nwy_test_cli_gpio_pull_set},
		{" GPIO irq register", NULL, nwy_test_cli_gpio_irq_register},
		{" GPIO irq enable", NULL, nwy_test_cli_gpio_irq_enable},
		{" GPIO irq disable", NULL, nwy_test_cli_gpio_irq_disable},
		{" GPIO irq wakeup enable", NULL, nwy_test_cli_gpio_irq_wakeup_enable},
		{" GPIO irq wakeup disable", NULL, nwy_test_cli_gpio_irq_wakeup_disable},
		//{" GPIO close", NULL, nwy_test_cli_gpio_close},
		{NULL, NULL, NULL}};
#else
static cli_menu_item * const gpio_menu_arry = NULL;
#endif
//1.5 ADC (no sub menu)
#ifndef NWY_OPENC_TEST_ADC
#define nwy_test_cli_adc_get NULL
#else
static cli_menu_item adc_menu_arry[] =
    {
        {" ADC voltage get", NULL, nwy_test_cli_adc_get},
        {NULL, NULL, NULL}};
#endif
//1.8 PWM
#if defined (NWY_OPEN_TEST_PWM) || defined (NWY_OPEN_TEST_PWM_EC618)
static cli_menu_item pwm_menu_arry[] =
    {
        {" Init", NULL, nwy_test_cli_pwm_init},
        {" Start", NULL, nwy_test_cli_pwm_start},
        {" Stop", NULL, nwy_test_cli_pwm_stop},
        {" Deinit", NULL, nwy_test_cli_pwm_deinit},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const pwm_menu_arry = NULL;
#endif
#if NWY_TEST_CLI_NOT_SUPPORT

// 3. Network
static cli_menu_item nw_menu_arry[] =
    {
        {" Get network mode", NULL, nwy_test_cli_nw_get_mode},
        {" Set network mode", NULL, nwy_test_cli_nw_set_mode},
        {" Get Forbidden netwrok list", NULL, nwy_test_cli_nw_get_fplmn},
        {" Manual network scan", NULL, nwy_test_cli_nw_manual_scan},
        {" Set network band lock", NULL, nwy_test_cli_nw_band_lock},
        {" Set network freq lock", NULL, nwy_test_cli_nw_freq_lock},
        {" Get network IMS state", NULL, nwy_test_cli_nw_get_ims_st},
        {" Set network IMS state", NULL, nwy_test_cli_nw_set_ims_st},
        {" Get default pdn info", NULL, nwy_test_cli_nw_get_def_pdn},
        {" Set default pdn info", NULL, nwy_test_cli_nw_set_def_pdn},
        {" Get radio state", NULL, nwy_test_cli_nw_get_radio_st},
        {" Set radio state", NULL, nwy_test_cli_nw_set_radio_st},
        {" Get Radio signal", NULL, nwy_test_cli_nw_get_radio_sign},
        {" Get CS reg status", NULL, nwy_test_cli_nw_cs_st},
        {" Get PS reg status", NULL, nwy_test_cli_nw_ps_st},
        {" Get LTE reg status", NULL, nwy_test_cli_nw_lte_st},
        {" Get current operator info", NULL, nwy_test_cli_nw_operator_info},
        {" Get EHPLMN info", NULL, nwy_test_cli_nw_get_ehplmn},
        {" Get signal rssi", NULL, nwy_test_cli_nw_get_signal_rssi},
        {" Get NETMSG info", NULL, nwy_test_cli_nw_get_netmsg},
        {" Get cfgdftpdn info", NULL, nwy_test_cli_nw_get_cfgdftpdn_info},
        {" Set cfgdftpdn info", NULL, nwy_test_cli_nw_set_cfgdftpdn_info},
        {" Get cell info", NULL, nwy_test_cli_nw_get_cellmsg},
        {NULL, NULL, NULL}};

//  Voice call
#ifdef NWY_OPEN_TEST_VOICE
static cli_menu_item voice_menu_arry[] =
    {
        {" Call start", NULL, nwy_test_cli_voice_call_start},
        {" Call end", NULL, nwy_test_cli_voice_call_end},
        {" Auto answer", NULL, nwy_test_cli_voice_auto_answ},
        {" Set VoLTE voice", NULL, nwy_test_cli_voice_volte_set},
        {" Caller id", NULL, nwy_test_cli_voice_caller_id},
        {" Call hold", NULL, nwy_test_cli_voice_call_hold},
        {" Call unhold", NULL, nwy_test_cli_voice_call_unhold},
        {" Call state", NULL, nwy_test_cli_voice_state_change},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const voice_menu_arry = NULL;
#endif



/*******************************************************************************
                        2. Driver & Kernel
********************************************************************************/
//1 Peripherals
//1.1 UART
static cli_menu_item uart_menu_arry[] =
    {
        {" Init", NULL, nwy_test_cli_uart_init},
        {" Set baudrate", NULL, nwy_test_cli_uart_set_baud},
        {" Get baudrate", NULL, nwy_test_cli_uart_get_baud},
        {" Set para", NULL, nwy_test_cli_uart_set_para},
        {" Get para", NULL, nwy_test_cli_uart_get_para},
        {" Set recv frame timeout", NULL, nwy_test_cli_uart_set_tout},
        {" Send data", NULL, nwy_test_cli_uart_send},
        {" Register recv callback", NULL, nwy_test_cli_uart_reg_rx_cb},
        {" Register tx callback", NULL, nwy_test_cli_uart_reg_tx_cb},
        {" Deinit", NULL, nwy_test_cli_uart_deinit},
        {NULL, NULL, NULL}};
//1.2 I2C
#ifdef NWY_OPEN_TEST_I2C
static cli_menu_item i2c_menu_arry[] =
    {
        {" Init", NULL, nwy_test_cli_i2c_init},
        {" Read(reg/mem)", NULL, nwy_test_cli_i2c_read},
        {" Write(reg/mem)", NULL, nwy_test_cli_i2c_write},
        {" Put raw byte", NULL, nwy_test_cli_i2c_put_raw},
        {" Get raw byte", NULL, nwy_test_cli_i2c_get_raw},
        {" Deinit", NULL, nwy_test_cli_i2c_deinit},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const i2c_menu_arry = NULL;
#endif

//1.3 SPI
#ifdef NWY_OPEN_TEST_SPI
static cli_menu_item spi_menu_arry[] =
    {
        {" Init", NULL, nwy_test_cli_spi_init},
        {" Transfer", NULL, nwy_test_cli_spi_trans},
        {" Deinit", NULL, nwy_test_cli_spi_deinit},
        {" General spi mount", NULL, nwy_test_cli_spi_flash_mount},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const spi_menu_arry = NULL;
#endif

//1.4 GPIO
static cli_menu_item gpio_menu_arry[] =
    {
        {" Set value", NULL, nwy_test_cli_gpio_set_val},
        {" Get value", NULL, nwy_test_cli_gpio_get_val},
        {" Set direction", NULL, nwy_test_cli_gpio_set_dirt},
        {" Get direction", NULL, nwy_test_cli_gpio_get_dirt},
        {" Config irq", NULL, nwy_test_cli_gpio_config_irq},
        {" Enable irq", NULL, nwy_test_cli_gpio_enable_irq},
        {" Disable irq", NULL, nwy_test_cli_gpio_disable_irq},
        {" Close", NULL, nwy_test_cli_gpio_close},
        {" Pull", NULL, nwy_test_cli_gpio_pull},
        {NULL, NULL, NULL}};
//1.5 ADC (no sub menu)
#ifndef NWY_OPEN_TEST_ADC
#define nwy_test_cli_adc_read NULL
#endif


//1.7 Keypad
#ifdef NWY_OPEN_TEST_KEYPAD
static cli_menu_item keypad_menu_arry[] =
    {
        {" Register callback", NULL, nwy_test_cli_keypad_reg_cb},
        {" Set debouce time", NULL, nwy_test_cli_keypad_set_debouce},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const keypad_menu_arry = NULL;
#endif

//1.8 PWM
#if defined (NWY_OPEN_TEST_PWM) || defined (NWY_OPEN_TEST_PWM_EC618)
static cli_menu_item pwm_menu_arry[] =
    {
        {" Init", NULL, nwy_test_cli_pwm_init},
        {" Start", NULL, nwy_test_cli_pwm_start},
        {" Stop", NULL, nwy_test_cli_pwm_stop},
        {" Deinit", NULL, nwy_test_cli_pwm_deinit},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const pwm_menu_arry = NULL;
#endif

static cli_menu_item peripherals_menu_arry[] =
    {
        {" UART", uart_menu_arry, NULL},
        {" I2C", i2c_menu_arry, NULL},
        {" SPI", spi_menu_arry, NULL},
        {" GPIO", gpio_menu_arry, NULL},
        {" ADC", NULL, nwy_test_cli_adc_read},
        {" PM", pm_menu_arry, NULL},
        {" Keypad", keypad_menu_arry, NULL},
        {" PWM", pwm_menu_arry, NULL},
        {NULL, NULL, NULL}};

//2. LCD
#ifdef NWY_OPEN_TEST_LCD
static cli_menu_item lcd_menu_arry[] =
    {
        {" Open lcd", NULL, nwy_test_cli_lcd_open},
        {" Draw line", NULL, nwy_test_cli_lcd_draw_line},
        {" Draw chinese", NULL, nwy_test_cli_lcd_draw_chinese},
        {" Close lcd", NULL, nwy_test_cli_lcd_close},
        {" Open backlight", NULL, nwy_test_cli_lcd_open_bl},
        {" Close backlight", NULL, nwy_test_cli_lcd_close_bl},
        {" Set backlight level", NULL, nwy_test_cli_lcd_set_bl_level},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const lcd_menu_arry = NULL;
#endif

#ifdef NWY_OPEN_TEST_CAMERA
static cli_menu_item camera_menu_arry[] =
    {
        {" Open camera", NULL, nwy_test_cli_camera_open},
        {" Close camera", NULL, nwy_test_cli_camera_close},
        {" get preview", NULL, nwy_test_cli_camera_get_preview},
        {" camera capture", NULL, nwy_test_cli_camera_capture},
        {NULL, NULL, NULL}
    };
#endif
//3. SD
#ifdef NWY_OPEN_TEST_SD
static cli_menu_item sd_menu_arry[] =
    {
        {" Get status", NULL, nwy_test_cli_sd_get_st},
        {" Mount", NULL, nwy_test_cli_sd_mnt},
        {" Unmount", NULL, nwy_test_cli_sd_unmnt},
        {" Format", NULL, nwy_test_cli_sd_format},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const sd_menu_arry = NULL;
#endif

// Flash
#ifdef NWY_OPEN_TEST_FLASH
static cli_menu_item flash_menu_arry[] =
    {
        {" Open", NULL, nwy_test_cli_flash_open},
        {" Erase", NULL, nwy_test_cli_flash_erase},
        {" Write", NULL, nwy_test_cli_flash_write},
        {" Read", NULL, nwy_test_cli_flash_read},
        {NULL, NULL, NULL}};
#else
        static cli_menu_item * const flash_menu_arry = NULL;
#endif

//5. TTS
#ifdef FEATURE_NWY_OPEN_TTS
static cli_menu_item tts_menu_arry[] =
    {
        {" Input content", NULL, nwy_test_cli_tts_input},
        {" Start play", NULL, nwy_test_cli_tts_play_start},
        {" Stop play", NULL, nwy_test_cli_tts_play_stop},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const tts_menu_arry = NULL;
#endif

//6. fota
#ifdef NWY_OPEN_TEST_FOTA_NS
static cli_menu_item * const fota_menu_arry = NULL;
#else
static cli_menu_item fota_menu_arry[] =
    {
        {" Base version update", NULL, nwy_test_cli_fota_base_ver},
        {" APP update", NULL, nwy_test_cli_fota_app_ver},
        {NULL, NULL, NULL}};
#endif

//7. Audio
#ifdef NWY_OPEN_TEST_AUDIO
static cli_menu_item audio_menu_arry[] =
    {
        {" Play", NULL, nwy_test_cli_audio_play},
        {" Record", NULL, nwy_test_cli_audio_rec},
        {" DTMF Play", NULL, nwy_test_cli_audio_dtmf},
        {" DTMF Detect Enable", NULL, nwy_test_cli_audio_dtmf_enable},
        {" DTMF Detect Disable", NULL, nwy_test_cli_audio_dtmf_disable},
#ifdef NWY_OPEN_AUDIO_SD_PLAY
        {" Play SD file", NULL, nwy_test_cli_audio_sd_play},
#endif
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const audio_menu_arry = NULL;
#endif
#endif

//1.6 PM
static cli_menu_item pm_menu_arry[] =
{
    //{"2. Control module sleep or wakeup", NULL, nwy_test_cli_pm_save_md},
    //{"3. Get module boot reason", NULL, nwy_test_cli_get_boot_cause},
    {" Control module power off or reset", NULL, nwy_test_cli_pm_pwr_off},
    {" Control module sleep or wakeup", NULL, nwy_test_cli_pm_save_md},
    {" Get module boot reason", NULL, nwy_test_cli_get_boot_cause},
#ifndef NWY_OPEN_TEST_PM_POWER_SWITCH_NS
    {" Switch sub power", NULL, nwy_test_cli_pm_power_switch},
#endif
#ifndef NWY_OPEN_TEST_PM_POWER_LEVEL_NS
    {" Set sub power level", NULL, nwy_test_cli_pm_level_set},
#endif
#ifdef NWY_OPEN_TEST_POWER_STATE
    {" Get power supply state", NULL, nwy_test_cli_power_state},
    {" Get power supply voltage", NULL, nwy_test_cli_power_voltage},
#endif
    {NULL, NULL, NULL}};

#ifdef NWY_OPEN_TEST_ALARM
static cli_menu_item alarm_menu_arry[] =
{
    {" Set boot alarm", NULL, nwy_test_cli_boot_alarm_set},
    {" Delete boot alarm", NULL, nwy_test_cli_boot_alarm_del},
    {NULL, NULL, NULL}};
#endif

static cli_menu_item peripherals_menu_arry[] =
{
    {" UART", uart_menu_arry, NULL},
    {" I2C", i2c_menu_arry, NULL},
    {" SPI", spi_menu_arry, NULL},
    {" GPIO", gpio_menu_arry, NULL},
    {" ADC", adc_menu_arry, NULL},
    {" PM", pm_menu_arry, NULL},
    //		{" Keypad", keypad_menu_arry, NULL},
#ifndef NWY_OPEN_TEST_PWM_NS
    {" PWM", pwm_menu_arry, NULL},
#endif
#ifdef NWY_OPEN_TEST_ALARM
    {" ALARM", alarm_menu_arry, NULL},
#endif
    {NULL, NULL, NULL}};

//8. fs
static cli_menu_item fs_menu_arry[] =
    {
        {" Open file", NULL, nwy_test_cli_fs_open},
        {" Write file", NULL, nwy_test_cli_fs_write},
        {" Read file", NULL, nwy_test_cli_fs_read},
        {" Read file size", NULL, nwy_test_cli_fs_fsize},
        {" Seek file", NULL, nwy_test_cli_fs_seek},
        {" Tell file", NULL, nwy_test_cli_fs_ftell},
        {" Sync file", NULL, nwy_test_cli_fs_sync},
        {" Get file state", NULL, nwy_test_cli_fs_fstate},
        {" Trunc file", NULL, nwy_test_cli_fs_trunc},
        {" Close file", NULL, nwy_test_cli_fs_close},
        {" Unlink/remove file", NULL, nwy_test_cli_fs_remove},
        {" Rename file", NULL, nwy_test_cli_fs_rename},
        {" Open dir", NULL, nwy_test_cli_dir_open},
        {" Read dir", NULL, nwy_test_cli_dir_read},
        {" Tell dir", NULL, nwy_test_cli_dir_tell},
        {" Seek dir", NULL, nwy_test_cli_dir_seek},
        {" Rewind dir", NULL, nwy_test_cli_dir_rewind},
        {" Close dir", NULL, nwy_test_cli_dir_close},
        {" Make dir", NULL, nwy_test_cli_dir_mk},
        {" Remove dir", NULL, nwy_test_cli_dir_remove},
        {" Get free size", NULL, nwy_test_cli_fs_free_size},
        {" Spi flash fs", NULL, nwy_test_cli_spi_flash_mount},
        {NULL, NULL, NULL}};

#ifdef NWY_OPEN_TEST_USB_NET_SWITCH
static cli_menu_item usb_menu_arry[] =
{
    {" Usb net type switch", NULL, nwy_test_cli_usb_net_switch},
    {" Usb net type get", NULL, nwy_test_cli_usb_net_get},
    {NULL, NULL, NULL}};
#endif

static cli_menu_item driver_menu_arry[] =
    {
#ifdef NWY_OPEN_TEST_PERIHERALS
    {" Peripherals", peripherals_menu_arry, NULL},
#endif
#if NWY_TEST_CLI_NOT_SUPPORT
    {" LCD", lcd_menu_arry, NULL},
    {" SD Card", sd_menu_arry, NULL},
    {" Flash", flash_menu_arry, NULL},
    {" TTS", tts_menu_arry, NULL},
    {" FOTA", fota_menu_arry, NULL},
    {" Audio", audio_menu_arry, NULL},
#endif
    {" File system", fs_menu_arry, NULL},
#ifdef NWY_OPEN_TEST_CAMERA
    {" Camera", camera_menu_arry, NULL},
#endif
#ifdef NWY_OPEN_TEST_USB
    {" USB", usb_menu_arry, NULL},
#endif
    {NULL, NULL, NULL}};

#if NWY_TEST_CLI_NOT_SUPPORT
/*******************************************************************************
                        3. Short Distance Wireless
********************************************************************************/
//1. BLE SERVICE
#ifdef NWY_OPEN_TEST_BLE_SERV
static cli_menu_item ble_service_menu_arry[] =
    {
        {" BLE open", NULL, nwy_test_cli_ble_open},
        {" BLE adv set", NULL, nwy_test_cli_ble_set_adv},
        {" BLE send data", NULL, nwy_test_cli_ble_send},
        {" BLE receive data", NULL, nwy_test_cli_ble_recv},
        {" BLE update connect", NULL, nwy_test_cli_ble_updata_connt},
        {" BLE get status", NULL, nwy_test_cli_ble_get_st},
        {" BLE get version", NULL, nwy_test_cli_ble_get_ver},
        {" BLE set device name", NULL, nwy_test_cli_ble_set_dev_name},
        {" BLE set beacon", NULL, nwy_test_cli_ble_set_beacon},
        {" BLE set manufacture", NULL, nwy_test_cli_ble_set_manufacture},
        {" BLE set adv server uuid", NULL, nwy_test_cli_ble_set_adv_server_uuid},
        {" BLE set service", NULL, nwy_test_cli_ble_set_srv},
        {" BLE set characteristic", NULL, nwy_test_cli_ble_set_char},
        {" BLE conn status(auto report)", NULL, nwy_test_cli_ble_conn_status_report},
        {" BLE conn status(manual get)", NULL, nwy_test_cli_ble_conn_status},
        {" BLE mac addr", NULL, nwy_test_cli_ble_mac_addr},
        {" BLE add server", NULL, nwy_test_cli_ble_add_server},
        {" BLE add charcter", NULL, nwy_test_cli_ble_add_char},
        {" BLE add send data", NULL, nwy_test_cli_ble_add_send_data},
        {" BLE add recv data", NULL, nwy_test_cli_ble_add_recv_data},
        {" BLE read req", NULL, nwy_test_cli_ble_read_req},
        {" BLE set read rsp", NULL, nwy_test_cli_ble_read_rsp},
        {" BLE disconnet", NULL, nwy_test_cli_ble_disconnect},
        {" BLE close", NULL, nwy_test_cli_ble_close},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const ble_service_menu_arry = NULL;
#endif

//2. BLE CLIENT
#ifdef NWY_OPEN_TEST_BLE_CLI
static cli_menu_item ble_client_menu_arry[] =
    {
        {" BLE client enable set", NULL, nwy_test_cli_ble_client_set_enable},
        {" BLE client scan dev", NULL, nwy_test_cli_ble_client_scan},
        {" BLE client connect", NULL, nwy_test_cli_ble_client_connect},
        {" BLE client disconnect", NULL, nwy_test_cli_ble_client_disconnect},
        {" BLE client discovery srv", NULL, nwy_test_cli_ble_client_discover_srv},
        {" BLE client discovery char", NULL, nwy_test_cli_ble_client_discover_char},
        {" BLE client send data", NULL, nwy_test_cli_ble_client_send_data},
        {" BLE client recv data", NULL, nwy_test_cli_ble_client_recv_data},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const ble_client_menu_arry = NULL;
#endif
#endif

//2. WIFI
#ifdef NWY_OPEN_TEST_WIFI
static cli_menu_item wifi_menu_arry[] =
    {
        {" get wifi current status", NULL, nwy_cli_test_wifi_get_st},
        {" enable wifi", NULL, nwy_cli_test_wifi_enable},
        {" set wifi work mode", NULL, nwy_cli_test_wifi_set_work_md},
        {" set wifi ap basic params", NULL, nwy_cli_test_wifi_set_ap_para},
        {" set wifi ap advance params", NULL, nwy_cli_test_wifi_set_ap_para_adv},
        {" get wifi ap connected client info", NULL, nwy_cli_test_wifi_get_clit_info},
        {" wifi sta start scan", NULL, nwy_cli_test_wifi_sta_scan},
        {" wifi sta get scan results", NULL, nwy_cli_test_wifi_sta_scan_ret},
        {" wifi sta connect external hotspot", NULL, nwy_cli_test_wifi_sta_connt},
        {" wifi sta disconnect", NULL, nwy_cli_test_wifi_sta_disconnt},
        {" get wifi sta connected external hostpot info", NULL, nwy_cli_test_wifi_sta_get_hostpot_info},
        {" disable wifi", NULL, nwy_cli_test_wifi_disable},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const wifi_menu_arry = NULL;
#endif
static cli_menu_item sdw_menu_arry[] =
    {
#if NWY_TEST_CLI_NOT_SUPPORT
        {" BLE service", ble_service_menu_arry, NULL},
        {" BLE client", ble_client_menu_arry, NULL},
#endif
        {" WIFI", wifi_menu_arry, NULL},
        {NULL, NULL, NULL}};
/*******************************************************************************
                         LBS
********************************************************************************/
//1. GNSS
#ifdef NWY_OPEN_TEST_GNSS
static cli_menu_item gnss_menu_arry[] =
    {
        {" Open location position", NULL, nwy_test_cli_gnss_open},
        {" Set position mode", NULL, nwy_test_cli_gnss_set_position_md},
        {" Set output format", NULL, nwy_test_cli_gnss_output_format},
        {" Delete aiding data", NULL,  nwy_test_cli_gnss_delete_aiding_data},
        {" Close location position", NULL, nwy_test_cli_gnss_close},
        {" Location and satellite info", NULL, nwy_test_cli_gnss_nmea_info_parse},
        {" Get raw nmea data", NULL, nwy_test_cli_gnss_nmea_data},
        {" Set location server", NULL, nwy_test_cli_gnss_set_server},
        {" Open assisted location", NULL, nwy_test_cli_gnss_open_assisted},
        {" Set LTE  priority", NULL, nwy_test_cli_gnss_set_priority},
        {" Open location resume", NULL, nwy_test_cli_gnss_open_resume},
        {NULL, NULL, NULL}};
#else
#if !defined(NWY_OPEN_TEST_GNSS_EXTERNAL_CHIP)
static cli_menu_item * const gnss_menu_arry = NULL;
#endif
#endif
#ifdef NWY_OPEN_TEST_GNSS_EXTERNAL_CHIP
static cli_menu_item gnss_menu_arry[] =
    {
        {" Open location position", NULL, nwy_test_cli_gnss_open},
        {" Close location position", NULL, nwy_test_cli_gnss_close},
        {" Get all nmea data", NULL, nwy_test_cli_gnss_nmea_data},
        {" Get specific nmea data", NULL, nwy_test_cli_gnss_nmea_specific_data},
        {NULL, NULL, NULL}};

#endif

#ifdef NWY_OPEN_TEST_LBS
static cli_menu_item loc_menu_arry[] =
    {
        {" Open location base position", NULL, nwy_test_cli_gnss_open_base},
        {" Open wifi loc position", NULL, nwy_test_cli_wifi_open_base},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const loc_menu_arry = NULL;
#endif
static cli_menu_item lbs_menu_arry[] =
    {
        {" GNSS", gnss_menu_arry, NULL},
        {" Loc", loc_menu_arry, NULL},
        {NULL, NULL, NULL}};

/*******************************************************************************
                        5. TCP/IP Protocol
********************************************************************************/
//1. TCP Client
static cli_menu_item tcp_cli_menu_arry[] =
    {
        {" TCP setup", NULL, nwy_test_cli_tcp_setup},
        {" TCP send", NULL, nwy_test_cli_tcp_send},
        {" TCP close", NULL, nwy_test_cli_tcp_close},
        {NULL, NULL, NULL}};

static cli_menu_item tcp_server_cli_menu_arry[] =
    {
        {"TCP server", NULL, nwy_test_cli_tcp_server},
        {NULL, NULL, NULL}};

//2. UDP Client
static cli_menu_item udp_cli_menu_arry[] =
    {
        {" UDP setup", NULL, nwy_test_cli_udp_setup},
        {" UDP send", NULL, nwy_test_cli_udp_send},
        {" UDP close", NULL, nwy_test_cli_udp_close},
        {NULL, NULL, NULL}};
/*EX test*/
static cli_menu_item tcp_cli_menu_arry_ex[] =
    {
        {" TCP setup ex", NULL, nwy_test_cli_tcp_setup_ex},
        {" TCP send ex", NULL, nwy_test_cli_tcp_send_ex},
        {" TCP close ex", NULL, nwy_test_cli_tcp_close_ex},
        {NULL, NULL, NULL}};

static cli_menu_item tcp_server_cli_menu_arry_ex[] =
    {
        {"TCP server ex", NULL, nwy_test_cli_tcp_server_ex},
        {NULL, NULL, NULL}};

static cli_menu_item udp_cli_menu_arry_ex[] =
    {
        {" UDP setup ex", NULL, nwy_test_cli_udp_setup_ex},
        {" UDP send ex", NULL, nwy_test_cli_udp_send_ex},
        {" UDP close ex", NULL, nwy_test_cli_udp_close_ex},
        {NULL, NULL, NULL}};
/*EX test*/

static cli_menu_item packet_test_cli_menu_arry[] =
    {
        {" PING test", NULL, nwy_test_cli_ping},
#ifdef NWY_OPEN_TEST_IP_PACKET
        {" IP packet test", NULL, nwy_test_cli_ip_packet},
#endif
        {NULL, NULL, NULL}};

//3. FTP
#ifdef NWY_OPEN_TEST_FTP
static cli_menu_item ftp_cli_menu_arry[] =
    {
        {" Login", NULL, nwy_test_cli_ftp_login},
        {" Get file", NULL, nwy_test_cli_ftp_get},
        {" Put file", NULL, nwy_test_cli_ftp_put},
        {" Get file size", NULL, nwy_test_cli_ftp_filesize},
        {" Delete file", NULL, nwy_test_cli_ftp_delete},
        {" Logout", NULL, nwy_test_cli_ftp_logout},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const ftp_cli_menu_arry = NULL;
#endif
// Http
#ifdef NWY_OPEN_TEST_HTTP
static cli_menu_item http_cli_menu_arry[] =
    {
        {" Setup", NULL, nwy_test_cli_http_setup},
        {" Get", NULL, nwy_test_cli_http_get},
        {" Head", NULL, nwy_test_cli_http_head},
        {" Post", NULL, nwy_test_cli_http_post},
        {" Put", NULL, nwy_test_cli_http_put},
        {" Open mode", NULL, nwy_test_cli_http_open_mode},
        {" Close", NULL, nwy_test_cli_http_close},
#if NWY_TEST_CLI_NOT_SUPPORT
        {"12. ping test", NULL, nwy_test_cli_ping},
#endif
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const http_cli_menu_arry = NULL;
#endif
static cli_menu_item sntp_cli_menu_arry[] =
    {
        {" ntp time", NULL, nwy_test_cli_sntp_get_time},
        {NULL, NULL, NULL}};
#ifdef NWY_OPEN_TEST_MBEDTLS
static cli_menu_item ssltcp_cli_menu_arry[] =
    {
        {" ssltcp init", NULL, nwy_test_cli_ssltcp_init},
        {" ssltcp setup", NULL, nwy_test_cli_ssltcp_setup},
        {" ssltcp send", NULL, nwy_test_cli_ssltcp_send},
        {" ssltcp Close", NULL, nwy_test_cli_ssltcp_close},
        {NULL, NULL, NULL}};

static cli_menu_item nwy_cli_mbedtls_arry[] =
    {
    #ifdef NWY_TEST_MBEDTLS_AES_SUPPORT
    {" mbedtls_aes", NULL, nwy_mbedtls_aes_test},
    #endif
    {" mbedtls_tls", ssltcp_cli_menu_arry, NULL},
    {" get mbedtls version", NULL, nwy_test_cli_get_sslinfo},
    {NULL, NULL, NULL}};
#else
static cli_menu_item * const nwy_cli_mbedtls_arry = NULL;
#endif
static cli_menu_item tcpip_menu_arry[] =
    {
        {" TCP Client", tcp_cli_menu_arry, NULL},
        {" TCP Server", tcp_server_cli_menu_arry, NULL},
        {" UDP Client", udp_cli_menu_arry, NULL},
        {" PACKET test", packet_test_cli_menu_arry, NULL},
        {" FTP Client", ftp_cli_menu_arry, NULL},
        {" HTTP Client", http_cli_menu_arry, NULL},
        {" SNTP", sntp_cli_menu_arry ,NULL},
        {NULL, NULL, NULL}};


static cli_menu_item tcpip_menu_arry_ex[] =
{
    {" TCP Client ex", tcp_cli_menu_arry_ex, NULL},
    {" TCP Server ex", tcp_server_cli_menu_arry_ex, NULL},
    {" UDP Client ex", udp_cli_menu_arry_ex, NULL},
    {NULL, NULL, NULL}
};




static cli_menu_item three_party_menu_arry[] =
    {
        {" mbedtls", nwy_cli_mbedtls_arry ,NULL},
        {NULL, NULL, NULL}};
#if (defined(FEATURE_NWY_PAHO_MQTT_V3)) || (defined(FEATURE_NWY_ALI_MQTT)) || (defined(FEATURE_NWY_PAHO_MQTT_V5))

/*******************************************************************************
                        6. IoT Protocol
********************************************************************************/
//1. Ali MQTT
#if (defined(FEATURE_NWY_ALI_MQTT)) || (defined(FEATURE_NWY_ALI_MQTT_V4X))
static cli_menu_item ali_mqtt_menu_arry[] =
    {
        {" Ali-MQTT connect", NULL, nwy_test_cli_alimqtt_connect},
        {" Ali-MQTT pub", NULL, nwy_test_cli_alimqtt_pub},
        {" Ali-MQTT sub", NULL, nwy_test_cli_alimqtt_sub},
        {" Ali-MQTT unsub", NULL, nwy_test_cli_alimqtt_unsub},
        {" Ali-MQTT state", NULL, nwy_test_cli_alimqtt_state},
        {" Ali-MQTT disconnect", NULL, nwy_test_cli_alimqtt_disconnect},
        {" Ali-MQTT del kv", NULL, nwy_test_cli_alimqtt_del_kv},
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const ali_mqtt_menu_arry = NULL;
#endif

//2. Paho MQTT
#if (defined(FEATURE_NWY_PAHO_MQTT_V5)) || (defined(FEATURE_NWY_PAHO_MQTT_V3))
static cli_menu_item paho_mqtt_menu_arry[] =
    {
        {" MQTT connect", NULL, nwy_test_cli_mqtt_connect},
        {" MQTT pub", NULL, nwy_test_cli_mqtt_pub},
        {" MQTT sub", NULL, nwy_test_cli_mqtt_sub},
        {" MQTT unsub", NULL, nwy_test_cli_mqtt_unsub},
        {" MQTT state", NULL, nwy_test_cli_mqtt_state},
        {" MQTT disconnect", NULL, nwy_test_cli_mqtt_disconnect},
#ifdef FEATURE_NWY_N58_OPEN_NIPPON
        {" MQTT 10k msg pub", NULL, nwy_test_cli_mqtt_pub_test},
#endif
        {NULL, NULL, NULL}};
#else
static cli_menu_item * const paho_mqtt_menu_arry = NULL;
#endif

static cli_menu_item iot_menu_arry[] =
    {
        #ifdef FEATURE_NWY_ALI_MQTT
        {" Ali-MQTT", ali_mqtt_menu_arry, NULL},
        #endif
        #ifdef FEATURE_NWY_PAHO_MQTT_V3
        {" Paho MQTT", paho_mqtt_menu_arry, NULL},
        #endif
        {NULL, NULL, NULL}};
#endif
/*******************************************************************************
                        7. OS & Device
********************************************************************************/
//1. AT
static cli_menu_item virtual_at_menu_arry[] =
    {
        {" Virtual AT Init", NULL, nwy_test_cli_virt_at_init},
        {" Register AT Unsolicited", NULL, nwy_test_cli_reg_at_unsolicited},
        {" Send Virtual AT", NULL, nwy_test_cli_send_virt_at},
        {" Regist forward AT", NULL, nwy_test_cli_reg_at_fwd},
        {NULL, NULL, NULL}};

//2. dev info
static cli_menu_item dev_info_menu_arry[] =
    {
        {" Get model", NULL, nwy_test_cli_get_model},
        {" Get SW version", NULL, nwy_test_cli_get_sw_ver},
        {" Get HW version", NULL, nwy_test_cli_get_hw_ver},
        {" Get IMEI", NULL, nwy_test_cli_get_imei},
        {" Get heap info", NULL, nwy_test_cli_get_heap_info},
        {" Get CPU temperature", NULL, nwy_test_cli_get_cpu_temp},
        {" Set APP Version", NULL, nwy_test_cli_set_app_version},
        {" Get SDK Version String", NULL, nwy_test_cli_get_open_sdk_ver_string},
        {NULL, NULL, NULL}};
//3. OS
static cli_menu_item os_api_menu_arry[] =
    {
        {" Start timer", NULL, nwy_test_cli_start_timer},
        {" Stop timer", NULL, nwy_test_cli_stop_timer},
        {" Get sys time", NULL, nwy_test_cli_get_time},
        {" Set sys time", NULL, nwy_test_cli_set_time},
        {" Semaphore", NULL, nwy_test_cli_test_semp},
        {" Thread priority test", NULL, nwy_test_cli_thread_priority_test},
        {" Thread event send/wait test", NULL, nwy_test_cli_event_wait_performance},
        {" Start ISR timer", NULL, nwy_test_cli_start_isr_timer},
        {" Start service timer", NULL, nwy_test_cli_start_service_timer},
        {" Start user timer", NULL, nwy_test_cli_start_user_timer},
        {" Start all timers", NULL, nwy_test_cli_start_all_timers},
        {" Stop all timers", NULL, nwy_test_cli_stop_all_timers},
        {" date and timestamp convert test", NULL, nwy_test_cli_date_timestamp},
        {NULL, NULL, NULL}};

static cli_menu_item dev_os_menu_arry[] =
    {
        {" Virtual AT", virtual_at_menu_arry, NULL},
        {" Device info", dev_info_menu_arry, NULL},
        {" OS API", os_api_menu_arry, NULL},
        {NULL, NULL, NULL}};

static cli_menu_item dev_fota_menu_arry[] =
    {
#ifdef NWY_OPEN_TEST_MINI_FOTA
        {" Base version or Base and App Update", NULL, nwy_test_cli_fota_base_update},
        {" App Update", NULL, nwy_test_cli_fota_local_update},
#else
        {" Local Update", NULL, nwy_test_cli_fota_local_update},
        {" Ftp fota Update", NULL, nwy_test_cli_ftp_fota_update},
#endif
        {" Get Update result", NULL, nwy_test_cli_fota_result},
        {" Get Base and App Version", NULL, nwy_test_cli_get_version},
        {NULL, NULL, NULL}};

/*******************************************************************************
                          MAIN
********************************************************************************/
static cli_menu_item mian_menu_arry[] =
    {
        {" Mobile Network", mobile_menu_arry, NULL},
    #if 0
        {" Driver & Kernel", driver_menu_arry, NULL},
        {" Short Distance Wireless", sdw_menu_arry, NULL},
        {" LBS", lbs_menu_arry, NULL},
        {" TCP/IP Protocol", tcpip_menu_arry, NULL},
        {" IoT Protocol", iot_menu_arry, NULL},
    #endif
		#if (defined(FEATURE_NWY_PAHO_MQTT_V3)) || (defined(FEATURE_NWY_ALI_MQTT)) || (defined(FEATURE_NWY_PAHO_MQTT_V5))
        {" IoT Protocol", iot_menu_arry, NULL},
        #endif
		{" Driver & Kernel", driver_menu_arry, NULL},
        {" OS & Device", dev_os_menu_arry, NULL},
        {" Short Distance Wireless", sdw_menu_arry, NULL},
        {" LBS", lbs_menu_arry, NULL},
        {" TCP/IP Protocol", tcpip_menu_arry, NULL},
        {" TCP/IP Protocol EX", tcpip_menu_arry_ex, NULL},
        {" Three party library", three_party_menu_arry, NULL},
        {" FOTA", dev_fota_menu_arry, NULL},

        {NULL, NULL, NULL}};

static cli_menu_item *menu_stack[TEST_CLI_MENU_STACK_MAX] =
    {mian_menu_arry, NULL, NULL, NULL, NULL};
static int curt_menu_stack_id = 0;

void nwy_test_cli_menu_display()
{
    int i;
    cli_menu_item *disp_menu = menu_stack[curt_menu_stack_id];

    nwy_test_cli_echo("\r\n");
    for (i = 0;; i++)
    {
        if (disp_menu[i].name_str == NULL)
        {
            break;
        }
        nwy_test_cli_echo("%d.%s\r\n", (i+1),disp_menu[i].name_str);
    }
}

void nwy_test_cli_menu_select(int opt)
{
    cli_menu_item *curt_menu = menu_stack[curt_menu_stack_id];
    int i;

    if (opt < 1)
    {
        nwy_test_cli_echo("\r\nInvalid option\r\n");
        return;
    }

    for (i = 1;; i++)
    {
        if (curt_menu[i - 1].name_str == NULL)
        {
            break;
        }
    }

    if (opt >= i)
    {
        nwy_test_cli_echo("\r\nInvalid option\r\n");
        return;
    }

    opt--;
    if (curt_menu[opt].sub_menu != NULL && curt_menu[opt].sub_menu[0].name_str != NULL)
    {
        curt_menu_stack_id++;
        menu_stack[curt_menu_stack_id] = curt_menu[opt].sub_menu;
    }
    else if (curt_menu[opt].func != NULL)
    {
        curt_menu[opt].func();
    }
    else
    {
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
    }
}

void nwy_test_cli_menu_back()
{
    if (curt_menu_stack_id != 0)
    {
        menu_stack[curt_menu_stack_id] = NULL;
        curt_menu_stack_id--;
    }
}
