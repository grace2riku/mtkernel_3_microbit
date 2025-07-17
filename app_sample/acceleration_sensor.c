/*
 * acceleration_sensor.c
 *
 *  Created on: 2025/07/17
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "iic.h"			// I2C関連ファイルのインクルード(※A)

//-------- LSM303AGRのI2Cアドレス -------------------------------
#define	IICADR_ACC	0x19	// 加速度センサー
#define	IICADR_MAG	0x1e	// 地磁気センサー

//-------- LSM303AGRの加速度センサーのレジスタアドレス ----------
#define	WHO_AM_I_A	0x0F	// デバイス識別(Who am I)
#define	CTRL_REG1_A	0x20	// 加速度センサー制御_1
#define	OUT_X_L_A	0x28	// 加速度センサー出力_X軸下位バイト
#define	OUT_X_H_A	0x29	// 加速度センサー出力_X軸上位バイト
#define	OUT_Y_L_A	0x2A	// 加速度センサー出力_Y軸下位バイト
#define	OUT_Y_H_A	0x2B	// 加速度センサー出力_Y軸上位バイト
#define	OUT_Z_L_A	0x2C	// 加速度センサー出力_Z軸下位バイト
#define	OUT_Z_H_A	0x2D	// 加速度センサー出力_Z軸上位バイト

//-------- iic_reg.cで定義された関数のインポート ----------------
IMPORT INT read_reg( INT adr, INT reg );
IMPORT ER write_reg( INT adr, INT reg, UB dat );
IMPORT ER read_reg_6( INT adr, INT reg, UB dat[6] );

//---------------------------------------------------------------
LOCAL INT get_acc10bit(UB L_dat, UB H_dat){	// 加速度データの後処理(※G)

	H	dat16bit, ret_dat;					// 符号付き16ビット整数(※K)

	// 上位8ビットと下位8ビットを16ビットデータにまとめる(※H)
	dat16bit = (H_dat << 8) | L_dat ;

	// 左詰めだった10ビットの符号付きデータを右に6ビットシフト(※J)
	ret_dat = dat16bit >> 6;

	return(ret_dat);
}

EXPORT void acceleration_sensor_init(void) {
	iicsetup(TRUE);								// I2Cドライバ起動

	write_reg(IICADR_ACC, CTRL_REG1_A,  0x57);	// ODR=5, LPen=0
}

EXPORT void acceleration_sensor_read(int* x, int* y, int* z) {
	UB	outreg[6];

	// X軸、Y軸、Z軸の上位バイトと下位バイトをまとめて読み出す
	read_reg_6(IICADR_ACC, OUT_X_L_A, outreg);

	// get_acc10bitによる後処理
	*x = get_acc10bit(outreg[0], outreg[1]);
	*y = get_acc10bit(outreg[2], outreg[3]);
	*z = get_acc10bit(outreg[4], outreg[5]);
}
