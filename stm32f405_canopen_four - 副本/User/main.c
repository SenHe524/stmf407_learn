#include "main.h"
//DEBUG_ERR_CONSOLE_ON
Message rxm = {0};
uint8_t i = 1;
int32_t posi_1 = 0;
int32_t posi_2 = 0;
int32_t posi_3 = 0;
int32_t posi_4 = 0;
int32_t posi1 = 0;
int32_t posi2 = 0;
int32_t posi3 = 0;
int32_t posi4 = 0;
Message mes_1 = {0x200+1,0,7,{0x0F,0x00,0x03,0xD4,0xFE,0xFF,0xFF}};
Message mes_2 = {0x300+2,0,7,{0x0F,0x00,0x03,0xD4,0xFE,0xFF,0xFF}};
Message mes_3 = {0x400+3,0,7,{0x0F,0x00,0x03,0xD4,0xFE,0xFF,0xFF}};
Message mes_4 = {0x500+4,0,7,{0x0F,0x00,0x03,0xD4,0xFE,0xFF,0xFF}};
int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	TIM3_Init();
	USART1_Init(115200);
	CAN1_Init(&Master_Data);
	unsigned char nodeID = 0x05; 
	setNodeId(&Master_Data, nodeID);
	setState(&Master_Data, Initialisation);
	delay_ms(2000);
//	heartbeat_timeset();
//	delay_ms(500);
//	tr_pdo_mapping();
//	delay_ms(500);
	setState(&Master_Data, Operational);
	delay_ms(500);
	Profile_Velocity_Init(0x01);
	Profile_Velocity_Init(0x02);
	Profile_Velocity_Init(0x03);
	Profile_Velocity_Init(0x04);
	delay_ms(150);
	NMT_Control(0x01, 0x00);
	Driver_Enable(0x01);
	Driver_Enable(0x02);
	Driver_Enable(0x03);
	Driver_Enable(0x04);
//	Reset_Save(0x01, 2);
//	Reset_Save(0x02, 2);
//	Reset_Save(0x03, 2);
//	Reset_Save(0x04, 2);
	while(1)
	{
		if(i)
		{
//			Profile_Velocity_Test();
			
			read_pdoinfo(1, MOTOR_ACTUAL_POSITION, &posi_1);
			read_pdoinfo(2, MOTOR_ACTUAL_POSITION, &posi_2);
			read_pdoinfo(3, MOTOR_ACTUAL_POSITION, &posi_3);
			read_pdoinfo(4, MOTOR_ACTUAL_POSITION, &posi_4);	
			printf("%d-%d-%d-%d\n",posi_1,posi_2,posi_3,posi_4);
			delay_ms(1000);
			mes_1.data[3] = 0x9C;
			mes_1.data[4] = 0xFF;
			mes_1.data[5] = 0xFF;
			mes_1.data[6] = 0xFF;

			mes_2.data[3] = 0x9C;
			mes_2.data[4] = 0xFF;
			mes_2.data[5] = 0xFF;
			mes_2.data[6] = 0xFF;

			mes_3.data[3] = 0x9C;
			mes_3.data[4] = 0xFF;
			mes_3.data[5] = 0xFF;
			mes_3.data[6] = 0xFF;

			mes_4.data[3] = 0x9C;
			mes_4.data[4] = 0xFF;
			mes_4.data[5] = 0xFF;
			mes_4.data[6] = 0xFF;
			canSend(CAN1, &mes_1);
			delay_ms(5);
			canSend(CAN1, &mes_2);
			delay_ms(5);
			canSend(CAN1, &mes_3);
			delay_ms(5);
			canSend(CAN1, &mes_4);
			delay_ms(5000);
		
			mes_1.data[3] = 0x00;
			mes_1.data[4] = 0x00;
			mes_1.data[5] = 0x00;
			mes_1.data[6] = 0x00;

			mes_2.data[3] = 0x00;
			mes_2.data[4] = 0x00;
			mes_2.data[5] = 0x00;
			mes_2.data[6] = 0x00;

			mes_3.data[3] = 0x00;
			mes_3.data[4] = 0x00;
			mes_3.data[5] = 0x00;
			mes_3.data[6] = 0x00;

			mes_4.data[3] = 0x00;
			mes_4.data[4] = 0x00;
			mes_4.data[5] = 0x00;
			mes_4.data[6] = 0x00;
			canSend(CAN1, &mes_1);
			delay_ms(5);
			canSend(CAN1, &mes_2);
			delay_ms(5);
			canSend(CAN1, &mes_3);
			delay_ms(5);
			canSend(CAN1, &mes_4);
			delay_ms(500);
			read_pdoinfo(1, MOTOR_ACTUAL_POSITION, &posi1);
			read_pdoinfo(2, MOTOR_ACTUAL_POSITION, &posi2);
			read_pdoinfo(3, MOTOR_ACTUAL_POSITION, &posi3);
			read_pdoinfo(4, MOTOR_ACTUAL_POSITION, &posi4);
			printf("%d-%d-%d-%d\n",posi_1,posi_2,posi_3,posi_4);
		}
	}

	(void ) i;
	return 0;
}
