/***************************************************************************************

 * 硬件连接：  PF11 -> W5500_SPI3_RST   
 *             PF12 -> W5500_SPI3_INT     
 *             PF13 -> W5500_SPI3_SCS      
 *             PC10 -> W5500_SPI3_SCK_PIN    
 *             PC11 -> W5500_SPI3_MISO_PIN    
 *             PC12 -> W5500_SPI3_MOSI_PIN    

***************************************************************************************/


#include "stm32f4xx.h"                  // Device header

#include <string.h>
#include "delay.h"
#include "W5500.h"
#include "myspi.h"
#include "port_func.h"
#include "loopback.h"
#include "socket.h"
uint8_t buffer[2048];
uint8_t destip[4] = {192,168,1,190};
uint16_t Server_Port = 8888;
uint16_t Client_Port = 8000;

void Timer2_Init_Config(void);		//Timer2初始化配置
unsigned int W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)

void Led_set(void){
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
}

int main(void)
{
	Timer2_Init_Config();
	delay_init();
	
	SPI_Configuration();
	W5500_GPIO_Configuration();
	w5500_init();
	Led_set();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	delay_ms(1000);
	while (1)
	{
		loopback_tcps(0, buffer, Server_Port);
//		socket(0, Sn_MR_TCP, 8888, SF_TCP_NODELAY);
//		if(listen(0) != SOCK_OK){
//		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//		}
//		loopback_tcpc(0, buffer, destip, Client_Port);
	}
}



void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能Timer2时钟
	
	TIM_TimeBaseStructure.TIM_Period = 9;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到10为1ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;					//设置用来作为TIMx时钟频率除数的预分频值(10KHz的计数频率)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 				//使能TIM2指定的中断
	// NVIV优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	// 启动定时器
	TIM_Cmd(TIM2, ENABLE);  									//使能TIMx外设
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		W5500_Send_Delay_Counter++;
	}
}

