//apb1 usart2 bit17
//usart connected to PA2 bit0

#include "stm32f4xx.h"                  // Device header
void USART2_Init(void);
void USART_Write(int ch);
void delayMs(int delay);

int main(void){
	USART2_Init();
	while(1){
		USART_Write('H');
		USART_Write('i');
		USART_Write('\r');
		USART_Write('\n');
		delayMs(10);
	}
	
	return 0;
}

void USART2_Init(void){
	
	RCC->APB1ENR |= 1<<17;
	RCC->AHB1ENR |= 1<<0;
	
	GPIOA->AFR[0] = 0x7<<8;								//SET AFRL port2 to AF7
	GPIOA->MODER |=	0x2<<4;								//SET GPIOA to alternate function
	
	USART2->BRR		= 0x0683;								//9600 at 16MHz
	USART2->CR1	 |= 1<<13;								//USART enable
	USART2->CR1	 &=~(1<<12);							//0: 1 Start bit, 8 Data bits, n Stop bit
	USART2->CR1	 |= 1<<3;									//TE: Transmitter enable
	USART2->CR2	 &=~(0x3<<12);						//STOP: 1 STOP bits
	
}

void USART_Write(int ch){
	while(!(USART2->SR&(1<<7)));					//p.835 wait while TX buffer is empty
	
	USART2->DR    = (ch&0xFF);
	
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++);
	}
}