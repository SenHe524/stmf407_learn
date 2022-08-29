#include <inttypes.h>

#include "stm32f4xx.h" // Device header

#include <string.h>
#include <stdio.h>

#include "delay.h"
#include "timer.h"
#include "port_func.h"
#include "ethercat.h"

int dorun=0;
int iomap_size;

#define EC_TIMEOUTMON 500

char IOmap[256] = {0}; //映射空间

int Servosetup(ecx_contextt *context, uint16 slave)
{
	uint8 u8val;
	uint16 u16val;
	uint32 u32val;
	
	// 0x1600-0x17FF:RxPDO映射
	u8val = 0;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1c12数据
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1600数据
	ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1601数据
	
	u32val = 0x60400010;
	ec_SDOwrite(slave, 0x1600, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x60600008;
	ec_SDOwrite(slave, 0x1600, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x607A0020;
	ec_SDOwrite(slave, 0x1600, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u32val = 0x60710010;
	ec_SDOwrite(slave, 0x1600, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1600子索引
	u8val = 4;
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u32val = 0x68400010;
	ec_SDOwrite(slave, 0x1610, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x68600008;   
	ec_SDOwrite(slave, 0x1610, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x687A0020;   
	ec_SDOwrite(slave, 0x1610, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u32val = 0x68710010;   
	ec_SDOwrite(slave, 0x1610, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);//增加0x1601子索引
	u8val = 4;             
	ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u16val = 0x1600;
	ec_SDOwrite(slave, 0x1c12, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u16val = 0x1610;
	ec_SDOwrite(slave, 0x1c12, 0x02, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	
	u8val = 2;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//启用PDO配置

	/**********************************************************************************************************/
	
	// 0x1A00-0x1BFF:TxPDO映射，结构体类型
	u8val = 0;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1c13数据
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1A00数据
	ec_SDOwrite(slave, 0x1A10, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//清除0x1A01数据
	
	u32val = 0x60410010;
	ec_SDOwrite(slave, 0x1A00, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60770010;
	ec_SDOwrite(slave, 0x1A00, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60640020;
	ec_SDOwrite(slave, 0x1A00, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x606C0020;
	ec_SDOwrite(slave, 0x1A00, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x603F0010;
	ec_SDOwrite(slave, 0x1A00, 0x05, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x60610008;
	ec_SDOwrite(slave, 0x1A00, 0x06, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u8val = 6;
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u32val = 0x68410010;
	ec_SDOwrite(slave, 0x1A10, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68770010;   
	ec_SDOwrite(slave, 0x1A10, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68640020;   
	ec_SDOwrite(slave, 0x1A10, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x686C0020;   
	ec_SDOwrite(slave, 0x1A10, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x683F0010;   
	ec_SDOwrite(slave, 0x1A10, 0x05, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u32val = 0x68610008;   
	ec_SDOwrite(slave, 0x1A10, 0x06, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTTXM);
	u8val = 6;             
	ec_SDOwrite(slave, 0x1A10, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//设定为PDO映射中的映射数
	
	u16val = 0x1A00;
	ec_SDOwrite(slave, 0x1c13, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u16val = 0x1A10;
	ec_SDOwrite(slave, 0x1c13, 0x02, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTTXM);//设定为所选的PDO映像配置
	u8val = 2;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);//启用PDO配置
	
	//	配置为周期性同步位置模式：见pdf112
//	u8val = 8;
//	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTTXM);
	return 1;
} 

int Soem_init(char *ifname)
{
	int slc, i;
	if(!ec_init(ifname))//初始化soem,将主站绑定到网口
	{
		printf("No socket connection on %s\nExecute as root\n", ifname);
		return 0;
	}
	printf("ec_init on %s succeeded.\n", ifname);
	if(ec_config_init(FALSE) <= 0)
	{
		printf("No slaves found!\n");
		return 0;
	}
	printf("%d slaves found and configured.\n", ec_slavecount);
	for(slc = 1; slc <= ec_slavecount; slc++)
	{
		if((ec_slave[slc].eep_man == 0x0000001A) && (ec_slave[slc].eep_id == 0x50440200))
		{
			int pbuff = {0};
			int sz = sizeof(pbuff);
			ec_SDOread(slc, 0x6502, 0x00, FALSE, &sz, &pbuff, EC_TIMEOUTRXM);
			// link slave specific setup to preop->safeop hook
			ec_slave[slc].PO2SOconfigx = &Servosetup;
		}
	}
    iomap_size = ec_config_map(&IOmap); //主站内存空间与从站内存空间映射
    ec_configdc(); //配置DC
	for(slc = 1; slc <= ec_slavecount; slc++)
	{
		ec_dcsync0(slc, TRUE, 1000000, 0);
	}
	printf("Slaves mapped.\n");
	//等待所有从机进入安全运行状态
	ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);

	printf("Request operational state for all slaves\n");
	ec_slave[0].state = EC_STATE_OPERATIONAL;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	ec_writestate(0);//	写入EC_STATE_OPERATIONAL状态
	//	等待所有从机进入运行状态
	ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
	ec_readstate();// 更新所有从机状态
	if (ec_slave[0].state == EC_STATE_OPERATIONAL )
	{
		printf("Operational state reached for all slaves.\n");
		dorun=1;
		Timer5_Init();
	}
	else
	{
		printf("Not all slaves reached operational state.\n");
		ec_readstate();
		for(i = 1; i<=ec_slavecount ; i++)
		{
			if(ec_slave[i].state != EC_STATE_OPERATIONAL)
			{
				printf("Slave %d State=0x%04x StatusCode=0x%04x\n",i, ec_slave[i].state, ec_slave[i].ALstatuscode);
			}
		}
	}
	(void) iomap_size;
	return 1;
}

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
void System_init(void)
{
	Timer2And3_Init();
    Timer2And3_start();

    delay_init();
    Led_Init();

    // 绑定w5500读取写入函数，硬件重置w5500，写入网络参数
    w5500_init();
    w5500_RegSetup();
    // 延时以保证能扫描到从机
    delay_ms(300);
}

//Rx_PDO_t *outputs[3];
//Tx_PDO_t *inputs[3];
int flag_led = 0;
int Test(char *ifname)
{
	int j = 1;
	//初始化soem,将主站绑定到网口 使从机运行到EC_STATE_OPERATIONAL状态
	if (!Soem_init(ifname))	
	{
		flag_led = 0;
		return 0;
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	while(j)
	{
		

	}
	

	/* stop SOEM, close socket */
	ec_slave[0].state = EC_STATE_INIT;
	ec_writestate(0);//	写入EC_STATE_INIT状态
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	//	等待所有从机进入初始化状态
	if(ec_statecheck(0, EC_STATE_INIT, EC_TIMEOUTSTATE) != EC_STATE_INIT)	return 0;
	ec_readstate();// 更新所有从机状态
	
	ec_close();
	flag_led = 1;
	return 1;
}

int main(void)
{	
	System_init(); 
	Test("T");

    while (1)
    {
		if(!flag_led)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			delay_ms(500);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			delay_ms(500);
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			delay_ms(1500);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			delay_ms(1500);
		}
		
    }
}
