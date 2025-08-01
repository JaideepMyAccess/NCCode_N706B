

/*
 *****************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_data_test.h
 * Author       : hujun
 * Created      : 2017-3-29
 * Description  : osi API ��������
 *
 *****************************************************************************
 */

#ifndef _NWY_DATA_TEST_H_
#define _NWY_DATA_TEST_H_

/*
 *****************************************************************************
 * 1 Other Header File Including
 *****************************************************************************
 */
#include "nwy_demo_utility.h"


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
/*
 *****************************************************************************
 * 3 Enum Type Definition
 *****************************************************************************
 */



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
nwy_error_e nwy_vir_at_test_init();
nwy_error_e nwy_vir_at_send(char *at_command, char *ok_fmt, char *err_fmt, char *resp, int len);


#ifdef __cplusplus
}
#endif


#endif








