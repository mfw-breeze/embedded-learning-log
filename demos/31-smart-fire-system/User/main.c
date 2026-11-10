#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "USART1.h"
#include <stdio.h>
#include "MQ2.h"
#include "MyADC.h"
#include "DHT11.h"
#include "alarm.h"
#include "Buzzer.h"
#include "Relay.h"
int main(void)
{
	uint8_t  i;
	uint16_t Smoke  = 0;         /* ★ 四个变量改成本地声明 */
	uint16_t Temp   = 0;
	uint16_t Humi   = 0;
	uint8_t  DhtCnt = 0;
	uint8_t  PrintCnt = 0;
	
		
	uint8_t  relayNow  = 0;          /* 继电器当前状态：0=断，1=吸合 */
	uint8_t  wantRelay = 0;          /* 继电器期望状态 */

	OLED_Init();
	LED_Init();
	USART1_Init();
	MQ2_Init();
	DHT11_Init();
	Buzzer_Init();
	Relay_Init();
	

	LED1_OFF();
	LED2_ON();
	


	/* ---------- MQ-2 预热 30 秒 ---------- */
	OLED_ShowString(1, 1, "Warming up...");
	for (i = 0; i < 30; i++)
	{
		OLED_ShowNum(2, 1, 30 - i, 2);
		Delay_ms(1000);
	}
	OLED_Clear();

	/* ★ 预热完再初始化基线：从稳定状态开始跟踪 */
	Alarm_Init();

		while (1)
	{
		/* ---- 1. 采样 ---- */
		Smoke = MQ2_GetRaw();

		if (++DhtCnt >= 10)                       /* 10 × 200ms = 2 秒 */
		{
			uint8_t h = 0, t = 0;
			DhtCnt = 0;
			if (DHT11_ReadData(&h, &t) == 0)
			{
				Humi = h;
				Temp = t;
				Alarm_FeedTemp((uint8_t)Temp);    /* ★ 新增：喂温度 */
			}
		}

		/* ---- 2. 状态机 ---- */
		Alarm_Update(Smoke);

		/* ---- 3. 输出：按当前状态执行（幂等，每轮都设一遍） ---- */
		
		wantRelay = (Alarm_GetState() == ALARM_FIRE) ? 1 : 0; 
		
		switch (Alarm_GetState())
		{
		case ALARM_FIRE:                          /* 火警 */
			LED1_ON();  LED2_OFF();
			Buzzer_SetMode(BUZZ_FAST);
			Relay_ON();  
			break;

		case ALARM_WARNING:                       /* 预警 */
			LED1_OFF(); LED2_ON();
			Buzzer_SetMode(BUZZ_SLOW);
			Relay_OFF();  						  /*预警不喷水 */
			break;

		default:                                  /* 正常 */
			LED1_OFF(); LED2_ON();
			Buzzer_SetMode(BUZZ_OFF);
			Relay_OFF();  
			break;
		}
		
		    /* 4-2 继电器：只在状态变化时动作，动作后丢弃受扰采样 */
		if (wantRelay != relayNow)
		{
			relayNow = wantRelay;

			if (wantRelay) Relay_ON();
			else           Relay_OFF();

			Delay_ms(50);                    /* 等触点弹跳 + 电源恢复 */
			(void)MQ2_GetRaw();              /* 丢弃第 1 次 */
			(void)MQ2_GetRaw();              /* 丢弃第 2 次 */
		}
		Buzzer_Task();                            /* 节奏控制，每轮调 */

		/* ---- 4. 显示 ---- */
		OLED_ShowString(1, 1, "S:");
		OLED_ShowNum(1, 3, Smoke, 4);
		OLED_ShowString(1, 8, "B:");
		OLED_ShowNum(1, 10, Alarm_GetBaseline(), 4);

		OLED_ShowString(2, 1, "T:");
		OLED_ShowNum(2, 3, Temp, 2);
		OLED_ShowString(2, 6, "H:");
		OLED_ShowNum(2, 8, Humi, 2);

		if (Alarm_GetState() == ALARM_FIRE)         OLED_ShowString(3, 1, "FIRE   ");
		else if (Alarm_GetState() == ALARM_WARNING) OLED_ShowString(3, 1, "WARNING");
		else                                        OLED_ShowString(3, 1, "NORMAL ");

		/* ---- 5. CSV 上报 ---- */
		if (++PrintCnt >= 5)
		{
			PrintCnt = 0;
			printf("%u,%u,%u,%u,%u,%lu\r\n",
			       (unsigned)Smoke,
			       (unsigned)Alarm_GetBaseline(),
			       (unsigned)Temp,
			       (unsigned)Humi,
			       (unsigned)Alarm_GetState(),
			       MyADC_GetVdda_mV());
		}

		Delay_ms(200);
	}
}



