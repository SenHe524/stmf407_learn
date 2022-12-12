#ifndef __HARDWARE_DRIVER_H
#define __HARDWARE_DRIVER_H

#include "main.h"
void delay_init(u8 sysclk, void (*userdelay_init)(u8));
void timer_init(void(*time_init)(void));
void usart_init(u32 bound, void (*userusart_init)(u32));
void can_init(CO_Data * d, unsigned char (*usercan_init)(CO_Data *));

void delay_ms(u16 nms);
void delay_us(u32 nus);
void usart_sendbuf(uint8_t* data, uint8_t len);
unsigned char canSend(CAN_PORT notused, Message *m);


#endif 

