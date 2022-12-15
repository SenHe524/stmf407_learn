#include "timer_user.h"

//TIMEVAL last_counter_val = 0;
//TIMEVAL elapsed_time = 0;

#define ODOMETRY_MAX_TIMES		2
uint32_t cur_time = 0;//时间计数
uint32_t next_time_set  = 0;//下一次触发时间计数
uint32_t odometry_times = 0;
uint8_t timer_10ms_ = 0;

static TIMEVAL last_time_set = TIMEVAL_MAX;//上一次的时间计数

// Initializes the timer, turn on the interrupt and put the interrupt time to zero
void TIM3_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 100-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;//84M频率/84为1000k(与timerscfg.h配置一致即可)，即1us间隔
//	TIM_TimeBaseStructure.TIM_Period = 65535;
//	TIM_TimeBaseStructure.TIM_Prescaler = 840;//84M频率/84为1000k(与timerscfg.h配置一致即可)，即1us间隔
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);
 
	/* TIM3 enable counter */  //启动TIM3
	TIM_Cmd(TIM3, ENABLE);

	/* Preset counter for a safe start */
	TIM_SetCounter(TIM3, 1);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void TIM4_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000;//40ms一个中断
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;//84M频率/840为100k，即10us间隔
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	
	TIM_ClearITPendingBit(TIM4, TIM_SR_UIF);
 
	/* TIM3 enable counter */  //启动TIM4
	TIM_Cmd(TIM4, ENABLE);

	/* Preset counter for a safe start */
	TIM_SetCounter(TIM4, 1);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}


//Set the timer for the next alarm.
void setTimer(TIMEVAL value)
{
  	next_time_set  = (cur_time  + value) % TIMEVAL_MAX;
}

//Return the elapsed time to tell the Stack how much time is spent since last call.
TIMEVAL getElapsedTime(void)
{
	return cur_time > last_time_set ? cur_time  - last_time_set : cur_time  + TIMEVAL_MAX - last_time_set;
}

void timer_callback(void)
{
	last_time_set = cur_time;
	cur_time++;
	if(cur_time > TIMEVAL_MAX)
	{
		cur_time = 0;
	}
	if(cur_time == next_time_set)
	{
		TimeDispatch();
	}
}

// This function handles Timer 3 interrupt request.
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3, TIM_SR_UIF) == RESET)//	过滤更新中断外的其他中断
		return;
	timer_callback();
	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM4, TIM_SR_UIF) == RESET)//	过滤更新中断外的其他中断
		return;
//	odometry_times++;
//	odometry_times %= ODOMETRY_MAX_TIMES;
//	if(odometry_times == 0)
//	{
//		Odometry_data();
//		imu_data_send();
//	}
	timer_10ms_ = 1;
//	Odometry_data();
//	imu_data_send();
	TIM_ClearITPendingBit(TIM4, TIM_SR_UIF);
}


////Set the timer for the next alarm.
//void setTimer(TIMEVAL value)
//{
//  	uint32_t timer = TIM_GetCounter(TIM3);        // Copy the value of the running timer
//	elapsed_time += timer - last_counter_val;
//	last_counter_val = 65535-value;
//	TIM_SetCounter(TIM3, 65535-value);
//	TIM_Cmd(TIM3, ENABLE);
//	//printf("setTimer %lu, elapsed %lu\r\n", value, elapsed_time);
//}

////Return the elapsed time to tell the Stack how much time is spent since last call.
//TIMEVAL getElapsedTime(void)
//{
//  	uint32_t timer = TIM_GetCounter(TIM3);        // Copy the value of the running timer
//	if(timer < last_counter_val)
//		timer += 65535;
//	TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
//	//printf("elapsed %lu - %lu %lu %lu\r\n", elapsed, timer, last_counter_val, elapsed_time);
//	return elapsed;
//}

//// This function handles Timer 3 interrupt request.
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM3, TIM_SR_UIF) == RESET)
//		return;
//	last_counter_val = 0;
//	elapsed_time = 0;
//	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);
//	TimeDispatch();
//}



