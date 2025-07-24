#ifndef MOTOR_H
#define MOTOR_H

/* 前後方向 */
typedef enum {
    eSTOP,      /* その場 */
    eFRONT,     /* 前方向 */
    eBACK       /* 後方向  */
} FrontBack_t;

typedef enum {
    eNO_TURN,   /* 直進 */
    eLEFT_TURN, /* 左方向 */
    eRIGHT_TURN /* 右方向 */
} LeftRight_t;

typedef struct {
    FrontBack_t front_back;
    LeftRight_t left_right;
} mt_direction_t;


extern void mt_init(void);
extern void mt_term(void);
extern void mt_drvMotor(mt_direction_t);

#endif /* MOTOR_H */