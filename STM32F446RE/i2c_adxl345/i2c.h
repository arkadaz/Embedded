#ifndef I2C_H_
#define I2C_H_
#include "stm32f4xx.h"                  // Device header

void i2c_Init(void);
void i2c_readbyte(char saddr,char maddr,char *data);
void i2c_burstread(char saddr,char maddr,int n,char *data);
void i2c_burstwrite(char saddr,char maddr,int n,char *data);



#endif /*I2C_H_*/