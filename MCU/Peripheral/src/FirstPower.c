/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : FirstPower.c
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
#include "FirstPower.h"
#include "flash.h"
#include "os_time_pro.h"
#include "Power_Ene.h"
#include "oled.h"
#include "gui.h"
#include "Mcu_SendData_Route.h"
#include "RFID.h"
#include "Beep.h"
#include "delay.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/*
*  Power   P
*  Stable  S
*	 Flash	 F
*/



/***********工作实时状态**************/
uint8_t NowState; 
uint8_t printsure; 
/**************************/

extern uint8_t ZeroArry[2];
extern uint8_t ExportFile; //审计文件导出开关
extern TTimerItem FirstPowerTimer;
extern TTimerItem PowerMatchTimer;
extern TTimerItem PowerAfterTimer; //打印机上电
extern uint8_t Uid;	//开机用户的ID	
extern uint8_t FingerState;  //指纹状态：0：未验证 1：验证成功
extern uint8_t PowerInitFlag;  
extern uint8_t MTK_TCP_Print;
TTimerItem StartTimer;
TTimerItem MatchTimer;
TTimerItem OneMinuteAutoPowerTimre;
TTimerItem StableTimer; 
TTimerItem StartStableTimer;
TTimerItem VariateCleanTimer;
TTimerItem PowerInitTimer;
TTimerItem WorkAfterTimer;
uint8_t MatchFlag = 0;
double NowPowerValue; //当前功率
double StablePowerValue; //稳定功率值
double StablePowerValueMax; //稳定功率值
double StablePowerValueMin; //稳定功率值
double PrintPowerValue; //打印功率
double CopyPowerValue; //复印功率
double ScanPowerValueMax; //扫描功率
double ScanPowerValue; //扫描功率

double PrintPowerValueT; //
double CopyPowerValueT; //
double ScanPowerValueT; //

double ScanFactor;  //扫描系数
double CopyFactor;  //复印系数
double PrintFactor; //打印系数

double RC1,RC2,RC3,RC4,RC5,RC6,RC7,RC8,RC9; //反向刻度
double midC,mids;
double FC9,FC8,FC7,FC6,FC5,FC4,FC3,FC2,FC1; //正向刻度
uint8_t RGrade; //反向匹配等级
uint8_t FGrade; //正向向匹配等级
uint8_t CalTimes;//匹配次数



	
//打印机行为判断
double StableMaxpower;
double StableMinpower;
double ScanMaxpower;
double ScanMinpower;
double WorkPowerFlag;  //行为判断值

int WorkCount = 0;
int Total = 0;

uint8_t TimeCount = 0;

uint32_t FlashStablePower;
uint32_t FlashPrintPower;
uint32_t FlashCopyPower;
uint32_t FlashScanPower;
uint8_t FlashPowerArr[4];
uint8_t FlashpCount[4];
uint32_t FlashpCountV;

extern POWER_ENE Power_Ene;

double PSFminPower; 
double PSFmaxPower;
double PMFminPower; 
double PMFmaxPower;

double StableValue; //稳定功率值
double SPower[12]; //上一次的功率值，用于功率稳定判断
double PSFLastPower[12]; //上一次的功率值，用于功率稳定判断
double PMFLastPower[12]; //上一次的功率值，用于功率稳定判断

double InitPSFLastPower[12]; //上一次的功率值，用于功率稳定判断
double InitPMFLastPower[12]; //上一次的功率值，用于功率稳定判断


uint8_t PowerStableFlag = 0; //功率稳定标志
uint8_t PowerMatchFlag; //匹配开关标志 1 允许匹配，0禁止匹配


uint16_t MtkPcout;
float pCount = 0;
float SendpCount;
int MatchCount = 0;  //功率值个数
int MatchCountmin = 0;  //功率值个数
int MatchCountmax = 0;  //功率值个数
double PowerMatchValue = 0.0;  //匹配时功率值累加值

uint8_t RiseCount;
uint8_t DownCount;

uint8_t RiseFlag;

uint8_t StateSwitch; //记录状态开关
uint8_t PowerDirection; //功率趋势
double  SwitchAverageValue; //
double  SwitchPowerValue;
uint8_t PowerAfterState = 0; //系统上电标志
uint8_t CountSum;
double PowerR[10] = {0};
double PowerD[10] = {0};

float Powerfactor;

uint8_t AutoPowerStable; //打印机自动上电后功率稳定标志

/*DirectionState
* DirectionState = 1 上升趋势
* DirectionState = 2 下降趋势
*
*/
uint8_t DirectionState;
uint8_t RInit = 1;

uint8_t DirectionTimeDOpen = 0;
uint8_t DirectionTimeR;
uint8_t DirectionTimeD;
/*PrinterMode 			打印机平均值模式
* PrinterMode = 0  待机状态
*	PrinterMode = 1  扫描状态
* PrinterMode = 2  打印状态
*	PrinterMode = 3  复印状态
* PrinterMode = 4  不确定模式
*/
uint8_t PrinterMode = 0;  

/*PrinterReleMode      打印机关联匹配模式
* PrinterReleMode = 0  待机状态
*	PrinterReleMode = 1  扫描状态
* PrinterReleMode = 2  打印状态
*	PrinterReleMode = 3  复印状态
* PrinterReleMode = 4  不确定模式
*/
uint8_t PrinterReleMode = 0;  
uint8_t CountArry[4] = {0};

/*记录关机状态 由关机->待机 0  由待机->关机 1*/
uint8_t powerOnOffFlag = 0;

/* USER CODE BEGIN PV */
uint8_t PSFcount = 0;
uint8_t InitPSFcount = 0;
uint8_t PMFcount = 0;

uint8_t WorkAfterTime;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

//延时20s
void delayfunc(void)
{
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}


uint8_t cRGrade(double data)
{
	uint8_t cRGrade; //反向匹配等级

	if (RC9 < data && data < midC)
		cRGrade = 9;
	else if (RC8 < data && data < RC9)
		cRGrade = 8;
	else if (RC7 < data && data < RC8)
		cRGrade = 7;
	else if (RC6 < data && data < RC7)
		cRGrade = 6;
	else if (RC5 < data && data < RC6)
		cRGrade = 5;
	else if (RC4 < data && data < RC5)
		cRGrade = 4;
	else if (RC3 < data && data < RC4)
		cRGrade = 3;
	else if (RC2 < data && data < RC3)
		cRGrade = 2;
	else if (RC1 < data && data < RC2)
		cRGrade = 1;
	else
		cRGrade = 0;

	return cRGrade;
}

/**
  * @brief  第一次上电，工作模式匹配
  * @retval void
  */
uint8_t cFGrade(double data)
{
	uint8_t cFGrade; //正向向匹配等级

	if (midC < data && data < FC9)
		cFGrade = 9;
	else if (FC9 < data && data < FC8)
		cFGrade = 8;
	else if (FC8 < data && data < FC7)
		cFGrade = 7;
	else if (FC7 < data && data < FC6)
		cFGrade = 6;
	else if (FC6 < data && data < FC5)
		cFGrade = 5;
	else if (FC5 < data && data < FC4)
		cFGrade = 4;
	else if (FC4 < data && data < FC3)
		cFGrade = 3;
	else if (FC3 < data && data < FC2)
		cFGrade = 2;
	else if (FC2 < data && data < FC1)
		cFGrade = 1;
	else
		cFGrade = 0;

	return cFGrade;
}

/**
  * @brief  上电后变量初始化		
  * @retval void
  */
	
void PowerInit(void)
{
		os_timer_arm(&StableTimer,1000,0,StartStable);
	  os_timer_disarm(&PowerInitTimer);	
}

/**
  * @brief  第一次上电，工作模式匹配
  * @retval void
  */
void FirstPower(void)
{
	uint8_t data;	
  Flash_Read(FIRSTPOWER,&data,1);
	PowerStableFlag = data;
	/*未匹配功率*/
	if(data == 0xff)
	{ 
		CalTimes = 0;
		OLED_Clear(0);
		GUI_ShowCHinese(32,8,16,"初始化中",1);
		os_timer_arm(&FirstPowerTimer,2000,0,Start);		
	}
	else  /*已匹配功率*/
	{
		os_timer_arm(&PowerInitTimer,1000,0,PowerInit);	
		
		/*读取稳定功率*/
		Flash_Read(STABLE_POWER_ADDR,FlashPowerArr,4);	
		FlashStablePower = ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		StablePowerValue = FlashStablePower/1000.0;
		/*读取扫描功率*/
		Flash_Read(SCAN_POWER_ADDR,FlashPowerArr,4);	
		FlashScanPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		ScanPowerValue = FlashScanPower/1000.0;
		/*读取打印功率*/
		Flash_Read(PRINT_POWER_ADDR,FlashPowerArr,4);	
		FlashPrintPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		PrintPowerValue = FlashPrintPower/1000.0;
		/*读取复印功率*/
		Flash_Read(COPY_POWER_ADDR,FlashPowerArr,4);	
		FlashCopyPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		CopyPowerValue = FlashCopyPower/1000.0;	
		/*打印次数分母*/
		Flash_Read(PRINTFCOUNT,FlashpCount,4);	
		FlashpCountV =  ((FlashpCount[0] << 32)|(FlashpCount[1] << 16)|(FlashpCount[2] << 8) | FlashpCount[3]);
		pCount = FlashpCountV/1000.0;
							
		/*1.扫描系数*/
		ScanFactor = StablePowerValue/ScanPowerValue;			
		/*2.打印系数*/
		PrintFactor = StablePowerValue/PrintPowerValue;
		/*3.复印系数*/
		CopyFactor = StablePowerValue/CopyPowerValue;	
		
		if(StablePowerValue < 4.0)
			Powerfactor = 0.22;
		else if(StablePowerValue < 5.0)
			Powerfactor = 0.21;
		else if(StablePowerValue < 6.0)
			Powerfactor = 0.20;
		else if(StablePowerValue < 7.0)
			Powerfactor = 0.19;
		else if(StablePowerValue < 8.0)
			Powerfactor = 0.18;
		else if(StablePowerValue < 9.0)
			Powerfactor = 0.17;
		else if(StablePowerValue < 10.0)
			Powerfactor = 0.16;
		else if (StablePowerValue > 11.0)
			Powerfactor = 0.15;
		
		
		/*趋势对比值初始化*/
		WorkPowerFlag = StablePowerValue*11;
		PowerR[0] = StablePowerValue+StablePowerValue*Powerfactor;
		PowerD[0] = (StablePowerValue+StablePowerValue*Powerfactor);
		
		StablePowerValueMax = StablePowerValue+StablePowerValue*Powerfactor;
		StablePowerValueMin = StablePowerValue-StablePowerValue*Powerfactor;
		
		/*稳定功率范围*/
		StableMaxpower = StablePowerValue+StablePowerValue*Powerfactor;
		StableMinpower = StablePowerValue-StablePowerValue*Powerfactor;
		/*扫描功率范围*/
		ScanMaxpower = ScanPowerValue+ScanPowerValue*Powerfactor;
		ScanMinpower = ScanPowerValue-ScanPowerValue*Powerfactor;
		
		
		ScanPowerValueMax = ScanPowerValue*2;
		SwitchAverageValue = 0;
		PowerMatchValue = 0;
		MatchCount = 0;
				
		/*计算匹配概率*/
		mids = StablePowerValue/10.0;
		RC1 = StablePowerValue-(mids*9);// 0-10
		RC2 = StablePowerValue-(mids*8);// 10-20
		RC3 = StablePowerValue-(mids*7);// 20-30
		RC4 = StablePowerValue-(mids*6);// 30-40
		RC5 = StablePowerValue-(mids*5);// 40-50
		RC6 = StablePowerValue-(mids*4);// 50-60
		RC7 = StablePowerValue-(mids*3);// 60-70 
		RC8 = StablePowerValue-(mids*2);// 70-80
		RC9 = StablePowerValue-(mids*1);// 80-90		
		midC = StablePowerValue - (mids*0); //90-100
		FC9 = StablePowerValue-(mids*1);// 80-90		
		FC8 = StablePowerValue-(mids*2);// 70-80
		FC7 = StablePowerValue-(mids*3);// 60-70
		FC6 = StablePowerValue-(mids*4);// 50-60
		FC5 = StablePowerValue-(mids*5);// 40-50
		FC4 = StablePowerValue-(mids*6);// 30-40
		FC3 = StablePowerValue-(mids*7);// 20-30
		FC2 = StablePowerValue-(mids*8);// 10-20
		FC1 = StablePowerValue-(mids*9);// 0-10	
		
		//上电初始化断电后自动上电稳定状态标志
		AutoPowerStable = 1;	
		ZeroArry[0] = 0;
		ZeroArry[1] = 0;
	}
	return;
}



/**
  * @brief  启动功率稳定值匹配
  * @retval void
  */
void Start(void)
{
	os_timer_disarm(&FirstPowerTimer);
	os_timer_arm(&StartTimer,2500,1,PowerStableFunc);
}

/**
  * @brief  判断功率是否稳定 PSF  2.5s一次 共10次  25s内判读是否已经是待机功率
  * @retval void
  */
void PowerStableFunc(void)
{	
	if(Power_Ene.power > 2)
	{
			if(InitPSFcount == 0)
			{
				InitPSFLastPower[0] = Power_Ene.power;
				PSFminPower = InitPSFLastPower[0] - InitPSFLastPower[0]*ErrorMinCoefficientPSF;
				PSFmaxPower = InitPSFLastPower[0] + InitPSFLastPower[0]*ErrorMinCoefficientPSF;		
				InitPSFcount++;   
			}
			else
			{
				InitPSFLastPower[InitPSFcount] = Power_Ene.power;  //count 从1开始。		
				//在LastPower[0]的误差范围内比较 
				if(PSFminPower < InitPSFLastPower[InitPSFcount] && InitPSFLastPower[InitPSFcount] < PSFmaxPower)
				{
					InitPSFcount++;
					if(InitPSFcount >= 12)
					{			
						/*匹配稳定功率成功标志*/						
						
						os_timer_disarm(&StartTimer);						
						InitPSFcount = 0;
						
						/*稳定功率值*/
						StablePowerValue = (double) (InitPSFLastPower[0]+InitPSFLastPower[1]+
																				InitPSFLastPower[2]+InitPSFLastPower[3]+
																				InitPSFLastPower[4]+InitPSFLastPower[5]+
																				InitPSFLastPower[6]+InitPSFLastPower[7]+
																				InitPSFLastPower[8]+InitPSFLastPower[9]+
																				InitPSFLastPower[10]+InitPSFLastPower[11])/12;						
						//double类型放大1000倍转16位再拆成2个8位存入Flash中					
						FlashStablePower = StablePowerValue*1000.0;
						FlashPowerArr[0] = (FlashStablePower >> 32)&0xff; //高8位
						FlashPowerArr[1] = (FlashStablePower >> 16)&0xff; //高8位
						FlashPowerArr[2] = (FlashStablePower >> 8)&0xff; //高8位
						FlashPowerArr[3] = FlashStablePower&0xff; //低8位						
						Flash_Write(STABLE_POWER_ADDR,FlashPowerArr,4);						
						
						/*开启功率初始化匹配*/		
						WorkPowerFlag = (StablePowerValue*11.0);
						PowerR[0] = (StablePowerValue+StablePowerValue*0.1);
						PowerD[0] = (StablePowerValue+StablePowerValue*0.1);
												
						StablePowerValueMax = StablePowerValue+StablePowerValue*0.100;
						StablePowerValueMin = StablePowerValue-StablePowerValue*0.100;
												
						PowerMatchFlag = 1;
						InitPSFcount = 0;	
						PMFcount = 0;
						RInit = 1;
						
						BeepOpen(1);
						delay_ms(1500);
						BeepOpen(0);						
						OLED_Clear(0);						
						GUI_ShowCHinese(24,8,16,"请开始复印",1);						
					}
				}	
				else 
				{
					InitPSFcount = 0;
					return;
				}
			}	
	}/*if Power_Ene.power > 2 end*/	
}



/**
  * @brief  开启功率初始化匹配
  * @retval void
  */
float PowerCalculate(float power)
{
	uint8_t WriteTip;
	/*上升趋势检测*/							
	if(RInit == 1)
	{
		if(DirectionState != 1)
		{
			if(power > PowerR[RiseCount])
			{
				RiseCount++;
				PowerR[RiseCount] = power;
				DirectionTimeR = 0;
			}
			else if(power == PowerR[RiseCount])
			{}
			else 
				DirectionTimeR++;	
			if(DirectionTimeR == 3) //连续3次都不符合则重新匹配
			{
				RiseCount = 0;
				DirectionTimeR = 0;			
			}						
			if(RiseCount > 2) 
			{	
				DirectionState = 1; //匹配上升趋势
				RInit = 0;			
				NowState = 1;
				RiseCount = 0;
				printsure = 0;
			}
		}
	}
	/*下降趋势检测*/			
	if(DirectionState == 1 )  
	{					
		if(power <= PowerD[DownCount])
		{
			DownCount++;
			PowerD[DownCount] = power;
			DirectionTimeD = 0;
		}	
		else if(power == PowerD[DownCount])
		{}					
		else 
			DirectionTimeD++;

		if(DirectionTimeD == 3) //连续3次都不符合则重新匹配
		{
			DownCount = 0;
			DirectionTimeD = 0;
		}
		if(DownCount > 3)
		{
			DirectionState = 2; //下降趋势确定
			DownCount = 0;
		}																							
	}	
	
	
	switch(PMFcount)
	{
		/*1.复印功率*/
		case 0: 			
		{							
			if(DirectionState == 1)
			{
				PowerMatchValue += power;
				MatchCount++;

				if(printsure == 0)
				{
					printsure = 1;
					OLED_Clear(0);
					GUI_ShowCHinese(32,0,16,"采集数据中",1);	
					GUI_ShowCHinese(48,16,16,"请稍后",1);	
				}	
			}	
			else if(DirectionState == 2)
			{					
				/*3.计算功率值*/
				if(MatchCount > 50)
				{					
					CopyPowerValue = PowerMatchValue/MatchCount;												
					/*写入flash*/
					FlashCopyPower = CopyPowerValue*1000.0;
					FlashPowerArr[0] = (FlashCopyPower >> 32)&0xff; //高8位
					FlashPowerArr[1] = (FlashCopyPower >> 16)&0xff; //高8位
					FlashPowerArr[2] = (FlashCopyPower >> 8)&0xff; //高8位
					FlashPowerArr[3] = FlashPrintPower&0xff; //低8位
					Flash_Write(COPY_POWER_ADDR,FlashPowerArr,4);	
					
					delay_ms(1500);								
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"已完成复印",1);	
					GUI_ShowCHinese(16,16,16,"请开始扫描",1);	
		
					PowerMatchValue = 0.0;								
					MatchCount = 0;												
					PMFcount = 1;
					DirectionState = 0;
					RInit = 1;
				}
				else 
				{						
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"采集数据失败",1);	
					GUI_ShowCHinese(16,16,16,"请再次复印",1);	
					PowerMatchValue = 0.0;								
					MatchCount = 0;												
					PMFcount = 0;
					RInit = 1;			
				}		
			}
			else
			{}	
			break;
		}
		
		/*2.扫描功率*/	
		case 1:
		{									
			if(DirectionState == 1)
			{
				PowerMatchValue += power;
				MatchCount++;
				if(printsure == 0)
				{
					printsure = 1;
					OLED_Clear(0);
					GUI_ShowCHinese(32,0,16,"采集数据中",1);	
					GUI_ShowCHinese(48,16,16,"请稍后",1);	
				}	
			}	
			else if(DirectionState == 2)
			{							
				if(MatchCount > 40)
				{					
					ScanPowerValue = PowerMatchValue/MatchCount; //扫描功率平均值			
					/*写入flash*/
					FlashScanPower =ScanPowerValue*1000.0;
					FlashPowerArr[0] = (FlashScanPower >> 32)&0xff; //高8位
					FlashPowerArr[1] = (FlashScanPower >> 16)&0xff; //高8位
					FlashPowerArr[2] = (FlashScanPower >> 8)&0xff; //高8位
					FlashPowerArr[3] = FlashScanPower&0xff; //低8位				
					Flash_Write(SCAN_POWER_ADDR,FlashPowerArr,4);			
					
					delay_ms(1500);									
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"已完成扫描",1);	
					GUI_ShowCHinese(16,16,16,"请开始打印",1);		
									
					PowerMatchValue = 0.0;					
					MatchCount = 0;					
					PMFcount = 2;
					DirectionState = 0;					
					RInit = 1;			
				}
				else 
				{
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"采集数据失败",1);	
					GUI_ShowCHinese(16,16,16,"请再次扫描",1);	
					PowerMatchValue = 0.0;								
					MatchCount = 0;												
					PMFcount = 1;
					RInit = 1;	
				}					
			}
			else
			{}			
			break;
		}			
		/*3.打印功率*/		
		case 2: 
		{							
			if(DirectionState == 1)
			{
				PowerMatchValue += power;
				MatchCount++;
				
				if(printsure == 0)
				{
					printsure = 1;
					OLED_Clear(0);
					GUI_ShowCHinese(32,0,16,"采集数据中",1);	
					GUI_ShowCHinese(48,16,16,"请稍后",1);	
				}
				
			}	
			else if(DirectionState == 2)
			{	
				if(MatchCount > 40)
				{					
					PrintPowerValue = PowerMatchValue/MatchCount;		//打印功率平均值																			
					/*写入flash*/						
					FlashPrintPower = PrintPowerValue*1000.0;
					FlashPowerArr[0] = (FlashPrintPower >> 32)&0xff; //高8位
					FlashPowerArr[1] = (FlashPrintPower >> 16)&0xff; //高8位
					FlashPowerArr[2] = (FlashPrintPower >> 8)&0xff; //高8位
					FlashPowerArr[3] = FlashPrintPower&0xff; //低8位
					Flash_Write(PRINT_POWER_ADDR,FlashPowerArr,4);	
					
					pCount = MatchCount/2.0;
					/*写入flash*/						
					FlashpCountV = pCount*1000.0;
					FlashpCount[0] = (FlashpCountV >> 32)&0xff; //高8位
					FlashpCount[1] = (FlashpCountV >> 16)&0xff; //高8位
					FlashpCount[2] = (FlashpCountV >> 8)&0xff; //高8位
					FlashpCount[3] = FlashpCountV&0xff; //低8位					
					Flash_Write(PRINTFCOUNT,FlashpCount,4);
																												
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"已完成打印",1);
					GUI_ShowCHinese(16,16,16,"正在重启中",1);
					
					delay_ms(1500);
					/*完成功率匹配工作，标志写入flash中*/
					WriteTip = 0xaa;
					Flash_Write_super(FIRSTPOWER,&WriteTip,1);											
				
					/*重启系统*/
					 __set_FAULTMASK(1);//关闭总中断
					NVIC_SystemReset();//请求单片机重启																
				}
				
				else 
				{				
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"采集数据失败",1);	
					GUI_ShowCHinese(16,16,16,"请再次打印",1);	
					PowerMatchValue = 0.0;								
					MatchCount = 0;												
					PMFcount = 2;
					RInit = 1;						
				}	
			}	
			else
			{}
			break;
		}					
		default:
			break;
	}
}


#if 1
/**
  * @brief  打印机工作模式匹配
  * @retval void
  */
void PrinterWorkingMode(float power)
{
	uint8_t Total;	
	double m1, m2, m3;
	double s1,s2,s3;
	uint8_t PrintArry[2];


	/*上升趋势检测*/	
	if(RInit == 1)
	{
		if(DirectionState != 1)
		{
			if(power > PowerR[RiseCount])
			{
				RiseCount++;
				PowerR[RiseCount] = power;
				DirectionTimeR = 0;
			}
			else if(power == PowerR[RiseCount])
			{}
			else 
				DirectionTimeR++;	
			if(DirectionTimeR == 3) //连续3次都不符合则重新匹配
			{
				RiseCount = 0;
				DirectionTimeR = 0;			
			}						
			if(RiseCount > 2) 
			{	
				DirectionState = 1; //匹配上升趋势
				RInit = 0;							
				RiseCount = 0;
				NowState = 1; //工作标志
				OLED_Clear(0);
				GUI_ShowCHinese(32,8,16,"工作中",1);
			}
		}
	}
	
	/*下降趋势检测*/			
	if(DirectionState == 1)  
	{					
		if(power <= PowerD[DownCount])
		{
			DownCount++;
			PowerD[DownCount] = power;
			DirectionTimeD = 0;
		}	
		else if(power == PowerD[DownCount])
		{}					
		else 
			DirectionTimeD++;		
		if(DirectionTimeD == 3) //连续2次都不符合则重新匹配
		{
			DownCount = 0;
			DirectionTimeD = 0;
		}		
		if(DownCount > 2)
		{
			DirectionState = 2; //下降趋势确定
			DirectionTimeDOpen = 0;
			DownCount = 0;
		}																							
	}	
		
	/*1.工作状态*/
	if(power > StableMinpower)
	{
		/*判断是否登录*/
		if(FingerState == 0 && power > StableMaxpower)
		{
			/*继电器常关*/
			RelayOpen(0);
			/*关闭工作模式匹配*/
			PowerInitFlag = 0;
			/*显示屏刷新*/
			OLED_Clear(0);
			GUI_ShowCHinese(32,0,16,"非法操作",1);
			GUI_ShowCHinese(32,16,16,"正在关机",1);
			/*发送审计记录*/
			Mcu_SendData_Route(0x02,ZeroArry,2,0); 			
			os_timer_arm(&OneMinuteAutoPowerTimre,30000,0,OneMinuteAutoPower);
			/*开蜂鸣器*/
			BeepOpen(1);
			delay_ms(1200);
			BeepOpen(0);
			delay_ms(1000);
			BeepOpen(1);
			delay_ms(1200);
			BeepOpen(0);
			delay_ms(1000);
			BeepOpen(1);
			delay_ms(1200);
			BeepOpen(0);						
		}		
		else 
		{																					
			/*************************1.打印机行为判断************************/
			/*************************(1).关联模式匹配************************/		
			if(DirectionState == 1)
			{				
				MatchCount++;								
				NowPowerValue += power;	
				if(power > ScanPowerValueMax)
					MatchCountmax++;
			}							
			else if(DirectionState == 2) 
			{
				//1.模式匹配
				NowPowerValue = NowPowerValue/MatchCount;
				if(ScanPowerValueMax <NowPowerValue)
				{
					PrinterReleMode = 2;
				}
				else
				{
					PrinterReleMode = 1; 
				}	
				
				//2.张数计算
				if(MatchCountmax >2)				
					SendpCount = (MatchCountmax/pCount)*1.0;				
				MtkPcout = (uint8_t)(SendpCount+0.5) > SendpCount?(SendpCount+0.5):SendpCount;
				MtkPcout = MtkPcout-1;
				PrintArry[0] = (MtkPcout >> 8)&0xff;
				PrintArry[1] = MtkPcout&0xff; //低8位		
				
				//3.消息发送
				if(PrinterReleMode ==2)
				{									
					if(MTK_TCP_Print == 1) /*MTK收到打印指令，确定为打印行为*/
					{
						Mcu_SendData_Route(0x05,PrintArry,2,Uid);						
						MTK_TCP_Print = 0;						
					}
					else  
					{
						Mcu_SendData_Route(0x06,PrintArry,2,Uid);
					}
				}					
				else 
					Mcu_SendData_Route(0x07,ZeroArry,2,Uid);
				
				DirectionState = 0;
				/*工作后连续检测是否处在稳定状态*/
				os_timer_arm(&WorkAfterTimer,200,1,WorkAfterTimeFunc);
				os_timer_arm(&VariateCleanTimer,3000,1,VariateCleanfunc);
			}
		}
	}	/****************************************/
	
	/******************************2.待机********************************/
	else if((1 < power) && (power < StableMaxpower))
	{			
		/*待机状态，未登录用户*/
		if(FingerState == 0)
		{
			WorkCount = 0;
			TimeCount = 0;
		}	
	}		
	
	/*3.关机*/
	else 
	{
		powerOnOffFlag = 1;
		
		/*显示屏刷新*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"关机",1);
	}
			
}

#endif


/**
  * @brief  非法操作后自动开机
  * @retval void
  */
void OneMinuteAutoPower(void)
{
	if(FingerState == 0)
	{
		/*指纹验证标志开*/
		FingerState = 1;
		/*继电器常开*/
		RelayOpen(1);
		/*显示屏刷新*/
		OLED_Clear(0);
		GUI_ShowCHinese(32,8,16,"正在开机",1);
		/*稳定功率判断*/
		PSFcount = 0;
		os_timer_arm(&StableTimer,1000,0,StartStable);			
	}
	else
	{
		/*继电器常开*/
		RelayOpen(1);
		/*显示屏刷新*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"待机",1);
		/*发送审计记录*/
		Mcu_SendData_Route(0x02,ZeroArry,2,0); 		
	}		
	os_timer_disarm(&PowerMatchTimer);
}

/**
  * @brief  非法操作后自动开机
  * @retval void
  */
void AutoPrintf(void)
{
	/*继电器常开*/
	RelayOpen(1);
	/*显示屏刷新*/
	OLED_Clear(0);
	GUI_ShowCHinese(48,8,16,"待机",1);
  /*发送审计记录*/
	Mcu_SendData_Route(0x02,ZeroArry,2,0); 		
	
	os_timer_disarm(&PowerMatchTimer);
}

/**
  * @brief  启动功率稳定值匹配
  * @retval void
  */
void StartStable(void)
{
	os_timer_disarm(&StableTimer);
	os_timer_arm(&StartStableTimer,1500,1,Stable);
}


/**
  * @brief  稳定功率判断函数
  * @retval void
  */
void Stable(void)
{	
	if(Power_Ene.power > 1.0)
	{
			if(PSFcount == 0)
			{
				SPower[0] = Power_Ene.power;
				PSFminPower = SPower[0] - SPower[0]*ErrorMinCoefficientPSF;
				PSFmaxPower = SPower[0] + SPower[0]*ErrorMinCoefficientPSF;		
				PSFcount++;   
			}
			else
			{
				SPower[PSFcount] = Power_Ene.power;  //count 从1开始。		
				//在LastPower[0]的误差范围内比较 
				if(PSFminPower < SPower[PSFcount] && SPower[PSFcount] < PSFmaxPower)
				{
					PSFcount++;
					if(PSFcount > 10)
					{									
						/*稳定功率值*/
						StableValue = (double) (SPower[0]+SPower[1]+
																				SPower[2]+SPower[3]+
																				SPower[4]+SPower[5]+
																				SPower[6]+SPower[7]+
																				SPower[8]+SPower[9]
																				)/10;
						PSFcount = 0;							
						if(StablePowerValueMin< StableValue && StableValue < StablePowerValueMax)
						{								
							PowerInitFlag = 1;
							AutoPowerStable = 1;
							/*指纹验证标志关*/
							FingerState = 0;
							OLED_Clear(0);
							GUI_ShowCHinese(48,8,16,"待机",1);
							os_timer_disarm(&StartStableTimer);							
						}							
					}
				}	
				else 
				{
					PSFcount = 0;
					return;
				}
			}	
	}/*if Power_Ene.power > 2 end*/	
}


void WorkAfterTimeFunc(void)
{
	if(Power_Ene.power < StablePowerValueMax)
	{
		WorkAfterTime++;
		if(WorkAfterTime>10)
		{				
			NowState = 0; //工作标志
			/*指纹已验证标志*/
			FingerState = 0;	
			OLED_Clear(0);
			GUI_ShowCHinese(48,8,16,"待机",1);
			/*发送审计记录*/
			Mcu_SendData_Route(0x02,ZeroArry,2,0); 		
			os_timer_disarm(&WorkAfterTimer);
		}			
	}
	else
		WorkAfterTime = 0;	
}

void VariateCleanfunc(void)
{
	if( Power_Ene.power < PowerR[0])
	{
		SwitchAverageValue = 0;
		PowerMatchValue = 0;
		MatchCount = 0;
		MatchCountmax = 0;
		RInit = 1;
		
		memset(CountArry,0,(sizeof(uint8_t)*4));
		os_timer_disarm(&VariateCleanTimer);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
