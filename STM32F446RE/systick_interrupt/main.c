#include "stm32f4xx.h"                  // Device header

int main(void){
	
	__disable_irq();
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;								//PA5 output
	
	SysTick->LOAD = 16000000-1;						//SysTick Reload Value Register
	SysTick->VAL  = 0;										//SysTick Current Value Register
	SysTick->CTRL = 1<<0|1<<1|1<<2;				//1 = counter enabled
																				//1 = counting down to zero asserts the SysTick exception request. Software can use COUNTFLAG to determine if SysTick has ever counted to zero.
																				//1 = processor clock.
	__enable_irq();
	
	while(1);
	
}

void SysTick_Handler(void){
	GPIOA->ODR ^= 0x20;
}
	


