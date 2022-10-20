#include "main.h"
//DEBUG_ERR_CONSOLE_ON
Message rxm = {0};

int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	TIM3_Init();
	USART1_Init(115200);
	CAN1_Init(&Master_Data);
	unsigned char nodeID = 0xFE; 
	setNodeId(&Master_Data, nodeID);
	setState(&Master_Data, Initialisation);
	delay_ms(2000);
//	heartbeat_timeset();
//	delay_ms(500);
//	tr_pdo_mapping();
//	delay_ms(500);
	setState(&Master_Data, Operational);
	delay_ms(500);
	motor1_control = 0x0F;
	motor2_control = 0x0F;
	motor3_control = 0x0F;
	motor4_control = 0x0F;
	motor1_mode = 3;
	motor2_mode = 3;
	motor3_mode = 3;
	motor4_mode = 3;
	Profile_Velocity_Init(MOTOR1);
	Profile_Velocity_Init(MOTOR2);
	Profile_Velocity_Init(MOTOR3);
	Profile_Velocity_Init(MOTOR4);
	delay_ms(150);
	NMT_Control(0x01, 0x00);
	Driver_Enable(MOTOR1);
	Driver_Enable(MOTOR2);
	Driver_Enable(MOTOR3);
	Driver_Enable(MOTOR4);
//	NMT_Control(0x01, 0x00);
//	motor1_mode = 3;
//	motor2_mode = 3;
//	motor3_mode = 3;
//	motor4_mode = 3;
//	motor1_control = 0x0F;
//	motor2_control = 0x0F;
//	motor3_control = 0x0F;
//	motor4_control = 0x0F;
//	Reset_Save(0x01, 2);
//	Reset_Save(0x02, 2);
//	Reset_Save(0x03, 2);
//	Reset_Save(0x04, 2);
	while(1)
	{
		printf("%d-%d-%d-%d\n",motor1_position,motor2_position,motor3_position,motor4_position);
		motor1_velocity = 100;
		motor2_velocity = 100;
		motor3_velocity = 100;
		motor4_velocity = 100;
		delay_ms(2000);

		motor1_velocity = -200;
		motor2_velocity = -200;
		motor3_velocity = -200;
		motor4_velocity = -200;
		delay_ms(2000);

	}

	return 0;
}
