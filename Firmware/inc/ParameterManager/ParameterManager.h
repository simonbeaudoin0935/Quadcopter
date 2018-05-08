/*
 * ParameterManager.h
 *
 *  Created on: May 7, 2018
 *      Author: simon
 */

#ifndef PARAMETERMANAGER_PARAMETERMANAGER_H_
#define PARAMETERMANAGER_PARAMETERMANAGER_H_
#include "stm32f4xx.h"
#include "Mavlink/common/mavlink.h"

typedef struct{
	char id[17];
	union{
		float floating;
		int32_t integer;
		uint32_t unsigned_integer;
		uint8_t bytes[4];
	}value;
	MAV_PARAM_TYPE type;
}Parameter_t;

extern Parameter_t param_list[];

int numberOfParameters(void);

#endif /* PARAMETERMANAGER_PARAMETERMANAGER_H_ */
