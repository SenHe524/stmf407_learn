#ifndef __USART_H
#define __USART_H


#include "sys.h" 
#include "main.h"

void usart1_init_bsp(u32 bound);
void usart_sendbuf(uint8_t* data, uint8_t len);

#endif
