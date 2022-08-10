/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : slaveinfo [ifname] [-sdo] [-map]
 * Ifname is NIC interface, f.e. eth0.
 * Optional -sdo to display CoE object dictionary.
 * Optional -map to display slave PDO mapping
 *
 * This shows the configured slave data.
 *
 * (c)Arthur Ketels 2010 - 2011
 */

#include <inttypes.h>

#include "stm32f4xx.h" // Device header

#include <string.h>
#include <stdio.h>

#include "delay.h"
#include "timer.h"
#include "port_func.h"
#include "ethercat.h"




char IOmap[4096];
ec_ODlistt ODlist;
ec_OElistt OElist;
boolean printSDO = FALSE;
boolean printMAP = FALSE;
char usdo[128];
char hstr[1024];

char *dtype2string(uint16 dtype)
{
    switch (dtype)
    {
    case ECT_BOOLEAN:
        sprintf(hstr, "BOOLEAN");
        break;
    case ECT_INTEGER8:
        sprintf(hstr, "INTEGER8");
        break;
    case ECT_INTEGER16:
        sprintf(hstr, "INTEGER16");
        break;
    case ECT_INTEGER32:
        sprintf(hstr, "INTEGER32");
        break;
    case ECT_INTEGER24:
        sprintf(hstr, "INTEGER24");
        break;
    case ECT_INTEGER64:
        sprintf(hstr, "INTEGER64");
        break;
    case ECT_UNSIGNED8:
        sprintf(hstr, "UNSIGNED8");
        break;
    case ECT_UNSIGNED16:
        sprintf(hstr, "UNSIGNED16");
        break;
    case ECT_UNSIGNED32:
        sprintf(hstr, "UNSIGNED32");
        break;
    case ECT_UNSIGNED24:
        sprintf(hstr, "UNSIGNED24");
        break;
    case ECT_UNSIGNED64:
        sprintf(hstr, "UNSIGNED64");
        break;
    case ECT_REAL32:
        sprintf(hstr, "REAL32");
        break;
    case ECT_REAL64:
        sprintf(hstr, "REAL64");
        break;
    case ECT_BIT1:
        sprintf(hstr, "BIT1");
        break;
    case ECT_BIT2:
        sprintf(hstr, "BIT2");
        break;
    case ECT_BIT3:
        sprintf(hstr, "BIT3");
        break;
    case ECT_BIT4:
        sprintf(hstr, "BIT4");
        break;
    case ECT_BIT5:
        sprintf(hstr, "BIT5");
        break;
    case ECT_BIT6:
        sprintf(hstr, "BIT6");
        break;
    case ECT_BIT7:
        sprintf(hstr, "BIT7");
        break;
    case ECT_BIT8:
        sprintf(hstr, "BIT8");
        break;
    case ECT_VISIBLE_STRING:
        sprintf(hstr, "VISIBLE_STRING");
        break;
    case ECT_OCTET_STRING:
        sprintf(hstr, "OCTET_STRING");
        break;
    default:
        sprintf(hstr, "Type 0x%4.4X", dtype);
    }
    return hstr;
}

char *SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype)
{
    int l = sizeof(usdo) - 1, i;
    uint8 *u8;
    int8 *i8;
    uint16 *u16;
    int16 *i16;
    uint32 *u32;
    int32 *i32;
    uint64 *u64;
    int64 *i64;
    float *sr;
    double *dr;
    char es[32];

    memset(&usdo, 0, 128);
    ec_SDOread(slave, index, subidx, FALSE, &l, &usdo, EC_TIMEOUTRXM);
    if (EcatError)
    {
        return ec_elist2string();
    }
    else
    {
        switch (dtype)
        {
        case ECT_BOOLEAN:
            u8 = (uint8 *)&usdo[0];
            if (*u8)
                sprintf(hstr, "TRUE");
            else
                sprintf(hstr, "FALSE");
            break;
        case ECT_INTEGER8:
            i8 = (int8 *)&usdo[0];
            sprintf(hstr, "0x%2.2x %d", *i8, *i8);
            break;
        case ECT_INTEGER16:
            i16 = (int16 *)&usdo[0];
            sprintf(hstr, "0x%4.4x %d", *i16, *i16);
            break;
        case ECT_INTEGER32:
        case ECT_INTEGER24:
            i32 = (int32 *)&usdo[0];
            sprintf(hstr, "0x%8.8x %d", *i32, *i32);
            break;
        case ECT_INTEGER64:
            i64 = (int64 *)&usdo[0];
            sprintf(hstr, "0x%16.16" PRIx64 " %" PRId64, *i64, *i64);
            break;
        case ECT_UNSIGNED8:
            u8 = (uint8 *)&usdo[0];
            sprintf(hstr, "0x%2.2x %u", *u8, *u8);
            break;
        case ECT_UNSIGNED16:
            u16 = (uint16 *)&usdo[0];
            sprintf(hstr, "0x%4.4x %u", *u16, *u16);
            break;
        case ECT_UNSIGNED32:
        case ECT_UNSIGNED24:
            u32 = (uint32 *)&usdo[0];
            sprintf(hstr, "0x%8.8x %u", *u32, *u32);
            break;
        case ECT_UNSIGNED64:
            u64 = (uint64 *)&usdo[0];
            sprintf(hstr, "0x%16.16" PRIx64 " %" PRIu64, *u64, *u64);
            break;
        case ECT_REAL32:
            sr = (float *)&usdo[0];
            sprintf(hstr, "%f", *sr);
            break;
        case ECT_REAL64:
            dr = (double *)&usdo[0];
            sprintf(hstr, "%f", *dr);
            break;
        case ECT_BIT1:
        case ECT_BIT2:
        case ECT_BIT3:
        case ECT_BIT4:
        case ECT_BIT5:
        case ECT_BIT6:
        case ECT_BIT7:
        case ECT_BIT8:
            u8 = (uint8 *)&usdo[0];
            sprintf(hstr, "0x%x", *u8);
            break;
        case ECT_VISIBLE_STRING:
            strcpy(hstr, usdo);
            break;
        case ECT_OCTET_STRING:
            hstr[0] = 0x00;
            for (i = 0; i < l; i++)
            {
                sprintf(es, "0x%2.2x ", usdo[i]);
                strcat(hstr, es);
            }
            break;
        default:
            sprintf(hstr, "Unknown type");
        }
        return hstr;
    }
}

/** Read PDO assign structure */
int si_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset)
{
    uint16 idxloop, nidx, subidxloop, rdat, idx, subidx;
    uint8 subcnt;
    int wkc, bsize = 0, rdl;
    int32 rdat2;
    uint8 bitlen, obj_subidx;
    uint16 obj_idx;
    int abs_offset, abs_bit;

    rdl = sizeof(rdat);
    rdat = 0;
    /* read PDO assign subindex 0 ( = number of PDO's) */
    wkc = ec_SDOread(slave, PDOassign, 0x00, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
    rdat = etohs(rdat);
    /* positive result from slave ? */
    if ((wkc > 0) && (rdat > 0))
    {
        /* number of available sub indexes */
        nidx = rdat;
        bsize = 0;
        /* read all PDO's */
        for (idxloop = 1; idxloop <= nidx; idxloop++)
        {
            rdl = sizeof(rdat);
            rdat = 0;
            /* read PDO assign */
            wkc = ec_SDOread(slave, PDOassign, (uint8)idxloop, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
            /* result is index of PDO */
            idx = etohl(rdat);
            if (idx > 0)
            {
                rdl = sizeof(subcnt);
                subcnt = 0;
                /* read number of subindexes of PDO */
                wkc = ec_SDOread(slave, idx, 0x00, FALSE, &rdl, &subcnt, EC_TIMEOUTRXM);
                subidx = subcnt;
                /* for each subindex */
                for (subidxloop = 1; subidxloop <= subidx; subidxloop++)
                {
                    rdl = sizeof(rdat2);
                    rdat2 = 0;
                    /* read SDO that is mapped in PDO */
                    wkc = ec_SDOread(slave, idx, (uint8)subidxloop, FALSE, &rdl, &rdat2, EC_TIMEOUTRXM);
                    rdat2 = etohl(rdat2);
                    /* extract bitlength of SDO */
                    bitlen = LO_BYTE(rdat2);
                    bsize += bitlen;
                    obj_idx = (uint16)(rdat2 >> 16);
                    obj_subidx = (uint8)((rdat2 >> 8) & 0x000000ff);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;
                    ODlist.Slave = slave;
                    ODlist.Index[0] = obj_idx;
                    OElist.Entries = 0;
                    wkc = 0;
                    /* read object entry from dictionary if not a filler (0x0000:0x00) */
                    if (obj_idx || obj_subidx)
                        wkc = ec_readOEsingle(0, obj_subidx, &ODlist, &OElist);
                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    if ((wkc > 0) && OElist.Entries)
                    {
                        printf(" %-12s %s\n", dtype2string(OElist.DataType[obj_subidx]), OElist.Name[obj_subidx]);
                    }
                    else
                        printf("\n");
                    bitoffset += bitlen;
                };
            };
        };
    };
    /* return total found bitlength (PDO) */
    return bsize;
}

int si_map_sdo(int slave)
{
    int wkc, rdl;
    int retVal = 0;
    uint8 nSM, iSM, tSM;
    int Tsize, outputs_bo, inputs_bo;
    uint8 SMt_bug_add;

    printf("PDO mapping according to CoE :\n");
    SMt_bug_add = 0;
    outputs_bo = 0;
    inputs_bo = 0;
    rdl = sizeof(nSM);
    nSM = 0;
    /* read SyncManager Communication Type object count */
    wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, 0x00, FALSE, &rdl, &nSM, EC_TIMEOUTRXM);
    /* positive result from slave ? */
    if ((wkc > 0) && (nSM > 2))
    {
        /* make nSM equal to number of defined SM */
        nSM--;
        /* limit to maximum number of SM defined, if true the slave can't be configured */
        if (nSM > EC_MAXSM)
            nSM = EC_MAXSM;
        /* iterate for every SM type defined */
        for (iSM = 2; iSM <= nSM; iSM++)
        {
            rdl = sizeof(tSM);
            tSM = 0;
            /* read SyncManager Communication Type */
            wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, iSM + 1, FALSE, &rdl, &tSM, EC_TIMEOUTRXM);
            if (wkc > 0)
            {
                if ((iSM == 2) && (tSM == 2)) // SM2 has type 2 == mailbox out, this is a bug in the slave!
                {
                    SMt_bug_add = 1; // try to correct, this works if the types are 0 1 2 3 and should be 1 2 3 4
                    printf("Activated SM type workaround, possible incorrect mapping.\n");
                }
                if (tSM)
                    tSM += SMt_bug_add; // only add if SMt > 0

                if (tSM == 3) // outputs
                {
                    /* read the assign RXPDO */
                    printf("  SM%1d outputs\n     addr b   index: sub bitl data_type    name\n", iSM);
                    Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&IOmap[0]), outputs_bo);
                    outputs_bo += Tsize;
                }
                if (tSM == 4) // inputs
                {
                    /* read the assign TXPDO */
                    printf("  SM%1d inputs\n     addr b   index: sub bitl data_type    name\n", iSM);
                    Tsize = si_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&IOmap[0]), inputs_bo);
                    inputs_bo += Tsize;
                }
            }
        }
    }

    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}

int si_siiPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset)
{
    uint16 a, w, c, e, er, Size;
    uint8 eectl;
    uint16 obj_idx;
    uint8 obj_subidx;
    uint8 obj_name;
    uint8 obj_datatype;
    uint8 bitlen;
    int totalsize;
    ec_eepromPDOt eepPDO;
    ec_eepromPDOt *PDO;
    int abs_offset, abs_bit;
    char str_name[EC_MAXNAME + 1];

    eectl = ec_slave[slave].eep_pdi;
    Size = 0;
    totalsize = 0;
    PDO = &eepPDO;
    PDO->nPDO = 0;
    PDO->Length = 0;
    PDO->Index[1] = 0;
    for (c = 0; c < EC_MAXSM; c++)
        PDO->SMbitsize[c] = 0;
    if (t > 1)
        t = 1;
    PDO->Startpos = ec_siifind(slave, ECT_SII_PDO + t);
    if (PDO->Startpos > 0)
    {
        a = PDO->Startpos;
        w = ec_siigetbyte(slave, a++);
        w += (ec_siigetbyte(slave, a++) << 8);
        PDO->Length = w;
        c = 1;
        /* traverse through all PDOs */
        do
        {
            PDO->nPDO++;
            PDO->Index[PDO->nPDO] = ec_siigetbyte(slave, a++);
            PDO->Index[PDO->nPDO] += (ec_siigetbyte(slave, a++) << 8);
            PDO->BitSize[PDO->nPDO] = 0;
            c++;
            /* number of entries in PDO */
            e = ec_siigetbyte(slave, a++);
            PDO->SyncM[PDO->nPDO] = ec_siigetbyte(slave, a++);
            a++;
            obj_name = ec_siigetbyte(slave, a++);
            a += 2;
            c += 2;
            if (PDO->SyncM[PDO->nPDO] < EC_MAXSM) /* active and in range SM? */
            {
                str_name[0] = 0;
                if (obj_name)
                    ec_siistring(str_name, slave, obj_name);
                if (t)
                    printf("  SM%1d RXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                else
                    printf("  SM%1d TXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                printf("     addr b   index: sub bitl data_type    name\n");
                /* read all entries defined in PDO */
                for (er = 1; er <= e; er++)
                {
                    c += 4;
                    obj_idx = ec_siigetbyte(slave, a++);
                    obj_idx += (ec_siigetbyte(slave, a++) << 8);
                    obj_subidx = ec_siigetbyte(slave, a++);
                    obj_name = ec_siigetbyte(slave, a++);
                    obj_datatype = ec_siigetbyte(slave, a++);
                    bitlen = ec_siigetbyte(slave, a++);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;

                    PDO->BitSize[PDO->nPDO] += bitlen;
                    a += 2;

                    str_name[0] = 0;
                    if (obj_name)
                        ec_siistring(str_name, slave, obj_name);

                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    printf(" %-12s %s\n", dtype2string(obj_datatype), str_name);
                    bitoffset += bitlen;
                    totalsize += bitlen;
                }
                PDO->SMbitsize[PDO->SyncM[PDO->nPDO]] += PDO->BitSize[PDO->nPDO];
                Size += PDO->BitSize[PDO->nPDO];
                c++;
            }
            else /* PDO deactivated because SM is 0xff or > EC_MAXSM */
            {
                c += 4 * e;
                a += 8 * e;
                c++;
            }
            if (PDO->nPDO >= (EC_MAXEEPDO - 1))
                c = PDO->Length; /* limit number of PDO entries in buffer */
        } while (c < PDO->Length);
    }
    if (eectl)
        ec_eeprom2pdi(slave); /* if eeprom control was previously pdi then restore */
    return totalsize;
}

int si_map_sii(int slave)
{
    int retVal = 0;
    int Tsize, outputs_bo, inputs_bo;

    printf("PDO mapping according to SII :\n");

    outputs_bo = 0;
    inputs_bo = 0;
    /* read the assign RXPDOs */
    Tsize = si_siiPDO(slave, 1, (int)(ec_slave[slave].outputs - (uint8 *)&IOmap), outputs_bo);
    outputs_bo += Tsize;
    /* read the assign TXPDOs */
    Tsize = si_siiPDO(slave, 0, (int)(ec_slave[slave].inputs - (uint8 *)&IOmap), inputs_bo);
    inputs_bo += Tsize;
    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}

void si_sdo(int cnt)
{
    int i, j;

    ODlist.Entries = 0;
    memset(&ODlist, 0, sizeof(ODlist));
    if (ec_readODlist(cnt, &ODlist))
    {
        printf(" CoE Object Description found, %d entries.\n", ODlist.Entries);
        for (i = 0; i < ODlist.Entries; i++)
        {
            ec_readODdescription(i, &ODlist);
            while (EcatError)
                printf("%s", ec_elist2string());
            printf(" Index: %4.4x Datatype: %4.4x Objectcode: %2.2x Name: %s\n",
                   ODlist.Index[i], ODlist.DataType[i], ODlist.ObjectCode[i], ODlist.Name[i]);
            memset(&OElist, 0, sizeof(OElist));
            ec_readOE(i, &ODlist, &OElist);
            while (EcatError)
                printf("%s", ec_elist2string());
            for (j = 0; j < ODlist.MaxSub[i] + 1; j++)
            {
                if ((OElist.DataType[j] > 0) && (OElist.BitLength[j] > 0))
                {
                    printf("  Sub: %2.2x Datatype: %4.4x Bitlength: %4.4x Obj.access: %4.4x Name: %s\n",
                           j, OElist.DataType[j], OElist.BitLength[j], OElist.ObjAccess[j], OElist.Name[j]);
                    if ((OElist.ObjAccess[j] & 0x0007))
                    {
                        printf("          Value :%s\n", SDO2string(cnt, ODlist.Index[i], j, OElist.DataType[j]));
                    }
                }
            }
        }
    }
    else
    {
        while (EcatError)
            printf("%s", ec_elist2string());
    }
}

#define EC_TIMEOUTMON 500

char IOmap[4096]; //映射空间


int Servosetup(uint16 slave)
{
	uint8 u8val;
	uint16 u16val;
	uint32 u32val;
	
	u8val = 0;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//���0x1c12����
	u8val = 0;
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//���0x1600����
	
	u32val = 0x60400010;
	ec_SDOwrite(slave, 0x1600, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);//����0x1600������
	u32val = 0x607A0020;
	ec_SDOwrite(slave, 0x1600, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);//����0x1600������
	u32val = 0x60600008;	
    ec_SDOwrite(slave, 0x1600, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
	u8val = 3;
	ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//�趨ΪPDOӳ���е�ӳ����
	
	u16val = 0x1600;
	ec_SDOwrite(slave, 0x1c12, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);//�趨Ϊ��ѡ��PDOӳ������
	u8val = 1;
	ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//����PDO����

	/**********************************************************************************************************/
	u8val = 0;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//���0x1c13����
	u8val = 0;
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//���0x1A00����
	
	u32val = 0x60410010;
	ec_SDOwrite(slave, 0x1A00, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
	u32val = 0x60640020;
	ec_SDOwrite(slave, 0x1A00, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
	u32val = 0x60610008;	
    ec_SDOwrite(slave, 0x1A00, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);		
	u8val = 3;
	ec_SDOwrite(slave, 0x1A00, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//�趨ΪPDOӳ���е�ӳ����
	
	u16val = 0x1A00;
	ec_SDOwrite(slave, 0x1c13, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);//�趨Ϊ��ѡ��PDOӳ������
	u8val = 1;
	ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//����PDO����
	
//	u8val = 8;
//	ec_SDOwrite(slave, 0x6060, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);//cspģʽ
	return 1;
}


int Soem_init(char *ifname)
{
	if(!ec_init(ifname))//初始化soem,将主站绑定到网口
	{
		printf("No socket connection on %s\nExecute as root\n", ifname);
		return 0;
	}
	printf("ec_init on %s succeeded.\n", ifname);
	if(ec_config_init(FALSE) <= 0)
	{
		printf("No slaves found!\n");
		return 0;
	}
	printf("%d slaves found and configured.\n", ec_slavecount);
    ec_config_map(&IOmap); //主站内存空间与从站内存空间映射
    ec_configdc(); //配置DC
	ec_dcsync0(1, TRUE, 1000000, 0);
	ec_dcsync0(2, TRUE, 1000000, 0);
	ec_dcsync0(3, TRUE, 1000000, 0);
	printf("Slaves mapped.\n");
	//等待所有从机进入安全运行状态
	if(ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE) != EC_STATE_SAFE_OP)	return 0;

	printf("Request operational state for all slaves\n");
	ec_slave[0].state = EC_STATE_OPERATIONAL;
	ec_writestate(0);//	写入EC_STATE_OPERATIONAL状态
	//	等待所有从机进入运行状态
	if(ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE) != EC_STATE_OPERATIONAL)	return 0;
	ec_readstate();// 更新所有从机状态
	
	return 1;
}

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // GPIO配置结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    // 开启外设时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // GPIO初始化
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void System_init(void)
{
	Timer2And3_Init();
    Timer2And3_start();

    delay_init();
    Led_Init();

    // 绑定w5500读取写入函数，硬件重置w5500，写入网络参数
    w5500_init();
    w5500_RegSetup();
    // 延时以保证能扫描到从机
    delay_ms(500);
}


int Test(char *ifname)
{
    //初始化soem,将主站绑定到网口 使从机运行到EC_STATE_OPERATIONAL状态
    if (!Soem_init(ifname))	return 0;

	GPIO_SetBits(GPIOA, GPIO_Pin_0);



	/* stop SOEM, close socket */
	ec_close();
	return 1;
}

int main(void)
{	
	System_init(); 
	Test("T");

    while (1)
    {
//		GPIO_SetBits(GPIOA, GPIO_Pin_0);
//        delay_ms(500);
//        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//        delay_ms(500);
    }
}
