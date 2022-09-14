#include "ecatfun.h"
#include "string.h"
#include <stdio.h>
#include "delay.h"
#include "timer.h"
#include "hardware_conf.h"
#include "ethercat.h"
#include "brake.h"

#define EC_TIMEOUTMON 500

int32_t cur_pos1, cur_pos2;
int speed = 10;
boolean rotaion = TRUE;
uint16_t slave_ = 3;
boolean debug_1 = FALSE, debug_2 = FALSE, debug_3 = FALSE, run = TRUE;

int dorun=0;
int iomap_size;

char IOmap[256] = {0}; //映射空间

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // GPIO配置结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    // 开启外设时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // GPIO初始化
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


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

void set_controlword(uint8_t slave, uint8_t index, uint16_t value)
{
	set_output_uint16(slave, index, value, Controlword_offset);
}

void set_mode_operation(uint8_t slave, uint8_t index, uint16_t value)
{
	set_output_int8(slave, index, value, Modes_Of_Operation_offset);
}

void set_position(uint8_t slave, uint8_t index, boolean direction,uint16_t position_change)
{
	int32_t pos = 0;
	position_change *= 367; 
	pos = get_position(slave, index);
	direction == TRUE?  (pos += position_change):(pos -= position_change);
	set_output_uint8(slave, index, 0x1F, Controlword_offset);
	set_output_int32(slave, index, pos, Target_Position_offset);
}


uint16_t get_statusword(uint8_t slave, uint8_t index)
{
	return get_input_uint16(slave, index, Statusword_offset);
}

int32_t get_position(uint8_t slave, uint8_t index)
{
	return get_input_int32(slave, index, P_actual_value_offset);
}

uint16_t get_errorcode(uint8_t slave, uint8_t index)
{
	return get_input_uint16(slave, index, Error_Code_offset);
}

uint8_t get_mode_operation(uint8_t slave, uint8_t index)
{
	return get_input_int8(slave, index, MOP_Display_offset);
}


boolean isEnable(uint8_t slave)
{
	if((get_statusword(slave, 0) & 0x6F)==0x27
		&& (get_statusword(slave, 1) & 0x6F)==0x27)
        return TRUE;
    else
        return FALSE;
}

boolean iswarning(uint8_t slave)
{
	if((get_statusword(slave, 0) & 0x004f) == STATUS_FAULT
		|| (get_statusword(slave, 1) & 0x004f) == STATUS_FAULT)
        return TRUE;
    else
        return FALSE;
}

void clear_fault(uint8_t slave)
{
	set_controlword(slave, 0, 0x86);
	set_controlword(slave, 1, 0x86);
	delay_ms(20);
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	set_controlword(slave, 0, 0x06);
	set_controlword(slave, 1, 0x06);
}

void Set_Disable(uint8_t slave)
{
	if(isEnable(slave))
	{
		cur_pos1 = get_position(slave, 0);
		cur_pos2 = get_position(slave, 1);
		set_position(slave, 0, TRUE, 0);
		set_position(slave, 1, TRUE, 0);
		set_controlword(slave, 0, COMMAND_SHUTDOWN);
		set_controlword(slave, 1, COMMAND_SHUTDOWN);
	}

}

boolean Set_Enable(uint8_t slave)
{
	ec_timet t1, t2;
	t1 = osal_current_time();
	t2 = osal_current_time();
	while(t2.sec - t1.sec < 3)
//	while(1)
	{
		t2 = osal_current_time();
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);
		uint16_t cur_status1 = get_statusword(slave, 0);
		uint16_t cur_status2 = get_statusword(slave, 1);
		if((cur_status1 & 0x004f) == STATUS_SWITCHEDONDISABLED
			&& (cur_status2 & 0x004f) == STATUS_SWITCHEDONDISABLED)// 0x0040
		{
			set_mode_operation(slave, 0, CYCLIC_SYNC_POSITION_MODE);
			set_mode_operation(slave, 1, CYCLIC_SYNC_POSITION_MODE);
			set_controlword(slave, 0, COMMAND_SHUTDOWN);// 0x0006
			set_controlword(slave, 1, COMMAND_SHUTDOWN);// 0x0006
		}
		else if((cur_status1 & 0x006f) == STATUS_READYTOSWITCHON
			&& (cur_status2 & 0x006f) == STATUS_READYTOSWITCHON)// 0x0021
		{
			set_controlword(slave, 0, COMMAND_SWITCHON);// 0x0007
			set_controlword(slave, 1, COMMAND_SWITCHON);// 0x0007
		}
		else if((cur_status1 & 0x006f) == STATUS_SWITCHEDON
			&& (cur_status2 & 0x006f) == STATUS_SWITCHEDON)// 0x0023
		{
			set_controlword(slave, 0, COMMAND_SWITCHON_ENABLEOPERATION);// 0x000F
			set_controlword(slave, 1, COMMAND_SWITCHON_ENABLEOPERATION);// 0x000F
		}
		else if((cur_status1 & 0x006f) == STATUS_OPERATIONENABLED
			&& (cur_status2 & 0x006f) == STATUS_OPERATIONENABLED)// 0x0027
		{
			set_controlword(slave, 0, 0x001F);
			set_controlword(slave, 1, 0x001F);
			cur_pos1 = get_position(slave, 0);
			cur_pos2 = get_position(slave, 1);
			set_position(slave, 0, TRUE, 0);
			set_position(slave, 1, TRUE, 0);
			return TRUE;
		}
		else if(iswarning(slave)) // 0x0008
		{
			set_mode_operation(slave, 0, CYCLIC_SYNC_POSITION_MODE);
			set_mode_operation(slave, 1, CYCLIC_SYNC_POSITION_MODE);
			cur_pos1 = get_position(slave, 0);
			cur_pos2 = get_position(slave, 1);
			set_position(slave, 0, TRUE, 0);
			set_position(slave, 1, TRUE, 0);
			clear_fault(slave);
		}
	}

	return FALSE;
}

int Servosetup(ecx_contextt *context, uint16 slave)
{
	uint8_t u8val;
	uint16_t u16val;
	uint32_t u32val;
	
	// 0x1600-0x17FF:RxPDO映射
	u8val = 0;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1c12数据
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1600数据
	ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1601数据
	
	u32val = 0x60400010;
	ec_SDOwrite(slave, 0x1600, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x60600008;
	ec_SDOwrite(slave, 0x1600, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x607A0020;
	ec_SDOwrite(slave, 0x1600, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x60710010;
	ec_SDOwrite(slave, 0x1600, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u8val = 4;
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u32val = 0x68400010;
	ec_SDOwrite(slave, 0x1610, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x68600008;   
	ec_SDOwrite(slave, 0x1610, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x687A0020;   
	ec_SDOwrite(slave, 0x1610, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x68710010;   
	ec_SDOwrite(slave, 0x1610, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u8val = 4;             
	ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u16val = 0x1600;
	ec_SDOwrite(slave, 0x1c12, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u16val = 0x1610;
	ec_SDOwrite(slave, 0x1c12, 0x02, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	
	u8val = 2;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//启用PDO配置

	/**********************************************************************************************************/
	
	// 0x1A00-0x1BFF:TxPDO映射，结构体类型
	u8val = 0;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1c13数据
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1A00数据
	ec_SDOwrite(slave, 0x1A10, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1A01数据
	
	u32val = 0x60410010;
	ec_SDOwrite(slave, 0x1A00, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60770010;
	ec_SDOwrite(slave, 0x1A00, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60640020;
	ec_SDOwrite(slave, 0x1A00, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x606C0020;
	ec_SDOwrite(slave, 0x1A00, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x603F0010;
	ec_SDOwrite(slave, 0x1A00, 0x05, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60610008;
	ec_SDOwrite(slave, 0x1A00, 0x06, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u8val = 6;
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u32val = 0x68410010;
	ec_SDOwrite(slave, 0x1A10, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68770010;   
	ec_SDOwrite(slave, 0x1A10, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68640020;   
	ec_SDOwrite(slave, 0x1A10, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x686C0020;   
	ec_SDOwrite(slave, 0x1A10, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x683F0010;   
	ec_SDOwrite(slave, 0x1A10, 0x05, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68610008;   
	ec_SDOwrite(slave, 0x1A10, 0x06, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u8val = 6;             
	ec_SDOwrite(slave, 0x1A10, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u16val = 0x1A00;
	ec_SDOwrite(slave, 0x1c13, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u16val = 0x1A10;
	ec_SDOwrite(slave, 0x1c13, 0x02, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u8val = 2;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//启用PDO配置
	
//	//	配置为周期性同步位置模式：见pdf112
//	u8val = 8;
//	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);
//	ec_SDOwrite(slave, 0x6860, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);
	return 1;
} 

boolean Soem_init(char *ifname)
{
	int slc, i;
	if(!ec_init(ifname))//初始化soem,将主站绑定到网口
	{
		printf("No socket connection on %s\nExecute as root\n", ifname);
		return FALSE;
	}
	if(ec_config_init(FALSE) <= 0)
	{
		printf("No slaves found!\n");
		return FALSE;
	}
	for(slc = 1; slc <= ec_slavecount; slc++)
	{
		if((ec_slave[slc].eep_man == 0x0000001A) && (ec_slave[slc].eep_id == 0x50440200))
		{
			// link slave specific setup to preop->safeop hook
			ec_slave[slc].PO2SOconfigx = &Servosetup;
		}
	}
	delay_ms(10);
    iomap_size = ec_config_map(&IOmap); //主站内存空间与从站内存空间映射
	if(iomap_size != 0x00000090)
	{
		return FALSE;
	}
    ec_configdc(); //配置DC
	for(slc = 1; slc <= ec_slavecount; slc++)
	{
		ec_dcsync0(slc, TRUE, 1000000, 0);
	}
	printf("Slaves mapped.\n");
	//等待所有从机进入安全运行状态
	ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
	printf("Request operational state for all slaves\n");
	ec_slave[0].state = EC_STATE_OPERATIONAL;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	ec_writestate(0);//	写入EC_STATE_OPERATIONAL状态
	//	等待所有从机进入运行状态
	ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
	ec_readstate();// 更新所有从机状态
	if (ec_slave[0].state == EC_STATE_OPERATIONAL )
	{
		printf("Operational state reached for all slaves.\n");
		dorun=1;
		Timer5_Init();
	}
	else
	{
		printf("Not all slaves reached operational state.\n");
		ec_readstate();
		for(i = 1; i<=ec_slavecount ; i++)
		{
			if(ec_slave[i].state != EC_STATE_OPERATIONAL)
			{
				printf("Slave %d State=0x%04x StatusCode=0x%04x\n",i, ec_slave[i].state, ec_slave[i].ALstatuscode);
			}
		}
		return FALSE;
	}
	(void) iomap_size;
	return TRUE;
}


void System_init(void)
{
	Timer2And3_Init();
    Timer2And3_start();

    delay_init();
    Led_Init();

    // 绑定w5500读取写入函数，硬件重置w5500，写入网络参数
    w5500_init();
    w5500_RegSetup();
    // 延时以保证能扫描到从机
    delay_ms(500);
}

boolean Soem_close(void)
{
	/* stop SOEM, close socket */
	ec_slave[0].state = EC_STATE_INIT;
	ec_writestate(0);//	写入EC_STATE_INIT状态
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	//	等待所有从机进入初始化状态
	if(ec_statecheck(0, EC_STATE_INIT, EC_TIMEOUTSTATE) != EC_STATE_INIT)	return FALSE;
	ec_readstate();// 更新所有从机状态
	
	ec_close();
	return TRUE;

}

void ecat_loop(void)
{
	int i;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);

	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	if(debug_1 && !isEnable(1)){
		Set_Enable(1);
	}
	if(debug_2 && !isEnable(2)){
		Set_Enable(2);
	}
	if(debug_3 && !isEnable(3)){
		Set_Enable(3);
	}
	if(((debug_1 && isEnable(1)) || (debug_2 && isEnable(2)) || (debug_3 && isEnable(3))) && run)
	{
		set_position(slave_, 0, rotaion, speed);// TRUE:顺时针  FALSE:逆时针
		set_position(slave_, 1, rotaion, speed);// TRUE:顺时针  FALSE:逆时针
	}

	(void )i;
}

