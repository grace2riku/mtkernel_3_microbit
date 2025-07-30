#include "Motor.h"

#include "Drive.h"

/* 前回進行方向 */
static directionVector_t lastDirection;

static FrontBack_t dr_cvtFrontBack(directForward_t);
static LeftRight_t dr_cvtLeftRight(directTurn_t);

void dr_init(void) {
    lastDirection.forward = eStopForward;
    lastDirection.turn = eStraight;
    return;
}


void dr_term(void) {
    return;
}


void dr_move(directionVector_t direct) {
    mt_direction_t motor;   /* モータの駆動方向 */

    if ((direct.forward != lastDirection.forward)
        || (direct.turn != lastDirection.turn)) {
        /* 前後方向または左右方向が変化した時にモータを駆動 */

        /* 前後方向値を変換する */
        motor.front_back = dr_cvtFrontBack(direct.forward);

        /* 左右方向値を変換する */
        motor.left_right = dr_cvtLeftRight(direct.turn);

        lastDirection = direct;
        
        /* モータを駆動する */
        mt_drvMotor(motor);
    }

    return;
}

static FrontBack_t dr_cvtFrontBack(directForward_t forward) {
    FrontBack_t front_back;

    switch (forward) {
    case eStopForward:
        front_back = eSTOP;
        break;
    case eMoveForward:
        front_back = eFRONT;
        break;
    case eMoveBackward:
    	front_back = eBACK;
    	break;
    default:
        /* 停止させる */
        front_back = eSTOP;
        break;
    }
    return front_back;
}

static LeftRight_t dr_cvtLeftRight(directTurn_t turn) {
    LeftRight_t left_right; /* 左右方向 */

    switch (turn)
    {
    case eTurnLeft:
        left_right = eLEFT_TURN;
        break;
    case eStraight:
        left_right = eNO_TURN;
        break;
    case eTurnRight:
        left_right = eRIGHT_TURN;
        break;    
    default:
        /* 左右方向なし */
        left_right = eNO_TURN;
        break;
    }
    return left_right;
}
