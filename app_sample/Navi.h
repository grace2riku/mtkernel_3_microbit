#ifndef NAVI_H
#define NAVI_H

extern void nv_init(void);
extern void nv_term(void);
extern directionVector_t nv_naviCourse(diffCourse_t);
extern void set_movefrontback_dir(int dir);
extern int get_movefrontback_dir(void);

#endif  /* NAVI_H */
