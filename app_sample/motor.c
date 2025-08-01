#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "pwm.h"
#include "motor_drv.h"
#include "Motor.h"

#define DEFAULT_DUTY (30)	// デフォルトDuty比(%)
static int mduty = DEFAULT_DUTY;

static int now_duty[2];

void mt_init(void) {
	pwm_init();

    return;
}


void mt_term(void) {

    return;
}


void mt_drvMotor(mt_direction_t direction) {
	int left_duty, right_duty;

    switch (direction.front_back) {
    case eSTOP:
    	left_duty = right_duty = 0;
        break;
    case eFRONT:
    	left_duty = right_duty = mduty;
        break;
    case eBACK:
    	left_duty = right_duty = -mduty;
        break;
    default:
    	left_duty = right_duty = 0;
        break;
    }

    switch (direction.left_right) {
    case eNO_TURN:
        break;
    case eLEFT_TURN:
    	left_duty = 0;
        break;
    case eRIGHT_TURN:
    	right_duty = 0;
        break;
    default:
        break;
    }

    now_duty[0] = left_duty;
    now_duty[1] = right_duty;

	motor_drive(left_duty, right_duty);
}

int get_duty(void) {
	return mduty;
}

void set_duty(int duty) {
	mduty = duty;
}

void get_now_duty(int* duty) {
	duty[0] = now_duty[0];
	duty[1] = now_duty[1];
}
