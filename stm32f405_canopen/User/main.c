#include "main.h"
//DEBUG_ERR_CONSOLE_ON
Message rxm = {0};
Message MES = {0x604,0,8,{0x40,0x60,0x60,0x00,0x00,0x00,0x00,0x00}};
uint8_t i = 1;
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
	tr_pdo_mapping();
	delay_ms(500);
	setState(&Master_Data, Operational);
	delay_ms(500);
	Profile_Velocity_Init(0x04);
	delay_ms(150);
	NMT_Control(0x01, 0x04);
	Driver_Enable(0x04);
////	Reset_Save(0x04, 1);
	while(1)
	{
		if(i)
		{
			delay_ms(1000);
//			canSend(CAN1, &MES);
			Profile_Velocity_Test(0x04);
		}
	}

	(void ) i;
	return 0;
}




