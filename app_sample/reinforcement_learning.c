/*
 * reinforcement_learning.c
 *
 *  Created on: 2025/08/03
 *      Author: k-abe
 */
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"

#include <stdlib.h>	// srand, rand
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "reinforcement_learning.h"
#include "ad.h"
#include "Sensor.h"
#include "motor_drv.h"

// Qtable
// 1次元目: 状態の数
// 2次元目: 行動の数
static float mQtable[1][5];

int rl_get_state(void) {
	int state = 3;
	_H adc_data[3];

    // adc_data[0] == P0(左側), adc_data[1] == P1(中央), adc_data[2] == P2(右側)
	analogRead3(adc_data);

	if (adc_data[2] > BLACK_THRESHORD) {
		state = 1;	// 右のセンサーが反応
	} else if (adc_data[0] > GRAY_THRESHORD) {
		state = 2;	// 左のセンサーが反応
	} else if (adc_data[1] > BLACK_THRESHORD){
		state = 0;	// 中央のセンサーが反応
	}

	return state;
}


int rl_get_reward(void) {
	int reword = 0;
	_H adc_data[3];

    // adc_data[0] == P0(左側), adc_data[1] == P1(中央), adc_data[2] == P2(右側)
	analogRead3(adc_data);

	if (adc_data[0] > GRAY_THRESHORD || adc_data[2] > BLACK_THRESHORD) {
		reword = 0;	// 左または右のセンサーが反応
	} else if (adc_data[1] > BLACK_THRESHORD){
		reword = 1;	// 中央のセンサーが反応
	} else {
		reword = -100;
	}

	return reword;
}


void rl_move(int action) {
	if (action == 0) {
		// 前進
		motor_drive(30, 30);
	} else if (action == 1) {
		// 右旋回
		motor_drive(0, 30);
	} else {
		// 右旋回
		motor_drive(30, 0);
	}
}

void rl_init_Qtable(void) {
	mQtable[0][0] = 1.0;
	mQtable[0][1] = 3.0;
	mQtable[0][2] = 2.0;
	mQtable[0][3] = 5.0;
	mQtable[0][4] = 1.0;
}

float rl_max_Qval(int state, int num_actions, QtablePtr Qtable) {
	float max;
	int i = 0;

	max = Qtable[state][0];
	for (i = 1; i < num_actions; i++) {
		if (Qtable[state][i] > max) {
			max = Qtable[state][i];
		}
	}
	return max;
}

QtablePtr rl_get_Qtable_address(void) {
	return mQtable;
}


int rl_select_action(int state, int num_actions, QtablePtr Qtable) {
	float max;
	int i = 0;
	int i_max;
	int action;

	max = Qtable[state][0];
	i_max = 0;

	for (i = 1; i < num_actions; i++) {
		if (Qtable[state][i] > max) {
			max = Qtable[state][i];
			i_max = i;
		}
	}
	action = i_max;
	return action;
}


void rl_init_epsilon_greedy(void) {
	SYSTIM	tim;
	tk_get_tim(&tim);		/* 現在時刻を取得 */

	tm_printf("rl_init_epsilon_greedy() seed(tk_get_tim() output parameter SYSTIM Lower 32bit)= %d\n", tim.lo);

	srand(tim.lo);
}


int rl_epsilon_greedy(int epsilon, int state, int num_action, QtablePtr Qtable) {
	int action;

	if (epsilon > rand() % 100) {
		// 無作為に行動を選択する
		action = rand() % num_action;

		tm_printf("rl_epsilon_greedy() rand action = %d\n", action);
	} else {
		// 最大のQ値を持つ行動を選択する
		action = rl_select_action(state, num_action, Qtable);

		tm_printf("rl_epsilon_greedy() select action = %d\n", action);
	}
	return action;
}
