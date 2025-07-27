/*
* File name: nwy_test_cli_ftp.c
* Author:gz  
* Date:2023.12.12
* Description:  FTP/ssl FTP interface testing program, supporting specified multi-channel CID testing

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
#include "nwy_ftp_api.h"

#define NWY_FTP_CLI_CID_MAX 7
nwy_ftp_handle_t ftp_test_handl[NWY_FTP_CLI_CID_MAX] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};

static void nwy_cli_ftp_result_cb(nwy_ftp_result_t *param)
{
    int *size;
    int ret;
    static int get_file_len = 0;
    char *g_ftp_data = NULL;
    unsigned short cid = 0;
    int clean_flg = 0;
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    
    NWY_SDK_LOG_DEBUG("event is %d", param->event);

    cid= param->channel;
    NWY_SDK_LOG_DEBUG("cli cid is %d", param->channel);

    switch(param->event)
    {
        case NWY_FTP_EVENT_DATA_GET_ERROR:
            nwy_test_cli_echo("\r\nFtp get error:%d",cid);
            break;
        case NWY_FTP_EVENT_LOGIN:
            nwy_test_cli_echo("\r\nFtp login success,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_DATA_OPEND:
            nwy_test_cli_echo("\r\nFtp data open,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_PASS_ERROR:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp passwd error,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_FILE_NOT_FOUND:
            nwy_test_cli_echo("\r\nFtp file not found,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_FILE_SIZE_ERROR:
            nwy_test_cli_echo("\r\nFtp file size error,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_DATA_SETUP_ERROR:
            nwy_test_cli_echo("\r\nFtp data setup error,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_SIZE:
            size = (int *)param->data;
            nwy_test_cli_echo("\r\nFtp size is %d,cid:%d", *size,cid);
            break;
        case NWY_FTP_EVENT_LOGOUT:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp logout,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_CLOSED:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp connection closed,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_FILE_DELE_SUCCESS:
            nwy_test_cli_echo("\r\nFtp file del success,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_FILE_DELE_ERROR:
            nwy_test_cli_echo("\r\nFtp file del error,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_DATA_PUT_FINISHED:
            nwy_test_cli_echo("\r\nFtp put file success,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_DATA_PUT_ERROR:
            nwy_test_cli_echo("\r\nFtp put file error,cid:%d",cid);
            break;

        case NWY_FTP_EVENT_DNS_ERR:
        case NWY_FTP_EVENT_OPEN_FAIL:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp login fail,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_DATA_GET:
        {
            if (NULL == param->data)
            {
                nwy_test_cli_echo("\r\ncid:%d,recved data is NULL,datalen:%d",cid,param->data_len);
                return;
            }

            if (param->data_len != 0)
            {
                g_ftp_data = (char *)malloc(param->data_len+1);
                memset(g_ftp_data,0x00,param->data_len+1);
                memcpy(g_ftp_data,param->data,param->data_len);
                nwy_test_cli_echo("\r\ncid:%d,recv data_len :%d,data:%s",cid, param->data_len,g_ftp_data);
                get_file_len=get_file_len+param->data_len;
                nwy_test_cli_echo("\r\ncid:%d,All lengths currently received:%d",cid,get_file_len);
                free(g_ftp_data);
                g_ftp_data = NULL;
                nwy_thread_sleep(50);
            }

        }
        break;
        case NWY_FTP_EVENT_DATA_GET_FINISHED:
            nwy_test_cli_echo("\r\ncid:%d,get completed, total data size is :%d",cid,param->data_len);
            break;
        case NWY_FTP_EVENT_DATA_CLOSED:
            get_file_len = 0;
            nwy_test_cli_echo("\r\nftp data close,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_SSL_CONNECT_FAIL:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nftp ssl error,cid:%d",cid);
            break;
        case NWY_FTP_EVENT_SSL_CONNECTED:
            nwy_test_cli_echo("\r\nftp ssl connected,cid:%d",cid);
            break;
        default:
            nwy_test_cli_echo("\r\nFtp unknown error,cid:%d",cid);
            break;

    }

    if( clean_flg == 1)
    {
        NWY_SDK_LOG_DEBUG("clean ftp client cid:%d", cid);
        nwy_test_cli_echo("\r\nclean ftp client cid:%d", cid);
        ftp_test_handl[cid-1]= NULL;
    }
    return;
}

void nwy_test_cli_ftp_login(void)
{
        char *sptr = NULL;
        int s_nwy_ftp_choice = 0;
        nwy_ftp_param_t ftp_param;
        nwy_ftp_ssl_cfg_t sslcfg;
        char host[255] = {0};
        char username[255] = {0};
        char pwd[255] = {0};
        int cid = 0;
        memset(&sslcfg,0x00,sizeof(sslcfg));

        memset(&ftp_param, 0x00, sizeof(ftp_param));
        
        sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
        cid = nwy_cli_str_to_int(sptr);
        if (cid < 1 || cid> 7)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }
        ftp_param.cid = (unsigned short)cid;

        if(ftp_test_handl[cid-1] != NULL)
        {
            nwy_test_cli_echo("\r\n ftp client exist ");
            return;
        }
        sptr = nwy_test_cli_input_gets("\r\n input ip(<=128):");
        if(strlen(sptr)>128)
        {
            nwy_test_cli_echo("\r\nip parameter error");
            return;
        }
    
        strcpy(host, sptr);
        ftp_param.host = host;
        sptr = nwy_test_cli_input_gets("\r\n input port(<=65535):");
        ftp_param.port = nwy_cli_str_to_int(sptr);
        
        sptr = nwy_test_cli_input_gets("\r\n input FTPtype (passive mode = 0 active mode = 1):");
        ftp_param.mode = nwy_cli_str_to_int(sptr);
        if (ftp_param.mode != 0 && ftp_param.mode != 1)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }
        sptr = nwy_test_cli_input_gets("\r\n input username(<=128):");
        if(strlen(sptr)>128)
        {
            nwy_test_cli_echo("\r\nusername parameter error");
            return;
        }
        strcpy(username, sptr);
        ftp_param.username = username;
        sptr = nwy_test_cli_input_gets("\r\n input passwd(<=128):");
        if(strlen(sptr)>128)
        {
            nwy_test_cli_echo("\r\npasswd parameter error");
            return;
        }
        strcpy(pwd, sptr);
        ftp_param.password = pwd;
        ftp_param.timeout_s = 60;
        ftp_param.cb = nwy_cli_ftp_result_cb;

        sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
        s_nwy_ftp_choice = nwy_cli_str_to_int(sptr);
        if (s_nwy_ftp_choice == 1)
        {
            sptr = nwy_test_cli_input_gets("\r\n input ftps type: 1-explicit,2-implicit:");
            sslcfg.ftps_type = nwy_cli_str_to_int(sptr);

            sptr = nwy_test_cli_input_gets("\r\n input ssl version: 0:SSL3.0 1:TLS1.0 2:TLS1.1 3:TLS1.2: 4:TLS1.3");
            sslcfg.ssl_config.ssl_version = (nwy_ssl_version_e)nwy_cli_str_to_int(sptr);

            sptr = nwy_test_cli_input_gets("\r\n input authmode: 0:No authentication 1:Manage server authentication 2:Manage server and client authentication:");
            sslcfg.ssl_config.authmode = (nwy_ssl_auth_mode_e)nwy_cli_str_to_int(sptr);
            if (NWY_SSL_AUTH_NONE_E != sslcfg.ssl_config.authmode)
            {
                //ca 
                sptr = nwy_test_cli_input_gets("\r\n input ca cert length(1-4096):");
                sslcfg.ssl_config.cacert.cert_len =  nwy_cli_str_to_int(sptr);
                if (sslcfg.ssl_config.cacert.cert_len < 1 || sslcfg.ssl_config.cacert.cert_len>4096)
                {
                    nwy_test_cli_echo("\r\n invalid ca cert size:%d", sslcfg.ssl_config.cacert.cert_len);
                    return;
                }
                sslcfg.ssl_config.cacert.cert_data = malloc(sslcfg.ssl_config.cacert.cert_len+1);
                memset(sslcfg.ssl_config.cacert.cert_data,0x00,sslcfg.ssl_config.cacert.cert_len+1);
                nwy_cli_get_trans_data(sslcfg.ssl_config.cacert.cert_data,sslcfg.ssl_config.cacert.cert_len);

                if(sslcfg.ssl_config.authmode == NWY_SSL_AUTH_MUTUAL_E)
                {
                    //client cert
                    sptr = nwy_test_cli_input_gets("\r\n input client cert length(1-4096):");
                    sslcfg.ssl_config.clientcert.cert_len =  nwy_cli_str_to_int(sptr);
                    if (sslcfg.ssl_config.clientcert.cert_len < 1 || sslcfg.ssl_config.clientcert.cert_len>4096)
                    {
                        nwy_test_cli_echo("\r\n invalid client cert size:%d", sslcfg.ssl_config.clientcert.cert_len);
                         goto error_exit;
                    }
                    sslcfg.ssl_config.clientcert.cert_data = malloc(sslcfg.ssl_config.clientcert.cert_len+1);
                    memset(sslcfg.ssl_config.clientcert.cert_data,0x00,sslcfg.ssl_config.clientcert.cert_len+1);
                    nwy_cli_get_trans_data(sslcfg.ssl_config.clientcert.cert_data,sslcfg.ssl_config.clientcert.cert_len);

                    //client key
                    sptr = nwy_test_cli_input_gets("\r\n input client key length(1-4096):");
                    sslcfg.ssl_config.clientkey.cert_len =  nwy_cli_str_to_int(sptr);
                    if (sslcfg.ssl_config.clientkey.cert_len < 1 || sslcfg.ssl_config.clientkey.cert_len>4096)
                    {
                        nwy_test_cli_echo("\r\n invalid client key size:%d", sslcfg.ssl_config.clientkey.cert_len);
                         goto error_exit;
                    }
                    sslcfg.ssl_config.clientkey.cert_data = malloc(sslcfg.ssl_config.clientkey.cert_len+1);
                    memset(sslcfg.ssl_config.clientkey.cert_data,0x00,sslcfg.ssl_config.clientkey.cert_len+1);
                    nwy_cli_get_trans_data(sslcfg.ssl_config.clientkey.cert_data,sslcfg.ssl_config.clientkey.cert_len);
                }
            }
    
            ftp_test_handl[cid-1] = nwy_ftp_login(&ftp_param,&sslcfg);
        }
        else
        {
            ftp_test_handl[cid-1] = nwy_ftp_login(&ftp_param,NULL);
        }

        
        if (NULL != ftp_test_handl[cid-1])
        {
            nwy_test_cli_echo("\r\n %s success", s_nwy_ftp_choice ? "nwy_ftps_setup" : "nwy_ftp_setup");
        }
        else
        {
            nwy_test_cli_echo("\r\n %s fail", s_nwy_ftp_choice ? "nwy_ftps_setup" : "nwy_ftp_setup");
        }
        NWY_SDK_LOG_DEBUG("handl:%p, cid:%d", ftp_test_handl[cid-1],cid);
        error_exit:

        if(sslcfg.ssl_config.cacert.cert_data)
        {
            free(sslcfg.ssl_config.cacert.cert_data);
        }
        
        if(sslcfg.ssl_config.clientcert.cert_data)
        {
            free(sslcfg.ssl_config.clientcert.cert_data);

        }
        if(sslcfg.ssl_config.clientkey.cert_data)
        {
             free(sslcfg.ssl_config.clientkey.cert_data);
        }

}

void nwy_test_cli_ftp_logout(void)
{
    char *sptr = NULL;
    int cid = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    NWY_SDK_LOG_DEBUG("handl:%p, cid:%d", ftp_test_handl[cid-1],cid);
    if(nwy_ftp_logout(ftp_test_handl[cid-1]) !=NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_logout error");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_logout ok");
    }
}

void nwy_test_cli_ftp_get(void)
{
    int result = NWY_GEN_E_UNKNOWN;
    int type = 0;
    int len = 0;
    int offset = 0;
    char filename[128+1] = {0};
    char *sptr = NULL;
    int cid = 0;
    nwy_ftp_get_param_t ftp_get_param;
    memset(&ftp_get_param,0x00,sizeof(ftp_get_param));
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input type:(1 ASCII 2 Binary)");
    type = nwy_cli_str_to_int(sptr);
    if (type < 1 || type > 2)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcpy(filename, sptr);
    
    sptr = nwy_test_cli_input_gets("\r\n input offset:");
    offset = nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input len:");
    len = nwy_cli_str_to_int(sptr);
    NWY_SDK_LOG_DEBUG("handl:%p, cid:%d", ftp_test_handl[cid-1],cid);

    ftp_get_param.len = len;
    ftp_get_param.offset = offset;
    ftp_get_param.remotefile = filename;
    ftp_get_param.type = type;

    result = nwy_ftp_get(ftp_test_handl[cid-1],&ftp_get_param);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_get success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_get fail");
    }

}

void nwy_test_cli_ftp_put(void)
{
        int type = 0;
        int mode = 0;
        int len = 0;
        int result = NWY_GEN_E_UNKNOWN;
        char filename[128+1] = {0};
        char *sptr = NULL;
        char *data = NULL;
        int cid = 0;
        int eof;
        nwy_ftp_put_param_t ftp_put_param;
        sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
        cid = nwy_cli_str_to_int(sptr);
        if (cid < 1 || cid > 7)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }

        sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
        if(strlen(sptr)>128)
        {
            nwy_test_cli_echo("\r\ninput error");
            return;
        }
    
        strcpy(filename, sptr);
    
        sptr = nwy_test_cli_input_gets("\r\n input type:(1 ASCII 2 Binary)");
        type = nwy_cli_str_to_int(sptr);
        if (type < 1 || type > 2)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }

        sptr = nwy_test_cli_input_gets("\r\n input eof:(0 It's not over, 1 it's over)");
        eof = nwy_cli_str_to_int(sptr);
        if (eof < 0 || eof > 1)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }

        sptr = nwy_test_cli_input_gets("\r\n input mode:(1 STOR 2 APPE)");
        mode = nwy_cli_str_to_int(sptr);
        if (mode < 1 || mode > 2)
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }

        sptr = nwy_test_cli_input_gets("\r\n input put data len[1-8096]:");
        len = nwy_cli_str_to_int(sptr);

        if(len <1 || len > 8096)
        {
            nwy_test_cli_echo("\r\n input data len error!\r\n");
            return;
        }

        data = malloc(len+1);
        memset(data,0x00,len+1);
        nwy_cli_get_trans_data(data,len);

        memset(&ftp_put_param,0x00,sizeof(ftp_put_param));
        ftp_put_param.data = (unsigned char*)data;
        ftp_put_param.eof = (unsigned char)eof;
        ftp_put_param.len = len;
        ftp_put_param.mode = mode;
        ftp_put_param.remotefile = filename;
        ftp_put_param.type =type;

        result = nwy_ftp_put(ftp_test_handl[cid-1],&ftp_put_param);
        if(result == NWY_SUCCESS)
        {
            nwy_test_cli_echo("\r\n nwy_ftp_put ok\r\n");
        }
        else
        {
            nwy_test_cli_echo("\r\n nwy_ftp_put error\r\n");
        }
        free(data);
}

void nwy_test_cli_ftp_filesize(void)
{
    char *sptr = NULL;
    char buff[128+1] = {0};
    int result = NWY_GEN_E_UNKNOWN;
    int cid = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }


    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcpy(buff, sptr);

    result = nwy_ftp_get_filesize(ftp_test_handl[cid-1],buff);
    if (NWY_SUCCESS == result)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_filesize ok");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_filesize error");
    }

}

void nwy_test_cli_ftp_delete(void)
{
    nwy_error_e ret = NWY_SUCCESS;
    char *sptr = NULL;
    char file_name[128 + 1] = "";
    int cid = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    nwy_test_cli_echo("\r\n delete get file");
    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcat(file_name, sptr);

    ret =nwy_ftp_del_file(ftp_test_handl[cid-1], file_name);
    if (ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\n delete %s file success", file_name);
    }
    else
    {
        nwy_test_cli_echo("\r\n delete %s file fail", file_name);
    }

}

