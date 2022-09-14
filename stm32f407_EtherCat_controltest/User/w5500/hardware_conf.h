#ifndef __HARDWARE_CONF_H
#define __HARDWARE_CONF_H                

/***************----- 网络参数变量定义 -----***************/
#define Gateway_IP  	{192,168,1,1};	//网关IP地址 
#define Sub_Mask  		{255,255,255,0};	//子网掩码 
#define Phy_Addr  		{0x00,0x08,0xDC,0x01,0x02,0x03};	//物理地址(MAC) 
#define IP_Addr  		{192,168,1,199};	//本机IP地址 
//#define S0_Port  		{0x1E,0x40};	//端口0的端口号(5000) 
#define Sn_DHAR_a		{0x00,0x28,0xEC,0x06,0x01,0x07}

#define S0_DIPR 		{192,168,1,199};		//端口0目的IP地址 
#define S0_DPort 		{0x1F,0x40};	//端口0目的端口号(6000) 

/* 定义该宏则表示在初始化网络信息时设置DHCP */
//#define USE_DHCP

void W5500_Hardware_Reset(void);

void w5500_init(void);

int w5500_RegSetup(void);

#endif


