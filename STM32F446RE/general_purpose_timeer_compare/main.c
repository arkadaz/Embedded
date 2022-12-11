#include "stm32f4xx.h"                  // Device header


int main(void){
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<11;								//PA5 alternate function
	GPIOA->AFR[0]|= 1<<20;							//Set PA5 to AF1 (TIMER2)
	
	RCC->APB1ENR |= 1<<0;
	TIM2->PSC     = 1600-1;								//16MHz Divide by 1600 = 10000
	TIM2->ARR     = 10000-1;							//10000 Divide by 10000= 1Hz
	TIM2->CCMR1   = 1<<4|1<<5;						//OC1M: Output compare 1 mode 011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1
	TIM2->CCR1    = 0;										//Set match mode
	TIM2->CCER   |= 1<<0;									//ENABLE comparemode at Chanel1
	TIM2->CNT     = 0;										//Intial count 0
	TIM2->CR1     = 1<<0;									//CEN: Counter enable
	
	while(1);
}
