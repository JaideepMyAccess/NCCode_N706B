#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_pwm_api.h"

#if defined NWY_OPEN_TEST_PWM && defined FEATURE_NWY_ASR_TEST_TMP
nwy_pwm_t *test_p = NULL;
void nwy_test_cli_pwm_init()
{
    char *opt;
    int pwm_num, freq, duty;

    opt = nwy_test_cli_input_gets("\r\nSet the pwm_id:");
    pwm_num = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the freq:");
    freq = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the duty:");
    duty = atoi(opt);
    switch(pwm_num)
    {
        case 0:
            test_p = nwy_pwm_init(NWY_PWM0, freq, duty);
            break;
        case 1:
            test_p = nwy_pwm_init(NWY_PWM1, freq, duty);
            break;
        case 2:
            test_p = nwy_pwm_init(NWY_PWM2, freq, duty);
            break;
        case 3:
            test_p = nwy_pwm_init(NWY_PWM3, freq, duty);
            break;
    }

    if (test_p == NULL)
    {
        nwy_test_cli_echo("\r\nPWM init failed!\r\n");
    }
    else
        nwy_test_cli_echo("\r\nPWM init success!\r\n");
}

void nwy_test_cli_pwm_start()
{
    nwy_pwm_start(test_p);
    nwy_test_cli_echo("\r\nTest in pwm start!\r\n");
}

void nwy_test_cli_pwm_stop()
{
    nwy_pwm_stop(test_p);
    nwy_test_cli_echo("\r\nTest in pwm stop!\r\n");
}

void nwy_test_cli_pwm_deinit()
{
    nwy_pwm_deinit(test_p);
    nwy_test_cli_echo("\r\nTest in pwm deinit!\r\n");
}
#endif

