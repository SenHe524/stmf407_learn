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
uint16_t slave_1 = 1, slave_2 = 2, slave_3 = 3, slave_ = 3;
uint8_t index_ = 1;
boolean debug_1 = FALSE, debug_2 = FALSE, debug_3 = FALSE;
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
int recognizePose(uint8_t slave)
{
	uint8_t u8val;
	uint32_t u32val, u32val1, u32val2;
	int i = 40;
	int u8sz = sizeof(u8val);
	int u32sz = sizeof(u32val);
	int u32 = sizeof(u32val);
	u8val = 0xC;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	set_output_int8(slave, 0, 0xC, Modes_Of_Operation_offset);
	delay_ms(20);
//	u8val = 0;
//	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	u32val = 0x11000000;
	ec_SDOwrite(slave, 0x3024, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//	u32val = 0;
//	ec_SDOread(slave, 0x3024, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
	while(i--)
	{
		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 == 0x200000 && u32val2 == 0x200000)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3024, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3024, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
		if(!i)
		{
			return 0;
		}
		delay_ms(10);
	}
	i = 40;
	while(i--)
	{
		ec_SDOread(slave, 0x2023, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2024, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 == 0 && u32val2 == 0)
		{
			delay_ms(1);
			u32val = 0;
			ec_SDOwrite(slave, 0x3024, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3024, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
		if(!i)
		{
			return 0;
		}
		delay_ms(10);
	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
//	u8val = 0;
//	ec_SDOread(slave, 0x6061, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	set_output_int8(slave, 0, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
	
	u8val = 0xC;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	set_output_int8(slave, 1, 0xC, Modes_Of_Operation_offset);
	delay_ms(20);
//	u8val = 0;
//	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	u32val = 0x11000000;
	ec_SDOwrite(slave, 0x3034, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//	u32val = 0;
//	ec_SDOread(slave, 0x3034, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
	while(i--)
	{
		ec_SDOread(slave, 0x2033, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2034, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 == 0x200000 && u32val2 == 0x200000)
		{
			delay_ms(10);
			u32val = 0x33000000;
			ec_SDOwrite(slave, 0x3034, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3034, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
		if(!i)
		{
			return 0;
		}
		delay_ms(10);
	}
	i = 40;
	while(i--)
	{
		ec_SDOread(slave, 0x2033, 0x00, FALSE, &u32sz, &u32val1, EC_TIMEOUTRXM);
		ec_SDOread(slave, 0x2034, 0x00, FALSE, &u32sz, &u32val2, EC_TIMEOUTRXM);
		if(u32val1 == 0 && u32val2 == 0)
		{
			delay_ms(1);
			u32val = 0;
			ec_SDOwrite(slave, 0x3034, 0x00, FALSE, u32sz, &u32val, EC_TIMEOUTRXM);
//			u32val = 0;
//			ec_SDOread(slave, 0x3034, 0x00, FALSE, &u32, &u32val, EC_TIMEOUTRXM);
			break;
		}
		if(!i)
		{
			return 0;
		}
		delay_ms(10);
	}
	u8val = CYCLIC_SYNC_POSITION_MODE;
	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, u8sz, &u8val, EC_TIMEOUTRXM);
	set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
//	u8val = 0;
//	ec_SDOread(slave, 0x6861, 0x00, FALSE, &u8sz, &u8val, EC_TIMEOUTRXM);
	return 1;
}

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
	set_output_int8(slave, 0, USER_DEFINED_BRAKE, Modes_Of_Operation_offset);
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
	set_output_int8(slave, 0, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
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
	set_output_int8(slave, 1, USER_DEFINED_BRAKE, Modes_Of_Operation_offset);
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
	set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
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


boolean status_control(uint8_t slave)
{
	boolean ret = TRUE;
	uint16_t cur_status1 = get_input_uint16(slave, 0, Statusword_offset);
	uint16_t cur_status2 = get_input_uint16(slave, 1, Statusword_offset);
//	uint16_t err = get_input_uint16(slave, index, Error_Code_offset);
	if((cur_status1 & 0x004f) == STATUS_SWITCHEDONDISABLED
		&& (cur_status2 & 0x004f) == STATUS_SWITCHEDONDISABLED)// 0x0040
	{
		set_output_int8(slave, 0, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
		set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
		set_output_uint16(slave, 0, COMMAND_SHUTDOWN, Controlword_offset);// 0x0006
		set_output_uint16(slave, 1, COMMAND_SHUTDOWN, Controlword_offset);// 0x0006
//		recognizePose(slave);
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
		flag |= 0x01 << slave;
	}
	else if(iswarning(slave, 0) || iswarning(slave, 1)) // 0x0008
	{
		clear_fault(slave);
	}
	return ret;
}

//boolean status_control(uint8_t slave)
//{
//	boolean ret = TRUE;
//	uint16_t cur_status2 = get_input_uint16(slave, 1, Statusword_offset);
//	if((cur_status2 & 0x004f) == STATUS_SWITCHEDONDISABLED)// 0x0040
//	{
//		set_output_int8(slave, 1, CYCLIC_SYNC_POSITION_MODE, Modes_Of_Operation_offset);
//		set_output_uint16(slave, 1, COMMAND_SHUTDOWN, Controlword_offset);// 0x0006
//	}
//	else if((cur_status2 & 0x006f) == STATUS_READYTOSWITCHON)// 0x0021
//	{
//		set_output_uint16(slave, 1, COMMAND_SWITCHON, Controlword_offset);// 0x0007
//	}
//	else if((cur_status2 & 0x006f) == STATUS_SWITCHEDON)// 0x0023
//	{
//		set_output_uint16(slave, 1, COMMAND_SWITCHON_ENABLEOPERATION, Controlword_offset);// 0x000F
//	}
//	else if((cur_status2 & 0x006f) == STATUS_OPERATIONENABLED)// 0x0027
//	{
//		set_output_uint8(slave, 1, 0x1F, Controlword_offset);
//		cur_pos2 = get_input_int32(slave, 1, P_actual_value_offset);
//		set_output_int32(slave, 1, cur_pos2, Target_Position_offset);
//		flag |= 0x01 << slave;
//	}
//	else if(iswarning(slave, 1)) // 0x0008
//	{
//		clear_fault(slave);
//	}
//	return ret;
//}
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
	if(flag != flag_index)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		if(debug_1)
		{
			status_control(slave_1);
		}
		if(debug_2)
		{
			status_control(slave_2);
		}
		if(debug_3)
		{
			status_control(slave_3);
		}

	}			
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		set_position(slave_, index_, rotaion, speed);// TRUE:顺时针  FALSE:逆时针
//		for(i = 1; i <= ec_slavecount; i++)
//		{
//			if(!isEnable(slave_, 0) || !isEnable(slave_, 1))
//			{
//				flag = 0;
//				break;
//			}
//		}
		if(debug_1)
		{
			if(!isEnable(slave_1, 0) || !isEnable(slave_, 1))
			{
				flag = 0;
			}
		}
		if(debug_2)
		{
			if(!isEnable(slave_2, 0) || !isEnable(slave_, 1))
			{
				flag = 0;
			}
		}
		if(debug_3)
		{
			if(!isEnable(slave_3, 0) || !isEnable(slave_, 1))
			{
				flag = 0;
			}
		}
	}
	(void )i;
}

