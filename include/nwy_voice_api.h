/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
    Copyright (c) 2018 Neoway Technologies, Inc.
    All rights reserved.
    Confidential and Proprietary - Neoway Technologies, Inc.
    Author: dongnengxiang
    Date: 2025/02
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __NWY_VOICE_API_H__
#define __NWY_VOICE_API_H__
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

#define NWY_VOICE_MAX_ADDR_LENGTH   21    // Maximum string length. */

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
typedef enum
{
    NWY_SOURCE_VOICE_IND    = 1,
    NWY_SOURCE_SMS_IND      = 2,
    NWY_SOURCE_DATA_IND     = 3,
    NWY_SOURCE_DEFAULT_IND  = 4,
}nwy_source_event_e;

typedef enum
{
    NWY_VOICE_CALL_PROCEEDING = 1,
    NWY_VOICE_ALERTING        = 2,
    NWY_VOICE_CONNECTED       = 3,
    NWY_VOICE_RELEASED        = 4,
    NWY_VOICE_INCOMMING       = 5,
    NWY_VOICE_WAITING         = 6,
    NWY_VOICE_HOLD            = 7,
    NWY_VOICE_RETRIEVE        = 8,
} nwy_voice_mo_state_e;


typedef enum
{
    NWY_VOICE_STATE_INCOMING = 1,
    NWY_VOICE_STATE_DIALING  = 2,
    NWY_VOICE_STATE_ALERTING  = 3,
    NWY_VOICE_STATE_ACTIVE   = 4,
    NWY_VOICE_STATE_HOLDING  = 5,
    NWY_VOICE_STATE_END      = 6,
    NWY_VOICE_STATE_WAITING  = 7,
} nwy_voice_call_state_e;

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
typedef struct nwy_voice_info_type{
    char phone_num[NWY_VOICE_MAX_ADDR_LENGTH];
    nwy_voice_call_state_e voice_state;
}nwy_voice_msg_t;

typedef void (*nwy_voice_evt_handler)
(
    nwy_sim_id_e sim_id,
    nwy_source_event_e urc_type,
    nwy_voice_msg_t *ind_struct
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
 * Prototype	 : nwy_voice_recv_cb_reg
 * Description	 : voice reg evt handler func pointer
 * Input		 : sim_id: operate which sim, evt_handler:voice handler function pointer, contextPtr: reserve
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : dongnengxiang
 *****************************************************************************
*/
nwy_error_e nwy_voice_urc_reg(nwy_sim_id_e sim_id, nwy_voice_evt_handler voice_reg_func);

/*
 *****************************************************************************
 * Prototype	 : nwy_voice_recv_cb_unreg
 * Description	 : voice unreg evt handler func pointer
 * Input		 : sim_id: operate which sim, evt_handler:voice handler function pointer
 * Output		 : None
 * Return Value  : nwy_error_e
 * Author		 : dongnengxiang
 *****************************************************************************
*/

nwy_error_e nwy_voice_urc_unreg(nwy_sim_id_e sim_id, nwy_voice_evt_handler voice_reg_func);


#ifdef __cplusplus
   }
#endif

#endif

