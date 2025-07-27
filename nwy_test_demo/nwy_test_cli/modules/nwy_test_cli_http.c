/*
* File name: nwy_test_cli_http.c
* Author:gz  
* Date:2023.12.12
* Description:  HTTP/ssl HTTP interface testing program, supporting specified multi-channel CID testing, each channel can establish three clients

* Others: 
* Function List:  

* History: 
   1. Date: 2023.12.12
      Author:gz
      version:V1.0
   2. ...
*/
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_http_api.h"
#define NWY_CLI_HTTP_CLIENT_ID_MAX 3
#define NWY_FTP_CLI_CID_MAX 7
typedef struct
{
    char cmd[16];
    char uri[1024+1];
    char domain[256];
    int connect_alive;
    int content_length;
    char content_type[128];
}test_open_param_header_t;

nwy_http_handle_t http_handle[NWY_FTP_CLI_CID_MAX][NWY_CLI_HTTP_CLIENT_ID_MAX] = {{NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL},
                                                                                  {NULL,NULL,NULL}};

int nwy_cli_get_http_clientid(nwy_http_handle_t handl,    unsigned short cid)
{
    int i = 0;
    for(i = 0;i<NWY_CLI_HTTP_CLIENT_ID_MAX;i++)
    {
        if(http_handle[cid-1][i] == handl)
        {
            return i;
        }
    }
    i = -1;
}
static void nwy_cli_https_result_cb(nwy_http_result_t *param)
{
    int clientid = 0;
    int clean_flg = 0;
    unsigned short cid = 0;
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    clientid = nwy_cli_get_http_clientid(param->http_handle,param->cid);
    cid = param->cid;

    NWY_CLI_LOG("event is %d", param->event);
    if (NWY_HTTP_DNS_ERR == param->event)
    {
        clean_flg = 1;
        nwy_test_cli_echo("\r\nHTTP dns err,clientid:%d,cid:%d",clientid,cid);
    }
    else if(NWY_HTTP_OPENED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP open ok,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTP_OPEN_FAIL == param->event)
    {
        clean_flg = 1;
        nwy_test_cli_echo("\r\nHTTP open fail,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTPS_SSL_CONNECTED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP setup success,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTP_CLOSED_PASV == param->event)
    {
        clean_flg = 1;
        nwy_test_cli_echo("\r\nHTTP closed pasv,clientid:%d,cid:%d",clientid,cid);
    }
    else if(NWY_HTTP_CLOSED == param->event)
    {
        clean_flg = 1;
        nwy_test_cli_echo("\r\nHTTP closed,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTP_DATA_RECVED == param->event)
    {
        if (NULL == param->data || param->data_len <=0)
        {
            nwy_test_cli_echo("\r\nclientid:%d,cid:%d,recved data is NULL,datalen:%d,cid:%d",clientid,cid,param->data_len);
            return;
        }
    
        if (param->data_len != 0)
        {
            nwy_test_cli_echo("\r\nclientid :%d,cid:%d,recv data_len :%d,data:%s", clientid,cid,param->data_len,param->data);
            nwy_thread_sleep(50);
        }

    }
    else if (NWY_HTTP_DATA_SEND_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP request send error,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTP_DATA_SEND_FINISHED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP request send ok,clientid:%d,cid:%d",clientid,cid);
    }
    else if (NWY_HTTP_EVENT_SSL_CONNECT_FAIL == param->event)
    {
        clean_flg = 1;
        nwy_test_cli_echo("\r\nHTTP SSL handshare fail,clientid:%d,cid:%d",clientid,cid);
    }
    else
    {
        nwy_test_cli_echo("\r\nHTTP unkown error,clientid:%d,cid:%d",clientid,cid);
    }
   if( clean_flg == 1)
    {
        http_handle[cid-1][clientid] = NULL;
    }

}


void nwy_test_cli_http_setup()
{
    int port = 0;
    unsigned short channel = 0;
    char host[256+1] = {0};
    char *sptr = NULL;
    int s_nwy_http_choice = 0;
    nwy_http_param_t http_param;
    nwy_ssl_conf_t sslcfg;
    nwy_http_handle_t *http_handle_ext = NULL;
    int http_client_id = 0;
    memset(&http_param,0x00,sizeof(http_param));
    memset(&sslcfg,0x00,sizeof(sslcfg));
    
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    
    sptr = nwy_test_cli_input_gets("\r\n input ip(<=256):");
    strcpy(host, sptr);
    
    sptr = nwy_test_cli_input_gets("\r\n input port:");
    port = nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input timeout(s <=60):");
    http_param.timeout_s = nwy_cli_str_to_int(sptr);
    if(http_param.timeout_s >60)
    {
        nwy_test_cli_echo("\r\nninput error ");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input 0  http, 1 ssl http:");
    s_nwy_http_choice = nwy_cli_str_to_int(sptr);
    if(s_nwy_http_choice != 0 && s_nwy_http_choice !=1)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] != NULL)
    {
        nwy_test_cli_echo("\r\nhttp exist");
        return;
    }

    http_param.host = host;
    http_param.cb = nwy_cli_https_result_cb;
    http_param.port = port;
    http_param.cid = channel;

    if (s_nwy_http_choice ==1)
    {

        sptr = nwy_test_cli_input_gets("\r\n input ssl version: 0:SSL3.0 1:TLS1.0 2:TLS1.1 3:TLS1.2: 4:TLS1.3");
        sslcfg.ssl_version = (nwy_ssl_version_e)nwy_cli_str_to_int(sptr);

        sptr = nwy_test_cli_input_gets("\r\n input authmode: 0:No authentication 1:Manage server authentication 2:Manage server and client authentication");
        sslcfg.authmode = (nwy_ssl_auth_mode_e)nwy_cli_str_to_int(sptr);
        if (NWY_SSL_AUTH_NONE_E != sslcfg.authmode)
        {
           //ca 
           sptr = nwy_test_cli_input_gets("\r\n input ca cert length(1-4096):");
           sslcfg.cacert.cert_len =  nwy_cli_str_to_int(sptr);
           if (sslcfg.cacert.cert_len < 1 || sslcfg.cacert.cert_len>4096)
           {
               nwy_test_cli_echo("\r\n invalid ca cert size:%d", sslcfg.cacert.cert_len);
               return;
           }
           sslcfg.cacert.cert_data = malloc(sslcfg.cacert.cert_len+1);
           memset(sslcfg.cacert.cert_data,0x00,sslcfg.cacert.cert_len+1);
           nwy_cli_get_trans_data(sslcfg.cacert.cert_data,sslcfg.cacert.cert_len);

           if(sslcfg.authmode == NWY_SSL_AUTH_MUTUAL_E)
           {
               //client cert
               sptr = nwy_test_cli_input_gets("\r\n input client cert length(1-4096):");
               sslcfg.clientcert.cert_len =  nwy_cli_str_to_int(sptr);
               if (sslcfg.clientcert.cert_len < 1 || sslcfg.clientcert.cert_len>4096)
               {
                   nwy_test_cli_echo("\r\n invalid client cert size:%d", sslcfg.clientcert.cert_len);
                    goto error_exit;
               }
               sslcfg.clientcert.cert_data = malloc(sslcfg.clientcert.cert_len+1);
               memset(sslcfg.clientcert.cert_data,0x00,sslcfg.clientcert.cert_len+1);
               nwy_cli_get_trans_data(sslcfg.clientcert.cert_data,sslcfg.clientcert.cert_len);

               //client key
               sptr = nwy_test_cli_input_gets("\r\n input client key length(1-4096):");
               sslcfg.clientkey.cert_len =  nwy_cli_str_to_int(sptr);
               if (sslcfg.clientkey.cert_len < 1 || sslcfg.clientkey.cert_len>4096)
               {
                   nwy_test_cli_echo("\r\n invalid client key size:%d", sslcfg.clientkey.cert_len);
                    goto error_exit;
               }
               sslcfg.clientkey.cert_data = malloc(sslcfg.clientkey.cert_len+1);
               memset(sslcfg.clientkey.cert_data,0x00,sslcfg.clientkey.cert_len+1);
               nwy_cli_get_trans_data(sslcfg.clientkey.cert_data,sslcfg.clientkey.cert_len);
           }
        }
        http_handle[channel-1][http_client_id]= nwy_http_setup(&http_param, &sslcfg);
    }
    else
    {
        http_handle[channel-1][http_client_id]= nwy_http_setup(&http_param, NULL);
    }
    if (NULL != http_handle[channel-1][http_client_id])
    {
        nwy_test_cli_echo("\r\n %s success", s_nwy_http_choice ? "nwy_https_setup" : "nwy_http_setup");
    }
    else
    {
        nwy_test_cli_echo("\r\n %s fail", s_nwy_http_choice ? "nwy_https_setup" : "nwy_http_setup");
    }

    error_exit:

    if(sslcfg.cacert.cert_data)
    {
        free(sslcfg.cacert.cert_data);
    }

    if(sslcfg.clientcert.cert_data)
    {
        free(sslcfg.clientcert.cert_data);
    }

    if(sslcfg.clientkey.cert_data)
    {
         free(sslcfg.clientkey.cert_data);
    }
}
void nwy_test_cli_http_get()
{
    char *sptr = NULL;
    int keepalive = 0;
    int offset = 0;
    int size = 0;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    int http_client_id = 0;
    unsigned short channel = 0;
    char uri[1024+1] = {0};
    http_get_param_t param;
    memset(&param,0x00,sizeof(param));
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input uri(<=1024):");
    strcpy(uri, sptr);

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", keepalive);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input offset:");
    offset = nwy_cli_str_to_int(sptr);
    sptr = nwy_test_cli_input_gets("\r\n input size:");
    size = nwy_cli_str_to_int(sptr);

    param.keepalive = keepalive;
    param.len = size;
    param.offset = offset;
    param.uri = uri;

    result = nwy_http_get(http_handle[channel-1][http_client_id], &param);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_get success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_get fail, error code: %d", result);
    }
}
void nwy_test_cli_http_head()
{
    char *sptr = NULL;
    int keepalive = 0;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    int http_client_id = 0;
    unsigned short channel = 0;
    char uri[1024+1] = {0};
    http_head_param_t param;
    memset(&param,0x00,sizeof(param));
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input uri(<=1024):");
    strcpy(uri, sptr);

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", keepalive);
        return;
    }
    param.keepalive = keepalive;
    param.uri = uri;

    result = nwy_http_head(http_handle[channel-1][http_client_id], &param);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_get success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_get fail, error code: %d", result);
    }

}
void nwy_test_cli_http_post()
{
    char *sptr = NULL;
    int keepalive = 0;
    int type = 0;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    char *data = NULL;
    int len = 0;
    int http_client_id = 0;
    unsigned short channel = 0;
    char uri[1024+1] = {0};
    http_post_param_t param;
    memset(&param,0x00,sizeof(param));
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input uri(<=1024):");
    strcpy(uri, sptr);

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", keepalive);
        return;
    }


    sptr = nwy_test_cli_input_gets("\r\n input type: 0:x-www-form-urlencoded 1:text 2:json 3:xml 4:html");
    type = nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input post data len[1-3072]:");
    len = nwy_cli_str_to_int(sptr);
    if(len == 0 || len > 3072)
    {
        nwy_test_cli_echo("\r\n input data len error!\r\n");
        return;
    }

    data = malloc(len+1);
    memset(data,0x00,len+1);
    nwy_cli_get_trans_data(data,len);
    
    param.keepalive = keepalive;
    param.uri = uri;
    param.data = data;
    param.len = len;
    param.type = type;

    result = nwy_http_post(http_handle[channel-1][http_client_id], &param);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_post success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_post fail, error code: %d", result);
    }
    free(data);
}

void nwy_test_cli_http_put()
{
    char *sptr = NULL;
    int keepalive = 0;
    int type = 0;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    char *data = NULL;
    int len = 0;
    int http_client_id = 0;
    unsigned short channel = 0;
    char uri[1024+1] = {0};
    http_put_param_t param;
    memset(&param,0x00,sizeof(param));
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input uri(<=1024):");
    strcpy(uri, sptr);

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", keepalive);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input type: 0:x-www-form-urlencoded 1:text 2:json 3:xml 4:html");
    type = nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input put data len[1-3072]:");
    len = nwy_cli_str_to_int(sptr);
    if(len == 0 || len > 3072)
    {
        nwy_test_cli_echo("\r\n input data len error!\r\n");
        return;
    }

    data = malloc(len+1);
    memset(data,0x00,len+1);
    nwy_cli_get_trans_data(data,len);

    param.keepalive = keepalive;
    param.uri = uri;
    param.data = data;
    param.len = len;
    param.type = type;

    result = nwy_http_put(http_handle[channel-1][http_client_id], &param);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_put success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_put fail, error code: %d", result);
    }
    free(data);

}
static void nwy_test_set_http_header(unsigned char *header_buf, test_open_param_header_t * header_param) {
    const char *http_version = "HTTP/1.1\r\n";
    const char *connection_str = header_param->connect_alive ? "Connection: keep-alive\r\n" : "Connection: close\r\n";
    const char *host_str = "Host: ";
    char content_length_str[30];
    const char *cache_control = "Cache-Control: no-cache\r\n";
    const char *crlf = "\r\n";
    char content_len[] = "Content-Length: ";

    // Action
    strcat(header_buf, header_param->cmd);
    strcat(header_buf, " ");
    // URL
    strcat(header_buf, header_param->uri);
    // HTTP Version
    strcat(header_buf, " ");
    strcat(header_buf, http_version);
    // Connection
    strcat(header_buf, connection_str);
    // Host
    strcat(header_buf, host_str);
    strcat(header_buf, header_param->domain);
    strcat(header_buf, crlf);

    // Content-Length and Content-Type for PUT and POST
    if(strcmp(header_param->cmd,"PUT")==0||strcmp(header_param->cmd, "POST")==0) {
        sprintf(content_length_str, "Content-Length: %d\r\n", header_param->content_length);
        strcat(header_buf, content_length_str);
        strcat((char*)header_buf, header_param->content_type);
        strcat(header_buf, crlf);
    }

    // Cache-Control
    strcat(header_buf, cache_control);

    // Final CRLF
    strcat(header_buf, crlf);
}

void nwy_test_cli_http_open_mode()
{
    char *sptr = NULL;
    int body_send_len = 0;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    char *data = NULL;
    int len = 0;
    int   header_len =0;
    int http_client_id = 0;
    unsigned short channel = 0;
    unsigned char header_buf[4096] = {0};
    test_open_param_header_t header_param;
    memset(&header_param,0x00,sizeof(header_param));
    
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input http cmd(GET,PUT,POST,HEAD)");
    if(strlen(sptr)>sizeof(header_param.cmd)-1 || (strcmp(sptr,"GET")!=0 && strcmp(sptr,"PUT")!=0 && strcmp(sptr,"POST")!=0&& strcmp(sptr,"HEAD")!=0))
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    snprintf(header_param.cmd,sizeof(header_param.cmd),"%s",sptr);

    
    sptr = nwy_test_cli_input_gets("\r\n input uri(<=1024):");
    if(strlen(sptr)>sizeof(header_param.uri)-1)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(sptr[0] != '/')
    {
        snprintf(header_param.uri,sizeof(header_param.uri),"/%s",sptr);
    }
    else
    {
        snprintf(header_param.uri,sizeof(header_param.uri),"%s",sptr);
    }

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    header_param.connect_alive = nwy_cli_str_to_int(sptr);
    if ((header_param.connect_alive != 0)&& (header_param.connect_alive != 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", header_param.connect_alive);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input header Host:");
    if(strlen(sptr)>sizeof(header_param.domain)-1)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }
    strcpy(header_param.domain, sptr);



    if(strcmp(header_param.cmd,"GET")!=0 && strcmp(header_param.cmd,"HEAD")!=0)
    {
        sptr = nwy_test_cli_input_gets("\r\n input content_type(e.g.:Content-Type: application/text):");
        if(strlen(sptr)>sizeof(header_param.content_type)-1)
        {
            nwy_test_cli_echo("\r\ninput error :max len %d",sizeof(header_param.content_type)-1);
            return;
        }
        strcpy(header_param.content_type, sptr);

        sptr = nwy_test_cli_input_gets("\r\n input body len[1-3072]:");
        header_param.content_length = nwy_cli_str_to_int(sptr);
        if (header_param.content_length <= 0|| len > 3072)
        {
            nwy_test_cli_echo("\r\nInvaild content_length ");
            return;
        }

        data = malloc(header_param.content_length+1);
        memset(data,0x00,header_param.content_length+1);
        nwy_cli_get_trans_data(data,header_param.content_length);

        nwy_test_set_http_header(header_buf, &header_param);
        header_len = strlen(header_buf);
        memcpy(&header_buf[header_len],data,header_param.content_length);
        header_len +=header_param.content_length;
        header_buf[header_len] = '\0';

    }
    else
    {
        nwy_test_set_http_header(header_buf, &header_param);
        header_len = strlen(header_buf);
        header_buf[header_len] = '\0';
    }


    result = nwy_http_open_mode(http_handle[channel-1][http_client_id], header_buf, header_len);
    if (NWY_SUCCESS == result)
    {
        if(strcmp(header_param.cmd,"GET")==0 || strcmp(header_param.cmd,"HEAD")==0)
        {
            nwy_test_cli_echo("\r\n nwy_http_open_mode success");
        }
        else
        {
            nwy_test_cli_echo("\r\n send http header success");
        }
    }
    else
    {
        nwy_test_cli_echo("\r\n send http header error: %d", result);
        return;
    }

    if(data != NULL)
        free(data);
}

void nwy_test_cli_http_close()
{
    char *sptr = NULL;
    nwy_error_e result = NWY_GEN_E_UNKNOWN;
    int http_client_id = 0;
    unsigned short channel = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = (unsigned short)nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input http client id (0,1,2)");
    http_client_id = nwy_cli_str_to_int(sptr);
    if(http_client_id != 0 && http_client_id !=1 && http_client_id != 2)
    {
        nwy_test_cli_echo("\r\ninput error ");
        return;
    }

    if(http_handle[channel-1][http_client_id] == NULL)
    {
        nwy_test_cli_echo("\r\nhttp not exist");
        return;
    }

    result = nwy_http_close(http_handle[channel-1][http_client_id]);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_close success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_close fail, error code: %d", result);
    }

}


