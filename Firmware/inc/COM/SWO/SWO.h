/*
 * SWO.h
 *
 *  Created on: May 8, 2018
 *      Author: simon
 */

#ifndef COM_SWO_SWO_H_
#define COM_SWO_SWO_H_

#include "stm32f4xx.h"

void SWO_Init(uint32_t portBits, uint32_t cpuCoreFreqHz);

void SWO_write(char c, uint8_t portNo);

void SWO_print(char* s, uint8_t portNo);

void SWO_println(char*s, uint8_t portNo);

#endif /* COM_SWO_SWO_H_ */
