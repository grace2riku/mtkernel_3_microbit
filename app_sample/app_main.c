#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "button.h"
#include "pwm.h"

/* usermain関数 */
EXPORT INT usermain(void)
{
	ID ntshell_tskid; // Task ID number

	tm_putstring((UB*)"Start User-main program.\n");

	button_init();
	pwm_init();

	IMPORT T_CTSK* get_ntshell_task_ctsk_addr();
	ntshell_tskid = tk_cre_tsk(get_ntshell_task_ctsk_addr());
	tk_sta_tsk(ntshell_tskid, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
