#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdint.h>
#include "spi.h"

#define DEVID_R						0x00
#define DEVICE_ADDR				0x53
#define DATA_FORMAT_R			0x31
#define POWER_CTL					0x2D
#define DATA_START_ADDR		0x32

#define READ_OPERATION		0x40
#define MULTI_BYTE_EN			0x40
#define FOUR_G						0x01		//Register 0x31—DATA_FORMAT (Read/Write)  01 ±4 g
#define RESET							0x00
#define SET_MEASURE_B			0x08		//Register 0x2D—POWER_CTL (Read/Write)  set bit 3



void adxl_init(void);
void adxl_write(uint8_t reg,char value);
void adxl_read(uint8_t adress,uint8_t *rxdata);




#endif /*ADXL345_H_*/
