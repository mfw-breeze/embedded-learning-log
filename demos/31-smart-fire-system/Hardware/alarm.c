#include "stm32f10x.h"
#include "alarm.h"

static AlarmState_t state = ALARM_NORMAL;
static uint16_t baseline  = 0;
static uint8_t  inited    = 0;
static uint8_t  overCnt   = 0;
static uint8_t  underCnt  = 0;
static uint8_t  fireCnt   = 0;
static uint8_t  coolCnt   = 0;

static uint8_t tempHist[ALARM_TEMP_HIST];
static uint8_t tempIdx = 0;
static uint8_t tempCnt = 0;
static uint8_t tempBad = 0;

void Alarm_Init(void)
{
	state = ALARM_NORMAL;
	baseline = 0;  inited = 0;
	overCnt = underCnt = fireCnt = coolCnt = 0;
	tempIdx = 0;  tempCnt = 0;  tempBad = 0;
}

/* 每次读到新温度时调用（约 2 秒一次） */
void Alarm_FeedTemp(uint8_t temp)
{
	uint8_t i, mn;

	tempHist[tempIdx] = temp;
	tempIdx = (uint8_t)((tempIdx + 1) % ALARM_TEMP_HIST);
	if (tempCnt < ALARM_TEMP_HIST) tempCnt++;

	/* 窗口内最小值 */
	mn = temp;
	for (i = 0; i < tempCnt; i++)
		if (tempHist[i] < mn) mn = tempHist[i];

	/* 绝对超温 或 温升过快 → 异常 */
	if (temp >= ALARM_TEMP_ABS || (uint8_t)(temp - mn) >= ALARM_TEMP_RISE)
		tempBad = 1;
	else
		tempBad = 0;
}

uint8_t Alarm_Update(uint16_t smoke)
{
	uint32_t margin, rel;

	/* 首次：当前值作基线 */
	if (!inited) { baseline = smoke; inited = 1; return ALARM_EVT_NONE; }

	/* 只在正常态跟踪基线；预警/火警期间冻结 */
	if (state == ALARM_NORMAL)
		baseline = (uint16_t)(((uint32_t)baseline * (ALARM_BASE_WEIGHT - 1) + smoke)
		                      / ALARM_BASE_WEIGHT);

	margin = (uint32_t)baseline * ALARM_RISE_PCT / 100;
	if (margin < ALARM_MIN_MARGIN) margin = ALARM_MIN_MARGIN;

	rel = (uint32_t)baseline * ALARM_FALL_PCT / 100;
	if (rel < ALARM_MIN_MARGIN / 2) rel = ALARM_MIN_MARGIN / 2;

	switch (state)
	{
	case ALARM_NORMAL:
		if (smoke > baseline + margin)
		{
			if (++overCnt >= ALARM_CONFIRM_CNT)
			{ overCnt = 0; fireCnt = 0; state = ALARM_WARNING; return ALARM_EVT_WARN; }
		}
		else overCnt = 0;
		break;

	case ALARM_WARNING:
		/* 先判断是否回落 */
		if (smoke < baseline + rel)
		{
			if (++underCnt >= ALARM_CONFIRM_CNT)
			{ underCnt = 0; fireCnt = 0; state = ALARM_NORMAL; return ALARM_EVT_CLEAR; }
		}
		else underCnt = 0;

		/* 再判断是否升级（烟雾仍高 且 温度异常） */
		if (smoke > baseline + margin && tempBad)
		{
			if (++fireCnt >= ALARM_FIRE_CNT)
			{ fireCnt = 0; coolCnt = 0; state = ALARM_FIRE; return ALARM_EVT_FIRE; }
		}
		else fireCnt = 0;
		break;

	case ALARM_FIRE:
		/* 温度回正常 → 降级为预警 */
		if (!tempBad)
		{
			if (++coolCnt >= ALARM_COOL_CNT)
			{ coolCnt = 0; underCnt = 0; state = ALARM_WARNING; return ALARM_EVT_DOWN; }
		}
		else coolCnt = 0;
		break;

	default:
		state = ALARM_NORMAL;
		break;
	}

	return ALARM_EVT_NONE;
}

AlarmState_t Alarm_GetState(void)    { return state; }
uint16_t     Alarm_GetBaseline(void) { return baseline; }
uint8_t      Alarm_IsTempBad(void)   { return tempBad; }
