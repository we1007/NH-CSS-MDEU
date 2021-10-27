#include "SysClockConfig.h"
#include "stm32f10x.h"

/* ����ΪHSIϵͳʱ��8M */
void RCC_Configuration(void)
{
		RCC_DeInit();
	 
		RCC_HSICmd(ENABLE);
	 
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		{
	 
		} 
		 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		 FLASH_SetLatency(FLASH_Latency_2);
		
		 RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHBʱ��Ϊϵͳʱ��SYSCLK
		
		 RCC_PCLK2Config(RCC_HCLK_Div1);  //APB1ʱ��ΪHCLK/2������HCLKΪAHBʱ��
		
		 RCC_PCLK1Config(RCC_HCLK_Div2);  //APB2ʱ��ΪHCLK
		
		 //���� PLL ʱ��Դ����Ƶϵ��
		
		 RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);/* ����PLLʱ��Դ����Ƶϵ����Ƶ��Ϊ8/2*16 = 64M*/ 
		
		 //ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE
		
		 RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��
		
		 //�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
		
		 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		 {
		
		 }
		 //����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ  
		 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		 //�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ
			 
		 // 0x00��HSI ��Ϊϵͳʱ��
		
		 // 0x04��HSE��Ϊϵͳʱ��
		
		 // 0x08��PLL��Ϊϵͳʱ��
		
		 while(RCC_GetSYSCLKSource() != 0x08)
		 {
		
		
		 }
 }
