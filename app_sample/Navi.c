#include "Course.h"
#include "Drive.h"

#include "Navi.h"

static int movefrontback_dir = 0;	/* 0:前進/1:後進 */

void nv_init(void) {

    return;
}


void nv_term(void) {

    return;
}

void set_movefrontback_dir(int dir) {
	movefrontback_dir = dir;
}

int get_movefrontback_dir(void) {
	return movefrontback_dir;
}

directionVector_t nv_naviCourse(diffCourse_t diff) {
    directionVector_t   navi;

    /* 前後方向のデフォルトは「前進」 */
    if (!movefrontback_dir) {
    	navi.forward = eMoveForward;
    } else {
    	navi.forward = eMoveBackward;
    }
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
