#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h" 
#include "AD.h" 
uint32_t adc,compare;

int main(void)
{
	OLED_Init();
	PWM_Init();
	AD_Init();
	
	while(1)
	{
		adc=AD_GetValue();
		compare = adc*100/4095;
		PWM_SetCompare1(compare);
		OLED_ShowNum(1,1,compare,5);
	}

}
