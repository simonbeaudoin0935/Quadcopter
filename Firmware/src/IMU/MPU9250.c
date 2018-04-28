#include "MEMS/MPU9250.h"
#include "SPI/SPI1.h"

void MPU_init(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	MPU_deselect;
}

uint8_t MPU_WriteReg(uint8_t WriteAddr, uint8_t WriteData ){

	MPU_select;

	SPI1_send(WriteAddr);
	uint8_t data = SPI1_send(WriteData);

	MPU_deselect;

	return data;
}

uint8_t MPU_ReadReg(uint8_t ReadAddr){

	return MPU_WriteReg(ReadAddr | READ_FLAG, 0x00);

}
void MPU_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes ){

	MPU_select;

	SPI1_send(ReadAddr | READ_FLAG);
	for(int i = 0; i < Bytes; i++)
	    ReadBuf[i] = SPI1_send(0x00);

    MPU_deselect;
}

uint8_t MPU_whoami(){

	return MPU_WriteReg(MPUREG_WHOAMI|READ_FLAG, 0x00);
}
