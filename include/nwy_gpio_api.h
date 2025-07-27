/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_gpio_api.c
 * Author       : Charlie
 * Created      : 2023-06-09
 * Description  : gpio API
 ******************************************************************************/
#ifndef __NWY_GPIO_API_H__
#define __NWY_GPIO_API_H__

#include "nwy_common.h"


#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
	PIN_NO_EDGE = 0,
	PIN_RISING_EDGE,
	PIN_FALLING_EDGE,
	PIN_BOTH_EDGE,
	PIN_HIGH_LEVEL,
	PIN_LOW_LEVEL,
} nwy_pin_edge_e;

typedef enum {
	PIN_DIRECTION_IN = 0,
	PIN_DIRECTION_OUT,
} nwy_dir_mode_e;

typedef enum {
	PIN_LEVEL_LOW = 0,
	PIN_LEVEL_HIGH,
} nwy_value_e;

typedef enum {
	PIN_PULL_DISABLE,
	PIN_PULL_PU,
	PIN_PULL_PD,
}nwy_pin_pull_e;

/**
 * @brief: get gpio direction
 *
 * @param option: 
 *		gpio_id		-	gpio id
 * @return:
 *      0	-	input  				 
 *      1	-	output   
 *     -4	-	FAILED   
 */
int nwy_gpio_direction_get(uint32_t gpio_id);

/**
 * @brief: set gpio direction
 *
 * @param option: 
 *		gpio_id		-	gpio id
 *		dirct		-	gpio direction
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_direction_set(uint32_t gpio_id, nwy_dir_mode_e dirct);

/**
 * @brief: get gpio value
 *
 * @param option: 
 *		gpio_id		-	gpio id
 * @return:
 *      0	-	low level		 
 *      1	-	high level    
 *     -4	-	FAILED   
 */
int nwy_gpio_value_get(uint32_t gpio_id);

/**
 * @brief: set gpio value
 *
 * @param option: 
 *		gpio_id		-	gpio id
 *		value		-	gpio value
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED     
 */
nwy_error_e nwy_gpio_value_set(uint32_t gpio_id, nwy_value_e value);

/**
 * @brief: get gpio pull state
 *
 * @param option: 
 *		gpio_id		-	gpio id
 * @return:
 *      0	-	no pullup and pulldown		 
 *      1	-	pullup    
 *      2	-	pulldown   
 *     -4	-	FAILED  
 */
int nwy_gpio_pull_get (uint32_t gpio_id);

/**
 * @brief: set gpio pull state
 *
 * @param option: 
 *		gpio_id		-	gpio id
 *		pull		-	pull state
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_pull_set (uint32_t gpio_id, nwy_pin_pull_e pull);

/**
 * @brief: register gpio irq
 *
 * @param option: 
 *		gpio_id				-	gpio id
 *		pin_edge			-	irq trigger mode
 *		pin_pull			-	pull config
 *		eint_cb				-	irq callback function
 *		wakeup_eint_cb		-	wakeup irq callback function from sleep
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_irq_register (uint32_t gpio_id, nwy_pin_edge_e pin_edge, nwy_pin_pull_e pin_pull, void *eint_cb, void *wakeup_eint_cb); 

/**
 * @brief: enable gpio irq
 *
 * @param option: 
 *		gpio_id				-	gpio id
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_irq_enable(uint32_t gpio_id);

/**
 * @brief: disable gpio irq
 *
 * @param option: 
 *		gpio_id				-	gpio id
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_irq_disable(uint32_t gpio_id);

/**
 * @brief: enable wakeup gpio irq
 *
 * @param option: 
 *		gpio_id				-	gpio id
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_irq_wakeup_enable (uint32_t gpio_id);

/**
 * @brief: disable wakeup gpio irq
 *
 * @param option: 
 *		gpio_id				-	gpio id
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_irq_wakeup_disable (uint32_t gpio_id);

/**
 * @brief: close gpio
 *
 * @param option: 
 *		gpio_id				-	gpio id
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_gpio_close(uint32_t gpio_id);

#ifdef __cplusplus
}
#endif

#endif

