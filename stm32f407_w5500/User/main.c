/***************************************************************************************

 * 硬件连接：  PF11 -> W5500_SPI3_RST   
 *             PF12 -> W5500_SPI3_INT     
 *             PF13 -> W5500_SPI3_SCS      
 *             PC10 -> W5500_SPI3_SCK_PIN    
 *             PC11 -> W5500_SPI3_MISO_PIN    
 *             PC12 -> W5500_SPI3_MOSI_PIN    

***************************************************************************************/


#include "stm32f4xx.h"                  // Device header

#include "W5500.h"			
#include <string.h>
#include "delay.h"
void Timer2_Init_Config(void);		//Timer2初始化配置
void System_Initialization(void);	//STM32系统初始化函数(初始化STM32时钟及外设)
unsigned int W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)

/*******************************************************************************
* 函数名  : W5500_Initialization
* 描述    : W5500初始货配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void W5500_Initialization(void)
{
	W5500_Init();		//初始化W5500寄存器函数
//	Detect_Gateway();	//检查网关服务器
//	Socket_Init(0);		//指定Socket(0~7)初始化,初始化端口0
}

/*******************************************************************************
* 函数名  : W5500_Socket_Set
* 描述    : W5500端口初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
*			从端口状态字节Socket_State可以判断端口的工作情况
*******************************************************************************/
void W5500_Socket_Set(void)
{
	if(S0_State==0)//端口0初始化配置
	{
		if(S0_Mode==TCP_SERVER)//TCP服务器模式 
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP客户端模式 
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else//UDP模式 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}

/*******************************************************************************
* 函数名  : Process_Socket_Data
* 描述    : W5500接收并发送接收到的数据
* 输入    : s:端口号
* 输出    : 无
* 返回值  : 无
* 说明    : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
*			然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
*			处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
*			发送数据。
*******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
	unsigned short size;
	size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
	memcpy(Tx_Buffer, Rx_Buffer, size);			
	Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
}

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
/*******************************************************************************
* 函数名  : main
* 描述    : 主函数，用户程序从main函数开始运行
* 输入    : 无
* 输出    : 无
* 返回值  : int:返回值为一个16位整形数
* 说明    : 无
*******************************************************************************/
int main(void)
{
	Timer2_Init_Config();		//Timer2初始化配置
	delay_init();
	
	SPI_Configuration();		//W5500 SPI初始化配置(STM32 SPI3)
	W5500_GPIO_Configuration();	//W5500 GPIO初始化配置	
	W5500_Hardware_Reset();		//硬件复位W5500
	W5500_Initialization();		//W5500初始化配置
	Led_set();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	while (1)
	{
//		W5500_Socket_Set();//W5500端口初始化配置

//		if(W5500_Interrupt)//处理W5500中断		
//		{
//			W5500_Interrupt_Process();//W5500中断处理程序框架
//			GPIO_SetBits(GPIOA,GPIO_Pin_0);
//		}
//		if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
//		{
//			S0_Data&=~S_RECEIVE;
//			Process_Socket_Data(0);//W5500接收并发送接收到的数据
//			GPIO_SetBits(GPIOA,GPIO_Pin_0);
//		}
//		 else if(W5500_Send_Delay_Counter >= 500)//定时发送字符串
//		{
//			if(S0_State == (S_INIT|S_CONN))
//			{
//				S0_Data&=~S_TRANSMITOK;
//				memcpy(Tx_Buffer, "\r\nWelcome To NiRenElec!\r\n", 23);	
//				Write_SOCK_Data_Buffer(0, Tx_Buffer, 23);//指定Socket(0~7)发送数据处理,端口0发送23字节数据
//			}
//			W5500_Send_Delay_Counter=0;
//			GPIO_SetBits(GPIOA,GPIO_Pin_0);
//			delay_ms(50);
//		}
//		else{
//			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//		}
	}
}




/*******************************************************************************
* 函数名  : Timer2_Init_Config
* 描述    : Timer2初始化配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
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

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		W5500_Send_Delay_Counter++;
	}
}

