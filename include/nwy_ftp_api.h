/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_ftp_api.c
 *Function description: Supports file upload, download, and deletion(ftp and ssl ftp),Data channels (1-7), each channel supports the establishment of an FTP client
 * Author       : nwy
 * Created      : 2023-12-12
 * Description  : ftp API
 ******************************************************************************/
#ifndef __NWY_FTP_API_H__
#define __NWY_FTP_API_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "nwy_common.h"
#include "nwy_ssl_config.h"

typedef void *nwy_ftp_handle_t;

typedef enum
{
    FTP_STOR_MODE=1,//Create a file on the server to write data, and if the file exists, overwrite the source file
    FTP_APPE_MODE ,//Create a file on the server to write data, and if the file exists, attach it to the end of the file
}nwy_ftp_put_mode_e;
    

typedef enum
{
    SSLFTP_TYPE_EXPLICIT =1,//Explicit ssl
    SSLFTP_TYPE_IMPLICIT //Implicitssl
}nwy_ssl_ftp_type_e; 

typedef struct{
    nwy_ssl_ftp_type_e ftps_type; 
    nwy_ssl_conf_t ssl_config;
}nwy_ftp_ssl_cfg_t;

typedef struct{
    unsigned char eof;
    char *remotefile;
    unsigned char *data;
    int len;
    unsigned char type;
    nwy_ftp_put_mode_e mode;
}nwy_ftp_put_param_t;

typedef struct{
    char * remotefile;
    unsigned char type;
    int offset;
    int len;
}nwy_ftp_get_param_t;


typedef enum
{
    NWY_FTP_EVENT_DNS_ERR,
    NWY_FTP_EVENT_OPEN_FAIL,
    NWY_FTP_EVENT_CLOSED,
    NWY_FTP_EVENT_LOGOUT,
    NWY_FTP_EVENT_DATA_GET,
    NWY_FTP_EVENT_DATA_GET_ERROR,
    NWY_FTP_EVENT_DATA_OPEND,
    NWY_FTP_EVENT_DATA_CLOSED,
    NWY_FTP_EVENT_DATA_SETUP_ERROR,
    NWY_FTP_EVENT_DATA_GET_FINISHED,
    NWY_FTP_EVENT_LOGIN,
    NWY_FTP_EVENT_SIZE,            
    NWY_FTP_EVENT_PASS_ERROR,
    NWY_FTP_EVENT_FILE_NOT_FOUND,
    NWY_FTP_EVENT_FILE_SIZE_ERROR,
    NWY_FTP_EVENT_FILE_DELE_SUCCESS,
    NWY_FTP_EVENT_FILE_DELE_ERROR,
    NWY_FTP_EVENT_DATA_PUT_FINISHED,
    NWY_FTP_EVENT_DATA_PUT_ERROR,
    NWY_FTP_EVENT_CLOSED_SSL_FAIL,
    NWY_FTP_EVENT_SSL_CONNECT_FAIL,
    NWY_FTP_EVENT_SSL_CONNECTED,
    NWY_FTP_EVENT_UNKOWN,
}nwy_ftp_event_e;

typedef struct{
    unsigned short channel;//Data channels (1-7), each channel supports the establishment of an FTP client
    nwy_ftp_handle_t ftp_handle; //FTP client handle
    unsigned int event; //event status 
    unsigned int data_len;//recv len
    char* data;  //Response data
}nwy_ftp_result_t;
typedef void (*nwy_ftp_resultcb)(nwy_ftp_result_t *event);

typedef struct{
    unsigned short cid;
    int timeout_s;
    char *host;
    int port;
    char *username;
    char *password;
    unsigned char mode;//0 pasv 1:port
    nwy_ftp_resultcb cb;//callback function
}nwy_ftp_param_t; 


/*
 *****************************************************************************
 * Prototype     : nwy_ftp_login
* Description    : ftp login
 * Input         : 
                 : nwy_ftp_param_t *param: ftp parameter information
 * Output        : nwy_ftp_ssl_cfg_t *sslcfg :SSL FTP configuration information,NULL means not using SSL
 * Return Value  : 
            Success: FTP client handle

            Failure: NULL
 * Author        : nwy
 *Note: Supports up to two FTP clients simultaneously
 *****************************************************************************
*/
nwy_ftp_handle_t nwy_ftp_login(nwy_ftp_param_t *param,nwy_ftp_ssl_cfg_t *sslcfg);

/*
 *****************************************************************************
 * Prototype     : nwy_ftp_logout
* Description    : ftp logout 
 * Input         : nwy_ftp_handle_t handle :FTP client handle
 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : nwy
 *****************************************************************************
*/
nwy_error_e nwy_ftp_logout (nwy_ftp_handle_t handle);

/*
 *****************************************************************************
 * Prototype     : nwy_ftp_get
* Description    : get file from remote server
 * Input         : 
                 nwy_ftp_handle_t handle: ftp/sslftp client handle
                 nwy_ftp_get_param_t *ftp_get_param:
                     char * remotefile: The file name to be obtained
                     unsigned char type: 1, ascii 2, binary
                     int offset: file offset
                     int len: The current length of the file to be obtained;

 *The parameters offset and len are set to 0 and default to full package download.
 
 *The size len of the entire packet data can be obtained from the response of the callback event ''NWY_FTP_EVENTSIZE' by calling the interface function ''nwy_ftp_filesize()''

 * Return Value  : nwy_error_e
 * Author        : nwy
 *****************************************************************************
*/
nwy_error_e nwy_ftp_get(nwy_ftp_handle_t handle, nwy_ftp_get_param_t *ftp_get_param);

/*
 *****************************************************************************
 * Prototype     : nwy_ftp_put
* Description    : ftp put file to server
 * Input         : 
                 nwy_ftp_handle_t handle: ftp/sslftp client handle
                 nwy_ftp_put_param_t *ftp_put_param:
                     unsigned char eof: Transmission end flag: 0x00 indicates that it has not ended, 0x01 indicates that it has ended
                     char * remotefile: Upload data to the specified file name on the server
                     unsigned char * data: The data that needs to be written to the file
                     int len: The length of data written (not greater than 4096)
                     unsigned char type: 1, ascii 2, binary
                     nwy_ftp_put_mode_e mode:
                     1 Create a file on the server to write data, and if the file exists, overwrite the source file
                     2 Create a file on the server to write data, and if the file exists, attach it to the end of the file

 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : nwy
 *****************************************************************************
*/
nwy_error_e nwy_ftp_put(nwy_ftp_handle_t handle, nwy_ftp_put_param_t *ftp_put_param);


/*
 *****************************************************************************
 * Prototype     : nwy_ftp_get
* Description    : ftp get file size 
 * Input         : 
                nwy_ftp_handle_t  handle: ftp/sslftp client handle
                const char * remotefile: The file name to be obtained
 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : nwy
 *****************************************************************************
*/
nwy_error_e nwy_ftp_get_filesize(nwy_ftp_handle_t handle, const char* filename);

/*
 *****************************************************************************
 * Prototype     : nwy_ftp_delete
* Description    : ftp remove file
 * Input         : 
                 nwy_ftp_handle_t  handle: ftp/sslftp client handle
                const char * remotefile: The file name to be obtained
 * Output        : None
 * Return Value  : nwy_error_e
 * Author        : nwy
 *****************************************************************************
*/
nwy_error_e nwy_ftp_del_file(nwy_ftp_handle_t handle, const char *remotefile);
#ifdef __cplusplus
   }
#endif
#endif
