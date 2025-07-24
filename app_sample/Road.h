#ifndef ROAD_H
#define ROAD_H

/* 路面の色 */
typedef enum {
    eWhite,     /* 白 */
    eLightGray, /* 明るい灰色 */
    eGray,      /* 灰色 */
    eDarkGray,  /* 暗い灰色 */
    eBlack      /* 黒 */
} roadColor_t;

extern void rd_init(void);
extern void rd_term(void);
extern roadColor_t rd_getRoadColor(void);

#endif /* ROAD_H */