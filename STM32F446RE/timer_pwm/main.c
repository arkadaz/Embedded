#include "stm32f4xx.h"                  // Device header


int main(void){
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<11;
	GPIOA->AFR[0]|= 1<<20;								//PA5 for TIM2
	
	
	//TIMER SETUP
	RCC->APB1ENR |= 1<<0;
	TIM2->PSC     = 10-1;									//16MHz Divide by 10 = 1.6MHz
	TIM2->ARR     = 26667-1;							//1.6MHz Divide by 26667 = 60Hz
	TIM2->CNT			= 0;
	TIM2->CCMR1   = 1<<5|1<<6;						//110: PWM mode 1 - In upcounting
	TIM2->CCER    = 1<<0;									//1: On - OC1 signal is output on the corresponding output pin
	TIM2->CCR1		= 8889-1;								//pulse width 1/3 the period [(TIM2->ARR)/3]
	TIM2->CR1     = 1<<0;									//1: Counter enabled
	/*0-8889 			on
	  8889-26667 	off
	*/
	while(1){
		
	}
	
	
	
	
	
}

