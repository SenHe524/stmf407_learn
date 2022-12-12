#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"   
#include "main.h"
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  


typedef struct
{
	 union_uint32 odometer[4];
}FLASH_DATA;


//扇区size
#define FLASH_SECTOR_SIZE		0x10000
//将扇区分为64个存储块
#define MAX_SAVE_AGV_DATA_CNT	64
//每个存储块数据最大size
#define MAX_DATA_SIZE			FLASH_SECTOR_SIZE / MAX_SAVE_AGV_DATA_CNT
//每个存储块最多存储多少个4字节数据
#define MAX_DATA_BYTE			MAX_DATA_SIZE / 4
//第一个存储块初始地址
#define FLASH_BEGIN_ADD			ADDR_FLASH_SECTOR_4	
////第一个存储块用于存储目前使用了多少个存储块
//#define FLASH_FLAG_ADD			FLASH_BEGIN_ADD
//从第二个存储块开始存储数据
#define FLASH_DATA_ADD			FLASH_BEGIN_ADD + MAX_DATA_SIZE		
//AGV数据大小
#define AGV_DATA_SIZE 			(sizeof(FLASH_DATA))
//AGV数据占据多少个4字节数据
#define AGV_DATA_BYTE 			(AGV_DATA_SIZE / 4) + ((AGV_DATA_SIZE % 4)? 1:0)
void read_flashflag(void);
void write_flashflag(void);
uint8_t read_agvparam(void);
void write_agvparam(void);
void agv_data_init(void);



//读出字
uint32_t STMFLASH_ReadWord(uint32_t faddr);
//从指定地址开始写入指定长度的数据
FLASH_Status STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);
//从指定地址开始读出指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);
						   
#endif

















