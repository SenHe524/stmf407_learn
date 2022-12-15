#ifndef __MYDMA_H
#define __MYDMA_H                
void usart1_init_bsp(u32 bound);
void usart1_rxdma_init_bsp(void);
void usart1_txdma_init_bsp(void)；
//DMA发送
void usart1_sendbuf(const uint8_t* data, uint8_t len)；
//DMA中断读取
void USART1_IRQHandler(void)；                	//串口1中断服务程序
#endif
