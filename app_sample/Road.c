#include <tk/tkernel.h>
#include "Sensor.h"

#include "Road.h"

void rd_init(void) {

    return;
}


void rd_term(void) {

    return;
}

roadColor_t rd_getRoadColor(void) {
    unsigned char    light;
    roadColor_t     roadColor;

    light = ss_getLightValue();

    if (light < WHITE_THRESHORD) {
        roadColor = eWhite;
    } else if (light < LIGHTGRAY_THRESHORD) {
        roadColor = eLightGray;
    } else if (light < GRAY_THRESHORD) {
        roadColor = eGray;
    } else if (light < DARKGRAY_THRESHORD) {
        roadColor = eDarkGray;
    } else {
        roadColor = eBlack;
    }

    return roadColor;
}
