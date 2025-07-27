/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_dm_api.h
 * Author       : shuiying
 * Created      : 2024-5-6
 * Description  : nwy_nft_api API function declarations
 *
 *****************************************************************************
 */

#ifndef __NWY_NFT_H__
#define __NWY_NFT_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_common.h"
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
* Prototype     : nwy_nft_cal_get
* Description   : Get calibration flag.
* Output        : calibration_flag: Obtained calibration flag
* Return Value  : nwy_error_e
* Author        : shuiying
*****************************************************************************
*/
nwy_error_e nwy_nft_cal_get(int *calibration_flag);


/*
*****************************************************************************
* Prototype     : nwy_nft_fit_get
* Description   : Get comprehensive test flag.
* Output        : fit_flag: Obtained comprehensive test flag
* Return Value  : nwy_error_e
* Author        : shuiying
*****************************************************************************
*/
nwy_error_e nwy_nft_fit_get(int *fit_flag);
/*
*****************************************************************************
* Prototype     : nwy_nft_qcnversion_get
* Description   : Get QCN version.
* Output        : ver_buff: Obtained QCN version
* Return Value  : nwy_error_e
* Author        : shuiying
*****************************************************************************
*/
nwy_error_e nwy_nft_qcnversion_get(char* ver_buff);

/*
*****************************************************************************
* Prototype     : nwy_nft_led_set
* Description   : NFT LED test
* Input         : state 0:low 1:high
* Return Value  : nwy_error_e
* Author        : liangxuebin
*****************************************************************************
*/
nwy_error_e  nwy_nft_led_set(int id, int state);

/*
*****************************************************************************
* Prototype     : nwy_nft_led_get
* Description   : NFT LED test
* Output        : led state 0:low 1:high
* Return Value  : nwy_error_e
* Author        : liangxuebin
*****************************************************************************
*/
nwy_error_e  nwy_nft_led_get(int id, int* state);

/*
*****************************************************************************
* Prototype     : nwy_nft_uart_set
* Description   : NFT UART test
* Output        : ret 0:success -1:fail
* Return Value  : nwy_error_e
* Author        : laihongxiao
*****************************************************************************
*/
nwy_error_e  nwy_nft_uart_set(int fd, int mode);


#ifdef __cplusplus
   }
#endif

#endif

