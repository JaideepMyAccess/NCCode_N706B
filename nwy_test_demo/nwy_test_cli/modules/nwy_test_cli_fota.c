#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_fota_api.h"
#include "nwy_ftp_api.h"

nwy_ota_package_t g_ota_pack = {0};
char g_filename[256+1] = {0};
nwy_ftp_get_param_t g_ftp_get_param;
nwy_ftp_handle_t g_ftp_fota_handl = NULL;

void nwy_test_cli_fota_trans_proc(const char *data, int length)
{
    int ret;
    int percent1 = 0;
    int percent2 = 0;
    int len = length;
    
    if(g_ota_pack.offset+length > g_ota_pack.total_size)
        len = g_ota_pack.total_size-g_ota_pack.offset;
    
    memcpy(g_ota_pack.data+g_ota_pack.offset, data, len);
    g_ota_pack.offset += len;
    percent2 = (g_ota_pack.offset * 100) /g_ota_pack.total_size;
    if(percent2 - percent1 >= 1)
    {
        nwy_test_cli_echo("=");
        percent1 = percent2;
    }
    if(percent2 == 100)
    {
        nwy_test_cli_echo("\r\n send fota package end! \r\n");
    }
        
    if (g_ota_pack.offset >= g_ota_pack.total_size)
    {
        nwy_test_cli_sio_quit_trans_mode();
        nwy_test_cli_send_trans_end();
    }
}

void nwy_test_cli_fota_local_update(void)
{
    int ret, pkt_size;
    char *sptr = NULL;

    int percent1 = 0;
    int percent2 = 0;
    
    sptr = nwy_test_cli_input_gets("\r\n Please input firmware packet size:");
    pkt_size = atoi(sptr);
    if (pkt_size <= 0)
    {
        nwy_test_cli_echo("\r\n Fota Error : invalid packet size:%s", sptr);
        return;
    }
    nwy_test_cli_echo("\r\n Please input firmware:\r\n");
    memset(&g_ota_pack, 0, sizeof(g_ota_pack));
    g_ota_pack.total_size = pkt_size;
    g_ota_pack.len = pkt_size;
    g_ota_pack.offset = 0;
    g_ota_pack.data = malloc(pkt_size);
    if(g_ota_pack.data == NULL)
    {
        nwy_test_cli_echo("\r\n data malloc error:\r\n");
        return;
    }
    
    ret = nwy_test_cli_sio_enter_trans_mode(nwy_test_cli_fota_trans_proc);
    if(ret < 0)
    {
        nwy_test_cli_echo("\r\n enter trans mode error\r\n");
        free(g_ota_pack.data);
        return;
    }
    nwy_test_cli_wait_trans_end();

    g_ota_pack.offset = 0;
    ret = nwy_fota_write(&g_ota_pack);
    if(ret == NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\n fota write success\r\n");
        nwy_test_cli_echo("\r\n The fota package matches the current version\r\n");
    }
    else if(ret == NWY_FOTA_CHECK_FAILED)
    {
        nwy_test_cli_echo("\r\n The fota package does not matche the current version\r\n");
        free(g_ota_pack.data);
        return;
    }
    else if(ret == NWY_FOTA_WRITE_FAILED)
    {
        nwy_test_cli_echo("\r\n fota write fail\r\n");
        free(g_ota_pack.data);
        return;
    }
    else if(ret == NWY_FS_SPACE_NOT_ENOUGH)
    {
        nwy_test_cli_echo("\r\n file system space not enough\r\n");
        free(g_ota_pack.data);
        return;
    }
    else if(ret == NWY_GEN_E_INVALID_PARA)
    {
        nwy_test_cli_echo("\r\n function input parameters error\r\n");
        free(g_ota_pack.data);
        return;
    }
    else if(ret == NWY_GEN_E_MALLOC_FAIL)
    {
        nwy_test_cli_echo("\r\n failed to apply for memory.\r\n");
        free(g_ota_pack.data);
        return;
    }
    free(g_ota_pack.data);

    nwy_test_cli_echo("\r\n update start...\r\n");
    ret = nwy_fota_update(1);
    if(ret == NWY_FOTA_CHECK_FAILED)
    {
        nwy_test_cli_echo("\r\n fota package check fail\r\n");
    }
    else if(ret == NWY_FOTA_WRITE_FLAG_FAIL)
    {
        nwy_test_cli_echo("\r\n write fota update flag file fail\r\n");
    }
    return;
}

static void nwy_cli_ftp_fota_result_cb(nwy_ftp_result_t *param)
{
    int *size;
    int result = NWY_GEN_E_UNKNOWN;
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
            if(g_ftp_fota_handl != NULL)
                nwy_ftp_logout(g_ftp_fota_handl);
            break;
        case NWY_FTP_EVENT_LOGIN:
            nwy_test_cli_echo("\r\nFtp login success,cid:%d",cid);
            result = nwy_ftp_get_filesize(g_ftp_fota_handl, g_ftp_get_param.remotefile);
            if (NWY_SUCCESS == result)
            {
                nwy_test_cli_echo("\r\n nwy_ftp_get_filesize success");
            }
            else
            {
                nwy_ftp_logout(g_ftp_fota_handl);
                nwy_test_cli_echo("\r\n nwy_ftp_get_filesize fail");
            }
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
            if(g_ftp_fota_handl != NULL)
                nwy_ftp_logout(g_ftp_fota_handl);
            break;
        case NWY_FTP_EVENT_FILE_SIZE_ERROR:
            nwy_test_cli_echo("\r\nFtp file size error,cid:%d",cid);
            if(g_ftp_fota_handl != NULL)
                nwy_ftp_logout(g_ftp_fota_handl);
            break;
        case NWY_FTP_EVENT_DATA_SETUP_ERROR:
            nwy_test_cli_echo("\r\nFtp data setup error,cid:%d",cid);
            if(g_ftp_fota_handl != NULL)
                nwy_ftp_logout(g_ftp_fota_handl);
            break;
        case NWY_FTP_EVENT_SIZE:
            size = (int *)param->data;
            nwy_test_cli_echo("\r\nFtp size is %d,cid:%d", *size,cid);
            g_ota_pack.total_size = *size;
            g_ota_pack.offset = 0;
            result = nwy_ftp_get(g_ftp_fota_handl,&g_ftp_get_param);
            if (NWY_SUCCESS == result)
            {
                nwy_test_cli_echo("\r\n nwy_ftp_get success");
            }
            else
            {
                nwy_ftp_logout(g_ftp_fota_handl);
                nwy_test_cli_echo("\r\n nwy_ftp_get fail");
            }
            break;
        case NWY_FTP_EVENT_LOGOUT:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp logout,cid:%d",cid);
            if(g_ota_pack.offset == g_ota_pack.total_size && g_ota_pack.offset != 0)
            {
                nwy_test_cli_echo("\r\n update start...\r\n");
                result = nwy_fota_update(1);
                if(result == NWY_FOTA_CHECK_FAILED)
                {
                    nwy_test_cli_echo("\r\n fota package check fail\r\n");
                }
                else if(result == NWY_FOTA_WRITE_FLAG_FAIL)
                {
                    nwy_test_cli_echo("\r\n write fota update flag file fail\r\n");
                }
            }
            break;
        case NWY_FTP_EVENT_CLOSED:
            clean_flg = 1;
            nwy_test_cli_echo("\r\nFtp connection closed,cid:%d",cid);
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
                nwy_ftp_logout(g_ftp_fota_handl);
                return;
            }

            if (param->data_len != 0)
            {
                g_ota_pack.len = param->data_len;
                g_ota_pack.data = malloc(param->data_len);
                if(g_ota_pack.data == NULL)
                {
                    nwy_test_cli_echo("\r\n data malloc error:\r\n");
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }
                memcpy(g_ota_pack.data, param->data, param->data_len);
                result = nwy_fota_write(&g_ota_pack);
                if(result == NWY_FOTA_CHECK_FAILED)
                {
                    nwy_test_cli_echo("\r\n The fota package does not matche the current version\r\n");
                    free(g_ota_pack.data);
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }
                else if(result == NWY_FOTA_WRITE_FAILED)
                {
                    nwy_test_cli_echo("\r\n fota write fail\r\n");
                    free(g_ota_pack.data);
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }
                else if(result == NWY_FS_SPACE_NOT_ENOUGH)
                {
                    nwy_test_cli_echo("\r\n file system space not enough\r\n");
                    free(g_ota_pack.data);
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }
                else if(result == NWY_GEN_E_INVALID_PARA)
                {
                    nwy_test_cli_echo("\r\n function input parameters error\r\n");
                    free(g_ota_pack.data);
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }
                else if(result == NWY_GEN_E_MALLOC_FAIL)
                {
                    nwy_test_cli_echo("\r\n failed to apply for memory.\r\n");
                    free(g_ota_pack.data);
                    nwy_ftp_logout(g_ftp_fota_handl);
                    break;
                }

                free(g_ota_pack.data);
                g_ota_pack.offset += g_ota_pack.len;
            }
        }
        break;
        case NWY_FTP_EVENT_DATA_GET_FINISHED:
            nwy_test_cli_echo("\r\ncid:%d,get completed, total data size is :%d",cid,param->data_len);
            break;
        case NWY_FTP_EVENT_DATA_CLOSED:
            if(g_ftp_fota_handl != NULL)
                nwy_ftp_logout(g_ftp_fota_handl);
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
        g_ftp_fota_handl = NULL;
    }
    return;
}

void nwy_test_cli_ftp_fota_update()
{
    char *sptr = NULL;
    int s_nwy_ftp_choice = 0;
    int cid = 0;
    nwy_ftp_param_t ftp_param;
    nwy_ftp_ssl_cfg_t sslcfg;
    char host[255] = {0};
    char username[255] = {0};
    char pwd[255] = {0};
    
    memset(&sslcfg,0x00,sizeof(sslcfg));
    memset(&ftp_param, 0x00, sizeof(ftp_param));

    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    cid = nwy_cli_str_to_int(sptr);
    if (cid < 1 || cid > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    if(g_ftp_fota_handl != NULL)
    {
        nwy_test_cli_echo("\r\n ftp client exist ");
        return;
    }
    ftp_param.cid = cid;
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
    ftp_param.cb = nwy_cli_ftp_fota_result_cb;

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
        sptr = nwy_test_cli_input_gets("\r\n input filename(<=256):");
        if(strlen(sptr)>256)
        {
            nwy_test_cli_echo("\r\ninput error");
            return;
        }
        memset(g_filename, 0, sizeof(g_filename));
        strcpy(g_filename, sptr);
        memset(&g_ota_pack, 0, sizeof(g_ota_pack));

        g_ftp_fota_handl = nwy_ftp_login(&ftp_param,&sslcfg);
    }
    else
    {
        sptr = nwy_test_cli_input_gets("\r\n input filename(<=256):");
        if(strlen(sptr)>256)
        {
            nwy_test_cli_echo("\r\ninput error");
            return;
        }
        memset(g_filename, 0, sizeof(g_filename));
        strcpy(g_filename, sptr);
        memset(&g_ota_pack, 0, sizeof(g_ota_pack));
        g_ftp_fota_handl = nwy_ftp_login(&ftp_param,NULL);
    }

    g_ftp_get_param.len = 0;
    g_ftp_get_param.offset = 0;
    g_ftp_get_param.remotefile = g_filename;
    g_ftp_get_param.type = 2;

    NWY_SDK_LOG_DEBUG("handl:%p", g_ftp_fota_handl);
    if(g_ftp_fota_handl == NULL)
    {
        nwy_test_cli_echo("\r\nftp login fail\r\n");
    }
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

void nwy_test_cli_fota_result(void)
{
    int ret;
    int fota_result;

    ret = nwy_fota_result_get(&fota_result);
    if(ret == NWY_SUCCESS)
    {
        if(fota_result == NWY_FOTA_RESULT_SUCCESS)
        {
            nwy_test_cli_echo("\r\n fota success: %d\r\n", fota_result);
        }
        else if(fota_result == NWY_FOTA_RESULT_FAIL)
        {
            nwy_test_cli_echo("\r\n fota failed: %d\r\n", fota_result);
        }
        else
        {
            nwy_test_cli_echo("\r\n fota result none: %d\r\n", fota_result);
        }
    }
    else if(ret == NWY_GEN_E_PLAT_NOT_SUPPORT)
    {
        nwy_test_cli_echo("\r\n This feature is not support!\r\n");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_fota_result_get failed: %d\r\n", ret);
    }
}

void nwy_test_cli_fota_base_update(void)
{
    int ret, mode;
    char *sptr = NULL;
    nwy_ota_download_param_t ota_download_param;
    
    sptr = nwy_test_cli_input_gets("\r\n Please input download mode (0:http; 1:ftp):");
    mode = atoi(sptr);
    if (mode<0 || mode>1)
    {
        nwy_test_cli_echo("\r\n Fota Error : invalid download mode:%s", sptr);
        return;
    }
    ota_download_param.dl_mode = mode;
    if(mode == 0)
    {
        char url[256+1] = {0};
        
        sptr = nwy_test_cli_input_gets("\r\n Please input server url(<=256):");
        strncpy(url, sptr, 256);
        ota_download_param.http_param.url = url;
        sptr = nwy_test_cli_input_gets("\r\n Please input server port:");
        ota_download_param.http_param.port = atoi(sptr);
    }
    else 
    {
        char ip[256+1] = {0};
        char user[128+1] = {0};
        char pwd[128+1] = {0};
        char filename[128+1] = {0};
        int port;
        
        sptr = nwy_test_cli_input_gets("\r\n Please input server ip(<=256):");
        strncpy(ip, sptr, 256);
        sptr = nwy_test_cli_input_gets("\r\n Please input server port:");
        port = atoi(sptr);
        sptr = nwy_test_cli_input_gets("\r\n Please input user name(<=128):");
        strncpy(user, sptr, 128);
        sptr = nwy_test_cli_input_gets("\r\n Please input password(<=128):");
        strncpy(pwd, sptr, 128);
        sptr = nwy_test_cli_input_gets("\r\n Please input filename(<=128):");
        strncpy(filename, sptr, 128);
        
        ota_download_param.ftp_param.ip = ip;
        ota_download_param.ftp_param.port = port;
        ota_download_param.ftp_param.username = user;
        ota_download_param.ftp_param.password = pwd;
        ota_download_param.ftp_param.filename = filename;
    }
    ret = nwy_fota_base_ver_update(&ota_download_param);
    if(ret < 0)
    {
        nwy_test_cli_echo("\r\n nwy_fota_write_download_param error:%d\r\n", ret);
        return;
    }
}

