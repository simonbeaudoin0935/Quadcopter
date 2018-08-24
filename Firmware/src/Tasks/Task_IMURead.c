#include "Tasks/Task_IMURead.h"


#include "IMU/MEMS/MPU9250/MPU9250.h"
#include "IMU/MEMS/LSM9DS1/LSM9DS1.h"
#include "IMU/MEMS/ADA9DOF/ADA9DOF.h"

#include "COM/UART/UART6.h"
#include "stdio.h"

#include "IMU/Madgwick/madgwick.h"



struct{
	float roll;
	float pitch;
	float yaw;
	float x_rate;
	float y_rate;
	float z_rate;
	float x_acc;
	float y_acc;
	float z_acc;

}IMU;

static float mpu_gyr_data[3];
static float mpu_acc_data[3];
static float mpu_mag_data[3];

int16_t mag[3];



extern struct{
	TaskHandle_t TaskHandle_FlashHeartbeatLED;
	TaskHandle_t TaskHandle_RPIReception;
	TaskHandle_t TaskHandle_PIDLoop;
	TaskHandle_t TaskHandle_IMURead;
	TaskHandle_t TaskHandle_SendAttitudeMessage;
	TaskHandle_t TaskHandle_PMUReader;
}TaskHandles;


static void vTask_IMURead( void * pvParameters )
{
	MPU_init();
	LSM_init();


	Madgwick_init(200,1);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 41; //tick frequency 1mhz
	TIM3->ARR = 4999;	//interrupt freq 200 hz
	TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
	TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
	NVIC_SetPriority(TIM3_IRQn, 5);
	NVIC_EnableIRQ(TIM3_IRQn);

    while(1)
    {
    	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    	MPU_read_gyr(mpu_gyr_data);
    	MPU_read_acc(mpu_acc_data);

    	float mag[3];
    	MPU_read_mag(mag);

    	mpu_mag_data[0] = 0.9 * mpu_mag_data[0] + 0.1 * mag[0];
    	mpu_mag_data[1] = 0.9 * mpu_mag_data[1] + 0.1 * mag[1];
    	mpu_mag_data[2] = 0.9 * mpu_mag_data[2] + 0.1 * mag[2];

    	IMU.x_rate = mpu_gyr_data[0];
    	IMU.y_rate = mpu_gyr_data[1];
    	IMU.z_rate = mpu_gyr_data[2];

    	IMU.x_acc = mpu_mag_data[0];
    	IMU.y_acc = mpu_mag_data[1];
    	IMU.z_acc = mpu_mag_data[2];

    	Madgwick_compute(mpu_gyr_data[0],
    					 mpu_gyr_data[1],
						 mpu_gyr_data[2],
    				     mpu_acc_data[0],
    			         mpu_acc_data[1],
						 mpu_acc_data[2],
						 mpu_mag_data[0],
						 mpu_mag_data[1],
						 mpu_mag_data[2]);


    	//pitch and roll inverted
    	float roll, pitch, yaw;
    	computeAngles(&pitch,&roll,&yaw);

    	roll *= -1.0;
    	pitch *= -1.0;
    	yaw *= -1.0;

    	IMU.pitch = 0.9 * IMU.pitch + 0.1 * pitch;
    	IMU.roll  = 0.9 * IMU.roll + 0.1 * roll;
    	IMU.yaw = 0.9 * IMU.yaw + 0.1 * yaw;

    	xTaskNotifyGive(TaskHandles.TaskHandle_PIDLoop);



//    	UART6_write('#');
//    	UART6_write(0x01);
//    	UART6_write(24);
//    	UART6_writeFloatUnion(roll);
//    	UART6_writeFloatUnion(pitch);
//    	UART6_writeFloatUnion(yaw);
//     	UART6_writeFloatUnion(0);
//     	UART6_writeFloatUnion(0);
//      UART6_writeFloatUnion(0);
//    	UART6_write('.');

    }
}

TaskHandle_t vCreateTask_IMURead(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
    		        vTask_IMURead,       /* Function that implements the task. */
                    "IMU Read",          /* Text name for the task. */
					stack_size,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
					priority,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned != pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        return NULL;
    }

    return xHandle;
}




void TIM3_IRQHandler(void){


	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
	{
	  TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag

	  vTaskNotifyGiveFromISR(TaskHandles.TaskHandle_IMURead, &xHigherPriorityTaskWoken);

	  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}


    //GPIO_ToggleBits(GPIOC, GPIO_Pin_14);


}
