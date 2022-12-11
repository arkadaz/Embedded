#include "stm32f4xx.h"                  // Device header

int main(void){
	
	__disable_irq();
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;								//PA5 output
	
	//TIM2
	RCC->APB1ENR |= 1<<0;									//TIM2 clock
	TIM2->PSC     = 16000-1;							//16MHz Divide by 16000 = 1000
	TIM2->ARR			= 1000-1;								//1000 Divide by 1000 = 1
	TIM2->CR1    |= 1<<0;									//1: Counter enabled
	TIM2->DIER	 |= 1<<0;									//1: Update interrupt enabled
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	__enable_irq();
	
	while(1);
}

void TIM2_IRQHandler(void){
	TIM2->SR &=~(1<<0);										//CLEAR flag
	GPIOA->ODR ^= 0x20;
}




