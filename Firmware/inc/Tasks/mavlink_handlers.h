/*
 * mavlink_handlers.h
 *
 *  Created on: May 7, 2018
 *      Author: simon
 */

#ifndef TASKS_MAVLINK_HANDLERS_H_
#define TASKS_MAVLINK_HANDLERS_H_

#include "Mavlink/common/mavlink.h"

void handle_heartbeat(mavlink_message_t* msg, mavlink_status_t* status);
void handle_param_request_read(mavlink_message_t* msg, mavlink_status_t* status);
void handle_param_request_list(mavlink_message_t* msg, mavlink_status_t* status);
void handle_param_set(mavlink_message_t* msg, mavlink_status_t* status);
void handle_mission_request_list(mavlink_message_t* msg, mavlink_status_t* status);
void handle_mission_ack(mavlink_message_t* msg, mavlink_status_t* status);
void handle_manual_control(mavlink_message_t* msg, mavlink_status_t* status);
void handle_command_long(mavlink_message_t* msg, mavlink_status_t* status);


#endif /* TASKS_MAVLINK_HANDLERS_H_ */
