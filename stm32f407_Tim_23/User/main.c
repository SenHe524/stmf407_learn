#include "stm32f4xx.h"                  // Device header

#include "delay.h"
#include "Timer.h"
uint32_t usec = 0;
uint16_t sec = 0;
int main(void)
{
	delay_init();
	Timer2And3_Init();
	Timer2And3_start();
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	// 开启外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	// GPIO初始化
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	while(1)
	{
		usec = getuSec();
		sec = getSec();
		if(sec%2 == 0)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
		}
		else{
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		}

	}
			
}
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//	{
//		num++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}

