#ifndef NWY_NW_LBS_H
#define NWY_NW_LBS_H
#include "nwy_common.h"
#include "nwy_wifi_api.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    NWY_LBS_STATUS_SUCCESS,
    NWY_LBS_STATUS_PKT_ERROR,
    NWY_LBS_STATUS_BUSY,
    NWY_LBS_STATUS_NET_ERROR,
    NWY_LBS_STATUS_SIM_STATUS_ERROR,
    NWY_LBS_STATUS_TIMEOUT,
    NWY_LBS_STATUS_SEND_FAIL,
    NWY_LBS_STATUS_RECV_FAIL,
    NWY_LBS_STATUS_CONNECT_FAIL,
    NWY_LBS_STATUS_DNS_FAIL,
    NWY_LBS_STATUS_RESP_ERROR,
    NWY_LBS_STATUS_UNKNOWN_ERROR
}NWY_LBS_STATUS_E;

typedef struct
{
    double lat;
    double lng;
    double alt;
}nwy_cipgsmloc_info_t;

typedef struct{
   NWY_LBS_STATUS_E result;    //0 == nwy_cipgsmloc_info_t, >NWY_LBS_STATUS_RESP_ERROR == errmsg
   union
   {
       nwy_cipgsmloc_info_t data;
       char errmsg[255];
   }info;
}nwy_lbs_result_t;

typedef void (*nwy_loc_cipgsmloc_callback)(nwy_lbs_result_t *text);


/*
*****************************************************************************
* Prototype     : nwy_lbs_wifi
* Description   : WiFi positioning
* Input         :
                 cid:1-7
                 scan_list: The result obtained from the wifi scanning interface needs to be called through the interface "nwy_wifi_scan()"
                 cb:Positioning result reporting function
* Output        : NA
* Return Value  : nwy_error_e
* Author        :

*note:
   1. The network registration status must be valid
   2. Must dial
   3. IMEI must be valid
   4.The following website can locate the current location based on the obtained longitude and latitude, and can be used to view the deviation size of the current location (Gaode Map)
https://lbs.amap.com/tools/picker
*****************************************************************************
*/
nwy_error_e nwy_lbs_wifi(int cid ,nwy_wifi_scan_list_t *scan_list,nwy_loc_cipgsmloc_callback cb);

/*
*****************************************************************************
* Prototype     : nwy_lbs_cip
* Description   : Base station positioning
* Input         :
                 cid:1-7
* Output        : NA
* Return Value  : nwy_error_e
* Author        :

*note:
   1. The network registration status must be valid
   2. Must dial
   3. IMEI must be valid
   4.The following website can locate the current location based on the obtained longitude and latitude, and can be used to view the deviation size of the current location (Gaode Map)
https://lbs.amap.com/tools/picker
*****************************************************************************
*/
nwy_error_e nwy_lbs_cip(int cid ,nwy_loc_cipgsmloc_callback cb);




#ifdef __cplusplus
   }
#endif

#endif


