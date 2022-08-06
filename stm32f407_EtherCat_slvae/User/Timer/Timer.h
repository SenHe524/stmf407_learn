#ifndef __TIMER_H
#define __TIMER_H                
#include "stm32f4xx.h"                  // Device header


void Timer2_Init(void);
void Timer5_Init(void);
void Timer2And3_Init(void);
void Timer2And3_start(void);
void Timer5_Init(void);
uint32_t getSec(void);
uint32_t getuSec(void);

#endif
