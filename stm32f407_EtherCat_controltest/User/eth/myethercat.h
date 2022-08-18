#ifndef _MYETHERCAT_H_
#define _MYETHERCAT_H_


#include "ethercat.h"


////	10字节
//PACKED_BEGIN
//typedef struct PACKED
//{
//	uint16_t controlword;
//	uint16_t TargetMode;
//	int32_t targetPostion;
//	int16_t targetTorque;
//}Rx_PDO_t; //1701
//PACKED_END


////	15字节
//PACKED_BEGIN
//typedef struct PACKED
//{
//	uint16_t statusWord;
//	uint16_t actualTorque;
//	int32_t actualPostion;
//	int32_t actualVelocity;
//	uint16_t errorCode;
//	int actualMode;
//}Tx_PDO_t;
//PACKED_END



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



#endif

