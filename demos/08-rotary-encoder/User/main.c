#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

int16_t Count;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"Count:");
	 while(1)
	{
		Count += Encoder_Get();
		OLED_ShowNum(1,7,Count,5);
	}

}
