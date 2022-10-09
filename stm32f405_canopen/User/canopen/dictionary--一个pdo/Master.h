
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef MASTER_H
#define MASTER_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 Master_valueRangeTest (UNS8 typeValue, void * value);
const indextable * Master_scanIndexOD (CO_Data *d, UNS16 wIndex, UNS32 * errorCode);

/* Master node data struct */
extern CO_Data Master_Data;
extern UNS16 Controlword;		/* Mapped at index 0x6040, subindex 0x00*/
extern UNS16 Statusword;		/* Mapped at index 0x6041, subindex 0x00*/
extern INTEGER8 Modes_of_operation;		/* Mapped at index 0x6060, subindex 0x00*/
extern INTEGER8 Modes_of_operation_display;		/* Mapped at index 0x6061, subindex 0x00*/
extern INTEGER32 Position_actual_value;		/* Mapped at index 0x6064, subindex 0x00*/
extern INTEGER32 Velocity_actual_value;		/* Mapped at index 0x606C, subindex 0x00*/
extern INTEGER32 Target_position;		/* Mapped at index 0x607A, subindex 0x00*/
extern UNS32 Profile_velocity;		/* Mapped at index 0x6081, subindex 0x00*/
extern UNS32 Profile_acceleration;		/* Mapped at index 0x6083, subindex 0x00*/
extern UNS32 Profile_deceleration;		/* Mapped at index 0x6084, subindex 0x00*/
extern UNS32 Quick_stop_deceleration;		/* Mapped at index 0x6085, subindex 0x00*/
extern INTEGER32 Target_velocity;		/* Mapped at index 0x60FF, subindex 0x00*/
extern UNS32 Supported_drive_modes;		/* Mapped at index 0x6502, subindex 0x00*/

#endif // MASTER_H
