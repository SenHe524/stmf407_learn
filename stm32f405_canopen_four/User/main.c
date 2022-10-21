#include "main.h"
//DEBUG_ERR_CONSOLE_ON

int i = 0;
//uint16_t reg = 0x6083;
//uint8_t len = 4;
//uint8_t buf[4] = {0};
int main(void)
{
//	int j = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//	外设初始化
	delay_init(168);
	TIM3_Init();
	USART1_Init(115200);
	CAN1_Init(&Master_Data);
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
	motor1_control = 0x06;
	motor2_control = 0x06;
	motor3_control = 0x06;
	motor4_control = 0x06;
	//	设置从站的运动模式为速度模式
	motor1_mode = 3;
	motor2_mode = 3;
	motor3_mode = 3;
	motor4_mode = 3;
	//	速度模式下的加减速时间配置
	Profile_Velocity_Init(MOTOR1);
	Profile_Velocity_Init(MOTOR2);
	Profile_Velocity_Init(MOTOR3);
	Profile_Velocity_Init(MOTOR4);
	delay_ms(150);
	//	使能电机
	Driver_Enable(MOTOR1);
	Driver_Enable(MOTOR2);
	Driver_Enable(MOTOR3);
	Driver_Enable(MOTOR4);
//	Reset_Save(0x01, 2);
//	Reset_Save(0x02, 2);
//	Reset_Save(0x03, 2);
//	Reset_Save(0x04, 2);
	while(1)
	{
//		for(j = 0; j < 4; j++)
//		{
//			printf("%d----", set_reg(j+1, reg, len, buf));
//		}
		printf("%d-%d-%d-%d\n",motor1_position,motor2_position,motor3_position,motor4_position);
		if(i == 1)
		{
			motor1_velocity = 100;
			motor2_velocity = 100;
			motor3_velocity = 100;
			motor4_velocity = 100;
			delay_ms(2000);
			motor1_velocity = 0;
			motor2_velocity = 0;
			motor3_velocity = 0;
			motor4_velocity = 0;
			delay_ms(2000);
		}
		
		if(i == 2)
		{
			motor1_velocity = -200;
			motor2_velocity = -200;
			motor3_velocity = -200;
			motor4_velocity = -200;
			delay_ms(2000);
			motor1_velocity = 0;
			motor2_velocity = 0;
			motor3_velocity = 0;
			motor4_velocity = 0;
			delay_ms(2000);
		}

	}

	return 0;
}
