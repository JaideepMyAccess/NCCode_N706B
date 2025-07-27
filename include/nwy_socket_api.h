/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_socket_api.h
 * Author       : hujun
 * Created      : 2023-6-1
 * Description  : Socket API function declarations
 *
 *****************************************************************************
 */
#ifndef _NWY_SOCKET_API_H_
#define _NWY_SOCKET_API_H_
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_common.h"
#include "nwy_osi_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */

#define NWY_DEFAULT_NETIF_ID        (1)
/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum {
    NWY_UNKNOWN      = -1,
    NWY_CLOSED      = 0,
    NWY_LISTEN      = 1,
    NWY_SYN_SENT    = 2,
    NWY_SYN_RCVD    = 3,
    NWY_ESTABLISHED = 4,
    NWY_FIN_WAIT_1  = 5,
    NWY_FIN_WAIT_2  = 6,
    NWY_CLOSE_WAIT  = 7,
    NWY_CLOSING     = 8,
    NWY_LAST_ACK    = 9,
    NWY_TIME_WAIT   = 10
}nwy_tcp_state_e;

/*
 *****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 5 STRUCT Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 6 UNION Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */

/*
*****************************************************************************
* Prototype     : nwy_socket_errno
* Description   : Get socket error
* Input         : NA
* Output        : NA
* Return Value  : Return error code
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_errno(void);
/*
*****************************************************************************
* Prototype     : nwy_socket_open
* Description   : Create socket
* Input         : domain: protocol family, commonly used families are AF_INET (IPv4)/ AF_INET6(IPv6)
                  type: socket type
                  protocol: communication protocol used, IPPROTO_TCP, IPPROTO_TCP, etc.
                  cid: cid, when cid is 0, use default netif
* Output        : NA
* Return Value  : Return socket id
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_open(int domain, int type, int protocol, int cid);

/*
*****************************************************************************
* Prototype     : nwy_socket_send
* Description   : Socket send data
* Input         : socket: socket id
                  data: data to send
                  size: length of data to send
                  flags: 0
* Output        : NA
* Return Value  : Actual length sent
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_send(int socket, const void *data, size_t size, int flags);
/*
*****************************************************************************
* Prototype     : nwy_socket_recv
* Description   : Socket receive data
* Input         : socket: socket id
                  len: maximum length of receive data buffer
                  flags: 0
* Output        : data: received data
* Return Value  : Length of received data
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_recv(int socket, void *data, size_t len, int flags);

/*
*****************************************************************************
* Prototype     : nwy_socket_sendto
* Description   : Socket send data to specified address
* Input         : socket: socket id
                  data: data to send
                  size: length of data to send
                  flags: 0
                  to: destination address
                  tolen: length
* Output        : NA
* Return Value  : Actual length sent
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_sendto(int socket, const void *data, size_t size, int flags,const struct sockaddr *to, socklen_t tolen);

/*
*****************************************************************************
* Prototype     : nwy_socket_recvfrom
* Description   : Socket receive data from specified address
* Input         : socket: socket id
                  size: length of receive data buffer
                  flags: 0
                  from: source address
                  tolen: length
* Output        : NA
* Return Value  : Actual length received
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_recvfrom(int socket, void *data, size_t len, int flags,struct sockaddr *from, socklen_t *fromlen);
/*
*****************************************************************************
* Prototype     : nwy_socket_setsockopt
* Description   : Set socket properties
* Input         : socket: socket id
                  level: option level, currently only supports SOL_SOCKET and IPPROTO_TCP levels
                  optname: option to set
                  optval: pointer to buffer containing option value
                  optlen: length of optval buffer
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_setsockopt(int socket, int level, int optname, const void * optval, socklen_t optlen);
/*
*****************************************************************************
* Prototype     : nwy_socket_getsockopt
* Description   : Get socket properties
* Input         : socket: socket id
                  level: option level, currently only supports SOL_SOCKET and IPPROTO_TCP levels
                  optname: option to get
* Output        : optval: pointer to buffer to store option value
                  optlen: length of optval
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_getsockopt(int socket, int level, int optname, void * optval, socklen_t * optlen);
/*
*****************************************************************************
* Prototype     : nwy_socket_gethost_by_name
* Description   : Get IP address by domain name
* Input         : name: domain name string
                  cid: cid, when cid is 0, use default netif
* Output        : isipv6: whether it is IPv6
* Return Value  : Return obtained IP address
* Author        : hujun
*****************************************************************************
*/
char* nwy_socket_gethost_by_name(const char *name, int *isipv6, int cid);

/*
*****************************************************************************
* Prototype     : nwy_socket_close
* Description   : Close socket
* Input         : socket: socket id
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_close(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_shutdown
* Description   : Shut down socket
* Input         : socket: socket id
                  how: shutdown method, SHUT_RD (shutdown input stream), SHUT_WR (shutdown output stream), SHUT_RDWR (shutdown both I/O streams)
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_shutdown(int socket, int how);
/*
*****************************************************************************
* Prototype     : nwy_socket_connect
* Description   : Socket connect
* Input         : socket: socket id
                  name: client address
                  namelen: name length
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_connect(int socket, const struct sockaddr *name, socklen_t namelen);

/*
*****************************************************************************
* Prototype     : nwy_socket_lport_bind
* Description   : Socket bind to specified port
* Input         : socket: socket id
                  lport: port number
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_lport_bind(int socket, uint16_t lport);
/*
*****************************************************************************
* Prototype     : nwy_socket_bind
* Description   : Socket bind to specified address
* Input         : socket: socket id
                  addr: specified address
                  addrlen: addr length
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_bind(int socket, const struct sockaddr *addr, socklen_t addrlen);

/*
*****************************************************************************
* Prototype     : nwy_socket_listen
* Description   : Socket listen
* Input         : socket: socket id
                  backlog: maximum number of connections to queue
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_listen(int socket, int backlog);
/*
*****************************************************************************
* Prototype     : nwy_socket_accept
* Description   : Socket accept, mainly used for servers
* Input         : socket: socket id
                  addr: accept socket address
                  addrlen: length of accept socket address
* Output        : NA
* Return Value  : Socket id of accepted socket
* Author        : hujun
*****************************************************************************
*/

int nwy_socket_accept(int socket, struct sockaddr *addr, socklen_t *addrlen);
/*
*****************************************************************************
* Prototype     : nwy_socket_get_state
* Description   : Get socket state
* Input         : socket: socket id
* Output        : NA
* Return Value  : Socket state, see nwy_tcp_state_e for details
* Author        : hujun
*****************************************************************************
*/
nwy_tcp_state_e nwy_socket_get_state(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_select
* Description   : Monitor socket
* Input         : maxfdp1: total number of file descriptors to monitor
                  readfds, writefds, exceptset: descriptor sets for readable, writable, and exceptional events
                  timeout: timeout period
* Output        : NA
* Return Value  : Returns 0 on timeout; returns -1 on failure; returns a positive integer on success, which represents the number of ready descriptors
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,nwy_timeval_t *timeout);

/*
*****************************************************************************
* Prototype     : nwy_socket_set_nonblock
* Description   : Set socket non-blocking
* Input         : socket: socket id
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_set_nonblock(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_get_state
* Description   : Reuse address
* Input         : socket: socket id
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_set_reuseaddr(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_set_nodelay
* Description   : TCP_NODELAY option is used to control whether to enable Nagle algorithm
* Input         : socket: socket id
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/

nwy_error_e nwy_socket_set_nodelay(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_set_keepalive
* Description   : Set TCP keepalive
* Input         : socket: socket id
                  mode: 1 to enable, 0 to disable
                  idle: channel idle interval
                  interval: packet sending interval
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_set_keepalive(int socket ,uint32_t mode ,uint32_t idle ,uint32_t interval);

/*
*****************************************************************************
* Prototype     : nwy_socket_get_keepalive
* Description   : Get TCP keepalive
* Input         : socket: socket id
* Output        : mode: 1 enabled, 0 disabled
                  idle: channel idle interval
                  interval: packet sending interval
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_get_keepalive(int socket ,uint32_t *mode ,uint32_t *idle ,uint32_t *interval);

/*
*****************************************************************************
* Prototype     : nwy_socket_set_linger
* Description   : Set socket linger
* Input         : socket: socket id
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_set_linger(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_get_ack
* Description   : Get socket received ACK
* Input         : socket: socket id
* Output        : NA
* Return Value  : Total number of ACKs, which is the actual number of successfully sent data
* Author        : hujun
*****************************************************************************
*/

int nwy_socket_get_ack(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_get_sent
* Description   : Get socket sent data count
* Input         : socket: socket id
* Output        : NA
* Return Value  : Total data sent by socket
* Author        : hujun
*****************************************************************************
*/
int nwy_socket_get_sent(int socket);
/*
*****************************************************************************
* Prototype     : nwy_socket_inet_ntop
* Description   : Convert IPv4 or IPv6 Internet network address to Internet standard format string
* Input         : family: protocol type, AF_INET6 and AF_INET
                  src: network address
                  size: length of src address
* Output        : dst: string
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_inet_ntop(int family, void *src, char *dst, int size);
/*
*****************************************************************************
* Prototype     : nwy_socket_get_state
* Description   : Convert IPv4 or IPv6 Internet network address from standard text representation to numeric binary form
* Input         : family: protocol type, AF_INET6 and AF_INET
                  src: IP address string
                  dst: converted binary address
* Output        : NA
* Return Value  : nwy_error_e
* Author        : hujun
*****************************************************************************
*/
nwy_error_e nwy_socket_inet_pton(int family, char *src, void *dst);

/*
*****************************************************************************
* Prototype     : nwy_socket_htons
* Description   : Used to convert a 16-bit unsigned integer from host byte order to network byte order
* Input         : value before conversion
* Output        :
* Return Value  : value after conversion
* Author        : hujun
*****************************************************************************
*/
int16  nwy_socket_htons(int16 value);

/*
*****************************************************************************
* Prototype     : nwy_socket_ntohs
* Description   : Used to convert a 16-bit unsigned integer from network byte order to host byte order
* Input         : value before conversion
* Output        :
* Return Value  : value after conversion
* Author        : hujun
*****************************************************************************
*/
int16  nwy_socket_ntohs(int16 value);

/*
*****************************************************************************
* Prototype     : nwy_socket_htonl
* Description   : Used to convert a 16-bit unsigned integer from host byte order to network byte order
* Input         : value before conversion
* Output        :
* Return Value  : value after conversion
* Author        : hujun
*****************************************************************************
*/
int  nwy_socket_htonl(int value);

/*
*****************************************************************************
* Prototype     : nwy_socket_ntohl
* Description   : Used to convert a 16-bit unsigned integer from network byte order to host byte order
* Input         : value before conversion
* Output        :
* Return Value  : value after conversion
* Author        : hujun
*****************************************************************************
*/
int  nwy_socket_ntohl(int value);

#endif

