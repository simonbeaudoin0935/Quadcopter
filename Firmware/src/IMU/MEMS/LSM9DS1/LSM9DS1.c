#include "IMU/LSM9DS1/LSM9DS1.h"
#include "COM/SPI/SPI1.h"
#include "FreeRTOS.h"
#include "task.h"

#define LSM_AG_SELECT   GPIOC->BSRRH = GPIO_Pin_9;
#define LSM_AG_DESELECT GPIOC->BSRRL = GPIO_Pin_9;

#define LSM_M_SELECT    GPIOA->BSRRH = GPIO_Pin_8;
#define LSM_M_DESELECT  GPIOA->BSRRL = GPIO_Pin_8;

static uint8_t LSM_AG_WriteReg(uint8_t WriteAddr, uint8_t WriteData );
static uint8_t LSM_AG_ReadReg(uint8_t ReadAddr);
static void    LSM_AG_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );

static uint8_t LSM_M_WriteReg(uint8_t WriteAddr, uint8_t WriteData );
static uint8_t LSM_M_ReadReg(uint8_t ReadAddr);
static void    LSM_M_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );

static float gyro_multiplier = 0.00875;
static float acc_multiplier  = 0.000061;
static float mag_multiplier  = 0.00014;

static float acc_bias[3] = {0.0, -0.11, 0.2};
static float gyr_bias[3] = {-5.75, 5.0, -0.75};
static float mag_bias[3] = {0.0, 0.0, 0.0};

void LSM_init(void){

	//Chip select pin initialisation
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	LSM_AG_DESELECT;
	LSM_M_DESELECT;


	#define LSM_AG_InitRegNum 23
	#define CTL1 (GYRO_ODR_952HZ | GYRO_SCALE_245DPS | GYRO_BANDWIDTH_4)
	#define CTL6 (ACC_ODR_50HZ | ACC_SCALE_2G | 0x00)

	int8_t LSM_AG_Init_Data[LSM_AG_InitRegNum][2] = {

		{0x04, CTRL_REG9},                    // LControl Register 9. (i2c disabled)
		{0x00, ACT_THS},                      // (not used) Activity threshold register.
		{0x00, ACT_DUR},                      // (not used) Inactivity duration register.
		{0x00, INT_GEN_CFG_XL},               // (not used) Linear acceleration sensor interrupt generator configuration register.
		{0x00, INT_GEN_THS_X_XL},             // (not used) X-axis interrupt threshold.
		{0x00, INT_GEN_THS_Y_XL},             // (not used) Y-axis interrupt threshold.
		{0x00, INT_GEN_THS_Z_XL},             // (not used) Z-axis interrupt threshold.
		{0x00, INT_GEN_DUR_XL},               // (not used) Linear acceleration sensor interrupt duration register.
		{0x00, REFERENCE_G},                  // Angular rate sensor reference value register for digital high-pass filter (r/w).
		{0x02, INT1_CTRL},                    // INT1_A/G pin control register. (Gyroscope data ready)
		{0x01, INT2_CTRL},                    // INT2_A/G pin control register. (Accelerometer data ready)
		{CTL1, CTRL_REG1_G },               // Angular rate sensor Control Register 1.
		{0x00, CTRL_REG2_G},    //**to verify // Angular rate sensor Control Register 2.
		{0x00, CTRL_REG3_G},    //**to verify // Angular rate sensor Control Register 3.
		{0x00, ORIENT_CFG_G},                 // Angular rate sensor sign and orientation register.
		{0x38, CTRL_REG4 },                   // Control register 4. (gyro x,y,z active)
		{0x38, CTRL_REG5_XL},                 // Linear acceleration sensor Control Register 5. (x,y,z active, no decimation)
		{CTL6, CTRL_REG6_XL},                 // Linear acceleration sensor Control Register 6.
		{0x00, CTRL_REG7_XL},   //**to verify // Linear acceleration sensor Control Register 7.
		{0x04, CTRL_REG8},                    // Control Register 8. (adress increment activated)
		{0x00, CTRL_REG10},                   // Control Register 10.(self test disabled)
		{0x00, FIFO_CTRL},                    // FIFO control register. (disabled)
		{0x00, INT_GEN_CFG_G}                 // Angular rate sensor interrupt generator configuration register.
		//INT_GEN_THS_X_G   left t configure
	};

	for(int i = 0; i < LSM_AG_InitRegNum; i++) {
	    LSM_AG_WriteReg(LSM_AG_Init_Data[i][1], LSM_AG_Init_Data[i][0]);
	    vTaskDelay(1);  // I2C must slow down the write speed, otherwise it won't work
	}

	LSM_M_WriteReg(CTRL_REG2_M, 0x0C);
	vTaskDelay(1);
	LSM_M_WriteReg(CTRL_REG3_M, 0x84);
	LSM_M_WriteReg(CTRL_REG1_M, 0xFC);

	LSM_M_WriteReg(CTRL_REG4_M, 0x0C);

}

/////////////////////////////////////////
// LSM9DS1 Accel/Gyro access functions //
/////////////////////////////////////////

uint8_t LSM_AG_WriteReg(uint8_t WriteAddr, uint8_t WriteData ){

	LSM_AG_SELECT;

	SPI1_send(WriteAddr);
	uint8_t data = SPI1_send(WriteData);

	LSM_AG_DESELECT;

	return data;
}

uint8_t LSM_AG_ReadReg(uint8_t ReadAddr){

	return LSM_AG_WriteReg(ReadAddr | READ_FLAG, 0x00);

}

void LSM_AG_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes ){

	LSM_AG_SELECT;

		SPI1_send(ReadAddr | READ_FLAG);

		for(int i = 0; i < Bytes; i++)
		{
			ReadBuf[i] = SPI1_send(0x00);
		}

	LSM_AG_DESELECT;
}


/////////////////////////////////////////
// LSM9DS1 Mag access functions        //
/////////////////////////////////////////

uint8_t LSM_M_WriteReg(uint8_t WriteAddr, uint8_t WriteData ){

	LSM_M_SELECT;

		SPI1_send(WriteAddr);
		uint8_t data = SPI1_send(WriteData);

	LSM_M_DESELECT;

	return data;
}

uint8_t LSM_M_ReadReg(uint8_t ReadAddr){

	return LSM_M_WriteReg(ReadAddr | READ_FLAG, 0x00);

}

void LSM_M_ReadRegs(uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes ){

	LSM_M_SELECT;

		SPI1_send(ReadAddr | READ_FLAG);

		for(int i = 0; i < Bytes; i++)
		{
			ReadBuf[i] = SPI1_send(0x00);
		}

	LSM_M_DESELECT;
}



uint8_t LSM_AG_whoami(void){
	return LSM_AG_ReadReg(WHO_AM_I_XG);
}

uint8_t LSM_AG_read_interrupt(void){
	return LSM_AG_ReadReg(INT_GEN_SRC_G);
}

int16_t LSM_AG_temp_read(void){
	uint8_t temp_l = LSM_AG_ReadReg(OUT_TEMP_L);
	uint8_t temp_h = LSM_AG_ReadReg(OUT_TEMP_H);

	return (int16_t)((int16_t)(temp_h << 8)) & ((int16_t) temp_l);
}

uint8_t LSM_AG_read_status_1(void){
	return LSM_AG_ReadReg(STATUS_REG_0);
}
uint8_t LSM_AG_read_status_2(void){
	return LSM_AG_ReadReg(STATUS_REG_1);
}
uint8_t LSM_AG_read_fifo_status(void){
	return LSM_AG_ReadReg(FIFO_SRC);
}



void LSM_read_gyr(float gyro_data[3]){
    uint8_t response[6];
    int16_t bit_data;
    float data;
    int i;

    LSM_AG_ReadRegs(OUT_X_L_G,response,6);

    for(i = 0; i < 3; i++) {
        bit_data = ((int16_t)response[i*2+1]<<8) | response[i*2];
        data = (float)bit_data;
        gyro_data[i] = data * gyro_multiplier - gyr_bias[i];
    }
}

void LSM_read_acc(float acc_data[3]){
    uint8_t response[6];

    LSM_AG_ReadRegs(OUT_X_L_XL,response,6);


    acc_data[0] = (float)((int16_t)((response[1]<<8) + response[0])) * acc_multiplier - acc_bias[0];
    acc_data[1] = (float)((int16_t)((response[3]<<8) + response[2])) * acc_multiplier - acc_bias[1];
    acc_data[2] = -((float)((int16_t)((response[5]<<8) + response[4])) * acc_multiplier - acc_bias[2]);
}

void LSM_read_mag(float mag_data[3]){
    uint8_t response[6];

    LSM_M_ReadRegs(OUT_X_L_M,response,6);

    mag_data[0] = (float)((int16_t)((response[1]<<8) + response[0])) * mag_multiplier - mag_bias[0];
    mag_data[1] = (float)((int16_t)((response[3]<<8) + response[2])) * mag_multiplier - mag_bias[1];
    mag_data[2] = (float)((int16_t)((response[5]<<8) + response[4])) * mag_multiplier - mag_bias[2];
}

uint8_t LSM_M_whoami(void){
	return LSM_M_ReadReg(WHO_AM_I_M);
}


void LSM_acc_cal(void){
	float data[3];

	acc_bias[0] = 0.0;
	acc_bias[1] = 0.0;
	acc_bias[2] = 0.0;



	for(int i = 0; i != 2000; i ++){
		LSM_read_acc(data);

		acc_bias[0] += data[0]/2000.0;
		acc_bias[1] += data[1]/2000.0;
		acc_bias[2] += data[2]/2000.0;
		vTaskDelay(1);
	}

	acc_bias[2] += 1.0;
}
void LSM_gyr_cal(void){
	float data[3];

	gyr_bias[0] = 0.0;
	gyr_bias[1] = 0.0;
	gyr_bias[2] = 0.0;



	for(int i = 0; i != 2000; i ++){
		LSM_read_gyr(data);

		gyr_bias[0] += data[0]/2000.0;
		gyr_bias[1] += data[1]/2000.0;
		gyr_bias[2] += data[2]/2000.0;
		vTaskDelay(1);
	}
}
