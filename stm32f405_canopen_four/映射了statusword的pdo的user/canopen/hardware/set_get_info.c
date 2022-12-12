#include "set_get_info.h"
extern uint8_t buf_temp[4];
extern uint8_t rpdo_flag[4];
extern int8_t write_flag;
extern FLASH_DATA AGV_PARAM;
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
		return 0xFFFF;
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
		return 0xFFFF;
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
		return 0xFFFF;
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


int8_t set_reg(motorID ID, uint16_t reg, uint8_t len, uint8_t* val_buf)
{
	Message mes = {0x600+ID,0,8,{0x2F,(reg & 0xFF),((reg  >> 8) & 0xFF),00,00,00,00,00}};
	int i = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
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
	while(write_flag == -2)
	{
		i++;
		if(i >= 0xFFF)
		return write_flag;
	}
	return write_flag;
}
int8_t motor_control(uint16_t* controlword, uint16_t *statusword, int32_t* velocity)
{
//	*controlword = 0x00;
//	delay_ms(10);
	uint16_t i = 0;
	*controlword = 0x06;
	*velocity = 0;
	delay_ms(10);
	*controlword = 0x07;
	delay_ms(10);
	*controlword = 0x0F;
	delay_ms(10);
	while(*statusword != 0x1027)
	{
		delay_ms(1);
		i++;
		if(i > 0xFF)
			return 0;
	}
	return 1;
}

int8_t motor_set(uint16_t *controlword, uint16_t *statusword, uint8_t cmd, uint8_t cmd_status)
{
	uint16_t i = 0;
	*controlword = cmd;
	while((*statusword & 0xFF) != cmd_status)
	{
		delay_ms(1);
		i++;
		if(i > 0xFF)
			return 0;
	}
	return 1;
}


int8_t isenable(motorID ID)
{
	uint16_t status = get_status(ID);
	int8_t ret = 0;
	switch(status & 0xFF)
	{
		case 0xFF:
			ret = -1;
			break;
		case 0x27:
			ret = 1;
			break;
		default:
			break;
	}
	return ret;

}

int8_t motor_enable(motorID ID)
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
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			ret = motor_control(&motor1_control, &motor1_status, &motor1_velocity);
			break;
		case MOTOR2:
			ret = motor_control(&motor2_control, &motor2_status, &motor2_velocity);
			break;
		case MOTOR3:
			ret = motor_control(&motor3_control, &motor3_status, &motor3_velocity);
			break;
		case MOTOR4:
			ret = motor_control(&motor4_control, &motor4_status, &motor4_velocity);
			break;
		default:
			break;
	}
	return ret;
	/*************************************************/
}


int8_t motor_disable(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			ret = motor_set(&motor1_control, &motor1_status,
						0x07, 0x23);
			break;
		case MOTOR2:
			ret = motor_set(&motor2_control, &motor2_status,
						0x07, 0x23);
			break;
		case MOTOR3:
			ret = motor_set(&motor3_control, &motor3_status,
						0x07, 0x23);
			break;
		case MOTOR4:
			ret = motor_set(&motor4_control, &motor4_status,
						0x07, 0x23);
			break;
		default:
			break;
	}
	return ret;
}

int8_t isfault(motorID ID)
{
	uint16_t status = get_status(ID);
	int8_t ret = 0;
	switch(status & 0xFF)
	{
		case 0xFF:
			ret = -1;
			break;
		case 0x80:
			ret = 1;
			break;
		default:
			break;
	}
	return ret;

}

int8_t clear_fault(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	if(isfault(ID) != 1)
		return -2;
	switch(ID)
	{
		case MOTOR1:
			ret = motor_set(&motor1_control, &motor1_status,
						0x80, 0x40);
			break;
		case MOTOR2:
			ret = motor_set(&motor2_control, &motor2_status,
						0x80, 0x40);
			break;
		case MOTOR3:
			ret = motor_set(&motor3_control, &motor3_status,
						0x80, 0x40);
			break;
		case MOTOR4:
			ret = motor_set(&motor4_control, &motor4_status,
						0x80, 0x40);
			break;
		default:
			break;
	}
	return ret;
}
int8_t quick_stop(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			ret = motor_set(&motor1_control, &motor1_status,
						0x02, 0x07);
			break;
		case MOTOR2:
			ret = motor_set(&motor2_control, &motor2_status,
						0x02, 0x07);
			break;
		case MOTOR3:
			ret = motor_set(&motor3_control, &motor3_status,
						0x02, 0x07);
			break;
		case MOTOR4:
			ret = motor_set(&motor4_control, &motor4_status,
						0x02, 0x07);
			break;
		default:
			break;
	}
	return ret;
}

int8_t quickstop_to_enable(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			if((motor1_status & 0xFF) == 0x07)
				ret = motor_set(&motor1_control, &motor1_status,
						0x0F, 0x27);
			break;
		case MOTOR2:
			if((motor2_status & 0xFF) == 0x07)
				ret = motor_set(&motor2_control, &motor2_status,
						0x0F, 0x27);
			break;
		case MOTOR3:
			if((motor3_status & 0xFF) == 0x07)
				ret = motor_set(&motor3_control, &motor3_status,
						0x0F, 0x27);
			break;
		case MOTOR4:
			if((motor4_status & 0xFF) == 0x07)
				ret = motor_set(&motor4_control, &motor4_status,
						0x0F, 0x27);
			break;
		default:
			break;
	}
	return ret;
}







//int8_t save_agvparam(motorID ID)
//{
//	for(int i = 0; i < 4; i++)
//	{
//		AGV_PARAM.odometer[i] = 
//	}
//}

uint16_t get_status(motorID ID)
{
	uint16_t status = 0;
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
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
	if(!IS_MOTOR_ID(ID))
		return -1;
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
	if(!IS_MOTOR_ID(ID))
		return -1;
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
	double temp = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			temp = get_count(MOTOR1) / 4096.0;
			rad = temp * 200 * PI;
			break;
		case MOTOR2:
			temp = get_count(MOTOR2) / 4096.0;
			rad = temp * 200 * PI;
			break;
		case MOTOR3:
			temp = get_count(MOTOR3) / 4096.0;
			rad = temp * 200 * PI;
			break;
		case MOTOR4:
			temp = get_count(MOTOR4) / 4096.0;
			rad = temp * 200 * PI;
			break;
		default:
			break;
	}
	return rad;
}

int32_t get_distance(motorID ID)
{
	int32_t meter = 0;
	double temp = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			temp = get_count(MOTOR1) / 4096.0;
			meter = temp * 20 * PI;
			break;
		case MOTOR2:
			temp = get_count(MOTOR2) / 4096.0;
			meter = temp * 20 * PI;
			break;
		case MOTOR3:
			temp = get_count(MOTOR3) / 4096.0;
			meter = temp * 20 * PI;
			break;
		case MOTOR4:
			temp = get_count(MOTOR4) / 4096.0;
			meter = temp * 20 * PI;
			break;
		default:
			break;
	}
	return meter;
}




uint16_t get_issave_rw(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2009);
}
uint16_t get_motor_temp(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2026);
}

uint16_t get_motor_status(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2027);
}

uint16_t get_hall_status(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2028);
}

uint16_t get_errorcode(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x603F);
}



int32_t get_actual_velocity(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return -1;
	return get_i32(ID, 0x606C);
}

uint16_t get_lock(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x200F);
}




uint16_t get_issave_rws(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2010);
}



uint16_t get_Vsmooth_factor(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2018);
}

uint16_t get_Eratio_gain(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2019);
}

uint16_t get_Eintegral_gain(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201A);
}

uint16_t get_feedforward_ratio(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201B);
}

uint16_t get_torque_ratio(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201C);
}

uint16_t get_VKp(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201D);
}

uint16_t get_VKi(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201E);
}

uint16_t get_Vfeedforward_Kf(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x201F);
}
uint16_t get_PKp(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2020);
}

uint16_t get_Pfeedforward_Kf(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u16(ID, 0x2021);
}





uint32_t get_accelerate_time(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u32(ID, 0x6083);
}

uint32_t get_decelerate_time(motorID ID)
{
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	return get_u32(ID, 0x6084);
}


int8_t set_issave_rw(motorID ID, uint16_t issave)
{
	uint8_t buf[2] = {0};
	if((issave != 1) && (issave != 2))
		return -3;
	buf[0] = issave & 0xFF;
	buf[1] = (issave >> 8)& 0xFF;

	return set_reg(ID, 0x2009, 2, buf);
}

int8_t set_lock(motorID ID, uint16_t lock)
{
	uint8_t buf[2] = {0};
	if(lock > 30000)
		return -3;
	buf[0] = lock & 0xFF;
	buf[1] = (lock >> 8)& 0xFF;

	return set_reg(ID, 0x200F, 2, buf);
}

int8_t set_issave_rws(motorID ID, uint16_t issave)
{
	uint8_t buf[2] = {0};
	if((issave != 0) && (issave != 1))
		return -3;
	buf[0] = issave & 0xFF;
	buf[1] = (issave >> 8)& 0xFF;

	return set_reg(ID, 0x2010, 2, buf);
}

int8_t set_accelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	if(time > 32767)
		return -3;
	buf[0] = time & 0xFF;
	buf[1] = (time >> 8)& 0xFF;
	buf[2] = (time >> 16) & 0xFF;
	buf[3] = (time >> 24) & 0xFF;
	return set_reg(ID, 0x6083, 4, buf);
}

int8_t set_decelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	if(time > 32767)
		return -3;
	buf[0] = time & 0xFF;
	buf[1] = (time >> 8)& 0xFF;
	buf[2] = (time >> 16) & 0xFF;
	buf[3] = (time >> 24) & 0xFF;
	return set_reg(ID, 0x6084, 4, buf);
}

int8_t set_Vsmooth_factor(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x2018, 2, buf);
}

int8_t set_Eratio_gain(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x2019, 2, buf);
}
int8_t set_Eintegral_gain(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201A, 2, buf);
}
int8_t set_feedforward_ratio(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201B, 2, buf);
}
int8_t set_torque_ratio(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201C, 2, buf);
}
int8_t set_VKp(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201D, 2, buf);
}
int8_t set_VKi(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201E, 2, buf);
}
int8_t set_Vfeedforward_Kf(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x201F, 2, buf);
}

int8_t set_PKp(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x2020, 2, buf);
}

int8_t set_Pfeedforward_Kf(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	buf[0] = factor & 0xFF;
	buf[1] = (factor >> 8)& 0xFF;

	return set_reg(ID, 0x2021, 2, buf);
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
