#include "DS1307.h"

uint8_t Ds1307::ds1307_init(void){
	//Make CH to 0
	ds1307_write(0x00,SLAVE_ADDR);
	
}

void Ds1307::ds1307_set_current_time(RTC_time_t *rtc_time){
	
}

void Ds1307::ds1307_get_current_time(RTC_time_t *rtc_time){
	
}
			
void Ds1307::ds1307_set_current_date(RTC_date_t *rtc_date){
	
}

void Ds1307::ds1307_get_current_date(RTC_date_t *rtc_date){
	
}

void Ds1307::ds1307_write(uint8_t value,uint8_t reg_addr){
	uint8_t tx[2];
	tx[0]=reg_addr;
	tx[1]=value;
}