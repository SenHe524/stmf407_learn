#include "ecatuser.h"

#include "stdio.h"
#include "string.h"


PDO_Outputs *outputs1;
PDO_Input *inputs1;

char IOmap_[200];
uint32_t dorun=0;
uint32_t OpenReady=0;
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


int IS620Nsetup(uint16 slvcnt)
{

	write8(slvcnt, 0x1C12, 00, 0);				//��0x1C12����			1. ֹͣPDO����
    //write8(slvcnt, 0x1600, 00, 0);				//��0x1600����			2. ֹͣPDOӳ��
    //write32(slvcnt, 0x1600, 01, 0x60400010);		//����0x1600������		3. PDOӳ��
    //write32(slvcnt, 0x1600, 02, 0x607A0020);		//����0x1600������
    //write8(slvcnt, 0x1600, 00, 2);				//��������������
    write16(slvcnt, 0x1C12, 01, 0x1701);			//����RxPDOӳ��			4. PDo����
    write8(slvcnt, 0x1C12, 00, 1);


    write8(slvcnt, 0x1C13, 00, 00);				//��0x1C12����							
    //write8(slvcnt, 0x1A00, 00, 00);				//��0x1600����							
    //write32(slvcnt, 0x1A00, 01, 0x603F0010);		//603Fh( ������ )				Uint16	
    //write32(slvcnt, 0x1A00, 02, 0x60410010);		//6041h( ״̬�� )				Uint16	
    //write32(slvcnt, 0x1A00, 03, 0x60640020);		//6064h( λ�÷��� ָ�λ )	Dint 32	
    //write32(slvcnt, 0x1A00, 04, 0x60770010);		//6077h( ת�ط��� )				int16
    //write32(slvcnt, 0x1A00, 05, 0x606C0020);		//606C( �ٶȷ��� )				int 32
    //write8(slvcnt, 0x1A00, 00, 05);				//��������������
    write16(slvcnt, 0x1C13, 01, 0x1b01);			//����TxPDOӳ��
    write8(slvcnt, 0x1C13, 00, 01);

    
    write8(slvcnt, 0x6060, 00, 8);
    
    
    return 0;
}



void ecat_init(void)
{
    int slc;
    int i;
    
    
    /* initialise SOEM, bind socket to ifname */
	if (ec_init("test"))
	{
		printf("ec_init succeeded.\r\n");
        
		if ( ec_config_init(FALSE) > 0 )
		{
			printf("%d slaves found and configured.\r\n",ec_slavecount);
            
			if ( ec_slavecount >= 1 ) 
			{
                for(slc = 1; slc <= ec_slavecount; slc++)
                 {
                     //�㴨�ŷ�
                     if((ec_slave[slc].eep_man == 0x00100000) && (ec_slave[slc].eep_id == 0x000c0108))
                     {
                         printf("Found %s at position %d\r\n", ec_slave[slc].name, slc);

                         ec_slave[slc].PO2SOconfig = &IS620Nsetup;
                     }
                 }
            }
            
            ec_configdc();
            
            ec_config_map(&IOmap_);
            
            ec_dcsync0(1, TRUE, SYNC0TIME, 0); // SYNC0 on slave 1
            
			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
            
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            
            ec_writestate(0);
            
			//printf("DC capable : %d\r\n",ec_configdc());
            
            //HAL_Delay(100);
            
            for( i=0; i<10; i++ )
            {
                ec_slave[0].state = EC_STATE_OPERATIONAL;
                 ec_writestate(0);
            }
               
            if (ec_slave[0].state == EC_STATE_OPERATIONAL )
            {
                
                outputs1 = (PDO_Outputs *)ec_slave[1].outputs;
                inputs1  = (PDO_Input *)ec_slave[1].inputs;
    
                dorun = 1;

                printf("all slaves reached operational state.\r\n");
            }
			
			else
			{
				printf("E/BOX not found in slave configuration.\r\n");
			}
		}
		else
		{
			printf("No slaves found!\r\n");
		}
	}
	else
	{
		printf("No socket connection Excecute as root\r\n");
	}
    
}






//1ms isr
void ecat_loop(void)
{
    if (dorun>0)
    {
        ec_send_processdata();
        
        ec_receive_processdata(EC_TIMEOUTRET);
        
        
        if( OpenReady )
        {
            outputs1->targetPostion += 10000;
        }
    }
}



void ctrl_state(void)
{
    switch (outputs1->controlword)
    {
    case 0:
        outputs1->controlword = 6;
        break;
    case 6:
        outputs1->controlword = 7;
        break;
    case 7:
        outputs1->targetPostion = inputs1->actualPostion;
        outputs1->controlword = 0x0f;
        break;
    case 0x0f:
        if( OpenReady == 0 )
            OpenReady = 1;
        else
            OpenReady = 0;
        
        break;
    default:
        outputs1->controlword = 6;
        break;
    }
    

    printf("\r\nctl = %d   OpenReady = %d\r\n",outputs1->controlword,OpenReady);
}






