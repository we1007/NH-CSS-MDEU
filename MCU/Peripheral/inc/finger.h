/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : finger.h
  * @brief          : Header for finger.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FINGER_H
#define __FINGER_H

#include "stm32f10x.h"
/* Includes ------------------------------------------------------------------*/

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
uint8_t *finger_uart_data_analyze(uint8_t rx_data);
void FingerControl(uint8_t *pData);
void FingerSend(void);
void Finger_Search_Init(void);
void Finger_BUTTON_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#endif /* __FINGER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
