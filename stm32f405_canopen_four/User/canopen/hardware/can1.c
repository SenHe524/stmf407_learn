// Includes for the Canfestival driver
#include "can1.h"


static CO_Data *co_data = NULL;
extern Message rxm;

//Initialize the CAN hardware 
unsigned char CAN1_Init(CO_Data * d)
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
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
	CAN_InitStructure.CAN_BS1 = CAN_BS1_16tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
	CAN_InitStructure.CAN_Prescaler = 4;
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
	printf("send:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",TxMessage.StdId, TxMessage.DLC,TxMessage.Data[0],TxMessage.Data[1]
										,TxMessage.Data[2], TxMessage.Data[3],TxMessage.Data[4],
			TxMessage.Data[5],TxMessage.Data[6], TxMessage.Data[7]);	
	i = 0;
	ret = CAN_Transmit(CANx, &TxMessage);
	while((CAN_TransmitStatus(CANx, ret) == CAN_TxStatus_Failed) && (i < 0xFFF))
		i++;
	if(i > 0xfff) return 0;//fail
	return 1;//success
}

//can口接收数据查询
//buf：数据缓存区
//返回值：0，无数据被接收，其它，接收的数据长度
unsigned char CAN1_Receive_Msg(unsigned char *buf){

	u32 i;
	CanRxMsg RxMessage;
	if(CAN_MessagePending(CAN1,CAN_FIFO0==0))return 0;//没有接收到数据，直接退出
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);//从CAN1 FIF0接收数据
	for(i=0; i<RxMessage.DLC; i++)
	buf[i] = RxMessage.Data[i];
	return RxMessage.DLC;//返回接收的数据长度
}

unsigned char canChangeBaudRate_driver( CAN_HANDLE fd, char* baud)
{
	return 0;
}

/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
	int i;
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
	printf("rec:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",rxm.cob_id,rxm.len,rxm.data[0], rxm.data[1],rxm.data[2],rxm.data[3]
										,rxm.data[4], rxm.data[5],rxm.data[6],rxm.data[7]);	
		canDispatch(co_data, &rxm);
}

