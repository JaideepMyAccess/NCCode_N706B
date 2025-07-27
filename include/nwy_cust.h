/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 Copyright (C), 2010-2020, Neoway Co., Ltd.

GENERAL DESCRIPTION
  File Name     : nwy_at_act.h
  Version       : Initial Draft
  Author        : shq
  Created       : 2017/12/19
  Last Modified :

FUNCTION LIST

EDIT HISTORY FOR MODULE
  1.Date         : 2017/12/19
    Author       : shq
    Modification : Created file

  All Rights Reserved.
  Neoway Confidential and Proprietary.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef NWY_CUST_H
#define NWY_CUST_H
#include "nwy_baseline_version.h"
/*===========================================================================

                MACROS

===========================================================================*/
#define NWY_MANUFTR "NEOWAY"
#define NWY_HW_VERSION "V1.0"
#define NWY_HWV_TIME "201024" //day|month|year
#define NWY_MODEL "N706B"    /*N706*/
#define NWY_SFT_VERSION "N706B-"NWY_BASELINE_VERSION"-STD-OE_CN1X_ITRI-009"
#define NWY_SFV_TIME "110725" //day|month|year
#define NWY_SOC_MODEL "ASR1605S"
//NWY_OPEN_SDK_VERSION e.g.
//"1.0" -> 0x00010000
//"2.0" -> 0x00020000
//"10.0" -> 0x000A0000
//"10.2" -> 0x000A0002
#define NWY_OPEN_SDK_VERSION 0x00040000
#define NWY_OPEN_SDK_VERSION_STRING NWY_SFT_VERSION
#endif