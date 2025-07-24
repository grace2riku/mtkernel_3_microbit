#ifndef DRIVE_H
#define DRIVE_H

/* 前後方向 */
typedef enum {
    eStopForward,   /* その場 */
    eMoveForward,   /* 前方向 */
} directForward_t;

/* 左右方向 */
typedef enum {
    eTurnLeft,      /* 左方向 */
    eStraight,      /* 直進方向 */
    eTurnRight,     /* 右方向 */
} directTurn_t;

typedef struct {
    directForward_t forward;
    directTurn_t    turn;
} directionVector_t;

extern void dr_init(void);
extern void dr_term(void);
extern void dr_move(directionVector_t);

#endif  /* DRIVE_H */