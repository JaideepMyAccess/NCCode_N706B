#ifndef _NWY_LOG_API_H_
#define _NWY_LOG_API_H_
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define NWY_LOG_LEVEL_ERROR   0
#define NWY_LOG_LEVEL_WARN    1
#define NWY_LOG_LEVEL_INFO    2
#define NWY_LOG_LEVEL_DEBUG   3
#define NWY_LOG_LEVEL_VERBOSE   4
#define NWY_LOG_LEVEL_NEVER  5

/*
*****************************************************************************
* Prototype     : nwy_sdk_log_printf
* Description   : log printf
* Input         : tag:log level
                 format:Log output format
* Output        :
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
void nwy_sdk_log_printf(int level, const char *file, int line, const char *function, const char *format, ...);
/*
*****************************************************************************
* Prototype     : nwy_debug_uart_log_printf
* Description   : log printf to uart debug
* Input         : tag:log level
                 format:Log output format
* Output        :
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
void nwy_debug_uart_log_printf(int level, const char *file, int line, const char *function, const char *format, ...);


#define NWY_SDK_LOG_DEBUG(format, ...)   nwy_sdk_log_printf(NWY_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_SDK_LOG_INFO(format, ...)    nwy_sdk_log_printf(NWY_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_SDK_LOG_WARN(format, ...)    nwy_sdk_log_printf(NWY_LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_SDK_LOG_ERROR(format, ...)   nwy_sdk_log_printf(NWY_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_SDK_LOG_VERBOSE(format, ...)   nwy_sdk_log_printf(NWY_LOG_LEVEL_VERBOSE, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define NWY_UART_LOG_DEBUG(format, ...)   nwy_debug_uart_log_printf(NWY_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_UART_LOG_INFO(format, ...)    nwy_debug_uart_log_printf(NWY_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_UART_LOG_WARN(format, ...)    nwy_debug_uart_log_printf(NWY_LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_UART_LOG_ERROR(format, ...)   nwy_debug_uart_log_printf(NWY_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define NWY_UART_LOG_VERBOSE(format, ...)   nwy_debug_uart_log_printf(NWY_LOG_LEVEL_VERBOSE, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#ifdef __cplusplus
   }
#endif

#endif

