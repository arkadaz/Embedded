#include "stm32f4xx.h"                  // Device header

int analogValue;

int main(void){
	
	RCC->AHB1ENR |= 1<<0;									//Clock enable for GPIOA
	GPIOA->MODER |= 1<<2|1<<3;						//PA1 to ANALOG
	
	RCC->APB2ENR |= 1<<8;									//1: ADC1 clock enabled
	ADC1->CR2    &=~(1<<0);								//0: Disable ADC conversion and go to power down mode
	ADC1->SQR3    =	1;									//PA1 so use Bits 4:0 SQ1[4:0]: 1st conversion in regular sequence
	ADC1->CR2		 |= 1<<0;									//1: Enable ADC

	while(1){
		ADC1->CR2  |= 1<<30;								//1: Starts conversion of regular channels
		while(!(ADC1->SR & 1<<1));					//Bit 1 EOC: Regular channel end of conversion
		analogValue=ADC1->DR;
	}
	
}











