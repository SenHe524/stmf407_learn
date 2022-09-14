#include "stm32f4xx.h" // Device header

#include <string.h>
#include <stdio.h>

#include "delay.h"
#include "ecatfun.h"


int flag_led = 0;
int Test(char *ifname)
{
	int j = 1;
	//	初始化硬件
	System_init();
	//	初始化soem,将主站绑定到网口 使从机运行到EC_STATE_OPERATIONAL状态
	while (!Soem_init(ifname))	
	{
		flag_led = 0;
//		return 0;
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	while(j)
	{

	}
	Soem_close();
	return 1;
}

int main(void)
{	
	 
	Test("eth");

    while (1)
    {
		if(!flag_led)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			delay_ms(500);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			delay_ms(500);
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			delay_ms(1500);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			delay_ms(1500);
		}
		
    }
}
