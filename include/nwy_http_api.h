/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_http_api.c
 *Function description: HTTP/ssl HTTP API interface, each cid supports the creation of three HTTP clients
 * Author       : nwy
 * Created      : 2024-2-3
 * Description  : http API
 ******************************************************************************/
#ifndef __NWY_HTTP_API_H__
#define __NWY_HTTP_API_H__

#include "nwy_common.h"
#include "nwy_ssl_config.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef void *nwy_http_handle_t;

typedef enum
{
    NWY_HTTP_DNS_ERR,       //Dns error
    NWY_HTTP_OPEN_FAIL,     //open fail
    NWY_HTTP_OPENED,        //setup success
    NWY_HTTPS_SSL_CONNECTED, //https setup success
    NWY_HTTP_CLOSED_PASV,   //server closed
    NWY_HTTP_CLOSED,        //client closed
    NWY_HTTP_DATA_RECVED,   //recv data
    NWY_HTTP_DATA_SEND_ERR, //data send error
    NWY_HTTP_EVENT_SSL_CONNECT_FAIL,
    NWY_HTTP_DATA_SEND_FINISHED, //data send finished
    NWY_HTTP_EVENT_UNKOWN,  //unkown error
}nwy_http_event_e;

typedef enum
{
    NWY_HTTP_CONTENT_TYPE_URLENCODE,//"Content-Type: application/x-www-form-urlencoded",
    NWY_HTTP_CONTENT_TYPE_TEXT,//"Content-Type: application/text", 
    NWY_HTTP_CONTENT_TYPE_JSON,//"Content-Type: application/json",
    NWY_HTTP_CONTENT_TYPE_XML,//"Content-Type: application/xml", 
    NWY_HTTP_CONTENT_TYPE_HTML,//"Content-Type: application/html",
}nwy_http_content_type_e;
    
typedef struct{
    unsigned short cid;//Channel range 1-7, one channel can support 3 HTTP clients
    nwy_http_handle_t http_handle; 
    unsigned int event;
    unsigned int data_len; 
    char* data; 
}nwy_http_result_t;

typedef void (*nwy_http_resultcb)(nwy_http_result_t *event);

typedef struct{
    unsigned short cid;
    int timeout_s;
    char *host;
    int port;
    nwy_http_resultcb cb;//callback function
}nwy_http_param_t;

typedef struct
{
    unsigned int keepalive;
    nwy_http_content_type_e type;
    const char* data;
    int len;
    char * uri;
}http_request_param_t;

typedef struct
{
    unsigned char keepalive;
    int offset;
    int len;
    char * uri;
}http_get_param_t;

typedef struct
{
    unsigned char keepalive;
    char * uri;
}http_head_param_t;

typedef http_request_param_t http_put_param_t;
typedef http_request_param_t http_post_param_t;

/*
@func
    nwy_http_setup
@desc
    http setup
@param
    HTTP_ Param:
    char *url: Path where the resource is located
    int port: target port number
    nwy_http_resultcb: cb event notification callback function
    int timeout_s Set the HTTP connection timeout time in seconds
    nwy_app_ssl_conf_t *ssl_cfg: The specific definition of cfg SSL configuration information can be found in nwy_ssl_config.h, NULL indicates HTTP, non NULL indicates using SSL HTTP encryption

@return
    Success: Return HTTP/ssl HTTP client handle
    Failure: NULL
@other
  Once http login is completed successfully, the callback function cb will be triggered.
  NWY_HTTP_OPENED event will report
*/
nwy_http_handle_t  nwy_http_setup(nwy_http_param_t *http_param, nwy_ssl_conf_t *ssl_cfg);

/*
@func
    nwy_http_get
@desc
    HTTP get action
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle
    
    unsigned char keepalive (0,1):
    
        0: Short link
    
        1: Long connection
    http_get_param_t *param:
        unsigned char keepalive (0,1):
        
            0: Short link
        
            1: Long connection
        
        Int offset: The offset specifies the starting position for downloading
    
        Int len: Length specifies the download length

        char * uri:Resource identifier,The resource path starts with a backslash "/", for example: "/boot/test. txt"

@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
  1. Offset of 0 and len of 0 indicate full package download
  2. The total package size can be determined through the interface nwy_http_get_head() is obtained.
  3. If len is not 0, it indicates a breakpoint request for download
  4.  Once get action success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_DATA_RECVED will report
*/
nwy_error_e nwy_http_get(nwy_http_handle_t handle, http_get_param_t *param);

/*
@func
    nwy_http_get_head
@desc
    HTTP get head action
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle
    http_head_param_t *param:
        unsigned char keepalive (0,1):
        
            0: Short link
        
            1: Long connection
         char * uri:Resource identifier,The resource path starts with a backslash "/", for example: "/boot/test. txt"
            

@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
1.  Once get head action success, The Server replies back to the data, the callback function cb will be triggered.
NWY_HTTP_DATA_RECVED will report
*/
nwy_error_e nwy_http_head(nwy_http_handle_t handle, http_head_param_t *param);

/*
@func
    nwy_http_post
@desc
    HTTP post action
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle
    
    unsigned char keepalive (0,1):
    
        0: Short link
    
        1: Long connection
     http_post_param_t *param:

        const char *data: User data(<3072)
        int len:         User data length
        nwy_http_content_type_e type:     request message data type
                     NWY_HTTP_CONTENT_TYPE_URLENCODE,//"Content-Type: application/x-www-form-urlencoded",
                     NWY_HTTP_CONTENT_TYPE_TEXT,//"Content-Type: application/text", 
                     NWY_HTTP_CONTENT_TYPE_JSON,//"Content-Type: application/json",
                     NWY_HTTP_CONTENT_TYPE_XML,//"Content-Type: application/xml", 
                     NWY_HTTP_CONTENT_TYPE_HTML,//"Content-Type: application/html",
        char * uri:Resource identifier,The resource path starts with a backslash "/", for example: "/boot/test. txt"


@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
1.  Once post action success, The Server replies back to the data, the callback function cb will be triggered.
NWY_HTTP_DATA_RECVED will report
2. Maximum upload data size 3072

3. If you want to upload big data, please use the open method to upload and call the nwy interface nwy_http_open_mode()

*/
nwy_error_e nwy_http_post(nwy_http_handle_t handle, http_post_param_t *param);
/*
@func
    nwy_http_put
@desc
    HTTP put action
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle
    
    unsigned char keepalive (0,1):
    
        0: Short link
    
        1: Long connection
        
     http_put_param_t *param:

        const char *data: User data(<3072)
        int len:         User data length
        nwy_http_content_type_e type:     request message data type
                     NWY_HTTP_CONTENT_TYPE_URLENCODE,//"Content-Type: application/x-www-form-urlencoded",
                     NWY_HTTP_CONTENT_TYPE_TEXT,//"Content-Type: application/text", 
                     NWY_HTTP_CONTENT_TYPE_JSON,//"Content-Type: application/json",
                     NWY_HTTP_CONTENT_TYPE_XML,//"Content-Type: application/xml", 
                     NWY_HTTP_CONTENT_TYPE_HTML,//"Content-Type: application/html",
        char * uri:Resource identifier,The resource path starts with a backslash "/", for example: "/boot/test. txt"
@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
1.  Once put action success, The Server replies back to the data, the callback function cb will be triggered.
NWY_HTTP_DATA_RECVED will report
2. Maximum upload data size 3072

3. If you want to upload big data, please use the open method to upload and call the nwy interface nwy_http_open_mode()

*/
nwy_error_e nwy_http_put(nwy_http_handle_t handle, http_put_param_t *param);

/*
@func
    nwy_http_open_mode
@desc
    Open definition interface for customizing HTTP protocol headers and message bodies
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle
    
    unsigned char keepalive (0,1):
    
        0: Short link
    
        1: Long connection

     const char *data: User data
     int len:User data length

@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
1.  Once put action success, The Server replies back to the data, the callback function cb will be triggered.
NWY_HTTP_DATA_RECVED will report
2. Custom mode interface requires adding HTTP header and body

3. Can be used to manipulate customer customized HTTP header information

4. Can be used to process big data
*/
nwy_error_e nwy_http_open_mode(nwy_http_handle_t handle,const char* data, int len);

/*
@func
    nwy_http_close
@desc
    Close HTTP client
@param
    nwy_http_handle_t handle£º http/ssl HTTP client handle

@return
    Success: NWY_ SUCCESS
    
    Failed: nwy_error_e

@other
1.  Once close success, The Server replies back to the data, the callback function cb will be triggered.
  NWY_HTTP_CLOSED will report
*/
nwy_error_e nwy_http_close(nwy_http_handle_t handle);

#ifdef __cplusplus
}
#endif
#endif //__NWY_HTTP_API_H__ endif
