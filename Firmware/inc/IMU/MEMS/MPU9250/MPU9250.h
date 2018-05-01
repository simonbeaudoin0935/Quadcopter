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

#define MPU_select   GPIOC->BSRRH = GPIO_Pin_11;
#define MPU_deselect GPIOC->BSRRL = GPIO_Pin_11;

void MPU_init(void);


uint8_t MPU_WriteReg(uint8_t WriteAddr, uint8_t WriteData );
uint8_t MPU_ReadReg(uint8_t ReadAddr);
void    MPU_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );

uint8_t MPU_whoami();

unsigned int MPU_set_acc_scale(int scale);
unsigned int MPU_set_gyro_scale(int scale);

void MPU_read_acc(float acc_data[3]);
void MPU_read_gyro(float gyro_data[3]);

void MPU_acc_cal(void);
void MPU_gyr_cal(void);



#endif /* MEMS_MPU9250_H_ */
