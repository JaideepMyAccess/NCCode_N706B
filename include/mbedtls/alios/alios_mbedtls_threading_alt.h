/*
 * Copyright (C) 2018 Alibaba Group Holding Limited
 */

#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#include "mbedtls/build_info.h"

#ifdef MBEDTLS_PLAT_USE_ALIOS

#include <stdlib.h>
#include <aos/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_PLATFORM_C) && \
    defined(MBEDTLS_PLATFORM_MEMORY) && \
    defined(XTENSA_MALLOC_IRAM)
/* Modify for AliOS Things begin. 2019-01-09 */
void * aos_mbedtls_calloc( size_t n, size_t size );
void aos_mbedtls_free( void *ptr );
#define MBEDTLS_PLATFORM_CALLOC_MACRO 	aos_mbedtls_calloc
#define MBEDTLS_PLATFORM_FREE_MACRO 	aos_mbedtls_free
/* Modify for AliOS Things end. 2019-01-09 */
#endif

#if defined(MBEDTLS_THREADING_ALT)
typedef struct
{
    aos_mutex_t mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

void threading_mutex_init( mbedtls_threading_mutex_t *mutex );
void threading_mutex_free( mbedtls_threading_mutex_t *mutex );
int threading_mutex_lock( mbedtls_threading_mutex_t *mutex );
int threading_mutex_unlock( mbedtls_threading_mutex_t *mutex );
#endif

#if defined(MBEDTLS_THREADING_ALT)
#define MBEDTLS_THREADING_ALT_FILE "alios_mbedtls_threading_alt.h"
#else
#define MBEDTLS_THREADING_ALT_FILE "threading_alt.h"
#endif

#define MBEDTLS_TIMING_ALT_FILE "timing_alt.h"

#ifdef __cplusplus
}

#endif

#endif /* MBEDTLS_THREADING_ALT_H */

#endif
