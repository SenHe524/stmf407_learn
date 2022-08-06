#ifndef __MYSPI_H
#define __MYSPI_H                
#include "stm32f4xx.h"                  // Device header

#define SPI3_PORT               GPIOC
#define SPI3_SCK_PIN            GPIO_Pin_10
#define SPI3_MISO_PIN           GPIO_Pin_11
#define SPI3_MOSI_PIN           GPIO_Pin_12

// SPI初始化
void MySpi_Init(void);
// 寄存器方式
uint8_t SPI2ReadWriteByte_reg(u8 add,u8 data);
// 库函数方式
uint8_t SPI3ReadWriteByte_lib(u8 data);

void SPI3_SetSpeed(uint8_t SPI_BaudRatePrescaler);
	
#endif
