#ifndef __UART3_H
#define __UART3_H

#include "stm32f10x.h"

void STM32_Uart5_Init(unsigned int Bound);
void UART5_Send_String(uint8_t *buf, uint16_t buf_size);
unsigned char UART5_putchar(unsigned char ch);
void UART5_IRQHandler(void);

uint8_t *hb_uart_data_analyze(uint8_t rx_data);

#endif


