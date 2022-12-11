#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

void USART2_Init(void);
int USART2_Write(int ch);
int USART2_Read(void);

int main(void){
	int n;
	char str[100];
	
	USART2_Init();
	
	
	printf("Hello from STM32F446\n");
	fprintf(stdout,"test for stdout \r\n");
	fprintf(stderr,"test for stderr \r\n");
	
	while(1){
		printf("How old are you?");
		scanf("%d",&n);
		printf("Your age is : %d\r\n",n);
		printf("Enter first name :");
		gets(str);
		printf("I like your style: ");
		puts(str);
		printf("\r\n");
	}
}



void USART2_Init(void){
	
	RCC->AHB1ENR |= 1<<0; 
	RCC->APB1ENR |= 1<<17;
	
	GPIOA->AFR[0]|= 0x7<<8;								//PA2
	GPIOA->AFR[0]|= 0x7<<12;							//PA3
	GPIOA->MODER |= 0x2<<4|0x2<<6;				//Enable alternate PA2 PA3
	
	USART2->BRR = 0x0683;									//9600 at 16MHz
	USART2->CR1 |= 1<<2|1<<3;
	USART2->CR1 |= 1<<13;
	USART2->CR1 &=~(1<<12); 
}

int USART2_Write(int ch){
	
	while(!(USART2->SR & 1<<7));
	USART2->DR = (ch&0xFF);
	return ch;
}

int USART2_Read(void){
	
	while(!(USART2->SR & 1<<5));
	
	return USART2->DR;
}

struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

int fgetc(FILE *f){
	int c;
	c = USART2_Read();
	if(c=='\r'){
		USART2_Write(c);
		c='\n';
	}
	USART2_Write(c);
	return c;
}

int fputc(int c,FILE *f){
	
	return USART2_Write(c);
}