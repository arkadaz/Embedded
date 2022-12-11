#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

void ST7735_GPIO_Init(void){
	/*
	SPI2 Pins
	RES: PB5
	DC:	 PB4
	CS:  PB3
	*/
	__GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
}

SPI_HandleTypeDef hspi2;

void ST7735_SPI_Init(void){
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_1LINE;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	hspi2.Init.CRCPolynomial = 10;
	
	HAL_SPI_Init(&hspi2);
}

void spi2_8b_init(void){
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	HAL_SPI_Init(&hspi2);
}

void spi2_16b_init(void){
	hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
	HAL_SPI_Init(&hspi2);
}

void lcd7735_senddata(unsigned char data){
	HAL_SPI_Transmit(&hspi2,&data,1,0x1);
}

void lcd7735_send16bData(uint8_t msb,uint8_t lsb){
	uint8_t masData[]={lsb,msb};
	HAL_SPI_Transmit(&hspi2,masData,1,0x1);
}

void lcd7735_sendData(unsigned char data){
	//Set DC high
	lcd7735_senddata(data);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi){
	/*
	SPI2 GPIO Configuration
	PB10:SPI2_SCK
	PC3:SPI2_MOSI
	*/
	__SPI2_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi){
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_10);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
}