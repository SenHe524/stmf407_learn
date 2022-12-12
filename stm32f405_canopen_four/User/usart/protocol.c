#include "protocol.h"
#include "stdlib.h"
// 接收状态机
uint8_t usart1_rxflag = 0;
// 命令接收缓存
uint8_t usart1_rxbuf[BUF_MAX_LEN] = {0};
uint8_t usart1_txbuf[BUF_MAX_LEN] = {0};
// 接收数据下标
uint8_t usart1_rxindex = 0;
// 返回值数组下标
uint8_t usart1_cnt = 0;
// 新命令数据长度
uint8_t usart1_rxlen = 0;
// 新命令接收标志
uint8_t usart1_cmdflag = 0;
//	上位机设置读取参数值
union_int8 data8;
uint8_t datatype_flag = 0xFF;
//引入外部全局变量
extern FLASH_DATA AGV_PARAM;
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
uint8_t* get_usart1rxbuf(void)
{
	return (uint8_t*)usart1_rxbuf;
}

// 获取命令长度
uint8_t get_usart1rxlen(void)
{
	return usart1_rxlen;
}

// 获取命令标志
uint8_t is_rcv_usart1cmd(void)
{
	return usart1_cmdflag;
}
// 清除命令数据和相关标志
void clear_usart1cmd(void)
{

	for (uint8_t i = 0; i < usart1_rxlen; i++)
		usart1_rxbuf[i] = 0;
	usart1_rxflag = 0;
	usart1_rxindex = 0;
	usart1_cmdflag = 0;
}


int usart1frame_packing(const uint8_t *buf, uint8_t *frame, uint8_t len, uint8_t func) {
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


// 接收串口单字节数据并保存
void usart1_rcv(uint8_t rxdata)
{
	switch (usart1_rxflag) {
		case FRAME_HEADER: // 帧头
		{
			if (rxdata == FIRST_CODE) {
				usart1_rxbuf[usart1_rxindex++] = FIRST_CODE;
				usart1_rxflag = FRAME_FUNC;
			} else {
				usart1_rxflag = 0;
				usart1_rxindex = 0;
				usart1_rxbuf[0] = 0x0;
			}
			break;
		}
		case FRAME_FUNC:// 标识位
		{
			if ((rxdata >= 0x01) && (rxdata <= 0x0A)) {
				usart1_rxbuf[usart1_rxindex++] = rxdata;
				usart1_rxflag = FRAME_LEN;
			} else {
				usart1_rxflag = 0;
				usart1_rxindex = 0;
				usart1_rxbuf[0] = 0;
				usart1_rxbuf[1] = 0;
			}
			break;
		}
		case FRAME_LEN:// 数据位长度
		{
			// New_CMD_length为数据帧总字节数 = 帧头+标识位+长度+校验位+帧尾(5 bytes)+数据位
			usart1_rxlen = rxdata + 5;
			if (usart1_rxlen < BUF_MAX_LEN) {
				usart1_rxbuf[usart1_rxindex++] = rxdata;
				usart1_rxflag = FRAME_DATA;
			} else {
				usart1_rxflag = 0;
				usart1_rxindex = 0;
				usart1_rxbuf[0] = 0;
				usart1_rxbuf[1] = 0;
				usart1_rxlen = 0;
			}
			break;
		}
		case FRAME_DATA:// 读取完剩余的所有字段
		{
			usart1_rxbuf[usart1_rxindex++] = rxdata;
			if (usart1_rxindex >= usart1_rxlen && usart1_rxbuf[usart1_rxlen-1] == END_CODE) {
				usart1_cmdflag = 1;
				usart1_rxflag = 0;
				usart1_rxindex = 0;				
			} else if (usart1_rxindex >= usart1_rxlen 
				&& usart1_rxbuf[usart1_rxlen-1] != END_CODE){
				clear_usart1cmd();
			}
			break;
		}
		default:
			clear_usart1cmd();
			break;
	}
}


void Odometry_data(void)
{
	union_int32 count_[4];
	union_int16 velo_[4];
	uint8_t data_buf[30] = {0};
	for(int i = 0; i < 4; i++)
	{
		count_[i].data_int32 = get_count((motorID)(i+1));
		data_buf[i*4] = count_[i].data8[0];
		data_buf[i*4+1] = count_[i].data8[1];
		data_buf[i*4+2] = count_[i].data8[2];
		data_buf[i*4+3] = count_[i].data8[3];
	}
	for(int i = 0; i < 4; i++)
	{
		velo_[i].data_int16 = (int16_t)get_actual_velocity((motorID)(i+1));
		data_buf[i*2+16] = velo_[i].data8[0];
		data_buf[i*2+17] = velo_[i].data8[1];
	}
	int8_t frame_len = 0;
	frame_len = usart1frame_packing(data_buf, usart1_txbuf, 24, ODOMETRY);
	usart1_sendbuf(usart1_txbuf, frame_len);
}

void control_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	union_int8 control_buf[4];
	uint8_t temp_buf[4] = {0};
	uint8_t frame_len = 0;
	if(data_len != 4)
		return ;
	switch(data_buf[1])
	{
		case SET_ENABLE:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = motor_enable((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case SET_DISENABLE:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = motor_disable((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case IS_ENABLE:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = isenable((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case CLEAR_FAULT:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = clear_fault((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case IS_FAULT:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = isfault((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case SET_STOP:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = quick_stop((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
		case STOP_TO_ENABLE:
			for(int i = 0; i < 4; i++)
			{
				if(data_buf[3+i] == 0x01)
				{
					control_buf[i].data_int8 = quickstop_to_enable((motorID)(i+1));
					temp_buf[i] = control_buf[i].data8;
				}
			}
			break;
//		case SAVE_ODOMETRY:
//			
////			for(int i = 0; i < 4; i++)
////			{
////				if(data_buf[3+i] == 0x01)
////				{
////					control_buf[i].data_int8 = quickstop_to_enable((motorID)(i+1));
////					temp_buf[i] = control_buf[i].data8;
////				}
////			}
////			write_agvparam();
//			break;
		default:
			break;
	}
	frame_len = usart1frame_packing(temp_buf, usart1_txbuf, 4, data_buf[1]);
	usart1_sendbuf(usart1_txbuf, frame_len);
}
void velo_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	if(data_len != 8)
		return ;
	uint8_t velo_buf[4] = {0}; 
	union_int16 velocity[4];
	for(int i = 0; i < 4; i++)
	{
		velocity[i].data_int16 = 0;
		velocity[i].data8[0] = data_buf[i*2+3];
		velocity[i].data8[1] = data_buf[i*2+4];
	}
	if(abs(velocity[0].data_int16) <= VELOCITY_MAX)
	{
		set_velocity_motor1(velocity[0].data_int16);
		velo_buf[0] = 1;
	}
	if(abs(velocity[1].data_int16) <= VELOCITY_MAX)
	{
		set_velocity_motor2(velocity[1].data_int16);
		velo_buf[1] = 1;
	}
	if(abs(velocity[2].data_int16) <= VELOCITY_MAX)
	{
		set_velocity_motor3(velocity[2].data_int16);
		velo_buf[2] = 1;
	}
	if(abs(velocity[3].data_int16) <= VELOCITY_MAX)
	{
		set_velocity_motor4(velocity[3].data_int16);
		velo_buf[3] = 1;
	}
	uint8_t frame_len = 0;
	frame_len = usart1frame_packing(velo_buf, usart1_txbuf, 4, data_buf[1]);
	usart1_sendbuf(usart1_txbuf, frame_len);
}



void param_set(const uint16_t reg, const uint8_t *data_buf, uint8_t *data_temp)
{
	union_uint16 data;
	switch(reg)
	{
		case SAVE_RW:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_issave_rw((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case LOCK_METHOD:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				if(data.data_uint16 == 0)
					data8.data_int8 = set_lock((motorID)(i+1), 0);
				else if(data.data_uint16 == 1)
					data8.data_int8 = set_lock((motorID)(i+1), 1);
				else
					data8.data_int8 = 0;
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case SAVE_RW_S:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_issave_rws((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case VELO_SMOOTH_FACTOR:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_Vsmooth_factor((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case ELEC_ERATIO_GAIN:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_Eratio_gain((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case ELEC_INTEGRAL:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_Eintegral_gain((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case FEEDFORWARD_RATIO:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_feedforward_ratio((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case TORQUE_RATIO:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_torque_ratio((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case VELO_KP:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_VKp((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case VELO_KI:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_VKi((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case VELO_FEEDFORWARD_KF:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_Vfeedforward_Kf((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case POSI_KP:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_PKp((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case POSI_FEEDFORWARD_KF:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_Pfeedforward_Kf((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case ACC_TIME:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_accelerate_time((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		case DE_TIME:{
			for(int i = 0; i < 4; i++)
			{
				data.data8[0] = data_buf[5+i*2];
				data.data8[1] = data_buf[6+i*2];
				data8.data_int8 = set_decelerate_time((motorID)(i+1), data.data_uint16);
				data_temp[usart1_cnt++] = data8.data8;
				delay_ms(1);
			}
			break;
		}
		default:
			break;
	}
}

void param_get_16(const uint16_t reg, uint8_t *data_temp)
{
	union_uint16 data;
	switch(reg)
	{
		case SAVE_RW:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_issave_rw((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			
			break;
		}
		case LOCK_METHOD:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_lock((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			
			break;
		}
		case SAVE_RW_S:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_issave_rws((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			
			break;
		}
		case VELO_SMOOTH_FACTOR:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_Vsmooth_factor((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case ELEC_ERATIO_GAIN:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_Eratio_gain((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case ELEC_INTEGRAL:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_Eintegral_gain((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case FEEDFORWARD_RATIO:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_feedforward_ratio((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case TORQUE_RATIO:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_torque_ratio((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case VELO_KP:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_VKp((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case VELO_KI:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_VKi((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case VELO_FEEDFORWARD_KF:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_Vfeedforward_Kf((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case POSI_KP:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_PKp((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case POSI_FEEDFORWARD_KF:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_Pfeedforward_Kf((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case MOTOR_TEMP:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_motor_temp((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case IS_MOTOR_MOVE:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_motor_status((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case MOTOR_HALL_STATUS:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_hall_status((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case ERROR_CODE:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_errorcode((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case MOTOR_STATUS:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_status((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case ACC_TIME:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_accelerate_time((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		case DE_TIME:{
			for(int i = 0; i < 4; i++)
			{
				data.data_uint16 = get_decelerate_time((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				delay_ms(1);
			}
			break;
		}
		default:
			break;
	}
}

void param_get_32(const uint16_t reg, uint8_t *data_temp)
{
	union_int32 data;
//	union_uint32 u32data;
	switch(reg)
	{
		case MODE_DISPLAY:{
			for(int i = 0; i < 4; i++)
			{
				data.data_int32 = get_mode((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				data_temp[usart1_cnt++] = data.data8[2];
				data_temp[usart1_cnt++] = data.data8[3];
				delay_ms(1);
			}
			break;
		}
		case ACTUAL_COUNT:{
			for(int i = 0; i < 4; i++)
			{
				data.data_int32 = get_count((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				data_temp[usart1_cnt++] = data.data8[2];
				data_temp[usart1_cnt++] = data.data8[3];
			}
			break;
		}
		case ACTUAL_VELOCITY:{
			for(int i = 0; i < 4; i++)
			{
				data.data_int32 = get_actual_velocity((motorID)(i+1));
				data_temp[usart1_cnt++] = data.data8[0];
				data_temp[usart1_cnt++] = data.data8[1];
				data_temp[usart1_cnt++] = data.data8[2];
				data_temp[usart1_cnt++] = data.data8[3];
				delay_ms(1);
			}
			break;
		}
		default:
			break;
	}
}


void param_cmd(const uint8_t *data_buf, uint8_t data_len)
{
	union_uint16 reg;
	uint8_t cnt = 0;
	uint8_t ret_buf[27] = {0};
	reg.data_uint16 = 0;
	usart1_cnt = 0;
	switch(data_buf[1])
	{
		case SET_PARAM:{//	set
			if((data_len % 10) != 0)
				return ;
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[3];
			reg.data8[1] = data_buf[4];
			ret_buf[usart1_cnt++] = reg.data8[0];
			ret_buf[usart1_cnt++] = reg.data8[1];
				// 每个电机的数据按顺序排列
			param_set(reg.data_uint16, data_buf, ret_buf);
			break;
		}
		case GET_PARAM:{//	get
			//解析reg数据，并将其原路返回
			reg.data8[0] = data_buf[3];
			reg.data8[1] = data_buf[4];
			ret_buf[usart1_cnt++] = reg.data8[0];
			ret_buf[usart1_cnt++] = reg.data8[1];
			//判断所需数据格式
			datatype_flag =  IS_GET_UINT16(reg.data_uint16);
			//数据类型存入buf
			ret_buf[usart1_cnt++] = datatype_flag;
			// 每个电机的数据按顺序排列
			if(datatype_flag == 1)//16位数据
			{
				//获取数据
				param_get_16(reg.data_uint16, ret_buf);
			}
			else if(datatype_flag == 0)//32位数据
			{
				//获取数据
				param_get_32(reg.data_uint16, ret_buf);
			}
			datatype_flag = 0xFF;
			break;
		}
		default:
			break;
	}
	cnt = usart1frame_packing(ret_buf, usart1_txbuf, usart1_cnt, data_buf[1]);
	usart1_sendbuf(usart1_txbuf, cnt);
}
// 指令解析，传入接收到的完整指令，及其长度
void usart1_analysis_cmd(const uint8_t* data_buf, uint8_t len)
{
	uint8_t data_len;
	data_len = data_buf[2];
	#if ENABLE_CHECKSUM
	// 计算校验
	uint8_t crc8buf[32] = {0};
	uint8_t i;
	for (i = 3; i < (len - 2); i++)
		crc8buf[i - 3]= data_buf[i];
	if (getcrc8tab(crc8buf, data_len) != data_buf[len-2])
		return;
	#endif

	switch (data_buf[1])
	{
		
		case SET_ENABLE:
		case SET_DISENABLE:
		case IS_ENABLE:
		case CLEAR_FAULT:
		case IS_FAULT:
		case SET_STOP:
		case STOP_TO_ENABLE:
//		case SAVE_ODOMETRY:
			control_cmd(data_buf, data_len);
			break;
		case SPEED:
			velo_cmd(data_buf, data_len);
			break;
		case SET_PARAM:
		case GET_PARAM:
			param_cmd(data_buf, data_len);
			break;
		default:
			break;
	}
}

