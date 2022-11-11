#ifndef __PROTOCOL_H
#define __PROTOCOL_H


#include "main.h"	

#define BUF_MAX_LEN			64

#define FRAME_HEADER		0
#define FRAME_FUNC			1
#define FRAME_LEN			2
#define	FRAME_DATA			3

#define FIRST_CODE 0x55
#define END_CODE 0xBB


#define VELOCITY_MAX		600

#define FUNC_CONTROL		0x01
#define FUNC_SPEED			0x02
#define FUNC_PARAM			0x03
#define ENABLE_CHECKSUM		1

typedef union 
{
    int8_t data_int8;
    unsigned char data8;
}union_int8;

typedef union 
{
    uint16_t data_uint16;
    unsigned char data8[2];
}union_uint16;


typedef union 
{
    uint32_t data_int32;
    unsigned char data8[4];
}union_int32;


uint8_t* get_rxbuf(void);
// 获取命令长度
uint8_t get_rxlen(void);
// 获取命令标志
uint8_t is_rcv_cmd(void);
// 清除命令数据和相关标志
void clear_cmd(void);
// 封装数据帧
int frame_packing(const uint8_t *buf, uint8_t *frame, uint8_t len, uint8_t func);
// 接收串口单字节数据接收并保存
void usart_rcv(uint8_t rxdata);
// 指令解析，传入接收到的完整指令，及其长度
void analysis_cmd(const uint8_t *data_buf, uint8_t len);

#endif
