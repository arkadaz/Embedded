#include "stm32f4xx.h"                  // Device header

void LED_play(int value);
void systickDelayMs(int n);
void UART2_Init(void);

int main(void){
	__disable_irq();
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10; 								//PA5 output
	UART2_Init();
	USART2->CR1  |= 1<<5;									//RXNEIE RX interrupt enable
	NVIC_EnableIRQ(USART2_IRQn);
	
	__enable_irq();
	
	while(1);
}


void UART2_Init(void){
	
	RCC->AHB1ENR |= 1<<0;
	RCC->APB1ENR |= 1<<17;
	
	GPIOA->AFR[0]|= 0x7<<12;							//PA3 AF7
	
	GPIOA->MODER |= 1<<7;									//PA3 alternate
	GPIOA->MODER &=~(1<<6);
	
	USART2->BRR   = 0x008B;								//115200 at 16MHz
	USART2->CR1  |= 1<<13;								//USART enable
	USART2->CR1  &=~(1<<12);							//0: 1 Start bit, 8 Data bits, n Stop bit
	USART2->CR1  |= 1<<2;									//RE: Receiver enable
}

void LED_play(int value){
	value %=16;
	for(;value>0;value--){
		GPIOA->BSRR = 0x20;
		systickDelayMs(1000);
		GPIOA->BSRR = 0x00200000;
		systickDelayMs(1000);
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

void USART2_IRQHandler(void){
	char c;
	if(USART2->SR & 1<<5){
		c=USART2->DR;
		LED_play(c);
	}
}

