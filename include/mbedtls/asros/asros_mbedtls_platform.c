#include "stdlib.h"
#include "osa.h"
#include "asros_mbedtls_platform.h"

unsigned long rti_get_current_tick(void);

int mbedtls_hardware_poll(void *data, unsigned char *output, 
                                   unsigned int len, unsigned int *olen)
{
    unsigned int seed = (unsigned int)(rti_get_current_tick() & 0xFFFFFFFFUL);
    unsigned int r;
    int i = 0;

    srand(seed);

    for(i = 0; i < len; i++)
    {
        output[i] = (unsigned char)rand();
    }
    *olen = len;

    return 0;
}

#if defined(MBEDTLS_TIMING_ALT)
static unsigned int mbedtls_timing_get_timer(mbedtls_timing_delay_context *ctx, int reset)
{
    if(reset)
    {
        ctx->timer = OSAGetTicks();
        return 0;
    }
    else
    {
        unsigned int now = OSAGetTicks();
        now = now - ctx->timer;
        return now * 5; // 5ms per tick
    }
}

int mbedtls_timing_get_delay(void *data)
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    unsigned int elapsed_ms;

    if(ctx->fin_ms == 0)
    {
        return -1;
    }

    elapsed_ms = mbedtls_timing_get_timer(ctx, 0);

    if(elapsed_ms >= ctx->fin_ms)
    {
        return 2;
    }

    if(elapsed_ms >= ctx->int_ms)
    {
        return 1;
    }

    return 0;
}

void mbedtls_timing_set_delay(void *data, unsigned int int_ms, 
                                                    unsigned int fin_ms)
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if(fin_ms != 0)
    {
        mbedtls_timing_get_timer(ctx, 1);
    }
}
#endif
