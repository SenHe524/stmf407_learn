#ifndef __USART_H
#define __USART_H


#include "sys.h" 
#include "main.h"	
void usart1_init_bsp(u32 bound);
void usart6_init_bsp(u32 bound);
void usart1_sendbuf(const uint8_t* data, uint8_t len);
void usart6_sendbuf(const uint8_t* data, uint8_t len);

#endif
