/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Mcu_SendData_Route.h
  * @brief          : Header for Mcu_SendData_Route.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCUROUTE_H
#define __MCUROUTE_H


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
void delay();
void RouteDataAnalyze(uint8_t *pData);
void Mcu_SendData_Route(uint8_t cmd,const uint8_t *data,uint8_t data_size,uint8_t FID);
uint16_t CheckSum16(const uint8_t *dat, uint16_t len);
void LogFileVerify(void);
/* USER CODE END Private defines */

#endif /* __MCUROUTE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
