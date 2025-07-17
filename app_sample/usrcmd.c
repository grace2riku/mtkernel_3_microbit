/**
 * @file usrcmd.c
 * @author CuBeatSystems
 * @author Shinichiro Nakamura
 * @copyright
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) Version 0.3.1
 * ===============================================================
 * Copyright (c) 2010-2016 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ntopt.h"
#include "ntlibc.h"

#include "ntshell.h"
#include "usrcmd.h"

/*
MEMO: size_t多重定義対応
・現象: xprintf.hでincludeしているstring.hとinclude/tk/syslib.hでsize_tが多重定義されコンパイルエラーとなる。
・対応内容: include/tk/syslib.hより先にxprintf.hをincludeする且つPROHIBIT_DEF_SIZE_Tを定義することで
　include/tk/syslib.hでsize_tが定義されない。
　結果、カーネル側のinclude/tk/syslib.hの変更が不要になる。
*/
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"	// xatoi

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#define uart_puts tm_printf

//#include "xprintf.h"	// xatoi

#include "ad.h"
#include "button.h"
#include "motor.h"
#include "device_id.h"
#include "acceleration_sensor.h"
#include "temperature_sensor.h"

typedef int (*USRCMDFUNC)(int argc, char **argv);

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj);
static int usrcmd_help(int argc, char **argv);
static int usrcmd_info(int argc, char **argv);
static int usrcmd_getLineSensorValue(int argc, char **argv);
static int usrcmd_getButton(int argc, char **argv);
static int usrcmd_driveMotor(int argc, char **argv);
static int usrcmd_get_device_id(int argc, char **argv);
static int usrcmd_read_acceleration(int argc, char **argv);
static int usrcmd_read_temp(int argc, char **argv);

typedef struct {
    const char* cmd;
    const char* desc;
    USRCMDFUNC func;
} cmd_table_t;

static const cmd_table_t cmdlist[] = {
    { "help", "This is a description text string for help command.", usrcmd_help },
    { "info", "This is a description text string for info command.", usrcmd_info },
    { "getls", "This command acquires the values of the left(P0), center(P1), and right(P2) line sensors.", usrcmd_getLineSensorValue },
    { "getbtn", "This command is used to obtain the press status of a button(A,B and LOGO).", usrcmd_getButton },
    { "drivemotor", "This command drives the motor.", usrcmd_driveMotor },
    { "getdevid", "This command is used to obtain the device ID.", usrcmd_get_device_id },
    { "readacc", "This command reads the accelerometer.", usrcmd_read_acceleration },
    { "readtemp", "This command reads the temperature.", usrcmd_read_temp },
};

enum {
  COMMAND_HELP,
  COMMAND_INFO,
  COMMAND_GETLINESENSORVALUE,
  COMMAND_GETBUTTON,
  COMMAND_DRIVEMOTOR,
  COMMAND_GETDEVID,
  COMMAND_MAX
};


int usrcmd_execute(const char *text)
{
    return ntopt_parse(text, usrcmd_ntopt_callback, 0);
}

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj)
{
    if (argc == 0) {
        return 0;
    }
    const cmd_table_t *p = &cmdlist[0];
    for (unsigned int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        if (ntlibc_strcmp((const char *)argv[0], p->cmd) == 0) {
            return p->func(argc, argv);
        }
        p++;
    }
    uart_puts("Unknown command found.\r\n");
    return 0;
}

static int usrcmd_help(int argc, char **argv)
{
    const cmd_table_t *p = &cmdlist[0];
    for (unsigned int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        uart_puts(p->cmd);
        uart_puts("\t:");
        uart_puts(p->desc);
        uart_puts("\r\n");
        p++;
    }
    return 0;
}

static int usrcmd_info(int argc, char **argv)
{
    if (argc != 2) {
        uart_puts("info sys\r\n");
        uart_puts("info ver\r\n");
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "sys") == 0) {
        uart_puts("mtk3 micro:bit\r\n");
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "ver") == 0) {
        uart_puts("Version 0.0\r\n");
        return 0;
    }
    uart_puts("Unknown sub command found\r\n");
    return -1;
}


static int usrcmd_getLineSensorValue(int argc, char **argv) {
	_H adc_data[3];
    uart_puts("left(P0), center(P1), right(P2)\r\n");

    // ch0=P0.02/AIN0(RING0==P0), ch1=P0.03/AIN1(RING1==P1), ch2=P0.04/AIN2(RING2==P2)でA/D変換
	analogRead3(adc_data);

	// A/D変換結果をコンソールに出力
	tm_printf("%d, %d, %d\n", adc_data[0], adc_data[1], adc_data[2]);

	return 0;
}


static int usrcmd_getButton(int argc, char **argv) {
	tm_printf("Button_A: %s, Button_B: %s, Button_LOGO: %s\n\n",
			(isButtonAPressed() ? "on ": "off"), (isButtonBPressed() ? "on ": "off"), (isLogoTouched() ? "Touched ": "Not touched"));

	return 0;
}


static int usrcmd_driveMotor(int argc, char **argv) {
	int duty[2];
	int drive_time_ms;

    if (argc != 4) {
        uart_puts("ex) drivemotor 50 50 1000\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&duty[0])) {
        uart_puts("Left motor duty error.\r\n");
		return -21;
	}
	if (!(-100 <= duty[0] && duty[0] <= 100)) {
        uart_puts("Left motor duty parameter error.\r\n");
		return -22;
	}
	if (!xatoi(&argv[2], (long*)&duty[1])) {
        uart_puts("Right motor duty error.\r\n");
		return -31;
	}
	if (!(-100 <= duty[1] && duty[1] <= 100)) {
        uart_puts("Right motor duty parameter error.\r\n");
		return -32;
	}
	if (!xatoi(&argv[3], (long*)&drive_time_ms)) {
        uart_puts("Motor drive time error.\r\n");
		return -41;
	}
	if (drive_time_ms < 0) {
        uart_puts("Motor drive time parameter error.\r\n");
		return -42;
	}

	tm_printf("left duty = %d (%%), right duty = %d (%%), drive time = %d (ms)\n\n",
			duty[0], duty[1], drive_time_ms);

	motor_drive(duty[0], duty[1]);
	tk_dly_tsk(drive_time_ms);
	motor_stop();

	return 0;
}

static int usrcmd_get_device_id(int argc, char **argv) {
	W devid[2];

	get_device_id(devid);

	tm_printf("This devid_0_1: %08x_%08x\n", devid[0], devid[1]);

	return 0;
}

static int usrcmd_read_acceleration(int argc, char **argv){
	int x, y, z;

	acceleration_sensor_read(&x, &y, &z);

	tm_printf("Acc: x,y,z=%4d,%4d,%4d\n", x, y, z);

	return 0;
}

static int usrcmd_read_temp(int argc, char **argv) {
	float temp = read_temperature();

	xprintf("temp = %f C\n", temp);

	return 0;
}
