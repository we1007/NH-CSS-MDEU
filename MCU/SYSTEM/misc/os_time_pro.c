#include "os_time_pro.h"

#include "stm32f10x.h"
#include <string.h>
#include "Timer.h"

TTimerItem				*ptimer_link;				//��ʱ������


/************************************************************************/
/*
�������ܣ��Զ�ʱ����ʼ�����ʹ�֮ǰһЩ������ʼ��
������void
����ֵ��void
*/
/************************************************************************/
void os_timer_init(void)
{
		TIM4_Int_Init((1000-1),(72-1)) ;				//��ʼ����ʱ��
		ptimer_link = NULL;			//����ָ���ʼ��ΪNULL
}



//�ص�����call_back��Ҫ�����¼����ûص�����cb
void os_timer_arm(TTimerItem *timer_arg, unsigned int num, unsigned char LoopFlag, call_back cb)
{
		TTimerItem			* ptimer;
		char				flag = 0;

		timer_arg->timer_switch		= 1;			//�򿪶�ʱ��
		timer_arg->timer_InterNum	= num;			//��ʱʱ��
		timer_arg->TimerOut_flag	= 0;			//��������־λ
		timer_arg->loop_flag		= LoopFlag;		//��ʾ�Ƿ���Ҫѭ����ʱ
		timer_arg->cost_InterNum	= num;
		timer_arg->timerout_cb		= cb;

		//�鿴�������Ƿ��Ѿ����ڸö�ʱ��
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

		//����ʱ���������ͷ
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
		timer_arg->timer_switch		= 0;			//�رն�ʱ��
		timer_arg->timer_InterNum	= 0;			//��ʱʱ��
		timer_arg->TimerOut_flag	= 0;			//��������־λ
		timer_arg->loop_flag		= 0;
		timer_arg->cost_InterNum	= 0;
		timer_arg->timerout_cb		= NULL;
}


/************************************************************************/
/*
�������ܣ����ڿ���LED��˸�Ķ�ʱ������ʼ�ǹر�״̬���ȵ����յ�LED��˸��ָ���������
1ms�ж�һ�Σ�����LED������ʱ�䡣
������void
����ֵ��void
*/
/************************************************************************/

void os_timer_handle(void)
{
		TTimerItem			*ptimer;

		//ѭ�����ÿ����ʱ���Ƿ����
		for(ptimer = ptimer_link; ; ptimer = ptimer->pnext)
		{
	
			if (!ptimer)
				{
						break;
				}

				if (ptimer->TimerOut_flag)
				{
						ptimer->TimerOut_flag = 0;			// ��������־λ
						if (ptimer->timerout_cb != NULL)
						{
								ptimer->timerout_cb();			// ����ص�����
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
�������ܣ���ʱ��4����жϣ�1ms �ж�һ�Ρ�10ms ����һ�����ݼ�1��LED����ʱ��
�ǽ��յ�ָ����������ݵ�10����
*/
/************************************************************************/

void TIM4_IRQHandler(void)
{
	 TTimerItem			* ptimer;
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//�ж��ж��Ƿ����
   {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		//����жϱ�־λ

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
												ptimer->timer_InterNum = ptimer->cost_InterNum;	//�ظ���ʱ
										}
										else
										{
												ptimer->timer_switch = 0;			//�ض�ʱ��
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

