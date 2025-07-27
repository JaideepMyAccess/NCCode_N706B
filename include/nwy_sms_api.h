/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
    Copyright (c) 2018 Neoway Technologies, Inc.
    All rights reserved.
    Confidential and Proprietary - Neoway Technologies, Inc.
    Author: gaohe
    Date: 2023/05
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __NWY_SMS_API_H__
#define __NWY_SMS_API_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */

#include <stdint.h>
#include "nwy_sim_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */

#define NWY_SMS_MAX_MO_MSG_LENGTH 512  // Maximum length of an MO SMS. (8bit-MAX-140,7bit-MAX-160,UCS2-MAX_280)*/
#define NWY_SMS_MAX_MT_MSG_LENGTH 160   // Maximum length of an SMS. */
#define NWY_SMS_MAX_ADDR_LENGTH   21    // Maximum string length. */
#define NWY_SMS_MAX_DATE_LENGTH   40    // Maximum string length. */
#define NWY_SMS_MAX_PDU_LENGTH    255   // Maximum string length. */
#define NWY_MAX_MSG_LIST_LEN (255)
#define NWY_SMS_MESSAGE_CLASS_LEN 6

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
    NWY_SMS_MSG_FORMAT_GSM7 = 1,
    NWY_SMS_MSG_FORMAT_8BIT = 2,
    NWY_SMS_MSG_FORMAT_UCS2 = 3,
}nwy_sms_msg_format_type_e;

typedef enum
{
    NWY_SMS_ENCODING_NONE = 0,
    NWY_SMS_ENCODING_GBK = 1,
} nwy_sms_context_decode_type_e;

typedef enum
{
    NWY_SMS_PP_IND      = 0,// PP msg
    NWY_SMS_CB_IND      = 1,// CB msg
    NWY_SMS_CB_CMAS_IND = 2,// CB CMAS msg
    NWY_SMS_SEND_IND = 3,// CB CMAS msg
}nwy_mt_sms_event_e;

typedef enum
{
    NWY_SMS_TRANSFER_UNKNOWN = -1,
    NWY_SMS_TRANSFER_AND_STORE,
    NWY_SMS_TRANSFER_ONLY,
} nwy_sms_transfer_type_e;

typedef enum
{
    NWY_SMS_TRANSFER_ONLINE_UNKNOWN = -1, 
    NWY_SMS_TRANSFER_ONLINE_DISCARD = 1,
    NWY_SMS_TRANSFER_ONLINE_CACHE,
    NWY_SMS_TRANSFER_ONLINE_DIRECT,
} nwy_sms_transfer_online_mode_e;

typedef enum
{
    NWY_SMS_STORAGE_TYPE_UIM, 
    NWY_SMS_STORAGE_TYPE_NV, 
    NWY_SMS_STORAGE_TYPE_NONE = -1, 
}nwy_sms_storage_type_e;

typedef enum
{
  NWY_SMS_MSG_DFLAG_NONE = 0,
  NWY_SMS_MSG_DFLAG_READ = 1,
  NWY_SMS_MSG_DFLAG_READ_SEND = 2,
  NWY_SMS_MSG_DFLAG_READ_SEND_UNSEND = 3,
  NWY_SMS_MSG_DFLAG_ALL = 4,
  NWY_SMS_MSG_DFLAG_MAX,
}nwy_sms_msg_dflag_e;

/*
 *****************************************************************************
 * 4 Global Variable Declaring
 *****************************************************************************
 */


/*
 *****************************************************************************
 * 5 STRUCT Type Definition
 *****************************************************************************
 */
typedef struct nwy_sms_info_type{
    char phone_num[NWY_SMS_MAX_ADDR_LENGTH];
    uint32_t msg_context_len;
    char msg_context[NWY_SMS_MAX_MO_MSG_LENGTH + 1];
    nwy_sms_msg_format_type_e msg_format;
}nwy_sms_info_type_t;

typedef struct nwy_sms_time_stamp
{
    uint16_t uYear;
    uint8_t uMonth;
    uint8_t uDay;
    uint8_t uHour;
    uint8_t uMinute;
    uint8_t uSecond;
    int8_t iZone;
} nwy_sms_time_stamp_t;

typedef struct nwy_sms_recv_info_type{
    uint16_t nIndex;
    nwy_sms_msg_format_type_e msg_format; // Message format.
    uint32_t msg_content_len;
    uint8_t msg_pdu[NWY_SMS_MAX_PDU_LENGTH + 1];
    uint8_t msg_content[NWY_SMS_MAX_MT_MSG_LENGTH + 1];
    char source_phone_num[NWY_SMS_MAX_ADDR_LENGTH + 1]; // Source address.
    nwy_sms_time_stamp_t date;
    nwy_sms_context_decode_type_e context_decode_type; 
    uint8_t msg_decoded_content[NWY_SMS_MAX_MT_MSG_LENGTH + 1];
    nwy_sms_transfer_type_e transfer_type;
    nwy_sms_storage_type_e storage_type;
    nwy_sim_id_e dst_id;
} nwy_sms_recv_info_type_t;  /* Message */

typedef struct {
    uint8_t phone_num[NWY_SMS_MAX_ADDR_LENGTH];
    uint8_t result;
    nwy_sim_id_e dst_id;
} nwy_sms_send_ind_t;

typedef struct {
    char sca[NWY_SMS_MAX_ADDR_LENGTH + 1];
    unsigned char tosca;
} nwy_sms_sca_t;

typedef struct nwy_sms_report_para{
    nwy_sms_transfer_type_e transfer_type;
    nwy_sms_transfer_online_mode_e transfer_online_mode;
} nwy_sms_report_para_t;

typedef struct {
    uint8_t len;
    uint8_t indices[NWY_MAX_MSG_LIST_LEN];
}nwy_sms_msg_list_t; 

typedef uint32_t sms_client_handle_t;

typedef  struct{
    nwy_sms_transfer_type_e transfer_type;
    nwy_sms_recv_info_type_t sms_info;
}nwy_sms_info_ind_t;

typedef void (*nwy_sms_evt_handler)
(
    nwy_sim_id_e sim_id,
    nwy_mt_sms_event_e urc_type,
    nwy_sms_info_ind_t *ind_struct
);

/*
 *****************************************************************************
 * 6 UNION Type Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 7 OTHERS Definition
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 8 Function Declare
 *****************************************************************************
 */

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_option_init
 * Description	 : sms func init
 * Input		 : sim_id: init which sim sms function
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_option_init(nwy_sim_id_e sim_id);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_storage_set
 * Description	 : set operation sms storage location
 * Input		 : sim_id: operate which sim, sms_storage: sms storage
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_storage_set(nwy_sim_id_e sim_id, nwy_sms_storage_type_e sms_storage);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_storage_get
 * Description	 : get  sms storage location
 * Input		 : sim_id: operate which sim
 * Output		 : sms_storage: sms storage
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_storage_get(nwy_sim_id_e sim_id, nwy_sms_storage_type_e *sms_storage);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_report_set
 * Description	 : set sms report mode
 * Input		 : sim_id: operate which sim, report_para: how to handle mt SMS
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_report_set(nwy_sim_id_e sim_id, nwy_sms_report_para_t report_para);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_sca_set
 * Description	 : set sms center
 * Input		 : sim_id: operate which sim, sca: sms center to be set
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_sca_set(nwy_sim_id_e sim_id, nwy_sms_sca_t sca);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_sca_get
 * Description	 : get sms center
 * Input		 : sim_id: operate which sim
 * Output		 : sca: sms center from sim
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_sca_get(nwy_sim_id_e sim_id, nwy_sms_sca_t *sca);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_msg_send
 * Description	 : send sms
 * Input		 : sim_id: operate which sim, p_sms_data: sms info to be send
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_msg_send(nwy_sim_id_e sim_id, nwy_sms_info_type_t *p_sms_data);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_recv_cb_reg
 * Description	 : sms reg evt handler func pointer
 * Input		 : sim_id: operate which sim, evt_handler:sms handler function pointer, contextPtr: reserve
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_recv_cb_reg (nwy_sim_id_e sim_id, nwy_sms_evt_handler evt_handler);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_recv_cb_unreg
 * Description	 : sms unreg evt handler func pointer
 * Input		 : sim_id: operate which sim, evt_handler:sms handler function pointer
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_recv_cb_unreg (nwy_sim_id_e sim_id, nwy_sms_evt_handler evt_handler);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_msg_del
 * Description	 : delete sms by index
 * Input		 : sim_id: operate which sim, index: which sms will be delete
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_msg_del (nwy_sim_id_e sim_id, uint16_t index);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_msg_read
 * Description	 : read single sms
 * Input		 : sim_id: operate which sim, index: which sms to read
 * Output		 : sms_data: sms info
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_msg_read(nwy_sim_id_e sim_id, uint16_t index, nwy_sms_recv_info_type_t *sms_data);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_msg_del_ext
 * Description	 : delete sms by sms type
 * Input		 : sim_id: operate which sim, delflag: which type sms will be delete
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_msg_del_ext(nwy_sim_id_e sim_id, nwy_sms_msg_dflag_e delflag);

/*
 *****************************************************************************
 * Prototype	 : nwy_sms_msg_list_read
 * Description	 : get sms list
 * Input		 : sim_id: operate which sim
 * Output		 : sms_list: sms list(nwy_sms_msg_status_e not all platforms supported)
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sms_msg_list_read(nwy_sim_id_e sim_id, nwy_sms_msg_list_t *sms_list);

#ifdef __cplusplus
   }
#endif

#endif

