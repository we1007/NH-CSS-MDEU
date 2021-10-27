/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : finger.c
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
#include "finger.h"
#include "stdio.h"
#include "string.h"
#include "os_time_pro.h"
#include "Uart5.h"
#include "Beep.h"
#include "oled.h"
#include "gui.h"
#include "Mcu_SendData_Route.h"
#include "common_cmd.h"
#include "McuDataFlow.h"
#include "Power_Ene.h"
#include "FirstPower.h"
#include "delay.h"

#define Aftertime 2000  //打印机上电40秒后

TTimerItem FingerSendTime;
TTimerItem PowerChangeTimer;
TTimerItem FirstPowerTimer;
TTimerItem PowerMatchTimer; 
TTimerItem PowerAfterTimer; //打印机上电

extern uint8_t ExportFlie;
extern uint8_t MatchFlag;
extern uint8_t RFIDState;
extern uint8_t PowerAfterState; //系统上电标志
extern uint8_t PowerInitFlag;
extern uint8_t ZeroArry[2];
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TX_BUFFER_MAX_LEN 150
#define DATA_HEAD 				0x55  //通信数据包头


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint8_t rx_index = 0;
static uint8_t rx_buffer[30];  //mcu接受zigbee数据缓存
uint16_t RX_CheckSum = 0;
uint8_t FSendCount = 0; //发送指纹匹配次数


uint8_t OpenLED[] = {0x55,0xAA,0X00,0X00,0X24,0X00,
										0X02,0X00,0X01,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X26,0X01};

uint8_t CloseLED[] = {0x55,0xAA,0X00,0X00,0X24,0X00,
										0X02,0X00,0X00,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X25,0X01};
										
uint8_t GetImage[]={0x55,0xAA,0X00,0X00,0X20,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X00,0X00,0X00,0X00,0X00,0X00,
										0X1F,0X01}; // 采集图像

uint8_t PickUpImage[]={ 0x55,0xAA,0X00,0X00,0X60,0X00,
												0X02,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0X61,0x01}; // 从暂存在ImageBuffer中的指纹图像产生模板

												
uint8_t SearchImage[]={ 0x55,0xAA,0X00,0X00,0X63,0X00,
												0X06,0X00,0X00,0X00,0X01,0X00,
												0X50,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0XB9,0x01}; // 

uint8_t FingerMode = 0xDB; //指纹模式 0：匹配模式，1：更新模式
uint8_t FingerState = 0;  //指纹状态：0：未验证 1：验证成功
uint8_t Finger_IRQn = 0;
uint8_t LEDState = 0;												
uint8_t Uid;	//开机用户的ID									
												
												

/*1分钟判断功率变化*/
uint8_t Powerchangecount = 0;
uint8_t PowerLimits = 0;
double PowerV = 0;
extern double StablePowerValue;
												
												


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



/**
  * @brief  指纹触发口初始化
  * @retval void
  */
void Finger_BUTTON_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the BUTTON Clock */
	/* 使能KEY按键对应GPIO的Clock时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure Button pin as input floating */
	/* 初始化KEY按键的GPIO管脚，配置为带上拉的输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

/**
  * @brief  指纹串口中断
  * @retval void
  */
void EXTI4_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line4); //
}

/**
  * @brief  指纹串口中断
  * @retval void
  */
void UART5_IRQHandler(void) 
{
	u8 Res;
	uint8_t *pRxBuffer;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(UART5);//读取接收到的数据	
		pRxBuffer = finger_uart_data_analyze( Res );
		if( pRxBuffer )
		{
			FingerControl(pRxBuffer);
			rx_index = 0;
		}		
	}
}

/**
  * @brief  指纹回传串口数据解析
  * @retval uint8_t
  */
uint8_t *finger_uart_data_analyze(uint8_t rx_data)
{
	
	static uint16_t pack_size = 0;
	
	rx_buffer[rx_index] = rx_data;
	
	if(rx_index == 0)
	{
		if(rx_buffer[rx_index] != 0xAA)
			rx_index = 0;
		else
			rx_index ++;
	}
	else if(rx_index == 1)
	{
		if(rx_buffer[rx_index] != 0x55)
			rx_index = 0;
		else
			rx_index ++;
	}
	
	else if(rx_index == 6)
	{
		pack_size = (rx_buffer[7] << 8) | rx_buffer[6];
		rx_index ++;		
		if(pack_size >= 50)	//数据包长度不能大于26 0-25
		{
			pack_size = 0;
			rx_index = 0;
		}
	}
	
	else if((rx_index-7) == (pack_size)) //计数 = 包长 
	{
		rx_index = 0;
		return rx_buffer;
	}
	
	else
	{
		rx_index ++;	
		if(rx_index > 26)
			rx_index = 0;					
	}	
	return NULL;
}

/** MUC发送数据给路由 EF 01 FF FF FF FF 02/08 00 len  data checksum 
  * @brief  
  * @retval int
  */
void Mcu_SendData_Finger(uint8_t cmd,const uint8_t *data,uint8_t data_size)
{
	uint8_t i;
	uint8_t pack_length;
	uint8_t check;
	
	uint8_t buf[TX_BUFFER_MAX_LEN];
	/*包头*/
	buf[pack_length++] =  0XEF;
	buf[pack_length++] =  0X01;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	
	/*数据包类型*/
	/*长度*/
	buf[pack_length++] =  11+data_size;
	/*指令*/
	buf[pack_length++] =  cmd;	
	/*数据*/
	if(data_size > 0)
	{
		for(i = 0;i < data_size;i++)
		{
			buf[pack_length++] =  *(data + i);	
		}
	}
	/*校验*/
	check = CheckSum16(buf,pack_length);
	buf[pack_length++] = (check >> 8)&0xff;
	buf[pack_length++] = check&0xff; //低8位
	
	UART5_Send_String(buf,pack_length);
}

/**
  * @brief  指纹匹配
  * @retval void
	* Finger_IRQn = 1  采集图像成功
	*	Finger_IRQn = 2  提取特征模板成功
	* Finger_IRQn = 3  匹配模板成功
	*
	*
  */
void FingerControl(uint8_t *pData)
{
	switch(FingerMode)
	{
		case 0XDB: //指纹匹配模式
		{
			if(pData[8] == 0x00) //回复采集图像正确，发送提取特征
			{	
				switch(FSendCount)
				{
					case 1: //采集图像成功，发送提取特征
					{
						Finger_IRQn = 1;
						FSendCount = 2;
						UART5_Send_String(PickUpImage,26);	
						break;
					}						
					case 2: //提取特征成功，发送搜索
					{
						Finger_IRQn = 2;
						FSendCount = 3;
						UART5_Send_String(SearchImage,26);			
						break;
					}						
					case 3:
					{
						if(pData[6] == 0x05) //搜索成功，开继电器
						{
							/*改变指纹状态*/
							Finger_IRQn = 3;
							/*指纹已验证标志*/
							FingerState = 1;
							Powerchangecount = 0;	
							/*开启匹配*/
							PowerInitFlag = 1;							
							/*关灯*/
							UART5_Send_String(CloseLED,26);										
							/*开继电器*/							
							RelayOpen(1); 	 							
							/*显示屏刷新*/					
							OLED_Clear(0);
							GUI_ShowCHinese(16,8,16,"指纹验证成功",1);
							/*发送审计记录*/
							Uid = pData[10]; 
							Mcu_SendData_Route(0X01,ZeroArry,2,Uid);							
							/*n分钟无操纵自动待机 500ms轮询一次*/
							os_timer_arm(&PowerChangeTimer,500,1,OneMinutePowerJudge);		
							/*指纹验证n秒后*/
							os_timer_arm(&PowerAfterTimer,Aftertime,0,PowerAfterFunc);	
								
						}
						else 
						{
							Finger_IRQn = 0;
						}
						break;
					}	/*case three end*/				
					default:
						break;						
				}/*switch end*/			
			}/*if(pData[9] == 0x00) end */
			else 
			{
				Finger_IRQn = 0;
				return;
			}				
			break;
		} 		
		default:
			break;
	}	//switch end 
}

/**
  * @brief  循环发送指纹匹配
  * @retval void
  */
void FingerSend(void)
{
	if((Finger_IRQn == 0) && (FingerMode == 0XDB)) // 指纹未验证 && 指纹验证模式
	{		
		FSendCount = 1;
		UART5_Send_String(GetImage,26);
		/*开指纹灯*/
		UART5_Send_String(OpenLED,26);
	}
	else
		return;
}

/**
  * @brief  
  * @retval void
  */
void Finger_Search_Init(void)
{
	os_timer_arm(&FingerSendTime,1000,1,FingerSend);
}

#if 1

/**
  * @brief  指纹验证后
  * @retval void
  */
void PowerAfterFunc()
{
	os_timer_disarm(&PowerAfterTimer);
	/*开指纹灯*/
	UART5_Send_String(OpenLED,26); 
	/*改变指纹状态*/
	Finger_IRQn = 0;
	/*显示屏刷新*/					
	OLED_Clear(0);
	if(ExportFlie == 1)
	{
		GUI_ShowCHinese(0,8,16,"导出审计文件完成",1);	
		ExportFlie = 0;
	}
		
	else 
		GUI_ShowCHinese(24,8,16,"用户已登录",1);
	
	
}

/**
  * @brief  One Minute Power Judge
  * @retval void
  */
void OneMinutePowerJudge(void)
{
	PowerV =(StablePowerValue + (StablePowerValue*ErrorMinCoefficient));
	
	if(Power_Ene.power <= PowerV)
		Powerchangecount++;
	else 
		Powerchangecount = 0;
	
	if(Powerchangecount >= 120)
	{
		Powerchangecount = 0;
		/*指纹未验证*/
		FingerState = 0;
		/*关闭继电器*/
		//RelayOpen(0); 
		/*发送审计记录*/
		Mcu_SendData_Route(0x02,ZeroArry,2,Uid); 
		/*显示屏刷新*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"待机",1);
		os_timer_disarm(&PowerChangeTimer);
	}			
}
#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
