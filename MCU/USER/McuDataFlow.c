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
#include "Uart4.h"
#include "Mcu_SendData_Route.h"
#include "Power_Ene.h"
#include "finger.h"
#include "ds3231.h"
#include "flash.h"
#include "Uart4.h"
#include "RFID.h"
#include "Beep.h"
#include "FirstPower.h"
#include "delay.h"

extern uint8_t ZeroArry[2];
extern uint8_t OpenLED[];
extern uint8_t FingerState;  //指纹状态：0：未验证 1：验证成功
extern uint8_t PowerStableFlag; //功率稳定标志
extern  TTimerItem StableTimer;
TTimerItem DS232Timer;
TTimerItem PowerLoopTimer;
TTimerItem RFIDLoopTimer;
TTimerItem BeepLoopTimer;
TTimerItem WOrkRFIDTimer;


uint8_t Time_flag;

/*上电初始化标志
* PowerInitFlag = 0 未初始化完成
* PowerInitFlag = 1 初始化完成
*/
uint8_t PowerInitFlag = 0;  


/**
  * @brief  
  * @retval void
  */
	
void MCU_Init(void)
{
	/*系统内部时钟初始化*/
	RCC_Configuration();
	/* 延时函数初始化 */
	delay_init();
	/*Tim4 任务调度定时器初始化*/
	os_timer_init();
	/*蜂鸣器and继电器IO口初始化*/
	IO_Init();
	/*时钟模块*/
	DS3231_Init();
	/*串口初始化*/
	STM32_Uart4_Init(115200); //RFID Uart4
	STM32_Uart1_Init(57600); 	//MCU与MTK
	STM32_Uart5_Init(115200); //指纹
	/*电量检查初始化*/
	HLW8110_Usart_Init(); 
	Write_Rmsl_Init();
	Read_Rmsl_Cmd_Time();
	/*OLED初始化*/
	OLED_Init();
	OLED_Clear(0);
	GUI_ShowCHinese(32,8,16,"正在开机",1);	
	/*上电给指纹模块开灯*/
	UART5_Send_String(OpenLED,1); //唤醒串口
	UART5_Send_String(OpenLED,26);
	FingerState = 1;	
	/*继电器常开*/
	RelayOpen(1);
	/*开LED指示灯*/
	LedOpen(1); 
	/*判断是否匹配好功率，读取flash中的功率值*/
	FirstPower();
	#if 0
	/*系统第一次上电初始化 设置时间 匹配功率*/
	Flash_Read(DS3231FIRSTPOWER,&Time_flag,1);
	if(Time_flag==0xFF)//上电默认初始化，第一次下代码，给一个时间数值
	{
		Set_DS3231_Time(21,8,10,5,10,30,0);					 
	}
	/*判断是否匹配好功率，读取flash中的功率值*/
	FirstPower();
	#endif	
}


/**
  * @brief  
  * @retval void
  */
void System(void)
{	
	/*蜂鸣器器控制 */
	//os_timer_arm(&BeepLoopTimer,1000,1,BeepLoopControl);
	/*时钟实时更新*/
	os_timer_arm(&DS232Timer,400,1,Get_DS3231_Time);	
	/*指纹搜索*/
	Finger_Search_Init();
	/*启用RFID*/
	os_timer_arm(&RFIDLoopTimer,2000,0,Send_Data);	
	
	#if 0	
	/*打印机工作模式轮询判读*/
	os_timer_arm(&PowerLoopTimer,50,1,Get_DS3231_Time);
	#endif
}
	

