#include "Tasks/mavlink_handlers.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32f4xx.h"
#include "ParameterManager/ParameterManager.h"
#include "string.h"
#include "COM/UART/UART1.h"
#include "COM/UART/UART6.h"

extern SemaphoreHandle_t xUART1Mutex;



void handle_heartbeat(mavlink_message_t* msg, mavlink_status_t* status){
	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
}

void handle_param_request_read(mavlink_message_t* msg, mavlink_status_t* status){
	mavlink_param_request_read_t   packet;
	mavlink_msg_param_request_read_decode(msg,&packet);

	uint8_t buf[50];
	mavlink_msg_param_value_pack(1,
								 200,
							     msg,
								 param_list[packet.param_index].id,
								 param_list[packet.param_index].value.floating,
								 param_list[packet.param_index].type,
								 numberOfParameters(),
								 packet.param_index);

	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

	   for(int i = 0; i != len; i++) UART1_write(buf[i]);

	xSemaphoreGive(xUART1Mutex);
}

void handle_param_request_list(mavlink_message_t* msg, mavlink_status_t* status){

	//mavlink_param_request_list_t   packet;
	//mavlink_msg_param_request_list_decode(msg,&packet);
	uint8_t buf[50];
	unsigned int num_param = numberOfParameters();

	for(int i = 0; i != num_param; i++){
		mavlink_msg_param_value_pack(1,
									 200,
									 msg,
									 param_list[i].id,
									 param_list[i].value.floating,
									 param_list[i].type,
									 num_param,
									 i);
		uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	    xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

	    	for(int j = 0; j != len; j++) UART1_write(buf[j]);

	    xSemaphoreGive(xUART1Mutex);
	}
}

void handle_param_set(mavlink_message_t* msg, mavlink_status_t* status){
	mavlink_param_set_t   packet;
	mavlink_msg_param_set_decode(msg,&packet);

	int param_index = 0;

	for(int i = 0; i != numberOfParameters(); i++){
		if(strncmp(packet.param_id, param_list[0].id, 16)){
			param_index = i;
		}
	}
	param_list[param_index].value.floating = packet.param_value;
	//TODO verification

	uint8_t buf[50];
	mavlink_msg_param_value_pack(1,
								 200,
							     msg,
								 param_list[param_index].id,
								 param_list[param_index].value.floating,
								 param_list[param_index].type,
								 numberOfParameters(),
								 param_index);

	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

	   for(int i = 0; i != len; i++) UART1_write(buf[i]);

	xSemaphoreGive(xUART1Mutex);
}

void handle_mission_request_list(mavlink_message_t* msg, mavlink_status_t* status){
	mavlink_mission_request_list_t   packet;
	mavlink_msg_mission_request_list_decode(msg,&packet);
	uint8_t buf[50];

	mavlink_msg_mission_count_pack(1,200,msg,msg->sysid,msg->compid,0,packet.mission_type);
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);
	xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

	   for(int i = 0; i != len; i++) UART1_write(buf[i]);

	xSemaphoreGive(xUART1Mutex);

}

void handle_mission_ack(mavlink_message_t* msg, mavlink_status_t* status){

}

struct{
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t r;
	uint16_t buttons;
}Manual_Setpoint;

void handle_manual_control(mavlink_message_t* msg, mavlink_status_t* status){
	mavlink_manual_control_t packet;
	mavlink_msg_manual_control_decode(msg, &packet);

	Manual_Setpoint.x = packet.x;
	Manual_Setpoint.y = packet.y;
	Manual_Setpoint.z = packet.z;
	Manual_Setpoint.r = packet.r;
	Manual_Setpoint.buttons = packet.buttons;
}

void handle_command_long(mavlink_message_t* msg, mavlink_status_t* status){

	mavlink_command_long_t packet;
	mavlink_msg_command_long_decode(msg,&packet);



	if(packet.command == 520 && (int)packet.param1 == 1){
		uint8_t a[8] = {1,0,0,0,0,0,0,0};
		uint8_t buf[200];
        #define CAPABILITIES MAV_PROTOCOL_CAPABILITY_MAVLINK2 | MAV_PROTOCOL_CAPABILITY_PARAM_FLOAT
		mavlink_msg_autopilot_version_pack(1,200,msg,
				                           CAPABILITIES, //bitmask of capabilities (see MAV_PROTOCOL_CAPABILITY enum)
										   1, //Firmware version number
										   0, //Middleware version number
										   1, //Operating system version number
										   0, //HW / board version (last 8 bytes should be silicon ID, if any)
										   a, //flight_custom_version
										   a, //middleware_custom_version
										   a, //os_custom_version
										   1,
										   1,
										   1);

		uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

		xSemaphoreTake(xUART1Mutex, portMAX_DELAY);

		   for(int i = 0; i != len; i++) UART1_write(buf[i]);

		xSemaphoreGive(xUART1Mutex);


	}
}




