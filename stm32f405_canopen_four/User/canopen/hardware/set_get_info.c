#include "set_get_info.h"
extern uint8_t buf_temp[4][4];
extern int8_t write_flag[4];
int8_t get_i8(motorID ID, uint16_t reg)
{
	int8_t reg_info = 0;
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	buf_temp[ID-1][0] = 0xFE;
	canSend(CAN1, &mes);
	delay_ms(1);
	reg_info = buf_temp[ID-1][0];
	return reg_info;
}
int16_t get_i16(motorID ID, uint16_t reg)
{
	int16_t reg_info = 0;
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	*(int16_t*)buf_temp[ID-1] = 0xFFFE;
	canSend(CAN1, &mes);
	delay_ms(1);
	reg_info = *(int16_t*)buf_temp[ID-1];
	return reg_info;
}
uint16_t get_u16(motorID ID, uint16_t reg)
{
	uint16_t reg_info = 0;
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	*(uint16_t*)buf_temp[ID-1] = 0xFFFE;
	canSend(CAN1, &mes);
	delay_ms(1);
	reg_info = *(uint16_t*)buf_temp[ID-1];
	return reg_info;
}
int32_t get_i32(motorID ID, uint16_t reg)
{
	int32_t reg_info = 0;
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	*(int32_t*)buf_temp[ID-1] = 0xFFFFFFFE;
	canSend(CAN1, &mes);
	delay_ms(1);
	reg_info = *(int32_t*)buf_temp[ID-1];
	return reg_info;
}
uint32_t get_u32(motorID ID, uint16_t reg)
{
	uint32_t reg_info = 0;
	Message mes = {0x600+ID,0,8,{0x40,(reg & 0xFF),((reg  >> 8) & 0xFF),0x00,0x00,0x00,0x00,0x00}};
	*(uint32_t*)buf_temp[ID-1] = 0xFFFFFFFE;
	canSend(CAN1, &mes);
	delay_ms(1);
	reg_info = *(uint32_t*)buf_temp[ID-1];
	return reg_info;
}


int8_t set_reg(motorID ID, uint16_t reg, uint8_t len, uint8_t* val_buf)
{
	Message mes = {0x600+ID,0,8,{0x2F,(reg & 0xFF),((reg  >> 8) & 0xFF),00,00,00,00,00}};
	uint32_t i = 0;
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
	delay_ms(1);
	int8_t temp_flag = write_flag[ID-1];
	write_flag[ID-1] = -2;
	return temp_flag;
}

int8_t status_change(motorID ID, uint8_t cmd, uint8_t cmd_status)
{
	int8_t ret_set = 0;
	uint16_t ret_get = 0;
	ret_set = set_status(ID, cmd);
	if(ret_set != 1)
		return -2;
	ret_get = get_status(ID);
	if((ret_get & 0xFF) != cmd_status)
		return 0;
	return 1;
}


int8_t isenable(motorID ID)
{
	uint16_t status = get_status(ID);
	int8_t ret = 0;
	switch(status & 0xFF)
	{
		case 0xFF:
		case 0xFE:
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
	int8_t ret_set = 0;
	uint16_t ret_get = 0;
	ret_set = set_status(ID, 0x06);
	if(ret_set != 1)
		return 0;
	ret_set = set_status(ID, 0x07);
	if(ret_set != 1)
		return 0;
	ret_set = set_status(ID, 0x0F);
	if(ret_set != 1)
		return 0;
	ret_get = get_status(ID);
	if((ret_get & 0xFF) != 0x27)
		return 0;
	return 1;
}


int8_t motor_disable(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	ret = status_change(ID, 0x07, 0x23);
	return ret;
}

int8_t isfault(motorID ID)
{
	uint16_t status = get_status(ID);
	int8_t ret = 0;
	switch(status & 0xFF)
	{
		case 0xFF:
		case 0xFE:
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
	ret = status_change(ID, 0x80, 0x40);
	return ret;
}
int8_t quick_stop(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	ret = status_change(ID, 0x02, 0x07);
	return ret;
}

int8_t quickstop_to_enable(motorID ID)
{
	int8_t ret = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	ret = status_change(ID, 0x0F, 0x27);
	return ret;
}

int32_t get_count(motorID ID)
{
	int32_t position = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			position = -motor1_position;
			break;
		case MOTOR2:
			position = motor2_position;
			break;
		case MOTOR3:
			position = -motor3_position;
			break;
		case MOTOR4:
			position = motor4_position;
			break;
		default:
			break;
	}
	return position;
}


uint16_t get_status(motorID ID)
{
	uint16_t status = 0;
	if(!IS_MOTOR_ID(ID))
		return 0xFFFF;
	status = get_u16(ID, MOTOR_STATUS);
	return status;
}

int8_t get_mode(motorID ID)
{
	int8_t mode = 0;
	if(!IS_MOTOR_ID(ID))
		return 0xFF;
	mode = get_i8(ID, MOTOR_MODE);
	return mode;
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

uint16_t is_motor_moving(motorID ID)
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
	int32_t velo_ = 0;
	if(!IS_MOTOR_ID(ID))
		return -1;
	switch(ID)
	{
		case MOTOR1:
			velo_ = -motor1_accvelo;
			break;
		case MOTOR2:
			velo_ = motor2_accvelo;
			break;
		case MOTOR3:
			velo_ = -motor3_accvelo;
			break;
		case MOTOR4:
			velo_ = motor4_accvelo;
			break;
		default:
			break;
	}
	return velo_;
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



int8_t set_status(motorID ID, uint16_t status_value)
{
	uint8_t buf[2] = {0};

	*(uint16_t*)buf = status_value;

	return set_reg(ID, MOTOR_CONTROL, 2, buf);
}

int8_t set_mode(motorID ID, int8_t mode)
{
	uint8_t buf[1] = {0};
	buf[0] = mode & 0xFF;
	return set_reg(ID, MOTOR_MODE, 1, buf);
}
int8_t set_issave_rw(motorID ID, uint16_t issave)
{
	uint8_t buf[2] = {0};
	if((issave != 1) && (issave != 2))
		return -3;
	*(uint16_t*)buf = issave;
	return set_reg(ID, 0x2009, 2, buf);
}

int8_t set_lock(motorID ID, uint16_t lock)
{
	uint8_t buf[2] = {0};
	if(lock > 30000)
		return -3;
	*(uint16_t*)buf = lock;

	return set_reg(ID, 0x200F, 2, buf);
}

int8_t set_issave_rws(motorID ID, uint16_t issave)
{
	uint8_t buf[2] = {0};
	if((issave != 0) && (issave != 1))
		return -3;
	*(uint16_t*)buf = issave;

	return set_reg(ID, 0x2010, 2, buf);
}

int8_t set_accelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	if(time > 32767)
		return -3;
	*(uint32_t*)buf = time;
	
	return set_reg(ID, 0x6083, 4, buf);
}

int8_t set_decelerate_time(motorID ID, uint32_t time)
{
	uint8_t buf[4] = {0};
	if(time > 32767)
		return -3;
	*(uint32_t*)buf = time;
	
	return set_reg(ID, 0x6084, 4, buf);
}

int8_t set_Vsmooth_factor(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x2018, 2, buf);
}

int8_t set_Eratio_gain(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x2019, 2, buf);
}
int8_t set_Eintegral_gain(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201A, 2, buf);
}
int8_t set_feedforward_ratio(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201B, 2, buf);
}
int8_t set_torque_ratio(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201C, 2, buf);
}
int8_t set_VKp(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201D, 2, buf);
}
int8_t set_VKi(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201E, 2, buf);
}
int8_t set_Vfeedforward_Kf(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x201F, 2, buf);
}

int8_t set_PKp(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

	return set_reg(ID, 0x2020, 2, buf);
}

int8_t set_Pfeedforward_Kf(motorID ID, uint16_t factor)
{
	uint8_t buf[2] = {0};
	if(factor > 30000)
		return -3;
	*(uint16_t*)buf = factor;

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
