#include "stm32f030x6.h"
#include "timer.h"
#include "power_drive.h"

unsigned int powerGridPeriode = 0; //zero => no signal or overflow
unsigned int powerGridZeroDuration = 0; //duration of "power grid voltage so small it is aproximated to zero
unsigned int triacStartTime = 0xFFFF; //0XFFFF => never fire the triac

void timerInit()
{
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST; //reset TIM3
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST; //end reset TIM3
	TIM3->PSC = 0x0001; //timer prescaler set to 2 --> count up to ~ 40000 for 100Hz event with 8Mhz clk
	TIM3->DIER = TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE; //CC1, CC2, CC3, CC4 interrupt enabled
	TIM3->SR = 0x0000; // reset status register
	TIM3->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | //CC1 channel is configured as input, IC1 is mapped on TI1. input compare filter N=8
				  TIM_CCMR1_CC2S_1 | TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1; //CC2 channel is configured as input, IC2 is mapped on TI1. input compare filter N=8
	TIM3->CCMR2 = TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE; //CC3 & CC4 preload enabled
	TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC2P; //input captur enabled on cc1, sensitiv on rising edge, input captur enabled on cc2, sensitiv on rising edge

	TIM3->SMCR = TIM_SMCR_SMS_2 | TIM_SMCR_TS_0 | TIM_SMCR_TS_2; //slave mode set to reset on TI1 rising edge
	TIM3->ARR = 0xFFFF; // Autoreload value (overflox event) set to max

	NVIC_SetPriority(TIM3_IRQn, 0x01); //TIM3 interrupt at 2nd highest priority
	NVIC_EnableIRQ(TIM3_IRQn); //TIM3 interrupt enable	

	TIM3->CR1 = TIM_CR1_CEN | TIM_CR1_URS; //enable counter, overflow generate update event
	
}

//**************************************************************
//CC1 used to measure sin power grid frequency
//CC3 and CC4 are used to generate pulse command for the triac
//update event = overflow is treated as absence of power -> no pulse commane is gnerated
//*************************************************************

void tim3_INTERRUPT()
{
	if (TIM3->SR & TIM_SR_CC1IF) //measure power grid voltage crosses zero
	{
		powerGridPeriode = TIM3->CCR1;
		TIM3->SR &= ~TIM_SR_CC1IF;

		GPIOA->BSRR = GPIO_BSRR_BS_7; //set PA7 at high lever
		TIM3->CCR3 = triacStartTime;//update CCR3
		TIM3->CCR4 = triacStartTime + TRIAC_PULSE_DURATION;//update CCR4

		PIDStep(); //run PID regulator (at 100Hz assuming 50Hz power grid)
	}

	if (TIM3->SR & TIM_SR_CC2IF) //measure power grid voltage end of crosses zero
	{
		powerGridZeroDuration = TIM3->CCR2;
		TIM3->SR &= ~TIM_SR_CC2IF;
	}
	if (TIM3->SR & TIM_SR_CC3IF) //start of triac triger pulse
	{
		if(triacStartTime != 0xFFFF && triacStartTime+TRIAC_PULSE_DURATION < powerGridPeriode)
			GPIOA->BSRR = GPIO_BSRR_BR_7; //set PA7 at low lever

		TIM3->SR &= ~TIM_SR_CC3IF;
	}

	if (TIM3->SR & TIM_SR_CC4IF) //end of triac triger pulse
	{
		GPIOA->BSRR = GPIO_BSRR_BS_7; //set PA7 at high lever

		TIM3->SR &= ~TIM_SR_CC4IF;
	}

	if (TIM3->SR & TIM_SR_UIF) //timer overflow
	{
		powerGridPeriode = 0;
		TIM3->SR &= ~TIM_SR_UIF;

		triacStartTime = 0xFFFF; //no triac triger pulse 

		GPIOA->BSRR = GPIO_BSRR_BS_7; //set PA7 at high lever
		TIM3->CCR3 = triacStartTime;//update CCR3
		TIM3->CCR4 = triacStartTime + TRIAC_PULSE_DURATION;//update CCR4

	}
}
