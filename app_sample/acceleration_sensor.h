/*
 * acceleration_sensor.h
 *
 *  Created on: 2025/07/17
 *      Author: k-abe
 */

#ifndef ACCELERATION_SENSOR_H
#define ACCELERATION_SENSOR_H

void acceleration_sensor_init(void);
void acceleration_sensor_read(int* x, int* y, int* z);

#endif /* ACCELERATION_SENSOR_H */
