#include "getINFO.h"
extern uint8_t buf_temp[4];
extern uint8_t rpdo_flag[4];

uint16_t get_reg_info(uint8_t id, uint16_t reg)
{
	uint16_t i = 0;
	uint16_t errorcode = 0;
	Message mes = {0x600+id,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	if(!rpdo_flag[id-1])
	{
		canSend(CAN1, &mes);
		while(rpdo_flag[id-1] == 0)
		{
			i++;
			if(i >= 0xFFF)
			return 0xFFFF;
		}
	}
	errorcode = buf_temp[1] << 8 | buf_temp[0];
	buf_temp[0] = 0;
	buf_temp[1] = 0;
	rpdo_flag[id-1] = 0;
	return errorcode;
}

uint16_t get_status(uint8_t ID)
{
	uint16_t status = 0;
	switch(ID)
	{
		case MOTOR1:
			status = motor1_status;
			break;
		case MOTOR2:
			status = motor2_status;
			break;
		case MOTOR3:
			status = motor3_status;
			break;
		case MOTOR4:
			status = motor4_status;
			break;
		default:
			break;
	}
	return status;
}

int8_t get_mode(uint8_t ID)
{
	int8_t mode = 0;
	switch(ID)
	{
		case MOTOR1:
			mode = motor1_mode_display;
			break;
		case MOTOR2:
			mode = motor2_mode_display;
			break;
		case MOTOR3:
			mode = motor3_mode_display;
			break;
		case MOTOR4:
			mode = motor4_mode_display;
			break;
		default:
			break;
	}
	return mode;
}
uint16_t get_position(uint8_t ID)
{
	int32_t position = 0;
	switch(ID)
	{
		case MOTOR1:
			position = motor1_position;
			break;
		case MOTOR2:
			position = motor2_position;
			break;
		case MOTOR3:
			position = motor3_position;
			break;
		case MOTOR4:
			position = motor4_position;
			break;
		default:
			break;
	}
	return position;
}
