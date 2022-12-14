#include "pdo_set.h"


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

void heartbeat_timeset(void)
{
	Message mes = {0x601,0,8,{0x2B,0x17,0x10,0x00,0x2C,0x01,0x00,0x00}};
	canSend(CAN1, &mes);
	mes.cob_id = 0x602;
	canSend(CAN1, &mes);
	mes.cob_id = 0x603;
	canSend(CAN1, &mes);
	mes.cob_id = 0x604;
	canSend(CAN1, &mes);
}


UNS32 index1016_callback(CO_Data* d, const indextable* table, UNS8 bSubindex)
{
	u16 value = *(uint8_t *) table->pSubindex[bSubindex].pObject;
	printf("Index1017_callback value:%d\r\n",value);
	return 0;
}
void heartbeat_error(CO_Data* d, UNS8 heartbeatID)
{
	printf("heartbeat_error heartbeatID:%d\r\n",heartbeatID);
}


uint8_t RPDO1_Config(motorID ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x00,0x14,0x02,0xFE,0x00,0x00,0x00}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x00,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  设置RPDO的传输方式为事件触发
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
	return 0x00;
}


uint8_t RPDO2_Config(motorID ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x01,0x14,0x02,0xFE,0x00,0x00,0x00}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x01,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  设置RPDO的传输方式为事件触发
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
	return 0x00;
}

uint8_t RPDO3_Config(motorID ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x02,0x14,0x02,0xFE,0x00,0x00,0x00}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x02,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  设置RPDO的传输方式为事件触发
	pdo_para_mes.data[0] = 0x2F;
	pdo_para_mes.data[1] = 0x02;
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
	pdo_map_mes.data[1] = 0x02;
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
	pdo_map_mes.data[1] = 0x02;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x60;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射60FF：target velocity
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x02;
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
	pdo_map_mes.data[1] = 0x02;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	return 0x00;
}

uint8_t RPDO4_Config(motorID ID)
{
	//	RPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x03,0x14,0x02,0xFE,0x00,0x00,0x00}};
	//	RPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x03,0x16,0x00,0x00,0x00,0x00,0x00}};
	//  设置RPDO的传输方式为事件触发
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空RPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6040：controlword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x03;
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
	pdo_map_mes.data[1] = 0x03;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x60;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射60FF：target velocity
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x03;
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
	pdo_map_mes.data[1] = 0x03;
	pdo_map_mes.data[2] = 0x16;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	return 0x00;
}


uint8_t TPDO1_Config(motorID ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x00,0x18,0x02,0xFF,0x00,0x00,0x00}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x00,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  设置TPDO的传输方式为定时触发
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时100*0.5 = 50ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x00;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0x64;
	pdo_para_mes.data[5] = 0x00;
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
	//	映射6064：position actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x00;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x64;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射606C：velocity actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x00;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x6C;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
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
	return 0x00;
}


uint8_t TPDO2_Config(motorID ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x01,0x18,0x02,0xFF,0x00,0x00,0x00}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x01,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  设置TPDO的传输方式为定时触发
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时100*0.5 = 50ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x01;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0x64;
	pdo_para_mes.data[5] = 0x00;
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
	//	映射6064：position actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x01;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x64;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射606C：velocity actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x01;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x6C;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
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
	return 0x00;
}
uint8_t TPDO3_Config(motorID ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x02,0x18,0x02,0xFF,0x00,0x00,0x00}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x02,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  设置TPDO的传输方式为定时触发
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时100*0.5 = 50ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x02;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0x64;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空TPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6041：statusword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x02;
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
	pdo_map_mes.data[1] = 0x02;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x61;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6064：position actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x02;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x64;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射606C：velocity actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x02;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x6C;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x02;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	return 0x00;
}

uint8_t TPDO4_Config(motorID ID)
{
	//	TPDO传输参数
	Message pdo_para_mes = {0x600+ID,0,8,{0x2F,0x03,0x18,0x02,0xFF,0x00,0x00,0x00}};
	//	TPDO映射参数
	Message pdo_map_mes = {0x600+ID,0,8,{0x2F,0x03,0x1A,0x00,0x00,0x00,0x00,0x00}};
	//  设置TPDO的传输方式为定时触发
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//  设置TPDO的定时100*0.5 = 50ms
	pdo_para_mes.data[0] = 0x2B;
	pdo_para_mes.data[1] = 0x03;
	pdo_para_mes.data[2] = 0x18;
	pdo_para_mes.data[3] = 0x05;
	pdo_para_mes.data[4] = 0x64;
	pdo_para_mes.data[5] = 0x00;
	pdo_para_mes.data[6] = 0x00;
	pdo_para_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_para_mes);
	delay_ms(5);
	//	清空TPDO映射
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6041：statusword
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x03;
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
	pdo_map_mes.data[1] = 0x03;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x02;
	pdo_map_mes.data[4] = 0x08;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x61;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	//	映射6064：position actual value
	pdo_map_mes.data[0] = 0x23;
	pdo_map_mes.data[1] = 0x03;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x03;
	pdo_map_mes.data[4] = 0x20;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x64;
	pdo_map_mes.data[7] = 0x60;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
//	//	映射606C：velocity actual value
//	pdo_map_mes.data[0] = 0x23;
//	pdo_map_mes.data[1] = 0x03;
//	pdo_map_mes.data[2] = 0x1A;
//	pdo_map_mes.data[3] = 0x03;
//	pdo_map_mes.data[4] = 0x20;
//	pdo_map_mes.data[5] = 0x00;
//	pdo_map_mes.data[6] = 0x6C;
//	pdo_map_mes.data[7] = 0x60;
//	canSend(CAN1, &pdo_map_mes);
//	delay_ms(5);
	//	传输个数写入：4
	pdo_map_mes.data[0] = 0x2F;
	pdo_map_mes.data[1] = 0x03;
	pdo_map_mes.data[2] = 0x1A;
	pdo_map_mes.data[3] = 0x00;
	pdo_map_mes.data[4] = 0x03;
	pdo_map_mes.data[5] = 0x00;
	pdo_map_mes.data[6] = 0x00;
	pdo_map_mes.data[7] = 0x00;
	canSend(CAN1, &pdo_map_mes);
	delay_ms(5);
	return 0x00;
}
void tr_pdo_mapping(void)
{
	RPDO1_Config(MOTOR1);
	delay_ms(100);
	RPDO2_Config(MOTOR2);
	delay_ms(100);
	RPDO3_Config(MOTOR3);
	delay_ms(100);
	RPDO4_Config(MOTOR4);
	delay_ms(100);
	TPDO1_Config(MOTOR1);
	delay_ms(100);
	TPDO2_Config(MOTOR2);
	delay_ms(100);
	TPDO3_Config(MOTOR3);
	delay_ms(100);
	TPDO4_Config(MOTOR4);
	delay_ms(100);
}


