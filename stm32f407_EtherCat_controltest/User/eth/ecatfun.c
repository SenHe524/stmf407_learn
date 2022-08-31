#include "ecatfun.h"
#include "string.h"
#include <stdio.h>
#include "delay.h"

extern char IOmap[256];
int32_t cur_pos1, cur_pos2;
uint8 flag=0x00;
uint8_t flag_index = 0x08;
int speed = 5000;
boolean rotaion = TRUE;
uint16_t slave_ = 2;
boolean debug_1 = FALSE, debug_2 = FALSE, debug_3 = FALSE, run = TRUE;
void set_output_int32 (uint8_t slave, uint8_t index, int32_t value, uint8 offset)
{
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].outputs;
	data_ptr += (index * 9 + offset);
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
	*data_ptr++ = (value >> 16) & 0xFF;
	*data_ptr++ = (value >> 24) & 0xFF;
}

void set_output_int16 (uint8_t slave, uint8_t index, int16_t value, uint8 offset)
{
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].outputs;
	data_ptr += (index * 9 + offset);
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}
void set_output_int8 (uint8_t slave, uint8_t index, int8_t value, uint8 offset)
{
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].outputs;
	data_ptr += (index * 9 + offset);
	*data_ptr++ = (value >> 0) & 0xFF;
}

/***************************************************************************************/
void set_output_uint8 (uint8_t slave, uint8_t index, uint8_t value, uint8 offset)
{
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].outputs;
	data_ptr += (index * 9 + offset);
	*data_ptr++ = (value >> 0) & 0xFF;
}

void set_output_uint16 (uint8_t slave, uint8_t index, uint16_t value, uint8 offset)
{
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].outputs;
	data_ptr += (index * 9 + offset);
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}


/***************************************************************************************/
int32_t get_input_int32(uint8_t slave, uint8_t index, uint8_t offset)
{
	int32_t return_value;
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].inputs;
	data_ptr += (index * 15 + offset);
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);
	return_value += (*data_ptr++ << 16);
	return_value += (*data_ptr++ << 24);
	return return_value;
}
int8_t get_input_int8(uint8_t slave, uint8_t index, uint8_t offset)
{
	int8_t return_value;
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].inputs;
	data_ptr += (index * 15 + offset);
	return_value = *data_ptr++;
	return return_value;
}

/***************************************************************************************/
uint16_t get_input_uint16(uint8_t slave, uint8_t index, uint8_t offset)
{
	uint16_t return_value;
	uint8_t *data_ptr;
	data_ptr = ec_slave[slave].inputs;
	data_ptr += (index * 15 + offset);
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);
	return return_value;
}

boolean isEnable(uint8_t slave)
{
	if((get_input_uint16(slave, 0, Statusword_offset) & 0x6F)==0x27
		&& (get_input_uint16(slave, 1, Statusword_offset) & 0x6F)==0x27)
        return TRUE;
    else
        return FALSE;
}

boolean iswarning(uint8_t slave)
{
	if((get_input_uint16(slave, 0, Statusword_offset) & 0x004f) == STATUS_FAULT
		|| (get_input_uint16(slave, 1, Statusword_offset) & 0x004f) == STATUS_FAULT)
        return TRUE;
    else
        return FALSE;
}

void clear_fault(uint8_t slave)
{
	set_output_int16(slave, 0, 0x86, 0);// 0x0080
	set_output_int16(slave, 1, 0x86, 0);// 0x0080
	delay_ms(20);
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	set_output_int16(slave, 0, 0x06, Controlword_offset);
	set_output_int16(slave, 1, 0x06, Controlword_offset);
}


boolean Set_Enable(uint8_t slave)
{
	ec_timet t1, t2;
	t1 = osal_current_time();
	t2 = osal_current_time();
//	while(t2.sec - t1.sec < 3)
	while(1)
	{
		t2 = osal_current_time();
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);
		uint16_t cur_status1 = get_input_uint16(slave, 0, Statusword_offset);
		uint16_t cur_status2 = get_input_uint16(slave, 1, Statusword_offset);
		if((cur_status1 & 0x004f) == STATUS_SWITCHEDONDISABLED
			&& (cur_status2 & 0x004f) == STATUS_SWITCHEDONDISABLED)// 0x0040
		{
			set_output_int8(slave, 0, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
			set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
			set_output_uint16(slave, 0, COMMAND_SHUTDOWN, Controlword_offset);// 0x0006
			set_output_uint16(slave, 1, COMMAND_SHUTDOWN, Controlword_offset);// 0x0006
		}
		else if((cur_status1 & 0x006f) == STATUS_READYTOSWITCHON
			&& (cur_status2 & 0x006f) == STATUS_READYTOSWITCHON)// 0x0021
		{
			set_output_uint16(slave, 0, COMMAND_SWITCHON, Controlword_offset);// 0x0007
			set_output_uint16(slave, 1, COMMAND_SWITCHON, Controlword_offset);// 0x0007
		}
		else if((cur_status1 & 0x006f) == STATUS_SWITCHEDON
			&& (cur_status2 & 0x006f) == STATUS_SWITCHEDON)// 0x0023
		{
			set_output_uint16(slave, 0, COMMAND_SWITCHON_ENABLEOPERATION, Controlword_offset);// 0x000F
			set_output_uint16(slave, 1, COMMAND_SWITCHON_ENABLEOPERATION, Controlword_offset);// 0x000F
		}
		else if((cur_status1 & 0x006f) == STATUS_OPERATIONENABLED
			&& (cur_status2 & 0x006f) == STATUS_OPERATIONENABLED)// 0x0027
		{
			set_output_uint8(slave, 0, 0x1F, Controlword_offset);
			set_output_uint8(slave, 1, 0x1F, Controlword_offset);
			cur_pos1 = get_input_int32(slave, 0, P_actual_value_offset);
			cur_pos2 = get_input_int32(slave, 1, P_actual_value_offset);
			set_output_int32(slave, 0, cur_pos1, Target_Position_offset);
			set_output_int32(slave, 1, cur_pos2, Target_Position_offset);
			return TRUE;
		}
		else if(iswarning(slave)) // 0x0008
		{
			set_output_int8(slave, 0, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
			set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
			
			clear_fault(slave);
		}
	}

	return FALSE;
}

void set_position(uint8_t slave, uint8_t index, boolean add,uint16_t position_change)
{
	int32_t pos = 0;
	
	pos = get_input_int32(slave, index, P_actual_value_offset);
//	add == TRUE?  (pos += position_change):(pos -= position_change);
	if(add)
	{
		pos += position_change;
	}
	else
	{
		pos -= position_change;
	}
	set_output_uint8(slave, index, 0x1F, Controlword_offset);
	set_output_int32(slave, index, pos, Target_Position_offset);
}

void ecat_loop(void)
{
	int i;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);

	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	if(debug_1 && !isEnable(1)){
		if(!Set_Enable(1))
			debug_1 = FALSE;
	}
	if(debug_2 && !isEnable(2)){
		if(!Set_Enable(2))
			debug_2 = FALSE;
	}
	if(debug_3 && !isEnable(3)){
		if(!Set_Enable(3))
			debug_3 = FALSE;
	}

	if((debug_1 || debug_2 || debug_3) && run){
		set_position(slave_, 0, rotaion, speed);// TRUE:顺时针  FALSE:逆时针
		set_position(slave_, 1, rotaion, speed);// TRUE:顺时针  FALSE:逆时针
	}

	(void )i;
}

