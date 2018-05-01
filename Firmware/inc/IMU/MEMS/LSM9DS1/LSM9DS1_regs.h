/*
 * LSM9DS1_regs.h
 *
 *  Created on: Apr 28, 2018
 *      Author: simon
 */

#ifndef IMU_LSM9DS1_LSM9DS1_REGS_H_
#define IMU_LSM9DS1_LSM9DS1_REGS_H_

#define READ_FLAG           0x80

/////////////////////////////////////////
// LSM9DS1 Accel/Gyro (XL/G) Registers //
/////////////////////////////////////////
#define ACT_THS				0x04
#define ACT_DUR				0x05
#define INT_GEN_CFG_XL		0x06
#define INT_GEN_THS_X_XL	0x07
#define INT_GEN_THS_Y_XL	0x08
#define INT_GEN_THS_Z_XL	0x09
#define INT_GEN_DUR_XL		0x0A
#define REFERENCE_G			0x0B
#define INT1_CTRL			0x0C
#define INT2_CTRL			0x0D
#define WHO_AM_I_XG			0x0F
#define CTRL_REG1_G			0x10
#define CTRL_REG2_G			0x11
#define CTRL_REG3_G			0x12
#define ORIENT_CFG_G		0x13
#define INT_GEN_SRC_G		0x14
#define OUT_TEMP_L			0x15
#define OUT_TEMP_H			0x16
#define STATUS_REG_0		0x17
#define OUT_X_L_G			0x18
#define OUT_X_H_G			0x19
#define OUT_Y_L_G			0x1A
#define OUT_Y_H_G			0x1B
#define OUT_Z_L_G			0x1C
#define OUT_Z_H_G			0x1D
#define CTRL_REG4			0x1E
#define CTRL_REG5_XL		0x1F
#define CTRL_REG6_XL		0x20
#define CTRL_REG7_XL		0x21
#define CTRL_REG8			0x22
#define CTRL_REG9			0x23
#define CTRL_REG10			0x24
#define INT_GEN_SRC_XL		0x26
#define STATUS_REG_1		0x27
#define OUT_X_L_XL			0x28
#define OUT_X_H_XL			0x29
#define OUT_Y_L_XL			0x2A
#define OUT_Y_H_XL			0x2B
#define OUT_Z_L_XL			0x2C
#define OUT_Z_H_XL			0x2D
#define FIFO_CTRL			0x2E
#define FIFO_SRC			0x2F
#define INT_GEN_CFG_G		0x30
#define INT_GEN_THS_XH_G	0x31
#define INT_GEN_THS_XL_G	0x32
#define INT_GEN_THS_YH_G	0x33
#define INT_GEN_THS_YL_G	0x34
#define INT_GEN_THS_ZH_G	0x35
#define INT_GEN_THS_ZL_G	0x36
#define INT_GEN_DUR_G		0x37

///////////////////////////////
// LSM9DS1 Magneto Registers //
///////////////////////////////
#define OFFSET_X_REG_L_M	0x05
#define OFFSET_X_REG_H_M	0x06
#define OFFSET_Y_REG_L_M	0x07
#define OFFSET_Y_REG_H_M	0x08
#define OFFSET_Z_REG_L_M	0x09
#define OFFSET_Z_REG_H_M	0x0A
#define WHO_AM_I_M			0x0F
#define CTRL_REG1_M			0x20
#define CTRL_REG2_M			0x21
#define CTRL_REG3_M			0x22
#define CTRL_REG4_M			0x23
#define CTRL_REG5_M			0x24
#define STATUS_REG_M		0x27
#define OUT_X_L_M			0x28
#define OUT_X_H_M			0x29
#define OUT_Y_L_M			0x2A
#define OUT_Y_H_M			0x2B
#define OUT_Z_L_M			0x2C
#define OUT_Z_H_M			0x2D
#define INT_CFG_M			0x30
#define INT_SRC_M			0x31
#define INT_THS_L_M			0x32
#define INT_THS_H_M			0x33

////////////////////////////////
// LSM9DS1 WHO_AM_I Responses //
////////////////////////////////
#define WHO_AM_I_AG_RSP		0x68
#define WHO_AM_I_M_RSP		0x3D

//////////////////////////////////
// LSM9DS1 Gyro ctl_1 reg masks //
//////////////////////////////////
#define GYRO_ODR_14_9Hz     0x20
#define GYRO_ODR_59_5HZ     0x40
#define GYRO_ODR_119HZ      0x60
#define GYRO_ODR_238HZ      0x80
#define GYRO_ODR_476HZ      0xA0
#define GYRO_ODR_952HZ      0xC0

#define GYRO_SCALE_245DPS   0x00
#define GYRO_SCALE_500DPS   0x08
#define GYRO_SCALE_2000DPS  0x18

#define GYRO_BANDWIDTH_1    0x00
#define GYRO_BANDWIDTH_2    0x01
#define GYRO_BANDWIDTH_3    0x02
#define GYRO_BANDWIDTH_4    0x03


//////////////////////////////////
// LSM9DS1 acc ctl_5  reg masks //
//////////////////////////////////
#define ACC_ODR_PWRDN       0x00
#define ACC_ODR_10HZ		0x20
#define ACC_ODR_50HZ		0x40
#define ACC_ODR_119HZ		0x60
#define ACC_ODR_238HZ		0x80
#define ACC_ODR_476HZ		0xA0
#define ACC_ODR_952HZ		0xC0

#define ACC_SCALE_2G        0x00
#define ACC_SCALE_4G        0x10
#define ACC_SCALE_8G        0x18
#define ACC_SCALE_16G       0x08



#endif /* IMU_LSM9DS1_LSM9DS1_REGS_H_ */


