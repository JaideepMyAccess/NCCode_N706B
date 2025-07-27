/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_network_v2.h
 * Author       : huangsangen
 * Created      : 2023-5-16
 * Description  : network API
 ******************************************************************************/

#ifndef NWY_NW_API_H
#define NWY_NW_API_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * 1 Other Header File Including
 ******************************************************************************/
#include "nwy_common.h"
#include "nwy_sim_api.h"



/******************************************************************************
 * 2 Macro Definition
******************************************************************************/
#define NWY_NW_LOCK_BAND_MAX_NUM  20
#define NWY_NW_LOCK_FREQ_MAX_NUM  8
#define NWY_NW_LOCK_PCI_MAX_NUM  8
#define NWY_NW_PRI_BAND_MAX_NUM  8
#define NWY_NW_LONG_EONS_MAX_LEN 32
#define NWY_NW_SHORT_EONS_MAX_LEN 16
#define NWY_NW_MNC_MCC_LEN   3
#define NWY_NW_SCAN_LIST_MAX_NUM 40
#define NWY_NW_SPN_LEN_MAX    32
#define NWY_NW_CDMA_POSITION_MAX 10
#define NWY_NW_NBR_CELLS_MAX_NUM 8
#define NWY_NW_SUPPORTED_BAND_MAX_NUM  20
#define NWY_NW_SUPPORTED_RAT_MAX 10 //GSM/WCDMA/LTE/CAT-M/NB/NR/CDMA/HDR/TD-SCDMA/RESERVED FOR 6G
#define NWY_NW_PLMN_LIST_MAX 20
#define NWY_AT_MAX_PARA_STR 20
#define NWY_NW_BLACKCELL_MAX_NUM  10

/******************************************************************************
 * 3 Enum Type Definition
******************************************************************************/
typedef enum
{
    NWY_TIME_ZONE_DISABLE = 0,
    NWY_TIME_ZONE_ENABLE = 1
}nwy_time_zone_switch_e;

typedef enum {
    NWY_NW_MODE_MASK_AUTO = 0x0,//unlocked mode
    NWY_NW_MODE_MASK_CDMA_1X   = 0x1,
    NWY_NW_MODE_MASK_CDMA_HDR   = 0x2,
    NWY_NW_MODE_MASK_GSM  = 0x4,
    NWY_NW_MODE_MASK_WCDMA = 0x8,
    NWY_NW_MODE_MASK_LTE = 0x10,//EUTRAN, Not included CATM1 and NB
    NWY_NW_MODE_MASK_TDSCDMA = 0x20,
    NWY_NW_MODE_MASK_SA = 0x40, //SA
    NWY_NW_MODE_MASK_CATM = 0x80,
    NWY_NW_MODE_MASK_NB = 0x100,
    NWY_NW_MODE_MASK_GSM_WCDMA,
    NWY_NW_MODE_MASK_GSM_LTE,
    NWY_NW_MODE_MASK_WCDMA_LTE,
    NWY_NW_MODE_MASK_GSM_WCDMA_LTE,
    NWY_NW_MODE_MASK_GSM_SA, //GSM+SA
    NWY_NW_MODE_MASK_WCDMA_SA,//WCDMA+SA
    NWY_NW_MODE_MASK_LTE_SA_NSA,//LTE+SA+NSA
    NWY_NW_MODE_MASK_WCDMA_LTE_SA_NSA, //WCDMA+LTE+SA+NSA
    NWY_NW_MODE_MASK_GSM_LTE_SA_NSA,//GSM+LTE+SA+NSA
    NWY_NW_MODE_MASK_NSA,//LTE+NSA
    NWY_NW_MODE_MASK_LTE_SA_WCDMA,
    NWY_NW_MODE_MASK_MAX
} nwy_nw_mode_mask_e;

typedef enum {
    NWY_NW_RAT_NONE   = 0, //not reg status
    NWY_NW_RAT_GSM   = 1,
    NWY_NW_RAT_WCDMA  = 2,
    NWY_NW_RAT_LTE = 3, //EUTRAN, Not included CATM1 and NB
    NWY_NW_RAT_CATM = 4,
    NWY_NW_RAT_NBIoT = 5,
    NWY_NW_RAT_NR = 6,
    NWY_NW_RAT_CDMA = 7,
    NWY_NW_RAT_HDR = 8,
    NWY_NW_RAT_TDSCDMA = 9,
    NWY_NW_RAT_MAX = 0xff
} nwy_nw_rat_type_e;

typedef enum {
    NWY_NW_NETINFO_AUTO = 1,//auto mode
    NWY_NW_NETINFO_2G   = 2,
    NWY_NW_NETINFO_3G   = 3,
    NWY_NW_NETINFO_4G   = 4,
    NWY_NW_NETINFO_2G_3G = 5,
    NWY_NW_NETINFO_2G_4G = 6,
    NWY_NW_NETINFO_3G_4G = 7,
    NWY_NW_NETINFO_2G_3G_4G = 8, //SA
    NWY_NW_NETINFO_5G = 9,
    NWY_NW_NETINFO_2G_5G = 10,
    NWY_NW_NETINFO_3G_5G = 11,
    NWY_NW_NETINFO_4G_5G = 12,
    NWY_NW_NETINFO_2G_4G_5G = 13,
    NWY_NW_NETINFO_3G_4G_5G = 14,
    NWY_NW_NETINFO_NSA = 15,
    NWY_NW_NETINFO_MAX
} nwy_netinfo_mode_e;

typedef enum {
    NWY_NW_LOCK_MODE_UNLOCK = 0,     //UNLOCK all
    NWY_NW_LOCK_MODE_LOCK_BAND  = 1, //lock band
    NWY_NW_LOCK_MODE_LOCK_FREQ  = 2, //lock freq
    NWY_NW_LOCK_MODE_LOCK_PCI = 3,    //lock cell
    NWY_NW_LOCK_MODE_UNLOCK_BAND = 4,
    NWY_NW_LOCK_MODE_UNLOCK_FREQ = 5,
    NWY_NW_LOCK_MODE_UNLOCK_PCI = 6
} nwy_nw_lockmode_e;

typedef enum {
    NWY_NW_PRI_BAND_UNLOCK = 0,
    NWY_NW_PRI_BAND_LOCK  = 1,
    NWY_NW_PRI_BAND_MAX
} nwy_nw_priband_mode_e;

typedef enum {
    NWY_NW_RADIO_MIN_MODE = 0,      //min mode, close rf/sim
    NWY_NW_RADIO_NORMAL_MODE = 1,      //normal mode, open rf and sim
    NWY_NW_RADIO_FLIGHT_MODE = 4,   //flight mode, close rf
} nwy_nw_radio_mode_e;

typedef enum {
    NWY_NW_EDRX_MODE_DISABLE = 0,
    NWY_NW_EDRX_MODE_ENABLE  = 1,
    NWY_NW_EDRX_MODE_ENABLE_REPORT  = 2,
    NWY_NW_EDRX_MODE_RESET_SETTING  = 3,
} nwy_nw_edrx_mode_e;

typedef enum {
    NWY_NW_PSM_MODE_DISABLE = 0,
    NWY_NW_PSM_MODE_ENABLE  = 1,

    /*mode reset: Disable the use of PSM and discard all parameters for PSM;
    if available, reset to the manufacturer specific default values.In this form, the read
    parameter <mode> will be set to 0.*/
    NWY_NW_PSM_MODE_RESET   = 2,
} nwy_nw_psm_mode_e;

typedef enum
{
    NWY_NW_NITZ = 1,
    NWY_NW_PSM_MODE = 2,
    NWY_NW_IMS_STATE = 3,
} nwy_nw_variable_mode_e;

typedef enum {
    NWY_NW_CONFIG_RW_NETAUTO = 0,   //option for open/close netauto;supoort read/write
    NWY_NW_CONFIG_RW_UEMODE  = 1,   //option for set uemode;supoort read/write
    NWY_NW_CONFIG_RW_DATAONLY  = 2, //option for open/close dataonly;supoort read/write
    NWY_NW_CONFIG_RW_ANTAUX  = 3,   //option for open/close antaux;supoort read/write
    NWY_NW_CONFIG_R_BAND_CAP =4,    //option for read band capability;supoort read
    NWY_NW_CONFIG_RW_NITZ_ONOFF = 5,//option for open/close nitz;supoort read/write
} nwy_nw_config_type_e;

typedef enum {
    NWY_NW_SPN_ENC_GSM7    = 0x00,   /* 7bit code */
    NWY_NW_SPN_ENC_UCS2    = 0x01,   /* UCS2 code */
}nwy_nw_spn_enc_e;

typedef enum {
    NWY_NW_SERVICE_NONE     = 0,    //out of service
    NWY_NW_SERVICE_LIMITED  = 1,    //limited service,only for emergency call,ex.110.119.120
    NWY_NW_SERVICE_FULL     = 2,
}nwy_nw_service_type_e;

typedef enum {
    NWY_NW_ROAM_STATE_OFF = 0,
    NWY_NW_ROAM_STATE_ON = 1,
}nwy_nw_roam_state_e;
    
typedef enum {
   NWY_NW_SRV_DOMAIN_OOS = 0,
   NWY_NW_SRV_DOMAIN_CS = 1,
   NWY_NW_SRV_DOMAIN_PS = 2,
   NWY_NW_SRV_DOMAIN_CS_PS = 3,
}nwy_nw_srv_domain_e;

typedef enum {
    NWY_BANDWIDTH_ENUM_TYPE_MIN = -1,
    NWY_LTE_BW_NRB_6_V01 = 0, /**<  LTE 1.4 MHz bandwidth \n  */
    NWY_LTE_BW_NRB_15_V01 = 1, /**<  LTE 3 MHz bandwidth \n  */
    NWY_LTE_BW_NRB_25_V01 = 2, /**<  LTE 5 MHz bandwidth \n  */
    NWY_LTE_BW_NRB_50_V01 = 3, /**<  LTE 10 MHz bandwidth \n  */
    NWY_LTE_BW_NRB_75_V01 = 4, /**<  LTE 15 MHz bandwidth \n  */
    NWY_LTE_BW_NRB_100_V01 = 5, /**<  LTE 20 MHz bandwidth \n */
    NWY_NR5G_BW_NRB_6_V01 = 6, /**<  NR5G 1.4 MHz bandwidth \n  */
    NWY_NR5G_BW_NRB_15_V01 = 7, /**<  NR5G 3 MHz bandwidth \n  */
    NWY_NR5G_BW_NRB_25_V01 = 8, /**<  NR5G 5 MHz bandwidth \n  */
    NWY_NR5G_BW_NRB_50_V01 = 9, /**<  NR5G 10 MHz bandwidth \n  */
    NWY_NR5G_BW_NRB_75_V01 = 10, /**<  NR5G 15 MHz bandwidth \n  */
    NWY_NR5G_BW_NRB_100_V01 = 11, /**<  NR5G 20 MHz bandwidth  */
    NWY_BANDWIDTH_ENUM_TYPE_MAX = 2147483647
}nwy_bandwidth_type_e;

typedef enum{
  NWY_RAT_SUB_NO_SERVICE,
  NWY_RAT_SUB_GSM,
  NWY_RAT_SUB_GPRS,
  NWY_RAT_SUB_EDGE,
  NWY_RAT_SUB_WCDMA,
  NWY_RAT_SUB_HSDPA = 5,
  NWY_RAT_SUB_HSUPA,
  NWY_RAT_SUB_HSDPA_HSUPA,
  NWY_RAT_SUB_TDSCDMA,
  NWY_RAT_SUB_HSPA_PLUS,
  NWY_RAT_SUB_TDD_LTE = 10,
  NWY_RAT_SUB_FDD_LTE,
  NWY_RAT_SUB_LTE_TO_5GC,
  NWY_RAT_SUB_NR_TO_5GC,
  NWY_RAT_SUB_NG_RAN,
  NWY_RAT_SUB_ENDC = 15,
  NWY_RAT_SUB_NR_TO_EPS,
  NWY_RAT_SUB_CDMA1X,
  NWY_RAT_SUB_HDR,
  NWY_RAT_SUB_CDMA1X_LTE,
  NWY_RAT_SUB_CATM = 20,
  NWY_RAT_SUB_NBIOT ,
  NWY_RAT_SUB_MAX
}nwy_rat_sub_type_e;

typedef enum {
    NWY_CDMA_PILOT_TYPE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    NWY_CDMA_PILOT_CURR_ACT_PLT_V01 = 0x00,
    NWY_CDMA_PILOT_NEIGHBOR_PLT_V01 = 0x01,
    NWY_CDMA_PILOT_TYPE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}nwy_cdma_pilot_e;

typedef enum {
    NWY_CDMA_CLASS_NONE        = -1,

    NWY_CDMA_BC0               = 0,    /**< Band Class 0: U.S. Cellular band (800 MHz). */
    NWY_CDMA_BC1               = 1,    /**< Band Class 1: U.S. */
    NWY_CDMA_BC3               = 3,    /**< Band Class 3: Japanese Cellular band (800 MHz). */
    NWY_CDMA_BC4               = 4,    /**< Band Class 4: Korean PCS band (1800 MHz). */
    NWY_CDMA_BC5               = 5,    /**< Band Class 5 (450 MHz). */
    NWY_CDMA_BC6               = 6,    /**< Band Class 6 (2 GHz). */
    NWY_CDMA_BC7               = 7,    /**< Band Class 7 (Upper 700 MHz). */
    NWY_CDMA_BC8               = 8,    /**< Band Class 8 (1800 MHz). */
    NWY_CDMA_BC9               = 9,    /**< Band Class 9 (900 MHz). */
    NWY_CDMA_BC10              = 10,   /**< Band Class 10 (Second 800 MHz). */
    NWY_CDMA_BC11              = 11,   /**< Band Class 11: European PAMR Band (400 MHz). */
    NWY_CDMA_BC12              = 12,   /**< Band Class 12: PAMR Band (800 MHz). */
    NWY_CDMA_BC13              = 13,   /**< Band Class 13: currently undefined. */
    NWY_CDMA_BC14              = 14,   /**< Band Class 14 (U.S. PCS 1.9 GHz Band). */
    NWY_CDMA_BC15              = 15,    /**< Band Class 15 (1700-2100 MHz -AWS). */
    NWY_CDMA_BC16              = 16,    /**< Band Class 16 (U.S. 2.5 GHz). */
    NWY_CDMA_BC17              = 17,    /**< Band Class 17 (U.S. 2.5 GHz Forward Link only band). */
    NWY_CDMA_BC18              = 18,    /**< Band Class 18 (700 MHz Public Safety Broadband). */
    NWY_CDMA_BC19              = 19,    /**< Band Class 19 (Lower 700 MHz band).*/

    NWY_CDMA_BC_MAX            = 20,
    /**< Upper boundary for CDMA band classes.  */
}nwy_band_cdma_class_e;

typedef enum {
    NWY_GSM_NONE        = -1,

    NWY_GSM_450           = 0,    /**< GSM 450 band (450 MHz). */
    NWY_GSM_480           = 1,    /**< GSM 480 band (480 MHz). */
    NWY_GSM_750           = 2,    /**< GSM 750 band (750 MHz). */
    NWY_GSM_850           = 3,    /**< GSM 850 band (850 MHz). */
    NWY_GSM_EGSM_900      = 4,    /**< GSM Extended GSM (E-GSM) 900 band (900 MHz). */
    NWY_GSM_PGSM_900      = 5,    /**< GSM Primary GSM (P-GSM) 900 band (900 MHz). */
    NWY_GSM_RGSM_900      = 6,    /**< GSM Railways GSM (R-GSM) 900 band (900 MHz). */
    NWY_GSM_DCS_1800      = 7,    /**< GSM DCS band (1800 MHz). */
    NWY_GSM_PCS_1900      = 8,    /**< GSM PCS band (1900 MHz). */
}nwy_band_gsm_class_e;

typedef enum {
    NWY_LTE_CLASS_NONE  = 0,/* LTE bands*/

    NWY_LTE_EUTRAN_BAND1 = 1,    /**< UL:1920-1980; DL:2110-2170. */
    NWY_LTE_EUTRAN_BAND2 = 2,    /**< UL:1850-1910; DL:1930-1990. */
    NWY_LTE_EUTRAN_BAND3 = 3,    /**< UL:1710-1785; DL:1805-1880. */
    NWY_LTE_EUTRAN_BAND4 = 4,    /**< UL:1710-1755; DL:2110-2155. */
    NWY_LTE_EUTRAN_BAND5 = 5,    /**< UL: 824-849; DL: 869- 894. */
    NWY_LTE_EUTRAN_BAND6 = 6,    /**< UL: 830-840; DL: 875-885. */
    NWY_LTE_EUTRAN_BAND7 = 7,    /**< UL:2500-2570; DL:2620-2690. */
    NWY_LTE_EUTRAN_BAND8 = 8,    /**< UL: 880-915; DL: 925-960. */
    NWY_LTE_EUTRAN_BAND9 = 9,    /**< UL:1749.9-1784.9; DL:1844.9-1879.9. */
    NWY_LTE_EUTRAN_BAND10 = 10,    /**< UL:1710-1770; DL:2110-2170. */
    NWY_LTE_EUTRAN_BAND11 = 11,    /**< UL:1427.9-1452.9; DL:1475.9-1500.9. */
    NWY_LTE_EUTRAN_BAND12 = 12,    /**< UL:698-716; DL:728-746. */
    NWY_LTE_EUTRAN_BAND13 = 13,    /**< UL: 777-787; DL: 746-756. */
    NWY_LTE_EUTRAN_BAND14 = 14,    /**< UL: 788-798; DL: 758-768. */

    /* Reserved for BAND 15, 16 */

    NWY_LTE_EUTRAN_BAND17 = 17,    /**< UL: 704-716; DL: 734-746. */
    NWY_LTE_EUTRAN_BAND18 = 18,    /**< UL: 815-830; DL: 860-875. */
    NWY_LTE_EUTRAN_BAND19 = 19,    /**< UL: 830-845; DL: 875-890. */
    NWY_LTE_EUTRAN_BAND20 = 20,    /**< UL: 832-862; DL: 791-821. */
    NWY_LTE_EUTRAN_BAND21 = 21,    /**< UL: 1447.9-1462.9; DL: 1495.9-1510.9. */
    NWY_LTE_EUTRAN_BAND23 = 23,    /**< UL: 2000-2020; DL: 2180-2200 */
    NWY_LTE_EUTRAN_BAND24 = 24,    /**< UL: 1626.5-1660.5; DL: 1525 -1559. */
    NWY_LTE_EUTRAN_BAND25 = 25,    /**< UL: 1850-1915; DL: 1930 -1995 . */
    NWY_LTE_EUTRAN_BAND26 = 26,    /**< UL: 814-849; DL: 859 -894 . */
    NWY_LTE_EUTRAN_BAND27 = 27,    /**< UL: 807.5 - 824;  DL: 852 - 869 */
    NWY_LTE_EUTRAN_BAND28 = 28,    /**<UL: 703-748; DL: 758-803 . */
    NWY_LTE_EUTRAN_BAND29 = 29,    /**<  UL: 1850-1910 or 1710 - 1755;  DL: 716-728. */
    NWY_LTE_EUTRAN_BAND30 = 30,    /**<  UL: 2305 - 2315 ;  DL: 2350 - 2360 */
    NWY_LTE_EUTRAN_BAND31 = 31,    /**<  UL: 452.5 -   457.5 ;  DL: 462.5 - 467.5  */
    NWY_LTE_EUTRAN_BAND32 = 32,    /**< DL: 9920 - 10359 */
    NWY_LTE_EUTRAN_BAND33 = 33,    /**< UL: 1900-1920; DL: 1900-1920. */
    NWY_LTE_EUTRAN_BAND34 = 34,    /**< UL: 2010-2025; DL: 2010-2025. */
    NWY_LTE_EUTRAN_BAND35 = 35,    /**< UL: 1850-1910; DL: 1850-1910. */
    NWY_LTE_EUTRAN_BAND36 = 36,    /**< UL: 1930-1990; DL: 1930-1990. */
    NWY_LTE_EUTRAN_BAND37 = 37,    /**< UL: 1910-1930; DL: 1910-1930. */
    NWY_LTE_EUTRAN_BAND38 = 38,    /**< UL: 2570-2620; DL: 2570-2620. */
    NWY_LTE_EUTRAN_BAND39 = 39,    /**< UL: 1880-1920; DL: 1880-1920. */
    NWY_LTE_EUTRAN_BAND40 = 40,    /**< UL: 2300-2400; DL: 2300-2400. */
    NWY_LTE_EUTRAN_BAND41 = 41,    /**< UL: 2496-2690; DL: 2496-2690. */
    NWY_LTE_EUTRAN_BAND42 = 42,    /**< UL: 3400-3600; DL: 3400-3600. */
    NWY_LTE_EUTRAN_BAND43 = 43,    /**< UL: 3600-3800; DL: 3600-3800. */
    NWY_LTE_EUTRAN_BAND46 = 46,    /**< DL: 5150-5925 MHz. */
    NWY_LTE_EUTRAN_BAND47 = 47,    /**< DL: ? MHz. */
    NWY_LTE_EUTRAN_BAND48 = 48,    /**< DL: ? MHz. */
    NWY_LTE_EUTRAN_BAND49 = 49,    /**< UL: ?; DL: ? */
    NWY_LTE_EUTRAN_BAND53 = 53,    /**< 2483.5 - 2495 MHz  TDD */
    NWY_LTE_EUTRAN_BAND59 = 59,    /**< 1785 - 1805MHz  TDD */
    NWY_LTE_EUTRAN_BAND67 = 67,    /**< UL: ?; DL: ? */
    NWY_LTE_EUTRAN_BAND66 = 66,    /**< UL: 1710-1780; DL: 2110-2200 */
    NWY_LTE_EUTRAN_BAND68 = 68,    /**< UL: ?; DL: ? */
    NWY_LTE_EUTRAN_BAND71 = 71,    /**< UL: ?; DL: ? */
    NWY_LTE_EUTRAN_BAND125 = 125,    /**< DL: 64835 - 64859 */
    NWY_LTE_EUTRAN_BAND126 = 126,    /**< DL: 64860 - 64974 */
    NWY_LTE_EUTRAN_BAND127 = 127,    /**< DL: 64975 - 64999  */
    NWY_LTE_EUTRAN_BAND250 = 250,    /**< UL: 3400-3800; DL: 3400 -3800 . */
    NWY_LTE_EUTRAN_BAND252 = 252,    /**< DL: 5150 - 5250  */
    NWY_LTE_EUTRAN_BAND255 = 255,    /**< DL: 5725 - 5850  */
}nwy_band_lte_class_e;


typedef enum {
    NWY_WCDMA_CLASS_NONE  = 0,
    NWY_WCDMA_I_IMT_2000  = 1,     /**< WCDMA Europe, Japan, and China IMT 2100 band.  W1*/
    NWY_WCDMA_II_PCS_1900 = 2,    /**< WCDMA U.S. PCS 1900 band.  W2*/
    NWY_WCDMA_III_1800    = 3,    /**< WCDMA Europe and China DCS 1800 band.  W3*/
    NWY_WCDMA_IV_1700     = 4,    /**< WCDMA U.S. 1700 band.  W4*/
    NWY_WCDMA_V_850       = 5,    /**< WCDMA U.S. 850 band. W5*/
    NWY_WCDMA_VI_800      = 6,    /**< WCDMA Japan 800 band. W6*/
    NWY_WCDMA_VII_2600    = 7,    /**< WCDMA Europe 2600 band. W7*/
    NWY_WCDMA_VIII_900    = 8,    /**< WCDMA Europe and China 900 band. W8*/
    NWY_WCDMA_IX_1700     = 9,    /**< WCDMA Japan 1700 band. W9*/
    /* Reserved W10 for WCDMA BC10-1700 band classes. */

    NWY_WCDMA_XI_1500     = 11,    /**< WCDMA 1500 band. W11*/
    NWY_WCDMA_XIX_850     = 19,    /**< WCDMA Japan 850 band. W19*/
}nwy_band_wcdma_class_e;

typedef enum {
    NWY_TDS_CLASS_NONE  = 0,      /* TD-SCDMA bands */
    NWY_TDS_BANDA = 1,    /**< TDS Band A 1900-1920 MHz, 2010-2020 MHz */
    NWY_TDS_BANDB = 2,    /**< TDS Band B 1850-1910 MHz, 1930-1990 MHz */
    NWY_TDS_BANDC = 3,    /**< TDS Band C 1910-1930 MHz */
    NWY_TDS_BANDD = 4,    /**< TDS Band D 2570-2620 MHz */
    NWY_TDS_BANDE = 5,    /**< TDS Band E 2300-2400 MHz */
    NWY_TDS_BANDF = 6,    /**< TDS Band F 1880-1920 MHz */
}nwy_band_tdscdma_class_e;

typedef enum
{
    NWY_NR5G_CLASS_NONE  = 0,

    /*NR bands in FR1*/
    NWY_NR5G_BAND1 = 1,    /**< UL: 1920 - 1980 MHz, DL: 2110 - 2170 MHz, FDD*/
    NWY_NR5G_BAND2 = 2,    /**< 1850 - 1910 MHz  1930 - 1990 MHz FDD */
    NWY_NR5G_BAND3 = 3,    /**< 1710 - 1785 MHz  1805 - 1880 MHz FDD */
    NWY_NR5G_BAND5 = 5,    /**< 824 - 849 MHz    869 - 894MHz    FDD */
    NWY_NR5G_BAND7 = 7,    /**< 2500 - 2570 MHz  2620 - 2690 MHz FDD */
    NWY_NR5G_BAND8 = 8,    /**<880 - 915 MHz 925 - 960 MHz   FDD */
    NWY_NR5G_BAND12 = 12,    /**<729 - 746 MHz 699 - 716 MHz   FDD */
    NWY_NR5G_BAND13 = 13,    /**<777 - 787 MHz 746 MHz - 756 MHz FDD */
    NWY_NR5G_BAND14 = 14,    /**<758 - 768 MHz 788 - 798 MHz FDD */
    NWY_NR5G_BAND18 = 18,    /**<815 - 830 MHz 860 - 875 MHz FDD */
    NWY_NR5G_BAND20 = 20,    /**< 832 - 862 MHz    791- 821MHz FDD */
    NWY_NR5G_BAND25 = 25,    /**< 1930 - 1995 MHz   1850 - 1915 MHz  FDD*/
    NWY_NR5G_BAND26 = 26,    /**<814 - 849 MHz 859 - 894 MHz FDD */
    NWY_NR5G_BAND28 = 28,    /**< 703 - 748 MHz    758 - 803 MHz   FDD */
    NWY_NR5G_BAND29 = 29,    /**<N/A   717 MHz - 728 MHz SDL */
    NWY_NR5G_BAND30 = 30,    /**<2305 - 2315 MHz 2350 - 2360 MHz FDD */
    NWY_NR5G_BAND34 = 34,    /**< 2010 - 2025 MHz  TDD */
    NWY_NR5G_BAND38 = 38,    /**< 2570 - 2620 MHz  2570 - 2620 MHz TDD*/
    NWY_NR5G_BAND39 = 39,    /**< 1880 - 1920 MHz  TDD */
    NWY_NR5G_BAND40 = 40,    /**< 2300 - 2400 MHz  TDD */
    NWY_NR5G_BAND41 = 41,    /**<2496 - 2690 MHz   2496 - 2690 MHz TDD */
    NWY_NR5G_BAND46 = 46,    /**< 5150-5925 MHz TDD */
    NWY_NR5G_BAND48 = 48,    /**< 3550 - 3700 MHz  TDD */
    NWY_NR5G_BAND50 = 50,    /**< 1432 - 1517 MHz  1432 - 1517 MHz TDD */
    NWY_NR5G_BAND51 = 51,    /**< 1427 - 1432 MHz  1427 - 1432 MHz TDD */
    NWY_NR5G_BAND53 = 53,    /**< 2483.5 - 2495 MHz  TDD */
    NWY_NR5G_BAND65 = 65,    /**< 2110 - 2200 MHz   1920 - 2010 MHz  FDD    */
    NWY_NR5G_BAND66 = 66,    /**< 1710 - 1780 MHz  2110 - 2200 MHz FDD*/
    NWY_NR5G_BAND70 = 70,    /**<1695 - 1710 MHz   1995- 2020 MHz  FDD */
    NWY_NR5G_BAND71 = 71,    /**< 663 - 698 MHz    617 - 652 MHz   FDD */
    NWY_NR5G_BAND74 = 74,    /**< 1427 -1470 MHz   1475 - 1518 MHz FDD */
    NWY_NR5G_BAND75 = 75,    /**< N/A  1432 - 1517 MHz SDL*/
    NWY_NR5G_BAND76 = 76,    /**<N/A   1427 - 1432 MHz SDL */
    NWY_NR5G_BAND77 = 77,    /**< 3.3 - 4.2 GHz    3.3 - 4.2 GHz   TDD */
    NWY_NR5G_BAND78 = 78,    /**< 3.3 - 3.8 GHz    3.3 - 3.8 GHz   TDD */
    NWY_NR5G_BAND79 = 79,    /**< 4.4 - 5.0 GHz    4.4 - 5.0 GHz   TDD*/
    NWY_NR5G_BAND80 = 80,    /**<1710 - 1785 MHz   N/A SUL  */
    NWY_NR5G_BAND81 = 81,    /**< 880 - 915 MHz    N/A SUL  */
    NWY_NR5G_BAND82 = 82,    /**< 832 - 862 MHz    N/A SUL */
    NWY_NR5G_BAND83 = 83,    /**< 703 - 748 MHz    N/A SUL*/
    NWY_NR5G_BAND84 = 84,    /**<1920 - 1980 MHz   N/A SUL  */
    NWY_NR5G_BAND85 = 85,    /**< 2496 - 2690 MHz  N/A SUL */
    NWY_NR5G_BAND86 = 86,    /**< 1710 - 1780 MHz  SUL  */
    NWY_NR5G_BAND91 = 91,    /**< 832 - 862 MHz  1427 - 1432 MHz  FDD */
    NWY_NR5G_BAND92 = 92,    /**< 832 - 862 MHz  1432 - 1517 MHz  FDD */
    NWY_NR5G_BAND93 = 93,    /**< 880 - 915 MHz  1427 - 1432 MHz  FDD */
    NWY_NR5G_BAND94 = 94,    /**< 880 - 915 MHz  1432 - 1517 MHz  FDD */

    /*NR bands in FR2*/
    NWY_NR5G_BAND257 = 257,    /**< 26.5 -29.5 GHz   26.5 -29.5 GHz  TDD */
    NWY_NR5G_BAND258 = 258,    /**< 24.25 - 27.5 GHz 24.25 - 27.5 GHz    TDD */
    NWY_NR5G_BAND259 = 259,    /**< N/A  N/A TDD */
    NWY_NR5G_BAND260 = 260,    /**< 37-40 GHz    37-40 GHz   TDD */
    NWY_NR5G_BAND261 = 261,    /**< 27.5-28.3 GHz TDD */
}nwy_band_nr5g_class_e;

typedef enum {
    NWY_NW_NET_STAT_UNKNOWN    = 0,   /* Network state unknown */
    NWY_NW_NET_STAT_AVAILABLE,        /* Network is available for registration */
    NWY_NW_NET_STAT_CURRENT,          /* Network currently registered */
    NWY_NW_NET_STAT_FORBIDDEN,        /* Network forbidden registraiton */
}nwy_nw_status_e;

typedef enum {
    NWY_NW_SELECT_DEREG   = 0,
    NWY_NW_SELECT_REG_AUTO = 1,
    NWY_NW_SELECT_REG_MANU = 2,
    NWY_NW_SELECT_REG_MANU_AUTO = 3,// if manual selection fails, automatic mode is entered
}nwy_nw_select_mode_e;

typedef enum {
    NWY_NW_REGS_NONE = -1,
    NWY_NW_REGS_DATA_IND = 0,   // data register state event
    NWY_NW_REGS_VOICE_IND = 1,  // voice register state event
}nwy_nw_regs_ind_type_e;

typedef enum{
    NWY_NW_CFG_DISABLE = 0,
    NWY_NW_CFG_ENABLE = 1,
} nwy_nw_cfg_switch_type_e;

typedef enum {
    NWY_NW_PS_MODE_II    = 0,   //UE only register EPS service,data centric
    NWY_NW_CS_PS_MODE_I  = 1,   //UE registers both EPS/CS services, voice centric
    NWY_NW_CS_PS_MODI_II = 2,   //UE registers both EPS/CS services, data centric
    NWY_NW_PS_MODE_I     = 3,   //UE only register EPS service,voice centric
} nwy_nw_cfg_uemode_type_e;

typedef enum{
    NWY_NW_GET_SCELL = 0,       //serving cell
    NWY_NW_GET_ALL_CELL = 1,    //serving cell + neighborhood cell
} nwy_nw_cellinfo_mode_e;

typedef enum{
    NWY_NW_CELLINFO_MODE_GSM = 0,
    NWY_NW_CELLINFO_MODE_TD_SCDMA,
    NWY_NW_CELLINFO_MODE_WCDMA,
    NWY_NW_CELLINFO_MODE_LTE,
    NWY_NW_CELLINFO_MODE_NR,
    NWY_NW_CELLINFO_MODE_MAX
}nwy_nw_scancell_mode_e;

/*****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************/


/******************************************************************************
 * 5 STRUCT Type Definition
******************************************************************************/
typedef struct {
    int band_nums;//if nums=0,unlock = lock all band
    uint16 band[NWY_NW_LOCK_BAND_MAX_NUM];
}nwy_nw_lock_band_t;

typedef struct {
    int freq_nums;//if num=0,unlock = lock all freq
    uint32 freq[NWY_NW_LOCK_FREQ_MAX_NUM];
}nwy_nw_lock_freq_t;

typedef struct {
    uint32 freq;
    int cell_nums;//if cell_num=0,unlock = lock all cell
    uint16 pci[NWY_NW_LOCK_PCI_MAX_NUM];
}nwy_nw_lock_pci_info_t;
typedef struct {
    int lock_pci_nums;
    nwy_nw_lock_pci_info_t lock_pci_info[NWY_NW_LOCK_PCI_MAX_NUM];
}nwy_nw_lock_pci_t;

typedef union {
    nwy_nw_lock_band_t lock_band;
    nwy_nw_lock_freq_t lock_freq;
    nwy_nw_lock_pci_t lock_pci;
}nwy_nw_lock_info_u;

typedef struct {
    nwy_nw_lockmode_e mode;
    nwy_nw_mode_mask_e rat;
    nwy_nw_lock_info_u lock_info;
} nwy_nw_lock_t;

typedef struct {
    nwy_nw_priband_mode_e mode;
    nwy_nw_mode_mask_e rat;
    int band_nums;
    int band[NWY_NW_PRI_BAND_MAX_NUM];
} nwy_nw_priband_t;


typedef struct {
    nwy_nw_rat_type_e rat;
    int band;
    //GSM:  0-PGSM,1-DCS,2-PCS,3-EGSM,4-GSM450,5-GSM480,6-GSM850;
    //WCDMA: 1-BAND1,2-BAND2,...,14:BAND14; 
    //LTE:  1-BAND1,2-BAND2...
    int power_value;
    int band_width;// 0: invalid;1: 1.4MHz;2: 3MHz;3: 5MHz;4: 10MHz;5: 15MHz;6: 20MHz;
} nwy_nw_power_backoff;

typedef struct {
    uint32 arfcn;
    uint16 cellId;   //if no spec cellid,please set cellid as 0xffff
} nwy_nw_blackcell_t;


typedef enum {
    NWY_NW_DELETE_ALL_BLACKCELL= 0,
    NWY_NW_ADD_SPEC_BLACKCELL = 1,
    NWY_NW_DELETE_SPEC_BLACKCELL  = 2,
} nwy_nw_blackcell_action_e;


typedef struct {
    uint8 numOfCell;
    nwy_nw_blackcell_t blackcell[NWY_NW_BLACKCELL_MAX_NUM];
} nwy_nw_blackcell_list_t;

typedef struct {
    nwy_nw_edrx_mode_e mode;
    nwy_nw_mode_mask_e rat;     //radio access technology,ex.gsm/wcdma..
    uint8 requested_eDRX_value; //see Extended DRX parameters information element in 3GPP TS 24.008 Table 10.5.5.32
    uint8 nw_provided_eDRX_value;
    uint8 Paging_time_window;
} nwy_nw_edrx_info_t;

typedef struct {
    nwy_nw_psm_mode_e mode;
    uint8 requested_Periodic_rau;  //T3312, see the GPRS Timer 3 IE in 3GPP TS 24.008 Table 10.5.163a
    uint8 requested_gprs_ready_timer; //T3314, see the GPRS Timer IE in 3GPP TS 24.008 Table 10.5.172

/*****************************************************************************
Timer 3 Bits
8 7 6
0 0 0 value is incremented in multiples of 10 minutes;
0 0 1 value is incremented in multiples of 1 hour;
0 1 0 value is incremented in multiples of 10 hours;
0 1 1 value is incremented in multiples of 2 seconds;
1 0 0 value is incremented in multiples of 30 seconds;
1 0 1 value is incremented in multiples of 1 minutes;
1 1 0 value is incremented in multiples of 320 hours;
1 1 1  the timer is deactivated

ex
010-00111 70 hours
101-00100 4 min
******************************************************************************/
    uint8 requested_Periodic_tau; //T3412, see the GPRS Timer 3 IE in 3GPP TS 24.008 Table 10.5.163a

/*****************************************************************************
Timer 2 Bits
8 7 6
0 0 0 value is incremented in multiples of 2 seconds;
0 0 1 value is incremented in multiples of 1 min;
0 1 0 value is incremented in multiples of decihours;
1 1 1  the timer is deactivated
Other values shall be interpreted as multiples of 1 minute

ex
101-00100 4 min
******************************************************************************/
    uint8 requested_active_timer; //T3324, see the GPRS Timer 2 IE in 3GPP TS 24.008 Table 10.5.163
} nwy_nw_psm_info_t;

typedef struct {
    char long_eons[NWY_NW_LONG_EONS_MAX_LEN + 1];    //long operator name,ex."CHINA MOBILE"
    char short_eons[NWY_NW_SHORT_EONS_MAX_LEN + 1];   //short operator name,ex."CMCC"
    char mcc[NWY_NW_MNC_MCC_LEN + 1];           //Mobile Country Code
    char mnc[NWY_NW_MNC_MCC_LEN + 1];           //Mobile Network Code
    char spn[NWY_NW_SPN_LEN_MAX + 1];           //Servoce Provider Name,0xFF - InValid
    nwy_nw_spn_enc_e spn_enc;
}nwy_nw_operator_t;

typedef struct{
    uint8 csq_rssi_level;
    uint8 ber;
}nwy_nw_get_csq_info_t;


typedef struct {
    int rssi; /*received signal strength indicator value,range[-113,-48]dBm see 3GPP TS 45.008 subclause 8.1.4
    		  0 - Invalid, indicates that not known or not detectable.*/
    int rssi_level;
    int ber; /* channel bit error rate, range[0,7], as RXQUAL values in the table in 3GPP TS 45.008 [20] subclause 8.2.4
              99 - Invalid, indicates that not known or not detectable.*/
}nwy_nw_gsm_signal_info_t;

typedef struct {
    int rssi; /*this indicates forward link pilot Power (AGC) + Ec/Io,positive dBm Range: -125 to -75 dBm
    		   0 - Invalid, indicates that not known or not detectable.*/
    int ecio; /*Strength of the pilot (in dB). Range: 0 to 64. see TIA/EIA/IS-2000-5-A
    		   99 - Invalid, indicates that not known or not detectable.*/
}nwy_nw_cdma_hdr_signal_info_t;

typedef struct {
    int rscp; /* Received signal code power, range [-120,-25]dBm ; see 3GPP TS 25.133 subclause 9.1.1.3
    		  the received power on one code measured in dBm on the primary CPICH channel of the cell.
    		  255 - Invalid, indicates that not known or not detectable. */
    int rscp_level;
    int ecio; /* the received energy per chip divided by the power density in the band measured in dBm on the primary
            	CPICH channel of the cell,range [-24,0]dB.
    		  255 - Invalid, indicates that not known or not detectable.*/
    int ecio_level;
    int rssi; /* range [-100,-25]dBm  see 3GPP TS 25133 subclause 9.1.3.3
              0 - Invalid, indicates that not known or not detectable.*/
    int rssi_level;
}nwy_nw_wcdma_signal_info_t;

typedef struct {
    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1400 to -440 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 36133 subclause  9.1.4
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrp_level;
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -200 to -30 (e.g., -200 means -20.0 dB).see 3GPP TS 36133 subclause  9.1.7
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq_level;
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1200 to -250 (e.g., -440 means -44.0 dBm). see 3GPP TS 36133 9.1.18.5
    			0 - Invalid, indicates that not known or not detectable.*/
    int rssi_level;
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 36133 9.1.17
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int sinr_level;
}nwy_nw_lte_signal_info_t;

typedef struct {
    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1560 to -310 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 38133 subclause  10.1.6
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrp_level;
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -430 to 200 (e.g., -200 means -20.0 dB).see 3GPP TS 38133 subclause  10.1.11
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq_level;
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1000 to -2500 (e.g., -440 means -44.0 dBm). see 3GPP TS 38133 10.1.34
    			0 - Invalid, indicates that not known or not detectable.*/
    int rssi_level;
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 38133 10.1.16
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int sinr_level;
}nwy_nw_nr_signal_info_t;

typedef struct {
    int ecno; /* Range: -128 to 0 dB
    		255 - Invalid, indicates that not known or not detectable.*/
    int rscp;/* Range: -128 to -25 dBm
    		255 - Invalid, indicates that not known or not detectable.*/
    int rssi;/* Range: -128 to -25 dBm
    		0 - Invalid, indicates that not known or not detectable.*/
}nwy_nw_tdscdma_signal_info_t;

typedef union{
   nwy_nw_gsm_signal_info_t gsm_signal_info;
   nwy_nw_wcdma_signal_info_t wcdma_signal_info;
   nwy_nw_cdma_hdr_signal_info_t cdmd_hdr_signal_info;
   nwy_nw_lte_signal_info_t lte_signal_info;
   nwy_nw_nr_signal_info_t nr_signal_info;
   nwy_nw_tdscdma_signal_info_t tdscdma_signal_info;
}nwy_nw_rat_signal_info_u;

typedef struct {
    nwy_nw_rat_type_e rat;
    nwy_nw_rat_signal_info_u  rat_signal_info;
}nwy_nw_signal_info_t;

typedef struct {
    nwy_nw_rat_type_e rat_type;    //Radio technology; see #NWY_nw_radio_tech_t
    nwy_nw_service_type_e regs_state;       //Registration state
    nwy_nw_roam_state_e roam_state;
    nwy_rat_sub_type_e sub_rat;
}nwy_nw_common_regs_type_t;

typedef struct {
    uint8 mm_regs_reject_info_valid;  //mm:Mobile Management
    uint8 mm_regs_rej_cause;
    uint8 sm_regs_reject_info_valid;  //sm:Session Management
    uint8 sm_regs_rej_cause;
    int rej_srv_domain;
}nwy_nw_res_reject_info_t;

/*The core network contains two parts, the CS domain, and the PS domain */
typedef struct {
    uint8 cs_regs_valid;
    nwy_nw_common_regs_type_t cs_regs;/* CS: 'Circuit Switching', handles voice calls in 2.3G net */
    uint8 ps_regs_valid;
    nwy_nw_common_regs_type_t ps_regs;/* PS: 'Packet Switching',  handles data sessions in 2.3.4G net */
    nwy_nw_res_reject_info_t reject_info;
}nwy_nw_regstatus_t;

typedef struct {
    uint16  mcc; /*A 16-bit integer representation of MCC. Range: 0 to 999.*/
    uint16  mnc; /*A 16-bit integer representation of MNC. Range: 0 to 999.*/
    uint8  mnc_includes_pcs_digit;
  /*This field is used to interpret the length of the corresponding
       MNC reported in this TLV. Values: \n
       - TRUE  -- MNC is a three-digit value; e.g., a reported value of
                  90 corresponds to an MNC value of 090  \n
       - FALSE -- MNC is a two-digit value; e.g., a reported value of
                  90 corresponds to an MNC value of 90
  */
}nwy_nw_plmn_id_type_t;

typedef struct {
    nwy_cdma_pilot_e pilot_type;
    /*<   Pilot information type. Values:
       - 0x00 -- NWY_CDMA_PILOT_CURR_ACT_PLT_V01 -- Current active pilot information \n
       - 0x01 -- NWY_CDMA_PILOT_NEIGHBOR_PLT_V01 -- Neighbor pilot information\n*/
    uint16 sid;
    /*System ID. Range: 0 to 32767. */
    uint16 nid;
    /*  Network ID. Range: 0 to 65535.*/
    uint16 base_id;
    /*   Base station ID. */

    nwy_nw_plmn_id_type_t plmn;
    nwy_band_cdma_class_e band;
    /* Active band of the current system. */
    int32 channel;
    /* Active channel of the current system. */
    uint16 pilot_pn;
    /*Pilot PN sequence offset index. Range: 0 to 511.*/
    uint16 pilot_strength;
    /*Strength of the pilot (in dB). Range: 0 to 64*/
    uint32 base_lat;
    /*Latitude of the current base station in units of 0.25 sec.*/
    uint32 base_long;
    /*Longitude of the current base station in units of 0.25 sec.*/
    uint64 time_stamp;
    /*Time (in milliseconds) from the start of GPS time when the measurement was taken.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
}nwy_cdma_bs_info_t ;

typedef struct {
    uint32 bs_len;
    nwy_cdma_bs_info_t bs[NWY_NW_CDMA_POSITION_MAX];
}nwy_cdma_hdr_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint32 cell_id;/* range 0..65535,
    				0xffffffff-Invalid, indicates that not known or not detectable.
    				(all data in nwy_gsm_serv_cell_info_t invalid when cell_id invalid) */
    uint32 timing_advance;/*Time offset; changes with the mobile distance from the base station. range 0-63
    				Measured delay (in bit periods; 1 bit period = 48/13 microsecond) of an access burst transmission on the RACH or PRACH to the expected signal from an MS at zero distance under static channel conditions.
    				0xffffffff - Invalid, indicates timing advance information is not present.*/
    uint16 lac;  /* range 0..65535, see 24.008 10.5.1.3 */
    nwy_band_gsm_class_e band;
    uint16 arfcn; /*range 0..1023 */
    uint8 bsic; /* range 0..63, see 23.003 4.3.2 */
    uint8 egprs_support; //1: is edge
    int rssi; /*received signal strength indicator value,range[-113,-48]dBm see 3GPP TS 45.008 subclause 8.1.4
    		   0-Invalid, indicates that not known or not detectable.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
    int32 ber;
}nwy_gsm_serv_cell_info_t;


typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint8  bsic;  /* range 0..63, see 23.003 4.3.2 */
    uint16 lac;   /* range 0..65535, see 24.008 10.5.1.3 */
    uint16 cell_id; /* range 0..65535,
    				0xffffffff-Invalid, indicates that not known or not detectable.*/
    uint16 arfcn; /*range 0..1023 */
    int16 rssi; /*received signal strength indicator value,range[-113,-48]dBm see 3GPP TS 45.008 subclause 8.1.4
    			 0-Invalid, indicates that not known or not detectable.*/
    uint16 RxLev;	//RX_LEV range 0..63, See 45.008 sect 8.1.4, default: 0
}nwy_nbr_gsm_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 lac;
    uint32 cellid;
    uint16 uArfcn; /*Absolute radio frequency channel number */
    uint16 psc_cellParameterId;   /*Primary scrambling code*/
    uint16 set; /*Intrafrequency cell type. */
    int rscp; /* Received signal code power, range [-120,-25]dBm ; see 3GPP TS 25.133 subclause 9.1.1.3
    		  the received power on one code measured in dBm on the primary CPICH channel of the cell.
    		  255 - Invalid, indicates that not known or not detectable. */
    int ecio; /* the received energy per chip divided by the power density in the band measured in dBm on the primary
            	CPICH channel of the cell,range [-24,0]dB.
    		  255 - Invalid, indicates that not known or not detectable.*/
    int16   s_rxlev;  /*Suitable receive level. */
}nwy_nbr_wcdma_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 tac;
    uint32 cellid;
    uint16  pci;
    uint32  earfcn;
    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1400 to -440 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 36133 subclause  9.1.4
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -200 to -30 (e.g., -200 means -20.0 dB).see 3GPP TS 36133 subclause  9.1.7
    			255 - Invalid, indicates that not known or not detectable.*/
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1200 to -250 (e.g., -440 means -44.0 dBm). see 3GPP TS 36133 9.1.18.5
    			0 - Invalid, indicates that not known or not detectable.*/
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 36133 9.1.17
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int16 srxlev; /*Suitable receive level. */
}nwy_nbr_lte_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 tac;
    uint64 nr_cellid;
    uint32 nr5g_arfcn;  /**< Raster frequency of the serving cell. */
    uint16 pci;   /**< Physical cell ID. */
    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1560 to -310 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 38133 subclause  10.1.6
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -430 to 200 (e.g., -200 means -20.0 dB).see 3GPP TS 38133 subclause  10.1.11
    			255 - Invalid, indicates that not known or not detectable.*/
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1000 to -2500 (e.g., -440 means -44.0 dBm). see 3GPP TS 38133 10.1.34
    			0 - Invalid, indicates that not known or not detectable.*/
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 38133 10.1.16
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int16 srxlev; /*Suitable receive level. */
}nwy_nbr_nr_cell_info_t;

typedef struct {
    uint16 uarfcn;    /*TD-SCDMA layer frequency. */
    uint16 lac;
    uint32 cellid;
    uint16 cpid;    /* TDS cell ID. */
    uint16 cell_position;   /*TDS cell position. */
    int16  rscp;   /*This is the absolute power level of the CPICH as received by the UE in dBm. Refer to 3GPP TS 25.304}.
				Range: -128 to -25 dBm
				255 - Invalid, indicates that not known or not detectable.*/
    int16  s_rxlev;  /*Suitable receive level. */
} nwy_nbr_tdscdma_cell_info_t;

typedef struct {
    nwy_gsm_serv_cell_info_t serv_cell;

    uint32 gsm_nbr_gsm_cell_len;
    nwy_nbr_gsm_cell_info_t nbr_gsm_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 gsm_nbr_wcdma_cell_len;
    nwy_nbr_wcdma_cell_info_t nbr_wcdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 gsm_nbr_tdscdma_cell_len;
    nwy_nbr_tdscdma_cell_info_t nbr_tdscdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 gsm_nbr_lte_cell_len;
    nwy_nbr_lte_cell_info_t nbr_lte_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 gsm_nbr_nr_cell_len;
    nwy_nbr_nr_cell_info_t nbr_nr_cell[NWY_NW_NBR_CELLS_MAX_NUM];
}nwy_gsm_cell_info_t;


typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 cell_id; /*Cell identity; as per 3G TS 25.331, 10.3.2.2 (28 bits) */
    uint16 lac; /*Location area code */
    uint8 is_rac_present; /*Boolean indicated is RAC is present*/
    uint8 rac;
    uint32 ura; /*URA identity */
    uint16 uarfcn; /*Absolute radio frequency channel number */
    nwy_band_wcdma_class_e band;
    uint16 psc; /*Primary scrambling code. */
    uint8 HSDPASupport;  /*TRUE - serving cell supports HSDPA; FALSE - other. */
    uint8 HSUPASupport;  /*TRUE - serving cell supports HSUPA; FALSE - other. \sa CCI API Ref Manual */
    int rscp; /* Received signal code power, range [-120,-25]dBm ; see 3GPP TS 25.133 subclause 9.1.1.3
    		  the received power on one code measured in dBm on the primary CPICH channel of the cell.
    		  255 - Invalid, indicates that not known or not detectable. */
    int ecio; /* the received energy per chip divided by the power density in the band measured in dBm on the primary
            	CPICH channel of the cell,range [-24,0]dB.
    		  255 - Invalid, indicates that not known or not detectable.*/
    int rssi; /* range [-100,-25]dBm  see 3GPP TS 25133 subclause 9.1.3.3
              0 - Invalid, indicates that not known or not detectable.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
}  nwy_wcdma_serv_cell_info_t;

typedef struct {
    nwy_wcdma_serv_cell_info_t serv_cell;

    uint8 wcdma_nbr_cell_len;
    nwy_nbr_wcdma_cell_info_t wcdma_nbr_wcdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 wcdma_gsm_nbr_len;
    nwy_nbr_gsm_cell_info_t wcdma_nbr_gsm_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 wcdma_nbr_lte_len;
    nwy_nbr_lte_cell_info_t wcdma_nbr_lte_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint32 wcdma_nbr_nr_len;
    nwy_nbr_nr_cell_info_t wcdma_nbr_nr_cell[NWY_NW_NBR_CELLS_MAX_NUM];
}nwy_wcdma_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint32 cell_id;   /*Cell ID (0xFFFFFFFF indicates cell ID information is not present).*/
    uint16 lac;   /*Location area code. (This field is ignored when cell_id is not present.)*/
    nwy_band_tdscdma_class_e band;
    uint16 uarfcn;   /*Absolute RF channel number.*/
    uint8 cell_parameter_id;   /*Cell parameter ID.*/
    uint8 pathloss;   /*Path loss in units of 1 dB.*/
    uint32 timing_advance;   /*Measured delay (in seconds) of an access burst transmission on the RACH
    					   or PRACH to the expected signal from an MS at zero distance under static
    					   channel conditions. */
    int rscp; /*Received signal code power in dBm.Range: -128 to -25 dBm
    		255 - Invalid, indicates that not known or not detectable.*/
}nwy_tdscdma_serv_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 uarfcn;   /*Absolute RF channel number.*/
    uint8 cell_parameter_id;   /*Cell parameter ID.*/
    int rscp;/*Received signal code power in dBm.Range: -128 to -25 dBm
    		255 - Invalid, indicates that not known or not detectable.*/
}nwy_tdscdma_nbr_tdscdma_cell_info_t;

typedef struct {
    nwy_tdscdma_serv_cell_info_t serv_cell;
    uint8 tdscdma_nbr_cell_len;
    nwy_tdscdma_nbr_tdscdma_cell_info_t  tdscdma_nbr_cell[NWY_NW_NBR_CELLS_MAX_NUM];
}nwy_tdscdma_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint16 tac;
    uint16 earfcn;  /* EARFCN of the serving cell. Range: 0 to 65535. */
    uint32 cell_id;
    uint16 pci; /*Range: 0 to 503.*/

    uint16 band;
    uint16 dlBandwidth;
    uint8 isTdd; /* 1--Tdd, 0--Fdd */
    uint16 cqi; /*Value range: 0~15, invalid(0xFFFF). */

    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1400 to -440 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 36133 subclause  9.1.4
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -200 to -30 (e.g., -200 means -20.0 dB).see 3GPP TS 36133 subclause  9.1.7
    			255 - Invalid, indicates that not known or not detectable.*/
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1200 to -250 (e.g., -440 means -44.0 dBm). see 3GPP TS 36133 9.1.18.5
    			0 - Invalid, indicates that not known or not detectable.*/
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 36133 9.1.17
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
}nwy_lte_serv_cell_info_t;


typedef struct {
    nwy_lte_serv_cell_info_t serv_cell;

    uint8 lte_nbr_len;
    nwy_nbr_lte_cell_info_t lte_nbr_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 lte_nbr_wcdma_len;
    nwy_nbr_wcdma_cell_info_t lte_nbr_wcdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 lte_nbr_gsm_len;
    nwy_nbr_gsm_cell_info_t lte_nbr_gsm_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 lte_nbr_cdma_len;
    nwy_cdma_bs_info_t lte_nbr_cdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 lte_nbr_cdma_hdr_len;
    nwy_cdma_bs_info_t lte_nbr_cdma_hdr_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8 lte_nbr_nr_len;
    nwy_nbr_nr_cell_info_t lte_nbr_nr_cell[NWY_NW_NBR_CELLS_MAX_NUM];
}nwy_lte_cell_info_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint32 tac; /*R5G Serving Cell Information*/
    uint64 cell_id;
    uint32 g_node_id; // same as LTE eNodeB id,0 means invaild
    uint32 arfcn;
    nwy_band_nr5g_class_e band;
    nwy_bandwidth_type_e dlBandwidth;
    uint16 pci;   /*Physical cell ID. */

    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1560 to -310 (e.g., -440 means -44.0 dBm)
    		see 3GPP TS 38133 subclause  10.1.6
    		255 - Invalid, indicates that not known or not detectable.*/
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    		Range: -430 to 200 (e.g., -200 means -20.0 dB).see 3GPP TS 38133 subclause  10.1.11
    		255 - Invalid, indicates that not known or not detectable.*/
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    		Range: -1000 to -2500 (e.g., -440 means -44.0 dBm). see 3GPP TS 38133 10.1.34
    		0 - Invalid, indicates that not known or not detectable.*/
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 38133 10.1.16
    		-240 - Invalid, indicates that not known or not detectable.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
    int if_FR1;
}nwy_nr_serv_cell_info_t;

typedef struct {
    nwy_nr_serv_cell_info_t serv_cell;

    uint8 nr_nbr_intra_len;
    nwy_nbr_nr_cell_info_t nr_nbr_intra_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8  nr_nbr_inter_len;
    nwy_nbr_nr_cell_info_t nr_nbr_inter_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8  nr_nbr_lte_len;
    nwy_nbr_lte_cell_info_t nr_nbr_lte_cell[NWY_NW_NBR_CELLS_MAX_NUM];

    uint8  nr_nbr_wcdma_len;
    nwy_nbr_wcdma_cell_info_t nr_nbr_wcdma_cell[NWY_NW_NBR_CELLS_MAX_NUM];
}nwy_nr5g_cell_info_t;

typedef struct
{
  nwy_rat_sub_type_e sub_rat;
  union{
            nwy_cdma_hdr_cell_info_t cdma_hdr_cell;
            nwy_gsm_cell_info_t gsm_cell;
            nwy_wcdma_cell_info_t wcdma_cell;
            nwy_tdscdma_cell_info_t tds_cell;
            nwy_lte_cell_info_t lte_cell;
            nwy_nr5g_cell_info_t nr5g_cell;
        }cell_info;
}nwy_nw_cellinfo_t;

typedef struct {
    nwy_nw_status_e net_status;
    nwy_nw_mode_mask_e net_rat;
    nwy_nw_operator_t net_name;
}nwy_nw_net_scan_item_t;

typedef struct {
    int result;
    int net_num;
    nwy_nw_net_scan_item_t net_list[NWY_NW_SCAN_LIST_MAX_NUM];
}nwy_nw_scan_rsp_list_t;

typedef struct {
    nwy_nw_scancell_mode_e net_mode;
    uint8 cell_num;
    nwy_lte_serv_cell_info_t scan_cell[NWY_NW_SCAN_LIST_MAX_NUM];
}nwy_nw_cell_scan_list_t;

typedef struct {
    nwy_nw_plmn_id_type_t plmn;
    uint32 tac;
    uint32 earfcn;  /* EARFCN of the serving cell. Range: 0 to 65535. */
    uint64 cell_id;
    uint16 pci; /*Range: 0 to 503.*/

    uint16 band;
    uint16 dlBandwidth;
    uint8 isTdd; /* 1--Tdd, 0--Fdd */
    uint16 cqi; /*Value range: 0~15, invalid(0xFFFF). */

    int rsrp; /*Current RSRP in 1/10 dBm as measured.  Range: -1400 to -440 (e.g., -440 means -44.0 dBm)
             	see 3GPP TS 36133 subclause  9.1.4
    			255 - Invalid, indicates that not known or not detectable.*/
    int rsrq;  /*<   Current RSRQ in 1/10 dB as measured.
    			Range: -200 to -30 (e.g., -200 means -20.0 dB).see 3GPP TS 36133 subclause  9.1.7
    			255 - Invalid, indicates that not known or not detectable.*/
    int rssi;  /*   Current RSSI in 1/10 dBm as measured.
    			Range: -1200 to -250 (e.g., -440 means -44.0 dBm). see 3GPP TS 36133 9.1.18.5
    			0 - Invalid, indicates that not known or not detectable.*/
    int sinr; /* Current SINR in 1/10 dB as measured. Range: -230 to 400.  see 3GPP TS 36133 9.1.17
    		    -240 - Invalid, indicates that not known or not detectable.*/
    int32  TxPower;    /* Tx power value in 1/10 dBm resolution. */
    int32  RxPower;   /* Rx power value in 1/10 dBm resolution. */
    int nrSCS;
    int nrRedCap;
} nwy_nw_cell_scan_cell_info_t;

typedef struct {
    nwy_nw_scancell_mode_e net_mode;
    uint8 cell_num;
    nwy_nw_cell_scan_cell_info_t scan_cell[NWY_NW_SCAN_LIST_MAX_NUM];
} nwy_nw_cell_scan_list_ex_t;

typedef struct {
    char mcc[NWY_NW_MNC_MCC_LEN + 1];
    char mnc[NWY_NW_MNC_MCC_LEN + 1];
    nwy_nw_mode_mask_e net_rat;
    nwy_nw_select_mode_e select_mode;
}nwy_nw_select_param_t;

typedef struct{
    uint16 onoff;
    uint16 timer;
} nwy_nw_config_netauto_t;
typedef struct{
    nwy_nw_mode_mask_e support_rat;
    uint16 band_nums;
    uint16 band[NWY_NW_SUPPORTED_BAND_MAX_NUM];
} nwy_nw_cfg_supported_band_t;

typedef struct{
    nwy_nw_cfg_supported_band_t supported_detail[NWY_NW_SUPPORTED_RAT_MAX];
    uint16 supported_rat_num;
} nwy_nw_cfg_net_capacity_t;

typedef struct{
   int auto_off;
   int auto_timer;
}nwy_nw_cfg_auto_search_t;

typedef struct
{
    int txrx;
    int rat ;
    int band ;
    int bw ;
    uint32 channel ;
    int chain_idx ;
    int expected_rxagc ;
    int tx_enable ;
    int tx_power ;
    int lte_start_rb ;
    int lte_num_rb ;
    int waveform ;
    int rx_agc ;
    int gain ;
}nwy_rftest_val_type;

/******************************************************************************
 * 6 UNION Type Definition
 ******************************************************************************/
typedef union {
/* netauto: for plat support multi rat, the func to control if ue restore automatic state while offline;
ex. if platform support LTE+GSM,
    if the func enable, module locked GSM, the module will restore auto(LTE+GSM) when offline <timer>min.
*/
    nwy_nw_config_netauto_t netauto;

/* uemode: the func to control voice/data centric
    voice centric: voice preferred, if the network can't support voice, fallback to 2/3G
    data centric: data preferred, not fallback
*/
    nwy_nw_cfg_uemode_type_e uemode;

/* dataonly: for plat support cs/ps, the func to control if ps only. enable: ps only */
    nwy_nw_cfg_switch_type_e dataonly;

/* antaux: for plat support multi ant(MIMO), the func to control if support aux(div) ant */
    nwy_nw_cfg_switch_type_e antaux;

/* net_capability: the func to get module net capacity */
    nwy_nw_cfg_net_capacity_t net_capacity;

/* nitz: the func to control if open nitz (default open) */
    nwy_nw_cfg_switch_type_e nitz_onoff;
} nwy_nw_config_info_u;

typedef  struct{
    nwy_nw_common_regs_type_t reg_stu;
    nwy_nw_res_reject_info_t reject_info;
}nwy_nw_info_ind_t;

typedef struct {
    uint8_t num;
    nwy_nw_plmn_id_type_t fplmn[NWY_NW_PLMN_LIST_MAX];
}nwy_nw_plmn_list_t;

/******************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************/
typedef void (*nwy_nw_scan_cb_func)
(
    nwy_nw_scan_rsp_list_t *net_list
);

typedef void (*nwy_nw_cb_func)
(
    nwy_nw_regs_ind_type_e ind_type,
    nwy_nw_info_ind_t *ind_p
);


/******************************************************************************
 * 8 Function Declare
******************************************************************************/
/******************************************************************************
*Prototype: nwy_nw_mode_get
*Descroption: get the network mode
*Input: sim_id
*Output:
    network mode: (each bit means a different mode, see nwy_nw_mode_mask_e)
        0x0:                auto mode;
        0x4:  0000 0100     GSM locked;
        0x10: 0001 0000     LTE locked;
        0x14: 0001 0100     LTE+GSM locked;
Note:
1.Only indicates the mode lock status, not the currently registered network.
2.if only support one mode, will always return AUTO.

*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_mode_get(nwy_sim_id_e sim_id,int *p_mode);

/******************************************************************************
*Prototype: nwy_nw_mode_set
*Descroption: set the network mode
*Input:
    sim_id,
    network mode: (each bit means a different mode,see nwy_nw_mode_mask_e)
        0x0:                set auto mode;
        0x4:  0000 0100     lock GSM;
        0x10: 0001 0000     lock LTE;
        0x14: 0001 0100     lock LTE+GSM
Note:
if only support one mode, will do nothing and return NWY_SUCCESS.

*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_mode_set(nwy_sim_id_e sim_id,int mode);

/******************************************************************************
*Prototype: nwy_nw_lock_get
*Descroption: lock band/freq/pci
*Input:
    sim_id;
    lock_info: According to the struct
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_lock_get(nwy_sim_id_e simid,nwy_nw_lock_t* p_lock_info);

/******************************************************************************
*Prototype: nwy_nw_lock_get
*Descroption: lock band/freq/pci
*Input:
    sim_id;
    lock_info: According to the struct
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_lock_set(nwy_sim_id_e simid,nwy_nw_lock_t* p_lock_info);

/******************************************************************************
*Prototype: nwy_nw_priband_set
*Descroption: set the priband
*Input: sim_id
*Output: priband info: band priority
Note: for some customer may set LTE+NR priband at the same time, please input the
    mode while get info; the api support getting priband of one netmode each time.

*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_priband_get(nwy_sim_id_e simid,nwy_nw_priband_t* p_priband_info);

/******************************************************************************
*Prototype: nwy_nw_priband_set
*Descroption: set the priband
*Input:
    sim_id,
    priband info: band priority
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_priband_set(nwy_sim_id_e simid,nwy_nw_priband_t* p_priband_info);

/******************************************************************************
*Prototype: nwy_nw_ims_get
*Descroption: get the ims_state
*Input: sim_id,ims_state
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_ims_get(nwy_sim_id_e simid, uint8* p_ims_state);

/******************************************************************************
*Prototype: nwy_nw_ims_set
*Descroption: set the ims_state
*Input: sim_id,ims_state
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_ims_set(nwy_sim_id_e simid, uint8 ims_state);

/******************************************************************************
*Prototype: nwy_nw_radio_get
*Descroption: get radio mode
*Input: NA
*Output: fun: radio mode
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_radio_get (nwy_nw_radio_mode_e *p_radio_mode);

/******************************************************************************
*Prototype: nwy_nw_radio_set
*Descroption: get radio mode
*Input: fun: radio mode
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_radio_set (nwy_nw_radio_mode_e radio_mode);

/******************************************************************************
*Prototype: nwy_nw_edrx_get
*Descroption: get the edrx info(edrx: a low power technology)
*Input: sim_id
*Output: edrx_info: please according to the struct.
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_edrx_get(nwy_sim_id_e simid, nwy_nw_edrx_info_t * p_edrx_info);

/******************************************************************************
*Prototype: nwy_nw_edrx_get
*Descroption: get the edrx info(edrx: a low power technology)
*Input:
    sim_id,
    edrx_info: please according to the struct.
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_edrx_set(nwy_sim_id_e simid, nwy_nw_edrx_info_t * p_edrx_info);

/******************************************************************************
*Prototype: nwy_nw_psm_get
*Descroption: get the psm info(psm: a low power technology)
*Input: sim_id
*Output: psm_info: please according to the struct.
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_psm_get(nwy_sim_id_e simid, nwy_nw_psm_info_t * p_psm_info);

/******************************************************************************
*Prototype: nwy_nw_psm_get
*Descroption: get the psm info(psm: a low power technology)
*Input:
    sim_id,
    psm_info: please according to the struct.
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_psm_set(nwy_sim_id_e simid, nwy_nw_psm_info_t * p_psm_info);

/******************************************************************************
*Prototype: nwy_nw_config_get
*Descroption: get the customized features info
*Input:
    sim_id,
    config_type: customized features type
*Output: config_info: according to the union
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_config_get(nwy_sim_id_e simid, nwy_nw_config_type_e config_type, nwy_nw_config_info_u * p_config_info);

/******************************************************************************
*Prototype: nwy_nw_config_set
*Descroption: set the customized features info
*Input:
    sim_id,
    config_type: customized features type
    config_info: according to the union
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_config_set(nwy_sim_id_e simid, nwy_nw_config_type_e config_type, nwy_nw_config_info_u * p_config_info);

/******************************************************************************
*Prototype: nwy_nw_operator_get
*Descroption: get the operator info
*Input: sim_id
*Output: opt_info: operator info
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_operator_get(nwy_sim_id_e simid,nwy_nw_operator_t *p_opt_info);

/******************************************************************************
*Prototype: nwy_nw_csq_get
*Descroption: get the csq
*Input: sim_id
*Output: csq_val
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_csq_get(nwy_sim_id_e simid, nwy_nw_get_csq_info_t* csq_val);

/******************************************************************************
*Prototype: nwy_nw_creg_get
*Descroption: get the creg
*Input: sim_id
*Output: creg_val
*Return value: nwy_error_e
*Author: yangtianhan
******************************************************************************/
nwy_error_e nwy_nw_creg_get(nwy_sim_id_e simid, uint8 *p_creg_val);


/******************************************************************************
*Prototype: nwy_nw_signal_get
*Descroption: get the signal info
*Input: sim_id
*Output: signal_info: according to the struct.
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_signal_get(nwy_sim_id_e simid,nwy_nw_signal_info_t* p_signal_info);

/******************************************************************************
*Prototype: nwy_nw_regstatus_get
*Descroption: get the regstatus
*Input: sim_id
*Output:
    reg_info:
        cs_regs: cs domain reg info;
        ps_regs: ps domain reg info;
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_regstatus_get(nwy_sim_id_e simid, nwy_nw_regstatus_t *p_regs_info);

/******************************************************************************
*Prototype: nwy_nw_cellinfo_get
*Descroption: get the cellinfo
*Input:
        sim_id,
        cellinfo_mode: For most plat, obtaining cell_info only obtains stored scanning
                       results from the modem , does not trigger scanning actions.
                       i.for scell mode: quickly return, most block 1s;
                       ii.for all_cell_mode: block func,most block 5s.
*Output: celll_info: According to the struct
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_cellinfo_get(nwy_sim_id_e simid, nwy_nw_cellinfo_mode_e cellinfo_mode, nwy_nw_cellinfo_t *p_cellinfo_info);

/******************************************************************************
*Prototype: nwy_nw_scan
*Descroption: get surrounding net info
*Input: sim_id
*Output:
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_scan(nwy_sim_id_e simid, nwy_nw_scan_cb_func scan_cb);

/******************************************************************************
*Prototype: nwy_nw_manual_select
*Descroption: manual select surrounding net
*Input:
    sim_id,
    net_select: the selected net info
*Output: NA
*Return value: nwy_error_e
*Author: huangsangen
******************************************************************************/
nwy_error_e nwy_nw_manual_select(nwy_sim_id_e simid, nwy_nw_select_param_t *p_net_select);

nwy_error_e nwy_nw_reg_cb(nwy_nw_cb_func cb);
nwy_error_e nwy_nw_unreg_cb(nwy_nw_cb_func cb);

/******************************************************************************
*Prototype: nwy_nw_set_cfg_auto_search
*Descroption: set cfg_auto_search
*Input:
    sim_id,
    auto_cfg: nwy_nw_cfg_auto_search_t
*Output: NA
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/
nwy_error_e nwy_nw_set_cfg_auto_search(nwy_sim_id_e simid,nwy_nw_cfg_auto_search_t auto_cfg);

/******************************************************************************
*Prototype: nwy_nw_get_cfg_auto_search
*Descroption: get cfg_auto_search
*Input:sim_id,
*Output:auto_cfg: nwy_nw_cfg_auto_search_t
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/

nwy_error_e nwy_nw_get_cfg_auto_search(nwy_sim_id_e simid,nwy_nw_cfg_auto_search_t *auto_cfg);
/******************************************************************************
*Prototype: nwy_nw_set_urcsysinfo_cfg
*Descroption: set urcsysinfo_cfg
*Input:sim_id,urcsysinfo_cfg 0/1
*Output:NULL
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/

nwy_error_e nwy_nw_set_urcsysinfo_cfg(nwy_sim_id_e simid,int urcsysinfo_cfg);

/******************************************************************************
*Prototype: nwy_nw_get_urcsysinfo_cfg
*Descroption: get urcsysinfo_cfg
*Input:sim_id,
*Output:urcsysinfo_cfg 0/1
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/

nwy_error_e nwy_nw_get_urcsysinfo_cfg(nwy_sim_id_e simid,int *urcsysinfo_cfg);

/******************************************************************************
*Prototype: nwy_nw_set_sysinfourc_cfg
*Descroption: set sysinfourc_cfg
*Input:sim_id,sysinfourc_cfg 0/1
*Output:NULL
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/

nwy_error_e nwy_nw_set_sysinfourc_cfg(nwy_sim_id_e simid,int sysinfourc_cfg);

/******************************************************************************
*Prototype: nwy_nw_get_sysinfourc_cfg
*Descroption: get sysinfourc_cfg
*Input:sim_id,
*Output:sysinfourc_cfg 0/1
*Return value: nwy_error_e
*Author: cuilisha
******************************************************************************/

nwy_error_e nwy_nw_get_sysinfourc_cfg(nwy_sim_id_e simid,int *sysinfourc_cfg);

nwy_error_e  nwy_nw_rftest_set(nwy_rftest_val_type * rftest_value);
nwy_error_e  nwy_nw_ftmmode_set(nwy_sim_id_e simid,int ftm_mode);
nwy_error_e  nwy_nw_ftmmode_get(nwy_sim_id_e simid,int * ftm_mode);
nwy_error_e nwy_nw_rftest_get(nwy_rftest_val_type * rftest_value);
nwy_error_e nwy_nw_rftest_pl_get_rxpower(int *rxpower);


nwy_error_e  nwy_nw_cellscan(nwy_sim_id_e simid, nwy_nw_cell_scan_list_t *p_cellscan_info);
nwy_error_e  nwy_nw_cellscanex(nwy_sim_id_e simid, nwy_nw_cell_scan_list_ex_t *p_cellscan_info);

/******************************************************************************
*Prototype: nwy_nw_ncell_earfcn_get
*Descroption: get necll earfcn list
*Output:necll earfcn list
*Return value: nwy_error_e
*Author: liucheng
******************************************************************************/
nwy_error_e nwy_nw_ncell_earfcn_get(char *earfcn);
int nwy_set_sys_auto_mode(void);

/******************************************************************************
*Prototype: nwy_nw_get_forbidden_plmn
*Descroption: get sim FPLMN list
*Input: sim_id,ims_state
*Output: nwy_nw_fplmn_list_t* fplmn_list
*Return value: nwy_error_e
*Author: wangchen
******************************************************************************/
nwy_error_e nwy_nw_get_forbidden_plmn(nwy_sim_id_e simid, nwy_nw_plmn_list_t* fplmn_list);

/******************************************************************************
*Prototype: nwy_get_time_zone_switch
*Descroption: get time_zone_switch
*Input: nwy_time_zone_switch_e *status
*Output: nwy_time_zone_switch_e *status
*Return value: nwy_error_e
*Author: zhaoyating
******************************************************************************/

nwy_error_e nwy_get_time_zone_switch(nwy_time_zone_switch_e *status);

/******************************************************************************
*Prototype: nwy_set_time_zone_switch
*Descroption: get time_zone_switch
*Input: nwy_time_zone_switch_e *status
*Output: nwy_time_zone_switch_e *status
*Return value: nwy_error_e
*Author: zhaoyating

******************************************************************************/

nwy_error_e nwy_set_time_zone_switch(nwy_time_zone_switch_e status);
nwy_error_e nwy_get_nwtms(int *enable);
nwy_error_e nwy_set_nwtms(int enable);
nwy_error_e nwy_get_blackcell(nwy_nw_rat_type_e rat,nwy_nw_blackcell_list_t *p_blackcell_info);
nwy_error_e nwy_set_blackcell(nwy_nw_blackcell_action_e action_type,nwy_nw_rat_type_e rat,nwy_nw_blackcell_list_t *p_blackcell_info);
nwy_error_e nwy_set_power_backoff(nwy_nw_power_backoff* p_power_backoff);
nwy_error_e nwy_get_power_backoff(nwy_nw_power_backoff p_power_backoff[],int *total_num);


#ifdef __cplusplus
   }
#endif

#endif
