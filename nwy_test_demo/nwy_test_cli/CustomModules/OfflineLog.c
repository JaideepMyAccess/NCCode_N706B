#include "global.h"
#include "nwy_adapt_platform.h"
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

// This file is used to store offline data for napkin.
bool is_napkin_data_available(void)
{
    // char buffer[256] = {0};
    if (!nwy_file_exist("napkinOfflineData")) {
        return false;
    }

    int fdd = nwy_file_open("napkinOfflineData", NWY_RDONLY);
    if (fdd < 0) {
        return false;
    }

    char buffer[500] = {0};
    

    int fddleng = nwy_file_read(fdd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fdd);
    if (fddleng <= 0) {
        nwy_test_cli_echo("napkinOfflineData file is empty or read failed\n");
    }else{
        nwy_test_cli_echo("Napkin Offline content (%d bytes):\n%s\n", fddleng, buffer);  
    }

    return fddleng > 0;
}

void append_napkin_data(const char *json_str)
{
    int fd = nwy_file_open("napkinOfflineData", NWY_AB_MODE);
    if (fd < 0) {
        nwy_test_cli_echo("Error opening napkinOfflineData for appending\n");
        return;
    }

    // Move to end of file before writing
    nwy_file_seek(fd, 0, 2); // SEEK_END
    char line[1024 + 2] = {0};
    snprintf(line, sizeof(line), "%s\n", json_str);

    nwy_file_write(fd, line, strlen(line));
    nwy_file_close(fd);
    nwy_test_cli_echo("Napkin Offline data appended\n");
}


bool get_napkin_data(char *out_json, int max_len)
{
    char buffer[4096] = {0};

    if (!nwy_file_exist("napkinOfflineData")) {
        return false;
    }

    int fd = nwy_file_open("napkinOfflineData", NWY_RDONLY);
    if (fd < 0) {
        return false;
    }

    int len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);
    nwy_test_cli_echo("\n ************************************************************************* \n");
    nwy_test_cli_echo(buffer);
    nwy_test_cli_echo("\n ************************************************************************* \n");

    if (len <= 0) {
        return false;
    }

    // Extract first line
    char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0'; // Terminate after first line
    }

    strncpy(out_json, buffer, max_len - 1);
    out_json[max_len - 1] = '\0';

    return true;
}

bool delete_napkin_data(void)
{
    char buffer[4096] = {0};

    if (!nwy_file_exist("napkinOfflineData")) {
        return false;
    }

    int fd = nwy_file_open("napkinOfflineData", NWY_RDONLY);
    if (fd < 0) {
        return false;
    }

    int len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        return false;
    }

    char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr != NULL) {
        newline_ptr++; // Point to start of next line
    } else {
        newline_ptr = ""; // Only one line
    }

    fd = nwy_file_open("napkinOfflineData",NWY_WRONLY);
    if (fd < 0) {
        return false;
    }

    nwy_test_cli_echo("\nWriting:\n%s\n", newline_ptr);

    // newline_ptr++; 
    int new_len = strlen(newline_ptr);
    // Overwrite file with remaining content
    nwy_file_seek(fd, 0, NWY_SEEK_SET);
    nwy_file_write(fd, newline_ptr, new_len);

    // Truncate to the new length
    nwy_file_fd_trunc(fd, new_len);

    nwy_file_close(fd);

    return true;
}


// For Incinerator
bool is_incinerator_data_available(void)
{
    // char buffer[256] = {0};
    if (!nwy_file_exist("incineratorOfflineData")) {
        return false;
    }

    int fdd = nwy_file_open("incineratorOfflineData", NWY_RDONLY);
    if (fdd < 0) {
        return false;
    }

    char buffer[500] = {0};
    

    int fddleng = nwy_file_read(fdd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fdd);
    if (fddleng <= 0) {
        nwy_test_cli_echo("incineratorOfflineData file is empty or read failed\n");
    }else{
        nwy_test_cli_echo("Incinerator Offline content (%d bytes):\n%s\n", fddleng, buffer);  
    }

    return fddleng > 0;
}

void append_incinerator_data(const char *json_str)
{
    int fd = nwy_file_open("incineratorOfflineData", NWY_AB_MODE);
    if (fd < 0) {
        nwy_test_cli_echo("Error opening incineratorOfflineData for appending\n");
        return;
    }

    // Move to end of file before writing
    nwy_file_seek(fd, 0, 2); // SEEK_END
    char line[1024 + 2] = {0};
    snprintf(line, sizeof(line), "%s\n", json_str);

    nwy_file_write(fd, line, strlen(line));
    nwy_file_close(fd);
    nwy_test_cli_echo("Incinerator Offline data appended\n");
}


bool get_incinerator_data(char *out_json, int max_len)
{
    char buffer[4096] = {0};

    if (!nwy_file_exist("incineratorOfflineData")) {
        return false;
    }

    int fd = nwy_file_open("incineratorOfflineData", NWY_RDONLY);
    if (fd < 0) {
        return false;
    }

    int len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);
    nwy_test_cli_echo("\n ************************************************************************* \n");
    nwy_test_cli_echo(buffer);
    nwy_test_cli_echo("\n ************************************************************************* \n");

    if (len <= 0) {
        return false;
    }

    // Extract first line
    char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0'; // Terminate after first line
    }

    strncpy(out_json, buffer, max_len - 1);
    out_json[max_len - 1] = '\0';

    return true;
}

bool delete_incinerator_data(void)
{
    char buffer[4096] = {0};

    if (!nwy_file_exist("incineratorOfflineData")) {
      
        return false;
    }

    int fd = nwy_file_open("incineratorOfflineData", NWY_RDONLY);
    if (fd < 0) {
        return false;
    }

    int len = nwy_file_read(fd, buffer, sizeof(buffer) - 1);
    nwy_file_close(fd);

    if (len <= 0) {
        return false;
    }

    char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr != NULL) {
        newline_ptr++; // Point to start of next line
    } else {
        newline_ptr = ""; // Only one line
    }

    fd = nwy_file_open("incineratorOfflineData", NWY_WRONLY);
    if (fd < 0) {
        return false;
    }

    nwy_test_cli_echo("\nWriting:\n%s\n", newline_ptr);

    // newline_ptr++; 
    int new_len = strlen(newline_ptr);
    // Overwrite file with remaining content
    nwy_file_seek(fd, 0, NWY_SEEK_SET);
    nwy_file_write(fd, newline_ptr, new_len);

    // Truncate to the new length
    nwy_file_fd_trunc(fd, new_len);

    nwy_file_close(fd);

    return true;
}
