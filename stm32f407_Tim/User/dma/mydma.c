#include "stm32f4xx.h"                  // Device header

#include "mydma.h"
uint16_t m_size;
void MyDma_Init(uint32_t addr_A, uint32_t addr_B, uint16_t size)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	m_size = size;
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = addr_A;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	
	DMA_InitStruct.DMA_Memory0BaseAddr = addr_B;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_BufferSize = size;
	DMA_InitStruct.DMA_Channel = DMA_Channel_1;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	
	DMA_Init(DMA1_Stream1,&DMA_InitStruct);
	
	// 若想初始化后立刻让DMA工作
//	DMA_Cmd(DMA1_Stream1,ENABLE);
}
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Stream1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Stream1, m_size);
	DMA_Cmd(DMA1_Stream1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_Stream1, DMA_FLAG_TCIF1) == RESET);
	DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
}



