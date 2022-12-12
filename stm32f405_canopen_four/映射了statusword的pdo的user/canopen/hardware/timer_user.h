#ifndef __TIMER_USER_H
#define __TIMER_USER_H                
#include "stm32f4xx.h"                  // Device header

#include "main.h"

void TIM3_Init(void);
void TIM4_Init(void);
void timer_callback(void);

//void Timer2_Init(void);

#endif
