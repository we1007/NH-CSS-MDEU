#include "IOConf.h"
#include "os_time_pro.h"
#include "Beep.h"

TTimerItem delay_read_time;

const uint32_t IOx_CLK[IO_MAX_NUM] = {IO1_GPIO_CLK,IO2_GPIO_CLK,IO3_GPIO_CLK};
const uint16_t IOx_PIN[IO_MAX_NUM] = {IO1_PIN,IO2_PIN,IO3_PIN}; 
GPIO_TypeDef*  IOx_PORT[IO_MAX_NUM] = {IO1_GPIO_PORT,IO2_GPIO_PORT,IO3_GPIO_PORT}; 

/*
* ��  ��:IO��ʼ��
* ��  ��:IOx - IOѡ�����
* ����ֵ:��				 
* ��  ע:��
*/
void IO_Init(void)
{
	STM32_IOConfigInit(IO1); //������
	STM32_IOConfigInit(IO2); //�̵���
	STM32_IOConfigInit(IO3); //LED
	BUTTON_Init();
}


/*
* ��  ��:stm32 I/OD��ʼ��
* ��  ��:Ledx - I/Oѡ�����
* ����ֵ:��				 
* ��  ע:��
*/
void STM32_IOConfigInit(IOx_SELECT IOx)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					 //ʹ��I/O�˿�ʱ��
    /* ʹ��I/O��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(IOx_CLK[IOx], ENABLE);
    /* ��ʼ��I/O��GPIO�ܽţ�����Ϊ������� */				  //���ö˿�
    GPIO_InitStructure.GPIO_Pin = IOx_PIN[IOx] ;	 	  //���öԶ˿ڵ�ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //����Ϊ ����(Push-Pull)���	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�������ٶ�Ϊ50MHz
    GPIO_Init(IOx_PORT[IOx], &GPIO_InitStructure);	      //����GPIO��ʼ������	
}

/*
* ��  ��:stm32 I/O״̬����
* ��  ��:Ledx -I/Oѡ�����
		 LedxState - ״̬
* ����ֵ:��				 
* ��  ע:��
*/
void STM32_IOControl(IOx_SELECT IOx, IOx_STATE IOxState)
{
	 switch( IOxState )
	 {
	 	case DESTROY:	   GPIO_ResetBits(IOx_PORT[IOx], IOx_PIN[IOx]);
			break;
		case LIGHT:  GPIO_SetBits(IOx_PORT[IOx], IOx_PIN[IOx]);
			break;
	    default:
			break;
	 }
}


/*
* ��  ��:I/O״̬����
* ��  ��:Ledx - I/Oѡ�����
* ����ֵ:��				 
* ��  ע:��
*/
void IOControl(IOx_SELECT IOx, IOx_STATE IOxState)
{
	STM32_IOControl(IOx, IOxState);
}


void BUTTON_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the BUTTON Clock */
	/* ʹ��KEY������ӦGPIO��Clockʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(BUTTON_GPIO_CLK, ENABLE);

	/* Configure Button pin as input floating */
	/* ��ʼ��KEY������GPIO�ܽţ�����Ϊ������������ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(BUTTON_PortSource,BUTTON_PinSource);

	EXTI_InitStructure.EXTI_Line=BUTTON_EXTI_Line;	//KEY2
	EXTI_InitStructure.EXTI_Mode = BUTTON_EXTI_Mode;	
	EXTI_InitStructure.EXTI_Trigger = BUTTON_EXTI_Trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQChannel;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		os_timer_arm(&delay_read_time,2000,0,SystemReboot);  //5�붨ʱ����ȥ��zigbee�������� 			
		EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ 	
	}

}

