#include "Tasks/Task_IMURead.h"
#include "SPI/SPI1.h"
#include "I2C/I2C1.h"
#include "I2C/tm_stm32f4_i2c.h"
#include "IMU/MPU9250/MPU9250.h"
#include "IMU/LSM9DS1/LSM9DS1.h"
#include "IMU/ADA9DOF/ADA9DOF.h"
#include "UART/UART6.h"
#include "stdio.h"




static void vTask_IMURead( void * pvParameters )
{
	SPI1_init();
	//I2C1_init();
	///TM_I2C_Init(I2C1, 400000);
	//ADA_init();
	MPU_init();
	LSM_init();

	UART6_init(115200);

    while(1)
    {
    	float data[3];
//    	uint8_t data = TM_I2C_Read(I2C1, I2C_ADDR_GYROSCOPE, GYRO_REGISTER_WHO_AM_I);
//    	UART6_write(data);
//

    	//ADA_gyro_read(gyro_data);
    	//MPU_read_acc(mpu_gyro_data);

    	LSM_read_gyro(data);

    	UART6_write('#');
    	UART6_write(0x01);
    	UART6_write(24);
    	UART6_writeFloatUnion(0);
    	UART6_writeFloatUnion(0);
    	UART6_writeFloatUnion(0);
    	UART6_writeFloatUnion(data[0]);
    	UART6_writeFloatUnion(data[1]);
    	UART6_writeFloatUnion(data[2]);
    	UART6_write('.');


  	   	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);



        vTaskDelay(50);
    }
}

TaskHandle_t vCreateTask_IMURead(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_IMURead,       /* Function that implements the task. */
                    "IMU Read",          /* Text name for the task. */
					1024,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					tskIDLE_PRIORITY+4,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}




