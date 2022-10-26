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

#include "data.h"
#include "timer_user.h"
#include "can1.h"
#include "Master.h"
#include "canfestival.h"
#include "pdo_set.h"
#include "set_get_info.h"
#include "sysdep.h"

#define PI 3.1515926

#endif



















