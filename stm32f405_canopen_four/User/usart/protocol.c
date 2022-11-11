#include "protocol.h"
#include "stdlib.h"
// 接收状态机
uint8_t rxflag = 0;
// 命令接收缓存
uint8_t rxbuf[BUF_MAX_LEN] = {0};
uint8_t txbuf[BUF_MAX_LEN] = {0};
// 接收数据下标
uint8_t rxindex = 0;
// 新命令数据长度
uint8_t rxlen = 0;
// 新命令接收标志
uint8_t cmdflag = 0;
// 上位机读取控制状态标志及数据缓存
uint8_t ctrl_ret[3] = {0};
uint8_t ctrl_flag = 0;
//	上位机读取参数值
union_uint16 data16;
union_int32 data32;
uint8_t datatype_flag = 0xFF;
// ------------------------查表法crc8校验-------
static const uint8_t  crc8tab[] = 
{0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,
0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,0x5f,0x01,0xe3,0xbd,0x3e,0x60,0x82,0xdc,
0x23,0x7d,0x9f,0xc1,0x42,0x1c,0xfe,0xa0,0xe1,0xbf,0x5d,0x03,0x80,0xde,0x3c,0x62,
0xbe,0xe0,0x02,0x5c,0xdf,0x81,0x63,0x3d,0x7c,0x22,0xc0,0x9e,0x1d,0x43,0xa1,0xff,
0x46,0x18,0xfa,0xa4,0x27,0x79,0x9b,0xc5,0x84,0xda,0x38,0x66,0xe5,0xbb,0x59,0x07,
0xdb,0x85,0x67,0x39,0xba,0xe4,0x06,0x58,0x19,0x47,0xa5,0xfb,0x78,0x26,0xc4,0x9a,
0x65,0x3b,0xd9,0x87,0x04,0x5a,0xb8,0xe6,0xa7,0xf9,0x1b,0x45,0xc6,0x98,0x7a,0x24,
0xf8,0xa6,0x44,0x1a,0x99,0xc7,0x25,0x7b,0x3a,0x64,0x86,0xd8,0x5b,0x05,0xe7,0xb9,
0x8c,0xd2,0x30,0x6e,0xed,0xb3,0x51,0x0f,0x4e,0x10,0xf2,0xac,0x2f,0x71,0x93,0xcd,
0x11,0x4f,0xad,0xf3,0x70,0x2e,0xcc,0x92,0xd3,0x8d,0x6f,0x31,0xb2,0xec,0x0e,0x50,
0xaf,0xf1,0x13,0x4d,0xce,0x90,0x72,0x2c,0x6d,0x33,0xd1,0x8f,0x0c,0x52,0xb0,0xee,
0x32,0x6c,0x8e,0xd0,0x53,0x0d,0xef,0xb1,0xf0,0xae,0x4c,0x12,0x91,0xcf,0x2d,0x73,
0xca,0x94,0x76,0x28,0xab,0xf5,0x17,0x49,0x08,0x56,0xb4,0xea,0x69,0x37,0xd5,0x8b,
0x57,0x09,0xeb,0xb5,0x36,0x68,0x8a,0xd4,0x95,0xcb,0x29,0x77,0xf4,0xaa,0x48,0x16,
0xe9,0xb7,0x55,0x0b,0x88,0xd6,0x34,0x6a,0x2b,0x75,0x97,0xc9,0x4a,0x14,0xf6,0xa8,
0x74,0x2a,0xc8,0x96,0x15,0x4b,0xa9,0xf7,0xb6,0xe8,0x0a,0x54,0xd7,0x89,0x6b,0x35};

uint8_t getcrc8tab(const uint8_t *buf, int len) {
  uint8_t crc = 0x00;
  while(len--)
  {
    crc = crc8tab[crc ^ *buf++];
  }
  return crc;
}
// 获取接收的数据
uint8_t* get_rxbuf(void)
{
	return (uint8_t*)rxbuf;
}

// 获取命令长度
uint8_t get_rxlen(void)
{
	return rxlen;
}

// 获取命令标志
uint8_t is_rcv_cmd(void)
{
	return cmdflag;
}
// 清除命令数据和相关标志
void clear_cmd(void)
{

	for (uint8_t i = 0; i < rxlen; i++)
		rxbuf[i] = 0;
	rxflag = 0;
	rxindex = 0;
	cmdflag = 0;
}


int frame_packing(const uint8_t *buf, uint8_t *frame, uint8_t len, uint8_t func) {
    uint8_t cnt = 0;
    frame[cnt++] = FIRST_CODE;
    frame[cnt++] = func;
    frame[cnt++] = len;
    for (int i = 0; i < len; i++) {
        frame[cnt++] = buf[i];
    }
    frame[cnt++] = getcrc8tab(buf, len);
    frame[cnt++] = END_CODE;
    return cnt;
}

// 接收串口单字节数据接收并保存
void usart_rcv(uint8_t rxdata)
{
//	rxbuf[rxindex++] =  rxdata;
//	rxindex %= BUF_MAX_LEN;
	switch (rxflag) {
		case FRAME_HEADER: // 帧头
		{
			if (rxdata == FIRST_CODE) {
				rxbuf[rxindex++] = FIRST_CODE;
				rxflag = 1;
			} else {
				rxflag = 0;
				rxindex = 0;
				rxbuf[0] = 0x0;
			}
			break;
		}
		case FRAME_FUNC:// 标识位
		{
			if (rxdata == FUNC_CONTROL || rxdata == FUNC_SPEED 
				|| rxdata == FUNC_PARAM) {
				rxbuf[rxindex++] = rxdata;
				rxflag = 2;
				rxindex = 2;
			} else {
				rxflag = 0;
				rxindex = 0;
				rxbuf[0] = 0;
				rxbuf[1] = 0;
			}
			break;
		}
		case FRAME_LEN:// 数据位长度
		{
			// New_CMD_length为数据帧总字节数 = 帧头+标识位+长度+校验位+帧尾(5 bytes)+数据位
			rxlen = rxdata+5;
			if (rxlen < BUF_MAX_LEN) {
				rxbuf[rxindex++] = rxdata;
				rxflag = 3;
			} else {
				rxflag = 0;
				rxindex = 0;
				rxbuf[0] = 0;
				rxbuf[1] = 0;
				rxlen = 0;
			}
			break;
		}
		case FRAME_DATA:// 读取完剩余的所有字段
		{
			rxbuf[rxindex++] = rxdata;
			if (rxindex >= rxlen && rxbuf[rxlen-1] == END_CODE) {
				cmdflag = 1;
				rxindex = 0;
				rxflag = 0;
			} else if (rxindex >= rxlen 
				&& rxbuf[rxlen-1] != END_CODE){
				clear_cmd();
			}
			break;
		}
		default:
			clear_cmd();
			break;
	}
}
void control_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	if(data_len != 6)
		return ;
	switch(data_buf[3])
	{
		case 0x01:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[4] & (0x01 << i))
				{
					//与motor_disable返回值不同的初始值
					int8_t ret = 3;
					ret = motor_disable((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[0] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[0] |= 0x80;
			break;
		case 0x02:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[4] & (0x01 << i))
				{
					//与motor_enable返回值不同的初始值
					int8_t ret = 3;
					ret = motor_enable((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[0] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[0] |= 0x40;
			break;
		case 0x03:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[4] & (0x01 << i))
				{
					int8_t ret = 3;//与isenable返回值不同的初始值
					ret = isenable((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[0] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[0] |= 0x20;
			break;
		default:
			break;
	}
	switch(data_buf[5])
	{
		case 0x01:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[6] & (0x01 << i))
				{
					int8_t ret = 3;//与clear_fault返回值不同的初始值
					ret = clear_fault((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[1] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[1] |= 0x80;
			break;
		case 0x02:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[6] & (0x01 << i))
				{
					int8_t ret = 3;//与isfault返回值不同的初始值
					ret = isfault((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[1] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[1] |= 0x40;
			break;
		default:
			break;
	}
	switch(data_buf[7])
	{
		case 0x01:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[8] & (0x01 << i))
				{
					int8_t ret = 3;//与quick_stop返回值不同的初始值
					ret = quick_stop((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[2] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[2] |= 0x80;
			break;
		case 0x02:
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[8] & (0x01 << i))
				{
					int8_t ret = 3;
					ret = quickstop_to_enable((motorID)(i+1));
					if(ret == 1){
						ctrl_ret[2] |= (0x01 << i);
					}
				}
			}
			ctrl_flag = 1;
			ctrl_ret[2] |= 0x40;
			break;
		default:
			break;
	}
}
void velo_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	if(data_len != 8)
		return ;
	int16_t velocity[4] = {0};
	for(int i = 0; i < 4; i++)
	{
	velocity[i] = (data_buf[i*2+4] << 8) | data_buf[i*2+3];
	}
	if(abs(velocity[0]) <= VELOCITY_MAX)
	set_velocity_motor1(velocity[0]);
	if(abs(velocity[1]) <= VELOCITY_MAX)
	set_velocity_motor2(velocity[1]);
	if(abs(velocity[2]) <= VELOCITY_MAX)
	set_velocity_motor3(velocity[2]);
	if(abs(velocity[3]) <= VELOCITY_MAX)
	set_velocity_motor4(velocity[3]);
}



int8_t param_set(motorID id, const uint16_t reg, const uint8_t *data_buf)
{
	int8_t ret = 0x55;
	union_uint16 data;
	switch(reg)
	{
		case SAVE_ALLPARAM:{
			data.data8[0] = data_buf[6];
			data.data8[1] = data_buf[7];
			break;
		}
		default:
			break;
	}
	return ret;
}

uint16_t param_get_uint16(motorID id, const uint16_t reg)
{
	union_uint16 data;
	switch(reg)
	{
		case LOCK_METHOD:{
			data.data_uint16 = get_lock(id);
			break;
		}
		case VELO_SMOOTH_FACTOR:{
			data.data_uint16 = get_Vsmooth_factor(id);
			break;
		}
		case ELEC_ERATIO_GAIN:{
			data.data_uint16 = get_Eratio_gain(id);
			break;
		}
		case ELEC_INTEGRAL:{
			data.data_uint16 = get_Eintegral_gain(id);
			break;
		}
		case FEEDFORWARD_RATIO:{
			data.data_uint16 = get_feedforward_ratio(id);
			break;
		}
		case TORQUE_RATIO:{
			data.data_uint16 = get_torque_ratio(id);
			break;
		}
		case VELO_KP:{
			data.data_uint16 = get_VKp(id);
			break;
		}
		case VELO_KI:{
			data.data_uint16 = get_VKi(id);
			break;
		}
		case VELO_FEEDFORWARD_KF:{
			data.data_uint16 = get_Vfeedforward_Kf(id);
			break;
		}
		case POSI_KP:{
			data.data_uint16 = get_PKp(id);
			break;
		}
		case POSI_FEEDFORWARD_KF:{
			data.data_uint16 = get_Pfeedforward_Kf(id);
			break;
		}
		case MOTOR_TEMP:{
			data.data_uint16 = get_motor_temp(id);
			break;
		}
		case IS_MOTOR_MOVE:{
			data.data_uint16 = get_motor_status(id);
			break;
		}
		case MOTOR_HALL_STATUS:{
			data.data_uint16 = get_hall_status(id);
			break;
		}
		case ERROR_CODE:{
			data.data_uint16 = get_errorcode(id);
			break;
		}
		case MOTOR_STATUS:{
			data.data_uint16 = get_errorcode(id);
			break;
		}
		default:
			break;
	}
	return data.data_uint16;
}

uint16_t param_get_int32(motorID id, const uint16_t reg)
{
	union_int32 data;
	switch(reg)
	{
		case MODE_DISPLAY:{
			data.data_int32 = get_mode(id);
			break;
		}
		case ACTUAL_POSITION:{
			data.data_int32 = get_count(id);
			break;
		}
		case ACTUAL_VELOCITY:{
			data.data_int32 = get_actual_velocity(id);
			break;
		}
		case ACC_TIME:{
			data.data_int32 = get_accelerate_time(id);
			break;
		}
		case DE_TIME:{
			data.data_int32 = get_decelerate_time(id);
			break;
		}
		
		default:
			break;
	}
	return data.data_int32;
}


void param_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	union_uint16 reg;
	uint8_t cnt = 0, cnt_ = 0;
	uint8_t temp[64] = {0};
	reg.data_uint16 = 0;
	reg.data_uint16 = 0;
	if(data_len != 8)
		return ;
	switch(data_buf[3] & 0xF0)
	{
		case 0x10:{//	set
			//如果data_buf[3]有数据，则作为返回帧的第一个数据
			temp[cnt_++] = data_buf[3];
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[4];
			reg.data8[1] = data_buf[5];
			temp[cnt_++] = reg.data8[0];
			temp[cnt_++] = reg.data8[1];
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[3] & (0x01 << i))
				{
					// 每个电机的数据按顺序排列
					int8_t ret = 3;
					ret = param_set((motorID)(i+1), reg.data_uint16, data_buf);
					if(ret == 1){
					}

				}
			}
			break;
		}
		case 0x20:{//	get
			//如果data_buf[3]有数据，则作为返回帧的第一个数据
			temp[cnt_++] = data_buf[3];
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[4];
			reg.data8[1] = data_buf[5];
			temp[cnt_++] = reg.data8[0];
			temp[cnt_++] = reg.data8[1];
			//判断所需数据格式
			datatype_flag =  IS_GET_UINT16(reg.data_uint16);
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[3] & (0x01 << i))
				{
					// 每个电机的数据按顺序排列
					if(datatype_flag == 1)//16位数据
					{
						//获取数据
						data16.data_uint16 = param_get_uint16((motorID)(i+1), reg.data_uint16);
						//数据类型存入buf
						temp[cnt_++] = datatype_flag;
						//存入buf
						temp[cnt_++] = data16.data8[0];
						temp[cnt_++] = data16.data8[1];
					}
					else if(datatype_flag == 0)//32位数据
					{
						//获取数据
						data32.data_int32 = param_get_int32((motorID)(i+1), reg.data_uint16);
						//数据类型存入buf
						temp[cnt_++] = datatype_flag;
						//存入buf
						temp[cnt_++] = data32.data8[0];
						temp[cnt_++] = data32.data8[1];
						temp[cnt_++] = data32.data8[2];
						temp[cnt_++] = data32.data8[3];
					}
					datatype_flag = 0xFF;
				}
			}
			break;
		}
		default:
			break;
	}
	switch(data_buf[8] & 0xF0)
	{
		case 0x10:{//	set
			//如果data_buf[8]有数据，原路返回
			temp[cnt_++] = data_buf[8];
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[9];
			reg.data8[1] = data_buf[10];
			temp[cnt_++] = reg.data8[0];
			temp[cnt_++] = reg.data8[1];
			for(int i = 0; i < 4; i++)
			{
				// 每个电机的数据按顺序排列
				if(data_buf[8] & (0x01 << i))
				{
					//与motor_disable返回值不同的初始值
					int8_t ret = 3;
					ret = param_set((motorID)(i+1), reg.data_uint16, data_buf);
					if(ret == 1){

					}

				}
			}
			break;
		}
		case 0x20:{//	get
			//如果data_buf[8]有数据，则作为返回帧的第一个数据
			temp[cnt_++] = data_buf[8];
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[4];
			reg.data8[1] = data_buf[5];
			temp[cnt_++] = reg.data8[0];
			temp[cnt_++] = reg.data8[1];
			//判断所需数据格式
			datatype_flag =  IS_GET_UINT16(reg.data_uint16);
			for(int i = 0; i < 4; i++)
			{
				//	通过位判断控制哪一个电机
				if(data_buf[8] & (0x01 << i))
				{
					// 每个电机的数据按顺序排列
					if(datatype_flag == 1)//16位数据
					{
						//获取数据
						data16.data_uint16 = param_get_uint16((motorID)(i+1), reg.data_uint16);
						//数据类型存入buf
						temp[cnt_++] = datatype_flag;
						//存入buf
						temp[cnt_++] = data16.data8[0];
						temp[cnt_++] = data16.data8[1];
					}
					else if(datatype_flag == 0)//32位数据
					{
						//获取数据
						data32.data_int32 = param_get_int32((motorID)(i+1), reg.data_uint16);
						//数据类型存入buf
						temp[cnt_++] = datatype_flag;
						//存入buf
						temp[cnt_++] = data32.data8[0];
						temp[cnt_++] = data32.data8[1];
						temp[cnt_++] = data32.data8[2];
						temp[cnt_++] = data32.data8[3];
					}
					datatype_flag = 0xFF;
				}
			}
			break;
		}
		default:
			break;
	}
	cnt = frame_packing(temp, txbuf, cnt_, FUNC_PARAM);
	USART1_sendbuf(txbuf, cnt);

}
// 指令解析，传入接收到的完整指令，及其长度
void analysis_cmd(const uint8_t *data_buf, uint8_t len)
{
	uint8_t func_id, data_len;
	func_id = data_buf[1];
	data_len = len - 5;
	#if ENABLE_CHECKSUM
	// 计算校验
	uint8_t crc8buf[32] = {0};
	uint8_t i;
	for (i = 3; i < (len - 2); i++)
		crc8buf[i - 3]= data_buf[i];
	if (getcrc8tab(crc8buf, data_len) != data_buf[len-2])
		return;
	#endif

	switch (func_id)
	{
		case FUNC_CONTROL:
		{
			control_cmd(data_buf, data_len);
			if(ctrl_flag)
			{
				uint8_t cnt = 0;
				cnt = frame_packing(ctrl_ret, txbuf, 3, FUNC_CONTROL);
				USART1_sendbuf(txbuf, cnt);
				ctrl_ret[0] = 0;
				ctrl_ret[1] = 0;
				ctrl_ret[2] = 0;
				ctrl_flag = 0;
			}
			break;
		}
		case FUNC_SPEED:
		{
			velo_cmd(data_buf, data_len);
			break;
		}
		case FUNC_PARAM:
		{
			param_cmd(data_buf, data_len);
			break;
		}
		default:
			break;
	}
}

