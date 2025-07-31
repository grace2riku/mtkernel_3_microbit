#include "Course.h"
#include "Drive.h"
#include "Navi.h"

#include "Trace.h"


typedef enum {
    eStop,			/* 停止中 */
    eRunning,       /* 走行中 */
} robotState_t;

static robotState_t current_state;

static void tr_traceCource(void);


void tr_init(void) {
    cs_init();
    nv_init();
    dr_init();
    current_state = eStop; /* 電源オン直後は停止 */

    return;
}


void tr_term(void) {
    dr_term();
    nv_term();
    cs_term();

    return;
}


void tr_run(void) {
    switch (current_state)
    {
    case eStop:		/* 停止中 */
        /* なにもしない */
        break;
    case eRunning:	/* 走行中 */
        /* 走行する */
        tr_traceCource();
        break;    
    default:
        /* 何もしない */
        break;
    }
    return;
}


void tr_runnable(void) {
    switch (current_state) {
    case eStop:
        current_state = eRunning;
        break;
    case eRunning:
        /* 何もしない */
        break;    
    default:
        /* 何もしない */
        break;
    }
    return;
}

void tr_stop(void) {
    directionVector_t drct;

    switch (current_state) {
    case eStop:
        /* 何もしない */
        break;
    case eRunning:
        drct.forward = eStopForward;
        drct.turn = eStraight;
        dr_move(drct);
        current_state = eStop;
        break;
    default:
        /* 何もしない */
        break;
    }
    return;
}

static void tr_traceCource(void) {
    diffCourse_t        diff;   /* コースとのズレ */
    directionVector_t   drct;   /* 進行方向 */

    diff = cs_detectDifference();   /* ズレを検出する */
    drct = nv_naviCourse(diff);     /* 進行方向を決める */
    dr_move(drct);                  /* 進行方向に進む */

    return;
}
