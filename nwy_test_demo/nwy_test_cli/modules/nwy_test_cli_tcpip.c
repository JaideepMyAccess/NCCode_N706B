#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

#define NWY_TCP_SOCKET_MAX 5
#define NWY_UDP_SOCKET_MAX 5
#define NWY_BUFF_MAX 128
#define NWY_THREAD_STACK_SIZE  (3*1024)

typedef enum
{
    NWY_CUSTOM_IP_TYPE_OR_DNS_NONE = -1,
    NWY_CUSTOM_IP_TYPE_OR_DNS_IPV4 = 0,
    NWY_CUSTOM_IP_TYPE_OR_DNS_IPV6 = 1,
    NWY_CUSTOM_IP_TYPE_OR_DNS_DNS = 2
} nwy_ip_type_or_dns_enum;

//socket
static int s_nwy_cli_tcp_fd = 0;
static int s_nwy_cli_udp_fd = 0;
static int nwy_tcp_default_cid = 0;
static int s_nwy_tcp_connect_flag = 0;
static int s_nwy_udp_connect_flag = 0;
static char g_udp_manual_close = 0;
nwy_osi_thread_t s_nwy_tcp_recv_thread = NULL;
nwy_osi_thread_t s_nwy_udp_recv_thread = NULL;
char s_nwy_tcp_recv_thread_stack[NWY_THREAD_STACK_SIZE] = {0};
char s_nwy_udp_recv_thread_stack[NWY_THREAD_STACK_SIZE] = {0};
static struct sockaddr_in s_nwy_server_v4 = {0};
static struct sockaddr_in6 s_nwy_server_v6 = {0};
static int s_nwy_inet_flag = AF_INET;

static nwy_osi_thread_t tcp_recv_thread_server = NULL;
static int tcp_listen_flag = 0;

#define NWY_TCP_ACCEPT_MAX 5

static int g_client_sockets[NWY_TCP_ACCEPT_MAX] = {-1, -1, -1, -1, -1}; 
static int g_server_socketfd = 0;

static bool nwy_cli_check_data_connect(int cid)
{

    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_callinfo_t addr_info = {0};
    ret = nwy_data_call_info_get(cid, &addr_info);
    if (ret == NWY_SUCCESS && addr_info.state == NWY_DATA_CALL_CONNECTED_STATE)
    {
        return true;
    }

    return false;
}
#if 0
//extern int nwy_test_cli_check_data_connect();
static bool nwy_cli_check_str_isdigit(char *str)
{
    if (NULL == str)
        return false;
    char *p = str;
    while ('\0' != *p)
    {
        if (!('0' <= *p && '9' >= *p))
            return false;
        p++;
    }
    return true;
}
static nwy_ip_type_or_dns_enum nwy_judge_ip_or_dns(char *str)
{
    int len = 0;
    int strLen = 0;
    nwy_ip_type_or_dns_enum retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_DNS;
    if (str == NULL)
    {
        return NWY_CUSTOM_IP_TYPE_OR_DNS_NONE;
    }
    else
    {
        if (strlen(str) <= 0)
        {
            return NWY_CUSTOM_IP_TYPE_OR_DNS_NONE;
        }
    }
    strLen = strlen(str);
    for (len = 0; len < strLen; len++)
    {
        if (((*(str + len) >= '0') && (*(str + len) <= '9')) || (*(str + len) == '.'))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (len == strLen)
    {
        retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_IPV4;
        return retValue;
    }
    len = 0;
    for (len = 0; len < strLen; len++)
    {
        if (((*(str + len) >= '0') && (*(str + len) <= '9')) ||
            ((*(str + len) >= 'a') && (*(str + len) <= 'f')) ||
            ((*(str + len) >= 'A') && (*(str + len) <= 'F')) ||
            (*(str + len) == ':'))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (len == strLen)
    {
        retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_IPV6;
        return retValue;
    }
    return retValue;
}
#endif

static int nwy_hostname_check(char *hostname)
{
    int a, b, c, d;
    char temp[32] = {0};
    if (strlen(hostname) > 15)
        return NWY_GEN_E_UNKNOWN;
    if ((sscanf(hostname, "%d.%d.%d.%d", &a, &b, &c, &d)) != 4)
        return NWY_GEN_E_UNKNOWN;
    if (!((a <= 255 && a >= 0) && (b <= 255 && b >= 0) && (c <= 255 && c >= 0)))
        return NWY_GEN_E_UNKNOWN;
    sprintf(temp, "%d.%d.%d.%d", a, b, c, d);

    strcpy(hostname, temp);
    return NWY_SUCCESS;
}

static int nwy_port_get(char *port_str, int *port)
{
    int i;
    if (port == NULL)
    {
        return NWY_GEN_E_INVALID_PARA;
    }
    for (i = 0; i < strlen(port_str); i++)
    {
        if (*(port_str + i) == '\r' || *(port_str + i) == '\n')
        {
            continue;
        }
        if ((port_str[i]) < '0' || (port_str[i]) > '9')
        {
            return NWY_GEN_E_INVALID_PARA;
        }
    }

    *port = atoi(port_str);
    if (*port < 1 || *port > 65535)
    {
        *port = 0;
        return NWY_GEN_E_INVALID_PARA;
    }
    else
    {
        return NWY_SUCCESS;
    }
}

static int nwy_cli_socket_destory(int *socketid)
{
    int ret = 0;
    if (*socketid == 0) {
        return;
    }
    ret = nwy_socket_close(*socketid);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nSocket close fail");
        return ret;
    }
    *socketid = 0;
    nwy_test_cli_echo("\r\nSocket close sucess");
    return ret;
}
void nwy_cli_tcp_recv_func(void *param)
{
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0, result = 0;
    fd_set rd_fd;
    fd_set ex_fd;
    FD_ZERO(&rd_fd);
    FD_ZERO(&ex_fd);
    FD_SET(s_nwy_cli_tcp_fd, &rd_fd);
    FD_SET(s_nwy_cli_tcp_fd, &ex_fd);
    struct timeval tv = {0};
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    while (1)
    {
        if (s_nwy_cli_tcp_fd <=0) {
            nwy_thread_sleep(1000);
            continue;
        }

        if (!nwy_cli_check_data_connect(nwy_tcp_default_cid)) {
            nwy_test_cli_echo("\r\nData call disconnect");
            nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
            s_nwy_tcp_connect_flag = 0;
            nwy_thread_sleep(1000);
            continue;
        }

        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        FD_SET(s_nwy_cli_tcp_fd, &rd_fd);
        FD_SET(s_nwy_cli_tcp_fd, &ex_fd);
        result = nwy_socket_select(s_nwy_cli_tcp_fd + 1, &rd_fd, NULL, &ex_fd, &tv);
        if (result < 0)
        {
            NWY_CLI_LOG("\r\ntcp select error:\r\n");
            nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
            s_nwy_tcp_connect_flag = 0;
            nwy_thread_sleep(1000);
            continue;
        }
        else if (result > 0)
        {
            if (FD_ISSET(s_nwy_cli_tcp_fd, &rd_fd))
            {
                memset(recv_buff, 0, NWY_UART_RECV_SINGLE_MAX + 1);
                recv_len = nwy_socket_recv(s_nwy_cli_tcp_fd, recv_buff, NWY_UART_RECV_SINGLE_MAX, 0);
                if (recv_len > 0)
                    nwy_test_cli_echo("\r\ntcp read:%d:%s\r\n", recv_len, recv_buff);
                else if (recv_len == 0)
                {
                    nwy_test_cli_echo("\r\ntcp close by server\r\n");
                    nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
                    s_nwy_tcp_connect_flag = 0;
                }
                else
                {
                    nwy_test_cli_echo("\r\ntcp connection error:%d\r\n",recv_len);
                    nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
                    s_nwy_tcp_connect_flag = 0;
                }
            }
            if (FD_ISSET(s_nwy_cli_tcp_fd, &ex_fd))
            {
                nwy_test_cli_echo("\r\ntcp select ex_fd:\r\n");
                nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
                s_nwy_tcp_connect_flag = 0;
            }
        }
        else
            NWY_CLI_LOG("\r\ntcp select timeout:\r\n");
        nwy_thread_sleep(1000);
    }
}
void nwy_cli_udp_recv_func(void *param)
{
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0, result = 0;
    fd_set rd_fd;
    fd_set ex_fd;
    FD_ZERO(&rd_fd);
    FD_ZERO(&ex_fd);
    FD_SET(s_nwy_cli_udp_fd, &rd_fd);
    FD_SET(s_nwy_cli_udp_fd, &ex_fd);
    struct timeval tv = {0};
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    while (1)
    {
        if (s_nwy_cli_udp_fd <=0) {
            nwy_thread_sleep(1000);
            continue;
        }

       if (!nwy_cli_check_data_connect(nwy_tcp_default_cid)) {
            nwy_test_cli_echo("\r\nData call disconnect");
            nwy_cli_socket_destory(&s_nwy_cli_udp_fd);
            s_nwy_udp_connect_flag = 0;
            nwy_thread_sleep(1000);
            continue;
        }

        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        FD_SET(s_nwy_cli_udp_fd, &rd_fd);
        FD_SET(s_nwy_cli_udp_fd, &ex_fd);
        result = nwy_socket_select(s_nwy_cli_udp_fd + 1, &rd_fd, NULL, &ex_fd, &tv);
        if (result < 0)
        {
            NWY_CLI_LOG("\r\nudp select error:\r\n");
            nwy_cli_socket_destory(&s_nwy_cli_udp_fd);
            s_nwy_udp_connect_flag = 0;
            nwy_thread_sleep(1000);
            continue;
        }
        else if (result > 0)
        {
            if (FD_ISSET(s_nwy_cli_udp_fd, &rd_fd))
            {
                memset(recv_buff, 0, 512);
                recv_len = nwy_socket_recv(s_nwy_cli_udp_fd, recv_buff, sizeof(recv_buff), 0);
                if (recv_len > 0)
                    nwy_test_cli_echo("\r\nudp read:%d:%s\r\n", recv_len, recv_buff);
                else if (recv_len == 0)
                    nwy_test_cli_echo("\r\nudp close by server\r\n");
                else
                {
                    if (g_udp_manual_close) {
                        continue;
                    }
                    nwy_test_cli_echo("\r\nudp connection error\r\n");
                    nwy_cli_socket_destory(&s_nwy_cli_udp_fd);
                    s_nwy_udp_connect_flag = 0;
                    nwy_thread_sleep(1000);
                    continue;
                }
            }
            if (FD_ISSET(s_nwy_cli_udp_fd, &ex_fd))
            {
                nwy_test_cli_echo("\r\nudp select ex_fd:\r\n");
                nwy_cli_socket_destory(&s_nwy_cli_udp_fd);
                s_nwy_udp_connect_flag = 0;
                nwy_thread_sleep(1000);
                continue;
            }
        }
        else
        {
        } //nwy_test_cli_echo("\r\nudp select timeout:\r\n");
    }
}
static int nwy_cli_tcp_connect(struct sockaddr_in *sa_v4, struct sockaddr_in6 *sa_v6, int af_inet_flag)
{
    uint64_t start = 0;
    int ret = NWY_GEN_E_INVALID_PARA;
    start = nwy_uptime_get();
    NWY_CLI_LOG("nwy_cli_tcp_connect enter");
    do
    {
        if ((nwy_uptime_get() - start) >= 10000)
        {
            nwy_test_cli_echo("\r\nsocket connect timeout\r\n");
            return NWY_GEN_E_UNKNOWN;
        }
        if (af_inet_flag == AF_INET6)
        {
            ret = nwy_socket_connect(s_nwy_cli_tcp_fd, (struct sockaddr *)sa_v6, sizeof(struct sockaddr_in6));
        }
        else
        {
            ret = nwy_socket_connect(s_nwy_cli_tcp_fd, (struct sockaddr *)sa_v4, sizeof(struct sockaddr_in));
        }
        NWY_CLI_LOG("nwy_cli_tcp_connect enter ret = %d error = %d", ret, nwy_socket_errno());
        if (ret != NWY_SUCCESS)
        {
            //nwy_test_cli_echo("\r\nnwy_cli_tcp_connect errno:%d",nwy_socket_errno());
            if (EISCONN == nwy_socket_errno())
            {
                nwy_test_cli_echo("\r\nnwy_net_connect_tcp connect ok..");
                s_nwy_tcp_connect_flag = 1;
                break;
            }
            else if(EALREADY == nwy_socket_errno() || EINPROGRESS == nwy_socket_errno())
            {
                nwy_test_cli_echo("\r\nnwy_net_connect_tcp connecting");
            }
            else
            {
                nwy_test_cli_echo("\r\nconnect errno = %d", nwy_socket_errno());
                nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
                break;
            }
        } else {
                nwy_test_cli_echo("\r\nnwy_net_co nnect_tcp connect ok..");
                s_nwy_tcp_connect_flag = 1;
                break;
        }
        nwy_thread_sleep(1000);
    } while (1);
    NWY_CLI_LOG("nwy_cli_tcp_connect s_nwy_tcp_connect_flag = %d", s_nwy_tcp_connect_flag);
    if (s_nwy_tcp_connect_flag)
    {
        nwy_test_cli_echo("\r\n[%s]TCP state:%d",__func__,nwy_socket_get_state(s_nwy_cli_tcp_fd));
        if (s_nwy_tcp_recv_thread == NULL) {
            nwy_thread_create(&s_nwy_tcp_recv_thread, "s_nwy_cli_recv_thread",NWY_OSI_PRIORITY_NORMAL, nwy_cli_tcp_recv_func, (void *)&s_nwy_cli_tcp_fd, 8,NWY_THREAD_STACK_SIZE*2, NULL);
            if (s_nwy_tcp_recv_thread == NULL)
            {
                nwy_test_cli_echo("\r\ncreate tcp recv thread failed, close connect");
                nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
                s_nwy_tcp_connect_flag = 0;
            }
        }

        return NWY_SUCCESS;
    }
    return NWY_GEN_E_UNKNOWN;
}
static int nwy_cli_socket_setup_pro(char *url_or_ip, int port, int socekt_type,int cid)
{
    int isipv6 = 0;
    int fd = 0;
    int on = 1;
    int opt = 1;
    int ret = NWY_GEN_E_UNKNOWN;
    char * ip_buf= NULL;

    if (url_or_ip == NULL)
    {
        return NWY_GEN_E_INVALID_PARA;
    }
    nwy_test_cli_echo("\r\nurl test start:%s:%d\r\n", url_or_ip, port);

    ip_buf = nwy_socket_gethost_by_name(url_or_ip, &isipv6, cid);
    if (ip_buf == NULL || 0 == strlen(ip_buf))
    {
        nwy_test_cli_echo("\r\nDomain name resolution failed:%s\r\n", url_or_ip);
        return NWY_GEN_E_UNKNOWN;
    }

    if (isipv6)
    {
        memset(&s_nwy_server_v6, 0, sizeof(s_nwy_server_v6));
        if (nwy_socket_inet_pton(AF_INET6, ip_buf, (void *)&s_nwy_server_v6.sin6_addr) < 0)
        {
            nwy_test_cli_echo("\r\ninput ip or url is invalid");
            return NWY_GEN_E_INVALID_PARA;
        }

        s_nwy_server_v6.sin6_len = sizeof(struct sockaddr_in6);
        s_nwy_server_v6.sin6_family = AF_INET6;
        s_nwy_server_v6.sin6_port = htons(port);
        s_nwy_inet_flag = AF_INET6;

    }
    else
    {
        memset(&s_nwy_server_v4, 0, sizeof(s_nwy_server_v4));
        ret = nwy_hostname_check(ip_buf);
        if (ret != NWY_SUCCESS)
        {
            nwy_test_cli_echo("\r\ninput ip or url is invalid");
            return NWY_GEN_E_INVALID_PARA;
        }

        if (nwy_socket_inet_pton(AF_INET, ip_buf, (void *)&s_nwy_server_v4.sin_addr) < 0)
        {
            nwy_test_cli_echo("\r\ninput ip error:\r\n");
            return NWY_GEN_E_INVALID_PARA;
        }
        s_nwy_server_v4.sin_len = sizeof(struct sockaddr_in);
        s_nwy_server_v4.sin_family = AF_INET;
        s_nwy_server_v4.sin_port = htons(port);
        s_nwy_inet_flag = AF_INET;
    }

    if (socekt_type == IPPROTO_TCP)
    {
        if (s_nwy_cli_tcp_fd <= 0)
        {
            s_nwy_cli_tcp_fd = nwy_socket_open(s_nwy_inet_flag, SOCK_STREAM, socekt_type, cid);
            if (fd < 0)
            {
                nwy_test_cli_echo("\r\n socket open fail:\r\n");
                return NWY_GEN_E_INVALID_PARA;
            }
        }

        NWY_CLI_LOG("nwy s_nwy_cli_tcp_fd = %d", s_nwy_cli_tcp_fd);
        nwy_socket_setsockopt(s_nwy_cli_tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));
        nwy_socket_setsockopt(s_nwy_cli_tcp_fd, IPPROTO_TCP, TCP_NODELAY, (void *)&opt, sizeof(opt));
        if (0 != nwy_socket_set_nonblock(s_nwy_cli_tcp_fd))
        {
            nwy_test_cli_echo("\r\nsocket set err\r\n");
            return NWY_GEN_E_INVALID_PARA;
        }

        ret = nwy_cli_tcp_connect(&s_nwy_server_v4, &s_nwy_server_v6, s_nwy_inet_flag);
        if (ret == NWY_SUCCESS)
        {
            nwy_test_cli_echo("\r\nsocket set up sucess\r\n");
        }
    }
    else if (socekt_type == IPPROTO_UDP)
    {
        if (s_nwy_cli_udp_fd <= 0)
        {
            s_nwy_cli_udp_fd = nwy_socket_open(s_nwy_inet_flag, SOCK_DGRAM, socekt_type, cid);
            if (s_nwy_cli_udp_fd <= 0)
            {
                nwy_test_cli_echo("\r\nsocket open fail:\r\n");
                return NWY_GEN_E_INVALID_PARA;
            }
        }

        if (s_nwy_udp_recv_thread == NULL) {
            nwy_thread_create(&s_nwy_udp_recv_thread, "nwy_udp_recv_thread",NWY_OSI_PRIORITY_NORMAL, nwy_cli_udp_recv_func, (void *)&s_nwy_cli_udp_fd, 8,NWY_THREAD_STACK_SIZE*2, NULL);
            if (s_nwy_udp_recv_thread == NULL)
            {
                nwy_test_cli_echo("\r\ncreate udp recv thread failed");
                return NWY_GEN_E_INVALID_PARA;
            }
        }
        s_nwy_udp_connect_flag = 1;
        nwy_test_cli_echo("\r\nUDP setup successful");

    }
    return ret;
}
/**************************TCP*********************************/
void nwy_test_cli_tcp_setup()
{
    char *tmp = NULL;
    char url_or_ip[128 + 1] = {0};
    int port = 0;
    int ret = 0;
    char *sptr = NULL;

    if (s_nwy_cli_tcp_fd > 0)
    {
        nwy_test_cli_echo("\r\nSocket has been setup");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    nwy_tcp_default_cid= nwy_cli_str_to_int(sptr);
    if (nwy_tcp_default_cid< 1 || nwy_tcp_default_cid> 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    if (!nwy_cli_check_data_connect(nwy_tcp_default_cid)) {
        nwy_test_cli_echo("\r\nData call not connect");
        return;
    }

    tmp = nwy_test_cli_input_gets("\r\nPlease input url: ");
    memcpy(url_or_ip, tmp, strlen(tmp));
    tmp = nwy_test_cli_input_gets("\r\nPlease input port: ");
    if (nwy_port_get(tmp, &port) != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nInput port error");
        return;
    }

    ret = nwy_cli_socket_setup_pro(url_or_ip, port, IPPROTO_TCP,nwy_tcp_default_cid);
    if (ret != NWY_SUCCESS)
    {
        nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
        nwy_test_cli_echo("\r\nTCP setup error");
    }
    nwy_test_cli_echo("\r\n[%s]TCP state:%d",__func__,nwy_socket_get_state(s_nwy_cli_tcp_fd));
}
void nwy_test_cli_tcp_send()
{
    int send_len = 0;
    int len = 0;
    char *buff = NULL;
    if (!s_nwy_tcp_connect_flag)
    {
        nwy_test_cli_echo("\r\ntcp not setup");
        return;
    }
    buff = nwy_test_cli_input_gets("\r\nPlease input send data(<=512): ");
    len = nwy_test_cli_input_len_gets();
    if (len > NWY_UART_RECV_SINGLE_MAX)
    {
        nwy_test_cli_echo("\r\nNo more than 512 bytes at a time to send ");
        return;
    }
    send_len = nwy_socket_send(s_nwy_cli_tcp_fd, buff, len, 0);
    if (send_len != len)
    {
        nwy_test_cli_echo("\r\nsend len=%d, return len=%d", len, send_len);
    }
    else{
        nwy_test_cli_echo("\r\nsend ok");
    }
    nwy_test_cli_echo("\r\n[%s]TCP state:%d",__func__,nwy_socket_get_state(s_nwy_cli_tcp_fd));
}
void nwy_test_cli_tcp_close()
{
    nwy_cli_socket_destory(&s_nwy_cli_tcp_fd);
}
/**************************UDP*********************************/
void nwy_test_cli_udp_setup()
{
    char *tmp = NULL;
    char * sptr = NULL;
    char url_or_ip[128 + 1] = {0};
    int port = 0;
    if (s_nwy_cli_udp_fd > 0)
    {
        nwy_test_cli_echo("\r\nSocket has been setup");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    nwy_tcp_default_cid= nwy_cli_str_to_int(sptr);
    if (nwy_tcp_default_cid < 1 || nwy_tcp_default_cid> 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    if (!nwy_cli_check_data_connect(nwy_tcp_default_cid)) {
        nwy_test_cli_echo("\r\nData call disconnect");
        return;
    }
    tmp = nwy_test_cli_input_gets("\r\nPlease input url: ");
    memcpy(url_or_ip, tmp, strlen(tmp));
    tmp = nwy_test_cli_input_gets("\r\nPlease input port: ");
    if (nwy_port_get(tmp, &port) != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nInput port error");
        return;
    }
    g_udp_manual_close = 0;
    nwy_cli_socket_setup_pro(url_or_ip, port, IPPROTO_UDP,nwy_tcp_default_cid);
}
void nwy_test_cli_udp_send()
{
    int send_len = 0;
    int len = 0;
    char *buff = NULL;
    buff = nwy_test_cli_input_gets("\r\nPlease input send data(<=512): ");
    len = nwy_test_cli_input_len_gets();
    if (len > NWY_UART_RECV_SINGLE_MAX)
    {
        nwy_test_cli_echo("\r\nNo more than 512 bytes at a time to send ");
        return;
    }
    if (s_nwy_inet_flag == AF_INET) {
        send_len= nwy_socket_sendto(s_nwy_cli_udp_fd, buff, len, 0, (struct sockaddr *)&s_nwy_server_v4, sizeof(s_nwy_server_v4));
    } else if (s_nwy_inet_flag == AF_INET6) {
        send_len= nwy_socket_sendto(s_nwy_cli_udp_fd, buff, len, 0, (struct sockaddr *)&s_nwy_server_v6, sizeof(s_nwy_server_v6));
    }
    if (send_len != strlen(buff))
    {
        nwy_test_cli_echo("\r\nsend len=%d, return len=%d", len, send_len);
    }
    else
        nwy_test_cli_echo("\r\nsend ok");
}
void nwy_test_cli_udp_close()
{
    g_udp_manual_close = 1;
    nwy_cli_socket_destory(&s_nwy_cli_udp_fd);
}


void nwy_ping_api_cb(void *arg)
{
    nwy_ping_cb_msg *cb_msg = (nwy_ping_cb_msg *)arg;
    int per = 0;
    char buff[256] = {0};
    switch(cb_msg->status)
    {
        case NWY_PING_ST_SOCKET_ERR:
            nwy_test_cli_echo("\r\n PING SOCKET ERROR");
            break;
        case NWY_PING_ST_DNS_ERR:
            nwy_test_cli_echo("\r\n PING DNS ERROR");
            break;
        case NWY_PING_ST_REPLY_TIMEOUT:
            nwy_test_cli_echo("\r\n PING TIMEOUT");
            break;
        case NWY_PING_ST_ALL_FINISH:
            if (cb_msg->fin_stats_info.num_pkts_sent > 0)
                per = (cb_msg->fin_stats_info.num_pkts_lost)/(cb_msg->fin_stats_info.num_pkts_sent);
            memset(buff, 0, 256);
            nwy_test_cli_echo("\r\n PING finished : sent = %lu, received = %lu, lost = %lu(%d%%loss)\r\n",
                cb_msg->fin_stats_info.num_pkts_sent, cb_msg->fin_stats_info.num_pkts_recvd,
                cb_msg->fin_stats_info.num_pkts_lost, (int)(per*100));
            nwy_test_cli_echo("\r\n PING RTT statistics : Minimum = %lu, Maximum = %lu, Average = %lu\r\n",
                cb_msg->fin_stats_info.min_rtt,
                cb_msg->fin_stats_info.max_rtt,
                cb_msg->fin_stats_info.avg_rtt);
            break;
        default:
            break;
    }
    return;
}

void nwy_test_cli_ping()
{
    char* sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    nwy_ping_api_param_t ping_param = {0};
    memset(&ping_param, 0, sizeof(nwy_ping_api_param_t));
    sptr = nwy_test_cli_input_gets("\r\n input ping url: ");
    memcpy(ping_param.ping_host, sptr, strlen(sptr));
    sptr = nwy_test_cli_input_gets("\r\n input ping count: ");
    ping_param.cnt = atoi(sptr);
    sptr = nwy_test_cli_input_gets("\r\n input ping intervals(s): ");
    ping_param.delay = atoi(sptr) * 1000;
    sptr = nwy_test_cli_input_gets("\r\n input ping timeout(s): ");
    ping_param.tout = atoi(sptr) * 1000;
    sptr = nwy_test_cli_input_gets("\r\n input ping payload (8-1460): ");
    ping_param.data_len = atoi(sptr);
    if (ping_param.data_len < 8 || ping_param.data_len > 1460)
    {
        nwy_test_cli_echo("\r\n ping payload error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input cid (1-7): ");
    ping_param.cid = atoi(sptr);
    if (ping_param.cid < 1 || ping_param.cid > 7)
    {
        nwy_test_cli_echo("\r\n cid input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input ip type 4-IPv4 6-IPv6: ");
    ping_param.ip_type = atoi(sptr);
    if (ping_param.ip_type != 4 && ping_param.ip_type != 6)
    {
        nwy_test_cli_echo("\r\n ip type error");
        return;
    }
    result = nwy_ping_start(&ping_param, nwy_ping_api_cb);
    if (0 == result)
        nwy_test_cli_echo("\r\n nwy ping success");
    else
        nwy_test_cli_echo("\r\n nwy ping fail");
}

void nwy_get_send_ip_packet_proc(uint8_t *data, int len)
{
    nwy_test_cli_echo("nwy_get_send_ip_packet_proc len=%d:\r\n", len);
    for(int i = 0; i < len; i++) {
        nwy_test_cli_echo("%02X ", data[i]);
        if((i + 1) % 16 == 0) {
            nwy_test_cli_echo("\r\n");
        }
    }
    nwy_test_cli_echo("\r\n");
}

void nwy_get_recv_ip_packet_proc(uint8_t *data, int len)
{
    nwy_test_cli_echo("nwy_recv_ip_packet_report len=%d:\r\n", len);
    for(int i = 0; i < len; i++) {
        nwy_test_cli_echo("%02X ", data[i]);
        if((i + 1) % 16 == 0) {
            nwy_test_cli_echo("\r\n");
        }
    }
    nwy_test_cli_echo("\r\n");
}
void nwy_test_cli_ip_packet()
{
    nwy_get_recv_ip_packet_reg(nwy_get_recv_ip_packet_proc);
    nwy_get_send_ip_packet_reg(nwy_get_send_ip_packet_proc);
}


// TCP server menu display
void nwy_test_cli_tcp_server_menu()
{
    nwy_test_cli_echo("\r\nPlease select an option to data test from the items listed below. \r\n");
    nwy_test_cli_echo("1. tcp listen setup\r\n");
    nwy_test_cli_echo("2. tcp listen close\r\n");
    nwy_test_cli_echo("3. tcp server send data to client\r\n");
    nwy_test_cli_echo("4. tcp server close client\r\n");
    nwy_test_cli_echo("5. Exit tcp function\r\n");
}

// TCP server thread function
static void nwy_tcp_server_func(void *param)
{
    // Initialize file descriptor sets for select()
    fd_set rset;
    fd_set ex_fd;
    int result = 0;
    char buf[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int maxfd = *(int *)param;
    int s = *(int *)param;
    char str[64]="Hello,I am server,haha";
    char recv_buff[NWY_UART_RECV_SINGLE_MAX +1] = {0};
    int ret=-1,socktfd=0,i=0,connfd=-1,sock=-1,n=-1;

    // Client address structures for both IPv4 and IPv6
    struct sockaddr_in client_addr;
    struct sockaddr_in6 client_addr_6;
    socklen_t client_len = sizeof(client_addr);
    socklen_t client_len_v6 = sizeof(client_addr_6);

    // Set select timeout
    struct timeval tv = {0};
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // Initialize client sockets array
    for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
    {
        g_client_sockets[i] = -1;
    }

    while(1)
    {
        // Check if server should stop first
        if(!tcp_listen_flag)
        {
            nwy_test_cli_echo("\r\nTcp server receive thread exit ");
            // Close all client connections
            for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
            {
                if((sock = g_client_sockets[i]) > 0)
                {
                    nwy_socket_close(sock);
                }
                g_client_sockets[i] = -1;
            }
            nwy_socket_close(s);
            *(int *)param = 0;
            nwy_thread_exit(NULL);
        }

        // Initialize fd sets for select
        FD_ZERO(&rset);
        FD_ZERO(&ex_fd);
        FD_SET(s,&rset);
        FD_SET(s,&ex_fd);
        
        // Add client sockets to fd sets
        for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
        {
            if((sock = g_client_sockets[i])<0)
            {
                continue;
            }
            FD_SET(sock,&rset);
            FD_SET(sock,&ex_fd);
        }

        // Wait for socket events
        result = nwy_socket_select(maxfd+1, &rset, NULL,&ex_fd, &tv);
        
        NWY_CLI_LOG("\r\nmaxfd = %d result = %d", maxfd, result);
        if (result < 0)
        {
            // Get specific error code
            int err = nwy_socket_errno();
            
            // EINTR means interrupted by signal, continue
            if (err == EINTR)
            {
                continue;
            }
            
            // EBADF means invalid file descriptor, check which socket is bad
            if (err == EBADF)
            {
                // Check server socket
                if (!tcp_listen_flag || g_server_socketfd <= 0)
                {
                    nwy_test_cli_echo("\r\nServer socket invalid, exiting...");
                    nwy_thread_exit(NULL);
                }
                
                // Check and clean up invalid client sockets
                for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
                {
                    if (g_client_sockets[i] > 0)
                    {
                        int error = 0;
                        socklen_t len = sizeof(error);
                        if (nwy_socket_getsockopt(g_client_sockets[i], SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0)
                        {
                            nwy_test_cli_echo("\r\nClient[%d] socket invalid, closing", i);
                            nwy_socket_close(g_client_sockets[i]);
                            g_client_sockets[i] = -1;
                        }
                    }
                }
                continue;
            }
            
            // Exit on other serious errors
            nwy_test_cli_echo("\r\nTcp server select error: %d", err);
            nwy_socket_close(s);
            tcp_listen_flag = 0;
            nwy_thread_exit(NULL);
        }
        else if(result > 0)
        {
            // Handle new connection - add flag check
            if (FD_ISSET(s, &rset) && tcp_listen_flag)  // Only accept if still listening
            {
                // Try IPv4 first, then IPv6
                connfd = nwy_socket_accept(s,(struct sockaddr*)&client_addr,&client_len);
                NWY_CLI_LOG("Try IPv6 first");
                if(connfd < 0)
                {
                    NWY_CLI_LOG("Then try IPv6");
                    connfd = nwy_socket_accept(s,(struct sockaddr*)&client_addr_6,&client_len_v6);
                }

                if(connfd > 0)
                {
                    // Find available slot for new client
                    for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
                    {
                        if(g_client_sockets[i] < 0)
                        {
                            nwy_test_cli_echo("\r\nAccept client index = %d,socket = %d", i, connfd);
                            g_client_sockets[i] = connfd;
                            break;
                        }
                    }

                    // Update max fd if needed
                    if (connfd > maxfd )
                    {
                        maxfd = connfd;
                    }

                    // Handle too many clients
                    if(i >= NWY_TCP_ACCEPT_MAX)
                    {
                        nwy_test_cli_echo("\r\nAccept too many clients,closed");
                        nwy_socket_close(connfd);
                        continue;
                    }
                }
            }

            // Handle server socket exception - add flag check 
            if (FD_ISSET(s, &ex_fd) && tcp_listen_flag)
            {
                nwy_test_cli_echo("\r\nTcp select ex_fd:");
                nwy_socket_close(s);
                *(int *)param = 0;
                tcp_listen_flag =0;
                nwy_thread_exit(NULL);
            }
        }

        // Handle client data
        for(i = 0; i < NWY_TCP_ACCEPT_MAX; i++)
        {
            if((sock = g_client_sockets[i])<0)
            {
                continue;
            }
            if(FD_ISSET(sock, &rset))
            {
                memset(buf, 0, sizeof(buf));
                // Receive data from client
                if((n = nwy_socket_recv(sock,buf,NWY_UART_RECV_SINGLE_MAX,0)) <= 0)
                {
                  // Receive error
                  nwy_test_cli_echo("\r\nTcp recv err,close client");
                  nwy_socket_close(sock);
                  g_client_sockets[i] = -1;
                  continue;
                }
                else
                {
                  // Echo received data and send test response
                  nwy_test_cli_echo("\r\nclient[%d] receive:\r\n%s", i, buf);
                  ret = nwy_socket_send(sock,str,strlen(str),0);
                  NWY_CLI_LOG("\r\nclient[%d] nwy_socket_send ret = %d", i, ret);
                  continue;
                }
            }

            // Handle client socket exception
            if (FD_ISSET(sock, &ex_fd))
            {
                nwy_test_cli_echo("\r\ntcp select ex_fd:");
                nwy_socket_close(sock);
                g_client_sockets[i] = -1;
                continue;
            }
        }
    }
    //nwy_thread_sleep(100);
}

void nwy_test_cli_tcp_server_listen(void)
{
    int port = 0;
    int ip_type = -1;
    int ret = 0;
    char* sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nInput channel(1-7):");
    nwy_tcp_default_cid = nwy_cli_str_to_int(sptr);
    if (nwy_tcp_default_cid < 1 || nwy_tcp_default_cid > 7)
    {
        nwy_test_cli_echo("\r\nnInput channel invalid");
        return;
    }

    if (!nwy_cli_check_data_connect(nwy_tcp_default_cid)) {
        nwy_test_cli_echo("\r\nData call not connect");
        return;
    }

    if(tcp_listen_flag)
    {
        nwy_test_cli_echo("\r\nTcp listening");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input ip type 0:ipv4,1:ipv6: ");
    ip_type = nwy_cli_str_to_int(sptr);
    if(0 != ip_type && 1 != ip_type){
        nwy_test_cli_echo("\r\nInput ip type invalid");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nPlease input port: ");
    if (nwy_port_get(sptr, &port)!= NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nInput port invalid"); 
        return;
    }

    nwy_test_cli_echo("\r\nTcp listen test start:ip type:%d,port:%d", ip_type, port);
    
    // Create socket
    if (0 == ip_type)
    {
        g_server_socketfd = nwy_socket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, nwy_tcp_default_cid);
        if(g_server_socketfd > 0)
        {
            ret = nwy_socket_lport_bind(g_server_socketfd, port);
        }
    }
    else if(1 == ip_type)
    {
        g_server_socketfd = nwy_socket_open(AF_INET6, SOCK_STREAM, IPPROTO_TCP, nwy_tcp_default_cid);
        if(g_server_socketfd > 0)
        {
            ret = nwy_socket_lport_bind(g_server_socketfd, port);
        }
    }
    else
    {
        return;
    }

    nwy_test_cli_echo("\r\nsocket fd is %d", g_server_socketfd);
    
    if(ret < 0 || g_server_socketfd <= 0)
    {
        nwy_test_cli_echo("\r\n nwy bind err!:errno %d", nwy_socket_errno());
        nwy_socket_close(g_server_socketfd);
        g_server_socketfd = 0;
        return;
    }

    // Set socket options
    if ((0 != nwy_socket_set_nonblock(g_server_socketfd))
        || (0 != nwy_socket_set_reuseaddr(g_server_socketfd))
        || (0 != nwy_socket_set_nodelay(g_server_socketfd)))
    {
        nwy_test_cli_echo("\r\nnwy set socket opt error!");
        nwy_socket_close(g_server_socketfd);
        g_server_socketfd = 0;
        return;
    }

    // Start listening
    ret = nwy_socket_listen(g_server_socketfd, port);
    NWY_CLI_LOG("nwy_socket_listen ret is %d", ret);
    nwy_test_cli_echo( "\r\nnwy_socket_listen %s!",ret == NWY_SUCCESS ? "success" : "failed");
    if(!ret)
    {
        tcp_listen_flag = 1;
    }
    else
    {
        nwy_socket_close(g_server_socketfd);
        g_server_socketfd = 0;
        tcp_listen_flag = 0;
        return;
    }

    // Create receive thread
    if(tcp_listen_flag)
    {
        nwy_thread_create(&tcp_recv_thread_server,
                         "tcp_recv_thread_server", 
                         NWY_OSI_PRIORITY_NORMAL, 
                         nwy_tcp_server_func, 
                         (void*)&g_server_socketfd, 
                         8,
                         NWY_THREAD_STACK_SIZE, 
                         NULL);
                         
        if(tcp_recv_thread_server == NULL)
        {
            nwy_test_cli_echo("\r\ncreate tcp recv thread failed, close connect");
            nwy_socket_close(g_server_socketfd);
            g_server_socketfd = 0;
            tcp_listen_flag = 0;
        }
    }
}

void nwy_test_cli_tcp_server_close_listen(void)
{
    if(!tcp_listen_flag) {
        nwy_test_cli_echo("\r\nServer not listening");
        return;
    }

    // Wait for server thread to exit
    nwy_thread_sleep(100);
    tcp_listen_flag = 0;

    // Close server socket
    if(g_server_socketfd > 0) {
        nwy_socket_shutdown(g_server_socketfd, SHUT_RDWR);
        nwy_socket_close(g_server_socketfd);
        g_server_socketfd = 0;
    }

    nwy_test_cli_echo("\r\nClose tcp listen success");
}

// Send data to specified client
void nwy_test_cli_tcp_server_send()
{
    char *sptr = NULL;
    int client_index = 0;
    char *data = NULL;
    int send_len = 0;
    int client_count = 0;
    
    // Display connected clients
    nwy_test_cli_echo("\r\nConnected clients:");
    for(int i = 0; i < NWY_TCP_ACCEPT_MAX; i++) {
        if(g_client_sockets[i] > 0) {
            nwy_test_cli_echo("\r\nClient[%d] socket=%d", i, g_client_sockets[i]);
            client_count++;
        }
    }
    
    if(client_count > 0)
    {
        // Select client
        sptr = nwy_test_cli_input_gets("\r\nPlease input client index(0-4): ");
        client_index = nwy_cli_str_to_int(sptr);
        if(client_index < 0 || client_index >= NWY_TCP_ACCEPT_MAX || g_client_sockets[client_index] <= 0) {
            nwy_test_cli_echo("\r\nInvalid client index");
            return;
        }

        // Input data to send
        data = nwy_test_cli_input_gets("\r\nPlease input send data(<=512): ");
        if(strlen(data) > NWY_UART_RECV_SINGLE_MAX) {
            nwy_test_cli_echo("\r\nData too long");
            return;
        }

        // Send data
        send_len = nwy_socket_send(g_client_sockets[client_index], data, strlen(data), 0);
        if(send_len != strlen(data)) {
            nwy_test_cli_echo("\r\nSend failed, len=%d, expected=%d", send_len, strlen(data));
        } else {
            nwy_test_cli_echo("\r\nSend success");
        }
        return;
    }

    nwy_test_cli_echo("\r\nThere are no connected clients!");
}

// Close specified client connection
void nwy_test_cli_tcp_server_close_client()
{
    char *sptr = NULL;
    int client_index = 0;
    int client_count = 0;

    // Display connected clients
    nwy_test_cli_echo("\r\nConnected clients:");
    for(int i = 0; i < NWY_TCP_ACCEPT_MAX; i++) {
        if(g_client_sockets[i] > 0) {
            nwy_test_cli_echo("\r\nClient[%d] socket=%d", i, g_client_sockets[i]);
            client_count++;
        }
    }

    if(client_count > 0)
    {
        // Select client to close
        sptr = nwy_test_cli_input_gets("\r\nPlease input client index(0-4): ");
        client_index = nwy_cli_str_to_int(sptr);
        if(client_index < 0 || client_index >= NWY_TCP_ACCEPT_MAX || g_client_sockets[client_index] <= 0) {
            nwy_test_cli_echo("\r\nInvalid client index");
            return;
        }

        // Gracefully close connection
        nwy_socket_shutdown(g_client_sockets[client_index], SHUT_RDWR);
        nwy_socket_close(g_client_sockets[client_index]);
        g_client_sockets[client_index] = -1;
        nwy_test_cli_echo("\r\nClient[%d] closed successfully", client_index);
        return;
    }

    nwy_test_cli_echo("\r\nThere are no connected clients!");
}

void nwy_test_cli_tcp_server(void)
{
    char* sptr = NULL;
    while (1)
    {
        nwy_test_cli_tcp_server_menu();
        sptr = nwy_test_cli_input_gets("\r\nPlease input option: ");
        switch(atoi(sptr))
        {
            case 1:
                nwy_test_cli_tcp_server_listen();
                break;
            case 2:
                nwy_test_cli_tcp_server_close_listen();
                break;
            case 3:
                nwy_test_cli_tcp_server_send();
                break;
            case 4:
                nwy_test_cli_tcp_server_close_client();
                break;
            case 5:
                return;
            default:
                break;
        }
    }
}


