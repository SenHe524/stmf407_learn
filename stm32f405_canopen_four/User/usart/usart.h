#ifndef __USART_H
#define __USART_H


#include "sys.h" 
#include "main.h"	
void USART1_Init(u32 bound);
void USART1_sendbuf(uint8_t* data, uint8_t len);

#endif
