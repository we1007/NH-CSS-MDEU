#ifndef __UART4_H
#define __UART4_H

#include "stm32f10x.h"

void STM32_Uart4_Init(unsigned int Bound);
void USART4_Send_String(uint8_t *buf, uint8_t buf_size);
unsigned char USART4_putchar(unsigned char ch);

#endif


