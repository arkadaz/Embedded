#include "adxl345.h"

/*-reg = memory want to read or write-*/
void adxl_read(uint8_t adress,uint8_t *rxdata){

	/*Set read operation*/
	adress |= READ_OPERATION;
	/*Enable multi-byte*/
	adress |= MULTI_BYTE_EN;
	/*Pull cs line low to enable slave*/
	cs_enable();
	/*Sent address*/
	spi_write_data(&adress,1);
	/*Read 6 bytes*/
	spi_receive_data(rxdata,6);
	/*Pull cs line high to disable slave*/
	cs_disable();
}

void adxl_write(uint8_t adress,char value){
	uint8_t data[2];
	/*Enable multi-byte,place address into buffer*/
	data[0] = adress|MULTI_BYTE_EN;
	/*place data into buffer*/
	data[1] = value;
	/*Pull cs line low to enable slave*/
	cs_enable();
	/*Transmit data and address*/
	spi_write_data(data,2);
	/*Pull cs line high to disable slave*/
	cs_disable();
}

void adxl_init(void){
	/*Enable SPI Init*/
	spi_Init();
	/*Set data format range to +-4g*/
	adxl_write(DATA_FORMAT_R,FOUR_G);
	/*Reset all bits*/
	adxl_write(POWER_CTL,RESET);
	/*Comfigure power control measure bit*/
	adxl_write(POWER_CTL,SET_MEASURE_B);
}
