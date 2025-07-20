/*
 * led.c
 *
 *  Created on: 2025/07/20
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "gpio.h"

typedef struct {
    UB port;   /* 0 or 1 */
    UB pin;    /* 0–31 */
} gpio_pin_t;

/* 行 (ROW1–5) */
static const gpio_pin_t row_table[5] = {
    {0,21}, {0,22}, {0,15}, {0,24}, {0,19}
};

/* 列 (COL1–5) */
static const gpio_pin_t col_table[5] = {
    {0,28}, {0,11}, {0,31}, {1, 5}, {0,30}
};

static inline BOOL get_row_pin(UB row, gpio_pin_t *dest) {
    if (row < 1 || row > 5 || !dest) return FALSE;
    *dest = row_table[row - 1];
    return TRUE;
}

static inline BOOL get_col_pin(UB col, gpio_pin_t *dest) {
    if (col < 1 || col > 5 || !dest) return FALSE;
    *dest = col_table[col - 1];
    return TRUE;
}

static inline BOOL get_led_pins(UB row, UB col, gpio_pin_t *r, gpio_pin_t *c) {
    return get_row_pin(row, r) && get_col_pin(col, c);
}

// LED制御のためのGPIOの初期設定
EXPORT void led_init(void)
{
	UB col;
	gpio_pin_t col_gpio;

	// COL1..COL5とROW1..ROW5に接続されたGPIOピンを出力に設定
	out_w(GPIO(P0, PIN_CNF(28)), 1);	// GPIO P0.28(COL1に接続)
	out_w(GPIO(P0, PIN_CNF(11)), 1);	// GPIO P0.11(COL2に接続)
	out_w(GPIO(P0, PIN_CNF(31)), 1);	// GPIO P0.31(COL3に接続)
	out_w(GPIO(P1, PIN_CNF(05)), 1);	// GPIO P1.05(COL4に接続)
	out_w(GPIO(P0, PIN_CNF(30)), 1);	// GPIO P0.30(COL5に接続)

	out_w(GPIO(P0, PIN_CNF(21)), 1);	// GPIO P0.21(ROW1に接続)
	out_w(GPIO(P0, PIN_CNF(22)), 1);	// GPIO P0.22(ROW2に接続)
	out_w(GPIO(P0, PIN_CNF(15)), 1);	// GPIO P0.15(ROW3に接続)
	out_w(GPIO(P0, PIN_CNF(24)), 1);	// GPIO P0.24(ROW4に接続)
	out_w(GPIO(P0, PIN_CNF(19)), 1);	// GPIO P0.19(ROW5に接続)

	for (col = 1; col <= 5;col++) {
		get_col_pin(col, &col_gpio);
		out_gpio_pin(col_gpio.port, col_gpio.pin, 1); // COL1-5に1を出力
	}
}

/**
 * @brief  5×5 LED の 1 ドットを点灯 / 消灯
 * @param  row    1〜5
 * @param  col    1〜5
 * @param  state  1: 点灯, 0: 消灯
 * @return true   正常に設定
 * @note   LED マトリックスは「行 High / 列 Low」で点灯する。
 *         ここではシンプルに “行と列を両方 Low” で消灯している。
 *         本格的なスキャン駆動を行う場合は行ごとの多重化を推奨。
 */
EXPORT BOOL led_set(UB row, UB col, UB state)
{
    gpio_pin_t row_gpio, col_gpio;
    if (!get_led_pins(row, col, &row_gpio, &col_gpio)) return FALSE;  /* 範囲チェック */

    /* 点灯制御 */
    if (state) {          /* 点灯: ROW High, COL Low */
        out_gpio_pin(row_gpio.port, row_gpio.pin, 1);
    	out_gpio_pin(col_gpio.port, col_gpio.pin, 0);
    } else {              /* 消灯: ROW Low, COL High */
        out_gpio_pin(row_gpio.port, row_gpio.pin, 0);
    	out_gpio_pin(col_gpio.port, col_gpio.pin, 1);
    }

    return TRUE;
}
