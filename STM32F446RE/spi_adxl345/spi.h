#ifndef SPI_H_
#define SPI_H_
#include "stm32f4xx.h"                  // Device header


void spi_Init(void);
void spi_write_data(uint8_t *data,uint32_t size);
void spi_receive_data(uint8_t *data,uint32_t size);
void cs_enable(void);
void cs_disable(void);







#endif /*SPI_H_*/
