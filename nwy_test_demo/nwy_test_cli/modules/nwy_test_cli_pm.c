#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_pm_api.h"
#ifdef FEATURE_NWY_ASR_TEST_TMP
void nwy_test_cli_pm_pwr_off()
{
    char *opt;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nChoose the power off mode(1-quickly,2-normal,3-reset):");
    mode = atoi(opt);
    nwy_pm_ctrl(mode);
}
#endif