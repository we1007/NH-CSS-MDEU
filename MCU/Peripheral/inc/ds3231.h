#ifndef __DS3231_H  
#define __DS3231_H  
#include "sys.h"    
#include "stm32f10x.h"
typedef struct
{
	u16 year;
	u8  month;
	u8  day;
	u8  week;
	u8  hour;
	u8  minute;
	u8  second;
}Calendar;

#define SCL_PIN        GPIO_Pin_10  //板上第11针
#define SDA_PIN        GPIO_Pin_11  //板上第17针
#define SCL_PORT       GPIOB  
#define SDA_PORT       GPIOB  
#define SCL_RCC_CLOCK  RCC_APB2Periph_GPIOB  
#define SDA_RCC_CLOCK  RCC_APB2Periph_GPIOB  
      
#define SCL_H         GPIOB->BSRR = GPIO_Pin_10 
#define SCL_L         GPIOB->BRR  = GPIO_Pin_10 
        
#define SDA_H         GPIOB->BSRR = GPIO_Pin_11
#define SDA_L         GPIOB->BRR  = GPIO_Pin_11 
      
#define SCL_read      GPIOB->IDR  & GPIO_Pin_10 
#define SDA_read      GPIOB->IDR  & GPIO_Pin_11 

      
#define DS3231_ADDRESS	      0x68 //I2C Slave address

/* DS3231 Registers. Refer Sec 8.2 of application manual */
#define DS3231_SEC_REG        0x00  
#define DS3231_MIN_REG        0x01  
#define DS3231_HOUR_REG       0x02
#define DS3231_WDAY_REG       0x03
#define DS3231_MDAY_REG       0x04
#define DS3231_MONTH_REG      0x05
#define DS3231_YEAR_REG       0x06

#define DS3231_AL1SEC_REG     0x07
#define DS3231_AL1MIN_REG     0x08
#define DS3231_AL1HOUR_REG    0x09
#define DS3231_AL1WDAY_REG    0x0A

#define DS3231_AL2MIN_REG     0x0B
#define DS3231_AL2HOUR_REG    0x0C
#define DS3231_AL2WDAY_REG    0x0D

#define DS3231_CONTROL_REG          0x0E
#define DS3231_STATUS_REG           0x0F
#define DS3231_AGING_OFFSET_REG     0x0F
#define DS3231_TMP_UP_REG           0x11
#define DS3231_TMP_LOW_REG          0x12

#define EverySecond     0x01
#define EveryMinute     0x02
#define EveryHour       0x03





void I2C_GPIO_Config(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
u8   I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
u8   I2C_ReceiveByte(void);

void DS3231_Init(void);
void Get_DS3231_Time(void);   

void Set_DS3231_Time(u8 syear,u8 smon,u8 sday,u8 week,u8 hour,u8 min,u8 sec);//设置时间 
#endif
