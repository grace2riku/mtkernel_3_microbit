#ifndef PWM_H
#define PWM_H

void pwm_init(void);
void pwm_start_duty(INT unit, INT duty);
void pwm_stop(INT unit);

#endif
