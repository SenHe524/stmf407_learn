#ifndef __CANOPEN_H
#define __CANOPEN_H

#include "main.h"
#include "data.h"


void tr_pdo_mapping(void);
uint8_t NMT_Control(uint8_t Data0, uint8_t Data1);
uint8_t Save_EEPROM(void);
uint8_t Reset_Save(uint8_t ID, uint8_t select);
void heartbeat_timeset(void);
UNS32 index1016_callback(CO_Data* d, const indextable* table, UNS8 bSubindex);
void heartbeat_error(CO_Data* d, UNS8 heartbeatID);
uint8_t RPDO1_Config(uint8_t ID);
uint8_t RPDO2_Config(uint8_t ID);
uint8_t RPDO3_Config(uint8_t ID);
uint8_t RPDO4_Config(uint8_t ID);

uint8_t TPDO1_Config(uint8_t ID);
uint8_t TPDO2_Config(uint8_t ID);
uint8_t TPDO3_Config(uint8_t ID);
uint8_t TPDO4_Config(uint8_t ID);

uint8_t Driver_Enable(uint8_t ID);
uint8_t Profile_Velocity_Init(uint8_t ID);
void Profile_Velocity_Test(uint8_t ID);



#endif 

