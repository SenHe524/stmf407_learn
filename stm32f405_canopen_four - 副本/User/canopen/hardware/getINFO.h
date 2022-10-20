#ifndef __GETINFO_H
#define __GETINFO_H

#include "main.h"

#define MOTOR_STATUS 0x6041
#define MOTOR_MODE 0x6061
#define MOTOR_ACTUAL_POSITION 0x6064

void read_pdoinfo(uint8_t id, uint16_t reg, int32_t* ret);


#endif 

