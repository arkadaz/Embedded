#include "stm32f4xx.h"                  // Device header

int timeStamp=0;
	
int main (void){
	
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<11;								//PA5 alternate
	GPIOA->AFR[0]|= 1<<20;								//Set pin to AF for TIM2 Ch1
	
	RCC->APB1ENR |= 1<<0;
	TIM2->PSC     = 1600-1;
	TIM2->ARR     = 10000-1;
	TIM2->CCMR1   = 1<<4|1<<5|1<<8|1<<9;	//Set output to toggle on match *****************
	TIM2->CCR1    = 0;
	TIM2->CCER	 |= 1<<0;
	TIM2->CNT			= 0;
	TIM2->CR1     = 1<<0;
	
	//Configure PA6 as input of TIM3 CH1
	RCC->AHB1ENR |= 1<<0;
	GPIOA->MODER |= 1<<13;
	GPIOA->AFR[0]|= 0x2000000;							//PA6 TIM3 AF2
	
	//Configure TIM3
	RCC->APB1ENR |= 1<<1;										//ENABLE TIM3
	TIM3->PSC     = 16000;
	TIM3->CCMR1   = 1<<0|1<<6;							//100: Force inactive level - OC1REF is forced low.
																					//01: CC1 channel is configured as input, IC1 is mapped on TI1
	TIM3->CCER    = 1<<0;										//1: Capture enabled
	TIM3->CR1     = 1<<0;

	while(1){
		while(!(TIM3->SR & 1<<1));
		timeStamp=TIM3->CCR1;
	}
}


/*#include "stm32f4xx.h"  // Device header
 
uint64_t timestamp;
int main(void)
	
{
RCC->AHB1ENR |=1<<0;												//enable gpio a clock
GPIOA ->MODER|=0x2800; 											//PA5 as output
GPIOA->AFR[0]|=0x00100000;
RCC->APB1ENR|=1;
	
 
TIM2->PSC=16000-1;
TIM2->ARR=1000-1;
TIM2->CCMR1|=0x30;
TIM2->CCR1=0;
TIM2->CCER|=1;
TIM2->CNT=0;
TIM2->CR1=0x01;
//----
RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
GPIOA->MODER|=0x2800; //set pa6 to alerate function
GPIOA->AFR[0]|=0x02000000;
RCC->APB1ENR|=2;
TIM3->PSC=16000;
//TIM3->ARR=1;
TIM3->CCMR1|=0x1;
TIM3->CCER|=1;
TIM3->CR1=1;
	
	
while(1)
{	
	while(!(TIM3->SR&2));
	
	timestamp=TIM3->CCR1;
	
	
}
 
 
}
*/
