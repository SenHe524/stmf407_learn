// Includes for the Canfestival driver
#include "CANopen.h"

void tr_pdo_mapping(void)
{
	RPDO1_Config(0x04);
	delay_ms(10);
	RPDO2_Config(0x03);
	delay_ms(10);
	TPDO1_Config(0x04);
	delay_ms(10);
	TPDO2_Config(0x03);
	delay_ms(10);
}

//保存新写入功能码参数配置至EEPROM
uint8_t Save_EEPROM(void)
{
	Message mes = {0x604,0,8,{0x2B,0x10,0x20,0x00,0x00,0x00,0x00,0x00}};

	return canSend(CAN1, &mes);
}

//1:恢复出厂配置
//2:保存所有RW属性的参数到EEPROM
uint8_t Reset_Save(uint8_t ID, uint8_t select)
{
	Message mes = {0x600+ID,0,8,{0x2B,0x09,0x20,0x00,select,0x00,0x00,0x00}};

	return canSend(CAN1, &mes);
}

//NMT发送
//0x01 node-id：开启node-id号节点驱动PDO传输（让节点进入操作状态）
//0x02 node-id：停止node-id号节点驱动PDO传输（让节点进入停止状态）
//0x80 node-id：让node-id号节点进入预操作状态
//0x81 node-id：复位node-id号节点应用层
//0x82 node-id：复位node-id号节点通讯
// node-id为零时，即对所有节点进行控制
uint8_t NMT_Control(uint8_t cs, uint8_t nodeid)
{
	Message mes = {0x000,0,2,{cs,nodeid}};

	return canSend(CAN1, &mes);
}


uint8_t RPDO1_Config(uint8_t ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x23,0x00,0x14,0x01,0x00+ID,0x02,0x00,0x80}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x00,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  失能RPDO0
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置RPDO的传输方式为事件触发
	pdo_para_mes.data[0] = 0x2F;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x02;
	pdo_para_mes.data[4] = 0xFE;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空RPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6040：controlword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x01;
	pdo_map_mes.data[4] = 0x10;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x40;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6060：mode of operation
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x60;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射607A：target position
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x00;
//	pdo_map_mes.data[2] = 0x16;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x7A;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	映射60FF：target velocity
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0xFF;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//  使能RPDO
	pdo_para_mes.data[0] = 0x23;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x01;
	pdo_para_mes.data[4] = 0x00+ID;
	pdo_para_mes.data[5] = 0x02;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	return 0x00;
}


uint8_t RPDO2_Config(uint8_t ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x23,0x01,0x14,0x01,0x00+ID,0x03,0x00,0x80}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x01,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  失能RPDO0
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置RPDO的传输方式为事件触发
	pdo_para_mes.data[0] = 0x2F;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x02;
	pdo_para_mes.data[4] = 0xFE;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空RPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6040：controlword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x01;
	pdo_map_mes.data[4] = 0x10;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x40;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6060：mode of operation
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x60;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射607A：target position
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x01;
//	pdo_map_mes.data[2] = 0x16;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x7A;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	映射60FF：target velocity
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0xFF;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//  使能RPDO
	pdo_para_mes.data[0] = 0x23;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x01;
	pdo_para_mes.data[4] = 0x00+ID;
	pdo_para_mes.data[5] = 0x03;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	return 0x00;
}



uint8_t TPDO1_Config(uint8_t ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x23,0x00,0x18,0x01,0x80+ID,0x01,0x00,0x80}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x00,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  失能TPDO0
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	pdo_para_mes.data[1] = 0x02;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	pdo_para_mes.data[1] = 0x03;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的传输方式为定时触发
	pdo_para_mes.data[0] = 0x2F;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x02;
	pdo_para_mes.data[4] = 0xFF;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时200*0.5 = 100ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0xE8;
	pdo_para_mes.data[5] = 0x03;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空TPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6041：statusword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x01;
	pdo_map_mes.data[4] = 0x10;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x41;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6061：mode of operation display
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x61;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射6064：position actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x00;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x64;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	映射606C：velocity actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x6C;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//  使能TPDO
	pdo_para_mes.data[0] = 0x23;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x01;
	pdo_para_mes.data[4] = 0x80+ID;
	pdo_para_mes.data[5] = 0x01;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	return 0x00;
}


uint8_t TPDO2_Config(uint8_t ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x23,0x01,0x18,0x01,0x80+ID,0x02,0x00,0x80}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x01,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  失能TPDO0
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的传输方式为定时触发
	pdo_para_mes.data[0] = 0x2F;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x02;
	pdo_para_mes.data[4] = 0xFF;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时200*0.5 = 100ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0xE8;
	pdo_para_mes.data[5] = 0x03;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空TPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6041：statusword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x01;
	pdo_map_mes.data[4] = 0x10;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x41;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6061：mode of operation display
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x61;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射6064：position actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x01;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x64;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	映射606C：velocity actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x6C;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//  使能TPDO
	pdo_para_mes.data[0] = 0x23;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x14;
	pdo_para_mes.data[3] = 0x01;
	pdo_para_mes.data[4] = 0x80+ID;
	pdo_para_mes.data[5] = 0x02;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	return 0x00;
}

uint8_t Driver_Enable(uint8_t ID)
{
	/*************************SDO****************************
	Message mes_1 = {0x600+4,0,8,{0x2B,0x40,0x60,0x00,0x00,0x00,0x00,0x00}};
	Message mes_2 = {0x600+3,0,8,{0x2B,0x40,0x60,0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5);	
	mes_1.data[0] = 0x2B;
	mes_1.data[1] = 0x40;
	mes_1.data[2] = 0x60;
	mes_1.data[3] = 0x00;
	mes_1.data[4] = 0x06;
	mes_1.data[5] = 0x00;
	mes_1.data[6] = 0x00;
	mes_1.data[7] = 0x00;
	mes_2.data[0] = 0x2B;
	mes_2.data[1] = 0x40;
	mes_2.data[2] = 0x60;
	mes_2.data[3] = 0x00;
	mes_2.data[4] = 0x06;
	mes_2.data[5] = 0x00;
	mes_2.data[6] = 0x00;
	mes_2.data[7] = 0x00;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5);
	mes_1.data[0] = 0x2B;
	mes_1.data[1] = 0x40;
	mes_1.data[2] = 0x60;
	mes_1.data[3] = 0x00;
	mes_1.data[4] = 0x07;
	mes_1.data[5] = 0x00;
	mes_1.data[6] = 0x00;
	mes_1.data[7] = 0x00;
	mes_2.data[0] = 0x2B;
	mes_2.data[1] = 0x40;
	mes_2.data[2] = 0x60;
	mes_2.data[3] = 0x00;
	mes_2.data[4] = 0x07;
	mes_2.data[5] = 0x00;
	mes_2.data[6] = 0x00;
	mes_2.data[7] = 0x00;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5);
	mes_1.data[0] = 0x2B;
	mes_1.data[1] = 0x40;
	mes_1.data[2] = 0x60;
	mes_1.data[3] = 0x00;
	mes_1.data[4] = 0x0F;
	mes_1.data[5] = 0x00;
	mes_1.data[6] = 0x00;
	mes_1.data[7] = 0x00;
	mes_2.data[0] = 0x2B;
	mes_2.data[1] = 0x40;
	mes_2.data[2] = 0x60;
	mes_2.data[3] = 0x00;
	mes_2.data[4] = 0x0F;
	mes_2.data[5] = 0x00;
	mes_2.data[6] = 0x00;
	mes_2.data[7] = 0x00;
	canSend(CAN1, &mes_1);
	return canSend(CAN1, &mes_2);
	*************************SDO****************************/

	Message mes_1 = {0x200+4,0,7,{0x06,0x00,0x03,0x00,0x00,0x00,0x00}};
	Message mes_2 = {0x300+3,0,7,{0x06,0x00,0x03,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(50);
	mes_1.data[0] = 0x07;
	mes_1.data[1] = 0x00;
	mes_2.data[0] = 0x07;
	mes_2.data[1] = 0x00;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(50);
	mes_1.data[0] = 0x0F;
	mes_1.data[1] = 0x00;
	mes_2.data[0] = 0x0F;
	mes_2.data[1] = 0x00;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(50);
	
	return 0x00;
}

uint8_t Profile_Velocity_Init(uint8_t ID)
{
	Message mes = {0x600+ID,0,8,{0x2F,0x60,0x60,0x00,0x00,0x00,0x00,0x00}};
	canSend(CAN1, &mes);
	delay_ms(5);
	mes.data[0] = 0x23;
	mes.data[1] = 0x83;
	mes.data[2] = 0x60;
	mes.data[3] = 0x00;
	mes.data[4] = 0x64;
	mes.data[5] = 0x00;
	mes.data[6] = 0x00;
	mes.data[7] = 0x00;
	canSend(CAN1, &mes);
	delay_ms(5);
	mes.data[0] = 0x23;
	mes.data[1] = 0x84;
	mes.data[2] = 0x60;
	mes.data[3] = 0x00;
	mes.data[4] = 0x64;
	mes.data[5] = 0x00;
	mes.data[6] = 0x00;
	mes.data[7] = 0x00;
	canSend(CAN1, &mes);
	delay_ms(5);
	mes.data[0] = 0x2F;
	mes.data[1] = 0x60;
	mes.data[2] = 0x60;
	mes.data[3] = 0x00;
	mes.data[4] = 0x03;
	mes.data[5] = 0x00;
	mes.data[6] = 0x00;
	mes.data[7] = 0x00;
	canSend(CAN1, &mes);
	delay_ms(5);
	return 0x00;
}

void Profile_Velocity_Test(uint8_t ID)
{
	Message mes_1 = {0x200+4,0,7,{0x0F,0x00,0x03,0xC8,0x00,0x00,0x00}};
	Message mes_2 = {0x300+3,0,7,{0x0F,0x00,0x03,0xC8,0x00,0x00,0x00}};
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5000);
	mes_1.data[3] = 0xD4;
	mes_1.data[4] = 0xFE;
	mes_1.data[5] = 0xFF;
	mes_1.data[6] = 0xFF;
	
	mes_2.data[3] = 0xD4;
	mes_2.data[4] = 0xFE;
	mes_2.data[5] = 0xFF;
	mes_2.data[6] = 0xFF;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5000);
	/*************************SDO****************************
	Message mes_1 = {0x600+4,0,8,{0x23,0xFF,0x60,0x00,0xC8,0x00,0x00,0x00}};
	Message mes_2 = {0x600+3,0,8,{0x23,0xFF,0x60,0x00,0xC8,0x00,0x00,0x00}};
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5000);
	mes_1.data[0] = 0x23;
	mes_1.data[1] = 0xFF;
	mes_1.data[2] = 0x60;
	mes_1.data[3] = 0x00;
	mes_1.data[4] = 0x9C;
	mes_1.data[5] = 0xFF;
	mes_1.data[6] = 0xFF;
	mes_1.data[7] = 0xFF;
	mes_2.data[0] = 0x23;
	mes_2.data[1] = 0xFF;
	mes_2.data[2] = 0x60;
	mes_2.data[3] = 0x00;
	mes_2.data[4] = 0x9C;
	mes_2.data[5] = 0xFF;
	mes_2.data[6] = 0xFF;
	mes_2.data[7] = 0xFF;
	canSend(CAN1, &mes_1);
	canSend(CAN1, &mes_2);
	delay_ms(5000);
	 *****************************************************/
}

