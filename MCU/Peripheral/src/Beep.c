/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Beep.c
  * @brief          : 
  ******************************************************************************
  * @attention
  *			蜂鸣器，继电器控制
  * 	
  * 
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "Beep.h"
#include "os_time_pro.h"
#include "flash.h"
#include "oled.h"
#include "gui.h"

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

/* USER CODE BEGIN PV */
TTimerItem RelayLoopTimer;
TTimerItem OpenRelayTimer;

uint8_t RelayState = 0;
uint8_t BeepState = 0;
extern uint8_t RFIDState;  //继电器状态 0关 1开
extern uint8_t FingerState;  //指纹状态：0：未验证 1：验证成功
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void BeepLoopControl(void)
{
	if(BeepState == 1)
	{			
		BeepOpen(0); //关闭蜂鸣器		
		os_timer_arm(&RelayLoopTimer,2500,0,RelayLoopControl);
	}		
}


/**
  * @brief  Beep control function
  * @retval void
  */
void BeepOpen(uint8_t state)
{
	switch(state)
	{
		case 0:
		{
			BeepState = 0;
			STM32_IOControl(IO1,DESTROY);
			break;
		}
		case 1:
		{
			BeepState = 1;
			STM32_IOControl(IO1,LIGHT);
			break;
		}	
		default:
			break;
	}
	return;
}
/**
  * @brief  Relay control function 
  * @retval void
  */
void RelayOpen(uint8_t state)
{
	switch(state)
	{
		case 0:  //关
		{
			RelayState = 0;
			STM32_IOControl(IO2,DESTROY);
		}			
			break;
		
		case 1: //开
		{
			RelayState = 1;
			STM32_IOControl(IO2,LIGHT);
		}			
			break;
		
		default:
			break;
	}
	return;
}

/**
  * @brief  LED control function 
  * @retval void
  */
void LedOpen(uint8_t state)
{
	switch(state)
	{
		case 0: //关
		{			
			STM32_IOControl(IO3,LIGHT);
			break;
		}				
		case 1: //开
		{
			STM32_IOControl(IO3,DESTROY);			
			break;
		}			
		default:
			break;
	}
	return;
}

//系统复位
void SystemReboot(void)
{
		uint8_t WriteTip;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==0)		//状态检测
		{		
			LedOpen(0);		
			WriteTip = 0xFF;
			Flash_Write_super(FIRSTPOWER,&WriteTip,1);
			
			__set_FAULTMASK(1);		//关闭所有中断
			NVIC_SystemReset();		//复位MCU    定义在 core_cm3
		}
		else
		{
			return;
		}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
