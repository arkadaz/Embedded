#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
//PA7 MOSI
//PA6 MISO
//PA5 SCK

//PA4 SS
void SPI_Init(void);

int main(void){
	
}

void spi_Init(void){
	RCC->AHB1ENR |= 1<<0;										//GPIOA clock enable
	RCC->APB2ENR |= 1<<12;									//1: SPI1 clock enabled
	
	GPIOA->MODER &=~(0x3<<14);
	GPIOA->MODER &=~(0x3<<12);
	GPIOA->MODER &=~(0x3<<10);
	GPIOA->MODER &=~(0x3<<8);
	
	GPIOA->MODER |= 1<<15;									//PA7 alternate
	GPIOA->MODER |= 1<<13;									//PA6 alternate
	GPIOA->MODER |= 1<<11;									//PA5 alternate
	GPIOA->MODER |= 1<<8;										//PA4 output
	
	GPIOA->AFR[0]&=~(0xF<<28);
	GPIOA->AFR[0]&=~(0xF<<24);
	GPIOA->AFR[0]&=~(0xF<<20);						
	GPIOA->AFR[0]&=~(0xF<<16);							
	
	GPIOA->AFR[0]|= (1<<30|1<<28);					//PA7 AF5 SPI1_MOSI
	GPIOA->AFR[0]|= (1<<26|1<<24);					//PA6 AF5 SPI1_MISS
	GPIOA->AFR[0]|= (1<<22|1<<20);					//PA5 AF5 SPI1_SCK
	GPIOA->AFR[0]|= (1<<18|1<<16);					//PA4 AF5 SPI1_NSS
	
	SPI1->CR1    |= 1<<2;										//1: Master configuration
	SPI1->CR1    |= 1<<3|1<<4;							//011: fPCLK/16
	SPI1->CR1    |= 1<<8;										//This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
	SPI1->CR1    |= 1<<9;										//1: Software slave management enabled
	SPI1->CR1    &=~(1<<11);								//0: 8-bit data frame format is selected for transmission/reception
	SPI1->CR1    |= 1<<0;										//1: The second clock transition is the first data capture edge ---- CPHA: Clock phase
	SPI1->CR1    |= 1<<1;										//1: CK to 1 when idle ---- CPOL: Clock polarity
	SPI1->CR1		 &=~(1<<10);								//0: full-duplex (Transmit and receive)
	SPI1->CR1		 &=~(1<<7);									//0: MSB transmitted first
	
	SPI1->CR2			= 0;
	SPI1->CR1		 |= 1<<6;										//1: Peripheral enabled SPI enable
}

void spi_write_data(uint8_t *data,uint32_t size){
	uint32_t i=0;
	uint8_t temp;
	
	while(i<size){
		while(!(SPI1->SR & 1<<1));						//1: Tx buffer empty
		SPI1->DR = data[i];										//Write data
		i++;
	}
	while(!(SPI1->SR & 1<<1));							//1: Tx buffer empty
	while(SPI1->SR & 1<<7);									//1: SPI (or I2S) is busy in communication or Tx buffer is not empty.This flag is set and cleared by hardware.
	
	/*Clear OVR flag*/
	temp = SPI1->DR;
	temp = SPI1->SR;
}

void spi_receive_data(uint8_t *data,uint32_t size){

	while(size){
		SPI1->DR = 0;													//Sent dummy data
		while(!(SPI1->SR & 1<<0));						//1: Rx buffer not empty
		*data++ = SPI1->DR;										//READ data
		size--;
	}
}

void cs_enable(void){
	GPIOA->ODR &=~(1<<4);										//0: for slave select
}

void cs_disable(void){
	GPIOA->ODR |= 1<<4;											//1: for slave deselect
}
