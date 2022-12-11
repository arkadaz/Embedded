#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

int USART2_Write(int ch);
void USART2_Init(void);

int data;
double celsius,voltage;

int main(void){
	
	RCC->AHB1ENR |= 1<<0;									//PA clock
	
	RCC->APB1ENR |= 1<<0;									//TIM2
	TIM2->PSC			= 1600-1;								//16MHz Divide by 1600 = 10000
	TIM2->ARR			=	10000-1;							//10000 Divide by 10000 = 1
	TIM2->CNT			= 0;
	TIM2->CCMR1  |= 1<<11|1<<13|1<<14;		//Bit 11 OC2PE: Output compare 2 preload enable
																				//Bits 14:12 OC2M[2:0]: Output compare 2 mode
	TIM2->CCER	 |= 1<<4;									//Bit 4 CC2E: Capture/Compare 2 output enable.
	TIM2->CCR2		= 50-1;
	TIM2->CR1		 |= 1<<0;									//1: Counter enabled
	
	//ADC
	RCC->APB2ENR |= 1<<8;									//1: ADC1 clock enabled
	ADC->CCR		 |=(1<<23);								//1: Temperature sensor and VREFINT channel enabled
	ADC->CCR		 &=~(1<<22);							//0: VBAT channel disabled	[ADC1]
	ADC1->SMPR1		= 1<<26;								//100: 84 cycles sampling time minimum 10us
	ADC1->SQR3		= 18;										//Tempsensor connect to chanel 18
	ADC1->CR2		 |=1<<24|1<<25|1<<28;			//0011: Timer 2 CC2 event
																				//01: Trigger detection on the rising edge
  ADC1->CR2		 |=1<<0;			
	
	USART2_Init();
	printf("STM32F4 Temperature \r\n");
	
	while(1){
		while(!(ADC1->SR));
		data = ADC1->DR;
		voltage=(double)data/4095*3.3;			//12bit=4095 Vref=3.3
		celsius = (voltage-0.76)/0.0025+25;
		
		printf("%d,%.2f\370C\r\n",data,celsius);
	}

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

struct __FILE{int handle;};
FILE __stdout = {1};
int fputc (int c, FILE *f){
	return USART2_Write(c);
}

int USART2_Write(int ch){
	while(!(USART2->SR & 1<<7));
	USART2->DR = ch;
	return ch;
}


/*according to the equation provided by stm32f4 user manual

Temperature (in °C) = {(V SENSE – V 25 ) / Avg_Slope} + 25

Where:

– V 25 = V SENSE value for 25° C

– Avg_Slope = average slope of the temperature vs. V SENSE curve (given in mV/°C or µV/°C)

according to the datasheet of the stm32f407vetx, the voltage at 25degree is 0.76V and the avg_slop is 2.5mV/C*/


/*#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

int USART2_Write(int ch);
void USART2_Init(void);

int data;
double celsius,voltage;

int main(void){
	
	RCC->AHB1ENR |= 1<<0;									//PA clock
	
	RCC->APB1ENR |= 1<<0;									//TIM2
	TIM2->PSC			= 1600-1;								//16MHz Divide by 1600 = 10000
	TIM2->ARR			=	10000-1;							//10000 Divide by 10000 = 1
	TIM2->CNT			= 0;
	TIM2->CCMR1  = 0x6800;		//Bit 11 OC2PE: Output compare 2 preload enable
																				//Bits 14:12 OC2M[2:0]: Output compare 2 mode
	TIM2->CCER	 = 0x0010;									//Bit 4 CC2E: Capture/Compare 2 output enable.
	TIM2->CCR2		= 50-1;
	TIM2->CR1		 = 1<<0;									//1: Counter enabled
	
	//ADC
	RCC->APB2ENR |= 0x100;									//1: ADC1 clock enabled
	ADC->CCR		 |=0x00800000;								//1: Temperature sensor and VREFINT channel enabled
	ADC->CCR		 &=~(0x00400000);							//0: VBAT channel disabled	[ADC1]
	ADC1->SMPR1		= 0x04000000;								//100: 84 cycles sampling time minimum 10us
	ADC1->SQR3		= 18;										//Tempsensor connect to chanel 18
	ADC1->CR2		 |=1<<24|1<<25|1<<28;			//0011: Timer 2 CC2 event
																				//01: Trigger detection on the rising edge
	ADC1->CR2		 |=1<<0;	
	USART2_Init();
	printf("STM32F4 Temperature \r\n");
	
	while(1){
		while(!(ADC1->SR));
		data = ADC1->DR;
		voltage=(double)data/4095*3.3;			//12bit=4095 Vref=3.3
		celsius = (voltage-0.76)/0.0025+25;
		
		printf("%d,%.2f\370C\r\n",data,celsius);
	}

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

struct __FILE{int handle;};
FILE __stdout = {1};
int fputc (int c, FILE *f){
	return USART2_Write(c);
}

int USART2_Write(int ch){
	while(!(USART2->SR & 1<<7));
	USART2->DR = ch;
	return ch;
}
*/





