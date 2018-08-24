#include "Tasks/Task_PIDLoop.h"
#include "Motor/Motor.h"
#include "COM/UART/UART6.h"

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



void vTask_PIDLoop(void * pvParameters)
{
	//Declares in mavlink_handlers.c at manual command function
	extern struct{
		int16_t x;
		int16_t y;
		int16_t z;
		int16_t r;
		uint16_t buttons;
	}Manual_Setpoint;

	extern struct{
		float roll; //rad
		float pitch; //rad
		float yaw;   //rad
		float x_rate; //deg
		float y_rate; //deg
		float z_rate; //deg
		float x_acc;
		float y_acc;
		float z_acc;
	}IMU;

	struct{
		struct{
			float setpoint;
			float integral;
			float last_error;
		}roll;

		struct{
			float setpoint;
			float integral;
			float last_error;
		}pitch;

		struct{
			float setpoint;
			float integral;
			float last_error;
		}yaw;
	}PID;

	const float dt = 0.005;
	const float integral_overrun = 100.0;

	float roll_kp = 1.0, roll_ki = 0.01, roll_kd = 0.01;
	float pitch_kp = 1.0, pitch_ki = 0.01, pitch_kd = 0.01;
	float yaw_kp = 1.0, yaw_ki = 1.0, yaw_kd = 1.0;

	//set_motor_speed_percent(1.0,1.0,1.0,1.0);
	//vTaskDelay(4000);
	//set_motor_speed_percent(0.0,0.0,0.0,0.0);
	//vTaskDelay(4000);

	int i = 0;
	while(1){

		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

//roll pid
		//roll deadband
		int16_t sp = Manual_Setpoint.x;

		if(sp > 200)sp -= 200;
		else if(sp < -200) sp += 200;
		else sp = 0;

		PID.roll.setpoint = ((float)sp / 800.0);

		float roll_error = PID.roll.setpoint - IMU.roll;

		PID.roll.integral += roll_error * dt;
		if(PID.roll.integral > integral_overrun) PID.roll.integral = integral_overrun;
		else if(PID.roll.integral < -integral_overrun) PID.roll.integral = -integral_overrun;

		float roll_derivative = (roll_error - PID.roll.last_error) * dt;
		PID.roll.last_error = roll_error;

		float roll_output = roll_kp * roll_error + roll_ki * PID.roll.integral + roll_kd * roll_derivative;

//pitch pid

		//pitch deadband
		sp = Manual_Setpoint.y;
		if(sp > 200)sp -= 200;
		else if(sp < -200) sp += 200;
		else sp = 0;

		PID.pitch.setpoint = ((float)sp / 800.0);

		float pitch_error = PID.pitch.setpoint - IMU.pitch;

		PID.pitch.integral += pitch_error * dt;
		if(PID.pitch.integral > integral_overrun) PID.pitch.integral = integral_overrun;
		else if(PID.pitch.integral < -integral_overrun) PID.pitch.integral = -integral_overrun;

		float pitch_derivative = (pitch_error - PID.pitch.last_error) * dt;
		PID.pitch.last_error = pitch_error;

		float pitch_output = pitch_kp * pitch_error + pitch_ki * PID.pitch.integral + pitch_kd * pitch_derivative;

		float output_1, output_2, output_3, output_4;

		output_1 = 0.2 - pitch_output + roll_output;
		output_2 = 0.2 - pitch_output - roll_output;
		output_3 = 0.2 + pitch_output - roll_output;
		output_4 = 0.2 + pitch_output + roll_output;

		if(output_1 < 0.2) output_1 = 0.2;
		if(output_1 > 1.0) output_1 = 1.0;
		if(output_2 < 0.2) output_2 = 0.2;
		if(output_2 > 1.0) output_2 = 1.0;
		if(output_3 < 0.2) output_3 = 0.2;
		if(output_3 > 1.0) output_3 = 1.0;
		if(output_4 < 0.2) output_4 = 0.2;
		if(output_4 > 1.0) output_4 = 1.0;
		/*
		 * 2     1
		 *  O   O
		 *   \ /
		 *    X
		 *   / \
		 *  O   O
		 * 3      4
		 */

#define DEG_2_RAD (3.14159265359 / 180.0)
if(i == 5){
		//set_motor_speed_percent(output_1,output_2,output_3,output_4);
		    	UART6_write('#');
		    	UART6_write(0x01);
		    	UART6_write(24);
		    	UART6_writeFloatUnion(0.001 * IMU.x_acc + 0.1);
		    	UART6_writeFloatUnion(0.001 * IMU.y_acc + 0.1);
		    	UART6_writeFloatUnion(0.001 * IMU.z_acc );
		     	UART6_writeFloatUnion(IMU.yaw);
		     	UART6_writeFloatUnion(IMU.roll);
		        UART6_writeFloatUnion(IMU.pitch);
		    	UART6_write('.');
		i = 0;
}
else{
	i++;
}

	}
}



