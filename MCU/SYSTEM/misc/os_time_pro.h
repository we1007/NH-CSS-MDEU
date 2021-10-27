#ifndef __OS_TIME_PRO_H
#define __OS_TIME_PRO_H

typedef void (*call_back)(void);

typedef struct _TIMER_CXT{
	char				timer_switch;		//��ʱ�����أ�1���򿪵�ǰ��ʱ����0���ر�
	unsigned int		timer_InterNum;		//��ʱ��ʱ�䣬��ʱ����
	char				TimerOut_flag;		//�ö�ʱ�������־λ
	unsigned char		loop_flag;			//��ʱ��ѭ����־λ
	unsigned int		cost_InterNum;		//���ڱ����жϴ�����
	call_back			timerout_cb;
	struct _TIMER_CXT * pnext;
}TTimerItem;


void os_timer_init(void);
void os_timer_handle(void);
void os_timer_arm(TTimerItem *timer_arg, unsigned int num, unsigned char LoopFlag, call_back cb);
void os_timer_disarm(TTimerItem *timer_arg);
void Timer4_Init(void);

#endif

