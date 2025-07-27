/**

* File name: nwy_tcp_cli_demo.c
* Author:gz  Date:2023
* Description: This is a TCP client demonstration program that demonstrates the process of client startup, data transmission, and data reception

 1.Change the following test server configuration information to connect to a TCP server:

#define NWY_TCP_DEMO_HOST_STR      "soft-s3.neoway.cn" //test Server address
#define NWY_TCP_DEMO_HOST_PORT      10118//test server port
#define NWY_TCP_DEMO_CONN_TIMEOUT_S    20//connection timeout


* Others:
* Function List:

* History:
   1. Date: 2023/11/14
      Author:gz
      version:V1.0
  2. ...
*/
#include "nwy_demo_tcp.h"

#define NWY_TCP_DEMO_HOST_STR      "soft-s3.neoway.cn" //test Server address
#define NWY_TCP_DEMO_HOST_PORT      10118//test server port
#define NWY_TCP_DEMO_CONN_TIMEOUT_S    20//connection timeout


typedef enum
{
    NWY_TCP_DEMO_RECV_STATE_INVALID = 0,
    NWY_TCP_DEMO_RECV_STATE_TIMEOUT = NWY_TCP_DEMO_RECV_STATE_INVALID,
    NWY_TCP_DEMO_RECV_STATE_ERROR=-1,
    NWY_TCP_DEMO_RECV_STATE_PASSIVE_CLOSE=-2,
    NWY_TCP_DEMO_RECV_STATE_SOCKET_CLOSE=-3,
}tcp_recv_state_E;

#define TCP_PDEMO_BUF_LEN_MAX    1024+1

static int nwy_tcp_demo_recv( int fd, char *buf, int len, int64 timeout_s)
{
    int recved_num = 0;
    fd_set         rd_fd;
    struct timeval timeout;
    int ret = 0;

    FD_ZERO(&rd_fd);
    FD_SET(fd, &rd_fd);

    timeout.tv_sec  = timeout_s;
    timeout.tv_usec = 0;

    ret = nwy_socket_select(fd + 1, &rd_fd, NULL, NULL, &timeout);
    if(ret > 0)
    {
        if (FD_ISSET(fd, &rd_fd))
        {
            recved_num = nwy_socket_recv(fd, buf, len, 0);
            if (recved_num > 0) {
                return recved_num;
            }
            else if (0 == recved_num) {
                NWY_SDK_LOG_INFO("socket passive close %d,%d", recved_num, nwy_socket_errno(),0);
                return NWY_TCP_DEMO_RECV_STATE_PASSIVE_CLOSE;
            }
            else {
                NWY_SDK_LOG_INFO("sock_recv %d,%d", recved_num, nwy_socket_errno(),0);
                if(EBADF ==nwy_socket_errno() ||ECONNRESET ==nwy_socket_errno())
                {
                    return NWY_TCP_DEMO_RECV_STATE_SOCKET_CLOSE;
                }

                return NWY_TCP_DEMO_RECV_STATE_ERROR;
            }
        }
        else
        {
            NWY_SDK_LOG_INFO("sock_recv %d,%d", recved_num, nwy_socket_errno(),0);
            return NWY_TCP_DEMO_RECV_STATE_INVALID;
        }
     }
     else if(ret == 0)
     {
        return NWY_TCP_DEMO_RECV_STATE_TIMEOUT;
     }
     else
     {
        NWY_SDK_LOG_INFO(" select read fail %d,%d", ret, nwy_socket_errno(),0);
        return NWY_TCP_DEMO_RECV_STATE_ERROR;
     }

}

static void nwy_tcp_demo_data_recv_func(void *param)
{
    int recv_len = 0;
    char *buf =(char *)malloc(TCP_PDEMO_BUF_LEN_MAX);
    int socket_fd = (int)param;
    NWY_SDK_LOG_INFO("recv forme socketfd:%x", socket_fd,0,0);

    while(1)
    {
        memset(buf,0,TCP_PDEMO_BUF_LEN_MAX);
        if(socket_fd <=0)
        {
            NWY_SDK_LOG_INFO("nwy_tcp_demo_cli_fd error",0,0,0);
            break;
        }

        memset(buf,0,TCP_PDEMO_BUF_LEN_MAX);
        recv_len = nwy_tcp_demo_recv(socket_fd,buf,TCP_PDEMO_BUF_LEN_MAX-1,2);
        if(recv_len > 0)
        {
            NWY_SDK_LOG_INFO("recv:socketfd:%d,recv_len:%d,%s ",socket_fd,recv_len,buf);
        }
        else if(recv_len == NWY_TCP_DEMO_RECV_STATE_TIMEOUT)
        {
            //recv:timeout ,No data, continue checking
            continue;
        }
        else if(recv_len ==NWY_TCP_DEMO_RECV_STATE_PASSIVE_CLOSE)
        {
            NWY_SDK_LOG_INFO("tcp cli demo passive close",0,0,0);
            nwy_socket_close(socket_fd);
            break;
        }
        else if(recv_len ==NWY_TCP_DEMO_RECV_STATE_SOCKET_CLOSE)
        {
            NWY_SDK_LOG_INFO("tcp cli demo socket close",0,0,0);
            break;
        }
        else
        {
            NWY_SDK_LOG_INFO("recv:error ",0,0,0);
            break;
        }

    }

    if(buf != NULL)
    {
        free(buf);
        buf = NULL;
    }
    NWY_SDK_LOG_INFO("exit nwy_tcp_demo_data_recv_func,socket_fd:%x",socket_fd,0,0);
    nwy_thread_exit(NULL);
}

int nwy_tcp_cli_demo_setup(char * url,int port)
{
    int ret = 0;
    int socket_fd = 0;
    ip_addr_t addr;
    int isipv6 =0;
    char *ip_buf = NULL;
    struct sockaddr_in sa_v4;
    struct sockaddr_in6 sa_v6;
    nwy_osi_thread_t nwy_tcp_recv_thread_hd = NULL;

    NWY_SDK_LOG_INFO("nwy_tcp_cli_demo_setup entry ",0,0,0);
    if(url == NULL)
    {
        return -1;
    }

    memset(&sa_v4,0x00,sizeof(struct sockaddr_in));
    memset(&sa_v6,0x00,sizeof(struct sockaddr_in6));
    ip_buf = nwy_socket_gethost_by_name(url, &isipv6);
    if (ip_buf == NULL)
    {
        NWY_SDK_LOG_INFO(" ip_buf error",0,0,0);
        return -1;
    }

    NWY_SDK_LOG_INFO("nwy ip_buf= %s isipv6 = %d", ip_buf,isipv6,0);
    if (isipv6 ==1)
    {
        if(nwy_socket_inet_pton(AF_INET6,ip_buf, &addr.u_addr.ip6)!=NWY_SUCCESS)
        {
            NWY_SDK_LOG_INFO("input ip or url is invalid",0,0,0);
            return -1;
        }

        inet6_addr_from_ip6addr(&sa_v6.sin6_addr, ip_2_ip6(&addr));
        sa_v6.sin6_len = sizeof(struct sockaddr_in);
        sa_v6.sin6_family = AF_INET6;
        sa_v6.sin6_port = htons(port);

         socket_fd = nwy_socket_open(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        if(socket_fd <= 0)
        {
            NWY_SDK_LOG_INFO(" nwy_socket_open error,errno:%d",nwy_socket_errno(),0,0);
            return -1;
        }

    }
    else
    {
        if(nwy_socket_inet_pton(AF_INET,ip_buf, &addr.u_addr.ip4)!=NWY_SUCCESS)
        {
            NWY_SDK_LOG_INFO("input ip or url is invalid",0,0,0);
            return -1;
        }

        inet_addr_from_ip4addr(&sa_v4.sin_addr, ip_2_ip4(&addr));
        sa_v4.sin_len = sizeof(struct sockaddr_in);
        sa_v4.sin_family = AF_INET;
        sa_v4.sin_port = htons(NWY_TCP_DEMO_HOST_PORT);
        socket_fd = nwy_socket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(socket_fd <= 0)
        {
            NWY_SDK_LOG_INFO(" nwy_socket_open error,errno:%d",nwy_socket_errno(),0,0);
            return -1;
        }

    }
    NWY_SDK_LOG_INFO(" nwy_socket_open ok,socket_fd:%d",socket_fd,0,0);

    if ((NWY_SUCCESS != nwy_socket_set_reuseaddr(socket_fd))|| (NWY_SUCCESS !=nwy_socket_set_nodelay(socket_fd)))
    {
        NWY_SDK_LOG_INFO("socket set err",0,0,0);

        ret = nwy_socket_close(socket_fd);
        if (ret != NWY_SUCCESS)
        {
            NWY_SDK_LOG_INFO("Socket close fail errno:%d",nwy_socket_errno(),0,0);
        }

        return -1;
    }

    if (isipv6) {
         ret = nwy_socket_connect(socket_fd, (struct sockaddr *)&sa_v6, sizeof(sa_v6));
    }
    else
    {
        ret = nwy_socket_connect(socket_fd, (struct sockaddr *)&sa_v4, sizeof(sa_v4));
    }

    if(ret == 0)
    {
        NWY_SDK_LOG_INFO("tcp connect ok,errno:%d",nwy_socket_errno(),0,0);
        ret = nwy_thread_create(&nwy_tcp_recv_thread_hd, "nwy_tcp_demo_recv_task",NWY_OSI_PRIORITY_NORMAL, nwy_tcp_demo_data_recv_func, (void *)socket_fd, 10,1024*3, NULL);
        if(ret != NWY_SUCCESS || nwy_tcp_recv_thread_hd == NULL)
        {
            NWY_SDK_LOG_INFO("create tcp recv thread failed",0,0,0);
            return -1;
        }
        NWY_SDK_LOG_INFO("create tcp recv thread ok",0,0,0);
        nwy_thread_sleep(200);
        return socket_fd;
    }
    else
    {
        NWY_SDK_LOG_INFO("tcp connect error:%d",nwy_socket_errno(),0,0);
        ret = nwy_socket_close(socket_fd);
        if (ret != NWY_SUCCESS)
        {
            NWY_SDK_LOG_INFO("Socket close fail errno:%d",nwy_socket_errno(),0,0);
        }

        return -1;
    }


}

void nwy_tcp_cli_demo_main(void)
{
    int socket_fd = 0;
    int ret = 0;
    char send_buf[]="hello tcp server";
    socket_fd = nwy_tcp_cli_demo_setup(NWY_TCP_DEMO_HOST_STR,NWY_TCP_DEMO_HOST_PORT);
    if(socket_fd<=0)
    {
        NWY_SDK_LOG_INFO("Socket setup error",0,0,0);
        return;
    }
    else
    {
        NWY_SDK_LOG_INFO("Socket setup ok",0,0,0);
    }

    ret = nwy_socket_send(socket_fd,send_buf,strlen(send_buf),0);
    if(ret != strlen(send_buf))
    {
        NWY_SDK_LOG_INFO("Socket send fail errno:%d",nwy_socket_errno(),0,0);
    }
    else
    {
        NWY_SDK_LOG_INFO("Socket send ok",0,0,0);
    }

}

