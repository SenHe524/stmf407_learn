#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "string.h"
#include "myspi.h"
#include "port_func.h"
#include "wizchip_conf.h"
#include "delay.h"



// w5500硬件复位
void W5500_Hardware_Reset(void)
{
	GPIO_ResetBits(W5500_SPI3_IR_PORT, W5500_SPI3_RST);//复位引脚拉低
	delay_ms(50);
	GPIO_SetBits(W5500_SPI3_IR_PORT, W5500_SPI3_RST);//复位引脚拉高
	delay_ms(200);
	while(wizphy_getphylink() != 1);//等待以太网连接完成
}

/**
 * @brief   Initializes WIZCHIP with socket buffer size
 * @param   none
 * @return  errcode
 * @retval  0   success
 * @retval  -1  fail
 */
int w5500_chip_init(void)
{
    /* default size is 2KB */
    return wizchip_init(NULL, NULL);
}

/**
 * @brief   set phy config if autonego is disable
 * @param   none
 * @return  none
 */
void w5500_phy_init(void)
{

    wiz_PhyConf conf;
    
    conf.by = PHY_CONFBY_SW;
    conf.mode = PHY_MODE_MANUAL;
    conf.speed = PHY_SPEED_100;
    conf.duplex = PHY_DUPLEX_FULL;
    
    wizphy_setphyconf(&conf);
}

/**
 * @brief   initializes the network infomation
 * @param   none
 * @return  none
 */
void w5500_network_info_init(void)
{
    wiz_NetInfo info;
    wiz_NetInfo info1;
	unsigned char j = getVERSIONR();
    uint8_t mac[6] = Phy_Addr;
    uint8_t ip[4] = IP_Addr;
    uint8_t sn[4] = Sub_Mask;
    uint8_t gw[4] = Gateway_IP;
    uint8_t dns[4] = DNS;

    memcpy(info.mac, mac, 6);
    memcpy(info.ip, ip, 4);
    memcpy(info.sn, sn, 4);
    memcpy(info.gw, gw, 4);
    memcpy(info.dns, dns, 4);
    
#ifdef USE_DHCP
    info.dhcp = NETINFO_DHCP;
#else
    info.dhcp = NETINFO_STATIC;
#endif
    wizchip_setnetinfo(&info);
	wizchip_getnetinfo(&info1);
	(void ) j;
}

/**
 * @brief   w5500 init
 * @param   none
 * @return  errcode
 * @retval  0   success
 * @retval  -1  chip init fail
 */
int w5500_init(void)
{
	unsigned char j = 0x99;
    /* Register spi driver function */
    reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);
    reg_wizchip_cs_cbfunc(SPI3_CS_Select, SPI3_CS_Deselect);
    reg_wizchip_spi_cbfunc(SPI3_Read_Byte, SPI3_Write_Byte);
	reg_wizchip_spiburst_cbfunc(Spi_ReadBurst, Spi_WriteBurst);
	/* W5500 hard reset */
    W5500_Hardware_Reset();
//	j = getPHYCFGR();
	
    /* socket buffer size init */
    if (w5500_chip_init() != 0) {
        return -1;
    }
    
    /* phy init */
    w5500_phy_init();
//    j = getPHYCFGR();
	
    /* network infomation init */
    w5500_network_info_init();
	
    (void) j;
    return 0;
}

