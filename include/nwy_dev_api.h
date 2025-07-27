#ifndef __NWY_DEV_H__
#define __NWY_DEV_H__
#include "nwy_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NWY_DEV_NAME_MAX_LEN 8
typedef enum
{
    NWY_DEV_TYPE_UART = 0,
    NWY_DEV_TYPE_ADC,
    NWY_DEV_TYPE_I2C,
    NWY_DEV_TYPE_SPI,
    NWY_DEV_TYPE_MAX
}nwy_device_type_e;
/*---------------------------Function Definition--------------------------*/
 /**
 * @brief: get the dev name list
 *
 * @param option: 
 *      dev_type      - dev type
 *      dev_name_list - dev name buffer
 * @return:
 *      dev num
 */
int nwy_dev_name_list_get(nwy_device_type_e dev_type, char dev_name_list[][NWY_DEV_NAME_MAX_LEN]);
#ifdef __cplusplus
}
#endif

#endif/*__NWY_ADC_H__*/

