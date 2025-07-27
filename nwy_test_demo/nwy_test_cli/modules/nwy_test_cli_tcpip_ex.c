#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_socket_api.h"


typedef enum {
    NWY_APP_SOCKET_GEN_ERR = 0,         // General error
    NWY_APP_SOCKET_BUSYING,             // Busy, reached maximum number of client connections in server mode
    NWY_APP_SOCKET_DNS_ERR,             // DNS resolution error
    NWY_APP_SOCKET_OPEN_FAIL,           // Open failed
    NWY_APP_SOCKET_OPENED,              // Opened (TCP socket connected\UDP socket created\TCP Server listening)
    NWY_APP_SOCKET_CLOSED,              // Closed (including active and passive closure)
    NWY_APP_SOCKET_DATA_RECEIVED,       // Data received
    NWY_APP_SOCKET_DATA_SEND_ERR,       // Send error
    NWY_APP_SOCKET_DATA_SEND_FINISHED,  // Send completed
    NWY_APP_SOCKET_NEW_CLIENT_ACCEPTED, // New client connected
    NWY_APP_SOCKET_ACPT_CLIENT_CLOSED,  // Client closed (including active and passive closure)
    NWY_APP_SOCKET_ACPT_CLIENT_CLOSED_ALL,  // All clients closed
} nwy_app_socket_state_e;

// Modify message structure
typedef struct {
    nwy_app_socket_state_e event;  // Event type
    int socket_fd;                 // Associated file descriptor
    void *data;                    // Data pointer
    int data_len;                 // Data length
    void *hdl;                    //nwy_sock_hdl_t* or nwy_server_hdl_t* hdl
} nwy_sock_msg_t;

// Define callback function type
typedef void (*nwy_sock_msg_cb_t)(nwy_sock_msg_t *msg);

// Socket option configuration structure
typedef struct {
    bool reuse_addr;      // Whether to allow address reuse
    bool nodelay;         // Whether TCP enables nodelay
    bool nonblock;        // Whether non-blocking
    bool keepalive;      // Whether to enable keepalive
    uint32_t keepidle;   // Keepalive idle time
    uint32_t keepintvl;  // Keepalive interval
} nwy_app_socket_opts_s;

// TCP/UDP client handle structure
typedef struct {
    int cid;                    // Channel ID
    int socket_fd;              // Socket file descriptor
    char host[256+1];          // Server address
    int port;                   // Port number
    struct sockaddr_in s_nwy_server_v4;  // IPv4 address structure
    struct sockaddr_in6 s_nwy_server_v6; // IPv6 address structure
    nwy_app_socket_state_e status;   // Connection status
    nwy_sock_msg_cb_t msg_cb;   // Message callback function
    int is_tcp;                 // 1:TCP 0:UDP
    nwy_osi_thread_t recv_thread; // Receive thread handle
    nwy_app_socket_opts_s opts;  // Socket options
} nwy_sock_hdl_t;

// TCP server client connection structure
typedef struct {
    int socket_fd;              // Client socket
    struct sockaddr_in addr_v4; // IPv4 client address
    struct sockaddr_in6 addr_v6;// IPv6 client address
    int af_inet_flag;          // Address family flag
} nwy_server_client_t;

// TCP server handle structure
typedef struct {
    int cid;                    // Channel ID
    int socket_fd;              // Listen socket
    int port;                   // Listen port
    int af_inet_flag;          // Address family flag(AF_INET/AF_INET6)
    nwy_app_socket_state_e status;   // Server status
    nwy_sock_msg_cb_t msg_cb;   // Message callback function
    nwy_server_client_t clients[5]; // Support up to 5 client connections
    int client_count;           // Current number of connected clients
    nwy_osi_thread_t recv_thread; // Receive thread handle
} nwy_server_hdl_t;


// Modify global variable definition
static nwy_sock_hdl_t *g_tcp_hdl = NULL;
static nwy_sock_hdl_t *g_udp_hdl = NULL; 
static nwy_server_hdl_t *g_server_hdl = NULL;

// Define thread stack size and event count
#define NWY_TEST_CLI_TCPIP_THREAD_STACK_SIZE (3*1024)  // Keep consistent with nwy_test_cli_tcpip.c
#define NWY_TEST_CLI_TCPIP_THREAD_EVENT_COUNT 8        // Each thread supports 8 events

// log control
#define NWY_CLI_TCPIP_LOG_LEVEL_ERROR   0
#define NWY_CLI_TCPIP_LOG_LEVEL_WARN    1
#define NWY_CLI_TCPIP_LOG_LEVEL_INFO    2
#define NWY_CLI_TCPIP_LOG_LEVEL_DEBUG   3
    
#ifndef NWY_CLI_TCPIP_LOG_ENABLE
#define NWY_CLI_TCPIP_LOG_ENABLE 1
#endif
    
#if NWY_CLI_TCPIP_LOG_ENABLE
#define NWY_CLI_TCPIP_LOG_DEBUG(format, ...) NWY_SDK_LOG_DEBUG("[TCPIP_EX:%s] " format, __func__, ##__VA_ARGS__)
#else
#define NWY_CLI_TCPIP_LOG_DEBUG(format, ...)
#endif


// Tool function declaration
static int nwy_sock_dns_parse(nwy_sock_hdl_t *hdl);
static bool nwy_sock_data_call_check(int cid);
static void nwy_sock_msg_notify(nwy_sock_hdl_t *hdl, nwy_app_socket_state_e event, void *data, int data_len);
static void nwy_server_msg_notify(nwy_server_hdl_t *hdl, nwy_app_socket_state_e event, void *data, int data_len, int client_or_server_fd);

// TCP/UDP client API declaration
nwy_sock_hdl_t* nwy_tcp_client_setup(
    uint16_t cid_id,
    const char* host, 
    uint16_t port,
    nwy_sock_msg_cb_t cb_func,
    nwy_app_socket_opts_s* opts);

int nwy_tcp_client_send(nwy_sock_hdl_t* hdl, const void* data, size_t len);

void nwy_tcp_client_close(nwy_sock_hdl_t* hdl);

nwy_sock_hdl_t* nwy_udp_client_setup(
    uint16_t cid_id,
    const char* host,
    uint16_t port, 
    nwy_sock_msg_cb_t cb_func,
    nwy_app_socket_opts_s* opts);

int nwy_udp_client_send(nwy_sock_hdl_t* hdl, const void* data, size_t len);

void nwy_udp_client_close(nwy_sock_hdl_t* hdl);

// TCP server API declaration
nwy_server_hdl_t* nwy_tcp_server_setup(
    uint16_t cid_id,
    uint16_t port,
    int af_inet_flag,
    nwy_sock_msg_cb_t cb_func, 
    nwy_app_socket_opts_s* opts);

int nwy_tcp_server_send(nwy_server_hdl_t* hdl, int client_fd, const void* data, size_t len);

void nwy_tcp_server_close_client(nwy_server_hdl_t* hdl, int client_fd);

void nwy_tcp_server_close(nwy_server_hdl_t* hdl);

/*add for test*/
static void trace_heap_memory(char* funcname,int line_num){
    int ret = NWY_GEN_E_UNKNOWN;
    nwy_heapinfo_t  heapinfo;
    memset(&heapinfo,0x00,sizeof(heapinfo));
    ret = nwy_dm_heapinfo(&heapinfo);
    if (ret == NWY_SUCCESS && funcname && line_num) {
        NWY_CLI_TCPIP_LOG_DEBUG("%s-%d get heapinfo: start:%ld,size:%ld,avail_size:%ld,max_block_size:%ld",
                             funcname,line_num,heapinfo.start,heapinfo.size,heapinfo.avail_size,heapinfo.max_block_size);
    }
    else
    {
        NWY_CLI_TCPIP_LOG_DEBUG("%s trace failed",__func__);
    }
}
/*add for test*/

// Handle initialization and cleanup
static nwy_sock_hdl_t * nwy_sock_hdl_init(void)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    trace_heap_memory(__func__,__LINE__);
    nwy_sock_hdl_t *hdl = NULL;
    hdl = (nwy_sock_hdl_t*)malloc(sizeof(nwy_sock_hdl_t));
    if(hdl) {
        memset(hdl, 0, sizeof(nwy_sock_hdl_t));
        hdl->msg_cb = NULL;
        hdl->status = NWY_APP_SOCKET_CLOSED;
        hdl->socket_fd = -1;
        hdl->recv_thread = NULL;
        hdl->opts.reuse_addr = false;
        hdl->opts.nodelay = false;
        hdl->opts.nonblock = false;
        hdl->opts.keepalive = false;
        hdl->opts.keepidle = 0;
        hdl->opts.keepintvl = 0;
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
    trace_heap_memory(__func__,__LINE__);
    return hdl;
}

static void nwy_sock_hdl_deinit(nwy_sock_hdl_t *hdl)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    trace_heap_memory(__func__,__LINE__);
    nwy_osi_thread_t recv_thread = NULL;
    if (hdl){
        if (hdl->recv_thread != NULL) {
            recv_thread = hdl->recv_thread;
            hdl->recv_thread = NULL;
        }
        // Close socket
        if (hdl->socket_fd > 0) {
            NWY_CLI_TCPIP_LOG_DEBUG("close socket %d",hdl->socket_fd);
            nwy_socket_close(hdl->socket_fd);
            hdl->socket_fd = -1;
        }

        // Reset status and other fields
        hdl->status = NWY_APP_SOCKET_CLOSED;
        hdl->msg_cb = NULL;
        memset(&hdl->s_nwy_server_v4, 0, sizeof(struct sockaddr_in));
        memset(&hdl->s_nwy_server_v6, 0, sizeof(struct sockaddr_in6));
        memset(&hdl->opts, 0, sizeof(nwy_app_socket_opts_s));
        NWY_CLI_TCPIP_LOG_DEBUG("free handle");
        free(hdl);
        hdl = NULL;
        if(recv_thread){
            nwy_thread_exit(recv_thread);
        }
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
    trace_heap_memory(__func__,__LINE__);
}

// Client connection initialization and cleanup
static void nwy_server_client_init(nwy_server_client_t *client)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    if(client) {
        memset(client, 0, sizeof(nwy_server_client_t));
        client->socket_fd = -1;
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
}

static void nwy_server_client_deinit(nwy_server_client_t *client)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    if(client) {
        if(client->socket_fd >= 0) {
            nwy_socket_close(client->socket_fd);
            client->socket_fd = -1;
        }
        memset(&client->addr_v4, 0, sizeof(client->addr_v4));
        memset(&client->addr_v6, 0, sizeof(client->addr_v6));
        client->af_inet_flag = 0;
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
}

static nwy_server_hdl_t* nwy_server_hdl_init(void)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    trace_heap_memory(__func__,__LINE__);
    nwy_server_hdl_t *hdl = NULL;
    hdl = (nwy_server_hdl_t*)malloc(sizeof(nwy_server_hdl_t));
    if(hdl) {
        memset(hdl, 0, sizeof(nwy_server_hdl_t));
        hdl->status = NWY_APP_SOCKET_CLOSED;
        hdl->socket_fd = -1;
        hdl->client_count = 0;
        hdl->recv_thread = NULL;
        for(int i = 0; i < 5; i++) {
            nwy_server_client_init(&hdl->clients[i]);
        }
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
    trace_heap_memory(__func__,__LINE__);
    return hdl;
}

static void nwy_server_hdl_deinit(nwy_server_hdl_t *hdl)
{
    NWY_CLI_TCPIP_LOG_DEBUG("ENTER");
    trace_heap_memory(__func__,__LINE__);
    int i = 0;
    nwy_osi_thread_t recv_thread = NULL;
    if (hdl){
        if(hdl->client_count > 0) {
            for (i = 0; i < hdl->client_count; i++) {
                if (hdl->clients[i].socket_fd > 0) {
                    nwy_server_client_deinit(&hdl->clients[i]);
                }
            }
            hdl->client_count = 0;
        }

        if (hdl->socket_fd > 0) {
            nwy_socket_close(hdl->socket_fd);
            hdl->socket_fd = -1;
        }
    
        if (hdl->recv_thread != NULL) {
            recv_thread = hdl->recv_thread;
            hdl->recv_thread = NULL;
        }

        hdl->status = NWY_APP_SOCKET_CLOSED;
        hdl->msg_cb = NULL;
        free(hdl);
        hdl = NULL;
        if(recv_thread != NULL){
            nwy_thread_exit(recv_thread);
        }
    }
    NWY_CLI_TCPIP_LOG_DEBUG("EXIT");
    trace_heap_memory(__func__,__LINE__);
}



// Implement tool functions
static int nwy_sock_dns_parse(nwy_sock_hdl_t *hdl)
{
    int isipv6 = 0;
    char *ip_buf = NULL;

    if (!hdl || !hdl->host[0])
        return AF_UNSPEC;

    ip_buf = nwy_socket_gethost_by_name(hdl->host, &isipv6, hdl->cid);
    if(ip_buf == NULL) {
        NWY_SDK_LOG_ERROR("getaddrinfo failed with param %s", hdl->host);
        return -1;
    }

    if(isipv6 == 1) {
        memset(&hdl->s_nwy_server_v6, 0, sizeof(hdl->s_nwy_server_v6));
        if (nwy_socket_inet_pton(AF_INET6, ip_buf, (void *)&hdl->s_nwy_server_v6.sin6_addr) < 0) {
            return AF_UNSPEC;
        }
        hdl->s_nwy_server_v6.sin6_len = sizeof(struct sockaddr_in6);
        hdl->s_nwy_server_v6.sin6_family = AF_INET6;
        hdl->s_nwy_server_v6.sin6_port = nwy_socket_htons(hdl->port);
        return AF_INET6;
    } else {
        memset(&hdl->s_nwy_server_v4, 0, sizeof(hdl->s_nwy_server_v4));
        if (nwy_socket_inet_pton(AF_INET, ip_buf, (void *)&hdl->s_nwy_server_v4.sin_addr) < 0) {
            return AF_UNSPEC;
        }
        hdl->s_nwy_server_v4.sin_len = sizeof(struct sockaddr_in);
        hdl->s_nwy_server_v4.sin_family = AF_INET;
        hdl->s_nwy_server_v4.sin_port = nwy_socket_htons(hdl->port);
        return AF_INET;
    }
}

static bool nwy_sock_data_call_check(int cid)
{
    nwy_data_callinfo_t addr_info = {0};
    int ret = nwy_data_call_info_get(cid, &addr_info);
    if (ret == NWY_SUCCESS && addr_info.state == NWY_DATA_CALL_CONNECTED_STATE) {
        return true;
    }
    return false;
}

static void nwy_sock_msg_notify(nwy_sock_hdl_t *hdl, nwy_app_socket_state_e event, void *data, int data_len)
{
    if(hdl->msg_cb) {
        nwy_sock_msg_t msg = {
            .event = event,
            .socket_fd = hdl->socket_fd,
            .data = data,
            .data_len = data_len,
            .hdl = hdl
        };
        hdl->status = event;
        hdl->msg_cb(&msg);
    }
}

static void nwy_server_msg_notify(nwy_server_hdl_t *hdl, nwy_app_socket_state_e event, void *data, int data_len, int client_or_server_fd)
{
    if(hdl->msg_cb) {
        nwy_sock_msg_t msg = {
            .event = event,
            .socket_fd = client_or_server_fd,
            .data = data,
            .data_len = data_len,
            .hdl = hdl
        };
        hdl->status = event;
        hdl->msg_cb(&msg);
    }
}

// TCP receive task
static void nwy_tcp_recv_task(void *param)
{
    nwy_sock_hdl_t *hdl = (nwy_sock_hdl_t *)param;
    struct timeval tv = {20, 0};
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0;
    fd_set rd_fd, wt_fd, ex_fd;

    NWY_CLI_TCPIP_LOG_DEBUG("enter");

    while (1) {
        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        FD_ZERO(&wt_fd);
        FD_SET(hdl->socket_fd, &rd_fd);
        if(nwy_socket_get_state(hdl->socket_fd) < NWY_ESTABLISHED){
            FD_SET(hdl->socket_fd, &wt_fd);
        }
        FD_SET(hdl->socket_fd, &ex_fd);

        int result = nwy_socket_select(hdl->socket_fd + 1, &rd_fd, &wt_fd, &ex_fd, &tv);
        NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_select ret=%d, status:%d", result,hdl->status);
        if (result < 0) {
            int err = nwy_socket_errno();
            if(err == EINTR) {
                continue;
            }
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0);  // Send close notification
            break;
        } else if (result > 0) {

            if (FD_ISSET(hdl->socket_fd, &ex_fd)) {
                nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0);
                break;
            }
            
            if (FD_ISSET(hdl->socket_fd, &wt_fd)) {
                if(nwy_socket_get_state(hdl->socket_fd) == NWY_ESTABLISHED){
                    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPENED, NULL, 0);
                    NWY_CLI_TCPIP_LOG_DEBUG("tcp connect success.");
                }
            }
            
            if (FD_ISSET(hdl->socket_fd, &rd_fd)) {
                memset(recv_buff, 0, sizeof(recv_buff));
                recv_len = nwy_socket_recv(hdl->socket_fd, recv_buff, NWY_UART_RECV_SINGLE_MAX, 0);
                
                if (recv_len > 0) {
                    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_RECEIVED, recv_buff, recv_len);
                } else if (recv_len == 0 || (recv_len < 0 && nwy_socket_errno() != EINTR)) {
                    // Connection closed or error occurred
                    NWY_CLI_TCPIP_LOG_DEBUG("connection closed or error occurred");
                    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0);
                    break;
                } else {
                    int err = nwy_socket_errno();
                    NWY_CLI_TCPIP_LOG_DEBUG("recv error, errno=%d", err);
                    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0);
                    break;
                }
            }

            
            
        }
    }

    nwy_thread_sleep(200);
    NWY_CLI_TCPIP_LOG_DEBUG("exit");
    nwy_sock_hdl_deinit(hdl);  // Clean up resources but do not send notification
}

// UDP receive task
static void nwy_udp_recv_task(void *param)
{
    nwy_sock_hdl_t *hdl = (nwy_sock_hdl_t *)param;
    char recv_buf[NWY_UART_RECV_SINGLE_MAX];
    int recv_len;
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    NWY_CLI_TCPIP_LOG_DEBUG("enter");
    while(1) {
        memset(recv_buf, 0, sizeof(recv_buf));
        recv_len = nwy_socket_recvfrom(hdl->socket_fd, recv_buf, sizeof(recv_buf)-1, 0,
                                     (struct sockaddr *)&from_addr, &addr_len);
        
        if(recv_len > 0) {
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_RECEIVED, recv_buf, recv_len);
        } else {
            if(nwy_socket_errno() != EAGAIN) {
                NWY_CLI_TCPIP_LOG_DEBUG("errno %d",nwy_socket_errno());
                if(hdl->status != NWY_APP_SOCKET_CLOSED){
                    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_GEN_ERR, NULL, 0);
                }
                break;
            }
        }
    }
    NWY_CLI_TCPIP_LOG_DEBUG("exit");
    nwy_sock_hdl_deinit(hdl);
}

// TCP server receive task
static void nwy_server_recv_task(void *param)
{
    nwy_server_hdl_t *hdl = (nwy_server_hdl_t *)param;
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0;
    fd_set rd_fd, ex_fd;
    struct timeval tv = {2, 0};
    int i, maxfd;
    NWY_CLI_TCPIP_LOG_DEBUG("enter");
    
    while (1) {
        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        
        // Check if listen socket is closed
        if(hdl->socket_fd < 0 || nwy_socket_get_state(hdl->socket_fd) == NWY_UNKNOWN) {
            NWY_CLI_TCPIP_LOG_DEBUG("listen socket closed, cleaning up");
            
            // 1. Close and notify each client closure
            for(i = 0; i < hdl->client_count; i++) {
                if(hdl->clients[i].socket_fd > 0) {
                    int client_fd = hdl->clients[i].socket_fd;
                    nwy_socket_close(client_fd);
                    nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED, NULL, 0, client_fd);
                    hdl->clients[i].socket_fd = -1;
                }
            }
            
            // 2. Notify all clients closed
            if(hdl->client_count > 0) {
                nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED_ALL, NULL, 0, hdl->socket_fd);
                hdl->client_count = 0;
            }
            
            // 3. Notify server closed
            nwy_server_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0, hdl->socket_fd);
            
            // 4. Set socket_fd to -1 and clean up resources
            hdl->socket_fd = -1;
            nwy_server_hdl_deinit(hdl);
            break;
        }
        
        // Listen to server socket
        FD_SET(hdl->socket_fd, &rd_fd);
        FD_SET(hdl->socket_fd, &ex_fd);
        maxfd = hdl->socket_fd;

        // Listen to all client sockets
        for (i = 0; i < hdl->client_count; i++) {
            if (hdl->clients[i].socket_fd > 0) {
                FD_SET(hdl->clients[i].socket_fd, &rd_fd);
                FD_SET(hdl->clients[i].socket_fd, &ex_fd);
                if (hdl->clients[i].socket_fd > maxfd)
                    maxfd = hdl->clients[i].socket_fd;
            }
        }

        int result = nwy_socket_select(maxfd + 1, &rd_fd, NULL, &ex_fd, &tv);
        if (result < 0) {
            int err = nwy_socket_errno();
            if(err == EINTR) {
                continue;  // Continue select if interrupted by signal
            }
            NWY_CLI_TCPIP_LOG_DEBUG("select error, errno=%d", err);
            break;
        } else if (result > 0) {
            // Handle new connections
            if (FD_ISSET(hdl->socket_fd, &rd_fd)) {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int new_socket = nwy_socket_accept(hdl->socket_fd, (struct sockaddr *)&client_addr, &addr_len);
                
                if (new_socket > 0) {
                    if (hdl->client_count >= 5) {
                        nwy_socket_close(new_socket);
                        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_BUSYING, NULL, 0, hdl->socket_fd);//Max count of accpeted clients arrived
                    } else {
                        hdl->clients[hdl->client_count].socket_fd = new_socket;
                        hdl->clients[hdl->client_count].af_inet_flag = AF_INET;
                        memcpy(&hdl->clients[hdl->client_count].addr_v4, &client_addr, sizeof(client_addr));
                        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_NEW_CLIENT_ACCEPTED, NULL, 0, new_socket);
                        hdl->client_count++;
                    }
                }
            }

            // Handle client data
            for (i = 0; i < hdl->client_count; i++) {
                int client_fd = hdl->clients[i].socket_fd;
                if (client_fd <= 0)
                    continue;

                if (FD_ISSET(client_fd, &rd_fd)) {
                    memset(recv_buff, 0, sizeof(recv_buff));
                    recv_len = nwy_socket_recv(client_fd, recv_buff, NWY_UART_RECV_SINGLE_MAX, 0);
                    
                    if (recv_len > 0) {
                        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_DATA_RECEIVED, recv_buff, recv_len, client_fd);
                    } else if (recv_len == 0 || (recv_len < 0 && nwy_socket_errno() != EINTR)) {
                        // Client closed connection or error occurred
                        int closed_fd = client_fd;  // Save fd to close
                        
                        // Move array elements
                        if (i < hdl->client_count - 1) {
                            memmove(&hdl->clients[i], &hdl->clients[i + 1],
                                    sizeof(nwy_server_client_t) * (hdl->client_count - i - 1));
                        }
                        hdl->client_count--;
                        i--; // Because elements moved, need to recheck current position
                        
                        // Send close notification
                        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED, NULL, 0, closed_fd);
                        
                        if (hdl->client_count == 0) {
                            nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED_ALL, NULL, 0, hdl->socket_fd);
                        }
                        continue;
                    }
                }

                if (FD_ISSET(client_fd, &ex_fd)) {
                    int closed_fd = client_fd;  // Save fd to close
                    
                    // Move array elements
                    if (i < hdl->client_count - 1) {
                        memmove(&hdl->clients[i], &hdl->clients[i + 1],
                                sizeof(nwy_server_client_t) * (hdl->client_count - i - 1));
                    }
                    hdl->client_count--;
                    i--; // Because elements moved, need to recheck current position
                    
                    // Send close notification
                    nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED, NULL, 0, closed_fd);
                    
                    if (hdl->client_count == 0) {
                        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_ACPT_CLIENT_CLOSED_ALL, NULL, 0, hdl->socket_fd);
                    }
                }
            }
        }
    }
    
    NWY_CLI_TCPIP_LOG_DEBUG("exit");
}

// TCP client callback function
static void nwy_tcp_client_msg_cb(nwy_sock_msg_t *msg)
{
    switch(msg->event) {
        case NWY_APP_SOCKET_GEN_ERR:
            nwy_test_cli_echo("\r\n[%s]TCP client unexpected error(fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_DNS_ERR:
            nwy_test_cli_echo("\r\n[%s]TCP client dns failed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_OPEN_FAIL:
            nwy_test_cli_echo("\r\n[%s]TCP client open failed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_OPENED:
            nwy_test_cli_echo("\r\n[%s]TCP client connected (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_CLOSED:
            nwy_test_cli_echo("\r\n[%s]TCP client closed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_DATA_RECEIVED:
            if(msg->data && msg->data_len > 0) {
                nwy_test_cli_echo("\r\n[%s]TCP client received %d bytes from fd=%d: %s", 
                                __func__, msg->data_len, msg->socket_fd, (char*)msg->data);
            }
            break;
        case NWY_APP_SOCKET_DATA_SEND_ERR:
            nwy_test_cli_echo("\r\n[%s]TCP client send error (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_DATA_SEND_FINISHED:
            nwy_test_cli_echo("\r\n[%s]TCP client send completed (fd=%d)", __func__, msg->socket_fd);
            break;
        default:
            nwy_test_cli_echo("\r\n[%s]TCP client unknown event (fd=%d)", __func__, msg->socket_fd);
            break;
    }
}

// UDP client callback function
static void nwy_udp_client_msg_cb(nwy_sock_msg_t *msg)
{
    switch(msg->event) {
        case NWY_APP_SOCKET_GEN_ERR:
            nwy_test_cli_echo("\r\n[%s]UDP client unexpected error(fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_DNS_ERR:
            nwy_test_cli_echo("\r\n[%s]UDP client dns failed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_udp_hdl) {
                g_udp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_OPEN_FAIL:
            nwy_test_cli_echo("\r\n[%s]UDP client open failed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_udp_hdl) {
                g_udp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_OPENED:
            nwy_test_cli_echo("\r\n[%s]UDP client ready (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_CLOSED:
            nwy_test_cli_echo("\r\n[%s]UDP client closed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_udp_hdl) {
                g_udp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_DATA_RECEIVED:
            if(msg->data && msg->data_len > 0) {
                nwy_test_cli_echo("\r\n[%s]UDP client received %d bytes from fd=%d: %s", 
                                __func__, msg->data_len, msg->socket_fd, (char*)msg->data);
            }
            break;
        case NWY_APP_SOCKET_DATA_SEND_ERR:
            nwy_test_cli_echo("\r\n[%s]UDP client send error (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_DATA_SEND_FINISHED:
            nwy_test_cli_echo("\r\n[%s]UDP client send completed (fd=%d)", __func__, msg->socket_fd);
            break;
        default:
            nwy_test_cli_echo("\r\n[%s]UDP client unknown event (fd=%d)", __func__, msg->socket_fd);
            break;
    }
}

// TCP server callback function
static void nwy_tcp_server_msg_cb(nwy_sock_msg_t *msg)
{
    switch(msg->event) {
        case NWY_APP_SOCKET_GEN_ERR:
            nwy_test_cli_echo("\r\n[%s]TCP server unexpected error(fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_tcp_hdl) {
                g_tcp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_BUSYING:
            nwy_test_cli_echo("\r\n[%s]TCP server (fd=%d) accessing the maximum number of accpted clients", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_OPEN_FAIL:
            nwy_test_cli_echo("\r\n[%s]TCP server start failed (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_OPENED:
            nwy_test_cli_echo("\r\n[%s]TCP server started (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_CLOSED:
            nwy_test_cli_echo("\r\n[%s]TCP server closed (fd=%d)", __func__, msg->socket_fd);
            if(msg->hdl == g_server_hdl) {
                g_udp_hdl = NULL;
            }
            break;
        case NWY_APP_SOCKET_NEW_CLIENT_ACCEPTED:
            nwy_test_cli_echo("\r\n[%s]New client connected (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_DATA_RECEIVED:
            if(msg->data && msg->data_len > 0) {
                nwy_test_cli_echo("\r\n[%s]Received[%d] from client fd=%d: %s", 
                                __func__, msg->data_len, msg->socket_fd, (char*)msg->data);
            }
            break;
        case NWY_APP_SOCKET_DATA_SEND_ERR:
            nwy_test_cli_echo("\r\n[%s]Send to client error (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_DATA_SEND_FINISHED:
            nwy_test_cli_echo("\r\n[%s]Send to client completed (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_ACPT_CLIENT_CLOSED:
            nwy_test_cli_echo("\r\n[%s]Client closed (fd=%d)", __func__, msg->socket_fd);
            break;
        case NWY_APP_SOCKET_ACPT_CLIENT_CLOSED_ALL:
            nwy_test_cli_echo("\r\n[%s]All clients closed", __func__);
            break;
        default:
            nwy_test_cli_echo("\r\n[%s]TCP server unknown event (fd=%d)", __func__, msg->socket_fd);
            break;
    }
}

// TCP client CLI interface
void nwy_test_cli_tcp_setup_ex(void)
{
    char *sptr = NULL;
    nwy_app_socket_opts_s opts = {0};

    if(g_tcp_hdl) {
        nwy_test_cli_echo("\r\nTCP client already running");
        return;
    }

    // Get CID
    sptr = nwy_test_cli_input_gets("\r\nInput CID(1-7): ");
    uint16_t cid = nwy_cli_str_to_int(sptr);
    if(cid < 1 || cid > 7) {
        nwy_test_cli_echo("\r\nInvalid CID");
        return;
    }

    // Get server address
    char host[257] = {0};
    sptr = nwy_test_cli_input_gets("\r\nInput server address: ");
    if(strlen(sptr) > 256) {
        nwy_test_cli_echo("\r\nAddress too long");
        return;
    }
    strncpy(host, sptr, sizeof(host)-1);

    // Get port
    sptr = nwy_test_cli_input_gets("\r\nInput port: ");
    uint16_t port = nwy_cli_str_to_int(sptr);
    if(port <= 0 || port > 65535) {
        nwy_test_cli_echo("\r\nInvalid port");
        return;
    }

    // Set socket options
    opts.nodelay = true;
    opts.reuse_addr = true;
    opts.nonblock = true;

    // Create TCP client
    g_tcp_hdl = nwy_tcp_client_setup(cid, host, port, nwy_tcp_client_msg_cb, &opts);
    if(!g_tcp_hdl) {
        nwy_test_cli_echo("\r\nTCP client setup failed");
        return;
    }

    nwy_test_cli_echo("\r\n TCP client setup ...");
}

void nwy_test_cli_tcp_send_ex(void)
{
    char *sptr = NULL;
    int len = 0;
    char *data = NULL;

    if(!g_tcp_hdl) {
        nwy_test_cli_echo("\r\nTCP client not connected");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nInput data length(1-2048): ");
    len = nwy_cli_str_to_int(sptr);
    if(len <= 0 || len > 2048) {
        nwy_test_cli_echo("\r\nInvalid data length");
        return;
    }

    data = malloc(len + 1);
    if(!data) {
        nwy_test_cli_echo("\r\nMemory allocation failed");
        return;
    }

    memset(data,0,len + 1);
    nwy_test_cli_echo("\r\nInput data: ");
    nwy_cli_get_trans_data(data, len);
    
    if(nwy_tcp_client_send(g_tcp_hdl, data, len) < 0) {
        nwy_test_cli_echo("\r\nSend failed");
    }

    free(data);
}

void nwy_test_cli_tcp_close_ex(void)
{
    if(!g_tcp_hdl) {
        nwy_test_cli_echo("\r\nTCP client not running");
        return;
    }

    nwy_tcp_client_close(g_tcp_hdl);
    nwy_test_cli_echo("\r\nTCP client closed");
}

// UDP client CLI interface
void nwy_test_cli_udp_setup_ex(void)
{
    char *sptr = NULL;
    nwy_app_socket_opts_s opts = {0};

    if(g_udp_hdl) {
        nwy_test_cli_echo("\r\nUDP client already running");
        return;
    }

    // Get CID
    sptr = nwy_test_cli_input_gets("\r\nInput CID(1-7): ");
    uint16_t cid = nwy_cli_str_to_int(sptr);
    if(cid < 1 || cid > 7) {
        nwy_test_cli_echo("\r\nInvalid CID");
        return;
    }

    // Get server address
    char host[257] = {0};
    sptr = nwy_test_cli_input_gets("\r\nInput server address: ");
    if(strlen(sptr) > 256) {
        nwy_test_cli_echo("\r\nAddress too long");
        return;
    }
    strncpy(host, sptr, sizeof(host)-1);

    // Get port
    sptr = nwy_test_cli_input_gets("\r\nInput port: ");
    uint16_t port = nwy_cli_str_to_int(sptr);
    if(port <= 0 || port > 65535) {
        nwy_test_cli_echo("\r\nInvalid port");
        return;
    }

    // Set socket options
    opts.reuse_addr = true;

    // Create UDP client
    g_udp_hdl = nwy_udp_client_setup(cid, host, port, nwy_udp_client_msg_cb, &opts);
    if(!g_udp_hdl) {
        nwy_test_cli_echo("\r\nUDP client setup failed");
        return;
    }

    nwy_test_cli_echo("\r\nUDP client setup success");
}

void nwy_test_cli_udp_send_ex(void)
{
    char *sptr = NULL;
    int len = 0;
    char *data = NULL;

    if(!g_udp_hdl) {
        nwy_test_cli_echo("\r\nUDP client not ready");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nInput data length(1-2048): ");
    len = nwy_cli_str_to_int(sptr);
    if(len <= 0 || len > 2048) {
        nwy_test_cli_echo("\r\nInvalid data length");
        return;
    }

    data = malloc(len + 1);
    if(!data) {
        nwy_test_cli_echo("\r\nMemory allocation failed");
        return;
    }

    memset(data,0,len + 1);
    nwy_test_cli_echo("\r\nInput data: ");
    nwy_cli_get_trans_data(data, len);
    
    if(nwy_udp_client_send(g_udp_hdl, data, len) < 0) {
        nwy_test_cli_echo("\r\nSend failed");
    }

    free(data);
}

void nwy_test_cli_udp_close_ex(void)
{
    if(!g_udp_hdl) {
        nwy_test_cli_echo("\r\nUDP client not running");
        return;
    }

    nwy_udp_client_close(g_udp_hdl);
    g_udp_hdl = NULL;
    nwy_test_cli_echo("\r\nUDP client closed");
}

// TCP server CLI interface
void nwy_test_cli_tcp_server_setup_ex(void)
{
    char *sptr = NULL;
    nwy_app_socket_opts_s opts = {0};

    if(g_server_hdl) {
        nwy_test_cli_echo("\r\nTCP server already running");
        return;
    }

    // Get CID
    sptr = nwy_test_cli_input_gets("\r\nInput CID(1-7): ");
    uint16_t cid = nwy_cli_str_to_int(sptr);
    if(cid < 1 || cid > 7) {
        nwy_test_cli_echo("\r\nInvalid CID");
        return;
    }

    // Get listen port
    sptr = nwy_test_cli_input_gets("\r\nInput listen port: ");
    uint16_t port = nwy_cli_str_to_int(sptr);
    if(port <= 0 || port > 65535) {
        nwy_test_cli_echo("\r\nInvalid port");
        return;
    }

    // Get IP version
    sptr = nwy_test_cli_input_gets("\r\nInput IP version(4/6): ");
    int af_inet_flag;
    if(nwy_cli_str_to_int(sptr) == 4) {
        af_inet_flag = AF_INET;
    } else if(nwy_cli_str_to_int(sptr) == 6) {
        af_inet_flag = AF_INET6;
    } else {
        nwy_test_cli_echo("\r\nInvalid IP version");
        return;
    }

    // Set socket options
    opts.reuse_addr = true;

    // Create TCP server
    g_server_hdl = nwy_tcp_server_setup(cid, port, af_inet_flag, nwy_tcp_server_msg_cb, &opts);
    if(!g_server_hdl) {
        nwy_test_cli_echo("\r\nTCP server setup failed");
        return;
    }

    nwy_test_cli_echo("\r\nTCP server setup success");
}

void nwy_tcp_server_list_accepted_client(nwy_server_hdl_t *server_hdl)
{
    if(server_hdl != NULL && server_hdl->client_count > 0){
        nwy_test_cli_echo("\r\nServer socket_fd:%d,Connected client(s) list:", server_hdl->socket_fd);
        for(int i = 0; i < server_hdl->client_count; i++) {
            if(server_hdl->af_inet_flag == AF_INET) {
                char ip[INET_ADDRSTRLEN];
                nwy_socket_inet_ntop(AF_INET, &server_hdl->clients[i].addr_v4.sin_addr,
                                   ip, sizeof(ip));
                uint16_t port = nwy_socket_ntohs(server_hdl->clients[i].addr_v4.sin_port);
                nwy_test_cli_echo("\r\nClient index %d,socket_fd %d: %s:%u", i, server_hdl->clients[i].socket_fd, ip, port);
            } else {
                char ip[INET6_ADDRSTRLEN];
                nwy_socket_inet_ntop(AF_INET6, &server_hdl->clients[i].addr_v6.sin6_addr,
                                   ip, sizeof(ip));
                uint16_t port = nwy_socket_ntohs(server_hdl->clients[i].addr_v6.sin6_port);
                nwy_test_cli_echo("\r\nClient index %d,socket_fd %d: [%s]:%u", i, server_hdl->clients[i].socket_fd, ip, port);
            }
        }
        nwy_test_cli_echo("\r\nList %d client(s) in total:", server_hdl->client_count);
    }
}

void nwy_test_cli_tcp_server_send_ex(void)
{
    char *sptr = NULL;
    int len = 0;
    char *data = NULL;

    if(!g_server_hdl) {
        nwy_test_cli_echo("\r\nTCP server not running");
        return;
    }

    if(g_server_hdl->client_count == 0) {
        nwy_test_cli_echo("\r\nNo client connected");
        return;
    }

    // Display current connected clients
    nwy_tcp_server_list_accepted_client(g_server_hdl);

    sptr = nwy_test_cli_input_gets("\r\nSelect client index: ");
    int client_index = nwy_cli_str_to_int(sptr);
    if(client_index < 0 || client_index >= g_server_hdl->client_count) {
        nwy_test_cli_echo("\r\nInvalid client index");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\nInput data length(1-2048): ");
    len = nwy_cli_str_to_int(sptr);
    if(len <= 0 || len > 2048) {
        nwy_test_cli_echo("\r\nInvalid data length");
        return;
    }

    data = malloc(len + 1);
    if(!data) {
        nwy_test_cli_echo("\r\nMemory allocation failed");
        return;
    }

    memset(data,0,len + 1);
    nwy_test_cli_echo("\r\nInput data: ");
    nwy_cli_get_trans_data(data, len);
    
    if(nwy_tcp_server_send(g_server_hdl, g_server_hdl->clients[client_index].socket_fd, data, len) < 0) {
        nwy_test_cli_echo("\r\nSend failed");
    }

    free(data);
}

void nwy_test_cli_tcp_server_close_client_ex(void)
{
    char *sptr = NULL;

    if(!g_server_hdl) {
        nwy_test_cli_echo("\r\nTCP server not running");
        return;
    }

    if(g_server_hdl->client_count == 0) {
        nwy_test_cli_echo("\r\nNo client connected");
        return;
    }

    // Display current connected clients
    nwy_tcp_server_list_accepted_client(g_server_hdl);

    sptr = nwy_test_cli_input_gets("\r\nSelect client index to close: ");
    int client_index = nwy_cli_str_to_int(sptr);
    if(client_index < 0 || client_index >= g_server_hdl->client_count) {
        nwy_test_cli_echo("\r\nInvalid client index");
        return;
    }

    nwy_tcp_server_close_client(g_server_hdl, g_server_hdl->clients[client_index].socket_fd);
    nwy_test_cli_echo("\r\nClient closed successfully");
}

void nwy_test_cli_tcp_server_close_ex(void)
{
    if(!g_server_hdl) {
        nwy_test_cli_echo("\r\nTCP server not running");
        return;
    }

    nwy_tcp_server_close(g_server_hdl);
    // Wait for receive thread to complete processing
    nwy_thread_sleep(100);  // Give receive thread some time to handle close events
    g_server_hdl = NULL;
    nwy_test_cli_echo("\r\nTCP server closing...");
}

// TCP server menu function
void nwy_test_cli_tcp_server_menu_ex(void)
{
    nwy_test_cli_echo("\r\nTCP Server Menu:");
    nwy_test_cli_echo("\r\n1. Start Server");
    nwy_test_cli_echo("\r\n2. Send Data");
    nwy_test_cli_echo("\r\n3. Close Client");
    nwy_test_cli_echo("\r\n4. Close Server");
    nwy_test_cli_echo("\r\n5. Exit");
    nwy_test_cli_echo("\r\nPlease select:");
}

// TCP server main function
void nwy_test_cli_tcp_server_ex(void)
{
    char *sptr = NULL;
    int opt = 0;

    while(1) {
        nwy_test_cli_tcp_server_menu_ex();
        sptr = nwy_test_cli_input_gets("\r\n");
        opt = nwy_cli_str_to_int(sptr);

        switch(opt) {
            case 1:
                nwy_test_cli_tcp_server_setup_ex();
                break;
            case 2:
                nwy_test_cli_tcp_server_send_ex();
                break;
            case 3:
                nwy_test_cli_tcp_server_close_client_ex();
                break;
            case 4:
                nwy_test_cli_tcp_server_close_ex();
                break;
            case 5:
                return;
            default:
                nwy_test_cli_echo("\r\nInvalid option");
                break;
        }
    }
}

// TCP client setup function
nwy_sock_hdl_t* nwy_tcp_client_setup(
    uint16_t cid_id,
    const char* host,
    uint16_t port,
    nwy_sock_msg_cb_t cb_func,
    nwy_app_socket_opts_s* opts)
{
    nwy_sock_hdl_t* hdl = NULL;
    nwy_error_e ret = NWY_FAIL;
    unsigned char family;
    char recv_thread_name[64] = {0};
    if(!host || !cb_func || port <= 0 || port > 65535 || cid_id < 1 || cid_id > 7) {
        nwy_test_cli_echo("\r\nparam invalid");
        goto FAIL;
    }

    // Check data connection
    if (nwy_sock_data_call_check(cid_id) == false) {
        nwy_test_cli_echo("\r\ndata call check failed");
        goto FAIL;
    }

    // Initialize handle
    hdl = nwy_sock_hdl_init();
    if (hdl == NULL) {
        goto FAIL;
    }
    hdl->cid = cid_id;
    hdl->port = port;
    hdl->msg_cb = cb_func;
    hdl->is_tcp = 1;
    strncpy(hdl->host, host, sizeof(hdl->host)-1);
    
    if (opts) {
        memcpy(&hdl->opts, opts, sizeof(nwy_app_socket_opts_s));
    }

    // DNS resolution
    family = nwy_sock_dns_parse(hdl);
    if(family != AF_INET6 && family != AF_INET) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DNS_ERR, NULL, 0);
        goto FAIL;
    }

    // Create socket
    hdl->socket_fd = nwy_socket_open(family, SOCK_STREAM, IPPROTO_TCP, hdl->cid);
    if (hdl->socket_fd < 0) {
        NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_open failed");
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
        goto FAIL;
    }

    // Set socket options
    if (hdl->opts.reuse_addr) {
        ret = nwy_socket_set_reuseaddr(hdl->socket_fd);
        if(ret != NWY_SUCCESS){
            NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_set_reuseaddr failed");
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
            goto FAIL;
        }
    }
    if (hdl->opts.nodelay) {
        ret = nwy_socket_set_nodelay(hdl->socket_fd);
        if(ret != NWY_SUCCESS){
            NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_set_nodelay failed");
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
            goto FAIL;
        }
    }
    if (hdl->opts.nonblock) {
        ret = nwy_socket_set_nonblock(hdl->socket_fd);
        if(ret != NWY_SUCCESS){
            NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_set_nonblock failed");
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
            goto FAIL;
        }
    }
    if (hdl->opts.keepalive) {
        ret = nwy_socket_set_keepalive(hdl->socket_fd, 1, hdl->opts.keepidle, hdl->opts.keepintvl);
        if(ret != NWY_SUCCESS){
            NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_set_keepalive failed");
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
            goto FAIL;
        }
    }

    // Establish connection
    if(family == AF_INET) {
        ret = nwy_socket_connect(hdl->socket_fd, (const struct sockaddr *)&hdl->s_nwy_server_v4,
                               sizeof(hdl->s_nwy_server_v4));
    } else {
        ret = nwy_socket_connect(hdl->socket_fd, (const struct sockaddr *)&hdl->s_nwy_server_v6,
                               sizeof(hdl->s_nwy_server_v6));
    }

    if(ret == NWY_SUCCESS){
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPENED, NULL, 0);
    }else{
        if (hdl->opts.nonblock){
            if (EISCONN == nwy_socket_errno()){
                NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_connect connect ok");
                nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPENED, NULL, 0);
            } else if (EINPROGRESS == nwy_socket_errno() || EALREADY == nwy_socket_errno()){
                NWY_CLI_TCPIP_LOG_DEBUG("nwy_socket_connect connecting...");
            } else {
                NWY_CLI_TCPIP_LOG_DEBUG("connect errno = %d", nwy_socket_errno());
                nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
                goto FAIL;
            }
        }else{
            nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
            goto FAIL;
        }
    }
    sprintf(recv_thread_name, "tcp_client_recv_thread-%d", hdl->socket_fd);
    ret = nwy_thread_create(&hdl->recv_thread,
                            recv_thread_name, 
                            NWY_OSI_PRIORITY_ABOVE_NORMAL,
                            nwy_tcp_recv_task, 
                            hdl, 
                            NWY_TEST_CLI_TCPIP_THREAD_EVENT_COUNT,
                            NWY_TEST_CLI_TCPIP_THREAD_STACK_SIZE,
                            NULL);
    if (hdl->recv_thread == NULL || ret != NWY_SUCCESS) {
        NWY_CLI_TCPIP_LOG_DEBUG("TCP client recv thread create failed");
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
        goto FAIL;
    }
    
    return hdl;
FAIL:
    nwy_sock_hdl_deinit(hdl);
    return NULL;
}

int nwy_tcp_client_send(nwy_sock_hdl_t* hdl, const void* data, size_t len)
{
    if(!hdl || !data || len == 0 || hdl->socket_fd < 0) {
        return -1;
    }

    // Send data
    int sent = nwy_socket_send(hdl->socket_fd, data, len, 0);
    if(sent < 0) {
        // Send failed
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_ERR, NULL, 0);
        return -1;
    } else if(sent > 0) {
        // Send succeeded
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_FINISHED, NULL, 0);
    }

    return sent;
}

void nwy_tcp_client_close(nwy_sock_hdl_t* hdl)
{
    if (hdl != NULL && hdl->is_tcp == 1) {
        nwy_socket_close(hdl->socket_fd);
        return;
    }
    NWY_CLI_TCPIP_LOG_DEBUG("param invalid!");
}

// UDP client setup function
nwy_sock_hdl_t* nwy_udp_client_setup(
    uint16_t cid_id,
    const char* host,
    uint16_t port,
    nwy_sock_msg_cb_t cb_func,
    nwy_app_socket_opts_s* opts)
{
    if(!host || !cb_func || port <= 0 || port > 65535 || cid_id < 1 || cid_id > 7) {
        nwy_test_cli_echo("\r\nparam invalid");
        goto FAIL;
    }

    // Allocate handle memory
    nwy_sock_hdl_t* hdl = NULL;
    nwy_error_e ret = NWY_FAIL;
    char recv_thread_name[64] = {0};
    // Check data connection
    if (nwy_sock_data_call_check(cid_id) == false) {
        nwy_test_cli_echo("\r\ndata call check failed");
        goto FAIL;
    }
    // Initialize handle
    hdl = nwy_sock_hdl_init();
    if(!hdl) {
        goto FAIL;
    }
    // Set basic parameters
    hdl->cid = cid_id;
    hdl->port = port;
    hdl->msg_cb = cb_func;
    hdl->is_tcp = 0;
    strncpy(hdl->host, host, sizeof(hdl->host)-1);

    // DNS resolution error notification
    int family = nwy_sock_dns_parse(hdl);
    if(family != AF_INET && family != AF_INET6) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DNS_ERR, NULL, 0);
        goto FAIL;
    }

    // Create UDP socket
    hdl->socket_fd = nwy_socket_open(family, SOCK_DGRAM, IPPROTO_UDP, cid_id);
    if(hdl->socket_fd < 0) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
        goto FAIL;
    }

    // Apply socket options
    if(opts) {
        if(opts->reuse_addr) {
            nwy_socket_set_reuseaddr(hdl->socket_fd);
        }
        if(opts->nonblock) {
            nwy_socket_set_nonblock(hdl->socket_fd);
        }
    }

    // Create receive thread
    sprintf(recv_thread_name, "udp_recv_thread-%d", hdl->socket_fd);
    ret = nwy_thread_create(&hdl->recv_thread,
                            recv_thread_name,
                            NWY_OSI_PRIORITY_ABOVE_NORMAL,
                            nwy_udp_recv_task,
                            hdl,
                            NWY_TEST_CLI_TCPIP_THREAD_EVENT_COUNT,
                            NWY_TEST_CLI_TCPIP_THREAD_STACK_SIZE,
                            NULL);

    if(!hdl->recv_thread || ret != NWY_SUCCESS) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0);
        goto FAIL;
    }

    nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_OPENED, NULL, 0);
    return hdl;
FAIL:
    nwy_sock_hdl_deinit(hdl);
    return NULL;
}

// UDP client send data
int nwy_udp_client_send(nwy_sock_hdl_t* hdl, const void* data, size_t len)
{
    if(!hdl || !data || len == 0 || hdl->socket_fd < 0) {
        return -1;
    }

    int sent;
    if(hdl->s_nwy_server_v4.sin_family == AF_INET) {
        sent = nwy_socket_sendto(hdl->socket_fd, data, len, 0,
                               (struct sockaddr*)&hdl->s_nwy_server_v4,
                               sizeof(hdl->s_nwy_server_v4));
    } else {
        sent = nwy_socket_sendto(hdl->socket_fd, data, len, 0,
                               (struct sockaddr*)&hdl->s_nwy_server_v6,
                               sizeof(hdl->s_nwy_server_v6));
    }

    if(sent < 0) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_ERR, NULL, 0);
        return -1;
    } else if(sent > 0) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_FINISHED, NULL, 0);
    }

    return sent;
}

// UDP client close
void nwy_udp_client_close(nwy_sock_hdl_t* hdl)
{
    if (hdl != NULL && hdl->is_tcp == 0) {
        nwy_sock_msg_notify(hdl, NWY_APP_SOCKET_CLOSED, NULL, 0);
        nwy_socket_close(hdl->socket_fd);
        return;
    }
    NWY_CLI_TCPIP_LOG_DEBUG("param invalid!");
}

// TCP server setup function
nwy_server_hdl_t* nwy_tcp_server_setup(
    uint16_t cid_id,
    uint16_t port,
    int af_inet_flag,
    nwy_sock_msg_cb_t cb_func,
    nwy_app_socket_opts_s* opts)
{
    nwy_server_hdl_t* hdl = NULL;
    nwy_error_e ret = NWY_FAIL;
    char recv_thread_name[64] = {0};
    // Check data connection
    if (nwy_sock_data_call_check(cid_id) == false) {
        goto FAIL;
    }

    // Initialize handle
    hdl = nwy_server_hdl_init();
    if (hdl == NULL) {
        goto FAIL;
    }
    hdl->cid = cid_id;
    hdl->port = port;
    hdl->msg_cb = cb_func;
    hdl->af_inet_flag = af_inet_flag;

    // Create socket
    hdl->socket_fd = nwy_socket_open(af_inet_flag, SOCK_STREAM, IPPROTO_TCP, cid_id);
    if(hdl->socket_fd < 0) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0, -1);
        goto FAIL;
    }

    // Set socket options
    if (opts) {
        if (opts->reuse_addr) {
            nwy_socket_set_reuseaddr(hdl->socket_fd);
        }
        if (opts->nodelay) {
            nwy_socket_set_nodelay(hdl->socket_fd);
        }
        if (opts->nonblock) {
            nwy_socket_set_nonblock(hdl->socket_fd);
        }
    }

    // Bind port
    ret = nwy_socket_lport_bind(hdl->socket_fd, port);
    if (ret != NWY_SUCCESS) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0, hdl->socket_fd);
        goto FAIL;
    }

    // Start listening
    ret = nwy_socket_listen(hdl->socket_fd, 5);
    if (ret != NWY_SUCCESS) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0, hdl->socket_fd);
        goto FAIL;
    }

    // Create receive thread
    sprintf(recv_thread_name, "server_recv_thread-%d-%d", hdl->socket_fd,port);
    ret = nwy_thread_create(&hdl->recv_thread,
                            recv_thread_name , 
                            NWY_OSI_PRIORITY_ABOVE_NORMAL, 
                            nwy_server_recv_task, 
                            hdl, 
                            NWY_TEST_CLI_TCPIP_THREAD_EVENT_COUNT,
                            NWY_TEST_CLI_TCPIP_THREAD_STACK_SIZE, 
                            NULL);
    if (hdl->recv_thread == NULL || ret != NWY_SUCCESS) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_OPEN_FAIL, NULL, 0, hdl->socket_fd);
        goto FAIL;
    }

    nwy_server_msg_notify(hdl, NWY_APP_SOCKET_OPENED, NULL, 0, hdl->socket_fd);
    return hdl;
FAIL:
    nwy_server_hdl_deinit(hdl);
    return NULL;
}

// TCP server send data to specified client
int nwy_tcp_server_send(nwy_server_hdl_t* hdl, int client_fd, const void* data, size_t len)
{
    if(!hdl || !data || len == 0 || client_fd < 0) {
        return -1;
    }

    // Verify client_fd belongs to this server
    int i;
    bool found = false;
    for(i = 0; i < hdl->client_count; i++) {
        if(hdl->clients[i].socket_fd == client_fd) {
            found = true;
            break;
        }
    }
    
    if(!found) {
        return -1;
    }

    int sent = nwy_socket_send(client_fd, data, len, 0);
    if(sent < 0) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_ERR, NULL, 0, client_fd);
        return -1;
    } else if(sent > 0) {
        nwy_server_msg_notify(hdl, NWY_APP_SOCKET_DATA_SEND_FINISHED, NULL, 0, client_fd);
    }

    return sent;
}

// TCP server close specified client
void nwy_tcp_server_close_client(nwy_server_hdl_t* hdl, int client_fd)
{
    if (hdl == NULL || client_fd < 0)
        return;

    int i;
    for(i = 0; i < hdl->client_count; i++) {
        if(hdl->clients[i].socket_fd == client_fd) {
            // Close only socket, let recv_task handle subsequent cleanup
            nwy_socket_close(client_fd);
            break;
        }
    }
}

// TCP server close
void nwy_tcp_server_close(nwy_server_hdl_t* hdl)
{
    if (hdl != NULL && hdl->socket_fd > 0) {
        // Close only server listening socket, let receive thread handle subsequent cleanup
        nwy_socket_close(hdl->socket_fd);
    }
}


