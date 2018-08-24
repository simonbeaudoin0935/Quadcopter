#include "IMU/MEMS/MPU9250/MPU9250.h"
#include "COM/SPI/SPI1.h"
#include "FreeRTOS.h"
#include "task.h"

#define MPU_select   GPIOC->BSRRH = GPIO_Pin_11;
#define MPU_deselect GPIOC->BSRRL = GPIO_Pin_11;

static float acc_divider, gyr_divider; //, mag_multiplier = 0.6;

static float acc_bias[3] = {-0.08, 0.0, -0.09};
static float gyr_bias[3] = {1.0, -1.6, -0.6};
static float mag_bias[3] = {0.68, 10.46, -3.37};
static const float mag_comp[3][3] = {{1.003,0.013,-0.018},{0.013,1.028,0.012},{-0.018,0.012,0.971}};


static uint8_t MPU_WriteReg(uint8_t WriteAddr, uint8_t WriteData );
static uint8_t MPU_ReadReg(uint8_t ReadAddr);
static void    MPU_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );

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


	uint8_t lpf_gyro = BITS_DLPF_CFG_5HZ;
	uint8_t lpf_acc  = BITS_DLPF_CFG_5HZ;

	int8_t MPU_Init_Data[MPU_InitRegNum][2] = {
		{0x80, MPUREG_PWR_MGMT_1},               // Reset Device
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
	    {0x16, MPUREG_I2C_SLV0_DO},              // Register value to 100Hz continuous measurement in 16bit
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

uint8_t MPU_AK8963_whoami(){
    uint8_t response;
    MPU_WriteReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    MPU_WriteReg(MPUREG_I2C_SLV0_REG, AK8963_WIA); //I2C slave 0 register address from where to begin data transfer
    MPU_WriteReg(MPUREG_I2C_SLV0_CTRL, 0x81); //Read 1 byte from the magnetometer

    //WriteReg(MPUREG_I2C_SLV0_CTRL, 0x81);    //Enable I2C and set bytes
    vTaskDelay(1);
    response = MPU_WriteReg(MPUREG_EXT_SENS_DATA_00|READ_FLAG, 0x00);    //Read I2C
    //ReadRegs(MPUREG_EXT_SENS_DATA_00,response,1);
    //response=WriteReg(MPUREG_I2C_SLV0_DO, 0x00);    //Read I2C

    return response;
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
        case BITS_FS_250DPS:   gyr_divider = 131;  break;
        case BITS_FS_500DPS:   gyr_divider = 65.5; break;
        case BITS_FS_1000DPS:  gyr_divider = 32.8; break;
        case BITS_FS_2000DPS:  gyr_divider = 16.4; break;
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


void MPU_read_acc(float acc_data[3])
{
    uint8_t response[6];

    MPU_ReadRegs(MPUREG_ACCEL_XOUT_H,response,6);

    acc_data[0] = (float)((int16_t)((response[0]<<8) + response[1])) / acc_divider - acc_bias[0];
    acc_data[1] = (float)((int16_t)((response[2]<<8) + response[3])) / acc_divider - acc_bias[1];
    acc_data[2] = (float)((int16_t)((response[4]<<8) + response[5])) / acc_divider - acc_bias[2];
}

void MPU_read_gyr(float gyr_data[3])
{
    uint8_t response[6];

    MPU_ReadRegs(MPUREG_GYRO_XOUT_H,response,6);

    gyr_data[0] = (float)((int16_t)((response[0]<<8) + response[1])) / gyr_divider - gyr_bias[0];
    gyr_data[1] = (float)((int16_t)((response[2]<<8) + response[3])) / gyr_divider - gyr_bias[1];
    gyr_data[2] = (float)((int16_t)((response[4]<<8) + response[5])) / gyr_divider - gyr_bias[2];
}

void MPU_read_mag(float mag_data[3]){

    uint8_t response[7];

    MPU_WriteReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG);  // Set the I2C slave addres of AK8963 and set for read.
    MPU_WriteReg(MPUREG_I2C_SLV0_REG, AK8963_HXL);                 // I2C slave 0 register address from where to begin data transfer
    MPU_WriteReg(MPUREG_I2C_SLV0_CTRL, 0x87);                      // Read 6 bytes from the magnetometer

    for(int i = 0; i != 2000; i++);
    MPU_ReadRegs(MPUREG_EXT_SENS_DATA_00,response,7);
    // must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.

//    extern int16_t mag[3];
//    mag[0] = (int16_t)((response[1]<<8) + response[0]);
//    mag[1] = (int16_t)((response[3]<<8) + response[2]);
//    mag[2] = (int16_t)((response[5]<<8) + response[4]);

    float x = ((float)((int16_t)((response[1]<<8) + response[0])))  - mag_bias[0];
    float y = ((float)((int16_t)((response[3]<<8) + response[2])))  - mag_bias[1];
    float z = ((float)((int16_t)((response[5]<<8) + response[4])))  - mag_bias[2];

    //x and y inversed
    mag_data[1] = x * mag_comp[0][0] + y * mag_comp[0][1] + z * mag_comp[0][2];
    mag_data[0] = x * mag_comp[1][0] + y * mag_comp[1][1] + z * mag_comp[1][2];
    //z flipped
    mag_data[2] = -(x * mag_comp[2][0] + y * mag_comp[2][1] + z * mag_comp[2][2]);


}

void MPU_acc_cal(void){
	float data[3];

	acc_bias[0] = 0.0;
	acc_bias[1] = 0.0;
	acc_bias[2] = 0.0;



	for(int i = 0; i != 2000; i ++){
		MPU_read_acc(data);

		acc_bias[0] += data[0]/2000.0;
		acc_bias[1] += data[1]/2000.0;
		acc_bias[2] += data[2]/2000.0;
		vTaskDelay(1);
	}

	acc_bias[2] -= 1.0;
}
void MPU_gyr_cal(void){
	float data[3];

	gyr_bias[0] = 0.0;
	gyr_bias[1] = 0.0;
	gyr_bias[2] = 0.0;



	for(int i = 0; i != 10000; i ++){
		MPU_read_gyr(data);

		gyr_bias[0] += data[0]/10000.0;
		gyr_bias[1] += data[1]/10000.0;
		gyr_bias[2] += data[2]/10000.0;
		vTaskDelay(1);
	}
}
