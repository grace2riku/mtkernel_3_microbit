/*
 * speaker.c
 *
 *  Created on: 2025/07/26
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "gpio.h"

LOCAL void speaker_onoff(void);

// 物理タイマ関連情報
static const T_DPTMR dptmr = {0, TA_HLNG, &speaker_onoff};
static const UINT ptmrno = 1;				// 物理タイマ番号として1を使用
static const INT ptmr_clk_mhz = 16;		// 物理タイマのクロック(MHz単位)

void speaker_init(void) {
	// スピーカーに接続されたGPIOピン(P0.00)を出力に設定
	out_w(GPIO(P0, PIN_CNF(0)), (1 << 0));

	DefinePhysicalTimerHandler(ptmrno, &dptmr);		// 物理タイマハンドラ定義
}

//-------- スピーカーへの出力 -----------------------------------
LOCAL BOOL	speaker_val = FALSE;		// 現在のスピーカーへの出力値

// スピーカーへの出力値の0と1を反転する関数
LOCAL void speaker_onoff(void)
{
	// スピーカーに接続されたGPIOピン(P0.00)にspeaker_valの値を出力
	out_gpio_pin(0, 0, speaker_val);	// port=0, pin=0を指定

	// 次の出力のために値を反転
	speaker_val = (! speaker_val);
}


// 周波数freq(Hz)の音をplay_time(ミリ秒)の間だけスピーカーから再生
void play_speaker(INT freq, INT play_time)
{
	// 物理タイマハンドラの起動周期時間(マイクロ秒単位)と上限値の計算
	INT cycle_us = 1000000 / freq / 2;				// 起動周期時間
	INT limit = cycle_us * ptmr_clk_mhz - 1;		// 物理タイマの上限値

	StartPhysicalTimer(ptmrno, limit, TA_CYC_PTMR);	// 物理タイマの動作開始
	tk_dly_tsk(play_time);							// 再生時間だけ待つ
	StopPhysicalTimer(ptmrno);
}
