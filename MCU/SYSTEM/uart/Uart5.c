#include "Uart5.h"
#define EN_USART5_RX 1
/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   ��STM32��USART��ʼ���ײ㺯��
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART3 ��Ӧ�������ϴ���2
  * @����2  ָ��һ����Ա�Ѹ�ֵUSART_InitTypeDef�ṹ���ָ��
  * @����ֵ ��
***------------------------------------------------------*/
void STM32_Uart5_Init(u32 Bound)    //��ʼ��
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if EN_USART5_RX
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��UART5��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	USART_DeInit(UART5);  //��λ����1
  // TX PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
	// RX PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);  

  // ��ʼ������
	USART_InitStructure.USART_BaudRate = Bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(UART5, &USART_InitStructure); //��ʼ������
#if EN_USART5_RX		  //���ʹ���˽���  
  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
#endif
  USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 
}


unsigned char UART5_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(UART5, (uint8_t) ch); /*����һ���ַ�����*/ 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET)/*�ȴ��������*/
  {
			
  }
  return ch;
}

void UART5_Send_String(uint8_t *buf, uint16_t buf_size)    //�������
{
	uint16_t i;
	for(i=0; i<buf_size; i++)
	{
		 UART5_putchar( (*(buf+i)) );
	}
}



