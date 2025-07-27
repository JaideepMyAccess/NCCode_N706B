/**
 * @file nwy_wifi_api.h
 * @brief WiFi HAL API declarations
 */

#ifndef __NWY_WIFI_API_H__
#define __NWY_WIFI_API_H__

#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif
#define NWY_WIFI_AP_SUPPORT_MAX_STA_NUM   21
#define NWY_WIFI_HOSTNAME_LEN             64
#define NWY_WIFI_STA_SCAN_LIST_MAX        50

/* WiFi频段定义 */
typedef enum {
    NWY_WIFI_BAND_2G = 1,    // 2.4G频段
    NWY_WIFI_BAND_5G = 2,    // 5G频段  
    NWY_WIFI_BAND_BOTH   // 双频
} nwy_wifi_band_t;

/* WiFi工作模式 */
typedef enum {
    NWY_WIFI_MODE_AP_ONLY = 1,   // 仅AP模式
    NWY_WIFI_MODE_STA_ONLY = 2,  // 仅STA模式
    NWY_WIFI_MODE_AP_STA,    // AP+STA模式
    NWY_WIFI_MODE_AP_AP      // 双AP模式
} nwy_wifi_mode_t;

/* WiFi认证模式 */
typedef enum {
    NWY_WIFI_AUTH_OPEN = 1,                      //Open
    NWY_WIFI_AUTH_WPA_PSK = 2,                       //WPA Personal
    NWY_WIFI_AUTH_WPA2_PSK,                      //WPA2 Personal
    NWY_WIFI_AUTH_WPA_WPA2_PSK,                  //WPA&WPA2 Personal
    NWY_WIFI_AUTH_WPA3_PSK,                      //WPA3 personal
    NWY_WIFI_AUTH_WPA2_WPA3_PSK,                 //WPA2&WPA3 personal
    NWY_WIFI_AUTH_SHARED,                        //Shared(requires WEP)
} nwy_wifi_auth_mode_t;

/* WiFi加密类型 */
typedef enum {
    NWY_WIFI_ENCRY_AUTO = 1,   // 不加密
    NWY_WIFI_ENCRY_TKIP = 2,   // WEP加密
    NWY_WIFI_ENCRY_AES,   // TKIP加密
    NWY_WIFI_ENCRY_WEP,     // AES加密
} nwy_wifi_encrypt_type_t;

/* WiFi状态 */
typedef enum {
    NWY_WIFI_STATUS_UNKNOWN, // 未知状态
    NWY_WIFI_STATUS_DISABLE, // 禁用状态
    NWY_WIFI_STATUS_ENABLE   // 启用状态
} nwy_wifi_status_t;

/* WiFi芯片类型 */
typedef enum {
    NWY_WIFI_CHIP_RTL8189,   // RTL8189芯片
    NWY_WIFI_CHIP_CM256,     // CM256芯片
    NWY_WIFI_CHIP_AIC8800    // AIC8800芯片
} nwy_wifi_chip_type_t;

/* WiFi 802.11协议 */
typedef enum {
    NWY_WIFI_WIRELESS_MODE_80211B = 1,           //IEEE 802.11b
    NWY_WIFI_WIRELESS_MODE_80211BG = 2,              //IEEE 802.11bg
    NWY_WIFI_WIRELESS_MODE_80211BGN,             //IEEE 802.11bgn
    NWY_WIFI_WIRELESS_MODE_80211A,               //IEEE 802.11a
    NWY_WIFI_WIRELESS_MODE_80211AN,              //IEEE 802.11an
    NWY_WIFI_WIRELESS_MODE_80211AC,              //IEEE 802.11ac(Wi-Fi 5)
    NWY_WIFI_WIRELESS_MODE_80211AX              //IEEE 802.11ax(Wi-Fi 6)
} nwy_wifi_protocol_t;

/* WiFi国家码 */
typedef enum {
    NWY_WIFI_COUNTRY_CN = 1,	// 中国
    NWY_WIFI_COUNTRY_US = 2,	// 美国
    NWY_WIFI_COUNTRY_JP,	// 日本
    NWY_WIFI_COUNTRY_EU,	// 欧盟
    NWY_WIFI_COUNTRY_KR,	// 韩国
    NWY_WIFI_COUNTRY_TW,	// 台湾
    NWY_WIFI_COUNTRY_AU,	// 澳大利亚
    NWY_WIFI_COUNTRY_IN,	// 印度
    NWY_WIFI_COUNTRY_GB,	// 英国
    NWY_WIFI_COUNTRY_DE,	// 德国
    NWY_WIFI_COUNTRY_FR,	// 法国
    NWY_WIFI_COUNTRY_IT,	// 意大利
    NWY_WIFI_COUNTRY_ES,	// 西班牙
    NWY_WIFI_COUNTRY_CA,	// 加拿大
    NWY_WIFI_COUNTRY_BR,	// 巴西
    NWY_WIFI_COUNTRY_RU,	// 俄罗斯
    NWY_WIFI_COUNTRY_SG,	// 新加坡
    NWY_WIFI_COUNTRY_MY,	// 马来西亚
    NWY_WIFI_COUNTRY_NZ,	// 新西兰
    NWY_WIFI_COUNTRY_MAX	// 最大值标记
} nwy_wifi_country_code_t;

typedef struct nwy_wifi_ap_sta_list {
    int num;
    struct {
        unsigned char mac[6];
        in_addr_t addr;
        char hostname[NWY_WIFI_HOSTNAME_LEN];
    } sta[NWY_WIFI_AP_SUPPORT_MAX_STA_NUM];
} nwy_wifi_ap_sta_list_t;

typedef enum nwy_wifi_mac_filter_list {
    NWY_MAC_BLACK_LIST  = 1,
    NWY_MAC_WHITE_LIST  = 2
} nwy_wifi_mac_filter_list_t;
typedef enum nwy_wifi_mac_rule_action {
    NWY_MAC_RULE_ADD = 1,
    NWY_MAC_RULE_DEL = 2,
    NWY_MAC_RULE_CLEAN = 3
} nwy_wifi_mac_rule_action_t;

typedef struct nwy_wifi_mac_filter {
    nwy_wifi_mac_rule_action_t action;
    nwy_wifi_mac_filter_list_t mac_acl;//黑白名单控制
    char *mac;
}nwy_wifi_mac_filter_t;

/* WiFi 带宽 */
typedef enum {
    NWY_WIFI_BANDWIDTH_HT20 = 1,
    NWY_WIFI_BANDWIDTH_HT40 = 2,
    NWY_WIFI_BANDWIDTH_HT80
} nwy_wifi_bandwidth_t;

typedef struct nwy_wifi_ap_mac_filter {
    nwy_wifi_mac_filter_list_t mac_acl;//黑白名单控制
    int                      deny_num;
    int                      accept_num;
    char                     deny_list[10][20];
    char                     accept_list[10][20];
}nwy_wifi_ap_mac_filter_t;

/* WiFi单频段配置 */
typedef struct {
    nwy_wifi_mode_t mode;           // 工作模式
    nwy_wifi_protocol_t wireless_mode;//协议版本
    uint8_t channel;                 // 信道
    nwy_wifi_bandwidth_t bandwidth;  // 带宽
    nwy_wifi_auth_mode_t auth_mode;  // 认证模式
    nwy_wifi_encrypt_type_t encrypt; // 加密类型
    char ssid[32];                   // SSID
    char password[64];               // 密码
    bool hidden;                     // 是否隐藏SSID
    uint8_t max_conn;                // 最大连接数
    nwy_wifi_country_code_t code;  // 国家码
    nwy_wifi_ap_mac_filter_t   mac_filter;//mac地址过滤
} nwy_wifi_config_t;

typedef struct nwy_wifi_info {
    nwy_wifi_chip_type_t chip;      // 芯片类型
    nwy_wifi_status_t status;     // WiFi状态
}nwy_wifi_info_t;

/* 芯片驱动操作接口 */
typedef struct {
    // 基本控制
    int (*enable)(uint8_t band_idx);
    int (*disable)(uint8_t band_idx);
    int (*restart)(uint8_t bnad_idx);

    // WiFiAP配置
    int (*set_ap_config)(uint8_t band_idx, nwy_wifi_config_t *config);
    int (*get_ap_config)(uint8_t band_idx, nwy_wifi_config_t *config);

    int (*set_mac_filter)(uint8_t band_idx, nwy_wifi_ap_mac_filter_t * mac_fil);
    int (*get_sta_number)(uint8_t band_idx);
    int (*get_mac_address)(uint8_t band_idx, char * mac_addr);
    int (*get_sta_list)(uint8_t band_idx, nwy_wifi_ap_sta_list_t * sta_list);
} nwy_wifi_ops_t;

int nwy_wifi_get_status(uint8_t band_idx);

/* API函数声明 */
int nwy_wifi_enable(uint8_t band_idx, nwy_wifi_chip_type_t chip);
int nwy_wifi_disable(uint8_t band_idx);
int nwy_wifi_restart(uint8_t band_idx);

int nwy_wifi_set_ap_config(uint8_t band_idx, nwy_wifi_config_t *config);
int nwy_wifi_get_ap_config(uint8_t band_idx, nwy_wifi_config_t *config);

int nwy_wifi_set_mac_filter(uint8_t band_idx, nwy_wifi_mac_filter_t *mac_filter);

int nwy_wifi_get_mac_addr(uint8_t band_idx, char * mac_addr);
int nwy_wifi_get_sta_number(uint8_t band_idx);
int nwy_wifi_get_sta_list(uint8_t band_idx, nwy_wifi_ap_sta_list_t * sta_list);

#ifdef __cplusplus
}
#endif

#endif /* __NWY_WIFI_API_H__ */ 
