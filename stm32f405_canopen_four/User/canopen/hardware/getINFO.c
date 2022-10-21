#include "getINFO.h"
extern uint8_t buf_temp[4];
extern uint8_t rpdo_flag[4];
extern uint8_t write_flag;

int8_t get_i8(uint8_t ID, uint16_t reg)
{
	uint16_t i = 0;
	int8_t reg_info = 0; 
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	while(rpdo_flag[ID-1] == 0)
	{
		i++;
		if(i >= 0xFFF)
		return 0xFF;
	}
	reg_info = buf_temp[0];
	buf_temp[0] = 0;
	rpdo_flag[ID-1] = 0;
	return reg_info;
}
int16_t get_i16(uint8_t ID, uint16_t reg)
{
	uint16_t i = 0;
	int16_t reg_info = 0; 
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	while(rpdo_flag[ID-1] == 0)
	{
		i++;
		if(i >= 0xFFF)
		return 0xFF;
	}
	reg_info = (buf_temp[1] << 8) | buf_temp[0];
	buf_temp[0] = 0;
	buf_temp[1] = 0;
	rpdo_flag[ID-1] = 0;
	return reg_info;
}
uint16_t get_u16(uint8_t ID, uint16_t reg)
{
	uint16_t i = 0;
	uint16_t reg_info = 0; 
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	while(rpdo_flag[ID-1] == 0)
	{
		i++;
		if(i >= 0xFFF)
		return 0xFF;
	}
	reg_info = (buf_temp[1] << 8) | buf_temp[0];
	buf_temp[0] = 0;
	buf_temp[1] = 0;
	rpdo_flag[ID-1] = 0;
	return reg_info;
}
int32_t get_i32(uint8_t ID, uint16_t reg)
{
	uint16_t i = 0;
	int32_t reg_info = 0; 
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	while(rpdo_flag[ID-1] == 0)
	{
		i++;
		if(i >= 0xFFF)
		return 0xFF;
	}
	reg_info = (buf_temp[3] << 24) | (buf_temp[2] << 16)
					| (buf_temp[1] << 8) | buf_temp[0];
	buf_temp[0] = 0;
	buf_temp[1] = 0;
	buf_temp[2] = 0;
	buf_temp[3] = 0;
	rpdo_flag[ID-1] = 0;
	return reg_info;
}
uint32_t get_u32(uint8_t ID, uint16_t reg)
{
	uint16_t i = 0;
	uint32_t reg_info = 0; 
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	while(rpdo_flag[ID-1] == 0)
	{
		i++;
		if(i >= 0xFFF)
		return 0xFF;
	}
	reg_info = (buf_temp[3] << 24) | (buf_temp[2] << 16)
					| (buf_temp[1] << 8) | buf_temp[0];
	buf_temp[0] = 0;
	buf_temp[1] = 0;
	buf_temp[2] = 0;
	buf_temp[3] = 0;
	rpdo_flag[ID-1] = 0;
	return reg_info;
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

uint8_t set_reg(uint8_t ID, uint16_t reg, uint8_t len, uint8_t* val_buf)
{
	Message mes = {0x600+ID,0,8,{0x2F,(reg & 0xFF),((reg  >> 8) & 0xFF),00,00,00,00,00}};
	int i = 0;
	switch(len)
	{
		case 2:
			mes.data[0] = 0x2B;
			break;
		case 3:
			mes.data[0] = 0x27;
			break;
		case 4:
			mes.data[0] = 0x23;
			break;
		default:
			break;
	}
	for(i = 0; i < len; i++)
	{
		mes.data[i+4] = val_buf[i];
	}
	canSend(CAN1, &mes);
	while(write_flag == 0xFF)
	{
		i++;
		if(i >= 0xFFF)
		return write_flag;
	}
	return write_flag;
}


