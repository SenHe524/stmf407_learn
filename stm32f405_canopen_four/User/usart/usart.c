#include "sys.h"
#include "usart.h"	


extern uint8_t usart1_txbuf[BUF_MAX_LEN];
extern uint8_t usart1_rxbuf[BUF_MAX_LEN];
extern uint8_t usart6_rxbuf[BUF_MAX_LEN];

extern uint8_t usart1_cmdflag;
extern uint8_t usart1_rxlen;
extern uint8_t usart6_cmdflag;
extern uint8_t usart6_rxlen;

uint8_t usart1_dma_rxbuf[BUF_MAX_LEN] = {0};
uint8_t usart1_dma_txbuf[BUF_MAX_LEN] = {0};
uint8_t usart6_dma_rxbuf[BUF_MAX_LEN] = {0};



//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}


/***************************USART1**********************************/
 //重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//	/* 等待串口输入数据 */
//	while (USART_GetFlagStatus(USART, USART_FLAG_RXNE) == RESET);
// 
//	return (int)USART_ReceiveData(USART);
//}


#endif



void usart1_init_bsp(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	//开启相关中断
	//USART_IT_TC:发送数据完成
	//USART_IT_RXNE:数据就位中断,进入此中断即可读取数据
	//USART_IT_IDLE:线路空闲中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	//usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	usart1_rxdma_init_bsp();
	usart1_txdma_init_bsp();

}

void usart6_init_bsp(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9与GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PG9，PG14

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART6, ENABLE);  //使能串口6
	
	//开启相关中断
	//USART_IT_RXNE:数据就位中断,进入此中断即可读取数据
	//USART_IT_IDLE:线路空闲中断
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);
	
	//usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
	USART_DMACmd(USART6,USART_DMAReq_Rx, ENABLE);
	
	usart6_rxdma_init_bsp();
}

void usart1_rxdma_init_bsp(void){
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream2);
	//等待DMA可配置
	while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);			//外设地址      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)usart1_dma_rxbuf;         //内存地址      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //dma传输方向单向      
    DMA_InitStructure.DMA_BufferSize = BUF_MAX_LEN;                            	//设置DMA在传输时缓冲区的长度     
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //失能DMA的外设递增模式        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //使能DMA的内存递增模式      
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //外设数据字长         
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //内存数据字长      
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //DMA的传输模式:单次
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     //设置DMA的优先级别     
    
 	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
 	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE);
}

void usart1_txdma_init_bsp(void){
    DMA_InitTypeDef   DMA_InitStructure;    	
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                        //启动DMA时钟  
    
    DMA_DeInit(DMA2_Stream7);
	//等待DMA可配置
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);         //DMA外设基地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)usart1_dma_txbuf;         //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                     //数据传输方向，从内存读取发送到外设
	//此处设为0，以免发送时一直有数据堵塞
	DMA_InitStructure.DMA_BufferSize = 0;								//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //DMA的传输模式:单次
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       	//DMA通道优先级 
    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	//初始化DMA2_Stream7
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	
//	//DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	//使能传输完成中断:DMA_IT_TC
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	//失能DMA
    DMA_Cmd(DMA2_Stream7,DISABLE);
}

void usart6_rxdma_init_bsp(void){
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream1);
	//等待DMA可配置
	while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);			//外设地址      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)usart6_dma_rxbuf;         //内存地址      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //dma传输方向单向      
    DMA_InitStructure.DMA_BufferSize = BUF_MAX_LEN;                            	//设置DMA在传输时缓冲区的长度     
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //失能DMA的外设递增模式        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //使能DMA的内存递增模式      
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //外设数据字长         
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //内存数据字长      
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //DMA的传输模式:单次
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     //设置DMA的优先级别     
    
 	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
 	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream1, ENABLE);
}


//DMA发送
void usart1_sendbuf(const uint8_t* data, uint8_t len)
{   
//	//清除TC中断标志位
//	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
//	//等待上次传输完成
//	while(DMA_GetCurrDataCounter(DMA2_Stream7))
//	{
////		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7) == SET)
////		{
//			//清除TC中断标志位
//			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
////		}	
//	};
//	while(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7) == RESET);
//	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	while(DMA_GetCurrDataCounter(DMA2_Stream7));
	//使能串口DMA发送
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	//准备数据
	memcpy(usart1_dma_txbuf, data, len); 
	
	//失能DMA2_Stream7
	DMA_Cmd(DMA2_Stream7, DISABLE);
	//等待DMA2_Stream7可配置
	while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){};
	//DMA通道的DMA缓存的大小,即传输的数据长度
 	DMA_SetCurrDataCounter(DMA2_Stream7, len);
	//使能串口1的DMA发送
 	DMA_Cmd(DMA2_Stream7, ENABLE);
}
void usart6_sendbuf(const uint8_t* data, uint8_t len)
{
	uint8_t i = 0;
	for(i = 0; i < len; i++)
	{
		while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
		USART6->DR = data[i];  
	}
}
//DMA中断读取
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t clear_flag;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //接收中断
	{
		// 读取SR和DR清零空闲中断
		clear_flag = USART1->SR;  
        clear_flag = USART1->DR;
		DMA_Cmd(DMA2_Stream2,DISABLE);
		
		/*处理接收到的数据*/
		usart1_cmdflag = 1;//串口1数据接收到的标志位
		//转存数据
		usart1_rxlen = BUF_MAX_LEN - DMA_GetCurrDataCounter(DMA2_Stream2);
		memcpy(usart1_rxbuf, usart1_dma_rxbuf, usart1_rxlen);
		
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2);
		DMA_SetCurrDataCounter(DMA2_Stream2, BUF_MAX_LEN);  
		DMA_Cmd(DMA2_Stream2, ENABLE); 
	}
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	(void) clear_flag;
}


//DMA中断读取
void USART6_IRQHandler(void)                	//串口6中断服务程序
{
	uint8_t clear_flag;
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)  //接收中断
	{
		// 读取SR和DR清零空闲中断
		clear_flag = USART6->SR;  
        clear_flag = USART6->DR;
		DMA_Cmd(DMA2_Stream1,DISABLE);
		
		/*处理接收到的数据*/
		usart6_cmdflag = 1;//串口6数据接收到的标志位
		//转存数据
		usart6_rxlen = BUF_MAX_LEN - DMA_GetCurrDataCounter(DMA2_Stream1);
		memcpy(usart6_rxbuf, usart6_dma_rxbuf, usart6_rxlen);
		
		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);
		DMA_SetCurrDataCounter(DMA2_Stream1, BUF_MAX_LEN);  
		DMA_Cmd(DMA2_Stream1, ENABLE); 
	}
	USART_ClearITPendingBit(USART6, USART_IT_IDLE);
	(void) clear_flag;
}

void DMA2_Stream7_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7) != RESET) 
	{
		//清除标志位
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
		//关闭DMA
		DMA_Cmd(DMA2_Stream7,DISABLE);
	}
}
//中断读取
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
//	uint8_t Res;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		Res =USART_ReceiveData(USART1);//读取接收到的数据
//		usart1_rcv(Res);
//	}
//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//}
//中断读取
//void USART6_IRQHandler(void)                	//串口6中断服务程序
//{
//	uint8_t Res;
//	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		Res =USART_ReceiveData(USART6);//读取接收到的数据
//		usart6_rcv(Res);
//	}
//	USART_ClearITPendingBit(USART6, USART_IT_RXNE);
//}

//普通发送
//void usart1_sendbuf(const uint8_t* data, uint8_t len)
//{
//	uint8_t i = 0;
//	for(i = 0; i < len; i++)
//	{
//		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//		USART1->DR = data[i];  
//	}
//}
//void usart6_sendbuf(const uint8_t* data, uint8_t len)
//{
//	uint8_t i = 0;
//	for(i = 0; i < len; i++)
//	{
//		while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
//		USART6->DR = data[i];  
//	}
//}
