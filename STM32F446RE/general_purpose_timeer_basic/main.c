#include "stm32f4xx.h"                  // Device header


int main(void){
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;
	
	RCC->APB1ENR |= 1<<0;									//TIMER2 clock enable
	TIM2->PSC     = 1600-1;								//16MHz Divide by 1600 = 10000 Hz
	TIM2->ARR     = 10000-1;							//10000 Divide by 10000 = 1 Hz
	TIM2->CNT     = 0;										//Initial value by 0
	TIM2->CR1     = 1<<0;									//Counter enable
	
	while(1){
		while(!(TIM2->SR & 1<<0));					//UIF: Update interrupt flag
		TIM2->SR &=~(1<<0);
		GPIOA->ODR ^= 0x20;
	}
	
}
