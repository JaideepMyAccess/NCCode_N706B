#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"



/**************************Location*********************************/
void nwy_test_cli_navigation_start()
{
    int ret = 0;
    ret = nwy_loc_navigation_start();
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGNSS navigation start fail ! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nGNSS navigation start success !\r\n");
    }
    return;
}

void nwy_test_cli_navigation_stop()
{
    int ret = 0;
    ret = nwy_loc_navigation_stop();
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nGNSS navigation stop fail ! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nGNSS navigation stop success !\r\n");
    }
    return;
}

void nwy_test_cli_loc_mode_set()
{
    int ret = 0;
    int mode = 0;
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nPlease input loc mode:\r\n0:GPS\r\n1:BD\r\n2:GPS_BD\r\n3:GPS_GLONASS\r\n4:GPS_GALILEO\r\n5:BD_GLONASS\r\n6:BD_GALILEO\r\n7:GLONASS_GALILEO\r\n8:GLONASS\r\n9:GALILEO\r\n10:GPS_BD_GLONASS\r\n11:GPS_BD_GALILEO\r\n12:GPS_GLONASS_GALILEO\r\n13:BD_GLONASS_GALILEO\r\n14:GPS_BD_GLONASS_GALILEO");

    mode = atoi(sptr);
    if(0 > mode || mode > 14)
    {
        nwy_test_cli_echo("\r\ninput loc mode not int range !\r\n");
        return;
    }
    ret = nwy_loc_mode_set(mode);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nLoc mode set fail! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nLoc mode set success !\r\n");
    }
    return;
}


void nwy_test_cli_startup_mode_set()
{
    int ret = 0;
    nwy_loc_startup_mode_e startup = NWY_LOC_HOT_START;
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\nset startup mode(0-hot 1-warm 2-cold):");

    startup = (nwy_loc_startup_mode_e)atoi(sptr);
    if(NWY_LOC_HOT_START > startup || startup > NWY_LOC_COLD_START)
    {
        nwy_test_cli_echo("\r\ninput startup mode not int range !\r\n");
        return;
    }
    ret = nwy_loc_startup_set(startup);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nstartup mode set fail! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nstartup mode set success !\r\n");
    }
    return;
}


void nwy_test_cli_loc_rate_set()
{
    int ret = 0;
    nwy_loc_rate_e rate = NWY_LOC_1HZ;

    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\ninput rate type:(0:1HZ 1:2HZ)");

    rate = atoi(sptr);

    ret = nwy_loc_rate_set(rate);
    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nloc_rate_set fail! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nloc_rate_set success !\r\n");
    }
    return;
}


void nwy_test_cli_nmea_data_get()
{
    int ret = 0;
    char nmea_data[2048] = {0};
    nwy_loc_output_type_e out_type = GNSS_OUTPUT_ZDA;
    char *sptr = NULL;

    sptr = nwy_test_cli_input_gets("\r\ninput nmea type(0-GG 1-GSA 2-GSV 3-RMC 4-VTG 5-GLL 6-ALL):");

    out_type = (nwy_loc_startup_mode_e)atoi(sptr);
    if(GNSS_OUTPUT_GGA > out_type || out_type > GNSS_OUTPUT_ZDA)
    {
        nwy_test_cli_echo("\r\ninput nmea type not int range !\r\n");
        return;
    }

    memset(nmea_data,0,2048);
    ret = nwy_loc_nmea_get(out_type, nmea_data);

    if (ret != NWY_SUCCESS)
    {
        nwy_test_cli_echo("\r\nnwy_loc_nmea_get_by_mask fail! ret:%d \r\n", ret);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\n%s \r\n",nmea_data);
    }
    return;
}


static void nwy_test_cli_loc_recv_output(nwy_gnss_output_info_ind_t *ind_msg)
{
    if(ind_msg == NULL)
    {
        nwy_test_cli_echo("\r\nrecv ind_msg is NULL!");
        return;
    }
    nwy_test_cli_echo("%s\n", ind_msg->nmea_data);
    return;
}

void nwy_test_cli_nmea_data_report_reg(void)
{
    int result = -1;
    result = nwy_loc_recv_cb_reg((nwy_loc_ind_event_func)nwy_test_cli_loc_recv_output);
    if(NWY_SUCCESS != result)
    {
        nwy_test_cli_echo("\r\nnwy_loc_recv_cb_reg fail:%d! \r\n", result);
        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nnwy_loc_recv_cb_reg success!");
    }

    return;
}

void nwy_test_cli_nmea_data_parse(void)
{
    int result = -1;
    nwy_gnss_nmea_info_t locMsg = {0};
    nwy_test_cli_echo("\r\n INTO nwy_loc_nmea_parse!");
    result = nwy_loc_nmea_parse(&locMsg);
    if(NWY_SUCCESS != result)
    {
        nwy_test_cli_echo("\r\nnwy_loc_nmea_parse fail:%d! \r\n", result);

        return;
    }
    else
    {
        nwy_test_cli_echo("\r\nnwy_loc_nmea_parse success!");
        nwy_test_cli_echo("\r\n locMsg.latitude:%f! \r\n", locMsg.latitude);
        nwy_test_cli_echo("\r\n locMsg.longitude:%f! \r\n", locMsg.longitude);
        nwy_test_cli_echo("\r\n locMsg.altitude:%f! \r\n", locMsg.altitude);
        nwy_test_cli_echo("\r\n locMsg.satellite_num:%d! \r\n", locMsg.satellite_num);
        nwy_test_cli_echo("\r\n locMsg.speed:%f! \r\n", locMsg.speed);
        nwy_test_cli_echo("\r\n locMsg.course:%f! \r\n", locMsg.course);
        nwy_test_cli_echo("\r\n locMsg.m_nUTCTime:%d! \r\n", locMsg.m_nUTCTime);
        nwy_test_cli_echo("\r\n locMsg.m_nUncertaintySemiMajor:%d! \r\n", locMsg.m_nUncertaintySemiMajor);
        nwy_test_cli_echo("\r\n locMsg.m_nUncertaintySemiMinor:%d! \r\n", locMsg.m_nUncertaintySemiMinor);
        nwy_test_cli_echo("\r\n locMsg.m_nBearing:%f! \r\n", locMsg.m_nBearing);
        nwy_test_cli_echo("\r\n locMsg.m_nDirection:%d! \r\n", locMsg.m_nDirection);
        nwy_test_cli_echo("\r\n locMsg.m_nHorizontalVelocity:%f! \r\n", locMsg.m_nHorizontalVelocity);
        nwy_test_cli_echo("\r\n locMsg.m_nVerticalVelocity:%f! \r\n", locMsg.m_nVerticalVelocity);
        nwy_test_cli_echo("\r\n locMsg.fix_flag:%d! \r\n", locMsg.fix_flag);
        nwy_test_cli_echo("\r\n locMsg.m_nHorizontalAccuracy:%f! \r\n", locMsg.m_nHorizontalAccuracy);
        nwy_test_cli_echo("\r\n locMsg.m_nPDOP:%f! \r\n", locMsg.m_nPDOP);
        nwy_test_cli_echo("\r\n locMsg.m_nHDOP:%f! \r\n", locMsg.m_nHDOP);
        nwy_test_cli_echo("\r\n locMsg.m_nVDOP:%f! \r\n", locMsg.m_nVDOP);
        nwy_test_cli_echo("\r\n locMsg.m_nSatelliteIdentifier:%d! \r\n", locMsg.m_nSatelliteIdentifier);
        nwy_test_cli_echo("\r\n locMsg.m_cn0:%d! \r\n", locMsg.m_cn0);
        nwy_test_cli_echo("\r\n locMsg.m_nElevation:%d! \r\n", locMsg.m_nElevation);
        nwy_test_cli_echo("\r\n locMsg.m_nAzimuth:%d! \r\n", locMsg.m_nAzimuth);
        nwy_test_cli_echo("\r\n locMsg.m_IsUsed:%d! \r\n", locMsg.m_IsUsed);
    }

    return;
}



void nwy_test_cli_agnss_start()
{
    int i = 0;
    int result = 0;
    int port = 0;
    int set_flag = 0;
    int len = 0;
    char *input_a =NULL;
    nwy_setserver_para_t  agps_info = {0};
    memset(&agps_info, 0, sizeof(nwy_setserver_para_t));

    input_a = nwy_test_cli_input_gets("please input AGPS server URL(neoway-api.rx-networks.cn)\n");
    len = strlen(input_a);
    if(len > 256)
    {
      nwy_test_cli_echo("\r\nAGPS server URL input error! length is less than 256 bytes\n");
      return ;
    }
    memcpy(agps_info.url, input_a, len);

    input_a = nwy_test_cli_input_gets("\r\nplease input AGPS server port(80)\n");
    port = atoi(input_a);
    if(port < 0 || port > 65535)
    {
      nwy_test_cli_echo("\r\nAGPS server port input error! port(0~65535)\n");
      return ;
    }
    agps_info.port = port;

    input_a = nwy_test_cli_input_gets("\r\nplease input AGPS server user(BaYp7H89p4)\n");
    len = strlen(input_a);
    if(len > 128)
    {
      nwy_test_cli_echo("\r\nAGPS server user input error! length is less than 128 bytes\n");
      return ;
    }
    memcpy(agps_info.user, input_a, len);


    input_a = nwy_test_cli_input_gets("\r\nplease input AGPS server user password(TlZSY2paNWtHN1NiTXJoUw==)\n");
    len = strlen(input_a);
    if(len > 64)
    {
      nwy_test_cli_echo("\r\nAGPS server user password input error! length is less than 64 bytes\n");
      return ;
    }
    memcpy(agps_info.pass, input_a, len);

    input_a = nwy_test_cli_input_gets("\r\nplease input AGPS server mId(unicore)\n");
    len = strlen(input_a);
    if(len > 64)
    {
        nwy_test_cli_echo("AGPS server mId input error! length is less than 64 bytes\n");
        return ;
    }
    memcpy(agps_info.mid, input_a, len);

    input_a = nwy_test_cli_input_gets("\r\nplease input AGPS server set_flag(0/1/2)\n");
    set_flag= atoi(input_a);
    agps_info.set_flag = set_flag;

    input_a = nwy_test_cli_input_gets("\r\nplease input gnss mode(0-GPS 1->BD 2->GPS+BD)\n");
    result = atoi(input_a);
    if(result < 0 || result > 2)
    {
      nwy_test_cli_echo("\r\ngnss mode input error!\n");
      return ;
    }
    agps_info.data_type = result;

    result = nwy_loc_agnss_start(&agps_info);
    if(result != 0)
    {
      nwy_test_cli_echo("\r\nFailed to get AGPS data!\n");
      return ;
    }
    else
    {
      nwy_test_cli_echo("\r\nAcquire AGPS data successfully\n");
    }

}


