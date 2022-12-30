// Includes for the Canfestival driver
#include "can1.h"


static CO_Data *co_data = NULL;
Message rxm;
//	SDO数据缓存
uint8_t buf_temp[4][4] = {0};
//	rpdo的标志位
uint8_t rpdo_flag = 0;
//	
int8_t write_flag[4] = {-2, -2, -2, -2};


//Initialize the CAN hardware 
unsigned char can1_init_bsp(CO_Data * d)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	/* save the canfestival handle */  
	co_data = d;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, GPIO_AF_CANx);
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, GPIO_AF_CANx); 

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	/* NVIC configuration *******************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = CANx_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* CAN configuration ********************************************************/  
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/* CAN register init */
	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;//非时间触发通信模式
	CAN_InitStructure.CAN_ABOM = ENABLE;//软件自动离线管理  
	CAN_InitStructure.CAN_AWUM = ENABLE;//睡眠模式通过软件唤醒
	CAN_InitStructure.CAN_NART = DISABLE;//允许报文自动传送 NART:no automatic retransmission不自动重传 
	CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定,新的覆盖旧的 
	CAN_InitStructure.CAN_TXFP = DISABLE;//优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	/* CAN Baudrate (CAN clocked at 42 MHz)  42e6 / ( prescaler * (1+BS1+BS2))  */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
	CAN_InitStructure.CAN_Prescaler = 12;
	CAN_Init(CANx, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

	return 1;
}

// The driver send a CAN message passed from the CANopen stack
unsigned char canSend(CAN_PORT notused, Message *m)
{
	int i, ret;
	CanTxMsg TxMessage = {0};
	TxMessage.StdId = m->cob_id;
	TxMessage.IDE = CAN_ID_STD;
	if(m->rtr)
		TxMessage.RTR = CAN_RTR_REMOTE;
	else
		TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = m->len;
	for(i=0 ; i<m->len ; i++)
		TxMessage.Data[i] = m->data[i];

//	printf("send:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",TxMessage.StdId, TxMessage.DLC,TxMessage.Data[0],TxMessage.Data[1]
//										,TxMessage.Data[2], TxMessage.Data[3],TxMessage.Data[4],
//			TxMessage.Data[5],TxMessage.Data[6], TxMessage.Data[7]);
	i = 0;
	ret = CAN_Transmit(CANx, &TxMessage);

	while((CAN_TransmitStatus(CANx, ret) != CAN_TxStatus_Ok) && (i < 0xFFF))
		i++;
	if(i >= 0xfff)
		return 0;//fail
	return 1;//success
}


void sdodata_handle(Message *m)
{
	UNS16 cob_id = UNS16_LE(m->cob_id);
	uint8_t len = 0;
	uint8_t i = 0;
	//根据cob_id填充数据缓存
	switch(cob_id)
	{
		case 0x581:
			rpdo_flag = 0;
			break;
		case 0x582:
			rpdo_flag = 1;
			break;
		case 0x583:
			rpdo_flag = 2;
			break;
		case 0x584:
			rpdo_flag = 3;
			break;
		default:
			return ;
			break;
	}
	//根据数据头选择数据长度或判断数据是否写入成功
	switch(m->data[0])
	{
		case 0x4F:
			len = 1;
			break;
		case 0x4B:
			len = 2;
			break;
		case 0x47:
			len = 3;
			break;
		case 0x43:
			len = 4;
			break;
		case 0x60:
			write_flag[rpdo_flag] = 1;
			break;
		case 0x80:
			write_flag[rpdo_flag] = 0;
			break;
		default:
			return ;
			break;
	}
	for(i = 0; i< len; i++)
		buf_temp[rpdo_flag][i] = m->data[i+4];
}


void can_handle(Message* rxm)
{
	//调用数据处理函数：此函数目前只处理PDO数据
	canDispatch(co_data, rxm);
	if(rxm->cob_id >> 7 == 0xB)
	{//调用自己的SDO数据处理
		sdodata_handle(rxm);
	}
}

/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
	int i = 0;
	CanRxMsg RxMessage = {0};
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	// Drop extended frames
	if(RxMessage.IDE == CAN_ID_EXT) //不处理扩展帧
		return;
	rxm.cob_id = RxMessage.StdId;
	if(RxMessage.RTR == CAN_RTR_REMOTE)//远程帧
		rxm.rtr = 1;
	rxm.len = RxMessage.DLC;
	for(i=0 ; i<8 ; i++)
		 rxm.data[i] = 0;
	for(i=0 ; i<rxm.len ; i++)
		 rxm.data[i] = RxMessage.Data[i];
//	printf("rec:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",rxm.cob_id,rxm.len,rxm.data[0], rxm.data[1],rxm.data[2],rxm.data[3]
//										,rxm.data[4], rxm.data[5],rxm.data[6],rxm.data[7]);	
	//数据处理
	can_handle(&rxm);
}


