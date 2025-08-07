/*
 * button.c
 *
 *  Created on: 2025/07/15
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "Trace.h"
#include "reinforcement_learning.h"

// GPIOTEのレジスタ定義
#define GPIOTE(r)           (GPIOTE_BASE + GPIOTE_##r)
#define GPIOTE_BASE         0x40006000
#define GPIOTE_EVENTS_IN(n) (0x100 + (n) * 4)
#define GPIOTE_INTENSET     0x304
#define GPIOTE_CONFIG(n)    (0x510 + (n) * 4)

// GPIOTE CONFIG[n]のレジスタの設定値
#define GPIOTE_CONFIG_Event     1
#define GPIOTE_CONFIG_Task      3
#define GPIOTE_CONFIG_LoToHi    1
#define GPIOTE_CONFIG_HiToLo    2
#define GPIOTE_CONFIG_Toggle    3

// GPIOTEの割込み番号と割込み優先度レベル
#define INTNO_GPIOTE            6   // GPIOTEの割込み番号(※G)
#define INTPRI_GPIOTE           6   // GPIOTEの割込み優先度レベル(※J)

// ボタンスイッチA,Bに対応するGPIO P0のピン番号
#define GPIO_P0_SW_A    14
#define GPIO_P0_SW_B    23
#define GPIO_P1_SW_LOGO 4

// オブジェクトID番号
LOCAL ID button_flgid;           // 割込み通知用イベントフラグのID

// タスクとイベントフラグの生成情報
LOCAL const T_CFLG cflg = {0, (TA_TFIFO | TA_WSGL), 0};

// 割込みハンドラ定義情報
LOCAL void btn_inthdr(UINT intno);
LOCAL const T_DINT dint = {TA_HLNG, btn_inthdr};

EXPORT void button_init(void) {
	// GPIO P0の14ピン(BTN_A)と23ピン(BTN_B)を入力に設定
	out_w(GPIO(P0, PIN_CNF(GPIO_P0_SW_A)), 0);
	out_w(GPIO(P0, PIN_CNF(GPIO_P0_SW_B)), 0);

	// GPIO P1の4ピン(LOGO)を入力に設定
	out_w(GPIO(P1, PIN_CNF(GPIO_P1_SW_LOGO)), 0);

	// 以下、ボタンスイッチの押下でGPIOTEのイベントを発生するための設定
	// ボタンスイッチAでチャンネル0のイベントを発生
	out_w(GPIOTE(CONFIG(0)),                // チャンネル0を指定
		(GPIOTE_CONFIG_Event << 0)        // MODEにEventを指定
		| (GPIO_P0_SW_A << 8)             // PSELにSW_AのGPIOピン番号14を指定
		| (0 << 13)                       // PORTにSW_AのGPIOポート番号0を指定
		| (GPIOTE_CONFIG_HiToLo << 16));  // POLARITYにHiToLo(立ち下がり)を指定

	// ボタンスイッチBでチャンネル1のイベントを発生
	out_w(GPIOTE(CONFIG(1)),                // チャンネル1を指定
		(GPIOTE_CONFIG_Event << 0)        // MODEにEventを指定
		| (GPIO_P0_SW_B << 8)             // PSELにSW_BのGPIOピン番号23を指定
		| (0 << 13)                       // PORTにSW_BのGPIOポート番号0を指定
		| (GPIOTE_CONFIG_HiToLo << 16));  // POLARITYにHiToLo(立ち下がり)を指定

	// GPIOTEのチャンネル0とチャンネル1の割込みを許可
	out_w(GPIOTE(INTENSET), (1 << 0) | (1 << 1));

	button_flgid = tk_cre_flg(&cflg);              // 割込み通知用のイベントフラグ作成

	tk_def_int(INTNO_GPIOTE, &dint);        // 割込みハンドラ定義
	EnableInt(INTNO_GPIOTE, INTPRI_GPIOTE); // 割込み許可
}

EXPORT ID button_get_flgid(void) {
	return button_flgid;
}

// GPIOTEの割込みハンドラ
LOCAL void btn_inthdr(UINT intno)
{
	INT n;                      // GPIOTEのチャンネル番号(0または1)
	for (n = 0; n < 2; n++) {   // GPIOTEのチャンネル0とチャンネル1を順に処理

		// EVENTS_IN[n]の最下位ビットを見てイベント発生状態を確認
		if (in_w(GPIOTE(EVENTS_IN(n))) & (1 << 0)) {
			// イベントがあった場合はイベント発生状態をクリア
			out_w(GPIOTE(EVENTS_IN(n)), 0);

			// 割込み通知用イベントフラグの対応ビットをセット
			tk_set_flg(button_flgid, (1 << n));

			if (n == 0) {
				// Aボタン押下
				// tr_runnable();
			}
			if (n == 1) {
				// Bボタン押下
				//tr_stop();
				tk_set_flg(rl_get_flgid(), RL_TRAIN_RESUME);
			}
		}
	}
}

/** Button A が押されていれば true */
EXPORT BOOL isButtonAPressed()
{
	UW gpio_p0in;

	// GPIO P0のINレジスタを読んでgpio_p0inに設定
	gpio_p0in = in_w(GPIO(P0, IN));

	// P0.14が0の場合にTRUE
	return ((gpio_p0in & (1 << GPIO_P0_SW_A)) == 0);
}

/** Button B が押されていれば true */
EXPORT BOOL isButtonBPressed()
{
	UW gpio_p0in;

	// GPIO P0のINレジスタを読んでgpio_p0inに設定
	gpio_p0in = in_w(GPIO(P0, IN));

	// P0.23が0の場合にTRUE
	return ((gpio_p0in & (1 << GPIO_P0_SW_B)) == 0);
}

/** LOGO がタッチされていれば true */
BOOL isLogoTouched()
{
	UW gpio_p1in;

	// GPIO P1のINレジスタを読んでgpio_p1inに設定
	gpio_p1in = in_w(GPIO(P1, IN));

	// P1.04が0の場合にTRUE
	return ((gpio_p1in & (1 << GPIO_P1_SW_LOGO)) == 0);
}
