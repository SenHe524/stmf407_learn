#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_REC_LEN		200		//定义最大接收字节数
#define EN_USART2_RX		0		//使能（1）/禁止（0）串口2接收

extern u8  USART_RX_BUF[USART_REC_LEN];		//接收缓冲，最大USART_REC_LEN个字节。末字节为换行符
extern u16 USART_RX_STA;					//接收状态标志

void uart_init(u32 bound);

#endif
