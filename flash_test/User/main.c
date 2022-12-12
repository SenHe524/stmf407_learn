#include "main.h"
//flash简介：https://blog.csdn.net/sgjiajia/article/details/110188834

extern FLASH_DATA AGV_PARAM;
int main(void)
{
	int j = 1;
//	uint16_t cnt = 0; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//	外设初始化
	delay_init_bsp(168);
	usart1_init_bsp(115200);
	
	agv_data_init();
	
//	write_flashflag();
	while(1)
	{
		for(int i = 0; i < 4; i++)
		{
			AGV_PARAM.odometer[i].data_uint32 += 486*j;
		}
		j++;
		write_agvparam();
		read_agvparam();
		delay_ms(2000);
	}
	

//	for(int i = 0; i < 4; i++)
//	{
//		save.odometer[i].data_int32 = i*8+48636;
//	}
//	while(1)
//	{
//		printf("*********save*******\n");
//		printf("ADDR_FLASH = %x\n", ((u32)0x08010000) + sizeof(FLASH_DATA) * k);
//		printf("save's address = %p\n", (uint32_t*)&save);
//		printf("num to save = %d\n", sizeof(FLASH_DATA)/4);
//		for(int i = 0; i < 4; i++)
//		{
//			printf("save.distance %d = %d\n", i+1, 
//					save.odometer[i].data_int32);
//		}
//		
//		printf("\n");
//		STMFLASH_Write(((u32)0x08010000 + sizeof(FLASH_DATA) * k), 
//				(uint32_t*)&save, sizeof(FLASH_DATA)/4);
////		printf("status: %d\n", status);
//		delay_ms(5000);
//		for(int m = 0; m <= k; m++)
//		{
//			STMFLASH_Read(((u32)0x08010000 + sizeof(FLASH_DATA) * m), (uint32_t*)&get, sizeof(FLASH_DATA)/4);
//			printf("**********get**********\n");
//	//		printf("status: %d\n", status);
//			printf("num get = %d\n", cnt);
//			for(int i = 0; i < 4; i++)
//			{
//				printf("get.distance %d = %d\n", i+1, 
//						get.odometer[i].data_int32);
//			}
//		
//		}
//		
//		printf("\n");
//		for(int i = 0; i < 4; i++)
//		{

//			save.odometer[i].data_int32 = i*8+48636*j;
//		}
//		j++;
//		k++;
//	}
	return 0;
}
//#include "sys.h"
//#include "delay.h"
//#include "usart.h"
//#include "led.h"
//#include "lcd.h"
//#include "stmflash.h" 
//#include "key.h"  

///*************************************************************************
////Tips1:	void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite) 
//	第一个元素要为4的倍数；第二个元素时地址，指写入的值的地址；第三个指的时写入的
//	元素所占用的内存，用size=ww/4+((ww%4)?1:0)求出。
//	当程序中再次掉用该函数对其他或者接下来的地址写数据的时候，第一个元素变为 上一
//	次地址4*size ，即偏移上次写入的指地址。
////Tips2: 因为可存储的是32位值，定义要写入的数组或者数值时最好以32位定义，如u32等	
//************************************************************************/
////////////////////////////////字符串测试/////////////////////////////////
////要写入到STM32 FLASH的字符串数组
//const u8 TEXT_Buffer[]={"FLASH TEST"};
//const u8 TEXT_Buffer2[]={"hello flash!"};
//#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度	
//#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)	//本次写入位数
//#define SIZE_NEXT (SIZE)*4											//下次写入递增地址数

//#define TEXT_LENTH2 sizeof(TEXT_Buffer2)	 		  	//数组长度	
//#define SIZE2 TEXT_LENTH2/4+((TEXT_LENTH2%4)?1:0) //本次写入位数
//#define SIZE_NEXT2 (SIZE2)*4											//下次写入递增地址数


//#define FLASH_SAVE_ADDR  0X0800C004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
//#define FLASH_SAVE_ADDR2  0X08020004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
//										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.
//int main(void)
//{ 
//	u8 key=0;
//	u16 i=0;
//	u8 datatemp[TEXT_LENTH]={0};	
//	u8 datatemp2[TEXT_LENTH2]={0};
//	u32 Flash_Save[3]={0};
//	int read=0,write=0;
//	
//	////////////////////////数组测试///////////////////////////
//	const u16 shuzu=sizeof(Flash_Save);	//数组占用大小
//	u16 size3=shuzu/4+((shuzu%4)?1:0);
//	u16 size_next3=size3*4;
//	u32 datatemp3[shuzu]={0};
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
//	delay_init_bsp(168);  //初始化延时函数
//	usart1_init_bsp(115200);		//初始化串口波特率为115200
//	
//	LED_Init();					//初始化LED 
// 	LCD_Init();					//LCD初始化  
// 	KEY_Init();					//按键初始化 
// 	POINT_COLOR=RED;//设置字体为红色 
//	
//	Flash_Save[2]=34;
//	
//	LCD_ShowString(30,30,200,16,16,"a=");//提示传送完成
//	LCD_ShowString(30,50,200,16,16,"b=");//提示传送完成
//	LCD_ShowString(30,130,200,16,16,"KEY1:Write  KEY0:Read");
//	
//	LCD_ShowxNum(30,70,shuzu,2,16,0);
//	write=10086;
//	while(1)
//	{
//		key=KEY_Scan(0);
//		if(key==KEY1_PRES)	//KEY1按下,写入STM32 FLASH
//		{
//			LCD_Fill(0,170,239,319,WHITE);//清除半屏    
// 			LCD_ShowString(30,170,200,16,16,"Start Write FLASH....");
//			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE);		//第一个元素必须是4的倍数写入字符串1
//			STMFLASH_Write(FLASH_SAVE_ADDR+SIZE_NEXT,(u32*)"hello flash!",SIZE2);//再次写入时，第一个元素值要比上一个偏移TEXT_LENTH（且为4的倍数）个		
//			STMFLASH_Write(FLASH_SAVE_ADDR+SIZE_NEXT+SIZE_NEXT2,(u32*)Flash_Save,size3);//写数组测试			
//			STMFLASH_Write(FLASH_SAVE_ADDR+SIZE_NEXT+SIZE_NEXT2+size_next3,(u32*)&write,1);//写一个数测试		
//			LCD_ShowString(30,170,200,16,16,"FLASH Write Finished!");//提示传送完成 
//		}

//		if(key==KEY0_PRES)	//KEY0按下,读取字符串并显示
//		{    
// 			LCD_ShowString(30,170,200,16,16,"Start Read FLASH.... ");			
//			STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);//读取字符串1
//			STMFLASH_Read(FLASH_SAVE_ADDR+SIZE_NEXT,(u32*)datatemp2,SIZE2);	//读取字符串2	
//			STMFLASH_Read(FLASH_SAVE_ADDR+SIZE_NEXT+SIZE_NEXT2,(u32*)datatemp3,size3);	//读数组测试	
//			STMFLASH_Read(FLASH_SAVE_ADDR+SIZE_NEXT+SIZE_NEXT2+size_next3,(u32*)&read,1);	//读一个数测试	
//			
//			LCD_ShowString(30,170,200,16,16,"The Data Readed Is:  ");//提示传送完成
//			LCD_ShowString(30,190,200,16,16,datatemp);//显示读到的字符串
//			LCD_ShowString(30,210,200,16,16,datatemp2);//显示读到的字符串		
//		  LCD_ShowxNum(50,230,datatemp3[0],4,16,0);	//显示数组第一位
//			LCD_ShowxNum(90,230,datatemp3[1],4,16,0);	//显示数组第二位
//			LCD_ShowxNum(130,230,datatemp3[2],4,16,0);//显示数组第三位
//			LCD_ShowxNum(50,250,read,5,16,0);	//显示读的单个数值
//			
//		}
//		
//		if(key==KEY2_PRES)	//KEY1按下,写入STM32 FLASH
//		{
//			Flash_Save[0]++;
//			LCD_ShowxNum(50,30,Flash_Save[0],4,16,0);
//		}
//		if(key==WKUP_PRES)	//KEY0按下,读取字符串并显示
//		{    
// 			Flash_Save[1]++;
//			LCD_ShowxNum(50,50,Flash_Save[1],4,16,0);
//		}
//		i++;
//		delay_ms(10);  
//		if(i==20)
//		{
//			LED0=!LED0;//提示系统正在运行	
//			i=0;
//		}		   
//	}    
//}

