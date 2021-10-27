#ifndef __UART1_H
#define __UART1_H

#include "stm32f10x.h"

unsigned char USART1_putchar(unsigned char ch);
void STM32_Uart1_Init(unsigned int Bound);
void USART1_Send_String(uint8_t *buf, uint8_t buf_size);

#endif
