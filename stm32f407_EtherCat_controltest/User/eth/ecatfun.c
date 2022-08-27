#include "ecatfun.h"
#include "string.h"
#include <stdio.h>
#include "delay.h"
int32_t cur_pos;
uint8 flag=0x00;
uint8_t flag_index = 0;
void set_output_int32 (uint16_t slave, uint8_t index, int32_t value, uint8 offset)
{
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].outputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 9 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
	*data_ptr++ = (value >> 16) & 0xFF;
	*data_ptr++ = (value >> 24) & 0xFF;
}

void set_output_int16 (uint16_t slave, uint8_t index, int16_t value, uint8 offset)
{
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].outputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 9 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}
void set_output_int8 (uint16_t slave, uint8_t index, int8_t value, uint8 offset)
{
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].outputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 9 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
}

/***************************************************************************************/
void set_output_uint8 (uint16_t slave, uint8_t index, uint8_t value, uint8 offset)
{
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].outputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 9 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
}

void set_output_uint16 (uint16_t slave, uint8_t index, uint16_t value, uint8 offset)
{
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].outputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 9 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}


/***************************************************************************************/
int32_t get_input_int32(uint16_t slave, uint8_t index, uint8_t offset)
{
	int32_t return_value;
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].inputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 15 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);
	return_value += (*data_ptr++ << 16);
	return_value += (*data_ptr++ << 24);

	return return_value;
}
int8_t get_input_int8(uint16_t slave, uint8_t index, uint8_t offset)
{
	int8_t return_value;
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].inputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 15 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	
	return return_value;
}

/***************************************************************************************/
uint16_t get_input_uint16(uint16_t slave, uint8_t index, uint8_t offset)
{
	uint16_t return_value;
	uint8_t *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave].inputs;
	/* Move pointer to correct module index */
	data_ptr += (index * 15 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);


	return return_value;
}
//int Close_brake1(uint8_t slave)
//{
//	uint8_t u8val;
//	uint32_t u32val, u32val1, u32val2;
//	int i = 40;
//	int u8sz = sizeof(u8val);
//	int u32sz = sizeof(u32val);
//	int u32 = sizeof(u32val);
//	u8val = USER_DEFINED_BRAKE;
//	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
//	delay_ms(20);
//	u8val = 0;
//	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
//	u32val = 0x11000000;
//	ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//	u32val = 0;
//	ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
//	while(i--)
//	{
//		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
//		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
//		if(u32val1 == 0x300000 && u32val2 == 0x300000)
//		{
//			delay_ms(10);
//			u32val = 0x33000000;
//			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
//			break;
//		}
//		if(!i)
//		{
//			return 0;
//		}
//		delay_ms(10);
//	}
//	i = 40;
//	while(i--)
//	{
//		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
//		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
//		if(u32val1 == 0 && u32val2 == 0)
//		{
//			delay_ms(1);
//			u32val = 0x22000000;
//			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
//			break;
//		}
//		if(!i)
//		{
//			return 0;
//		}
//		delay_ms(10);
//	}
//	i = 40;
//	while(i--)
//	{
//		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
//		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
//		if(u32val1 == 0x400000 && u32val2 == 0x400000)
//		{
//			delay_ms(10);
//			u32val = 0x33000000;
//			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
//			break;
//		}
//		if(!i)
//		{
//			return 0;
//		}
//		delay_ms(10);
//	}
//	i = 40;
//	while(i--)
//	{
//		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
//		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
//		if(u32val1 == 0 && u32val2 == 0)
//		{
//			delay_ms(1);
//			u32val = 0;
//			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
//			break;
//		}
//		if(!i)
//		{
//			return 0;
//		}
//		delay_ms(10);
//	}
//	u8val = CYCLIC_SYNC_POSITION_MODE;
//	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
//	u8val = 0;
//	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
//	return 1;
//}

boolean isEnable(uint8_t slave, uint8_t index)
{
	if((get_input_uint16(slave, index, Statusword_offset) & 0xf)==0x7)
        return TRUE;
    else
        return FALSE;
}

boolean iswarning(uint8_t slave, uint8_t index)
{
	if((get_input_uint16(slave, index, Statusword_offset) & 0x004f) == STATUS_FAULT)
        return TRUE;
    else
        return FALSE;
}

boolean open_brake1(uint8_t slave)
{
	uint8_t u8val;
	uint32_t u32val, u32val1, u32val2;
	int u8sz = sizeof(u8val);
	int u32sz = sizeof(u32val);
	int u32 = sizeof(u32val);
	boolean isopen = 0;
	u8val = USER_DEFINED_BRAKE;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	delay_ms(20);
	u8val = 0;
	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	while(1)
	{
		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 == 0 && u32val2 == 0 && isopen == FALSE)
		{
			u32val = 0x11000000;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0x300000 && u32val2 == 0x300000 && isopen == FALSE)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			isopen = TRUE;
		}
		else if(u32val1 == 0 && u32val2 == 0 && isopen == TRUE)
		{
			u32val = 0;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}

	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	u8val = 0;
	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	return TRUE;
}

boolean Close_brake1(uint8_t slave)
{
	uint8_t u8val;
	uint32_t u32val, u32val1, u32val2;
	int u8sz = sizeof(u8val);
	int u32sz = sizeof(u32val);
	int u32 = sizeof(u32val);
	boolean isclose = FALSE;
	u8val = USER_DEFINED_BRAKE;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	delay_ms(20);
	u8val = 0;
	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	while(1)
	{
		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 != 0 && u32val2 != 0)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0 && u32val2 == 0 && isclose == FALSE)
		{
			u32val = 0x22000000;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0x400000 && u32val2 == 0x400000 && isclose == FALSE)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			isclose = TRUE;
		}
		else if(u32val1 == 0 && u32val2 == 0 && isclose == TRUE)
		{
			u32val = 0;
			ec_SDOwrite(slave, 0x3023, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3023, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	u8val = 0;
	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	return TRUE;
}
boolean open_brake2(uint8_t slave)
{
	uint8_t u8val;
	uint32_t u32val, u32val1, u32val2;
	int u8sz = sizeof(u8val);
	int u32sz = sizeof(u32val);
	int u32 = sizeof(u32val);
	boolean isopen = 0;
	u8val = USER_DEFINED_BRAKE;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	delay_ms(20);
	u8val = 0;
	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	while(1)
	{
		ec_SDOread(slave, 0x2033, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2034, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		
		if(u32val1 != 0 && u32val2 != 0)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			isopen = TRUE;
		}
		else if(u32val1 == 0 && u32val2 == 0 && isopen == FALSE)
		{
			u32val = 0x11000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0x300000 && u32val2 == 0x300000 && isopen == FALSE)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			isopen = TRUE;
		}
		else if(u32val1 == 0 && u32val2 == 0 && isopen == TRUE)
		{
			u32val = 0;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}

	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	u8val = 0;
	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	return TRUE;
}

boolean Close_brake2(uint8_t slave)
{
	uint8_t u8val;
	uint32_t u32val, u32val1, u32val2;
	int u8sz = sizeof(u8val);
	int u32sz = sizeof(u32val);
	int u32 = sizeof(u32val);
	boolean isclose = FALSE;
	u8val = USER_DEFINED_BRAKE;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	delay_ms(20);
	u8val = 0;
	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	while(1)
	{
		u32val1 = 0;
		u32val2 = 0;
		ec_SDOread(slave, 0x2033, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2034, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
//		if(u32val1 == 0x400000 && u32val2 == 0x400000)
//			break;
		if(u32val1 != 0 && u32val2 != 0)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0 && u32val2 == 0 && isclose == FALSE)
		{
			u32val = 0x22000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
		}
		else if(u32val1 == 0x400000 && u32val2 == 0x400000 && isclose == FALSE)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 0;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			isclose = TRUE;
		}
		else if(u32val1 == 0 && u32val2 == 0 && isclose == TRUE)
		{
			u32val = 0;
			ec_SDOwrite(slave, 0x3033, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
			u32val = 1;
			ec_SDOread(slave, 0x3033, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	u8val = 0;
	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	return TRUE;
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


boolean status_control(uint8_t slave, uint8_t index)
{
	boolean ret = TRUE;
	uint16_t cur_status = get_input_uint16(slave, index, Statusword_offset);
	uint16_t err = get_input_uint16(slave, index, Error_Code_offset);
	if((cur_status & 0x004f) == STATUS_SWITCHEDONDISABLED)// 0x0040
	{
		set_output_int8(slave, index, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
		set_output_uint16(slave, index, COMMAND_SHUTDOWN, 0);// 0x0006
	}
	else if((cur_status & 0x006f) == STATUS_READYTOSWITCHON)// 0x0021
	{
		set_output_uint16(slave, index, COMMAND_SWITCHON, 0);// 0x0007
	}
	else if((cur_status & 0x006f) == STATUS_SWITCHEDON)// 0x0023
	{
		set_output_uint16(slave, index, COMMAND_SWITCHON_ENABLEOPERATION, 0);// 0x000F
	}
	else if((cur_status & 0x006f) == STATUS_OPERATIONENABLED)// 0x0027
	{
		set_output_uint8(slave, index, 0x1F, Controlword_offset);
		cur_pos = get_input_int32(slave, index, P_actual_value_offset);
		set_output_int32(slave, index, cur_pos, Target_Position_offset);
		flag |= (0x01 << (slave + 4)) + (0x01 << flag_index);
	}
	else if(iswarning(slave, index)) // 0x0008
	{
		clear_fault(slave);
		if(index)
		{
			ret = Close_brake2(slave);
		}
		else{
			ret = Close_brake1(slave);
		}
	}
	flag_index++;
	flag_index %= 2;
	return ret;
}

void set_position(uint8_t slave, uint8_t index, boolean add,uint16_t position_change)
{
	int32_t pos = 0;
	pos = get_input_int32(slave, index, P_actual_value_offset);
	if(add)
	{
		cur_pos = pos + position_change;
	}
	else
	{
		cur_pos = pos - position_change;
	}
	set_output_uint8(slave, index, 0x1F, Controlword_offset);
	set_output_int32(slave, index, cur_pos, Target_Position_offset);
}

void ecat_loop(void)
{
	int i;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	if(flag != 0x43)
	{
		if(!status_control(2, 0))
		{
			printf("I'm here!");
		}
		if(!status_control(2, 1))
		{
			printf("I'm here!");
		}
	}			
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		set_position(2, 1, TRUE, 10);
		for(i = 1; i <= ec_slavecount; i++)
		{
			if(!isEnable(i, 0) || !isEnable(i, 1))
			{
				flag = 0;
				break;
			}
		}
	}
}

