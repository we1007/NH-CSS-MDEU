#include "Uart4.h"
#include "UartPrintf.h"

#define EN_USART4_RX 1
/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   ��STM32��USART��ʼ���ײ㺯��
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART3 ��Ӧ�������ϴ���2
  * @����2  ָ��һ����Ա�Ѹ�ֵUSART_InitTypeDef�ṹ���ָ��
  * @����ֵ ��
***------------------------------------------------------*/

void STM32_Uart4_Init(u32 Bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if EN_USART4_RX
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	USART_DeInit(UART4);  //��λ����4

	//USART4_TX   PC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��

	//USART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PA10

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = Bound;//һ������Ϊ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(UART4, &USART_InitStructure); //��ʼ������
#if EN_USART4_RX		  //���ʹ���˽���  
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
	
#endif
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
}

#if 0
void USART4_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
				Res =USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
				//USART1_putchar( Res );
		}
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif


unsigned char USART4_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(UART4, (uint8_t) ch); /*����һ���ַ�����*/ 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)/*�ȴ��������*/
  {
			
  }
  return ch;
}

void USART4_Send_String(uint8_t *buf, uint8_t buf_size)
{
	uint8_t i;
	for(i=0; i<buf_size; i++)
	{
		 USART4_putchar( (*(buf+i)) );
	}
}





