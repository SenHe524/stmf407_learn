#include "myspi.h"
#include "stm32f4xx.h"                  // Device header

void MySpi_Init(void)
{
	// 定义配置结构体,开启时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	
	// 引脚复用
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// 引脚复用
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// 推完输出
	GPIO_InitStructure.GPIO_Pin = SPI3_SCK_PIN | SPI3_MISO_PIN | SPI3_MOSI_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//  波特率预分频值：256
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// 第二个时钟沿采样
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 串行同步时钟空闲状态为低电平
	SPI_InitStructure.SPI_CRCPolynomial = 7;// CRC校验
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 双向全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 高位在前
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// 主模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 软件拉高拉低片选线
	SPI_Init(SPI3, &SPI_InitStructure);
	// SPI外设使能
	SPI_Cmd(SPI3, ENABLE);
}

// 寄存器方式
uint8_t SPI3ReadWriteByte_reg(u8 add,u8 data)
{
    while((SPI3->SR & 0x0002) == 0);
    SPI3->DR = (add << 8 | data);
    while((SPI3->SR & 0x0001) == 0);
    return (uint8_t)(SPI_I2S_ReceiveData(SPI3));                                 // return (unsigned char)SPI_ReceiveData(SPI2);
}

// 库函数方式
uint8_t SPI3ReadWriteByte_lib(u8 data)
{

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) ;
	SPI_I2S_SendData(SPI3, data);
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) ;
    
	return (uint8_t)(SPI_I2S_ReceiveData(SPI3));                                 // return (unsigned char)SPI_ReceiveData(SPI2);
}

void SPI3_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	
	SPI3->CR1 &= 0XFFC7; //&1111 1111 1100 0111 把D3~D4清零
	SPI3->CR1 |= SPI_BaudRatePrescaler; //这里是把SPI_BaudRatePrescaler的值与SPI2->CR1中的值按2进制的方式相加
	//如果SPI_BaudRatePrescaler的值为0x038，二进制数为00111000与1111 1111 1100 0111相或后结果是1111 1111 1111 1111->0XFFFF; 
	
	SPI_Cmd(SPI3, ENABLE);

}

