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

extern uint8_t RelayState;  //�̵���״̬ 0�� 1��
extern uint8_t BeepState;  //�̵���״̬ 0�� 1��
extern uint8_t Uid;
extern uint8_t PrinterMode;  
extern uint8_t Finger_IRQn;
extern uint8_t PowerAfterState; //ϵͳ�ϵ��־
extern uint8_t OpenLED[];
extern uint8_t FingerState;  //ָ��״̬��0��δ��֤ 1����֤�ɹ�
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
static uint8_t RFIDrx_buffer[36];  //mcu����zigbee���ݻ���

uint16_t countTwo = 0; //RFID�������� 
uint16_t countendTwo = 0; //RFID�������� 
uint16_t countOne = 0; //RFID�������� 
uint8_t RFIDState = 0; //RFID״̬ 0��δ������1������
uint8_t WarningCount = 0; //������״̬��RFID���������ÿ��+1��


uint8_t OldFingerState;
uint8_t OldRFIDState;
uint8_t OldNowState;

u8 RFID_Stop[10] ={0x5A,0X00,0X01,0X02,0XFF,0X00,0X00,0X88,0X5A};
u8 RFID_Read[15] ={0x5A,0x00,0x01,0x02,0x10,0x00,0x05,0X00,0X00,0X00,0X02,0X01,0XA1,0XD4}; //����RFIDΪ�Զ��ظ�ģʽ

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
  * @brief  RFIDģʽ���ú���
  * @retval void
  */
void Send_Data(void)
{
	/*RFID ����ģʽΪ�Զ����Ͷ���N����ǩ����N��֪ͨ֡*/
	USART4_Send_String(RFID_Stop,9);
	USART4_Send_String(RFID_Read,14);
	os_timer_disarm(&RFIDLoopTimer);
}

/**
  * @brief  RFID�ش��������ݽ���
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
  * @brief  RFIDģʽ���ú���
  * @retval void
  */
void UART4_IRQHandler(void) 
{
	u8 Res;
	uint8_t *pRxBuffer;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(UART4);//��ȡ���յ�������	
		pRxBuffer = RFID_uart_data_analyze( Res );
		if( pRxBuffer )
		{
			RFIDFunction(pRxBuffer);
		}		
	}
}

/**
  * @brief  RFID ѭ������Ƿ��Ǵ��ڱ���״̬
  * @retval void
  */
void RFIDCountClean(void)
{
	if(RFIDState == 1) //RFID��������
	{
		if(OldNowState == 0)//��һ��״̬Ϊ����״̬
		{
			if(NowState == 1) //��һ״̬Ϊ����״̬
			{
				/*�رռ̵���*/
				RelayOpen(0);
				Mcu_SendData_Route(0x0A,ZeroArry,2,Uid);
				OLED_Clear(0);
				GUI_ShowCHinese(32,0,16,"Υ�����",1);
				GUI_ShowCHinese(32,16,16,"���ڹػ�",1);
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
			/*�ط�����*/
			BeepOpen(0);
			/*LCD��ʾ*/
			OLED_Clear(0);
			GUI_ShowCHinese(48,8,16,"����",1);			
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
			/*�����̵���*/
			RelayOpen(1);
			OLED_Clear(0);
			GUI_ShowCHinese(32,8,16,"���ڿ���",1);	
			/*��ӡ�������ϵ����������Ƿ����ȶ�״̬*/
			os_timer_arm(&WorkAfterTimer,200,1,WorkAfterTimeFunc);
			os_timer_disarm(&RelayLoopTimer);		
		}
	}
}



/**
  * @brief  RFID���ƺ���
  * @retval int
  */
int RFIDFunction(uint8_t *Pack)
{
	if(Pack[27] == 0x00 && BeepState == 0) //RFID��鵽��ǩ���ҷ�����״̬Ϊ��
	{
		Pack[27] = 0xFF;
		countTwo++;
		if(countTwo > 2 ) //�������3�� ȷ��RFID�Ƿ��鶼��ǩ
		{		
			countTwo=0;
			RFIDState = 1;
			if(NowState == 0) //RFID��δ����ת��������¼��ǰ��ָ�ƣ�����ģʽ,RFID��״̬ 
			{
				OldFingerState = FingerState;
				OldRFIDState = RFIDState;
				OldNowState = NowState;
				os_timer_arm(&RFIDWarnTimer,500,1,RFIDCountClean);				
			}							
			/*��������*/
			BeepOpen(1);								

			OLED_Clear(0);
			GUI_ShowCHinese(0,8,16,"���������ļ�����",1);				
			Mcu_SendData_Route(0X08,ZeroArry,2,Uid);
		}
	}
	else 
	{		
	}
	return 0;	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
