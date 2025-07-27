/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_fota_api.h
 * Author       : wenjingshun
 * Created      : 2023-5-22
 * Description  : nwy_fota_api API
 *
 *****************************************************************************
 */
#ifndef __NWY_FOTA_API_H__
#define __NWY_FOTA_API_H__
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
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */


/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum{
    NWY_FOTA_REBOOT_MANUAL  = 0,        //start fota update after manual reboot device
    NWY_FOTA_REBOOT_NOW     = 1,        //immediately reboot device, then start fota update
    NWY_FOTA_REBOOT_MAX
}nwy_fota_reboot_mode_e;

typedef enum{
    NWY_FOTA_RESULT_NONE    = 0,
    NWY_FOTA_RESULT_SUCCESS = 1,        //fota result success
    NWY_FOTA_RESULT_FAIL    = 2,        //fota result failed
    NWY_FOTA_RESULT_MAX
}nwy_fota_result_e;

typedef enum{
    NWY_FOTA_DOWNLOAD_HTTP    = 0,
    NWY_FOTA_DOWNLOAD_FTP = 1,
    NWY_FOTA_DOWNLOAD_MAX
}nwy_fota_download_mode_e;

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
typedef struct {
    uint32 total_size;      //ota package total size (byte)
    uint32 offset;          //data will be written to device by offset
    uint32 len;             //the length of data
    uint8  *data;           //ota data
} nwy_ota_package_t;

typedef struct {
    char *url;
    int port;
}nwy_ota_http_param_t;

typedef struct {
    char *ip;
    int port;
    char *username;
    char *password;
    char *filename;
}nwy_ota_ftp_param_t;

typedef struct {
    nwy_ota_http_param_t  http_param;
    nwy_ota_ftp_param_t   ftp_param;
    nwy_fota_download_mode_e dl_mode;
} nwy_ota_download_param_t;

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
 * Prototype     : nwy_fota_write
 * Description   : write fota pack
 * Input         : ota_pack
 * Output        : None
 * Return Value  : write successful length
 * Author        : yixudong
 *****************************************************************************
*/
int nwy_fota_write (nwy_ota_package_t *ota_pack);

/*
 *****************************************************************************
 * Prototype     : nwy_fota_update
 * Description   : check fota pack and start fota 
 * Input         : mode: 0: manual reboot, 1:reboot now
 * Output        : None
 * Return Value  : fota pack check result
 * Author        : yixudong
 *****************************************************************************
*/
nwy_error_e nwy_fota_update (int mode);

/*
 *****************************************************************************
 * Prototype     : nwy_fota_result_get
 * Description   : get fota result
 * Input         : result: 1: fota success 2: fota fail
 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : yixudong
 *****************************************************************************
*/
nwy_error_e nwy_fota_result_get (int *result);

/*
 *****************************************************************************
 * Prototype     : nwy_fota_base_ver_update
 * Description   : fota upgrade base version 
 * Input         : param
 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : lileilei
 *****************************************************************************
*/
nwy_error_e nwy_fota_base_ver_update(nwy_ota_download_param_t* param);

#ifdef __cplusplus
   }
#endif

#endif

