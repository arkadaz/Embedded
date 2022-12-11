#include "stm32f4xx.h"                  // Device header

void systickDelayMs(int n);

int main(void){
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;
	
	while(1){
		systickDelayMs(1000);
		GPIOA->ODR ^= 0x20;
	}
}

void systickDelayMs(int n){
	int i=0;
	SysTick->LOAD = 16000;								//Reload with number of clock per msec
	SysTick->VAL = 0;											//CLEAR_BIT current value register
	SysTick->CTRL = 1<<0|1<<2;						
	
	for(i=0;i<n;i++){
		while((SysTick->CTRL & 1<<16)==0);	//wait until the count flag is set
	}
	SysTick->CTRL = 0;
}
