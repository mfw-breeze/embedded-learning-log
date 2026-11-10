#ifndef __ALARM_H
#define __ALARM_H



#include "stm32f10x.h"

/* ---------- 烟雾判据---------- */
#define ALARM_RISE_PCT      32      /* 1262 x 32% = 404 */
#define ALARM_FALL_PCT       16      /* 解除点：1262 + 202 = 1464 */
#define ALARM_MIN_MARGIN   400      /* 绝对下限，防基线过低 */
#define ALARM_CONFIRM_CNT    8      /* 8 x 200ms = 1.6秒确认 */
#define ALARM_BASE_WEIGHT   64      /* 基线跟踪时间常数，越大越慢 */

/*----------- 温度判据 ---------*/
#define ALARM_TEMP_ABS      45      /*绝对温度≥45°C-->异常*/
#define ALARM_TEMP_RISE      3		/*10秒内升温≥3°C-->异常*/

#define ALARM_TEMP_HIST      5		/*5次×2s=10秒窗口*/
#define ALARM_FIRE_CNT       5		/*连续5次（1s）满足升级条件-->火警*/
#define ALARM_COOL_CNT      10		/*连续10次（2s）温度正常-->降级*/

/* ---------- 状态 ---------- */
typedef enum {
	ALARM_NORMAL = 0,
	ALARM_WARNING = 1,
	ALARM_FIRE   = 2
} AlarmState_t;

/* ---------- 事件（Update 的返回值）---------- */
#define ALARM_EVT_NONE   0      /* 无变化 */
#define ALARM_EVT_WARN   1      /* 正常-->预警 */
#define ALARM_EVT_FIRE   2      /* 预警-->火警 */
#define ALARM_EVT_DOWN   3      /* 火警-->预警 */
#define ALARM_EVT_CLEAR  4      /* 任意-->正常 */

/* ---------- 接口 ---------- */
void          Alarm_Init(void);
uint8_t       Alarm_Update(uint16_t smoke);   /* 每 200ms 调一次 */
void          Alarm_FeedTemp(uint8_t temp);   /* 每次读到 DHT11 调一次 */
AlarmState_t  Alarm_GetState(void);
uint16_t      Alarm_GetBaseline(void);
uint8_t       Alarm_IsTempBad(void);

#endif



