/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_i2c_api.h
 * Author       : Charlie
 * Created      : 2023-06-09
 * Description  : i2c API
 ******************************************************************************/

#ifndef __NWY_I2C_API_H__
#define __NWY_I2C_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NAME_I2C_BUS_1  "I2C1"
#define NAME_I2C_BUS_2  "I2C2"
#define NAME_I2C_BUS_3  "I2C3"

typedef enum
{
    NWY_I2C_BPS_100K, ///< normal 100Kbps
    NWY_I2C_BPS_400K, ///< fast 400Kbps
    NWY_I2C_BPS_3P5M, ///< high speed 3.5Mbps
} nwy_i2c_mode_e;

/*---------------------------Function Definition--------------------------*/
/**
 * @brief: creat and initialize i2c bus
 *
 * @param option: 
 *		i2cDev		-	i2c bus name
 *		mode		-	i2c bus mode
 * @return:
 *     >=0	-	i2c handler  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
int nwy_i2c_init(const char * i2cDev, nwy_i2c_mode_e mode);

/**
 * @brief: read one byte data from i2c bus
 *
 * @param option: 
 *		fd			-	i2c bus handler
 *		data		-	data storage address
 *		start_flag	-	start ctrl flag
 *		stop_flag	-	stop ctrl flag
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_i2c_raw_get_byte(int fd, uint8_t *data, int start_flag, int stop_flag);

/**
 * @brief: write one byte data from i2c bus
 *
 * @param option: 
 *		fd			-	i2c bus handler
 *		data		-	data to write
 *		start_flag	-	start ctrl flag
 *		stop_flag	-	stop ctrl flag
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_i2c_raw_put_byte(int fd, uint8_t data, int start_flag, int stop_flag);

/**
 * @brief: read data from i2c bus
 *
 * @param option: 
 *		fd			-	i2c bus handler
 *		slaveAddr	-	slave address
 *		ofstAddr	-	register address
 *		ptrBuff		-	data storage address
 *		length		-	data length
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED   
 */
nwy_error_e nwy_i2c_read(int fd, unsigned char slaveAddr, unsigned short ofstAddr,
                 unsigned char* ptrBuff, unsigned short length);

 /**
  * @brief: write data to i2c bus
  *
  * @param option: 
  * 	 fd 		 -	 i2c bus handler
  * 	 slaveAddr	 -	 slave address
  * 	 ofstAddr	 -	 register address
  * 	 ptrBuff	 -	 data storage address
  * 	 length 	 -	 data length
  * @return:
  * 	>=0  -	 SUCCESS				  
  * 	-2	 -	 INVALID PARA	 
  * 	-3	 -	 FAILED   
  */
nwy_error_e nwy_i2c_write(int fd, unsigned char slaveAddr, unsigned short ofstAddr,
                  unsigned char* ptrData, unsigned short length);

/**
* @brief: release i2c bus
*
* @param option: 
*	   fd		   -   i2c bus handler
* @return:
*	 >=0  -   SUCCESS				   
*	 -2   -   INVALID PARA	  
*	 -3   -   FAILED   
*/
nwy_error_e nwy_i2c_deinit(int fd);

#ifdef __cplusplus
   }
#endif

#endif 

