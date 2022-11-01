#include "main.h"
//DEBUG_ERR_CONSOLE_ON

int i = 0, j = 0;
uint32_t s = 0;
motorID id = MOTOR1;
uint16_t reg = 0x6083;
uint16_t buf[4] = {0};
int32_t velo = 0;
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
	delay_ms(150);
	//	使能电机
	motor_enable(MOTOR1);
	motor_enable(MOTOR2);
	motor_enable(MOTOR3);
	motor_enable(MOTOR4);
//	Reset_Save(0x01, 2);
//	Reset_Save(0x02, 2);
//	Reset_Save(0x03, 2);
//	Reset_Save(0x04, 2);
	while(1)
	{
		buf[0] = get_motor_temp(id);
		buf[1] = get_motor_status(id);
		buf[2] = get_hall_status(id);
		buf[3] = get_errorcode(id);

		printf("%d&&&&%d&&&&%d&&&&%d\n",get_count(MOTOR1),get_count(MOTOR2),
					get_count(MOTOR3),get_count(MOTOR4));
		printf("%d&&&&%d&&&&%d&&&&%d\n",get_rad(MOTOR1),get_rad(MOTOR2),
					get_rad(MOTOR3),get_rad(MOTOR4));
		printf("%d&&&&%d&&&&%d&&&&%d\n",get_distance(MOTOR1),get_distance(MOTOR2),
					get_distance(MOTOR3),get_distance(MOTOR4));
		
		if(i == 1)
		{
			set_velocity_motor1(velo);
			set_velocity_motor2(velo);
			set_velocity_motor3(velo);
			set_velocity_motor4(velo);
			delay_ms(2000);
			if(j == 1)
			{
				quickstop_to_enable(MOTOR1);
				quickstop_to_enable(MOTOR2);
				quickstop_to_enable(MOTOR3);
				quickstop_to_enable(MOTOR4);
				j = 0;
			}
			if(j == 3)
			{
				motor_enable(MOTOR1);
				motor_enable(MOTOR2);
				motor_enable(MOTOR3);
				motor_enable(MOTOR4);
				j = 0;
			}
			delay_ms(2000);
//			set_velocity_motor1(0);
//			set_velocity_motor2(0);
//			set_velocity_motor3(0);
//			set_velocity_motor4(0);
//			delay_ms(2000);

		}
		if(i == 2)
		{
			set_velocity_motor1(-200);
			set_velocity_motor2(-200);
			set_velocity_motor3(-200);
			set_velocity_motor4(-200);
			delay_ms(2000);
			velo = get_actual_velocity(id);
			if(j == 2)
			{
				quick_stop(MOTOR1);
				quick_stop(MOTOR2);
				quick_stop(MOTOR3);
				quick_stop(MOTOR4);
				j = 0;
			}
			if(j == 4)
			{
				motor_disable(MOTOR1);
				motor_disable(MOTOR2);
				motor_disable(MOTOR3);
				motor_disable(MOTOR4);
				j = 0;
			}
			delay_ms(2000);
			set_velocity_motor1(0);
			set_velocity_motor2(0);
			set_velocity_motor3(0);
			set_velocity_motor4(0);
			delay_ms(2000);
		}
		

	}
	return 0;
}
