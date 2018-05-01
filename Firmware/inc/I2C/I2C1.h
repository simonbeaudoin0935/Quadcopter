#ifndef I2C1_H_
#define I2C1_H_

#include "stm32f4xx.h"

void I2C1_init(void);

uint8_t I2C1_IsDeviceConnected(uint8_t address);

void I2C1_WriteReg(uint8_t address, uint8_t reg, uint8_t data);

uint8_t I2C1_ReadReg(uint8_t address, uint8_t reg, uint8_t* data);

void I2C1_ReadRegs(uint8_t address, uint8_t reg_start, uint8_t* data, uint8_t size);

#endif //I2C1_H_
