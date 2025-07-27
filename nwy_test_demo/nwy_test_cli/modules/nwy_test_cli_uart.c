#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_uart_api.h"

/**************************UART*********************************/
#if defined NWY_OPENC_TEST_UART && defined FEATURE_NWY_ASR_TEST_TMP
static int hd = -1;
static char* name;

void nwy_test_cli_uart_open()
{
    char *opt;
	char* namelist[3] = {"URT1", "URT2", "URT3"};
    uint8_t port;
	uint32_t baudrate;
	uint8_t flowctrl;
#ifdef NWY_EC618_UART_OPEN_TEST
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(1-URT0, 2-URT1 or 3-URT2):");
#else
     opt = nwy_test_cli_input_gets("\r\nPlease input the uart id(1-URT1, 2-URT2 or 3-URT3):");
#endif
    port = atoi(opt);
    if(port < 1 || port > 3)
    {
        nwy_test_cli_echo("\r\n Input UART id is invalid!");
        return;
    }
    name = namelist[port-1];
    nwy_test_cli_echo("\r\nTest port = %d!\r\n", port); //delet

	opt = nwy_test_cli_input_gets("\r\nPlease input the uart baudrate (4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600):");
	baudrate = atoi(opt);
	nwy_test_cli_echo("\r\n Input UART baudrate = %d!\r\n", baudrate);

	opt = nwy_test_cli_input_gets("\r\nPlease input the uart flowctrl(1-FC_RTSCTS, 2-FC_XONXOFF):");
	flowctrl = atoi(opt);
	nwy_test_cli_echo("\r\n Input UART flowctrl = %d!\r\n", flowctrl);
	
    hd = nwy_uart_open(name, baudrate, flowctrl);
    if (hd < 0)
        nwy_test_cli_echo("\r\nTest uart error!\r\n"); //delet
    nwy_test_cli_echo("\r\nTest uart success!\r\n");   //delet
}

void nwy_test_cli_uart_read()
{
    char *opt;

    if (hd < 0)
	{
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

	nwy_test_cli_echo("\r\nOption not Supported!\r\n");

}

void nwy_test_cli_uart_write()
{
    char *opt;

    if (hd < 0)
	{
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart send data:");

    nwy_uart_write(hd, (uint8 *)opt, strlen(opt));
}

void nwy_test_cli_uart_dcb_set()
{
	nwy_uartdcb_t dcb = {
		.baudrate = 115200,
		.databits = 8,
		.stopbits = 1,
		.parity = PB_NONE,
		.flowctrl = FC_XONXOFF
	};
    unsigned int baudrate; 
    unsigned int databits;
    unsigned int stopbits;
    nwy_paritybits_e parity;
    nwy_flowctrl_e flowctrl;

    char *opt;

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart param:(b[115200], p[1 2], d[7 8], s[1 2], f[1 2])");

    switch (*opt++)
    {
        case 'b':
	        baudrate = atoi(opt);
			dcb.baudrate = baudrate;
	        if (0 <= 300 && 2 >= 961200)
	        {
	            nwy_uart_dcb_set(hd, &dcb);
	            nwy_test_cli_echo("\r\nSwitch baudrate to:%d\r\n", baudrate);
	        }
	        else
	            nwy_test_cli_echo("\r\nTest invalid baudrate:%d!\r\n", baudrate);
	        break;
	    case 'p':
			parity = atoi(opt);
			dcb.parity = parity;
	        if (0 <= parity && 2 >= parity)
	        {
	            nwy_uart_dcb_set(hd, &dcb);
	            nwy_test_cli_echo("\r\nSwitch parity to:%d\r\n", parity);
	        }
	        else
	            nwy_test_cli_echo("\r\nTest invalid parity:%d!\r\n", parity);
	        break;

	    case 'd':
			databits = atoi(opt);
			dcb.databits = databits;
	        if (7 <= databits && 8 >= databits)
	        {
	            nwy_uart_dcb_set(hd, &dcb);
	            nwy_test_cli_echo("\r\nSwitch data_size to:%d\r\n", databits);
	        }
	        else
	            nwy_test_cli_echo("\r\nInvalid data_size:%d\r\n", databits);
	        break;

	    case 's':
	        stopbits = atoi(opt);
			dcb.stopbits = stopbits;
	        if (1 <= stopbits && 2 >= stopbits)
	        {
	            nwy_uart_dcb_set(hd, &dcb);
	            nwy_test_cli_echo("\r\nSwitch stop_size to:%d\r\n", stopbits);
	        }
	        else
	            nwy_test_cli_echo("\r\nInvalid stop_size:%d\r\n", stopbits);
	        break;

	    case 'f':
	        flowctrl = atoi(opt);
			dcb.flowctrl = flowctrl;
	        if (0 <= flowctrl && 2 >= flowctrl)
	        {
	            nwy_uart_dcb_set(hd, &dcb);
	            nwy_test_cli_echo("\r\nSwitch flow_ctrl to:%d\r\n", flowctrl);
	        }
	        else
	            nwy_test_cli_echo("\r\nInvalid flow_ctrl:%d\r\n", flowctrl);
	        break;

	    default:
	        break;
    }
}

void nwy_test_cli_uart_dcb_get()
{
	nwy_uartdcb_t dcb;

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_dcb_get(hd, &dcb);
	nwy_test_cli_echo("\r\nUart baudrate = %d\r\n", dcb.baudrate);
    nwy_test_cli_echo("\r\nUart parity = %d\r\n", dcb.parity);
    nwy_test_cli_echo("\r\nUart databits = %d\r\n", dcb.databits);
    nwy_test_cli_echo("\r\nUart stopbits = %d\r\n", dcb.stopbits);
    nwy_test_cli_echo("\r\nUart flowctrl = %d\r\n", dcb.flowctrl);
}

void nwy_test_cli_uart_close()
{
    int close = 0;
    char *opt = NULL;

    if (hd < 0){
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nSure to close this uart(0-no, 1-yes):");

    close = atoi(opt);
    if (close)
	{
        nwy_uart_close(hd);
        hd = -1;
    }
}

static void nwy_uart_recv_handle(const char *str, unsigned int length)
{
    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    nwy_uart_write(hd, (uint8 *)str, length);
    nwy_test_cli_echo("\r\nUart send data length = %d\r\n", length);
}
#ifdef NWY_EC618_UART_OPEN_TEST
static void nwy_uart_recv0(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, 10);
}
static void nwy_uart_recv1(uint32_t event)
{
    char *str;
    str = nwy_getbuff();
    nwy_uart_send_data(hd, (uint8 *)str, strlen(str));
}
#endif

void nwy_test_cli_uart_rx_cb_register()
{
    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }
#ifdef NWY_EC618_UART_OPEN_TEST
    if(hd == 0)
        nwy_uart_rx_cb_register(hd, nwy_uart_recv0);
    if(hd == 1)
        nwy_uart_rx_cb_register(hd, nwy_uart_recv1);
#else
    nwy_uart_rx_cb_register(hd, nwy_uart_recv_handle);
#endif

}

/*if send completly, the callback func will set RS485 as rx state*/
static void nwy_uart_send_complet_handle(int param)
{
    nwy_thread_sleep(10);
    nwy_test_cli_echo("\r\nUart send complet handle success!\r\n");
}

void nwy_test_cli_uart_tx_cb_register()
{
    char *pstsnd = "hellors485";

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    /*register cb func to uart drv */
    nwy_uart_tx_cb_register(hd, nwy_uart_send_complet_handle);

    /* for send, set RS485 as tx state */
    nwy_uart_write(hd, (uint8_t *)pstsnd, strlen(pstsnd));
}

void nwy_test_cli_uart_rx_frame_timeout_set()
{
    int timeout;
    char *opt;

    if (hd < 0)
    {
        nwy_test_cli_echo("\r\n Uart port is not inited!!!\r\n");
        return;
    }

    opt = nwy_test_cli_input_gets("\r\nTest in set the uart receive timeout(default:32ms):");
    timeout = atoi(opt);

    nwy_uart_rx_frame_timeout_set(hd, timeout);
    nwy_test_cli_echo("\r\nSet rx frame timeout = %d\r\n", timeout);
}
#endif

