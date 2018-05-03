#include "Motor/Motor.h"


void Motors_init(uint32_t hertz){

	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	TIM_OCInitTypeDef TIM_OCStruct;



	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
	RCC->APB1ENR |= RCC_APB1Periph_TIM2;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

	/*
		TIM2 is connected to APB1 bus, which is 84 MHz / 2 = 42 MHz

		timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)
		timer_tick_frequency = 42 000 000 / (20 + 1) = 2 MHZ

		We want a PWM frequency of 'hertz' Hz
		TIM_Period = (timer_tick_frequency / PWM_frequency) - 1
		TIM_Period = (2000000 / 200) - 1 = 19,999
	*/

	TIM_BaseStruct.TIM_Prescaler = 20;						//Gives a tick frequency of 1.68 MHz
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//Counts up
	TIM_BaseStruct.TIM_Period = 19999;						//Gives PWM frequency of 200 Hz
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);				//Setup TIM10

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;				//PWM mode 2 = Clear on compare match */
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCStruct.TIM_Pulse = 4000; 							// 1.0ms

	TIM_OC1Init(TIM2, &TIM_OCStruct);
	TIM_OC2Init(TIM2, &TIM_OCStruct);
	TIM_OC3Init(TIM2, &TIM_OCStruct);
	TIM_OC4Init(TIM2, &TIM_OCStruct);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_Cmd(TIM2, ENABLE);

}

void set_motor_speed_percent(float speed_1, float speed_2, float speed_3, float speed_4){

	TIM2->CCR1 = (uint32_t)(speed_1 * 4000.0) + 4000;
	TIM2->CCR2 = (uint32_t)(speed_2 * 4000.0) + 4000;
	TIM2->CCR3 = (uint32_t)(speed_3 * 4000.0) + 4000;
	TIM2->CCR4 = (uint32_t)(speed_4 * 4000.0) + 4000;
}
