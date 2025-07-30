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

static void null_putc(int c)      { (void)c; }             /* 送信なし */

static BOOL enable_log = 1;  /* 0:OFF, 1:ON */

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
