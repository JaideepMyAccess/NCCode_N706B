/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_dm_api.h
 * Author       : hujun
 * Created      : 2023-5-22
 * Description  : nwy_dm_api API declarations
 *
 *****************************************************************************
 */

#ifndef __NWY_DM_H__
#define __NWY_DM_H__

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_common.h"
#include "nwy_sim_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 *****************************************************************************
 * 2 Macro Definition
 *****************************************************************************
 */
#define NWY_DM_SN_LENGTH_MAX                        32
#define NWY_DM_APP_VER_LENGTH_MAX                   64
#define NWY_DM_APP_DEFAULE_VER                     ("")//Not allowed AT+NAPPCHECK? to get appversion if app not load
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
* Prototype     : nwy_dm_dev_model_get
* Description   : Get device model information
* Input         : buf_len: length of the buffer to store device information
* Output        : model_buf: buffer to receive device information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_dev_model_get(char *model_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_inner_version_get
* Description   : Get inner version information
* Input         : buf_len: length of the buffer to store version information
* Output        : version_buf: buffer to receive version information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_inner_version_get(char *version_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_open_sdk_version_get
* Description   : Get open SDK version information
* Input         : buf_len: length of the buffer to store version information
* Output        : version_buf: buffer to receive version information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_open_sdk_version_get(char *version_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_hw_version_get
* Description   : Get hardware version information
* Input         : buf_len: length of the buffer to store version information
* Output        : version_buf: buffer to receive version information
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_hw_version_get(char *version_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_rftemperature_get
* Description   : Get device RF temperature
* Input         : NA
* Output        : outvalue: buffer to receive RF temperature
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e  nwy_dm_rftemperature_get(float *outvalue);

/*
*****************************************************************************
* Prototype     : nwy_dm_sn_get
* Description   : Get device SN (serial number)
* Input         : len: length of the buffer to store SN
* Output        : sn: buffer to receive SN
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_sn_get(char *sn, int len);

/*
*****************************************************************************
* Prototype     : nwy_dm_imei_get
* Description   : Get device IMEI
* Input         : simid: SIM ID
*                 buf_len: length of the buffer to store IMEI
* Output        : imei_buf: buffer to receive IMEI
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_imei_get(nwy_sim_id_e simid, char* imei_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_imei_set
* Description   : Set device IMEI
* Input         : simid: SIM ID
*                 buf_len: length of the buffer to store IMEI
* Output        : imei_buf: buffer of IMEI
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_imei_set(nwy_sim_id_e simid, char* imei_buf, int buf_len);


/*
*****************************************************************************
* Prototype     : nwy_dm_app_version_set
* Description   : Set app version
* Input         : version_buf: buffer containing app version
*                 buf_len: length of the buffer
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_dm_app_version_set(char *version_buf, int buf_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_nvinfo_get
* Description   : Get NV information
* Input         : project_info: NV project name
*                project_len: Length of the project_info string
*                version_info: NV version name
*                version_len: Length of the version_info string
* Output        : NA
* Return Value  : nwy_error_e
* Author        : yangtianhan
*****************************************************************************
*/
nwy_error_e nwy_dm_nvinfo_get(char* project_info,int project_len, char* version_info, int version_len);

/*
*****************************************************************************
* Prototype     : nwy_dm_nwcfg_get
* Description   : Get nwcfg settings
* Input         : cfgname: eg "none_eitf"
* Output        : cfg_value: should be decoded as expected
* Return Value  : nwy_error_e
* Author        : cxk
*****************************************************************************
*/
nwy_error_e  nwy_dm_nwcfg_get(char *cfgname, void *cfg_value);

/*
*****************************************************************************
* Prototype     : nwy_dm_nwcfg_set
* Description   : Set nwcfg settings
* Input         : cfgname: eg "none_eitf"
* Output        : cfg_value: should be coded as expected
* Return Value  : nwy_error_e
* Author        : cxk
*****************************************************************************
*/
nwy_error_e  nwy_dm_nwcfg_set(char *cfgname, void *cfg_value);

typedef struct
{
    uint32 start;
    uint32 size;
    uint32 avail_size;
    uint32 max_block_size;
}nwy_heapinfo_t;
/*
*****************************************************************************
* Prototype     : nwy_dm_heapinfo
* Description   : Dynamic RAM remaining memory retrieval
* Input         : heapinfo
* Output        : heapinfo:
                    start: heap starting address
                    size: Total heap size
                    available_ram: RAM remaining available size
                    max_block_size: Maximum assignable size of RAM
* Return Value  : nwy_error_e
* Author        :
*****************************************************************************
*/
nwy_error_e nwy_dm_heapinfo(nwy_heapinfo_t * heapinfo);
/*
*****************************************************************************
* Prototype     : nwy_dm_soc_model_get
* Description   : get chip platform 
* Input         : 
* Output        : chip_platform:
                  buf_len: chip platform str len
* Return Value  : nwy_error_e
* Author        :
*****************************************************************************
*/

nwy_error_e nwy_dm_soc_model_get(char *chip_platform, int buf_len);

#ifdef __cplusplus
}
#endif

#endif

