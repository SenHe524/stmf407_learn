#ifndef __MYSPI_H
#define __MYSPI_H                

/***************----- W5500 GPIO定义 -----***************/
#define W5500_SPI3_PORT             GPIOC
#define W5500_SPI3_SCK_PIN          GPIO_Pin_10
#define W5500_SPI3_MISO_PIN         GPIO_Pin_11
#define W5500_SPI3_MOSI_PIN         GPIO_Pin_12

#define W5500_SPI3_SCS_PORT			GPIOF
#define W5500_SPI3_SCS				GPIO_Pin_13	//定义W5500的CS引脚

#define W5500_SPI3_IR_PORT			GPIOF
#define W5500_SPI3_INT				GPIO_Pin_12	//定义W5500的INT引脚
#define W5500_SPI3_RST				GPIO_Pin_11	//定义W5500的RST引脚

void SPI_Configuration(void);
void W5500_GPIO_Configuration(void);void EXTI15_10_IRQHandler(void);

void SPI3_Write_Byte(unsigned char dat);// 单字节写

unsigned char SPI3_Read_Byte(void);// 单字节读

void Spi_ReadBurst(uint8_t* pBuf, uint16_t len);// 多字节读

void Spi_WriteBurst(uint8_t* pBuf, uint16_t len);// 多字节写

void SPI3_CS_Select(void);//片选函数

void SPI3_CS_Deselect(void);//片选取消函数

void SPI_CrisEnter(void);//进入临界区函数

void SPI_CrisExit(void);//退出临界区函数


#endif

