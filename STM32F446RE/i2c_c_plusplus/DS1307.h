#ifndef _DS_1307_H
#define _DS_1307_H
#include "stm32f4xx.h"
#include <stdint.h>

#define SLAVE_ADDR		0x68							//p.12 ds1307ref

class Ds1307
{
	public:
		
		typedef struct{
			uint8_t year;
      uint8_t month;
      uint8_t day;
      uint8_t date;
    }RTC_date_t;

		typedef struct{
      uint8_t hour;
      uint8_t minute;
      uint8_t second;
      uint8_t time_format;
    }RTC_time_t;
		
		enum MONTH : uint8_t {
      MONTH_JAN = 1,
      MONTH_FEB = 2,
      MONTH_MAR = 3,
      MONTH_APR = 4,
      MONTH_MAY = 5,
      MONTH_JUN = 6,
      MONTH_JUL = 7,
      MONTH_AUG = 8,
      MONTH_SET = 9,
      MONTH_OCT = 10,
      MONTH_NOV = 11,
      MONTH_DEC = 12
      };
		
		uint8_t ds1307_init(void);
			
		void ds1307_set_current_time(RTC_time_t *rtc_time);
		void ds1307_get_current_time(RTC_time_t *rtc_time);
			
		void ds1307_set_current_date(RTC_date_t *rtc_date);
		void ds1307_get_current_date(RTC_date_t *rtc_date);
		

	private:
		uint8_t _i2c_address;
		void ds1307_write(uint8_t value,uint8_t reg_addr);

};

#endif // _DS_1307_H