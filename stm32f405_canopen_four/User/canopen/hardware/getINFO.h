#ifndef __GETINFO_H
#define __GETINFO_H

#include "main.h"

int8_t get_i8(uint8_t ID, uint16_t reg);
int16_t get_i16(uint8_t ID, uint16_t reg);
uint16_t get_u16(uint8_t ID, uint16_t reg);
int32_t get_i32(uint8_t ID, uint16_t reg);
uint32_t get_u32(uint8_t ID, uint16_t reg);

uint16_t get_status(uint8_t ID);
int8_t get_mode(uint8_t ID);
uint16_t get_position(uint8_t ID);

uint8_t set_reg(uint8_t ID, uint16_t reg, uint8_t len, uint8_t* val_buf);
#endif 

