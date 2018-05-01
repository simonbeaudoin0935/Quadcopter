/*
 * MPU9250.h
 *
 *  Created on: Apr 27, 2018
 *      Author: simon
 */

#ifndef MEMS_MPU9250_H_
#define MEMS_MPU9250_H_

#include "stm32f4xx.h"
#include "MPU9250_regs.h"

void MPU_init(void);

uint8_t MPU_whoami();
uint8_t MPU_AK8963_whoami();

unsigned int MPU_set_acc_scale(int scale);
unsigned int MPU_set_gyro_scale(int scale);

void MPU_read_acc(float acc_data[3]);
void MPU_read_gyr(float gyr_data[3]);
void MPU_read_mag(float mag_data[3]);

void MPU_acc_cal(void);
void MPU_gyr_cal(void);



#endif /* MEMS_MPU9250_H_ */
