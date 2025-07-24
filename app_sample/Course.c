#include "Road.h"
#include "Course.h"

void cs_init(void) {
    rd_init();
    return;
}


void cs_term(void) {
    rd_term();
    return;
}


diffCourse_t cs_detectDifference(void) {
    roadColor_t     color;
    diffCourse_t    diff;

    color = rd_getRoadColor();

    switch (color) {
    case eWhite:
        diff = eDiffLeft;
        break;
    case eLightGray:
        diff = eDiffLeft;
        break;
    case eGray:
        diff = eNoDiff;
        break;
    case eDarkGray:
        diff = eDiffRight;
        break;
    case eBlack:
        diff = eDiffRight;
        break;    
    default:
        diff = eDiffRight;
        break;
    }

    return diff;
}
