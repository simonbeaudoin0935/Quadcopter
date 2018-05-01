#include "I2C/I2C1.h"
#include "stm32f4xx.h"

typedef enum{
	I2C_TRANSMITTER_MODE,
	I2C_RECEIVER_MODE
}I2C_DIRECTION;

typedef enum{
	I2C_ACK_DISABLE,
	I2C_ACK_ENABLE
}I2C_ACK;

#define I2C_TIMEOUT	20000

static uint8_t _I2C1_Start(uint8_t address, I2C_DIRECTION direction, I2C_ACK ack);
static uint8_t _I2C1_WriteData(uint8_t data);
static uint8_t _I2C1_Stop();
static uint8_t _I2C1_ReadAck(uint8_t* data);
static uint8_t _I2C1_ReadNack(uint8_t* data);


void I2C1_init(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOB;
	RCC->APB1ENR |= RCC_APB1Periph_I2C1;


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C1, &I2C_InitStruct); //init i2c1

	I2C1->CR1 |= I2C_CR1_PE;

}

uint8_t I2C1_IsDeviceConnected(uint8_t address){
	uint8_t connected = 1;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!_I2C1_Start(address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) {
		connected = 0;
	}

	/* STOP I2C */
	_I2C1_Stop();

	/* Return status */
	return connected;
}

void I2C1_WriteReg(uint8_t address, uint8_t reg, uint8_t data){
	_I2C1_Start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	_I2C1_WriteData(reg);
	_I2C1_WriteData(data);
	_I2C1_Stop();
}

uint8_t I2C1_ReadReg(uint8_t address, uint8_t reg, uint8_t* data){

	if(_I2C1_Start(address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) return 1;
	if(_I2C1_WriteData(reg)) return 1;
	if(_I2C1_Stop()) return 1;
	if(_I2C1_Start(address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE)) return 1;
	if(_I2C1_ReadAck(data)) return 1;
	return 0;
}

void I2C1_ReadRegs(uint8_t address, uint8_t reg_start, uint8_t* data, uint8_t count){

	_I2C1_Start(address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	_I2C1_WriteData(reg_start);
	//TM_I2C_Stop(I2Cx);
	_I2C1_Start(address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--) {
		if (!count) {
			/* Last byte */
			_I2C1_ReadNack(data++);
		} else {
			_I2C1_ReadAck(data++);
		}
	}
}


uint8_t _I2C1_Start(uint8_t address, I2C_DIRECTION direction, I2C_ACK ack){

	uint32_t timeout;


	I2C1->CR1 |= I2C_CR1_START;				/* Generate I2C start pulse */

	timeout = I2C_TIMEOUT;					/* Wait until start bit sent */
	while (!(I2C1->SR1 & I2C_SR1_SB)) {
		if (--timeout == 0x00) {
			return 1;
		}
	}


	if(ack == I2C_ACK_ENABLE) I2C1->CR1 |= I2C_CR1_ACK;         /* Enable ack if desired */


	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send address with zero last bit */
		I2C1->DR = address & ~I2C_OAR1_ADD0;

		/* Wait till finished */
		timeout = I2C_TIMEOUT;
		while (!(I2C1->SR1 & I2C_SR1_ADDR)) {
			if (--timeout == 0x00) {
				return 1;
			}
		}
		}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send address with 1 last bit */
		I2C1->DR = address | I2C_OAR1_ADD0;

		/* Wait till finished */
		timeout = I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--timeout == 0x00) {
				return 1;
			}
		}
	}

	/* Read status register to clear ADDR flag */
	I2C1->SR2;

	/* Return 0, everything ok */
	return 0;
}

uint8_t _I2C1_WriteData(uint8_t data) {

	/* Wait till I2C is not busy anymore */
	uint32_t timeout = I2C_TIMEOUT;
	while (!(I2C1->SR1 & I2C_SR1_TXE)) {
		if (--timeout == 0x00) {
			return 1;
		}
	}

	/* Send I2C data */
	I2C1->DR = data;

	return 0;
}

uint8_t _I2C1_Stop(){
	/* Wait till transmitter not empty */
	uint32_t timeout = I2C_TIMEOUT;
	while (((!(I2C1->SR1 & I2C_SR1_TXE)) || (!(I2C1->SR1 & I2C_SR1_BTF)))) {
		if (--timeout == 0x00) {
			return 1;
		}
	}

	/* Generate stop */
	I2C1->CR1 |= I2C_CR1_STOP;

	/* Return 0, everything ok */
	return 0;
}

uint8_t _I2C1_ReadAck(uint8_t* data){


	/* Enable ACK */
	I2C1->CR1 |= I2C_CR1_ACK;

	/* Wait till not received */
	uint32_t timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	*data = I2C1->DR;

	/* Return data */
	return 0;
}

uint8_t _I2C1_ReadNack(uint8_t* data) {

	/* Disable ACK */
	I2C1->CR1 &= ~I2C_CR1_ACK;

	/* Generate stop */
	I2C1->CR1 |= I2C_CR1_STOP;

	/* Wait till received */
	uint32_t timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	*data = I2C1->DR;

	/* Return data */
	return 0;
}
