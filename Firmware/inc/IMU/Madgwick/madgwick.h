/*
 * madgwick.h
 *
 *  Created on: Apr 30, 2018
 *      Author: simon
 */

#ifndef IMU_MADGWICK_MADGWICK_H_
#define IMU_MADGWICK_MADGWICK_H_

void Madgwick_init(float frequency, float b);
void Madgwick_compute(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);


void computeAngles(float* roll, float* pitch, float* yaw);

#endif /* IMU_MADGWICK_MADGWICK_H_ */

