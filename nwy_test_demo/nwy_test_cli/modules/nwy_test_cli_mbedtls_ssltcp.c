#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
/*
* File name:
* Author:
* Date:
* Description:  This is a demonstration program for ssltcp client.
                1. Supports three types of authentication: one-way authentication, two-way authentication, and non authentication connection methods.
                2. ssltcp status
                NWY_SSLTCP_STATUS_IDEL = 0,
                NWY_SSLTCP_STATUS_CONNECTING,
                NWY_SSLTCP_STATUS_CONNECTED,
                NWY_SSLTCP_STATUS_CONNECT_FAILED,
                NWY_SSLTCP_STATUS_DISCONNECTED,
                NWY_SSLTCP_STATUS_RECV_OK,
                NWY_SSLTCP_STATUS_RECV_ERROR,
                NWY_SSLTCP_STATUS_SEND_OK,
                NWY_SSLTCP_STATUS_SEND_ERROR,
                NWY_SSLTCP_STATUS_CLOSE_OK,
                NWY_SSLTCP_STATUS_CLOSE_ERROR,
*/

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/build_info.h"
#include "nwy_socket_api.h"

//#include "mbedtls/certs.h"



#define NWY_SSLTCP_PACKET_SIZE (1024*2)

typedef enum 
{
    NWY_SSLTCP_STATUS_IDEL = 0,
    NWY_SSLTCP_STATUS_CONNECTING,
    NWY_SSLTCP_STATUS_CONNECTED,
    NWY_SSLTCP_STATUS_CONNECT_FAILED,
    NWY_SSLTCP_STATUS_DISCONNECTED,
    NWY_SSLTCP_STATUS_RECV_OK,
    NWY_SSLTCP_STATUS_RECV_ERROR,
    NWY_SSLTCP_STATUS_SEND_OK,
    NWY_SSLTCP_STATUS_SEND_ERROR,
    NWY_SSLTCP_STATUS_CLOSE_OK,
    NWY_SSLTCP_STATUS_CLOSE_ERROR,
}NWY_SSLTCP_STATUS_E;

typedef struct{
    NWY_SSLTCP_STATUS_E event;
    void *data;
    int data_len;
}nwy_ssltcp_msg_t;

typedef void (*nwy_ssltcp_msg_cb_t)(nwy_ssltcp_msg_t *msg);


typedef struct{
    int cid;
    int socket_fd;
    char *host;
    int port;
    struct sockaddr_in s_nwy_server_v4;
    struct sockaddr_in6 s_nwy_server_v6;
    NWY_SSLTCP_STATUS_E status;
    nwy_ssltcp_msg_cb_t msg_cb;
    //tls cfg
    mbedtls_ssl_config conf;
    mbedtls_ssl_context ssl;
    mbedtls_net_context netctx;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clientcert;
    mbedtls_pk_context clientkey;
}nwy_ssl_tcp_handl_t;

static int nwy_ssltcp_init(nwy_ssl_tcp_handl_t *handl,nwy_ssl_conf_t *ssl_config);
static void nwy_ssltcp_deinit(nwy_ssl_tcp_handl_t *handl);
static int nwy_ssltcp_close(nwy_ssl_tcp_handl_t *handl);
static int nwy_ssltcp_read(nwy_ssl_tcp_handl_t *handl);
static int nwy_ssltcp_connect(nwy_ssl_tcp_handl_t *handl, nwy_ssl_conf_t *ssl_config);
static int nwy_ssltcp_send(nwy_ssl_tcp_handl_t *handl,char *buf,int len);
static void nwy_ssltcp_msg_notify(nwy_ssl_tcp_handl_t *handl,
                                        NWY_SSLTCP_STATUS_E event,
                                        void *data,
                                        int data_len);

static int nwy_ssltcp_crt_set(nwy_ssl_tcp_handl_t *sockset, nwy_ssl_conf_t *ssl_config)
{
    int ret = 0;

    if(ssl_config->sni_name != NULL && ssl_config->sni_name_size>0)
    {
        NWY_SDK_LOG_DEBUG("sockset->sni:%s", ssl_config->sni_name);
        if(mbedtls_ssl_set_hostname( &(sockset->ssl), ssl_config->sni_name)!=0)
        {
            NWY_SDK_LOG_ERROR("sni set error");
        }
    }

    if (ssl_config->authmode != NWY_SSL_AUTH_NONE_E)
    {
        if (mbedtls_x509_crt_parse(&sockset->cacert, (const unsigned char *)ssl_config->cacert.cert_data,
                                  ssl_config->cacert.cert_len + 1) < 0)
        {
            NWY_SDK_LOG_ERROR("mbedtls_x509_crt_parse ca_pem failed");
            goto error_exit;
        }

        /* RFC 8446 section 4.4.3
         *
         * If the verification fails, the receiver MUST terminate the handshake with
         * a "decrypt_error" alert.
         *
         * If the client is configured as TLS 1.3 only with optional verify, return
         * bad config.
         *
         */
        mbedtls_ssl_conf_authmode(&sockset->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        mbedtls_ssl_conf_ca_chain(&sockset->conf, &sockset->cacert, NULL);

        if (ssl_config->authmode == NWY_SSL_AUTH_MUTUAL_E)
        {
            if (mbedtls_x509_crt_parse(&(sockset->clientcert), (const unsigned char *)ssl_config->clientcert.cert_data,
                    ssl_config->clientcert.cert_len + 1) != 0)
            {
                NWY_SDK_LOG_ERROR("mbedtls_x509_crt_parse client_pem failed");
                goto error_exit;
            }

            ret = mbedtls_pk_parse_key(&(sockset->clientkey), (const unsigned char *)ssl_config->clientkey.cert_data,
                                      ssl_config->clientkey.cert_len + 1, NULL, 0, mbedtls_ctr_drbg_random, &(sockset->ctr_drbg));
            if (ret != 0)
            {
                NWY_SDK_LOG_ERROR("mbedtls_pk_parse_key failed");
                goto error_exit;
            }

            if (mbedtls_ssl_conf_own_cert(&(sockset->conf), &(sockset->clientcert), &(sockset->clientkey)) != 0)
            {
                NWY_SDK_LOG_ERROR("mbedtls_ssl_conf_own_cert failed");
                goto error_exit;
            }
        }
    }
    else
    {
        mbedtls_ssl_conf_authmode(&sockset->conf, MBEDTLS_SSL_VERIFY_NONE);
    }

    return 0;

error_exit:
    return -1;
}


static int nwy_ssltcp_init(nwy_ssl_tcp_handl_t *handl,nwy_ssl_conf_t *ssl_config)
{
    int ret=0;
    NWY_SDK_LOG_ERROR("nwy_ssltcp_init in");
    mbedtls_ssl_init(&(handl->ssl));
    mbedtls_net_init(&(handl->netctx));
    mbedtls_ssl_config_init(&handl->conf);
    mbedtls_x509_crt_init(&handl->cacert);
    mbedtls_x509_crt_init(&handl->clientcert);
    mbedtls_pk_init(&(handl->clientkey));
    mbedtls_ctr_drbg_init(&handl->ctr_drbg);
    mbedtls_entropy_init(&handl->entropy);

    char *pers = "nwy_test_tls";
    /* Initialize the PSA crypto library. */
    psa_status_t status = psa_crypto_init();
    if (status != PSA_SUCCESS)
    {
        NWY_SDK_LOG_ERROR("Failed to initialize PSA Crypto implementation: %d\n",(int)status);
        goto FAILED;
    }

    if ((ret = mbedtls_ctr_drbg_seed(&(handl->ctr_drbg), mbedtls_entropy_func, &(handl->entropy),
                                         (const unsigned char *)pers, strlen(pers))) != 0)
    {
        NWY_SDK_LOG_ERROR("mbedtls_ctr_drbg_seed failed, ret=%d", ret);
        goto FAILED;
    }


    if ((ret = mbedtls_ssl_config_defaults(&(handl->conf),
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        NWY_SDK_LOG_ERROR("mbedtls_ssl_config_defaults failed, returned %d", ret);
        goto FAILED;
    }

    if (nwy_ssltcp_crt_set(handl,ssl_config)!=0)
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_crt_set error");
        goto FAILED;
    }

    mbedtls_ssl_conf_max_version(&(handl->conf), MBEDTLS_SSL_MAJOR_VERSION_3, ssl_config->ssl_version);
    mbedtls_ssl_conf_min_version(&(handl->conf), MBEDTLS_SSL_MAJOR_VERSION_3, ssl_config->ssl_version);

    mbedtls_ssl_conf_rng( &(handl->conf), mbedtls_ctr_drbg_random, &(handl->ctr_drbg));

    return 0;

FAILED:
    nwy_ssltcp_deinit(handl);

    return -1;
}


static void nwy_ssltcp_deinit(nwy_ssl_tcp_handl_t *handl)
{
    if (handl == NULL) return;

    NWY_SDK_LOG_ERROR("nwy_ssltcp_deinit in");
    mbedtls_net_free( &handl->netctx );
    mbedtls_x509_crt_free(&handl->cacert);
    mbedtls_x509_crt_free(&handl->clientcert);
    mbedtls_pk_free(&(handl->clientkey));
    mbedtls_ssl_free(&(handl->ssl));
    mbedtls_ssl_config_free(&handl->conf);
    mbedtls_ctr_drbg_free(&handl->ctr_drbg);
    mbedtls_entropy_free(&handl->entropy);
    handl->status = NWY_SSLTCP_STATUS_IDEL;
}


static int nwy_ssltcp_close(nwy_ssl_tcp_handl_t *handl)
{

    int ret = 0;
    while( ( ret = mbedtls_ssl_close_notify(&(handl->ssl) ) ) < 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ &&
            ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            NWY_SDK_LOG_DEBUG( "failed: mbedtls_ssl_close_notify returned -0x%04x\n",ret );
            nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_CLOSE_ERROR,NULL,0);
            return -1;
        }
    }
    mbedtls_net_free(&handl->netctx);
    return 0;
}

static void nwy_ssltcp_msg_notify(nwy_ssl_tcp_handl_t *handl,
                                        NWY_SSLTCP_STATUS_E event,
                                        void *data,
                                        int data_len)
{
    nwy_ssltcp_msg_t msg;
    memset(&msg,0x00,sizeof(msg));
    msg.event = event;
    msg.data=data;
    msg.data_len = data_len;
    if(handl->msg_cb != NULL)
    {
        handl->msg_cb(&msg);
    }
}


static int nwy_ssltcp_read(nwy_ssl_tcp_handl_t *handl) {

    unsigned char buffer[NWY_UART_RECV_SINGLE_MAX+1] = {0};
    int numBytes = 0;
    if (handl == NULL) {
        NWY_SDK_LOG_DEBUG("Invalid handle");
        return NWY_GEN_E_UNKNOWN;
    }

    retry_read:
    if (handl->status == NWY_SSLTCP_STATUS_CONNECTED)
    {
        memset(buffer,0x00,NWY_UART_RECV_SINGLE_MAX+1);

        do {
            numBytes = mbedtls_ssl_read(&(handl->ssl), buffer, NWY_UART_RECV_SINGLE_MAX);
        } while (numBytes == MBEDTLS_ERR_SSL_WANT_READ || numBytes == MBEDTLS_ERR_SSL_WANT_WRITE);

        if (numBytes > 0)
        {
            nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_RECV_OK,(void *)buffer,numBytes);
            return NWY_SUCCESS;
        }
        else if (numBytes == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY ||numBytes ==MBEDTLS_ERR_NET_CONN_RESET)
        {
            NWY_SDK_LOG_DEBUG("Connection closed by peer:%x",numBytes * (-1));
            goto error_exit;
        }
        else if(numBytes ==MBEDTLS_ERR_SSL_TIMEOUT)
        {
            NWY_SDK_LOG_DEBUG(" wait data ing\n\n" );
            goto retry_read;
        }
        else
        {
            NWY_SDK_LOG_DEBUG("SSL read error: 0x%x", numBytes * (-1));
            goto error_exit;
        }
    }

    return 0;

error_exit:
    handl->status = NWY_SSLTCP_STATUS_DISCONNECTED;
    nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_DISCONNECTED,NULL,0);
    return -1;
}

static int nwy_ssltcp_dns(nwy_ssl_tcp_handl_t *handl)
{
    int isipv6 = 0;
    char *ip_buf = NULL;

    if (!handl || !handl->host)
        return AF_UNSPEC;

    ip_buf = nwy_socket_gethost_by_name(handl->host, &isipv6,handl->cid);
    if(ip_buf == NULL)
    {
        NWY_SDK_LOG_ERROR("getaddrinfo failed with param %s", handl->host);
        return -1;
    }

    if(isipv6 == 1)
    {
        NWY_SDK_LOG_DEBUG("V6: [%s],host:[%s]", ip_buf,handl->host);
        memset(&handl->s_nwy_server_v6, 0, sizeof(handl->s_nwy_server_v6));
        if (nwy_socket_inet_pton(AF_INET6, ip_buf, (void *)&handl->s_nwy_server_v6.sin6_addr) < 0)
        {
            NWY_SDK_LOG_ERROR("getaddrinfo failed with param %s", handl->host);
            return AF_UNSPEC;
        }
        handl->s_nwy_server_v6.sin6_len = sizeof(struct sockaddr_in6);
        handl->s_nwy_server_v6.sin6_family = AF_INET6;
        handl->s_nwy_server_v6.sin6_port = nwy_socket_htons(handl->port);
        return AF_INET6;

    }
    else
    {
         NWY_SDK_LOG_DEBUG("V4: [%s],host:[%s]", ip_buf,handl->host);
        memset(&handl->s_nwy_server_v4, 0, sizeof(handl->s_nwy_server_v4));
        if (nwy_socket_inet_pton(AF_INET, ip_buf, (void *)&handl->s_nwy_server_v4.sin_addr) < 0)
        {
            NWY_SDK_LOG_ERROR("getaddrinfo failed with param %s", handl->host);
            return AF_UNSPEC;
        }
        handl->s_nwy_server_v4.sin_len = sizeof(struct sockaddr_in);
        handl->s_nwy_server_v4.sin_family = AF_INET;
        handl->s_nwy_server_v4.sin_port = nwy_socket_htons(handl->port);
        return  AF_INET;
    }
}
static int nwy_ssltcp_data_call_check(int cid)
{

    int ret = NWY_GEN_E_UNKNOWN;
    nwy_data_callinfo_t addr_info = {0};
    ret = nwy_data_call_info_get(cid, &addr_info);
    if (ret == NWY_SUCCESS && addr_info.state == NWY_DATA_CALL_CONNECTED_STATE)
    {
        return 0;
    }

    return -1;
}

static int nwy_ssltcp_connect_server(nwy_ssl_tcp_handl_t *handl)
{
    int ret =0;
    unsigned char family;

    NWY_SDK_LOG_INFO("ssltcp_connetc server in cid:%d",handl->cid);

    if (handl == NULL)
    {
        NWY_SDK_LOG_ERROR("sockset is NULL");
        return -1;
    }

    if(nwy_ssltcp_data_call_check(handl->cid)!=0)
    {
        NWY_SDK_LOG_ERROR("No dialing");
        return -1;
    }

    family = nwy_ssltcp_dns(handl);
    if(family !=AF_INET6 && family !=AF_INET)
    {
        NWY_SDK_LOG_ERROR("family error:%d",family);
        return -1;
    }

    handl->socket_fd = nwy_socket_open(family, SOCK_STREAM, IPPROTO_TCP,handl->cid);
    if (handl->socket_fd < 0)
    {
        NWY_SDK_LOG_ERROR("socket failederror:errno: %d", nwy_socket_errno());
        return -1;
    }

    if ((0 != nwy_socket_set_reuseaddr(handl->socket_fd))
          || (0 != nwy_socket_set_nodelay(handl->socket_fd)))
    {
          NWY_SDK_LOG_ERROR("nwy_socket_set_nonblock() failed errno:%d", nwy_socket_errno());
          nwy_socket_close(handl->socket_fd);
          handl->socket_fd = -1;
          return -1;
    }

    if(family == AF_INET)
    {
        ret = nwy_socket_connect(handl->socket_fd, (const struct sockaddr *)&handl->s_nwy_server_v4,sizeof(handl->s_nwy_server_v4));
    }
    else
    {
        ret = nwy_socket_connect(handl->socket_fd, (const struct sockaddr *)&handl->s_nwy_server_v6,sizeof(handl->s_nwy_server_v6));
    }

    if (ret == 0)
    {
        return 0;
    }
    else
    {
        NWY_SDK_LOG_ERROR("connect() failed, error: %d", nwy_socket_errno());
        nwy_socket_close(handl->socket_fd);
        handl->socket_fd = -1;
        return -1;
    }

}


static int nwy_ssltcp_connect(nwy_ssl_tcp_handl_t *handl, nwy_ssl_conf_t *ssl_config)
{
    int ret = 0;
    int flags = 0;

    char port[8] = {0};
    if (handl == NULL ||ssl_config == NULL)
    {
        NWY_SDK_LOG_ERROR("not initialized");
        return NWY_GEN_E_UNKNOWN;
    }

    if(handl->host==NULL)
    {
         NWY_SDK_LOG_ERROR("host error");
         return NWY_GEN_E_UNKNOWN;
    }


   // snprintf(port,sizeof(port),"%d",handl->port);
   // if (ret =mbedtls_net_connect(&(handl->netctx), handl->host, port, MBEDTLS_NET_PROTO_TCP) != 0) {
   //     NWY_SDK_LOG_DEBUG("mbedtls_net_connect failed, returned %d", ret);
   //     goto error;
  //  }

    ret = nwy_ssltcp_connect_server(handl);
    if(ret<0)
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_connect_server error: %d", ret);
        goto error;
    }

    handl->netctx.fd = handl->socket_fd;

    if ((ret = mbedtls_ssl_setup(&(handl->ssl), &handl->conf)) != 0)
    {
        NWY_SDK_LOG_DEBUG("mbedtls_ssl_setup failed, returned %d", ret);
        goto error;
    }

    mbedtls_ssl_conf_read_timeout(&(handl->conf), 5*1000);
    mbedtls_ssl_set_bio(&(handl->ssl), &handl->netctx, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

    while ((ret = mbedtls_ssl_handshake(&(handl->ssl))) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            NWY_SDK_LOG_ERROR("SSL handshake failed: -0x%x", (unsigned int)-ret);
            goto error;
        }
    }

    NWY_SDK_LOG_DEBUG("SSL handshake success on socket");

    if (ssl_config->authmode == NWY_SSL_AUTH_ONE_WAY_E || ssl_config->authmode == NWY_SSL_AUTH_MUTUAL_E) {
        flags = mbedtls_ssl_get_verify_result(&(handl->ssl));
        if (flags != 0) {
            char vrfy_buf[125];
            mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), " ! ", flags);
            NWY_SDK_LOG_ERROR("Certificate verification failed: %s", vrfy_buf);
            goto error;
        }
    }

    handl->status = NWY_SSLTCP_STATUS_CONNECTED;
    nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_CONNECTED,NULL,0);

    return NWY_SUCCESS;

error:
    handl->status = NWY_SSLTCP_STATUS_CONNECT_FAILED;
    nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_CONNECT_FAILED,NULL,0);
    return NWY_GEN_E_UNKNOWN;
}

static int nwy_ssltcp_send(nwy_ssl_tcp_handl_t *handl,char *buf,int len)
{
    int ret  = 0;
    while( ( ret = mbedtls_ssl_write( &(handl->ssl), (const unsigned char *)buf, len ) ) <= 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            NWY_SDK_LOG_DEBUG("SSL failed ApiCommSSLSend ret=%d", ret );
            nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_SEND_ERROR,NULL,0);
            return -1;
        }
    }
    nwy_ssltcp_msg_notify(handl,NWY_SSLTCP_STATUS_SEND_OK,NULL,0);

    return ret;
}




///////////////////////////////////////cli test///////////////////////////////////////////////////

typedef struct{
     int init_flag;
     char host[256+1];
     int port;
     nwy_ssl_tcp_handl_t test_ssltcp_handl;
     nwy_ssl_conf_t sslcfg;
}nwy_cli_ssltcp_param_t;

static nwy_cli_ssltcp_param_t nwy_cli_ssltcp_param;


#define NWY_CLI_SSLTCP_SEND_DATA_MAX_LEN 2048

static void nwy_ssltcp_msg_cb(nwy_ssltcp_msg_t *msg)
{
    switch(msg->event)
    {
        case NWY_SSLTCP_STATUS_IDEL:
            nwy_test_cli_echo("\r\nstatus idel");
            break;
        case NWY_SSLTCP_STATUS_CONNECTING:
            nwy_test_cli_echo("\r\nstatus connecting");
            break;
        case NWY_SSLTCP_STATUS_CONNECTED:
            nwy_test_cli_echo("\r\nstatus connected");
            break;
        case NWY_SSLTCP_STATUS_CONNECT_FAILED:
            nwy_test_cli_echo("\r\nstatus connect failed");
            break;
        case NWY_SSLTCP_STATUS_DISCONNECTED:
             nwy_test_cli_echo("\r\nstatus disconnected\r\n");
            break;
        case NWY_SSLTCP_STATUS_RECV_OK:
            if (msg->data_len != 0 && msg->data != NULL)
            {
                nwy_test_cli_echo("\r\nrecv len:%d,data:%s",msg->data_len,(char *)msg->data);
            }
            else
            {
                nwy_test_cli_echo("\r\nrecv NULL data");
            }
            break;
        case NWY_SSLTCP_STATUS_RECV_ERROR:
            nwy_test_cli_echo("\r\nstatus recv error");
            break;
        case NWY_SSLTCP_STATUS_SEND_OK:
            nwy_test_cli_echo("\r\nstatus send ok");
            break;
        case NWY_SSLTCP_STATUS_SEND_ERROR:
            nwy_test_cli_echo("\r\nstatus send error");
            break;
        case NWY_SSLTCP_STATUS_CLOSE_OK:
            nwy_test_cli_echo("\r\nstatus close ok");
            break;
        case NWY_SSLTCP_STATUS_CLOSE_ERROR:
            nwy_test_cli_echo("\r\nstatus close error");
            break;
         default:
            nwy_test_cli_echo("\r\nstatus unknown");
            break;
    }
}


void nwy_test_cli_ssltcp_init(void)
{
    int ret = 0;
    int cid = 0;
    char *sptr = NULL;
    memset(&nwy_cli_ssltcp_param,0x00,sizeof(nwy_cli_ssltcp_param));
    memset(&nwy_cli_ssltcp_param.test_ssltcp_handl,0x00,sizeof(nwy_cli_ssltcp_param.test_ssltcp_handl));
    memset(&nwy_cli_ssltcp_param.sslcfg,0x00,sizeof(nwy_cli_ssltcp_param.sslcfg));
    memset(&nwy_cli_ssltcp_param.sslcfg.cacert,0x00,sizeof(nwy_cli_ssltcp_param.sslcfg.cacert));
    memset(&nwy_cli_ssltcp_param.sslcfg.clientcert,0x00,sizeof(nwy_cli_ssltcp_param.sslcfg.clientcert));
    memset(&nwy_cli_ssltcp_param.sslcfg.clientkey,0x00,sizeof(nwy_cli_ssltcp_param.sslcfg.clientkey));

    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid> 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    nwy_cli_ssltcp_param.test_ssltcp_handl.cid = cid;

    sptr = nwy_test_cli_input_gets("\r\n input ip(<=256):");
    if(strlen(sptr)>256)
    {
        nwy_test_cli_echo("\r\n input ip error");
        return;
    }
    snprintf(nwy_cli_ssltcp_param.host,sizeof(nwy_cli_ssltcp_param.host), "%s",sptr);

    sptr = nwy_test_cli_input_gets("\r\n input port:");
    nwy_cli_ssltcp_param.port = nwy_cli_str_to_int(sptr);
    if(nwy_cli_ssltcp_param.port<=0)
    {
        nwy_test_cli_echo("\r\n input port error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input ssl version: 0:SSL3.0 1:TLS1.0 2:TLS1.1 3:TLS1.2: 4:TLS1.3");
    nwy_cli_ssltcp_param.sslcfg.ssl_version = (nwy_ssl_version_e)nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input authmode: 0:No authentication 1:Manage server authentication 2:Manage server and client authentication");
    nwy_cli_ssltcp_param.sslcfg.authmode = (nwy_ssl_auth_mode_e)nwy_cli_str_to_int(sptr);
    if (NWY_SSL_AUTH_NONE_E != nwy_cli_ssltcp_param.sslcfg.authmode)
    {
       //ca 
       sptr = nwy_test_cli_input_gets("\r\n input ca cert length(1-4096):");
       nwy_cli_ssltcp_param.sslcfg.cacert.cert_len =  nwy_cli_str_to_int(sptr);
       if (nwy_cli_ssltcp_param.sslcfg.cacert.cert_len < 1 || nwy_cli_ssltcp_param.sslcfg.cacert.cert_len>4096)
       {
           nwy_test_cli_echo("\r\n invalid ca cert size:%d", nwy_cli_ssltcp_param.sslcfg.cacert.cert_len);
           return;
       }
       nwy_cli_ssltcp_param.sslcfg.cacert.cert_data = malloc(nwy_cli_ssltcp_param.sslcfg.cacert.cert_len+1);
       memset(nwy_cli_ssltcp_param.sslcfg.cacert.cert_data,0x00,nwy_cli_ssltcp_param.sslcfg.cacert.cert_len+1);
       nwy_cli_get_trans_data(nwy_cli_ssltcp_param.sslcfg.cacert.cert_data,nwy_cli_ssltcp_param.sslcfg.cacert.cert_len);

       if(nwy_cli_ssltcp_param.sslcfg.authmode == NWY_SSL_AUTH_MUTUAL_E)
       {
           //client cert
           sptr = nwy_test_cli_input_gets("\r\n input client cert length(1-4096):");
           nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len =  nwy_cli_str_to_int(sptr);
           if (nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len < 1 || nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len>4096)
           {
               nwy_test_cli_echo("\r\n invalid client cert size:%d", nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len);
                goto error_exit;
           }
           nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data = malloc(nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len+1);
           memset(nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data,0x00,nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len+1);
           nwy_cli_get_trans_data(nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data,nwy_cli_ssltcp_param.sslcfg.clientcert.cert_len);

           //client key
           sptr = nwy_test_cli_input_gets("\r\n input client key length(1-4096):");
           nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len =  nwy_cli_str_to_int(sptr);
           if (nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len < 1 || nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len>4096)
           {
               nwy_test_cli_echo("\r\n invalid client key size:%d", nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len);
                goto error_exit;
           }
           nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data = malloc(nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len+1);
           memset(nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data,0x00,nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len+1);
           nwy_cli_get_trans_data(nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data,nwy_cli_ssltcp_param.sslcfg.clientkey.cert_len);
       }
    }

    ret = nwy_ssltcp_init(&(nwy_cli_ssltcp_param.test_ssltcp_handl),&(nwy_cli_ssltcp_param.sslcfg));
    if(ret != 0)
    {
        nwy_test_cli_echo("\r\n nwy_ssltcp_init error");
    }
    else
    {
        nwy_cli_ssltcp_param.init_flag =1;
        nwy_test_cli_echo("\r\n nwy_ssltcp_init ok");
    }

    error_exit:
    if(nwy_cli_ssltcp_param.sslcfg.cacert.cert_data)
    {
        free(nwy_cli_ssltcp_param.sslcfg.cacert.cert_data);
        nwy_cli_ssltcp_param.sslcfg.cacert.cert_data = NULL;
    }

    if(nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data)
    {
        free(nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data);
        nwy_cli_ssltcp_param.sslcfg.clientcert.cert_data = NULL;
    }

    if(nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data)
    {
         free(nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data);
         nwy_cli_ssltcp_param.sslcfg.clientkey.cert_data = NULL;
    }

}
static void nwy_test_ssltcp_recv_task(void *param)
{
    int ret = 0;
    nwy_cli_ssltcp_param_t *nwy_cli_ssltcp_param =(nwy_cli_ssltcp_param_t *)param;
    nwy_cli_ssltcp_param->test_ssltcp_handl.host = nwy_cli_ssltcp_param->host;
    nwy_cli_ssltcp_param->test_ssltcp_handl.port = nwy_cli_ssltcp_param->port;
    nwy_cli_ssltcp_param->test_ssltcp_handl.msg_cb = nwy_ssltcp_msg_cb;

    ret = nwy_ssltcp_connect(&nwy_cli_ssltcp_param->test_ssltcp_handl,&nwy_cli_ssltcp_param->sslcfg);
    if(ret != 0)
    {
        NWY_SDK_LOG_DEBUG("nwy_ssltcp_connect error");
        goto exit;
    }
    NWY_SDK_LOG_DEBUG(" nwy_ssltcp_connect ok");

    while(1)
    {
        ret = nwy_ssltcp_read(&(nwy_cli_ssltcp_param->test_ssltcp_handl));
        if(ret != 0)
        {
            break;
        }
    }
    exit:
    nwy_ssltcp_deinit(&nwy_cli_ssltcp_param->test_ssltcp_handl);
    nwy_cli_ssltcp_param->init_flag = 0;
    NWY_SDK_LOG_DEBUG("Exit the receiving task");
    nwy_test_cli_echo("\r\n Exit the receiving task");
    nwy_thread_exit(NULL);//End task
}


void nwy_test_cli_ssltcp_setup(void)
{
    int ret = 0;
    nwy_osi_thread_t nwy_ssltcp_recv_hd = NULL;
    NWY_SDK_LOG_DEBUG("nwy_test_cli_ssltcp_setup in");

    if(nwy_cli_ssltcp_param.init_flag != 1)
    {
        nwy_test_cli_echo("\r\n not initialized");
        return;
    }
    if(nwy_cli_ssltcp_param.test_ssltcp_handl.status != NWY_SSLTCP_STATUS_IDEL)
    {
        nwy_test_cli_echo("\r\n Connected already");
        return;
    }

    if(nwy_thread_create(&nwy_ssltcp_recv_hd, "nwy_ssltcp_demo_recv_task", NWY_OSI_PRIORITY_NORMAL,
                        nwy_test_ssltcp_recv_task, 
                        (void *)&nwy_cli_ssltcp_param,10,1024*10, NULL)!=NWY_SUCCESS)
    {
        NWY_SDK_LOG_DEBUG("nwy_ssltcp_recv_hd create error");
        nwy_ssltcp_deinit(&nwy_cli_ssltcp_param.test_ssltcp_handl);
        nwy_cli_ssltcp_param.init_flag =0;
        nwy_test_cli_echo(" nwy_test_cli_ssltcp_setup error");
        return;
    }
    nwy_test_cli_echo(" nwy_test_cli_ssltcp_setup ok");

}

void nwy_test_cli_ssltcp_send(void)
{
    int ret = 0;
    char *sptr = NULL;
    int len = 0;
    char * data = NULL;

    if(nwy_cli_ssltcp_param.init_flag != 1)
    {
        nwy_test_cli_echo("\r\n not initialized");
        return;
    }

    if(nwy_cli_ssltcp_param.test_ssltcp_handl.status != NWY_SSLTCP_STATUS_CONNECTED)
    {
        nwy_test_cli_echo("\r\n The client is not connected");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input data len[1-2048]:");
    len = nwy_cli_str_to_int(sptr);
    if(len <= 0 || len > NWY_CLI_SSLTCP_SEND_DATA_MAX_LEN)
    {
        nwy_test_cli_echo("\r\n input data len error\r\n");
        return;
    }

    data = malloc(len+1);
    if(data == NULL)
    {
        nwy_test_cli_echo("\r\n malloc error\r\n");
        return;
    }

    memset(data,0x00,len+1);
    nwy_cli_get_trans_data(data,len);

    ret = nwy_ssltcp_send(&nwy_cli_ssltcp_param.test_ssltcp_handl,data,len);
    if(ret != 0)
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_send error");
    }
    else
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_send ok");
    }
    free(data);

}
void nwy_test_cli_ssltcp_close(void)
{
    int ret = 0;
    if(nwy_cli_ssltcp_param.init_flag != 1)
    {
        nwy_test_cli_echo("\r\n not initialized");
        return;
    }

    if(nwy_cli_ssltcp_param.test_ssltcp_handl.status != NWY_SSLTCP_STATUS_CONNECTED)
    {
        nwy_test_cli_echo("\r\n The client is not connected");
        return;
    }
    ret = nwy_ssltcp_close(&nwy_cli_ssltcp_param.test_ssltcp_handl);
    if(ret != 0)
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_close error");
    }
    else
    {
        NWY_SDK_LOG_ERROR("nwy_ssltcp_close ok");
    }

}


