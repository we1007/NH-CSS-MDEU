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



/***********����ʵʱ״̬**************/
uint8_t NowState; 
uint8_t printsure; 
/**************************/

extern uint8_t ZeroArry[2];
extern uint8_t ExportFile; //����ļ���������
extern TTimerItem FirstPowerTimer;
extern TTimerItem PowerMatchTimer;
extern TTimerItem PowerAfterTimer; //��ӡ���ϵ�
extern uint8_t Uid;	//�����û���ID	
extern uint8_t FingerState;  //ָ��״̬��0��δ��֤ 1����֤�ɹ�
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
double NowPowerValue; //��ǰ����
double StablePowerValue; //�ȶ�����ֵ
double StablePowerValueMax; //�ȶ�����ֵ
double StablePowerValueMin; //�ȶ�����ֵ
double PrintPowerValue; //��ӡ����
double CopyPowerValue; //��ӡ����
double ScanPowerValueMax; //ɨ�蹦��
double ScanPowerValue; //ɨ�蹦��

double PrintPowerValueT; //
double CopyPowerValueT; //
double ScanPowerValueT; //

double ScanFactor;  //ɨ��ϵ��
double CopyFactor;  //��ӡϵ��
double PrintFactor; //��ӡϵ��

double RC1,RC2,RC3,RC4,RC5,RC6,RC7,RC8,RC9; //����̶�
double midC,mids;
double FC9,FC8,FC7,FC6,FC5,FC4,FC3,FC2,FC1; //����̶�
uint8_t RGrade; //����ƥ��ȼ�
uint8_t FGrade; //������ƥ��ȼ�
uint8_t CalTimes;//ƥ�����



	
//��ӡ����Ϊ�ж�
double StableMaxpower;
double StableMinpower;
double ScanMaxpower;
double ScanMinpower;
double WorkPowerFlag;  //��Ϊ�ж�ֵ

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

double StableValue; //�ȶ�����ֵ
double SPower[12]; //��һ�εĹ���ֵ�����ڹ����ȶ��ж�
double PSFLastPower[12]; //��һ�εĹ���ֵ�����ڹ����ȶ��ж�
double PMFLastPower[12]; //��һ�εĹ���ֵ�����ڹ����ȶ��ж�

double InitPSFLastPower[12]; //��һ�εĹ���ֵ�����ڹ����ȶ��ж�
double InitPMFLastPower[12]; //��һ�εĹ���ֵ�����ڹ����ȶ��ж�


uint8_t PowerStableFlag = 0; //�����ȶ���־
uint8_t PowerMatchFlag; //ƥ�俪�ر�־ 1 ����ƥ�䣬0��ֹƥ��


uint16_t MtkPcout;
float pCount = 0;
float SendpCount;
int MatchCount = 0;  //����ֵ����
int MatchCountmin = 0;  //����ֵ����
int MatchCountmax = 0;  //����ֵ����
double PowerMatchValue = 0.0;  //ƥ��ʱ����ֵ�ۼ�ֵ

uint8_t RiseCount;
uint8_t DownCount;

uint8_t RiseFlag;

uint8_t StateSwitch; //��¼״̬����
uint8_t PowerDirection; //��������
double  SwitchAverageValue; //
double  SwitchPowerValue;
uint8_t PowerAfterState = 0; //ϵͳ�ϵ��־
uint8_t CountSum;
double PowerR[10] = {0};
double PowerD[10] = {0};

float Powerfactor;

uint8_t AutoPowerStable; //��ӡ���Զ��ϵ�����ȶ���־

/*DirectionState
* DirectionState = 1 ��������
* DirectionState = 2 �½�����
*
*/
uint8_t DirectionState;
uint8_t RInit = 1;

uint8_t DirectionTimeDOpen = 0;
uint8_t DirectionTimeR;
uint8_t DirectionTimeD;
/*PrinterMode 			��ӡ��ƽ��ֵģʽ
* PrinterMode = 0  ����״̬
*	PrinterMode = 1  ɨ��״̬
* PrinterMode = 2  ��ӡ״̬
*	PrinterMode = 3  ��ӡ״̬
* PrinterMode = 4  ��ȷ��ģʽ
*/
uint8_t PrinterMode = 0;  

/*PrinterReleMode      ��ӡ������ƥ��ģʽ
* PrinterReleMode = 0  ����״̬
*	PrinterReleMode = 1  ɨ��״̬
* PrinterReleMode = 2  ��ӡ״̬
*	PrinterReleMode = 3  ��ӡ״̬
* PrinterReleMode = 4  ��ȷ��ģʽ
*/
uint8_t PrinterReleMode = 0;  
uint8_t CountArry[4] = {0};

/*��¼�ػ�״̬ �ɹػ�->���� 0  �ɴ���->�ػ� 1*/
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

//��ʱ20s
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
	uint8_t cRGrade; //����ƥ��ȼ�

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
  * @brief  ��һ���ϵ磬����ģʽƥ��
  * @retval void
  */
uint8_t cFGrade(double data)
{
	uint8_t cFGrade; //������ƥ��ȼ�

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
  * @brief  �ϵ�������ʼ��		
  * @retval void
  */
	
void PowerInit(void)
{
		os_timer_arm(&StableTimer,1000,0,StartStable);
	  os_timer_disarm(&PowerInitTimer);	
}

/**
  * @brief  ��һ���ϵ磬����ģʽƥ��
  * @retval void
  */
void FirstPower(void)
{
	uint8_t data;	
  Flash_Read(FIRSTPOWER,&data,1);
	PowerStableFlag = data;
	/*δƥ�书��*/
	if(data == 0xff)
	{ 
		CalTimes = 0;
		OLED_Clear(0);
		GUI_ShowCHinese(32,8,16,"��ʼ����",1);
		os_timer_arm(&FirstPowerTimer,2000,0,Start);		
	}
	else  /*��ƥ�书��*/
	{
		os_timer_arm(&PowerInitTimer,1000,0,PowerInit);	
		
		/*��ȡ�ȶ�����*/
		Flash_Read(STABLE_POWER_ADDR,FlashPowerArr,4);	
		FlashStablePower = ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		StablePowerValue = FlashStablePower/1000.0;
		/*��ȡɨ�蹦��*/
		Flash_Read(SCAN_POWER_ADDR,FlashPowerArr,4);	
		FlashScanPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		ScanPowerValue = FlashScanPower/1000.0;
		/*��ȡ��ӡ����*/
		Flash_Read(PRINT_POWER_ADDR,FlashPowerArr,4);	
		FlashPrintPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		PrintPowerValue = FlashPrintPower/1000.0;
		/*��ȡ��ӡ����*/
		Flash_Read(COPY_POWER_ADDR,FlashPowerArr,4);	
		FlashCopyPower =  ((FlashPowerArr[0] << 32)|(FlashPowerArr[1] << 16)|(FlashPowerArr[2] << 8) | FlashPowerArr[3]);
		CopyPowerValue = FlashCopyPower/1000.0;	
		/*��ӡ������ĸ*/
		Flash_Read(PRINTFCOUNT,FlashpCount,4);	
		FlashpCountV =  ((FlashpCount[0] << 32)|(FlashpCount[1] << 16)|(FlashpCount[2] << 8) | FlashpCount[3]);
		pCount = FlashpCountV/1000.0;
							
		/*1.ɨ��ϵ��*/
		ScanFactor = StablePowerValue/ScanPowerValue;			
		/*2.��ӡϵ��*/
		PrintFactor = StablePowerValue/PrintPowerValue;
		/*3.��ӡϵ��*/
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
		
		
		/*���ƶԱ�ֵ��ʼ��*/
		WorkPowerFlag = StablePowerValue*11;
		PowerR[0] = StablePowerValue+StablePowerValue*Powerfactor;
		PowerD[0] = (StablePowerValue+StablePowerValue*Powerfactor);
		
		StablePowerValueMax = StablePowerValue+StablePowerValue*Powerfactor;
		StablePowerValueMin = StablePowerValue-StablePowerValue*Powerfactor;
		
		/*�ȶ����ʷ�Χ*/
		StableMaxpower = StablePowerValue+StablePowerValue*Powerfactor;
		StableMinpower = StablePowerValue-StablePowerValue*Powerfactor;
		/*ɨ�蹦�ʷ�Χ*/
		ScanMaxpower = ScanPowerValue+ScanPowerValue*Powerfactor;
		ScanMinpower = ScanPowerValue-ScanPowerValue*Powerfactor;
		
		
		ScanPowerValueMax = ScanPowerValue*2;
		SwitchAverageValue = 0;
		PowerMatchValue = 0;
		MatchCount = 0;
				
		/*����ƥ�����*/
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
		
		//�ϵ��ʼ���ϵ���Զ��ϵ��ȶ�״̬��־
		AutoPowerStable = 1;	
		ZeroArry[0] = 0;
		ZeroArry[1] = 0;
	}
	return;
}



/**
  * @brief  ���������ȶ�ֵƥ��
  * @retval void
  */
void Start(void)
{
	os_timer_disarm(&FirstPowerTimer);
	os_timer_arm(&StartTimer,2500,1,PowerStableFunc);
}

/**
  * @brief  �жϹ����Ƿ��ȶ� PSF  2.5sһ�� ��10��  25s���ж��Ƿ��Ѿ��Ǵ�������
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
				InitPSFLastPower[InitPSFcount] = Power_Ene.power;  //count ��1��ʼ��		
				//��LastPower[0]����Χ�ڱȽ� 
				if(PSFminPower < InitPSFLastPower[InitPSFcount] && InitPSFLastPower[InitPSFcount] < PSFmaxPower)
				{
					InitPSFcount++;
					if(InitPSFcount >= 12)
					{			
						/*ƥ���ȶ����ʳɹ���־*/						
						
						os_timer_disarm(&StartTimer);						
						InitPSFcount = 0;
						
						/*�ȶ�����ֵ*/
						StablePowerValue = (double) (InitPSFLastPower[0]+InitPSFLastPower[1]+
																				InitPSFLastPower[2]+InitPSFLastPower[3]+
																				InitPSFLastPower[4]+InitPSFLastPower[5]+
																				InitPSFLastPower[6]+InitPSFLastPower[7]+
																				InitPSFLastPower[8]+InitPSFLastPower[9]+
																				InitPSFLastPower[10]+InitPSFLastPower[11])/12;						
						//double���ͷŴ�1000��ת16λ�ٲ��2��8λ����Flash��					
						FlashStablePower = StablePowerValue*1000.0;
						FlashPowerArr[0] = (FlashStablePower >> 32)&0xff; //��8λ
						FlashPowerArr[1] = (FlashStablePower >> 16)&0xff; //��8λ
						FlashPowerArr[2] = (FlashStablePower >> 8)&0xff; //��8λ
						FlashPowerArr[3] = FlashStablePower&0xff; //��8λ						
						Flash_Write(STABLE_POWER_ADDR,FlashPowerArr,4);						
						
						/*�������ʳ�ʼ��ƥ��*/		
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
						GUI_ShowCHinese(24,8,16,"�뿪ʼ��ӡ",1);						
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
  * @brief  �������ʳ�ʼ��ƥ��
  * @retval void
  */
float PowerCalculate(float power)
{
	uint8_t WriteTip;
	/*�������Ƽ��*/							
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
			if(DirectionTimeR == 3) //����3�ζ�������������ƥ��
			{
				RiseCount = 0;
				DirectionTimeR = 0;			
			}						
			if(RiseCount > 2) 
			{	
				DirectionState = 1; //ƥ����������
				RInit = 0;			
				NowState = 1;
				RiseCount = 0;
				printsure = 0;
			}
		}
	}
	/*�½����Ƽ��*/			
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

		if(DirectionTimeD == 3) //����3�ζ�������������ƥ��
		{
			DownCount = 0;
			DirectionTimeD = 0;
		}
		if(DownCount > 3)
		{
			DirectionState = 2; //�½�����ȷ��
			DownCount = 0;
		}																							
	}	
	
	
	switch(PMFcount)
	{
		/*1.��ӡ����*/
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
					GUI_ShowCHinese(32,0,16,"�ɼ�������",1);	
					GUI_ShowCHinese(48,16,16,"���Ժ�",1);	
				}	
			}	
			else if(DirectionState == 2)
			{					
				/*3.���㹦��ֵ*/
				if(MatchCount > 50)
				{					
					CopyPowerValue = PowerMatchValue/MatchCount;												
					/*д��flash*/
					FlashCopyPower = CopyPowerValue*1000.0;
					FlashPowerArr[0] = (FlashCopyPower >> 32)&0xff; //��8λ
					FlashPowerArr[1] = (FlashCopyPower >> 16)&0xff; //��8λ
					FlashPowerArr[2] = (FlashCopyPower >> 8)&0xff; //��8λ
					FlashPowerArr[3] = FlashPrintPower&0xff; //��8λ
					Flash_Write(COPY_POWER_ADDR,FlashPowerArr,4);	
					
					delay_ms(1500);								
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"����ɸ�ӡ",1);	
					GUI_ShowCHinese(16,16,16,"�뿪ʼɨ��",1);	
		
					PowerMatchValue = 0.0;								
					MatchCount = 0;												
					PMFcount = 1;
					DirectionState = 0;
					RInit = 1;
				}
				else 
				{						
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"�ɼ�����ʧ��",1);	
					GUI_ShowCHinese(16,16,16,"���ٴθ�ӡ",1);	
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
		
		/*2.ɨ�蹦��*/	
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
					GUI_ShowCHinese(32,0,16,"�ɼ�������",1);	
					GUI_ShowCHinese(48,16,16,"���Ժ�",1);	
				}	
			}	
			else if(DirectionState == 2)
			{							
				if(MatchCount > 40)
				{					
					ScanPowerValue = PowerMatchValue/MatchCount; //ɨ�蹦��ƽ��ֵ			
					/*д��flash*/
					FlashScanPower =ScanPowerValue*1000.0;
					FlashPowerArr[0] = (FlashScanPower >> 32)&0xff; //��8λ
					FlashPowerArr[1] = (FlashScanPower >> 16)&0xff; //��8λ
					FlashPowerArr[2] = (FlashScanPower >> 8)&0xff; //��8λ
					FlashPowerArr[3] = FlashScanPower&0xff; //��8λ				
					Flash_Write(SCAN_POWER_ADDR,FlashPowerArr,4);			
					
					delay_ms(1500);									
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"�����ɨ��",1);	
					GUI_ShowCHinese(16,16,16,"�뿪ʼ��ӡ",1);		
									
					PowerMatchValue = 0.0;					
					MatchCount = 0;					
					PMFcount = 2;
					DirectionState = 0;					
					RInit = 1;			
				}
				else 
				{
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"�ɼ�����ʧ��",1);	
					GUI_ShowCHinese(16,16,16,"���ٴ�ɨ��",1);	
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
		/*3.��ӡ����*/		
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
					GUI_ShowCHinese(32,0,16,"�ɼ�������",1);	
					GUI_ShowCHinese(48,16,16,"���Ժ�",1);	
				}
				
			}	
			else if(DirectionState == 2)
			{	
				if(MatchCount > 40)
				{					
					PrintPowerValue = PowerMatchValue/MatchCount;		//��ӡ����ƽ��ֵ																			
					/*д��flash*/						
					FlashPrintPower = PrintPowerValue*1000.0;
					FlashPowerArr[0] = (FlashPrintPower >> 32)&0xff; //��8λ
					FlashPowerArr[1] = (FlashPrintPower >> 16)&0xff; //��8λ
					FlashPowerArr[2] = (FlashPrintPower >> 8)&0xff; //��8λ
					FlashPowerArr[3] = FlashPrintPower&0xff; //��8λ
					Flash_Write(PRINT_POWER_ADDR,FlashPowerArr,4);	
					
					pCount = MatchCount/2.0;
					/*д��flash*/						
					FlashpCountV = pCount*1000.0;
					FlashpCount[0] = (FlashpCountV >> 32)&0xff; //��8λ
					FlashpCount[1] = (FlashpCountV >> 16)&0xff; //��8λ
					FlashpCount[2] = (FlashpCountV >> 8)&0xff; //��8λ
					FlashpCount[3] = FlashpCountV&0xff; //��8λ					
					Flash_Write(PRINTFCOUNT,FlashpCount,4);
																												
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"����ɴ�ӡ",1);
					GUI_ShowCHinese(16,16,16,"����������",1);
					
					delay_ms(1500);
					/*��ɹ���ƥ�乤������־д��flash��*/
					WriteTip = 0xaa;
					Flash_Write_super(FIRSTPOWER,&WriteTip,1);											
				
					/*����ϵͳ*/
					 __set_FAULTMASK(1);//�ر����ж�
					NVIC_SystemReset();//����Ƭ������																
				}
				
				else 
				{				
					OLED_Clear(0);
					GUI_ShowCHinese(16,0,16,"�ɼ�����ʧ��",1);	
					GUI_ShowCHinese(16,16,16,"���ٴδ�ӡ",1);	
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
  * @brief  ��ӡ������ģʽƥ��
  * @retval void
  */
void PrinterWorkingMode(float power)
{
	uint8_t Total;	
	double m1, m2, m3;
	double s1,s2,s3;
	uint8_t PrintArry[2];


	/*�������Ƽ��*/	
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
			if(DirectionTimeR == 3) //����3�ζ�������������ƥ��
			{
				RiseCount = 0;
				DirectionTimeR = 0;			
			}						
			if(RiseCount > 2) 
			{	
				DirectionState = 1; //ƥ����������
				RInit = 0;							
				RiseCount = 0;
				NowState = 1; //������־
				OLED_Clear(0);
				GUI_ShowCHinese(32,8,16,"������",1);
			}
		}
	}
	
	/*�½����Ƽ��*/			
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
		if(DirectionTimeD == 3) //����2�ζ�������������ƥ��
		{
			DownCount = 0;
			DirectionTimeD = 0;
		}		
		if(DownCount > 2)
		{
			DirectionState = 2; //�½�����ȷ��
			DirectionTimeDOpen = 0;
			DownCount = 0;
		}																							
	}	
		
	/*1.����״̬*/
	if(power > StableMinpower)
	{
		/*�ж��Ƿ��¼*/
		if(FingerState == 0 && power > StableMaxpower)
		{
			/*�̵�������*/
			RelayOpen(0);
			/*�رչ���ģʽƥ��*/
			PowerInitFlag = 0;
			/*��ʾ��ˢ��*/
			OLED_Clear(0);
			GUI_ShowCHinese(32,0,16,"�Ƿ�����",1);
			GUI_ShowCHinese(32,16,16,"���ڹػ�",1);
			/*������Ƽ�¼*/
			Mcu_SendData_Route(0x02,ZeroArry,2,0); 			
			os_timer_arm(&OneMinuteAutoPowerTimre,30000,0,OneMinuteAutoPower);
			/*��������*/
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
			/*************************1.��ӡ����Ϊ�ж�************************/
			/*************************(1).����ģʽƥ��************************/		
			if(DirectionState == 1)
			{				
				MatchCount++;								
				NowPowerValue += power;	
				if(power > ScanPowerValueMax)
					MatchCountmax++;
			}							
			else if(DirectionState == 2) 
			{
				//1.ģʽƥ��
				NowPowerValue = NowPowerValue/MatchCount;
				if(ScanPowerValueMax <NowPowerValue)
				{
					PrinterReleMode = 2;
				}
				else
				{
					PrinterReleMode = 1; 
				}	
				
				//2.��������
				if(MatchCountmax >2)				
					SendpCount = (MatchCountmax/pCount)*1.0;				
				MtkPcout = (uint8_t)(SendpCount+0.5) > SendpCount?(SendpCount+0.5):SendpCount;
				MtkPcout = MtkPcout-1;
				PrintArry[0] = (MtkPcout >> 8)&0xff;
				PrintArry[1] = MtkPcout&0xff; //��8λ		
				
				//3.��Ϣ����
				if(PrinterReleMode ==2)
				{									
					if(MTK_TCP_Print == 1) /*MTK�յ���ӡָ�ȷ��Ϊ��ӡ��Ϊ*/
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
				/*��������������Ƿ����ȶ�״̬*/
				os_timer_arm(&WorkAfterTimer,200,1,WorkAfterTimeFunc);
				os_timer_arm(&VariateCleanTimer,3000,1,VariateCleanfunc);
			}
		}
	}	/****************************************/
	
	/******************************2.����********************************/
	else if((1 < power) && (power < StableMaxpower))
	{			
		/*����״̬��δ��¼�û�*/
		if(FingerState == 0)
		{
			WorkCount = 0;
			TimeCount = 0;
		}	
	}		
	
	/*3.�ػ�*/
	else 
	{
		powerOnOffFlag = 1;
		
		/*��ʾ��ˢ��*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"�ػ�",1);
	}
			
}

#endif


/**
  * @brief  �Ƿ��������Զ�����
  * @retval void
  */
void OneMinuteAutoPower(void)
{
	if(FingerState == 0)
	{
		/*ָ����֤��־��*/
		FingerState = 1;
		/*�̵�������*/
		RelayOpen(1);
		/*��ʾ��ˢ��*/
		OLED_Clear(0);
		GUI_ShowCHinese(32,8,16,"���ڿ���",1);
		/*�ȶ������ж�*/
		PSFcount = 0;
		os_timer_arm(&StableTimer,1000,0,StartStable);			
	}
	else
	{
		/*�̵�������*/
		RelayOpen(1);
		/*��ʾ��ˢ��*/
		OLED_Clear(0);
		GUI_ShowCHinese(48,8,16,"����",1);
		/*������Ƽ�¼*/
		Mcu_SendData_Route(0x02,ZeroArry,2,0); 		
	}		
	os_timer_disarm(&PowerMatchTimer);
}

/**
  * @brief  �Ƿ��������Զ�����
  * @retval void
  */
void AutoPrintf(void)
{
	/*�̵�������*/
	RelayOpen(1);
	/*��ʾ��ˢ��*/
	OLED_Clear(0);
	GUI_ShowCHinese(48,8,16,"����",1);
  /*������Ƽ�¼*/
	Mcu_SendData_Route(0x02,ZeroArry,2,0); 		
	
	os_timer_disarm(&PowerMatchTimer);
}

/**
  * @brief  ���������ȶ�ֵƥ��
  * @retval void
  */
void StartStable(void)
{
	os_timer_disarm(&StableTimer);
	os_timer_arm(&StartStableTimer,1500,1,Stable);
}


/**
  * @brief  �ȶ������жϺ���
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
				SPower[PSFcount] = Power_Ene.power;  //count ��1��ʼ��		
				//��LastPower[0]����Χ�ڱȽ� 
				if(PSFminPower < SPower[PSFcount] && SPower[PSFcount] < PSFmaxPower)
				{
					PSFcount++;
					if(PSFcount > 10)
					{									
						/*�ȶ�����ֵ*/
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
							/*ָ����֤��־��*/
							FingerState = 0;
							OLED_Clear(0);
							GUI_ShowCHinese(48,8,16,"����",1);
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
			NowState = 0; //������־
			/*ָ������֤��־*/
			FingerState = 0;	
			OLED_Clear(0);
			GUI_ShowCHinese(48,8,16,"����",1);
			/*������Ƽ�¼*/
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
