#ifndef __UART2_H

#include "stm32f10x.h"

unsigned char USART2_putchar(unsigned char ch);
void STM32_Uart2_Init(u32 Bound);
void USART2_Send_String(uint8_t *buf, uint8_t buf_size);

#endif
