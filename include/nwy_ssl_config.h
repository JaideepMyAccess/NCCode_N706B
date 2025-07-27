#ifndef __NWY_SSL_CONFIG_H__
#define __NWY_SSL_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    NWY_VERSION_SSL_V3_E,         //SSL3.0
    NWY_VERSION_TLS_V1_0_E,       //SSL1.0
    NWY_VERSION_TLS_V1_1_E,       //SSL1.1
    NWY_VERSION_TLS_V1_2_E,       //SSL1.2
    NWY_VERSION_TLS_V1_3_E,       //SSL1.3
    NWY_VERSION_MAX_E
}nwy_ssl_version_e;

typedef enum{
    NWY_SSL_AUTH_NONE_E = 0,   //No authentication
    NWY_SSL_AUTH_ONE_WAY_E,  //Manage server authentication
    NWY_SSL_AUTH_MUTUAL_E,   //Manage server and client authentication if requested by the remote server*/
    NWY_SSL_AUTH_MAX_E
}nwy_ssl_auth_mode_e;

typedef struct 
{
    char *cert_data;
    int cert_len;
}nwy_ssl_cert_t;

typedef struct 
{
    nwy_ssl_version_e ssl_version;
    nwy_ssl_auth_mode_e authmode;
    nwy_ssl_cert_t cacert;
    nwy_ssl_cert_t clientcert;
    nwy_ssl_cert_t clientkey;
    int sni_name_size;    /**< Length of the SNI server name. */
    char *sni_name; /**< Server name for SNI. */
}nwy_ssl_conf_t;



typedef struct {
    char version_string[64];
    int tls_version;
    int dtls_version;
} nwy_ssl_version_info_t;

/*
*****************************************************************************
* Prototype     : Retrieve current Mbed TLS version information.
* Description   :
                This function fills the nwy_ssl_version_info_t structure,
                containing the version string of Mbed TLS,
                supported TLS versions, and DTLS versions.
                Version information is stored in the form of bitwise OR,
                with each bit representing a specific protocol version.
* Input         :nwy_ssl_version_info_t *info:
* Output        : nwy_ssl_version_info_t *info

    if (info.tls_version & (1 << 0)) NWY_SDK_LOG_INFO(" support SSL 3.0");
    if (info.tls_version & (1 << 1)) NWY_SDK_LOG_INFO(" support TLS 1.0");
    if (info.tls_version & (1 << 2)) NWY_SDK_LOG_INFO(" support TLS 1.1");
    if (info.tls_version & (1 << 3)) NWY_SDK_LOG_INFO(" support TLS 1.2");
    if (info.tls_version & (1 << 4)) NWY_SDK_LOG_INFO(" support TLS 1.3");
    if (info.dtls_version & (1 << 0)) NWY_SDK_LOG_INFO(" support DTLS 1.0");
    if (info.dtls_version & (1 << 1)) NWY_SDK_LOG_INFO(" support DTLS 1.2");

* Return Value  : nwy_error_e
*****************************************************************************
*/
int nwy_ssl_version_info_get(nwy_ssl_version_info_t *info);

#ifdef __cplusplus
   }
#endif

#endif

