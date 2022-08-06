#ifndef __PORT_FUNC_H
#define __PORT_FUNC_H                

/***************----- 网络参数变量定义 -----***************/
#define Gateway_IP  	{192,168,1,1};	//网关IP地址 
#define Sub_Mask  		{255,255,255,0};	//子网掩码 
#define Phy_Addr  		{0x00,0x08,0xDC,0x01,0x02,0x03};	//物理地址(MAC) 
#define IP_Addr  		{192,168,1,199};	//本机IP地址 
//#define S0_Port  		{0x1E,0x40};	//端口0的端口号(5000) 
#define DNS 			{8,8,8,8};

//#define S0_DIP 			{192,168,1,199};		//端口0目的IP地址 
//#define S0_DPort 		{0x1F,0x40};	//端口0目的端口号(6000) 

/* 定义该宏则表示在初始化网络信息时设置DHCP */
//#define USE_DHCP

void SPI3_Write_Byte(unsigned char dat);// 单字节写

unsigned char SPI3_Read_Byte(void);// 单字节读

void Spi_ReadBurst(uint8_t* pBuf, uint16_t len);// 多字节读

void Spi_WriteBurst(uint8_t* pBuf, uint16_t len);// 多字节写

void SPI3_CS_Select(void);//片选函数

void SPI3_CS_Deselect(void);//片选取消函数

void SPI_CrisEnter(void);//进入临界区函数

void SPI_CrisExit(void);//退出临界区函数

void W5500_Hardware_Reset(void);

int w5500_init(void);

#endif


