#ifndef __NWY_USB_API_H__
#define __NWY_USB_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "nwy_common.h"

typedef enum {
 NWY_USB_NET_MODE_RNDIS = 0,
 NWY_USB_NET_MODE_ECM   = 1,

 NWY_USB_NET_MODE_NONE
}nwy_usb_net_mode_e;

/*
*****************************************************************************
* Prototype     : nwy_usb_net_mode_set
* Description   : set usb net mode, this config 
* Input         : mode
* Output        : NA
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
nwy_error_e nwy_usb_net_mode_set(nwy_usb_net_mode_e mode);

/*
*****************************************************************************
* Prototype     : nwy_usb_net_mode_get
* Description   : get usb net mode
* Input         : NA
* Output        : mode
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
nwy_error_e nwy_usb_net_mode_get(nwy_usb_net_mode_e *mode);

/*
*****************************************************************************
* Prototype     : nwy_usb_enable
* Description   : enable or disable usb
* Input         : 1-enable 0-disable
* Output        : NA
* Return Value  : nwy_error_e
* Author        : zhaoyong
*****************************************************************************
*/
nwy_error_e nwy_usb_enable(int enable);

#ifdef __cplusplus
   }
#endif

#endif // __NWY_USB_API_H__


