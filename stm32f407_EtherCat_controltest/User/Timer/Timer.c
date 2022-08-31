#include "stm32f4xx.h"                  // Device header

#include "Timer.h"
#include "ecatfun.h"


void Timer2_Init(void)
{
	// 外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	// 选择内部时钟
	TIM_InternalClockConfig(TIM2);
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000*10 - 1; // 定时器计次到1000 * 1000 -1后产生中断，即此时计时了1秒
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1; // 84*1000*1000 / ((84-1)+1) = 1000 * 1000 HZ(定时器一秒钟计次10*1000次)
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	// 此函数会在初始化时立马进入一次中断函数以更新预分频的配置
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	// 若想不进入，添加如下语句：
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	
	
	
	// 使能中断
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	// NVIV优先级分组
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//	// 启动定时器
//	TIM_Cmd(TIM2,ENABLE);
}

void Timer3_Init(void)
{
	// 外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// 选择外部时钟
	TIM_ITRxExternalClockConfig(TIM3, TIM_TS_ITR1);
	
	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
	
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000000; 
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	// 此函数会在初始化时立马进入一次中断函数以更新预分频的配置
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	// 若想不进入，添加如下语句：
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	// 使能中断
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	// NVIV优先级分组
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//	// 启动定时器
//	TIM_Cmd(TIM5,ENABLE);
}

void Timer5_Init(void)
{
	// 外设时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	// 选择内部时钟
	TIM_InternalClockConfig(TIM5);
	// 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1; // 定时器计次到1000后产生中断，即此时计时了1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1; // 84*1000*1000 / ((84-1)+1) = 1000 * 1000 HZ(定时器一秒钟计次1000*1000次)
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	// 此函数会在初始化时立马进入一次中断函数以更新预分频的配置
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	// 若想不进入，添加如下语句：
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	
	// 使能中断
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	// NVIV优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	// 启动定时器
	TIM_Cmd(TIM5,ENABLE);
}

void Timer2And3_Init(void)
{
	Timer2_Init();
	Timer3_Init();
}
void Timer2And3_start(void)
{
	// 启动定时器
	TIM_Cmd(TIM3,ENABLE);
	// 启动定时器
	TIM_Cmd(TIM2,ENABLE);
}


uint32_t getSec(void)
{
	return TIM3->CNT;
}

uint32_t getuSec(void)
{
	return TIM2->CNT;
}

extern int dorun;
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) == SET)
	{
		if(dorun==1)
		{
			ecat_loop();
		} 
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
