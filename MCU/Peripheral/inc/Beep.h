/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Beep.h
  * @brief          : Header for Beep.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BEEP_H
#define __BEEP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "IOConf.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
void Beep_Init();
void BeepOpen(uint8_t state);
void BeepLoopControl(void);
void RelayOpen(uint8_t state);
void LedOpen(uint8_t state);
void RelayLoopControl(void);
void OpenRelayTimerFunc(void);
void SystemReboot(void);
#endif /* __BEEP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
