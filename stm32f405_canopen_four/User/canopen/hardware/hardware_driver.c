#include "hardware_driver.h"

void delay_init(u8 sysclk, void (*userdelay_init)(u8))
{
	userdelay_init(sysclk);
}
void timer_init(void(*usertimer_init)(void))
{
	usertimer_init();
}

void usart_init(u32 bound, void (*userusart_init)(u32))
{
	userusart_init(bound);
}
void can_init(CO_Data * d, unsigned char (*usercan_init)(CO_Data *))
{
	usercan_init(d);
}

__weak void delay_ms(u16 nms)
{
	//	必须实现自己的ms延时
}

__weak void delay_us(u32 nus)
{
	//	必须实现自己的us延时
}

__weak void usart1_sendbuf(uint8_t* data, uint8_t len)
{
	//	必须实现自己的串口发送函数
}

__weak void usart6_sendbuf(uint8_t* data, uint8_t len)
{
	//	必须实现自己的串口发送函数
}

__weak unsigned char canSend(CAN_PORT notused, Message *m)
{
	//	必须实现自己的can发送函数
	return 0;//success
}




