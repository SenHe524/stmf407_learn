#ifndef _ETHERCATCONTROL_H_
#define _ETHERCATCONTROL_H_
#ifdef __cplusplus
extern "C"
{
#endif

//	int EtherCat_Open(void);
//	int EtherCat_Send(uint8_t *pBuff, int Len);
//	int EtherCat_Recv(uint8_t *pBuff, int Len);
//	
    int EtherCat_MACRAW_Open(void);
	int EtherCat_MACRAW_Send(uint8_t *pBuff, int Len);
	int EtherCat_MACRAW_Recv(uint8_t *pBuff, int Len);
	void EtherCat_Close(void);
	
#ifdef __cplusplus
}
#endif

#endif
