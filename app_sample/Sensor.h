#ifndef SENSOR_H
#define SENSOR_H

// TODO: ラインセンサー値の見直し・修正
#define WHITE_THRESHORD         52	/* 白の上限値 */
#define LIGHTGRAY_THRESHORD     159	/* 淡い灰色の上限値 */
#define GRAY_THRESHORD          266	/* 灰色の上限値 */
#define DARKGRAY_THRESHORD      373	/* 濃い灰色の上限値 */
#define BLACK_THRESHORD         480	/* 黒の上限値 */

extern void ss_init(void);
extern void ss_term(void);
_H ss_getLightValue(void);

#endif /* SENSOR_H */
