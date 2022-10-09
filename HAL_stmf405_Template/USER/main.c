#include "main.h"

CAN_HandleTypeDef hcan1;

static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_CAN1_Init(void);
uint8_t bsp_can1_send_msg(uint16_t ID,uint8_t *pData,uint8_t DATA_Lenth);
CAN_TxHeaderTypeDef        TxMessage;
CAN_RxHeaderTypeDef        RxMessage;
uint8_t  Data[8];
int main(void)
{

	HAL_Init();
	/* Configure the system clock to 168 MHz */
	SystemClock_Config();
	hcan1.Instance = CAN1;
	HAL_CAN_MspInit(&hcan1);
	MX_CAN1_Init();


	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.TransmitGlobalTime = DISABLE;
	
	while (1)
	{
		//		if(HAL_CAN_Transmit(&hcan1, 10) != HAL_OK)
//		{
//		  /* Transmission Error */
//		//      Error_Handler();
//			printf("Im");
//		}
		uint8_t  Send_Buffer[8]={40,00,20,00,00,00,00,00};
		bsp_can1_send_msg(0x604, Send_Buffer, 8);
		HAL_Delay(200);
	}
}


uint8_t bsp_can1_filter_config(void)
{
    CAN_FilterTypeDef filter = {0};
    filter.FilterActivation = ENABLE;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterIdLow = 0;
    filter.FilterIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.FilterMaskIdHigh = 0;
	filter.FilterBank = 0;
	filter.SlaveStartFilterBank = 0;
    HAL_CAN_ConfigFilter(&hcan1, &filter);
    return 1;
}
/* CAN1 init function */

static void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 4;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  bsp_can1_filter_config();
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
  /* USER CODE END CAN1_Init 2 */
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t index = 0;
	uint8_t recv_data[8];

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxMessage, recv_data);
	if(RxMessage.IDE == CAN_ID_STD)
	{
          printf("StdId ID:%d\n",RxMessage.StdId);
	}
	else
	{
          printf("ExtId ID:%d\n",RxMessage.ExtId);
	}
	printf("CAN IDE:0x%x\n",RxMessage.IDE);
	printf("CAN RTR:0x%x\n",RxMessage.RTR);
	printf("CAN DLC:0x%x\n",RxMessage.DLC);
	printf("RECV DATA:");
	for(index = 0; index < RxMessage.DLC; index++)
	{
          printf("0x%x ",recv_data[index]);
	}
	printf("\n");
}
uint8_t bsp_can1_send_msg(uint16_t ID,uint8_t *pData,uint8_t DATA_Lenth)
{
	uint32_t free_level = 0U;
	TxMessage.StdId = ID;  //标准帧ID
	TxMessage.DLC   = DATA_Lenth;//发送的帧长度
	TxMessage.IDE   = CAN_RTR_DATA;//数据帧
	while(free_level==0)//一共有三个邮箱，只要有一个邮箱空闲就可以了。 
	{
		free_level=HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
	}
	HAL_CAN_AddTxMessage(&hcan1,&TxMessage,pData,(uint32_t*)CAN_TX_MAILBOX0); 
	return 0;
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}



static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT


void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
