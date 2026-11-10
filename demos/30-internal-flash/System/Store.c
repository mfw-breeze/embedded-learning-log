#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

#define STORE_START_ADDRESS 0x0800FC00

uint16_t Store_Data[512]={0};

void Store_Init(void)
{
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)
	{
		MyFLASH_ErisePage(STORE_START_ADDRESS);
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS,0xA5A5);
		for(uint16_t i = 1;i < 512;i++)
		{
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i*2,0x0000);
			
		}
	
	}
	for(uint16_t i = 0;i < 512;i++)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord (STORE_START_ADDRESS + i*2);
	
	}
}
void Store_Save(void)
{
	MyFLASH_ErisePage(STORE_START_ADDRESS);
	for(uint16_t i = 0;i < 512;i++)
	{
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i*2,Store_Data[i]);
	
	}
}

void Store_Clear(void)
{
	for(uint16_t i = 1;i < 512;i++)
	{
		Store_Data[i]=0x0000;
	
	}
	Store_Save();
}
