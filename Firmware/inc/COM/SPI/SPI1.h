/*
 * SPI1.h
 *
 *  Created on: Apr 27, 2018
 *      Author: simon
 */

#ifndef SPI_SPI1_H_
#define SPI_SPI1_H_

#include "stm32f4xx.h"

void SPI1_init(void);

uint8_t SPI1_send(uint8_t data);

#endif /* SPI_SPI1_H_ */
