#include "ecatfun.h"
#include "ethercat.h"
#include "delay.h"

boolean Open_brake1(uint8_t slave)
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
boolean Open_brake2(uint8_t slave)
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
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);
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
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);
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
