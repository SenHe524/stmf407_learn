#include "delay.h"


void delay_init(void) 
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 83;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}                                    

void delay_ms(u16 nms)
{    
    u32 cnt;
    cnt = nms*1000;
    TIM_Cmd(TIM4, ENABLE);
    while(cnt--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE);         
}   
//延时nus
//nus为要延时的us数.                                               
void delay_us(u32 nus)
{
    u32 cnt;
    cnt = nus;
    TIM_Cmd(TIM4, ENABLE);
    while(cnt--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE);
}
