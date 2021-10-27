/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : __RFID_H.c
  * @brief          : 
  ******************************************************************************
  * @attention
  *
  * 
  * 
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "RFID.h"
#include "Uart4.h"
#include "Beep.h"
#include "stdio.h"
#include "string.h"
#include "oled.h"
#include "gui.h"
#include "Mcu_SendData_Route.h"
#include "Uart5.h"
#include "delay.h"
#include "os_time_pro.h"
#include "FirstPower.h"

extern uint8_t NowState; 

extern uint8_t RelayState;  //继电器状态 0关 1开
extern uint8_t BeepState;  //继电器状态 0关 1开
extern uint8_t Uid;
extern uint8_t PrinterMode;  
extern uint8_t Finger_IRQn;
extern uint8_t PowerAfterState; //系统上电标志
extern uint8_t OpenLED[];
extern uint8_t FingerState;  //指纹状态：0：未验证 1：验证成功
extern uint8_t ZeroArry[2];

extern TTimerItem RelayLoopTimer;
extern TTimerItem RFIDLoopTimer;
extern TTimerItem WorkAfterTimer;
TTimerItem RFIDWarnTimer;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static uint8_t RFIDrx_buffer[36];  //mcu接受zigbee数据缓存

uint16_t countTwo = 0; //RFID触发计数 
uint16_t countendTwo = 0; //RFID触发计数 
uint16_t countOne = 0; //RFID触发计数 
uint8_t RFIDState = 0; //RFID状态 0：未触发，1：触发
uint8_t WarningCount = 0; //工作中状态下RFID警告计数，每秒+1；


uint8_t OldFingerState;
uint8_t OldRFIDState;
uint8_t OldNowState;

u8 RFID_Stop[10] ={0x5A,0X00,0X01,0X02,0XFF,0X00,0X00,0X88,0X5A};
u8 RFID_Read[15] ={0x5A,0x00,0x01,0x02,0x10,0x00,0x05,0X00,0X00,0X00,0X02,0X01,0XA1,0XD4}; //设置RFID为自动回复模式

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  RFID模式设置函数
  * @retval void
  */
void Send_Data(void)
{
	/*RFID 设置模式为自动发送读到N个标签返回N个通知帧*/
	USART4_Send_String(RFID_Stop,9);
	USART4_Send_String(RFID_Read,14);
	os_timer_disarm(&RFIDLoopTimer);
}

/**
  * @brief  RFID回传串口数据解析
  * @retval uint8_t
  */
uint8_t *RFID_uart_data_analyze(uint8_t rx_data)
{
	static uint8_t rx_index = 0;
	static uint8_t pack_size = 0;
	
	RFIDrx_buffer[rx_index] = rx_data;
	
	if(rx_index == 0)
	{
		if(RFIDrx_buffer[rx_index] != 0x5A)
			rx_index = 0;
		else
			rx_index ++;
	}

	else
	{
		rx_index ++;	
		if(rx_index >= 34)
		{
			rx_index = 0;
			return RFIDrx_buffer;
		}						
	}	
	return NULL;
}


/**
  * @brief  RFID模式设置函数
  * @retval void
  */
void UART4_IRQHandler(void) 
{
	u8 Res;
	uint8_t *pRxBuffer;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(UART4);//读取接收到的数据	
		pRxBuffer = RFID_uart_data_analyze( Res );
		if( pRxBuffer )
		{
			RFIDFunction(pRxBuffer);
		}		
	}
}

/**
  * @brief  RFID 循环检测是否还是处于报警状态
  * @retval void
  */
void RFIDCountClean(void)
{
	if(RFIDState == 1) //RFID触发警报
	{
		if(OldNowState == 0)//上一个状态为待机状态
		{
			if(NowState == 1) //下一状态为工作状态
			{
				/*关闭继电器*/
				RelayOpen(0);
				Mcu_SendData_Route(0x0A,ZeroArry,2,Uid);
				OLED_Clear(0);
				GUI_ShowCHinese(32,0,16,"违规操作",1);
				GUI_ShowCHinese(32,16,16,"正在关机",1);
				os_timer_arm(&RelayLoopTimer,30000,0,RelayLoopControl);
			}
		}		
		RFIDState = 0;
		countendTwo = 0;				
	}
	
	else //if(RFIDState == 0)
	{		
		countendTwo++;
		if(countendTwo>6)
		{
			countendTwo = 0;
			RFIDState = 0;
			/*关蜂鸣器*/
			BeepOpen(0);
			/*LCD显示*/
			OLED_Clear(0);
			GUI_ShowCHinese(48,8,16,"待机",1);			
		}		
	}
}

void RelayLoopControl(void)
{
	if(RFIDState == 0)
	{	
		countendTwo++;
		if(countendTwo>6)
		{	  
			/*开启继电器*/
			RelayOpen(1);
			OLED_Clear(0);
			GUI_ShowCHinese(32,8,16,"正在开机",1);	
			/*打印机重新上电后连续检测是否处在稳定状态*/
			os_timer_arm(&WorkAfterTimer,200,1,WorkAfterTimeFunc);
			os_timer_disarm(&RelayLoopTimer);		
		}
	}
}



/**
  * @brief  RFID控制函数
  * @retval int
  */
int RFIDFunction(uint8_t *Pack)
{
	if(Pack[27] == 0x00 && BeepState == 0) //RFID检查到标签并且蜂鸣器状态为关
	{
		Pack[27] = 0xFF;
		countTwo++;
		if(countTwo > 2 ) //连续检查3次 确认RFID是否检查都标签
		{		
			countTwo=0;
			RFIDState = 1;
			if(NowState == 0) //RFID从未报警转报警，记录当前的指纹，工作模式,RFID的状态 
			{
				OldFingerState = FingerState;
				OldRFIDState = RFIDState;
				OldNowState = NowState;
				os_timer_arm(&RFIDWarnTimer,500,1,RFIDCountClean);				
			}							
			/*开蜂鸣器*/
			BeepOpen(1);								

			OLED_Clear(0);
			GUI_ShowCHinese(0,8,16,"警告涉密文件靠近",1);				
			Mcu_SendData_Route(0X08,ZeroArry,2,Uid);
		}
	}
	else 
	{		
	}
	return 0;	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
