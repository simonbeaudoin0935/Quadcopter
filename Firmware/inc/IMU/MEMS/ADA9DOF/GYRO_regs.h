/*
 * GYRO_regs.h
 *
 *  Created on: Apr 30, 2018
 *      Author: simon
 */

#ifndef IMU_ADA9DOF_GYRO_REGS_H_
#define IMU_ADA9DOF_GYRO_REGS_H_

#define    GYRO_REGISTER_WHO_AM_I             0x0F   // 11010100   r
#define    GYRO_REGISTER_CTRL_REG1            0x20   // 00000111   rw
#define    GYRO_REGISTER_CTRL_REG2            0x21   // 00000000   rw
#define    GYRO_REGISTER_CTRL_REG3            0x22   // 00000000   rw
#define    GYRO_REGISTER_CTRL_REG4            0x23   // 00000000   rw
#define    GYRO_REGISTER_CTRL_REG5            0x24   // 00000000   rw
#define    GYRO_REGISTER_REFERENCE            0x25   // 00000000   rw
#define    GYRO_REGISTER_OUT_TEMP             0x26   //            r
#define    GYRO_REGISTER_STATUS_REG           0x27   //            r
#define    GYRO_REGISTER_OUT_X_L              0x28   //            r
#define    GYRO_REGISTER_OUT_X_H              0x29   //            r
#define    GYRO_REGISTER_OUT_Y_L              0x2A   //            r
#define    GYRO_REGISTER_OUT_Y_H              0x2B   //            r
#define    GYRO_REGISTER_OUT_Z_L              0x2C   //            r
#define    GYRO_REGISTER_OUT_Z_H              0x2D   //            r
#define    GYRO_REGISTER_FIFO_CTRL_REG        0x2E   // 00000000   rw
#define    GYRO_REGISTER_FIFO_SRC_REG         0x2F   //            r
#define    GYRO_REGISTER_INT1_CFG             0x30   // 00000000   rw
#define    GYRO_REGISTER_INT1_SRC             0x31   //            r
#define    GYRO_REGISTER_TSH_XH               0x32   // 00000000   rw
#define    GYRO_REGISTER_TSH_XL               0x33   // 00000000   rw
#define    GYRO_REGISTER_TSH_YH               0x34   // 00000000   rw
#define    GYRO_REGISTER_TSH_YL               0x35   // 00000000   rw
#define    GYRO_REGISTER_TSH_ZH               0x36   // 00000000   rw
#define    GYRO_REGISTER_TSH_ZL               0x37   // 00000000   rw
#define    GYRO_REGISTER_INT1_DURATION        0x38   // 00000000   rw

#define      LSM303_REGISTER_ACCEL_CTRL_REG1_A          0x20   // 00000111   rw
#define      LSM303_REGISTER_ACCEL_CTRL_REG2_A          0x21   // 00000000   rw
#define      LSM303_REGISTER_ACCEL_CTRL_REG3_A          0x22   // 00000000   rw
#define      LSM303_REGISTER_ACCEL_CTRL_REG4_A          0x23   // 00000000   rw
#define      LSM303_REGISTER_ACCEL_CTRL_REG5_A          0x24   // 00000000   rw
#define      LSM303_REGISTER_ACCEL_CTRL_REG6_A          0x25   // 00000000   rw
#define      LSM303_REGISTER_ACCEL_REFERENCE_A          0x26   // 00000000   r
#define      LSM303_REGISTER_ACCEL_STATUS_REG_A         0x27   // 00000000   r
#define      LSM303_REGISTER_ACCEL_OUT_X_L_A            0x28
#define      LSM303_REGISTER_ACCEL_OUT_X_H_A            0x29
#define      LSM303_REGISTER_ACCEL_OUT_Y_L_A            0x2A
#define      LSM303_REGISTER_ACCEL_OUT_Y_H_A            0x2B
#define      LSM303_REGISTER_ACCEL_OUT_Z_L_A            0x2C
#define      LSM303_REGISTER_ACCEL_OUT_Z_H_A            0x2D
#define      LSM303_REGISTER_ACCEL_FIFO_CTRL_REG_A      0x2E
#define      LSM303_REGISTER_ACCEL_FIFO_SRC_REG_A       0x2F
#define      LSM303_REGISTER_ACCEL_INT1_CFG_A           0x30
#define      LSM303_REGISTER_ACCEL_INT1_SOURCE_A        0x31
#define      LSM303_REGISTER_ACCEL_INT1_THS_A           0x32
#define      LSM303_REGISTER_ACCEL_INT1_DURATION_A      0x33
#define      LSM303_REGISTER_ACCEL_INT2_CFG_A           0x34
#define      LSM303_REGISTER_ACCEL_INT2_SOURCE_A        0x35
#define      LSM303_REGISTER_ACCEL_INT2_THS_A           0x36
#define      LSM303_REGISTER_ACCEL_INT2_DURATION_A      0x37
#define      LSM303_REGISTER_ACCEL_CLICK_CFG_A          0x38
#define      LSM303_REGISTER_ACCEL_CLICK_SRC_A          0x39
#define		 LSM303_REGISTER_ACCEL_CLICK_THS_A          0x3A
#define      LSM303_REGISTER_ACCEL_TIME_LIMIT_A         0x3B
#define      LSM303_REGISTER_ACCEL_TIME_LATENCY_A       0x3C
#define      LSM303_REGISTER_ACCEL_TIME_WINDOW_A        0x3D

#endif /* IMU_ADA9DOF_GYRO_REGS_H_ */
