

/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_data_test.h
 * Author       : hujun
 * Created      : 2017-3-29
 * Description  : osi API º¯ÊýÉùÃ÷
 *
 *****************************************************************************
 */

#ifndef _NWY_DEMO_DATA_H_
#define _NWY_DEMO_DATA_H_

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
 *****************************************************************************
 * 2 Macro Definition
 ****************************************************************************
 */
#define NWY_DEFAULT_SIM_ID                  (0)
#define NWY_PUBLIC_NET_APN_NAME                  ""
#define NWY_PUBLIC_NET_APN_PWD                  ""
#define NWY_DATA_AUTO_CONNET_INTERVAL       2    /* s */
#define NWY_DATA_AUTO_CONNET_TIME           5
#define NWY_DATA_HANDL_COUNT                3

/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */

typedef enum
{
    NWY_DATA_PUBLIC_HANDL = 0,
    NWY_DATA_PRIVATE_HANDL = 1,
    NWY_DATA_HANDL_MAX =2
}nwy_demo_data_handl_e;

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
nwy_error_e nwy_demo_call_info_get(int profile_idx, nwy_data_callinfo_t *info);
void nwy_demo_data_call_clear();
nwy_error_e nwy_demo_data_call_start(void);


#ifdef __cplusplus
}
#endif


#endif








