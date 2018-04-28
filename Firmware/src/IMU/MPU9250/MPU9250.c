#include "IMU/MPU9250/MPU9250.h"
#include "SPI/SPI1.h"
#include "FreeRTOS.h"
#include "task.h"

float acc_divider, gyro_divider;
float acc_bias[3] = {0.0, 0.0, 0.0};
float gyro_bias[3] = {0.0, 0.0, 0.0};

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

#define MPU_InitRegNum 17


	uint8_t lpf_gyro = BITS_DLPF_CFG_188HZ;
	uint8_t lpf_acc  = BITS_DLPF_CFG_188HZ;

	int8_t MPU_Init_Data[MPU_InitRegNum][2] = {
		{BIT_H_RESET, MPUREG_PWR_MGMT_1},        // Reset Device
	    {0x01, MPUREG_PWR_MGMT_1},               // Clock Source
	    {0x00, MPUREG_PWR_MGMT_2},               // Enable Acc & Gyro
	    {lpf_gyro, MPUREG_CONFIG},               // Use DLPF set Gyroscope bandwidth 184Hz, temperature bandwidth 188Hz
	    {BITS_FS_250DPS, MPUREG_GYRO_CONFIG},    // +-250dps
	    {BITS_FS_2G, MPUREG_ACCEL_CONFIG},       // +-2G
	    {lpf_acc, MPUREG_ACCEL_CONFIG_2},        // Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz
	    {0x12, MPUREG_INT_PIN_CFG},              //
	  //{0x40, MPUREG_I2C_MST_CTRL},             // I2C Speed 348 kHz
	  //{0x20, MPUREG_USER_CTRL},                // Enable AUX
	    {0x30, MPUREG_USER_CTRL},                // I2C Master mode and set I2C_IF_DIS to disable slave mode I2C bus
	    {0x0D, MPUREG_I2C_MST_CTRL},             // I2C configuration multi-master  IIC 400KHz

	    {AK8963_I2C_ADDR, MPUREG_I2C_SLV0_ADDR}, // Set the I2C slave addres of AK8963 and set for write.
	  //{0x09, MPUREG_I2C_SLV4_CTRL},
	  //{0x81, MPUREG_I2C_MST_DELAY_CTRL},       // Enable I2C delay

	    {AK8963_CNTL2, MPUREG_I2C_SLV0_REG},     // I2C slave 0 register address from where to begin data transfer
	    {0x01, MPUREG_I2C_SLV0_DO},              // Reset AK8963
	    {0x81, MPUREG_I2C_SLV0_CTRL},            // Enable I2C and set 1 byte

	    {AK8963_CNTL1, MPUREG_I2C_SLV0_REG},     // I2C slave 0 register address from where to begin data transfer
	#ifdef AK8963FASTMODE
	    {0x16, MPUREG_I2C_SLV0_DO},              // Register value to 100Hz continuous measurement in 16bit
	#else
	    {0x12, MPUREG_I2C_SLV0_DO},              // Register value to 8Hz continuous measurement in 16bit
	#endif
	    {0x81, MPUREG_I2C_SLV0_CTRL}             //Enable I2C and set 1 byte
	};

	for(int i = 0; i < MPU_InitRegNum; i++) {
	    MPU_WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
	    vTaskDelay(1);  // I2C must slow down the write speed, otherwise it won't work
	}

	MPU_set_acc_scale(BITS_FS_2G);
	MPU_set_gyro_scale(BITS_FS_250DPS);
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


/*                                ACCELEROMETER SCALE
 * usage: call this function at startup, after initialization, to set the right range for the
 * accelerometers. Suitable ranges are:
 * BITS_FS_2G
 * BITS_FS_4G
 * BITS_FS_8G
 * BITS_FS_16G
 * returns the range set (2,4,8 or 16)
 */
unsigned int MPU_set_acc_scale(int scale){
    unsigned int temp_scale;
    MPU_WriteReg(MPUREG_ACCEL_CONFIG, scale);

    switch (scale){
        case BITS_FS_2G:
            acc_divider=16384.0;
        break;
        case BITS_FS_4G:
            acc_divider=8192.0;
        break;
        case BITS_FS_8G:
            acc_divider=4096.0;
        break;
        case BITS_FS_16G:
            acc_divider=2048.0;
        break;
    }
    temp_scale = MPU_ReadReg(MPUREG_ACCEL_CONFIG);

    switch (temp_scale){
        case BITS_FS_2G:
            temp_scale=2;
        break;
        case BITS_FS_4G:
            temp_scale=4;
        break;
        case BITS_FS_8G:
            temp_scale=8;
        break;
        case BITS_FS_16G:
            temp_scale=16;
        break;
    }
    return temp_scale;
}



/*                                 GYROSCOPE SCALE
 * usage: call this function at startup, after initialization, to set the right range for the
 * gyroscopes. Suitable ranges are:
 * BITS_FS_250DPS
 * BITS_FS_500DPS
 * BITS_FS_1000DPS
 * BITS_FS_2000DPS
 * returns the range set (250,500,1000 or 2000)
 */

unsigned int MPU_set_gyro_scale(int scale){
    unsigned int temp_scale;
    MPU_WriteReg(MPUREG_GYRO_CONFIG, scale);

    switch (scale){
        case BITS_FS_250DPS:   gyro_divider = 131;  break;
        case BITS_FS_500DPS:   gyro_divider = 65.5; break;
        case BITS_FS_1000DPS:  gyro_divider = 32.8; break;
        case BITS_FS_2000DPS:  gyro_divider = 16.4; break;
    }

    temp_scale = MPU_ReadReg(MPUREG_GYRO_CONFIG);


    switch (temp_scale){
        case BITS_FS_250DPS:   temp_scale = 250;    break;
        case BITS_FS_500DPS:   temp_scale = 500;    break;
        case BITS_FS_1000DPS:  temp_scale = 1000;   break;
        case BITS_FS_2000DPS:  temp_scale = 2000;   break;
    }
    return temp_scale;
}

/*                                 READ ACCELEROMETER
 * usage: call this function to read accelerometer data. Axis represents selected axis:
 * 0 -> X axis
 * 1 -> Y axis
 * 2 -> Z axis
 */
void MPU_read_acc(float acc_data[3])
{
    uint8_t response[6];
    int16_t bit_data;
    float data;
    int i;
    MPU_ReadRegs(MPUREG_ACCEL_XOUT_H,response,6);
    for(i = 0; i < 3; i++) {
        bit_data = ((int16_t)response[i*2]<<8)|response[i*2+1];
        data = (float)bit_data;
        acc_data[i] = data/acc_divider - acc_bias[i];
    }

}


/*                                 READ GYROSCOPE
 * usage: call this function to read gyroscope data. Axis represents selected axis:
 * 0 -> X axis
 * 1 -> Y axis
 * 2 -> Z axis
 */

void MPU_read_gyro(float gyro_data[3])
{
    uint8_t response[6];
    int16_t bit_data;
    float data;
    int i;
    MPU_ReadRegs(MPUREG_GYRO_XOUT_H,response,6);
    for(i = 0; i < 3; i++) {
        bit_data = ((int16_t)response[i*2]<<8) | response[i*2+1];
        data = (float)bit_data;
        gyro_data[i] = data/gyro_divider - gyro_bias[i];
    }

}
