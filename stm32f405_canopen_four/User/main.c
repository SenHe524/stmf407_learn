#include "main.h"
//DEBUG_ERR_CONSOLE_ON


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//	延时初始化
	delay_init(168, delay_init_bsp);
	//	canopen主站定时器初始化
	timer_init(TIM3_Init);
	//	里程计上传定时器初始化
//	timer_init(TIM4_Init);
	//	与上位机通信的串口1初始化
	usart_init(115200,usart1_init_bsp);
	//	与IMU通信的串口6初始化
	usart_init(460800,usart6_init_bsp);
	//	初始化can外设
	can_init(&Master_Data, can1_init_bsp);
	//	转换总线状态
	unsigned char nodeID = 0x5; 
	setNodeId(&Master_Data, nodeID);
	setState(&Master_Data, Initialisation);
	//	延时以等待从站上线报文
	delay_ms(2000);
//	tr_pdo_mapping();
//	delay_ms(500);
	setState(&Master_Data, Operational);
	//	延时保证从站状态切换
	delay_ms(500);
	//	开启四个节点pdo通信
	NMT_Control(0x01, 0x00);
	//	设置从站的运动模式为速度模式
	set_mode(MOTOR1, 3);
	set_mode(MOTOR2, 3);
	set_mode(MOTOR3, 3);
	set_mode(MOTOR4, 3);
	delay_ms(150);
	//	使能电机
	motor_enable(MOTOR1);
	motor_enable(MOTOR2);
	motor_enable(MOTOR3);
	motor_enable(MOTOR4);
//	set_issave_rws(MOTOR1, 0);
//	set_issave_rws(MOTOR2, 0);
//	set_issave_rws(MOTOR3, 0);
//	set_issave_rws(MOTOR4, 0);
	while(1)
	{
		if(is_rcv_usart1cmd())
		{
			usart1_analysis_cmd(get_usart1rxbuf(), get_usart1rxlen());
			clear_usart1cmd();
		}
		if(is_rcv_usart6cmd())
		{
			usart6_analysis_cmd(get_usart6rxbuf(), get_usart6rxlen());
			clear_usart6cmd();
		}
	}
	return 0;
}
