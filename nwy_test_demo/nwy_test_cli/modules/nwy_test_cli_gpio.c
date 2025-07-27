#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "nwy_gpio_api.h"
#ifdef FEATURE_NWY_ASR_TEST_TMP
void nwy_test_cli_gpio_direction_get()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    dir = nwy_gpio_direction_get(port);
    nwy_test_cli_echo("\r\nGet the GPIO dir = %d\r\n", dir);
}

void nwy_test_cli_gpio_direction_set()
{
    char *opt;
    uint8_t port, dir;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO dir(0-input,1-output):");
    dir = atoi(opt);

    nwy_gpio_direction_set(port, (nwy_dir_mode_e)dir);
}

void nwy_test_cli_gpio_value_get()
{
    char *opt;
    uint32_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    vol = nwy_gpio_value_get(port);
    nwy_test_cli_echo("\r\nGet the GPIO value = %d\r\n", vol);
}

void nwy_test_cli_gpio_value_set()
{
    char *opt;
    uint8_t port, vol;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the gpio value(0-NONE,1-UP,2-DOWN):");
    vol = atoi(opt);

    nwy_gpio_value_set(port, (nwy_value_e)vol);
}

void nwy_test_cli_gpio_pull_get()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_gpio_pull()
{
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(0-PULL NONE,1-PULL UP,2-PULL DOWN:");
    mode = atoi(opt);
    nwy_gpio_pullup_or_pulldown(port, mode);
}
void nwy_test_cli_gpio_pull_set()
{
#ifdef NWY_EC618_GPIO_OPEN_TEST
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(0-pull up,1-pull down):");
    mode = atoi(opt);
    nwy_gpio_pullup_or_pulldown(port, mode);
#else
    char *opt;
    uint8_t port;
    uint8_t mode;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO pull mode(1-pull up,2-pull down):");
    mode = atoi(opt);

    nwy_gpio_pull_set (port, mode);
#endif
}
	void nwy_test_cli_gpio_irq_register()
	{
#ifdef NWY_EC618_GPIO_OPEN_TEST
		char *opt;
		uint8_t port, mode, data;
		opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
		port = atoi(opt);
		opt = nwy_test_cli_input_gets("\r\nSet the GPIO irq mode(0-disable,1-low,2-high,3-falling,4-rising):");
		mode = atoi(opt);
	
		if (port == 9)
			data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb9);
		else if (port == 8)
			data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb8);
		else if (port == 24)
			data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, nwy_irq_open_cb24);
		else
			data = nwy_open_gpio_irq_config(port, (nwy_irq_mode_t)mode, _gpioisropen);
#else
		char *opt;
		uint8_t port, pin_edge, pin_pull;
		opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
		port = atoi(opt);
		opt = nwy_test_cli_input_gets("\r\nSet the GPIO pin edge(1-rising,2-falling,3-both edge,4-high,5-low):");
		pin_edge = atoi(opt);
		opt = nwy_test_cli_input_gets("\r\nSet the GPIO pin pull(1-pullup,2-pulldown):");
		pin_pull = atoi(opt);
		
		//nwy_gpio_close(port);
		int data = nwy_gpio_irq_register (port, pin_edge, pin_pull, _gpioisropen, NULL);
#endif
		if (!data)
		{
			nwy_test_cli_echo("\r\nGpio isr register success!\r\n");
		}
		else
		{
			nwy_test_cli_echo("\r\nGpio isr register failed!\r\n");
		}
	}

void nwy_test_cli_gpio_irq_enable()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

    int ret = nwy_gpio_irq_enable(port);

	if(0 == ret)
    	nwy_test_cli_echo("\r\nGpio enable isr success!\r\n");
	else
		nwy_test_cli_echo("\r\nGpio enable isr fail!\r\n");

}

void nwy_test_cli_gpio_irq_disable()
{
    char *opt;
    uint8_t port;
    opt = nwy_test_cli_input_gets("\r\nSet the GPIO id:");
    port = atoi(opt);

	int ret = nwy_gpio_irq_disable(port);
	if(0 == ret)
		nwy_test_cli_echo("\r\nGpio disenable isr success!\r\n");
	else
		nwy_test_cli_echo("\r\nGpio disenable isr fail!\r\n");
	}

void nwy_test_cli_gpio_irq_wakeup_enable()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}

void nwy_test_cli_gpio_irq_wakeup_disable()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}
#endif