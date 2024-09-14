#include "stm32f030x6.h"
#include "adc.h"

void adc_init()
{
	ADC1->CR |= ADC_CR_ADDIS; //Disable ADC
	ADC1->CFGR1 &= ~(ADC_CFGR1_DMAEN); //Disable DMA
	while ((ADC1->CR & ADC_CR_ADEN) != 0);

	ADC1->CHSELR = ADC_CHSELR_CHSEL0; //select ADC_IN0
	ADC1->CR |= ADC_CR_ADCAL; //Start ADC calibration
	while ((ADC1->CR & ADC_CR_ADCAL) != 0); //Wait for calibration to be done

	ADC1->CFGR1 |= ADC_CFGR1_CONT; //Set continious conversion mode

	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) //Clear ADRDY
	{
		ADC1->ISR |= ADC_ISR_ADRDY; 
	}
	ADC1->SMPR = ADC_SMPR_SMP_0 | ADC_SMPR_SMP_2; // samplint time set to 55.5 ADC clock cycles
	ADC1->CR |= ADC_CR_ADEN; //Enable ADC
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); 
	ADC1->CR |= ADC_CR_ADSTART; //Start ADC
}

int adc_read_temp()
{
	double mesure = (double)((int)((unsigned int)ADC1->DR) - code_0_deg);
	mesure *=100;
	mesure /= (code_100_deg - code_0_deg);
	return (int)mesure;
}
