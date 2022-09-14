#include "stm32f4xx.h" // Device header

#include "ethercatcontrol.h"
#include "socket.h"
#include "string.h"

#ifndef DATA_BUF_SIZE
	#define DATA_BUF_SIZE			2048
#endif

uint8_t sn;
//int EtherCat_Open(void)
//{
//	socket(sn, Sn_MR_TCP, 8888, SF_TCP_NODELAY);
//	if (listen(sn) != SOCK_OK)
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//		return 0;
//	}
//	return 1;
//}

//int EtherCat_Send(uint8_t *pBuff, int Len)
//{
//	int32_t ret, i = 1;
//	int tempsize = Len;
//	uint16_t sentsize=0;
//	while(Len != sentsize)
//	{
//		tempsize = Len - DATA_BUF_SIZE * i;
//		if(tempsize > 0)
//		{
//			while(sentsize != (DATA_BUF_SIZE * i))
//			{
//				ret = send(sn, pBuff+sentsize, (DATA_BUF_SIZE * i)-sentsize);
//				if(ret < 0)
//				{
//					close(sn);
//					return ret;
//				}
//				sentsize += ret;
//			}
//			i++;
//		}
//		else
//		{
//			while(sentsize != Len)
//			{
//				ret = send(sn, pBuff+sentsize, Len-sentsize);
//				if(ret < 0)
//				{
//					close(sn);
//					return ret;
//				}
//				sentsize += ret;
//		
//			}

//		}
//	}
//	return 1;
//}

//int EtherCat_Recv(uint8_t *pBuff, int Len)
//{
//	int ret;
//	int size = getSn_RX_RSR(sn);
//	
//	if(size > 0)
//	{
//		if(size > Len) size = Len;
//		ret = recv(sn, pBuff, size);
//		if(ret <= 0) return ret;
//	}
//	else
//	{
//		return 0;
//	}
//	return size;
//}

void EtherCat_Close(void)
{
	close(sn);
}

int EtherCat_MACRAW_Open(void)
{
	socket(sn, Sn_MR_MACRAW, 8000, SF_ETHER_OWN);//SF_ETHER_OWN
	
	if ((getSn_SR(0) & SOCK_MACRAW) != SOCK_MACRAW)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		return 0;
	}
	return 1;
}

int EtherCat_MACRAW_Send(uint8_t *pBuff, int Len)
{
	uint16_t ret=0;
	
	if(Len > getSn_TxMAX(sn)) ret = getSn_TxMAX(sn);
	else ret = Len;
	
	wiz_send_data(sn, pBuff, Len);
	
	setSn_CR(sn, Sn_CR_SEND);
	while(getSn_CR(sn));
	while((getSn_IR(sn) & Sn_IR_SENDOK) != Sn_IR_SENDOK);
	setSn_IR(sn, Sn_IR_SENDOK);//清除中断标志位
	return ret;
}

int EtherCat_MACRAW_Recv(uint8_t *pBuff, int Len)
{
	uint16_t data_len=0;
	if(getSn_RX_RSR(sn) > 0)
	{
		wiz_recv_data(sn, pBuff, 2);
		setSn_CR(sn, Sn_CR_RECV);// 利用RECV命令更新Sn_RX_RD
		data_len = pBuff[0];
		data_len = ((data_len << 8) & 0xFF00) | pBuff[1];
		data_len -= 2;
		if(data_len > Len)
		{
			data_len = Len;
		}
		memset(pBuff, 0x00, data_len+1);
		wiz_recv_data(sn, pBuff, data_len);
		setSn_CR(sn, Sn_CR_RECV);// 利用RECV命令更新Sn_RX_RD
		while(getSn_CR(sn));
	}

	return data_len;
}






