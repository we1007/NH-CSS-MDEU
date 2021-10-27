#include "UartPrintf.h"
#include <stdio.h>
#include "stm32f10x.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Uart1.h"
#include "Uart2.h"
#include "Uart5.h"
#include "Uart4.h"

#if 1
//#define UART2_PRINTF

#ifndef UART2_PRINTF
	#define USART  USART1
#else
	#define USART  USART2
#endif
/*
    加入以下代码,支持printf函数,不需要选择use MicroLIB	  
*/
#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(USART, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
  /* Your implementation of ferror */  
  return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)USART->DR & 0xFF;
	
    putchar(ch); //回显
	
	return ch;
}

#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
#endif

#ifdef UART1_DEBUG
void Uar1_Debug_Init(void)
{
		STM32_Uart1_Init( 115200 );
}
#endif

#ifdef UART1_DEBUG
void uart1_printf(const char *pstring, ...)
{
		//__va_list args;
    va_list args;    
    uint8_t str_size;
    uint8_t buf[128];
                
    va_start(args, pstring);
		str_size = vsprintf((char *)buf, pstring, args);      //-- format string in p
    va_end(args);
		USART1_Send_String((uint8_t *)buf,str_size); 
}
#endif

#ifdef UART2_DEBUG
void uart2_printf(const char *pstring, ...)
{
		//__va_list args;
    va_list args;    
    uint8_t str_size;
    uint8_t buf[128];
                
    va_start(args, pstring);
		
	  str_size = vsprintf((char *)buf, pstring, args);      //-- format string in p
    va_end(args);
		USART2_Send_String((uint8_t *)buf,str_size); 
}
#endif

#ifdef UART3_DEBUG
void uart3_printf(const char *pstring, ...)
{
		//__va_list args;
    va_list args;    
    uint8_t str_size;
    uint8_t buf[128];
                
    va_start(args, pstring);
		
	  str_size = vsprintf((char *)buf, pstring, args);      //-- format string in p
    va_end(args);
		USART3_Send_String((uint8_t *)buf,str_size); 
}
#endif

#ifdef UART4_DEBUG
void uart4_printf(const char *pstring, ...)
{
		//__va_list args;
    va_list args;    
    uint8_t str_size;
    uint8_t buf[128];
                
    va_start(args, pstring);
		
	  str_size = vsprintf((char *)buf, pstring, args);      //-- format string in p
    va_end(args);
		USART4_Send_String((uint8_t *)buf,str_size); 
}
#endif
