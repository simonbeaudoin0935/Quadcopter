#include "I2C/tm_stm32f4_i2c.h"
#include "IMU/ADA9DOF/ADA9DOF.h"





void ADA_init(void){
	TM_I2C_Write(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_CTRL_REG1, 0x00);
	TM_I2C_Write(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_CTRL_REG1, 0x0F);

	TM_I2C_Write(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_CTRL_REG4, 0x00);

}

void ADA_gyro_read(float data[3]){
	uint8_t buf[6];

//	TM_I2C_ReadMulti(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_X_L, buf, 6);
//
//	data[0] = ((float) ((int16_t)(buf[1] << 8 | buf[0]))) * GYRO_SENSITIVITY_250DPS;
//	data[1] = ((float) ((int16_t)(buf[3] << 8 | buf[2]))) * GYRO_SENSITIVITY_250DPS;
//	data[2] = ((float) ((int16_t)(buf[5] << 8 | buf[4]))) * GYRO_SENSITIVITY_250DPS;

	uint8_t xl = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_X_L);
	uint8_t xh = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_X_H);
	uint8_t yl = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_Y_L);
	uint8_t yh = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_Y_H);
	uint8_t zl = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_Z_L);
	uint8_t zh = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_OUT_Z_H);

	data[0] = ((float) ((int16_t)(xh << 8 | xl))) * GYRO_SENSITIVITY_250DPS;
	data[1] = ((float) ((int16_t)(yh << 8 | yl))) * GYRO_SENSITIVITY_250DPS;
	data[2] = ((float) ((int16_t)(zh << 8 | zl))) * GYRO_SENSITIVITY_250DPS;
}

