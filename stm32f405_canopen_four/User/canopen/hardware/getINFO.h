#ifndef __GETINFO_H
#define __GETINFO_H

#include "main.h"

uint16_t get_reg_info(uint8_t id, uint16_t reg);
uint16_t get_status(uint8_t ID);
int8_t get_mode(uint8_t ID);
uint16_t get_position(uint8_t ID);
#endif 

