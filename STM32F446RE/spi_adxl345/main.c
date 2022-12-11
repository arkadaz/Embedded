#include "stm32f4xx.h"                  // Device header
#include "spi.h"
#include "adxl345.h"

//SCL:PB8 I2C1_ SCL
//SAD:PB9 I2C1_ SDA
uint16_t x,y,z;
float xg,yg,zg;
uint8_t data_rec[6];
void delayMs(int n);


int main(void){
	RCC->AHB1ENR |= 1<<0;									//PA5 for LED
	GPIOA->MODER &=~(0x00000C00);
	GPIOA->MODER |= 0x00000400;
	
	adxl_init();
	
	while(1){
		adxl_read(DATA_START_ADDR,data_rec);
		x=((data_rec[1]<<8)|data_rec[0]);
		y=((data_rec[3]<<8)|data_rec[2]);
		z=((data_rec[5]<<8)|data_rec[4]);
		
		xg=(x*0.0078);
		yg=(y*0.0078);
		zg=(z*0.0078);
	}
}



void delayMs(int n){
	int i=0;
	for(;n>0;n--){
		for(i=0;i<3195;i++){
		}
	}
}

