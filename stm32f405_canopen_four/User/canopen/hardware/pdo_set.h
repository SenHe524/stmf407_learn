#ifndef __PDO_SET_H
#define __PDO_SET_H

#include "main.h"



void tr_pdo_mapping(void);
uint8_t NMT_Control(uint8_t cs, uint8_t nodeid);


void heartbeat_timeset(void);
UNS32 index1016_callback(CO_Data* d, const indextable* table, UNS8 bSubindex);
void heartbeat_error(CO_Data* d, UNS8 heartbeatID);

uint8_t RPDO1_Config(motorID ID);
uint8_t RPDO2_Config(motorID ID);
uint8_t RPDO3_Config(motorID ID);
uint8_t RPDO4_Config(motorID ID);

uint8_t TPDO1_Config(motorID ID);
uint8_t TPDO2_Config(motorID ID);
uint8_t TPDO3_Config(motorID ID);
uint8_t TPDO4_Config(motorID ID);



#endif 

