#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
int main(void)
{
	OLED_Init();
	Serial_Init();
	
//	Serial_SendByte(0x41);
	
//	uint8_t myarry[]={0x01,0x02,0x03,0x04};
//	Serial_SendArray(myarry,4);
	
//	Serial_SendString("HelloWord!\r\n");
	
//	Serial_SendNumber(12345,5);
	
//	printf("Num==%d\r\n",999);
	
	Serial_Printf("你好呀！");
	
	while(1)
	{
	
	}

}
