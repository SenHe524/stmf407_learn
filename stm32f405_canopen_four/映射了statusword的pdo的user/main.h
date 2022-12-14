#ifndef __MIAN_H
#define __MIAN_H

#include "stm32f4xx.h" // Device header
#include <stdio.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"

typedef enum MOTOR
{
	MOTOR1 = 0x01,
	MOTOR2 = 0x02,
	MOTOR3 = 0x03,
	MOTOR4 = 0x04
}motorID;

#define IS_MOTOR_ID(ID)			(((ID) == MOTOR1) || \
								 ((ID) == MOTOR2) || \
								 ((ID) == MOTOR3) || \
								 ((ID) == MOTOR4))
#define SAVE_RW					0x2009
#define LOCK_METHOD 			0x200F
#define SAVE_RW_S 				0x2010
#define VELO_SMOOTH_FACTOR 		0x2018
#define ELEC_ERATIO_GAIN 		0x2019
#define ELEC_INTEGRAL 			0x201A
#define FEEDFORWARD_RATIO 		0x201B
#define TORQUE_RATIO 			0x201C
#define VELO_KP 				0x201D
#define VELO_KI 				0x201E
#define VELO_FEEDFORWARD_KF 	0x201F
#define POSI_KP 				0x2020
#define POSI_FEEDFORWARD_KF 	0x2021
#define MOTOR_TEMP 				0x2026
#define IS_MOTOR_MOVE 			0x2027
#define MOTOR_HALL_STATUS		0x2028
#define ERROR_CODE	 			0x603f
#define MOTOR_STATUS			0x6040
#define MOTOR_CONTROL 			0x6041
#define MOTOR_MODE				0x6060
#define MODE_DISPLAY 			0x6061
#define ACTUAL_COUNT			0x6064
#define ACTUAL_VELOCITY			0x606C
#define ACC_TIME	 			0x6083
#define DE_TIME					0x6084
#define TARGET_VELOCITY			0x60FF
#define AGV_ODOMETRY			0x6101

#define IS_GET_UINT16(REG)		(((REG) == SAVE_RW) || \
								 ((REG) == LOCK_METHOD) || \
								 ((REG) == SAVE_RW_S) || \
								 ((REG) == VELO_SMOOTH_FACTOR) || \
								 ((REG) == ELEC_ERATIO_GAIN) || \
								 ((REG) == ELEC_INTEGRAL) || \
								 ((REG) == FEEDFORWARD_RATIO) || \
								 ((REG) == TORQUE_RATIO) || \
								 ((REG) == VELO_KP) || \
								 ((REG) == VELO_KI) || \
								 ((REG) == VELO_FEEDFORWARD_KF) || \
								 ((REG) == POSI_KP) || \
								 ((REG) == POSI_FEEDFORWARD_KF) || \
								 ((REG) == MOTOR_TEMP) || \
								 ((REG) == IS_MOTOR_MOVE) || \
								 ((REG) == MOTOR_HALL_STATUS) || \
								 ((REG) == ERROR_CODE) || \
								 ((REG) == MOTOR_STATUS) || \
								 ((REG) == ACC_TIME) || \
								 ((REG) == DE_TIME))

typedef union 
{
    int8_t data_int8;
    unsigned char data8;
}union_int8;

typedef union 
{
    uint16_t data_uint16;
    unsigned char data8[2];
}union_uint16;

typedef union 
{
    int16_t data_int16;
    unsigned char data8[2];
}union_int16;


typedef union 
{
    int32_t data_int32;
    unsigned char data8[4];
}union_int32;


typedef union 
{
    uint32_t data_uint32;
    unsigned char data8[4];
}union_uint32;


#include "data.h"
#include "hardware_driver.h"
#include "timer_user.h"
#include "can1.h"
#include "Master.h"
#include "canfestival.h"
#include "pdo_set.h"
#include "set_get_info.h"
#include "sysdep.h"
#include "protocol.h"
#include "stmflash.h"
#define PI 3.1515926
#define __weak __attribute__((weak))

#endif



















