#include "Uart2.h"
#define EN_USART2_RX 1

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   ��STM32��USART��ʼ���ײ㺯��
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  ָ��һ����Ա�Ѹ�ֵUSART_InitTypeDef�ṹ���ָ��
  * @����ֵ ��
***------------------------------------------------------*/
void STM32_Uart2_Init(u32 Bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
#if EN_USART2_RX
	  NVIC_InitTypeDef NVIC_InitStructure;
#endif
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	  USART_DeInit(USART2);  //��λ����2
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

    //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = Bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_Even;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
#if EN_USART2_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�����ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}

unsigned char USART2_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch); /*����һ���ַ�����*/ 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)/*�ȴ��������*/
  {
			
  }
  return ch;
}

void USART2_Send_String(uint8_t *buf, uint8_t buf_size)
{
	uint8_t i;
	for(i=0; i<buf_size; i++)
	{
		 USART2_putchar( (*(buf+i)) );
	}
}

#if 0

#endif
