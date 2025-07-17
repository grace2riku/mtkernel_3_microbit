/*
 * device_id.c
 *
 *  Created on: 2025/07/17
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

//-------- ボードIDの取得 ---------------------------------------
#define	NRF5_FICR_BASE	0x10000000
#define	DEVICEID_0		0x060		// Device identifier 0
#define	DEVICEID_1		0x064		// Device identifier 1

// ボードのデバイスIDを取得してコンソールに表示
EXPORT void get_device_id(W* devid)
{
	devid[0] = in_w(NRF5_FICR_BASE + DEVICEID_0);	// DEVICE[0]の読み出し
	devid[1] = in_w(NRF5_FICR_BASE + DEVICEID_1);	// DEVICE[1]の読み出し
}
