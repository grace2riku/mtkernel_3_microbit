#include <tk/tkernel.h>

#include "ad.h"
#include "Sensor.h"

void ss_init(void) {

    return;
}


void ss_term(void) {

    return;
}


_H ss_getLightValue(void) {
	_H lightValue[3];

    // ch0=P0.02/AIN0(RING0==P0), ch1=P0.03/AIN1(RING1==P1), ch2=P0.04/AIN2(RING2==P2)でA/D変換
	analogRead3(lightValue);

    return lightValue[1];
}
