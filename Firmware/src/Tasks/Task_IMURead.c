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

int16_t mag[3];

static float lsm_gyr_data[3];
static float lsm_acc_data[3];
static float lsm_mag_data[3];

static float ada_gyr_data[3];
static float ada_acc_data[3];
static float ada_mag_data[3];

static void init_tim1_interrupt(void);

static void vTask_IMURead( void * pvParameters )
{
	MPU_init();
	LSM_init();
	ADA_init();

	Madgwick_init(200,1);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 41; //tick frequency 1mhz
	TIM3->ARR = 4999;	//interrupt freq 200 hz
	TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
	TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
	NVIC_SetPriority(TIM3_IRQn, 7);
	NVIC_EnableIRQ(TIM3_IRQn);

    while(1)
    {
    	uint32_t ulNotifiedValue = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    	MPU_read_gyr(mpu_gyr_data);
    	MPU_read_acc(mpu_acc_data);
    	MPU_read_mag(mpu_mag_data);

    	x_rate = mpu_gyr_data[0];
    	y_rate = mpu_gyr_data[1];
    	z_rate = mpu_gyr_data[2];

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
//    	UART6_writeFloatUnion(roll);
//    	UART6_writeFloatUnion(pitch);
//    	UART6_writeFloatUnion(yaw);
//     	UART6_writeFloatUnion(0);
//     	UART6_writeFloatUnion(0);
//      UART6_writeFloatUnion(0);
//    	UART6_write('.');

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


void init_tim1_interrupt(void){

	  // Clock enable

}

void TIM3_IRQHandler(void){
	extern struct{
		TaskHandle_t TaskHandle_FlashHeartbeatLED;
		TaskHandle_t TaskHandle_RPIReception;
		TaskHandle_t TaskHandle_PIDLoop;
		TaskHandle_t TaskHandle_IMURead;
		TaskHandle_t TaskHandle_SendAttitudeMessage;
		TaskHandle_t TaskHandle_PMUReader;
	}TaskHandles;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
	{
	  TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag

	  vTaskNotifyGiveFromISR(TaskHandles.TaskHandle_IMURead, &xHigherPriorityTaskWoken);

	  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}


    //GPIO_ToggleBits(GPIOC, GPIO_Pin_14);


}
