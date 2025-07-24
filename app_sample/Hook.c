#include "Hook.h"
#include "SysCtrl.h"

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "button.h"
#include "pwm.h"
#include "acceleration_sensor.h"
#include "led.h"

void device_initialize(void) {
    sc_init();

	button_init();
	acceleration_sensor_init();
	led_init();

    return;
}


void device_terminate(void) {
    sc_term();
    return;
}
