#ifndef __NWY_LOC_API_H__
#define __NWY_LOC_API_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "nwy_common.h"


typedef enum
{
    NWY_LOC_HOT_START = 0,
    NWY_LOC_WARM_START = 1,
    NWY_LOC_COLD_START = 2,
    NWY_LOC_FACTORY_START = 3,
} nwy_loc_startup_mode_e;


typedef enum
{
    GNSS_OUTPUT_GGA  = 0,
    GNSS_OUTPUT_GSA  = 1,
    GNSS_OUTPUT_GSV  = 2,
    GNSS_OUTPUT_RMC  = 3,
    GNSS_OUTPUT_VTG  = 4,
    GNSS_OUTPUT_GLL  = 5,
    GNSS_OUTPUT_ZDA  = 6,
    GNSS_OUTPUT_ALL  = 99,
}nwy_loc_output_type_e;

typedef enum
{
    NWY_LOC_1HZ  = 0,
    NWY_LOC_2HZ  = 1,
    NWY_LOC_4HZ  = 2,
    NWY_LOC_5HZ  = 3,
    NWY_LOC_10HZ  = 4,
}nwy_loc_rate_e;

#define NWY_LOC_PARA_CNT 7
#define NWY_LOC_INFO_SIZE 150
#define NWY_LOC_MAX_PKT 20
#define NWY_GNSS_RX_BUFFER_SIZE   (4096)
//#define NWY_GNSS_RX_BUFFER_SIZE   (1024)

#define NWY_LOC_NMEA_MASK_GGA   (0x00000001) /**<  Enable GGA type  */
#define NWY_LOC_NMEA_MASK_GSA   (0x00000002) /**<  Enable GSA type  */
#define NWY_LOC_NMEA_MASK_GSV   (0x00000004) /**<  Enable GSV type  */
#define NWY_LOC_NMEA_MASK_RMC   (0x00000008) /**<  Enable RMC type  */
#define NWY_LOC_NMEA_MASK_VTG   (0x00000010) /**<  Enable VTG type  */
#define NWY_LOC_NMEA_MASK_GLL   (0x00000020) /**<  Enable GLL type  */
#define NWY_LOC_NMEA_MASK_ZDA   (0x00000040) /**<  Enable ZDA type  */
#define NWY_LOC_NMEA_MASK_GST   (0x00000080) /**<  Enable GST type  */
#define NWY_LOC_NMEA_MASK_ALL   (0x00000FFF) /**<  获取全部的NMEA数据*/


typedef struct
{
    uint16_t length;
    uint16_t type;
    uint8_t nmea_data[4096];
}nwy_gnss_output_info_ind_t;



typedef struct
{
    float longitude;
    float latitude;
    float altitude;
    uint8_t satellite_num;
    float speed;
    float course;
    uint16_t clockdrift;
    uint32_t m_nUTCTime;
    uint8_t m_nUncertaintySemiMajor;
    uint8_t m_nUncertaintySemiMinor;
    float m_nBearing;
    uint16_t m_nDirection;
    float m_nHorizontalVelocity;
    float m_nVerticalVelocity;
    uint32_t fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP;
    float m_nHDOP;
    float m_nVDOP;
    uint16_t m_nSatelliteIdentifier;
    uint16_t m_cn0;
    uint16_t m_nElevation;
    uint16_t m_nAzimuth;
    uint8_t m_IsUsed;
}nwy_gnss_nmea_info_t;


typedef enum
{
    NWY_GNSS_COORDINATE_SYS_NULL = 0,
    NWY_GNSS_COORDINATE_WGS84    = 1,  //GPS WGS-84
    NWY_GNSS_COORDINATE_PZ90     = 2,  //GLONASS PZ-90
    NWY_GNSS_COORDINATE_CGCS2000 = 3,  //BDS CGCS-2000
    NWY_GNSS_COORDINATE_SYS_MAX
}nwy_gnss_coordinate_sys_e;

typedef enum
{
    NWY_GNSS_FORMAT_NULL      = 0,
    NWY_GNSS_FORMAT_QCOM      = 1,    //qualcomm
    NWY_GNSS_FORMAT_BDS       = 2,    //BDS
    NWY_GNSS_FORMAT_ORIGINAL  = 3,    //Original
    NWY_GNSS_FORMAT_MAX
}nwy_gnss_bds_format_e;

typedef struct {
    int value;
}gnss_param_type_t;

typedef struct
{
    //Navigation coordinate system
    int coordinate_valid;
    gnss_param_type_t coordinate_sys;

    //Navigation cutoff angle, range: 5-15
    int angle_threshold_valid;
    gnss_param_type_t angle_threshold;

    //GNSS chip temperature
    int temperature_valid;
    gnss_param_type_t temperature_min;
    gnss_param_type_t temperature_max;

    //GNSS antenna configuration
    int antenna_valid;
    gnss_param_type_t antenna;

    //Beidou navigation header format
    int bds_header_valid;
    gnss_param_type_t bds_header_format;
}nwy_gnss_param_t;

typedef struct
{
    int set_flag;
    int data_type;
    char url[521+1];
    char user[128];
    char pass[128];
    char mid[64];
    int port;

    double lat;
    double lng;
    double alt;
}nwy_setserver_para_t;

typedef void (*nwy_loc_ind_event_func)(nwy_gnss_output_info_ind_t *ind_msg);
nwy_error_e nwy_loc_recv_cb_reg(nwy_loc_ind_event_func ind_handler);
nwy_error_e nwy_loc_recv_cb_del(void);
void nwy_loc_init(void);
nwy_error_e nwy_loc_navigation_start(void);
nwy_error_e nwy_loc_navigation_stop(void);
nwy_error_e  nwy_loc_navigation_reset(void);
nwy_error_e nwy_loc_mode_set (int mode);
nwy_error_e nwy_loc_startup_set(nwy_loc_startup_mode_e startup);
nwy_error_e nwy_loc_rate_set(nwy_loc_rate_e rate);
nwy_error_e nwy_loc_cfg_nmea_type(unsigned int nmea_type);
nwy_error_e nwy_loc_nmea_get(int type,char *data);
nwy_error_e nwy_loc_nmea_parse(nwy_gnss_nmea_info_t *locMsg);
nwy_error_e nwy_loc_agnss_start(nwy_setserver_para_t *server_info);
nwy_error_e nwy_loc_param_set(nwy_gnss_param_t *param); //currently not supported
nwy_error_e nwy_loc_param_get(nwy_gnss_param_t *param); //currently not supported

#ifdef __cplusplus
   }
#endif

#endif


