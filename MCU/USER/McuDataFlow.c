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
extern uint8_t FingerState;  //ָ��״̬��0��δ��֤ 1����֤�ɹ�
extern uint8_t PowerStableFlag; //�����ȶ���־
extern  TTimerItem StableTimer;
TTimerItem DS232Timer;
TTimerItem PowerLoopTimer;
TTimerItem RFIDLoopTimer;
TTimerItem BeepLoopTimer;
TTimerItem WOrkRFIDTimer;


uint8_t Time_flag;

/*�ϵ��ʼ����־
* PowerInitFlag = 0 δ��ʼ�����
* PowerInitFlag = 1 ��ʼ�����
*/
uint8_t PowerInitFlag = 0;  


/**
  * @brief  
  * @retval void
  */
	
void MCU_Init(void)
{
	/*ϵͳ�ڲ�ʱ�ӳ�ʼ��*/
	RCC_Configuration();
	/* ��ʱ������ʼ�� */
	delay_init();
	/*Tim4 ������ȶ�ʱ����ʼ��*/
	os_timer_init();
	/*������and�̵���IO�ڳ�ʼ��*/
	IO_Init();
	/*ʱ��ģ��*/
	DS3231_Init();
	/*���ڳ�ʼ��*/
	STM32_Uart4_Init(115200); //RFID Uart4
	STM32_Uart1_Init(57600); 	//MCU��MTK
	STM32_Uart5_Init(115200); //ָ��
	/*��������ʼ��*/
	HLW8110_Usart_Init(); 
	Write_Rmsl_Init();
	Read_Rmsl_Cmd_Time();
	/*OLED��ʼ��*/
	OLED_Init();
	OLED_Clear(0);
	GUI_ShowCHinese(32,8,16,"���ڿ���",1);	
	/*�ϵ��ָ��ģ�鿪��*/
	UART5_Send_String(OpenLED,1); //���Ѵ���
	UART5_Send_String(OpenLED,26);
	FingerState = 1;	
	/*�̵�������*/
	RelayOpen(1);
	/*��LEDָʾ��*/
	LedOpen(1); 
	/*�ж��Ƿ�ƥ��ù��ʣ���ȡflash�еĹ���ֵ*/
	FirstPower();
	#if 0
	/*ϵͳ��һ���ϵ��ʼ�� ����ʱ�� ƥ�书��*/
	Flash_Read(DS3231FIRSTPOWER,&Time_flag,1);
	if(Time_flag==0xFF)//�ϵ�Ĭ�ϳ�ʼ������һ���´��룬��һ��ʱ����ֵ
	{
		Set_DS3231_Time(21,8,10,5,10,30,0);					 
	}
	/*�ж��Ƿ�ƥ��ù��ʣ���ȡflash�еĹ���ֵ*/
	FirstPower();
	#endif	
}


/**
  * @brief  
  * @retval void
  */
void System(void)
{	
	/*������������ */
	//os_timer_arm(&BeepLoopTimer,1000,1,BeepLoopControl);
	/*ʱ��ʵʱ����*/
	os_timer_arm(&DS232Timer,400,1,Get_DS3231_Time);	
	/*ָ������*/
	Finger_Search_Init();
	/*����RFID*/
	os_timer_arm(&RFIDLoopTimer,2000,0,Send_Data);	
	
	#if 0	
	/*��ӡ������ģʽ��ѯ�ж�*/
	os_timer_arm(&PowerLoopTimer,50,1,Get_DS3231_Time);
	#endif
}
	

