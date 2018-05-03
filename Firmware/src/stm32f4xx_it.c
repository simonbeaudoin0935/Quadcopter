/*
 * stm32f4xx_it.c
 *
 *  Created on: Apr 27, 2018
 *      Author: simon
 */
#include "stm32f4xx_it.h"

void NMI_Handler(void){
	while(1);
}

void HardFault_Handler(void){
	while(1){
		GPIOC->BSRRL = GPIO_Pin_13 | GPIO_Pin_14;
		for(int i = 0; i != 200000; i++);
		GPIOC->BSRRH = GPIO_Pin_13 | GPIO_Pin_14;
		for(int i = 0; i != 2000000; i++);
	}
}

void MemManage_Handler(void){
	while(1);
}
void BusFault_Handler(void){
	while(1);
}
void UsageFault_Handler(void){
	while(1);
}

