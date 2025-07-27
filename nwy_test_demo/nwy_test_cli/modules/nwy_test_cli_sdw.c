#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

/**************************BLE*********************************/
#ifdef NWY_OPEN_TEST_BLE_SERV
void nwy_test_cli_ble_open()
{
    int status;
    status = nwy_ble_enable();
    if (status)
    {
        nwy_test_cli_echo("\r\n Ble Opened Successfully!\r\n");
    }
}

void nwy_test_cli_ble_set_adv()
{
    int valu;
    char *input_valu;
    input_valu = nwy_test_cli_input_gets("\r\nset the adv enable(0-disable,1-enable): ");
    valu = atoi(input_valu);
    nwy_ble_set_adv(valu);
    if (valu == 1)
        nwy_test_cli_echo("\r\n %d->Ble Adv Open \r\n", valu);
    else
        nwy_test_cli_echo("\r\n %d->Ble Adv Close \r\n", valu);
}

void nwy_test_cli_ble_send()
{
    char data_str[1024] = {0};
    char *send_data = NULL;
    int status, value;
    char *input_value;
    input_value = nwy_test_cli_input_gets("\r\n ble send mode(0-notify,1-indify): ");
    value = atoi(input_value);
    send_data = nwy_test_cli_input_gets("\r\n send data:");
    strcpy(data_str, send_data);
    nwy_test_cli_echo("\r\nnwy_ble_send_data(%s) ", data_str);
    if (value == 0)
        status = nwy_ble_send_data(sizeof(data_str), data_str);
    else
        status = nwy_ble_send_indify_data(sizeof(data_str), data_str);
    if (status)
        nwy_test_cli_echo("\r\nBle Send Successfully!\r\n");
}

void nwy_ble_recv_data_func()
{
    char *length = NULL;
    char *precv = NULL;
    length = nwy_ble_receive_data(0);
    precv = nwy_ble_receive_data(1);
    if ((NULL != precv) & (NULL != length))
    {
        nwy_test_cli_echo("\r\n nwy_bel_receive_data:(%d),%s", length, precv);
    }
    else
    {
        nwy_test_cli_echo("nwy_ble receive data is null.");
    }
    nwy_ble_receive_data(2);

    return;
}

void nwy_test_cli_ble_recv()
{
    //nwy_ble_register_callback(nwy_ble_recv_data_func);
    nwy_ble_open_register_cb(nwy_ble_recv_data_func, NWY_BLE_WRITE_FLAG);
    nwy_test_cli_echo("\r\n nwy ble register callback OK");
}

void nwy_test_cli_ble_updata_connt()
{
    nwy_ble_update_conn(0, 60, 80, 4, 500);
}

void nwy_test_cli_ble_get_st()
{
    int status;
    status = nwy_read_ble_status();
    nwy_test_cli_echo("\r\nBle status:%d", (status ? 1 : 0));
}

void nwy_test_cli_ble_get_ver()
{
    char *dev_ver = NULL;
    dev_ver = nwy_ble_get_version();
    if (NULL != dev_ver)
    {
        nwy_test_cli_echo("\r\n nwy_ble_get_version:%s\r\n", dev_ver);
    }
    else
    {
        nwy_test_cli_echo("nwy_ble version data is null.");
    }
}

void nwy_test_cli_ble_set_dev_name()
{
    char *input_value;
    input_value = nwy_test_cli_input_gets("\r\n ble name: ");
    nwy_ble_set_device_name(input_value);
    nwy_test_cli_echo("\r\n new_set_dev_name:(%s) \r\n", input_value);
}

void nwy_test_cli_ble_set_beacon()
{
    int status;
    UINT8 uuid[16] = {0xb9, 0x40, 0x7f, 0x30, 0xf5, 0xf8, 0x46, 0x6e, 0xaf, 0xf9, 0x25, 0x55, 0x6b, 0x57, 0xfe, 0x6d};
    UINT8 major[2] = {0x00, 0x01};
    UINT8 minor[2] = {0x00, 0x02};

    status = nwy_ble_beacon(uuid, major, minor);
    if (status)
        nwy_test_cli_echo("\r\nSet Beacon parm,OK!\r\n");
}

void nwy_test_cli_ble_set_manufacture()
{
    int status;
    UINT8 Manufacture_Data[8] = {0x13, 0xff, 0};
    status = nwy_set_manufacture(Manufacture_Data);
    if (status)
        nwy_test_cli_echo("\r\nSet manufacture parm,OK!\r\n");
}

void nwy_test_cli_ble_set_adv_server_uuid()
{
    int status;
    UINT8 adv_uuid[2] = {0x13, 0xff};
    status = nwy_set_adv_server_uuid(adv_uuid);
    if (status)
        nwy_test_cli_echo("\r\nSet adv server uuid ok!\r\n");
}

void nwy_test_cli_ble_set_srv()
{
    UINT8 srv_uuid[16] = {0xf5, 0x89, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x10, 0xfe, 0x00, 0x00};
    if (!nwy_ble_set_service(srv_uuid))
        nwy_test_cli_echo("\r\n BLE HAS STARTED !\r\n");
}

void nwy_test_cli_ble_set_char()
{
    char *input_valu;
    UINT8 char_uuid_1[16] = {0xf5, 0x89, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x11, 0xfe, 0x00, 0x00};
    UINT8 char_uuid_2[16] = {0xf5, 0x89, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0xfe, 0x00, 0x00};

    UINT8 char_prop;
    nwy_test_cli_echo("\r\n char_prop: 0:Write 1:Write | Notify | indify\r\n");
    input_valu = nwy_test_cli_input_gets("char index 0 prop:");

    char_prop = atoi(input_valu);
    if (!nwy_ble_set_character(0, char_uuid_1, char_prop))
        nwy_test_cli_echo("\r\n BLE HAS STARTED !\r\n");

    nwy_test_cli_echo("\r\n char_prop: 0:Read 1:Read | Notify | indify\r\n");
    input_valu = nwy_test_cli_input_gets("char index 1 prop:");
    char_prop = atoi(input_valu);
    if (!nwy_ble_set_character(1, char_uuid_2, char_prop))
        nwy_test_cli_echo("\r\n BLE HAS STARTED !\r\n");
}

void nwy_ble_conn_status_func()
{
    int conn_status = 0;
    conn_status = nwy_ble_get_conn_status();
    if (conn_status != 0)
    {
        nwy_test_cli_echo("\r\nBle connect,status:%d", conn_status);
    }
    else
    {
        nwy_test_cli_echo("\r\nBle disconnect,status:%d", conn_status);
    }
}

void nwy_test_cli_ble_conn_status_report()
{
    nwy_ble_conn_status_cb(nwy_ble_conn_status_func);
}

void nwy_test_cli_ble_conn_status()
{
    int conn_status = 0;
    conn_status = nwy_ble_get_conn_status();
    nwy_test_cli_echo("\r\nnwy_ble_conn_status:%d \r\n", conn_status);
}

void nwy_test_cli_ble_mac_addr()
{
    char *ble_mac = NULL;
    ble_mac = nwy_ble_get_mac_addr();
    if (NULL != ble_mac)
    {
        nwy_test_cli_echo("\r\nBle mac addr:%02x:%02x:%02x:%02x:%02x:%02x", ble_mac[5], ble_mac[4], ble_mac[3], ble_mac[2], ble_mac[1], ble_mac[0]);
    }
    else
    {
        nwy_test_cli_echo("\r\nble get mac addr is null.");
    }
}

void nwy_test_cli_ble_add_server()
{
    uint8_t uuid0[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x89, 0x00, 0x00};
    uint8_t uuid1[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xe1, 0xff, 0x00, 0x00};
    uint8_t uuid2[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xe2, 0xff, 0x00, 0x00};

    nwy_ble_service_info ser_info_0;
    nwy_ble_service_info ser_info_1;
    nwy_ble_service_info ser_info_2;

    ser_info_0.ser_index = 1;
    ser_info_1.ser_index = 2;
    ser_info_2.ser_index = 3;

    memcpy(ser_info_0.ser_uuid, uuid0, 16);
    memcpy(ser_info_1.ser_uuid, uuid1, 16);
    memcpy(ser_info_2.ser_uuid, uuid2, 16);

    ser_info_0.char_num = 2;
    ser_info_1.char_num = 2;
    ser_info_2.char_num = 2;

    ser_info_0.p = 1;
    ser_info_1.p = 1;
    ser_info_2.p = 1;

    if (!nwy_ble_add_service(&ser_info_0))
        nwy_test_cli_echo("\r\nadd ble service 1 error");
    if (!nwy_ble_start_service(1))
        nwy_test_cli_echo("\r\nstart ble service 1 error");

    if (!nwy_ble_add_service(&ser_info_1))
        nwy_test_cli_echo("\r\nadd ble service 2 error");
    if (!nwy_ble_start_service(2))
        nwy_test_cli_echo("\r\nstart ble service 2 error");

    if (!nwy_ble_add_service(&ser_info_2))
        nwy_test_cli_echo("\r\nadd ble service 3 error");
    if (!nwy_ble_start_service(3))
        nwy_test_cli_echo("\r\nstart ble service 3 error");
}

void nwy_test_cli_ble_add_char()
{
    int sel;
    char *input_valu;
    uint8_t uuid0[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0x89, 0x00, 0x00};
    uint8_t uuid1[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x02, 0x89, 0x00, 0x00};
    uint8_t uuid2[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x03, 0x89, 0x00, 0x00};
    uint8_t uuid3[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x04, 0x89, 0x00, 0x00};
    uint8_t uuid4[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x05, 0x89, 0x00, 0x00};

    nwy_ble_char_info char_info_0;
    nwy_ble_char_info char_info_1;
    nwy_ble_char_info char_info_2;
    nwy_ble_char_info char_info_3;
    nwy_ble_char_info char_info_4;

    char_info_0.ser_index = 1;
    char_info_1.ser_index = 1;
    char_info_2.ser_index = 1;
    char_info_3.ser_index = 1;
    char_info_4.ser_index = 1;

    char_info_0.char_index = 0;
    char_info_1.char_index = 1;
    char_info_2.char_index = 2;
    char_info_3.char_index = 3;
    char_info_4.char_index = 4;

    memcpy(char_info_0.char_uuid, uuid0, 16);
    memcpy(char_info_1.char_uuid, uuid1, 16);
    memcpy(char_info_2.char_uuid, uuid2, 16);
    memcpy(char_info_3.char_uuid, uuid3, 16);
    memcpy(char_info_4.char_uuid, uuid4, 16);

    //  nwy_test_cli_echo("\r\npermisssion: 0:read 1:write 2:read | write");
    //  input_valu = nwy_test_cli_input_gets("\r\nchar_info_0.char_per:");
    //  sel = atoi(input_valu);
    char_info_0.char_per = 2;
    //  input_valu = nwy_test_cli_input_gets("\r\nchar_info_1.char_per:");
    //  sel = atoi(input_valu);
    char_info_1.char_per = 2;
    //  input_valu = nwy_test_cli_input_gets("\r\nchar_info_2.char_per:");
    //  sel = atoi(input_valu);
    char_info_2.char_per = 2;
    char_info_3.char_per = 2;
    char_info_4.char_per = 2;

    char_info_0.char_des = 1;
    char_info_1.char_des = 1;
    char_info_2.char_des = 1;
    char_info_3.char_des = 1;
    char_info_4.char_des = 1;

    nwy_test_cli_echo("\r\nproperties: 0:write 1:read 2:notify 3:indify 4:write | notify 5:read | notify 6:all");
    input_valu = nwy_test_cli_input_gets("\r\nchar_info_0.char_cp:");
    sel = atoi(input_valu);
    char_info_0.char_cp = sel;
    input_valu = nwy_test_cli_input_gets("\r\nchar_info_1.char_cp:");
    sel = atoi(input_valu);
    char_info_1.char_cp = sel;
    input_valu = nwy_test_cli_input_gets("\r\nchar_info_2.char_cp:");
    sel = atoi(input_valu);
    char_info_2.char_cp = sel;
    input_valu = nwy_test_cli_input_gets("\r\nchar_info_3.char_cp:");
    sel = atoi(input_valu);
    char_info_3.char_cp = sel;
    input_valu = nwy_test_cli_input_gets("\r\nchar_info_4.char_cp:");
    sel = atoi(input_valu);
    char_info_4.char_cp = sel;

    if (!nwy_add_ble_charcter(&char_info_0))
        nwy_test_cli_echo("\r\nadd ble charcter 0 error");

    if (!nwy_add_ble_charcter(&char_info_1))
        nwy_test_cli_echo("\r\nadd ble charcter 1 error");

    if (!nwy_add_ble_charcter(&char_info_2))
        nwy_test_cli_echo("\r\nadd ble charcter 2 error");

    if (!nwy_add_ble_charcter(&char_info_3))
        nwy_test_cli_echo("\r\nadd ble charcter 3 error");

    if (!nwy_add_ble_charcter(&char_info_4))
        nwy_test_cli_echo("\r\nadd ble charcter 4 error");
}

void nwy_ble_add_recv_data_func()
{
    nwy_ble_recv_info recv_data;
    nwy_ble_add_recv_data(&recv_data);
    nwy_test_cli_echo("\r\nrecv_data info:srv_id=%d,char_id=%d", recv_data.ser_id, recv_data.char_id);
    nwy_test_cli_echo("\r\nrecv_data:(%d):%s", recv_data.datalen, recv_data.data);

    return;
}

void nwy_test_cli_ble_add_recv_data()
{
    int ret = 0;
    ret = nwy_ble_open_register_cb(nwy_ble_add_recv_data_func, NWY_BLE_ADD_WRITE_FLAG);
    if (ret)
        nwy_test_cli_echo("\r\n recv data register ok");
    else
        nwy_test_cli_echo("\r\n recv data register error");
}
void nwy_test_cli_ble_add_send_data()
{
    char *send_data = NULL;
    int sel;
    char *input_valu;
    int status;
    nwy_ble_send_info ble_send_data;

    input_valu = nwy_test_cli_input_gets("\r\nser_id:");
    sel = atoi(input_valu);
    ble_send_data.ser_id = sel;

    input_valu = nwy_test_cli_input_gets("\r\nchar_id:");
    sel = atoi(input_valu);
    ble_send_data.char_id = sel;

    input_valu = nwy_test_cli_input_gets("\r\nsend mode(0-notify,1-indify):");
    sel = atoi(input_valu);
    ble_send_data.op = sel;

    send_data = nwy_test_cli_input_gets("\r\n send data:");
    strcpy((char *)ble_send_data.data, send_data);
    ble_send_data.datalen = strlen((char *)ble_send_data.data);
    nwy_test_cli_echo("\r\nnwy_ble_send_data(%d)%s ", ble_send_data.datalen, ble_send_data.data);
    status = nwy_ble_add_send_data(&ble_send_data);
    if (status)
        nwy_test_cli_echo("\r\nBle Send Successfully!\r\n");
}

void nwy_test_cli_ble_disconnect()
{
    if (nwy_ble_disconnect())
        nwy_test_cli_echo("\r\nBle disconnect ok!\r\n");
    else
        nwy_test_cli_echo("\r\nBle disconnect error!\r\n");
}

void nwy_ble_read_info_func()
{
    nwy_ble_read_info read_info;

    nwy_ble_read_req_info(&read_info);
    nwy_test_cli_echo("\r\nread_req info:srv_id=%d,char_id=%d,rw_flag=%d", read_info.ser_id, read_info.char_id, read_info.rw_flag);

    return;
}
void nwy_test_cli_ble_read_req()
{
    int ret = 0;
    ret = nwy_ble_open_register_cb(nwy_ble_read_info_func, NWY_BLE_READ_FLAG);
    if (ret)
        nwy_test_cli_echo("\r\n read info register ok");
    else
        nwy_test_cli_echo("\r\n read info register error");
}

void nwy_test_cli_ble_read_rsp()
{
    char *send_data = NULL;
    int sel;
    char *input_valu;
    nwy_ble_read_rsp read_rsp;

    input_valu = nwy_test_cli_input_gets("\r\nser_id:(0~1)");
    sel = atoi(input_valu);
    read_rsp.ser_id = sel;

    input_valu = nwy_test_cli_input_gets("\r\nchar_id:(ser_id:0,0~1;ser_id:1,0~4)");
    sel = atoi(input_valu);
    read_rsp.char_id = sel;

    input_valu = nwy_test_cli_input_gets("\r\ndata_len:");
    sel = atoi(input_valu);
    read_rsp.rsp_len = sel;

    send_data = nwy_test_cli_input_gets("\r\nsend_data:");
    memcpy(read_rsp.read_rsp, send_data, read_rsp.rsp_len);
    nwy_test_cli_echo("\r\nnwy_read_rsp_data:%s ", read_rsp.read_rsp);

    if (!nwy_ble_read_rsp_info(&read_rsp))
        nwy_test_cli_echo("\r\nnwy read rsp parm error");
}

void nwy_test_cli_ble_close()
{
    nwy_ble_disable();
    nwy_test_cli_echo("\r\nBle Closed Successfully!\r\n");
}
#endif

/**************************BLE Client*********************************/
#ifdef NWY_OPEN_TEST_BLE_CLI
void nwy_test_cli_ble_client_set_enable()
{
    char *input_valu;
    uint32_t sel = 0;

    nwy_test_cli_echo("\r\nset ble client open and close:1->open,0->close\r\n");
    input_valu = nwy_test_cli_input_gets("select:");
    sel = atoi(input_valu);
    if (nwy_ble_client_set_enable(sel))
        nwy_test_cli_echo("Ble Client Open");
    else
        nwy_test_cli_echo("Ble Client Close");
}

bool scan_dev_flag = true;
void nwy_ble_client_scan_dev_func()
{
    nwy_ble_c_scan_dev scan_info;
    nwy_ble_client_scan_result(&scan_info);
    nwy_test_cli_echo("\r\n%02x:%02x:%02x:%02x:%02x:%02x,%d,%s",
                      scan_info.bdAddress.addr[0],
                      scan_info.bdAddress.addr[1],
                      scan_info.bdAddress.addr[2],
                      scan_info.bdAddress.addr[3],
                      scan_info.bdAddress.addr[4],
                      scan_info.bdAddress.addr[5],
                      scan_info.addr_type,
                      scan_info.name);
}
void nwy_test_cli_ble_client_scan()
{
    char *input_valu;
    int status;
    uint8_t scan_time = 0;
    if (scan_dev_flag)
    {
        scan_dev_flag = false;
        if (nwy_ble_client_register_cb(nwy_ble_client_scan_dev_func, BLE_CLIENT_SCAN_DEV))
            nwy_test_cli_echo("nwy ble client scan dev cb register ok");
    }
    input_valu = nwy_test_cli_input_gets("\r\nsacn time:");
    scan_time = atoi(input_valu);
    nwy_test_cli_echo("\r\nnwy ble client scanning");
    status = nwy_ble_client_scan(scan_time);
    if (status)
        nwy_test_cli_echo("\r\nnwy ble client scan end");
}

void nwy_test_cli_ble_client_connect()
{
    char data_str[18] = {0};
    char *mac_addr = NULL;
    int status;
    char *input_valu;
    uint32_t sel = 0;

    input_valu = nwy_test_cli_input_gets("\r\naddr type:");
    sel = atoi(input_valu);
    mac_addr = nwy_test_cli_input_gets("\r\n mac addr:");
    strcpy(data_str, mac_addr);
    nwy_test_cli_echo("\r\nnwy_ble_send_data(%s) ", data_str);
    status = nwy_ble_client_connect(sel, data_str);
    if (status)
        nwy_test_cli_echo("\r\nBle Client Conn Set Parm OK!\r\n");
    else
        nwy_test_cli_echo("\r\nBle Client Conn Set Parm Error!\r\n");
}

void nwy_test_cli_ble_client_disconnect()
{
    int status;
    status = nwy_ble_client_disconnect();
    if (status)
        nwy_test_cli_echo("\r\nBle Client Disconnect OK!\r\n");
    else
        nwy_test_cli_echo("\r\nBle Client Disconnect Error!\r\n");
}

int srv_num;
void nwy_test_cli_ble_client_discover_srv()
{
    nwy_ble_c_discover srv_info[20];
    int i;
    srv_num = nwy_ble_client_discover_srv(srv_info);
    if (srv_num)
        for (i = 0; i < srv_num; i++)
            nwy_test_cli_echo("\r\nser_id:%d,0x%04x", i, srv_info[i].uuid);
}

void nwy_test_cli_ble_client_discover_char()
{
    nwy_ble_c_discover char_info[10];
    nwy_ble_client_discover_char(char_info);
    uint8_t char_num;
    int i, j;

    nwy_test_cli_echo("\r\ndiscover_char printf format:srv_id,char_id,char uuid,prop");
    for (i = 0; i < srv_num; i++)
    {
        char_num = char_info[i].charNum;
        for (j = 0; j < char_num; j++)
        {
            nwy_test_cli_echo("\r\n%d,%d,0x%04x,0x%02x", i, j, char_info[i].discover_char[j].uuid,
                              char_info[i].discover_char[j].properties);
        }
    }
}

void nwy_test_cli_ble_client_send_data()
{
    int sel;
    char *input_valu;
    int status;
    nwy_ble_c_send_param ble_send_data;

    input_valu = nwy_test_cli_input_gets("\r\nser_id:");
    sel = atoi(input_valu);
    ble_send_data.ser_id = sel;

    input_valu = nwy_test_cli_input_gets("\r\nchar_id:");
    sel = atoi(input_valu);
    ble_send_data.char_id = sel;

    ble_send_data.data = (uint8_t *)nwy_test_cli_input_gets("\r\n send data:");
    nwy_test_cli_echo("\r\nnwy_ble_send_data ");
    ble_send_data.len = strlen((char *)ble_send_data.data);
    nwy_test_cli_echo("\r\nnwy_ble_send_data(%d)%s ", ble_send_data.len, ble_send_data.data);
    status = nwy_ble_client_send_data(&ble_send_data);
    if (status)
        nwy_test_cli_echo("\r\nBle Send Successfully!\r\n");
}

void nwy_ble_client_recv_data_func()
{
    nwy_ble_c_recv_info recv_data;
    nwy_ble_client_recv_data(&recv_data);
    nwy_test_cli_echo("\r\nrecv data:%d,%d,[%d]%s\r\n", recv_data.ser_id, recv_data.char_id, recv_data.len, recv_data.data);
}

void nwy_test_cli_ble_client_recv_data()
{
    if (nwy_ble_client_register_cb(nwy_ble_client_recv_data_func, BLE_CLIENT_RECV_DATA))
        nwy_test_cli_echo("nwy ble client recv data register ok");
}
#endif
/**************************WIFI*********************************/
void nwy_cli_test_wifi_get_st()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_enable()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_set_work_md()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_set_ap_para()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_set_ap_para_adv()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_get_clit_info()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_sta_scan()
{
    int i = 0;
    nwy_wifi_scan_list_t scan_list;

    memset(&scan_list, 0, sizeof(scan_list));

    nwy_wifi_scan(&scan_list);

    for (i = 0; i < scan_list.num; i++)
    {	nwy_test_cli_echo("\r\nwifissid = %s", scan_list.ap_list[i].ssid);
     	nwy_test_cli_echo("	wifi ap mac is %02x:%02x:%02x:%02x:%02x:%02x",
                          scan_list.ap_list[i].mac[5], scan_list.ap_list[i].mac[4], scan_list.ap_list[i].mac[3], scan_list.ap_list[i].mac[2],
                          scan_list.ap_list[i].mac[1], scan_list.ap_list[i].mac[0]);
        nwy_test_cli_echo("	channel = %d", scan_list.ap_list[i].channel);
        nwy_test_cli_echo("	rssi = %d", scan_list.ap_list[i].rssival);
    }
    if (i == 0)
    {
        nwy_test_cli_echo("\r\n Wifi scan nothing");
    }
}

void nwy_cli_test_wifi_sta_scan_ret()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_sta_connt()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_sta_disconnt()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_sta_get_hostpot_info()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_cli_test_wifi_disable()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}
