/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Mcu_SendData_Route.c
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
#include "Mcu_SendData_Route.h"
#include "Uart1.h"
#include "Uart5.h"
#include "ds3231.h"
#include "stdio.h"
#include "string.h"
#include "common_cmd.h"
#include "finger.h"
#include "oled.h"
#include "gui.h"
#include "delay.h"
#include "os_time_pro.h"
#include "FirstPower.h"

#define RX_ZIGBEE_BUFFER_MAX_LEN 1024

extern TTimerItem OneMinuteAutoPowerTimre;
TTimerItem LogFileVerifyTimre;
static uint8_t Route_rx_buffer[RX_ZIGBEE_BUFFER_MAX_LEN];  //mcu接受zigbee数据缓存
extern Calendar calendar;
extern uint8_t FingerMode;  //指纹模式
extern uint8_t Uid;
extern uint8_t RelayState;  //继电器状态 0关 1开
extern uint8_t FingerState;  //指纹状态：0：未验证 1：验证成功

uint8_t FRegisterCount = 0;  //注册指纹计数
uint8_t FRegisterArr[80];

uint8_t MTK_TCP_Print;
uint8_t ZeroArry[2];

uint8_t ExportFlie;
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_HEAD 				 0x55  //通信数据包头
#define DATA_TAIL 				 0xAA  //通信数据包尾
#define RX_BUFFER_MAX_LEN 50


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t FingerCOmMode = 0;
uint8_t DownloadImage[] = {0x55,0xAA,0X00,0X00,0X43,0X00,
													0X02,0X00,0XF2,0X03,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X39,0X02}; // 下载指纹模块存在												

uint8_t FingerDelete[] = {0x55,0xAA,0X00,0X00,0X44,0X00,
													0X04,0X00,0XF2,0X03,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X39,0X02};										
uint8_t SendFingerArr[1024] = {0};

uint8_t DeleteFinger[] = {0x55,0xAA,0X00,0X00,0X44,0X00,
													0X04,0X00,0X01,0X01,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X00,0X00,0X00,0X00,0X00,0X00,
													0X98,0X01};

uint16_t FingerID; //指纹ID 


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void delay()
{
	int i = 0;
	int j = 0;
	for(i = 0;i< 999;i++)
	 for(j = 0;j<999;j++);
}

/**
  * @brief  校验
  * @retval uint8_t
  */
uint8_t CheckCode(const uint8_t *dat, uint8_t len)
{
		uint8_t i;
		uint8_t Num = 0;
		for(i=0; i<len; i++)
		{
				Num += dat[i];
		}
		return Num;
}

/**
  * @brief  CheckSum16校验
  * @retval uint16_t
  */
uint16_t CheckSum16(const uint8_t *dat, uint16_t len)
{
		uint16_t i;
		uint16_t Num = 0;
		for(i=0; i<len; i++)
		{
				Num +=  dat[i];		
		}
		return Num;
}


/**
  * @brief  指纹回传串口数据解析
  * @retval uint8_t
  */
uint8_t *Route_uart_data_analyze(uint8_t rx_data)
{
	static uint16_t rx_index = 0;
	static uint16_t pack_size = 0;
	
	Route_rx_buffer[rx_index] = rx_data;
	
	if(rx_index == 0) /*包头*/
	{
		if(Route_rx_buffer[rx_index] != 0x55)
			rx_index = 0;
		else
			rx_index ++;
	}

	else if(rx_index == 2)/* data_size field */
	{			
		pack_size = (Route_rx_buffer[1] << 8) | Route_rx_buffer[2];
		rx_index ++;
		if(pack_size > RX_ZIGBEE_BUFFER_MAX_LEN)	//数据包长度不能大于102
		{
			pack_size = 0;
			rx_index = 0;
		}
	}
	
	else if(rx_index == (pack_size-1)) //计数 = 包长 
	{		
		if(Route_rx_buffer[rx_index] == DATA_TAIL)  //包尾
		{
			rx_index = 0;
			return Route_rx_buffer;
		}
		rx_index = 0;    
	} 

	else
	{
		rx_index ++;	
		if(rx_index >= RX_ZIGBEE_BUFFER_MAX_LEN)
      rx_index = 0;			
	}	
	
	return NULL;
}


/** MCU接收路由数据中断
  * @brief  
  * @retval int
  */
void USART1_IRQHandler(void)
{
	u8 Res;
	uint8_t *pRxBuffer;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);//读取接收到的数据	
		pRxBuffer = Route_uart_data_analyze( Res );
		if( pRxBuffer )
		{
			RouteDataAnalyze(pRxBuffer);
		}		
	}
	
}

/** MTK-> MCU 数据格式
  * @brief  
  * @retval int
  */
void RouteDataAnalyze(uint8_t *pData)
{
	uint16_t SendCount = 0;
	uint8_t DataCount = 0;
	uint16_t SendCheckSum = 0;
	uint8_t FidArry[2];
	
	switch(pData[3])
	{		
		case 0x05:
		{			
			MTK_TCP_Print = 1;
			break;
		}
		
		/*MTK发送消息到MCU请求导入指纹*/
		case 0xDA: 
		{
			/*发送删除所有指纹指令*/
			UART5_Send_String(FingerDelete,26);		
			/*LCD刷新 */
			OLED_Clear(0);
			GUI_ShowCHinese(16,8,16,"正在导入指纹",1);				
			Mcu_SendData_Route(0xDB,ZeroArry,2,0);
			break;			
		}  
		
		/*mtk发送指纹模块数据包,mcu接收*/
		case 0xDB:  
		{
			FingerID = (pData[4] << 8) | pData[5]; //TempID / UserID
			FidArry[0] = pData[4];
			FidArry[1] = pData[5];

			if(pData[6] == 0x01)  //指纹添加指令
			{
				UART5_Send_String(DownloadImage,26); //下发给指纹		
				delay();				
				FRegisterArr[FRegisterCount] = pData[4];
				FRegisterCount++;	
				SendFingerArr[0] = 0x5A;SendFingerArr[1] = 0xA5;
				SendFingerArr[2] = 0x00;SendFingerArr[3] = 0x00;
				SendFingerArr[4] = 0x43;SendFingerArr[5] = 0x00;
				SendFingerArr[6] = 0xF2;SendFingerArr[7] = 0x03;
				SendFingerArr[8] = 0x00;SendFingerArr[9] = 0x00;
				
				for(SendCount = 0;SendCount < 1008;SendCount++)
				{				
					SendFingerArr[10+SendCount] = pData[7+SendCount];
				}
				
				/*取出MTK下发的指纹数据包并转发给指纹模块*/
				SendCheckSum = CheckSum16(SendFingerArr,1018); //从PID开始检验和
				SendFingerArr[10+SendCount] =  SendCheckSum&0xff;	
				SendFingerArr[11+SendCount] = (SendCheckSum >> 8) & 0xff;						
				UART5_Send_String(SendFingerArr,1020);
				
				delay();
				/*发送指定 Temp ID 模板存储位置 */
				SendFingerArr[0] = 0x55;SendFingerArr[1] = 0xAA;
				SendFingerArr[2] = 0x00;SendFingerArr[3] = 0x00;
				SendFingerArr[4] = 0x40;SendFingerArr[5] = 0x00;
				SendFingerArr[6] = 0x04;SendFingerArr[7] = 0x00;
				SendFingerArr[8] = FidArry[1];SendFingerArr[9] = FidArry[0];
				SendFingerArr[10] = 0x00;SendFingerArr[11] = 0x00;
				SendFingerArr[12] = 0x00;SendFingerArr[13] = 0x00;
				SendFingerArr[14] = 0x00;SendFingerArr[15] = 0x00;
				SendFingerArr[16] = 0x00;SendFingerArr[17] = 0x00;
				SendFingerArr[18] = 0x00;SendFingerArr[19] = 0x00;
				SendFingerArr[20] = 0x00;SendFingerArr[21] = 0x00;
				SendFingerArr[22] = 0x00;SendFingerArr[23] = 0x00;
				
				SendCheckSum = CheckSum16(SendFingerArr,24); //从PID开始检验和
				SendFingerArr[24] =  SendCheckSum&0xff;	
				SendFingerArr[25] = (SendCheckSum >> 8) & 0xff;						
				UART5_Send_String(SendFingerArr,26);	
				Mcu_SendData_Route(0x00,ZeroArry,2,Uid); 
			}
			break;
		}
		/*接收指纹模块数据包完成*/
		case 0xDC:
		{
			OLED_Clear(0);
			GUI_ShowCHinese(16,8,16,"指纹导入完成",1);
			FingerMode = 0xDB; //进入指纹匹配模式
			os_timer_arm(&OneMinuteAutoPowerTimre,1500,0,AutoPrintf);	
			break;
		}		
		case 0xDD:
		{			
			OLED_Clear(0);
			GUI_ShowCHinese(16,0,16,"导出审计文件",1);
			GUI_ShowCHinese(16,16,16,"请验证指纹",1);
			os_timer_arm(&LogFileVerifyTimre,500,1,LogFileVerify);		
			break;
		}
		
		case 0xDF:
		{
			OLED_Clear(0);
			GUI_ShowCHinese(0,8,16,"导出审计文件完成",1);	
			ExportFlie = 1;
			FingerState = 0;
			os_timer_arm(&OneMinuteAutoPowerTimre,1500,0,AutoPrintf);			
			break;
		}
		
		default:
			break;		
	}
}


/** MUC->MTK 数据格式
  * @brief  
  * @retval int
  */
void Mcu_SendData_Route(uint8_t cmd,const uint8_t *data,uint8_t data_size,uint8_t FID)
{
	uint8_t i;
	uint8_t pack_length = 0;
	uint8_t check;
	uint8_t buf[RX_BUFFER_MAX_LEN];
	/*包头*/
	buf[pack_length++] =  DATA_HEAD;
	/*长度*/
	buf[pack_length++] =  12+data_size;
	/*时间*/
	buf[pack_length++] =  calendar.year;
	buf[pack_length++] =  calendar.month;
	buf[pack_length++] =  calendar.day;
	buf[pack_length++] =  calendar.hour;
	buf[pack_length++] =  calendar.minute;
	buf[pack_length++] =  calendar.second;
	/*Fid  TempID*/
	buf[pack_length++] = FID;
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
	check = CheckCode(buf,pack_length);
	buf[pack_length++] = check;
	/*包尾*/
	buf[pack_length++] = DATA_TAIL;
	
	USART1_Send_String(buf,pack_length);
	pack_length = 0;
}



/**
  * @brief  轮询检测审计员是否按指纹
  * @retval void
  */
void LogFileVerify(void)
{
	if(FingerState == 1)
	{
		if(Uid == 3)
		{
			Mcu_SendData_Route(0xDE,ZeroArry,2,Uid);			
			os_timer_disarm(&LogFileVerifyTimre);		
		}
	}

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
