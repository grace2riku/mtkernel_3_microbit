/*
 * reinforcement_learning.c
 *
 *  Created on: 2025/08/03
 *      Author: k-abe
 */
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "ad.h"
#include "Sensor.h"

int rl_get_state(void) {
	int state = 3;
	_H adc_data[3];

    // adc_data[0] == P0(左側), adc_data[1] == P1(中央), adc_data[2] == P2(右側)
	analogRead3(adc_data);

	if (adc_data[2] > BLACK_THRESHORD) {
		state = 1;	// 右のセンサーが反応
	} else if (adc_data[0] > BLACK_THRESHORD) {
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

	if (adc_data[2] > BLACK_THRESHORD) {
		reword = -100;	// 右のセンサーが反応
	} else if (adc_data[1] > BLACK_THRESHORD){
		reword = 1;	// 中央のセンサーが反応
	}

	return reword;
}
