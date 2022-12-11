#include "stm32f4xx.h"               // Device header


int main(void){
	RCC->AHB1ENR |= 1;								//GPIOA clock en
	RCC->AHB1ENR |= 1<<2;							//GPIOC clock en
	GPIOA->MODER |= 0x400;
	GPIOC->MODER &=~(0x3<<26);
	while(1){
		if(GPIOC->IDR & 1<<13){					//if PC13 is high
			GPIOA->BSRR=0x002;						//turn off led
		}else{
			GPIOA->BSRR=0x20;							//turn on led
		}
	}
	
	
}

