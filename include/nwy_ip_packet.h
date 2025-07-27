#ifndef __NWY_IP_PACKET_H__
#define __NWY_IP_PACKET_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*nwy_get_ip_packet)(uint8_t *data,int len);
/*
*****************************************************************************
* Prototype     : nwy_get_recv_ip_packet_reg
* Description   : Obtain the bare IP recv packet
* Input         :
                 cb:register the callback function and report the IP package
* Output        : NA
* Return Value  : NA
* Author        : shuiying
*****************************************************************************
*/


void nwy_get_recv_ip_packet_reg(nwy_get_ip_packet cb);
/*
*****************************************************************************
* Prototype     : nwy_get_send_ip_packet_reg
* Description   : Obtain the bare IP send packet
* Input         :
                 cb:register the callback function and report the IP package
* Output        : NA
* Return Value  : NA
* Author        : shuiying
*****************************************************************************
*/

void nwy_get_send_ip_packet_reg(nwy_get_ip_packet cb);


#ifdef __cplusplus
}
#endif

#endif
