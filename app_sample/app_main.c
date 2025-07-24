#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"	// xatoi

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "Hook.h"
#include "SysCtrl.h"
#include "button.h"
#include "pwm.h"
#include "acceleration_sensor.h"
#include "led.h"

LOCAL void trace_task(void);
// 周期ハンドラの関連情報
const T_CCYC ccyc = {0, TA_HLNG, &trace_task, 1, 0};
ID cycid;							// 周期ハンドラSのID

/* usermain関数 */
EXPORT INT usermain(void)
{
	ID ntshell_tskid; // Task ID number

	tm_putstring((UB*)"Start User-main program.\n");

	device_initialize();

	/* xprintfシリアル送信関数登録 */
	xdev_out(tm_putchar);

	IMPORT T_CTSK* get_ntshell_task_ctsk_addr();
	ntshell_tskid = tk_cre_tsk(get_ntshell_task_ctsk_addr());
	tk_sta_tsk(ntshell_tskid, 0);

	cycid = tk_cre_cyc(&ccyc);	// 周期ハンドラの生成
	tk_sta_cyc(cycid);			// 周期ハンドラSの動作開始

	tk_slp_tsk(TMO_FEVR);

	return 0;
}

void trace_task(void)
{
    sc_traceMain();
}
