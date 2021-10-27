#include "IOConf.h"
#include "os_time_pro.h"
#include "Beep.h"

TTimerItem delay_read_time;

const uint32_t IOx_CLK[IO_MAX_NUM] = {IO1_GPIO_CLK,IO2_GPIO_CLK,IO3_GPIO_CLK};
const uint16_t IOx_PIN[IO_MAX_NUM] = {IO1_PIN,IO2_PIN,IO3_PIN}; 
GPIO_TypeDef*  IOx_PORT[IO_MAX_NUM] = {IO1_GPIO_PORT,IO2_GPIO_PORT,IO3_GPIO_PORT}; 

/*
* 功  能:IO初始化
* 形  参:IOx - IO选择变量
* 返回值:无				 
* 备  注:无
*/
void IO_Init(void)
{
	STM32_IOConfigInit(IO1); //蜂鸣器
	STM32_IOConfigInit(IO2); //继电器
	STM32_IOConfigInit(IO3); //LED
	BUTTON_Init();
}


/*
* 功  能:stm32 I/OD初始化
* 形  参:Ledx - I/O选择变量
* 返回值:无				 
* 备  注:无
*/
void STM32_IOConfigInit(IOx_SELECT IOx)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					 //使能I/O端口时钟
    /* 使能I/O对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(IOx_CLK[IOx], ENABLE);
    /* 初始化I/O的GPIO管脚，配置为推挽输出 */				  //设置端口
    GPIO_InitStructure.GPIO_Pin = IOx_PIN[IOx] ;	 	  //设置对端口的模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //设置为 推挽(Push-Pull)输出	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最大输出速度为50MHz
    GPIO_Init(IOx_PORT[IOx], &GPIO_InitStructure);	      //调用GPIO初始化函数	
}

/*
* 功  能:stm32 I/O状态控制
* 形  参:Ledx -I/O选择变量
		 LedxState - 状态
* 返回值:无				 
* 备  注:无
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
* 功  能:I/O状态控制
* 形  参:Ledx - I/O选择变量
* 返回值:无				 
* 备  注:无
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
	/* 使能KEY按键对应GPIO的Clock时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(BUTTON_GPIO_CLK, ENABLE);

	/* Configure Button pin as input floating */
	/* 初始化KEY按键的GPIO管脚，配置为带上拉的输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(BUTTON_PortSource,BUTTON_PinSource);

	EXTI_InitStructure.EXTI_Line=BUTTON_EXTI_Line;	//KEY2
	EXTI_InitStructure.EXTI_Mode = BUTTON_EXTI_Mode;	
	EXTI_InitStructure.EXTI_Trigger = BUTTON_EXTI_Trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQChannel;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		os_timer_arm(&delay_read_time,2000,0,SystemReboot);  //5秒定时后再去读zigbee按键输入 			
		EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位 	
	}

}

