#include "stm32f4xx.h"                  // Device header

int period;
float freq;

int main(void){
	int last_value=0;
	int current_value=0;
	
	
	
	///PA5
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<11;								//PA5 alternate
	GPIOA->AFR[0]|= 1<<20;								//AF1 for TIM2 CH1
	
	RCC->APB1ENR |= 1<<0;									//TIM2
	TIM2->PSC     = 1600-1;
	TIM2->ARR     = 10000-1;
	TIM2->CCMR1   =	1<<4|1<<5;						//011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1
	TIM2->CCR1		= 0;
	TIM2->CCER	 |= 1<<0;									//1: On - OC1 signal is output on the corresponding output pin
	TIM2->CNT     = 0;
	TIM2->CR1     = 1<<0;									//1: Counter enabled
	
	//PA6
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<13;
	GPIOA->AFR[0]|= 1<<25;									//AF2 for TIM3 CH1
	
	RCC->APB1ENR |= 1<<1;
	TIM3->PSC     = 16000-1;
	TIM3->CCMR1   = 1<<0|1<<6;						//01: CC1 channel is configured as input, IC1 is mapped on TI1.
																				//100: Force inactive level - OC1REF is forced low
	TIM3->CCER    = 1<<0|1<<1|1<<3;				//1: Capture enabled
																				//01: inverted/falling edge
																				//CC1 channel configured as input: This bit is used in conjunction with CC1P to define TI1FP1/TI2FP1 polarity. refer to CC1P description
	TIM3->CR1     = 1<<0;									//1: Counter enabled
	
	while(1){
		while(!(TIM3->SR & 1<<1));
		current_value=TIM3->CCR1;
		period = current_value-last_value;
		last_value=current_value;
		freq=1000.0f/period;
		last_value=current_value;
		
	}
	
}
