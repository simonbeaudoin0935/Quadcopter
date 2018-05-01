/*
 * LSM9DS1.h
 *
 *  Created on: Apr 28, 2018
 *      Author: simon
 */

#ifndef IMU_LSM9DS1_LSM9DS1_H_
#define IMU_LSM9DS1_LSM9DS1_H_

#include "LSM9DS1_regs.h"
#include "stm32f4xx.h"

void LSM_init(void);

uint8_t LSM_AG_whoami(void);

uint8_t LSM_AG_read_interrupt(void);

int16_t LSM_AG_temp_read(void);

uint8_t LSM_AG_read_status_1(void);

uint8_t LSM_AG_read_status_2(void);

uint8_t LSM_AG_read_fifo_status(void);

void LSM_read_gyr(float gyr_data[3]);

void LSM_read_acc(float acc_data[3]);

void LSM_read_mag(float mag_data[3]);

uint8_t LSM_M_whoami(void);

void LSM_acc_cal(void);

void LSM_gyr_cal(void);

#endif /* IMU_LSM9DS1_LSM9DS1_H_ */
