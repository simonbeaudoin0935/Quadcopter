/*
 * Motor.h
 *
 *  Created on: May 3, 2018
 *      Author: simon
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "stm32f4xx.h"

void Motors_init(uint32_t hertz);

void set_motor_speed_percent(float speed_1, float speed_2, float speed_3, float speed_4);

#endif /* MOTOR_MOTOR_H_ */
