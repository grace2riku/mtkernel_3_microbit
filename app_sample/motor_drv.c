/*
 * motor.c
 *
 *  Created on: 2025/07/16
 *      Author: k-abe
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "pwm.h"
#include "gpio.h"

EXPORT void motor_drive(int left_duty, int right_duty) {
	if (left_duty >= 0) {
		// ch1 左輪逆転PWM停止
		pwm_stop(1);
		// 左輪正転
		out_gpio_pin(0, 17, 0);			// P0.17(== P13) = 0
		pwm_start_duty(0, left_duty);	// P0.13(== P15) = PWM 0
	} else {
		// ch0 左輪正転PWM停止
		pwm_stop(0);
		// 左輪逆転
		pwm_start_duty(1, -left_duty);	// P0.17(== P13) = PWM 1
		out_gpio_pin(0, 13, 0);			// P0.13(== P15) = 0
	}
	if (right_duty >= 0) {
		// ch3 右輪逆転PWM停止
		pwm_stop(3);
		// 右輪正転
		out_gpio_pin(0, 1, 0);			// P0.01(== P14) = 0
		pwm_start_duty(2, right_duty);	// P1.02(== P16) = PWM 2
	} else {
		// ch2 右輪正転PWM停止
		pwm_stop(2);
		// 右輪逆転
		pwm_start_duty(3, -right_duty);	// P0.01(== P14) = PWM 3
		out_gpio_pin(1, 2, 0);			// P1.02(== P16) = 0
	}
}

EXPORT void motor_stop(void) {
	out_gpio_pin(0, 17, 1);	// P0.17(== P13)
	out_gpio_pin(0, 13, 1);	// P0.13(== P15)
	out_gpio_pin(0, 1, 1);	// P0.01(== P14)
	out_gpio_pin(1, 2, 1);	// P1.02(== P16)
	pwm_stop(0);			// ch0 左輪正転PWM停止
	pwm_stop(1);			// ch1 左輪逆転PWM停止
	pwm_stop(2);			// ch2 右輪正転PWM停止
	pwm_stop(3);			// ch3 右輪逆転PWM停止
}
