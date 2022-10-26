#include "set_get_info.h"
extern uint8_t buf_temp[4];
extern uint8_t rpdo_flag[4];
extern uint8_t write_flag;

int8_t get_i8(motorID ID, uint16_t reg)
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
int16_t get_i16(motorID ID, uint16_t reg)
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
uint16_t get_u16(motorID ID, uint16_t reg)
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
int32_t get_i32(motorID ID, uint16_t reg)
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
uint32_t get_u32(motorID ID, uint16_t reg)
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


uint8_t set_reg(motorID ID, uint16_t reg, uint8_t len, uint8_t* val_buf)
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


void motor_control(uint16_t* controlword, int32_t* velocity)
{
//	*controlword = 0x00;
//	delay_ms(10);
	*controlword = 0x06;
	*velocity = 0;
	delay_ms(10);
	*controlword = 0x07;
	delay_ms(10);
	*controlword = 0x0F;
	delay_ms(10);
}


void motor_enable(motorID ID)
{
	//*************************SDO****************************
//	Message mes = {0x600+ID,0,8,{0x23,0xFF,0x60,0x00,0x00,0x00,0x00,0x00}};
//	canSend(CAN1, &mes);
//	delay_ms(5);
//	mes.data[0] = 0x2B;
//	mes.data[1] = 0x40;
//	mes.data[4] = 0x06;
//	canSend(CAN1, &mes);
//	delay_ms(5);
//	mes.data[4] = 0x07;
//	canSend(CAN1, &mes);
//	delay_ms(5);
//	mes.data[4] = 0x0F;

//	canSend(CAN1, &mes);
	/*****************************************************/

	/**********************PDO*************************/
	switch(ID)
	{
		case MOTOR1:
			motor_control(&motor1_control, &motor1_velocity);
			break;
		case MOTOR2:
			motor_control(&motor2_control, &motor2_velocity);
			break;
		case MOTOR3:
			motor_control(&motor3_control, &motor3_velocity);
			break;
		case MOTOR4:
			motor_control(&motor4_control, &motor4_velocity);
			break;
		default:
			break;
	}
	/*************************************************/
}

void motor_disable(motorID ID)
{
	switch(ID)
	{
		case MOTOR1:
			motor1_control = 0x00;
			break;
		case MOTOR2:
			motor2_control = 0x00;
			break;
		case MOTOR3:
			motor3_control = 0x00;
			break;
		case MOTOR4:
			motor4_control = 0x00;
			break;
		default:
			break;
	}
}


uint16_t get_status(motorID ID)
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

int8_t get_mode(motorID ID)
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
int32_t get_count(motorID ID)
{
	int32_t position = 0;
	switch(ID)
	{
		case MOTOR1:
			position = motor1_position;
			position = -position;
			break;
		case MOTOR2:
			position = motor2_position;
			break;
		case MOTOR3:
			position = motor3_position;
			position = -position;
			break;
		case MOTOR4:
			position = motor4_position;
			break;
		default:
			break;
	}
	return position;
}

int32_t get_rad(motorID ID)
{
	int32_t rad = 0;
	switch(ID)
	{
		case MOTOR1:
			rad = get_count(MOTOR1) * 200 * PI / 4096;
			break;
		case MOTOR2:
			rad = get_count(MOTOR2) * 200 * PI / 4096;
			break;
		case MOTOR3:
			rad = get_count(MOTOR3) * 200 * PI / 4096;
			break;
		case MOTOR4:
			rad = get_count(MOTOR4) * 200 * PI / 4096;
			break;
		default:
			break;
	}
	return rad;
}

int32_t get_meter(motorID ID)
{
	int32_t meter = 0;
	switch(ID)
	{
		case MOTOR1:
			meter = get_count(MOTOR1) * 40 * PI / 4096;
			break;
		case MOTOR2:
			meter = get_count(MOTOR2) * 40 * PI / 4096;
			break;
		case MOTOR3:
			meter = get_count(MOTOR3) * 40 * PI / 4096;
			break;
		case MOTOR4:
			meter = get_count(MOTOR4) * 40 * PI / 4096;
			break;
		default:
			break;
	}
	return meter;
}


uint16_t get_motor_temp(motorID ID)
{
	return get_u16(ID, 0x2026);
}

uint16_t get_motor_status(motorID ID)
{
	return get_u16(ID, 0x2027);
}

uint16_t get_hall_status(motorID ID)
{
	return get_u16(ID, 0x2028);
}

uint16_t get_errorcode(motorID ID)
{
	return get_u16(ID, 0x603F);
}

int32_t get_actual_velocity(motorID ID)
{
	return get_i32(ID, 0x606C);
}

uint8_t set_accelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	buf[0] = time & 0xFF;
	buf[1] = (time >> 8)& 0xFF;
	buf[2] = (time >> 16) & 0xFF;
	buf[3] = (time >> 24) & 0xFF;
	return set_reg(ID, 0x6083, 4, buf);
}

uint8_t set_decelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	buf[0] = time & 0xFF;
	buf[1] = (time >> 8)& 0xFF;
	buf[2] = (time >> 16) & 0xFF;
	buf[3] = (time >> 24) & 0xFF;
	return set_reg(ID, 0x6084, 4, buf);
}

void clear_fault(motorID ID)
{
	switch(ID)
	{
		case MOTOR1:
			motor1_control = 0x80;
			break;
		case MOTOR2:
			motor2_control = 0x80;
			break;
		case MOTOR3:
			motor3_control = 0x80;
			break;
		case MOTOR4:
			motor4_control = 0x80;
			break;
		default:
			break;
	}
}

void quick_stop(motorID ID)
{
	switch(ID)
	{
		case MOTOR1:
			motor1_control = 0x02;
			break;
		case MOTOR2:
			motor2_control = 0x02;
			break;
		case MOTOR3:
			motor3_control = 0x02;
			break;
		case MOTOR4:
			motor4_control = 0x02;
			break;
		default:
			break;
	}
}

void quickstop_to_enable(motorID ID)
{
	switch(ID)
	{
		case MOTOR1:
			if((motor1_status & 0xFF) == 0x07)
				motor1_control = 0x0F;
			break;
		case MOTOR2:
			if((motor2_status & 0xFF) == 0x07)
				motor2_control = 0x0F;
			break;
		case MOTOR3:
			if((motor3_status & 0xFF) == 0x07)
				motor3_control = 0x0F;
			break;
		case MOTOR4:
			if((motor4_status & 0xFF) == 0x07)
				motor4_control = 0x0F;
			break;
		default:
			break;
	}
}

void set_velocity_motor1(int32_t velocity)
{
	motor1_velocity = -velocity;
}

void set_velocity_motor2(int32_t velocity)
{
	motor2_velocity = velocity;
}
void set_velocity_motor3(int32_t velocity)
{
	motor3_velocity = -velocity;
}
void set_velocity_motor4(int32_t velocity)
{
	motor4_velocity = velocity;
}
