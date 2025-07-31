/*
 * log.h
 *
 *  Created on: 2025/07/30
 *      Author: k-abe
 */

#ifndef LOG_H
#define LOG_H

void set_eneble_log(BOOL flg);
BOOL get_enable_log(void);
void update_xprintf_route(void);

extern void set_acc_sensor_read_timing(unsigned short timing);
extern unsigned short get_acc_sensor_read_timing(void);

#endif /* LOG_H */
