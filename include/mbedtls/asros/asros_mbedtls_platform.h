/**
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */


#ifndef __PLATFORM_MBED__H__
#define __PLATFORM_MBED__H__

#include "mbedtls/build_info.h"

#if (defined(TARGET_PSA) && defined(MBEDTLS_ENTROPY_NV_SEED))

#include "default_random_seed.h"

#if !defined(MBEDTLS_PLATFORM_NV_SEED_READ_MACRO)
#define MBEDTLS_PLATFORM_NV_SEED_READ_MACRO mbed_default_seed_read
#endif

#if !defined(MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO)
#define MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO mbed_default_seed_write
#endif

/* Automatically enable the Mbed Crypto entropy injection API if
 * MBEDTLS_ENTROPY_NV_SEED is enabled. */
#define MBEDTLS_PSA_INJECT_ENTROPY

#endif  // (defined(TARGET_PSA) && defined(MBEDTLS_ENTROPY_NV_SEED))

#if DEVICE_TRNG
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#endif

#if defined(MBEDTLS_CONFIG_HW_SUPPORT)
#include "mbedtls_device.h"
#endif

#if defined(TARGET_PSA)
/* The following configurations are a needed for Mbed Crypto submodule.
 * They are related to the persistent key storage feature.
 */
#define MBEDTLS_PSA_CRYPTO_STORAGE_C
#define MBEDTLS_PSA_CRYPTO_STORAGE_ITS_C
#undef MBEDTLS_PSA_CRYPTO_STORAGE_FILE_C
#endif

/*
 * MBEDTLS_ERR_PLATFORM_HW_FAILED is deprecated and should not be used.
 */
#define MBEDTLS_ERR_PLATFORM_HW_FAILED       -0x0080
#define MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED -0x0070

#if defined(MBEDTLS_TIMING_ALT)
typedef struct mbedtls_timing_delay_context
{
    unsigned int timer; // ticks
    unsigned int int_ms;
    unsigned int fin_ms;
} mbedtls_timing_delay_context;

int mbedtls_timing_get_delay(void *data);
void mbedtls_timing_set_delay(void *data, unsigned int int_ms, unsigned int fin_ms);
#endif

#if defined(MBEDTLS_HAVE_TIME)
#if defined(MBEDTLS_PLATFORM_TIME_ALT)
time_t GetGMTTimeSecond(void);
static time_t mbedtls_gmt_time ( time_t* time )
{
    return GetGMTTimeSecond();
}
#define MBEDTLS_PLATFORM_STD_TIME	mbedtls_gmt_time
#endif
#endif

#endif  // __PLATFORM_MBED__H__
