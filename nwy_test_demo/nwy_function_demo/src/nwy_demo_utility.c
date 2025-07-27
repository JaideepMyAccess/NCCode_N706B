
#include "nwy_demo_utility.h"


void nwy_demo_uart_echo(char *fmt, ...)
{
    static char echo_str[2*1024];
    static nwy_osi_mutex_t echo_mutex = NULL;
    va_list a;
    int i, size;

    if (NULL == echo_mutex) {
        nwy_sdk_mutex_create(&echo_mutex);
    }
    if (NULL == echo_mutex) {
        return;
    }
    nwy_sdk_mutex_lock(echo_mutex, NWY_OSA_SUSPEND);
    va_start(a, fmt);
    vsnprintf(echo_str, sizeof(echo_str), fmt, a);
    va_end(a);
    size = strlen((char *)echo_str);
    i = 0;
    while (1)
    {
        int tx_size;

        tx_size = nwy_usb_serial_send((char *)echo_str + i, size - i);
        if (tx_size <= 0)
            break;
        i += tx_size;
        if ((i < size))
            nwy_thread_sleep(10);
        else
            break;
    }

    nwy_sdk_mutex_unlock(echo_mutex);
}

