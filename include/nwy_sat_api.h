/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
    Copyright (c) 2018 Neoway Technologies, Inc.
    All rights reserved.
    Confidential and Proprietary - Neoway Technologies, Inc.
    Author: gaohe
    Date: 2023/05
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __NWY_STK_API_H__
#define __NWY_STK_API_H__
/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define STK_REG_MAX 10
/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */
 typedef enum nwy_sat_onoff_type
 {
	 NWY_SAT_TYPE_OFF = 0,
	 NWY_SAT_TYPE_ON = 1,
	 NWY_SAT_TYPE_MAX,
 }nwy_sat_onoff_type_e;
 
 typedef enum nwy_sat_pci_type
 {
	 NWY_SAT_PCITYPE_HANDLE_BY_TE = 0,
	 NWY_SAT_PCITYPE_HANDLE_BY_ME = 1,
	 NWY_SAT_PCITYPE_NO_OTHER,
 }nwy_sat_pci_type_e;

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
 
typedef struct nwy_sat_notify_info_type
{
  nwy_sat_pci_type_e pcitype;
  uint16_t nDataLen;
  char pData[540];
}nwy_sat_notify_info_type_t;

typedef void (*nwy_sat_evt_handler)
(
	nwy_sat_notify_info_type_t *sat_data
);
#define STK_REG_MAX 10
typedef  struct nwy_stk_info{
    int reg_stk_hdl;
    nwy_sat_evt_handler sat_reg_cb_tab[STK_REG_MAX];
}nwy_stk_info_t;

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
 * Prototype	       : nwy_sat_set_on_off
 * Description	 : sat switch
 * Input		       : sim_id: on or off sat
 * Output		 : None
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sat_set_on_off(nwy_sat_onoff_type_e switch_type);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_get_on_off
 * Description	 : get sat state
 * Input		       : sim_id: None
 * Output		 : sat state
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
int nwy_sat_get_on_off(void);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_terminal_response
 * Description	 : sat tr response
 * Input		       : data: 
 * Output		 : 
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e  nwy_sat_terminal_response(char *data);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_envelope_command
 * Description	 : set env command
 * Input		       : data: 
 * Output		 : None
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sat_envelope_command(char *data);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_recv_data
 * Description	 : stkpci msg
 * Input		       : None
 * Output		 : stkpci msg
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
void nwy_sat_recv_data(nwy_sat_notify_info_type_t *sat_data);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_set_profile
 * Description	 : set profile
 * Input		       : data:
 * Output		 : None
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sat_set_profile(char *data);

/*
 *****************************************************************************
 * Prototype	       : nwy_sat_get_profile
 * Description	 : stk get profile
 * Input		       : None
 * Output		 : data
 * Return Value     : nwy_error_e
 * Author		 : gaohe
 *****************************************************************************
*/
nwy_error_e nwy_sat_get_profile(char *data);

#ifdef __cplusplus
   }
#endif

#endif

