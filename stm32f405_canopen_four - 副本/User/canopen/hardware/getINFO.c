#include "getINFO.h"
extern uint8_t status_1[7];
extern uint8_t status_2[7];
extern uint8_t status_3[7];
extern uint8_t status_4[7];

//boolean iswarning(uint8_t id)
//{
//	Message mes = {0x600+id,0,8,{40,3F,60,00,00,00,00,00}}
//	
//	if()
//}

void read_pdoinfo(uint8_t id, uint16_t reg, int32_t* ret)
{
	switch(id)
	{
		case 1:{
			switch(reg)
			{
				case MOTOR_STATUS:
					*ret = (status_1[1] << 8) + status_1[0];
					break;
				case MOTOR_MODE:
					*ret = status_1[2];
					break;
				case MOTOR_ACTUAL_POSITION:
					*ret = (status_1[6] << 24) + (status_1[5] << 16) +
							(status_1[4] << 8) + status_1[3];
					break;
				default:
					break;
			}

			break;
		}
		case 2:{
			switch(reg)
			{
				case MOTOR_STATUS:
					*ret = (status_2[1] << 8) + status_2[0];
					break;
				case MOTOR_MODE:
					*ret = status_2[2];
					break;
				case MOTOR_ACTUAL_POSITION:
					*ret = (status_2[6] << 24) + (status_2[5] << 16) +
							(status_2[4] << 8) + status_2[3];
					break;
				default:
					break;
			}
			break;
		}
		case 3:{
			switch(reg)
			{
				case MOTOR_STATUS:
					*ret = (status_3[1] << 8) + status_3[0];
					break;
				case MOTOR_MODE:
					*ret = status_1[2];
					break;
				case MOTOR_ACTUAL_POSITION:
					*ret = (status_3[6] << 24) + (status_3[5] << 16) +
							(status_3[4] << 8) + status_3[3];
					break;
				default:
					break;
			}
			break;
		}
		case 4:{
			switch(reg)
			{
				case MOTOR_STATUS:
					*ret = (status_4[1] << 8) + status_4[0];
					break;
				case MOTOR_MODE:
					*ret = status_4[2];
					break;
				case MOTOR_ACTUAL_POSITION:
					*ret = (status_4[6] << 24) + (status_4[5] << 16) +
							(status_4[4] << 8) + status_4[3];
					break;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}


