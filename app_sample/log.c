/*
 * log.c
 *
 *  Created on: 2025/07/30
 *      Author: k-abe
 */
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "acceleration_sensor.h"
#include "Motor.h"

static UH acc_sensor_read_timing = 100;

static void null_putc(int c)      { (void)c; }             /* 送信なし */

static BOOL enable_log = 0;  /* 0:OFF, 1:ON */

void set_eneble_log(BOOL flg) {
	enable_log = flg;
}

BOOL get_enable_log(void) {
	return enable_log;
}

/* どこか一か所で呼び出す */
void update_xprintf_route(void) {
	if (enable_log) {
		/* xprintfシリアル送信関数登録 */
		xdev_out(tm_putchar);
	} else {
		xdev_out(null_putc);
	}
}

LOCAL void log_task(INT stacd, void *exinf);	// task execution function
LOCAL T_CTSK log_task_ctsk = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= log_task,
	.tskatr		= TA_HLNG | TA_RNG3,
};


EXPORT T_CTSK* get_log_task_ctsk_addr()
{
	return &log_task_ctsk;
}


LOCAL void log_task(INT stacd, void *exinf)
{
	int x, y, z = 0;
	int duty[2];

	while(1) {
		tk_dly_tsk(acc_sensor_read_timing);

		// 加速度センサーの取得
		acceleration_sensor_read(&x, &y, &z);

		// 現在のdutyを取得
		get_now_duty(duty);

		xprintf("Acc: x,y,z=%4d,%4d,%4d/Duty: left,right=%d,%d \n",
				x, y, z, duty[0], duty[1]);
	}
}

void set_acc_sensor_read_timing(unsigned short timing) {
	acc_sensor_read_timing = timing;
}

unsigned short get_acc_sensor_read_timing(void) {
	return acc_sensor_read_timing;
}
