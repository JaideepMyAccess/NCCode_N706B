#ifndef __NWY_WIFI_API_H__
#define __NWY_WIFI_API_H__
#include "nwy_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NWY_WIFI_SCAN_TIMEOUT       300
#define NWY_WIFI_SCAN_AP_MAX        16
#define NWY_WIFI_SCAN_MAC_LEN       6
#define NWY_WIFI_SCAN_SSID_LEN_MAX  32

typedef struct
{
    uint8 mac[NWY_WIFI_SCAN_MAC_LEN];
    uint8 channel;     ///< channel id
    int  rssival;      ///< signal strength
    uint8 ssid[NWY_WIFI_SCAN_SSID_LEN_MAX+1];
}nwy_wifi_ap_info_t;

typedef struct
{
    int num;
    nwy_wifi_ap_info_t ap_list[NWY_WIFI_SCAN_AP_MAX];
}nwy_wifi_scan_list_t;

/*
*****************************************************************************
* Prototype     : nwy_wifi_scan
* Description   : Obtain WiFi scan results
* Input         : scan_list: Used to save WiFi scan results, external application memory cannot be empty
* Output        : scan_list: Used to save WiFi scan results, external application memory cannot be empty
* Return Value  : nwy_error_e
* Author        :

*note:
   1. When both LTE and GNSS (Global Navigation Satellite System) are not working, the Wi Fi Scan function can scan wireless network information normally.
   Generally, scanning can be completed in 1-5 seconds, and it will automatically close after scanning. The normal scan timeout is 30 seconds.
*****************************************************************************
*/
nwy_error_e nwy_wifi_scan(nwy_wifi_scan_list_t *scan_list);

#ifdef __cplusplus
   }
#endif

#endif

