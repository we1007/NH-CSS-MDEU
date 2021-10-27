#ifndef __LEDCONF_H
#define __LEDCONF_H

#include "stm32f10x.h"

#define IO_MAX_NUM  4   //led灯的数量



/* led选择枚举 */
typedef enum
{
	IO1 = 0 ,
	IO2 = 1 ,
	IO3 = 2,
	IO4 = 3,
}IOx_SELECT;

/* led选择枚举 */
typedef enum
{
	LIGHT = 1 ,
	DESTROY = 0,
}IOx_STATE;

/** I/O管脚 **/
#define IO1_PIN                  GPIO_Pin_1
#define IO1_GPIO_PORT            GPIOA
#define IO1_GPIO_CLK             RCC_APB2Periph_GPIOA 

/** I/O管脚  **/
#define IO2_PIN                  GPIO_Pin_2
#define IO2_GPIO_PORT            GPIOB
#define IO2_GPIO_CLK             RCC_APB2Periph_GPIOB 

/** I/O管脚 LED灯 **/
#define IO3_PIN                  GPIO_Pin_11
#define IO3_GPIO_PORT            GPIOA
#define IO3_GPIO_CLK             RCC_APB2Periph_GPIOA

/** I/O管脚 按键 **/
#define BUTTON_PIN                GPIO_Pin_1 
#define BUTTON_GPIO_PORT          GPIOC
#define BUTTON_GPIO_CLK  					RCC_APB2Periph_GPIOC
//exit
#define BUTTON_PortSource         GPIO_PortSourceGPIOC 
#define BUTTON_PinSource          GPIO_PinSource1 
#define BUTTON_EXTI_Line          EXTI_Line1 
#define BUTTON_EXTI_Mode          EXTI_Mode_Interrupt 
#define BUTTON_EXTI_Trigger       EXTI_Trigger_Falling
//NVIC
#define BUTTON_IRQChannel         EXTI1_IRQn 

void IO_Init(void);
void STM32_IOConfigInit(IOx_SELECT IOx);
void STM32_IOControl(IOx_SELECT IOx, IOx_STATE IOxState);
void IOControl(IOx_SELECT IOx, IOx_STATE IOxState);
void BUTTON_Init(void);
#endif


