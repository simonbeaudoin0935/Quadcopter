/*
 * ADA9DOF.h
 *
 *  Created on: Apr 30, 2018
 *      Author: simon
 */

#ifndef IMU_ADA9DOF_ADA9DOF_H_
#define IMU_ADA9DOF_ADA9DOF_H_

#include "stm32f4xx.h"
#include "GYRO_regs.h"

#define I2C_ADDR_ACCELEROMETER (0x19 << 1) //0b00011001
#define I2C_ADDR_MAGNETOMETER  (0x1E << 1) //0b00011110
#define I2C_ADDR_GYROSCOPE     (0x69 << 1) //0b01101001

#define GYRO_SENSITIVITY_250DPS  (0.00875F)
#define GYRO_SENSITIVITY_500DPS  (0.0175F)
#define GYRO_SENSITIVITY_2000DPS (0.070F)

void ADA_init(void);
void ADA_gyro_read(float data[3]);

#endif /* IMU_ADA9DOF_ADA9DOF_H_ */
