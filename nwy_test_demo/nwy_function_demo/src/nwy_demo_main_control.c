#include "nwy_demo_utility.h"

extern nwy_osi_thread_t nwy_main_control_thread;

void nwy_main_data_call_change_proc(int profile_id, nwy_data_call_state_e state)
{
    NWY_SDK_LOG_DEBUG("profile_id = %d  state = %d ...",profile_id, state, 0);

    if (state == NWY_DATA_CALL_CONNECTED) {
        /*start tcp */
        nwy_tcp_cli_demo_main();
    }
}
void nwy_main_control_func(void *param)
{
    nwy_error_e ret = NWY_GEN_E_UNKNOWN;
    nwy_event_msg_t event;

    while (1) {
        memset(&event, 0, sizeof(event));
        ret = nwy_thread_event_wait(nwy_main_control_thread, &event, NWY_OSA_SUSPEND);
        if (ret == NWY_SUCCESS) {
            switch (event.id) {
                case NWY_MAIN_DATA_CALL_CHANGE_MSG:
                    nwy_main_data_call_change_proc(event.param1, event.param2);
                    break;

            }
        }
    }
}




