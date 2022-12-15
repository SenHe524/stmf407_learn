#ifndef __PROTOCOL_H
#define __PROTOCOL_H


#include "main.h"	

#define FRAME_HEADER			0
#define FRAME_FUNC				1
#define FRAME_LEN				2
#define	FRAME_DATA				3

#define FIRST_CODE 0x55
#define END_CODE 0xBB


#define VELOCITY_MAX			600

#define SET_ENABLE				0x01
#define SET_DISENABLE			0x02
#define IS_ENABLE				0x03
#define CLEAR_FAULT				0x04
#define IS_FAULT				0x05
#define SET_STOP				0x06
#define STOP_TO_ENABLE			0x07
#define SPEED					0x08
#define SET_PARAM				0x09
#define GET_PARAM				0x0A
#define ODOMETRY				0x0B
#define IMU						0x0C


#define ENABLE_CHECKSUM			1


uint8_t* get_usart1rxbuf(void);
// 获取命令长度
uint8_t get_usart1rxlen(void);
// 获取命令标志
uint8_t is_rcv_usart1cmd(void);
// 清除命令数据和相关标志
void clear_usart1cmd(void);
// 封装数据帧
int usart1frame_packing(const uint8_t *buf, uint8_t *frame, uint8_t len, uint8_t func);
// 接收串口单字节数据接收并保存
void usart1_rcv(uint8_t rxdata);
// 指令解析，传入接收到的完整指令，及其长度
void usart1_analysis_cmd(const uint8_t* data_buf, uint8_t len);
void Odometry_data(const uint8_t* imudata, uint8_t len);
#endif
