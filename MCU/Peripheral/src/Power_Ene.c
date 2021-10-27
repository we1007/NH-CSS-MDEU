#include "Power_Ene.h"
#include "Uart2.h"
#include "os_time_pro.h"
#include "flash.h"
#include "FirstPower.h"

float dat[1000] = {0.0};
int time = 0;
extern uint8_t PowerMatchFlag; //匹配开关标志 1 允许匹配，0禁止匹配
extern uint8_t PowerAfterState; //系统上电标志
extern uint8_t PowerStableFlag; //功率稳定标志
extern uint8_t PowerInitFlag; 

float idata;
uint16_t REG_RMS_IAC = 0,REG_RMS_UC = 0,REG_RMS_POWER_PAC = 0,REG_ENERGY_AC = 0;  //电流系数，电压系数，功率系数,电量系数

unsigned char	u8_TxBuf[10]; 
unsigned char	u8_RxBuf[10];
unsigned char	u8_TX_Length;
unsigned char	u8_RX_Length;
unsigned char	u8_RX_Index;

TTimerItem HLW8110_Time,Write_Rmsl_Time,Write_Amount_Time,AutoDelay_Time,Ene_Count_Time;
TTimerItem Read_Rmsl_U_Timer; //读电压
TTimerItem Read_Rmsl_A_Timer; //读电流
TTimerItem Read_Rmsl_P_Timer; //读功率
TTimerItem Read_VAP_Init_Timer; //数据处理

TTimerItem Read_Rmsl_UC_Timer;  //读电压系数
TTimerItem Read_Rmsl_IAC_Timer; //读电流系数
TTimerItem Read_Rmsl_POWER_PAC_Timer; //读功率系数

TTimerItem AnswerPower;
RMSL_DATA Rmsl_Data;

POWER_ENE Power_Ene;

uint8_t Usart2_Rx_Data[10];
uint8_t Rmsl_Addr_Flag = 0;  //电压电流功率电量查询标志
uint8_t Rmsl_Addr[4] = {0x26,0x24,0x2c,0x28};  //电压有效值,通道A电流有效值,功率有效值,电量有效值




unsigned char R_AutoSetSec[2] ;   //自动延时关闭秒
unsigned char R_AutoSetMin[2];   //自动延时关闭分
unsigned int R_AutoSetTime[2] ;   //自动延时关闭总秒数
unsigned int R_AutoDlyCnt[2];    //自动延时时间计算递减

uint8_t old_data[3]; //用于电量写入falsh时进行判断
uint8_t new_data[3]; //用于电量写入falsh时进行判断
uint8_t wt_flash_flag=0;//用于电量写入falsh时进行判断

//数组比较函数
uint8_t array_cmp1(uint8_t *a,uint8_t* b)  //数组对比
{
  uint8_t g;
  
  for(g = 0;g<3;g++)
  {
    if(a[g]!=b[g])
      return 1;  // 数据不相等，返回1
  }
  return 0;   //数据相等，返回0
}

/****************************************************************************
 *@函数名  ：HLW8110_checkSum_Read
 *@功能    ：HLE8110读功能校验和计算
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
unsigned char HLW8110_checkSum_Read(unsigned char addr,unsigned char u8_Reg_length)
{
	unsigned char i;
	unsigned char Temp_u8_checksum;
	unsigned int a;

	a = 0x0000;
	Temp_u8_checksum = 0;
	for (i = 0; i< (u8_Reg_length); i++)
	{
		a += Usart2_Rx_Data[i];
	}
		
	a = a + 0xa5 + addr;
	a = ~a;
		
	Temp_u8_checksum = a & 0xff;

	return Temp_u8_checksum;
	
}

/****************************************************************************
 *@函数名  ：HLW8110_Usart_Init
 *@功能    ：HLE8110串口初始化
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void HLW8110_Usart_Init(void)
{
	STM32_Uart2_Init(9600);
}
/****************************************************************************
 *@函数名  ：Uart_HLW8110_WriteREG_DIS
 *@功能    ：HLE8110写保护
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Uart_HLW8110_WriteREG_DIS(void)
{
	uint8_t data[5];
		
	data[0] = 0xA5;
	data[1] = 0xEA;
	data[2] = 0xDC;
	data[3] = 0x94;  //checksum  ~(0xA5 + 0xEA + 0xDC)     
	USART2_Send_String(data,4);
	
	os_timer_disarm(&Write_Rmsl_Time);	
}
/****************************************************************************
 *@函数名  ：Write_Rmsl_E_No_Zero
 *@功能    ：HLE8110通道A读电量不清零
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Write_Rmsl_E_No_Zero(void)
{
	uint8_t data[5];
	
	data[0] = 0xA5;
	data[1] = 0x93;
	data[2] = 0x04;
	data[3] = 0x65;
	data[4] = 0x5E;
	USART2_Send_String(data,5);
	os_timer_arm(&Write_Rmsl_Time,200,0,Uart_HLW8110_WriteREG_DIS);
}
/****************************************************************************
 *@函数名  ：Write_Rmsl_E_Add
 *@功能    ：HLE8110写脉冲寄存器
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Write_Rmsl_E_Add(void)
{
	uint8_t data[5];
	
	data[0] = 0xA5;
	data[1] = 0x82;
	data[2] = 0x00;
	data[3] = 0x01;
	data[4] = 0xD7;
	USART2_Send_String(data,5);
	os_timer_arm(&Write_Rmsl_Time,200,0,Write_Rmsl_E_No_Zero);
}
/****************************************************************************
 *@函数名  ：Uart_HLW8110_WriteREG_EN
 *@功能    ：HLE8110取消写保护
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Uart_HLW8110_WriteREG_EN(void)
{
	uint8_t data[5];
	data[0] = 0xA5;
	data[1] = 0xEA;
	data[2] = 0xE5;
	data[3] = 0x8b;  //checksum

	USART2_Send_String(data,4);
	os_timer_arm(&Write_Rmsl_Time,200,0,Write_Rmsl_E_Add);
}
/****************************************************************************
 *@函数名  ：Write_Rmsl_Init
 *@功能    ：HLE8110写功能定时器
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Write_Rmsl_Init(void)
{
	os_timer_arm(&Write_Rmsl_Time,200,0,Uart_HLW8110_WriteREG_EN);
}


/****************************************************************************
 *@函数名  ：Read_Rmsl_P
 *@功能    ：HLE8110读功率
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Read_Rmsl_P(void)
{
	uint8_t data[8];
	Rmsl_Addr_Flag = 0;
	data[0] = 0xA5;
	data[1] = REG_POWER_PA_ADDR;
	USART2_Send_String(data,2);
}

void Read_Rmsl_Cmd_Time(void)
{
	os_timer_arm(&Read_Rmsl_POWER_PAC_Timer,3000,1,Read_Rmsl_POWER_PAC);
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t clear;
	static uint8_t ucLampIndex = 0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Usart2_Rx_Data[ucLampIndex++] = USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		clear = USART2->SR;
		clear = USART2->DR;

		Read_VAP_Init();
		ucLampIndex = 0;		
	}
}
/****************************************************************************
 *@函数名  ：Read_VAP_Handle
 *@功能    ：HLE8110串口接收处理
 *@编写日期：20200609
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Read_VAP_Handle(void)
{
	static uint8_t Power_Up_Flag = 0;
	uint8_t Check_Num = 0;
	float data;
	uint32_t PUA = 0,b;
	
	switch(Rmsl_Addr_Flag)
	{										
		case 0: //功率
		{
			PUA = (unsigned long)(Usart2_Rx_Data[0]<<24) + (unsigned long)(Usart2_Rx_Data[1]<<16) + (unsigned long)(Usart2_Rx_Data[2]<<8) + (unsigned long)(Usart2_Rx_Data[3]);
		
			if (PUA > 0x80000000)
			{
				 b = ~PUA;
				 data = (float)b;
			}
			else
			 data =  (float)PUA;
			data = data*REG_RMS_POWER_PAC;
			data = data/0x80000000;
			data = data/1.067;  										// 1 = 电流系数
			data = data/0.333;  										// 1 = 电压系数			 
			if(data > 6553.4)
			{
				os_timer_arm(&Read_Rmsl_U_Timer,160,0,Read_Rmsl_P);
				return ;
			}			
		  Power_Ene.power = data;
			/*打印机功率初始化匹配*/
			if(PowerMatchFlag == 1)
				PowerCalculate(data);
			
			/*打印机工作模式匹配*/
			if(PowerInitFlag ==1)
			{
				if(PowerStableFlag == 0xaa)
				{
					PrinterWorkingMode(data);
				}		
			}			
			break;
		}
		
		case 1:
			Check_Num = HLW8110_checkSum_Read(REG_RMSLU_ADDR,2);
			REG_RMS_POWER_PAC = (Usart2_Rx_Data[0]<<8)+Usart2_Rx_Data[1];
			if(REG_RMS_POWER_PAC > 0)
				os_timer_disarm(&Read_Rmsl_POWER_PAC_Timer);	
			
			os_timer_arm(&Read_Rmsl_U_Timer,160,1,Read_Rmsl_P);
		break;			
		default:
		{
			os_timer_arm(&Read_Rmsl_U_Timer,160,1,Read_Rmsl_P);
			break;
		}			
	}
	
	os_timer_disarm(&Read_VAP_Init_Timer);
}

void Read_VAP_Init(void)
{
	os_timer_arm(&Read_VAP_Init_Timer,1,0,Read_VAP_Handle);
}



/****************************************************************************
 *@函数名  ：Read_Rmsl_POWER_PAC
 *@功能    ：HLE8110读功率系数
 *@编写日期：20200708
 *@编写者  ：张
 *@修改日期：无
 *@修改者  ：无
 *@修改功能：无
****************************************************************************/
void Read_Rmsl_POWER_PAC(void)
{
	uint8_t data[8];	
	Rmsl_Addr_Flag = 1; 
	data[0] = 0xA5;
	data[1] = REG_POWER_PAC_ADDR;
	USART2_Send_String(data,2);
}
