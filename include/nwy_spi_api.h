/******************************************************************************
 * Copyright (c) 2023, Neoway Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : nwy_spi_api.h
 * Author       : Charlie
 * Created      : 2023-06-09
 * Description  : spi API
 ******************************************************************************/
#ifndef __NWY_SPI_API_H__
#define __NWY_SPI_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_BUS_NUM       2
#define SPI_DEV_NUM       2

#define SPI_MODE_CPOL     0x01
#define SPI_MODE_CPHA     0x02

#define SPI_MODE_0        (0x00          |          0x00)
#define SPI_MODE_1        (SPI_MODE_CPHA |          0x00)
#define SPI_MODE_2        (0x00          | SPI_MODE_CPOL)
#define SPI_MODE_3        (SPI_MODE_CPHA | SPI_MODE_CPOL)

#define SPI_CS_0  0x00
#define SPI_CS_1  0x01

#define NAME_SPI_BUS_1  "SPI1"
#define NAME_SPI_BUS_2  "SPI2"

/**
 * @brief: creat and initialize spi bus
 *
 * @param option: 
 *		spibus		-	spi bus name
 *		mode		-	spi bus mode
 *		speed		-	spi bus speed
 *		bits		-	spi bus data frame bits
 * @return:
 *     >=0	-	spi bus handler  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
int nwy_spi_init(char *spibus, uint8_t mode, uint32_t speed, uint8_t bits);

int nwy_spi_cs_cfg(int hd, uint8_t cs, bool sel);

/**
 * @brief: read data from spi bus
 *
 * @param option: 
 *		hd			-	spi bus handler 
 *		sendaddr	-	point to senddata address.in this case, you can specify the content of datas.
 *		readaddr	-	point to readaddr address.
 *		len			-	data length
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_spi_read(int hd,  void *sendaddr, void *readaddr, uint32_t len);

/**
 * @brief: write data from spi bus
 *
 * @param option: 
 *		hd			-	spi bus handler 
 *		readaddr	-	data adress
 *		len			-	data length
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_spi_write(int hd, void *sendaddr, uint32_t len);

/**
 * @brief: transfer data from spi bus
 *
 * @param option: 
 *		hd			-	spi bus handler 
 *		cs			-	cs choice of spi device
 *		tx			-	point to senddata address
 *		rx			-	point to readaddr address
  *		size		-	data len to be transfer.
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_spi_transfer(int hd, uint8_t cs, uint8_t *tx, uint8_t *rx, uint32_t size);

/**
 * @brief: release spi bus 
 *
 * @param option: 
 *		hd			-	spi bus handler 
 * @return:
 *     >=0	-	SUCCESS  				 
 *     -2	-	INVALID PARA    
 *     -3	-	FAILED    
 */
nwy_error_e nwy_spi_deinit(int hd);


#ifdef __cplusplus
   }
#endif

#endif

