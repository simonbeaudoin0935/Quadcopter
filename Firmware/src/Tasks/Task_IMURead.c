#include "Tasks/Task_IMURead.h"


#include "IMU/MEMS/MPU9250/MPU9250.h"
#include "IMU/MEMS/LSM9DS1/LSM9DS1.h"
#include "IMU/MEMS/ADA9DOF/ADA9DOF.h"

#include "COM/UART/UART6.h"
#include "stdio.h"

#include "IMU/Madgwick/madgwick.h"

float roll,pitch,yaw;
float x_rate, y_rate, z_rate;

static float mpu_gyr_data[3];
static float mpu_acc_data[3];
static float mpu_mag_data[3];



static float lsm_gyr_data[3];
static float lsm_acc_data[3];
static float lsm_mag_data[3];

static float ada_gyr_data[3];
static float ada_acc_data[3];
static float ada_mag_data[3];

static void vTask_IMURead( void * pvParameters )
{



	MPU_init();
	LSM_init();
	ADA_init();

	Madgwick_init(50,1);


	//MPU_acc_cal();
	//MPU_gyr_cal();

	//LSM_acc_cal();
	//LSM_gyr_cal();


    while(1)
    {



    	MPU_read_gyr(mpu_gyr_data);
    	MPU_read_acc(mpu_acc_data);
    	MPU_read_mag(mpu_mag_data);

    	x_rate = mpu_gyr_data[0];
    	y_rate = mpu_gyr_data[1];
    	z_rate = mpu_gyr_data[2];
//
//    	LSM_read_gyr(lsm_gyr_data);
//    	LSM_read_acc(lsm_acc_data);
//    	LSM_read_mag(lsm_mag_data);

//    	ADA_gyro_read(ada_gyr_data);
//    	ADA_acc_read(ada_acc_data);

    	Madgwick_compute(mpu_gyr_data[0],
    					 mpu_gyr_data[1],
						 mpu_gyr_data[2],
    				     mpu_acc_data[0],
    			         mpu_acc_data[1],
						 mpu_acc_data[2],
						 mpu_mag_data[0],
						 mpu_mag_data[1],
						 mpu_mag_data[2]);


    	computeAngles(&roll,&pitch,&yaw);


//    	UART6_write('#');
//    	UART6_write(0x01);
//    	UART6_write(24);
//
//    	UART6_writeFloatUnion(roll);
//    	UART6_writeFloatUnion(pitch);
//    	UART6_writeFloatUnion(yaw);
//
//     	UART6_writeFloatUnion(0);
//       	UART6_writeFloatUnion(0);
//        UART6_writeFloatUnion(0);
//
//
//    	UART6_write('.');


  	   	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);



        vTaskDelay(20);
    }
}

TaskHandle_t vCreateTask_IMURead(uint32_t stack_size)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_IMURead,       /* Function that implements the task. */
                    "IMU Read",          /* Text name for the task. */
					stack_size,      /* Stack size in words, not bytes. */
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




