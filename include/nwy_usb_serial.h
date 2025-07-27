#ifndef __NWY_USB_SERIAL_H__
#define __NWY_USB_SERIAL_H__
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief function type to notify data in
 */
 typedef int (* nwy_sio_recv_cb_t)(void *data, size_t size);

/**
 * @brief register usb serial receive callback function
 *
 * @note data may not send to hardware right away
 *
 * Send data via usb serial, the data may not send to hardware directly
 * because the usb serial hw may be busy, and the data not sent to hw will
 * be cached to a software tx buffer.
 *
 * Therefore the return value include the data had been sent and the
 * data had been cached.
 * @param recv_cb  receive callback function pointer
 * @return      The number of bytes actually sent (or cached)
 *      - true      sucess
 *      - false     fail
 */
bool nwy_usb_serial_reg_recv_cb(nwy_sio_recv_cb_t recv_cb);
bool nwy_usb_nmea_reg_recv_cb(nwy_sio_recv_cb_t recv_cb);

/**
 * @brief send data via the usb serial
 *
 * @note data may not send to hardware right away
 *
 * Send data via usb serial, the data may not send to hardware directly
 * because the usb serial hw may be busy, and the data not sent to hw will
 * be cached to a software tx buffer.
 *
 * Therefore the return value include the data had been sent and the
 * data had been cached.
 * @param data  data buffer to be sent
 * @param size  data buffer size
 * @return      The number of bytes actually sent (or cached)
 */
int nwy_usb_serial_send(void *data, size_t size);
int nwy_usb_nmea_send(void *data, size_t size);

#ifdef __cplusplus
   }
#endif

#endif // __NWY_USB_SERIAL_H__

