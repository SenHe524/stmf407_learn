///***************************************************************************************

// * 硬件连接：  PF11 -> W5500_SPI3_RST
// *             PF12 -> W5500_SPI3_INT
// *             PF13 -> W5500_SPI3_SCS
// *             PC10 -> W5500_SPI3_SCK_PIN
// *             PC11 -> W5500_SPI3_MISO_PIN
// *             PC12 -> W5500_SPI3_MOSI_PIN

//***************************************************************************************/

#include "stm32f4xx.h" // Device header

//#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "timer.h"
//#include "W5500.h"
//#include "myspi.h"
#include "port_func.h"
#include "socket.h"
#include "ethercatcontrol.h"

#include "osal.h"
#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "ethercatdc.h"
#include "osal_defs.h"

//uint8_t destip[4] = {192, 168, 1, 190};
//uint16_t destport = 8000;
uint8_t temp[] = "0123456789";
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // GPIO配置结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	// 开启外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// GPIO初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//int main(void)
//{
//	int argc = 0;
//	char argv[2] = "ss";
//	
//	Timer2And3_Init();
//	delay_init();
//	Led_Init();
//	Timer2And3_start();
//	// 绑定w5500读取写入函数，硬件重置w5500，写入网络参数
//	w5500_init();
//	w5500_RegSetup();
//	
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
//ec_adaptert * adapter = NULL;
////   printf("SOEM (Simple Open EtherCAT Master)\nSlaveinfo\n");

//   if (argc > 1)
//   {
//      if ((argc > 2) && (strncmp(argv[2], "-sdo", sizeof("-sdo")) == 0)) printSDO = TRUE;
//      if ((argc > 2) && (strncmp(argv[2], "-map", sizeof("-map")) == 0)) printMAP = TRUE;
//      /* start slaveinfo */
//      strcpy(ifbuf, argv[1]);
//      slaveinfo(ifbuf);
//   }
//   else
//   {
////      printf("Usage: slaveinfo ifname [options]\nifname = eth0 for example\nOptions :\n -sdo : print SDO info\n -map : print mapping\n");
//   	/* Print the list */
////      printf ("Available adapters\n");
//      adapter = ec_find_adapters ();
//      while (adapter != NULL)
//      {
////         printf ("Description : %s, Device to use for wpcap: %s\n", adapter->desc,adapter->name);
//         adapter = adapter->next;
//      }
//   }
//}


