#include "stm32f4xx.h"                  // Device header


int main(void){
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;
	
	SysTick->LOAD = 8000000-1;						//at 16MHz want 0.5sec
	SysTick->VAL = 0;
	SysTick->CTRL = 1<<0|1<<2;						//p.249 SysTick Control and Status Register Enable counter and Select clock source as Processor
	
	while(1){
		if(SysTick->CTRL & 1<<16){					//P.249 from arm cortex m4  SysTick Control and Status Register
			GPIOA->ODR ^=0x20;
		}
		
	}
	
}
