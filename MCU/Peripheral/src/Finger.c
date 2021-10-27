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

#define Aftertime 2000  //��ӡ���ϵ�40���

TTimerItem FingerSendTime;
TTimerItem PowerChangeTimer;
TTimerItem FirstPowerTimer;
TTimerItem PowerMatchTimer; 
TTimerItem PowerAfterTimer; //��ӡ���ϵ�

extern uint8_t ExportFlie;
extern uint8_t MatchFlag;
extern uint8_t RFIDState;
extern uint8_t PowerAfterState; //ϵͳ�ϵ��־
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
#define DATA_HEAD 				0x55  //ͨ�����ݰ�ͷ


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint8_t rx_index = 0;
static uint8_t rx_buffer[30];  //mcu����zigbee���ݻ���
uint16_t RX_CheckSum = 0;
uint8_t FSendCount = 0; //����ָ��ƥ�����


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
										0X1F,0X01}; // �ɼ�ͼ��

uint8_t PickUpImage[]={ 0x55,0xAA,0X00,0X00,0X60,0X00,
												0X02,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0X61,0x01}; // ���ݴ���ImageBuffer�е�ָ��ͼ�����ģ��

												
uint8_t SearchImage[]={ 0x55,0xAA,0X00,0X00,0X63,0X00,
												0X06,0X00,0X00,0X00,0X01,0X00,
												0X50,0X00,0X00,0X00,0X00,0X00,
												0X00,0X00,0X00,0X00,0X00,0X00,
												0XB9,0x01}; // 

uint8_t FingerMode = 0xDB; //ָ��ģʽ 0��ƥ��ģʽ��1������ģʽ
uint8_t FingerState = 0;  //ָ��״̬��0��δ��֤ 1����֤�ɹ�
uint8_t Finger_IRQn = 0;
uint8_t LEDState = 0;												
uint8_t Uid;	//�����û���ID									
												
												

/*1�����жϹ��ʱ仯*/
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
  * @brief  ָ�ƴ����ڳ�ʼ��
  * @retval void
  */
void Finger_BUTTON_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the BUTTON Clock */
	/* ʹ��KEY������ӦGPIO��Clockʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure Button pin as input floating */
	/* ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
}

/**
  * @brief  ָ�ƴ����ж�
  * @retval void
  */
void EXTI4_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line4); //
}

/**
  * @brief  ָ�ƴ����ж�
  * @retval void
  */
void UART5_IRQHandler(void) 
{
	u8 Res;
	uint8_t *pRxBuffer;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(UART5);//��ȡ���յ�������	
		pRxBuffer = finger_uart_data_analyze( Res );
		if( pRxBuffer )
		{
			FingerControl(pRxBuffer);
			rx_index = 0;
		}		
	}
}

/**
  * @brief  ָ�ƻش��������ݽ���
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
		if(pack_size >= 50)	//���ݰ����Ȳ��ܴ���26 0-25
		{
			pack_size = 0;
			rx_index = 0;
		}
	}
	
	else if((rx_index-7) == (pack_size)) //���� = ���� 
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

/** MUC�������ݸ�·�� EF 01 FF FF FF FF 02/08 00 len  data checksum 
  * @brief  
  * @retval int
  */
void Mcu_SendData_Finger(uint8_t cmd,const uint8_t *data,uint8_t data_size)
{
	uint8_t i;
	uint8_t pack_length;
	uint8_t check;
	
	uint8_t buf[TX_BUFFER_MAX_LEN];
	/*��ͷ*/
	buf[pack_length++] =  0XEF;
	buf[pack_length++] =  0X01;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	buf[pack_length++] =  0XFF;
	
	/*���ݰ�����*/
	/*����*/
	buf[pack_length++] =  11+data_size;
	/*ָ��*/
	buf[pack_length++] =  cmd;	
	/*����*/
	if(data_size > 0)
	{
		for(i = 0;i < data_size;i++)
		{
			buf[pack_length++] =  *(data + i);	
		}
	}
	/*У��*/
	check = CheckSum16(buf,pack_length);
	buf[pack_length++] = (check >> 8)&0xff;
	buf[pack_length++] = check&0xff; //��8λ
	
	UART5_Send_String(buf,pack_length);
}

/**
  * @brief  ָ��ƥ��
  * @retval void
	* Finger_IRQn = 1  �ɼ�ͼ��ɹ�
	*	Finger_IRQn = 2  ��ȡ����ģ��ɹ�
	* Finger_IRQn = 3  ƥ��ģ��ɹ�
	*
	*
  */
void FingerControl(uint8_t *pData)
{
	switch(FingerMode)
	{
		case 0XDB: //ָ��ƥ��ģʽ
		{
			if(pData[8] == 0x00) //�ظ��ɼ�ͼ����ȷ��������ȡ����
			{	
				switch(FSendCount)
				{
					case 1: //�ɼ�ͼ��ɹ���������ȡ����
					{
						Finger_IRQn = 1;
						FSendCount = 2;
						UART5_Send_String(PickUpImage,26);	
						break;
					}						
					case 2: //��ȡ�����ɹ�����������
					{
						Finger_IRQn = 2;
						FSendCount = 3;
						UART5_Send_String(SearchImage,26);			
						break;
					}						
					case 3:
					{
						if(pData[6] == 0x05) //�����ɹ������̵���
						{
							/*�ı�ָ��״̬*/
							Finger_IRQn = 3;
							/*ָ������֤��־*/
							FingerState = 1;
							Powerchangecount = 0;	
							/*����ƥ��*/
							PowerInitFlag = 1;							
							/*�ص�*/
							UART5_Send_String(CloseLED,26);										
							/*���̵���*/							
							RelayOpen(1); 	 							
							/*��ʾ��ˢ��*/					
							OLED_Clear(0);
							GUI_ShowCHinese(16,8,16,"ָ����֤�ɹ�",1);
							/*������Ƽ�¼*/
							Uid = pData[10]; 
							Mcu_SendData_Route(0X01,ZeroArry,2,Uid);							
							/*n�����޲����Զ����� 500ms��ѯһ��*/
							os_timer_arm(&PowerChangeTimer,500,1,OneMinutePowerJudge);		
							/*ָ����֤n���*/
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
  * @brief  ѭ������ָ��ƥ��
  * @retval void
  */
void FingerSend(void)
{
	if((Finger_IRQn == 0) && (FingerMode == 0XDB)) // ָ��δ��֤ && ָ����֤ģʽ
	{		
		FSendCount = 1;
		UART5_Send_String(GetImage,26);
		/*��ָ�Ƶ�*/
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
  * @brief  ָ����֤��
  * @retval void
  */
void PowerAfterFunc()
{
	os_timer_disarm(&PowerAfterTimer);
	/*��ָ�Ƶ�*/
	UART5_Send_String(OpenLED,26); 
	/*�ı�ָ��״̬*/
	Finger_IRQn = 0;
	/*��ʾ��ˢ��*/					
	OLED_Clear(0);
	if(ExportFlie == 1)
	{
		GUI_ShowCHinese(0,8,16,"��������ļ����",1);	
		ExportFlie = 0;
	}
		
	else 
		GUI_ShowCHinese(24,8,16,"�û��ѵ�¼",1);
	
	
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
		/*ָ��δ��֤*/
		FingerState = 0;
		/*�رռ̵���*/
		//RelayOpen(0); 
		/*������Ƽ�¼*/
		Mcu_SendData_Route(0x02,ZeroArry,2,Uid); 
		/*��ʾ��ˢ��*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"����",1);
		os_timer_disarm(&PowerChangeTimer);
	}			
}
#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
