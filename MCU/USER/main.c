#include "stm32f10x.h"
#include "SysClockConfig.h"
#include "os_time_pro.h"
#include "UartPrintf.h"
#include "McuDataFlow.h" 
#include "IOConf.h"
#include "oled.h"
#include "gui.h"
#include "delay.h"
#include "Uart1.h"
#include "Uart5.h"
#include "Mcu_SendData_Route.h"
#include "Power_Ene.h"
#include "finger.h"
#include "ds3231.h"
#include "flash.h"

int main(void)
{	
	uint8_t arry[20];
	/*初始化*/
	MCU_Init();
	/*系统任务调度*/
	System();
	
	while(1)
	{
		os_timer_handle();
	}
}

