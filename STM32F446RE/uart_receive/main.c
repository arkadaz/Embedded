#include "stm32f4xx.h"                  // Device header

void delayMs(int delay);
void USART2_Init(void);
char USART2_read(void);
void LED_play(int value);

char ch;

int main(void){
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<10;
	USART2_Init();
	while(1){
		ch=USART2_read();
		LED_play(ch);
	}
	return 0;
}





void USART2_Init(void){
	RCC->AHB1ENR |= 1<<0;
	RCC->APB1ENR |= 1<<17;
	
	GPIOA->AFR[0]|= 0x7<<12;
	GPIOA->MODER |= 0x2<<6;
	
	USART2->BRR   = 0x008B;								//115200 at 16MHz
	USART2->CR1	 |= 1<<13;
	USART2->CR1	 &=~(1<<12);
	USART2->CR1	 |= 1<<2;
}

char USART2_read(void){
	while(!(USART2->SR & 1<<5));
	return USART2->DR;
}

void LED_play(int value){
	value %=16;
	for(;value>0;value--){
		GPIOA->BSRR = 0x20;
		delayMs(1000);
		GPIOA->BSRR = 0x00200000;
		delayMs(1000);
	}
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++){
		}
	}
}

