#include "main.h"
//DEBUG_ERR_CONSOLE_ON

//int i = 0, j = 0;
//uint8_t k = 0;
//uint8_t s = 0;
//motorID id = MOTOR1;
//uint16_t reg = 0x6083;
//uint16_t buf[4] = {0};
//int32_t velo = 0;
//int32_t dis[4] = {0};
uint8_t  crc8tab[] = 
{0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,
0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,
0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,
0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e};

int main(void)
{
//	int j = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//	外设初始化
	delay_init(168, delay_init_bsp);
	timer_init(TIM3_Init);
	timer_init(TIM4_Init);
	usart_init(115200,usart1_init_bsp);
	can_init(&Master_Data, can1_init_bsp);
//	agv_data_init();
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
//	usart_sendbuf(crc8tab, 64);
	while(1)
	{
		if(is_rcv_cmd())
		{
			analysis_cmd(get_rxbuf(), get_rxlen());
			clear_cmd();
		}
//		set_velocity_motor1(100);
//		set_velocity_motor2(100);
//		set_velocity_motor3(100);
//		set_velocity_motor4(100);
//		usart_sendbuf(crc8tab, sizeof(crc8tab));
//		delay_ms(5000);
//		printf("HE\n");
	}
	return 0;
}
