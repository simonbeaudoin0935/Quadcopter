#include "Tasks/Task_PIDLoop.h"
#include "Motor/Motor.h"

void vTask_PIDLoop(void * pvParameters);

TaskHandle_t vCreateTask_PIDLoop(uint32_t stack_size, uint32_t priority)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	/* Create the task, storing the handle. */
    xReturned = xTaskCreate(vTask_PIDLoop,       /* Function that implements the task. */
	                        "PID Loop",          /* Text name for the task. */
							stack_size,      /* Stack size in words, not bytes. */
	                        NULL,    /* Parameter passed into the task. */
	                        priority,/* Priority at which the task is created. */
	                        &xHandle );      /* Used to pass out the created task's handle. */

	if( xReturned != pdPASS ) return NULL;

	return xHandle;
}

float pid_roll_i_int, pid_pitch_i_int, pid_yaw_i_int;
float pid_roll_last_d_error, pid_pitch_last_d_error, pid_yaw_last_d_error;_

void vTask_PIDLoop(void * pvParameters)
{
	extern float x;
	set_motor_speed_percent(1.0,1.0,1.0,1.0);
	vTaskDelay(4000);
	set_motor_speed_percent(0.0,0.0,0.0,0.0);
	vTaskDelay(4000);

	while(1){

		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		float output_1, output_2, output_3, output_4;
		set_motor_speed_percent(output_1,output_2,output_3,output_4);


	}
}



