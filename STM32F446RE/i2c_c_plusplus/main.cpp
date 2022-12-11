#include "stm32f4xx.h"                  // Device header
#include "DS1307.h"

//SCL:PB8 I2C1_ SCL
//SAD:PB9 I2C1_ SDA
#define SLAVE_ADDR		0x68							//p.12 ds1307ref

void i2c_Init(void);
void i2c_readbyte(char saddr,char maddr,char *data);
void delayMs(int n);


int main(void){
	char data;
	i2c_Init();
	RCC->AHB1ENR |= 1<<0;									//PA5 for LED
	GPIOA->MODER &=~(0x00000C00);
	GPIOA->MODER |= 0x00000400;
	
	while(1){

		i2c_readbyte(SLAVE_ADDR,0,&data);
		if(data&1){
			GPIOA->ODR ^= (0x00000020);
			delayMs(1000);
		}else{
			GPIOA->ODR &=~(0x00000020);
			delayMs(1000);
		}
	}
}

void i2c_Init(void){
	
	RCC->AHB1ENR |= 1<<1;									//GPIOB
	RCC->APB1ENR |= 1<<21;								//I2C1 Enable
	
	GPIOB->MODER &=~(1<<16);							//PB8 alternate
	GPIOB->MODER |= 1<<17;								
	GPIOB->MODER &=~(1<<18);							//PB9 alternate 
	GPIOB->MODER |= 1<<19;
	
	GPIOB->AFR[1]&=~(0xF<<0);							//0100: AF4 PB8 I2C SCL
	GPIOB->AFR[1]|= 0x1<<2;
	
	GPIOB->AFR[1]&=~(0xF<<4);							//0100: AF4	PB9 I2C SDA
	GPIOB->AFR[1]|= 0x1<<6;
	
	GPIOB->OTYPER |= (1<<8|1<<9);					//PB8 PB9 open-drain
	
	GPIOB->PUPDR  &=~(1<<17|1<<19);
	GPIOB->PUPDR  |= 1<<16|1<<18;					//PB8 PB9 pull-up
	
	//I2C config
	I2C1->CR1  = 1<<15;										//1: I2C Peripheral under reset state
	I2C1->CR1 &=~(1<<15);									//0: I2C Peripheral not under reset
	
	I2C1->CR2 |= 1<<4;										//0b010000 = 16MHz
	
	I2C1->CCR &=~(1<<14);									//Standart mode
	I2C1->CCR  = 80;											//100kHz clock ----- T = 2 * CCR * TPCLK1
	
	I2C1->TRISE = 17;											//max rise time
	
	I2C1->CR1	|= 1<<0;										//1: Peripheral enable
}

void i2c_readbyte(char saddr,char maddr,char *data){
	
	/*
	1.write wait
	2.write wait
	3.read wait
	4.write wait
	*/
	
	volatile int tmp;
	while(I2C1->SR2 & 1<<1);							//1: Communication ongoing on the bus
	/*Genearte start*/
	I2C1->CR1 |= 1<<8;										//1: Repeated start generation
	while(!(I2C1->SR1 & 1<<0));						//1: Start condition generated.
	/*Transmit slave addr + Write*/
	I2C1->DR = saddr<<1;									//DR[7:0] 8-bit data register
	while(!(I2C1->SR1 & 1<<1));						//1: Communication ongoing on the bus
	tmp = I2C1->SR2;											//Clear addr flag
	
	while(!(I2C1->SR1 & 1<<7));						//1: Data register empty
	/*Sent memory inside slave addr that want to read*/
	I2C1->DR = maddr;											//DR[7:0] 8-bit data register
	while(!(I2C1->SR1 & 1<<7));						//1: Data register empty
	/*Genearte restart*/
	I2C1->CR1 |= 1<<8;										//1: Repeated start generation
	while(!(I2C1->SR1 & 1<<0));						//1: Start condition generated.
	/*Transmit slave addr + Read*/
	I2C1->DR = saddr<<1|1;
	
	while(!(I2C1->SR1 & 1<<1));						//1: Communication ongoing on the bus
	/*Disable acknowledge*/
	I2C1->CR1 &=~(1<<10);									//0: No acknowledge returned
	tmp = I2C1->SR2;											//Clear addr
	
	I2C1->CR1 |= 1<<9;										//1: Stop generation after the current byte transfer or after the current Start condition is sent.
	
	while(!(I2C1->SR1 & 1<<6));						//1: Data register not empty
	*data++ = I2C1->DR;
	
}

void i2c_burstread(char saddr,char maddr,int n,char *data){
	volatile int tmp;
	
	while(I2C1->SR2 & 1<<1);							//1: Communication ongoing on the bus
	/*Genearte start*/
	I2C1->CR1 |= 1<<8;										//1: Repeated start generation
	while(!(I2C1->SR1 & 1<<0));						//1: Start condition generated.
	/*Transmit slave addr + Write*/
	I2C1->DR = saddr<<1;									//DR[7:0] 8-bit data register
	while(!(I2C1->SR1 & 1<<1));						//1: Communication ongoing on the bus
	tmp = I2C1->SR2;											//Clear addr flag
	while(!(I2C1->SR1 & 1<<7));						//1: Data register empty
	/*Sent memory inside slave addr that want to read*/
	I2C1->DR = maddr;											//DR[7:0] 8-bit data register
	while(!(I2C1->SR1 & 1<<7));						//1: Data register empty
	/*Genearte restart*/
	I2C1->CR1 |= 1<<8;										//1: Repeated start generation
	while(!(I2C1->SR1 & 1<<0));						//1: Start condition generated.
	/*Transmit slave addr + Read*/
	I2C1->DR = saddr<<1|1;
	while(!(I2C1->SR1 & 1<<1));						//1: Communication ongoing on the bus
	tmp = I2C1->SR2;											//Clear addr
	I2C1->CR1 |= 1<<10;										//1: Acknowledge returned after a byte is received (matched address or data)
	
	while(n>0U){
		/*IF ONE BYTE*/
		if(n==1U){
			/*Disable acknowledge*/
			I2C1->CR1 &=~(1<<10);							//0: No acknowledge returned
			I2C1->CR1 |= 1<<9;								//1: Stop generation after the current byte transfer or after the current Start condition is sent.
			while(!(I2C1->SR1 & 1<<6));				//1: Data register not empty
			*data++ = I2C1->DR;
			break;
		}else{
			while(!(I2C1->SR1 & 1<<6));				//1: Data register not empty
			*data++ = I2C1->DR;
			n--;
		}
	}
}

void i2c_burstwrite(char saddr,char maddr,int n,char *data){
	volatile int tmp;
	while(I2C1->SR2 & 1<<1);							//1: Communication ongoing on the bus
	/*Genearte start*/
	I2C1->CR1 |= 1<<8;										//1: Repeated start generation
	while(!(I2C1->SR1 & 1<<0));						//1: Start condition generated.
	/*Transmit slave addr + Write*/
	I2C1->DR = saddr<<1;									//DR[7:0] 8-bit data register
	while(!(I2C1->SR1 & 1<<1));						//1: Communication ongoing on the bus
	tmp = I2C1->SR2;											//Clear addr flag
	while(!(I2C1->SR1 & 1<<7));						//1: Data register empty
	/*Sent memory inside slave addr that want to read*/
	I2C1->DR = maddr;											//DR[7:0] 8-bit data register
	for(int i=0;i<n;i++){
		while(!(I2C1->SR1 & 1<<7));					//1: Data register empty
		I2C1->DR = *data++;
	}
	while(!(I2C1->SR1&1<<2));							//1: Data byte transfer succeeded
	I2C1->CR1 |= 1<<9;										//1: Stop generation after the current byte transfer or after the current Start condition is sent.
}


void delayMs(int n){
	int i=0;
	for(;n>0;n--){
		for(i=0;i<3195;i++){
		}
	}
}











