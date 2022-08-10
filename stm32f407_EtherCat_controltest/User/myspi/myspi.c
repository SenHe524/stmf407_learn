#include "stm32f4xx.h"                  // Device header
#include "myspi.h"


/*******************************************************************************
* 函数名  : SPI_Configuration
* 描述    : W5500 SPI初始化配置(STM32 SPI3)
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void SPI_Configuration(void)
{
			// 定义配置结构体,开启时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	
	// 引脚复用
	GPIO_PinAFConfig(W5500_SPI3_PORT, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(W5500_SPI3_PORT, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(W5500_SPI3_PORT, GPIO_PinSource12, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(W5500_SPI3_SCS_PORT, GPIO_PinSource4, GPIO_AF_SPI3);
	/* 初始化SCK */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// 引脚复用
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// 推挽输出
	GPIO_InitStructure.GPIO_Pin = W5500_SPI3_SCK_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;// 下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W5500_SPI3_PORT, &GPIO_InitStructure);
	
	/* MISO、MOSI引脚 */
	GPIO_InitStructure.GPIO_Pin = W5500_SPI3_MISO_PIN | W5500_SPI3_MOSI_PIN;
	GPIO_Init(W5500_SPI3_PORT, &GPIO_InitStructure);
	
	/* 初始化SCS引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;// 引脚复用	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// 推挽输出
	GPIO_InitStructure.GPIO_Pin = W5500_SPI3_SCS;
	GPIO_Init(W5500_SPI3_SCS_PORT, &GPIO_InitStructure);
	GPIO_SetBits(W5500_SPI3_SCS_PORT, W5500_SPI3_SCS);
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//  波特率预分频值：2
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// 第二个时钟沿采样
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


/*******************************************************************************
* 函数名  : W5500_GPIO_Configuration
* 描述    : W5500 GPIO初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void W5500_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	/* W5500_SPI3_RST引脚初始化配置(PF11) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// 推挽输出
	GPIO_InitStructure.GPIO_Pin = W5500_SPI3_RST;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(W5500_SPI3_IR_PORT, &GPIO_InitStructure);
	GPIO_SetBits(W5500_SPI3_IR_PORT, W5500_SPI3_RST);
	
	/* W5500_INT引脚初始化配置(PF12) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = W5500_SPI3_INT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(W5500_SPI3_IR_PORT, &GPIO_InitStructure);	

	/* Connect EXTI Line12 to PF12 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource12);
	
	/* PC4 as W5500 interrupt input */
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*
	抢占优先级高的可以打断当前中断，实现中断嵌套，响应优先级
	高的可以优先排队，抢占优先级和响应优先级相同的按中断号排队
	*/
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the EXTI4 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* 函数名  : EXTI4_IRQHandler
* 描述    : 中断线4中断服务函数(W5500 INT引脚中断服务函数)
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);

	}
}

void SPI3_Write_Byte(unsigned char dat)
{
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//等待数据寄存器空
	SPI_I2S_SendData(SPI3,dat);//写1个字节数据
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI3);
}
unsigned char SPI3_Read_Byte(void)
{
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//等待数据寄存器空
	SPI_I2S_SendData(SPI3,0xff);//写1个字节数据
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	return (unsigned char)SPI_I2S_ReceiveData(SPI3);
}
void Spi_ReadBurst(uint8_t* pBuf, uint16_t len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		pBuf[i] = SPI3_Read_Byte();
	}
}

void Spi_WriteBurst(uint8_t* pBuf, uint16_t len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		SPI3_Write_Byte(*pBuf++);//写1个字节数据
	}
}

//片选函数
void SPI3_CS_Select(void)
{
    GPIO_ResetBits(W5500_SPI3_SCS_PORT,W5500_SPI3_SCS);
}
//片选取消函数
void SPI3_CS_Deselect(void)
{
    GPIO_SetBits(W5500_SPI3_SCS_PORT,W5500_SPI3_SCS);
}

//进入临界区函数
void SPI_CrisEnter(void)
{
    __set_PRIMASK(1);
}
//退出临界区函数
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}


