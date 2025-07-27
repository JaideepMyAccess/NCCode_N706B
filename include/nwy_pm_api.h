/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_pm_api.h
 * Author       : Charlie
 * Created      : 2023-06-09
 * Description  : power management API
 ******************************************************************************/
#ifndef __NWY_PM_API_H__
#define __NWY_PM_API_H__
#include "nwy_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NWY_BOOTCAUSE_UNKNOWN     0            ///< placeholder for unknown reason
#define NWY_BOOTCAUSE_PWRKEY      (1 << 0)     ///< boot by power key
#define NWY_BOOTCAUSE_PIN_RESET   (1 << 1)     ///< boot by pin reset
#define NWY_BOOTCAUSE_ALARM       (1 << 2)     ///< boot by alarm
#define NWY_BOOTCAUSE_CHARGE      (1 << 3)     ///< boot by charge in
#define NWY_BOOTCAUSE_WDG         (1 << 4)     ///< boot by watchdog
#define NWY_BOOTCAUSE_PIN_WAKEUP  (1 << 5)     ///< boot by wakeup
#define NWY_BOOTCAUSE_PSM_WAKEUP  (1 << 6)     ///< boot from PSM wakeup
#define NWY_BOOTCAUSE_DOWNLOAD    (1 << 7)     ///< boot enter download mode
#define NWY_BOOTCAUSE_FACTORY     (1 << 8)     ///< boot enter factory mode

typedef enum
{
	NWY_POWER_LCD,
	NWY_POWER_KEYLED,
	NWY_POWER_BACK_LIGHT,
	NWY_POWER_SD,
	NWY_POWER_CAMA,
	NWY_POWER_CAMD,
	NWY_POWER_SIM1,
	NWY_POWER_MAX
}nwy_pm_id_e;

typedef enum
{
	NWY_BOOT_CAUSE_UNKNOWN = 0,
	NWY_BOOT_CAUSE_PWRKEY,
	NWY_BOOT_CAUSE_PIN_RESET,
	NWY_BOOT_CAUSE_ALARM,
	NWY_BOOT_CAUSE_CHARGE,
	NWY_BOOT_CAUSE_WDG,
	NWY_BOOT_CAUSE_GPIO_WAKE,
	NWY_BOOT_CAUSE_SMPL,
	NWY_BOOT_CAUSE_GPT,
	NWY_BOOT_CAUSE_WDG_RST,
	NWY_BOOT_CAUSE_PANIC_RST,
	NWY_BOOT_CAUSE_PM2_BOOT,
	NWY_BOOT_CAUSE_PSM_WAKEUP
}nwy_boot_cause;

typedef enum {
    NWY_POWER_DROP_STATE   = 0,
    NWY_POWER_NORMAL_STATE = 1
}nwy_power_state_t;

/**
 * @brief: nwy sleep callback for user app
 *
 * @param enter_sleep: 
 *      0   -   exit sleep
 *      1   -   enter sleep
 */
typedef void (*nwy_sleep_callback_t)(int enter_sleep);

#define NWY_PM_POWER_OFF_QUCIKLY    1 // DIRECTLY POWER OFF
#define NWY_PM_POWER_OFF_NORMAL     2 // SYNC MODEM BEFORE POWER OFF
#define NWY_PM_POWER_OFF_RESET      3 // AUTO POWER ON AFTER POWER OFF

#define NWY_PM_SLEEP_DISABLE        0 // disable sleep feature
#define NWY_PM_SLEEP_ENABLE         1 // enable sleep feature

/**
 * @brief: control module power off or reset
 *
 * @param option: 
 *		1	-	quickly power off
 *		2	-	normal power off
 *		3	-	reset
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_pm_ctrl(int option);

/**
 * @brief: control module sleep or wakeup
 *
 * @param option: 
 *		0	-	NWY_PM_SLEEP_DISABLE
 *		1	-	NWY_PM_SLEEP_ENABLE
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED     
 */
nwy_error_e nwy_pm_state_set(int mode);

/**
 * @brief: config wakeup
 *
 * @param wake_source:
 *		0	-	GPIO(DTR)
 * @param wake_enable:
 *		1	-	enable wake soirce
 *		0	-	disable wake soirce
 * @param wake_level:
 *		0	-	low level wakeup
 *		1	-	high level wakeup
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED     
 */
nwy_error_e nwy_pm_sleep_wakeup_set(int wake_source, int wake_enable, int wake_level);

/**
 * @brief: set sleep callback
 *
 * @param cb: user app sleep callback function
 * @return:
 *      0	-	SUCCESS
 *     -2	-	INVALID PARA
 *     -3	-	FAILED
 */
nwy_error_e nwy_pm_sleep_callback_set(nwy_sleep_callback_t cb);

/**
 * @brief: get module boot reason
 *
 * @param option: 
 *		void
 * @return:
 *      0	-	placeholder for unknown reason   
 *      1	-	boot by power key   
 *      2	-	boot by pin reset 
 *      3	-	boot by alarm   
 *      4	-	boot by charge in   
 *      5	-	boot by watchdog
 *      6	-	boot by gpio wakeup    
 *      7	-	boot from SMPL
 *      8	-	boot from GPT
 *      9	-	boot by watchdog rst 
 *      10	-	boot by panic reset
 *      11	-	boot from PM2 cold boot
 */
int nwy_pm_boot_res(void);

/**
 * @brief: nwy_pm_level_set
 *
 * @param id: 
 *		nwy_pm_id_e
 *		mv:
 *		power voltage
 * @return:
 *		0	-	success   
 *		-2	-	invalid param
 *		-3	-	failed
 */
nwy_error_e nwy_pm_level_set(nwy_pm_id_e id, uint32_t mv);

/**
 * @brief: nwy_pm_power_switch
 *
 * @param id: 
 *		nwy_pm_id_e
 *		enabled:
 *		0	-	enable
 *		1	-	disable
 *		lp_enabled:
 *		0	-	enable
 *		1	-	disable
 * @return:
 *		0	-	success 
 *		-2	-	invalid param  
 *		-3	-	failed
 */
nwy_error_e nwy_pm_power_switch(nwy_pm_id_e id, int enabled, int lp_enabled);

/**
 * @brief view the pm state
 *
 * @param 
 * @return
 *      - NWY_POWER_DROP_STATE    0
 *      - NWY_POWER_NORMAL_STATE  1
 */
int nwy_power_state(void);

/**
 * @brief: get the power supply voltage
 *
 * @param option: 
 *		vbat-	voltage in mV
 * @return:
 *		0	-	success 
 */
nwy_error_e nwy_pm_vbat_voltage_get(int *vbat);

/**
* @brief: get the battery voltage
*
* @param option: 
*      percent-   percent in 0-100
* @return:
*      0   -   success 
*      -2  -   INVALID_PARA 
*      -4   -   GET_FAILED 
*/
nwy_error_e nwy_battery_percent_get(int *percent);

/**
* @brief: get the battery charge state
*
* @param option: 
*        state -   charge state 
*        0  -  not charge
*        1  -  charging
*        2  -  charge full
*
* @return:
*       0   -   success 
*      -2   -   INVALID_PARA 
*      -4   -   GET_FAILED 
*/
nwy_error_e nwy_pm_charge_state_get(int *state);

#ifdef __cplusplus
   }
#endif

#endif
