#include "Timer.h"
#include <stdio.h>

#if 0
/* 启动定时器2 */
void Start_Timer1(void)
{
	TIM_Cmd(TIM1, ENABLE);
}

/* 关闭定时器2 */
 void Close_Timer1(void)
{
	TIM_Cmd(TIM1, DISABLE);
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器1
void TIM1_Int_Init(uint16_t arr,uint16_t psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
	
	//Step1.时钟设置：启动TIM1 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   
	//TIM_OCInitTypeDef TIM_OCInitStructure;  //比较PWM的时候用。
	//TIM1 使用内部时钟 
	//TIM_InternalClockConfig(TIM1);
	//TIM1基本设置 
	//设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz 
	//TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，计数值归零 
	//向上计数模式 
	//TIM_RepetitionCounter(TIM1_RCR)=0，每次向上溢出都产生更新事件 
	TIM_BaseInitStructure.TIM_Period = arr; 
	TIM_BaseInitStructure.TIM_Prescaler = psc; 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
	//清中断，以免一启用中断后立即产生中断 
	TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
	//使能TIM1中断源 
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
	//Step3.中断NVIC设置：允许中断，设置优先级 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    //更新事件 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //响应优先级1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
	NVIC_Init(&NVIC_InitStructure);                             //写入设置

	//TIM1总开关：关闭 
	TIM_Cmd(TIM1, DISABLE); 
}


/* 启动定时器2 */
void Start_Timer2(void)
{
	TIM_Cmd(TIM2, ENABLE);
}

/* 关闭定时器2 */
 void Close_Timer2(void)
{
	TIM_Cmd(TIM2, DISABLE);
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2!
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, DISABLE);  //关闭使能TIM2	 
}

/* 启动定时器3 */
void Start_Timer3(void)
{
	TIM_Cmd(TIM3, ENABLE);
}

/* 关闭定时器3 */
 void Close_Timer3(void)
{
	TIM_Cmd(TIM3, DISABLE);
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, DISABLE);  //使能TIMx					 
}


/* 启动定时器4 */
void Start_Timer4(void)
{
	TIM_Cmd(TIM4, ENABLE);
}

/* 关闭定时器4 */
 void Close_Timer4(void)
{
	TIM_Cmd(TIM4, DISABLE);
}
#endif


//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
void TIM4_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  //使能TIM4					 
}





