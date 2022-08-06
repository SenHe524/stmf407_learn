#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_REC_LEN		200		//�����������ֽ���
#define EN_USART2_RX		1		//ʹ�ܣ�1��/��ֹ��0������2����

extern u8  USART_RX_BUF[USART_REC_LEN];		//���ջ��壬���USART_REC_LEN���ֽڡ�ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;					//����״̬��־

void uart_init(u32 bound);

#endif
