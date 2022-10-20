#ifndef __CAN1_H
#define __CAN1_H

#include "main.h"
// CAN bus defines for cortex-M4 STM32F407

#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_11
#define CAN_TX_PIN                 GPIO_Pin_12
#define CAN_GPIO_PORT              GPIOA
#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define CANx_RX0_IRQn              CAN1_RX0_IRQn

#define GPIO_AF_CANx               GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource11
#define CAN_TX_SOURCE              GPIO_PinSource12


//#define CANx                       CAN1
//#define CAN_CLK                    RCC_APB1Periph_CAN1
//#define CAN_RX_PIN                 GPIO_Pin_8
//#define CAN_TX_PIN                 GPIO_Pin_9
//#define CAN_GPIO_PORT              GPIOB
//#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOB
//#define CANx_RX0_IRQn              CAN1_RX0_IRQn

//#define GPIO_AF_CANx               GPIO_AF_CAN1
//#define CAN_RX_SOURCE              GPIO_PinSource8
//#define CAN_TX_SOURCE              GPIO_PinSource9




unsigned char CAN1_Init(CO_Data * d);
unsigned char canSend(CAN_PORT notused, Message *m);

#endif 

