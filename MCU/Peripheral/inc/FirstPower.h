/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : FirstPower.h
  * @brief          : Header for FirstPower.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FIRSTPOWER_H
#define __FIRSTPOWER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

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
#define ErrorMinCoefficientPSF 0.1000
#define ErrorMinCoefficient 0.2200
#define ErrorMaxCoefficient 0.750
#define RandomTimes 11.0

/* USER CODE END Private defines */

void delayfunc(void);
void PowerAfterFunc(void);
void Start(void);
void FirstPower(void);
void PowerMatchTimerFunc(void);
void PowerStableFunc(void);
void PowerMatchFunction(void);
float PowerCalculate(float power);
void PrinterWorkingMode(float power);
void OneMinuteAutoPower(void);
void Stable(void);
void StartStable(void);
void VariateCleanfunc(void);
void AutoPrintf(void);
void WorkAfterTimeFunc(void);

#endif /* __FIRSTPOWER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
