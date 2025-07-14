/*
 * nt-shell_task.c
 *
 *  Created on: Aug 18, 2024
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

//#include <stdio.h>
#include "ntshell.h"
#include "usrcmd.h"


ID dd_com;

static int serial_read(char* buf, int cnt, void* extobj) {
	SZ asz_com;
	ER err;

	err = tk_srea_dev(dd_com, 0, (UB*)buf, cnt, &asz_com);
	if (err != E_OK) {
		tm_printf((UB*)"READ Error %d\n", dd_com);
		return 0;
	}

	return (int)asz_com;
}

static int serial_write(const char* text, int cnt, void* extobj) {
	SZ asz_com;

	tk_swri_dev(dd_com, 0, (UB*)text, cnt, &asz_com);

	return (int)asz_com;
}

static int user_callback(const char* text, void* extobj) {
	usrcmd_execute(text);
	return 0;
}


LOCAL void ntshell_task(INT stacd, void *exinf);	// task execution function
LOCAL T_CTSK ntshell_task_ctsk = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 2048,	// 1024
	.task		= ntshell_task,
	.tskatr		= TA_HLNG | TA_RNG3,
};


EXPORT T_CTSK* get_ntshell_task_ctsk_addr()
{
	return &ntshell_task_ctsk;
}


LOCAL void ntshell_task(INT stacd, void *exinf)
{
	ntshell_t nts;

//	setbuf(stdout, NULL);
	ntshell_init(&nts, serial_read, serial_write, user_callback, 0);
	ntshell_set_prompt(&nts, ">");

	dd_com = tk_opn_dev((UB*)"sera", TD_UPDATE);
	if (dd_com < E_OK) {
		tm_printf((UB*)"Open Error %d\n", dd_com);
		tk_ext_tsk();
	}

	while(1) {
		ntshell_execute(&nts);
	}
}
