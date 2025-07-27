/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
    Copyright (c) 2018 Neoway Technologies, Inc.
    All rights reserved.
    Confidential and Proprietary - Neoway Technologies, Inc.
    Author: gaohe
    Date: 2023/5
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __NWY_SIM_API_H__
#define __NWY_SIM_API_H__

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
*/

#include "nwy_common.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
*/



/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */

typedef enum
{
    NWY_SIM_NOT_INSERTED,
    NWY_SIM_READY,
    NWY_SIM_PIN_REQ,
    NWY_SIM_PUK_REQ,
    NWY_SIM_BUSY,
    NWY_SIM_BLOCKED,
    NWY_SIM_UNKNOWN
}nwy_sim_status_e;

typedef enum
{
    NWY_SIM_PIN_MODE_DISABLED,
    NWY_SIM_PIN_MODE_ENABLED,
    NWY_SIM_PIN_MODE_UNKOWN
}nwy_sim_pin_mode_e;

typedef enum
{
    NWY_SIM_ID_SLOT_1,
    NWY_SIM_ID_SLOT_2,
    NWY_SIM_ID_SLOT_3,
    NWY_SIM_ID_SLOT_4,
    NWY_SIM_ID_MAX
}nwy_sim_id_e;

typedef enum
{
    NWY_SIM_DETECT_TYPE_DISABLE,
    NWY_SIM_DETECT_TYPE_ENABLE,
    NWY_SIM_DETECT_TYPE_MAX
}nwy_sim_detect_type_e;

typedef enum
{
    NWY_SIM_DETECT_MODE_LOW,
    NWY_SIM_DETECT_MODE_HIGH,
    NWY_SIM_DETECT_MODE_MAX
}nwy_sim_detect_mode_e;

typedef enum
{
    NWY_SIM_CFG_TYPE_NOT_SAVE,
    NWY_SIM_CFG_TYPE_SAVE,
    NWY_SIM_CFG_TYPE_MAX
}nwy_sim_cfg_type_e;

typedef enum
{
    NWY_SIM_URC_TYPE_STATUS,
    NWY_SIM_URC_TYPE_MAX
}nwy_sim_urc_type_e;

typedef enum
{
    NWY_SIM_WRITE_COUNT_DISABLE,
    NWY_SIM_WRITE_COUNT_ENABLE,
    NWY_SIM_WRITE_COUNT_CLEAR
}nwy_sim_write_count_ctl_e;

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

typedef struct {
    char mcc[4];
    char mnc[4];
    int  mnc_len;
}nwy_sim_plmn_t;

typedef struct {
    char text[32];
    char msisdn[21];
    int  msisdn_type;
}nwy_sim_msisdn_t;

typedef struct {
    int list_num;
    nwy_sim_msisdn_t msisdn_list[2];
}nwy_sim_msisdn_list_t;

typedef  struct{
    int sim_detect_status;
    nwy_sim_status_e sim_status;
}nwy_sim_info_ind_t;

typedef void (*nwy_sim_evt_handler)
(
    nwy_sim_id_e sim_id,
    nwy_sim_urc_type_e urc_type,
    nwy_sim_info_ind_t *ind_struct
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
* Prototype     : nwy_dm_active_card_get
* Description   : Get active card
* Input         : card
* Output        : NA
* Return Value  : nwy_error_e
* Author        : yangtianhan
*****************************************************************************
*/
nwy_error_e nwy_sim_active_card_get(int* card);

/*
 *****************************************************************************
 * Prototype	 : nwy_sim_imsi_get
 * Description	 : get sim imsi
 * Input		 : sim_id: operate which sim, imsi_len: input buf size
 * Output		 : imsi_buf: imsi
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_imsi_get(nwy_sim_id_e sim_id, char* imsi_buf, uint8_t imsi_len);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_iccid_get
 * Description	 : get sim iccid
 * Input		 : sim_id: operate which sim, iccid_len: input buf size
 * Output		 : iccid_buf: iccid
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_iccid_get(nwy_sim_id_e sim_id, char* iccid_buf, uint8_t iccid_len);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_status_get
 * Description	 : get sim status
 * Input		 : sim_id: operate which sim
 * Output		 : status: sim status
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_status_get(nwy_sim_id_e sim_id, nwy_sim_status_e *status);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_verify
 * Description	 : input pin code to enable sim
 * Input		 : sim_id: operate which sim, pin: input pin code
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_verify(nwy_sim_id_e sim_id, char* pin);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_unlock
 * Description	 : input puk code to unlock sim puk and set new pin code
 * Input		 : sim_id: operate which sim, puk: puk code, pin: new pin code
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_unlock(nwy_sim_id_e sim_id, char* puk, char* new_pin);

/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_mode_get
 * Description	 : get pin lock if enable
 * Input		 : sim_id: operate which sim
 * Output		 : pin_mode: pin lock if enable
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_mode_get(nwy_sim_id_e sim_id, nwy_sim_pin_mode_e *pin_mode);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_enable
 * Description	 : enable pin lock
 * Input		 : sim_id: operate which sim, pin: pin code
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_enable(nwy_sim_id_e sim_id, char* pin);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_disable
 * Description	 : disable pin lock
 * Input		 : sim_id: operate which sim, pin: pin code
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_disable(nwy_sim_id_e sim_id, char* pin);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_change
 * Description	 : change pin code
 * Input		 : sim_id: operate which sim, old_pin: pin code, new_pin: new pin code
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_change(nwy_sim_id_e sim_id, char* old_pin, char* new_pin);

/*
 *****************************************************************************
 * Prototype	 : nwy_sim_pin_times_get
 * Description	 : get pin and puk remaining times
 * Input		 : sim_id: operate which sim
 * Output		 : pin_times: remaining times, puk_times: remaining times
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_pin_times_get(nwy_sim_id_e sim_id, uint8_t *pin_times, uint8_t *puk_times);


/*
 *****************************************************************************
 * Prototype	 : nwy_sim_msisdn_get
 * Description	 : get sim msisdn
 * Input		 : sim_id: operate which sim, msisdn_len: input buf size
 * Output		 : msisdn_buf: msisdn buf
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_msisdn_get(nwy_sim_id_e sim_id, char* msisdn_buf, uint8_t msisdn_len);

/*
 *****************************************************************************
 * Prototype	 : nwy_sim_msisdn_get_ext
 * Description	 : get sim msisdn
 * Input		 : sim_id: operate which sim, msisdn_size: input struct size
 * Output		 : msisdn_t: msisdn struct
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e  nwy_sim_msisdn_get_ext(nwy_sim_id_e sim_id, nwy_sim_msisdn_list_t *msisdn_t);

/*
 *****************************************************************************
 * Prototype	 : nwy_sim_slot_set
 * Description	 : set sim slot
 * Input		 : sim_id: set to which sim
                   save: whether reboot save, different platforms have different support situations, reference API doc
 * Output		 : NULL
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_slot_set(nwy_sim_id_e sim_id, nwy_sim_cfg_type_e save);
/*
 * Prototype	 : nwy_sim_slot_get
 * Description	 : get sim slot
 * Input		 : null
 * Output		 : sim_id: which sim slot used
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_slot_get(nwy_sim_id_e *sim_id);

/*
 * Prototype	 : nwy_sim_check_if_switch_reboot_save
 * Description	 : check_if_switch_reboot_save
 * Input		 : NULL
 * Output		 : save: nwy_sim_cfg_type_e
 * Return Value  : nwy_error_e
 * Author		 : cuilisha
 *****************************************************************************
*/
nwy_error_e nwy_sim_check_if_switch_reboot_save(nwy_sim_cfg_type_e *save);

/*
 * Prototype	 : nwy_sim_detect_set
 * Description	 : set sim detect on/off, Detection level
 * Input		 : sim_id: operate which sim, detect_enable: detect on/off, detect_mode: detect level
 * Output		 : NULL
 * Return Value  : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_detect_set(nwy_sim_id_e sim_id, nwy_sim_detect_type_e detect_enable, nwy_sim_detect_mode_e detect_mode);
/*
 * Prototype	 : nwy_sim_detect_get
 * Description	 : get sim detect on/off, Detection level
 * Input		 : sim_id: operate which sim
 * Output		 : detect_enable: detect on/off, detect_mode: detect level
 * Return Value  : nwy_error_e
 * Author		 : cuilisha
 *****************************************************************************
*/
nwy_error_e nwy_sim_detect_get(nwy_sim_id_e sim_id, nwy_sim_detect_type_e *detect_enable, nwy_sim_detect_mode_e *detect_mode);

/*
 * Prototype     : nwy_sim_check_if_support_sim_detect
 * Description   : check_if_sim_id_support
 * Input         : nwy_sim_id_e sim_id
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : cuilisha
 *****************************************************************************
*/
nwy_error_e nwy_sim_check_if_support_sim_detect(nwy_sim_id_e sim_id);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_reset
 * Description   : sim soft reset
 * Input         : nwy_sim_id_e sim_id
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : gaohe
 *****************************************************************************
*/
nwy_error_e  nwy_sim_reset(nwy_sim_id_e sim_id);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_csim
 * Description   : sim generic Access
 * Input         : nwy_sim_id_t sim_id, char *indata, int indata_len
 * Output        : char *outdata, int outdata_len
 * Return Value  : nwy_error_e
 * Author        : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_csim(nwy_sim_id_e sim_id, char *indata, int indata_len, char *outdata, int outdata_len);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_urc_reg
 * Description   : reg sim urc
 * Input         : nwy_sim_id_e sim_id nwy_sim_evt_handler sim_reg_func
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_urc_reg(nwy_sim_id_e sim_id, nwy_sim_evt_handler sim_reg_func);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_urc_unreg
 * Description   : unreg sim urc
 * Input         : nwy_sim_id_e sim_id nwy_sim_evt_handler sim_reg_func
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sim_urc_unreg(nwy_sim_id_e sim_id, nwy_sim_evt_handler sim_reg_func);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_power_set
 * Description   : sim spower mode set
 * Input         : nwy_sim_id_e sim_id,int power_set
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : gaohe
 *****************************************************************************
*/
nwy_error_e  nwy_sim_power_set(nwy_sim_id_e sim_id, int power_set);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_write_count_set
 * Description   : set sim write count switch
 * Input         : nwy_sim_id_e sim_id,nwy_sim_write_count_ctl_e ctl
 * Output        : NULL
 * Return Value  : nwy_error_e
 * Author        : wangchen
 *****************************************************************************
*/
nwy_error_e  nwy_sim_write_count_set(nwy_sim_id_e sim_id, nwy_sim_write_count_ctl_e ctl);

/*
 *****************************************************************************
 * Prototype     : nwy_sim_write_count_get
 * Description   : get sim write count
 * Input         : nwy_sim_id_e sim_id
 * Output        : int *count
 * Return Value  : nwy_error_e
 * Author        : wangchen
 *****************************************************************************
*/
nwy_error_e  nwy_sim_write_count_get(nwy_sim_id_e sim_id, unsigned long *count);

#ifdef __cplusplus
   }
#endif


#endif
