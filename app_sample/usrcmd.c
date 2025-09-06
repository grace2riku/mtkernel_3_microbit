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

#include "ad.h"
#include "button.h"
#include "motor_drv.h"
#include "device_id.h"
#include "acceleration_sensor.h"
#include "temperature_sensor.h"
#include "led.h"
#include "Motor.h"
#include "Trace.h"
#include "speaker.h"
#include "time.h"

#include "Course.h"
#include "Drive.h"
#include "Navi.h"

#include "log.h"
#include "reinforcement_learning.h"

typedef int (*USRCMDFUNC)(int argc, char **argv);

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj);
static int usrcmd_help(int argc, char **argv);
static int usrcmd_info(int argc, char **argv);
static int usrcmd_getLineSensorValue(int argc, char **argv);
static int usrcmd_stream_line_sensor_data(int argc, char **argv);
static int usrcmd_getButton(int argc, char **argv);
static int usrcmd_driveMotor(int argc, char **argv);
static int usrcmd_get_device_id(int argc, char **argv);
static int usrcmd_read_acceleration(int argc, char **argv);
static int usrcmd_read_acceleration_g(int argc, char **argv);
static int usrcmd_read_acceleration_g_loop(int argc, char **argv);
static int usrcmd_read_temp(int argc, char **argv);
static int usrcmd_chk_btn_interrupt(int argc, char **argv);
static int usrcmd_led_set(int argc, char **argv);
static int usrcmd_get_duty(int argc, char **argv);
static int usrcmd_set_duty(int argc, char **argv);
static int usrcmd_memory_read(int argc, char **argv);
static int usrcmd_tr_run(int argc, char **argv);
static int usrcmd_tr_stop(int argc, char **argv);
static int usrcmd_play_speaker(int argc, char **argv);
static int usrcmd_set_frdir(int argc, char **argv);
static int usrcmd_get_frdir(int argc, char **argv);
static int usrcmd_log_on(int argc, char **argv);
static int usrcmd_log_off(int argc, char **argv);
static int usrcmd_set_acc_read_time(int argc, char **argv);
static int usrcmd_get_acc_read_time(int argc, char **argv);
static int usrcmd_rl_get_state(int argc, char **argv);
static int usrcmd_rl_get_reward(int argc, char **argv);
static int usrcmd_rl_move(int argc, char **argv);
static int usrcmd_rl_get_max_Qval(int argc, char **argv);
static int usrcmd_rl_select_action(int argc, char **argv);
static int usrcmd_rl_epsilon_greedy(int argc, char **argv);
static int usrcmd_rl_learning(int argc, char **argv);
static int usrcmd_rl_action(int argc, char **argv);
static int usrcmd_rl_output_Qtable(int argc, char **argv);
static int usrcmd_rl_set_example_Qtable(int argc, char **argv);

static char mr_cmd_example[] = "mr <[b|h|w]> <addr> [count]\n"
"1byte * 16count read example) >mr b 0x1801e35d 16\n"
"1801E35D  54 4F 50 50 45 52 53 2F 41 53 50 20 4B 65 72 6E  TOPPERS/ASP Kern\n"
"\n"
"2byte * 8count read example) >mr h 0x1801e35d 8\n"
"1801E35D  4F54 5050 5245 2F53 5341 2050 654B 6E72\n"
"\n"
"4byte * 4count read example) >mr w 0x1801e35d 4\n"
"1801E35D  50504F54 2F535245 20505341 6E72654B\n"
"\n"
"1byte read example) >mr b 0x1801e35d\n"
"1801E35D  54  T\n"
"\n"
"2byte read example) >mr h 0x1801e35d\n"
"1801E35D  4F54\n"
"\n"
"4byte read example) >mr w 0x1801e35d\n"
"1801E35D  50504F54\n";

static const char* const rllineStateMsg[] = {
    "State 0: The line is in the center.\n",
    "State 1: The line is on the right.\n",
    "State 2: The line is on the left.\n",
    "State 3: Anything else.\n"
};

typedef struct {
    const char* cmd;
    const char* desc;
    USRCMDFUNC func;
} cmd_table_t;

static const cmd_table_t cmdlist[] = {
    { "help", "This is a description text string for help command.", usrcmd_help },
    { "info", "This is a description text string for info command.", usrcmd_info },
    { "getls", "This command acquires the values of the left(P0), center(P1), and right(P2) line sensors.", usrcmd_getLineSensorValue },
    { "getlsloop", "This command continues to acquire the values of the left (P0), center (P1), and right (P2) line sensors until button B is pressed.", usrcmd_stream_line_sensor_data },
    { "getbtn", "This command is used to obtain the press status of a button(A,B and LOGO).", usrcmd_getButton },
    { "drivemotor", "This command drives the motor.", usrcmd_driveMotor },
    { "getdevid", "This command is used to obtain the device ID.", usrcmd_get_device_id },
    { "readacc", "This command reads the accelerometer(LSB).", usrcmd_read_acceleration },
    { "readaccg", "This command reads the accelerometer(g).", usrcmd_read_acceleration_g },
    { "readaccglp", "This command reads the accelerometer(g).", usrcmd_read_acceleration_g_loop },
    { "readtemp", "This command reads the temperature.", usrcmd_read_temp },
    { "chkbtnint", "This command checks for button interruptions.", usrcmd_chk_btn_interrupt },
    { "ledset", "This command turns the LEDs on and off.", usrcmd_led_set },
    { "getduty", "This command is used to get Duty.", usrcmd_get_duty },
    { "setduty", "This command is used to set Duty.", usrcmd_set_duty },
    { "mr", mr_cmd_example, usrcmd_memory_read },
    { "trrun", "Line tracing.", usrcmd_tr_run },
    { "trstop", "Stop line tracing.", usrcmd_tr_stop },
    { "speaker", "The specified frequency is sounded from the speaker for the specified time.", usrcmd_play_speaker },
    { "setfrdir", "This command sets the forward/backward direction of movement.", usrcmd_set_frdir },
    { "getfrdir", "This command gets the forward/backward direction of movement.", usrcmd_get_frdir },
    { "logon", "This command turns on logging.", usrcmd_log_on },
    { "logoff", "This command turns off logging.", usrcmd_log_off },
    { "setaccreadtime", "Sets the ACC sensor read timing.", usrcmd_set_acc_read_time },
    { "getaccreadtime", "Gets the ACC sensor read timing.", usrcmd_get_acc_read_time },
    { "rlgetstate", "Let's check the line sensor state acquisition function used in reinforcement learning.", usrcmd_rl_get_state },
    { "rlgetrwd", "This command gets the reinforcement learning reward.", usrcmd_rl_get_reward },
    { "rlmove", "This command checks the reinforcement learning action.", usrcmd_rl_move },
    { "rlgetmaxq", "This command gets the maximum Q value.", usrcmd_rl_get_max_Qval },
    { "rlselact", "This command tests the function that selects the action with the maximum Q value in reinforcement learning.", usrcmd_rl_select_action },
    { "rlegreedy", "This command executes probabilistic action selection using the e-greedy method.", usrcmd_rl_epsilon_greedy },
    { "rl", "This is the command to run reinforcement learning.", usrcmd_rl_learning },
    { "rlact", "This is a command to execute the policy obtained through reinforcement learning.", usrcmd_rl_action },
    { "rloutqtbl", "This command outputs the value of a Qtable.", usrcmd_rl_output_Qtable },
    { "rlsetexqtbl", "This command sets the value of the Qtable in the example settings stored within the program.", usrcmd_rl_set_example_Qtable },
};

enum {
  COMMAND_HELP,
  COMMAND_INFO,
  COMMAND_GETLINESENSORVALUE,
  COMMAND_GETLINESENSORVALUELOOP,
  COMMAND_GETBUTTON,
  COMMAND_DRIVEMOTOR,
  COMMAND_GETDEVID,
  COMMAND_READACC,
  COMMAND_READACC_G,
  COMMAND_READACC_G_LOOP,
  COMMAND_REATEMP,
  COMMAND_CHKBTNINT,
  COMMAND_LEDSET,
  COMMAND_GETDUTY,
  COMMAND_SETDUTY,
  COMMAND_MEMORYREAD,
  COMMAND_LINETRACERUN,
  COMMAND_LINETRACESTOP,
  COMMAND_PLAYSPEAKER,
  COMMAND_SETFRDIR,
  COMMAND_GETFRDIR,
  COMMAND_LOGON,
  COMMAND_LOGOFF,
  COMMAND_SETACCREADTIME,
  COMMAND_GETACCREADTIME,
  COMMAND_RLGETSTATE,
  COMMAND_RLGETREWARD,
  COMMAND_RLMOVE,
  COMMAND_RLGETMAXQ,
  COMMAND_RLSELCTACTION,
  COMMAND_RLEGREEDY,
  COMMAND_RL,
  COMMAND_RLACTION,
  COMMAND_RLOUTQTABLE,
  COMMAND_RLSETQTABLE,
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

static int usrcmd_stream_line_sensor_data(int argc, char **argv) {
	_H adc_data[3];
	UW interval_time = 1000;

    if (argc == 2) {
    	if (!xatoi(&argv[1], (long*)&interval_time)) {
            uart_puts("Interval time parameter error.\r\n");
    		return -21;
    	}
    }

    uart_puts("left(P0), center(P1), right(P2)\r\n");

    while (!isButtonBPressed()) {
        // ch0=P0.02/AIN0(RING0==P0), ch1=P0.03/AIN1(RING1==P1), ch2=P0.04/AIN2(RING2==P2)でA/D変換
    	analogRead3(adc_data);

    	// A/D変換結果をコンソールに出力
    	tm_printf("%d, %d, %d\n", adc_data[0], adc_data[1], adc_data[2]);

    	tk_dly_tsk(interval_time);
    }

    uart_puts("Button-B pressed.Exit usrcmd_stream_line_sensor_data().\r\n");

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

static int usrcmd_read_acceleration_g(int argc, char **argv){
	int x, y, z;
	float x_g, y_g, z_g;

	acceleration_sensor_read(&x, &y, &z);
	x_g = (float)(x * 4) / 1000;
	y_g = (float)(y * 4) / 1000;
	z_g = (float)(z * 4) / 1000;

	xprintf("%f,%f,%f\n", x_g, y_g, z_g);

	return 0;
}


static int usrcmd_read_acceleration_g_loop(int argc, char **argv){
	int x, y, z;
	float x_g, y_g, z_g;
	int time_ms = 10000;
	int read_interval_time_ms = 20;
	W cmd_start_time = cur_time(); // コマンド開始時刻を取得する

	while ((cur_time() - cmd_start_time) < time_ms) {
		acceleration_sensor_read(&x, &y, &z);
		x_g = (float)(x * 4) / 1000;
		y_g = (float)(y * 4) / 1000;
		z_g = (float)(z * 4) / 1000;

		xprintf("%f,%f,%f\n", x_g, y_g, z_g);

		tk_dly_tsk(read_interval_time_ms);
	}

	return 0;
}

static int usrcmd_read_temp(int argc, char **argv) {
	float temp = read_temperature();

	xprintf("temp = %f C\n", temp);

	return 0;
}

static int usrcmd_chk_btn_interrupt(int argc, char **argv) {
	UINT flgptn;
	UINT waitflgptn = 0;

	tm_printf("Wait until buttons A and B are pressed.\n");

	// AボタンとBボタンが押されるまでループ
	while(waitflgptn != 3){
		// 割込み通知用イベントフラグの下位2ビットをOR待ち
		tk_wai_flg(button_get_flgid(), 0b11, (TWF_ORW | TWF_CLR), &flgptn, TMO_FEVR);

		// 最下位ビットが1の場合
		if((flgptn & (1 << 0)) != 0) {
			tm_printf("Button_SW_A: pushed\n"); // ボタンスイッチAの押下を表示
			waitflgptn |= 1;
		}

		if((flgptn & (1 << 1)) != 0) {
			// 最下位から2番目のビットが1の場合
			tm_printf("Button_SW_B: pushed\n"); // ボタンスイッチBの押下を表示
			waitflgptn |= 2;
		}
	}

	tm_printf("Button_SW A & B: pushed\n");

	return 0;
}

static int usrcmd_led_set(int argc, char **argv){
	UW row, col, led_level;

    if (argc != 4) {
        uart_puts("ex) ledset 1 1 1.\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&col)) {
        uart_puts("COL led number error.\r\n");
		return -21;
	}
	if (!(1 <= col && col <= 5)) {
        uart_puts("COL led number parameter error.\r\n");
		return -22;
	}
	if (!xatoi(&argv[2], (long*)&row)) {
        uart_puts("ROW led number error.\r\n");
		return -31;
	}
	if (!(1 <= row && row <= 5)) {
        uart_puts("ROW led number parameter error.\r\n");
		return -32;
	}
	if (!xatoi(&argv[3], (long*)&led_level)) {
        uart_puts("LED level parameter error.\r\n");
		return -41;
	}

	if (!led_set((UB)row, (UB)col, (UB)led_level)) {
        uart_puts("led_set() error.\r\n");
		return -51;
	}

	return 0;
}

static int usrcmd_get_duty(int argc, char **argv) {
	tm_printf("duty = %d\n", get_duty());

	return 0;
}

static int usrcmd_set_duty(int argc, char **argv) {
	int duty;

	if (argc != 2) {
        uart_puts("ex) setduty 50\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&duty)) {
        uart_puts("duty error.\r\n");
		return -11;
	}
	if (!(0 <= duty && duty <= 100)) {
        uart_puts("Duty parameter error.\r\n");
		return -12;
	}

	set_duty(duty);

    return 0;
}

static int usrcmd_memory_read(int argc, char **argv){
	long address;
	long count = 1;
	int read_size;
	char *ptr;

	if (argc < 3) {
		uart_puts("mrb <[b|h|w]> <addr> [count]");
    	return 0;
    }

    if (ntlibc_strcmp(argv[1], "b") == 0) {
		read_size = 1;
    }
    else if (ntlibc_strcmp(argv[1], "h") == 0) {
		read_size = 2;
    }
    else if (ntlibc_strcmp(argv[1], "w") == 0) {
		read_size = 4;
    } else {
    	uart_puts("memory read size is [b|h|w].");
    	return 0;
    }

	if (!xatoi(&argv[2], &address)) {
		uart_puts("address error");
		return 0;
	}

	if (argc == 4) {
		if (!xatoi(&argv[3], &count)) {
			uart_puts("count error");
			return 0;
		}
    }

	for (ptr = (char*)address; count >= 16 / read_size; ptr += 16, count -= 16 / read_size)
		put_dump(ptr, (UW)ptr, 16 / read_size, read_size);
	if (count) put_dump((char*)ptr, (UINT)ptr, count, read_size);

	return 0;
}

static int usrcmd_tr_run(int argc, char **argv) {
	tr_runnable();
	return 0;
}

static int usrcmd_tr_stop(int argc, char **argv) {
	tr_stop();
	return 0;
}

static int usrcmd_play_speaker(int argc, char **argv) {
	INT f, play_time;

	if (argc != 3) {
        uart_puts("ex) speaker 523 500\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&f)) {
        uart_puts("frequency error.\r\n");
		return -11;
	}

	if (!xatoi(&argv[2], (long*)&play_time)) {
        uart_puts("Play time error.\r\n");
		return -21;
	}

	play_speaker(f, play_time);

	return 0;
}

static int usrcmd_set_frdir(int argc, char **argv) {
	int dir = 0;

	if (argc != 2) {
        uart_puts("ex) setfrdir 0\r\n");
        uart_puts("ex) setfrdir 1\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&dir)) {
        uart_puts("dir error.\r\n");
		return -11;
	}

	set_movefrontback_dir(dir);

	return 0;
}

static int usrcmd_get_frdir(int argc, char **argv) {
	int dir = get_movefrontback_dir();

	if (!dir) {
		tm_printf("Movement is forward direction.\n");
	} else {
		tm_printf("Movement is backward direction.\n");
	}

	return 0;
}

static int usrcmd_log_on(int argc, char **argv) {
	set_eneble_log(1);

	return 0;
}

static int usrcmd_log_off(int argc, char **argv) {
	set_eneble_log(0);

	return 0;
}

static int usrcmd_set_acc_read_time(int argc, char **argv) {
	UH timing;

	if (argc != 2) {
        uart_puts("ex 1: 20ms(10ms * 2) setaccreadtime 2\r\n");
        uart_puts("ex 2: 100ms(10ms * 10) setaccreadtime 10\r\n");
        return -1;
    }

	if (!xatoi(&argv[1], (long*)&timing)) {
        uart_puts("Timing error.\r\n");
		return -11;
	}

	set_acc_sensor_read_timing(timing);

	return 0;
}

static int usrcmd_get_acc_read_time(int argc, char **argv) {
    tm_printf("acc_read_time = %d\n\n", get_acc_sensor_read_timing());

    return 0;
}

static int usrcmd_rl_get_state(int argc, char **argv) {
	int state = rl_get_state();

	tm_printf(rllineStateMsg[state]);

	return 0;
}

static int usrcmd_rl_get_reward(int argc, char **argv) {
	int reward = rl_get_reward();

	tm_printf("The reinforcement learning reward is %d.\n", reward);

	return 0;
}


static int usrcmd_rl_move(int argc, char **argv) {
	rl_move(0);	// 前進
	tk_dly_tsk(1000);

	rl_move(1);	// 右旋回
	tk_dly_tsk(1000);

	rl_move(2);	// 左旋回
	tk_dly_tsk(1000);

	motor_stop();

	return 0;
}


static int usrcmd_rl_get_max_Qval(int argc, char **argv) {
	int state = 0;
	int num_actions = 5;
	float max = 0.0;

	rl_init_Qtable();
	max = rl_max_Qval(state, num_actions, rl_get_Qtable_address());

	xprintf("max Qval = %f\n", max);

	return 0;
}


static int usrcmd_rl_select_action(int argc, char **argv) {
	int state = 0;
	int num_actions = 5;

	rl_init_Qtable();
	int action = rl_select_action(state, num_actions, rl_get_Qtable_address());

	tm_printf("action = %d\n", action);

	return 0;
}


static int usrcmd_rl_epsilon_greedy(int argc, char **argv) {
	int state = 0;
	int num_actions = 5;
	int epsilon = 10;
	int action;

	rl_init_Qtable();
	rl_init_epsilon_greedy();
	action = rl_epsilon_greedy(epsilon, state, num_actions, rl_get_Qtable_address());

	tm_printf("action(epsilon_greedy) = %d\n", action);

	return 0;
}

static int usrcmd_rl_learning(int argc, char **argv) {
	tk_set_flg(rl_get_flgid(), RL_START);
	return 0;
}

static int usrcmd_rl_action(int argc, char **argv) {
	tk_set_flg(rl_get_flgid(), RL_ACTION_START);

	return 0;
}

static int usrcmd_rl_output_Qtable(int argc, char **argv){
	rl_output_Qtable();

	return 0;
}

static int usrcmd_rl_set_example_Qtable(int argc, char **argv) {
	rl_set_example_Qtable_memcpy();

	return 0;
}
