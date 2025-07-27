
/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_osi_api.h
 * Author       : hujun
 * Created      : 2017-3-29
 * Description  : osi API º¯ÊýÉùÃ÷
 *
 *****************************************************************************
 */

#ifndef _NWY_ADAPT_PLAT_H_
#define _NWY_ADAPT_PLAT_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#ifndef FEATURE_NWY_OPEN_APP
#include "sockets.h"
#include "netdb.h"
#else
#include "nwy_common.h"

#ifndef LWIP_IPV6
#define LWIP_IPV6       1
#endif

#define AF_UNSPEC       0
#define AF_INET         2
#if LWIP_IPV6
#define AF_INET6        10
#else /* LWIP_IPV6 */
#define AF_INET6        AF_UNSPEC
#endif /* LWIP_IPV6 */
#define PF_INET         AF_INET
#define PF_INET6        AF_INET6
#define PF_UNSPEC       AF_UNSPEC

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#if LWIP_IPV6
#define IPPROTO_IPV6    41
#define IPPROTO_ICMPV6  58
#endif /* LWIP_IPV6 */
#define IPPROTO_UDPLITE 136
#define IPPROTO_RAW     255

#define TCP_NODELAY    0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE   0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL  0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT    0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */


/*
 * Option flags per-socket. These must match the SOF_ flags in ip.h (checked in init.c)
 */
#define SO_REUSEADDR   0x0004 /* Allow local address reuse */
#define SO_KEEPALIVE   0x0008 /* keep connections alive */
#define SO_BROADCAST   0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */


/*
 * Additional options, not kept in so_options.
 */
#define SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_SNDBUF       0x1001 /* Unimplemented: send buffer size */
#define SO_RCVBUF       0x1002 /* receive buffer size */
#define SO_SNDLOWAT     0x1003 /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT     0x1004 /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO     0x1005 /* send timeout */
#define SO_RCVTIMEO     0x1006 /* receive timeout */
#define SO_ERROR        0x1007 /* get error status and clear */
#define SO_TYPE         0x1008 /* get socket type */
#define SO_CONTIMEO     0x1009 /* Unimplemented: connect timeout */
#define SO_NO_CHECK     0x100a /* don't create UDP checksum */
#define SO_BINDTODEVICE 0x100b /* bind to device */

/* Flags we can use with send and recv. */
#define MSG_PEEK       0x01    /* Peeks at an incoming message */
#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define MSG_MORE       0x10    /* Sender will send more */
#define MSG_NOSIGNAL   0x20    /* Uninmplemented: Requests not to send the SIGPIPE signal if an attempt to send is made on a stream-oriented socket that is no longer connected. */
/* commands for fnctl */
#ifndef F_GETFL
#define F_GETFL 3
#endif
#ifndef F_SETFL
#define F_SETFL 4
#endif

/* File status flags and file access modes for fnctl,
   these are bits in an int. */
#ifndef O_NONBLOCK
#define O_NONBLOCK  1 /* nonblocking I/O */
#endif
#ifndef O_NDELAY
#define O_NDELAY    1 /* same as O_NONBLOCK, for compatibility */
#endif
#ifndef O_RDONLY
#define O_RDONLY    2
#endif
#ifndef O_WRONLY
#define O_WRONLY    4
#endif
#ifndef O_RDWR
#define O_RDWR      (O_RDONLY|O_WRONLY)
#endif

#ifndef SHUT_RD
  #define SHUT_RD   0
  #define SHUT_WR   1
  #define SHUT_RDWR 2
#endif
#define IP4ADDR_STRLEN_MAX  16
#define IP6ADDR_STRLEN_MAX    46

#ifndef INADDR_NONE
/** 255.255.255.255 */
#define INADDR_NONE         ((uint32)0xffffffffUL)
#endif

#ifndef INADDR_LOOPBACK
/** 127.0.0.1 */
#define INADDR_LOOPBACK     ((uint32)0x7f000001UL)
#endif

#ifndef INADDR_ANY
/** 0.0.0.0 */
#define INADDR_ANY          ((uint32)0x00000000UL)
#endif

#ifndef INADDR_BROADCAST
/** 255.255.255.255 */
#define INADDR_BROADCAST    ((uint32)0xffffffffUL)
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN     IP4ADDR_STRLEN_MAX
#endif
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN    IP6ADDR_STRLEN_MAX
#endif
/*
 * Structure used for manipulating linger option.
 */
struct linger {
  int l_onoff;                /* option on/off */
  int l_linger;               /* linger time in seconds */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define  SOL_SOCKET  0xfff    /* options for socket level */

struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};

#ifndef SHUT_RD
  #define SHUT_RD   0
  #define SHUT_WR   1
  #define SHUT_RDWR 2
#endif


typedef int socklen_t;

/* lawrence start */
#define AI_PASSIVE      0x01
struct addrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    socklen_t         ai_addrlen;    /* Length of socket address. */
    struct sockaddr  *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo  *ai_next;       /* Pointer to next in list. */
};

struct hostent {
    char  *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int    h_addrtype;  /* Address type. */
    int    h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};
/* lawrence end */

typedef unsigned char uint8;
#if !defined(sa_family_t) && !defined(SA_FAMILY_T_DEFINED)
typedef uint8 sa_family_t;
#endif

#if !defined(in_port_t) && !defined(IN_PORT_T_DEFINED)
typedef unsigned short   in_port_t;
#endif

/* If your port already typedef's in_addr_t, define IN_ADDR_T_DEFINED
   to prevent this code from redefining it. */
#if !defined(in_addr_t) && !defined(IN_ADDR_T_DEFINED)
typedef uint32 in_addr_t;
#endif

struct in_addr {
  in_addr_t s_addr;
};

struct in6_addr {
  union {
    uint32 u32_addr[4];
    uint8  u8_addr[16];
  } un;
#define s6_addr  un.u8_addr
};

enum sal_ip_addr_type {
    /** IPv4 */
    IPADDR_TYPE_V4 =   0U,
    /** IPv6 */
    IPADDR_TYPE_V6 =   6U,
    /** IPv4+IPv6 ("dual-stack") */
    IPADDR_TYPE_ANY = 46U
};

typedef struct ip4_addr {
    uint32 addr;
} ip4_addr_t;

typedef struct ip6_addr {
    uint32 addr[4];

#if 1 //LWIP_IPV6_SCOPES
    uint8 zone;
#endif /* LWIP_IPV6_SCOPES */
} ip6_addr_t;

typedef struct _ip_addr {
    union {
        ip6_addr_t ip6;
        ip4_addr_t ip4;
    } u_addr;
    /** @ref sal_ip_addr_type */
    uint8 type;
} ip_addr_t;



//#if LWIP_IPV4
/* members are in network byte order */
struct sockaddr_in {
  uint8            sin_len;
  sa_family_t     sin_family;
  in_port_t       sin_port;
  struct in_addr  sin_addr;
#define SIN_ZERO_LEN 8
  char            sin_zero[SIN_ZERO_LEN];
};
//#endif /* LWIP_IPV4 */

//#if LWIP_IPV6
struct sockaddr_in6 {
  uint8            sin6_len;      /* length of this structure    */
  sa_family_t     sin6_family;   /* AF_INET6                    */
  in_port_t       sin6_port;     /* Transport layer port #      */
  uint32           sin6_flowinfo; /* IPv6 flow information       */
  struct in6_addr sin6_addr;     /* IPv6 address                */
};
//#endif /* LWIP_IPV6 */

struct sockaddr {
  uint8 sa_len;
  uint8 sa_family;
  uint8 sa_data[22];
};

struct sockaddr_storage {
  uint8        s2_len;
  sa_family_t ss_family;
  char        s2_data1[2];
  uint8       s2_data2[3];
//#if LWIP_IPV6
  uint32       s2_data3[3];
//#endif /* LWIP_IPV6 */
};

/**
 * Convert an uint16 from host- to network byte order.
 *
 * @param n uint16 in host byte order
 * @return n in network byte order
 */
static inline uint16 lwip_htons(uint16 n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

/**
 * Convert an uint16 from network- to host byte order.
 *
 * @param n uint16 in network byte order
 * @return n in host byte order
 */
static inline uint16 lwip_ntohs(uint16 n)
{
  return lwip_htons(n);
}

/**
 * Convert an uint32 from host- to network byte order.
 *
 * @param n uint32 in host byte order
 * @return n in network byte order
 */
static inline uint32 lwip_htonl(uint32 n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

/**
 * Convert an uint32 from network- to host byte order.
 *
 * @param n uint32 in network byte order
 * @return n in host byte order
 */
static inline uint32 lwip_ntohl(uint16 n)
{
  return lwip_htonl(n);
}
#define lwip_ntohs(x) lwip_htons(x)
#define lwip_ntohl(x) lwip_htonl(x)

/* These macros should be calculated by the preprocessor and are used
   with compile-time constants only (so that there is no little-endian
   overhead at runtime). */
#define PP_HTONS(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#define PP_NTOHS(x) PP_HTONS(x)
#define PP_HTONL(x) ((((x) & 0x000000ffUL) << 24) | \
                     (((x) & 0x0000ff00UL) <<  8) | \
                     (((x) & 0x00ff0000UL) >>  8) | \
                     (((x) & 0xff000000UL) >> 24))
#define PP_NTOHL(x) PP_HTONL(x)


/* Provide usual function names as macros for users, but this can be turned off */
#define htons(x) lwip_htons(x)
#define ntohs(x) lwip_ntohs(x)
#define htonl(x) lwip_htonl(x)
#define ntohl(x) lwip_ntohl(x)

#define inet_addr_from_ip4addr(target_inaddr, source_ipaddr) ((target_inaddr)->s_addr = ip4_addr_get_u32(source_ipaddr))
#define inet_addr_to_ip4addr(target_ipaddr, source_inaddr)   (ip4_addr_set_u32(target_ipaddr, (source_inaddr)->s_addr))

/* directly map this to the lwip internal functions */
#define inet_addr(cp)                   nwy_ipv4_addr_addr(cp)
#define inet_aton(cp, addr)             nwy_ipv4_addr_aton(cp, (ip4_addr_t*)addr)
#define inet_ntoa(addr)                 nwy_ipv4_addr_ntoa((const ip4_addr_t*)&(addr))
#define inet_ntoa_r(addr, buf, buflen)  nwy_ipv4_addr_ntoa_r((const ip4_addr_t*)&(addr), buf, buflen)

//#endif /* LWIP_IPV4 */

//#if LWIP_IPV6
#define inet6_addr_from_ip6addr(target_in6addr, source_ip6addr) {(target_in6addr)->un.u32_addr[0] = (source_ip6addr)->addr[0]; \
                                                                 (target_in6addr)->un.u32_addr[1] = (source_ip6addr)->addr[1]; \
                                                                 (target_in6addr)->un.u32_addr[2] = (source_ip6addr)->addr[2]; \
                                                                 (target_in6addr)->un.u32_addr[3] = (source_ip6addr)->addr[3];}
#define inet6_addr_to_ip6addr(target_ip6addr, source_in6addr)   {(target_ip6addr)->addr[0] = (source_in6addr)->un.u32_addr[0]; \
                                                                 (target_ip6addr)->addr[1] = (source_in6addr)->un.u32_addr[1]; \
                                                                 (target_ip6addr)->addr[2] = (source_in6addr)->un.u32_addr[2]; \
                                                                 (target_ip6addr)->addr[3] = (source_in6addr)->un.u32_addr[3]; \
                                                                 ip6_addr_clear_zone(target_ip6addr);}

/* directly map this to the lwip internal functions */
#define inet6_aton(cp, addr)            ip6addr_aton(cp, (ip6_addr_t*)addr)
#define inet6_ntoa(addr)                ip6addr_ntoa((const ip6_addr_t*)&(addr))
#define inet6_ntoa_r(addr, buf, buflen) ip6addr_ntoa_r((const ip6_addr_t*)&(addr), buf, buflen)

//#endif /* LWIP_IPV6 */
#if LWIP_IPV6
/** @ingroup ip4addr */
#define IP_IS_V4_VAL(ipaddr)          (IP_GET_TYPE(&ipaddr) == IPADDR_TYPE_V4)
/** @ingroup ip6addr */
#define IP_IS_V6_VAL(ipaddr)          (IP_GET_TYPE(&ipaddr) == IPADDR_TYPE_V6)
/** @ingroup ip4addr */
#define IP_IS_V4(ipaddr)              (((ipaddr) == NULL) || IP_IS_V4_VAL(*(ipaddr)))
/** @ingroup ip6addr */
#define IP_IS_V6(ipaddr)              (((ipaddr) != NULL) && IP_IS_V6_VAL(*(ipaddr)))

#define IP_SET_TYPE_VAL(ipaddr, iptype) do { (ipaddr).type = (iptype); }while(0)
#define IP_SET_TYPE(ipaddr, iptype)     do { if((ipaddr) != NULL) { IP_SET_TYPE_VAL(*(ipaddr), iptype); }}while(0)
#define IP_GET_TYPE(ipaddr)           ((ipaddr)->type)
#endif

/** @ingroup ip6addr
 * Convert generic ip address to specific protocol version
 */
#define ip_2_ip6(ipaddr)   (&((ipaddr)->u_addr.ip6))
/** @ingroup ip4addr
 * Convert generic ip address to specific protocol version
 */
#define ip_2_ip4(ipaddr)   (&((ipaddr)->u_addr.ip4))


/** IPv4 only: set the IP address given as an uint32 */
#define ip4_addr_set_u32(dest_ipaddr, src_u32) ((dest_ipaddr)->addr = (src_u32))
/** IPv4 only: get the IP address as an uint32 */
#define ip4_addr_get_u32(src_ipaddr) ((src_ipaddr)->addr)

#ifdef FD_SET
#undef FD_SET
#undef FD_CLR
#undef FD_ISSET
#undef FD_ZERO
#undef fd_set
#endif

#ifndef MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN                128
#endif
#ifndef LWIP_SOCKET_OFFSET
#define LWIP_SOCKET_OFFSET                0
#endif


/* FD_SET used for lwip_select */
#ifndef FD_SET
#undef  FD_SETSIZE
/* Make FD_SETSIZE match NUM_SOCKETS in socket.c */
#define FD_SETSIZE    MEMP_NUM_NETCONN
#define FDSETSAFESET(n, code)  do {\
    if ((((int)(n) - LWIP_SOCKET_OFFSET) < (MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0)) {code;}\
    } while(0)
#define FDSETSAFEGET(n, code) (((((int)(n) - LWIP_SOCKET_OFFSET) < (MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0)) ? (code) : 0)

#define FD_SET(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] |=  (1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_CLR(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] &= ~(1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_ISSET(n,p) FDSETSAFEGET(n, (p)->fd_bits[((n) - LWIP_SOCKET_OFFSET)/8] &   (1 << (((n) - LWIP_SOCKET_OFFSET) & 7)))
#define FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))

typedef struct fd_set {
  unsigned char fd_bits [(FD_SETSIZE * 2 + 7)/8];
} fd_set;

#endif /* FD_SET */


#define  ERROK         0  /* err ok set, no err happen */
#define  EPERM         1  /* Operation not permitted */
#define  ENOENT        2  /* No such file or directory */
#define  ESRCH         3  /* No such process */
#define  EINTR         4  /* Interrupted system call */
#define  EIO           5  /* I/O error */
#define  ENXIO         6  /* No such device or address */
#define  E2BIG         7  /* Arg list too long */
#define  ENOEXEC       8  /* Exec format error */
#define  EBADF         9  /* Bad file number */
#define  ECHILD       10  /* No child processes */
#define  EAGAIN       11  /* Try again */
#undef ENOMEM
#define  ENOMEM       12  /* Out of memory */
#define  EACCES       13  /* Permission denied */
#define  EFAULT       14  /* Bad address */
#define  ENOTBLK      15  /* Block device required */
#define  EBUSY        16  /* Device or resource busy */
#define  EEXIST       17  /* File exists */
#define  EXDEV        18  /* Cross-device link */
#define  ENODEV       19  /* No such device */
#define  ENOTDIR      20  /* Not a directory */
#define  EISDIR       21  /* Is a directory */
#undef EINVAL
#define  EINVAL       22  /* Invalid argument */
#define  ENFILE       23  /* File table overflow */
#define  EMFILE       24  /* Too many open files */
#define  ENOTTY       25  /* Not a typewriter */
#define  ETXTBSY      26  /* Text file busy */
#define  EFBIG        27  /* File too large */
#define  ENOSPC       28  /* No space left on device */
#define  ESPIPE       29  /* Illegal seek */
#define  EROFS        30  /* Read-only file system */
#define  EMLINK       31  /* Too many links */
#define  EPIPE        32  /* Broken pipe */
#define  LWIPEDOM     33  /* Math argument out of domain of func */
#define  LWIPERANGE   34  /* Math result not representable */
#define  EDEADLK      35  /* Resource deadlock would occur */
#define  ENAMETOOLONG 36  /* File name too long */
#define  ENOLCK       37  /* No record locks available */
#define  ENOSYS       38  /* Function not implemented */
#define  ENOTEMPTY    39  /* Directory not empty */
#define  ELOOP        40  /* Too many symbolic links encountered */
#define  EWOULDBLOCK  EAGAIN  /* Operation would block */
#define  ENOMSG       42  /* No message of desired type */
#define  EIDRM        43  /* Identifier removed */
#define  ECHRNG       44  /* Channel number out of range */
#define  EL2NSYNC     45  /* Level 2 not synchronized */
#define  EL3HLT       46  /* Level 3 halted */
#define  EL3RST       47  /* Level 3 reset */
#define  ELNRNG       48  /* Link number out of range */
#define  EUNATCH      49  /* Protocol driver not attached */
#define  ENOCSI       50  /* No CSI structure available */
#define  EL2HLT       51  /* Level 2 halted */
#define  EBADE        52  /* Invalid exchange */
#define  EBADR        53  /* Invalid request descriptor */
#define  EXFULL       54  /* Exchange full */
#define  ENOANO       55  /* No anode */
#define  EBADRQC      56  /* Invalid request code */
#define  EBADSLT      57  /* Invalid slot */

#define  EDEADLOCK    EDEADLK

#define  EBFONT       59  /* Bad font file format */
#define  ENOSTR       60  /* Device not a stream */
#define  ENODATA      61  /* No data available */
#define  ETIME        62  /* Timer expired */
#define  ENOSR        63  /* Out of streams resources */
#define  ENONET       64  /* Machine is not on the network */
#define  ENOPKG       65  /* Package not installed */
#define  EREMOTE      66  /* Object is remote */
#define  ENOLINK      67  /* Link has been severed */
#define  EADV         68  /* Advertise error */
#define  ESRMNT       69  /* Srmount error */
#define  ECOMM        70  /* Communication error on send */
#define  EPROTO       71  /* Protocol error */
#define  EMULTIHOP    72  /* Multihop attempted */
#define  EDOTDOT      73  /* RFS specific error */
#define  EBADMSG      74  /* Not a data message */
#define  EOVERFLOW    75  /* Value too large for defined data type */
#define  ENOTUNIQ     76  /* Name not unique on network */
#define  EBADFD       77  /* File descriptor in bad state */
#define  EREMCHG      78  /* Remote address changed */
#define  ELIBACC      79  /* Can not access a needed shared library */
#define  ELIBBAD      80  /* Accessing a corrupted shared library */
#define  ELIBSCN      81  /* .lib section in a.out corrupted */
#define  ELIBMAX      82  /* Attempting to link in too many shared libraries */
#define  ELIBEXEC     83  /* Cannot exec a shared library directly */
#define  LWIPEILSEQ   84  /* Illegal byte sequence */
#define  ERESTART     85  /* Interrupted system call should be restarted */
#define  ESTRPIPE     86  /* Streams pipe error */
#define  EUSERS       87  /* Too many users */
#define  ENOTSOCK     88  /* Socket operation on non-socket */
#define  EDESTADDRREQ 89  /* Destination address required */
#define  EMSGSIZE     90  /* Message too long */
#define  EPROTOTYPE   91  /* Protocol wrong type for socket */
#define  ENOPROTOOPT  92  /* Protocol not available */
#define  EPROTONOSUPPORT 93  /* Protocol not supported */
#define  ESOCKTNOSUPPORT 94  /* Socket type not supported */
#define  EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#define  EPFNOSUPPORT    96  /* Protocol family not supported */
#define  EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  EADDRINUSE      98  /* Address already in use */
#define  EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  ENETDOWN       100  /* Network is down */
#define  ENETUNREACH    101  /* Network is unreachable */
#define  ENETRESET      102  /* Network dropped connection because of reset */
#define  ECONNABORTED   103  /* Software caused connection abort */
#define  ECONNRESET     104  /* Connection reset by peer */
#define  ENOBUFS        105  /* No buffer space available */
#define  EISCONN        106  /* Transport endpoint is already connected */
#define  ENOTCONN       107  /* Transport endpoint is not connected */
#define  ESHUTDOWN      108  /* Cannot send after transport endpoint shutdown */
#define  ETOOMANYREFS   109  /* Too many references: cannot splice */
#define  ETIMEDOUT      110  /* Connection timed out */
#define  ECONNREFUSED   111  /* Connection refused */
#define  EHOSTDOWN      112  /* Host is down */
#define  EHOSTUNREACH   113  /* No route to host */
#define  EALREADY       114  /* Operation already in progress */
#define  EINPROGRESS    115  /* Operation now in progress */
#define  ESTALE         116  /* Stale NFS file handle */
#define  EUCLEAN        117  /* Structure needs cleaning */
#define  ENOTNAM        118  /* Not a XENIX named type file */
#define  ENAVAIL        119  /* No XENIX semaphores available */
#define  EISNAM         120  /* Is a named type file */
#define  EREMOTEIO      121  /* Remote I/O error */
#define  EDQUOT         122  /* Quota exceeded */

#define  ENOMEDIUM      123  /* No medium found */
#define  EMEDIUMTYPE    124  /* Wrong medium type */

#endif


/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
 
/******************************************************************************
 * 2 STRUCT Type Definition about HAISI AT
******************************************************************************/
typedef enum{
	NWY_RDONLY	 = 0x0,
	NWY_WRONLY	 = 0x1,
	NWY_RDWR	 = 0x2,
	NWY_CREAT	 = 0x40,
	NWY_TRUNC	 = 0x04,
	NWY_APPEND	 = 0x400,

	NWY_RB_MODE,
	NWY_WB_MODE,
	NWY_RB_PLUS_MODE,
	NWY_AB_MODE = NWY_WRONLY | NWY_CREAT | NWY_APPEND,
	NWY_WB_PLUS_MODE = NWY_CREAT | NWY_RDWR | NWY_TRUNC,
	NWY_AB_PLUS_MODE = NWY_CREAT | NWY_RDWR | NWY_APPEND,
}nwy_file_open_mode_e;

typedef struct nwy_ip_addr_t {
    union {
        ip6_addr_t ip6;
        ip4_addr_t ip4;
    } u_addr;
    /** @ref sal_ip_addr_type */
    unsigned char type;
} nwy_ip_addr_t;





#ifdef __cplusplus
}
#endif


#endif

