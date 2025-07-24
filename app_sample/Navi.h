#ifndef NAVI_H
#define NAVI_H

extern void nv_init(void);
extern void nv_term(void);
extern directionVector_t nv_naviCourse(diffCourse_t);

#endif  /* NAVI_H */