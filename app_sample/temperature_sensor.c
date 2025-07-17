/*
 * temperature_sensor.c
 *
 *  Created on: 2025/07/17
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#define TEMP(r)				(TEMP_BASE + TEMP_##r)
#define TEMP_BASE           0x4000C000
#define TEMP_TASKS_START    0x000
#define TEMP_TASKS_STOP     0x004
#define TEMP_EVENTS_DATARDY 0x100
#define TEMP_TEMP           0x508

EXPORT float read_temperature(void)
{
    out_w(TEMP(EVENTS_DATARDY), 0);
    out_w(TEMP(TASKS_START), 1);

    while (in_w(TEMP(EVENTS_DATARDY)) == 0);  // データ準備まで待機

    int raw = in_w(TEMP(TEMP));           // 単位: 0.25℃
    out_w(TEMP(TASKS_STOP), 1);

    return raw / 4.0f;	// 実温度に変換（例: 100 → 25.0℃）
}
