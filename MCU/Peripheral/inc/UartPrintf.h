#ifndef __UARTPRINTF_H
#define __UARTPRINTF_H

//#define UART4_DEBUG
//#define UART2_DEBUG
//#define UART1_DEBUG
//#define UART3_DEBUG

#ifdef UART1_DEBUG
void Uar1_Debug_Init(void);
void uart1_printf(const char *pstring, ...);
#endif

#ifdef UART2_DEBUG
void uart2_printf(const char *pstring, ...);
#endif

#ifdef UART3_DEBUG
void uart3_printf(const char *pstring, ...);
#endif

#ifdef UART4_DEBUG
void uart4_printf(const char *pstring, ...);
#endif

#endif

