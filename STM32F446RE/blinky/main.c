//pa5						led
//ahb1		
//rcc->ahb1enr
//gpiox_moder		mode
//gpiox_odr			output

#include "stm32f4xx.h"                  // Device header

void delayMs(int delay);

int main(void){
	RCC->AHB1ENR |= 1;								//GPIOA clock en
	GPIOA->MODER |= 0x400;
	while(1){
		GPIOA->ODR=0x20;
		//GPIOA->BSRR=1<<5;
		delayMs(1000);
		GPIOA->ODR&=~0x20;
		//GPIOA->BSRR=1<<21;
		delayMs(1000);
	}
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		
		for(i=0;i<3195;i++){
		}
	}
}
