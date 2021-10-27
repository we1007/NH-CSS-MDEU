#ifndef __OS_TIME_PRO_H
#define __OS_TIME_PRO_H

typedef void (*call_back)(void);

typedef struct _TIMER_CXT{
	char				timer_switch;		//定时器开关，1：打开当前定时器。0：关闭
	unsigned int		timer_InterNum;		//定时器时间，定时次数
	char				TimerOut_flag;		//该定时器溢出标志位
	unsigned char		loop_flag;			//定时器循环标志位
	unsigned int		cost_InterNum;		//用于备份中断次数的
	call_back			timerout_cb;
	struct _TIMER_CXT * pnext;
}TTimerItem;


void os_timer_init(void);
void os_timer_handle(void);
void os_timer_arm(TTimerItem *timer_arg, unsigned int num, unsigned char LoopFlag, call_back cb);
void os_timer_disarm(TTimerItem *timer_arg);
void Timer4_Init(void);

#endif

