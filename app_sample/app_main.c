#if 0
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
LOCAL void task_com(INT stacd, void* exinf);	// 実行関数

LOCAL ID tskid_com;
LOCAL T_CTSK ctsk_com = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_com,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_com(INT stacd, void* exinf) {
	ID dd_com;
	UB data_com;
	SZ asz_com;
	ER err;

	dd_com = tk_opn_dev((UB*)"sera", TD_UPDATE);
	if (dd_com < E_OK) {
		tm_printf((UB*)"Open Error %d\n", dd_com);
		tk_ext_tsk();
	}

	while (1) {
		err = tk_srea_dev(dd_com, 0, &data_com, 1, &asz_com);
		if (err >= E_OK) {
			err = tk_swri_dev(dd_com, 0, &data_com, 1, &asz_com);
		} else {
			tm_printf((UB*)"READ Error %d\n", err);
		}
	}
}

/* usermain関数 */
EXPORT INT usermain(void)
{
	tskid_com = tk_cre_tsk(&ctsk_com);
	tk_sta_tsk(tskid_com, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}

#else
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/* usermain関数 */
EXPORT INT usermain(void)
{
	ID ntshell_tskid; // Task ID number

//	tm_putstring((UB*)"Start User-main program.\n");

	IMPORT T_CTSK* get_ntshell_task_ctsk_addr();
	ntshell_tskid = tk_cre_tsk(get_ntshell_task_ctsk_addr());
	tk_sta_tsk(ntshell_tskid, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
#endif
