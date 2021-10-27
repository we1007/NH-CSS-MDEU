 #ifndef __TIMER_H
 #define __TIMER_H

#include "stm32f10x.h"
void Start_Timer1(void);
void Close_Timer1(void);
void TIM1_Int_Init(uint16_t arr,uint16_t psc);

void Start_Timer2(void);
void Close_Timer2(void);
void TIM2_Int_Init(unsigned short arr,unsigned short  psc);

void Start_Timer3(void);
void Close_Timer3(void);
void TIM3_Int_Init(uint16_t arr,uint16_t psc);

void Start_Timer4(void);
void Close_Timer4(void);
void TIM4_Int_Init(uint16_t arr,uint16_t psc);

#endif

