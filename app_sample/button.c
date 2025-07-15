/*
 * button.c
 *
 *  Created on: 2025/07/15
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

EXPORT void button_init(void) {
	// GPIO P0の14ピン(BTN_A)と23ピン(BTN_B)を入力に設定
	out_w(GPIO(P0, PIN_CNF(14)), 0);
	out_w(GPIO(P0, PIN_CNF(23)), 0);

	// GPIO P1の4ピン(LOGO)を入力に設定
	out_w(GPIO(P1, PIN_CNF(4)), 0);
}

/** Button A が押されていれば true */
EXPORT BOOL isButtonAPressed()
{
	UW gpio_p0in;

	// GPIO P0のINレジスタを読んでgpio_p0inに設定
	gpio_p0in = in_w(GPIO(P0, IN));

	// P0.14が0の場合にTRUE
	return ((gpio_p0in & (1 << 14)) == 0);
}

/** Button B が押されていれば true */
EXPORT BOOL isButtonBPressed()
{
	UW gpio_p0in;

	// GPIO P0のINレジスタを読んでgpio_p0inに設定
	gpio_p0in = in_w(GPIO(P0, IN));

	// P0.23が0の場合にTRUE
	return ((gpio_p0in & (1 << 23)) == 0);
}

/** LOGO がタッチされていれば true */
BOOL isLogoTouched()
{
	UW gpio_p1in;

	// GPIO P1のINレジスタを読んでgpio_p1inに設定
	gpio_p1in = in_w(GPIO(P1, IN));

	// P1.04が0の場合にTRUE
	return ((gpio_p1in & (1 << 4)) == 0);
}
