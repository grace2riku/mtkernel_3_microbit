/*
 * time.c
 *
 *  Created on: 2025/09/06
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

// ミリ秒単位の現在時刻の下位32ビットを返す
W cur_time(void)
{
	SYSTIM	tim;
	tk_get_tim(&tim); // 現在時刻を取得
	return(tim.lo);
}
