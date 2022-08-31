#ifndef _ECATFUN_H
#define _ECATFUN_H
#include "osal.h"
#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatsoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

/*-----------------------------------------------------------------------------------------
------    Defines and Types
-----------------------------------------------------------------------------------------*/

/*---------------------------------------------
-    ControlWord Commands (IEC61800_184e)
-----------------------------------------------*/
#define COMMAND_SHUTDOWN                         0x0006 /**< \brief Shutdown command*/
#define COMMAND_SWITCHON                         0x0007 /**< \brief Switch on command*/
#define COMMAND_SWITCHON_ENABLEOPERATION         0x000F /**< \brief Switch on & Enable command*/
#define COMMAND_DISABLEVOLTAGE                   0x0000 /**< \brief Disable voltage command*/
#define COMMAND_QUICKSTOP                        0x0002 /**< \brief Quickstop command*/
#define COMMAND_DISABLEOPERATION                 0x0007 /**< \brief Disable operation command*/
#define COMMAND_ENABLEOPERATION                  0x000F /**< \brief Enable operation command*/
#define COMMAND_FAULTRESET                       0x0080 /**< \brief Fault reset command*/

/*---------------------------------------------
-    StatusWord
-----------------------------------------------*/
#define STATUS_NOTREADYTOSWITCHON                  0x0000 /**< \brief Not ready to switch on*/
#define STATUS_SWITCHEDONDISABLED                  0x0040 /**< \brief Switched on but disabled*/
#define STATUS_READYTOSWITCHON                     0x0021 /**< \brief Ready to switch on*/
#define STATUS_SWITCHEDON                          0x0023 /**< \brief Switched on*/
#define STATUS_OPERATIONENABLED                    0x0027 /**< \brief Operation enabled*/
#define STATUS_QUICKSTOPACTIVE                     0x0007 /**< \brief Quickstop active*/
#define STATUS_FAULTREACTIONACTIVE                 0x000F /**< \brief Fault reaction active*/
#define STATUS_FAULT                               0x0008 /**< \brief Fault state*/


/*---------------------------------------------
-    CiA402 Modes of Operation (object 0x6060) (IEC61800_184e)
-----------------------------------------------*/
// -128 to -1 Manufacturer-specific operation modes
#define NO_MODE                     0 /**< \brief No mode*/
#define PROFILE_POSITION_MODE       1 /**< \brief Position Profile mode*/
#define VELOCITY_MODE               2 /**< \brief Velocity mode*/
#define PROFILE_VELOCITY_MOCE       3 /**< \brief Velocity Profile mode*/
#define PROFILE_TORQUE_MODE         4 /**< \brief Torque Profile mode*/
//5 reserved                          
#define HOMING_MODE                 6 /**< \brief Homing mode*/
#define INTERPOLATION_POSITION_MODE 7 /**< \brief Interpolation Position mode*/
#define CYCLIC_SYNC_POSITION_MODE   8 /**< \brief Cyclic Synchronous Position mode*/
#define CYCLIC_SYNC_VELOCITY_MODE   9 /**< \brief Cyclic Synchronous Velocity mode*/
#define CYCLIC_SYNC_TORQUE_MODE     10/**< \brief Cyclic Synchronous Torque mode*/
#define USER_DEFINED_BRAKE			11
//+11 to +127 reserved

// offset
#define	Controlword_offset			0
#define	Modes_Of_Operation_offset	2
#define	Target_Position_offset		3
#define	Target_Torque_offset		7
#define	Statusword_offset			0
#define	T_actual_value_offset		2
#define	P_actual_value_offset		4
#define	V_actual_value_offset		8
#define	Error_Code_offset			12
#define	MOP_Display_offset			14


//#define	Controlword_offset			0
//#define	Modes_Of_Operation_offset	2
//#define	Target_Position_offset		4
//#define	Target_Torque_offset		8
//#define	VelFF_ch1_offset			10

//#define	Statusword_offset			0
//#define	T_actual_value_offset		2
//#define	P_actual_value_offset		4
//#define	V_actual_value_offset		8
//#define	Error_Code_offset			12
//#define	MOP_Display_offset			14
//#define	AccelerometerX_offset		16
//#define	MPA_offset					20


void set_output_int32 (uint8_t slave, uint8_t index, int32_t value, uint8 offset);
void set_output_int16 (uint8_t slave, uint8_t index, int16_t value, uint8 offset);
void set_output_int8 (uint8_t slave, uint8_t index, int8_t value, uint8 offset);
void set_output_uint16 (uint8_t slave, uint8_t index, uint16_t value, uint8 offset);
int32_t get_input_int32(uint8_t slave, uint8_t index, uint8_t offset);
int8_t get_input_int8(uint8_t slave, uint8_t index, uint8_t offset);
uint16_t get_input_uint16(uint8_t slave, uint8_t index, uint8_t offset);

void ecat_loop(void);

#endif
