#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_ssl_config.h"
void nwy_test_cli_get_sslinfo(void)
{
    nwy_ssl_version_info_t info;
    int ret = 0;
    memset(&info,0x00,sizeof(info));
    ret =nwy_ssl_version_info_get(&info);
    if (ret == NWY_SUCCESS) {
        nwy_test_cli_echo("\r\nMbed TLS Version: %s\r\n", info.version_string);
        nwy_test_cli_echo("\r\nSupported TLS Versions: 0x%02X\n", info.tls_version);
        nwy_test_cli_echo("\r\nSupported DTLS Versions: 0x%02X\n", info.dtls_version);

        if (info.tls_version & (1 << 0)) nwy_test_cli_echo("  - SSL 3.0\n");
        if (info.tls_version & (1 << 1)) nwy_test_cli_echo("  - TLS 1.0\n");
        if (info.tls_version & (1 << 2)) nwy_test_cli_echo("  - TLS 1.1\n");
        if (info.tls_version & (1 << 3)) nwy_test_cli_echo("  - TLS 1.2\n");
        if (info.tls_version & (1 << 4)) nwy_test_cli_echo("  - TLS 1.3\n");
        if (info.dtls_version & (1 << 0)) nwy_test_cli_echo("  - DTLS 1.0\n");
        if (info.dtls_version & (1 << 1)) nwy_test_cli_echo("  - DTLS 1.2\n");
    }
    else
    {
        nwy_test_cli_echo("Failed to get version information: %d\n", ret);
    }
}


