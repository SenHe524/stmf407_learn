#ifndef __SET_GET_INFO_H
#define __SET_GET_INFO_H

#include "main.h"

int8_t get_i8(motorID ID, uint16_t reg);
int16_t get_i16(motorID ID, uint16_t reg);
uint16_t get_u16(motorID ID, uint16_t reg);
int32_t get_i32(motorID ID, uint16_t reg);
uint32_t get_u32(motorID ID, uint16_t reg);
uint8_t set_reg(motorID ID, uint16_t reg, uint8_t len, uint8_t* val_buf);

void motor_enable(motorID ID);
void motor_disable(motorID ID);

uint16_t get_status(motorID ID);
int8_t get_mode(motorID ID);

int32_t get_count(motorID ID);
int32_t get_rad(motorID ID);
int32_t get_meter(motorID ID);

uint16_t get_motor_temp(motorID ID);
uint16_t get_motor_status(motorID ID);
uint16_t get_hall_status(motorID ID);
uint16_t get_errorcode(motorID ID);
int32_t get_actual_velocity(motorID ID);

uint8_t set_accelerate_time(motorID ID, uint32_t time);
uint8_t set_decelerate_time(motorID ID, uint32_t time);

void clear_fault(motorID ID);
void quick_stop(motorID ID);
void quickstop_to_enable(motorID ID);

void set_velocity_motor1(int32_t velocity);
void set_velocity_motor2(int32_t velocity);
void set_velocity_motor3(int32_t velocity);
void set_velocity_motor4(int32_t velocity);

#endif 

