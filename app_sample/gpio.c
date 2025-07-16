/*
 * gpio.c
 *
 *  Created on: 2025/07/16
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

// GPIOの特定のピンpinの出力をval(0または1)に設定
// pinで指定された以外のピンの状態は変化しない
EXPORT void out_gpio_pin(UW port, UW pin, UW val)
{
	INT port_addr;

	if(port == 1){							// P1の場合
		if(val)
			port_addr = GPIO(P1, OUTSET);	// P1でvalが1の場合
		else
			port_addr = GPIO(P1, OUTCLR);	// P1でvalが0の場合

	} else {								// P0の場合
		if(val)
			port_addr = GPIO(P0, OUTSET);	// P0でvalが1の場合
		else
			port_addr = GPIO(P0, OUTCLR);	// P0でvalが0の場合
	}
	out_w(port_addr, (1 << pin));			// 指定のピンに1を出力
}
