#ifndef __SET_GET_INFO_H
#define __SET_GET_INFO_H

#include "main.h"

int8_t get_i8(motorID ID, uint16_t reg);
int16_t get_i16(motorID ID, uint16_t reg);
uint16_t get_u16(motorID ID, uint16_t reg);
int32_t get_i32(motorID ID, uint16_t reg);
uint32_t get_u32(motorID ID, uint16_t reg);
int8_t set_reg(motorID ID, uint16_t reg, uint8_t len, uint8_t* val_buf);

int8_t isenable(motorID ID);
int8_t motor_enable(motorID ID);
int8_t motor_disable(motorID ID);

int8_t isfault(motorID ID);
int8_t clear_fault(motorID ID);
int8_t quick_stop(motorID ID);
int8_t quickstop_to_enable(motorID ID);


uint16_t get_status(motorID ID);
int8_t get_mode(motorID ID);
int32_t get_count(motorID ID);
int32_t get_rad(motorID ID);
int32_t get_distance(motorID ID);

uint16_t get_issave(motorID ID);
uint16_t get_motor_temp(motorID ID);
uint16_t get_motor_status(motorID ID);
uint16_t get_hall_status(motorID ID);
uint16_t get_errorcode(motorID ID);
int32_t get_actual_velocity(motorID ID);
uint16_t get_lock(motorID ID);

uint32_t get_accelerate_time(motorID ID);
uint32_t get_decelerate_time(motorID ID);
uint16_t get_Vsmooth_factor(motorID ID);
uint16_t get_Eratio_gain(motorID ID);
uint16_t get_Eintegral_gain(motorID ID);
uint16_t get_feedforward_ratio(motorID ID);
uint16_t get_torque_ratio(motorID ID);
uint16_t get_VKp(motorID ID);
uint16_t get_VKi(motorID ID);
uint16_t get_Vfeedforward_Kf(motorID ID);
uint16_t get_PKp(motorID ID);
uint16_t get_Pfeedforward_Kf(motorID ID);

int8_t set_issave_rw(motorID ID, uint16_t issave);
int8_t set_lock(motorID ID, uint16_t lock);
int8_t set_issave_rws(motorID ID, uint16_t issave);
int8_t set_accelerate_time(motorID ID, uint32_t time);
int8_t set_decelerate_time(motorID ID, uint32_t time);
int8_t set_Vsmooth_factor(motorID ID, uint16_t factor);
int8_t set_Eratio_gain(motorID ID, uint16_t factor);
int8_t set_Eintegral_gain(motorID ID, uint16_t factor);
int8_t set_feedforward_ratio(motorID ID, uint16_t factor);
int8_t set_torque_ratio(motorID ID, uint16_t factor);
int8_t set_VKp(motorID ID, uint16_t factor);
int8_t set_VKi(motorID ID, uint16_t factor);
int8_t set_Vfeedforward_Kf(motorID ID, uint16_t factor);
int8_t set_PKp(motorID ID, uint16_t factor);
int8_t set_Pfeedforward_Kf(motorID ID, uint16_t factor);



void set_velocity_motor1(int32_t velocity);
void set_velocity_motor2(int32_t velocity);
void set_velocity_motor3(int32_t velocity);
void set_velocity_motor4(int32_t velocity);

#endif 

