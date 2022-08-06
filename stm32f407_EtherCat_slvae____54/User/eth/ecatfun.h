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

#define EK1100             1
#define EL2008_1           2
#define EL2008_2           3
#define EL2008_3           4
#define NUMBER_OF_SLAVES   4

typedef __packed struct
{
	uint8    out1;
	uint8    out2;
	uint8    out3;
	uint8    out4;
	uint8    out5;
	uint8    out6;
	uint8    out7;
	uint8    out8;
} out_EL2008_t;


typedef __packed struct 
{
	uint16 controlword;	//6040
	int32  targetPostion;	//607a
	uint8 TargetMode;
}PDO_Output;



typedef  __packed struct 
{
	uint16 statusWord;	//6041
	int32  actualPostion;//6064
	uint8 CurrentMode;
}PDO_Input;


uint32 network_configuration(void);

void set_output_int32 (uint16 slave_no, uint8 module_index, int32 value,uint8 offset);
void set_output_int16 (uint16 slave_no, uint8 module_index, int16 value,uint8 offset);
void set_output_int8 (uint16 slave_no, uint8 module_index, int8 value,uint8 offset);
void set_output_bit (uint16 slave_no, uint8 module_index, uint8 value);

void set_output_uint32 (uint16 slave_no, uint8 module_index, uint32 value,uint8 offset);
void set_output_uint16 (uint16 slave_no, uint8 module_index, uint16 value,uint8 offset);
void set_output_uint8 (uint16 slave_no, uint8 module_index,  uint8 value,uint8 offset);

int32 get_input_int32(uint16 slave_no,uint8 module_index,uint8 offset);
int16 get_input_int16(uint16 slave_no,uint8 module_index,uint8 offset);
int8 get_input_int8(uint16 slave_no,uint8 module_index,uint8 offset);

uint32 get_input_uint32(uint16 slave_no,uint8 module_index,uint8 offset);
uint16 get_input_uint16(uint16 slave_no,uint8 module_index,uint8 offset);
uint8 get_input_uint8(uint16 slave_no,uint8 module_index,uint8 offset);

void ecat_loop(void);

#endif
