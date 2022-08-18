#include "myethercat.h"

#include "stdio.h"
#include "string.h"


//PDO_Outputs outputs[6];
//PDO_Input inputs[6];

#define SYNC0TIME 1000000

#define DEBUG 0


int write8(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint8 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}
int write16(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint16 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}
int write32(uint16 slave, uint16 index, uint8 subindex, int value)
{
	uint32 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);
	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
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








