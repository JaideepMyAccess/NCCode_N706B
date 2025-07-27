/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_uart_api.h
 * Author       : Charlie
 * Created      : 2023-06-09
 * Description  : uart API
 ******************************************************************************/
#ifndef __NWY_UART_API_H__
#define __NWY_UART_API_H__
#include "nwy_common.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FC_NONE = 0,  // None Flow Control
    FC_RTSCTS,    // Hardware Flow Control (rtscts)
    FC_XONXOFF    // Software Flow Control (xon/xoff)
}nwy_flowctrl_e;

typedef enum {
    PB_NONE = 0,
    PB_ODD,
    PB_EVEN,
    PB_SPACE,
    PB_MARK
}nwy_paritybits_e;

typedef struct {
    unsigned int baudrate; 
    unsigned int databits;
    unsigned int stopbits;
    nwy_paritybits_e parity;
    nwy_flowctrl_e flowctrl;
}nwy_uartdcb_t;

typedef enum {
    BR_300 		= 300,
    BR_600 		= 600,
    BR_1200 	= 1200,
    BR_2400 	= 2400,
    BR_4800     = 4800,
    BR_9600 	= 9600,
    BR_19200 	= 19200,
    BR_38400 	= 38400,
    BR_57600 	= 57600,
    BR_115200   = 115200,
    BR_230400   = 230400,
    BR_460800   = 460800,
    BR_921600   = 921600
}nwy_baudrate_e;

typedef enum
{
	NWY_UART_MODE_AT = 0,
	NWY_UART_MODE_DATA = 1
} nwy_uart_mode_e;

typedef void (*nwy_uart_rx_callback) (const char *str,uint32_t length);
typedef void(*nwy_uart_tx_callback)(int param);

#define NWY_MAKE_TAG(a, b, c, d) ((unsigned)(a) | ((unsigned)(b) << 8) | ((unsigned)(c) << 16) | ((unsigned)(d) << 24))

#define NWY_NAME_UART1    NWY_MAKE_TAG('U', 'R', 'T', '1')
#define NWY_NAME_UART2    NWY_MAKE_TAG('U', 'R', 'T', '2')
#define NWY_NAME_UART3    NWY_MAKE_TAG('U', 'R', 'T', '3')
#define NWY_NAME_UART6    NWY_MAKE_TAG('U', 'R', 'T', '6')

/**
 * @brief: creat uart device and initialize
 *
 * @param option: 
 *		port		-	uart device name
 *		baudrate	-	baudrate control
 *		flowctrl	-	flow control
 * @return:
 *     >=0	-	uart device number   				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
int nwy_uart_open(const char* port, unsigned int baudrate, nwy_flowctrl_e flowctrl);
/**
 * @brief: uart read data
 *
 * @param option: 
 *		fd		-	device number
 *		buf		-	data storage location
 *		buf_len	-	data storage length
 * @return:
 *     -1	-	INVALID PARA   				 
 *     >=0	-	The number of bytes actually receive from UART       
 */
int nwy_uart_read (int fd,unsigned char* buf, unsigned int buf_len);
/**
 * @brief: uart write data
 *
 * @param option: 
 *		fd		-	device number
 *		buf		-	data storage location
 *		buf_len	-	data storage length
 * @return: 				 
 *     -1	-	INVALID PARA    
 *     >=0	-	The number of bytes actually sent (or cached)   
 */
int nwy_uart_write(int fd, const unsigned char* buf, unsigned int buf_len);
/**
 * @brief: set uart dcb parameter
 *
 * @param option: 
 *		fd		-	device number
 *		dcb		-	dcb storage location
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_uart_dcb_set(int fd, nwy_uartdcb_t *dcb);
/**
 * @brief: get uart dcb parameter
 *
 * @param option: 
 *		fd		-	device number
 *		dcb		-	dcb storage location
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED
 */
nwy_error_e nwy_uart_dcb_get(int fd, nwy_uartdcb_t *dcb);
/**
 * @brief: close uart device
 *
 * @param option: 
 *		fd		-	device number
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED
 */
nwy_error_e nwy_uart_close(int fd);
/**
 * @brief: register rx callback func
 *
 * @param option: 
 *		fd		-	device number
 *		cb		-	NULL 	 : polling mode
 *					not NULL : interupt mode
 * @return:
 *      void   				 
 */
nwy_error_e nwy_uart_rx_cb_register(int fd, nwy_uart_rx_callback cb);

/**
 * @brief: register tx callback func
 *
 * @param option: 
 *		fd		-	device number
 *		cb		-	NULL 	 : polling mode
 *					not NULL : interupt mode
 * @return:
 *      void   				 
 */
nwy_error_e nwy_uart_tx_cb_register(int fd, nwy_uart_tx_callback cb);

/**
 * @brief: set uart_rx frame timeout
 *
 * @param option: 
 *		fd		-	device number
  *		time	-	timeout
 * @return:
 *      0	-	SUCCESS    				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED
 */
nwy_error_e nwy_uart_rx_frame_timeout_set(int fd, unsigned int time);


#ifdef __cplusplus
   }
#endif

#endif

