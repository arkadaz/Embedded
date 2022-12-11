#include "stm32f4xx.h"                  // Device header
/*
LED:PA5
BUTTON:PC13
*/
void systickDelayMs(int n);

int counter=0;


int main(void){
	
	__disable_irq();
	
	RCC->AHB1ENR |= 1<<0;									//PortA clock enable
	RCC->AHB1ENR |= 1<<2;									//PortC clock enable
	RCC->APB2ENR |= 1<<14;								//SYSCFGEN: System configuration controller clock enable
	
	GPIOA->MODER |= 1<<10;								//PA5 as output
	GPIOA->MODER &=~(1<<11);
	
	GPIOC->MODER &=~(1<<26|1<<27);				//PC13 as input
	
	SYSCFG->EXTICR[3] |= 1<<5;						//EXTICR4 because use PC13 (EXTI13)
	SYSCFG->EXTICR[3] &=~(1<<4|1<<6|1<<7);
	
	EXTI->IMR    |= 1<<13;								//p.274 ref 1: Interrupt request from line x is not masked
	EXTI->FTSR   |= 1<<13;								//1: Falling trigger enabled (for Event and Interrupt) for input line.

	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();
	
	while(1);
}

void EXTI15_10_IRQHandler(void){
	
	GPIOA->BSRR = 0x20;
	systickDelayMs(1000);
	GPIOA->BSRR = 0x00200000;
	systickDelayMs(1000);
	GPIOA->BSRR = 0x20;
	systickDelayMs(1000);
	GPIOA->BSRR = 0x00200000;
	systickDelayMs(1000);
	counter++;
	EXTI->PR = 1<<13;
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

