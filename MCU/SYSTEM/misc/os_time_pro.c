#include "os_time_pro.h"

#include "stm32f10x.h"
#include <string.h>
#include "Timer.h"

TTimerItem				*ptimer_link;				//定时器链表


/************************************************************************/
/*
函数功能：对定时器初始化，和打开之前一些变量初始化
参数：void
返回值：void
*/
/************************************************************************/
void os_timer_init(void)
{
		TIM4_Int_Init((1000-1),(72-1)) ;				//初始化定时器
		ptimer_link = NULL;			//链表指针初始化为NULL
}



//回调函数call_back主要触发事件调用回调函数cb
void os_timer_arm(TTimerItem *timer_arg, unsigned int num, unsigned char LoopFlag, call_back cb)
{
		TTimerItem			* ptimer;
		char				flag = 0;

		timer_arg->timer_switch		= 1;			//打开定时器
		timer_arg->timer_InterNum	= num;			//定时时间
		timer_arg->TimerOut_flag	= 0;			//清除溢出标志位
		timer_arg->loop_flag		= LoopFlag;		//表示是否需要循环计时
		timer_arg->cost_InterNum	= num;
		timer_arg->timerout_cb		= cb;

		//查看链表中是否已经存在该定时器
		for(ptimer = ptimer_link; ; ptimer = ptimer->pnext)
		{
				if (!ptimer)
				{
						break;
				}
				if (ptimer == timer_arg)
				{
						flag = 1;
						break;
				}

				if (!ptimer->pnext)
				{
						break;
				}
		}

		//将定时器添加至链头
		if (!flag)
		{
				static int num = 0;
				num++;
				timer_arg->pnext			= ptimer_link;
				ptimer_link					= timer_arg;
		}
}

void os_timer_disarm(TTimerItem *timer_arg)
{
		timer_arg->timer_switch		= 0;			//关闭定时器
		timer_arg->timer_InterNum	= 0;			//定时时间
		timer_arg->TimerOut_flag	= 0;			//清除溢出标志位
		timer_arg->loop_flag		= 0;
		timer_arg->cost_InterNum	= 0;
		timer_arg->timerout_cb		= NULL;
}


/************************************************************************/
/*
函数功能：用于控制LED闪烁的定时器，开始是关闭状态。等到接收到LED闪烁的指令，才启动。
1ms中断一次，控制LED的亮灭时间。
参数：void
返回值：void
*/
/************************************************************************/

void os_timer_handle(void)
{
		TTimerItem			*ptimer;

		//循环检测每个定时器是否溢出
		for(ptimer = ptimer_link; ; ptimer = ptimer->pnext)
		{
	
			if (!ptimer)
				{
						break;
				}

				if (ptimer->TimerOut_flag)
				{
						ptimer->TimerOut_flag = 0;			// 清除溢出标志位
						if (ptimer->timerout_cb != NULL)
						{
								ptimer->timerout_cb();			// 进入回调函数
						}
				}

				if (!ptimer->pnext)
				{
						break;
				}
		}
}

/************************************************************************/
/*
函数功能：定时器4溢出中断，1ms 中断一次。10ms 进行一次数据加1，LED开关时间
是接收到指令的数据内容的10倍。
*/
/************************************************************************/

void TIM4_IRQHandler(void)
{
	 TTimerItem			* ptimer;
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//判断中断是否溢出
   {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		//清楚中断标志位

				for(ptimer = ptimer_link; ; ptimer = ptimer->pnext)
				{
					  if (!ptimer)
					  {
							break;
						}
						if (ptimer->timer_switch)
						{
								ptimer->timer_InterNum--;
								if (!ptimer->timer_InterNum)
								{
										if (ptimer->loop_flag)
										{
												ptimer->timer_InterNum = ptimer->cost_InterNum;	//重复计时
										}
										else
										{
												ptimer->timer_switch = 0;			//关定时器
										}
										ptimer->TimerOut_flag = 1;
								 }
						}

						if (!ptimer->pnext)
						{
								break;
						}
				}
		}		
}

