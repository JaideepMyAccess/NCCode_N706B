#include "nwy_test_cli_adpt.h"
#include "nwy_test_cli_utils.h"

#define NWY_CLI_ECHO_LEN (2 * 1024)
#define NWY_CLI_RX_LEN (2 * 1024)


static char nwy_cli_opt_input_buff[NWY_CLI_RX_LEN] = {0};
static int nwy_ext_sio_len = 0;
static uint8 nwy_sio_mode = 0;
static nwy_sio_trans_cb nwy_test_cli_trans_func = NULL;

typedef struct dataRecvContext
{
    unsigned size;
    unsigned pos;
    char *data;
} dataRecvContext_t;
static dataRecvContext_t s_nwy_trans_recv;

void nwy_test_cli_echo(char *fmt, ...)
{
    static char echo_str[NWY_CLI_ECHO_LEN];
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
    vsnprintf(echo_str, NWY_CLI_ECHO_LEN, fmt, a);
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

char *nwy_test_cli_input_gets(char *msg, ...)
{
    static char echo_str[NWY_CLI_ECHO_LEN] = {0};

    va_list a;
    va_start(a, msg);
    vsprintf(echo_str, msg, a);
    va_end(a);
    nwy_usb_serial_send(echo_str, strlen((char *)echo_str));
    nwy_test_cli_wait_select();
    return nwy_cli_opt_input_buff;
}

int nwy_test_cli_input_len_gets()
{
    return nwy_ext_sio_len;
}

void nwy_test_cli_sio_data_proc(const char *data, uint32 length)
{
    static int len = 0;
    NWY_CLI_LOG("\r\nnwy length = %d, len = %d", length,len);
    NWY_CLI_LOG("\r\nnwy nwy_sio_mode = %d, size = %d", nwy_sio_mode,len + length);
    if (nwy_sio_mode == 0)
    {
        if (length == 2 && data[length - 1] == '\n' && data[length - 2] == '\r')
        {
            NWY_CLI_LOG("\r\ndrop \\r\\n and input end nwy_ext_sio_len = %d len = %d", nwy_ext_sio_len, len);
            if (len == 0)
            {
                memset(nwy_cli_opt_input_buff, 0, NWY_CLI_RX_LEN);
                nwy_ext_sio_len = 0;
            }
            len = 0;
            nwy_test_cli_echo("\r\n%s", nwy_cli_opt_input_buff);
            nwy_test_cli_select_enter();
            return;
        }
        // clear the nwy_ext_sio_recv_buff,in next time
        if (len == 0)
        {
            NWY_CLI_LOG("\r\nnwy reset buff");
            nwy_ext_sio_len = 0;
            memset(nwy_cli_opt_input_buff, 0, NWY_CLI_RX_LEN);
        }
        if (len + length > NWY_CLI_RX_LEN)
        {
            memcpy(nwy_cli_opt_input_buff + len, data, (NWY_CLI_RX_LEN - len));
            nwy_ext_sio_len = nwy_ext_sio_len + (NWY_CLI_RX_LEN - len);
            len = 0;
            nwy_test_cli_echo("\r\n%s", nwy_cli_opt_input_buff);
            nwy_test_cli_select_enter();
        }
        else
        {
            memcpy(nwy_cli_opt_input_buff + len, data, length);
            NWY_CLI_LOG("\r\nnwy length = %d, len = %d", length,len);

            if (length > 1)
            {
                if (data[length - 1] == '\n' && data[length - 2] == '\r')
                {
                    nwy_cli_opt_input_buff[len + length - 2] = '\0';
                    nwy_ext_sio_len = nwy_ext_sio_len + length - 2;
                    len = 0;

                    NWY_CLI_LOG("nwy: recv over nwy_ext_sio_len = %d", nwy_ext_sio_len);
                    nwy_test_cli_echo("\r\n%s", nwy_cli_opt_input_buff);
                    nwy_test_cli_select_enter();
                    return;
                }
                else
                {
                    nwy_ext_sio_len = nwy_ext_sio_len + length;
                }
            }
            else if (length == 1)
            {
                if (data[length - 1] == '\r' || data[length - 1] == '\n')
                {
                    nwy_cli_opt_input_buff[nwy_ext_sio_len - 1] = '\0';
                    nwy_ext_sio_len = nwy_ext_sio_len - 1;
                    len = 0;
                    nwy_test_cli_echo("\r\n%s", nwy_cli_opt_input_buff);
                    nwy_test_cli_select_enter();
                    return;
                }
                else
                {
                    memcpy(nwy_cli_opt_input_buff + len, data, length);
                    nwy_ext_sio_len = nwy_ext_sio_len + length;
                }
            }
        }
        len = len + length;
    }
    else if (nwy_sio_mode == 1)
    {
        nwy_test_cli_trans_func(data, length);
    }
}

int nwy_test_cli_sio_enter_trans_mode(nwy_sio_trans_cb recv_cb)
{
    if (recv_cb == NULL)
    {
        return -1;
    }

    if (nwy_sio_mode == 1)
    {
        return -1;
    }

    nwy_sio_mode = 1;
    nwy_test_cli_trans_func = recv_cb;
    return 0;
}

void nwy_test_cli_sio_quit_trans_mode()
{
    nwy_sio_mode = 0;
}


static void nwy_cli_data_trans_proc(const char *data, int length)
{
    if (length >= s_nwy_trans_recv.size - s_nwy_trans_recv.pos)
    {
        length = s_nwy_trans_recv.size - s_nwy_trans_recv.pos;
        NWY_CLI_LOG("\r\ninput data len > mem len so cut off %d", length);
    }

    memcpy((s_nwy_trans_recv.data + s_nwy_trans_recv.pos), data, length);
    s_nwy_trans_recv.pos = s_nwy_trans_recv.pos + length;
    nwy_test_cli_echo("\r\n nwy_app_info.rev_len = %d length = %d\r\n", s_nwy_trans_recv.pos, length);
    if (s_nwy_trans_recv.pos >= s_nwy_trans_recv.size)
    {
        nwy_test_cli_send_trans_end();
        nwy_test_cli_sio_quit_trans_mode();
    }
}

int nwy_cli_get_trans_data(char *data,int length)
{
    if(data == NULL || length<=0)
    {
        return -1;
    }
    memset(&s_nwy_trans_recv, 0, sizeof(dataRecvContext_t));
    s_nwy_trans_recv.data =data;
    s_nwy_trans_recv.size = length;
    s_nwy_trans_recv.pos = 0;
    nwy_test_cli_sio_enter_trans_mode(nwy_cli_data_trans_proc);
    nwy_test_cli_echo("\r\nplease input data");
    nwy_test_cli_wait_trans_end();
    return 0;
}

int nwy_cli_str_to_int(const char* str)
{
    // sign
    int flag = 1;

    if(str == NULL)
    return -1;
    // Empty String
    if (*str == '\0')
    {
        return -1;
    }

    // Process Numbers
    int ret = 0;
    while (*str != '\0')
    {
        if (* str>= '0' && * str <= '9')
        {
            ret = ret * 10 + flag * (*str - '0');
            // Determine if there is a storage range exceeding int
            if (ret > 0x7fffffff || ret < -0x7fffffff-1)
            {
                return -1;
            }
            else
            {
                ++str;
            }
        }
        else
        {
            return -1;
        }
     } // end of while

    return (int)ret;
}

