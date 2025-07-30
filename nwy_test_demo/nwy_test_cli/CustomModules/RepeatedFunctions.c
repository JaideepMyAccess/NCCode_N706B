#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_cust.h"

extern CurrentTimeString[22];

void FetchCurrentTimeF1(){
    nwy_time_t julian_time = {0};
    char timezone = 0;

    nwy_date_get(&julian_time, &timezone);
    sprintf(CurrentTimeString, "%02d%02d%02d%02d%02d%02d", 
            julian_time.day,  julian_time.mon, julian_time.year % 100,
            julian_time.hour, julian_time.min, julian_time.sec);

            nwy_test_cli_echo("Called FetchCurrentTimeF1() function. Time Fetched: %s\n", CurrentTimeString);
}


void FetchCurrentTimeF2(){
    nwy_time_t julian_time = {0};
    char timezone = 0;

    nwy_date_get(&julian_time, &timezone);
    sprintf(CurrentTimeString, "%02d%02d%02d%02d%02d%02d", 
            julian_time.year, julian_time.mon, julian_time.day,
            julian_time.hour, julian_time.min, julian_time.sec);

            nwy_test_cli_echo("Called FetchCurrentTimeF1() function. Time Fetched: %s\n", CurrentTimeString);
}

time_t get_epoch_from_nwy_time(void)
{
    nwy_time_t julian_time = {0};
    nwy_timeval_t timestamp = {0};
    char timezone = 0;

    // Get current date/time from system
    nwy_date_get(&julian_time, &timezone);

    // Convert using the Neoway SDK API
    if (nwy_date_to_timestamp(&julian_time, &timestamp) != NWY_SUCCESS)
    {
        nwy_test_cli_echo("Failed to convert date to timestamp!\r\n");
        return -1;
    }

    // Optional: Print
    nwy_test_cli_echo("Epoch Time: %ld\r\n", timestamp.tv_sec);

    return timestamp.tv_sec;
}


time_t get_epoch_from_datetime_str(const char *datetime_str)
{
    if (!datetime_str || strlen(datetime_str) != 12)
    {
        nwy_ext_echo("Invalid datetime string!\r\n");
        return -1;
    }

    int dd, mm, yy, hh, mi, ss;
    if (sscanf(datetime_str, "%2d%2d%2d%2d%2d%2d", &dd, &mm, &yy, &hh, &mi, &ss) != 6)
    {
        nwy_ext_echo("Failed to parse datetime string!\r\n");
        return -1;
    }

    // Fill nwy_time_t
    nwy_time_t time_data = {0};
    time_data.year = 2000 + yy;
    time_data.mon  = mm;
    time_data.day  = dd;
    time_data.hour = hh;
    time_data.min  = mi;
    time_data.sec  = ss;

    // Convert to timestamp
    nwy_timeval_t timestamp = {0};
    if (nwy_date_to_timestamp(&time_data, &timestamp) != NWY_SUCCESS)
    {
        nwy_ext_echo("Failed to convert date to timestamp!\r\n");
        return -1;
    }

    // Output log
    nwy_ext_echo("Parsed datetime: %02d-%02d-%04d %02d:%02d:%02d\r\n", dd, mm, 2000 + yy, hh, mi, ss);
    nwy_ext_echo("Epoch time: %ld\r\n", timestamp.tv_sec);

    return timestamp.tv_sec;
}
