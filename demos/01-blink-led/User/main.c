#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef structure;
	structure.GPIO_Mode = GPIO_Mode_Out_PP;
	structure.GPIO_Pin = GPIO_Pin_0;
	structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&structure);
	
	
	
	while(1)
	{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//		Delay_ms(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
//		Delay_ms(500);
//	
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//		Delay_ms(500);

	}

}
