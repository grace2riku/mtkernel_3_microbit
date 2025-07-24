#include "Course.h"
#include "Drive.h"

#include "Navi.h"

void nv_init(void) {

    return;
}


void nv_term(void) {

    return;
}


directionVector_t nv_naviCourse(diffCourse_t diff) {
    directionVector_t   navi;

    /* 前後方向は常に「前進」 */
    navi.forward = eMoveForward;

    /* 左右のブレの補正 */
    switch (diff) {
    case eNoDiff:
        navi.turn = eStraight;
        break;
    case eDiffRight:
        navi.turn = eTurnLeft;
        break;
    case eDiffLeft:
        navi.turn = eTurnRight;
        break;
    default:
        navi.turn = eStraight;
        break;
    }

    return navi;
}
