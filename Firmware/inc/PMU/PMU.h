/*
 * PMU.h
 *
 *  Created on: May 3, 2018
 *      Author: simon
 */

#ifndef PMU_PMU_H_
#define PMU_PMU_H_

#include "stm32f4xx.h"

void PMU_init(void);

void PMU_read(float* voltage, float* current);

#endif /* PMU_PMU_H_ */
