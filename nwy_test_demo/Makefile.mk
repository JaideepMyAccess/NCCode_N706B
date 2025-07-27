ifeq ("$(proj_dir)","nwy_test_demo/nwy_function_demo")
inc_dir = inc 

without_dir = $(notdir $(wildcard $(proj_dir)/src/*.c))

srcs =$(addprefix src/, $(without_dir))
else
inc_dir = comm \
		  modules

srcs =  comm/nwy_test_cli_utils.c \
		comm/nwy_test_cli_adpt.c \
		comm/nwy_test_cli_menu.c      \
		modules/nwy_test_cli_os.c \
		modules/nwy_test_cli_driver.c \
		modules/nwy_test_cli_http.c \
		modules/nwy_test_cli_ftp.c \
		modules/nwy_test_cli_sntp.c \
		modules/nwy_test_cli_sim.c \
		modules/nwy_test_cli_sms.c \
		modules/nwy_test_cli_sdw.c \
		modules/nwy_test_cli_lbs.c \
		modules/nwy_test_cli_loc.c \
		modules/nwy_test_cli_iot.c \
                modules/nwy_test_cli_mbedtls_aes.c \
		nwy_test_cli_mbedtls_ssltcp.c \
		modules/nwy_test_cli_network.c \
		modules/nwy_test_cli_data.c \
		modules/nwy_test_cli_tcpip.c \
		modules/nwy_test_cli_stk.c \
		modules/nwy_test_cli_mbedtls_config.c \
		modules/nwy_test_cli_fota.c \
        #modules/nwy_test_cli_driver.c  \
		#modules/nwy_test_cli_data.c \
		#modules/nwy_test_cli_uart.c \
		#modules/nwy_test_cli_gpio.c \
		#modules/nwy_test_cli_pm.c \
		#modules/nwy_test_cli_pwm.c \
		#modules/nwy_test_cli_http.c \
		#modules/nwy_test_cli_mobile.c 
modules/nwy_test_temp.c\
		CustomModules/LCD.c\
		CustomModules/I2C.c\
endif
