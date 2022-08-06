#include "ecatfun.h"
#include "string.h"
#include <stdio.h>
out_EL2008_t   slave_EL2008_1;
out_EL2008_t   slave_EL2008_2;
out_EL2008_t   slave_EL2008_3;

uint32 network_configuration(void)
{
	/* Do we got expected number of slaves from config */
	if (ec_slavecount < NUMBER_OF_SLAVES)
		return 0;

	/* Verify slave by slave that it is correct*/
	if (strcmp(ec_slave[EK1100].name,"EK1100"))
		return 0;
	else if (strcmp(ec_slave[EL2008_1].name,"EL2008"))
		return 0;
	else if (strcmp(ec_slave[EL2008_2].name,"EL2008"))
		return 0;
	else if (strcmp(ec_slave[EL2008_3].name,"EL2008"))
		return 0;

	return 1;
}

void set_output_int32 (uint16 slave_no, uint8 module_index, int32 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 4 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
	*data_ptr++ = (value >> 16) & 0xFF;
	*data_ptr++ = (value >> 24) & 0xFF;
}

void set_output_int16 (uint16 slave_no, uint8 module_index, int16 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 2 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}

void set_output_int8 (uint16 slave_no, uint8 module_index,  int8 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	 /* Move pointer to correct module index */
	data_ptr += (module_index + offset);
	/* Read value byte by byte since all targets can handle misaligned
	 * addresses
	 */
	*data_ptr++ = value & 0xFF;
}

void set_output_bit (uint16 slave_no, uint8 module_index, uint8 value)
{
	/* Get the the startbit position in slaves IO byte */
	uint8 startbit = ec_slave[slave_no].Ostartbit;
	/* Set or Clear bit */
	if (value == 0)
		*ec_slave[slave_no].outputs &= ~(1 << (module_index - 1 + startbit));
	else
		*ec_slave[slave_no].outputs |= (1 << (module_index - 1 + startbit));
}
/***************************************************************************************/
void set_output_uint32 (uint16 slave_no, uint8 module_index, uint32 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 4+ offset);
	/* Read value byte by byte since all targets can handle misaligned
	* addresses
	*/
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
	 *data_ptr++ = (value >> 16) & 0xFF;
	*data_ptr++ = (value >> 24) & 0xFF;
}

void set_output_uint16 (uint16 slave_no, uint8 module_index, uint16 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 2 + offset);
	/* Read value byte by byte since all targets can handle misaligned
	* addresses
	*/
	*data_ptr++ = (value >> 0) & 0xFF;
	*data_ptr++ = (value >> 8) & 0xFF;
}

void set_output_uint8 (uint16 slave_no, uint8 module_index,  uint8 value,uint8 offset)
{
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].outputs;
	 /* Move pointer to correct module index */
	data_ptr += (module_index + offset);
	/* Read value byte by byte since all targets can handle misaligned
	* addresses
	*/
	*data_ptr++ = value & 0xFF;
}
/***************************************************************************************/
int32 get_input_int32(uint16 slave_no,uint8 module_index,uint8 offset)
{
	int32 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 4 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);
	return_value += (*data_ptr++ << 16);
	return_value += (*data_ptr++ << 24);

	return return_value;
}

int16 get_input_int16(uint16 slave_no,uint8 module_index,uint8 offset)
{
	int16 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 2 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);

	return return_value;
}

int8 get_input_int8(uint16 slave_no,uint8 module_index,uint8 offset)
{
	int8 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;

	return return_value;
}
/***************************************************************************************/
uint32 get_input_uint32(uint16 slave_no,uint8 module_index,uint8 offset)
{
	uint32 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 4 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);
	return_value += (*data_ptr++ << 16);
	return_value += (*data_ptr++ << 24);

	return return_value;
}

uint16 get_input_uint16(uint16 slave_no,uint8 module_index,uint8 offset)
{
	uint16 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index * 2 + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;
	return_value += (*data_ptr++ << 8);

	return return_value;
}

uint8 get_input_uint8(uint16 slave_no,uint8 module_index,uint8 offset)
{
	uint8 return_value;
	uint8 *data_ptr;
	/* Get the IO map pointer from the ec_slave struct */
	data_ptr = ec_slave[slave_no].inputs;
	/* Move pointer to correct module index */
	data_ptr += (module_index + offset);
	/* Read value byte by byte since all targets can't handle misaligned
	* addresses
	*/
	return_value = *data_ptr++;

	return return_value;
}

extern uint16 cur_status;
extern int32 cur_pos;
extern uint8 cur_mode;
extern uint8 flag_time;
extern PDO_Output *outputs1;
extern PDO_Input *inputs1;
uint32 time_cnt=0;
uint8 flag=1;

void ecat_loop(void)
{
	if(flag_time == 1)
	{
		time_cnt++;
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);

		cur_status = inputs1->statusWord;//0x6041

		if(flag == 1){
			if((cur_status & 0x004f) == 0x0040)
			{
				outputs1->controlword = 0x06;//0x6040
				printf("0x06\n");
			}
			else if((cur_status & 0x006f) == 0x0021)
			{
				outputs1->controlword = 0x07;
				printf("0x07\n");
			}
			else if((cur_status & 0x006f) == 0x023)
			{
				outputs1->controlword = 0x0F;
				printf("0x0F\n");
			}
			else if((cur_status & 0x006f) == 0x0027)
			{
				outputs1->controlword = 0x1F;
				printf("0x1F\n");
				cur_pos = inputs1->actualPostion;//0x6064
				outputs1->targetPostion = cur_pos;//0x607A
				
				flag = 2;
			}
		}			

		if(flag == 2){
			cur_pos += 100;
			outputs1->controlword = 0x1F;
		}
	}
	
	outputs1->targetPostion = cur_pos;//0x607A
	outputs1->TargetMode = 0x8;
}

