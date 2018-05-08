#include "PMU/PMU.h"

void PMU_init(void){
	//ch 8 : current
	//ch 9 : volt

	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	/* Init ADC settings */
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/* Set common ADC settings */
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);


	ADC_Init(ADC1, &ADC_InitStruct);
	ADC1->CR2 |= ADC_CR2_ADON;
}

void PMU_read(float* voltage, float* current){

	ADC_RegularChannelConfig(ADC1, 8, 1, ADC_SampleTime_15Cycles);

	/* Start software conversion */
	ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;

	/* Wait till done */
	while (!(ADC1->SR & ADC_SR_EOC));

	/* Return result */
	uint32_t current_raw =  ADC1->DR;

	ADC_RegularChannelConfig(ADC1, 9, 1, ADC_SampleTime_15Cycles);

	/* Start software conversion */
	ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;

	/* Wait till done */
	while (!(ADC1->SR & ADC_SR_EOC));

	/* Return result */
	uint32_t voltage_raw =  ADC1->DR;

	*current = ((float)current_raw) * 0.001611328;
	*voltage = ((float)voltage_raw) * 0.008217773;
}
