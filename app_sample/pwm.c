/*
 * pwm.c
 *
 *  Created on: 2025/07/16
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

//-------- PWMのレジスタ定義 -------------------------------
#define PWM_BASE(un)	((un) == 0 ? 0x4001c000 : (un) == 1 ? 0x40021000 : (un) == 2 ? 0x40022000 : 0x4002d000)
#define PWM(un, r)				(PWM_BASE(un) + PWM_##r)
#define PWM_TASKS_STOP			0x004
#define PWM_TASKS_SEQSTART(n)	(0x008 + (n) * 0x04)
#define PWM_LOOPSDONE			0x11C
#define PWM_ENABLE				0x500
#define PWM_MODE				0x504
#define PWM_COUNTERTOP			0x508
#define PWM_PRESCALER			0x50c
#define PWM_DECODER				0x510
#define PWM_LOOP				0x514
#define PWM_SEQ_PTR(n)			(0x520 + (n) * 0x20)
#define PWM_SEQ_CNT(n)			(0x524 + (n) * 0x20)
#define PWM_SEQ_REFRESH(n)		(0x528 + (n) * 0x20)
#define PWM_SEQ_ENDDELAY(n)		(0x52c + (n) * 0x20)
#define PWM_PSEL_OUT(n)			(0x560 + (n) * 0x04)

// カウンタ上限値の定義
#define	TOP				1023

// 比較値COMPを格納する16ビット幅のメモリ領域
static _UH pwm_comp[4];

EXPORT void pwm_init(void) {
	// GPIOピンを出力に設定
	out_w(GPIO(P0, PIN_CNF(13)), 1);	// GPIO P0.13 == P15 左輪 正転PWM
	out_w(GPIO(P0, PIN_CNF(17)), 1);	// GPIO P0.17 == P13 左輪 逆転PWM
	out_w(GPIO(P1, PIN_CNF(2)), 1);		// GPIO P1.02 == P16 右輪 正転PWM
	out_w(GPIO(P0, PIN_CNF(1)), 1);		// GPIO P0.01 == P14 右輪 逆転PWM

	// PWMのチャンネル0をP0.13(== P15) 左輪 正転PWMに出力
	out_w(PWM(0, PSEL_OUT(0)), ((0 << 5) | (13 << 0)));

	// PWMのチャンネル1をP0.17(== P13) 左輪 逆転PWMに出力
	out_w(PWM(1, PSEL_OUT(1)), ((0 << 5) | (17 << 0)));

	// PWMのチャンネル2をP1.02(== P16) 右輪 正転PWMに出力
	out_w(PWM(2, PSEL_OUT(2)), ((1 << 5) | (2 << 0)));

	// PWMのチャンネル3をP0.01(== P14) 右輪 逆転PWMに出力
	out_w(PWM(3, PSEL_OUT(3)), ((0 << 5) | (14 << 0)));
}

// パーセント単位のデューティ比dutyを指定してPWMの動作を開始
EXPORT void pwm_start_duty(INT unit, INT duty)
{
	out_w(PWM(unit, COUNTERTOP), TOP);		// カウンタ上限値の設定

	// パーセント単位のデューティ比dutyからカウンタ比較値を計算して設定
	pwm_comp[unit] = (TOP - (TOP * duty / 100));

	out_w(PWM(unit, ENABLE), (1 << 0));
	out_w(PWM(unit, MODE), 0);
	out_w(PWM(unit, PRESCALER), 4);			// DIV_16でクロック1MHzを指定
	out_w(PWM(unit, LOOP), 0);
	out_w(PWM(unit, DECODER), 0);			// LOAD=commonを指定

	// 比較値を入れるRAMアドレスとサイズの設定
	out_w(PWM(unit, SEQ_PTR(0)), (UW)&pwm_comp[unit]);
	out_w(PWM(unit, SEQ_CNT(0)), 1);
	out_w(PWM(unit, SEQ_REFRESH(0)), 0);
	out_w(PWM(unit, SEQ_ENDDELAY(0)), 0);

	// PWMのシーケンス0の動作を開始
	out_w(PWM(unit, TASKS_SEQSTART(0)), 1);
}

// unitのPWMの動作を停止
EXPORT void pwm_stop(INT unit)
{
	out_w(PWM(unit, TASKS_STOP), 1);
}
