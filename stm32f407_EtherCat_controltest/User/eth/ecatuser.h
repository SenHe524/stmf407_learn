#ifndef _ECATUSER_H_
#define _ECATUSER_H_


#include "ethercat.h"

#include "osal.h"



extern int32_t ecOffset;







PACKED_BEGIN
typedef struct PACKED
{
	uint16 controlword;
	uint32 targetPostion;
	uint16 touchProbeFunc;
	uint32 phyOutputs;
}PDO_Outputs; //1701
PACKED_END


PACKED_BEGIN
typedef struct PACKED
{
	uint16 errorCode;
	uint16 statusWord;
	uint32 actualPostion;
	uint16 actualTorque;
	uint32 actualFollowError;
	uint16 touchProbeStatus;
	uint32 touchProbePos1;
	uint32 phyDigInputs;
}PDO_Input;
PACKED_END






void ecat_loop_init();



void ecat_init(void);
void ecat_loop(void);
void ctrl_state(void);



#endif

