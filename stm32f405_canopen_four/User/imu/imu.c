#include <stddef.h>
#include "imu.h"

/*------------------------------------------------MARCOS define------------------------------------------------*/
#define PROTOCOL_FIRST_BYTE			(uint8_t)0x59
#define PROTOCOL_SECOND_BYTE		(uint8_t)0x53

#define PROTOCOL_FIRST_BYTE_POS 		0
#define PROTOCOL_SECOND_BYTE_POS		1

#define PROTOCOL_TID_LEN				2
#define PROTOCOL_MIN_LEN				7	/*header(2B) + tid(2B) + len(1B) + CK1(1B) + CK2(1B)*/

#define CRC_CALC_START_POS				2
#define CRC_CALC_LEN(payload_len)		((payload_len) + 3)	/*3 = tid(2B) + len(1B)*/
#define PROTOCOL_CRC_DATA_POS(payload_len)			(CRC_CALC_START_POS + CRC_CALC_LEN(payload_len))

#define PAYLOAD_POS						5

#define SINGLE_DATA_BYTES				4

/*data id define*/
#define ACCEL_ID				(uint8_t)0x10
#define ANGLE_ID				(uint8_t)0x20
#define MAGNETIC_ID				(uint8_t)0x30     /*归一化值*/
#define RAW_MAGNETIC_ID			(uint8_t)0x31     /*原始值*/
#define EULER_ID				(uint8_t)0x40
#define QUATERNION_ID			(uint8_t)0x41
#define UTC_ID					(uint8_t)0x50
#define LOCATION_ID				(uint8_t)0x60
#define SPEED_ID				(uint8_t)0x70

/*length for specific data id*/
#define ACCEL_DATA_LEN			(uint8_t)12
#define ANGLE_DATA_LEN			(uint8_t)12
#define MAGNETIC_DATA_LEN		(uint8_t)12
#define MAGNETIC_RAW_DATA_LEN	(uint8_t)12
#define EULER_DATA_LEN			(uint8_t)12
#define QUATERNION_DATA_LEN		(uint8_t)16
#define UTC_DATA_LEN			(uint8_t)11
#define LOCATION_DATA_LEN		(uint8_t)12
#define SPEED_DATA_LEN          (uint8_t)12

/*factor for sensor data*/
#define NOT_MAG_DATA_FACTOR			0.000001f
#define MAG_RAW_DATA_FACTOR			0.001f

/*factor for gnss data*/
#define LONG_LAT_DATA_FACTOR		0.0000001
#define ALT_DATA_FACTOR				0.001f
#define SPEED_DATA_FACTOR			0.001f
//帧结构
#define IMU_FIRST_HEADER		0
#define IMU_SECOND_HEADER		1
#define IMU_TIME_ID_L			2
#define IMU_TIME_ID_H			3
#define IMU_LEN					4
#define	IMU_DATA				5

//odo数据长度

/*------------------------------------------------Variables define------------------------------------------------*/
protocol_info_t g_output_info;

// 接收状态机
uint8_t usart6_rxflag = 0;
// 命令接收缓存
uint8_t usart6_rxbuf[BUF_MAX_LEN] = {0};
uint8_t usart6_txbuf[BUF_MAX_LEN] = {0};
// 接收数据下标
uint8_t usart6_rxindex = 0;
// 新命令数据长度
uint8_t usart6_rxlen = 0;
// 新命令接收标志
uint8_t usart6_cmdflag = 0;
// 组帧后的帧长
uint8_t frame_len;
uint8_t imu_data_buf[64] = {0};

extern uint8_t usart1_txbuf[BUF_MAX_LEN];
/*------------------------------------------------Functions declare------------------------------------------------*/
int get_signed_int(const uint8_t *data);
int calc_checksum(const uint8_t *data, uint16_t len, uint16_t *checksum);

/*-------------------------------------------------------------------------------------------------------------*/
// 获取接收的数据
uint8_t* get_usart6rxbuf(void)
{
	return (uint8_t*)usart6_rxbuf;
}

// 获取命令长度
uint8_t get_usart6rxlen(void)
{
	return usart6_rxlen;
}

// 获取命令标志
uint8_t is_rcv_usart6cmd(void)
{
	return usart6_cmdflag;
}
// 清除命令数据和相关标志
void clear_usart6cmd(void)
{

	for (uint8_t i = 0; i < usart6_rxlen; i++)
		usart6_rxbuf[i] = 0;
	usart6_rxflag = 0;
	usart6_rxindex = 0;
	usart6_cmdflag = 0;
}

uint8_t check_data_len_by_id(uint8_t id, uint8_t len, const uint8_t *data)
{
	uint8_t ret = 0xff;

	switch(id)
	{
		case ACCEL_ID:
		{
			if(ACCEL_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.accel_x = get_signed_int(data) * NOT_MAG_DATA_FACTOR;
				g_output_info.accel_y = get_signed_int(data + SINGLE_DATA_BYTES) * NOT_MAG_DATA_FACTOR;
				g_output_info.accel_z = get_signed_int(data + SINGLE_DATA_BYTES * 2) * NOT_MAG_DATA_FACTOR;
//				printf("accel_x:%f, accel_y:%f, accel_z:%f\n",
//						g_output_info.accel_x, g_output_info.accel_y, g_output_info.accel_z);
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		case ANGLE_ID:
		{
			if(ANGLE_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.angle_x = get_signed_int(data) * NOT_MAG_DATA_FACTOR;
				g_output_info.angle_y = get_signed_int(data + SINGLE_DATA_BYTES) * NOT_MAG_DATA_FACTOR;
				g_output_info.angle_z = get_signed_int(data + SINGLE_DATA_BYTES * 2) * NOT_MAG_DATA_FACTOR;
//				printf("angle_x:%f, angle_y:%f, angle_z:%f\n",
//						g_output_info.angle_x, g_output_info.angle_y, g_output_info.angle_z);
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		case MAGNETIC_ID:
		{
			if(MAGNETIC_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.mag_x = get_signed_int(data) * NOT_MAG_DATA_FACTOR;
				g_output_info.mag_y = get_signed_int(data + SINGLE_DATA_BYTES) * NOT_MAG_DATA_FACTOR;
				g_output_info.mag_z = get_signed_int(data + SINGLE_DATA_BYTES * 2) * NOT_MAG_DATA_FACTOR;
//				printf("mag_x:%f, mag_y:%f, mag_z:%f\n",
//						g_output_info.mag_x, g_output_info.mag_y, g_output_info.mag_z);
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		case RAW_MAGNETIC_ID:
		{
			if(MAGNETIC_RAW_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.raw_mag_x = get_signed_int(data) * MAG_RAW_DATA_FACTOR;
				g_output_info.raw_mag_y = get_signed_int(data + SINGLE_DATA_BYTES) * MAG_RAW_DATA_FACTOR;
				g_output_info.raw_mag_z = get_signed_int(data + SINGLE_DATA_BYTES * 2) * MAG_RAW_DATA_FACTOR;
//				printf("raw_mag_x:%f, raw_mag_y:%f, raw_mag_z:%f\n",
//						g_output_info.raw_mag_x, g_output_info.raw_mag_y, g_output_info.raw_mag_z);
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		case EULER_ID:
		{
			if(EULER_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.pitch = get_signed_int(data) * NOT_MAG_DATA_FACTOR;
				g_output_info.roll = get_signed_int(data + SINGLE_DATA_BYTES) * NOT_MAG_DATA_FACTOR;
				g_output_info.yaw = get_signed_int(data + SINGLE_DATA_BYTES * 2) * NOT_MAG_DATA_FACTOR;
//				printf("pitch:%f, roll:%f, yaw:%f\n",
//						g_output_info.pitch, g_output_info.roll, g_output_info.yaw);
//				send_rpy(g_output_info.pitch, g_output_info.roll, g_output_info.yaw);
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		case QUATERNION_ID:
		{
			if(QUATERNION_DATA_LEN == len)
			{
				ret = (uint8_t)0x1;
				g_output_info.quaternion_data0 = get_signed_int(data) * NOT_MAG_DATA_FACTOR;
				g_output_info.quaternion_data1 = get_signed_int(data + SINGLE_DATA_BYTES) * NOT_MAG_DATA_FACTOR;
				g_output_info.quaternion_data2 = get_signed_int(data + SINGLE_DATA_BYTES * 2) * NOT_MAG_DATA_FACTOR;
				g_output_info.quaternion_data3 = get_signed_int(data + SINGLE_DATA_BYTES * 3) * NOT_MAG_DATA_FACTOR;
//				printf("quaternion_data0:%f, quaternion_data1:%f, quaternion_data2:%f, quaternion_data3:%f\n",
//						g_output_info.quaternion_data0, g_output_info.quaternion_data1,
//						g_output_info.quaternion_data2, g_output_info.quaternion_data3);
			
			}
			else
			{
				ret = (uint8_t)0x00;
			}
		}
		break;

		default:
		break;
	}

	return ret;
}
void imu_data_upload(protocol_info_t* data_t)
{
	uint8_t ret_buf[64]={0};
	//四字节拷贝
	for(int i = 0; i < 3; i++)
	{
		*(float*)(ret_buf+i*4) = *(&(data_t->accel_x)+i);
	}
	for(int i = 0; i < 3; i++)
	{
		*(float*)(ret_buf+i*4+12) = (*(&(data_t->angle_x)+i) * PI) / 180.0f;
	}
	for(int i = 0; i < 3; i++)
	{
		*(float*)(ret_buf+i*4+24) = *(&(data_t->pitch)+i);
	}
	for(int i = 0; i < 4; i++)
	{
		*(float*)(ret_buf+i*4+36) = *(&(data_t->quaternion_data0)+i);
	}
	memcpy(imu_data_buf, ret_buf, ODO_DATA_LEN);
}
/*--------------------------------------------------------------------------------------------------------------
* 输出协议为：header1(0x59) + header2(0x53) + tid(2B) + payload_len(1B) + payload_data(Nbytes) + ck1(1B) + ck2(1B)
* crc校验从TID开始到payload data的最后一个字节
*/
int usart6_analysis_cmd(const uint8_t *data, short len)
{
	uint16_t payload_len = 0;
	uint16_t check_sum = 0;
	uint16_t pos = 0;
	uint8_t ret = 0xff;
	output_data_header_t *header = NULL;
	payload_data_t *payload = NULL;

	if(NULL == data || 0 >= len)
	{
		return para_err;
	}

	if(len < PROTOCOL_MIN_LEN)
	{
			return data_len_err;
	}

	/*judge protocol header*/
	if(PROTOCOL_FIRST_BYTE == data[PROTOCOL_FIRST_BYTE_POS] && \
		PROTOCOL_SECOND_BYTE == data[PROTOCOL_SECOND_BYTE_POS])
	{
		/*further check*/
		header = (output_data_header_t *)data;//数据头地址获取
		payload_len = header->len;//有效数据长度

		if(payload_len + PROTOCOL_MIN_LEN > len)
		{//若有效数据长度加上功能数据长度大于帧长，则数据长度出错
			return 	data_len_err;
		}

		/*checksum*/
		//data + CRC_CALC_START_POS：数据帧帧号起始位置
		//CRC_CALC_LEN(payload_len)：从帧号到数据尾的数据长度
		calc_checksum(data + CRC_CALC_START_POS, CRC_CALC_LEN(payload_len), &check_sum);
		if(check_sum != *((uint16_t *)(data + PROTOCOL_CRC_DATA_POS(payload_len))))
		{
			return crc_err;
		}

		/*analysis payload data*/
		pos = PAYLOAD_POS;//有效数据从第6个数据(数据下标5)开始

		while(payload_len > 0)
		{
			//有效数据：data_id+len+data
			//有效数据的数据id内存地址
			payload = (payload_data_t *)(data + pos);
			//根据data_id解析数据
			ret = check_data_len_by_id(payload->data_id, payload->data_len, (uint8_t *)payload + 2);
			if((uint8_t)0x01 == ret)//如果解析成功
			{
				//移动有效数据指针到下一个数据包
				pos += payload->data_len + sizeof(payload_data_t);
				//未解析数据长度减去本次已解析数据包的长度
				payload_len -= payload->data_len + sizeof(payload_data_t);
			}
			else
			{//如果失败，则从下一个字节数据开始解析
				pos++;
				payload_len--;
			}
		}
//		printf("Im here\n");
		imu_data_upload(&g_output_info);
		return analysis_ok;
	}
	else
	{
		return analysis_done;
	}
}

int get_signed_int(const uint8_t *data)
{
	int temp = 0;

	temp = (int)((data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0]);

	return temp;
}

int calc_checksum(const uint8_t *data, uint16_t len, uint16_t *checksum)
{
	uint8_t check_a = 0;
	uint8_t check_b = 0;
	uint16_t i;

	if(NULL == data || 0 == len || NULL == checksum)
	{
		return -1;
	}

	for(i = 0; i < len; i++)
	{
		check_a += data[i];
		check_b += check_a;
	}

	*checksum = ((uint16_t)(check_b << 8) | check_a);

	return 0;
}



// 处理串口单字节数据并保存
void usart6_rcv(uint8_t rxdata)
{
	switch (usart6_rxflag) {
		case IMU_FIRST_HEADER: // 帧头
		{
			if (rxdata == PROTOCOL_FIRST_BYTE) {
				usart6_rxbuf[usart6_rxindex++] = PROTOCOL_FIRST_BYTE;
				usart6_rxflag = IMU_SECOND_HEADER;
			}
			break;
		}
		case IMU_SECOND_HEADER: // 帧头
		{
			if (rxdata == PROTOCOL_SECOND_BYTE) {
				usart6_rxbuf[usart6_rxindex++] = PROTOCOL_SECOND_BYTE;
				usart6_rxflag = IMU_TIME_ID_L;
			} else {
				usart6_rxflag = IMU_FIRST_HEADER;
				usart6_rxindex = 0;
				usart6_rxbuf[0] = 0x00;
			}
			break;
		}
		case IMU_TIME_ID_L:// 标识位
		{
			usart6_rxbuf[usart6_rxindex++] = rxdata;
			usart6_rxflag = IMU_TIME_ID_H;
			break;
		}
		case IMU_TIME_ID_H:// 标识位
		{
			usart6_rxbuf[usart6_rxindex++] = rxdata;
			usart6_rxflag = IMU_LEN;
			break;
		}
		case IMU_LEN:// 数据位长度
		{
			// usart6_rxlen为数据帧总字节数 = 帧头(2)+标识位(2)+长度(1)+数据位(nbytes)+校验位(2)
			usart6_rxlen = rxdata + 7;
			if (usart6_rxlen < BUF_MAX_LEN) {
				usart6_rxbuf[usart6_rxindex++] = rxdata;
				usart6_rxflag = IMU_DATA;
			} else {
				usart6_rxflag = IMU_FIRST_HEADER;
				usart6_rxindex = 0;
				usart6_rxbuf[0] = 0;
				usart6_rxbuf[1] = 0;
				usart6_rxbuf[2] = 0;
				usart6_rxbuf[3] = 0;
				usart6_rxlen = 0;
			}
			break;
		}
		case IMU_DATA:// 读取完剩余的所有字段
		{
			usart6_rxbuf[usart6_rxindex++] = rxdata;
			if(usart6_rxindex >= usart6_rxlen){
				usart6_cmdflag = 1;
				usart6_rxflag = IMU_FIRST_HEADER;
				usart6_rxindex = 0;
			}
			break;
		}
		default:
			clear_usart6cmd();
			break;
	}
}

