#include "adxl345.h"

char data;
uint8_t data_rec[6];
/*-reg = memory want to read or write-*/
void adxl_read_address(uint8_t reg){
	i2c_readbyte(DEVICE_ADDR,reg,&data);
}

void adxl_write(uint8_t reg,char value){
	char data[1];
	data[0]=value;
	i2c_burstwrite(DEVICE_ADDR,reg,1,data);
}
/*read DATAX0 to DATAZ1 [0x32 to 0x37] size=6*/
void adxl_read_values(uint8_t reg){
	i2c_burstread(DEVICE_ADDR,reg,6,(char *)data_rec);
}

void adxl_init(void){
	i2c_Init();
	/*Read the DEVID, this should return 0xE5*/
	adxl_read_address(DEVID_R);
	/*Set data format range to +-4g*/
	adxl_write(DATA_FORMAT_R,FOUR_G);
	/*Reset all bits*/
	adxl_write(POWER_CTL,RESET);
	/*Comfigure power control measure bit*/
	adxl_write(POWER_CTL,SET_MEASURE_B);
}