/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_vir_at_api.h
 * Author       : hujun
 * Created      : 2023-6-15
 * Description  : Virtual AT function declarations
 *
 *****************************************************************************
 */
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */

#ifndef __NWY_VIR_AT_H__
#define __NWY_VIR_AT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "nwy_common.h"

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_AT_BUF_MAX                  2048
#define NWY_AT_TIMEOUT_MAX              (30)//30S
#define NWY_AT_TIMEOUT_MIN              (0)//0S
#define NWY_AT_TIMEOUT_DEFAULT          (30)//30S
#define NWY_AT_UNISOLICITE_REG_MAX      (50)
#define NWY_AT_FORWARD_MAX  50

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
typedef int (*nwy_at_unsolicite_handle)(uint8 *data_str, int len);
typedef void (*nwy_get_original_at_rsp)(char *rsp, int len);
typedef void (*nwy_at_rsp_report_cb)(char *resp, int len);
/*********************************************************************
handle: uart or usb, or vir at handle
type:   0: AT_CMD_SET, 1: AT_CMD_TEST, 2: AT_CMD_READ; 3: AT_CMD_EXE;
**********************************************************************/
typedef void (*nwy_at_forward_process_cb)(void* handle, char* atcmd,int type, char* para0, char* para1, char* para2);


typedef struct nwy_at_info
{
    char at_command[NWY_AT_BUF_MAX + 1];
    char ok_fmt[64];
    char err_fmt[64];
    int ok_flag;
    int length;
}nwy_at_info_t;

typedef struct nwy_at_unsolicite_table
{
    char at_prefix[32];
    nwy_at_unsolicite_handle nwy_at_unsolicite_cb;
}nwy_at_unsolicite_table_t;

typedef struct nwy_report_info
{
    uint8 *report_data;
    int len;
}nwy_report_info_t;

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
* Prototype     : nwy_virt_at_parameter_init
* Description   : Initialize virtual AT
* Input         : NA
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_parameter_init(void);

/*
*****************************************************************************
* Prototype     : nwy_virt_at_cmd_send
* Description   : Send virtual AT command
* Input         : pInfo: Virtual AT information
                  resp_len: Response buffer length
                  timeout: Timeout time, default 30S
* Output        : resp: Response information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_cmd_send(nwy_at_info_t *pInfo, char *resp, int resp_len, int timeout);

/*
*****************************************************************************
* Prototype     : nwy_virt_at_unsolicited_cb_reg
* Description   : Register unsolicited report handler function
* Input         : at_prefix: Unsolicited report prefix
                  p_func: Unsolicited report handler function
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_unsolicited_cb_reg(char *at_prefix, void *p_func);

/*
*****************************************************************************
* Prototype     : nwy_virt_at_get_original_rsp_cb
* Description   : Register original AT response handler function
* Input         : cb: Handler function
* Output        : NA
* Return Value  : NA
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_get_original_rsp_cb(nwy_get_original_at_rsp cb);

/*
*****************************************************************************
* Prototype     : nwy_virt_at_forward_cb
* Description   : Register custom AT handler function
* Input         : index: Sequence number
                  name: AT name
                  cb: Handler function
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_forward_cb(int index,const char* name, nwy_at_forward_process_cb cb);

/*
*****************************************************************************
* Prototype     : nwy_virt_at_forward_send
* Description   : Send self-registered AT command
* Input         : handle: uart or usb, or vir at handle
                  buf: AT parameters
                  len: buf length
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_virt_at_forward_send(void* handle, char* buf, int len);

#ifdef __cplusplus
   }
#endif

#endif
