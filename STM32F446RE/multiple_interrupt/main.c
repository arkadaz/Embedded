#include "stm32f4xx.h"                  // Device header

void systickDelayMs(int n);

volatile int pb10_counter=0;
volatile int pc13_counter=0;

int main(void){
	
	__disable_irq();
	
	RCC->AHB1ENR |= 1<<0|1<<1|1<<2;
	
	RCC->APB2ENR |= 1<<14;								//SYSCFGEN: System configuration controller clock enable
	
	GPIOA->MODER |= 1<<10;								//01: General purpose output mode
	
	//PC13
	SYSCFG->EXTICR[3] |= 1<<5; 						//EXTICR4 PORTC EXTI13
	SYSCFG->EXTICR[3]&=~(1<<4|1<<6|1<<7);
	
	EXTI->IMR		 |= 1<<13;								//Unmark EXTI13
	EXTI->FTSR   |= 1<<13;								//Falling edge trigger
	
	//PB10
	GPIOB->PUPDR |= 1<<20;								//Pull-up PB10
	GPIOB->PUPDR &=~(1<<21);
	
	SYSCFG->EXTICR[2] |= 1<<8;						//EXTICR3 PORTB EXTI10
	SYSCFG->EXTICR[2]&=~(1<<9|1<<10|1<<11);
	
	EXTI->IMR		 |= 1<<10;								//Unmark EXTI10
	EXTI->FTSR   |= 1<<10;								//Falling edge trigger
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();
	
	while(1);
	
}

void EXTI15_10_IRQHandler(void){
	
	if(EXTI->PR == 1<<13){
		pc13_counter++;
		EXTI->PR = 1<<13;
	}else if(EXTI->PR == 1<<10){
		pb10_counter++;
		EXTI->PR = 1<<10;
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

